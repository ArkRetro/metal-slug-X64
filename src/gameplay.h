#ifndef MSX_GAMEPLAY_H
#define MSX_GAMEPLAY_H
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "neo_vehicle.h"
#include "neo_world.h"
#include "neo_hud.h"
#include "neo_end.h"
#include "neo_continue.h"
#include "neo_landing.h"
#include "neo_fade.h"
#include "neo_mission.h"
#include "marco_hgun.h"
#include "marco_legs.h"
#include "marco_death.h"
// Whole Mission 1: map/collision 3840 px (camera progress 0..3520, the
// original command list holds at 3520). The player's right limit keeps the
// arcade fraction camera+292-1/65536 (measured at 1164 for camera 872 and
// 2212 for 1920); MSX_REGION_END_X is the end of the village region.
enum { MSX_BULLETS=8, MSX_ENEMY_BULLETS=4, MSX_UNIT=65536, MSX_WORLD_WIDTH=3840 };
// Historical observed prototype start; cinematic handoff supplies its own measured position.
enum { MSX_START_X=0x78bad9, MSX_REGION_END_X=2212*MSX_UNIT-1, MSX_END_X=MSX_WORLD_WIDTH*MSX_UNIT-1 };
/* up: fired with up held (moves up, original pistol attack row 4). */
/* kind: weapon id that fired it (RAM 10C71E); 0 = pistol. */
#ifndef MSX_HUD_TIMER_DIP
#define MSX_HUD_TIMER_DIP 0   /* RAM 10FD8C (003586 index): 60 */
#endif
typedef struct { int x,y,direction; bool active,up; uint8_t kind; } msx_bullet_t;
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
    /* Fix-layer HUD (neo_hud.c, docs/ARMAS_VEHICULOS.md §11), updated with
     * the world; timer_resets_seen: 01913C spawns already applied. */
    neo_hud_t hud;unsigned timer_resets_seen,hud_game_over;
    /* Game over and continue (neo_hud.c 0C9848): the new life found no life
     * (no_life: one HUD update), the player stays gone until the continue;
     * start_edge/jump_edge: Start and A pressed since the previous update. */
    bool no_life,awaiting_continue,start_edge,jump_edge;
    unsigned continues,game_over_frames;
    /* Mission end sequence 0x018846 (gameplay.c mission_end_step). */
    uint8_t mission_end;bool end_started;uint16_t end_timer;unsigned end_stops;
    /* Its actors: the prisoner tally 0308C0 (neo_tally.c) and the MISSION
     * COMPLETE letters 06090A (neo_end.c); RAM 10EA1B (cae6c: highest
     * mission reached). end_ram: the RAM the tally reads and writes (copied
     * from and back to the world); end_fire_was/end_jump_was: A and B held
     * at the previous update (new presses 10C35D). */
    neo_end_t end;uint8_t best_mission;
    /* Stage flow of the mission actor (neo_mission.c): Mission 1 stage
     * 016DCE until 01891E selects the next mission. */
    neo_mission_t flow;
    bool start_letters;unsigned start_stops;   /* MISSION 1 START 0607DE in end (descent) */
    neo_end_ram_t end_ram;bool end_fire_was,end_jump_was;
    /* The big CONTINUE? and digit: the mission actor's program 018120
     * (neo_continue.c), started at the mission start (01868A). */
    neo_continue_t continue_sprites;
    /* The parachute landing's canopy and player marker (neo_landing.c),
     * created at the entry handoff (landing_pending) and run after the
     * player; landing_x/y: the player's projected longs of the previous
     * update (+44/+48). */
    neo_landing_t landing;bool landing_pending,landing_on;int32_t landing_x,landing_y;
    /* The parachute descent (marco_descent.c) from the entry handoff at its
     * start: hanging (0C162A) or falling after the cut (0C170C); fire
     * latched for its update; the scene root of the previous update (01AD52). */
    uint8_t descent;bool descent_fire;int32_t landing_root_x,landing_root_y;unsigned descent_cuts;
    neo_fade_t fade;   /* RAM 10991A..10992C (9C0A/698C) */
    neo_vehicle_t vehicle;
    unsigned vehicle_events,wreckage_cells,vehicle_flash;
    unsigned shots;   /* pistol bullets created (sound request 0x10F7 each) */
    /* Dynamic platform the player stands on (kernel object NEO_KOBJ_DYN0 +
     * ride - 1, 0 = none) and the feet relative to it (Q16.16): the original
     * body's +28/+2C are relative to its parent object (+38), so it moves
     * with the platform (wagon 033C98, boss). */
    uint8_t ride;int32_t ride_x,ride_y;unsigned rides;
    unsigned wall_stops,pit_deaths;   /* side stops of the sweeps 0143EE/014594 (wall_stop) */
    /* Reference build only: the last grounded position, where a fall
     * below the screen puts the player back (pit_rescues) instead of the
     * fall death 0C3712 (the invulnerability also covers pits). */
    int32_t safe_x,safe_y;bool safe;unsigned pit_rescues;
    unsigned solid_pushes;   /* updates the push-out 004ED2 met a solid body */
    /* Weapons (world.weapons, RAM 10C718; docs/ARMAS_VEHICULOS.md). A shot
     * reserves through 0C0D64 for the current weapon; only the pistol's
     * constructor 01B782 has a port (the bullets above). A shot of another
     * weapon is an explicit stop: counted with its constructor (table
     * 0C42AA) and fired as the port's pistol bullet; the H fires native
     * bullets (neo_bullet.c). grenade: input (bomb button, set by the
     * caller like aim_up); a throw consumes a bomb (0C478E) and throws the
     * native grenade 01BF46/01BF10 (grenades); other bomb types stop
     * explicitly (grenade_stops). */
    bool grenade,grenade_was_down;
    /* H upper body (marco_hgun.c): fire states, sweeps and their diagonal
     * bullets; fire_was: the fire input of the previous tick (+9B bit 4). */
    marco_hgun_t hgun;bool fire_was;unsigned hgun_diagonal;uint8_t hgun_kind;
    marco_legs_t legs;   /* +72/+74 of the body programs (marco_legs.c) */
    /* Pistol/other weapons fire state (0C3CD0/0C3D9A, docs/ARMAS_VEHICULOS.md
     * §3): fire_request is +9B bit 4 (a new press, kept until a shot takes
     * it; holding never re-sets it), pistol_wait the updates until the
     * program's +76 lets the next press fire (level 24C70A: +76 at 1, state
     * latency 1 -> 2; up/down 24C7F2/24EABC: +76 at 2 -> 3), and
     * pistol_pending the ticks until the bullet whose constructor runs the
     * update after the fire callback (00411C at the callback, 01B782 one
     * update later) joins the list. Both counts are in ticks (2 per update);
     * a press fires on the tick it arrives, as the original's state does in
     * the update of the press (hold2: press 2190, callback 2191). */
    bool fire_request;unsigned pistol_pending,pistol_wait;msx_bullet_t pistol_next;
    /* The knife 0C3B46: reach of 0C3744 (+9A bit 0), ticks left of the cut
     * program, cuts, cuts that hit. */
    bool knife_reach;unsigned knife_t,knife_cuts,knife_hits;
    /* Death and new life (marco_death.c, docs/JEFE_MISION1.md): one hit
     * kills (0C2956); dying/lying/gone/entering phases; death_stops counts
     * the deaths of kinds 8 / 0x12 (own routines, not translated). */
    marco_death_t life;unsigned death_stops,time_overs;bool time_over_seen;
    unsigned weapon_shots,weapon_stops,grenades,grenade_stops,expiries;uint32_t weapon_stop_constructor;
    unsigned h_shots;   /* native H bullets fired (01B6DC/01B716) */
    unsigned c_shots;   /* native C shells fired (063F32 with the flash 0C58D2) */
    unsigned c_fires;   /* C fire states entered (0C3CD0: its program starts even when 0C0D64 refuses) */
    unsigned down_shots; /* pistol shots aimed down in the air: native 01B782 at angle C0 */
    bool aim_down;      /* 0C0FEA bit 1 in the air (down held, not grounded): +92 = FF */
    /* The mounted player (marco_mount.c, ROM 0C3078..0C3260) on player 1's
     * node record world.player_node[0]: mount = none, riding (0C3078/
     * 0C309C: the port's player is hidden, has no hurtbox and follows the
     * slot) or the eject jump (0C3250). pad_held: the held input bits of the
     * previous update (0CA1B8 new presses). mount_stop: ROM label of the
     * last untranslated path met (explicit stop, logged). */
    uint8_t mount;uint8_t pad_held;unsigned mounts,dismounts,mount_stops;uint32_t mount_stop;
    bool debug_dropped;   /* screenshot cartridge only (MSX_DEBUG_CAMEL_DROP) */
    /* Stages of a mission pack (docs/MISION2.md "Fase 2"): walk-in to
     * walk_in_x (Q16) at the stage start; stage_step: the stage end
     * (018722, exit kind +6) until the flow's 0183D0 asks for the next
     * stage (stage_load: actor.c loads it and calls msx_game_begin_stage);
     * stage_camera_max: the list's hold; stage_exit_hooks: stage ends set by
     * the reference-build hook (MSX_REFERENCE_STAGE_EXIT), not by 0B6F24
     * (stage 1 of Mission 2 has none: the gate 03889C makes 0B6ED2). */
    int walk_in_x;uint8_t stage_step;uint16_t stage_timer;bool stage_load,in_stage;
    int stage_camera_max;unsigned stage_exit_hooks,stage_lock_releases,lock_held,lock_timeouts;
    /* Reference build only (msx_autopilot.h): the test's stage-3 pilot. */
    struct {bool on;int8_t air_dir,pre_dir,kind,jdir,jpre;int16_t air_y,goal_x,goal_y,take_x,jair_y;uint16_t still,last_x,plans,jumps,plan_tick;} pilot;
} msx_game_t;
enum { MSX_MOUNT_NONE, MSX_MOUNT_RIDE, MSX_MOUNT_EJECT };
void msx_game_init(msx_game_t *game);
/* Accept a measured grounded world position only when native support agrees. */
bool msx_game_begin_at(msx_game_t *game,int32_t world_x,int32_t world_y);
enum { MSX_DESCENT_NONE, MSX_DESCENT_HANG, MSX_DESCENT_FALL };
/* The handoff at the start of the parachute descent (MAME 3860: +28/+2C,
 * +30/+34 after the player's first 0C162A update), with the canopy. */
