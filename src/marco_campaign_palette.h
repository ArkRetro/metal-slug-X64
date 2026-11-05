#ifndef MSX_MARCO_CAMPAIGN_PALETTE_H
#define MSX_MARCO_CAMPAIGN_PALETTE_H
#include "marco_common.h"

/* Palette tables selected at C35FC/C3614/C361C. The recovered character
 * domain is 0..3. Original indexed reads outside these four-word tables do
 * not establish additional native character support and are rejected. */
bool marco_campaign_palette_lookup(void *context,marco_common_palette_t,
    uint16_t character,uint16_t *descriptor);
#endif
