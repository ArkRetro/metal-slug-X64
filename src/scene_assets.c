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
/* Measured RAM budget (KB), see "RAM budget" below. */
enum {
    SCENE_RESERVE_LOGO_KB=224,     // boot logo peak, freed before the game (measured 169)
    SCENE_RESERVE_GAME_KB=640,     // actor_init after the cache (measured 560)
    SCENE_RESERVE_RUNTIME_KB=320,  // gameplay growth (measured peak in the docs)
    SCENE_HEAP_MARGIN_KB=256,
    /* Expansion Pak floor: the pinned banks and map (49 chunks), the boss
     * set (65) and three soldier programs (83) with some room; below it
     * something else took the RAM (the cache is sized last, from the free
     * heap left after the audio, so whatever loads before shrinks it). */
    SCENE_EXPANDED_MIN_SLOTS=208,
    /* Slots left for the streamed actors once the map is pinned. */
    SCENE_EXPANDED_STREAM_SLOTS=128,
    SCENE_IDLE_MARGIN_US=1200,     // idle decodes end this long before the next blank
    SCENE_AHEAD_PIXELS=384,        // script lookahead distance (camera pixels)
};

typedef struct { int chunk; bool ready; unsigned used_epoch,lru; } cache_slot_t;
static cache_slot_t *slots;
static int chunk_slot[SCENE_CHUNK_COUNT];
static unsigned requests[SCENE_CHUNK_COUNT];
static bool pinned[SCENE_CHUNK_COUNT],map_chunk[SCENE_CHUNK_COUNT];
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
/* Warm list (scene_assets_begin): the chunks loaded before gameplay, in
 * order: pinned player banks, the whole map (pinned with the Expansion Pak),
 * then the script's first spawns. Taken only into free slots, after every
 * request, so warming never evicts. `warm_required` leading entries must be
 * resident before the first frame (scene_assets_init finishes them). */
static uint16_t warm_list[SCENE_CHUNK_COUNT];
static unsigned warm_count,warm_next,warm_required;
static bool warming,map_resident_all;
static unsigned map_chunks,ahead_cursor;
static int last_camera=-1;
// PI status bits 0/1: DMA and IO busy. A nonblocking poll keeps prefetch asynchronous.
static bool pi_busy(void) { return (*PI_STATUS & 3u)!=0; }
/* Idle-time decoding (scene_assets_idle): the VI interrupt stamps every
 * vertical blank; a decode starts only if its estimated cost (the slowest
 * recent decode) ends before the next blank minus a margin, so the frame
 * that waits for that blank is never delayed. */
static volatile uint32_t vblank_ticks,vblank_period;
static uint32_t decode_estimate;
static void on_vblank(void) {
    uint32_t now=TICKS_READ(),d=now-vblank_ticks;
    if (vblank_ticks && d>TICKS_FROM_US(15000) && d<TICKS_FROM_US(18000)) vblank_period=d;
    vblank_ticks=now;
}
static uint32_t idle_left(void) {
    uint32_t v=vblank_ticks,period=vblank_period?vblank_period:TICKS_FROM_US(16683);
    if (!v) return 0;
    uint32_t since=TICKS_READ()-v,margin=TICKS_FROM_US(SCENE_IDLE_MARGIN_US);
    if (since+margin>=period) return 0;
    return period-since-margin;
}

