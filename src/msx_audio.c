#include "msx_audio.h"
#include <libdragon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msx_pcm.h"
static wav64_t waves[MSX_AUDIO_VOICES];
static bool opened[MSX_AUDIO_VOICES],initialized;
static int file_loop[MSX_AUDIO_VOICES];   /* loop length stored in the WAV64 header */
/* Optional loop body played after the voice's file (an intro): started on the
 * same channel by a mixer event exactly when the intro ends. */
static wav64_t tails[MSX_AUDIO_VOICES];
static bool tail_open[MSX_AUDIO_VOICES];
static float tail_volume[MSX_AUDIO_VOICES];
static unsigned tail_voice[MSX_AUDIO_VOICES];
/* A voice's file may be an MSXL file: a raw 16-bit WAV64 of the cartridge
 * stored without loss (src/msx_pcm.h, docs/COMPRESION.md). wave_pcm /
 * tail_pcm: its reader, in the heap while the file is open (else the WAV64). */
static msx_pcm_t *wave_pcm[MSX_AUDIO_VOICES],*tail_pcm[MSX_AUDIO_VOICES];
/* After the rest of the program's code, like src/msx_pcm.c (see there). */
#define LAST __attribute__((section("keep.text.msx_audio")))
#define SMALL LAST __attribute__((noinline))
static waveform_t *wave_of(unsigned v) {return wave_pcm[v]?&wave_pcm[v]->wave:&waves[v].wave;}
static waveform_t *tail_of(unsigned v) {return tail_pcm[v]?&tail_pcm[v]->wave:&tails[v].wave;}
static SMALL void close_file(msx_pcm_t **pcm,wav64_t *w) {
    if (*pcm) {msx_pcm_close(*pcm);free(*pcm);*pcm=NULL;} else wav64_close(w);
}
static void close_wave(unsigned v) {close_file(&wave_pcm[v],&waves[v]);opened[v]=false;}
static void close_tail(unsigned v) {close_file(&tail_pcm[v],&tails[v]);tail_open[v]=false;}
/* Opens the rom file `path` (it exists): an MSXL file with a reader in
 * *pcm, any other as a WAV64. false: an MSXL file that cannot be read. */
