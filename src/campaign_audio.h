#ifndef MSX_CAMPAIGN_AUDIO_H
#define MSX_CAMPAIGN_AUDIO_H
#include <stdbool.h>
#include <stdint.h>
/* Original isolated sound-board commands; native mixer voice allocation. */
bool campaign_audio_init(void);
bool campaign_audio_play(uint16_t token);
#endif
