#include "scene_renderer.h"
#include "neo_fade.h"
#include "retro_effects.h"
#include "mission_camera.h"
#include "msx_perf.h"
#include "scene_cull.h"
#include "visual_config.h"

// Optional depth25d extras (MSX_DEPTH_STYLE bits); the release presentation
// is layered 2D scenery with contact shadows only (style 0).
enum { DEPTH_PERSPECTIVE=1, DEPTH_LIGHTS=2, DEPTH_FOG=4 };

typedef struct { surface_t *surface; unsigned epoch,start_ticks; } completion_t;
static completion_t completions[4],*current;
static unsigned frame,clock_pal,draws,uploads,palette_uploads,tmem_bytes;
static int pal_ids[16],last_page,last_slot,last_format,last_width,last_height;
static bool deferrable;
void scene_render_set_deferrable(bool on) { deferrable=on; }
/* Palette fade (neo_fade.c, RAM 10991A..): each TLUT uploaded while it is on
 * is a converted copy, every 5-bit channel plus its offset clamped to 0..31
 * like the original's palette conversion 005A0A. Copies live in a ring per
 * frame in flight (the RDP reads them later). */
enum { FADE_COPIES=128 };
static int8_t fade_offset[3];static bool fade_on;
static uint16_t fade_copies[4][FADE_COPIES][16] __attribute__((aligned(16)));
static unsigned fade_used,fade_ring,fade_overflow;
void scene_render_set_fade(int8_t r,int8_t g,int8_t b) {fade_offset[0]=r;fade_offset[1]=g;fade_offset[2]=b;fade_on=r||g||b;}
const uint16_t *scene_render_fade_palette(const uint16_t *source) {
    if (!fade_on) return source;
    if (fade_used==FADE_COPIES) {fade_overflow++;return source;}
    uint16_t *d=fade_copies[fade_ring][fade_used++];
    for (unsigned i=0;i<16;i++) {
        uint16_t c=source[i];
        unsigned red=neo_fade_channel((c>>11)&31u,fade_offset[0]),green=neo_fade_channel((c>>6)&31u,fade_offset[1]),blue=neo_fade_channel((c>>1)&31u,fade_offset[2]);
        d[i]=(uint16_t)(red<<11|green<<6|blue<<1|(c&1u));
    }
    data_cache_hit_writeback(d,sizeof(fade_copies[0][0]));
    return d;
}
static unsigned pal_lru[16];
// Render-mode values this renderer last set and still knows to be current
// (-1: unknown). Setting the same TLUT/filter again only re-sent identical
// SET_OTHER_MODES/SET_COMBINE and a pipe sync; draws and their state are the same.
static int mode_tlut=-1,mode_filter=-1;
static void set_tlut(rdpq_tlut_t tlut) {if (mode_tlut!=(int)tlut) {rdpq_mode_tlut(tlut);mode_tlut=(int)tlut;}}
static void set_point_filter(void) {if (mode_filter!=FILTER_POINT) {rdpq_mode_filter(FILTER_POINT);mode_filter=FILTER_POINT;}}
static bool depth_mode;
/* Collapse of the domed building (0B58CC, neo_ga_flags_t): the facade of
 * layer 3 (map 0B5392 at column 0x74) is replaced by the rubble map 0B53A8
 * (scene_collapse_cells; 10C72E then draws from SCB slot 65) and the sky
 * (10E83E) moves its column origin by +2 (the extracted cells are in map
 * coordinates: drawn 32 px further left).
 * The white flash (RAM 109944) turns every background palette to 0x7FFF. */
