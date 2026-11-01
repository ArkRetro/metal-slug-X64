#ifndef MSX_ACTOR_H
#define MSX_ACTOR_H
#include <libdragon.h>
void actor_init(void);
void actor_set_active(bool active);
bool actor_begin_mission(int32_t world_x,int32_t world_y);
void actor_begin_descent(int32_t world_x,int32_t world_y,int32_t vx,int32_t vy);
/* The descent actor_init's reset starts with (the boot's mission handoff):
 * actor_begin_descent with the same values then does not run it twice. */
void actor_prepare_descent(int32_t world_x,int32_t world_y,int32_t vx,int32_t vy);
void actor_draw(surface_t *surface, joypad_inputs_t input);
#endif
