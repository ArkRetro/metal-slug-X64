#ifndef MSX_GAMEPLAY_H
#define MSX_GAMEPLAY_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_vehicle.h"
#include "neo_world.h"
// Whole Mission 1: map/collision 3840 px (camera progress 0..3520, the
// original command list holds at 3520). The player's right limit keeps the
// arcade fraction camera+292-1/65536 (measured at 1164 for camera 872 and
// 2212 for 1920); MSX_REGION_END_X is the end of the village region.
enum { MSX_BULLETS=8, MSX_ENEMY_BULLETS=4, MSX_UNIT=65536, MSX_WORLD_WIDTH=3840 };
// Historical observed prototype start; cinematic handoff supplies its own measured position.
enum { MSX_START_X=0x78bad9, MSX_REGION_END_X=2212*MSX_UNIT-1, MSX_END_X=MSX_WORLD_WIDTH*MSX_UNIT-1 };
/* up: fired with up held (moves up, original pistol attack row 4). */
typedef struct { int x,y,direction; bool active,up; } msx_bullet_t;
/* Screen/world mapping of the native region world: the original scene is
 * at (-camera, 256); its projected y word is 256 + original y (y up), and the
 * N64 draws original y at screen row 248 - y. */
enum { MSX_SCREEN_BASE_Y=248, MSX_PROJECTED_BASE_Y=256 };
enum { MSX_IDLE, MSX_RUN, MSX_FIRE, MSX_JUMP, MSX_RUN_JUMP, MSX_RUN_FIRE };
typedef struct {
    int x,y,velocity_x,velocity_y,facing,camera,checkpoint;
    /* Displayed camera: camera = ceil(RAM 10E98E), camera_height = ceil(RAM
     * 10E992) of the native mission camera (world.camera); region_end is
     * set once the village region's script waits are passed at 1920. */
    int camera_height;bool region_end,backdrop_after;
    /* Boss death: ROM 018F92 (mission complete, step 1) then 018F40 (step
     * 2, results); `complete` follows step 2 and stops the simulation. */
    bool mission_complete;
    unsigned ticks,air_ticks,shot_age,cooldown,hits,health,invulnerable,kills,deaths;
    bool grounded,jump_was_down,jump_pending,moving,shooting,complete;
    bool aim_up;   /* input: up held (set by the caller before msx_game_update) */
    bool crouch;   /* input: down held; grounded = crouched in place (the original
                    * crawls: not ported, see docs/MARCO_CAMPAIGN.md) */
    bool crouching,jump_moving;   /* state: crouched this update; takeoff while moving */
    msx_bullet_t bullets[MSX_BULLETS];
    /* Hostile straight projectiles observed by the depth25d event layer.
     * No actor of the native region emits one (rebel soldiers here knife and
     * throw objects); the list stays empty in the campaign. */
    msx_bullet_t enemy_bullets[MSX_ENEMY_BULLETS];
    /* Native region world (level script, rebel soldiers, thrown objects,
     * death splashes), updated with the player on alternate video ticks. */
    neo_world_t world;bool world_enabled;
    neo_vehicle_t vehicle;
    unsigned vehicle_events,wreckage_cells,vehicle_flash;
    unsigned shots;   /* pistol bullets created (sound request 0x10F7 each) */
    /* Dynamic platform the player stands on (kernel object NEO_KOBJ_DYN0 +
     * ride - 1, 0 = none) and the feet relative to it (Q16.16): the original
     * body's +28/+2C are relative to its parent object (+38), so it moves
     * with the platform (wagon 033C98, boss). */
    uint8_t ride;int32_t ride_x,ride_y;unsigned rides;
    unsigned solid_pushes;   /* updates the push-out 004ED2 met a solid body */
} msx_game_t;
void msx_game_init(msx_game_t *game);
/* Accept a measured grounded world position only when native support agrees. */
bool msx_game_begin_at(msx_game_t *game,int32_t world_x,int32_t world_y);
void msx_game_update(msx_game_t *game,int direction,bool jump,bool fire);
/* One hit on the player from world x `from` (knockback away from it). */
void msx_game_hurt(msx_game_t *game,int from);
/* True only in the reference traversal build (MSX_REFERENCE_INVULNERABLE). */
extern const bool msx_reference_invulnerable;
int msx_game_animation(const msx_game_t *game);
/* Projected words of the player (+3C/+40) as the native world receives them. */
void msx_game_projected(const msx_game_t *game,uint16_t *x,uint16_t *y);
int msx_ground_offset(int x16);
/* Internal reference encounter; campaign spawn/wave logic is not recovered yet. */
void msx_game_vehicle_reference(msx_game_t *game);
#endif
