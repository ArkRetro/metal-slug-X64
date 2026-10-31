#ifndef MSX_SCENE_CLOCK_H
#define MSX_SCENE_CLOCK_H
#include <stdint.h>
#include <stdbool.h>
/* Original Neo Geo tick, separate from VI presentation. Integer phase accumulator. */
typedef struct { uint32_t previous; uint64_t phase; bool initialized; } scene_clock_t;
static inline void scene_clock_reset(scene_clock_t *c) { c->phase=0;c->initialized=false; }
static inline unsigned scene_clock_steps(scene_clock_t *c,uint32_t now,uint32_t frequency) {
    if (!c->initialized) { c->previous=now;c->initialized=true;return 0; }
    uint32_t elapsed=now-c->previous;c->previous=now;
    c->phase+=(uint64_t)elapsed*59185606u;
    uint64_t threshold=(uint64_t)frequency*1000000u;
    unsigned count=0;
    // Limit per-render catch-up, retaining all debt instead of dropping simulation ticks.
    while (c->phase>=threshold && count<8) { c->phase-=threshold;count++; }
    return count;
}
/* Gives `count` steps back to the clock: they stay as debt for later frames
 * (the frame runs fewer steps, the simulation loses none). */
static inline void scene_clock_defer(scene_clock_t *c,unsigned count,uint32_t frequency) {
    c->phase+=(uint64_t)count*((uint64_t)frequency*1000000u);
}
#endif