static int warm_candidate(void) {
    while (warm_next<warm_count) {
        unsigned c=warm_list[warm_next];
        if (chunk_slot[c]<0) return (int)c;
        warm_next++;
    }
    return -1;
}
static void start_next(void) {
    if (active_chunk>=0 || pi_busy()) return;
    int chosen=-1;bool warm=false;
    for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++)
        if (requests[i] && (chosen<0 || requests[i]>requests[chosen])) chosen=(int)i;
    if (chosen<0) { chosen=warm_candidate();warm=true; }
    if (chosen<0) return;
    int victim=-1;
    for (unsigned i=0;i<capacity;i++) {
        if (slots[i].chunk<0) { victim=(int)i;break; }
        if (warm) continue;   // warming only fills free slots
        if (!slots[i].ready || pinned[slots[i].chunk] || block_pins[slots[i].chunk] || slots[i].used_epoch>completed) continue;
        if (victim<0 || slots[i].lru<slots[victim].lru) victim=(int)i;
    }
    if (victim<0) { if (warm) warm_count=warm_next; return; }   // cache full: stop warming
    if (warm) warm_next++;
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
/* Decode the staged chunk (its DMA done) into its slot; returns the ticks. */
static uint32_t decode_active(void) {
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
    uint32_t spent=TICKS_READ()-started;
    // Slowest recent decode, decaying 1/16 per decode.
    decode_estimate-=decode_estimate/16;if (spent>decode_estimate) decode_estimate=spent;
    if (spent>stats.decode_max_ticks) stats.decode_max_ticks=spent;
    if (warming) stats.warm_decodes++;
    return spent;
}
void scene_service(void) {
    if (!cache) return;
    if (active_chunk<0) { start_next();return; }
    if (pi_busy()) return;
    // Prefetch spends at most SCENE_SERVICE_DECODES decodes per frame; the
    // chunk stays in its staging buffer for the next frame. A synchronous
    // wait (require_chunk) is never held back.
    // The budget is shared with the synchronous decodes of deferrable parts.
    if (!timing_depth && frame_service_decodes+frame_sync_decodes>=SCENE_SERVICE_DECODES) { stats.held++;return; }
    if (!timing_depth) frame_service_decodes++;
    uint32_t spent=decode_active();
    if (!timing_depth) msx_perf_add(MSX_PERF_ASSET,spent);
}
bool scene_assets_idle(void) {
    if (!cache) return false;
    if (active_chunk<0) start_next();
    if (active_chunk<0) return false;          // nothing requested or warming
    uint32_t left=idle_left();
    if (left<decode_estimate) return false;    // would run into the next blank
    if (pi_busy()) return true;                // DMA in flight: poll again
    decode_active();stats.idle_decodes++;
    return true;
}
static void request(unsigned chunk,unsigned priority) {
    assertf(chunk<SCENE_CHUNK_COUNT,"Scene chunk ID");
    if (chunk_slot[chunk]>=0) return;
    if (requests[chunk]<priority) requests[chunk]=priority;
}
void scene_request_image(const scene_image_t *image,unsigned priority) { request(image->page/8,priority); }
/* Every chunk decoded and resident: requests are no-ops (request() returns for
 * resident chunks) and nothing is ever evicted again (eviction only serves a
 * request). The pack (711 chunks, 11.1 MiB decoded) does not fit the 8 MiB
 * console: this stays false; scene_assets_map_resident is what holds. */
bool scene_assets_all_resident(void) { return resident==SCENE_CHUNK_COUNT; }
static bool resident_chunk(unsigned chunk) { return chunk_slot[chunk]>=0 && slots[chunk_slot[chunk]].ready; }
/* Every map chunk pinned and resident (Expansion Pak, after the warm-up):
 * the background never misses and its prefetch walk is skipped. */
