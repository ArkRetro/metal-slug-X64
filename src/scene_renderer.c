#include "scene_renderer.h"
#include "retro_effects.h"
#include "mission_camera.h"

typedef struct { surface_t *surface; unsigned epoch,start_ticks; } completion_t;
static completion_t completions[4],*current;
static unsigned frame,clock_pal,draws,uploads,palette_uploads,tmem_bytes;
static int pal_ids[16],last_page,last_slot,last_format,last_width,last_height;
static unsigned pal_lru[16];
static bool depth_mode;
static retro_frame_t depth_frame;
static retro_plane_view_t depth_views[5];
void scene_render_set_depth_mode(bool enabled) {depth_mode=enabled;}
void scene_render_depth_frame(const retro_frame_t *f) {
    depth_frame=*f;
    // This presentation uses layered 2D scenery and contact shadows only.
    depth_frame.light_count=0;depth_frame.camera.z=0;
}
void scene_render_foreground_fog(void) {}
void scene_render_forget_texture(void) {
    last_page=last_slot=last_format=last_width=last_height=-1;
    for (unsigned i=0;i<16;i++) pal_ids[i]=-1;
}

void scene_renderer_init(void) {
    rdpq_text_register_font(1,rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO));
    retro_effects_init();
}
static void finished(void *arg) {
    completion_t *done=arg;
    scene_complete_epoch(done->epoch);
    display_show(done->surface);
    if (done->epoch==1) debugf("MSX first frame ready\n");
}
void scene_render_begin(surface_t *surface) {
    current=&completions[++frame%4];*current=(completion_t){surface,frame,TICKS_READ()};
    scene_begin_epoch(frame);scene_service();
    for (unsigned i=0;i<16;i++) { pal_ids[i]=-1;pal_lru[i]=0; }
    last_page=last_slot=last_format=last_width=last_height=-1;draws=uploads=palette_uploads=tmem_bytes=0;
    rdpq_attach_clear(surface,NULL);rdpq_set_mode_standard();
    // Every frame starts from a known state, whatever ran before it (the
    // parachute cinematic's blits, the CPU-drawn graphics lab, or font
    // uploads): full scissor, point sampling, no assumed TMEM/TLUT content.
    rdpq_set_scissor(0,0,320,240);
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
static void blit_scaled(const scene_image_t *image,float x,float y,int s,int t,int width,int height,float draw_width,float draw_height,bool mirror,bool flash) {
    if (x>=320 || y>=240 || x+draw_width<=0 || y+draw_height<=0) return;
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
    rdpq_texture_rectangle_scaled(TILE0,x,y,x+draw_width,y+draw_height,
        mirror?s+width-1:s,t,mirror?s-1:s+width,t+height);
    draws++;
}
static float background_x(const scene_map_cell_t *cell,int fallback) {
    if (!depth_mode) return fallback;
    const retro_plane_view_t *view=&depth_views[cell->layer];
    return cell->x*view->scale+view->offset_x;
}
static void blit(const scene_image_t *image,int x,int y,int s,int t,int width,int height,bool mirror,bool flash) {
    blit_scaled(image,x,y,s,t,width,height,width,height,mirror,flash);
}
void scene_render_image(const scene_image_t *image,int x,int y,bool mirror,bool flash) {
    rdpq_mode_filter(FILTER_POINT); // Sprites never inherit the fog sampler.
    blit(image,x,y,0,0,image->width,image->height,mirror,flash);
}
void scene_render_background(unsigned camera,unsigned video_tick) {
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    // Layer 4 holds the market props; it scrolls with the foreground.
    int height=msx_camera_height((int)camera),offsets[5],rises[5];bool visible[5];
    for (unsigned i=0;i<5;i++) {
        unsigned source=i<4?i:3;
        offsets[i]=msx_layer_scroll(source,(int)camera);rises[i]=msx_layer_scroll(source,height);
        visible[i]=msx_layer_visible(source,(int)camera);
    }
    // Keep the projection outside assert(): NDEBUG would otherwise drop it.
    if (depth_mode) for (unsigned i=0;i<5;i++) {
        bool ok=retro_layered_2d_projection(depth_frame.camera,i,&depth_views[i]);
        assertf(ok,"Retro plane projection");(void)ok;
    }
    // Each source cell is CI4 inside a 64x64 / 2048-byte palette atlas.
    // Prefetch a margin on both sides, including the next autoanimation phase.
    for (unsigned i=0;i<count;i++) {
        if (!visible[cells[i].layer]) continue;
        float x=background_x(&cells[i],cells[i].x-offsets[cells[i].layer]);
        if (x< -48 || x>=352) continue;
        scene_request_image(scene_map_image(cells[i].phases[video_tick&7]),2);
        scene_request_image(scene_map_image(cells[i].phases[(video_tick+1)&7]),1);
    }
    scene_service();
    rdpq_set_scissor(8,8,312,232);
    scene_render_box(8,8,304,224,color_from_packed32(scene_map_backdrop()));
    if (!depth_mode) {
        for (unsigned i=0;i<count;i++) {
            const scene_map_cell_t *cell=&cells[i];
            if (!visible[cell->layer]) continue;
            int x=cell->x-offsets[cell->layer],y=8+cell->y+rises[cell->layer];
            if (x+16<=8 || x>=312 || y+16<=8 || y>=232) continue;
            const scene_image_t *im=scene_map_image(cell->phases[video_tick&7]);
            blit(im,x,y,im->x,im->y,16,16,false,false);
        }
        rdpq_set_scissor(0,0,320,240);return;
    }
    for (unsigned layer=0;layer<5;layer++) {
      if (!visible[layer]) continue;
      for (unsigned i=0;i<count;i++) {
        const scene_map_cell_t *cell=&cells[i];
        if (cell->layer!=layer) continue;
        float x=background_x(cell,cell->x-offsets[cell->layer]);
        if (x+16*depth_views[layer].scale<=8 || x>=312) continue;
        const scene_image_t *im=scene_map_image(cell->phases[video_tick&7]);
        const retro_plane_view_t *view=&depth_views[layer];
        // The depth camera carries the height (actor.c), so planes keep their own parallax.
        float y=(8+cell->y)*view->scale+view->offset_y,size=16*view->scale;
        blit_scaled(im,x,y,im->x,im->y,16,16,size,size,false,false);
      }
    }
    retro_effects_mesh(&depth_frame);
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
        scene_cache_stats_t s=scene_cache_stats();(void)s;
        if (depth_mode) debugf("MSX DEPTH: triangles=%u limit=%u fog=%u B lights=%u fog_uploads=0 planes=%u shadows=%u shadow_tris=%u\n",
            retro_effects_triangles(),RETRO_MAX_TRIANGLES,0u,depth_frame.light_count,
            retro_effects_visible_planes(),depth_frame.shadow_count,retro_effects_shadow_triangles());
        debugf("MSX RDP: draws=%u tex=%u TLUT=%u TMEM=%u B CPU=%lu us PI=%u loads/%u bytes misses=%u evictions=%u\n",
            draws,uploads,palette_uploads,tmem_bytes,(unsigned long)TICKS_TO_US(TICKS_SINCE(current->start_ticks)),
            s.loads,s.dma_bytes,s.misses,s.evictions);
    }
    // One completion fence per frame: RDRAM pages stay pinned until RDP is done.
    rdpq_detach_cb(finished,current);
}
