#ifndef MSX_ACTOR_H
#define MSX_ACTOR_H
#include <libdragon.h>
void actor_init(void);
void actor_set_active(bool active);
bool actor_begin_mission(int32_t world_x,int32_t world_y);
void actor_draw(surface_t *surface, joypad_inputs_t input);
#endif
