/* Pause (not arcade): see msx_pause.h. */
#include "msx_pause.h"
/* A reset (R) or a new game only ends the pause: the button history goes
 * on (a button still held is no press), as the main loop's did before. */
void msx_pause_reset(msx_pause_t *p) {p->paused=false;}
msx_pause_frame_t msx_pause_frame(msx_pause_t *p,bool start,bool a,bool b,bool can_pause) {
    msx_pause_frame_t f={0};
    bool press=start && !p->start_down;
    p->start_down=start;
    if (p->paused) {
        p->paused_frames++;
        if (press) {p->paused=false;f.toggled=true;}
        return f;
    }
    if (press && can_pause) {p->paused=true;p->pauses++;p->paused_frames++;f.toggled=true;return f;}
    f.step=true;
    f.start_edge=press;f.a_edge=a && !p->prev_a;f.b_edge=b && !p->prev_b;
    p->prev_a=a;p->prev_b=b;
    return f;
}