enum { COLLAPSE_FACADE_X=0x74*16, COLLAPSE_SKY_SHIFT=32 };
static bool collapsed,flash,patched;
static void classic_cell(const scene_map_cell_t *cell,const int *offsets,const int *rises,unsigned video_tick,unsigned n);
void scene_render_set_collapse(bool done,bool white) {collapsed=done;flash=white;}
void scene_render_set_patch(bool done) {patched=done;}
static bool patch_cell(const scene_map_cell_t *cell) {
    unsigned n;const scene_map_cell_t *p=scene_patch_cells(&n);
    for (unsigned i=0;i<n;i++) if (p[i].x==cell->x && p[i].y==cell->y) return true;
    return false;
}
static inline bool hidden(const scene_map_cell_t *cell) {
    if (collapsed && cell->layer==3 && cell->x>=COLLAPSE_FACADE_X) return true;
    /* Destroyed shop: column 0x9F-3.. of layer 10D7B6 (0B6882). */
    return patched && cell->layer==2 && cell->x>=(0x9f-3)*16 && cell->x<(0x9f+8)*16 && patch_cell(cell);
}
/* A merged column shares one x: only a column inside a replaced range can
 * mix hidden and kept cells (the others keep their merged single draw). */
static inline bool column_may_hide(const scene_map_cell_t *cell) {
    if (collapsed && cell->layer==3 && cell->x>=COLLAPSE_FACADE_X) return true;
    return patched && cell->layer==2 && cell->x>=(0x9f-3)*16 && cell->x<(0x9f+8)*16;
}
static void draw_cells(const scene_map_cell_t *p,unsigned n,const int *offsets,const int *rises,unsigned video_tick) {
    for (unsigned i=0;i<n;i++) {
        int x=p[i].x-offsets[p[i].layer];
        if (x>-16 && x<320) classic_cell(&p[i],offsets,rises,video_tick,1);
    }
}
static retro_frame_t depth_frame;
static retro_plane_view_t depth_views[5];
static scene_cell_index_t cell_index;
void scene_render_set_depth_mode(bool enabled) {depth_mode=enabled;}
void scene_render_depth_frame(const retro_frame_t *f) {
    depth_frame=*f;
    if (!(MSX_DEPTH_STYLE&DEPTH_LIGHTS)) depth_frame.light_count=0;
    if (!(MSX_DEPTH_STYLE&DEPTH_PERSPECTIVE)) depth_frame.camera.z=0;
}
void scene_render_foreground_fog(void) {if (depth_mode && (MSX_DEPTH_STYLE&DEPTH_FOG)) retro_effects_fog(&depth_frame,2);}
void scene_render_forget_texture(void) {
    last_page=last_slot=last_format=last_width=last_height=-1;
    for (unsigned i=0;i<16;i++) pal_ids[i]=-1;
    mode_tlut=mode_filter=-1; // Callers changed the render mode outside this renderer.
}

