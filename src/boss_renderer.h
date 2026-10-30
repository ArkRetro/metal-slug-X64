#ifndef MSX_BOSS_RENDERER_H
#define MSX_BOSS_RENDERER_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_animation.h"
/* Draw one sprite-list entry of the native Mission 1 boss (neo_boss
 * services.draw) with the existing scene renderer: descriptor parts of the
 * streamed SCENE_NATIVE bank (tools/export_boss_sprites.py), the logical
 * palette resource the boss allocated for the entry's slot, the original
 * mirror rule (one pixel left) and the original column cut (00A17A: a 16 px
 * column whose left edge is outside -8..311 is not shown). `y_offset` is the
 * camera height the caller applies to every gameplay sprite. With
 * request_only the parts are only queued for streaming. Returns false if
 * the descriptor or the palette is not in the pack (counted). */
bool boss_render_draw(const neo_anim_draw_t *draw,unsigned resource,int y_offset,bool flash,bool request_only);
typedef struct { unsigned missing_descriptor,missing_palette,parts,clipped; } boss_render_stats_t;
boss_render_stats_t boss_render_stats(void);
#endif
