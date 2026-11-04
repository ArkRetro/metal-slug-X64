#ifndef MSX_SCENE_ASSETS_H
#define MSX_SCENE_ASSETS_H
#include <stdint.h>
#include <stdbool.h>
enum { SCENE_PAGE_BYTES=2048, SCENE_CHUNK_BYTES=16384 };
/* Mission packs (romfs/missionN.pack, tools/pack_mission.py): at most this
 * many chunks; their pages follow scene.pack's (SCENE_MISSION_PAGE_BASE,
 * set from scene_meta.h in scene_assets.c). */
enum { SCENE_MISSION_CHUNKS_MAX=432 };   /* Mission 2 with its map patches, the boulder (B37, 03A2AA) and the Slugnoid (B38) */
enum { SCENE_BASE, SCENE_ACTIONS, SCENE_SOLDIER, SCENE_MOTION, SCENE_PROJECTILES, SCENE_NATIVE, SCENE_BANK_COUNT };
typedef struct { uint32_t offset,length,crc,codec; } scene_chunk_t;
typedef struct { uint16_t start,first,second,interval,width,height; } scene_bank_t;
typedef struct { uint16_t page,palette; int16_t x,y; uint16_t width,height,format; } scene_image_t;
/* A map cell: layer, position and its image per video phase (& 7), as the
 * first phase's image plus, for an animated cell, a shared pattern of image
 * deltas (scene_cell_patterns; SCENE_CELL_STATIC: the same image in every
 * phase). 8 bytes instead of 8 ids (RAM budget, docs/RENDIMIENTO.md). */
enum { SCENE_CELL_STATIC=255, SCENE_CELL_PATTERNS=16 };
typedef struct { uint8_t layer,anim; int16_t x,y; uint16_t phase; } scene_map_cell_t;
extern int8_t scene_cell_patterns[SCENE_CELL_PATTERNS][8];
static inline uint16_t scene_cell_phase(const scene_map_cell_t *cell,unsigned phase) {
    return cell->anim==SCENE_CELL_STATIC?cell->phase:(uint16_t)(cell->phase+scene_cell_patterns[cell->anim][phase&7u]);
}
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
/* The cache begun in the boot cinematics' spare time: as many slots as fit
 * now keeping `keep` bytes free, sized for the heap it will have once
 * `coming` more bytes are freed (the movies); scene_assets_idle warms them
 * and scene_assets_begin adds the rest. False when nothing fits yet. */
bool scene_assets_begin_early(int coming,int keep);
/* Build the warm list ahead, a bounded step before `deadline` (TICKS_READ;
 * 0: all of it); true while steps remain. */
bool scene_assets_plan_step(uint32_t deadline);
/* More slots for a cache begun early, while the heap keeps `keep` bytes
 * (a movie closed). */
void scene_assets_grow_early(int keep);
/* Bytes of actor_init's allocations already made before the cache (Marco's
 * parts preloaded during the boot cinematics): taken off its reserve. */
void scene_assets_game_preloaded(unsigned bytes);
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
/* Cache slots holding a chunk, and slots allocated (diagnostics). */
void scene_assets_slots(unsigned *resident,unsigned *capacity);
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
/* Mission packs (romfs/missionN.pack, docs/MISION2.md "Fase 2"). A layer of
 * a stage scrolls by camera*x_num/x_den and rises by camera_y*y_num/y_den;
 * it is drawn while visible_min <= camera x <= visible_max. */
typedef struct { uint8_t x_num,x_den,y_num,y_den; int16_t visible_min,visible_max; } scene_layer_rule_t;
enum { SCENE_STAGE_DYNAMIC_MAPS=16 };
typedef struct {
    unsigned mission,index;          /* mission number (2..), stage index in the pack (0..) */
    uint32_t descriptor;             /* stage descriptor (016DDE..) */
    uint16_t width,camera_max;uint32_t backdrop;
    uint16_t columns,rows;           /* collision grid (8-px cells, column major, row 0 at the bottom) */
    const uint8_t *grid;uint8_t *grid_dynamic;const uint8_t *unknown;uint8_t *ring; /* kernel cell ring (rows > 32) */
    const uint8_t *camera_list;uint32_t camera_rom;uint16_t camera_len;   /* descriptor +0 */
    const uint8_t *script;uint32_t script_rom;uint16_t script_len;        /* +8 -> table 0x000800 */
    const uint8_t *players;uint32_t players_rom;uint16_t players_len;     /* +4 (019604 records) */
    scene_layer_rule_t rules[4];
    const scene_map_cell_t *cells;unsigned cell_count;
    /* Map patches 0C70FC (B37, tools/extract_map_patches.py): patch i's cells
     * follow the stage's cells with layer byte 0x80 | i << 3 | layer. */
    const scene_map_cell_t *patch_cells;unsigned patch_cell_count,patch_count;
    uint32_t patch_submap[16],patch_layer_object[16];
    void *index_order,*index_runs;   /* memory for the renderer's cull index (scene_cull.h) */
    /* Dynamic maps (0140C0) of the pack's 'MSXR' block (Mission 3): ROM
     * label, bytes (the ROM's, docs/MISION3.md "Reconocimiento del mapa"). */
    unsigned dynamic_map_count;uint32_t dynamic_map_rom[SCENE_STAGE_DYNAMIC_MAPS];uint16_t dynamic_map_length[SCENE_STAGE_DYNAMIC_MAPS];
    const uint8_t *dynamic_map[SCENE_STAGE_DYNAMIC_MAPS];
} scene_stage_t;
/* ROM bytes at `address` held by the loaded stage (its dynamic maps), or
 * NULL: the generic actors' ga_rom fallback (ga_rom_set_extra). */
const uint8_t *scene_stage_rom(uint32_t address);
/* Opens /missionN.pack (header only; Mission 1's map goes back to the LRU). */
bool scene_mission_load(unsigned mission);
/* Reads one stage's tables into reserved cache slots and pins its map. */
bool scene_stage_load(unsigned index);
void scene_assets_set_load_idle(void (*fn)(void));
/* Back to Mission 1's compiled tables. */
void scene_mission_unload(void);
/* The loaded stage, or NULL while Mission 1's compiled tables are active. */
const scene_stage_t *scene_stage(void);
#endif
