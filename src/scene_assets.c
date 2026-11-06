#include <libdragon.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "scene_assets.h"
#include "scene_prerender.h"
#ifndef MSX_BG_CRC
#define MSX_BG_CRC 0
#endif
#include "scene_codec.h"
#include "scene_meta.h"
#include "mission_camera.h"
#include "msx_perf.h"
#include "msx_diag.h"
#include "scene_cull.h"
#include <stdio.h>
enum { SCENE_MISSION_PAGE_BASE=SCENE_CHUNK_COUNT*8 };
_Static_assert(SCENE_MISSION_PAGE_BASE+SCENE_MISSION_CHUNKS_MAX*8<=0xffff,"Mission page ids are 16-bit");
_Static_assert(SCENE_MAP_CAMERA_MAX==MSX_CAMERA_MAX,"Scene pack and gameplay camera limits differ (filesystem/mission1/rest/map.json missing?)");
/* Measured RAM budget (KB), see "RAM budget" below. */
enum {
    SCENE_RESERVE_LOGO_KB=224,     // boot logo peak, freed before the game (measured 169)
    /* actor_init's whole heap, part of it preloaded before the cache
     * (scene_assets_game_preloaded): Marco's parts and fixed.bin 559, the
     * renderer 28, the HUD strip 67 (304x224 CI8; 133 as RGBA16) = 654.
     * At 640 with the RGBA16 strip (719 in 1afa21a) the game had 510 KB free
     * once loaded instead of 576, and 234 KB at the end of the Mission 1
     * boss. The "MSX actor_init" line gives the parts. */
    SCENE_RESERVE_GAME_KB=672,
    SCENE_RESERVE_RUNTIME_KB=320,  // gameplay growth (measured 263 in Mission 1, mostly background blocks)
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

/* Chunk ids: scene.pack's, then (SCENE_CHUNK_COUNT..) the loaded mission
 * pack's (scene_mission_load); a mission page is SCENE_CHUNK_COUNT*8+page. */
enum { CHUNKS_ALL=SCENE_CHUNK_COUNT+SCENE_MISSION_CHUNKS_MAX };
typedef struct { int chunk; bool ready,reserved; unsigned used_epoch,lru; } cache_slot_t;
static cache_slot_t *slots;
static int chunk_slot[CHUNKS_ALL];
static unsigned requests[CHUNKS_ALL];
static bool pinned[CHUNKS_ALL],map_chunk[CHUNKS_ALL];
static uint8_t block_pins[CHUNKS_ALL];   // bit per recorded-block group
static int recording=-1;
static uint8_t *cache,*staging[2];
static uint8_t **slot_mem;   /* each slot's 16 KiB (the cache can span several heap blocks) */
static unsigned capacity,clock_lru,epoch,rom,resident;
static unsigned capacity_final;   // slots the cache gets; more than `capacity` while begun early (scene_assets_begin_early)
static volatile unsigned completed;
static int active_chunk=-1,active_slot=-1;
static bool active_warm;   // the staged chunk comes from the warm list, not a request
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
static uint16_t warm_list[CHUNKS_ALL];
static unsigned warm_count,warm_next,warm_required;
static bool warming,map_resident_all;
static unsigned map_chunks,ahead_cursor;
static int last_camera=-1;
/* Loaded mission pack (scene_mission_load): its chunk table and ROM base. */
static const scene_chunk_t *mission_chunks;
static unsigned mission_chunk_count;
static uint32_t mission_rom,mission_bytes;
static const scene_chunk_t *chunk_info(unsigned c,uint32_t *base,uint32_t *size) {
    if (c<SCENE_CHUNK_COUNT) {*base=rom;*size=SCENE_PACK_BYTES;return &scene_chunks[c];}
    assertf(c-SCENE_CHUNK_COUNT<mission_chunk_count,"Mission chunk ID");
    *base=mission_rom;*size=mission_bytes;return &mission_chunks[c-SCENE_CHUNK_COUNT];
}
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
    for (unsigned i=0;i<CHUNKS_ALL;i++)
        if (requests[i] && (chosen<0 || requests[i]>requests[chosen])) chosen=(int)i;
    if (chosen<0) { chosen=warm_candidate();warm=true; }
    if (chosen<0) return;
    int victim=-1;
    for (unsigned i=0;i<capacity;i++) {
        if (slots[i].reserved) continue;   // stage tables (scene_stage_load)
        if (slots[i].chunk<0) { victim=(int)i;break; }
        if (warm) continue;   // warming only fills free slots
        if (!slots[i].ready || pinned[slots[i].chunk] || block_pins[slots[i].chunk] || slots[i].used_epoch>completed) continue;
        if (victim<0 || slots[i].lru<slots[victim].lru) victim=(int)i;
    }
    if (victim<0) { if (warm && capacity>=capacity_final) warm_count=warm_next; return; }   // cache full: stop warming (not while begun early)
    if (warm) warm_next++;
    if (slots[victim].chunk>=0) { chunk_slot[slots[victim].chunk]=-1;stats.evictions++;resident--; }
    slots[victim]=(cache_slot_t){.chunk=chosen};chunk_slot[chosen]=victim;requests[chosen]=0;
    active_chunk=chosen;active_slot=victim;active_stage=last_stage^1;last_stage=active_stage;active_warm=warm;
    uint32_t base,size;const scene_chunk_t *c=chunk_info((unsigned)chosen,&base,&size);
    unsigned length=(c->length+63)&~63u;
    assertf(length<=SCENE_CHUNK_BYTES && c->offset+length<=size,"Pack chunk bounds");
    data_cache_hit_invalidate(staging[active_stage],length);
    dma_read_async(staging[active_stage],base+c->offset,length);
    stats.loads++;stats.dma_bytes+=length;
}
/* Decode the staged chunk (its DMA done) into its slot; returns the ticks. */
static uint32_t decode_active(void) {
    uint32_t started=TICKS_READ();
    int chunk=active_chunk,slot=active_slot;unsigned stage=active_stage;
    active_chunk=active_slot=-1;
    // Reserve the other staging buffer and overlap its PI read with this decode.
    start_next();
    uint32_t base,size;const scene_chunk_t *c=chunk_info((unsigned)chunk,&base,&size);uint8_t *dst=slot_mem[slot];(void)size;
    bool decoded=scene_decode(dst,SCENE_CHUNK_BYTES,staging[stage],c->length,c->codec);
    assertf(decoded,"Bad scene codec chunk=%d codec=%lu length=%lu stage=%u first=%02x/%02x rom=%08x",chunk,(unsigned long)c->codec,(unsigned long)c->length,stage,staging[stage][0],staging[stage][1],(unsigned)(base+c->offset));
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
    /* A warm-list chunk (nobody asked for it yet) is decoded in the spare
     * time before the blank (scene_assets_idle), not in the frame's own
     * time: after the boot cinematics ~300 of them were each a 5 ms decode
     * in the first 70 gameplay frames, a third of them shown late. A
     * synchronous wait or a pending request still takes it now. */
    if (active_warm && !timing_depth) {
        bool asked=false;
        for (unsigned i=0;i<CHUNKS_ALL && !asked;i++) asked=requests[i]!=0;
        if (!asked) return;
    }
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
    assertf(chunk<SCENE_CHUNK_COUNT+mission_chunk_count,"Scene chunk ID");
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
    assertf(image->page<SCENE_PAGE_COUNT || (image->page>=SCENE_MISSION_PAGE_BASE && image->page/8u<SCENE_CHUNK_COUNT+mission_chunk_count),"Scene page ID");
    unsigned chunk=image->page/8;require_chunk(chunk,deferrable);
    if (recording>=0) block_pins[chunk]|=(uint8_t)(1u<<recording);
    cache_slot_t *s=&slots[chunk_slot[chunk]];s->used_epoch=epoch;s->lru=++clock_lru;
    return slot_mem[chunk_slot[chunk]]+(image->page%8)*SCENE_PAGE_BYTES;
}
const void *scene_page(const scene_image_t *image) { return page_of(image,false); }
const void *scene_page_try(const scene_image_t *image) {
    assertf(image->page<SCENE_PAGE_COUNT || image->page>=SCENE_MISSION_PAGE_BASE,"Scene page ID");
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
/* Part of actor_init's allocations made before the cache (Marco's parts and
 * the HUD's S-ROM copy, read in the boot cinematics' spare time): no longer
 * reserved. */
static int game_preloaded;
void scene_assets_game_preloaded(unsigned bytes) { game_preloaded=bytes>SCENE_RESERVE_GAME?SCENE_RESERVE_GAME:(int)bytes; }
static int heap_reserve(void) {
    int game=SCENE_RESERVE_GAME-game_preloaded+SCENE_RESERVE_RUNTIME;
    return (SCENE_RESERVE_LOGO>game?SCENE_RESERVE_LOGO:game)+SCENE_HEAP_MARGIN;
}
static int heap_free_min=0x7fffffff;
void scene_assets_mem(const char *stage) {
    heap_stats_t h;sys_get_heap_stats(&h);
    msx_diag_heap(stage);
    debugf("MSX MEM %s: heap used=%d KB free=%d KB of %d KB (cache=%u KB) t=%lu ms\n",stage,h.used/1024,(h.total-h.used)/1024,h.total/1024,stats.cache_bytes/1024,(unsigned long)get_ticks_ms());
}
void scene_assets_check_budget(void) {
    if (!cache || get_memory_size()<8*1024*1024) return;
    int free_now=heap_free();
    scene_assets_mem("game loaded");
    debugf("MSX CACHE slots=%u (%u KB, floor %u) free=%d KB (budget %d)\n",capacity,capacity*SCENE_CHUNK_BYTES/1024,SCENE_EXPANDED_MIN_SLOTS,free_now/1024,(SCENE_RESERVE_RUNTIME+SCENE_HEAP_MARGIN)/1024);
    // What the game still allocates while it runs must fit what is left.
    assertf(free_now>=SCENE_RESERVE_RUNTIME+SCENE_HEAP_MARGIN/2,"RAM budget: %d KB free after loading, %d KB reserved for gameplay (scene_assets.c)",free_now/1024,(SCENE_RESERVE_RUNTIME+SCENE_HEAP_MARGIN/2)/1024);
}
void scene_assets_slots(unsigned *resident_out,unsigned *capacity_out) { *resident_out=resident;*capacity_out=capacity; }
int scene_assets_heap_low(void) {
    int f=heap_free();if (f<heap_free_min) heap_free_min=f;
    return heap_free_min;
}

static bool warm_queued[CHUNKS_ALL];
static void warm_add(unsigned chunk) {
    if (warm_queued[chunk]) return;
    warm_queued[chunk]=true;warm_list[warm_count++]=(uint16_t)chunk;
}
static void mark_map(const scene_map_cell_t *cells,unsigned count) {
    for (unsigned i=0;i<count;i++) for (unsigned phase=0;phase<8;phase++) {
        unsigned c=scene_map_images[scene_cell_phase(&cells[i],phase)].page/8;
        if (!map_chunk[c]) {map_chunk[c]=true;map_chunks++;}
    }
}
/* The Expansion Pak's warm list (pinned banks, the whole map in camera
 * order, then the script's spawns) is arithmetic on the compiled tables:
 * ~27 ms that scene_assets_begin did at once, a skipped frame when the cache
 * begins beside a movie. Built ahead in small steps in the boot cinematics'
 * spare time (scene_assets_plan_step); begin uses it when the cache takes
 * the expanded branch, else builds the list itself as before. */
enum { PLAN_BANKS,PLAN_MAP,PLAN_PATCH,PLAN_CAMERA,PLAN_SELECT,PLAN_AHEAD,PLAN_DONE };
static int plan_stage;static unsigned plan_pos,plan_banks;static int *plan_camera;
static void plan_banks_now(void) {
    // Marco's branch transitions stay pinned (all banks but the streamed ones).
    for (unsigned bank=0;bank<SCENE_BANK_COUNT;bank++) if (bank!=SCENE_SOLDIER && bank!=SCENE_NATIVE) {
        const scene_bank_t *b=&scene_banks[bank];
        for (unsigned i=0;i<b->first+b->second;i++) {unsigned c=scene_images[b->start+i].page/8;pinned[c]=true;warm_add(c);}
    }
    plan_banks=warm_count;
}
/* First camera that brings each map cell within 416 px (inverse of msx_layer_scroll). */
static void plan_cameras(unsigned from,unsigned to) {
    for (unsigned i=from;i<to;i++) {
        const scene_map_cell_t *cell=&scene_map_cells_data[i];unsigned layer=cell->layer<4?cell->layer:3;
        int ahead=cell->x-416,camera=ahead<=0?0:layer==0?ahead*2:layer==1?ahead*4/3:ahead;
        if (layer==2 && camera<MSX_MID_FULL_SPEED_CAMERA) camera=MSX_MID_FULL_SPEED_CAMERA;
        for (unsigned phase=0;phase<8;phase++) {
            unsigned c=scene_map_images[scene_cell_phase(cell,phase)].page/8;
            if (camera<plan_camera[c]) plan_camera[c]=camera;
        }
    }
}
/* The map chunk the camera reaches first, pinned and queued (37 in all). */
static bool plan_select_one(void) {
    int best=-1;
    for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) if (map_chunk[c] && !pinned[c] && (best<0 || plan_camera[c]<plan_camera[best])) best=(int)c;
    if (best<0) return false;
    pinned[best]=true;warm_add((unsigned)best);
    return true;
}
static void plan_ahead(void) {
    warm_required=warm_count;
    // Then the actors of the script's spawns in camera order, into the
    // free slots only (the lookahead keeps them; the LRU recycles them).
    for (unsigned i=0;i<SCENE_AHEAD_COUNT;i++) warm_add(scene_ahead[i][1]);
}
bool scene_assets_plan_step(uint32_t deadline) {
    enum { CELLS=256 };
    if (cache) return false;   // begin has built it
    while (plan_stage!=PLAN_DONE) {
        if (deadline && (int32_t)(deadline-TICKS_READ())<(int32_t)TICKS_FROM_US(1500)) return true;
        switch (plan_stage) {
        case PLAN_BANKS: plan_banks_now();plan_stage=PLAN_MAP;plan_pos=0;break;
        case PLAN_MAP: {
            unsigned n=SCENE_MAP_CELL_COUNT-plan_pos;if (n>CELLS) n=CELLS;
            mark_map(scene_map_cells_data+plan_pos,n);plan_pos+=n;
            if (plan_pos>=SCENE_MAP_CELL_COUNT) plan_stage=PLAN_PATCH;
            break;
        }
        case PLAN_PATCH:
            if (!plan_camera && !(plan_camera=malloc(SCENE_CHUNK_COUNT*sizeof(int)))) return false;   // begin builds it then
            mark_map(scene_patch_cells_data,SCENE_PATCH_CELL_COUNT);
            mark_map(scene_collapse_cells_data,SCENE_COLLAPSE_CELL_COUNT);
            for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) plan_camera[c]=0x7fffffff;
            plan_stage=PLAN_CAMERA;plan_pos=0;break;
        case PLAN_CAMERA: {
            unsigned n=SCENE_MAP_CELL_COUNT-plan_pos;if (n>CELLS) n=CELLS;
            plan_cameras(plan_pos,plan_pos+n);plan_pos+=n;
            if (plan_pos>=SCENE_MAP_CELL_COUNT) plan_stage=PLAN_SELECT;
            break;
        }
        case PLAN_SELECT:
            for (unsigned k=0;k<4 && plan_stage==PLAN_SELECT;k++) if (!plan_select_one()) plan_stage=PLAN_AHEAD;
            break;
        case PLAN_AHEAD:
            plan_ahead();free(plan_camera);plan_camera=NULL;plan_stage=PLAN_DONE;break;
        }
    }
    return false;
}
/* Slot memory in as few heap blocks as fit, the largest first (the free
 * total can be split by earlier allocations: cinematic slots freed around the
 * resident sounds), up to `target` slots and only while the heap keeps
 * `keep` bytes free. */
