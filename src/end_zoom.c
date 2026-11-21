/* The ending's shrunk sprites on the console (end_zoom.h): exact RDP texture rectangles (end_zoom_rdp.c) from the
 * descriptor's tiles as they are, in 16 x 256 CI4 pages (16 tiles), each page loaded once per run of rectangles.
 * Memory is taken at the first shrunk draw, after the scene cache has been sized (the scene cache keeps its slots). */
#include <libdragon.h>
#include <string.h>
#include <malloc.h>
#include "end_zoom.h"
#include "end_zoom_core.h"
#include "end_zoom_rdp.h"
#include "scene_renderer.h"
enum {
    BLOCK_BUDGET=96*1024,   /* descriptors' tiles in pages (the dial's plane 1D6B44: 488 tiles, 31 pages, 62 KB) */
    DESCRIPTORS_MAX=64,
};
static bool tried,ready;
static FILE *file;
static end_zoom_file_t ez;
static end_zoom_rdp_t rt;
static uint8_t *meta,*tables;
static void *work;static end_zoom_rect_t *sorted;static uint16_t (*local_work)[32];static uint16_t *local_map[DESCRIPTORS_MAX];
static uint8_t *block[DESCRIPTORS_MAX];static uint32_t block_frame[DESCRIPTORS_MAX],block_used[DESCRIPTORS_MAX],block_bytes;
static uint32_t clock_lru,frame_no;
static end_zoom_stats_t st;
static uint32_t frame_ticks,frame_rects,frame_loads;
static unsigned cur_palette;static int cur_desc,cur_page;
static uint32_t pages_bytes(int k) {uint32_t f,c;end_zoom_descriptor_tiles(&ez,k,&f,&c);return 2048u*((c+15)/16);}
static bool open_file(void) {
    tried=true;
    file=fopen("rom:/endzoom.bin","rb");
    if (!file) {debugf("MSX ENDZOOM no rom:/endzoom.bin: shrunk sprites scaled\n");return false;}
    uint8_t h[16];
    if (fread(h,1,16,file)!=16) return false;
    uint32_t n=end_zoom_rest_size(h);
    meta=malloc(n);work=malloc(END_ZOOM_WORK_BYTES);sorted=malloc(2*sizeof(end_zoom_rect_t)*END_ZOOM_RECTS);local_work=malloc(sizeof(uint16_t)*32*END_ZOOM_COLUMNS);
    if (!meta || !work || !sorted || !local_work || fread(meta,1,n,file)!=n || !end_zoom_parse(&ez,h,meta,n)) {debugf("MSX ENDZOOM bad file or no memory\n");return false;}
    if (ez.descriptors>DESCRIPTORS_MAX) {debugf("MSX ENDZOOM %u descriptors: more than %u\n",ez.descriptors,DESCRIPTORS_MAX);return false;}
    fseek(file,0,SEEK_END);long size=ftell(file);uint32_t at=ez.l0_offset+65536,tn=(uint32_t)size-at;
    tables=malloc(tn);
    if (!tables) return false;
    fseek(file,(long)at,SEEK_SET);
    if (fread(tables,1,tn,file)!=tn || !end_zoom_rdp_parse(&rt,tables,tn)) {debugf("MSX ENDZOOM no RDP tables\n");return false;}
    st.cache_bytes=n+tn+END_ZOOM_WORK_BYTES+2*sizeof(end_zoom_rect_t)*END_ZOOM_RECTS+sizeof(uint16_t)*32*END_ZOOM_COLUMNS+BLOCK_BUDGET;
    debugf("MSX ENDZOOM loaded: %u descriptors, %u tiles, %u sequences, %u KB (tiles up to %u KB)\n",ez.descriptors,ez.tiles,rt.nseq,(unsigned)(st.cache_bytes/1024),BLOCK_BUDGET/1024);
    return true;
}
/* Descriptor k's tiles in RAM (one read, padded to whole pages); the least recently used descriptors not drawn in
 * this frame or the previous one (the RDP may still read those) are freed to stay within BLOCK_BUDGET. */
