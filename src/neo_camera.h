#ifndef MSX_NEO_CAMERA_H
#define MSX_NEO_CAMERA_H
#include <stdbool.h>
#include <stdint.h>
/* Native translation of the original mission camera (RAM 10E94E..10E9D9,
 * ROM labels kept as provenance):
 *  - 018D96..018F0A  desired movement from the player point (01903A), dead
 *                    zones around 69B8/69BA, *3/16, clamp to the mission
 *                    actor's +72/+76 (10.0 px), smoothing with the previous
 *                    root velocity;
 *  - 0C81C0          autoscroll override (695A/695C/6960), 0C7CA6 camera
 *                    (path progress 0C7ECE/0C7F3A, bounds 69AE..69B4 and the
 *                    script stop 69BC/69C0..69C6, monotonic progress 699E,
 *                    view rectangle 69D2..69D8), shake decay 0C804A and the
 *                    map/camera command list 0C83E6 (pointer 6964);
 *  - 0C7B0C/0C7C12   script stop and release, 0C7B4E/0C7BBA autoscroll
 *                    (script ops D/E), 0C801A/0C8022 shake requests.
 * The background layer objects (10C72E/10D7B6/10E83E/10E8C6) are not moved
 * here: the renderer derives them from the camera with the measured rules of
 * mission_camera.h. Map commands that touch layers, palettes or sound are
 * reported as events (the extracted map already contains what they load).
 * Positions are Q16.16; y grows upwards (camera height). */
enum { NEO_CAMERA_EVENTS=8 };
typedef enum {
    NEO_CAMERA_EV_PALETTE=0x0c,     /* 009D8C with the ROM block `address` */
    NEO_CAMERA_EV_LOAD_LAYER=0x18, NEO_CAMERA_EV_LAYER_ATTR=0x1c, NEO_CAMERA_EV_LAYER_RELOAD=0x20,
    NEO_CAMERA_EV_LAYER_24=0x24, NEO_CAMERA_EV_LAYER_28=0x28,
    NEO_CAMERA_EV_CALL=0x2c,        /* routine `address` (e.g. 0B58AE layer copy) */
    NEO_CAMERA_EV_SOUND=0x38,       /* 002802 with `value` */
    NEO_CAMERA_EV_LAYER_FLAGS=0x3c, NEO_CAMERA_EV_REBASE_LAYER=0x44, NEO_CAMERA_EV_PATCH_LAYER=0x48,
    NEO_CAMERA_EV_GRAPHICS=0x4c,    /* 01AA28 with `address` */
    NEO_CAMERA_EV_COND_CALL=0x54,   /* 0x54/0x58 condition true: routine `address` */
    NEO_CAMERA_EV_UNSUPPORTED=0xff
} neo_camera_event_kind_t;
typedef struct { uint8_t kind; uint32_t address; uint16_t value; uint16_t progress; } neo_camera_event_t;
typedef struct {
    uint32_t vx694e,vy6952;          /* custom layer velocities (unused in Mission 1) */
    uint8_t layer_flags[4];          /* 6956..6959 */
    uint32_t l400e;                  /* RAM 10C00E (op 0x50): cell group map table */
    uint8_t auto_mode,auto_owner;    /* 695A, 695B */
    int32_t auto_vx,auto_vy;         /* 695C, 6960 */
    uint16_t command;                /* 6964: offset into the command list */
    int32_t root_vx,root_vy;         /* 6970/6974: velocity of the root 10E968 (-delta) */
    int32_t x,y;                     /* 698E, 6992 */
    int32_t dx,dy;                   /* 6996, 699A */
    uint32_t progress;               /* 699E (high word = RAM 10E99E) */
    uint32_t target;                 /* 69A2 */
    int32_t path_x,path_y;           /* 69A6, 69AA */
    int16_t bounds[4];               /* 69AE x-, 69B0 y-, 69B2 x+, 69B4 y+ */
    uint16_t mode;                   /* 69B6 path mode 0..7 */
    uint32_t lock;                   /* 69BC (0xFFFFFFFF = none) */
    int16_t lock_bounds[4];          /* 69C0..69C6 (0x8000 = no change) */
    int32_t shake;                   /* 69C8 */
    int32_t shake_applied;           /* 69CC */
    int32_t root_shake;              /* 69CC as added to the root by this update (0C7E36) */
    uint16_t restart;                /* 69D0 */
    int16_t view[4];                 /* 69D2..69D8 */
    int32_t speed_x,speed_y;         /* mission actor +72/+76 */
    bool held;                       /* command list reached op 8 */
    const uint8_t *list;uint32_t list_base;uint16_t list_size;
    neo_camera_event_t events[NEO_CAMERA_EVENTS];unsigned event_count,event_dropped;
    unsigned unsupported;uint32_t unsupported_at;
    /* Players of the last update (+3C/+40 projected words) for op 4. */
    struct { bool alive; uint16_t px,py; } op4_players[2];
} neo_camera_t;
/* One player as ROM 01903A reads it. */
typedef struct { bool alive; uint8_t follow_x,follow_y; uint16_t px,py; } neo_camera_player_t;
/* State of a mission start (0C7C1A/0C8084 after the list pointer is set):
 * list = ROM bytes of the mission's command list at `base`. */
