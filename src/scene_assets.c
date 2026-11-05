#include <libdragon.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "scene_assets.h"
#include "scene_codec.h"
#include "scene_meta.h"
#include "mission_camera.h"
_Static_assert(SCENE_MAP_CAMERA_MAX==MSX_CAMERA_MAX,"Scene pack and gameplay camera limits differ");

typedef struct { int chunk; bool ready; unsigned used_epoch,lru; } cache_slot_t;
static cache_slot_t *slots;
static int chunk_slot[SCENE_CHUNK_COUNT];
static unsigned requests[SCENE_CHUNK_COUNT];
static bool pinned[SCENE_CHUNK_COUNT];
static uint8_t *cache,*staging[2];
static unsigned capacity,clock_lru,epoch,rom;
static volatile unsigned completed;
static int active_chunk=-1,active_slot=-1;
static unsigned active_stage,last_stage;
static scene_cache_stats_t stats;
enum { RAM_PAGE_BIT=0x8000,MAX_REPLACEMENTS=360 };
static uint16_t *ram_pages[MAX_REPLACEMENTS];
static scene_image_t ram_images[MAX_REPLACEMENTS];
static scene_map_cell_t ram_cells[MAX_REPLACEMENTS],extra_cells[MAX_REPLACEMENTS];
static int16_t cell_replacement[SCENE_MAP_CELL_COUNT];
static unsigned ram_count,extra_count;
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
        if (!slots[i].ready || pinned[slots[i].chunk] || slots[i].used_epoch>completed) continue;
        if (victim<0 || slots[i].lru<slots[victim].lru) victim=(int)i;
    }
    if (victim<0) return;
    if (slots[victim].chunk>=0) { chunk_slot[slots[victim].chunk]=-1;stats.evictions++; }
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
    slots[slot].ready=true;slots[slot].lru=++clock_lru;
}
static void request(unsigned chunk,unsigned priority) {
    assertf(chunk<SCENE_CHUNK_COUNT,"Scene chunk ID");
    if (chunk_slot[chunk]>=0) return;
    if (requests[chunk]<priority) requests[chunk]=priority;
}
void scene_request_image(const scene_image_t *image,unsigned priority) {
    if (!(image->page&RAM_PAGE_BIT)) request(image->page/8,priority);
}
static void require_chunk(unsigned chunk) {
    if (chunk_slot[chunk]>=0 && slots[chunk_slot[chunk]].ready) { stats.hits++;return; }
    stats.misses++;request(chunk,255);
    while (chunk_slot[chunk]<0 || !slots[chunk_slot[chunk]].ready) {
        scene_service();
        if (active_chunk>=0 || pi_busy()) dma_wait();
        else {
            rspq_wait();scene_service();
            assertf(active_chunk>=0 || (chunk_slot[chunk]>=0 && slots[chunk_slot[chunk]].ready),"Scene working set exceeds cache");
        }
    }
}
const void *scene_page(const scene_image_t *image) {
    if (image->page&RAM_PAGE_BIT) {
        unsigned id=image->page&~RAM_PAGE_BIT;
        assertf(id<ram_count && ram_pages[id],"Scene replacement page");return ram_pages[id];
    }
    assertf(image->page<SCENE_PAGE_COUNT,"Scene page ID");
    unsigned chunk=image->page/8;require_chunk(chunk);
    cache_slot_t *s=&slots[chunk_slot[chunk]];s->used_epoch=epoch;s->lru=++clock_lru;
    return cache+(unsigned)chunk_slot[chunk]*SCENE_CHUNK_BYTES+(image->page%8)*SCENE_PAGE_BYTES;
}
void scene_assets_init(void) {
    if (SCENE_MAP_CELL_SIZE==32 && get_memory_size()<8*1024*1024) {
        debugf("MSX ERROR: prerender backgrounds require Expansion Pak (8 MiB)\n");
        assertf(false,"Prerender backgrounds require 8 MiB");abort();
    }
    rom=PhysicalAddr((void*)(uintptr_t)dfs_rom_addr("/scene.pack"));
    assertf(rom && dfs_rom_size("/scene.pack")==SCENE_PACK_BYTES && !(rom&7),"Scene ROM pack");
    capacity=(get_memory_size()>=8*1024*1024?3*1024*1024:512*1024)/SCENE_CHUNK_BYTES;
    if (capacity>SCENE_CHUNK_COUNT) capacity=SCENE_CHUNK_COUNT;
    cache=memalign(64,capacity*SCENE_CHUNK_BYTES);slots=calloc(capacity,sizeof(*slots));
    staging[0]=memalign(64,SCENE_CHUNK_BYTES);staging[1]=memalign(64,SCENE_CHUNK_BYTES);
    assertf(cache && slots && staging[0] && staging[1],"Scene cache allocation");
    for (unsigned i=0;i<capacity;i++) slots[i].chunk=-1;
    for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++) chunk_slot[i]=-1;
    stats.cache_bytes=capacity*SCENE_CHUNK_BYTES;
    memset(cell_replacement,255,sizeof(cell_replacement));
    data_cache_hit_writeback((void*)scene_palettes,sizeof(scene_palettes));
    data_cache_hit_writeback((void*)scene_ci8_palettes,sizeof(scene_ci8_palettes));
    // Warm the present zone outside gameplay. Marco's branch transitions stay pinned.
    for (unsigned bank=0;bank<SCENE_BANK_COUNT;bank++) if (bank!=SCENE_SOLDIER) {
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
    debugf("MSX pack: %u pages, %u palettes, %u chunks; cache=%u KB DMA=%u bytes startup evictions=%u\n",SCENE_PAGE_COUNT,SCENE_PALETTE_COUNT,SCENE_CHUNK_COUNT,stats.cache_bytes/1024,stats.dma_bytes,stats.evictions);
    stats.misses=stats.evictions=0; // Startup waits/evictions are outside the gameplay deadline.
}
const scene_bank_t *scene_bank(unsigned id) { assert(id<SCENE_BANK_COUNT);return &scene_banks[id]; }
const scene_image_t *scene_frame(unsigned id,unsigned frame) {
    const scene_bank_t *b=scene_bank(id);assert(frame<b->first+b->second);return &scene_images[b->start+frame];
}
const scene_image_t *scene_background_tile(unsigned row,unsigned col) { assert(row<7 && col<10);return &scene_background[row*10+col]; }
const scene_map_cell_t *scene_map_cells(unsigned *count) { *count=SCENE_MAP_CELL_COUNT;return scene_map_cells_data; }
const scene_image_t *scene_map_image(unsigned id) {
    if (id&RAM_PAGE_BIT) {id&=~RAM_PAGE_BIT;assert(id<ram_count);return &ram_images[id];}
    assert(id<sizeof(scene_map_images)/sizeof(*scene_map_images));return &scene_map_images[id];
}
const scene_image_t *scene_map_cell_image(unsigned cell,unsigned phase) {
    assert(cell<SCENE_MAP_CELL_COUNT && phase<8);
    int replacement=cell_replacement[cell];
    return replacement<0?scene_map_image(scene_map_cells_data[cell].phases[phase]):&ram_images[replacement];
}
const scene_map_cell_t *scene_map_extra_cells(unsigned *count) {*count=extra_count;return extra_cells;}
void scene_clear_replacements(void) {
    /* Every previous frame ends in detach/full-sync. Fence before freeing or
     * mutating RAM textures that its queued RDP commands may still reference. */
    rspq_wait();
    for (unsigned i=0;i<ram_count;i++) {free(ram_pages[i]);ram_pages[i]=NULL;}
    ram_count=extra_count=0;memset(cell_replacement,255,sizeof(cell_replacement));
}
bool scene_replace_foreground_tile(int x,int y,const uint16_t rgba[256]) {
    if (!rgba || x%16 || y%16 || rdpq_is_attached()) return false;
    int found=-1;
    for (unsigned i=0;i<ram_count;i++) {
        const scene_map_cell_t *c=&ram_cells[i];
        if (x>=c->x && x+16<=c->x+c->size && y>=c->y && y+16<=c->y+c->size) {found=(int)i;break;}
    }
    rspq_wait();
    if (found<0) {
        if (ram_count==MAX_REPLACEMENTS) return false;
        int original=-1;scene_map_cell_t cell={.layer=3,.size=SCENE_MAP_CELL_SIZE};
        for (unsigned i=0;i<SCENE_MAP_CELL_COUNT;i++) {
            const scene_map_cell_t *c=&scene_map_cells_data[i];
            if (c->layer==3 && x>=c->x && x+16<=c->x+c->size && y>=c->y && y+16<=c->y+c->size) {
                original=(int)i;cell=*c;break;
            }
        }
        if (original<0) {
            int size=cell.size;
            cell.x=(int16_t)((x>=0?x/size:(x-size+1)/size)*size);
            cell.y=(int16_t)((y>=0?y/size:(y-size+1)/size)*size);
        }
        /* Current material is static. Do not freeze a future animated block
         * by turning its other quadrants into a single snapshot. */
        if (original>=0) for (unsigned phase=1;phase<8;phase++)
            if (cell.phases[phase]!=cell.phases[0]) return false;
        uint16_t *pixels=memalign(64,SCENE_PAGE_BYTES);if (!pixels) return false;
        memset(pixels,0,SCENE_PAGE_BYTES);
        if (original>=0) {
            const scene_image_t *image=scene_map_image(cell.phases[0]);
            const uint8_t *source=scene_page(image);
            const uint16_t *palette=image->format==2?scene_ci8_palette(image->palette):
                image->format==0?scene_palette(image->palette):NULL;
            if (image->format>2) {free(pixels);return false;}
            for (unsigned row=0;row<cell.size;row++) for (unsigned col=0;col<cell.size;col++) {
                unsigned sx=image->x+col,sy=image->y+row;uint16_t value;
                if (image->format==1) value=((const uint16_t*)source)[sy*32+sx];
                else {
                    unsigned pen=image->format==2?source[sy*32+sx]:
                        (source[sy*32+sx/2]>>((sx&1)?0:4))&15u;
                    value=palette[pen];
                }
                pixels[row*32+col]=value;
            }
        }
        found=(int)ram_count++;
        ram_pages[found]=pixels;ram_cells[found]=cell;
        ram_images[found]=(scene_image_t){.page=(uint16_t)(RAM_PAGE_BIT|found),.palette=UINT16_MAX,
            .width=cell.size,.height=cell.size,.format=1};
        if (original>=0) cell_replacement[original]=(int16_t)found;
        else {
            for (unsigned phase=0;phase<8;phase++) cell.phases[phase]=(uint16_t)(RAM_PAGE_BIT|found);
            extra_cells[extra_count++]=cell;
        }
    }
    const scene_map_cell_t *cell=&ram_cells[found];
    for (unsigned row=0;row<16;row++) memcpy(ram_pages[found]+(y-cell->y+row)*32+x-cell->x,rgba+row*16,32);
    data_cache_hit_writeback(ram_pages[found],SCENE_PAGE_BYTES);return true;
}
unsigned scene_map_camera_max(void) { return SCENE_MAP_CAMERA_MAX; }
unsigned scene_map_cell_size(void) { return SCENE_MAP_CELL_SIZE; }
uint32_t scene_map_backdrop(void) { return SCENE_MAP_BACKDROP; }
const uint16_t *scene_palette(unsigned id) { assert(id<SCENE_PALETTE_COUNT);return scene_palettes[id]; }
const uint16_t *scene_ci8_palette(unsigned id) {
    assert(id<sizeof(scene_ci8_palettes)/sizeof(scene_ci8_palettes[0]));return scene_ci8_palettes[id];
}
void scene_begin_epoch(unsigned next) { epoch=next; }
void scene_complete_epoch(unsigned done) { completed=done; }
scene_cache_stats_t scene_cache_stats(void) { return stats; }
