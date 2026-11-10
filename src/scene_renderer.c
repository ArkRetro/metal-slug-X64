#include "scene_renderer.h"
#include "neo_fade.h"
#include "retro_effects.h"
#include "mission_camera.h"
#include "msx_perf.h"
#include "scene_cull.h"
#include "scene_strips.h"
#include "scene_prerender.h"
#include "visual_config.h"
#include <stdlib.h>
#include <math.h>

// Optional depth25d extras (MSX_DEPTH_STYLE bits); the release presentation
// is layered 2D scenery with contact shadows only (style 0).
enum { DEPTH_PERSPECTIVE=1, DEPTH_LIGHTS=2, DEPTH_FOG=4 };

typedef struct { surface_t *surface; unsigned epoch,start_ticks; } completion_t;
static completion_t completions[4],*current;
static unsigned frame,clock_pal,draws,uploads,palette_uploads,tmem_bytes;
static uint32_t upload_ticks,rect_ticks;   /* B47: time inside rdpq_tex_upload_sub / rdpq_texture_rectangle (MSX BG line) */
static int pal_ids[16],last_page,last_slot,last_format,last_width,last_height;
static bool deferrable;
#ifndef MSX_BG_DIRECT
#define MSX_BG_DIRECT 0   /* 1: the direct walk and rdpq_tex_upload_sub by default (local baseline builds) */
#endif
/* B47: the strips (scene_strips.h) and the raw page uploads; false: the
 * direct index walk and rdpq_tex_upload_sub (the MSX_BG_CRC comparison). */
static bool strips_on=!MSX_BG_DIRECT,raw_upload=!MSX_BG_DIRECT;
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
/* Map patches 0C70FC of a mission stage (B37 = B15): the active patches'
 * cells (scene_stage_t patch_cells) are drawn instead of the stage's cells
 * of their layer inside each patch's rectangle (the original's SCB1 words
 * of those blocks are overwritten). */
typedef struct { uint8_t layer;int16_t x0,y0,x1,y1;unsigned first,count; } map_patch_t;
static map_patch_t map_patches[16];static unsigned map_patch_total;static uint16_t map_patch_mask;
/* The pack's layer word against the game's (B63: target column D4 in bits
 * 21..31; a pack entry without it, Mission 2 before B63, takes any). */
static bool patch_object_match(uint32_t pack,uint32_t game) {
    return !((pack^game)&0x1fffffu) && (!(pack>>21) || pack>>21==game>>21);
}
void scene_render_set_map_patches(const uint32_t (*list)[2],unsigned n) {
    const scene_stage_t *st=scene_stage();uint16_t mask=0;
    if (st) for (unsigned k=0;k<n;k++) for (unsigned i=0;i<st->patch_count && i<map_patch_total;i++)
        if (st->patch_submap[i]==list[k][0] && patch_object_match(st->patch_layer_object[i],list[k][1])) mask|=(uint16_t)(1u<<i);
    static uint32_t seen_key;uint32_t key=n?list[n-1][0]^list[n-1][1]:0;
    if (mask!=map_patch_mask || key!=seen_key) debugf("MSX map patches drawn mask=%04x (%u recorded, last %08lx %08lx)\n",mask,n,n?(unsigned long)list[n-1][0]:0ul,n?(unsigned long)list[n-1][1]:0ul);
    seen_key=key;
    map_patch_mask=mask;
}
static void map_patch_index(const scene_stage_t *st) {
    map_patch_total=0;map_patch_mask=0;
    if (!st) return;
    for (unsigned i=0;i<st->patch_count && i<16;i++) {
        map_patch_t *m=&map_patches[i];*m=(map_patch_t){0,32767,32767,-32768,-32768,0,0};
        for (unsigned c=0;c<st->patch_cell_count;c++) {
            const scene_map_cell_t *cell=&st->patch_cells[c];
            if (((cell->layer>>3)&15u)!=i) continue;
            if (!m->count) m->first=c;
            m->count=c+1-m->first;m->layer=cell->layer&7u;
            if (cell->x<m->x0) m->x0=cell->x;
            if (cell->y<m->y0) m->y0=cell->y;
            if (cell->x+16>m->x1) m->x1=(int16_t)(cell->x+16);
            if (cell->y+16>m->y1) m->y1=(int16_t)(cell->y+16);
        }
        map_patch_total=i+1;
    }
}
static inline bool map_patch_column(const scene_map_cell_t *cell) {
    for (unsigned i=0;i<map_patch_total;i++)
        if ((map_patch_mask>>i)&1u && map_patches[i].layer==cell->layer && cell->x>=map_patches[i].x0 && cell->x<map_patches[i].x1) return true;
    return false;
}
static inline bool map_patch_hides(const scene_map_cell_t *cell) {
    for (unsigned i=0;i<map_patch_total;i++) {
        const map_patch_t *m=&map_patches[i];
        if ((map_patch_mask>>i)&1u && m->layer==cell->layer && cell->x>=m->x0 && cell->x<m->x1 && cell->y>=m->y0 && cell->y<m->y1) return true;
    }
    return false;
}
static void map_patch_draw(unsigned layer,const retro_plane_view_t *view,const int *offsets,const int *rises,unsigned video_tick);
/* Tower templates of the loaded stage (cell index per type, row, column;
 * 0xFFFF: none) and the pieces to draw (scene_render_set_tower). */
static uint16_t tower_cell[SCENE_TOWER_TYPES][3][24];static bool tower_stage;
static const uint8_t *tower_types;static unsigned tower_count;
static bool layer_own[4];static int layer_own_offset[4];
void scene_render_set_layer_offset(unsigned layer,bool on,int offset) {if (layer<4) {layer_own[layer]=on;layer_own_offset[layer]=offset;}}
/* Layers drawn by their hardware column ring (scene_render_set_layer_ring). */
typedef struct { bool on;uint8_t first;int16_t x;int16_t map_x[32]; } layer_ring_t;
static layer_ring_t layer_ring[4];
void scene_render_set_layer_ring(unsigned layer,bool on,int x,unsigned first_slot,const int16_t map_x[32]) {
    if (layer>=4) return;
    layer_ring_t *r=&layer_ring[layer];
    if (!on) {memset(r,0,sizeof(*r));return;}
    r->on=true;r->first=(uint8_t)(first_slot&31u);r->x=(int16_t)x;memcpy(r->map_x,map_x,sizeof(r->map_x));
}
/* Runs of ring columns with one offset (map x - screen x): [x0, x1) on screen. */
typedef struct { int x0,x1,offset; } ring_segment_t;
static unsigned ring_segments(const layer_ring_t *r,ring_segment_t *seg) {
    unsigned n=0;
    for (unsigned k=0;k<32;k++) {
        int sx=r->x+16*(int)k;
        if (r->map_x[k]==INT16_MIN || sx+16<=8 || sx>=312) continue;
        int off=r->map_x[k]-sx;
        if (n && seg[n-1].offset==off && seg[n-1].x1==sx) seg[n-1].x1=sx+16;
        else seg[n++]=(ring_segment_t){sx,sx+16,off};
    }
    return n;
}
void scene_render_set_tower(const uint8_t *types,unsigned count) {tower_types=types;tower_count=tower_stage?count:0u;}
static void tower_index(const scene_stage_t *st) {
    memset(tower_cell,0xff,sizeof(tower_cell));tower_stage=false;tower_count=0;
    if (!st) return;
    for (unsigned i=0;i<st->cell_count;i++) {
        const scene_map_cell_t *c=&st->cells[i];
        if (c->layer!=3 || c->x<SCENE_TOWER_TEMPLATE_X) continue;
        unsigned col=(unsigned)(c->x-SCENE_TOWER_TEMPLATE_X)/16u,row=(unsigned)c->y/16u;
        if (col<24 && row<3u*SCENE_TOWER_TYPES) {tower_cell[row/3][row%3][col]=(uint16_t)i;tower_stage=true;}
    }
}
/* The visible pieces, cell by cell (classic: depth_view NULL). */
static void depth_cell(const scene_map_cell_t *cell,const retro_plane_view_t *view,const int *offsets,unsigned video_tick,unsigned n);
/* Classic path: the pieces' cells never overlap (a 16-pixel grid, pieces
 * 48 pixels apart), so their order inside the tower is free. Drawn grouped
 * by texture page and palette, with the column strips of one page merged:
 * cell by cell in piece order each draw reloaded its page (Mission 2 stage
 * 3 at the top: ~210 texture loads and ~35 ms of CPU per frame). */
