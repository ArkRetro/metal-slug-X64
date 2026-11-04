/* Independent per-frame pixel CRCs, including compact movie-cache snapshots. */
#include <libdragon.h>
#include <assert.h>
#include "../src/cinematic.h"
#include "frontend_fixture.h"
int main(void) {
    debug_init_emulog();assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);
    assert(get_memory_size()>=8*1024*1024);rdpq_init();
    display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    static const struct {const char *path;const unsigned long *crc;unsigned count;} cases[]={
        {"rom:/frontend/tutorial.msxc",frontend_tutorial_crc,sizeof(frontend_tutorial_crc)/sizeof(frontend_tutorial_crc[0])},
        {"rom:/frontend/select.msxc",frontend_select_crc,sizeof(frontend_select_crc)/sizeof(frontend_select_crc[0])},
        {"rom:/frontend/select-confirm.msxc",frontend_select_confirm_crc,sizeof(frontend_select_confirm_crc)/sizeof(frontend_select_confirm_crc[0])}
    };
    unsigned total=0;
    for (unsigned s=0;s<sizeof(cases)/sizeof(cases[0]);s++) {
        assert(msx_cinematic_open(cases[s].path));
        assert(msx_cinematic_stats().frames==cases[s].count);
        for (unsigned i=0;i<cases[s].count;i++) {
            assert(msx_cinematic_stats().index==i);
            uint32_t crc=msx_cinematic_pixels_crc();
            assertf(crc==cases[s].crc[i],"Frontend CRC case=%u frame=%u actual=%08lx expected=%08lx",s,i,(unsigned long)crc,cases[s].crc[i]);
            if (!i || i==cases[s].count/2 || i+1==cases[s].count) msx_cinematic_draw(display_get());
            if (i+1<cases[s].count) assert(msx_cinematic_next());
            total++;
        }
        assert(!msx_cinematic_next());msx_cinematic_close();
        debugf("MSX FRONTEND CODEC case=%u frames=%u PASS\n",s,cases[s].count);
    }
    debugf("MSX FRONTEND CODEC PASS frames=%u\n",total);
    for (;;) wait_ms(100);
}
