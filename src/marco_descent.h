#ifndef MSX_MARCO_DESCENT_H
#define MSX_MARCO_DESCENT_H
#include <stdbool.h>
#include <stdint.h>
/* The player's parachute descent at the mission start (docs/MISSION_ENTRY.md,
 * "Descenso nativo"), manual translation over the player's node bytes `m`
 * (offsets as the original's 1014C4 node; labels are provenance only).
 *   0C15C6  start: +7A = FF, +7B = 0, +30 = 0, +34 = -8.0, +66/+1E = E0EC,
 *           +84 = 0x78; the canopy 0C4A58 (caller), program 0BF818 (caller,
 *           0C0F32), +61 = 0, +90 = 2, +9A bit 2; routine 0C162A.
 *   0C162A  +34 += (T - +34) >> 3 with T = -1.0 - ((+40 - 448.0) >> 5) when
 *           +40 >= 448.0 (else -1.0); +30 += (R - +30 - 0x8000) >> 4 with R
 *           the random word of 0029C6 under the high word of the previous
 *           D0 (ANDI.W leaves it); the input 0C0FEA: B (bit 5) cuts, A
 *           (bit 4) cuts while the +40 word is below 0x1C0; then the sweep
 *           0143EE (caller): on the ground the marker 0C5286 (caller),
 *           +61 = 1 and 0C1F4A, else +78 = (+34 word >= -1).
 *   0C16D8  cut: the marker (caller), +50 = 2D0AA8, program 0BF758 (caller),
 *           +90 = 2, +78 = 0, +30 = 0, +34 = 2.0, +61 = 2 (set at 0C167E);
 *           routine 0C170C, run in the same update.
 *   0C170C  +34 += (-22.0 - +34) >> 5, then 0C109C and 00427A (caller); on
 *           the ground 0C1EBE.
 * The slot release 0AA4C8 and the common processing 0C32F4/0C330E are the
 * caller's (the player has no vehicle slot during the descent). */
enum { MARCO_DESCENT_HANG=0xc162au,MARCO_DESCENT_FALL=0xc170cu,MARCO_DESCENT_LANDED=0xc1f4au,MARCO_DESCENT_FELL=0xc1ebeu };
typedef enum { MARCO_DESCENT_MORE,MARCO_DESCENT_CUT } marco_descent_result_t;
void marco_descent_begin(uint8_t *m);
/* 0C162A up to the sweep: `input` is 0C0FEA's D0 (marco_input_filter). */
marco_descent_result_t marco_descent_hang(uint8_t *m,uint16_t random,uint32_t input);
/* After the caller's sweep 0143EE (+15 bit 6 set on the ground): true on
 * the landing (routine 0C1F4A, the caller creates the marker). */
bool marco_descent_hang_after(uint8_t *m);
/* 0C170C before 0C109C. */
void marco_descent_fall(uint8_t *m);
/* After 0C109C/00427A: true on the ground (routine 0C1EBE). */
bool marco_descent_fall_after(uint8_t *m);
#endif
