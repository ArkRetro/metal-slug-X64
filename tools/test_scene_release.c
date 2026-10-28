/* Internal comparison harness: compile rendering modules with and without
 * NDEBUG; keep this driver checked in both cases. No public demo. */
#include <libdragon.h>
#include <assert.h>
#include "../src/scene_renderer.h"
#include "../src/cinematic_codec.h"
int main(void) {
    debug_init_emulog();
    assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);
    rdpq_init();scene_assets_init();scene_renderer_init();
    display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    const unsigned cameras[]={0,64,320,872};
    uint32_t checksums[2][4];
    for (unsigned mode=0;mode<2;mode++) {
        scene_render_set_depth_mode(mode!=0);
        for (unsigned sample=0;sample<4;sample++) {
            retro_frame_t frame={.camera={cameras[sample],0,0,320},.tick=120,
                .light_count=1,.lights={{{cameras[sample]+160,160,-16},{255,160,64},96,1}}};
            scene_render_depth_frame(&frame);
            surface_t *surface=display_get();scene_render_begin(surface);
            rdpq_mode_dithering(DITHER_NONE_NONE);
            scene_render_background(cameras[sample],3);
            scene_render_foreground_fog();scene_render_end();rspq_wait();
            /* display_get returns an uncached framebuffer; the RDP fence above
             * completes writes before CPU readback. */
            checksums[mode][sample]=msx_cine_crc32(surface->buffer,surface->stride*surface->height);
            debugf("MSX RELEASE SAMPLE mode=%u camera=%u crc=%08lx\n",mode,cameras[sample],(unsigned long)checksums[mode][sample]);
        }
    }
    for (unsigned sample=0;sample<4;sample++) assert(checksums[0][sample]!=checksums[1][sample]);
    debugf("MSX RELEASE REGRESSION PASS samples=8\n");
    for (;;) wait_ms(100);
}
