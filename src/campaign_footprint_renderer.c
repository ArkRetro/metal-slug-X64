#include <libdragon.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "campaign_footprint_renderer.h"
#include "mission_camera.h"
#include "retro25d.h"
#include "scene_assets.h"
#include "campaign_footprint_meta.h"

static sprite_t *atlases[CAMPAIGN_FOOTPRINT_ATLASES];
static bool loaded;
static unsigned max_upload;
static int current_atlas;

unsigned campaign_footprint_renderer_max_upload(void) {return max_upload;}
void campaign_footprint_renderer_close(void) {
    for(unsigned i=0;i<CAMPAIGN_FOOTPRINT_ATLASES;i++) {
        if(atlases[i]) {sprite_free(atlases[i]);atlases[i]=NULL;}
    }
    loaded=false;max_upload=0;current_atlas=-1;
}
bool campaign_footprint_renderer_load(void) {
    if(loaded) return true;
    for(unsigned i=0;i<CAMPAIGN_FOOTPRINT_ATLASES;i++) {
        char path[64];snprintf(path,sizeof(path),"rom:/campaign-footprints/a%u.sprite",i);
        atlases[i]=sprite_load(path);
        if(!atlases[i]) {
            debugf("MSX FOOTPRINT missing atlas=%u path=%s\n",i,path);
            campaign_footprint_renderer_close();return false;
        }
        surface_t pixels=sprite_get_pixels(atlases[i]);
        if(sprite_get_format(atlases[i])!=FMT_CI4 || pixels.width!=64 || pixels.height!=64 ||
            (unsigned)pixels.stride*pixels.height>2048) {
            debugf("MSX FOOTPRINT invalid atlas=%u format=%u dimensions=%ux%u stride=%u\n",
                i,sprite_get_format(atlases[i]),pixels.width,pixels.height,pixels.stride);
            campaign_footprint_renderer_close();return false;
        }
        const uint16_t *palette=sprite_get_palette(atlases[i]);
        unsigned id=campaign_footprint_atlases[i].palette;
        for(unsigned pen=0;pen<16;pen++) if(palette[pen]!=campaign_footprint_palettes[id][pen]) {
            debugf("MSX FOOTPRINT palette atlas=%u pen=%u got=%04x expected=%04x\n",
                i,pen,palette[pen],campaign_footprint_palettes[id][pen]);
            campaign_footprint_renderer_close();return false;
        }
        uint32_t fingerprint=2166136261u;const uint8_t *data=pixels.buffer;
        for(unsigned y=0;y<64;y++) for(unsigned x=0;x<64;x++) {
            unsigned pen=(data[y*pixels.stride+x/2]>>((x&1)?0:4))&15u;
            fingerprint=(fingerprint^pen)*16777619u;
        }
        if(fingerprint!=campaign_footprint_atlases[i].fingerprint) {
            debugf("MSX FOOTPRINT pixels atlas=%u got=%08lx expected=%08lx\n",i,
                (unsigned long)fingerprint,(unsigned long)campaign_footprint_atlases[i].fingerprint);
            campaign_footprint_renderer_close();return false;
        }
    }
    data_cache_hit_writeback((void*)campaign_footprint_palettes,sizeof(campaign_footprint_palettes));
    loaded=true;return true;
}
bool campaign_footprint_pixels(int column,int source_row,uint16_t rgba[256],bool *changed) {
    if (!loaded || !rgba || !changed) return false;
    *changed=false;
    if (column<0 || column>=72 || source_row<0 || source_row>=5) return true;
    unsigned image=campaign_footprint_stamps[column][source_row].image;
    if (!campaign_footprint_stamps[column][source_row].changed) return true;
    *changed=true;
    memset(rgba,0,256*sizeof(*rgba));
    if (image==UINT16_MAX) return true;
    if (image>=sizeof(campaign_footprint_images)/sizeof(*campaign_footprint_images)) return false;
    unsigned atlas=campaign_footprint_images[image].atlas;
    surface_t pixels=sprite_get_pixels(atlases[atlas]);
    const uint8_t *data=pixels.buffer;
    const uint16_t *palette=campaign_footprint_palettes[campaign_footprint_atlases[atlas].palette];
    unsigned left=campaign_footprint_images[image].x,top=campaign_footprint_images[image].y;
    for (unsigned y=0;y<16;y++) for (unsigned x=0;x<16;x++) {
        unsigned sx=left+x,sy=top+y;
        unsigned pen=(data[sy*pixels.stride+sx/2]>>((sx&1)?0:4))&15u;
        rgba[y*16+x]=palette[pen];
    }
    return true;
}
static bool tile(unsigned image,float x,float y,float size) {
    if(image==UINT16_MAX) return true; /* Original completely transparent tile. */
    if(image>=sizeof(campaign_footprint_images)/sizeof(*campaign_footprint_images)) return false;
    unsigned atlas=campaign_footprint_images[image].atlas;
    if(current_atlas!=(int)atlas) {
        surface_t pixels=sprite_get_pixels(atlases[atlas]);
        int bytes=rdpq_tex_upload(TILE0,&pixels,NULL);
        if(bytes<0 || bytes>2048) return false;
        if((unsigned)bytes>max_upload) max_upload=(unsigned)bytes;
        unsigned palette=campaign_footprint_atlases[atlas].palette;
        rdpq_tex_upload_tlut((uint16_t*)campaign_footprint_palettes[palette],0,16);
        current_atlas=(int)atlas;
    }
    int s=campaign_footprint_images[image].x,t=campaign_footprint_images[image].y;
    rdpq_texture_rectangle_scaled(TILE0,x,y,x+size,y+size,s,t,s+16,t+16);
    return true;
}
bool campaign_footprint_renderer_draw_stamp(int column,int source_row,unsigned camera,bool depth) {
    if(column<0 || column>=72 || source_row<0 || source_row>=5) return true;
    if(!loaded) return false;
    if(!campaign_footprint_stamps[column][source_row].changed) return true;
    retro_plane_view_t views[5];int height=msx_camera_height((int)camera);
    for(unsigned layer=0;layer<5;layer++) {
        if(depth) {
            retro_camera_t view={camera,-height,0,320};
            if(!retro_layered_2d_projection(view,layer,&views[layer])) return false;
        } else {
            unsigned source=layer<4?layer:3;
            views[layer]=(retro_plane_view_t){1,-msx_layer_scroll(source,(int)camera),
                msx_layer_scroll(source,height)};
        }
    }
    float x=column*16*views[3].scale+views[3].offset_x;
    float y=(120+source_row*16)*views[3].scale+views[3].offset_y;
    float size=16*views[3].scale;
    int left=(int)ceilf(x),top=(int)ceilf(y),right=(int)ceilf(x+size),bottom=(int)ceilf(y+size);
    if(left<8) left=8;
    if(top<8) top=8;
    if(right>312) right=312;
    if(bottom>232) bottom=232;
    if(left>=right || top>=bottom) return true;
    rdpq_mode_push();rdpq_set_mode_fill(color_from_packed32(scene_map_backdrop()));
    rdpq_set_scissor(left,top,right,bottom);rdpq_fill_rectangle(left,top,right,bottom);
    rdpq_set_mode_standard();rdpq_mode_filter(FILTER_POINT);
    rdpq_mode_dithering(DITHER_NONE_NONE);rdpq_mode_alphacompare(1);
    rdpq_mode_tlut(TLUT_RGBA16);current_atlas=-1;
    /* Replace the entire foreground cell. Its transparent pens must expose
     * the actual lower planes, rather than retain old prerendered sand. */
    for(unsigned layer=0;layer<5;layer++) {
        if(layer==3) {
            if(!tile(campaign_footprint_stamps[column][source_row].image,x,y,size)) goto fail;
            continue;
        }
        if(!msx_layer_visible(layer<4?layer:3,(int)camera)) continue;
        const retro_plane_view_t *view=&views[layer];float draw_size=16*view->scale;
        unsigned first=campaign_footprint_layer_first[layer];
        unsigned end=first+campaign_footprint_layer_count[layer];
        for(unsigned i=first;i<end;i++) {
            float xx=campaign_footprint_cells[i].x*view->scale+view->offset_x;
            float yy=(8+campaign_footprint_cells[i].y)*view->scale+view->offset_y;
            if(xx>=right || yy>=bottom || xx+draw_size<=left || yy+draw_size<=top) continue;
            if(!tile(campaign_footprint_cells[i].images[0],xx,yy,draw_size)) goto fail;
        }
    }
    rdpq_set_scissor(0,0,320,240);rdpq_mode_pop();return true;
fail:
    rdpq_set_scissor(0,0,320,240);rdpq_mode_pop();return false;
}
