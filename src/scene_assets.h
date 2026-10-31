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
/* misses: every wait for a chunk (sync_misses: those of non-deferrable
 * draws: background, Marco, HUD); idle_decodes: decoded while the CPU waited
 * for a framebuffer (scene_assets_idle). */
typedef struct { unsigned dma_bytes,loads,hits,misses,evictions,cache_bytes,deferred,held,sync_misses,idle_decodes,warm_decodes;uint32_t decode_max_ticks; } scene_cache_stats_t;
/* Original sprite descriptors drawn by the native animation programs: parts
 * are consecutive SCENE_NATIVE images whose x/y are origin-relative (y down).
 * Palettes are the original logical resources (009EA8 ids). */
typedef struct { uint32_t tag; uint16_t first,count; } scene_native_descriptor_t;
typedef struct { uint16_t resource,palette; } scene_native_palette_t;
const scene_native_descriptor_t *scene_native_descriptor(uint32_t tag);
/* Pack palette index of a logical resource, or -1 when the capture set did
 * not show it allocated. */
int scene_native_palette(unsigned resource);
/* Allocates the scene cache (sized by the measured RAM budget) and queues
 * the warm-up; scene_assets_idle then loads it in spare frame time (the boot
 * logo's). scene_assets_init calls it if needed, then loads synchronously
 * what the first frame requires. */
void scene_assets_begin(void);
void scene_assets_init(void);
/* One step of idle-time work while the CPU waits for a framebuffer: start the
 * next DMA or decode a staged chunk if it ends before the next vertical blank.
 * Returns false when there is nothing to do or no time left in this frame. */
bool scene_assets_idle(void);
/* Heap report ("MSX MEM <stage>") and the asserted budget once the game is
 * loaded; scene_assets_heap_low samples and returns the lowest free heap. */
void scene_assets_mem(const char *stage);
void scene_assets_check_budget(void);
int scene_assets_heap_low(void);
const scene_bank_t *scene_bank(unsigned id);
const scene_image_t *scene_frame(unsigned id,unsigned frame);
const scene_image_t *scene_background_tile(unsigned row,unsigned col);
const scene_map_cell_t *scene_map_cells(unsigned *count);
/* Layer-2 cells of the destroyed-shop patch (0B6882), drawn instead of the
 * cells at the same coordinates once the building is down. */
const scene_map_cell_t *scene_patch_cells(unsigned *count);
/* Layer-3 cells of the rubble map 0B53A8 (0B58CC), drawn instead of the
 * facade once the domed building has collapsed. */
const scene_map_cell_t *scene_collapse_cells(unsigned *count);
const scene_image_t *scene_map_image(unsigned id);
unsigned scene_map_camera_max(void);
uint32_t scene_map_backdrop(bool after);
const uint16_t *scene_palette(unsigned id);
bool scene_assets_all_resident(void);
bool scene_assets_map_resident(void);
void scene_request_image(const scene_image_t *image,unsigned priority);
void scene_service(void);
const void *scene_page(const scene_image_t *image);
/* Non-blocking variant for sprite parts: once this frame has spent its
 * decode budget (SCENE_SYNC_DECODES, shared with the prefetch decodes of
 * scene_service), a part whose chunk is not
 * resident is requested at top priority and NULL is returned (the part is
 * not drawn this frame) instead of waiting for another DMA + decode. */
const void *scene_page_try(const scene_image_t *image);
enum { SCENE_SYNC_DECODES=1, SCENE_SERVICE_DECODES=1 };
/* Low-priority requests for every chunk of the boss poses (the fight's whole
 * set fits the 8 MiB cache next to the arena map), so its animation changes
 * find their parts resident instead of decoding several chunks in one frame. */
void scene_prefetch_boss(void);
/* Script lookahead: requests (lowest priority) the chunks of the actors the
 * script spawns within SCENE_AHEAD_PIXELS ahead of the camera (scene_ahead,
 * tools/pack_scene.py from data/mission1_rest_inventory.json) and keeps the
 * resident ones most recently used. Rendering only: no effect on the game. */
void scene_prefetch_ahead(int camera);
/* Recorded background blocks (scene_renderer.c): while recording, every page
 * served pins its chunk under `group` (0..3) until scene_unpin_group; a pinned
 * chunk is never evicted (the block keeps reading its RDRAM). */
void scene_record_pins(int group);
void scene_unpin_group(int group);
unsigned scene_completed_epoch(void);
void scene_begin_epoch(unsigned epoch);
void scene_complete_epoch(unsigned epoch);
scene_cache_stats_t scene_cache_stats(void);
#endif