bool scene_assets_map_resident(void) { return map_resident_all && !warming; }
static void require_chunk(unsigned chunk,bool deferrable) {
    if (resident_chunk(chunk)) { stats.hits++;return; }
    stats.misses++;
    if (!deferrable && !warming) {
        stats.sync_misses++;
        if (stats.sync_misses<=32) debugf("MSX SCENE sync miss chunk=%u map=%u camera=%d frame=%u\n",chunk,map_chunk[chunk],last_camera,epoch);
    }
    request(chunk,255);
    uint32_t started=TICKS_READ();timing_depth++;
    while (!resident_chunk(chunk)) {
        scene_service();
        if (active_chunk>=0 || pi_busy()) dma_wait();
        else {
            rspq_wait();scene_service();
            assertf(active_chunk>=0 || resident_chunk(chunk),"Scene working set exceeds cache");
        }
    }
    if (!--timing_depth && !warming) msx_perf_add(MSX_PERF_ASSET,TICKS_READ()-started);
}
static const void *page_of(const scene_image_t *image,bool deferrable) {
    assertf(image->page<SCENE_PAGE_COUNT,"Scene page ID");
    unsigned chunk=image->page/8;require_chunk(chunk,deferrable);
    if (recording>=0) block_pins[chunk]|=(uint8_t)(1u<<recording);
    cache_slot_t *s=&slots[chunk_slot[chunk]];s->used_epoch=epoch;s->lru=++clock_lru;
    return cache+(unsigned)chunk_slot[chunk]*SCENE_CHUNK_BYTES+(image->page%8)*SCENE_PAGE_BYTES;
}
const void *scene_page(const scene_image_t *image) { return page_of(image,false); }
const void *scene_page_try(const scene_image_t *image) {
    assertf(image->page<SCENE_PAGE_COUNT,"Scene page ID");
    unsigned chunk=image->page/8;
    if (!resident_chunk(chunk)) {
        if (frame_sync_decodes+frame_service_decodes>=SCENE_SYNC_DECODES) { stats.deferred++;request(chunk,255);return NULL; }
        frame_sync_decodes++;
    }
    return page_of(image,true);
}
void scene_prefetch_boss(void) {
    static const uint16_t boss_chunks[]=SCENE_BOSS_CHUNK_LIST;
    if (capacity<SCENE_CHUNK_COUNT/4) return;   // 4 MiB: the boss set does not fit (MISION1_COMPLETA.md)
    for (unsigned i=0;i<sizeof(boss_chunks)/sizeof(boss_chunks[0]);i++) request(boss_chunks[i],1);
}
void scene_prefetch_ahead(int camera) {
    last_camera=camera;
    if (capacity<SCENE_CHUNK_COUNT/4 || camera<0) return;   // 4 MiB: no room ahead of the working set
    /* The camera only moves forward in a mission; after a restart it goes
     * back to 0: rewind the cursor. */
    if (ahead_cursor && scene_ahead[ahead_cursor-1][0]>=(unsigned)camera) ahead_cursor=0;
    while (ahead_cursor<SCENE_AHEAD_COUNT && scene_ahead[ahead_cursor][0]<(unsigned)camera) ahead_cursor++;
    for (unsigned i=ahead_cursor;i<SCENE_AHEAD_COUNT && scene_ahead[i][0]<=(unsigned)camera+SCENE_AHEAD_PIXELS;i++) {
        unsigned chunk=scene_ahead[i][1];
        // Resident: keep it the most recently used, so the LRU keeps it until the spawn.
        if (resident_chunk(chunk)) slots[chunk_slot[chunk]].lru=++clock_lru;
        else request(chunk,1);
    }
}

/* ---- RAM budget (Expansion Pak, docs/RENDIMIENTO.md "Presupuesto de RAM") ----
 * The scene cache takes what the heap has left once everything else is
 * accounted for. Allocated before the cache (already in `used`): display
 * buffers, mixer and the game audio. Allocated after it, measured on ares
 * with every private asset (log lines "MSX MEM"):
 *  - the boot logo (freed before the game): z-buffer, mesh, jingle stream;
 *  - actor_init: Marco's poses, the HUD's S-ROM copy and strip, the
 *    renderer's map index and font;
 *  - gameplay growth: recorded background blocks (rspq), vehicle sprites,
 *    libdragon's transient buffers;
 * plus a margin for the world pools of later branches and fragmentation.
 * scene_assets_check_budget asserts the measured free heap once the game is
 * loaded; the "MSX RDP" line reports the lowest free heap seen. */
enum {
    SCENE_RESERVE_LOGO=SCENE_RESERVE_LOGO_KB*1024,
    SCENE_RESERVE_GAME=SCENE_RESERVE_GAME_KB*1024,
    SCENE_RESERVE_RUNTIME=SCENE_RESERVE_RUNTIME_KB*1024,
    SCENE_HEAP_MARGIN=SCENE_HEAP_MARGIN_KB*1024,
    SCENE_HEAP_RESERVE=(SCENE_RESERVE_LOGO>SCENE_RESERVE_GAME+SCENE_RESERVE_RUNTIME?SCENE_RESERVE_LOGO:SCENE_RESERVE_GAME+SCENE_RESERVE_RUNTIME)+SCENE_HEAP_MARGIN,
};
static int heap_free(void) { heap_stats_t h;sys_get_heap_stats(&h);return h.total-h.used; }
static int heap_free_min=0x7fffffff;
void scene_assets_mem(const char *stage) {
    heap_stats_t h;sys_get_heap_stats(&h);
    debugf("MSX MEM %s: heap used=%d KB free=%d KB of %d KB (cache=%u KB) t=%lu ms\n",stage,h.used/1024,(h.total-h.used)/1024,h.total/1024,stats.cache_bytes/1024,(unsigned long)get_ticks_ms());
}
void scene_assets_check_budget(void) {
    if (!cache || get_memory_size()<8*1024*1024) return;
    int free_now=heap_free();
    scene_assets_mem("game loaded");
    // What the game still allocates while it runs must fit what is left.
    assertf(free_now>=SCENE_RESERVE_RUNTIME+SCENE_HEAP_MARGIN/2,"RAM budget: %d KB free after loading, %d KB reserved for gameplay (scene_assets.c)",free_now/1024,(SCENE_RESERVE_RUNTIME+SCENE_HEAP_MARGIN/2)/1024);
}
int scene_assets_heap_low(void) {
    int f=heap_free();if (f<heap_free_min) heap_free_min=f;
    return heap_free_min;
}

