#ifndef MSX_NEO_BOSS_H
#define MSX_NEO_BOSS_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_animation.h"
#include "neo_contact.h"
#include "neo_cull.h"
/* Native translation of the Mission 1 boss of Metal Slug X (script record
 * 0x2D70FC, constructor ROM 0D5976, routine family 0D5976..0D74D4) and of
 * the generic shell path it fires (02A260/02A2AC/02A2FE/02A244). Routine
 * labels are provenance tags only; nothing here decodes or executes 68000
 * code. See docs/JEFE_MISION1.md.
 *
 * Every boss actor keeps its original field block (+10..+AF) as a
 * big-endian byte image: the original reuses bytes with several widths and
 * new nodes inherit the bytes their previous occupant left (ROM 0040BC does
 * not clear them), so the image is the exact state. It never holds CPU
 * addresses: fields that held pointers keep opaque tags (ROM data labels,
 * NEO_BOSS_TAG node tags, the player tags 0x1014C4/0x101574 and scene object
 * tags). The animation program/cursor (+20) is native. */
enum { NEO_BOSS_NODES=40,NEO_BOSS_FIELDS=0xb0 };
#define NEO_BOSS_TAG(index) (0x00b05000u|((uint32_t)(index)<<4))
#define NEO_BOSS_IS_TAG(v) (((v)&0xfffff00fu)==0x00b05000u)
#define NEO_BOSS_TAG_INDEX(v) ((unsigned)(((v)>>4)&0xffu))
#define NEO_BOSS_PLAYER1 0x1014c4u
#define NEO_BOSS_PLAYER2 0x101574u
typedef struct {
    uint32_t routine;            /* +00: ROM label of the next update (0 = free) */
    int16_t creator;             /* +0C: creator node index, -1 = the script actor */
    int16_t next;                /* list order among boss nodes (-1 = last) */
    uint8_t depth;               /* +10 (list depth, used by 0041AC deletes) */
    uint8_t f[NEO_BOSS_FIELDS];  /* +00..+AF image; +00..+0F and +20..+23 unused */
    neo_anim_state_t anim;       /* +20 program/cursor; +17/+18/+24/+26 mirrored in f */
} neo_boss_node_t;
/* Generic actors the boss creates or turns into (not part of this module):
 * the world builds them from the seed (the original constructor label tells
 * which). `node` is the creator, or the converted node itself. */
typedef enum {
    NEO_BOSS_EFFECT_SPAWN,      /* child allocated by 0040BC with constructor `label` */
    NEO_BOSS_EFFECT_CONVERT,    /* boss node jumps into generic code `label` (01C71E, 01C706, 08EEBC) */
    NEO_BOSS_EFFECT_DEBRIS,     /* 01DA40 with the ROM debris table `label` */
    NEO_BOSS_EFFECT_SMOKE,      /* 01DC8E with parameter `param` */
    NEO_BOSS_EFFECT_DELETE_TREE /* 0041AC: delete the generic descendants that follow `node` */
} neo_boss_effect_kind_t;
typedef struct {
    neo_boss_effect_kind_t kind;
    uint32_t label;             /* ROM constructor / routine / table label (provenance) */
    uint16_t param;
    int16_t node;               /* boss node index */
    uint8_t image[NEO_BOSS_FIELDS]; /* seed fields as written by the boss code */
    bool has_image;
} neo_boss_effect_t;
/* Original results of the external services. Each returns false when the
 * world cannot answer (the update then fails with NEO_BOSS_UNSUPPORTED). */
