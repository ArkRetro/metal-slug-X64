#ifndef MSX_NEO_END_H
#define MSX_NEO_END_H
#include "neo_gactor.h"
#include "neo_fade.h"
#include "neo_text.h"
/* Actors of the mission actor's end sequence (0x018846..0x01893A): the
 * prisoner tally 0308C0 (src/neo_tally.c) and the MISSION COMPLETE letters
 * 06090A (root, the letters 060A72 and the mission number 060B06). A small actor list of its own: the end sequence runs after
 * the world stops (gameplay.c), the nodes are created by the mission actor
 * 101364 (0040BC: after it, so at the head of this list) or by each other,
 * and a deleted node keeps its bytes (the letters read their root's +60/+72
 * after the root deleted itself, as in the original RAM). */
/* The title 0A7FFE has up to 47 nodes at once (its sparks) and creates 24 in one update. The list's own storage on the
 * console is 32 nodes (the letters, the tally, the results); the title runs on storage lent to the list (neo_end_lend:
 * the world's generic actors, idle then). The host builds own all 96. */
#ifdef N64
enum { NEO_END_OWN=32 };
#else
enum { NEO_END_OWN=96 };
#endif
enum { NEO_END_NODES=96, NEO_END_DRAWS=128, NEO_END_SOUNDS=8, NEO_END_MIXES=16 };
/* RAM the prisoner tally 0308C0 (src/neo_tally.c) reads and writes, and its
 * services. NULL in neo_end_inputs_t: a tally node is an explicit stop. */
typedef struct neo_end_ram {
    neo_hud_t *hud;                  /* fix map (VRAM 7000..74FF) */
    void (*on_write)(void *ctx,uint16_t address,uint16_t word);void *write_ctx; /* test hook */
    neo_fade_t *fade;                /* RAM 10991A.. (009C0A) */
    uint16_t rescued[2];             /* RAM 10C5FC/10C600 (counted down) */
    uint8_t special[2][2];           /* RAM 10C5FE/10C5FF, 10C602/10C603 */
    uint8_t score[2][4];             /* RAM 10C46A/10C472 (BCD, 057948) */
    uint8_t players;                 /* RAM 10C33C (01AEB0: bit p = player p in play) */
    uint8_t region;                  /* RAM 100000 byte (0: Japanese texts) */
    uint8_t medal[2];                /* RAM 10C306/10C307 (018FEA) */
    uint8_t pressed[2];              /* RAM 10C35D/10C363 (new presses) */
    uint16_t video;                  /* RAM 10000E */
    uint16_t (*rand)(void *ctx);void *rand_ctx;   /* 0029C6 */
    /* Outputs: sounds (0120F8), audio commands (01228C, not reproduced),
     * the mission actor's +60, text stops. */
    uint16_t sounds[NEO_END_SOUNDS];unsigned sound_count,sounds_dropped;
    unsigned audio_commands,unsupported_text;
    uint8_t mission60;
    /* The ending 0570A0 (src/neo_ending.c): the RAM bytes of the original it
     * reads and writes (scores 10C46A.., the table 10F010.., the text states
     * 10C384/10C450, inputs 10C35A..), the environment reset 056FCC. */
    uint8_t (*rd)(void *ctx,uint32_t address);void (*wr)(void *ctx,uint32_t address,uint8_t value);void *ram_ctx;
    void (*setup)(void *ctx);unsigned setups;
    /* The credits (0646FE.., 0A4408..): 009CCE(slot, target palette, level) of a node's palette slot (handle: the node's +14 / +19..)
     * and the audio command of 0A453A (01228C). */
    void (*slot_level)(void *ctx,neo_gactor_t *a,uint8_t handle,uint16_t target,uint8_t level);void *slot_ctx;unsigned slot_levels;
    bool (*rand_negative)(void *ctx);   /* the N flag 0029C6 returned with (0A83D6) */
    uint16_t audio_command;
} neo_end_ram_t;
typedef struct {
    uint32_t descriptor;
    uint16_t x,y,attributes;
    uint16_t palette;            /* logical resource */
    uint8_t zoom_x,zoom_y;       /* +12/+13 (0xFF: full size) */
    /* 009CCE on the draw's palette slot (neo_end_slot_level): the weight (0: none) towards the palette mix_target. */
    uint8_t mix_weight;uint16_t mix_target;
} neo_end_draw_t;
typedef struct {
    /* Scene root 10E968 (x, y, previous x/y) as 01ACF2 reads it. */
    neo_contact_parent_t root;
    neo_cull_bounds_t bounds;    /* RAM 10C2F0 */
    uint8_t epoch;               /* RAM 10C33A */
    bool front_layer;            /* 012022: RAM 10B954 < 10B95C */
    uint16_t mission;            /* RAM 10C2F4 (0: Mission 1) */
    const neo_anim_program_t *(*program)(uint32_t tag);
    neo_end_ram_t *ram;          /* the tally's RAM (NULL: tally nodes stop) */
} neo_end_inputs_t;
typedef struct neo_end {
    neo_gactor_t own[NEO_END_OWN];
    neo_gactor_t *lent;uint8_t lent_count;   /* storage of another owner (neo_end_lend), NULL: own */
    int8_t creator[NEO_END_NODES];       /* pool index, -1: the mission actor */
    uint8_t order[NEO_END_NODES];unsigned count;
    uint8_t mission60;                    /* mission actor 101364 +60 */
    neo_end_draw_t draws[NEO_END_DRAWS];unsigned draw_count;
    /* 009CCE: the palette slots of the list's nodes with a target and a weight (node = pool index + 1, 0: free;
     * handle = the slot's number in the node, 1..4), set by the nodes through neo_end_ram_t.slot_level. */
    struct {uint8_t node,handle,weight;uint16_t target;} mixes[NEO_END_MIXES];unsigned mix_overflow;
    unsigned spawned;                     /* nodes created so far (test binding) */
    int8_t spawn_order[NEO_END_NODES];    /* pool index by creation order */
    unsigned unsupported;uint32_t unsupported_at;
    /* Test hook: called when a node is taken (before its creator's writes). */
    void (*on_alloc)(void *ctx,neo_gactor_t *a,unsigned spawn);void *on_alloc_ctx;
    /* Test hook: called before each node's update (it may change the inputs' epoch and scene root). */
    void (*on_update)(void *ctx,neo_gactor_t *a);
    /* Test hook: called after each node's update (deleted: routine 003F1E). */
    void (*on_updated)(void *ctx,neo_gactor_t *a);
} neo_end_t;
static inline neo_gactor_t *neo_end_nodes(const neo_end_t *s) {return s->lent?s->lent:(neo_gactor_t *)s->own;}
static inline unsigned neo_end_capacity(const neo_end_t *s) {return s->lent?s->lent_count:(unsigned)NEO_END_OWN;}
/* 03CC10..03CC54 (03CD20.. for player 2's block 10C360), a player's port block of the main loop's port read 03CB3A:
 * +1 the held byte before, +2 `held` (the port inverted: up 1, down 2, left 4, right 8, A 0x10, B 0x20, C 0x40,
 * D 0x80), +3 the new presses, +4 the repeat byte (the new presses; else, when the count +5 runs out, the held byte,
 * and the count is RAM 10C381 again; else 0), +5 the count (RAM 10C380 at a new press). */
