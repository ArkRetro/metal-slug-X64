#ifndef MSX_MARCO_TURN_H
#define MSX_MARCO_TURN_H
#include <stdbool.h>
/* B84. The run's turn as the fire sees it (gameplay.c): in the original the
 * body state runs before the weapon state (0C32F4 calls +7C after the body),
 * and the turn 0C2746 sets the weapon state 0C381C with +90 = FF (0C0F16),
 * which takes no input: a press in the update the turn starts, or in the
 * next one (0C27A8, the turn program's second update), is no shot (0C3CD0
 * clears the +9B latch at the next shot). After a visual update
 * (marco_visual_turn_gate): ready, forms 0/1 on the ground in the run, the
 * idle or the stop (their stick starts the run 0C285C, which goes on to
 * 0C28BE) with +98 != 2 (0C28C8), and its facing; hold, the turn program
 * not finished (0C27A8 in the next update). MAME marco-rt0/rt1/rtc. */
typedef struct { bool ready,hold; int facing; } marco_turn_gate_t;
/* The update with these inputs is a turn's (0C28BE: +9A bit 3, the stick
 * against the facing of the previous update), or the turn goes on. */
static inline bool marco_turn_gate_blocks(const marco_turn_gate_t *t,bool grounded,bool crouch,bool moving,int facing) {
    return t->hold || (t->ready && grounded && !crouch && moving && facing!=t->facing);
}
#endif