typedef struct {
    int32_t x,y,vx,vy;          /* kernel d0..d3 */
    uint32_t parent;            /* scene tag (A0) */
    uint16_t flags;             /* D6 bits 0..4, 7 */
    uint16_t kind;              /* D7 >> 8 (+6C) */
} neo_boss_sweep_t;
typedef struct {
    void *context;
    /* ROM 0029C6. */
    uint16_t (*random)(void *context);
    /* ROM 0040BC: true when more than `reserve` actor nodes are free. On
     * success `image` receives the bytes the new node inherits. */
    bool (*allocate)(void *context,uint16_t reserve,uint8_t image[NEO_BOSS_FIELDS]);
    /* ROM 009EA8 (D1=0): palette slot for a logical resource. */
    bool (*palette)(void *context,uint16_t resource,uint8_t *slot);
    /* ROM 009F88: release the node's palette references. */
    void (*palette_release)(void *context,const uint8_t *slots,unsigned count);
    /* ROM 0120F8. */
    void (*sound)(void *context,uint16_t id);
    /* Projection parent that is a scene object (+14/+18 and +25 of the object). */
    bool (*scene)(void *context,uint32_t tag,neo_contact_parent_t *out);
    /* ROM 013968 through 0143E4 (air step) and 013DB2 through 0146E8. */
    bool (*sweep)(void *context,const neo_contact_request_t *request,neo_boss_sweep_t *out);
    bool (*floor)(void *context,int32_t x,int32_t y,uint32_t parent,neo_boss_sweep_t *out);
    /* ROM 0140C0: register/update object platform `table` (ROM label) for a
     * node; returns the slot (+6A, 0xFF = none). ROM 013F1E releases it. */
    bool (*platform)(void *context,int16_t node,uint8_t slot,uint32_t table,const int32_t position[4],
        uint32_t parent,uint8_t *slot_out);
    void (*platform_release)(void *context,int16_t node,uint8_t slot);
    /* ROM 004C24 registration in the hit broadphase: `offset` 0x58/0x59 and
     * the index byte stored there; kill_all is the 004BAC condition. */
    bool (*hurtbox)(void *context,int16_t node,uint8_t *offset,uint8_t *index,bool *kill_all);
    /* ROM 0049E8: the node attacks with the rows of ROM table `rows`;
     * returns the highest target type hit (0 = none). */
    bool (*attack)(void *context,int16_t node,uint32_t rows,uint16_t *result);
    /* ROM 019504 scene bounds (RAM 10C2F0 block). */
    bool (*bounds)(void *context,neo_cull_bounds_t *out);
    void (*draw)(void *context,int16_t node,const neo_anim_draw_t *draw);
    /* Generic actors created or converted by the boss (see effect kinds). */
    bool (*effect)(void *context,const neo_boss_effect_t *effect);
    /* ROM 018F92 (mission complete) and 018F40 (results). */
    void (*mission)(void *context,int16_t node,uint8_t step);
} neo_boss_services_t;
/* Global state read by the boss (all external to it). */
typedef struct {
    uint8_t epoch;              /* RAM 10C33A */
    uint8_t facing606;          /* RAM 10C606 (copied to +76 by the constructor) */
    uint8_t rank;               /* RAM 10FD8B */
    uint8_t players88;          /* RAM 100088 */
    uint8_t alive_mask;         /* RAM 10C33C (01AE4A) */
    bool p1_node,p2_node;       /* player nodes not 0x3F1E (0040AC) */
    int16_t p1x,p1y,p2x,p2y;    /* player +3C/+40 words */
    bool mission_over;          /* RAM 10C300 (019424) */
    uint16_t frame;             /* RAM 10000E */
} neo_boss_env_t;
typedef enum { NEO_BOSS_MORE,NEO_BOSS_DELETED,NEO_BOSS_HANDED_OFF,NEO_BOSS_UNSUPPORTED } neo_boss_result_t;
typedef struct {
    neo_boss_node_t nodes[NEO_BOSS_NODES];
    int16_t first;              /* first boss node in list order */
    neo_boss_services_t services;
    neo_boss_env_t env;
    uint32_t unsupported_label; /* ROM label of the last unsupported path */
    uint32_t phase_changes,deaths;
    bool mission_complete;      /* set when 018F92 was requested */
    int16_t born[16];           /* nodes linked by the last neo_boss_step (in order) */
    uint8_t born_count;
} neo_boss_t;
void neo_boss_init(neo_boss_t *boss,const neo_boss_services_t *services);
/* Script record 0x2D70FC (op 7, constructor 0D5976): creates the main node
 * with the inherited image (01A136 copies the record words to +6E..). */
int16_t neo_boss_spawn(neo_boss_t *boss,const uint8_t image[NEO_BOSS_FIELDS]);
/* One original update (ROM 004246) of node `index`. */
neo_boss_result_t neo_boss_step(neo_boss_t *boss,int16_t index);
/* All boss nodes in list order (children created during the walk run in
 * the same walk, as in the original). Returns the first failure. */
neo_boss_result_t neo_boss_tick(neo_boss_t *boss);
int16_t neo_boss_next(const neo_boss_t *boss,int16_t index);
/* Damage intake: ROM 004892..0049E0 applied by an attacker's narrow phase
 * (0049E8) to boss node `index`. kind/damage/class are the attack row's
 * bytes (+8, D3 low word, D3 high word); impact is the narrow-phase point
 * (RAM 106750/106752); attacker5e is the attacker's +5E. Returns false if
 * the node is not a registered boss part. */
typedef struct {
    int32_t px,py,cx,cy;        /* attacker +3C/+40/+44/+48 */
    uint8_t kind,klass,attacker5e;
    uint16_t damage;
    int16_t impact_x,impact_y;
    bool scale_row;             /* +54 table present: damage scaled by row word */
    uint16_t scale;
} neo_boss_hit_t;
bool neo_boss_receive(neo_boss_t *boss,int16_t index,const neo_boss_hit_t *hit);
/* Big-endian field access (offsets are the original ones). */
uint8_t neo_boss_b(const neo_boss_node_t *n,unsigned offset);
uint16_t neo_boss_w(const neo_boss_node_t *n,unsigned offset);
uint32_t neo_boss_l(const neo_boss_node_t *n,unsigned offset);
/* Main body helpers for the integration: health (+4E) and phase (+79). */
uint16_t neo_boss_health(const neo_boss_t *boss);
/* ROM address of the node's current animation command (provenance, for tests). */
uint32_t neo_boss_anim_address(const neo_boss_node_t *node);
uint8_t neo_boss_phase(const neo_boss_t *boss);
#endif
