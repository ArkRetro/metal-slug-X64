#ifndef MSX_MARCO_RENDERER_H
#define MSX_MARCO_RENDERER_H
#include <stdint.h>
#include <stdbool.h>
#include "neo_animation.h"
/* Anchored original descriptor parts. This resource layer does not choose a
 * player action or claim that the observed descriptor catalog is complete.
 * Drawing changes TMEM/TLUT: invalidate any caller texture-residency cache. */
bool marco_renderer_load(void);
/* Background load of the parts (the boot cinematics' spare time, ~0.3 s of
 * ROM reads at the mission handoff before): loads parts while each fits
 * before `deadline` (TICKS_READ); false when all are in, one failed or the
 * heap would keep less than `keep` bytes. marco_renderer_load finishes. */
bool marco_renderer_preload(unsigned long deadline,int keep);
/* Heap bytes the parts loaded so far take (the scene cache's game reserve
 * counts them as already spent). */
unsigned marco_renderer_preloaded_bytes(void);
void marco_renderer_close(void); /* Caller must first wait for pending RDP work. */
unsigned marco_renderer_count(void);
unsigned marco_renderer_palette_count(void);
unsigned marco_renderer_max_upload(void);
uint32_t marco_renderer_source_tag(unsigned pose);
bool marco_renderer_find(uint32_t source_tag,unsigned *pose);
unsigned marco_renderer_burnt_palette(void);
/* The mummy's palette profiles (build_marco_poses.py --name-palette MUMMY /
 * MUMMY2): slot 63, or slot 64 with `second`; 0 when not in the catalog. */
unsigned marco_renderer_mummy_palette(bool second);
bool marco_renderer_draw(unsigned pose,int anchor_x,int anchor_y,bool mirror);
bool marco_renderer_draw_palette(unsigned pose,int anchor_x,int anchor_y,bool mirror,unsigned palette);
/* Draw a native helper emission in Neo Geo 9-bit coordinates. viewport_x/y
 * translate the original raster; caller owns the scissor/active viewport.
 * palette is an explicit native TLUT profile, not an original bank address. */
bool marco_renderer_draw_native(const neo_anim_draw_t *draw,int viewport_x,int viewport_y,unsigned palette);
#endif
