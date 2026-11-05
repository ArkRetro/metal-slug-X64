#include <libdragon.h>
#include <stdio.h>
#include "marco_campaign_renderer.h"
#include "marco_campaign_pose_meta.h"

static sprite_t *parts[MARCO_CAMPAIGN_PARTS];
static bool loaded;
static unsigned max_upload;

unsigned marco_campaign_renderer_count(void) {return MARCO_CAMPAIGN_POSES;}
unsigned marco_campaign_renderer_max_upload(void) {return max_upload;}

void marco_campaign_renderer_close(void) {
    for (unsigned i=0;i<MARCO_CAMPAIGN_PARTS;i++) {
        if (parts[i]) {sprite_free(parts[i]);parts[i]=NULL;}
    }
    loaded=false;max_upload=0;
}

bool marco_campaign_renderer_load(void) {
    if (loaded) return true;
    for (unsigned i=0;i<MARCO_CAMPAIGN_PARTS;i++) {
        char path[64];snprintf(path,sizeof(path),"rom:/marco-campaign/p%u.sprite",i);
        parts[i]=sprite_load(path);
        if (!parts[i]) {marco_campaign_renderer_close();return false;}
        surface_t pixels=sprite_get_pixels(parts[i]);
        if (sprite_get_format(parts[i])!=FMT_CI4 ||
            parts[i]->width!=marco_campaign_parts[i].width ||
            parts[i]->height!=marco_campaign_parts[i].height ||
            (unsigned)pixels.stride*pixels.height>2048) {
            marco_campaign_renderer_close();return false;
        }
        const uint16_t *palette=sprite_get_palette(parts[i]);
        for (unsigned pen=0;pen<16;pen++) {
            if (palette[pen]!=marco_campaign_palettes[0][pen]) {
                marco_campaign_renderer_close();return false;
            }
        }
        uint32_t fingerprint=2166136261u;
        const uint8_t *data=pixels.buffer;
        for (unsigned y=0;y<pixels.height;y++) for (unsigned x=0;x<pixels.width;x++) {
            unsigned pen=(data[y*pixels.stride+x/2]>>((x&1)?0:4))&15u;
            fingerprint=(fingerprint^pen)*16777619u;
        }
        if (fingerprint!=marco_campaign_parts[i].indices_fnv) {
            marco_campaign_renderer_close();return false;
        }
    }
    data_cache_hit_writeback((void*)marco_campaign_palettes,sizeof(marco_campaign_palettes));
    loaded=true;return true;
}

bool marco_campaign_renderer_draw_native(const neo_anim_draw_t *draw,int viewport_x,int viewport_y) {
    if (!loaded || !draw) return false;
    unsigned lo=0,hi=MARCO_CAMPAIGN_POSES;
    while (lo<hi) {
        unsigned mid=lo+(hi-lo)/2;
        if (marco_campaign_poses[mid].source_tag<draw->descriptor) lo=mid+1;
        else hi=mid;
    }
    if (lo==MARCO_CAMPAIGN_POSES || marco_campaign_poses[lo].source_tag!=draw->descriptor) return false;
    unsigned bank=draw->attributes>>8,palette;
    if (bank==0x63) palette=0; /* original descriptor 0376, cartridge 206EC0 */
    else if (bank==0x64) palette=1; /* original flash descriptor 0390, cartridge 207200 */
    else return false;
    int x=draw->x&511u,y=(0u-draw->y)&511u;
    bool mirror=(draw->attributes&1u)!=0,flip_y=(draw->attributes&2u)!=0;
    rdpq_mode_push();rdpq_set_mode_standard();rdpq_mode_filter(FILTER_POINT);
    rdpq_mode_dithering(DITHER_NONE_NONE);rdpq_mode_alphacompare(1);
    rdpq_mode_tlut(TLUT_RGBA16);
    rdpq_tex_upload_tlut((uint16_t*)marco_campaign_palettes[palette],0,16);
    for (unsigned p=0;p<marco_campaign_poses[lo].count;p++) {
        unsigned i=marco_campaign_poses[lo].first+p;
        int width=marco_campaign_parts[i].width,height=marco_campaign_parts[i].height;
        int left=x+(mirror?-marco_campaign_parts[i].x-width-1:marco_campaign_parts[i].x);
        int top=y+(flip_y?-marco_campaign_parts[i].y-height-1:marco_campaign_parts[i].y);
        left=(int)((unsigned)left&511u);top=(int)((unsigned)top&511u);
        bool uploaded=false;
        for (unsigned wy=0;wy<2;wy++) for (unsigned wx=0;wx<2;wx++) {
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