static void blit_scaled(const scene_image_t *image,float x,float y,int s,int t,int width,int height,float draw_width,float draw_height,bool mirror,bool flip_y,bool flash);
static void blit(const scene_image_t *image,int x,int y,int s,int t,int width,int height,bool mirror,bool flash);
/* The pieces' cells never overlap (a 16-pixel grid, pieces 48 pixels
 * apart), so their order inside the tower is free: drawn grouped by texture
 * page and palette, with the column strips of one page merged. Cell by cell
 * in piece order each draw reloaded its page (Mission 2 stage 3 at the top:
 * ~210 texture loads per frame). Pieces wholly off screen are skipped. */
typedef struct { const scene_image_t *im;int16_t x,y; } tower_draw_t;
static int tower_draw_cmp(const void *a,const void *b) {
    const tower_draw_t *p=a,*q=b;
    if (p->im->page!=q->im->page) return p->im->page<q->im->page?-1:1;
    if (p->im->palette!=q->im->palette) return p->im->palette<q->im->palette?-1:1;
    if (p->x!=q->x) return p->x<q->x?-1:1;
    return p->y<q->y?-1:p->y>q->y;
}
static void draw_tower(const int *offsets,const int *rises,const retro_plane_view_t *view,unsigned video_tick) {
    if (!tower_count || !tower_types) return;
    enum { TOWER_DRAWS=8*72 };
    static tower_draw_t list[TOWER_DRAWS];static unsigned n;
    const scene_stage_t *st=scene_stage();
    /* The sorted list depends on the pieces whose rows reach the screen and
     * on the video phase only (B47): kept while they repeat. The pieces are
     * taken whole, so a piece partly on screen lists its off-screen cells too
     * (blit_scaled drops them, as the per-cell test did). */
    unsigned k0=tower_count,k1=0;
    for (unsigned k=0;k<tower_count;k++) {
        int top=SCENE_TOWER_BASE_Y-48*(int)k,y=8+top+rises[3];
        if (!view && (y+48<=8 || y>=232)) continue;
        if (view) {float py=(8+top)*view->scale+view->offset_y;if (py+48*view->scale<=0 || py>=240) continue;}
        if (k<k0) k0=k;
        k1=k+1;
    }
    static unsigned key_k0=~0u,key_k1,key_phase,key_count;static const uint8_t *key_types;static float key_x0,key_x1;
    float x0=view?view->offset_x:(float)-offsets[3],x1=view?view->scale:1.0f;   /* the horizontal test's inputs */
    bool same=strips_on && key_k0==k0 && key_k1==k1 && key_phase==(video_tick&7u) && key_count==tower_count && key_types==tower_types && key_x0==x0 && key_x1==x1;
    if (!same) {
    key_k0=k0;key_k1=k1;key_phase=video_tick&7u;key_count=tower_count;key_types=tower_types;key_x0=x0;key_x1=x1;n=0;
    for (unsigned k=k0;k<k1;k++) {
        int top=SCENE_TOWER_BASE_Y-48*(int)k,y=8+top+rises[3];
        if (!view && (y+48<=8 || y>=232)) continue;
        if (view) {float py=(8+top)*view->scale+view->offset_y;if (py+48*view->scale<=0 || py>=240) continue;}
        unsigned t=tower_types[k];if (t>=SCENE_TOWER_TYPES) continue;
        for (unsigned j=0;j<3;j++) for (unsigned i=0;i<24;i++) {
            uint16_t id=tower_cell[t][j][i];if (id==0xffff || n==TOWER_DRAWS) continue;
            /* Map coordinates; the screen tests are classic_cell's/depth_cell's. */
            /* Map coordinates; the horizontal screen test is classic_cell's/depth_cell's. */
            int cx=256+16*(int)i,cy=top+16*(int)j;
            if (!view) {int sx=cx-offsets[3];if (sx+16<=8 || sx>=312) continue;}
            else {float sx=cx*view->scale+view->offset_x;if (sx>=320 || sx+16*view->scale<=0) continue;}
            list[n++]=(tower_draw_t){scene_map_image(scene_cell_phase(&st->cells[id],video_tick)),(int16_t)cx,(int16_t)cy};
        }
    }
    qsort(list,n,sizeof(*list),tower_draw_cmp);
    }
    for (unsigned k=0;k<n;) {
        const scene_image_t *im=list[k].im;unsigned m=1;
        while (im->format==2 && k+m<n && m<16 && list[k+m].im->page==im->page && list[k+m].im->palette==im->palette &&
               list[k+m].x==list[k].x && list[k+m].y==list[k].y+16*(int)m && list[k+m].im->x==im->x && list[k+m].im->y==im->y+16*(int)m) m++;
        if (!view) blit(im,list[k].x-offsets[3],8+list[k].y+rises[3],im->x,im->y,16,16*(int)m,false,false);
        else {float size=16*view->scale;blit_scaled(im,list[k].x*view->scale+view->offset_x,(8+list[k].y)*view->scale+view->offset_y,im->x,im->y,16,16*(int)m,size,size*(float)m,false,false,false);}
        k+=m;
    }
}
static bool patch_cell(const scene_map_cell_t *cell) {
    unsigned n;const scene_map_cell_t *p=scene_patch_cells(&n);
    for (unsigned i=0;i<n;i++) if (p[i].x==cell->x && p[i].y==cell->y) return true;
    return false;
}
static inline bool hidden(const scene_map_cell_t *cell) {
    if (collapsed && cell->layer==3 && cell->x>=COLLAPSE_FACADE_X) return true;
    if (map_patch_mask && map_patch_hides(cell)) return true;
    /* Destroyed shop: column 0x9F-3.. of layer 10D7B6 (0B6882). */
    return patched && cell->layer==2 && cell->x>=(0x9f-3)*16 && cell->x<(0x9f+8)*16 && patch_cell(cell);
}
/* A merged column shares one x: only a column inside a replaced range can
 * mix hidden and kept cells (the others keep their merged single draw). */
