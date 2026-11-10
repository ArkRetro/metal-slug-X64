#include "msx_audio.h"
#include <libdragon.h>
#include <stdio.h>
#include <string.h>
static wav64_t waves[MSX_AUDIO_VOICES];
static bool opened[MSX_AUDIO_VOICES],initialized;
static int file_loop[MSX_AUDIO_VOICES];   /* loop length stored in the WAV64 header */
/* Optional loop body played after the voice's file (an intro): started on the
 * same channel by a mixer event exactly when the intro ends. */
static wav64_t tails[MSX_AUDIO_VOICES];
static bool tail_open[MSX_AUDIO_VOICES];
static float tail_volume[MSX_AUDIO_VOICES];
static unsigned tail_voice[MSX_AUDIO_VOICES];
static int start_tail(void *ctx) {
    unsigned v=*(unsigned *)ctx;
    wav64_set_loop(&tails[v],true);wav64_play(&tails[v],v*2);mixer_ch_set_vol(v*2,tail_volume[v],tail_volume[v]);
    return 0;
}
static msx_audio_stats_t stats;
/* Pause (msx_pause.h): the mixer is not polled, so every voice, stream and
 * mixer event (the loop tails) stands where it was; silence goes out. */
static bool paused;
void msx_audio_pause(bool on) {paused=on;}
bool msx_audio_paused(void) {return paused;}
static bool submitted;   /* AI has received a buffer from msx_audio_service */
void msx_audio_init(void) {
    assert(!initialized);
    audio_init(MSX_AUDIO_RATE,2);mixer_init(MSX_AUDIO_VOICES*2);
    for (unsigned ch=0;ch<MSX_AUDIO_VOICES*2;ch++) mixer_ch_set_limits(ch,16,MSX_AUDIO_RATE,8192);
    memset(opened,0,sizeof(opened));memset(tail_open,0,sizeof(tail_open));stats=(msx_audio_stats_t){0};submitted=false;initialized=true;
}
void msx_audio_stop(unsigned voice) {
    if (!initialized || voice>=MSX_AUDIO_VOICES) return;
    if (tail_open[voice]) mixer_remove_event(start_tail,&tail_voice[voice]);
    if (opened[voice]) mixer_ch_stop(voice*2);
}
bool msx_audio_open_loop(unsigned voice,const char *path) {
    if (!initialized || voice>=MSX_AUDIO_VOICES || !opened[voice] || !path || strncmp(path,"rom:/",5)) return false;
    FILE *file=fopen(path,"rb");if (!file) return false;fclose(file);
    msx_audio_stop(voice);
    if (tail_open[voice]) {wav64_close(&tails[voice]);tail_open[voice]=false;}
    wav64_open(&tails[voice],path);
    const waveform_t *w=&tails[voice].wave,*h=&waves[voice].wave;
    if (w->channels!=h->channels || w->bits!=16 || w->frequency!=h->frequency) {wav64_close(&tails[voice]);return false;}
    tail_voice[voice]=voice;tail_open[voice]=true;return true;
}
bool msx_audio_open(unsigned voice,const char *path) {
    if (!initialized || voice>=MSX_AUDIO_VOICES || !path || strncmp(path,"rom:/",5)) return false;
    FILE *file=fopen(path,"rb");if (!file) return false;fclose(file);
    msx_audio_stop(voice);
    if (opened[voice]) {wav64_close(&waves[voice]);opened[voice]=false;}
    /* A new file drops the previous loop body (open_loop comes after this):
     * the select music's (B44) must not follow the next movie's audio. */
    if (tail_open[voice]) {wav64_close(&tails[voice]);tail_open[voice]=false;}
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
    // loop the whole file or play it once. The pinned libdragon's VADPCM
    // reader only wraps whole files (wav64_vadpcm.c asserts loop_len == len
    // when the end is reached): a partial loop of a compressed file loops the
    // whole file instead (docs/AUDIO_JUEGO.md).
    if (loop && tail_open[voice]) {
        /* Intro once, then the loop body from the intro's last sample: the
         * event delay is the intro length in mixer output samples. */
        wav64_set_loop(&waves[voice],false);wav64_play(&waves[voice],voice*2);
        mixer_ch_set_vol(voice*2,volume,volume);tail_volume[voice]=volume;
        int64_t delay=(int64_t)waves[voice].wave.len*MSX_AUDIO_RATE/(int64_t)waves[voice].wave.frequency;
        mixer_add_event(delay,start_tail,&tail_voice[voice]);
        return true;
    }
    bool vadpcm=waves[voice].format==1;   /* WAV64_FORMAT_VADPCM (internal header) */
    if (loop && file_loop[voice] && !(vadpcm && file_loop[voice]!=waves[voice].wave.len)) waves[voice].wave.loop_len=file_loop[voice];
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
    if (paused) {memset(stereo,0,(size_t)frames*2*sizeof(*stereo));return;}
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
void msx_audio_idle(void) {if (initialized) msx_audio_service();}
void msx_audio_close(void) {
    if (!initialized) return;
    for (unsigned v=0;v<MSX_AUDIO_VOICES;v++) {
        if (opened[v]) {msx_audio_stop(v);wav64_close(&waves[v]);opened[v]=false;}
        if (tail_open[v]) {wav64_close(&tails[v]);tail_open[v]=false;}
    }
    mixer_close();audio_close();initialized=false;submitted=false;
}
