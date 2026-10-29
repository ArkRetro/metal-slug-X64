#include <libdragon.h>
#include <assert.h>
#include "../src/marco_renderer.h"
#include "../src/cinematic_codec.h"
#include "marco_pose_meta.h"
int main(void) {
    debug_init_emulog();assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);
    rdpq_init();display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    assert(marco_renderer_load() && marco_renderer_count()==MARCO_POSES);
    unsigned frames=0;
    for (unsigned pose=0;pose<MARCO_POSES;pose++) for (unsigned palette=0;palette<MARCO_PALETTES;palette++) for (unsigned mirror=0;mirror<2;mirror++) {
        unsigned found=~0u;assert(marco_renderer_find(marco_poses[pose].source_tag,&found) && found==pose);
        surface_t *surface=display_get();rdpq_attach_clear(surface,NULL);
        rdpq_set_mode_fill(RGBA32(0,0,0,255));rdpq_fill_rectangle(0,0,320,240);
        assert(marco_renderer_draw_palette(pose,marco_poses[pose].anchor_x,marco_poses[pose].anchor_y,mirror!=0,palette));rdpq_detach_wait();
        uint32_t crc=msx_cine_crc32(surface->buffer,surface->stride*surface->height);
        if (crc!=marco_poses[pose].crc[palette][mirror]) debugf("MSX MARCO PIXELS mismatch pose=%u mirror=%u palette=%u got=%08lx expected=%08lx\n",pose,mirror,palette,(unsigned long)crc,(unsigned long)marco_poses[pose].crc[palette][mirror]);
        assert(crc==marco_poses[pose].crc[palette][mirror]);display_show(surface);frames++;
    }
    assert(!marco_renderer_draw(MARCO_POSES,0,0,false));
    assert(!marco_renderer_draw_palette(0,0,0,false,MARCO_PALETTES));
    assert(!marco_renderer_find(0,NULL));unsigned tmem=marco_renderer_max_upload();assert(tmem && tmem<=2048);marco_renderer_close();marco_renderer_close();
    debugf("MSX MARCO PIXELS PASS poses=%u frames=%u chunks=%u palettes=%u tmem_max=%u\n",MARCO_POSES,frames,MARCO_PARTS,MARCO_PALETTES,tmem);
    for (;;) wait_ms(100);
}
