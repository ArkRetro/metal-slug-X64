#include <assert.h>
#include <stdio.h>
#include "../src/scene_clock.h"
#include "../src/msx_vi.h"
/* The original's loop (src/scene_clock.h: 0036EE, 003A42..003AE8): vblanks
 * since the start of the update, two awaited, the rest cleared. Returns the
 * updates run when update i ends `work[i]` vblanks after it starts. */
static unsigned original_updates(const unsigned *work,unsigned n,unsigned vblanks) {
    unsigned t=0,u=0;
    for (unsigned i=0;i<n && t<vblanks;i++) {unsigned c=work[i]<2?2:work[i];t+=c;u++;}
    return u;
}
int main(void) {
    scene_clock_t clock={0};unsigned total=0;const uint32_t freq=46875000;
    for (unsigned f=0;f<=3600;f++) total+=scene_clock_steps(&clock,(uint32_t)((uint64_t)f*freq/60),freq,2);
    assert(total==3551); // 60 real seconds * 59.185606 Hz, rounded down.
    scene_clock_reset(&clock);assert(scene_clock_steps(&clock,0xffff0000u,freq,2)==0);
    assert(scene_clock_steps(&clock,0xffff0000u+freq/59,freq,2)==1); // Timer wrap.
    /* A one-second frame: one update (two steps), the other vblanks lost. */
    scene_clock_reset(&clock);assert(scene_clock_steps(&clock,10,freq,2)==0);
    assert(scene_clock_steps(&clock,10+freq,freq,2)==2);
    assert(scene_clock_steps(&clock,10+freq,freq,2)==0);   // no debt left
    total=0;for (unsigned f=1;f<=60;f++) total+=scene_clock_steps(&clock,10+freq+(uint32_t)((uint64_t)f*freq/60),freq,2);
    assert(total==59); // the next second runs at the original cadence, no catch-up
    /* Frames of 3 vblanks each: the original runs one update per 3 vblanks
     * (MAME, docs/TAS.md: loops of 3 vblanks); so does the clock. */
    scene_clock_reset(&clock);uint32_t now=0;scene_clock_steps(&clock,now,freq,2);
    unsigned steps=0;const uint64_t vb=(uint64_t)freq*1000000u/59185606u+1;   // just over one vblank in ticks
    for (unsigned i=0;i<100;i++) {now+=(uint32_t)(3*vb);steps+=scene_clock_steps(&clock,now,freq,2);}
    unsigned work[100];for (unsigned i=0;i<100;i++) work[i]=3;
    assert(steps/2==original_updates(work,100,300) && steps==200);
    /* The VI at the Neo Geo's refresh (src/msx_vi.h): the step ratio is the registers' field against 6 MHz / 384 / 264. */
    assert((unsigned __int128)MSX_VI_STEP_NUM*101376u*MSX_VI_CLOCK==(unsigned __int128)MSX_VI_STEP_DEN*6000000u*MSX_VI_LINES*MSX_VI_LINE);
    assert(MSX_VI_LINES==265 && MSX_VI_LINE==3104);
    /* A frame on time (one VI field) runs one step, every frame, over 25 000 fields (7 minutes): no frame without a
     * step and none with two; the 39 ppm the VI is slower add one step per 25 621 fields. */
    scene_clock_reset(&clock);assert(scene_clock_rate_steps(&clock,7,MSX_VI_STEP_NUM,MSX_VI_STEP_DEN,2)==0);
    for (uint32_t v=8;v<8+25000;v++) assert(scene_clock_rate_steps(&clock,v,MSX_VI_STEP_NUM,MSX_VI_STEP_DEN,2)==1);
    total=0;for (uint32_t v=8+25000;v<8+30000;v++) total+=scene_clock_rate_steps(&clock,v,MSX_VI_STEP_NUM,MSX_VI_STEP_DEN,2);
    assert(total==5001);   /* floor(30000 * 401640625 / 401624949) - 25000 */
    /* VI wrap; a frame of 3 fields: one update (two steps), the third field dropped (003A48), no catch-up after. */
    scene_clock_reset(&clock);scene_clock_rate_steps(&clock,0xfffffffeu,MSX_VI_STEP_NUM,MSX_VI_STEP_DEN,2);
    assert(scene_clock_rate_steps(&clock,0xffffffffu,MSX_VI_STEP_NUM,MSX_VI_STEP_DEN,2)==1);
    assert(scene_clock_rate_steps(&clock,2u,MSX_VI_STEP_NUM,MSX_VI_STEP_DEN,2)==2);
    assert(scene_clock_rate_steps(&clock,3u,MSX_VI_STEP_NUM,MSX_VI_STEP_DEN,2)==1);
    puts("PASS: VI at 59.183296 Hz (265 x 3104 clocks): one step per field on time, VI wrap, a 3-field frame drops its third");
    puts("PASS: original cadence, reset, timer wrap; slow frames slow down (003A48 clears the vblanks), no catch-up");
}
