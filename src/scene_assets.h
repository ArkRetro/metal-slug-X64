#ifndef MSX_SCENE_ASSETS_H
#define MSX_SCENE_ASSETS_H
#include <stdint.h>
#include <stdbool.h>
enum { SCENE_PAGE_BYTES=2048, SCENE_CHUNK_BYTES=16384 };
enum { SCENE_BASE, SCENE_ACTIONS, SCENE_SOLDIER, SCENE_MOTION };
typedef struct { uint32_t offset,length,crc,codec; } scene_chunk_t;
typedef struct { uint16_t start,first,second,interval,width,height; } scene_bank_t;
typedef struct { uint16_t page,palette; int16_t x,y; uint16_t width,height,format; } scene_image_t;
typedef struct { uint16_t layer; int16_t x,y; uint16_t phases[8]; } scene_map_cell_t;
typedef struct { unsigned dma_bytes,loads,hits,misses,evictions,cache_bytes; } scene_cache_stats_t;
void scene_assets_init(void);
const scene_bank_t *scene_bank(unsigned id);
const scene_image_t *scene_frame(unsigned id,unsigned frame);
const scene_image_t *scene_background_tile(unsigned row,unsigned col);
const scene_map_cell_t *scene_map_cells(unsigned *count);
const scene_image_t *scene_map_image(unsigned id);
unsigned scene_map_camera_max(void);
uint32_t scene_map_backdrop(void);
const uint16_t *scene_palette(unsigned id);
void scene_request_image(const scene_image_t *image,unsigned priority);
void scene_service(void);
const void *scene_page(const scene_image_t *image);
void scene_begin_epoch(unsigned epoch);
void scene_complete_epoch(unsigned epoch);
scene_cache_stats_t scene_cache_stats(void);
#endif
