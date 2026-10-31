#include <libdragon.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "scene_assets.h"
#include "scene_codec.h"
#include "scene_meta.h"
#include "mission_camera.h"
#include "msx_perf.h"
_Static_assert(SCENE_MAP_CAMERA_MAX==MSX_CAMERA_MAX,"Scene pack and gameplay camera limits differ (filesystem/mission1/rest/map.json missing?)");

typedef struct { int chunk; bool ready; unsigned used_epoch,lru; } cache_slot_t;
static cache_slot_t *slots;
static int chunk_slot[SCENE_CHUNK_COUNT];
static unsigned requests[SCENE_CHUNK_COUNT];
static bool pinned[SCENE_CHUNK_COUNT];
static uint8_t block_pins[SCENE_CHUNK_COUNT];   // bit per recorded-block group
static int recording=-1;
static uint8_t *cache,*staging[2];
static unsigned capacity,clock_lru,epoch,rom,resident;
static volatile unsigned completed;
static int active_chunk=-1,active_slot=-1;
static unsigned active_stage,last_stage;
static scene_cache_stats_t stats;
static unsigned timing_depth; // require_chunk times its whole wait; nested service is not counted twice.
// Per-frame decode budget (reset by scene_begin_epoch): prefetch decodes in
// scene_service and synchronous misses of deferrable sprite parts.
static unsigned frame_service_decodes,frame_sync_decodes;
// PI status bits 0/1: DMA and IO busy. A nonblocking poll keeps prefetch asynchronous.
static bool pi_busy(void) { return (*PI_STATUS & 3u)!=0; }

static void start_next(void) {
    if (active_chunk>=0 || pi_busy()) return;
    int chosen=-1;
    for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++)
        if (requests[i] && (chosen<0 || requests[i]>requests[chosen])) chosen=(int)i;
    if (chosen<0) return;
    int victim=-1;
    for (unsigned i=0;i<capacity;i++) {
        if (slots[i].chunk<0) { victim=(int)i;break; }
        if (!slots[i].ready || pinned[slots[i].chunk] || block_pins[slots[i].chunk] || slots[i].used_epoch>completed) continue;
        if (victim<0 || slots[i].lru<slots[victim].lru) victim=(int)i;
    }
    if (victim<0) return;
    if (slots[victim].chunk>=0) { chunk_slot[slots[victim].chunk]=-1;stats.evictions++;resident--; }
    slots[victim]=(cache_slot_t){.chunk=chosen};chunk_slot[chosen]=victim;requests[chosen]=0;
    active_chunk=chosen;active_slot=victim;active_stage=last_stage^1;last_stage=active_stage;
    const scene_chunk_t *c=&scene_chunks[chosen];
    unsigned length=(c->length+63)&~63u;
    assertf(length<=SCENE_CHUNK_BYTES && c->offset+length<=SCENE_PACK_BYTES,"Pack chunk bounds");
    data_cache_hit_invalidate(staging[active_stage],length);
    dma_read_async(staging[active_stage],rom+c->offset,length);
    stats.loads++;stats.dma_bytes+=length;
}
void scene_service(void) {
    if (active_chunk<0) { start_next();return; }
    if (pi_busy()) return;
    // Prefetch spends at most SCENE_SERVICE_DECODES decodes per frame; the
    // chunk stays in its staging buffer for the next frame. A synchronous
    // wait (require_chunk) is never held back.
    // The budget is shared with the synchronous decodes of deferrable parts.
    if (!timing_depth && frame_service_decodes+frame_sync_decodes>=SCENE_SERVICE_DECODES) { stats.held++;return; }
    if (!timing_depth) frame_service_decodes++;
    uint32_t started=TICKS_READ();
    int chunk=active_chunk,slot=active_slot;unsigned stage=active_stage;
    active_chunk=active_slot=-1;
    // Reserve the other staging buffer and overlap its PI read with this decode.
    start_next();
    const scene_chunk_t *c=&scene_chunks[chunk];uint8_t *dst=cache+slot*SCENE_CHUNK_BYTES;
    bool decoded=scene_decode(dst,SCENE_CHUNK_BYTES,staging[stage],c->length,c->codec);
    assertf(decoded,"Bad scene codec chunk=%d codec=%lu length=%lu stage=%u first=%02x/%02x rom=%08x",chunk,(unsigned long)c->codec,(unsigned long)c->length,stage,staging[stage][0],staging[stage][1],(unsigned)(rom+c->offset));
    if (!decoded) abort();
    uint32_t crc=scene_crc32(dst,SCENE_CHUNK_BYTES);
    assertf(crc==c->crc,"Scene CRC mismatch");
    if (crc!=c->crc) abort();
    data_cache_hit_writeback(dst,SCENE_CHUNK_BYTES);
    slots[slot].ready=true;slots[slot].lru=++clock_lru;resident++;
    if (!timing_depth) msx_perf_add(MSX_PERF_ASSET,TICKS_READ()-started);
}
static void request(unsigned chunk,unsigned priority) {
    assertf(chunk<SCENE_CHUNK_COUNT,"Scene chunk ID");
    if (chunk_slot[chunk]>=0) return;
    if (requests[chunk]<priority) requests[chunk]=priority;
}
void scene_request_image(const scene_image_t *image,unsigned priority) { request(image->page/8,priority); }
/* Every chunk decoded and resident: requests are no-ops (request() returns for
 * resident chunks) and nothing is ever evicted again (eviction only serves a
 * request). With the Expansion Pak the whole pack fits after startup. */
