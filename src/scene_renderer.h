#ifndef MSX_SCENE_RENDERER_H
#define MSX_SCENE_RENDERER_H
#include <libdragon.h>
#include "scene_assets.h"
#include "retro25d.h"
void scene_renderer_init(void);
void scene_render_begin(surface_t *surface);
/* Map layers for the displayed camera X and height (mission_camera.h
 * rules); backdrop_after selects the palette loaded at progress 2304. */
void scene_render_background(unsigned camera,int height,bool backdrop_after,unsigned video_tick);
void scene_render_image(const scene_image_t *image,int x,int y,bool mirror,bool flash);
void scene_render_box(int x,int y,int width,int height,color_t color);
void scene_render_text(int x,int y,const char *text);
void scene_render_end(void);
unsigned scene_render_draw_count(void);
void scene_render_set_depth_mode(bool enabled);
void scene_render_depth_frame(const retro_frame_t *frame);
void scene_render_foreground_fog(void);
void scene_render_forget_texture(void);
#endif
