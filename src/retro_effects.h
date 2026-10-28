#ifndef MSX_RETRO_EFFECTS_H
#define MSX_RETRO_EFFECTS_H
#include <libdragon.h>
#include "retro25d.h"
void retro_effects_init(void);
float retro_effects_layer_depth(unsigned layer);
void retro_effects_mesh(const retro_frame_t *frame);
void retro_effects_fog(const retro_frame_t *frame,unsigned slice);
unsigned retro_effects_triangles(void);
unsigned retro_effects_visible_planes(void);
unsigned retro_effects_shadow_triangles(void);
#endif