static void allocate_slots(unsigned target,int keep) {
    unsigned block=target-capacity;
    while (capacity<target && block) {
        if (block>target-capacity) block=target-capacity;
        if (heap_free()-(int)(block*SCENE_CHUNK_BYTES)<keep) {block/=2;continue;}
        uint8_t *m=memalign(64,block*SCENE_CHUNK_BYTES);
        if (!m) {block/=2;continue;}
        if (!cache) cache=m;
        for (unsigned i=0;i<block;i++) slot_mem[capacity++]=m+i*SCENE_CHUNK_BYTES;
    }
    stats.cache_bytes=capacity*SCENE_CHUNK_BYTES;
}
static void begin(int coming,int keep);
void scene_assets_begin(void) {
    if (cache && capacity<capacity_final) {
        /* Begun early (the boot cinematics' spare time): the rest of the
         * slots, now that the movies are gone. */
        uint32_t began=TICKS_READ();unsigned before=capacity;
        /* The size foreseen when it began, checked against the heap there is
         * now, as a cache begun here would be sized. */
        // The slot records (begin) are already allocated for capacity_final.
        int room=heap_free()+(int)(capacity*SCENE_CHUNK_BYTES)-heap_reserve()-64;
        unsigned now=room>0?(unsigned)room/SCENE_CHUNK_BYTES:0;
        if (now<capacity_final) capacity_final=now>capacity?now:capacity;
        allocate_slots(capacity_final,0);
        assertf(capacity>=SCENE_EXPANDED_MIN_SLOTS,"RAM budget: %u scene slots allocated (minimum %u)",capacity,SCENE_EXPANDED_MIN_SLOTS);
        capacity_final=capacity;
        debugf("MSX pack: cache completed %u -> %u slots (%u KB) in %lu us, %u resident\n",before,capacity,stats.cache_bytes/1024,(unsigned long)TICKS_TO_US(TICKS_READ()-began),resident);
        scene_assets_mem("scene cache");
        return;
    }
    if (cache) return;
    begin(0,0);
}
void scene_assets_grow_early(int keep) {
    if (!cache || capacity>=capacity_final) return;
    unsigned before=capacity;allocate_slots(capacity_final,keep);
    if (capacity>before) debugf("MSX pack: early cache %u -> %u slots\n",before,capacity);
}
bool scene_assets_begin_early(int coming,int keep) {
    if (cache || get_memory_size()<8*1024*1024) return cache!=NULL;
    begin(coming,keep);
    return cache!=NULL;
}
/* coming: heap bytes freed before scene_assets_begin completes the cache
 * (0: not early); keep: what the heap keeps meanwhile. */
