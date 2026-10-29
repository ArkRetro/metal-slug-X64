#ifndef MSX_NEO_WORLD_H
#define MSX_NEO_WORLD_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_script.h"
#include "neo_soldier_ai.h"
#include "neo_throw_actor.h"
/* Closed native world of the playable Mission 1 region: level script and its
 * runners, rebel soldiers (constructor + routine family + pistol death), the
 * objects they throw, the death splash child (ROM 040F88) and the shared
 * delay grid/RNG. One call = one original logic update (every second video
 * tick). Actors run in the original list order: the mission actor's grid
 * decay and group recount (ROM 0186A0 -> 00469C/019E40), the script, then
 * every actor in creation order, each new actor inserted right after its
 * creator (ROM 0040BC). The player is not part of this module: the caller
 * supplies its projected position and receives the damage requests.
 *
 * Positions are scene-local Q16.16 with Y up (the native collision grid);
 * the scene object sits at (-camera, 256) as RAM 10C742/10C746 do. */
enum { NEO_WORLD_SOLDIERS=12, NEO_WORLD_THROWS=8, NEO_WORLD_SPLASHES=8, NEO_WORLD_ORDER=40,
    NEO_WORLD_DRAWS=48 };
typedef enum { NEO_WORLD_SCRIPT, NEO_WORLD_RUNNER, NEO_WORLD_SOLDIER, NEO_WORLD_THROW,
    NEO_WORLD_SPLASH } neo_world_kind_t;
typedef struct { uint8_t kind,index; } neo_world_slot_t;
typedef struct {
    neo_contact_actor_t body;
    neo_anim_state_t anim;
    neo_anim_render_t render;
    bool active,started;
} neo_world_splash_t;
/* One emitted animation draw (ROM 004364), with the logical palette resource
 * the renderer must use (requested through 009EA8 by the original). */
typedef struct {
    uint32_t descriptor;
    uint16_t x,y,attributes;   /* projected words; attributes bit 0/1 = flips */
    uint16_t palette;          /* logical resource, e.g. 0x020E */
    bool flash;
    uint8_t kind;              /* neo_world_kind_t of the owner */
} neo_world_draw_t;
typedef struct {
    uint16_t px,py;            /* projected words (+3C/+40) left by the player's
                                * own previous update: screen x, y + 256 */
    uint8_t flags15;           /* flips and ground bit as registered */
    bool alive,dead;           /* actor present; +5F bits 7/6 */
} neo_world_player_t;
typedef struct {
    neo_script_t script;
    neo_soldier_t soldiers[NEO_WORLD_SOLDIERS];
    neo_soldier_spawn_t pending[NEO_WORLD_SOLDIERS];bool constructed[NEO_WORLD_SOLDIERS];
    neo_throw_actor_t throws[NEO_WORLD_THROWS];uint8_t throw_palettes[NEO_WORLD_THROWS][4];
    neo_world_splash_t splashes[NEO_WORLD_SPLASHES];
    neo_world_slot_t order[NEO_WORLD_ORDER];unsigned order_count;
    neo_kscene_t kernel;
    neo_contact_parent_t terrain,root;
    neo_rng_t rng;
    uint16_t grid[128];unsigned grid_cursor;
    uint8_t epoch;
    uint16_t video_phase;          /* RAM 10000E, set by the caller per update */
    int32_t scroll_x;              /* RAM 10E97C: -camera, Q16.16 */
    /* Optional trace hook, called before every actor update (tests). */
    void (*trace)(void *context,uint8_t kind,uint8_t index);void *trace_context;
    neo_world_draw_t draws[NEO_WORLD_DRAWS];unsigned draw_count;
    /* Counters for the caller and the documentation of the gaps. */
    unsigned ticks,spawned,kills,player_hits,external_spawns,unsupported,overflow;
    uint32_t external_constructor;  /* last skipped spawn-table entry */
    uint32_t unsupported_routine;   /* last soldier routine that stopped */
} neo_world_t;
/* Starts the region script at its first record with the RNG ring dumped at
 * the mission handoff (data/soldier_ai_tables.json). */
void neo_world_init(neo_world_t *world,int32_t scroll_x);
/* One original logic update. Returns the number of hits on the player
 * (knife 0AEF14 and thrown-object flight attack ROM 0E0894). */
unsigned neo_world_tick(neo_world_t *world,int32_t scroll_x,const neo_world_player_t *player);
/* Player pistol shot segment against the registered soldiers (ROM 0049E8 with
 * attack row ROM 01B4C2: kind 6, class 1, damage 10). Coordinates are
 * scene-local Q16.16 of the bullet before and after its move. Returns true
 * when a soldier consumed the bullet. */
bool neo_world_shot(neo_world_t *world,int32_t previous_x,int32_t x,int32_t y,bool left);
/* True once the script has passed the region's "all groups cleared" waits
 * (ROM 2D650E/2D6514). */
bool neo_world_region_clear(const neo_world_t *world);
unsigned neo_world_alive(const neo_world_t *world);
#endif
