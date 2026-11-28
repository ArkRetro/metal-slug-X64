/* Thin Marco's fire with the weapons C/R/0D/E, S and F (+93 = 1, ids 2/3/4,
 * 7/8/9, 0C..0E): the upper-body states that start and repeat the fire
 * programs, translated from the ROM (docs/APROXIMACIONES.md A3).
 *
 *  0C0FEA   the input word with the fire press (+9B bit 4) removed while
 *           +9A bit 0 is clear and 10C71C (cap) <= 10C71A (live shots).
 *  0C3A5E   rest: a fresh fire press goes to 0C3CD0 in the same update;
 *           otherwise the aim rules (0C3A72..0C3AC0: 0C4100/12/24/36/48/5A,
 *           +92 = 1 / 0 / FF and the aim program, state 0C4176).
 *  0C4176   aim change: a fresh press fires in the same update; the end of
 *           its program sets 0C397E for the next update (0C397E/0C38B2 fall
 *           into 0C3A5E in that update).
 *  0C3CD0   +9B/+9C bit 4 cleared, +98 = 2, +92 from the stick (up 1, down
 *           FF, else 0), the program of table 0C3C10/0C3C50/0C3C90 (id, row
 *           +93*4+6E) and 0C3D9A in the same update.
 *  0C3D9A   0C0FEA (latches a press in +9B), then with +76 already written
 *           (+98 = 0) a latched press sets 0C3CD0 for the next update; then
 *           0C3778 steps the program (its first step in the update of
 *           0C3CD0: the CALL 0C4372 that fires); its END sets 0C38B2
 *           (overwriting 0C3CD0).
 *  Crouched (body 0C246A / 0C2588, table 0C24DE): the program runs as the
 *           body's (00427A) and +76 is tested after the step, so a latched
 *           press fires in the update after the one that wrote +76, END or
 *           not.
 *  Body states that drop the upper body (0C0F16: +7C = 0C381C, +90 = FF):
 *           the landing 0C1EBE and the walk stop 0C28E2 with +80 = 0 (both
 *           through 0C1EFA: kept with +92 > 0 or +98 != 0, else 0C1F24 with
 *           +92 = 0), the turn 0C2746 (kept with +98 = 1 or 2), the crouch
 *           0C2214 and the stand 0C1E34 (0C1F24). In 0C381C the upper body
 *           waits for +90: the stand 0C1FA6 sets it (0C1F4A) on a fresh fire,
 *           up or grenade press while +9A bit 5 is clear, the walk start
 *           0C285C (1) and the jump 0C2642/0C2654 (3/2); with it the
 *           dispatcher 0C37CE ends in 0C3A5E in the same update.
 * Program timing (004260/00427E: a WAIT n holds n updates, the END after a
 * WAIT ends in the update the WAIT expires) is in marco_thinfire.c. */
#ifndef MARCO_THINFIRE_H
#define MARCO_THINFIRE_H
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    THIN_REST,      /* 0C3A5E (and 0C397E/0C38B2, which fall into it) */
    THIN_SETTLE,    /* 0C397E or 0C38B2 set for the next update */
    THIN_AIM,       /* 0C4176 */
    THIN_WAIT,      /* 0C3D9A */
    THIN_NEXT,      /* 0C3CD0 set for the next update */
    THIN_DROPPED,   /* 0C381C with +90 = FF */
    THIN_CROUCH_ATTACK, /* body 0C2588 */
    THIN_CROUCH_NEXT    /* body 0C246A set for the next update */
} marco_thin_state_t;

typedef struct {
    marco_thin_state_t state;
    uint8_t step,t76,end;   /* program update, its +76 write and END updates */
    bool wrote76;           /* +76 */
    bool latched;           /* +9B bit 4 */
    int8_t aim92;           /* +92 */
    uint8_t action98;       /* +98 */
    bool rest_bit5;         /* +9A bit 5 */
    uint8_t walk80;         /* +80 of the walk (0C285C: 0x3F, ASR each update) */
    uint8_t turn;           /* updates left of the turn 0C27A8 */
    bool settle_pose0;      /* THIN_SETTLE is 0C397E (else 0C38B2) */
    bool walking,crouched,grounded;
} marco_thin_t;

typedef struct {
    uint8_t id;             /* 10C71E */
    bool fresh;             /* a fire press since the previous update */
    bool up,down;           /* the stick held */
    bool grounded;          /* +15 bit 6 */
    bool crouch;            /* the crouched body (0C2214..) */
    bool landed;            /* this update runs the landing 0C1EBE */
    bool turned;            /* the stick against the facing on the ground (0C28BE) */
    bool walking;           /* L/R held on the ground, not crouched (0C2888) */
    bool jumped;            /* this update starts a jump (0C2642/0C2654) */
    bool knife;             /* +9A bit 0 */
    uint8_t live,cap;       /* 10C71A, 10C71C */
} marco_thin_in_t;

typedef struct { bool fire; int8_t aim; bool crouched; } marco_thin_out_t;

void marco_thin_reset(marco_thin_t *s);
/* One player update. `out->fire`: the program's first step (CALL 0C4372)
 * runs in this update, with `aim` 1 up, -1 down, 0 level. */
marco_thin_out_t marco_thin_step(marco_thin_t *s,const marco_thin_in_t *in);
/* (+76 write, END) updates of the fire program of `id` for an aim (1 up, 0,
 * -1 down) or crouched; false for an id without a thin fire program here. */
bool marco_thin_program(uint8_t id,int aim,bool crouched,uint8_t *t76,uint8_t *end);
#endif
