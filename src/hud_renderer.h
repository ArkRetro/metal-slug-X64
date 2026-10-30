#ifndef MSX_HUD_RENDERER_H
#define MSX_HUD_RENDERER_H
#include <stdbool.h>
#include "neo_hud.h"
/* Loads rom:/fixed.bin and rom:/hud-palette.bin; false (fix layer off) when absent. */
bool hud_renderer_init(void);
/* Draws the fix map above everything (call after the sprites, before text). */
void hud_renderer_draw(const neo_hud_t *hud);
#endif
