#ifndef MSX_PAUSE_H
#define MSX_PAUSE_H
/* Pause (not arcade, like the life bar B42; docs/PORT_STATUS.md "Pausa").
 * Start during play (player 1's panel 0C9848 in its play routine, where the
 * arcade ignores Start: only the continue panel 0C98CC reads it) pauses and
 * the next Start press resumes. This module owns the button history the
 * main loop turns into edges (Start, A, B), so that a paused run is the
 * unpaused run with the pause frames cut out:
 *  - the frame of the press that pauses, every frame while paused and the
 *    frame of the press that resumes run no simulation step (step=false);
 *  - those frames leave the A/B/Start history untouched: the first frame
 *    after the pause takes its edges against the last frame before it, as
 *    if the pause frames had not existed;
 *  - the Start presses that pause and resume never reach the arcade's
 *    Start handling: start_edge is the raw press of a frame that runs, so
 *    a Start still held after the resume is no press either.
 * Host-compilable (make check: test_gameplay pause). */
#include <stdbool.h>
typedef struct {
    bool paused;
    bool start_down;               /* Start held on the previous frame (any frame) */
    bool prev_a,prev_b;            /* A/B history of the frames that ran */
    unsigned pauses,paused_frames; /* counters for the logs */
} msx_pause_t;
typedef struct {
    bool step;                     /* the simulation runs on this frame */
    bool toggled;                  /* this frame paused or resumed */
    bool start_edge,a_edge,b_edge; /* edges for the game (only when step) */
} msx_pause_frame_t;
void msx_pause_reset(msx_pause_t *p);
/* One presented frame of raw buttons. can_pause: play is on (the arcade
 * ignores Start there); when false, Start goes to the arcade as before and
 * a held pause is kept (never entered there by this module). */
msx_pause_frame_t msx_pause_frame(msx_pause_t *p,bool start,bool a,bool b,bool can_pause);
#endif
