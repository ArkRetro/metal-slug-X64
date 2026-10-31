#ifndef MSX_NEO_LANDING_H
#define MSX_NEO_LANDING_H
#include "neo_end.h"
/* The parachute landing of the mission entry (docs/MISION1_COMPLETA.md,
 * "Aterrizaje"): the canopy 0C4A58, which follows the player during the
 * descent and, when he touches the ground (+15 bit 6), collapses behind him
 * (0C4B62: program 0C49D4) and is deleted at its end; and the player marker
 * 0C5286 ("1P" and the arrow, program 0C519E, palette 0x2A3) that the
 * player's 0C162A creates at the landing. The port starts at the landing
 * (the entry cinematic ends there): both are created then, in the state the
 * original has after the descent. */
enum { NEO_LANDING_DRAWS=16, NEO_LANDING_PLAYER=0x1014ecu /* the player's +28 block (+38 of the canopy) */, NEO_LANDING_ROOT=0x10e968u /* the scene root */ };
typedef struct {
    int32_t x,y,previous_x,previous_y;  /* player +3C/+40 and +44/+48 (projected, Q16.16) */
    uint8_t correction;                 /* player +4D */
    uint8_t flags15;                    /* player +15 (bit 6: on the ground) */
    uint16_t z;                         /* player +1E */
    uint16_t player;                    /* player +70 (0: player 1) */
    uint8_t epoch;                      /* RAM 10C33A */
    bool front_layer;                   /* 012022 */
    uint8_t flags5f,state61;            /* player +5F (bit 7: dying) and +61 (1 landed, 2 cut away) */
    int32_t root_x,root_y,root_previous_x,root_previous_y;  /* scene root 10E968 +14/+18, +1C/+20 (01AD52) */
    uint8_t root_correction;            /* its +25 */
    bool odd_phase;                     /* RAM 10000F bit 0 (00436E: +5F bit 3 hides the poses) */
} neo_landing_input_t;
typedef struct {
    neo_gactor_t canopy,marker;
    /* The marker's draws first (marker_draws of them, +1E FFFE: over the
     * player), then the canopy's (+1E = player's - 1: behind him). */
    neo_end_draw_t draws[NEO_LANDING_DRAWS];unsigned draw_count,marker_draws;
    unsigned unsupported;uint32_t unsupported_at;
    /* Test hook: called after each node's update (deleted: routine 003F1E). */
    void (*on_updated)(void *ctx,neo_gactor_t *a);void *ctx;
} neo_landing_t;
/* The handoff: the canopy as 0C4AAE leaves it in the descent (+28 = -4 or
 * +4 by its flip, +38 = the player, palette 0x38D), the marker 0C5286 to be
 * constructed in the first update. */
void neo_landing_start(neo_landing_t *s,const neo_landing_input_t *in,const neo_anim_program_t *(*program)(uint32_t tag));
/* The descent (0C15C6 creates the canopy, constructor 0C4A58 in its first
 * update; the marker comes with neo_landing_marker at the landing or the
 * cut, 0C16B0/0C16D8). */
void neo_landing_descent(neo_landing_t *s);
void neo_landing_marker(neo_landing_t *s);
/* One update after the player's (marker first: 0040BC inserts it right
 * after the player, before the canopy). */
void neo_landing_update(neo_landing_t *s,const neo_landing_input_t *in,const neo_anim_program_t *(*program)(uint32_t tag));
#endif
