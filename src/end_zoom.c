/* The ending's shrunk sprites on the console (end_zoom.h): end_zoom_core.c's strips, composed on first use into small
 * CI4 textures kept in a fixed LRU cache of their own (allocated at the first shrunk draw, after the scene cache has
 * been sized: the scene cache keeps its slots), drawn 1:1. */
#include <libdragon.h>
#include <string.h>
#include <malloc.h>
#include "end_zoom.h"
#include "end_zoom_core.h"
#include "scene_renderer.h"
enum {
    STRIP_SLOTS=384,      /* composed strips, 16 x 16 CI4, 128 B each: 48 KB (read by the CPU only, into the columns) */
    STRIP_HASH=1024,
    /* A column's strips are copied into one CI4 texture 16 pixels wide (one upload, one rectangle per column);
     * the RDP reads a frame's columns while the next frame is built: two halves, one per frame. 52 608 B is the
     * most a frame of the captures needs (T1 / ZC / ZT: 8 bytes per line from the first to the last drawn line of
     * every shrunk column). */
    RING_HALF=56*1024,
    BLOCK_BUDGET=88*1024, /* descriptors' tiles read whole (the largest, the dial's plane 1D6B44, is 488 tiles: 61 KB) */
    DESCRIPTORS_MAX=64,
    ROW_SLOTS=8,          /* L0 rows, 256 B each */
};
typedef struct { end_zoom_strip_t key;uint32_t used;uint32_t frame;int16_t hash; } strip_slot_t;
static bool tried,ready;
static FILE *file;
static end_zoom_file_t ez;
static uint8_t *meta;
static uint8_t *textures;            /* STRIP_SLOTS x 128 */
static strip_slot_t *strips;
static int16_t *hash;                /* slot index + 1, 0 empty, -1 deleted */
static uint8_t *block[DESCRIPTORS_MAX];static uint32_t block_used[DESCRIPTORS_MAX],block_bytes;
static unsigned hand;
static uint8_t *ring,*column;static uint32_t ring_used;static int col_x,col_w,col_lo,col_hi;
static int cur_desc;
static void *work;
static uint8_t *rows;                /* ROW_SLOTS x 256 */
static int16_t row_key[ROW_SLOTS];static uint32_t row_used[ROW_SLOTS];
static uint32_t clock_lru,frame_no;
static end_zoom_stats_t st;
static uint32_t frame_ticks;
static unsigned cur_palette;
static bool open_file(void) {
    tried=true;
    file=fopen("rom:/endzoom.bin","rb");
    if (!file) {debugf("MSX ENDZOOM no rom:/endzoom.bin: shrunk sprites scaled\n");return false;}
    uint8_t h[16];
    if (fread(h,1,16,file)!=16) return false;
    uint32_t n=end_zoom_rest_size(h);
    meta=malloc(n);textures=memalign(16,STRIP_SLOTS*128);strips=malloc(sizeof(strip_slot_t)*STRIP_SLOTS);
    hash=malloc(sizeof(int16_t)*STRIP_HASH);rows=malloc(ROW_SLOTS*256);work=malloc(END_ZOOM_WORK_BYTES);
    ring=memalign(16,2*RING_HALF);column=malloc(240*8);
    if (!meta || !textures || !strips || !hash || !rows || !work || !ring || !column || fread(meta,1,n,file)!=n || !end_zoom_parse(&ez,h,meta,n)) {
        debugf("MSX ENDZOOM bad file or no memory\n");return false;
    }
    memset(strips,0,sizeof(strip_slot_t)*STRIP_SLOTS);memset(hash,0,sizeof(int16_t)*STRIP_HASH);
    for (unsigned i=0;i<STRIP_SLOTS;i++) strips[i].hash=-1;
    if (ez.descriptors>DESCRIPTORS_MAX) {debugf("MSX ENDZOOM %u descriptors: more than %u\n",ez.descriptors,DESCRIPTORS_MAX);return false;}
    for (unsigned i=0;i<ROW_SLOTS;i++) row_key[i]=-1;
    memset(column,0,240*8);col_lo=240;col_hi=-1;
    st.cache_bytes=STRIP_SLOTS*128+sizeof(strip_slot_t)*STRIP_SLOTS+sizeof(int16_t)*STRIP_HASH+BLOCK_BUDGET+ROW_SLOTS*256+END_ZOOM_WORK_BYTES+2*RING_HALF+240*8+n;
    debugf("MSX ENDZOOM loaded: %u descriptors, %u tiles, %u KB of cache and tables\n",ez.descriptors,ez.tiles,(unsigned)(st.cache_bytes/1024));
    return true;
}
static const uint8_t *l0_row(void *ctx,unsigned zy) {
    (void)ctx;unsigned v=0;
    for (unsigned i=0;i<ROW_SLOTS;i++) {
        if (row_key[i]==(int)zy) {row_used[i]=++clock_lru;return rows+256*i;}
        if (row_used[i]<row_used[v]) v=i;
    }
    fseek(file,(long)(ez.l0_offset+256*zy),SEEK_SET);fread(rows+256*v,1,256,file);st.file_reads++;
    row_key[v]=(int16_t)zy;row_used[v]=++clock_lru;
    return rows+256*v;
}
/* Descriptor k's tiles in RAM (one read), the least recently used descriptors' freed to stay within BLOCK_BUDGET. */
static bool load_block(int k) {
    if (block[k]) {block_used[k]=++clock_lru;return true;}
    uint32_t first,count;end_zoom_descriptor_tiles(&ez,k,&first,&count);
    uint32_t need=128*count;
    while (block_bytes+need>BLOCK_BUDGET) {
        int v=-1;
        for (int i=0;i<(int)ez.descriptors;i++) if (block[i] && (v<0 || block_used[i]<block_used[v])) v=i;
        if (v<0) break;
        uint32_t f2,c2;end_zoom_descriptor_tiles(&ez,v,&f2,&c2);
        free(block[v]);block[v]=NULL;block_bytes-=128*c2;
    }
    if (block_bytes+need>BLOCK_BUDGET || !(block[k]=malloc(need))) return false;
    fseek(file,(long)(ez.tiles_offset+128*first),SEEK_SET);fread(block[k],1,need,file);st.file_reads++;
    block_bytes+=need;block_used[k]=++clock_lru;
    return true;
}
static uint32_t key_hash(const end_zoom_strip_t *k) {
    uint32_t h=k->tile*2654435761u^((uint32_t)k->flips<<28)^((uint32_t)k->zx<<24)^((uint32_t)k->n<<16);
    for (unsigned i=0;i<k->n;i++) h=(h^k->line[i])*16777619u;
    return h;
}
static bool same(const end_zoom_strip_t *a,const end_zoom_strip_t *b) {
    return a->tile==b->tile && a->flips==b->flips && a->zx==b->zx && a->n==b->n && !memcmp(a->line,b->line,a->n);
}
static void hash_remove(int slot) {
    int h=strips[slot].hash;
    if (h>=0) hash[h]=-1;
    strips[slot].hash=-1;
}
static int lookup(const end_zoom_strip_t *k,bool *hit) {
    uint32_t h=key_hash(k)&(STRIP_HASH-1);int free_h=-1;
    for (unsigned probe=0;probe<STRIP_HASH;probe++,h=(h+1)&(STRIP_HASH-1)) {
        int e=hash[h];
        if (e==0) {if (free_h<0) free_h=(int)h;break;}
        if (e<0) {if (free_h<0) free_h=(int)h;continue;}
        if (same(&strips[e-1].key,k)) {*hit=true;return e-1;}
    }
    *hit=false;
    /* victim: the clock hand's slot (a strip is copied into its column at once: no slot is still being read) */
    int v=(int)hand;hand=(hand+1)%STRIP_SLOTS;
    if (free_h<0) return -1;
    hash_remove(v);
    strips[v].key=*k;strips[v].hash=(int16_t)free_h;hash[free_h]=(int16_t)(v+1);
    return v;
}
static void strip(void *ctx,const end_zoom_strip_t *s,int x,int line) {
    (void)ctx;bool hit;
    int slot=lookup(s,&hit);
    if (slot<0) {st.no_slot++;return;}
    uint8_t *tex=textures+128*slot;
    if (!hit) {
        int t=end_zoom_find_tile(&ez,cur_desc,s->tile);
        if (t<0) {st.missing_tiles++;hash_remove(slot);strips[slot].used=0;return;}
        uint32_t first,count;end_zoom_descriptor_tiles(&ez,cur_desc,&first,&count);
        uint32_t c0=TICKS_READ();end_zoom_compose(s,block[cur_desc]+128*((uint32_t)t-first),tex);st.compose_ticks+=TICKS_READ()-c0;st.misses++;
    } else st.hits++;
    strips[slot].used=++clock_lru;strips[slot].frame=frame_no;
    /* into the column: raster line 8 is the console's line 0 (the 224 visible lines are 16..239 and 8..231 here) */
    col_x=x;col_w=s->zx+1;
    for (unsigned i=0;i<s->n;i++) {
        int y=line-8+(int)i;
        if (y<0 || y>=240) continue;
        memcpy(column+8*y,tex+8*i,8);
        if (y<col_lo) col_lo=y;
        if (y>col_hi) col_hi=y;
    }
    st.strips++;
}
static void column_end(void *ctx) {
    (void)ctx;
    if (col_hi<col_lo) return;
    uint32_t bytes=8u*(uint32_t)(col_hi-col_lo+1);
    if (ring_used+bytes>RING_HALF) st.no_slot++;
    else {
        uint8_t *tex=ring+(frame_no&1u)*RING_HALF+ring_used;ring_used+=bytes;
        memcpy(tex,column+8*col_lo,bytes);data_cache_hit_writeback(tex,bytes);
        uint32_t d0=TICKS_READ();scene_render_ci4_strip(tex,(unsigned)col_w,(unsigned)(col_hi-col_lo+1),cur_palette,col_x,col_lo);st.draw_ticks+=TICKS_READ()-d0;
        st.columns++;
        if (ring_used>st.ring_peak) st.ring_peak=ring_used;
    }
    memset(column+8*col_lo,0,bytes);col_lo=240;col_hi=-1;
}
void end_zoom_frame(void) {
    if (frame_ticks>st.worst_frame_ticks) st.worst_frame_ticks=frame_ticks;
    st.last_frame_ticks=frame_ticks;frame_ticks=0;frame_no++;ring_used=0;
}
bool end_zoom_draw(uint32_t descriptor,uint16_t x,uint16_t y,uint8_t zoom_x,uint8_t zoom_y,uint16_t attributes,unsigned palette) {
    if (!tried) ready=open_file();
    if (!ready) return false;
    int k=end_zoom_find_descriptor(&ez,descriptor);
    if (k<0) {st.missing_descriptors++;return false;}
    uint32_t t0=TICKS_READ();
    if (!load_block(k)) {st.no_block++;frame_ticks+=TICKS_READ()-t0;return false;}
    cur_palette=palette;cur_desc=k;
    end_zoom_io_t io={l0_row,strip,column_end,NULL,0,work};
    if (!end_zoom_entry(&ez,k,x,y,(uint16_t)(zoom_x<<8|zoom_y),attributes,8,247,&io)) st.overflow++;
    st.auto_animated+=io.auto_animated;st.entries++;
    frame_ticks+=TICKS_READ()-t0;st.total_ticks+=TICKS_READ()-t0;
    return true;
}
const end_zoom_stats_t *end_zoom_stats(void) {return &st;}
