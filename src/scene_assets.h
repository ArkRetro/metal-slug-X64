#ifndef MSX_SCENE_ASSETS_H
#define MSX_SCENE_ASSETS_H
#include <stdint.h>
#include <stdbool.h>
enum { SCENE_PAGE_BYTES=2048, SCENE_CHUNK_BYTES=16384 };
enum { SCENE_BASE, SCENE_ACTIONS, SCENE_SOLDIER, SCENE_MOTION, SCENE_PROJECTILES, SCENE_NATIVE, SCENE_BANK_COUNT };
typedef struct { uint32_t offset,length,crc,codec; } scene_chunk_t;
typedef struct { uint16_t start,first,second,interval,width,height; } scene_bank_t;
typedef struct { uint16_t page,palette; int16_t x,y; uint16_t width,height,format; } scene_image_t;
typedef struct { uint16_t layer; int16_t x,y; uint16_t phases[8]; } scene_map_cell_t;
typedef struct { unsigned dma_bytes,loads,hits,misses,evictions,cache_bytes; } scene_cache_stats_t;
/* Original sprite descriptors drawn by the native animation programs: parts
 * are consecutive SCENE_NATIVE images whose x/y are origin-relative (y down).
 * Palettes are the original logical resources (009EA8 ids). */
typedef struct { uint32_t tag; uint16_t first,count; } scene_native_descriptor_t;
typedef struct { uint16_t resource,palette; } scene_native_palette_t;
const scene_native_descriptor_t *scene_native_descriptor(uint32_t tag);
/* Pack palette index of a logical resource, or -1 when the capture set did
 * not show it allocated. */
int scene_native_palette(unsigned resource);
void scene_assets_init(void);
const scene_bank_t *scene_bank(unsigned id);
const scene_image_t *scene_frame(unsigned id,unsigned frame);
const scene_image_t *scene_background_tile(unsigned row,unsigned col);
const scene_map_cell_t *scene_map_cells(unsigned *count);
const scene_image_t *scene_map_image(unsigned id);
unsigned scene_map_camera_max(void);
uint32_t scene_map_backdrop(void);
const uint16_t *scene_palette(unsigned id);
bool scene_assets_all_resident(void);
void scene_request_image(const scene_image_t *image,unsigned priority);
void scene_service(void);
const void *scene_page(const scene_image_t *image);
void scene_begin_epoch(unsigned epoch);
void scene_complete_epoch(unsigned epoch);
scene_cache_stats_t scene_cache_stats(void);
#endif
