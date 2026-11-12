#ifndef MSX_NEO_GACTOR_H
#define MSX_NEO_GACTOR_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_animation.h"
#include "neo_contact.h"
#include "neo_control.h"
#include "neo_cull.h"
#include "neo_kernel.h"
#include "neo_rng.h"
#include "marco_ammo.h"
#include "neo_vslot.h"
/* Generic native actor for the region entities other than the rebel soldier
 * (vehicle, flying bomber, prisoners, scene objects, effects). Each family is
 * a manual C translation of its original routines (ROM labels are kept as
 * provenance); the actor's fields are kept in the original layout as a
 * native big-endian record `m` (+00..+BF), so a translation reads/writes the
 * same offsets as the original and a fixture can compare records directly.
 * Pointer-sized fields hold native tags (block tags, ROM data addresses as
 * provenance labels), never CPU addresses: +00 routine label, +0C/+38 block
 * tags, +50 hurtbox data label. The animation cursor (+20) is native. */
enum { NEO_GA_BYTES=0xC0, NEO_GA_TAG_BASE=0x40000000u };
#define NEO_GA_TAG(index) (NEO_GA_TAG_BASE|(uint32_t)(index))
typedef struct neo_gactor {
    uint8_t m[NEO_GA_BYTES];
    const neo_anim_program_t *program;uint16_t cursor;uint32_t program_tag;
    uint8_t palette_handles[4];
    /* Logical resources behind the native palette handles: handles 1..4 are
     * the node's own references (+19..+1C, counted in +1D); handles 5 and 6
     * hold the draw/flash slots (+14/+16) a node converted from the boss
     * brought along without a reference, which the original keeps drawing
     * with until the allocator reuses the slot. */
    uint16_t palette[6];
    int16_t index;            /* pool slot */
    /* Bytes written since the node was taken from the free list (bit per
     * offset). The fields ROM 004126 does not clear keep the previous
     * occupant's values in the original; the closed-loop test uses this to
     * tell such stale bytes from written ones. */
    uint8_t written[NEO_GA_BYTES/8];
    bool active;
    /* Deleted (routine 003F1E) but kept in the list while deeper nodes follow
     * it (ROM 004126 -> 00408C): its fields stay readable by its children. */
    bool placeholder;
    /* Program interrupted by a flinch (the original keeps +20 in +86 and
     * restarts it through 004256): native resume point. */
    const neo_anim_program_t *resume_program;uint16_t resume_cursor;uint32_t resume_tag;
} neo_gactor_t;
static inline uint8_t ga_b(const neo_gactor_t *a,unsigned o) {return a->m[o];}
static inline uint16_t ga_w(const neo_gactor_t *a,unsigned o) {return (uint16_t)(a->m[o]<<8|a->m[o+1]);}
static inline uint32_t ga_l(const neo_gactor_t *a,unsigned o) {return (uint32_t)ga_w(a,o)<<16|ga_w(a,o+2);}
static inline void ga_sb(neo_gactor_t *a,unsigned o,uint8_t v) {a->m[o]=v;a->written[o>>3]|=(uint8_t)(1u<<(o&7u));}
static inline void ga_sw(neo_gactor_t *a,unsigned o,uint16_t v) {ga_sb(a,o,(uint8_t)(v>>8));ga_sb(a,o+1,(uint8_t)v);}
static inline void ga_sl(neo_gactor_t *a,unsigned o,uint32_t v) {ga_sw(a,o,(uint16_t)(v>>16));ga_sw(a,o+2,(uint16_t)v);}
typedef struct neo_genv neo_genv_t;
/* One of ROM 0049E8's other results (004928..004952, returned in A0/A1 at
 * 004B96): a registered body the query hit (after the hit matrix 004582 and
 * the +54 multiplier, +5F bits 7 and 6 clear), as 01B162 reads it: +3C/+40
 * words and the first rectangle of its +50 descriptor (`box` false: +50 is
 * 0). */