static bool load_block(int k) {
    if (block[k]) {block_used[k]=++clock_lru;block_frame[k]=frame_no;return true;}
    uint32_t first,count;end_zoom_descriptor_tiles(&ez,k,&first,&count);
    uint32_t need=pages_bytes(k);
    while (block_bytes+need>BLOCK_BUDGET) {
        int v=-1;
        for (int i=0;i<(int)ez.descriptors;i++)
            if (block[i] && block_frame[i]+2<=frame_no && (v<0 || block_used[i]<block_used[v])) v=i;
        if (v<0) break;
        free(block[v]);block[v]=NULL;block_bytes-=pages_bytes(v);free(local_map[v]);local_map[v]=NULL;
    }
    if (block_bytes+need>BLOCK_BUDGET || !(block[k]=memalign(16,need))) return false;
    memset(block[k]+128*count,0,need-128*count);
    fseek(file,(long)(ez.tiles_offset+128*first),SEEK_SET);fread(block[k],1,128*count,file);st.file_reads++;
    data_cache_hit_writeback_invalidate(block[k],need);
    /* the descriptor's words -> local tile index (end_zoom_rdp_local_map), read with the block */
    uint32_t nw=end_zoom_descriptor_words(&ez,k);
    if ((local_map[k]=malloc(2*(nw?nw:1)))) end_zoom_rdp_local_map(&ez,k,local_map[k]);
    block_bytes+=need;block_used[k]=++clock_lru;block_frame[k]=frame_no;
    return true;
}
static void rect(void *ctx,const end_zoom_rect_t *r) {
    (void)ctx;uint32_t e0=TICKS_READ();
    if (r->page!=cur_page) {scene_render_zoom_page(block[cur_desc]+2048u*r->page,cur_palette);cur_page=r->page;frame_loads++;st.loads++;}
    scene_render_zoom_rect(r->x0,r->y0,r->x1,r->y1,r->s0,r->t0,r->dsdx,r->dtdy);
    frame_rects++;st.emit_ticks+=TICKS_READ()-e0;
}
void end_zoom_frame(void) {
    if (frame_ticks>st.worst_frame_ticks) st.worst_frame_ticks=frame_ticks;
    if (frame_ticks>st.window_ticks) st.window_ticks=frame_ticks;
    if (frame_rects>st.window_rects) st.window_rects=frame_rects;
    if (frame_loads>st.window_loads) st.window_loads=frame_loads;
    st.last_frame_ticks=frame_ticks;frame_ticks=frame_rects=frame_loads=0;frame_no++;
}
void end_zoom_window_reset(void) {st.window_ticks=st.window_rects=st.window_loads=0;}
bool end_zoom_draw(uint32_t descriptor,uint16_t x,uint16_t y,uint8_t zoom_x,uint8_t zoom_y,uint16_t attributes,unsigned palette) {
    if (!tried) ready=open_file();
    if (!ready) return false;
    int k=end_zoom_find_descriptor(&ez,descriptor);
    if (k<0) {st.missing_descriptors++;return false;}
    uint32_t t0=TICKS_READ();
    if (!load_block(k)) {st.no_block++;frame_ticks+=TICKS_READ()-t0;return false;}
    cur_palette=palette;cur_desc=k;cur_page=-1;
    end_zoom_rdp_io_t io={rect,NULL,work,sorted,END_ZOOM_RECTS,local_map[k],local_work,0,0,0,0,0};
    if (!end_zoom_rdp_entry(&ez,&rt,k,x,y,(uint16_t)(zoom_x<<8|zoom_y),attributes,&io)) st.overflow++;
    st.rects+=io.rects;st.refits+=io.refits;st.unfit+=io.unfit;st.auto_animated+=io.auto_animated;st.missing_tiles+=io.missing_tiles;st.entries++;
    uint32_t dt=TICKS_READ()-t0;frame_ticks+=dt;st.total_ticks+=dt;
    return true;
}
const end_zoom_stats_t *end_zoom_stats(void) {return &st;}