static void warm_add(unsigned chunk) {
    static bool queued[SCENE_CHUNK_COUNT];
    if (queued[chunk]) return;
    queued[chunk]=true;warm_list[warm_count++]=(uint16_t)chunk;
}
static void mark_map(const scene_map_cell_t *cells,unsigned count) {
    for (unsigned i=0;i<count;i++) for (unsigned phase=0;phase<8;phase++) {
        unsigned c=scene_map_images[cells[i].phases[phase]].page/8;
        if (!map_chunk[c]) {map_chunk[c]=true;map_chunks++;}
    }
}
void scene_assets_begin(void) {
    if (cache) return;
    uint32_t began=TICKS_READ();
    rom=PhysicalAddr((void*)(uintptr_t)dfs_rom_addr("/scene.pack"));
    assertf(rom && dfs_rom_size("/scene.pack")==SCENE_PACK_BYTES && !(rom&7),"Scene ROM pack");
    bool expanded=get_memory_size()>=8*1024*1024;
    /* 4 MiB: 32 slots (512 KiB), the map streams through them. Expansion
     * Pak: every byte the RAM budget leaves (SCENE_HEAP_RESERVE above). */
    capacity=512*1024/SCENE_CHUNK_BYTES;
    staging[0]=memalign(64,SCENE_CHUNK_BYTES);staging[1]=memalign(64,SCENE_CHUNK_BYTES);
    if (expanded) {
        int room=heap_free()-SCENE_HEAP_RESERVE-(int)(sizeof(cache_slot_t)*SCENE_CHUNK_COUNT)-64;
        assertf(room>=(int)(SCENE_EXPANDED_MIN_SLOTS*SCENE_CHUNK_BYTES),"RAM budget: %d KB left for the scene cache (minimum %u KB)",room/1024,SCENE_EXPANDED_MIN_SLOTS*SCENE_CHUNK_BYTES/1024);
        capacity=(unsigned)room/SCENE_CHUNK_BYTES;
        if (capacity>SCENE_CHUNK_COUNT) capacity=SCENE_CHUNK_COUNT;
    }
    cache=memalign(64,capacity*SCENE_CHUNK_BYTES);
    slots=calloc(capacity,sizeof(*slots));
    assertf(cache && slots && staging[0] && staging[1],"Scene cache allocation");
    for (unsigned i=0;i<capacity;i++) slots[i].chunk=-1;
    for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++) chunk_slot[i]=-1;
    stats.cache_bytes=capacity*SCENE_CHUNK_BYTES;
    decode_estimate=TICKS_FROM_US(4000);
    register_VI_handler(on_vblank);
    data_cache_hit_writeback((void*)scene_palettes,sizeof(scene_palettes));
    // Marco's branch transitions stay pinned (all banks but the streamed ones).
    for (unsigned bank=0;bank<SCENE_BANK_COUNT;bank++) if (bank!=SCENE_SOLDIER && bank!=SCENE_NATIVE) {
        const scene_bank_t *b=&scene_banks[bank];
        for (unsigned i=0;i<b->first+b->second;i++) {unsigned c=scene_images[b->start+i].page/8;pinned[c]=true;warm_add(c);}
    }
    mark_map(scene_map_cells_data,SCENE_MAP_CELL_COUNT);
    mark_map(scene_patch_cells_data,SCENE_PATCH_CELL_COUNT);
    mark_map(scene_collapse_cells_data,SCENE_COLLAPSE_CELL_COUNT);
    if (expanded && capacity>=warm_count+map_chunks+SCENE_EXPANDED_STREAM_SLOTS) {
        /* Expansion Pak: the whole map (every cell, the shop patch and the
         * rubble) is pinned and loaded before the first frame, in camera
         * order: the background never misses. */
        map_resident_all=true;
        static int first_camera[SCENE_CHUNK_COUNT];
        for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) first_camera[c]=0x7fffffff;
        for (unsigned i=0;i<SCENE_MAP_CELL_COUNT;i++) {
            const scene_map_cell_t *cell=&scene_map_cells_data[i];unsigned layer=cell->layer<4?cell->layer:3;
            // First camera that brings the cell within 416 px (inverse of msx_layer_scroll).
            int ahead=cell->x-416,camera=ahead<=0?0:layer==0?ahead*2:layer==1?ahead*4/3:ahead;
            if (layer==2 && camera<MSX_MID_FULL_SPEED_CAMERA) camera=MSX_MID_FULL_SPEED_CAMERA;
            for (unsigned phase=0;phase<8;phase++) {
                unsigned c=scene_map_images[cell->phases[phase]].page/8;
                if (camera<first_camera[c]) first_camera[c]=camera;
            }
        }
        for (;;) {   // 37 chunks: selection by first camera
            int best=-1;
            for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) if (map_chunk[c] && !pinned[c] && (best<0 || first_camera[c]<first_camera[best])) best=(int)c;
            if (best<0) break;
            pinned[best]=true;warm_add((unsigned)best);
        }
        warm_required=warm_count;
        // Then the actors of the script's spawns in camera order, into the
        // free slots only (the lookahead keeps them; the LRU recycles them).
        for (unsigned i=0;i<SCENE_AHEAD_COUNT;i++) warm_add(scene_ahead[i][1]);
    } else {
        /* 4 MiB: the opening view only (the extended map does not fit);
         * prefetch streams the rest. tools/check_scene_budget.py bounds the
         * working set. */
        for (unsigned i=0;i<SCENE_MAP_CELL_COUNT;i++) {
            const scene_map_cell_t *cell=&scene_map_cells_data[i];unsigned layer=cell->layer<4?cell->layer:3;
            int x=cell->x-msx_layer_scroll(layer,0);
            if (!msx_layer_visible(layer,0) || x< -48 || x>=352) continue;
            for (unsigned phase=0;phase<8;phase++) warm_add(scene_map_images[cell->phases[phase]].page/8);
        }
        warm_required=warm_count;
    }
    warming=true;
    debugf("MSX pack: %u pages, %u palettes, %u chunks (%u map); cache=%u KB (%u slots) reserve=%u KB warm=%u (required %u) setup=%lu us\n",
        SCENE_PAGE_COUNT,SCENE_PALETTE_COUNT,SCENE_CHUNK_COUNT,map_chunks,stats.cache_bytes/1024,capacity,(unsigned)SCENE_HEAP_RESERVE/1024,
        warm_count,warm_required,(unsigned long)TICKS_TO_US(TICKS_READ()-began));
    scene_assets_mem("scene cache");
}
void scene_assets_init(void) {
    scene_assets_begin();
    /* The warm-up started during the boot logo (scene_assets_idle); what it
     * did not reach of the required part is loaded now, synchronously. */
    uint32_t began=TICKS_READ();unsigned before=stats.warm_decodes;
    for (unsigned i=0;i<warm_required;i++) require_chunk(warm_list[i],false);
    if (warm_next<warm_required) warm_next=warm_required;
    warming=false;
    debugf("MSX pack warm: %u/%u chunks resident before the first frame (%u in the logo's idle time, %u now in %lu us); map resident=%u startup evictions=%u\n",
        resident,warm_count,before,stats.warm_decodes-before,(unsigned long)TICKS_TO_US(TICKS_READ()-began),scene_assets_map_resident(),stats.evictions);
    // Startup waits/evictions are outside the gameplay deadline.
    stats.misses=stats.sync_misses=stats.evictions=stats.deferred=stats.held=stats.idle_decodes=0;stats.decode_max_ticks=0;
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
