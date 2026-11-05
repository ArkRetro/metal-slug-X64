#ifndef MSX_MARCO_CAMPAIGN_RENDERER_H
#define MSX_MARCO_CAMPAIGN_RENDERER_H
#include <stdbool.h>
#include "neo_animation.h"

/* Cartridge-derived normal Marco resources. The live controller owns action
 * selection and emits original descriptors; missing descriptors/palette banks
 * fail explicitly. Drawing changes TMEM/TLUT, so invalidate scene residency. */
bool marco_campaign_renderer_load(void);
void marco_campaign_renderer_close(void); /* Wait for pending RDP work first. */
bool marco_campaign_renderer_draw_native(const neo_anim_draw_t *,int viewport_x,int viewport_y);
unsigned marco_campaign_renderer_count(void);
unsigned marco_campaign_renderer_max_upload(void);
#endif