void neo_camera_init(neo_camera_t *c,const uint8_t *list,uint16_t size,uint32_t base);
/* One mission-actor update: desired movement (018D96, players + the script
 * point `point69b8`/`point69ba` RAM 69B8/69BA, flag RAM 10C322 = `follow`)
 * then 0C81C0. The command list may rewrite the point (op 0x34). */
void neo_camera_update(neo_camera_t *c,const neo_camera_player_t players[2],bool follow,
    uint16_t *point69b8,uint16_t *point69ba);
/* The two halves of neo_camera_update: 018D96..0C7CA6 (camera position,
 * root, view) and the rest of 0C81C0 (shake decay 0C804A, command list).
 * `finish` clears the event list before filling it. */
void neo_camera_track(neo_camera_t *c,const neo_camera_player_t players[2],bool follow,uint16_t point69b8,uint16_t point69ba);
void neo_camera_finish(neo_camera_t *c,uint16_t *point69b8,uint16_t *point69ba);
/* 0C7CA6 with an explicit movement (also used by the tests). */
void neo_camera_move(neo_camera_t *c,int32_t dx,int32_t dy);
void neo_camera_commands(neo_camera_t *c,uint16_t *point69b8,uint16_t *point69ba);  /* 0C83E6 */
void neo_camera_lock(neo_camera_t *c,uint16_t x,const uint16_t bounds[4]);         /* 0C7B0C */
void neo_camera_release(neo_camera_t *c);                                          /* 0C7C12 */
void neo_camera_autoscroll(neo_camera_t *c);                                       /* 0C7B4E */
void neo_camera_autoscroll_stop(neo_camera_t *c);                                  /* 0C7BBA */
static inline void neo_camera_set_auto(neo_camera_t *c,uint8_t mode) {c->auto_mode=mode;} /* 0C7B48 */
void neo_camera_shake_start(neo_camera_t *c);                                      /* 0C801A */
void neo_camera_shake_pulse(neo_camera_t *c);                                      /* 0C8022 */
/* Root object 10E968 as the actors see it: x = -camera, y = 0x01000000 -
 * height + the shake applied before this update's decay. */
static inline int32_t neo_camera_root_x(const neo_camera_t *c) {return (int32_t)(0u-(uint32_t)c->x);}
static inline int32_t neo_camera_root_y(const neo_camera_t *c) {return (int32_t)(0x01000000u-(uint32_t)c->y+(uint32_t)c->root_shake);}
static inline uint16_t neo_camera_progress(const neo_camera_t *c) {return (uint16_t)(c->progress>>16);}
#endif
