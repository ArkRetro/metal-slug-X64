#ifndef MSX_AUDIO_H
#define MSX_AUDIO_H
#include <stdbool.h>
#include <stdint.h>
/* Native libdragon backend, not a Z80/YM2610 emulator. Each voice reserves a
 * stereo pair: intro, music, announcer, then five effect voices. */
enum { MSX_AUDIO_INTRO,MSX_AUDIO_MUSIC,MSX_AUDIO_ANNOUNCER,MSX_AUDIO_EFFECT_FIRST,
    MSX_AUDIO_VOICES=8,MSX_AUDIO_RATE=32000 };
/* underruns: service calls that found the AI idle (both hardware buffers
 * drained, i.e. an audible gap) after the first submission. */
typedef struct { uint32_t buffers,underruns;uint64_t frames; } msx_audio_stats_t;
void msx_audio_init(void);
bool msx_audio_open(unsigned voice,const char *path);
/* loop=true keeps the loop point stored in the WAV64 (audioconv64
 * --wav-loop-offset) when the file has one, else loops the whole file. */
bool msx_audio_play(unsigned voice,bool loop,float volume);
void msx_audio_stop(unsigned voice);
bool msx_audio_playing(unsigned voice);
/* Produce stereo PCM with RSP mixing into caller storage. The caller must
 * service the AI. Mixing is split into bounded RSP batches; this entry also permits independent native PCM validation. */
void msx_audio_mix(int16_t *stereo,unsigned frames);
/* Nonblocking AI service, at most two buffers per call. Call outside RDP work. */
unsigned msx_audio_service(void);
msx_audio_stats_t msx_audio_stats(void);
void msx_audio_close(void);
#endif