// In long ares runs a frame's tail (including its SYNC_FULL completion that
// presents the framebuffer) was occasionally left in the rspq queue with the
// RSP halted, so display_get() timed out with both buffers locked. That is
// the ares JIT missing the two-write wake-up in rspq_flush_internal (libdragon
// cd5b8f567 adds a branch hint for it; applied in tools/libdragon-patches).
// If no buffer frees up for 50 ms, flush the queue again to wake the RSP; give
// up to display_get()'s own diagnostic crash after 20 attempts. rspq_flush only
// sets SIG_MORE and clears HALT/BROKE, which the RSP protocol tolerates at any
// time, so this kick cannot disturb the highpri (mixer) queue: the real RSP
// hang on hardware was the SIG_HIGHPRI_REQUESTED race fixed by patch 0001
// (docs/RENDIMIENTO.md).
surface_t *msx_display_get(void) {
    surface_t *surface;uint32_t start=TICKS_READ();unsigned kicks=0;
    while (!(surface=display_try_get())) {
        // Spare time before the next blank: stream scene chunks (scene_assets_idle).
        scene_assets_idle();
        if (TICKS_SINCE(start)<TICKS_FROM_MS(50)) continue;
        if (++kicks>20) return display_get();
        debugf("MSX rspq kick %u: no framebuffer for 50 ms\n",kicks);
        rspq_flush();start=TICKS_READ();
    }
    return surface;
}
void scene_renderer_init(void) {
    rdpq_text_register_font(1,rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO));
    retro_effects_init();
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    bool indexed=scene_cell_index_build(&cell_index,cells,count);
    assertf(indexed,"Map cell index");(void)indexed;
}
static void finished(void *arg) {
    completion_t *done=arg;
    scene_complete_epoch(done->epoch);
    display_show(done->surface);
    if (done->epoch==1) debugf("MSX first frame ready t=%lu ms\n",(unsigned long)get_ticks_ms());
}
void scene_render_begin(surface_t *surface) {
    current=&completions[++frame%4];*current=(completion_t){surface,frame,TICKS_READ()};
    scene_begin_epoch(frame);scene_service();
    fade_ring=frame%4;fade_used=0;
    for (unsigned i=0;i<16;i++) { pal_ids[i]=-1;pal_lru[i]=0; }
    last_page=last_slot=last_format=last_width=last_height=-1;draws=uploads=palette_uploads=tmem_bytes=0;
    rdpq_attach_clear(surface,NULL);rdpq_set_mode_standard();
    // Every frame starts from a known state, whatever ran before it (the
    // parachute cinematic's blits, the CPU-drawn graphics lab, or font
    // uploads): full scissor, point sampling, no assumed TMEM/TLUT content.
    rdpq_set_scissor(0,0,320,240);
    rdpq_mode_filter(FILTER_POINT);rdpq_mode_alphacompare(1);
    mode_tlut=-1;mode_filter=FILTER_POINT;
}
static unsigned palette_slot(unsigned id) {
    for (unsigned i=0;i<16;i++) if (pal_ids[i]==(int)id) {pal_lru[i]=++clock_pal;return i;}
    unsigned slot=0;
    for (unsigned i=0;i<16;i++) {
        if (pal_ids[i]<0) {slot=i;break;}
        if (pal_lru[i]<pal_lru[slot]) slot=i;
    }
    rdpq_tex_upload_tlut((uint16_t*)scene_render_fade_palette(scene_palette(id)),slot*16,16);
    pal_ids[slot]=(int)id;pal_lru[slot]=++clock_pal;palette_uploads++;
    return slot;
}
static void blit_scaled(const scene_image_t *image,float x,float y,int s,int t,int width,int height,float draw_width,float draw_height,bool mirror,bool flash) {
    if (x>=320 || y>=240 || x+draw_width<=0 || y+draw_height<=0) return;
    /* A shrink below 1/16 (sprite zoom near 0, e.g. a missile leaving its
     * launcher) would overflow the RDP's texture step: less than a pixel
     * per 16 texels, nothing visible to draw. */
    if (draw_width*16.0f<(float)width || draw_height*16.0f<(float)height) return;
    unsigned slot=0;
    if (image->format!=1) {set_tlut(TLUT_RGBA16);slot=palette_slot(flash?0:image->palette);}
    else set_tlut(TLUT_NONE);
    if (last_page!=image->page || last_slot!=(int)slot || last_format!=image->format || last_width!=image->width || last_height!=image->height) {
        const void *data=deferrable?scene_page_try(image):scene_page(image);
        if (!data) return;   // chunk deferred to a later frame (scene_page_try)
        /* Formats: 0 CI4 64x64, 1 RGBA16 32x32, 2 CI4 16x256 (map tiles stacked). */
        surface_t texture=image->format==1?surface_make((void*)data,FMT_RGBA16,32,32,64):
            image->format==2?surface_make((void*)data,FMT_CI4,16,256,8):surface_make((void*)data,FMT_CI4,64,64,32);
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
void scene_render_image_scaled(const scene_image_t *image,float x,float y,float width,float height,bool mirror,bool flash) {
    set_point_filter();
    blit_scaled(image,x,y,0,0,image->width,image->height,width,height,mirror,flash);
}
void scene_render_image(const scene_image_t *image,int x,int y,bool mirror,bool flash) {
    set_point_filter(); // Sprites never inherit the fog sampler.
    blit(image,x,y,0,0,image->width,image->height,mirror,flash);
}
/* Cells following `k` in the window that continue cell k's column: same
 * layer and x, 16 px lower each, and the next texels of the same tall page
 * (format 2), so one rectangle draws them all with the same texels. */
static unsigned column_run(const scene_map_cell_t *cells,const uint16_t *order,unsigned k,unsigned end,unsigned video_tick) {
    const scene_map_cell_t *c=&cells[order[k]];
    const scene_image_t *im=scene_map_image(c->phases[video_tick&7]);
    unsigned n=1;
    if (im->format!=2) return 1;
    while (k+n<end && n<16) {
        const scene_map_cell_t *d=&cells[order[k+n]];
        const scene_image_t *jm=scene_map_image(d->phases[video_tick&7]);
        if (d->layer!=c->layer || d->x!=c->x || d->y!=c->y+16*(int)n || jm->page!=im->page || jm->palette!=im->palette ||
            jm->x!=im->x || jm->y!=im->y+16*(int)n) break;
        n++;
    }
    return n;
}
static void classic_cell(const scene_map_cell_t *cell,const int *offsets,const int *rises,unsigned video_tick,unsigned n) {
    int x=cell->x-offsets[cell->layer],y=8+cell->y+rises[cell->layer];
    if (x+16<=8 || x>=312 || y+16*(int)n<=8 || y>=232) return;
    const scene_image_t *im=scene_map_image(cell->phases[video_tick&7]);
    blit(im,x,y,im->x,im->y,16,16*(int)n,false,false);
}
// Cull window of one layer: the same v=x*scale+offset the full scan computed.
static scene_cell_window_t layer_window(unsigned layer,int offset,float size,float left,float right,bool inclusive) {
    if (depth_mode) return (scene_cell_window_t){depth_views[layer].scale,depth_views[layer].offset_x,size,left,right,inclusive};
    return (scene_cell_window_t){1.0f,(float)-offset,size,left,right,inclusive};
}
static void background(unsigned camera,int height,bool backdrop_after,unsigned video_tick,unsigned begin,unsigned end);
static void depth_cell(const scene_map_cell_t *cell,const retro_plane_view_t *view,const int *offsets,unsigned video_tick,unsigned n) {
    float x=background_x(cell,cell->x-offsets[cell->layer]);
    float y=(8+cell->y)*view->scale+view->offset_y,size=16*view->scale;
    if (x>=320 || x+size<=0) return;
    const scene_image_t *im=scene_map_image(cell->phases[video_tick&7]);
    // The depth camera carries the height (actor.c), so planes keep their own parallax.
    blit_scaled(im,x,y,im->x,im->y,16,16*(int)n,size,size*(float)n,false,false);
}
void scene_render_background(unsigned camera,int height,bool backdrop_after,unsigned video_tick) {background(camera,height,backdrop_after,video_tick,0,5);}
/* Layers 0..2, then (scene_render_front) 3 and 4: sprites whose attribute
 * bit 2 is set (actor +15 bit 2, e.g. the domes 054C2C) go between them. */
void scene_render_back(unsigned camera,int height,bool backdrop_after,unsigned video_tick) {background(camera,height,backdrop_after,video_tick,0,3);}
void scene_render_front(unsigned camera,int height,bool backdrop_after,unsigned video_tick) {background(camera,height,backdrop_after,video_tick,3,5);}
static void draw_layers(unsigned begin,unsigned end,const int *offsets,const int *rises,const bool *visible,unsigned video_tick) {
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    if (!depth_mode) {
        // Pack order is layer order (checked at init), so walking the layers
        // in turn draws the same cells in the same order as one array walk.
        for (unsigned layer=begin;layer<end;layer++) {
          if (!visible[layer]) continue;
          scene_cell_window_t w=layer_window(layer,offsets[layer],16,8,312,false);
          unsigned runs=cell_index.layer_major?cell_index.layer_runs[layer+1]:cell_index.layer_runs[layer];
          for (unsigned r=cell_index.layer_runs[layer];r<runs;r++) {
            unsigned first,end;scene_cell_run_window(&cell_index,r,&w,&first,&end);
            for (unsigned k=first;k<end;) {
                unsigned n=column_run(cells,cell_index.order,k,end,video_tick);
                /* A merged column can mix hidden (replaced) and kept cells:
                 * once a replacement is active, its layer goes cell by cell. */
                if (column_may_hide(&cells[cell_index.order[k]])) {
                    for (unsigned j=0;j<n;j++)
                        if (!hidden(&cells[cell_index.order[k+j]])) classic_cell(&cells[cell_index.order[k+j]],offsets,rises,video_tick,1);
                    k+=n;continue;
                }
                classic_cell(&cells[cell_index.order[k]],offsets,rises,video_tick,n);k+=n;
            }
          }
          if (layer==2 && patched) {unsigned n;const scene_map_cell_t *p=scene_patch_cells(&n);draw_cells(p,n,offsets,rises,video_tick);}
          if (layer==3 && collapsed) {unsigned n;const scene_map_cell_t *p=scene_collapse_cells(&n);draw_cells(p,n,offsets,rises,video_tick);}
        }
        // Any other pack order: the plain array walk.
        if (!cell_index.layer_major) for (unsigned i=0;i<count;i++)
            if (cells[i].layer>=begin && cells[i].layer<end && visible[cells[i].layer] && !hidden(&cells[i])) classic_cell(&cells[i],offsets,rises,video_tick,1);
        return;
    }
    // Same split as the classic path: layers begin..end-1 only (the bit-2
    // sprites are drawn between the two passes); the mesh after layer 4.
    for (unsigned layer=begin;layer<end;layer++) {
      if (!visible[layer]) continue;
      const retro_plane_view_t *view=&depth_views[layer];
      scene_cell_window_t w=layer_window(layer,offsets[layer],16*view->scale,8,312,false);
      for (unsigned r=cell_index.layer_runs[layer];r<cell_index.layer_runs[layer+1];r++) {
        unsigned first,end;scene_cell_run_window(&cell_index,r,&w,&first,&end);
        for (unsigned k=first;k<end;) {
          const scene_map_cell_t *cell=&cells[cell_index.order[k]];
          unsigned n=column_run(cells,cell_index.order,k,end,video_tick);
          /* Same rule as the classic path: once a replacement is active, a
           * merged column may mix hidden and kept cells, so go cell by cell. */
          if (column_may_hide(cell)) {
              for (unsigned j=0;j<n;j++)
                  if (!hidden(&cells[cell_index.order[k+j]])) depth_cell(&cells[cell_index.order[k+j]],view,offsets,video_tick,1);
              k+=n;continue;
          }
          if (hidden(cell)) {k+=n;continue;}
          depth_cell(cell,view,offsets,video_tick,n);
          k+=n;
        }
      }
      // Replacement cells (shop patch, post-collapse rubble) on their layer.
      if (layer==2 && patched) {unsigned m;const scene_map_cell_t *p=scene_patch_cells(&m);for (unsigned i=0;i<m;i++) depth_cell(&p[i],view,offsets,video_tick,1);}
      if (layer==3 && collapsed) {unsigned m;const scene_map_cell_t *p=scene_collapse_cells(&m);for (unsigned i=0;i<m;i++) depth_cell(&p[i],view,offsets,video_tick,1);}
      if ((MSX_DEPTH_STYLE&DEPTH_FOG) && layer<2) {
          retro_effects_fog(&depth_frame,layer);rdpq_set_scissor(8,8,312,232);
      }
    }
}
/* Static backgrounds as recorded rspq blocks. While the layer inputs (scroll
 * offsets, rises, visibility, replacement cells, depth projection) stay the
 * same, each pass (back: layers 0..2, front: 3..4) and animation phase
 * (video_tick & 7) is recorded once and replayed: the same RDP commands
 * without rebuilding them (the boss arena: ~10 ms of CPU per frame). The
 * renderer's TMEM/TLUT bookkeeping is reset before recording and after a
 * replay, so a block uploads everything it draws. Chunks a block reads stay
 * pinned until the RDP has finished the last frame that ran it; then the
 * blocks are freed. Off while a fade or the white flash is on. */
typedef struct {
    bool valid;bool collapsed,patched;int offsets[5],rises[5];bool visible[5];
    retro_plane_view_t views[5];
} bg_key_t;
static bg_key_t bg_key;
static rspq_block_t *bg_blocks[2][8];static unsigned bg_block_draws[2][8];
static unsigned bg_group,bg_last_used;     // pin group of the live blocks; last frame that ran one
/* Retired generations (one per pin group): freed once their last frame is done. */
typedef struct {bool pending;unsigned epoch;rspq_block_t *blocks[16];} bg_retired_t;
static bg_retired_t bg_retired[4];
static unsigned bg_replays,bg_records;
static void reset_texture_state(void) {
    for (unsigned i=0;i<16;i++) { pal_ids[i]=-1;pal_lru[i]=0; }
    last_page=last_slot=last_format=last_width=last_height=-1;mode_tlut=-1;
}
static void bg_collect(void) {
    for (unsigned g=0;g<4;g++) {
        bg_retired_t *r=&bg_retired[g];
        if (!r->pending || scene_completed_epoch()<r->epoch) continue;
        for (unsigned i=0;i<16;i++) if (r->blocks[i]) {rspq_block_free(r->blocks[i]);r->blocks[i]=NULL;}
        scene_unpin_group((int)g);r->pending=false;
    }
}
/* Retire the live blocks; false when no pin group is free for new ones. */
static bool bg_retire(void) {
    bg_retired_t *r=&bg_retired[bg_group];unsigned n=0;
    for (unsigned p=0;p<2;p++) for (unsigned t=0;t<8;t++) if (bg_blocks[p][t]) {r->blocks[n++]=bg_blocks[p][t];bg_blocks[p][t]=NULL;}
    bg_key.valid=false;
    if (!n) {scene_unpin_group((int)bg_group);return true;}
    r->pending=true;r->epoch=bg_last_used;
    for (unsigned g=1;g<=4;g++) if (!bg_retired[(bg_group+g)&3u].pending) {bg_group=(bg_group+g)&3u;return true;}
    return false;
}
static bool bg_static(const int *offsets,const int *rises,const bool *visible) {
    if (!bg_key.valid || bg_key.collapsed!=collapsed || bg_key.patched!=patched) return false;
    for (unsigned i=0;i<5;i++) if (bg_key.offsets[i]!=offsets[i] || bg_key.rises[i]!=rises[i] || bg_key.visible[i]!=visible[i]) return false;
    if (depth_mode && memcmp(bg_key.views,depth_views,sizeof(bg_key.views))) return false;
    for (unsigned p=0;p<2;p++) for (unsigned t=0;t<8;t++) if (!bg_blocks[p][t]) return false;
    return true;
}
static void draw_layers_cached(unsigned begin,unsigned end,const int *offsets,const int *rises,const bool *visible,unsigned video_tick) {
    bg_collect();
    bool cacheable=!fade_on && !flash && !(MSX_DEPTH_STYLE&DEPTH_FOG) && !bg_retired[bg_group].pending;
    bg_key_t key;memset(&key,0,sizeof(key));
    key.valid=true;key.collapsed=collapsed;key.patched=patched;
    for (unsigned i=0;i<5;i++) {key.offsets[i]=offsets[i];key.rises[i]=rises[i];key.visible[i]=visible[i];}
    if (depth_mode) memcpy(key.views,depth_views,sizeof(key.views));
    if (!cacheable || memcmp(&key,&bg_key,sizeof(key))) {
        if (bg_key.valid) bg_retire();
        // Record only once the inputs held for a second frame (scrolling never records).
        static bg_key_t previous;
        bool repeat=cacheable && !bg_retired[bg_group].pending && !memcmp(&key,&previous,sizeof(key));
        previous=key;
        if (!repeat) {draw_layers(begin,end,offsets,rises,visible,video_tick);return;}
        bg_key=key;
    }
    unsigned pass=begin?1u:0u,phase=video_tick&7u;
    if (!bg_blocks[pass][phase]) {
        reset_texture_state();
        unsigned before=draws;
        scene_record_pins((int)bg_group);
        rspq_block_begin();
        draw_layers(begin,end,offsets,rises,visible,video_tick);
        bg_blocks[pass][phase]=rspq_block_end();
        scene_record_pins(-1);bg_records++;
        bg_block_draws[pass][phase]=draws-before;draws=before;
    }
    rspq_block_run(bg_blocks[pass][phase]);bg_replays++;bg_last_used=frame;draws+=bg_block_draws[pass][phase];
    reset_texture_state();rdpq_set_scissor(8,8,312,232);
}
static void background(unsigned camera,int height,bool backdrop_after,unsigned video_tick,unsigned begin,unsigned end) {
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    // Layer 4 holds the market props; it scrolls with the foreground.
    int offsets[5],rises[5];bool visible[5];
    for (unsigned i=0;i<5;i++) {
        unsigned source=i<4?i:3;
        offsets[i]=msx_layer_scroll(source,(int)camera);rises[i]=msx_layer_scroll(source,height);
        visible[i]=msx_layer_visible(source,(int)camera);
    }
    if (collapsed) offsets[0]+=COLLAPSE_SKY_SHIFT;
    // Keep the projection outside assert(): NDEBUG would otherwise drop it.
    if (depth_mode) for (unsigned i=0;i<5;i++) {
        bool ok=(MSX_DEPTH_STYLE&DEPTH_PERSPECTIVE)?
            retro_background_projection(depth_frame.camera,i,retro_effects_layer_depth(i),&depth_views[i]):
            retro_layered_2d_projection(depth_frame.camera,i,&depth_views[i]);
        assertf(ok,"Retro plane projection");(void)ok;
    }
    // Each source cell is CI4 inside a 64x64 / 2048-byte palette atlas.
    // Prefetch a margin on both sides, including the next autoanimation phase.
    // Only the runs' visible ranges are visited (scene_cull.h); requests keep
    // the highest priority per chunk, so their order does not matter, and
    // nothing is serviced inside the loop: repeating the previous chunk's
    // request at the same priority changes nothing and is skipped.
    // With the whole map pinned and resident (Expansion Pak) every request is
    // a no-op: skip the walk.
    // Every phase of both passes recorded for these exact inputs: the chunks
    // they read are pinned (resident), so every request would be a no-op.
    unsigned last_now=~0u,last_next=~0u;bool prefetch=begin==0 && !scene_assets_map_resident() && !bg_static(offsets,rises,visible);
    for (unsigned layer=0;layer<5 && prefetch;layer++) {
        if (!visible[layer]) continue;
        scene_cell_window_t w=layer_window(layer,offsets[layer],0,-48,352,true);
        for (unsigned r=cell_index.layer_runs[layer];r<cell_index.layer_runs[layer+1];r++) {
            unsigned first,end;scene_cell_run_window(&cell_index,r,&w,&first,&end);
            for (unsigned k=first;k<end;k++) {
                const scene_map_cell_t *cell=&cells[cell_index.order[k]];
                const scene_image_t *now=scene_map_image(cell->phases[video_tick&7]);
                const scene_image_t *next=scene_map_image(cell->phases[(video_tick+1)&7]);
                if (now->page/8!=last_now) {scene_request_image(now,2);last_now=now->page/8;}
                if (next->page/8!=last_next) {scene_request_image(next,1);last_next=next->page/8;}
            }
        }
    }
    if (begin==0) {scene_service();msx_perf_mark(MSX_PERF_BG_PREFETCH);}
    rdpq_set_scissor(8,8,312,232);
    if (flash) {
        if (begin) {rdpq_set_scissor(0,0,320,240);return;}
        scene_render_box(8,8,304,224,RGBA32(255,255,255,255));
        rdpq_set_scissor(0,0,320,240);msx_perf_mark(MSX_PERF_BG_DRAW);return;
    }
    if (begin==0) {
        color_t backdrop=color_from_packed32(scene_map_backdrop(backdrop_after));
        if (fade_on) {   /* the backdrop entry 0xFFF fades with the others (MAME runK) */
            backdrop.r=(uint8_t)(neo_fade_channel(backdrop.r>>3,fade_offset[0])<<3);
            backdrop.g=(uint8_t)(neo_fade_channel(backdrop.g>>3,fade_offset[1])<<3);
            backdrop.b=(uint8_t)(neo_fade_channel(backdrop.b>>3,fade_offset[2])<<3);
        }
        scene_render_box(8,8,304,224,backdrop);
    }
    draw_layers_cached(begin,end,offsets,rises,visible,video_tick);
    if (!depth_mode) {rdpq_set_scissor(0,0,320,240);msx_perf_mark(MSX_PERF_BG_DRAW);return;}
    msx_perf_mark(MSX_PERF_BG_DRAW);
    if (end==5) {retro_effects_mesh(&depth_frame);msx_perf_mark(MSX_PERF_MESH);}
    rdpq_set_scissor(0,0,320,240);
}
unsigned scene_render_draw_count(void) {return draws;}
void scene_render_box(int x,int y,int width,int height,color_t color) {
    if (x>=320 || y>=240 || x+width<=0 || y+height<=0) return;
    rdpq_mode_push();rdpq_set_mode_fill(color);rdpq_fill_rectangle(x,y,x+width,y+height);rdpq_mode_pop();
}
void scene_render_text(int x,int y,const char *text) {
    rdpq_text_print(NULL,1,x,y+8,text);
    // Fonts can overwrite TMEM; do not retain stale texture/palette residency.
    last_page=last_slot=last_format=-1;
    for (unsigned i=0;i<16;i++) pal_ids[i]=-1;
    mode_tlut=mode_filter=-1;
}
void scene_render_end(void) {
    if (!(frame%30)) scene_assets_heap_low();
    if (!(frame%300)) {
        scene_cache_stats_t s=scene_cache_stats();(void)s;
        if (depth_mode) debugf("MSX DEPTH: triangles=%u limit=%u fog=%u B lights=%u fog_uploads=0 planes=%u shadows=%u shadow_tris=%u\n",
            retro_effects_triangles(),RETRO_MAX_TRIANGLES,0u,depth_frame.light_count,
            retro_effects_visible_planes(),depth_frame.shadow_count,retro_effects_shadow_triangles());
        debugf("MSX RDP: draws=%u tex=%u TLUT=%u TMEM=%u B CPU=%lu us PI=%u loads/%u bytes misses=%u evictions=%u deferred=%u held=%u sync=%u idle=%u decode_max=%lu us heap_low=%d KB bgblocks=%u/%u\n",
            draws,uploads,palette_uploads,tmem_bytes,(unsigned long)TICKS_TO_US(TICKS_SINCE(current->start_ticks)),
            s.loads,s.dma_bytes,s.misses,s.evictions,s.deferred,s.held,s.sync_misses,s.idle_decodes,(unsigned long)TICKS_TO_US(s.decode_max_ticks),scene_assets_heap_low()/1024,bg_replays,bg_records);
    }
    // One completion fence per frame: RDRAM pages stay pinned until RDP is done.
    rdpq_detach_cb(finished,current);
}
