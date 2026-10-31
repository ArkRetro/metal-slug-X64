#ifndef MSX_MARCO_RENDERER_H
#define MSX_MARCO_RENDERER_H
#include <stdint.h>
#include <stdbool.h>
#include "neo_animation.h"
/* Anchored original descriptor parts. This resource layer does not choose a
 * player action or claim that the observed descriptor catalog is complete.
 * Drawing changes TMEM/TLUT: invalidate any caller texture-residency cache. */
bool marco_renderer_load(void);
void marco_renderer_close(void); /* Caller must first wait for pending RDP work. */
unsigned marco_renderer_count(void);
unsigned marco_renderer_palette_count(void);
unsigned marco_renderer_max_upload(void);
uint32_t marco_renderer_source_tag(unsigned pose);
bool marco_renderer_find(uint32_t source_tag,unsigned *pose);
unsigned marco_renderer_burnt_palette(void);
bool marco_renderer_draw(unsigned pose,int anchor_x,int anchor_y,bool mirror);
bool marco_renderer_draw_palette(unsigned pose,int anchor_x,int anchor_y,bool mirror,unsigned palette);
/* Draw a native helper emission in Neo Geo 9-bit coordinates. viewport_x/y
 * translate the original raster; caller owns the scissor/active viewport.
 * palette is an explicit native TLUT profile, not an original bank address. */
bool marco_renderer_draw_native(const neo_anim_draw_t *draw,int viewport_x,int viewport_y,unsigned palette);
#endif
