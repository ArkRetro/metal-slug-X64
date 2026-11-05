#ifndef MSX_PLAYER_RUNTIME_H
#define MSX_PLAYER_RUNTIME_H
#include "gameplay.h"
#include "marco_campaign.h"
#include "marco_campaign_world.h"
#include "marco_campaign_sensing.h"
#include "marco_campaign_queries.h"
#include "marco_campaign_pistol.h"
#include "neo_rng.h"

enum { PLAYER_RUNTIME_SOUNDS=16,PLAYER_RUNTIME_STAMP_COLUMNS=72,PLAYER_RUNTIME_STAMP_ROWS=5 };
/* Live campaign owner. All controller state is initialized native state; the
 * offline campaign assets contain cartridge data, never a captured timeline. */
typedef struct {
    marco_campaign_t campaign;
    marco_campaign_world_t world;
    marco_campaign_sensing_t scene;
    marco_campaign_environment_t environment;
    marco_campaign_environment_effect_t world_policy;
    marco_campaign_pistol_t pistol;
    neo_rng_t random;
    marco_campaign_hit_registry_t registry[2];
    msx_game_t *game;
    unsigned bank;
    int draw_camera,draw_offset_x,draw_offset_y;
    uint16_t palette_descriptor,palette_bank;
    int16_t scene_death_line;
    int16_t damage_offset_x,damage_offset_y;
    /* Native source-map owner used by 019432/B5870's foreground guard. */
    uint32_t foreground_resource_tag;
    bool foreground_parent,foreground_visual_difference;
    unsigned pending_environment_effects,pending_visual_edits,world_policy_writes;
    uint8_t footprints[PLAYER_RUNTIME_STAMP_COLUMNS*PLAYER_RUNTIME_STAMP_ROWS];
    unsigned footprint_writes;
    marco_campaign_melee_request_t last_melee_request;
    unsigned melee_requests;
    bool projectile_owned[MSX_BULLETS],projectile_started[MSX_BULLETS];
    unsigned projectile_born[MSX_BULLETS];
    uint8_t projectile_kind[MSX_BULLETS];
    uint16_t sound_queue[PLAYER_RUNTIME_SOUNDS];
    unsigned sound_head,sound_count;
    bool initialized;
} player_runtime_t;

/* Starts at the real game's supported feet and emits the first native frame.
 * The game calls step on alternate VI and after_world after every world tick. */
bool player_runtime_start(player_runtime_t*,msx_game_t*);
bool player_runtime_step(void*,msx_game_t*,const msx_game_input_t*);
void player_runtime_after_world(player_runtime_t*,msx_game_t*);
/* Original 0120F8 sound tokens; the native audio owner consumes this queue. */
bool player_runtime_pop_sound(player_runtime_t*,uint16_t*);
bool player_runtime_stamp(const player_runtime_t*,int column,int source_row);
#endif
