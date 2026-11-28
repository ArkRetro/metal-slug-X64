/* The few libdragon names src/msx_pcm.c uses, for its host build in
 * tools/test_msxl.c (make check; docs/COMPRESION.md). The cartridge file is
 * a file of the host; the mixer's sample buffer is the test's. Nothing here
 * is used by the console build. */
#ifndef HOST_MSXL_LIBDRAGON_H
#define HOST_MSXL_LIBDRAGON_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
typedef struct samplebuffer_s samplebuffer_t;
typedef void (*WaveformRead)(void *ctx,samplebuffer_t *sbuf,int wpos,int wlen,bool seeking);
typedef struct waveform_s { const char *name;uint8_t bits,channels;float frequency;int len,loop_len;WaveformRead read;void *ctx; } waveform_t;
void *samplebuffer_append(samplebuffer_t *sbuf,int wlen);
#define CachedAddr(p) ((void *)(p))
extern uint32_t host_ticks;
#define TICKS_READ() (host_ticks+=16u)
#define TICKS_TO_US(t) ((uint32_t)(t))
#define TICKS_FROM_US(us) ((uint32_t)(us))
static inline void data_cache_hit_writeback(const void *p,unsigned long n) {(void)p;(void)n;}
#define debugf(...) ((void)0)
#endif
