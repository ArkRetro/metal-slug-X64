/* Actual RDP readback for tile-vs-prerender comparison, not a public demo. */
#include <libdragon.h>
#include <assert.h>
#include "../src/scene_renderer.h"
#include "../src/cinematic_codec.h"
#include "../src/mission_camera.h"
int main(void) {
    debug_init_emulog();assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);
    rdpq_init();scene_assets_init();scene_renderer_init();
    display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    const unsigned cameras[]={0,31,64,320,872,1022,1023,1920};
    unsigned samples=0;
    for (unsigned mode=0;mode<2;mode++) {
        scene_render_set_depth_mode(mode!=0);
        for (unsigned n=0;n<8;n++) for (unsigned phase=0;phase<8;phase++) {
            unsigned camera=cameras[n];
            retro_frame_t frame={.camera={camera,-msx_camera_height(camera),0,320},.tick=phase};
            scene_render_depth_frame(&frame);
            surface_t *surface=display_get();scene_render_begin(surface);
            rdpq_mode_dithering(DITHER_NONE_NONE);
            scene_render_background(camera,phase);scene_render_end();rspq_wait();
            uint32_t crc=msx_cine_crc32(surface->buffer,surface->stride*surface->height);
#ifdef MSX_BACKGROUND_DUMP_PIXELS
            if (mode==1 && camera==0 && phase==0) {
                const uint8_t *pixels=surface->buffer;static const char hex[]="0123456789abcdef";
                for (unsigned row=0;row<240;row++) {
                    char line[1281];
                    for (unsigned byte=0;byte<640;byte++) {
                        uint8_t value=pixels[row*surface->stride+byte];
                        line[byte*2]=hex[value>>4];line[byte*2+1]=hex[value&15];
                    }
                    line[1280]=0;debugf("MSX BACKGROUND PIXELS row=%u %s\n",row,line);
                }
            }
#endif
            scene_render_stats_t s=scene_render_stats();
            debugf("MSX BACKGROUND mode=%u camera=%u phase=%u crc=%08lx draws=%u uploads=%u tlut=%u bytes=%u\n",
                mode,camera,phase,(unsigned long)crc,s.draws,s.uploads,s.palette_uploads,s.tmem_bytes);
            samples++;
        }
    }
    // Exercise TLUT ownership in both directions within one attached frame:
    // shared CI8 background -> actor CI4 -> shared CI8 -> flashed/normal CI4.
    scene_render_set_depth_mode(false);
    surface_t *surface=display_get();scene_render_begin(surface);
    rdpq_mode_dithering(DITHER_NONE_NONE);
    const scene_image_t *actor=scene_frame(SCENE_BASE,0);
    scene_render_background(0,0);scene_render_image(actor,100,120,false,false);
    scene_render_background(0,0);scene_render_image(actor,180,120,true,true);
    scene_render_image(actor,220,160,false,false);scene_render_end();rspq_wait();
    debugf("MSX BACKGROUND SWITCH crc=%08lx\n",(unsigned long)msx_cine_crc32(surface->buffer,surface->stride*surface->height));
    debugf("MSX BACKGROUND PASS samples=%u cell=%u\n",samples,scene_map_cell_size());
    for (;;) wait_ms(100);
}
