#include <libdragon.h>
#include <stdio.h>
#include "marco_renderer.h"
#include "scene_renderer.h"
#include "marco_pose_meta.h"
static sprite_t *parts[MARCO_PARTS];
static bool loaded;
static unsigned next_part,part_bytes;static bool part_failed;   // marco_renderer_preload
static unsigned max_upload;
unsigned marco_renderer_count(void) {return MARCO_POSES;}
/* The palette profile of the burnt death (0BF358, kinds 7/0x10): the one
 * the catalog build names BURNT (build_marco_poses.py --name-palette), else
 * the first. */
unsigned marco_renderer_burnt_palette(void) {
#ifdef MARCO_PALETTE_BURNT
    return MARCO_PALETTE_BURNT;
#else
    return 0;
#endif
}
unsigned marco_renderer_mummy_palette(bool second) {
#if defined(MARCO_PALETTE_MUMMY) && defined(MARCO_PALETTE_MUMMY2)
    return second?MARCO_PALETTE_MUMMY2:MARCO_PALETTE_MUMMY;
#else
    (void)second;return 0;
#endif
}
unsigned marco_renderer_palette_count(void) {return MARCO_PALETTES;}
unsigned marco_renderer_max_upload(void) {return max_upload;}
uint32_t marco_renderer_source_tag(unsigned pose) {return pose<MARCO_POSES?marco_poses[pose].source_tag:0;}
bool marco_renderer_find(uint32_t tag,unsigned *pose) {
    unsigned lo=0,hi=MARCO_POSES;
    while (lo<hi) {
        unsigned mid=lo+(hi-lo)/2;
        if (marco_poses[mid].source_tag<tag) lo=mid+1;else hi=mid;
    }
    if (!pose || lo==MARCO_POSES || marco_poses[lo].source_tag!=tag) return false;
    *pose=lo;return true;
}
void marco_renderer_close(void) {
    for (unsigned i=0;i<MARCO_PARTS;i++) if (parts[i]) {sprite_free(parts[i]);parts[i]=NULL;}
    loaded=false;max_upload=0;next_part=part_bytes=0;part_failed=false;
}
static uint32_t part_cost;   // ticks of the slowest recent part load
static bool load_part(unsigned i) {
    {
        heap_stats_t h0;sys_get_heap_stats(&h0);
        char path[48];snprintf(path,sizeof(path),"rom:/marco/p%u.sprite",i);parts[i]=sprite_load(path);
        heap_stats_t h1;sys_get_heap_stats(&h1);part_bytes+=(unsigned)(h1.used-h0.used);
        if (!parts[i]) {debugf("MSX marco part %u: not loaded\n",i);return false;}
        surface_t pixels=sprite_get_pixels(parts[i]);
        if (sprite_get_format(parts[i])!=FMT_CI4 || parts[i]->width!=marco_parts[i].width ||
            parts[i]->height!=marco_parts[i].height || (unsigned)pixels.stride*pixels.height>2048) {
            debugf("MSX marco part %u: format/size %ux%u\n",i,parts[i]->width,parts[i]->height);return false;
        }
        const uint16_t *palette=sprite_get_palette(parts[i]);
        for (unsigned pen=0;pen<16;pen++) if (palette[pen]!=marco_palettes[0][pen]) {
            debugf("MSX marco part %u: palette pen %u\n",i,pen);return false;
        }
        uint32_t fingerprint=2166136261u;const uint8_t *data=pixels.buffer;
        for (unsigned y=0;y<pixels.height;y++) for (unsigned x=0;x<pixels.width;x++)
            fingerprint=(fingerprint^((data[y*pixels.stride+x/2]>>((x&1)?0:4))&15u))*16777619u;
        if (fingerprint!=marco_parts[i].indices_fnv) {debugf("MSX marco part %u: pixels differ\n",i);return false;}
    }
    return true;
}
bool marco_renderer_preload(unsigned long deadline,int keep) {
    if (loaded || part_failed || next_part>=MARCO_PARTS) return false;
    if (!part_cost) part_cost=TICKS_FROM_US(1500);
    int32_t left=(int32_t)((uint32_t)deadline-TICKS_READ());
    if (left<=(int32_t)part_cost) return false;
    heap_stats_t h;sys_get_heap_stats(&h);
    if (h.total-h.used<keep+4096) return false;
    uint32_t t0=TICKS_READ();
    if (!load_part(next_part)) {part_failed=true;return false;}
    next_part++;
    uint32_t spent=TICKS_READ()-t0;part_cost-=part_cost/16;if (spent>part_cost) part_cost=spent;
    return true;
}
unsigned marco_renderer_preloaded_bytes(void) {return part_bytes;}
bool marco_renderer_load(void) {
    if (loaded) return true;
    if (part_failed) {marco_renderer_close();return false;}
    unsigned before=next_part;
    for (;next_part<MARCO_PARTS;next_part++) if (!load_part(next_part)) {marco_renderer_close();return false;}
    if (before) debugf("MSX marco parts: %u of %u loaded before the handoff\n",before,MARCO_PARTS);
    data_cache_hit_writeback((void*)marco_palettes,sizeof(marco_palettes));
    loaded=true;return true;
}
static bool draw_pose(unsigned pose,int x,int y,bool mirror,bool flip_y,unsigned palette,bool wrapped,int viewport_x,int viewport_y) {
    if (!loaded || pose>=MARCO_POSES || palette>=MARCO_PALETTES) return false;
    rdpq_mode_push();rdpq_set_mode_standard();rdpq_mode_filter(FILTER_POINT);
    rdpq_mode_dithering(DITHER_NONE_NONE);rdpq_mode_alphacompare(1);
    rdpq_mode_tlut(TLUT_RGBA16);
    rdpq_tex_upload_tlut((uint16_t*)scene_render_fade_palette(marco_palettes[palette]),0,16);
    for (unsigned p=0;p<marco_poses[pose].count;p++) {
        unsigned i=marco_poses[pose].first+p;
        int width=marco_parts[i].width,height=marco_parts[i].height;
        int left=x+(mirror?-marco_parts[i].x-width-1:marco_parts[i].x);
        int top=y+(flip_y?-marco_parts[i].y-height-1:marco_parts[i].y);
        if (wrapped) {left=(int)((unsigned)left&511u);top=(int)((unsigned)top&511u);}
        unsigned copies=wrapped?2:1;bool uploaded=false;
        for (unsigned wy=0;wy<copies;wy++) for (unsigned wx=0;wx<copies;wx++) {
            int xx=left+viewport_x-(int)wx*512,yy=top+viewport_y-(int)wy*512;
            if (xx>=320 || xx+width<=0 || yy>=240 || yy+height<=0) continue;
            if (!uploaded) {
                surface_t pixels=sprite_get_pixels(parts[i]);
                int bytes=rdpq_tex_upload(TILE0,&pixels,NULL);
                if (bytes<0 || bytes>2048) {rdpq_mode_pop();return false;}
                if ((unsigned)bytes>max_upload) max_upload=(unsigned)bytes;
                uploaded=true;
            }
            rdpq_texture_rectangle_scaled(TILE0,xx,yy,xx+width,yy+height,
                mirror?width-1:0,flip_y?height-1:0,mirror?-1:width,flip_y?-1:height);
        }
    }
    rdpq_mode_pop();return true;
}
bool marco_renderer_draw_palette(unsigned pose,int x,int y,bool mirror,unsigned palette) {
    return draw_pose(pose,x,y,mirror,false,palette,false,0,0);
}
bool marco_renderer_draw(unsigned pose,int x,int y,bool mirror) {
    return marco_renderer_draw_palette(pose,x,y,mirror,0);
}
bool marco_renderer_draw_native(const neo_anim_draw_t *draw,int viewport_x,int viewport_y,unsigned palette) {
    unsigned pose;
    if (!draw || !marco_renderer_find(draw->descriptor,&pose)) return false;
    return draw_pose(pose,draw->x&511u,(0u-draw->y)&511u,
        (draw->attributes&1u)!=0,(draw->attributes&2u)!=0,palette,true,viewport_x,viewport_y);
}
