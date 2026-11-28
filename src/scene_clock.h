#ifndef MSX_SCENE_CLOCK_H
#define MSX_SCENE_CLOCK_H
#include <stdint.h>
#include <stdbool.h>
/* Original Neo Geo tick (one step = one vblank of the 68K), separate from VI
 * presentation. Integer phase accumulator at 59.185606 Hz.
 *
 * The original never catches up (P-ROM, MAME 0.264 dasm):
 *   0036EE  vblank IRQ: 10F009 += 1 (saturating, bcs 003702)
 *   003A42  main loop: 10008A = 10F009; 10F009 = 0 (003A48); one update
 *   003AE0  cmpi.b #2,10F009 / bcs 003AE0: wait until two vblanks have passed
 *   003AE8  bra 003A42
 * An update that overruns starts the next one at once and the vblanks past the
 * two are cleared with the counter: the game slows down, no debt is kept (the
 * only reader of 10008A, 003B4C via 059FA8, is a wait of its own).
 * So a frame runs at most `limit` steps (two: one original update) and the
 * whole vblanks beyond them are dropped; the fraction of the next vblank stays
 * (the vblank comes at its fixed time). */
typedef struct { uint32_t previous; uint64_t phase; bool initialized; } scene_clock_t;
static inline void scene_clock_reset(scene_clock_t *c) { c->phase=0;c->initialized=false; }
/* `now` counts in units of which `rate_den` make `rate_num` Neo Geo vblanks: the VI count with
 * MSX_VI_STEP_NUM / MSX_VI_STEP_DEN (src/msx_vi.h: the VI programmed to the Neo Geo's refresh, one step per field),
 * or the CPU count with 59185606 / (TICKS_PER_SECOND * 1000000) where the VI keeps libdragon's timing. */
static inline unsigned scene_clock_rate_steps(scene_clock_t *c,uint32_t now,uint64_t rate_num,uint64_t rate_den,unsigned limit) {
    if (!c->initialized) { c->previous=now;c->initialized=true;return 0; }
    uint32_t elapsed=now-c->previous;c->previous=now;
    c->phase+=(uint64_t)elapsed*rate_num;
    uint64_t count=c->phase/rate_den;c->phase%=rate_den;
    return count>limit?limit:(unsigned)count;
}
static inline unsigned scene_clock_steps(scene_clock_t *c,uint32_t now,uint32_t frequency,unsigned limit) {
    return scene_clock_rate_steps(c,now,59185606u,(uint64_t)frequency*1000000u,limit);
}
#endif
