#ifndef MSX_NEO_SOLDIER_AI_H
#define MSX_NEO_SOLDIER_AI_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_animation.h"
#include "neo_contact.h"
#include "neo_control.h"
#include "neo_cull.h"
#include "neo_kernel.h"
#include "neo_rng.h"
/* Native translation of the Mission 1 rebel soldier (constructor ROM 0AF17E,
 * routine family 0AE9C0..0B09B0) and of the shared pistol-death path
 * (0406B6/0407CA, 040DB4/040DF0). Routine labels are provenance tags only.
 *
 * Fields +00..+6D use the named native fields of the existing backend. The
 * routine-private block +6E..+93 is a big-endian byte block because the
 * original reuses the same bytes with other widths after death (+6E byte vs
 * word, +76 long over two words). It is never a pointer into CPU memory. */
enum { NEO_SOLDIER_LOCALS=0x26 };
/* Tag of dynamic collision object i (RAM 10C06E + 64*i). */
#define NEO_DYN_TAG(i) (0x10c06eu+64u*(unsigned)(i))
typedef struct {
    uint32_t routine;
    neo_contact_actor_t b;
    neo_anim_state_t anim;
    neo_anim_render_t render;
    uint16_t palette_resource;     /* logical resource requested through 009EA8 */
    uint16_t health;               /* +4E */
    uint8_t f16,f5b,f5c,f5d,f5e,f60,variant68,group69;
    uint8_t locals[NEO_SOLDIER_LOCALS]; /* +6E..+93 */
    bool active;
} neo_soldier_t;
/* A registered hurtbox (original 004C24 entry): position/facing are those of
 * the owner at query time, as the original narrow phase reads them. */
typedef struct {
    uint16_t x,y;          /* owner's projected words (+3C/+40) */
    uint8_t flags15,type,klass;
    int16_t rect[4];       /* descriptor rectangle (x,y,w,h) */
    bool dead;             /* +5F bit 7 or 6 */
} neo_hurtbox_t;
typedef enum { NEO_SOLDIER_EVENT_SOUND,NEO_SOLDIER_EVENT_THROW,NEO_SOLDIER_EVENT_DEATH_CHILD,
    NEO_SOLDIER_EVENT_KNIFE_HIT,NEO_SOLDIER_EVENT_GRENADE } neo_soldier_event_kind_t;   /* GRENADE: 03C2DE, the grenade 03C306 after the script actor at (0, 2C) */
typedef struct {
    neo_soldier_event_kind_t kind;
    uint16_t value;
    neo_contact_actor_t actor; /* spawn seed for THROW / DEATH_CHILD */
} neo_soldier_event_t;
typedef struct {
    neo_kscene_t *kernel;
    neo_contact_parent_t terrain,root;   /* tags 0x10C72E and 0x10E968 */
    const neo_contact_parent_t *dyn;     /* NEO_KDYN_COUNT dynamic objects, or NULL */
    uint8_t epoch;                        /* RAM 10C33A */
    neo_control_frame_t players;          /* p1/p2 projected words, mask, mode, rank */
    uint16_t *grid;                       /* 16x8 delay grid (RAM 1067E6) */
    neo_rng_t *rng;
    const neo_hurtbox_t *targets;unsigned target_count; /* other actors' hurtboxes */
    neo_cull_bounds_t bounds;             /* scene bounds for 01945C/019504 */
    uint16_t camera_word;                 /* RAM 10E97C high word (used by 0B094E) */
    const neo_anim_program_t *(*program)(uint32_t tag);
    neo_anim_emit_t emit;void *emit_context;
    void (*event)(void *context,const neo_soldier_event_t *event);void *event_context;
    bool blood;                           /* RAM 10FD8F nonzero: 01B2BE returns carry clear */
    /* Player actors 1014C4/101574 as 0B03E0 reads them: routine alive (0040AC),
     * +7A (0 = scripted, the soldiers stay idle) and RAM 10E9F8/10E9FC
     * (nonzero: CA2F8 carry). */
    struct { bool alive; uint8_t follow7a; bool vehicle; uint8_t follow7b; uint16_t x,y; } player_state[2];
    uint16_t point[2];                    /* RAM 69B8/69BA: script point (01903A) */
    const neo_marker_t *markers;unsigned marker_count; /* list queried this update */
} neo_soldier_env_t;
typedef enum { NEO_SOLDIER_MORE,NEO_SOLDIER_DELETED,NEO_SOLDIER_UNSUPPORTED } neo_soldier_result_t;
/* Spawn record words copied to +6E..+78 (ROM 01A136) and creator data. */
typedef struct {
    int32_t x,y;            /* local to the scene root, Q16.16 */
    uint16_t words[6];      /* +6E,+70,+72,+74,+76,+78 */
    uint8_t variant68,group69;
    uint32_t constructor;   /* spawn-table entry: 0 = 0AF17E.., or 04115A */
    uint8_t flags81;        /* +81 of the entry (0AF17E 2, 0AF18C 10, 0AF19A 12, 0AF1B6 4, 0AF1C4 8, 0AF1D2 C, 0AF1E0 E; 0: 2) */
} neo_soldier_spawn_t;
/* ROM 01ABB6 placement + 0AF17E/0AEF30 constructor, including its control
 * target/prefix/probes. Returns false (state untouched) for an unrecovered
 * constructor variant. */