typedef struct { bool valid; uint16_t x,y; bool box; int16_t rect[4]; uint16_t y_low,x_low; uint32_t tag; } neo_ga_target_t;   /* +42 / +3E: the low words the rocket 06381C reads; tag: the node itself (A0 / A1), for e->node */
/* RAM bytes shared by the Mission 1 building actors and the mission. */
typedef struct neo_ga_flags {
    uint8_t dome_signal,dome_group;  /* RAM 10C44E/10C44F (spawn-table entry 05486A) */
    uint8_t player_lock;             /* RAM 10E9E3 (0C8CD6 / 0C8CE6) */
    uint8_t player_handlers;         /* RAM 10E9F8/10E9FC (0CA2E0): 0 none, 1 = 054B18 (no-op), 2 = 0C0538 (018F92), 3 = 0CA3EC (walk to walk_to[], 0CA340), 5 = 0537E4 (sub.l d7,d7: no input; the Mission 6 mid-boss), 4 = 0BBF0E (sub.w d7,d7: no input, as 054B18; the metro's end 0BBF12) */
    uint16_t walk_to[2];             /* RAM 10EA00/10EA02: screen X the 0CA3EC handler walks each player to (0CA340) */
    uint8_t mission_end;             /* RAM 10C300 = FF (018F92: halt; the level script actor 101414 is deleted, 0041D0) */
    uint8_t end_kind;                /* RAM 10C304 (018F92: +5E of the caller) */
    uint8_t scroll_lock;             /* RAM 10C00B = FF (0B6EF4) */
    uint8_t ram400a,wrote400a;       /* RAM 10C00A as the last sweep left it (0128DA: 0; 0143CC: FF), and whether one ran */
    uint8_t stage_done;              /* 018F40 ran (RAM 10C303 = 0: the flow's 018722 ends the stage) */
    uint8_t player_guard;            /* 0C09D8 ran: each player's +84 = FFFF, +5F bit 6 set, +9A bit 2 clear */
    uint8_t flash_request;           /* RAM 1067E5 (004C06) */
    uint8_t kill_request;            /* RAM 1067E3 (004BFE), copied to 1067E2 by 00469C */
    uint8_t flash;                   /* RAM 109944 (009BB0: white screen) */
    uint8_t collapsed;               /* 0B58CC ran: the building's map is swapped */
    uint8_t barrel_patched;          /* 0B6882 ran: the destroyed shop's map patch */
    uint8_t gate_patched;            /* 0B6E06 ran (Mission 2 stage 1): map patches 0B6E4A/0B6E74 (0C70FC) */
    uint8_t signal358;               /* RAM 10C358 = 1 (03A892; read by 03A75A) */
    uint16_t limit700;               /* RAM 10C700: x limit of 08A732 (08A654), written by 0BC542/0BC548 (Mission 3 wagon 038D26) */
    uint8_t platform6bd;             /* RAM 10C6BD: Mission 3 platform 0B9286 locked (read by 0B9400) */
    uint16_t camera_restart;         /* RAM 10E9D0 (camera 69D0, read by 0B9286) */
    uint8_t m4[0x40];                /* RAM 10C6C0..10C6FF: the Mission 4 props (neo_m4props.c; 0BA942 resets them) */
    uint16_t rescued[2];             /* RAM 10C5FC/10C600: prisoners rescued per player (09AB38) */
    uint16_t tally[2];               /* RAM 10E9E8/10E9EA: rescues per player (0C8CC2, 099DCC family) */
    uint8_t gifts[2];                /* RAM 10C5FF/10C603: rescues of a +74 = FFFE prisoner (09AAD6) */
    uint8_t special[2];              /* RAM 10C5FE/10C602: rescues of the tied prisoner 099862 (09AB38) */
    uint8_t m6_signal;               /* RAM 10C44C (Mission 6 mid-boss 05310E: cleared by 052F8A, set by 0530F4) */
    uint8_t guard_min;               /* 0C09DA's D0 last asked (each player's +84 raised to it, +5F bit 6 set, +9A bit 2 clear; not applied yet) */
} neo_ga_flags_t;
/* The players' weapon, bomb and score state the item pickups serve
 * (neo_item.c, ROM 08B568..08B9B0); NULL in neo_genv_t: a pickup is an
 * explicit stop. RAM provenance per field. */
