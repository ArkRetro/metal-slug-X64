#include "scene_renderer.h"

typedef struct { surface_t *surface; unsigned epoch,start_ticks; } completion_t;
static completion_t completions[4],*current;
static unsigned frame,clock_pal,draws,uploads,palette_uploads,tmem_bytes;
static int pal_ids[16],last_page,last_slot,last_format,last_width,last_height;
static unsigned pal_lru[16];

void scene_renderer_init(void) {
    rdpq_text_register_font(1,rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO));
}
static void finished(void *arg) {
    completion_t *done=arg;
    scene_complete_epoch(done->epoch);
    display_show(done->surface);
}
void scene_render_begin(surface_t *surface) {
    current=&completions[++frame%4];*current=(completion_t){surface,frame,TICKS_READ()};
    scene_begin_epoch(frame);scene_service();
    for (unsigned i=0;i<16;i++) { pal_ids[i]=-1;pal_lru[i]=0; }
    last_page=last_slot=last_format=last_width=last_height=-1;draws=uploads=palette_uploads=tmem_bytes=0;
    rdpq_attach_clear(surface,NULL);rdpq_set_mode_standard();
    rdpq_mode_filter(FILTER_POINT);rdpq_mode_alphacompare(1);
}
static unsigned palette_slot(unsigned id) {
    for (unsigned i=0;i<16;i++) if (pal_ids[i]==(int)id) {pal_lru[i]=++clock_pal;return i;}
    unsigned slot=0;
    for (unsigned i=0;i<16;i++) {
        if (pal_ids[i]<0) {slot=i;break;}
        if (pal_lru[i]<pal_lru[slot]) slot=i;
    }
    rdpq_tex_upload_tlut((uint16_t*)scene_palette(id),slot*16,16);
    pal_ids[slot]=(int)id;pal_lru[slot]=++clock_pal;palette_uploads++;
    return slot;
}
static void blit(const scene_image_t *image,int x,int y,int s,int t,int width,int height,bool mirror,bool flash) {
    if (x>=320 || y>=240 || x+width<=0 || y+height<=0) return;
    unsigned slot=0;
    if (image->format==0) {rdpq_mode_tlut(TLUT_RGBA16);slot=palette_slot(flash?0:image->palette);}
    else rdpq_mode_tlut(TLUT_NONE);
    if (last_page!=image->page || last_slot!=(int)slot || last_format!=image->format || last_width!=image->width || last_height!=image->height) {
        const void *data=scene_page(image);
        unsigned side=image->format?32:64;
        surface_t texture=surface_make((void*)data,image->format?FMT_RGBA16:FMT_CI4,side,side,image->format?64:32);
        int bytes=rdpq_tex_upload_sub(TILE0,&texture,&(rdpq_texparms_t){.palette=(int)slot},0,0,image->width,image->height);
        assertf(bytes<=2048,"Scene TMEM page overflow");
        last_page=image->page;last_slot=(int)slot;last_format=image->format;
        last_width=image->width;last_height=image->height;uploads++;tmem_bytes+=(unsigned)bytes;
    }
    rdpq_texture_rectangle_scaled(TILE0,x,y,x+width,y+height,
        mirror?s+width:s,t,mirror?s:s+width,t+height);
    draws++;
}
void scene_render_image(const scene_image_t *image,int x,int y,bool mirror,bool flash) {
    blit(image,x,y,0,0,image->width,image->height,mirror,flash);
}
void scene_render_background(unsigned camera,unsigned video_tick) {
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    int offsets[]={(camera+1)/2,(camera*3+2)/4,camera,camera,camera};
    // Each source cell is CI4 inside a 64x64 / 2048-byte palette atlas.
    // Prefetch a margin on both sides, including the next autoanimation phase.
    for (unsigned i=0;i<count;i++) {
        int x=cells[i].x-offsets[cells[i].layer];
        if (x< -48 || x>=352) continue;
        scene_request_image(scene_map_image(cells[i].phases[video_tick&7]),2);
        scene_request_image(scene_map_image(cells[i].phases[(video_tick+1)&7]),1);
    }
    scene_service();
    rdpq_set_scissor(8,8,312,232);
    scene_render_box(8,8,304,224,color_from_packed32(scene_map_backdrop()));
    for (unsigned i=0;i<count;i++) {
        const scene_map_cell_t *cell=&cells[i];int x=cell->x-offsets[cell->layer];
        if (x+16<=8 || x>=312) continue;
        const scene_image_t *im=scene_map_image(cell->phases[video_tick&7]);
        blit(im,x,8+cell->y,im->x,im->y,16,16,false,false);
    }
    rdpq_set_scissor(0,0,320,240);
}
void scene_render_box(int x,int y,int width,int height,color_t color) {
    if (x>=320 || y>=240 || x+width<=0 || y+height<=0) return;
    rdpq_mode_push();rdpq_set_mode_fill(color);rdpq_fill_rectangle(x,y,x+width,y+height);rdpq_mode_pop();
}
void scene_render_text(int x,int y,const char *text) {
    rdpq_text_print(NULL,1,x,y+8,text);
    // Fonts can overwrite TMEM; do not retain stale texture/palette residency.
    last_page=last_slot=last_format=-1;
    for (unsigned i=0;i<16;i++) pal_ids[i]=-1;
}
void scene_render_end(void) {
    if (!(frame%300)) {
        scene_cache_stats_t s=scene_cache_stats();
        debugf("MSX RDP: draws=%u tex=%u TLUT=%u TMEM=%u B CPU=%lu us PI=%u loads/%u bytes misses=%u evictions=%u\n",
            draws,uploads,palette_uploads,tmem_bytes,(unsigned long)TICKS_TO_US(TICKS_SINCE(current->start_ticks)),
            s.loads,s.dma_bytes,s.misses,s.evictions);
    }
    // One completion fence per frame: RDRAM pages stay pinned until RDP is done.
    rdpq_detach_cb(finished,current);
}
