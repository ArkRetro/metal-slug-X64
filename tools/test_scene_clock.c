#include <assert.h>
#include <stdio.h>
#include "../src/scene_clock.h"
int main(void) {
    scene_clock_t clock={0};unsigned total=0;const uint32_t freq=46875000;
    for (unsigned f=0;f<=3600;f++) total+=scene_clock_steps(&clock,(uint32_t)((uint64_t)f*freq/60),freq);
    assert(total==3551); // 60 real seconds * 59.185606 Hz, rounded down.
    scene_clock_reset(&clock);assert(scene_clock_steps(&clock,0xffff0000u,freq)==0);
    assert(scene_clock_steps(&clock,0xffff0000u+freq/59,freq)==1); // Timer wrap.
    scene_clock_reset(&clock);assert(scene_clock_steps(&clock,10,freq)==0);
    assert(scene_clock_steps(&clock,10+freq,freq)==8);
    total=8;
    for (unsigned i=0;i<10;i++) total+=scene_clock_steps(&clock,10+freq,freq);
    assert(total==59); // Catch-up preserves simulation debt.
    scene_clock_reset(&clock);assert(scene_clock_steps(&clock,10,freq)==0);
    unsigned steps=scene_clock_steps(&clock,10+freq/10,freq);assert(steps==5);
    scene_clock_defer(&clock,steps-2,freq);total=2;     // A frame keeps two steps ...
    while ((steps=scene_clock_steps(&clock,10+freq/10,freq))) {
        if (steps>2) scene_clock_defer(&clock,steps-2,freq),steps=2;
        total+=steps;
    }
    assert(total==5); // ... and the deferred ones run in later frames.
    puts("PASS: original cadence, pause/reset, timer wrap and retained catch-up debt, deferred steps");
}
