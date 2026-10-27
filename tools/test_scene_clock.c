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
    puts("PASS: original cadence, pause/reset, timer wrap and retained catch-up debt");
}
