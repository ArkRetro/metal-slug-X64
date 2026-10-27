#ifndef MSX_GAMEPLAY_H
#define MSX_GAMEPLAY_H
#include <stdbool.h>
enum { MSX_BULLETS=8, MSX_ENEMY_BULLETS=4, MSX_UNIT=65536, MSX_WORLD_WIDTH=1192 };
enum { MSX_END_X=1164*MSX_UNIT-1 };
typedef struct { int x,y,direction; bool active; } msx_bullet_t;
enum { MSX_IDLE, MSX_RUN, MSX_FIRE, MSX_JUMP, MSX_RUN_JUMP, MSX_RUN_FIRE };
typedef struct { int x,facing; unsigned health,flash,cooldown,respawn; bool alive; } msx_enemy_t;
typedef struct {
    int x,y,velocity_x,velocity_y,facing,camera,checkpoint;
    unsigned ticks,air_ticks,shot_age,cooldown,hits,health,invulnerable,kills,deaths;
    bool grounded,jump_was_down,jump_pending,moving,shooting,complete;
    msx_bullet_t bullets[MSX_BULLETS], enemy_bullets[MSX_ENEMY_BULLETS];
    msx_enemy_t enemy;
} msx_game_t;
void msx_game_init(msx_game_t *game);
void msx_game_update(msx_game_t *game,int direction,bool jump,bool fire);
int msx_game_animation(const msx_game_t *game);
int msx_ground_offset(int x16);
#endif