enum { NEO_ITEM_KINDS=0x5a };
typedef struct neo_pickup_env {
    marco_weapons_t *weapons;            /* RAM 10C718 */
    marco_weapon_player_t *player[2];    /* player actor fields (+93/+94/+95, 0040AC) */
    uint8_t *slot[2];                    /* vehicle slots 10C608/10C62C (NULL: none) */
    uint8_t score[2][4];                 /* RAM 10C46A / 10C472 (BCD, 057948) */
    uint32_t points[2];                  /* RAM 10C5A0+4p: value of the last pickup */
    uint32_t stamp[2];                   /* RAM 10C594+4p: frame of the last combo pickup */
    uint32_t frame;                      /* RAM 10C590 (frame counter the stamps compare) */
    uint16_t combo[2];                   /* RAM 10C5AC+2p (08B910) */
    uint8_t combo_lock[2];               /* RAM 10C5BE+p (08B7E6) */
    uint16_t mission;                    /* RAM 10C2F6 (0C0BAE) */
    uint16_t shot_sound;                 /* RAM 10C33E: frame of the last H sound (01B60E) */
    unsigned bullet_hits,blast_hits;     /* player bullets that hit; grenade explosion rows that hit (0049E8) */
    uint16_t label_count[2];             /* RAM 10C5B2+2p: score labels 08D330 made (08D33C) */
    unsigned foods[2];uint32_t food_points[2];  /* food pickups (record byte 3) and the last one's value (B42 life bar) */
    unsigned taken,popups,labels,cures,refuels;        /* pickups served; score labels 08D330 and
                                          * pickup labels 08C752 (both external) */
    /* Per item kind (+6E < 0x5A, records 0E581A): items constructed with a
     * native motion, items handed back as external (motion not translated:
     * not created) and pickups served. Provenance counters (the original
     * keeps none). */
    uint16_t item_created[NEO_ITEM_KINDS],item_external[NEO_ITEM_KINDS],item_taken[NEO_ITEM_KINDS];
} neo_pickup_env_t;
/* What the player's vehicles (Camel Slug 03458E, SV-001 092A32, their
 * turret 03B4E0) read and write outside their nodes; NULL in neo_genv_t: a
 * vehicle service is an explicit stop. RAM provenance per field. */
typedef struct neo_vehicle_env {
    neo_vslots_t *slots;             /* RAM 10C608/10C62C, 10C2E8/10C2EC (neo_vslot.c) */
    uint8_t *player[2];              /* player nodes 1014C4/101574 (+00..+BF original layout) */
    bool exists[2];                  /* 0040AC of the player nodes */
    uint16_t input[2][2];            /* 0CA2CE: 10E9EC/10E9F2 +0 new, +2 held */
    uint8_t repeat[2];               /* 0CA614: RAM 10F084+p */
    uint8_t halted,system;           /* RAM 10C301 (018F24), 10C300 (019424) */
    uint8_t dip10f000,dip10f001;     /* RAM 10F000 (bit 7: 03519E), 10F001 (bit 2: free bombs) */
    uint8_t solids;                  /* bodies registered in the table 106756 (004ED2) */
    /* The table 106756 itself (16 entries): each body's node (+00..+BF,
     * writable: 004ED2 sets its +15 bits 3/4) and descriptor; NULL free. */
    uint8_t *solid_node[16];uint32_t solid_desc[16];
    unsigned grenades;               /* grenades 0C0808 made for a rider (counted; the player's code) */
    uint32_t grenade_last;           /* routine of the last one */
} neo_vehicle_env_t;
/* RAM the Mission 2 stage-3 boss 026254 reads and writes outside its nodes
 * (neo_m2boss.c); NULL in neo_genv_t: its tail and 059FA8 stop explicitly. */
typedef struct neo_m2boss_env {
    int32_t camera_dy;               /* RAM 10E99A (camera 699A, read) */
    uint16_t point69ba;              /* RAM 10E9BA (written: 0x1A0, 0x110 while +8F bit 4) */
    int32_t speed[2];                /* RAM 1013D6/1013DA: mission actor +72/+76 (019388) */
    int16_t tower_mode;              /* RAM 10C354 (02624C/026230/026244) */
    uint8_t frame_step;              /* 003B4C: RAM 10008A + 10F009 (byte sum) */
} neo_m2boss_env_t;
/* The Mission 3 mid-boss 0A4A94 (neo_m3mid.c): its layer object 10E8C6
 * (RAM 10E8C6..10E94D: block +0..+25, map +2E, offsets +32/+34, window
 * +36/+38, column list +3E, column table +42..+81, flags +82/+83) and RAM
 * 10E959 (0C7BD0); the drawing is the renderer's: `draw` (0C73CE: d0..d5
 * of 0C714E, the layer's map +2E), `patch` (0A6D7C: 0C714E with the
 * sub-map) and `clear` (0C702A/0C703C/0C6D04). NULL in neo_genv_t: the
 * body stops explicitly. */
