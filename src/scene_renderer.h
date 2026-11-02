#ifndef MSX_SCENE_RENDERER_H
#define MSX_SCENE_RENDERER_H
#include <libdragon.h>
#include "scene_assets.h"
#include "retro25d.h"
void scene_renderer_init(void);
/* After scene_stage_load / scene_mission_unload (docs/MISION2.md). */
void scene_renderer_stage_changed(void);
/* display_get() with a watchdog: re-kicks rspq when no framebuffer frees up. */
surface_t *msx_display_get(void);
void scene_render_begin(surface_t *surface);
/* Map layers for the displayed camera X and height (mission_camera.h
 * rules); backdrop_after selects the palette loaded at progress 2304. */
void scene_render_set_collapse(bool collapsed,bool flash);
void scene_render_set_patch(bool patched);
/* Mission 2 stage 3 tower (docs/MISION2.md "Fase 2d"): pieces 0..count-1
 * of the writer 0B7C38, types[k] drawn from the stage's templates (layer-3
 * cells at x >= SCENE_TOWER_TEMPLATE_X, tools/pack_mission.py) at
 * (256 + 16 i, SCENE_TOWER_BASE_Y - 48 k + 16 j). */
enum { SCENE_TOWER_TEMPLATE_X=0x4000, SCENE_TOWER_BASE_Y=-1408, SCENE_TOWER_TYPES=13 };
void scene_render_set_tower(const uint8_t *types,unsigned count);
/* Mission packs: layer `layer` drawn at `offset` (its own motion, e.g. the
 * Mission 3 train, neo_train_scroll) instead of its camera rule. */
void scene_render_set_layer_offset(unsigned layer,bool on,int offset);
void scene_render_image_scaled(const scene_image_t *image,float x,float y,float width,float height,bool mirror,bool flash);
void scene_render_back(unsigned camera,int height,bool backdrop_after,unsigned video_tick);
void scene_render_front(unsigned camera,int height,bool backdrop_after,unsigned video_tick);
void scene_render_background(unsigned camera,int height,bool backdrop_after,unsigned video_tick);
void scene_render_layers(unsigned camera,int height,bool backdrop_after,unsigned video_tick,unsigned mask,bool first);
void scene_render_image(const scene_image_t *image,int x,int y,bool mirror,bool flash);
/* A sprite part with the Neo Geo flips: mirror = attribute bit 0 (H),
 * flip_y = bit 1 (V); the caller places the part (flipped offsets). */
void scene_render_sprite(const scene_image_t *image,int x,int y,bool mirror,bool flip_y,bool flash);
void scene_render_sprite_scaled(const scene_image_t *image,float x,float y,float width,float height,bool mirror,bool flip_y,bool flash);
/* Sprite parts drawn while on use scene_page_try (per-frame decode budget). */
void scene_render_set_deferrable(bool on);
/* Palette fade offsets (-31..31 per channel, 0 = off) for the TLUTs uploaded
 * from now on; scene_render_fade_palette returns the converted copy. */
void scene_render_set_fade(int8_t red,int8_t green,int8_t blue);
const uint16_t *scene_render_fade_palette(const uint16_t *source);
void scene_render_box(int x,int y,int width,int height,color_t color);
void scene_render_text(int x,int y,const char *text);
void scene_render_end(void);
unsigned scene_render_draw_count(void);
void scene_render_set_depth_mode(bool enabled);
void scene_render_depth_frame(const retro_frame_t *frame);
void scene_render_foreground_fog(void);
void scene_render_forget_texture(void);
#endif
