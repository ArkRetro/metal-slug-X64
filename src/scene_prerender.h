#ifndef MSX_SCENE_PRERENDER_H
#define MSX_SCENE_PRERENDER_H
/* Pre-rendered background columns (docs/RENDIMIENTO.md
 * "Fondos pre-renderizados"; on by default,
 * -DMSX_BG_PRERENDER=0 leaves it out). A private file /mMsS.bgp
 * (tools/prerender_stage.py render) holds every static cell of each layer of
 * one stage laid out by 16-px column; the columns the camera reaches stream
 * from the cartridge into a ring per layer (async PI DMA, a margin ahead),
 * instead of the stage's whole map pinned in the scene cache. Animated and
 * patch cells stay on the tile path. */
#include <stdbool.h>
#include <stdint.h>
#ifndef MSX_BG_PRERENDER
#define MSX_BG_PRERENDER 1
#endif
/* A column run: n cells from y (layer coordinates), one palette (the pack's
 * mission palette index), 128 bytes of CI4 texels per cell after the runs. */
typedef struct { int16_t y;uint8_t n,pad;uint16_t palette,pad2; } scene_prerender_run_t;
#if MSX_BG_PRERENDER
/* Reads the stage's file header: the bytes its column tables and rings
 * take (0: no file, the tile path draws everything). */
unsigned scene_prerender_probe(unsigned mission,unsigned index);
/* Opens the probed file in `mem` (probe's bytes, 16-aligned: the stage's
 * reserved cache slots, released with its tables). */
bool scene_prerender_open(uint8_t *mem,unsigned palette_base);
/* Scene palette id of a run's palette: palette_base + run palette. */
unsigned scene_prerender_palette_base(void);
void scene_prerender_close(void);
/* Bit n: layer n is pre-rendered. */
unsigned scene_prerender_layers(void);
/* Columns of `layer` whose x lies in [x0,x1): first index and x of column 0. */
void scene_prerender_range(unsigned layer,int x0,int x1,int *first,int *end,int *column_x);
/* Column `c` resident (waits for its DMA, or reads it now): its runs, or
 * NULL when the column is empty. */
const scene_prerender_run_t *scene_prerender_column(unsigned layer,int c,unsigned *runs,const uint8_t **texels);
/* Starts the DMA of the columns of [x0,x1) not resident yet (one call per
 * frame and layer; a ring slot is reused only by a column of the window). */
void scene_prerender_prefetch(unsigned layer,int x0,int x1);
/* Ring bytes allocated, columns read synchronously (misses) and async. */
void scene_prerender_stats(unsigned *ring_bytes,unsigned *sync_reads,unsigned *async_reads);
#else
static inline unsigned scene_prerender_probe(unsigned mission,unsigned index) {(void)mission;(void)index;return 0;}
static inline bool scene_prerender_open(uint8_t *mem,unsigned palette_base) {(void)mem;(void)palette_base;return false;}
static inline void scene_prerender_close(void) {}
static inline unsigned scene_prerender_layers(void) {return 0;}
#endif
#endif