#define MSX_HAS_M3MID 1   /* the mid-boss's layer code in the world and the renderer */
typedef struct neo_m3mid_env {
    uint8_t *layer;uint8_t flags959;
    void (*draw)(void *ctx,uint32_t map,const int16_t d[6]);
    void (*patch)(void *ctx,uint32_t submap,const int16_t d[6]);
    void (*clear)(void *ctx);
} neo_m3mid_env_t;
/* The Mission 4 boss 02267E (neo_m4boss.c): its picture is the layer object
 * 10D7B6 (RAM 10D7B6..10D83D: the map 02225E, its patches by 0C714E), moved
 * by RAM 10E94E/10E952 while 10E957 (0C7BD0) is 3; RAM 10E9B8 (0xE0 at the
 * end); `anim_speed` is 002802 (D0 frames per step, neo_lspc.h). NULL in
 * neo_genv_t: the body stops explicitly. */
typedef struct neo_m4boss_env {
    uint8_t *layer;uint8_t owner;int32_t speed[2];uint16_t point69b8;
    void (*draw)(void *ctx,uint32_t map,const int16_t d[6]);
    void (*patch)(void *ctx,uint32_t submap,const int16_t d[6]);
    void (*clear)(void *ctx);
    void (*anim_speed)(void *ctx,uint16_t frames);
} neo_m4boss_env_t;
/* The Mission 6 stage 1 hanging bridge's actors 0BD18E / 0BD2BA / 0BD2CE and
 * its spans 0BCFA4..0BD004 (neo_m6props.c): the bridge layer 10D7B6 as the
 * update reads it (+0 x, +32 column origin, +36 local column) and RAM
 * 10C70C..10C714 (ram[0] = 10C70C: the creators' lock bytes 10C70C /
 * 10C70D, the previous x 10C70E, the swing bytes 10C712..10C714). NULL in
 * neo_genv_t: an explicit stop. */