bool neo_soldier_construct(neo_soldier_t *s,const neo_soldier_spawn_t *spawn,neo_soldier_env_t *env);
/* Source line of the last NEO_SOLDIER_UNSUPPORTED cause (diagnostics). */
extern unsigned neo_soldier_unsupported_line;
extern uint32_t neo_soldier_unsupported_detail; /* animation result<<28 | command asset */
/* Deaths by a hit kind whose entry of the tables 0406EA/040750/0407F4 is not
 * translated (explicit stops): count and kinds (bit per kind). */
extern unsigned neo_soldier_death_stops;extern uint32_t neo_soldier_death_stop_kinds;
/* One original actor update (every second video tick). */
neo_soldier_result_t neo_soldier_tick(neo_soldier_t *s,neo_soldier_env_t *env);
/* ROM 0B05FE / 0B0626 / 0B0632 on a converted generic node (see the .c). */
bool neo_soldier_become(neo_soldier_t *s,uint32_t entry,neo_soldier_env_t *env);
/* Big-endian accessors of the routine-private block. */
uint8_t neo_soldier_b(const neo_soldier_t *s,unsigned offset);
uint16_t neo_soldier_w(const neo_soldier_t *s,unsigned offset);
uint32_t neo_soldier_l(const neo_soldier_t *s,unsigned offset);
void neo_soldier_set_b(neo_soldier_t *s,unsigned offset,uint8_t v);
void neo_soldier_set_w(neo_soldier_t *s,unsigned offset,uint16_t v);
void neo_soldier_set_l(neo_soldier_t *s,unsigned offset,uint32_t v);
/* Registration geometry for others' queries (original +50 descriptors). */
bool neo_soldier_hurtbox(const neo_soldier_t *s,neo_hurtbox_t *out);
/* Original 0049E8 attack against registered hurtboxes: returns the highest
 * target type hit (0 = none). Damage application is the caller's. */
uint8_t neo_hit_query(const neo_contact_actor_t *attacker,const int16_t rect[4],const neo_hurtbox_t *targets,
    unsigned count,uint8_t attack_kind,uint8_t klass,int *first_hit);
/* Original 0049E8..0049E0 damage side effects on one soldier hit by an attack
 * row (x,y,w,h,kind,class,damage). `attacker` gives the attacker's projected
 * and cached positions. Returns true when the narrow phase hit the soldier
 * (the attacker is consumed); kills splat the grid for class-1 attacks. */
bool neo_soldier_receive(neo_soldier_t *s,const neo_contact_actor_t *attacker,uint8_t attacker5e,
    const int16_t attack[7],uint16_t *grid);
/* Original 002EB4: 8-bit angle of a word vector. */
uint8_t neo_angle(int16_t x,int16_t y);
/* Original 00505A: add the 3x3 danger splat at a projected position. */
void neo_grid_splat(uint16_t *grid,uint16_t x,uint16_t y);
/* Original 00469C: halve one grid row per call (rotating cursor). */
void neo_grid_decay(uint16_t *grid,unsigned *cursor);
#endif
