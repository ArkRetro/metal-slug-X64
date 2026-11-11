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
/* Test menu (main.c): mission (2..) and stage (1..) entered on the first
 * gameplay frame through the mission transition's path. */
void actor_select_mission(unsigned mission,unsigned stage);
/* SOLDIER SELECT's choice (+6E: 0 Marco, 1 Tarma, 2 Eri, 3 Fio;
 * msx_character.h), before actor_init or between games. */
bool actor_select_character(unsigned character);
void actor_draw(surface_t *surface, joypad_inputs_t input);
#endif