static void begin(int coming,int keep) {
    uint32_t began=TICKS_READ();
    rom=PhysicalAddr((void*)(uintptr_t)dfs_rom_addr("/scene.pack"));
    assertf(rom && dfs_rom_size("/scene.pack")==SCENE_PACK_BYTES && !(rom&7),"Scene ROM pack");
    bool expanded=get_memory_size()>=8*1024*1024;
    /* 4 MiB: 32 slots (512 KiB), the map streams through them. Expansion
     * Pak: every byte the RAM budget leaves (heap_reserve above). */
    unsigned target=512*1024/SCENE_CHUNK_BYTES;
    staging[0]=memalign(64,SCENE_CHUNK_BYTES);staging[1]=memalign(64,SCENE_CHUNK_BYTES);
    if (expanded) {
        /* Each slot costs its 16 KiB plus its two records (slot_mem, slots
         * below), allocated for the slots only: before, the records of every
         * chunk of the pack (1007 x 16 B) were reserved, ~12 KB too many. */
        int room=heap_free()+coming-heap_reserve()-64;
        enum { SLOT_COST=SCENE_CHUNK_BYTES+sizeof(cache_slot_t)+sizeof(uint8_t *) };
        unsigned fit=room>0?(unsigned)room/SLOT_COST:0;
        if (coming && fit<SCENE_EXPANDED_MIN_SLOTS) {   // early: the verdict is scene_assets_begin's
            debugf("MSX pack: early cache refused, %d KB foreseen\n",room/1024);
            free(staging[0]);free(staging[1]);staging[0]=staging[1]=NULL;return;
        }
        assertf(fit>=SCENE_EXPANDED_MIN_SLOTS,"RAM budget: %d KB left for the scene cache (minimum %u KB)",room/1024,(unsigned)(SCENE_EXPANDED_MIN_SLOTS*SLOT_COST/1024));
        target=fit;
        if (target>SCENE_CHUNK_COUNT) target=SCENE_CHUNK_COUNT;
    }
    slot_mem=calloc(target,sizeof(*slot_mem));
    slots=calloc(target,sizeof(*slots));
    assertf(slot_mem && slots && staging[0] && staging[1],"Scene cache allocation");
    capacity=0;allocate_slots(target,coming?keep:0);
    if (coming && !capacity) {   // no room yet: begun later
        free(slot_mem);free(slots);free(staging[0]);free(staging[1]);slot_mem=NULL;slots=NULL;staging[0]=staging[1]=NULL;
        return;
    }
    if (!coming && capacity<target) {
        assertf(!expanded || capacity>=SCENE_EXPANDED_MIN_SLOTS,"RAM budget: %u scene slots allocated (minimum %u)",capacity,SCENE_EXPANDED_MIN_SLOTS);
        target=capacity;
    }
    capacity_final=target;
    assertf(cache,"Scene cache allocation");
    for (unsigned i=0;i<capacity_final;i++) slots[i].chunk=-1;
    for (unsigned i=0;i<CHUNKS_ALL;i++) chunk_slot[i]=-1;
    decode_estimate=TICKS_FROM_US(coming?8000:4000);   // early: beside a movie, the first guess on the safe side
    register_VI_handler(on_vblank);
    data_cache_hit_writeback((void*)scene_palettes,sizeof(scene_palettes));
    bool planned=plan_stage==PLAN_DONE;
    if (!(planned && expanded && capacity_final>=plan_banks+map_chunks+SCENE_EXPANDED_STREAM_SLOTS)) {
        if (plan_stage!=PLAN_BANKS) {   // a plan for another branch, or unfinished: from the start
            for (unsigned c=0;c<CHUNKS_ALL;c++) pinned[c]=map_chunk[c]=warm_queued[c]=false;
            warm_count=warm_required=map_chunks=0;free(plan_camera);plan_camera=NULL;
        }
        planned=false;plan_stage=PLAN_DONE;
        plan_banks_now();
        mark_map(scene_map_cells_data,SCENE_MAP_CELL_COUNT);
        mark_map(scene_patch_cells_data,SCENE_PATCH_CELL_COUNT);
        mark_map(scene_collapse_cells_data,SCENE_COLLAPSE_CELL_COUNT);
    }
    if (expanded && capacity_final>=plan_banks+map_chunks+SCENE_EXPANDED_STREAM_SLOTS) {
        /* Expansion Pak: the whole map (every cell, the shop patch and the
         * rubble) is pinned and loaded before the first frame, in camera
         * order: the background never misses. */
        map_resident_all=true;
        if (!planned) {
            /* Scratch in the idle staging buffer (no DMA before the warm-up). */
            _Static_assert(SCENE_CHUNK_COUNT*sizeof(int)<=SCENE_CHUNK_BYTES,"first_camera fits a staging buffer");
            plan_camera=(int*)staging[0];
            for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) plan_camera[c]=0x7fffffff;
            plan_cameras(0,SCENE_MAP_CELL_COUNT);
            while (plan_select_one()) {}
            plan_camera=NULL;
            plan_ahead();
        }
    } else {
        /* 4 MiB: the opening view only (the extended map does not fit);
         * prefetch streams the rest. tools/check_scene_budget.py bounds the
         * working set. */
        for (unsigned i=0;i<SCENE_MAP_CELL_COUNT;i++) {
            const scene_map_cell_t *cell=&scene_map_cells_data[i];unsigned layer=cell->layer<4?cell->layer:3;
            int x=cell->x-msx_layer_scroll(layer,0);
            if (!msx_layer_visible(layer,0) || x< -48 || x>=352) continue;
            for (unsigned phase=0;phase<8;phase++) warm_add(scene_map_images[scene_cell_phase(cell,phase)].page/8);
        }
        warm_required=warm_count;
    }
    warming=true;
    debugf("MSX pack: %u pages, %u palettes, %u chunks (%u map); cache=%u KB (%u slots%s) reserve=%u KB warm=%u (required %u) setup=%lu us%s\n",
        SCENE_PAGE_COUNT,SCENE_PALETTE_COUNT,SCENE_CHUNK_COUNT,map_chunks,stats.cache_bytes/1024,capacity,coming?", begun early":"",(unsigned)heap_reserve()/1024,
        warm_count,warm_required,(unsigned long)TICKS_TO_US(TICKS_READ()-began),planned?" (warm list planned ahead)":"");
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
const scene_image_t *scene_background_tile(unsigned row,unsigned col) { assert(row<7 && col<10);return &scene_background[row*10+col]; }
/* ---- Mission packs (docs/MISION2.md "Fase 2") ----
 * The game keeps Mission 1's tables compiled in; another mission's pack is
 * opened at the mission switch (01891E) and each stage's tables are read
 * into slots of the scene cache taken out of the LRU (`reserved`), so the
 * RAM budget does not change: the cache still has every slot it had, a
 * stage borrows what its tables need (stage 1 of Mission 2: 3 slots) and
 * its map chunks are pinned like Mission 1's. Back to Mission 1 (game over
 * or reset), the slots are released and Mission 1's map pinned again. */
