#include <libdragon.h>
#include <assert.h>
#include <stdio.h>
#include "../src/msx_audio.h"
static uint32_t digest(int16_t *pcm,unsigned frames) {
    uint32_t h=2166136261u;
    for (unsigned i=0;i<frames*2;i++) h=(h^(uint16_t)pcm[i])*16777619u;
    return h;
}
int main(void) {
    debug_init_emulog();console_init();assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);
    msx_audio_init();rdpq_init();
    assert(!msx_audio_open(MSX_AUDIO_VOICES,"rom:/title-reference.wav64"));
    assert(!msx_audio_open(0,"rom:/missing.wav64"));
    assert(!msx_audio_play(0,false,0.5f));
    assert(msx_audio_open(0,"rom:/title-reference.wav64"));
    assert(msx_audio_open(1,"rom:/title-reference.wav64"));
    assert(msx_audio_play(0,false,0.5f));
    int16_t *pcm=malloc_uncached(1024*4);assert(pcm);
    unsigned nonzero=0;uint32_t checksum=0;
    for (unsigned i=0;i<180;i++) {
        if (i==8) assert(msx_audio_play(1,false,0.25f));
        if (i==24) msx_audio_stop(1);
        msx_audio_mix(pcm,1024);checksum^=digest(pcm,1024);
        for (unsigned s=0;s<2048;s++) nonzero+=pcm[s]!=0;
    }
    assert(nonzero>1024 && !msx_audio_playing(0) && !msx_audio_playing(1));
    assert(msx_audio_play(0,true,0.5f));
    for (unsigned i=0;i<180;i++) msx_audio_mix(pcm,1024);
    assert(msx_audio_playing(0));msx_audio_stop(0);
    msx_audio_mix(pcm,1024);msx_audio_mix(pcm,1024);
    for (unsigned s=0;s<2048;s++) assert(!pcm[s]);
    assert(msx_audio_play(0,false,0.5f));
    console_close();display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    unsigned queued=0,rendered=0;uint32_t deadline=TICKS_READ()+TICKS_FROM_MS(4000);
    while (queued<24 && (int32_t)(TICKS_READ()-deadline)<0) {
        unsigned n=msx_audio_service();assert(n<=2);queued+=n;
        surface_t *surface=display_get();rdpq_attach_clear(surface,NULL);
        rdpq_set_mode_fill(RGBA32(24,96,48,255));rdpq_fill_rectangle(16,16,304,224);
        rdpq_detach_wait();display_show(surface);rendered++;
    }
    assert(queued>=24 && rendered>0);msx_audio_stats_t stats=msx_audio_stats();assert(stats.buffers==362+queued);
    msx_audio_close();assert(!msx_audio_playing(0));msx_audio_close();free_uncached(pcm);
    debugf("MSX AUDIO REGRESSION PASS buffers=%u nonzero=%u checksum=%08lx queued=%u rendered=%u\n",(unsigned)stats.buffers,nonzero,(unsigned long)checksum,queued,rendered);
    for (;;) wait_ms(100);
}