void neo_end_port_block(neo_end_ram_t *m,uint32_t block,uint8_t held);
void neo_end_reset(neo_end_t *s);
/* An empty list on `count` nodes of the caller (at most NEO_END_NODES), until the next neo_end_reset. */
void neo_end_lend(neo_end_t *s,neo_gactor_t *nodes,unsigned count);
/* 0040BC from the mission actor (A6 = 101364): 06090A. Returns the node. */
neo_gactor_t *neo_end_start(neo_end_t *s,uint32_t routine);
/* One update of every node in list order (nodes created during the pass run
 * in the same pass, as in the original list walk). */
void neo_end_update(neo_end_t *s,const neo_end_inputs_t *in);
/* neo_end_ram_t.slot_level of the list (ctx: the list); neo_end_update installs it when the RAM has none. */
void neo_end_slot_level(void *ctx,neo_gactor_t *a,uint8_t handle,uint16_t target,uint8_t level);
bool neo_end_routine(uint32_t routine);
/* Prisoner tally 0308C0 (src/neo_tally.c). */
bool neo_tally_routine(uint32_t routine);
ga_result_t neo_tally_tick(struct neo_end *s,neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m);
/* The ending's results and ranking 0570A0 (src/neo_ending.c). */
bool neo_ending_routine(uint32_t routine);
bool neo_ending_world_routine(uint32_t routine);
/* The list and the RAM come in e->end_list / e->end_ram (on the console the code is an overlay, src/msx_overlay.h). */
ga_result_t neo_ending_tick(neo_gactor_t *a,neo_genv_t *e);
#endif
