#include <libdragon.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../src/cinematic_codec.h"
#include "../src/cinematic.h"
#include "entry_meta.h"
int main(void) {
    debug_init_emulog();console_init();assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);
    rdpq_init();console_close();display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    uint8_t *pixels=malloc(304*224*2);assert(pixels);unsigned rendered=0;
    assert(msx_cinematic_open("rom:/entry.msxc"));assert(msx_cinematic_stats().frames==MSX_ENTRY_FRAMES);
    for (unsigned i=0;i<MSX_ENTRY_FRAMES;i++) {
        if (i) assert(msx_cinematic_next());
        assert(msx_cinematic_pixels_crc()==msx_entry_crc[i]);
        if (i==0 || i==29 || i==MSX_ENTRY_FRAMES-1) {
            surface_t *surface=display_get();msx_cinematic_draw(surface);
            for (unsigned y=0;y<224;y++) memcpy(pixels+y*304*2,(uint8_t *)surface->buffer+(y+8)*surface->stride+8*2,304*2);
            uint32_t crc=msx_cine_crc32(pixels,304*224*2);
            debugf("MSX CINEMATIC framebuffer index=%u crc=%08lx expected=%08lx\n",i,(unsigned long)crc,msx_entry_crc[i]);
            assert(crc==msx_entry_crc[i]);rendered++;
        }
    }
    assert(!msx_cinematic_next());msx_cinematic_close();
    assert(!msx_cinematic_open("rom:/missing.msxc"));
    free(pixels);debugf("MSX CINEMATIC REGRESSION PASS frames=%u framebuffers=%u\n",MSX_ENTRY_FRAMES,rendered);
    for (;;) wait_ms(100);
}
