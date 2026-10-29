#ifndef MSX_GAMEPLAY_H
#define MSX_GAMEPLAY_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_vehicle.h"
// Observed region: camera 0..1920. The right limit keeps the arcade fraction
// (camera+292-1/65536), measured at 1164 for camera 872 and 2212 for 1920.
enum { MSX_BULLETS=8, MSX_ENEMY_BULLETS=4, MSX_UNIT=65536, MSX_WORLD_WIDTH=2240 };
// Historical observed prototype start; cinematic handoff supplies its own measured position.
enum { MSX_START_X=0x78bad9, MSX_END_X=2212*MSX_UNIT-1 };
typedef struct { int x,y,direction; bool active; } msx_bullet_t;
enum { MSX_IDLE, MSX_RUN, MSX_FIRE, MSX_JUMP, MSX_RUN_JUMP, MSX_RUN_FIRE };
typedef struct { int x,facing; unsigned health,flash,cooldown,respawn; bool alive; } msx_enemy_t;
typedef struct {
    int x,y,velocity_x,velocity_y,facing,camera,checkpoint;
    unsigned ticks,air_ticks,shot_age,cooldown,hits,health,invulnerable,kills,deaths;
    bool grounded,jump_was_down,jump_pending,moving,shooting,complete;
    msx_bullet_t bullets[MSX_BULLETS], enemy_bullets[MSX_ENEMY_BULLETS];
    msx_enemy_t enemy;
    neo_vehicle_t vehicle;
    unsigned vehicle_events,wreckage_cells,vehicle_flash;
} msx_game_t;
void msx_game_init(msx_game_t *game);
/* Accept a measured grounded world position only when native support agrees. */
bool msx_game_begin_at(msx_game_t *game,int32_t world_x,int32_t world_y);
void msx_game_update(msx_game_t *game,int direction,bool jump,bool fire);
int msx_game_animation(const msx_game_t *game);
int msx_ground_offset(int x16);
/* Internal reference encounter; campaign spawn/wave logic is not recovered yet. */
void msx_game_vehicle_reference(msx_game_t *game);
#endif