static inline bool column_may_hide(const scene_map_cell_t *cell) {
    if (collapsed && cell->layer==3 && cell->x>=COLLAPSE_FACADE_X) return true;
    if (map_patch_mask && map_patch_column(cell)) return true;
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
static scene_cell_index_t mission1_index;
void scene_renderer_init(void) {
    rdpq_text_register_font(1,rdpq_font_load_builtin(FONT_BUILTIN_DEBUG_MONO));
    retro_effects_init();
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    bool indexed=scene_cell_index_build(&cell_index,cells,count);
    assertf(indexed,"Map cell index");(void)indexed;
    mission1_index=cell_index;
}
static void finished(void *arg) {
    completion_t *done=arg;
    scene_complete_epoch(done->epoch);
    display_show(done->surface);msx_frames_present();
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
/* A whole 2048-byte page into TMEM (B47): the commands rdpq_tex_upload_sub
 * emits for these three fixed surfaces (texload_block / texload_block_4bpp,
 * libdragon rdpq_tex.c) without its per-call setup, and without repeating
 * the tile descriptors while the page format and palette slot stay those of
 * the previous upload: SET_TEXTURE_IMAGE and LOAD_BLOCK only. The tiles are
 * re-sent after scene_render_forget_texture (last_format=-1) and at every
 * frame start. The autosync of the public inline commands still applies. */
static int page_upload(const surface_t *texture,int format,unsigned slot) {
    /* Formats: 0 CI4 64x64 (pitch 32), 1 RGBA16 32x32 (pitch 64), 2 CI4 16x256 (pitch 8); 1024 texels per LOAD_BLOCK. */
    static const uint16_t pitch[3]={32,64,8},image_width[3]={16,32,4},image_height[3]={64,32,256},tile_s1[3]={64*4,32*4,16*4},tile_t1[3]={64*4,32*4,256*4};
    tex_format_t fmt=format==1?FMT_RGBA16:FMT_CI4;
    rdpq_set_texture_image_raw(surface_get_placeholder_index(texture),PhysicalAddr(texture->buffer),FMT_RGBA16,image_width[format],image_height[format]);
    if (last_format!=format || last_slot!=(int)slot) {
        rdpq_set_tile(TILE1,FMT_RGBA16,0,0,NULL);
        rdpq_set_tile(TILE0,fmt,0,pitch[format],&(rdpq_tileparms_t){.palette=(uint8_t)slot,.s.clamp=true,.t.clamp=true});
    }
    rdpq_load_block(TILE1,0,0,1024,pitch[format]);
    if (last_format!=format) rdpq_set_tile_size_fx(TILE0,0,0,tile_s1[format],tile_t1[format]);
    return 2048;
}
static void blit_scaled(const scene_image_t *image,float x,float y,int s,int t,int width,int height,float draw_width,float draw_height,bool mirror,bool flip_y,bool flash) {
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
        uint32_t u0=TICKS_READ();
        int bytes=raw_upload?page_upload(&texture,image->format,slot):rdpq_tex_upload_sub(TILE0,&texture,&(rdpq_texparms_t){.palette=(int)slot},0,0,image->width,image->height);
        upload_ticks+=TICKS_READ()-u0;
        assertf(bytes<=2048,"Scene TMEM page overflow");
        last_page=image->page;last_slot=(int)slot;last_format=image->format;
        last_width=image->width;last_height=image->height;uploads++;tmem_bytes+=(unsigned)bytes;
    }
    uint32_t r0=TICKS_READ();
    rdpq_texture_rectangle_scaled(TILE0,x,y,x+draw_width,y+draw_height,
        mirror?s+width-1:s,flip_y?t+height-1:t,mirror?s-1:s+width,flip_y?t-1:t+height);
    rect_ticks+=TICKS_READ()-r0;
    draws++;
}
static float background_x(const scene_map_cell_t *cell,int fallback) {
    if (!depth_mode) return fallback;
    const retro_plane_view_t *view=&depth_views[cell->layer];
    return cell->x*view->scale+view->offset_x;
}
static void blit(const scene_image_t *image,int x,int y,int s,int t,int width,int height,bool mirror,bool flash) {
    blit_scaled(image,x,y,s,t,width,height,width,height,mirror,false,flash);
}
void scene_render_image_scaled(const scene_image_t *image,float x,float y,float width,float height,bool mirror,bool flash) {
    scene_render_sprite_scaled(image,x,y,width,height,mirror,false,flash);
}
void scene_render_sprite_scaled(const scene_image_t *image,float x,float y,float width,float height,bool mirror,bool flip_y,bool flash) {
    set_point_filter();
    blit_scaled(image,x,y,0,0,image->width,image->height,width,height,mirror,flip_y,flash);
}
void scene_render_sprite(const scene_image_t *image,int x,int y,bool mirror,bool flip_y,bool flash) {
    set_point_filter();
    blit_scaled(image,x,y,0,0,image->width,image->height,image->width,image->height,mirror,flip_y,flash);
}
void scene_render_image(const scene_image_t *image,int x,int y,bool mirror,bool flash) {
    set_point_filter(); // Sprites never inherit the fog sampler.
    blit(image,x,y,0,0,image->width,image->height,mirror,flash);
}
/* Cells following `k` in the window that continue cell k's column: same
 * layer and x, 16 px lower each, and the next texels of the same tall page
 * (format 2), so one rectangle draws them all with the same texels. */
static unsigned run_touch;   /* B47 diagnostics: cells column_run examined */
static unsigned column_run(const scene_map_cell_t *cells,const uint16_t *order,unsigned k,unsigned end,unsigned video_tick) {
    const scene_map_cell_t *c=&cells[order[k]];run_touch++;
    const scene_image_t *im=scene_map_image(scene_cell_phase(c,video_tick));
    unsigned n=1;
    if (im->format!=2) return 1;
    while (k+n<end && n<16) {
        const scene_map_cell_t *d=&cells[order[k+n]];
        const scene_image_t *jm=scene_map_image(scene_cell_phase(d,video_tick));run_touch++;
        if (d->layer!=c->layer || d->x!=c->x || d->y!=c->y+16*(int)n || jm->page!=im->page || jm->palette!=im->palette ||
            jm->x!=im->x || jm->y!=im->y+16*(int)n) break;
        n++;
    }
    return n;
}
static void classic_cell(const scene_map_cell_t *cell,const int *offsets,const int *rises,unsigned video_tick,unsigned n) {
    int x=cell->x-offsets[cell->layer],y=8+cell->y+rises[cell->layer];
    if (x+16<=8 || x>=312 || y+16*(int)n<=8 || y>=232) return;
    const scene_image_t *im=scene_map_image(scene_cell_phase(cell,video_tick));
    blit(im,x,y,im->x,im->y,16,16*(int)n,false,false);
}
// Cull window of one layer: the same v=x*scale+offset the full scan computed.
static scene_cell_window_t layer_window(unsigned layer,int offset,float size,float left,float right,bool inclusive) {
    if (depth_mode) return (scene_cell_window_t){depth_views[layer].scale,depth_views[layer].offset_x,size,left,right,inclusive};
    return (scene_cell_window_t){1.0f,(float)-offset,size,left,right,inclusive};
}
static void background(unsigned camera,int height,bool backdrop_after,unsigned video_tick,unsigned mask,bool first);
static void depth_cell(const scene_map_cell_t *cell,const retro_plane_view_t *view,const int *offsets,unsigned video_tick,unsigned n) {
    float x=background_x(cell,cell->x-offsets[cell->layer]);
    float y=(8+cell->y)*view->scale+view->offset_y,size=16*view->scale;
    if (x>=320 || x+size<=0) return;
    const scene_image_t *im=scene_map_image(scene_cell_phase(cell,video_tick));
    // The depth camera carries the height (actor.c), so planes keep their own parallax.
    blit_scaled(im,x,y,im->x,im->y,16,16*(int)n,size,size*(float)n,false,false,false);
}
/* The active patches' cells of `layer` (cell layer byte 0x80 | i << 3 | L:
 * drawn as a cell of layer L). */
static void map_patch_draw(unsigned layer,const retro_plane_view_t *view,const int *offsets,const int *rises,unsigned video_tick) {
    const scene_stage_t *st=scene_stage();
    if (!st) return;
    for (unsigned i=0;i<map_patch_total;i++) {
        const map_patch_t *m=&map_patches[i];
        if (!((map_patch_mask>>i)&1u) || m->layer!=layer) continue;
        for (unsigned c=0;c<m->count;c++) {
            scene_map_cell_t cell=st->patch_cells[m->first+c];cell.layer=(uint8_t)layer;
            if (view) depth_cell(&cell,view,offsets,video_tick,1);
            else if (cell.x-offsets[layer]>-16 && cell.x-offsets[layer]<320) classic_cell(&cell,offsets,rises,video_tick,1);
        }
    }
}
void scene_render_background(unsigned camera,int height,bool backdrop_after,unsigned video_tick) {background(camera,height,backdrop_after,video_tick,0x1fu,true);}
/* Layers 0..2, then (scene_render_front) 3 and 4: sprites whose attribute
 * bit 2 is set (actor +15 bit 2, e.g. the domes 054C2C) go between them. */
void scene_render_back(unsigned camera,int height,bool backdrop_after,unsigned video_tick) {background(camera,height,backdrop_after,video_tick,0x07u,true);}
void scene_render_front(unsigned camera,int height,bool backdrop_after,unsigned video_tick) {background(camera,height,backdrop_after,video_tick,0x18u,false);}
/* The layers of `mask` (bit n = layer n, drawn in index order, which is
 * their SCB slot order within one pass); `first`: the frame's first pass
 * (backdrop, prefetch, the white flash). */
void scene_render_layers(unsigned camera,int height,bool backdrop_after,unsigned video_tick,unsigned mask,bool first) {background(camera,height,backdrop_after,video_tick,mask,first);}
/* Per-layer cost of the sampled frame (index 5: the tower), "MSX BG" line. */
static uint32_t layer_ticks[6],layer_blit[6];static unsigned layer_draws[6],layer_uploads[6],layer_visits[6],layer_touch[6];
/* Background draw lists (scene_strips.h): one strip per layer over a window
 * STRIP_MARGIN wider than the screen on every side, rebuilt when the camera
 * leaves it (every ~96 px of that layer's scroll), when a replacement state
 * changes (strip_state) or when the stage changes. The pool is a bump
 * allocator: when a rebuild does not fit, every strip is dropped and rebuilt
 * on demand (the cost of one old-style walk per layer). A layer whose window
 * does not fit even alone draws through the direct walk (draw_layer_direct). */
enum { STRIP_MARGIN=96 };
/* The pool lives with the loaded stage's tables (scene_stage_t, cache
 * slots): the program image keeps the RAM floor (208 slots). Without a
 * stage (Mission 1) the layers draw through the direct walk. */
_Static_assert(sizeof(scene_strip_item_t)==SCENE_STRIP_ENTRY_BYTES && sizeof(scene_strip_group_t)==SCENE_STRIP_ENTRY_BYTES,"strip entry size");
static scene_strip_item_t *strip_items;static scene_strip_group_t *strip_groups;
static scene_strip_pool_t strip_pool;
static scene_strip_t strips[5];static uint32_t strip_state=~0u;static unsigned strip_builds,strip_resets,strip_direct;static uint32_t strip_build_ticks;
static const scene_strip_env_t strip_env={scene_map_image,column_may_hide};
static void strips_drop(void) {
    for (unsigned i=0;i<5;i++) strips[i].valid=false;
    strip_pool.items_used=strip_pool.groups_used=0;
}
/* The pool of the loaded stage (scene_renderer_stage_changed), or none. */
static void strips_attach(const scene_stage_t *st) {
    strips_drop();
    strip_items=st?st->strip_items:NULL;strip_groups=st?st->strip_groups:NULL;
    strip_pool=(scene_strip_pool_t){strip_items,strip_groups,st?SCENE_STRIP_ITEMS:0u,st?SCENE_STRIP_GROUPS:0u,0,0};
}
/* The live strips moved to the front of the pool (their regions are
 * disjoint and in allocation order), so a rebuild gets the space of the
 * strips already dropped without dropping the others. */
static void strips_compact(void) {
    /* B50: each strip moves once (a strip with no items, e.g. an empty
     * layer window at the top of Mission 2 stage 3, has first_item ==
     * items and was picked again for ever: the game hung there). */
    unsigned items=0,groups=0;bool moved[5]={false,false,false,false,false};
    for (;;) {
        scene_strip_t *s=NULL;unsigned pick=0;
        for (unsigned i=0;i<5;i++) if (strips[i].valid && !moved[i] && (!s || strips[i].first_item<s->first_item)) {s=&strips[i];pick=i;}
        if (!s) break;
        moved[pick]=true;
        if (s->first_item!=items) {
            memmove(&strip_items[items],&strip_items[s->first_item],s->items*sizeof(*strip_items));
            memmove(&strip_groups[groups],&strip_groups[s->first_group],s->groups*sizeof(*strip_groups));
            unsigned delta=s->first_item-items;
            for (unsigned g=0;g<s->groups;g++) {strip_groups[groups+g].first-=(uint16_t)delta;strip_groups[groups+g].end-=(uint16_t)delta;}
            s->first_item=items;s->first_group=groups;
        }
        items+=s->items;groups+=s->groups;
    }
    strip_pool.items_used=items;strip_pool.groups_used=groups;
}
static scene_strip_t *strip_rebuild(unsigned layer,int wx0,int wx1,int wy0,int wy1) {
    scene_strip_t *s=&strips[layer];uint32_t t0=TICKS_READ();
    s->valid=false;strips_compact();   /* the space of this and any other dropped strip first (a failed build walks the whole window) */
    for (int margin=STRIP_MARGIN;;margin/=2) {
        for (unsigned tries=0;tries<2;tries++) {
            if (scene_strip_build(s,&strip_pool,&cell_index,layer,wx0-margin,wx1+margin,wy0-margin,wy1+margin,&strip_env)) {strip_builds++;strip_build_ticks+=TICKS_READ()-t0;return s;}
            if (!tries) {strips_drop();strip_resets++;}   /* full even compacted: start over */
        }
        if (!margin) {strip_direct++;return NULL;}
    }
}
static const scene_strip_t *layer_strip(unsigned layer,int wx0,int wx1,int wy0,int wy1) {
    uint32_t state=(collapsed?1u:0u)|(patched?2u:0u)|((uint32_t)map_patch_mask<<2);
    if (state!=strip_state) {strips_drop();strip_state=state;}
    scene_strip_t *s=&strips[layer];
    if (!scene_strip_covers(s,wx0,wx1,wy0,wy1)) return strip_rebuild(layer,wx0,wx1,wy0,wy1);
    /* Ahead of need, one layer per frame: a strip that still covers the
     * window but with less than STRIP_AHEAD of its margin left is rebuilt
     * now, so layers that scroll together (2 and 3 at the camera's speed)
     * do not all rebuild on the same frame. */
    enum { STRIP_AHEAD=16 };
    static unsigned refresh_frame;
    if (refresh_frame!=frame && (s->x0>wx0-STRIP_AHEAD || s->x1<wx1+STRIP_AHEAD || s->y0>wy0-STRIP_AHEAD || s->y1<wy1+STRIP_AHEAD)) {
        refresh_frame=frame;return strip_rebuild(layer,wx0,wx1,wy0,wy1);
    }
    return s;
}
/* The image of an item in a video phase (scene_cell_phase for a merged column). */
static inline const scene_image_t *strip_image(const scene_strip_item_t *it,unsigned video_tick) {
    return scene_map_image(it->anim==SCENE_CELL_STATIC?it->image:(uint16_t)(it->image+scene_cell_patterns[it->anim][video_tick&7u]));
}
/* The window of `layer` in cell coordinates (x+16>wx0, x<wx1, y+16>wy0,
 * y<wy1 is what the renderer's own tests pass; see scene_strips.h). */
static void layer_bounds(unsigned layer,const int *offsets,const int *rises,int *wx0,int *wx1,int *wy0,int *wy1) {
    if (!depth_mode) {*wx0=offsets[layer]+8;*wx1=offsets[layer]+312;*wy0=-rises[layer];*wy1=224-rises[layer];return;}
    const retro_plane_view_t *view=&depth_views[layer];
    *wx0=(int)floorf((8.0f-view->offset_x)/view->scale);*wx1=(int)ceilf((312.0f-view->offset_x)/view->scale);
    int ytop=(int)floorf(-view->offset_y/view->scale)-24-32,ybot=(int)ceilf((240.0f-view->offset_y)/view->scale)-8+32;
    *wy0=ytop-16;*wy1=ybot;
}
static void draw_strip_item(const scene_strip_item_t *it,unsigned layer,const int *offsets,const int *rises,unsigned video_tick) {
    const scene_map_cell_t *cells=cell_index.cells;
    if (it->anim==SCENE_STRIP_CELL) {
        const scene_map_cell_t *c=&cells[it->image];
        if (hidden(c)) return;
        if (depth_mode) depth_cell(c,&depth_views[layer],offsets,video_tick,1); else classic_cell(c,offsets,rises,video_tick,1);
        return;
    }
    const scene_image_t *im=strip_image(it,video_tick);
    if (!depth_mode) {
        int x=it->x-offsets[layer],y=8+it->y+rises[layer];
        if (x+16<=8 || x>=312 || y+16*(int)it->n<=8 || y>=232) return;
        blit(im,x,y,im->x,im->y,16,16*(int)it->n,false,false);return;
    }
    const retro_plane_view_t *view=&depth_views[layer];
    float x=it->x*view->scale+view->offset_x,y=(8+it->y)*view->scale+view->offset_y,size=16*view->scale;
    if (x>=320 || x+size<=0) return;
    blit_scaled(im,x,y,im->x,im->y,16,16*(int)it->n,size,size*(float)it->n,false,false,false);
}
/* The visible items of a strip: the group's x range first, then one binary
 * search per group that reaches the screen, then the vertical test. */
static void draw_layer_strip(unsigned layer,const scene_strip_t *s,const int *offsets,const int *rises,unsigned video_tick) {
    scene_cell_window_t w=layer_window(layer,offsets[layer],depth_mode?16*depth_views[layer].scale:16,8,312,false);
    int ytop=0,ybot=0;
    if (depth_mode) {const retro_plane_view_t *view=&depth_views[layer];ytop=(int)floorf(-view->offset_y/view->scale)-24-32;ybot=(int)ceilf((240.0f-view->offset_y)/view->scale)-8+32;}
    for (unsigned g=s->first_group;g<s->first_group+s->groups;g++) {
        unsigned first,end;
        if (!scene_strip_group_window(&strip_pool,&strip_groups[g],&w,&first,&end)) continue;
        layer_visits[layer]+=end-first;
        for (unsigned k=first;k<end;k++) {
            const scene_strip_item_t *it=&strip_items[k];
            if (depth_mode && (it->y>=ybot || it->y+16*(int)it->n<=ytop)) continue;
            draw_strip_item(it,layer,offsets,rises,video_tick);
        }
    }
}
/* The direct walk of the cell index (the renderer before the strips):
 * the fallback when a strip does not fit, and the reference of MSX_BG_CRC. */
static void draw_layer_direct(unsigned layer,const int *offsets,const int *rises,unsigned video_tick) {
    const scene_map_cell_t *cells=cell_index.cells;
    if (!depth_mode) {
        scene_cell_window_t w=layer_window(layer,offsets[layer],16,8,312,false);
        unsigned runs=cell_index.layer_major?cell_index.layer_runs[layer+1]:cell_index.layer_runs[layer];
        for (unsigned r=cell_index.layer_runs[layer];r<runs;r++) {
            unsigned first,end;scene_cell_run_window(&cell_index,r,&w,&first,&end);
            layer_visits[layer]+=end-first;
            for (unsigned k=first;k<end;) {
                /* Vertical cull without visiting the column: cells above the
                 * screen are skipped to the first one that reaches it, cells
                 * below it to the next column (the tall map of Mission 2
                 * stage 3 walked ~8,000 cells per pass). A merged strip that
                 * started above now starts at its first visible cell: the
                 * same pixels. */
                int sy=8+cells[cell_index.order[k]].y+rises[layer];
                if (sy+16<=8) {k=scene_cell_column_from(&cell_index,k,scene_cell_column_end(&cell_index,k,end),-rises[layer]-15);continue;}
                if (sy>=232) {k=scene_cell_column_end(&cell_index,k,end);continue;}
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
        return;
    }
    const retro_plane_view_t *view=&depth_views[layer];
    scene_cell_window_t w=layer_window(layer,offsets[layer],16*view->scale,8,312,false);
    /* Vertical cull (as the classic path): cell rows whose projected
     * square lies wholly outside the screen, with two rows of margin. */
    int ytop=(int)floorf(-view->offset_y/view->scale)-24-32,ybot=(int)ceilf((240.0f-view->offset_y)/view->scale)-8+32;
    for (unsigned r=cell_index.layer_runs[layer];r<cell_index.layer_runs[layer+1];r++) {
        unsigned first,end;scene_cell_run_window(&cell_index,r,&w,&first,&end);
        layer_visits[layer]+=end-first;
        for (unsigned k=first;k<end;) {
            const scene_map_cell_t *cell=&cells[cell_index.order[k]];
            if (cell->y<ytop) {k=scene_cell_column_from(&cell_index,k,scene_cell_column_end(&cell_index,k,end),ytop);continue;}
            if (cell->y>=ybot) {k=scene_cell_column_end(&cell_index,k,end);continue;}
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
}
/* Pre-rendered columns (scene_prerender.h): each run of a
 * resident column is one upload of the ring's texels (the 16x256 CI4 page
 * layout from the run's first texel) and one rectangle; a column a map
 * patch may replace goes cell by cell (hidden, as the tile path). Animated
 * cells of the layer keep the tile path. */
#if MSX_BG_PRERENDER
static bool prerender_on=true;
static uint16_t *prerender_anim[4];static unsigned prerender_anim_count[4];
static void blit_raw(const uint8_t *texels,unsigned palette,int x,int y,unsigned cells) {
    set_tlut(TLUT_RGBA16);unsigned slot=palette_slot(palette);
    surface_t texture=surface_make((void*)texels,FMT_CI4,16,256,8);
    uint32_t u0=TICKS_READ();tmem_bytes+=(unsigned)page_upload(&texture,2,slot);upload_ticks+=TICKS_READ()-u0;uploads++;
    last_page=-1;last_slot=(int)slot;last_format=2;last_width=16;last_height=256;
    uint32_t r0=TICKS_READ();
    rdpq_texture_rectangle_scaled(TILE0,x,y,x+16,y+16*(int)cells,0,0,16,16*(int)cells);
    rect_ticks+=TICKS_READ()-r0;draws++;
}
static void draw_layer_prerender(unsigned layer,const int *offsets,const int *rises,unsigned video_tick) {
    int off=offsets[layer],first,end,cx;
    scene_prerender_prefetch(layer,off+8-15-64,off+312+64);
    scene_prerender_range(layer,off+8-15,off+312,&first,&end,&cx);
    unsigned base=scene_prerender_palette_base();
    for (int c=first;c<end;c++) {
        unsigned n;const uint8_t *tex;const scene_prerender_run_t *r=scene_prerender_column(layer,c,&n,&tex);
        if (!r) continue;
        layer_visits[layer]+=n;
        scene_map_cell_t probe={(uint8_t)layer,SCENE_CELL_STATIC,(int16_t)(cx+16*c),0,0};
        bool may=column_may_hide(&probe);int sx=probe.x-off;
        for (unsigned k=0;k<n;k++) {
            int y=8+r[k].y+rises[layer];unsigned cells=r[k].n;
            if (!(y+16*(int)cells<=8 || y>=232)) {
                if (!may) blit_raw(tex,base+r[k].palette,sx,y,cells);
                else for (unsigned j=0;j<cells;j++) {probe.y=(int16_t)(r[k].y+16*(int)j);if (!hidden(&probe)) blit_raw(tex+128*j,base+r[k].palette,sx,y+16*(int)j,1);}
            }
            tex+=128*cells;
        }
    }
    const scene_map_cell_t *cells=cell_index.cells;
    for (unsigned i=0;i<prerender_anim_count[layer];i++) {
        const scene_map_cell_t *cell=&cells[prerender_anim[layer][i]];
        if (cell->x-off>-16 && cell->x-off<320 && !hidden(cell)) classic_cell(cell,offsets,rises,video_tick,1);
    }
}
/* The animated cells of the pre-rendered layers of the loaded stage. */
static void prerender_attach(const scene_stage_t *st) {
    for (unsigned l=0;l<4;l++) {free(prerender_anim[l]);prerender_anim[l]=NULL;prerender_anim_count[l]=0;}
    unsigned m=st?scene_prerender_layers():0u;
    for (unsigned l=0;l<4;l++) if (m>>l&1u) {
        unsigned n=0;
        for (unsigned i=0;i<st->cell_count;i++) if (st->cells[i].layer==l && st->cells[i].anim!=SCENE_CELL_STATIC) n++;
        if (!n) continue;
        prerender_anim[l]=malloc(n*sizeof(uint16_t));
        for (unsigned i=0;i<st->cell_count;i++) if (st->cells[i].layer==l && st->cells[i].anim!=SCENE_CELL_STATIC) prerender_anim[l][prerender_anim_count[l]++]=(uint16_t)i;
    }
}
#else
static void prerender_attach(const scene_stage_t *st) {(void)st;}
#endif
/* A layer drawn by its hardware column ring (Mission 3 train 10D7B6,
 * neo_train.c; docs/MISSION3_DEBUG.md G24): column k of the ring at screen x
 * + 16 k shows the pack column at map_x[k], each with its own offset, as the
 * sprites of the original keep the column loaded with the origin of their
 * time. Pre-rendered: column k stays in ring slot (first + k) & 31, the
 * original's slot, which the original reloads only off screen. Tile path:
 * the runs of one offset, each under its own scissor. */
static void draw_layer_ring(unsigned layer,const int *offsets,const int *rises,const bool *visible,unsigned video_tick) {
    const layer_ring_t *ring=&layer_ring[layer];
    int offs[5];memcpy(offs,offsets,sizeof(offs));
    uint32_t t0=TICKS_READ();unsigned d0=draws,u0=uploads;uint32_t b0=upload_ticks+rect_ticks;unsigned c0=run_touch;
#if MSX_BG_PRERENDER
    if (prerender_on && (scene_prerender_layers()>>layer&1u)) {
        int col[32];
        for (unsigned k=0;k<32;k++) {
            col[k]=ring->map_x[k]==INT16_MIN?-1:scene_prerender_column_of(layer,ring->map_x[k]);
            if (col[k]>=0) scene_prerender_prefetch_slot(layer,ring->first+k,col[k]);
        }
        unsigned base=scene_prerender_palette_base();
        for (unsigned k=0;k<32;k++) {
            int sx=ring->x+16*(int)k;
            if (col[k]<0 || sx+16<=8 || sx>=312) continue;
            unsigned n;const uint8_t *tex;const scene_prerender_run_t *r=scene_prerender_column_slot(layer,ring->first+k,col[k],&n,&tex);
            if (!r) continue;
            layer_visits[layer]+=n;
            scene_map_cell_t probe={(uint8_t)layer,SCENE_CELL_STATIC,ring->map_x[k],0,0};
            bool may=column_may_hide(&probe);
            for (unsigned j=0;j<n;j++) {
                int y=8+r[j].y+rises[layer];unsigned cells=r[j].n;
                if (!(y+16*(int)cells<=8 || y>=232)) {
                    if (!may) blit_raw(tex,base+r[j].palette,sx,y,cells);
                    else for (unsigned i=0;i<cells;i++) {probe.y=(int16_t)(r[j].y+16*(int)i);if (!hidden(&probe)) blit_raw(tex+128*i,base+r[j].palette,sx,y+16*(int)i,1);}
                }
                tex+=128*cells;
            }
            /* The layer's animated cells keep the tile path (none on 10D7B6). */
            for (unsigned i=0;i<prerender_anim_count[layer];i++) {
                const scene_map_cell_t *cell=&cell_index.cells[prerender_anim[layer][i]];
                if (cell->x==ring->map_x[k] && !hidden(cell)) {offs[layer]=ring->map_x[k]-sx;classic_cell(cell,offs,rises,video_tick,1);}
            }
        }
    } else
#endif
    {
        ring_segment_t seg[32];unsigned segs=ring_segments(ring,seg);
        for (unsigned g=0;g<segs;g++) {
            rdpq_set_scissor(seg[g].x0<8?8:seg[g].x0,8,seg[g].x1>312?312:seg[g].x1,232);
            offs[layer]=seg[g].offset;
            if (cell_index.layer_major) {
                const scene_strip_t *st=NULL;
                if (strips_on && strip_items) {int wx0,wx1,wy0,wy1;layer_bounds(layer,offs,rises,&wx0,&wx1,&wy0,&wy1);st=layer_strip(layer,wx0,wx1,wy0,wy1);}
                if (st) draw_layer_strip(layer,st,offs,rises,video_tick); else draw_layer_direct(layer,offs,rises,video_tick);
            } else {
                unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
                for (unsigned i=0;i<count;i++) if (cells[i].layer==layer && !hidden(&cells[i])) classic_cell(&cells[i],offs,rises,video_tick,1);
            }
        }
        rdpq_set_scissor(8,8,312,232);
    }
    if (map_patch_mask) {
        ring_segment_t seg[32];unsigned segs=ring_segments(ring,seg);
        for (unsigned g=0;g<segs;g++) {
            rdpq_set_scissor(seg[g].x0<8?8:seg[g].x0,8,seg[g].x1>312?312:seg[g].x1,232);
            offs[layer]=seg[g].offset;map_patch_draw(layer,NULL,offs,rises,video_tick);
        }
        rdpq_set_scissor(8,8,312,232);
    }
    (void)visible;
    layer_blit[layer]+=upload_ticks+rect_ticks-b0;layer_touch[layer]+=run_touch-c0;
    layer_ticks[layer]+=TICKS_READ()-t0;layer_draws[layer]+=draws-d0;layer_uploads[layer]+=uploads-u0;
}
static void draw_layers(unsigned mask,const int *offsets,const int *rises,const bool *visible,unsigned video_tick) {
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    // Pack order is layer order (checked at init), so walking the layers
    // in turn draws the same cells in the same order as one array walk.
    // Same split in both render modes: the layers of the mask only (the
    // sprite windows are drawn between the passes); the mesh after layer 4.
    for (unsigned layer=0;layer<5;layer++) {
      if (!(mask&(1u<<layer)) || !visible[layer]) continue;
      if (layer<4 && layer_ring[layer].on && !depth_mode) {draw_layer_ring(layer,offsets,rises,visible,video_tick);continue;}
      uint32_t t0=TICKS_READ();unsigned d0=draws,u0=uploads;uint32_t b0=upload_ticks+rect_ticks;unsigned c0=run_touch;
#if MSX_BG_PRERENDER
      /* Only mission pack stages have pre-rendered layers: their depth views
       * are 1:1 at (-offsets, rises), the classic coordinates. */
      if (prerender_on && (scene_prerender_layers()>>layer&1u)) draw_layer_prerender(layer,offsets,rises,video_tick);
      else
#endif
      if (depth_mode || cell_index.layer_major) {
          const scene_strip_t *s=NULL;
          if (strips_on && strip_items) {int wx0,wx1,wy0,wy1;layer_bounds(layer,offsets,rises,&wx0,&wx1,&wy0,&wy1);s=layer_strip(layer,wx0,wx1,wy0,wy1);}
          if (s) draw_layer_strip(layer,s,offsets,rises,video_tick); else draw_layer_direct(layer,offsets,rises,video_tick);
      }
      const retro_plane_view_t *view=depth_mode?&depth_views[layer]:NULL;
      // Replacement cells (shop patch, post-collapse rubble) on their layer.
      if (layer==2 && patched) {unsigned n;const scene_map_cell_t *p=scene_patch_cells(&n);if (view) for (unsigned i=0;i<n;i++) depth_cell(&p[i],view,offsets,video_tick,1); else draw_cells(p,n,offsets,rises,video_tick);}
      if (layer==3 && collapsed) {unsigned n;const scene_map_cell_t *p=scene_collapse_cells(&n);if (view) for (unsigned i=0;i<n;i++) depth_cell(&p[i],view,offsets,video_tick,1); else draw_cells(p,n,offsets,rises,video_tick);}
      if (map_patch_mask) map_patch_draw(layer,view,offsets,rises,video_tick);
      uint32_t t1=TICKS_READ();unsigned d1=draws,u1=uploads;layer_blit[layer]+=upload_ticks+rect_ticks-b0;layer_touch[layer]+=run_touch-c0;
      if (layer==3) draw_tower(offsets,rises,view,video_tick);
      layer_ticks[layer]+=t1-t0;layer_draws[layer]+=d1-d0;layer_uploads[layer]+=u1-u0;
      layer_ticks[5]+=TICKS_READ()-t1;layer_draws[5]+=draws-d1;layer_uploads[5]+=uploads-u1;
      if (depth_mode && (MSX_DEPTH_STYLE&DEPTH_FOG) && layer<2) {
          retro_effects_fog(&depth_frame,layer);rdpq_set_scissor(8,8,312,232);
      }
    }
    // Any other pack order (classic mode): the plain array walk.
    if (!depth_mode && !cell_index.layer_major) for (unsigned i=0;i<count;i++)
        if (cells[i].layer<5 && (mask&(1u<<cells[i].layer)) && visible[cells[i].layer] && !(cells[i].layer<4 && layer_ring[cells[i].layer].on) && !hidden(&cells[i])) classic_cell(&cells[i],offsets,rises,video_tick,1);
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
    bool valid;bool collapsed,patched;uint16_t map_patches;unsigned tower;int offsets[5],rises[5];bool visible[5];
    retro_plane_view_t views[5];
    layer_ring_t rings[4];
} bg_key_t;
static bg_key_t bg_key;
static rspq_block_t *bg_blocks[2][8];static unsigned bg_block_draws[2][8];
static unsigned bg_group,bg_last_used;     // pin group of the live blocks; last frame that ran one
/* Retired generations (one per pin group): freed once their last frame is done. */
typedef struct {bool pending;unsigned epoch;rspq_block_t *blocks[16];} bg_retired_t;
static bg_retired_t bg_retired[4];
static unsigned bg_replays,bg_records;
static int bg_block_max;   // largest heap taken by one recording (MSX_DIAG): the block sizes BG_RECORD_MIN_FREE allows for
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
    if (!bg_key.valid || bg_key.collapsed!=collapsed || bg_key.patched!=patched || bg_key.map_patches!=map_patch_mask || bg_key.tower!=tower_count) return false;
    for (unsigned i=0;i<5;i++) if (bg_key.offsets[i]!=offsets[i] || bg_key.rises[i]!=rises[i] || bg_key.visible[i]!=visible[i]) return false;
    if (depth_mode && memcmp(bg_key.views,depth_views,sizeof(bg_key.views))) return false;
    for (unsigned p=0;p<2;p++) for (unsigned t=0;t<8;t++) if (!bg_blocks[p][t]) return false;
    return true;
}
/* Record only after the inputs held this many frames (two game updates). */
enum { BG_RECORD_HELD=3, BG_RECORD_MIN_FREE=(256+32)*1024 };
/* A block needs heap for its RSP and RDP buffers, whose allocations
 * libdragon does not check: never record with the heap nearly full. The
 * blocks are optional (the layers draw directly), so they stop short of the
 * 256 KB margin of the RAM budget (scene_assets.c) by one block (bgmax in
 * the MSX RDP line); at 192 KB they took the Mission 1 boss to 234 KB free. */
static bool bg_heap_ok(void) { heap_stats_t h;sys_get_heap_stats(&h);return h.total-h.used>=BG_RECORD_MIN_FREE; }
/* After a block runs, the RSP keeps writing the following RDP commands
 * (the rest of the frame, its SYNC_FULL, the next frames until a block or
 * a full buffer switches it) into the trailing space of that block's last
 * static RDP buffer (libdragon __rdpq_block_end). Freeing a retired block
 * once its frame is done (bg_collect) therefore let the RSP write commands
 * into freed heap: corrupted malloc lists (mallinfo read a framebuffer
 * address as a chunk link), then a hang without exception in Mission 2
 * stage 2, where the blocks are retired every other frame. Every replay
 * is followed by this permanent block (the scissor the caller wanted
 * anyway), so the RSP's write pointer is always in a buffer never freed.
 * scene_renderer_stage_changed frees after rspq_wait, which switches the
 * RSP back to its dynamic buffer. */
static rspq_block_t *bg_tail;
static void bg_tail_run(void) {
    if (!bg_tail) {rspq_block_begin();rdpq_set_scissor(8,8,312,232);bg_tail=rspq_block_end();}
    rspq_block_run(bg_tail);
}
static void draw_layers_cached(unsigned mask,bool first,const int *offsets,const int *rises,const bool *visible,unsigned video_tick) {
    bg_collect();
    bool cacheable=!fade_on && !flash && !(MSX_DEPTH_STYLE&DEPTH_FOG) && !bg_retired[bg_group].pending;
    bg_key_t key;memset(&key,0,sizeof(key));
    key.valid=true;key.collapsed=collapsed;key.patched=patched;key.map_patches=map_patch_mask;key.tower=tower_count;
    for (unsigned i=0;i<5;i++) {key.offsets[i]=offsets[i];key.rises[i]=rises[i];key.visible[i]=visible[i];}
    memcpy(key.rings,layer_ring,sizeof(key.rings));
    if (depth_mode) memcpy(key.views,depth_views,sizeof(key.views));
    /* Frames the inputs have held, counted once per frame (back pass): the
     * game updates every other frame, so a scrolling camera repeats each
     * key once. Recording on that repeat retired and re-recorded both
     * passes every two frames (Mission 2 stage 2: ~200 KB of blocks in
     * flight); with the heap exhausted, the unchecked malloc_uncached of
     * rspq_block_begin/__rdpq_block_next_buffer wrote through address 0
     * (uncached KSEG1) and the RSP/RDP hung (docs/MISION2.md, fase 2c). */
    static bg_key_t previous;static unsigned held;
    if (first) {held=memcmp(&key,&previous,sizeof(key))?0:held+1;previous=key;}
    if (!cacheable || memcmp(&key,&bg_key,sizeof(key))) {
        if (bg_key.valid) bg_retire();
        bool repeat=cacheable && !bg_retired[bg_group].pending && held>=BG_RECORD_HELD && !memcmp(&key,&previous,sizeof(key)) && bg_heap_ok();
        if (!repeat) {draw_layers(mask,offsets,rises,visible,video_tick);return;}
        bg_key=key;
    }
    unsigned pass=first?0u:1u,phase=video_tick&7u;
    if (!bg_blocks[pass][phase] && !bg_heap_ok()) {draw_layers(mask,offsets,rises,visible,video_tick);return;}
    if (!bg_blocks[pass][phase]) {
        reset_texture_state();
        unsigned before=draws;
        scene_record_pins((int)bg_group);
#if MSX_DIAG
        heap_stats_t h0;sys_get_heap_stats(&h0);
#endif
        rspq_block_begin();
        draw_layers(mask,offsets,rises,visible,video_tick);
        bg_blocks[pass][phase]=rspq_block_end();
#if MSX_DIAG
        {heap_stats_t h1;sys_get_heap_stats(&h1);if (h1.used-h0.used>bg_block_max) bg_block_max=h1.used-h0.used;}
#endif
        scene_record_pins(-1);bg_records++;
        bg_block_draws[pass][phase]=draws-before;draws=before;
    }
    rspq_block_run(bg_blocks[pass][phase]);bg_replays++;bg_last_used=frame;draws+=bg_block_draws[pass][phase];
    reset_texture_state();bg_tail_run();
}
static void background(unsigned camera,int height,bool backdrop_after,unsigned video_tick,unsigned mask,bool first) {
    unsigned count;const scene_map_cell_t *cells=scene_map_cells(&count);
    // Layer 4 holds the market props; it scrolls with the foreground.
    int offsets[5],rises[5];bool visible[5];
    const scene_stage_t *st=scene_stage();
    for (unsigned i=0;i<5;i++) {
        unsigned source=i<4?i:3;
        if (st) {
            /* Mission pack stage: the fitted rules (tools/pack_mission.py). */
            const scene_layer_rule_t *r=&st->rules[source];
            offsets[i]=((int)camera*r->x_num+r->x_den-1)/r->x_den;
            if (i<4 && layer_own[i]) offsets[i]=layer_own_offset[i];
            /* A ring layer: its columns carry their own offsets (draw_layers). */
            if (i<4 && layer_ring[i].on) offsets[i]=0;
            rises[i]=scene_layer_rise(r,source,height);
            visible[i]=i<4 && (((int)camera>=r->visible_min && (int)camera<=r->visible_max) || layer_own[i] || layer_ring[i].on);
            continue;
        }
        offsets[i]=msx_layer_scroll(source,(int)camera);rises[i]=msx_layer_scroll(source,height);
        visible[i]=msx_layer_visible(source,(int)camera);
    }
    if (collapsed) offsets[0]+=COLLAPSE_SKY_SHIFT;
    // Keep the projection outside assert(): NDEBUG would otherwise drop it.
    if (depth_mode) for (unsigned i=0;i<5;i++) {
        /* Mission pack stages: the planes keep the stage's own scroll
         * (the fitted rules above: MAME's layer objects, e.g. Mission 2
         * stage 1's water strip 1:1 and 10D7B6 at 1/2), unscaled. The
         * Mission 1 parallax (sky at 0.04, scale 1.08) left that stage's
         * bottom strip almost still under the moving foreground. */
        bool ok=st?(depth_views[i]=(retro_plane_view_t){1.0f,(float)-offsets[i],(float)rises[i]},true):
            (MSX_DEPTH_STYLE&DEPTH_PERSPECTIVE)?
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
    unsigned last_now=~0u,last_next=~0u;bool prefetch=first && !scene_assets_map_resident() && !bg_static(offsets,rises,visible);
    for (unsigned layer=0;layer<5 && prefetch;layer++) {
        if (!visible[layer]) continue;
        ring_segment_t seg[32];unsigned segs=1;
        if (layer<4 && layer_ring[layer].on && !depth_mode) segs=ring_segments(&layer_ring[layer],seg);
        else seg[0]=(ring_segment_t){-48,352,offsets[layer]};
        for (unsigned g=0;g<segs;g++) {
        scene_cell_window_t w=layer_window(layer,seg[g].offset,0,(float)(seg[g].x0<8?-48:seg[g].x0),(float)(seg[g].x1>312?352:seg[g].x1),true);
        for (unsigned r=cell_index.layer_runs[layer];r<cell_index.layer_runs[layer+1];r++) {
            unsigned first,end;scene_cell_run_window(&cell_index,r,&w,&first,&end);
            for (unsigned k=first;k<end;k++) {
                const scene_map_cell_t *cell=&cells[cell_index.order[k]];
                const scene_image_t *now=scene_map_image(scene_cell_phase(cell,video_tick));
                const scene_image_t *next=scene_map_image(scene_cell_phase(cell,video_tick+1));
                if (now->page/8!=last_now) {scene_request_image(now,2);last_now=now->page/8;}
                if (next->page/8!=last_next) {scene_request_image(next,1);last_next=next->page/8;}
            }
        }
        }
    }
    if (first) {scene_service();msx_perf_mark(MSX_PERF_BG_PREFETCH);}
    rdpq_set_scissor(8,8,312,232);
    if (flash) {
        if (!first) {rdpq_set_scissor(0,0,320,240);return;}
        scene_render_box(8,8,304,224,RGBA32(255,255,255,255));
        rdpq_set_scissor(0,0,320,240);msx_perf_mark(MSX_PERF_BG_DRAW);return;
    }
    if (first) {
        color_t backdrop=color_from_packed32(scene_map_backdrop(backdrop_after));
        if (fade_on) {   /* the backdrop entry 0xFFF fades with the others (MAME runK) */
            backdrop.r=(uint8_t)(neo_fade_channel(backdrop.r>>3,fade_offset[0])<<3);
            backdrop.g=(uint8_t)(neo_fade_channel(backdrop.g>>3,fade_offset[1])<<3);
            backdrop.b=(uint8_t)(neo_fade_channel(backdrop.b>>3,fade_offset[2])<<3);
        }
        scene_render_box(8,8,304,224,backdrop);
    }
#if MSX_BG_CRC
    /* Pixel check of the strips and the raw page uploads (B47, local
     * builds: MSX_EXTRA_CFLAGS=-DMSX_BG_CRC=1): every 97th frame each pass
     * is drawn twice before the real one, new path then old path, and the
     * framebuffer hashed after each; the hashes must agree. */
    if (!(frame%97) && !fade_on) {
        uint32_t sums[2],raw[2];
        for (unsigned way=0;way<2;way++) {
            strips_on=raw_upload=!way;
#if MSX_BG_PRERENDER
            prerender_on=!way;
#endif
           reset_texture_state();
            draw_layers(mask,offsets,rises,visible,video_tick);
            rspq_wait();
            const surface_t *fb=current->surface;const uint8_t *base=fb->buffer;uint32_t h=2166136261u,g=h;
            data_cache_hit_invalidate(fb->buffer,fb->stride*fb->height);
            /* The colour bits (RGB 5:5:5); the low bit is the coverage the
             * RDP writes (1 in copy mode, the texel's alpha; 7 in 1-cycle
             * mode), which FILTERS_RESAMPLE never shows: hashed apart. */
            for (unsigned y=8;y<232;y++) {const uint16_t *row=(const uint16_t*)(base+y*fb->stride);for (unsigned x=8;x<312;x++) {h=(h^(row[x]&0xfffeu))*16777619u;g=(g^row[x])*16777619u;}}
            sums[way]=h;raw[way]=g;reset_texture_state();
        }
        strips_on=raw_upload=true;
#if MSX_BG_PRERENDER
        prerender_on=true;
#endif
       
        debugf("MSX BG CRC frame=%u camera=%u mask=%x rgb new=%08lx old=%08lx %s (with coverage %08lx/%08lx)\n",frame,camera,mask,(unsigned long)sums[0],(unsigned long)sums[1],sums[0]==sums[1]?"same":"DIFFERENT",(unsigned long)raw[0],(unsigned long)raw[1]);
    }
#endif
    draw_layers_cached(mask,first,offsets,rises,visible,video_tick);
    if (!depth_mode) {rdpq_set_scissor(0,0,320,240);msx_perf_mark(MSX_PERF_BG_DRAW);return;}
    msx_perf_mark(MSX_PERF_BG_DRAW);
    if (mask&0x10u) {retro_effects_mesh(&depth_frame);msx_perf_mark(MSX_PERF_MESH);}
    rdpq_set_scissor(0,0,320,240);
}
/* A mission pack stage was loaded or Mission 1 restored (scene_assets.c):
 * the recorded blocks read the old map's slots, the cull index its cells. */
void scene_renderer_stage_changed(void) {
    rspq_wait();
    for (unsigned g=0;g<4;g++) {
        bg_retired_t *r=&bg_retired[g];
        for (unsigned i=0;i<16;i++) if (r->blocks[i]) {rspq_block_free(r->blocks[i]);r->blocks[i]=NULL;}
        r->pending=false;scene_unpin_group((int)g);
    }
    for (unsigned p=0;p<2;p++) for (unsigned t=0;t<8;t++) if (bg_blocks[p][t]) {rspq_block_free(bg_blocks[p][t]);bg_blocks[p][t]=NULL;}
    bg_key.valid=false;collapsed=flash=patched=false;
    const scene_stage_t *st=scene_stage();
    tower_index(st);map_patch_index(st);strips_attach(st);prerender_attach(st);
    if (st) {
        bool indexed=scene_cell_index_build_into(&cell_index,st->cells,st->cell_count,st->index_order,st->index_runs);
        assertf(indexed,"Stage cell index");(void)indexed;
    } else cell_index=mission1_index;
    scene_render_forget_texture();
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
        debugf("MSX RDP: draws=%u tex=%u TLUT=%u TMEM=%u B CPU=%lu us PI=%u loads/%u bytes misses=%u evictions=%u deferred=%u held=%u sync=%u idle=%u decode_max=%lu us heap_low=%d KB bgblocks=%u/%u bgmax=%d KB\n",
            draws,uploads,palette_uploads,tmem_bytes,(unsigned long)TICKS_TO_US(TICKS_SINCE(current->start_ticks)),
            s.loads,s.dma_bytes,s.misses,s.evictions,s.deferred,s.held,s.sync_misses,s.idle_decodes,(unsigned long)TICKS_TO_US(s.decode_max_ticks),scene_assets_heap_low()/1024,bg_replays,bg_records,bg_block_max/1024);
    }
    if (!(frame%300)) debugf("MSX BG: per frame (300) layer us/draws/uploads/visits 0=%lu/%u/%u/%u 1=%lu/%u/%u/%u 2=%lu/%u/%u/%u 3=%lu/%u/%u/%u 4=%lu/%u/%u/%u tower=%lu/%u/%u upload=%lu us rect=%lu us blit/touch 0=%lu/%u 1=%lu/%u 2=%lu/%u 3=%lu/%u strips builds=%u (%lu us) resets=%u direct=%u items=%u\n",
        (unsigned long)TICKS_TO_US(layer_ticks[0]/300),layer_draws[0]/300,layer_uploads[0]/300,layer_visits[0]/300,(unsigned long)TICKS_TO_US(layer_ticks[1]/300),layer_draws[1]/300,layer_uploads[1]/300,layer_visits[1]/300,
        (unsigned long)TICKS_TO_US(layer_ticks[2]/300),layer_draws[2]/300,layer_uploads[2]/300,layer_visits[2]/300,(unsigned long)TICKS_TO_US(layer_ticks[3]/300),layer_draws[3]/300,layer_uploads[3]/300,layer_visits[3]/300,
        (unsigned long)TICKS_TO_US(layer_ticks[4]/300),layer_draws[4]/300,layer_uploads[4]/300,layer_visits[4]/300,(unsigned long)TICKS_TO_US(layer_ticks[5]/300),layer_draws[5]/300,layer_uploads[5]/300,
        (unsigned long)TICKS_TO_US(upload_ticks/300),(unsigned long)TICKS_TO_US(rect_ticks/300),
        (unsigned long)TICKS_TO_US(layer_blit[0]/300),layer_touch[0]/300,(unsigned long)TICKS_TO_US(layer_blit[1]/300),layer_touch[1]/300,(unsigned long)TICKS_TO_US(layer_blit[2]/300),layer_touch[2]/300,(unsigned long)TICKS_TO_US(layer_blit[3]/300),layer_touch[3]/300,
        strip_builds,(unsigned long)TICKS_TO_US(strip_build_ticks),strip_resets,strip_direct,strip_pool.items_used);
#if MSX_BG_PRERENDER
    if (!(frame%300) && scene_prerender_layers()) {
        unsigned ring,sync,async;scene_prerender_stats(&ring,&sync,&async);
        debugf("MSX PRERENDER: layers=%x ring=%u KB column reads sync=%u async=%u\n",scene_prerender_layers(),ring/1024,sync,async);
    }
#endif
    if (!(frame%300)) {
        memset(layer_blit,0,sizeof(layer_blit));memset(layer_touch,0,sizeof(layer_touch));upload_ticks=rect_ticks=0;
        memset(layer_ticks,0,sizeof(layer_ticks));memset(layer_draws,0,sizeof(layer_draws));memset(layer_uploads,0,sizeof(layer_uploads));memset(layer_visits,0,sizeof(layer_visits));
        strip_builds=strip_resets=strip_direct=0;strip_build_ticks=0;
    }
    // One completion fence per frame: RDRAM pages stay pinned until RDP is done.
    rdpq_detach_cb(finished,current);
}