bool scene_assets_all_resident(void) { return resident==SCENE_CHUNK_COUNT; }
static void require_chunk(unsigned chunk) {
    if (chunk_slot[chunk]>=0 && slots[chunk_slot[chunk]].ready) { stats.hits++;return; }
    stats.misses++;request(chunk,255);
    uint32_t started=TICKS_READ();timing_depth++;
    while (chunk_slot[chunk]<0 || !slots[chunk_slot[chunk]].ready) {
        scene_service();
        if (active_chunk>=0 || pi_busy()) dma_wait();
        else {
            rspq_wait();scene_service();
            assertf(active_chunk>=0 || (chunk_slot[chunk]>=0 && slots[chunk_slot[chunk]].ready),"Scene working set exceeds cache");
        }
    }
    if (!--timing_depth) msx_perf_add(MSX_PERF_ASSET,TICKS_READ()-started);
}
const void *scene_page(const scene_image_t *image) {
    assertf(image->page<SCENE_PAGE_COUNT,"Scene page ID");
    unsigned chunk=image->page/8;require_chunk(chunk);
    if (recording>=0) block_pins[chunk]|=(uint8_t)(1u<<recording);
    cache_slot_t *s=&slots[chunk_slot[chunk]];s->used_epoch=epoch;s->lru=++clock_lru;
    return cache+(unsigned)chunk_slot[chunk]*SCENE_CHUNK_BYTES+(image->page%8)*SCENE_PAGE_BYTES;
}
static bool resident_chunk(unsigned chunk) { return chunk_slot[chunk]>=0 && slots[chunk_slot[chunk]].ready; }
const void *scene_page_try(const scene_image_t *image) {
    assertf(image->page<SCENE_PAGE_COUNT,"Scene page ID");
    unsigned chunk=image->page/8;
    if (!resident_chunk(chunk)) {
        if (frame_sync_decodes+frame_service_decodes>=SCENE_SYNC_DECODES) { stats.deferred++;request(chunk,255);return NULL; }
        frame_sync_decodes++;
    }
    return scene_page(image);
}
void scene_prefetch_boss(void) {
    static const uint16_t boss_chunks[]=SCENE_BOSS_CHUNK_LIST;
    if (capacity<SCENE_CHUNK_COUNT/4) return;   // 4 MiB: the boss set does not fit (MISION1_COMPLETA.md)
    for (unsigned i=0;i<sizeof(boss_chunks)/sizeof(boss_chunks[0]);i++) request(boss_chunks[i],1);
}
enum { SCENE_HEAP_RESERVE=1536*1024 };
void scene_assets_init(void) {
    rom=PhysicalAddr((void*)(uintptr_t)dfs_rom_addr("/scene.pack"));
    assertf(rom && dfs_rom_size("/scene.pack")==SCENE_PACK_BYTES && !(rom&7),"Scene ROM pack");
    // Expansion Pak: the whole Mission 1 pack resident (300 chunks, 4.7 MiB,
    // with the rest-of-mission actors) when it fits, else 3.5 MiB.
    capacity=(get_memory_size()>=8*1024*1024?SCENE_CHUNK_COUNT*SCENE_CHUNK_BYTES:512*1024)/SCENE_CHUNK_BYTES;
    if (capacity>SCENE_CHUNK_COUNT) capacity=SCENE_CHUNK_COUNT;
    /* The heap left after the cache must still hold the display buffers,
     * Marco's poses and the audio (allocated later): keep a reserve, the
     * rest of the pack streams through the LRU slots. */
    {heap_stats_t h;sys_get_heap_stats(&h);
     int room=h.total-h.used-SCENE_HEAP_RESERVE;
     unsigned fit=room>0?(unsigned)room/SCENE_CHUNK_BYTES:0;
     if (get_memory_size()>=8*1024*1024 && fit<capacity) capacity=fit>(7u*512u*1024u/SCENE_CHUNK_BYTES)?fit:7u*512u*1024u/SCENE_CHUNK_BYTES;}
    cache=memalign(64,capacity*SCENE_CHUNK_BYTES);
    if (!cache && get_memory_size()>=8*1024*1024) {capacity=7*512*1024/SCENE_CHUNK_BYTES;cache=memalign(64,capacity*SCENE_CHUNK_BYTES);}
    slots=calloc(capacity,sizeof(*slots));
    staging[0]=memalign(64,SCENE_CHUNK_BYTES);staging[1]=memalign(64,SCENE_CHUNK_BYTES);
    assertf(cache && slots && staging[0] && staging[1],"Scene cache allocation");
    for (unsigned i=0;i<capacity;i++) slots[i].chunk=-1;
    for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++) chunk_slot[i]=-1;
    stats.cache_bytes=capacity*SCENE_CHUNK_BYTES;
    data_cache_hit_writeback((void*)scene_palettes,sizeof(scene_palettes));
    // Warm the present zone outside gameplay. Marco's branch transitions stay pinned.
    for (unsigned bank=0;bank<SCENE_BANK_COUNT;bank++) if (bank!=SCENE_SOLDIER && bank!=SCENE_NATIVE) {
        const scene_bank_t *b=&scene_banks[bank];
        for (unsigned i=0;i<b->first+b->second;i++) pinned[scene_images[b->start+i].page/8]=true;
    }
    // With 4 MiB the extended map (camera 0..1920) no longer fits completely:
    // warm the opening view last, so LRU eviction keeps it resident, and stream
    // the rest by prefetch. tools/check_scene_budget.py bounds the working set.
    static bool opening[SCENE_CHUNK_COUNT];
    for (unsigned i=0;i<SCENE_MAP_CELL_COUNT;i++) {
        const scene_map_cell_t *cell=&scene_map_cells_data[i];unsigned layer=cell->layer<4?cell->layer:3;
        int x=cell->x-msx_layer_scroll(layer,0);
        if (!msx_layer_visible(layer,0) || x< -48 || x>=352) continue;
        for (unsigned phase=0;phase<8;phase++) opening[scene_map_images[cell->phases[phase]].page/8]=true;
    }
    for (unsigned pass=0;pass<2;pass++) for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++)
        if (opening[i]==(pass==1)) { request(i,1);require_chunk(i); }
    {heap_stats_t h;sys_get_heap_stats(&h);
     debugf("MSX pack: %u pages, %u palettes, %u chunks; cache=%u KB (%u chunks) DMA=%u bytes startup evictions=%u heap free after cache=%d KB\n",SCENE_PAGE_COUNT,SCENE_PALETTE_COUNT,SCENE_CHUNK_COUNT,stats.cache_bytes/1024,capacity,stats.dma_bytes,stats.evictions,(h.total-h.used)/1024);}
    stats.misses=stats.evictions=stats.deferred=stats.held=0; // Startup waits/evictions are outside the gameplay deadline.
}
const scene_bank_t *scene_bank(unsigned id) { assert(id<SCENE_BANK_COUNT);return &scene_banks[id]; }
const scene_image_t *scene_frame(unsigned id,unsigned frame) {
    const scene_bank_t *b=scene_bank(id);assert(frame<b->first+b->second);return &scene_images[b->start+frame];
}
const scene_image_t *scene_background_tile(unsigned row,unsigned col) { assert(row<7 && col<10);return &scene_background[row*10+col]; }
const scene_map_cell_t *scene_map_cells(unsigned *count) { *count=SCENE_MAP_CELL_COUNT;return scene_map_cells_data; }
const scene_map_cell_t *scene_patch_cells(unsigned *count) { *count=SCENE_PATCH_CELL_COUNT;return scene_patch_cells_data; }
const scene_map_cell_t *scene_collapse_cells(unsigned *count) { *count=SCENE_COLLAPSE_CELL_COUNT;return scene_collapse_cells_data; }
const scene_image_t *scene_map_image(unsigned id) { assert(id<sizeof(scene_map_images)/sizeof(*scene_map_images));return &scene_map_images[id]; }
unsigned scene_map_camera_max(void) { return SCENE_MAP_CAMERA_MAX; }
uint32_t scene_map_backdrop(bool after) { return after?SCENE_MAP_BACKDROP_AFTER:SCENE_MAP_BACKDROP; }
const scene_native_descriptor_t *scene_native_descriptor(uint32_t tag) {
    unsigned lo=0,hi=sizeof(scene_native_descriptors)/sizeof(scene_native_descriptors[0]);
    while (lo<hi) {
        unsigned mid=(lo+hi)/2;
        if (scene_native_descriptors[mid].tag==tag) return &scene_native_descriptors[mid];
        if (scene_native_descriptors[mid].tag<tag) lo=mid+1; else hi=mid;
    }
    return NULL;
}
int scene_native_palette(unsigned resource) {
    for (unsigned i=0;i<sizeof(scene_native_palettes)/sizeof(scene_native_palettes[0]);i++)
        if (scene_native_palettes[i].resource==resource) return scene_native_palettes[i].palette;
    return -1;
}
const uint16_t *scene_palette(unsigned id) { assert(id<SCENE_PALETTE_COUNT);return scene_palettes[id]; }
void scene_record_pins(int group) { recording=group; }
void scene_unpin_group(int group) { for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++) block_pins[i]&=(uint8_t)~(1u<<group); }
unsigned scene_completed_epoch(void) { return completed; }
void scene_begin_epoch(unsigned next) { epoch=next;frame_service_decodes=frame_sync_decodes=0; }
void scene_complete_epoch(unsigned done) { completed=done; }
scene_cache_stats_t scene_cache_stats(void) { return stats; }