typedef struct neo_m6bridge_env {
    int32_t x;int16_t origin,o36;uint8_t *ram;
} neo_m6bridge_env_t;
struct neo_genv {
    neo_kscene_t *kernel;uint8_t epoch;neo_rng_t *rng;
    neo_control_frame_t players;     /* P1/P2 projected words, mask, mode, rank */
    uint8_t player_flags15[2];       /* the players' +15 (RAM 1014D9 / 101589): bit 6 on the ground (01AF1A) */
    neo_cull_bounds_t bounds;        /* RAM 10C2F0 scene descriptor */
    uint16_t video;                  /* RAM 10000E */
    uint16_t ref69b8;                /* RAM 10E9B8: the script's player reference x (op 10, 01A30C; read by 098678) */
    uint16_t camera;                 /* RAM 10E99E */
    uint16_t camera_y;               /* RAM 10E992 (read by the Mission 6 mid-boss 05310E at its end) */
    uint16_t camera_low;             /* RAM 10E9A0 (low word of the progress long, 0C7B26) */
    uint32_t camera_stop;            /* RAM 10E9BC (0C7B26; FFFFFFFF: no stop. The world must fill it: 0 keeps the GO arrow idle) */
    uint8_t *arrow;                  /* RAM 10C44A: GO arrow direction (0529C0, 05287C) */
    uint16_t player_low[2];          /* RAM 101502/1015B2: the players' +3E (low word of the X long; 0613CA's long compares) */
    uint16_t player_ylow[2];         /* RAM 101506/1015B6: the players' +42 (low word of the Y long; 085FE4) */
    int32_t scroll_x,scroll_y;       /* RAM 10E97C/10E980 */
    uint16_t *grid;                  /* delay grid RAM 1067E6 */
    void *ctx;
    /* Position block of `tag` (terrain, root, dynamic object or an actor). */
    bool (*parent)(void *ctx,uint32_t tag,neo_contact_parent_t *out);
    const neo_anim_program_t *(*program)(uint32_t tag);
    neo_anim_emit_t emit;void *emit_ctx;
    /* ROM 0040BC (after the creator) or 00411C with the script actor as
     * creator (`after_script`). Returns NULL when the pool is exhausted. */
    neo_gactor_t *(*spawn)(void *ctx,neo_gactor_t *creator,uint32_t routine,bool after_script);
    /* ROM 0040BC with its reserve D0: no node unless more than `reserve`
     * nodes are free (RAM 1063D8). NULL: `spawn` (the reserve is ignored). */
    neo_gactor_t *(*spawn_reserve)(void *ctx,neo_gactor_t *creator,uint32_t routine,uint16_t reserve);
    /* ROM 009EA8 leaves in D0.w the hardware palette slot it gave (the
     * byte it stores at +19+n); the bomb 01C362 adds D0/D1 to its speed
     * after the call. The allocator is not modelled: NULL reads 0. */
    uint8_t (*palette_slot)(void *ctx,neo_gactor_t *a,unsigned n);
    /* Soldier spawned by an actor (spawn-table entry 0AF17E style). */
    void (*sound)(void *ctx,uint16_t id);
    /* ROM 0049E8: attack row against the registered hurtboxes of the given
     * class; applies the original damage side effects; returns max type. */
    uint8_t (*attack)(void *ctx,neo_gactor_t *a,const int16_t row[7]);
    /* With `want_targets` set (ga_attack_targets), the attack callback also
     * leaves in target[0]/target[1] the bodies 0049E8 returns in A0 (highest
     * +4E, `>=`) and A1 (lowest +4E, `<=`): the last body visited wins a
     * tie, in the registry's slot order (004B06: 16..31, 0..15, 48..63,
     * 32..47). */
    bool want_targets;
    neo_ga_target_t target[2];
    /* The (class, damage) pair of the row the attack callback is called for (0: the first; ga_attack calls it once
     * per pair of a row with several, 004B06). */
    uint8_t attack_pair;
    /* The node behind a target's tag or a player's node address (1014C4 /
     * 101574), as the fields the original reads of another node: +00 (0040AC:
     * 3F1E once deleted), +3C..+43, +5E, +5F. NULL: the tag is unknown. The
     * fighting prisoner 0DDCDA keeps the node 0049E8 gave it in +6E and its
     * shot 0DE0FE homes on it. */
    const uint8_t *(*node)(void *ctx,uint32_t tag);
    /* ROM 004C24: register this actor's hurtbox descriptor (+50). */
    void (*register_box)(void *ctx,neo_gactor_t *a);
    /* ROM 0140C0 / 013F1E: attach or release the actor's dynamic collision
     * object (+6A) with the cell map at ROM `map`, offset (dx,dy) pixels. */
    void (*dyn_set)(void *ctx,neo_gactor_t *a,uint32_t map,int16_t dx,int16_t dy);
    void (*dyn_release)(void *ctx,neo_gactor_t *a);
    /* ROM 004E76: register (desc != 0) or remove the actor as a solid body. */
    void (*solid)(void *ctx,neo_gactor_t *a,uint32_t desc);
    /* ROM 00409A: carry set when the creator (+0C) is alive. */
    bool (*creator_alive)(void *ctx,neo_gactor_t *a);
    neo_gactor_t *(*creator)(void *ctx,neo_gactor_t *a);
    /* The generic actor of a node tag held in a field (the SV-001's tread
     * node 014A4E in +8C); NULL when none. */
    neo_gactor_t *(*actor)(void *ctx,uint32_t tag);
    /* +04 of the original node: the node before `a` in the actor list (a
     * deleted node with deeper ones after it stays as a placeholder, 004126);
     * NULL when it is not a generic actor. */
    neo_gactor_t *(*previous)(void *ctx,neo_gactor_t *a);
    /* ROM 0C88B0: wreck template stamped into the static collision grid. */
    void (*stamp)(void *ctx,neo_gactor_t *a,uint32_t template_address);
    /* ROM 0C8A3C (A0 = 10C72E): the template's cells written over the
     * ring at the cell (+28 word >> 3, +2C word >> 3), no mask and no
     * window (0BC38E, the Mission 5 sewer gates 0BC3A6). */
    void (*stamp_cells)(void *ctx,neo_gactor_t *a,uint32_t template_address);
    /* ROM 0C70FC: copy the sub-map at ROM `submap` (+0 tiles, +4 collision,
     * +8 column pairs, +A blocks) into layer `layer` at block column d4,
     * block row d5 (d0/d1 source offsets, d2/d3 counts). */
    void (*map_patch)(void *ctx,uint32_t layer,uint32_t submap,const int16_t d[6]);
    uint16_t view[4];                /* RAM 10E9D2..10E9D8 (visible rectangle) */
    uint16_t pickup_box[8];          /* RAM 10C5EC/10C5F4: item pickup boxes (x,y,w,h) of players 1/2 */
    /* ROM 040110: object-list entry (scene-root-relative position). */
    void (*marker)(void *ctx,int32_t x,int32_t y,uint8_t height,uint8_t id);
    uint8_t *marker_seq;             /* RAM 10C446 */
    /* Object list read this update (RAM 10C3A2/10C3F4 by 10000F bit 0:
     * the one written during the previous update), for 040200/0402E6. */
    const neo_marker_t *markers;unsigned marker_count;
    uint8_t flag448;                 /* RAM 10E448 */
    bool blood;                      /* RAM 10FD8F nonzero (soft-DIP BLOOD ON): 01B2BE clears the carry */
    const uint8_t *groups;           /* RAM 10C326..10C32E live counts (019DCA) */
    uint8_t *flag606;                /* RAM 10C606 (toggled by 0A8C46, read by the boss) */
    /* An original actor this world does not create (counted, provenance). */
    void (*external)(void *ctx,uint32_t constructor);
    /* Mission flags outside the actor list (domed building 054904). */
    struct neo_ga_flags *flags;
    /* Layer object of the static collision terrain: 10C72E in the village,
     * 10D7B6 from camera 1023 (the floor answers of the traces). 0 = 10C72E. */
    uint32_t terrain_tag;
    bool kill_all;                   /* RAM 1067E2 (004BAC: registered non-class-1 bodies die) */
    bool clear_shots;                /* RAM 1067E4 (copy of 1067E5 by 0046E0): 004C0E carry, enemy shots vanish */
    void (*camera_mode)(void *ctx,uint8_t mode);       /* 0C7B48: RAM 10E95A */
    void (*camera_rise)(void *ctx,int32_t vy);         /* RAM 10E960 (autoscroll vy) */
    void (*camera_push)(void *ctx,int32_t vx);         /* RAM 10E95C (autoscroll vx) */
    void (*shake)(void *ctx);                          /* 0C801A */
    bool (*map_swap)(void *ctx);                       /* 0B595A: carry when inserted */
    bool (*front_layer)(void *ctx);                    /* 012022: RAM 10B954 < 10B95C */
    void (*kill_children)(void *ctx,neo_gactor_t *a);  /* 004192: delete the deeper nodes after `a` */
    bool (*has_children)(void *ctx,neo_gactor_t *a);   /* 00408C: carry when the next node is deeper than `a` */
    /* Sub-script actor 019E82: run the level-script record at ROM `record`
     * for actor `a` (the interpreter of the world, freed with the actor);
     * true once op 3 ran. */
    bool (*subscript)(void *ctx,neo_gactor_t *a,uint32_t record);
    neo_pickup_env_t *pickup;
    neo_vehicle_env_t *vehicle;
    neo_m2boss_env_t *m2boss;
    neo_m3mid_env_t *m3mid;
    neo_m4boss_env_t *m4boss;
    neo_m6bridge_env_t *m6bridge;
    void (*shake_pulse)(void *ctx);  /* 0C8022 (RAM 10E9C8/10E9CC) */
    /* RAM 10C340..10C34F: the players' beam blocks of weapons 06/0B (P1 at
     * +0, P2 at +8: point +0/+2, direction +4, life +5, alive +6, sound +7;
     * 01E13A/01E166/01E19E, neo_pweapon.c); NULL: the beam stops explicitly. */
    uint8_t *laser;
    /* Low word of the node's address as the slot services keep it (slot
     * +00, 0AA760; a turret's +0E): a nonzero id per node (NULL: 8000|index). */
    uint16_t (*node_word)(void *ctx,const neo_gactor_t *a);
    /* JMP 0B05FE / 0B0626 / 0B0632 from a generic node: the node becomes a
     * native soldier after this update (the world moves the record); false
     * when the world has no soldier slot (NULL: explicit stop). */
    bool (*become_soldier)(void *ctx,neo_gactor_t *a,uint32_t entry);
    /* A program's SPAWN_THROW (03BB8E / 03BBBC / 03BBEA) on a generic node:
     * the thrown object (neo_throw) after the script actor, as the soldiers'
     * NEO_SOLDIER_EVENT_THROW; false when the world has no slot (NULL:
     * explicit stop). */
    bool (*spawn_throw)(void *ctx,neo_gactor_t *a,uint32_t routine);
    unsigned unsupported;uint32_t unsupported_at;
};
typedef enum { GA_MORE, GA_DELETE, GA_DELETE_TREE, GA_UNSUPPORTED } ga_result_t;
/* ROM rom data imported by tools/extract_region_rom.py. */
const uint8_t *ga_rom(uint32_t address);
/* A lookup tried after the program's ROM blobs (the stage pack's ROM data). */
void ga_rom_set_extra(const uint8_t *(*lookup)(uint32_t address));
/* Dispatch one update of a generic actor by its routine label. */
ga_result_t neo_region_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_region_routine(uint32_t routine);
/* Shared original services used by every family (ROM labels in the names). */
neo_contact_actor_t ga_contact(const neo_gactor_t *a);
void ga_set_contact(neo_gactor_t *a,const neo_contact_actor_t *c);
bool ga_parent(neo_genv_t *e,uint32_t tag,neo_contact_parent_t *out);
void ga_project(neo_gactor_t *a,neo_genv_t *e);                 /* 01ABC8 */
void ga_place(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy,uint32_t parent); /* 01AB98 */
void ga_place_from(neo_gactor_t *child,const neo_gactor_t *source,neo_genv_t *e,int16_t dx,int16_t dy); /* 01B220 */
void ga_move(neo_gactor_t *a,neo_genv_t *e);                    /* 01AC38 */
uint16_t ga_air(neo_gactor_t *a,neo_genv_t *e);                 /* 0143E4 */
uint16_t ga_air_400a(neo_gactor_t *a,neo_genv_t *e);            /* 0143CC (RAM 10C00A = FF) */
uint16_t ga_air_kind(neo_gactor_t *a,neo_genv_t *e,uint32_t *kind); /* 0143E4, D7 out */
uint16_t ga_air_d4(neo_gactor_t *a,neo_genv_t *e,uint16_t *d4); /* 0143E4, and D4.w at its return (the sweep's 13968) */
uint16_t ga_ground(neo_gactor_t *a,neo_genv_t *e,uint32_t *kind); /* 01456A */
/* The same sweeps with RAM 10C00B/10C00D (0143AC/014574: both FF; 0128E8 of
 * 0143EE/014594: 10C00D FF, the screen bound 0138A0 of kernel->config). */
