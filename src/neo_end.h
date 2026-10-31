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
enum { NEO_END_NODES=32, NEO_END_DRAWS=64, NEO_END_SOUNDS=8 };
/* RAM the prisoner tally 0308C0 (src/neo_tally.c) reads and writes, and its
 * services. NULL in neo_end_inputs_t: a tally node is an explicit stop. */
typedef struct {
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
} neo_end_ram_t;
typedef struct {
    uint32_t descriptor;
    uint16_t x,y,attributes;
    uint16_t palette;            /* logical resource */
    uint8_t zoom_x,zoom_y;       /* +12/+13 (0xFF: full size) */
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
    neo_gactor_t nodes[NEO_END_NODES];
    int8_t creator[NEO_END_NODES];       /* pool index, -1: the mission actor */
    uint8_t order[NEO_END_NODES];unsigned count;
    uint8_t mission60;                    /* mission actor 101364 +60 */
    neo_end_draw_t draws[NEO_END_DRAWS];unsigned draw_count;
    unsigned spawned;                     /* nodes created so far (test binding) */
    int8_t spawn_order[NEO_END_NODES];    /* pool index by creation order */
    unsigned unsupported;uint32_t unsupported_at;
    /* Test hook: called when a node is taken (before its creator's writes). */
    void (*on_alloc)(void *ctx,neo_gactor_t *a,unsigned spawn);void *on_alloc_ctx;
    /* Test hook: called after each node's update (deleted: routine 003F1E). */
    void (*on_updated)(void *ctx,neo_gactor_t *a);
} neo_end_t;
void neo_end_reset(neo_end_t *s);
/* 0040BC from the mission actor (A6 = 101364): 06090A. Returns the node. */
neo_gactor_t *neo_end_start(neo_end_t *s,uint32_t routine);
/* One update of every node in list order (nodes created during the pass run
 * in the same pass, as in the original list walk). */
void neo_end_update(neo_end_t *s,const neo_end_inputs_t *in);
bool neo_end_routine(uint32_t routine);
/* Prisoner tally 0308C0 (src/neo_tally.c). */
bool neo_tally_routine(uint32_t routine);
ga_result_t neo_tally_tick(struct neo_end *s,neo_gactor_t *a,neo_genv_t *e,neo_end_ram_t *m);
#endif
