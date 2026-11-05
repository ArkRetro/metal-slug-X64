/* Actual RDP crop/reference and erasure regression. Expected crops are private
 * cartridge-derived resources, generated separately from the native renderer. */
#include <libdragon.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "../src/scene_renderer.h"
#include "../src/campaign_footprint_renderer.h"
#include "../src/mission_camera.h"
#include "../src/cinematic_codec.h"
#include "footprint_expected.h"

int main(void) {
    debug_init_emulog();assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);
    rdpq_init();scene_assets_init();scene_renderer_init();
    assert(campaign_footprint_renderer_load());
    display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    uint8_t *before=malloc(320*240*2);assert(before);unsigned samples=0,pixels=0;
    for(unsigned mode=0;mode<2;mode++) for(unsigned n=0;n<sizeof(footprint_expected)/sizeof(*footprint_expected);n++) {
        unsigned camera=footprint_expected[n].camera;
        retro_frame_t frame={.camera={camera,-msx_camera_height(camera),0,320}};
        scene_render_set_depth_mode(mode!=0);scene_render_depth_frame(&frame);
        surface_t *surface=display_get();scene_render_begin(surface);
        rdpq_mode_dithering(DITHER_NONE_NONE);scene_render_background(camera,0);rspq_wait();
        memcpy(before,surface->buffer,320*240*2);
        assert(campaign_footprint_renderer_draw_stamp(footprint_expected[n].column,
            footprint_expected[n].row,camera,mode!=0));rspq_wait();
        const uint16_t *actual=surface->buffer;const uint16_t *old=(const uint16_t*)before;
        int left=footprint_expected[n].x,top=footprint_expected[n].y;
        for(int y=0;y<240;y++) for(int x=0;x<320;x++) {
            if(x<left || x>=left+16 || y<top || y>=top+16) assert(actual[y*320+x]==old[y*320+x]);
            else if(!mode) {
                uint16_t expected=footprint_expected[n].pixels[(y-top)*16+x-left];
                if(actual[y*320+x]!=expected) {
                    debugf("FOOTPRINT mismatch sample=%u xy=%d,%d got=%04x expected=%04x\n",n,x,y,actual[y*320+x],expected);
                    assert(false);
                }
                ++pixels;
            }
        }
        uint32_t crc=msx_cine_crc32(surface->buffer,surface->stride*surface->height);
        assert(campaign_footprint_renderer_draw_stamp(footprint_expected[n].column,
            footprint_expected[n].row,camera,mode!=0));rspq_wait();
        assert(crc==msx_cine_crc32(surface->buffer,surface->stride*surface->height));
        scene_render_forget_texture();scene_render_end();rspq_wait();++samples;
        debugf("MSX FOOTPRINT sample=%u mode=%u camera=%u crc=%08lx\n",n,mode,camera,(unsigned long)crc);
    }
    assert(campaign_footprint_renderer_max_upload()<=2048);
    debugf("MSX FOOTPRINT PASS samples=%u pixels=%u max_upload=%u\n",samples,pixels,campaign_footprint_renderer_max_upload());
    for(;;) wait_ms(100);
}