uint16_t ga_air_flags(neo_gactor_t *a,neo_genv_t *e,uint8_t b400b,uint8_t b400d,uint32_t *kind);
uint16_t ga_ground_flags(neo_gactor_t *a,neo_genv_t *e,uint8_t b400b,uint8_t b400d,uint32_t *kind);
void ga_stationary(neo_gactor_t *a,neo_genv_t *e);              /* 01436A */
void ga_floor(neo_gactor_t *a,neo_genv_t *e);                   /* 0146E8 */
bool ga_probe_forward(neo_gactor_t *a,neo_genv_t *e,int16_t distance); /* 014850: carry */
bool ga_probe_vertical(neo_gactor_t *a,neo_genv_t *e,int16_t distance); /* 01495C: carry */
uint16_t ga_probe_vertical_d6(neo_gactor_t *a,neo_genv_t *e,int16_t distance); /* 01495C: D6 */
bool ga_anim_start(neo_gactor_t *a,neo_genv_t *e,uint32_t tag,uint16_t speed); /* 004256 */
/* 00427A. `call` handles family callbacks (NEO_ANIM_CALL / events); returns
 * 1 complete, 0 more, -1 unsupported. */
typedef bool (*ga_call_t)(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
int ga_anim_step(neo_gactor_t *a,neo_genv_t *e,ga_call_t call);
void ga_palette(neo_gactor_t *a,uint16_t resource);           /* 009EA8 (slot = +1D++) */
/* 011FF6 with D0 = 0: the descriptor drawn at the actor's projected words
 * (+3C/+40 less the +4D correction bits), attribute +14/+15, depth +1E. */
void ga_draw(neo_gactor_t *a,neo_genv_t *e,uint32_t descriptor);
void ga_flash_palette(neo_gactor_t *a,uint16_t resource);     /* 004520 */
uint8_t ga_attack(neo_gactor_t *a,neo_genv_t *e,uint32_t row); /* 0049E8 */
/* 0049E8 with its A0/A1 results in e->target[0]/[1] (cleared first). */
uint8_t ga_attack_targets(neo_gactor_t *a,neo_genv_t *e,uint32_t row);
bool ga_register(neo_gactor_t *a,neo_genv_t *e,int16_t *dx,int16_t *dy); /* 004C24: carry */
bool ga_players_in(neo_gactor_t *a,neo_genv_t *e,uint32_t rect); /* 01AFFC: carry when none */
bool ga_outside_view(neo_gactor_t *a,neo_genv_t *e,uint32_t rect); /* 0C8BEC */
uint16_t ga_rank_word(const neo_gactor_t *a,neo_genv_t *e,const uint8_t *table); /* 0260C8 (a type-2 table takes a random number) */
void ga_fail(neo_genv_t *e,uint32_t at);
/* The kernel object of a parent tag (0xFF: an actor's block) and back, and
 * the self exclusion of the actor running a query (01424C). */
uint8_t ga_kobj(uint32_t tag);
uint32_t ga_ktag(uint8_t id);
void ga_self(neo_gactor_t *a,neo_genv_t *e);
/* 004ED2 after 0128E8 (10C004 = 0, 10C009 = b4009): the push out of the
 * solid bodies of e->vehicle (table 106756). False on an explicit stop;
 * *top = the D7 = -1 of a kind-1 body under the actor. */
bool ga_push_solids(neo_gactor_t *a,neo_genv_t *e,uint8_t b4009,bool *top);
bool ga_cull(neo_gactor_t *a,neo_genv_t *e,uint32_t rect);        /* 01945C: carry = remove */
bool ga_cull_rect(neo_gactor_t *a,neo_genv_t *e,uint32_t rect);   /* 019504: carry = outside */
uint16_t ga_rand(neo_genv_t *e);                                  /* 0029C6 */
uint16_t ga_rand_range(neo_genv_t *e,uint16_t range);             /* 0029F2: (rng*range)>>16 */
int16_t ga_sine(uint8_t angle);                                   /* table ROM 0029FC */
int16_t ga_cosine(uint8_t angle);                                 /* table ROM 0029FC + 0x80 */
void ga_polar(uint32_t speed,uint8_t angle,uint32_t *vx,uint32_t *vy); /* 002E7C */
neo_gactor_t *ga_spawn_from(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy); /* 0040BC + 01B220 */
void ga_place_at(neo_gactor_t *child,const neo_gactor_t *source,neo_genv_t *e,int16_t dx,int16_t dy); /* 01ACF2 */
bool ga_creator_alive(neo_gactor_t *a,neo_genv_t *e);             /* 00409A carry */
int ga_marker_cross(neo_gactor_t *a,neo_genv_t *e,uint8_t *flag);  /* 040200: marker id or -1 */
int16_t ga_marker_follow(neo_gactor_t *a,neo_genv_t *e,uint8_t id); /* 0402E6 */
void ga_sweep_place(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *child,int16_t dx,int16_t dy); /* 014A02 */
/* 014A02 with its D0 (0: 10C008 = FF) and the result D6 & 15. */
uint16_t ga_sweep_place_d0(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *child,int16_t dx,int16_t dy,uint8_t d0);
/* 014906: the sweep from the actor's block by (dx,dy) pixels (10C004 = 0,
 * 10C008 = FF unless +15 bit 5, 10C009 = 0), nothing written back; the D6
 * of 013968 (bit 1: the carry). */
uint16_t ga_sweep_probe(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy);
/* 0149CA: carry when a solid cell lies under the block + (dx,dy) (013D5E,
 * 10C008 = FF unless +15 bit 5). */
bool ga_probe_point(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy);
/* 01495C with its D0: carry (true) with the vertical distance in pixels to the
 * surface met (the absolute y after the sweep less the one before). */
bool ga_probe_vertical_dy(neo_gactor_t *a,neo_genv_t *e,int16_t distance,int16_t *dy);
/* The same with 013968's D7 (group << 16 | angle word) in *kind. */
bool ga_probe_vertical_kind(neo_gactor_t *a,neo_genv_t *e,int16_t distance,int16_t *dy,uint32_t *kind);
#endif