static SMALL bool open_file(const char *path,msx_pcm_t **pcm,wav64_t *wav) {
    FILE *file=fopen(path,"rb");char id[4]={0};
    bool msxl=file && fread(id,1,4,file)==4 && !memcmp(id,"MSXL",4);
    if (file) fclose(file);
    if (!msxl) {wav64_open(wav,path);return true;}
    *pcm=malloc(sizeof(**pcm));
    if (*pcm && msx_pcm_open(*pcm,path)) return true;
    free(*pcm);*pcm=NULL;return false;
}
static SMALL void play_wave(waveform_t *w,int loop_len,int ch) {w->loop_len=loop_len;mixer_ch_play(ch,w);}
static int start_tail(void *ctx) {
    unsigned v=*(unsigned *)ctx;
    if (tail_pcm[v]) play_wave(&tail_pcm[v]->wave,tail_pcm[v]->wave.len,(int)v*2);
    else {wav64_set_loop(&tails[v],true);wav64_play(&tails[v],v*2);}
    mixer_ch_set_vol(v*2,tail_volume[v],tail_volume[v]);
    return 0;
}
static msx_audio_stats_t stats;
static bool submitted;   /* AI has received a buffer from msx_audio_service */
void msx_audio_init(void) {
    assert(!initialized);
    audio_init(MSX_AUDIO_RATE,2);mixer_init(MSX_AUDIO_VOICES*2);
    for (unsigned ch=0;ch<MSX_AUDIO_VOICES*2;ch++) mixer_ch_set_limits(ch,16,MSX_AUDIO_RATE,8192);
    memset(opened,0,sizeof(opened));memset(tail_open,0,sizeof(tail_open));
    memset(wave_pcm,0,sizeof(wave_pcm));memset(tail_pcm,0,sizeof(tail_pcm));stats=(msx_audio_stats_t){0};submitted=false;initialized=true;
}
void msx_audio_stop(unsigned voice) {
    if (!initialized || voice>=MSX_AUDIO_VOICES) return;
    if (tail_open[voice]) mixer_remove_event(start_tail,&tail_voice[voice]);
    if (opened[voice]) mixer_ch_stop(voice*2);
    /* A stopped MSXL stream gives its block buffers back to the heap. */
    if (wave_pcm[voice]) msx_pcm_release(wave_pcm[voice]);
    if (tail_pcm[voice]) msx_pcm_release(tail_pcm[voice]);
}
bool msx_audio_open_loop(unsigned voice,const char *path) {
    if (!initialized || voice>=MSX_AUDIO_VOICES || !opened[voice] || !path || strncmp(path,"rom:/",5)) return false;
    FILE *file=fopen(path,"rb");if (!file) return false;fclose(file);
    msx_audio_stop(voice);
    if (tail_open[voice]) close_tail(voice);
    if (!open_file(path,&tail_pcm[voice],&tails[voice])) return false;
    tail_open[voice]=true;
    const waveform_t *w=tail_of(voice),*h=wave_of(voice);
    if (w->channels!=h->channels || w->bits!=16 || w->frequency!=h->frequency) {close_tail(voice);return false;}
    tail_voice[voice]=voice;return true;
}
bool msx_audio_open(unsigned voice,const char *path) {
    if (!initialized || voice>=MSX_AUDIO_VOICES || !path || strncmp(path,"rom:/",5)) return false;
    FILE *file=fopen(path,"rb");if (!file) return false;fclose(file);
    msx_audio_stop(voice);
    if (opened[voice]) close_wave(voice);
    /* A new file drops the previous loop body (open_loop comes after this):
     * the select music's (B44) must not follow the next movie's audio. */
    if (tail_open[voice]) close_tail(voice);
    if (!open_file(path,&wave_pcm[voice],&waves[voice])) return false;
    opened[voice]=true;
    waveform_t *w=wave_of(voice);
    if ((w->channels!=1 && w->channels!=2) || w->bits!=16 || w->frequency<=0 || w->frequency>MSX_AUDIO_RATE) {
        close_wave(voice);return false;
    }
    file_loop[voice]=w->loop_len;return true;
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
        if (wave_pcm[voice]) play_wave(&wave_pcm[voice]->wave,0,(int)voice*2);
        else {wav64_set_loop(&waves[voice],false);wav64_play(&waves[voice],voice*2);}
        mixer_ch_set_vol(voice*2,volume,volume);tail_volume[voice]=volume;
        const waveform_t *head=wave_of(voice);
        int64_t delay=(int64_t)head->len*MSX_AUDIO_RATE/(int64_t)head->frequency;
        mixer_add_event(delay,start_tail,&tail_voice[voice]);
        return true;
    }
    if (wave_pcm[voice]) {
        /* As a raw WAV64: the stored loop point, else the whole file or once. */
        waveform_t *w=&wave_pcm[voice]->wave;
        play_wave(w,loop?(file_loop[voice]?file_loop[voice]:w->len):0,(int)voice*2);
        mixer_ch_set_vol(voice*2,volume,volume);return true;
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
LAST void msx_audio_prefetch(uint32_t deadline) {
    if (!initialized || !deadline) return;
    for (unsigned v=0;v<MSX_AUDIO_EFFECT_FIRST;v++) {   /* the stream voices */
        if (!mixer_ch_playing((int)v*2)) continue;
        if (wave_pcm[v]) msx_pcm_prefetch(wave_pcm[v],deadline);
        if (tail_pcm[v]) msx_pcm_prefetch(tail_pcm[v],deadline);
    }
}
void msx_audio_idle(void) {if (initialized) msx_audio_service();}
void msx_audio_close(void) {
    if (!initialized) return;
    for (unsigned v=0;v<MSX_AUDIO_VOICES;v++) {
        if (opened[v]) {msx_audio_stop(v);close_wave(v);}
        if (tail_open[v]) close_tail(v);
    }
    mixer_close();audio_close();initialized=false;submitted=false;
}
