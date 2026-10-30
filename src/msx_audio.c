#include "msx_audio.h"
#include <libdragon.h>
#include <stdio.h>
#include <string.h>
static wav64_t waves[MSX_AUDIO_VOICES];
static bool opened[MSX_AUDIO_VOICES],initialized;
static int file_loop[MSX_AUDIO_VOICES];   /* loop length stored in the WAV64 header */
static msx_audio_stats_t stats;
static bool submitted;   /* AI has received a buffer from msx_audio_service */
void msx_audio_init(void) {
    assert(!initialized);
    audio_init(MSX_AUDIO_RATE,2);mixer_init(MSX_AUDIO_VOICES*2);
    for (unsigned ch=0;ch<MSX_AUDIO_VOICES*2;ch++) mixer_ch_set_limits(ch,16,MSX_AUDIO_RATE,8192);
    memset(opened,0,sizeof(opened));stats=(msx_audio_stats_t){0};submitted=false;initialized=true;
}
void msx_audio_stop(unsigned voice) {
    if (initialized && voice<MSX_AUDIO_VOICES && opened[voice]) mixer_ch_stop(voice*2);
}
bool msx_audio_open(unsigned voice,const char *path) {
    if (!initialized || voice>=MSX_AUDIO_VOICES || !path || strncmp(path,"rom:/",5)) return false;
    FILE *file=fopen(path,"rb");if (!file) return false;fclose(file);
    msx_audio_stop(voice);
    if (opened[voice]) {wav64_close(&waves[voice]);opened[voice]=false;}
    wav64_open(&waves[voice],path);
    waveform_t *w=&waves[voice].wave;
    if ((w->channels!=1 && w->channels!=2) || w->bits!=16 || w->frequency<=0 || w->frequency>MSX_AUDIO_RATE) {
        wav64_close(&waves[voice]);return false;
    }
    file_loop[voice]=w->loop_len;opened[voice]=true;return true;
}
bool msx_audio_play(unsigned voice,bool loop,float volume) {
    if (!initialized || voice>=MSX_AUDIO_VOICES || !opened[voice] || !(volume>=0 && volume<=1)) return false;
    msx_audio_stop(voice);
    // Keep a loop point stored by audioconv64 (--wav-loop-offset); otherwise
    // loop the whole file or play it once.
    if (loop && file_loop[voice]) waves[voice].wave.loop_len=file_loop[voice];
    else wav64_set_loop(&waves[voice],loop);
    wav64_play(&waves[voice],voice*2);
    mixer_ch_set_vol(voice*2,volume,volume);return true;
}
bool msx_audio_playing(unsigned voice) {
    return initialized && voice<MSX_AUDIO_VOICES && opened[voice] && mixer_ch_playing(voice*2);
}
void msx_audio_mix(int16_t *stereo,unsigned frames) {
    assert(initialized && stereo && frames && !(frames&1u));
    stats.buffers++;stats.frames+=frames;
    while (frames) {
        unsigned count=frames>1024?1024:frames;
        mixer_poll(stereo,count);stereo+=count*2;frames-=count;
    }
}
unsigned msx_audio_service(void) {
    assert(initialized);unsigned count=0;
    // AI_STATUS bit 30: DMA busy. Idle after the first buffer = a gap.
    if (submitted && !(*(volatile uint32_t *)0xA450000C & (1u<<30))) stats.underruns++;
    while (count<2 && audio_can_write()) {
        int16_t *buffer=audio_write_begin();msx_audio_mix(buffer,audio_get_buffer_length());audio_write_end();count++;submitted=true;
    }
    return count;
}
msx_audio_stats_t msx_audio_stats(void) {return stats;}
void msx_audio_close(void) {
    if (!initialized) return;
    for (unsigned v=0;v<MSX_AUDIO_VOICES;v++) if (opened[v]) {msx_audio_stop(v);wav64_close(&waves[v]);opened[v]=false;}
    mixer_close();audio_close();initialized=false;submitted=false;
}
