#ifndef MSX_SCENE_RENDERER_H
#define MSX_SCENE_RENDERER_H
#include <libdragon.h>
#include "scene_assets.h"
void scene_renderer_init(void);
void scene_render_begin(surface_t *surface);
void scene_render_background(unsigned camera,unsigned video_tick);
void scene_render_image(const scene_image_t *image,int x,int y,bool mirror,bool flash);
void scene_render_box(int x,int y,int width,int height,color_t color);
void scene_render_text(int x,int y,const char *text);
void scene_render_end(void);
#endif
