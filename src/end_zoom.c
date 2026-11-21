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
    STRIP_SLOTS=512,      /* 16 x 16 CI4 textures, 128 B each: 64 KB */
    STRIP_HASH=1024,
    TILE_SLOTS=64,        /* C-ROM tiles read from the file, 128 B each */
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
static uint8_t *tiles;               /* TILE_SLOTS x 128 */
static int32_t tile_key[TILE_SLOTS];static uint32_t tile_used[TILE_SLOTS];
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
    hash=malloc(sizeof(int16_t)*STRIP_HASH);tiles=malloc(TILE_SLOTS*128);rows=malloc(ROW_SLOTS*256);
    if (!meta || !textures || !strips || !hash || !tiles || !rows || fread(meta,1,n,file)!=n || !end_zoom_parse(&ez,h,meta,n)) {
        debugf("MSX ENDZOOM bad file or no memory\n");return false;
    }
    memset(strips,0,sizeof(strip_slot_t)*STRIP_SLOTS);memset(hash,0,sizeof(int16_t)*STRIP_HASH);
    for (unsigned i=0;i<STRIP_SLOTS;i++) strips[i].hash=-1;
    for (unsigned i=0;i<TILE_SLOTS;i++) tile_key[i]=-1;
    for (unsigned i=0;i<ROW_SLOTS;i++) row_key[i]=-1;
    st.cache_bytes=STRIP_SLOTS*128+sizeof(strip_slot_t)*STRIP_SLOTS+sizeof(int16_t)*STRIP_HASH+TILE_SLOTS*128+ROW_SLOTS*256+n;
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
static const uint8_t *tile_data(int index) {
    unsigned v=0;
    for (unsigned i=0;i<TILE_SLOTS;i++) {
        if (tile_key[i]==index) {tile_used[i]=++clock_lru;return tiles+128*i;}
        if (tile_used[i]<tile_used[v]) v=i;
    }
    fseek(file,(long)(ez.tiles_offset+128u*(unsigned)index),SEEK_SET);fread(tiles+128*v,1,128,file);st.file_reads++;
    tile_key[v]=index;tile_used[v]=++clock_lru;
    return tiles+128*v;
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
    /* LRU victim among the slots no frame in flight still reads (the RDP may draw the previous frame's list). */
    int v=-1;
    for (int i=0;i<STRIP_SLOTS;i++) {
        if (strips[i].frame+2>frame_no && strips[i].used) continue;
        if (v<0 || strips[i].used<strips[v].used) v=i;
    }
    if (v<0 || free_h<0) return -1;
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
        int t=end_zoom_find_tile(&ez,s->tile);
        if (t<0) {st.missing_tiles++;hash_remove(slot);strips[slot].used=0;return;}
        end_zoom_compose(s,tile_data(t),tex);data_cache_hit_writeback(tex,128);st.misses++;
    } else st.hits++;
    strips[slot].used=++clock_lru;strips[slot].frame=frame_no;
    /* raster line 8 is the console's line 0 (the 224 visible lines are 16..239 and 8..231 here) */
    scene_render_ci4_strip(tex,s->zx+1u,s->n,cur_palette,x,line-8);
    st.strips++;
}
void end_zoom_frame(void) {
    if (frame_ticks>st.worst_frame_ticks) st.worst_frame_ticks=frame_ticks;
    st.last_frame_ticks=frame_ticks;frame_ticks=0;frame_no++;
}
bool end_zoom_draw(uint32_t descriptor,uint16_t x,uint16_t y,uint8_t zoom_x,uint8_t zoom_y,uint16_t attributes,unsigned palette) {
    if (!tried) ready=open_file();
    if (!ready) return false;
    int k=end_zoom_find_descriptor(&ez,descriptor);
    if (k<0) {st.missing_descriptors++;return false;}
    uint32_t t0=TICKS_READ();
    cur_palette=palette;
    end_zoom_io_t io={l0_row,strip,NULL,0};
    if (!end_zoom_entry(&ez,k,x,y,(uint16_t)(zoom_x<<8|zoom_y),attributes,8,247,&io)) st.overflow++;
    st.auto_animated+=io.auto_animated;st.entries++;
    frame_ticks+=TICKS_READ()-t0;
    return true;
}
const end_zoom_stats_t *end_zoom_stats(void) {return &st;}