typedef struct __attribute__((packed)) {
    uint32_t descriptor;uint16_t width,camera_max;uint32_t backdrop,first_cell,cell_count;
    uint16_t columns,rows;int16_t origin_x,origin_y;uint32_t grid,unknown,image_first,image_count;
    struct __attribute__((packed)) {uint32_t rom,offset,length;} blob[3];
    scene_layer_rule_t rules[4];
    uint32_t patch_offset;uint16_t patch_count;   /* map patches 0C70FC (B37) */
    uint32_t recon_offset,recon_length;           /* map reconnaissance 'MSXR' (Mission 3): its dynamic maps are read */
} pack_stage_t;
_Static_assert(sizeof(pack_stage_t)<=128,"pack_mission.py stage record");
typedef struct __attribute__((packed)) {
    char magic[4];uint16_t version,mission,stages,palettes;uint32_t images,chunks,cells,grid_bytes,actors,actor_images,resources;
    uint32_t data_offset,data_length;   /* version 4: the mission's own data 'MSXD' (pack_mission.py mission_data) */
} pack_header_t;
static uint32_t mission_header_bytes;
static pack_header_t mission_head;
static uint32_t mission_cells_at,mission_images_at,mission_palettes_at,mission_chunks_at,mission_actors_at;
/* The mission's native actors (pack version 3, docs/MISION2.md "Fase 2e"):
 * descriptors the scene pack lacks, their parts (SCENE_NATIVE frames after
 * the bank's own) and the palette resources the scene pack lacks. */
typedef struct { uint16_t resource,palette; } mission_resource_t;
static scene_native_descriptor_t *stage_actors;static scene_image_t *stage_actor_images;static mission_resource_t *stage_resources;
static scene_stage_t stage;static bool stage_on;
static unsigned stage_images_count(void);
static int reserved_first=-1;static unsigned reserved_count;
static unsigned stage_map_first,stage_map_count;   // mission chunk ids of the stage map
static uint8_t *reserve_mem;static unsigned reserve_used,reserve_size;
static scene_map_cell_t *stage_cells;static scene_image_t *stage_images;static uint16_t (*stage_palettes)[16];static unsigned stage_image_total;
static unsigned stage_images_count(void) {return stage_image_total;}
const scene_image_t *scene_frame(unsigned id,unsigned frame) {
    const scene_bank_t *b=scene_bank(id);
    if (id==SCENE_NATIVE && frame>=b->first+b->second) {
        assert(stage_on && frame-(b->first+b->second)<mission_head.actor_images);
        return &stage_actor_images[frame-(b->first+b->second)];
    }
    assert(frame<b->first+b->second);return &scene_images[b->start+frame];
}
static void rom_read(void *dst,uint32_t offset,uint32_t length) {
    assertf(offset+length<=mission_bytes,"Mission pack read");
    if (!length) return;   /* empty table (Mission 3: no actor section yet) */
    data_cache_hit_writeback_invalidate(dst,length);dma_read(dst,mission_rom+offset,length);
}
static void *take(unsigned bytes) {
    void *p=reserve_mem+reserve_used;reserve_used+=(bytes+15)&~15u;
    assertf(reserve_used<=reserve_size,"Stage tables exceed their slots");
    return p;
}
const scene_stage_t *scene_stage(void) { return stage_on?&stage:NULL; }
/* The mission's own data 'MSXD' (pack version 4, docs/MEMORY_OPTIMIZATION.md
 * rule 5): ROM blobs served to ga_rom and animation programs served to the
 * world (neo_world.c program) while a stage of the mission is loaded. */
