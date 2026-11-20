#ifndef MSX_HUD_RENDERER_H
#define MSX_HUD_RENDERER_H
#include <stdbool.h>
#include "neo_hud.h"
/* Loads rom:/fixed.bin and rom:/hud-palette.bin; false (fix layer off) when absent. */
bool hud_renderer_init(void);
/* Draws the fix map above everything (call after the sprites, before text). */
void hud_renderer_draw(const neo_hud_t *hud);
/* Fix palette `slot` (0..15) shows `colours` (16 RGBA5551 words, pen 0 clear) until it is given back with NULL; two slots at most. */
void hud_renderer_slot(unsigned slot,const uint16_t *colours);
#endif
