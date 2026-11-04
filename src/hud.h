#ifndef MSX_HUD_H
#define MSX_HUD_H
#include <stdint.h>

/* Current campaign counters, NOT the arcade score/lives/timer state.
 * The original FIX font is reused; labels/layout are an N64 adaptation. */
typedef struct { uint32_t health,kills; } msx_hud_state_t;
/* Draw last, after world sprites. Replaces RDP TMEM/TLUT contents. */
void msx_hud_draw(const msx_hud_state_t *state);
/* 8-pixel FIX glyphs for native labels; caller provides an attached RDP. */
void msx_hud_text(int x,int y,const char *value);
#endif