typedef struct { uint32_t tag; neo_anim_program_t program; } stage_program_t;
static const uint8_t *mission_data;static unsigned mission_blobs;static stage_program_t *stage_programs;static unsigned stage_program_count;
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static void load_mission_data(unsigned programs) {
    mission_data=NULL;mission_blobs=0;stage_programs=NULL;stage_program_count=0;
    if (mission_head.data_length<12u) return;
    uint8_t *d=take(mission_head.data_length);rom_read(d,mission_head.data_offset,mission_head.data_length);
    if (memcmp(d,"MSXD",4) || (d[4]<<8|d[5])!=1u) {debugf("MSX mission data block not read (explicit stop)\n");return;}
    mission_data=d;mission_blobs=d[6]<<8|d[7];
    stage_programs=take(programs*sizeof(stage_program_t));
    _Static_assert(sizeof(neo_anim_command_t)==10,"MSXD command record");
    for (unsigned i=0;i<programs;i++) {
        const uint8_t *r=d+12u+12u*mission_blobs+12u*i;
        uint32_t tag=rd32(r),at=rd32(r+4);unsigned n=r[8]<<8|r[9];
        neo_anim_command_t *c=(neo_anim_command_t*)(d+at);
        for (unsigned k=0;k<n;k++) {   /* in place: the record and the struct are 10 bytes */
            const uint8_t *q=d+at+10u*k;
            int32_t asset=(int32_t)((uint32_t)q[1]<<16|(uint32_t)q[2]<<8|q[3]);if (asset&0x800000) asset-=0x1000000;
            neo_anim_command_t v={(neo_anim_kind_t)q[0],asset,(uint16_t)(q[4]<<8|q[5]),q[6],q[7],(int8_t)q[8],(int8_t)q[9]};
            c[k]=v;
        }
        stage_programs[i]=(stage_program_t){tag,{c,(uint16_t)n}};
    }
    stage_program_count=programs;
}
const neo_anim_program_t *scene_stage_program(uint32_t tag) {
    if (!stage_on) return NULL;
    unsigned lo=0,hi=stage_program_count;
    while (lo<hi) {unsigned m=(lo+hi)/2;if (stage_programs[m].tag<tag) lo=m+1; else hi=m;}
    return lo<stage_program_count && stage_programs[lo].tag==tag?&stage_programs[lo].program:NULL;
}
const uint8_t *scene_stage_rom(uint32_t address) {
    if (!stage_on) return NULL;
    for (unsigned i=0;i<stage.dynamic_map_count;i++)
        if (address>=stage.dynamic_map_rom[i] && address<stage.dynamic_map_rom[i]+stage.dynamic_map_length[i]) return stage.dynamic_map[i]+(address-stage.dynamic_map_rom[i]);
    for (unsigned i=0;i<mission_blobs;i++) {
        const uint8_t *r=mission_data+12u+12u*i;uint32_t rom=rd32(r),len=rd32(r+8);
        if (address>=rom && address<rom+len) return mission_data+rd32(r+4)+(address-rom);
    }
    return NULL;
}
static void release_reserved(void) {
    if (reserved_first<0) return;
    for (unsigned i=0;i<reserved_count;i++) slots[reserved_first+i].reserved=false;
    reserved_first=-1;reserved_count=0;reserve_mem=NULL;
}
/* Takes `count` contiguous slots (one heap block) out of the cache, the last
 * such run so the pinned player banks (warmed first) are rarely in it. */
static bool reserve_slots(unsigned count) {
    for (int first=(int)capacity-(int)count;first>=0;first--) {
        bool ok=true;
        for (unsigned i=1;i<count && ok;i++) ok=slot_mem[first+i]==slot_mem[first]+i*SCENE_CHUNK_BYTES;
        if (!ok) continue;
        for (unsigned i=0;i<count;i++) {
            cache_slot_t *sl=&slots[first+i];
            if (sl->chunk>=0) {chunk_slot[sl->chunk]=-1;if (sl->ready) resident--;stats.evictions++;}
            *sl=(cache_slot_t){.chunk=-1,.reserved=true};
        }
        reserved_first=first;reserved_count=count;reserve_mem=slot_mem[first];reserve_used=0;reserve_size=count*SCENE_CHUNK_BYTES;
        return true;
    }
    return false;
}
static void forget_mission_chunks(void) {
    for (unsigned c=SCENE_CHUNK_COUNT;c<CHUNKS_ALL;c++) {
        pinned[c]=false;map_chunk[c]=false;requests[c]=0;
        if (chunk_slot[c]>=0) {slots[chunk_slot[c]]=(cache_slot_t){.chunk=-1};chunk_slot[c]=-1;resident--;}
    }
}
/* Waits for the chunk in flight (its slot may be reserved next). */
static void quiesce(void) {
    while (active_chunk>=0) { if (pi_busy()) dma_wait(); else decode_active(); }
    for (unsigned c=0;c<CHUNKS_ALL;c++) requests[c]=0;
}
bool scene_mission_load(unsigned mission) {
    char path[32];snprintf(path,sizeof(path),"/mission%u.pack",mission);
    uint32_t at=(uint32_t)dfs_rom_addr(path);
    if (!at) {debugf("MSX mission pack %s missing\n",path);return false;}
    quiesce();scene_mission_unload();
    mission_rom=PhysicalAddr((void*)(uintptr_t)at);mission_bytes=(uint32_t)dfs_rom_size(path);
    static pack_header_t head __attribute__((aligned(16)));
    data_cache_hit_writeback_invalidate(&head,sizeof(head));dma_read(&head,mission_rom,sizeof(head));
    if (memcmp(head.magic,"MSXM",4) || (head.version!=3 && head.version!=4) || head.mission!=mission || head.chunks>SCENE_MISSION_CHUNKS_MAX) {
        debugf("MSX mission pack %s: bad header (version %u, %lu chunks)\n",path,head.version,(unsigned long)head.chunks);return false;
    }
    /* Version 3 has no mission data (its header ends at `resources`). */
    mission_header_bytes=head.version>=4?sizeof(head):sizeof(head)-8u;
    if (head.version<4) head.data_offset=head.data_length=0;
    mission_head=head;
    uint32_t o=mission_header_bytes+128u*head.stages;
    mission_cells_at=o;o+=22u*head.cells;
    mission_images_at=o;o+=8u*head.images;
    mission_palettes_at=o;o+=32u*head.palettes;
    mission_chunks_at=o;o+=16u*head.chunks;
    mission_actors_at=o;
    /* Mission 1's map is no longer drawn: its chunks go back to the LRU. */
    for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) if (map_chunk[c]) pinned[c]=false;
    map_resident_all=false;
    debugf("MSX mission pack %s: %u stages, %lu chunks, %lu images, %lu cells, %lu bytes\n",path,head.stages,(unsigned long)head.chunks,(unsigned long)head.images,(unsigned long)head.cells,(unsigned long)mission_bytes);
    return true;
}
/* Called between the reads and chunk loads of a stage: the audio service
 * (the loads take 0.14..0.64 s, longer than the AI's two buffers) and, in
 * actor.c, a black frame shown every ~33 ms (the original loads behind its
 * wipe/fade 3AED8/3AEF0, with the screen black). */
