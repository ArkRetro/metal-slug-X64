#ifndef MSX_PERF_H
#define MSX_PERF_H
/* Cheap per-frame CPU profile (docs/RENDIMIENTO.md). Sequential phases are
 * split by msx_perf_mark(): each mark charges the ticks since the previous
 * mark to one phase. Asset service/DMA waits overlap those phases and are
 * accumulated separately. One "MSX PERF" line every 300 frames reports mean
 * and max per phase in microseconds; no per-frame logging. */
#include <stdint.h>
enum {
    MSX_PERF_SIM,        /* scene_clock catch-up: every msx_game_update of the frame */
    MSX_PERF_PREP,       /* log diffs, animation pick, prefetch requests, effects frame */
    MSX_PERF_BEGIN,      /* scene_render_begin: service, attach/clear, mode */
    MSX_PERF_BG_PREFETCH,/* background cell prefetch requests + service */
    MSX_PERF_BG_DRAW,    /* background backdrop + cell blits */
    MSX_PERF_MESH,       /* contact-shadow mesh */
    MSX_PERF_ACTORS,     /* vehicle, native world draws, Marco, projectiles */
    MSX_PERF_HUD,        /* boxes and text */
    MSX_PERF_END,        /* scene_render_end: stats + detach */
    MSX_PERF_OUTSIDE,    /* outside actor_draw: display_get wait, audio, joypad */
    MSX_PERF_PHASES,
    MSX_PERF_ASSET=MSX_PERF_PHASES, /* overlapping: decode/CRC + require waits */
    MSX_PERF_STEP,       /* overlapping: slowest single simulation step */
    MSX_PERF_COUNT
};
/* Build with MSX_EXTRA_CFLAGS=-DMSX_PROFILE=0 to compile the profile out. */
#ifndef MSX_PROFILE
#define MSX_PROFILE 1
#endif
#if MSX_PROFILE
void msx_perf_frame_start(void);
void msx_perf_mark(unsigned phase);
void msx_perf_add(unsigned bucket,uint32_t ticks);
void msx_perf_steps(unsigned steps);
void msx_perf_counts(unsigned draws,unsigned soldiers);
void msx_perf_frame_end(void);
#else
static inline void msx_perf_frame_start(void) {}
static inline void msx_perf_mark(unsigned phase) {(void)phase;}
static inline void msx_perf_add(unsigned bucket,uint32_t ticks) {(void)bucket;(void)ticks;}
static inline void msx_perf_steps(unsigned steps) {(void)steps;}
static inline void msx_perf_counts(unsigned draws,unsigned soldiers) {(void)draws;(void)soldiers;}
static inline void msx_perf_frame_end(void) {}
#endif
#endif
