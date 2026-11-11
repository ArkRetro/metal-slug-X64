#ifndef MSX_PCM_H
#define MSX_PCM_H
#include <libdragon.h>
#include <stdbool.h>
#include "msx_pcm_codec.h"
/* MSX_PCM_STATS=1 (make MSX_PCM_STATS=1, measurements of docs/COMPRESION.md;
 * the host test): counts and times every block and logs "MSX PCM" when a
 * stream is released. */
#ifndef MSX_PCM_STATS
#define MSX_PCM_STATS 0
#endif
/* A block is decoded ahead only with this much spare time left: twice the
 * slowest block measured in ares (docs/COMPRESION.md). */
enum { MSX_PCM_PREFETCH_US=4000 };
/* A waveform of the mixer read from an MSXL file (lossless 16-bit PCM,
 * src/msx_pcm_codec.h): the samples the mixer receives are those of the raw
 * WAV64 the file was made from, at the same positions. The file is read a
 * block at a time (one read of at most 4 KiB + 10 bytes per 1024 samples).
 * Three decoded blocks are kept: the one the mixer is reading and the two
 * after it (a mix of 1280 samples can touch three), decoded ahead in the
 * spare frame time (msx_pcm_prefetch) so that the mix only copies. The
 * buffers live in the heap only between the first read after a play and
 * msx_pcm_release. */
enum { MSX_PCM_SLOTS=3 };
typedef struct {
    waveform_t wave;
    char name[48];
    msxl_info_t info;
    int fd;
    int16_t *pcm[MSX_PCM_SLOTS];uint8_t *raw;   /* the decoded blocks and the stored one */
    int32_t have[MSX_PCM_SLOTS];     /* the block in each, -1 none */
    uint8_t current;                 /* the one the mixer read last */
    uint32_t next,next_offset,next_size;   /* the block the file position is at */
    uint32_t errors;
#if MSX_PCM_STATS
    uint32_t load_ticks,load_max,loads,ahead,decode_ticks;   /* measurements (MSX PCM line) */
#endif
} msx_pcm_t;
/* false (and nothing left open) unless `path` is a well-formed MSXL file. */
bool msx_pcm_open(msx_pcm_t *pcm,const char *path);
/* Spare frame time until `deadline` (TICKS_READ value): reads and decodes
 * the first block after the one being played that is not ready, when the
 * time left is MSX_PCM_PREFETCH_US or more. true when it did. */
bool msx_pcm_prefetch(msx_pcm_t *pcm,uint32_t deadline);
/* Free the block buffers; the channel playing the waveform must be stopped. */
void msx_pcm_release(msx_pcm_t *pcm);
void msx_pcm_close(msx_pcm_t *pcm);
#endif