static void (*load_idle)(void);
void scene_assets_set_load_idle(void (*fn)(void)) {load_idle=fn;}
/* Phase patterns of the map cells: Mission 1's (scene_meta.h), then those a
 * stage adds (scene_stage_load). */
int8_t scene_cell_patterns[SCENE_CELL_PATTERNS][8]=SCENE_CELL_PATTERNS_INIT;
static unsigned cell_patterns=SCENE_CELL_PATTERN_COUNT,pattern_stops;
static uint8_t cell_pattern(const uint16_t ph[8]) {
    int8_t d[8];bool moving=false;
    for (unsigned k=0;k<8;k++) {
        int v=(int)ph[k]-(int)ph[0];
        if (v<-128 || v>127) {pattern_stops++;return SCENE_CELL_STATIC;}
        d[k]=(int8_t)v;moving|=v!=0;
    }
    if (!moving) return SCENE_CELL_STATIC;
    for (unsigned p=0;p<cell_patterns;p++) if (!memcmp(scene_cell_patterns[p],d,8)) return (uint8_t)p;
    if (cell_patterns==SCENE_CELL_PATTERNS) {pattern_stops++;return SCENE_CELL_STATIC;}
    memcpy(scene_cell_patterns[cell_patterns],d,8);return (uint8_t)cell_patterns++;
}
bool scene_stage_load(unsigned index) {
    if (!mission_bytes || index>=mission_head.stages) return false;
    uint32_t began=TICKS_READ();
    quiesce();
    static pack_stage_t ps __attribute__((aligned(16)));
    data_cache_hit_writeback_invalidate(&ps,sizeof(ps));dma_read(&ps,mission_rom+mission_header_bytes+128u*index,sizeof(ps));
    /* The previous stage's chunks and tables. */
    release_reserved();forget_mission_chunks();stage_on=false;mission_chunk_count=0;
    unsigned cells=ps.cell_count,images=ps.image_count,grid=(unsigned)ps.columns*ps.rows;
    unsigned need=((cells*22u+15)&~15u)+((images*sizeof(scene_image_t)+15)&~15u)+32u*mission_head.palettes+16u*mission_head.chunks+
        2u*((grid+15)&~15u)+(ps.rows>32u?4096u:0u)+(ps.unknown?((grid+7)/8+16):0)+((cells*(sizeof(uint16_t)+sizeof(scene_cell_run_t))+31)&~15u)+
        (SCENE_STRIP_ITEMS+SCENE_STRIP_GROUPS)*SCENE_STRIP_ENTRY_BYTES;
    for (unsigned b=0;b<3;b++) need+=(ps.blob[b].length+15)&~15u;
    /* 'MSXR' (pack_mission.py recon_blob, docs/MISION3.md): the dynamic maps
     * (0140C0) of the stage's actors, served to ga_rom (scene_stage_rom). */
    static uint8_t recon_head[16] __attribute__((aligned(16)));
    static struct __attribute__((packed)) {uint32_t rom,offset,length;} recon_dyn[SCENE_STAGE_DYNAMIC_MAPS] __attribute__((aligned(16)));
    unsigned dmaps=0;
    if (ps.recon_length>=16u) {
        rom_read(recon_head,ps.recon_offset,16);
        unsigned maps=recon_head[6]<<8|recon_head[7],pats=recon_head[8]<<8|recon_head[9];
        dmaps=recon_head[10]<<8|recon_head[11];
        if (memcmp(recon_head,"MSXR",4) || (recon_head[4]<<8|recon_head[5])!=1u || dmaps>SCENE_STAGE_DYNAMIC_MAPS) {
            debugf("MSX stage %u: map reconnaissance block not read (version %u, %u dynamic maps; explicit stop)\n",index+1,recon_head[4]<<8|recon_head[5],dmaps);dmaps=0;
        } else if (dmaps) {
            rom_read(recon_dyn,ps.recon_offset+16u+28u*maps+20u*pats,12u*dmaps);
            for (unsigned i=0;i<dmaps;i++) need+=(recon_dyn[i].length+15)&~15u;
        }
    }
    need+=((8u*mission_head.actors+15)&~15u)+((mission_head.actor_images*sizeof(scene_image_t)+15)&~15u)+((4u*mission_head.resources+15)&~15u);
    /* 'MSXD': the mission's ROM blobs and programs (read whole, the
     * commands converted in place) and the program table. */
    static uint8_t data_head[16] __attribute__((aligned(16)));
    unsigned data_programs=0;
    if (mission_head.data_length>=12u) {
        rom_read(data_head,mission_head.data_offset,16);
        data_programs=data_head[8]<<8|data_head[9];
        need+=((mission_head.data_length+15)&~15u)+((data_programs*sizeof(stage_program_t)+15)&~15u);
    }
    /* Pre-rendered columns (scene_prerender.h): tables and rings
     * in the stage's slots, in place of its pinned static map chunks. */
    unsigned prerender_bytes=scene_prerender_probe(mission_head.mission,index);need+=prerender_bytes;
    unsigned count=(need+SCENE_CHUNK_BYTES-1)/SCENE_CHUNK_BYTES;
    if (!reserve_slots(count)) {debugf("MSX stage %u tables: %u contiguous slots not found\n",index+1,count);return false;}
    /* Chunk table and palettes (mission palette n = SCENE_PALETTE_COUNT+n). */
    uint8_t *prerender_mem=prerender_bytes?take(prerender_bytes):NULL;
    scene_chunk_t *chunks=take(16u*mission_head.chunks);rom_read(chunks,mission_chunks_at,16u*mission_head.chunks);
    mission_chunks=chunks;mission_chunk_count=mission_head.chunks;
    stage_palettes=take(32u*mission_head.palettes);rom_read(stage_palettes,mission_palettes_at,32u*mission_head.palettes);
    if (load_idle) load_idle();
    /* Cells: 22-byte records (u8 layer, 8 image ids) converted in place to
     * the 8-byte scene_map_cell_t (never over an unread record), the image
     * ids rebased to the stage's images; an animated cell's phase deltas
     * become a pattern (Mission 2: one, 0..7) appended after Mission 1's. */
    _Static_assert(sizeof(scene_map_cell_t)==8,"scene_map_cell_t layout");
    uint8_t *raw=take(cells*22u);rom_read(raw,mission_cells_at+22u*ps.first_cell,cells*22u);
    stage_cells=(scene_map_cell_t*)raw;cell_patterns=SCENE_CELL_PATTERN_COUNT;pattern_stops=0;
    for (unsigned i=0;i<cells;i++) {
        const uint8_t *r=raw+22u*i;scene_map_cell_t c;uint16_t ph[8];
        for (unsigned k=0;k<8;k++) ph[k]=(uint16_t)((r[6+2*k]<<8|r[7+2*k])-ps.image_first);
        c.layer=r[0];c.x=(int16_t)(r[2]<<8|r[3]);c.y=(int16_t)(r[4]<<8|r[5]);c.phase=ph[0];c.anim=cell_pattern(ph);
        memcpy(&stage_cells[i],&c,sizeof(c));
        if (load_idle && (i&1023u)==1023u) load_idle();
    }
    if (pattern_stops) debugf("MSX stage %u cells: %u animated cells drawn with their first image (explicit stop: more than %u phase patterns or a delta beyond int8)\n",index+1,pattern_stops,SCENE_CELL_PATTERNS);
    if (load_idle) load_idle();
    /* Images: 8-byte records (page, palette, y, height) read at the end of
     * their table and expanded forward (never over an unread record). */
    stage_images=take(images*sizeof(scene_image_t));stage_image_total=images;
    uint8_t *ri=(uint8_t*)stage_images+images*(sizeof(scene_image_t)-8u);rom_read(ri,mission_images_at+8u*ps.image_first,images*8u);
    unsigned min_page=0xffff,max_page=0;
    for (unsigned i=0;i<images;i++) {
        const uint8_t *r=ri+8u*i;uint16_t page=(uint16_t)(r[0]<<8|r[1]),pal=(uint16_t)(r[2]<<8|r[3]),y=(uint16_t)(r[4]<<8|r[5]),h=(uint16_t)(r[6]<<8|r[7]);
        stage_images[i]=(scene_image_t){(uint16_t)(SCENE_MISSION_PAGE_BASE+page),(uint16_t)(SCENE_PALETTE_COUNT+pal),0,(int16_t)y,16,h,2};
        if (page<min_page) min_page=page;
        if (page>max_page) max_page=page;
    }
    if (load_idle) load_idle();
    /* Actors: descriptors (first rebased after the native bank's frames),
     * 12-byte parts expanded forward from the end of their table as the
     * map images, the resource map. */
    _Static_assert(sizeof(scene_native_descriptor_t)==8,"pack actor record size");
    stage_actors=take(8u*mission_head.actors);rom_read(stage_actors,mission_actors_at,8u*mission_head.actors);
    for (unsigned i=0;i<mission_head.actors;i++) stage_actors[i].first=(uint16_t)(stage_actors[i].first+scene_bank(SCENE_NATIVE)->first+scene_bank(SCENE_NATIVE)->second);
    stage_actor_images=take(mission_head.actor_images*sizeof(scene_image_t));
    uint8_t *ra=(uint8_t*)stage_actor_images+mission_head.actor_images*(sizeof(scene_image_t)-12u);
    rom_read(ra,mission_actors_at+8u*mission_head.actors,12u*mission_head.actor_images);
    for (unsigned i=0;i<mission_head.actor_images;i++) {
        const uint8_t *r=ra+12u*i;
        stage_actor_images[i]=(scene_image_t){(uint16_t)(SCENE_MISSION_PAGE_BASE+(r[0]<<8|r[1])),(uint16_t)(SCENE_PALETTE_COUNT+(r[2]<<8|r[3])),
            (int16_t)(r[4]<<8|r[5]),(int16_t)(r[6]<<8|r[7]),(uint16_t)(r[8]<<8|r[9]),(uint16_t)(r[10]<<8|r[11]),0};
    }
    stage_resources=take(4u*mission_head.resources);
    rom_read(stage_resources,mission_actors_at+8u*mission_head.actors+12u*mission_head.actor_images,4u*mission_head.resources);
    for (unsigned i=0;i<mission_head.resources;i++) stage_resources[i].palette=(uint16_t)(SCENE_PALETTE_COUNT+stage_resources[i].palette);
    if (load_idle) load_idle();
    /* Grid, its stamp copy and the unknown mask. */
    uint8_t *g=take(grid),*dyn=take(grid),*unknown=NULL,*ring=ps.rows>32u?take(4096u):NULL;rom_read(g,ps.grid,grid);
    if (ps.unknown) {unknown=take((grid+7)/8);rom_read(unknown,ps.unknown,(grid+7)/8);}
    const uint8_t *blob[3];
    for (unsigned b=0;b<3;b++) {uint8_t *d=take(ps.blob[b].length);if (ps.blob[b].length) rom_read(d,ps.blob[b].offset,ps.blob[b].length);blob[b]=d;}
    if (load_idle) load_idle();
    /* Map patches: their table and the split of the cells (the patch
     * cells are the stage's last). */
    unsigned patches=ps.patch_count<16u?ps.patch_count:16u,normal=cells;
    uint32_t patch_table[32];
    if (patches) {static uint32_t raw_patch[32] __attribute__((aligned(16)));rom_read(raw_patch,ps.patch_offset,8u*patches);memcpy(patch_table,raw_patch,8u*patches);}
    while (normal && (stage_cells[normal-1].layer&0x80u)) normal--;
    uint16_t *order=take(cells*sizeof(uint16_t));scene_cell_run_t *runs=take(cells*sizeof(scene_cell_run_t));
    void *strip_items=take(SCENE_STRIP_ITEMS*SCENE_STRIP_ENTRY_BYTES),*strip_groups=take(SCENE_STRIP_GROUPS*SCENE_STRIP_ENTRY_BYTES);
    const uint8_t *dyn_data[SCENE_STAGE_DYNAMIC_MAPS];
    for (unsigned i=0;i<dmaps;i++) {uint8_t *d=take(recon_dyn[i].length);rom_read(d,ps.recon_offset+recon_dyn[i].offset,recon_dyn[i].length);dyn_data[i]=d;}
    load_mission_data(data_programs);
    data_cache_hit_writeback(reserve_mem,reserve_used);
    stage=(scene_stage_t){.mission=mission_head.mission,.index=index,.descriptor=ps.descriptor,.width=ps.width,.camera_max=ps.camera_max,
        .backdrop=ps.backdrop,.columns=ps.columns,.rows=ps.rows,.grid=g,.grid_dynamic=dyn,.ring=ring,.unknown=unknown,
        .camera_list=blob[0],.camera_rom=ps.blob[0].rom,.camera_len=(uint16_t)ps.blob[0].length,
        .script=blob[1],.script_rom=ps.blob[1].rom,.script_len=(uint16_t)ps.blob[1].length,
        .players=blob[2],.players_rom=ps.blob[2].rom,.players_len=(uint16_t)ps.blob[2].length,
        .cells=stage_cells,.cell_count=normal,.patch_cells=stage_cells+normal,.patch_cell_count=cells-normal,.patch_count=patches,
        .index_order=order,.index_runs=runs,.strip_items=strip_items,.strip_groups=strip_groups};
    for (unsigned i=0;i<patches;i++) {stage.patch_submap[i]=patch_table[2*i];stage.patch_layer_object[i]=patch_table[2*i+1];}
    stage.dynamic_map_count=dmaps;
    for (unsigned i=0;i<dmaps;i++) {stage.dynamic_map_rom[i]=recon_dyn[i].rom;stage.dynamic_map_length[i]=(uint16_t)recon_dyn[i].length;stage.dynamic_map[i]=dyn_data[i];}
    memcpy(stage.rules,ps.rules,sizeof(stage.rules));
    stage_on=true;
    /* The stage map, pinned and loaded now (the screen is black). */
    stage_map_first=min_page/8u;stage_map_count=images?max_page/8u-min_page/8u+1u:0;
    map_chunks=0;
    /* Pre-rendered layers (scene_prerender.h): their static
     * cells stream as columns, not pinned (kept pinned in MSX_BG_CRC
     * builds, whose reference pass draws them through the tiles). */
    unsigned prerendered=scene_prerender_open(prerender_mem,SCENE_PALETTE_COUNT)?scene_prerender_layers():0u;
    if (MSX_BG_CRC) prerendered=0;
    for (unsigned i=0;i<cells;i++) for (unsigned k=0;k<8;k++) {
        if (stage_cells[i].layer<4 && stage_cells[i].anim==SCENE_CELL_STATIC && (prerendered>>stage_cells[i].layer&1u)) break;
        unsigned c=SCENE_CHUNK_COUNT+stage_images[scene_cell_phase(&stage_cells[i],k)].page/8u-SCENE_MISSION_PAGE_BASE/8u;
        if (!map_chunk[c]) {map_chunk[c]=true;pinned[c]=true;map_chunks++;}
    }
    unsigned before=stats.loads;
    if (load_idle) load_idle();
    for (unsigned c=SCENE_CHUNK_COUNT;c<SCENE_CHUNK_COUNT+mission_chunk_count;c++) if (map_chunk[c]) {require_chunk(c,true);if (load_idle) load_idle();}
    /* Pinned player banks evicted by the reservation come back too. */
    for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) if (pinned[c] && !resident_chunk(c)) {require_chunk(c,true);if (load_idle) load_idle();}
    map_resident_all=true;
    debugf("MSX stage load mission %u stage %u: descriptor %06lX %u cells %u images grid %ux%u%s, tables %u KB in %u slots, map %u chunks (%u loads) in %lu us\n",
        stage.mission,index+1,(unsigned long)ps.descriptor,cells,images,ps.columns,ps.rows,unknown?" (unknown cells masked)":"",
        reserve_used/1024,count,map_chunks,stats.loads-before,(unsigned long)TICKS_TO_US(TICKS_READ()-began));
    stats.misses=stats.sync_misses=0;
    return true;
}
void scene_mission_unload(void) {
    if (!mission_bytes) return;
    quiesce();release_reserved();forget_mission_chunks();scene_prerender_close();
    stage_on=false;mission_chunk_count=0;mission_bytes=0;mission_chunks=NULL;
    /* Mission 1's map pinned and resident again. */
    map_chunks=0;
    for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) if (map_chunk[c]) {pinned[c]=true;map_chunks++;}
    for (unsigned c=0;c<SCENE_CHUNK_COUNT;c++) if (pinned[c] && !resident_chunk(c)) require_chunk(c,true);
    map_resident_all=capacity>=warm_required+SCENE_EXPANDED_STREAM_SLOTS;
    debugf("MSX mission pack unloaded: Mission 1 map pinned again\n");
}
const scene_map_cell_t *scene_map_cells(unsigned *count) {
    if (stage_on) {*count=stage.cell_count;return stage_cells;}
    *count=SCENE_MAP_CELL_COUNT;return scene_map_cells_data;
}
const scene_map_cell_t *scene_patch_cells(unsigned *count) { if (stage_on) {*count=0;return NULL;} *count=SCENE_PATCH_CELL_COUNT;return scene_patch_cells_data; }
const scene_map_cell_t *scene_collapse_cells(unsigned *count) { if (stage_on) {*count=0;return NULL;} *count=SCENE_COLLAPSE_CELL_COUNT;return scene_collapse_cells_data; }
const scene_image_t *scene_map_image(unsigned id) {
    if (stage_on) {assert(id<stage_images_count());return &stage_images[id];}
    assert(id<sizeof(scene_map_images)/sizeof(*scene_map_images));return &scene_map_images[id];
}
unsigned scene_map_camera_max(void) { return stage_on?stage.camera_max:SCENE_MAP_CAMERA_MAX; }
uint32_t scene_map_backdrop(bool after) { if (stage_on) return stage.backdrop;return after?SCENE_MAP_BACKDROP_AFTER:SCENE_MAP_BACKDROP; }
const scene_native_descriptor_t *scene_native_descriptor(uint32_t tag) {
    unsigned lo=0,hi=sizeof(scene_native_descriptors)/sizeof(scene_native_descriptors[0]);
    while (lo<hi) {
        unsigned mid=(lo+hi)/2;
        if (scene_native_descriptors[mid].tag==tag) return &scene_native_descriptors[mid];
        if (scene_native_descriptors[mid].tag<tag) lo=mid+1; else hi=mid;
    }
    if (stage_on) {
        lo=0;hi=mission_head.actors;
        while (lo<hi) {
            unsigned mid=(lo+hi)/2;
            if (stage_actors[mid].tag==tag) return &stage_actors[mid];
            if (stage_actors[mid].tag<tag) lo=mid+1; else hi=mid;
        }
    }
    return NULL;
}
int scene_native_palette(unsigned resource) {
    for (unsigned i=0;i<sizeof(scene_native_palettes)/sizeof(scene_native_palettes[0]);i++)
        if (scene_native_palettes[i].resource==resource) return scene_native_palettes[i].palette;
    if (stage_on) for (unsigned i=0;i<mission_head.resources;i++) if (stage_resources[i].resource==resource) return stage_resources[i].palette;
    return -1;
}
const uint16_t *scene_palette(unsigned id) {
    if (id>=SCENE_PALETTE_COUNT && stage_on) {assert(id-SCENE_PALETTE_COUNT<mission_head.palettes);return stage_palettes[id-SCENE_PALETTE_COUNT];}
    assert(id<SCENE_PALETTE_COUNT);return scene_palettes[id];
}
void scene_record_pins(int group) { recording=group; }
void scene_unpin_group(int group) { for (unsigned i=0;i<CHUNKS_ALL;i++) block_pins[i]&=(uint8_t)~(1u<<group); }
unsigned scene_completed_epoch(void) { return completed; }
void scene_begin_epoch(unsigned next) { epoch=next;frame_service_decodes=frame_sync_decodes=0; }
void scene_complete_epoch(unsigned done) { completed=done; }
scene_cache_stats_t scene_cache_stats(void) { return stats; }
