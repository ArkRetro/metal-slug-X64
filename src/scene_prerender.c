#include "scene_prerender.h"
#if MSX_BG_PRERENDER
#include <libdragon.h>
#include <string.h>
#include <stdio.h>
/* See scene_prerender.h. Layout of the file: tools/prerender_stage.py. */
enum { RING_SLOTS=32, LOAD_PAD=2048 };   /* a LOAD_BLOCK reads 2048 bytes from a run's start */
typedef struct { uint32_t offset;uint16_t bytes,runs; } column_t;
typedef struct {
    bool on;int x0;unsigned columns,slot_bytes;column_t *table;
    uint8_t *ring;int slot_column[RING_SLOTS];uint8_t slot_state[RING_SLOTS];   /* 0 empty, 1 DMA started, 2 ready */
} layer_t;
static layer_t layers[4];static uint32_t rom;static unsigned mask,palette_base,ring_total,sync_reads,async_reads;
static inline uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|p[1]<<16|p[2]<<8|p[3];}
static inline uint16_t rd16(const uint8_t *p) {return (uint16_t)(p[0]<<8|p[1]);}
static void rom_read(void *dst,uint32_t offset,unsigned length) {
    data_cache_hit_writeback_invalidate(dst,(length+15)&~15u);dma_read(dst,rom+offset,length);
}
void scene_prerender_close(void) {
    if (!mask) return;
    rspq_wait();   /* the RDP may still read the rings (their memory: the stage's reserved slots) */
    memset(layers,0,sizeof(layers));mask=0;ring_total=0;
}
static uint8_t head[80] __attribute__((aligned(16)));static bool head_ok;
static inline unsigned layer_bytes(const uint8_t *h) {unsigned n=rd16(h+2);return n?((8*n+15)&~15u)+RING_SLOTS*rd16(h+4)+LOAD_PAD:0u;}
unsigned scene_prerender_probe(unsigned mission,unsigned index) {
    scene_prerender_close();head_ok=false;
    char path[24];snprintf(path,sizeof(path),"/m%us%u.bgp",mission,index+1);
    uint32_t at=(uint32_t)dfs_rom_addr(path);
    if (!at) {debugf("MSX prerender %s missing: tile path\n",path);return 0;}
    rom=PhysicalAddr((void*)(uintptr_t)at);rom_read(head,0,sizeof(head));
    if (memcmp(head,"MSXB",4) || rd16(head+4)!=1 || rd16(head+6)!=index) {debugf("MSX prerender %s: bad header\n",path);return 0;}
    head_ok=true;unsigned bytes=0;
    for (unsigned l=0;l<4;l++) bytes+=layer_bytes(head+16+16*l);
    return bytes;
}
bool scene_prerender_open(uint8_t *mem,unsigned base) {
    scene_prerender_close();palette_base=base;
    if (!head_ok || !mem) return false;
    for (unsigned l=0;l<4;l++) {
        const uint8_t *h=head+16+16*l;layer_t *ly=&layers[l];
        unsigned n=rd16(h+2);if (!n) continue;
        ly->x0=(int16_t)rd16(h);ly->columns=n;ly->slot_bytes=rd16(h+4);
        ly->table=(column_t*)mem;rom_read(ly->table,rd32(h+8),8*n);   /* column_t is the file's big-endian record */
        ly->ring=mem+((8*n+15)&~15u);
        memset(ly->ring+RING_SLOTS*ly->slot_bytes,0,LOAD_PAD);data_cache_hit_writeback_invalidate(ly->ring,RING_SLOTS*ly->slot_bytes+LOAD_PAD);
        for (unsigned s=0;s<RING_SLOTS;s++) {ly->slot_column[s]=-1;ly->slot_state[s]=0;}
        ly->on=true;mask|=1u<<l;ring_total+=layer_bytes(h);mem+=layer_bytes(h);
    }
    debugf("MSX prerender: layers %x, ring %u KB in the stage's slots\n",mask,ring_total/1024);
    return mask!=0;
}
unsigned scene_prerender_layers(void) {return mask;}
unsigned scene_prerender_palette_base(void) {return palette_base;}
static inline int floor_div16(int v) {return v>=0?v/16:-((15-v)/16);}
void scene_prerender_range(unsigned layer,int x0,int x1,int *first,int *end,int *column_x) {
    const layer_t *ly=&layers[layer];
    int a=floor_div16(x0-ly->x0+15),b=floor_div16(x1-ly->x0+15);
    if (a<0) a=0;
    if (b>(int)ly->columns) b=(int)ly->columns;
    *first=a;*end=b<a?a:b;*column_x=ly->x0;
}
/* Slot `s` takes column `c` (DMA started; false: the column is empty). */
static bool start_at(layer_t *ly,unsigned s,int c) {
    const column_t *t=&ly->table[c];
    ly->slot_column[s]=c;
    if (!t->bytes) {ly->slot_state[s]=2;return false;}
    uint8_t *dst=ly->ring+s*ly->slot_bytes;
    data_cache_hit_invalidate(dst,ly->slot_bytes);   /* the CPU read the old runs */
    dma_read_async(dst,rom+t->offset,t->bytes);ly->slot_state[s]=1;
    return true;
}
static bool start(layer_t *ly,int c) {return start_at(ly,(unsigned)c%RING_SLOTS,c);}
const scene_prerender_run_t *scene_prerender_column(unsigned layer,int c,unsigned *runs,const uint8_t **texels) {
    return scene_prerender_column_slot(layer,(unsigned)c%RING_SLOTS,c,runs,texels);
}
const scene_prerender_run_t *scene_prerender_column_slot(unsigned layer,unsigned s,int c,unsigned *runs,const uint8_t **texels) {
    layer_t *ly=&layers[layer];s%=RING_SLOTS;
    if (ly->slot_column[s]!=c) {if (start_at(ly,s,c)) sync_reads++;}
    if (ly->slot_state[s]==1) {dma_wait();for (unsigned k=0;k<RING_SLOTS;k++) if (ly->slot_state[k]==1) ly->slot_state[k]=2;}
    const column_t *t=&ly->table[c];
    if (!t->bytes) return NULL;
    const uint8_t *base=ly->ring+s*ly->slot_bytes;
    *runs=t->runs;*texels=base+((8u*t->runs+15)&~15u);
    return (const scene_prerender_run_t*)base;
}
void scene_prerender_prefetch(unsigned layer,int x0,int x1) {
    layer_t *ly=&layers[layer];
    if (!ly->on) return;
    int first,end,cx;scene_prerender_range(layer,x0,x1,&first,&end,&cx);
    for (int c=first;c<end && c<first+RING_SLOTS;c++) if (ly->slot_column[(unsigned)c%RING_SLOTS]!=c && start(ly,c)) async_reads++;
}
void scene_prerender_prefetch_slot(unsigned layer,unsigned s,int c) {
    layer_t *ly=&layers[layer];s%=RING_SLOTS;
    if (ly->on && c>=0 && c<(int)ly->columns && ly->slot_column[s]!=c && start_at(ly,s,c)) async_reads++;
}
int scene_prerender_column_of(unsigned layer,int x) {
    const layer_t *ly=&layers[layer];
    if (!ly->on || x<ly->x0 || (x-ly->x0)%16) return -1;
    int c=(x-ly->x0)/16;return c<(int)ly->columns?c:-1;
}
void scene_prerender_stats(unsigned *ring_bytes,unsigned *sync,unsigned *async) {*ring_bytes=ring_total;*sync=sync_reads;*async=async_reads;}
#endif