void msx_game_begin_descent(msx_game_t *game,int32_t world_x,int32_t world_y,int32_t vx,int32_t vy);
void msx_game_update(msx_game_t *game,int direction,bool jump,bool fire);
/* Reference build: the route planner of Mission 2 stage 3 (msx_autopilot.h)
 * replaces the pad's direction and jump while game->pilot.on; the release
 * build leaves them as they are. */
void msx_game_autopilot(msx_game_t *game,int *direction,bool *jump);
/* A stage of a mission pack (the collision grid already set): the world's
 * script and camera list, the HUD, flow, weapons and score kept; the player
 * enters walking from the left edge to start_x (pixels), as in the run4
 * capture of Mission 2 (19440..19530). */
void msx_game_begin_stage(msx_game_t *game,const neo_world_stage_t *stage,int start_x,int camera_max);
/* Stage end (018F40 + 018722 for a stage whose next record is a stage). */
void msx_game_stage_end(msx_game_t *game);
/* A hit on the player (from: world x of the attacker, used without the
 * native world): the death 0C2956 unless invulnerable or already dying. */
void msx_game_hurt(msx_game_t *game,int from);
/* True only in the reference traversal build (MSX_REFERENCE_INVULNERABLE). */
extern const bool msx_reference_invulnerable;
/* Layout of msx_game_t as one object file sees it. gameplay.c exports its
 * own; actor.c compares it with the value it was compiled with, so a stale
 * object built against an older neo_world_t (the reference cartridge's
 * gameplay.o once missed the item fields: its ga_flags.player_handlers read
 * the low byte of item_pickups and the first item taken froze the player's
 * input) stops at start instead of corrupting the game. */
enum { MSX_END_NONE,MSX_END_WAIT,MSX_END_TALLY,MSX_END_LETTERS,MSX_END_FADE,MSX_END_DONE };
/* Updates of the GAME OVER panel before the original game ends (MAME go1). */
#define MSX_GAME_OVER_UPDATES 190u
#define MSX_GAME_LAYOUT ((uint32_t)sizeof(msx_game_t)^(uint32_t)sizeof(neo_world_t)<<16^ \
    (uint32_t)offsetof(msx_game_t,world.ga_flags)<<8^(uint32_t)offsetof(msx_game_t,ride)<<4)
extern const uint32_t msx_game_layout;
int msx_game_animation(const msx_game_t *game);
/* Projected words of the player (+3C/+40) as the native world receives them. */
void msx_game_projected(const msx_game_t *game,uint16_t *x,uint16_t *y);
int msx_ground_offset(int x16);
/* Internal reference encounter; campaign spawn/wave logic is not recovered yet. */
void msx_game_vehicle_reference(msx_game_t *game);
#endif
