#ifndef MSX_ACTOR_H
#define MSX_ACTOR_H
#include <libdragon.h>
void actor_init(void);
void actor_set_active(bool active);
void actor_draw(surface_t *surface, joypad_inputs_t input);
#endif
