#ifndef MSX_NEO_WORLD_H
#define MSX_NEO_WORLD_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_script.h"
#include "neo_soldier_ai.h"
#include "neo_throw_actor.h"
#include "neo_gactor.h"
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
enum { NEO_WORLD_SOLDIERS=12, NEO_WORLD_THROWS=8, NEO_WORLD_SPLASHES=8, NEO_WORLD_GACTORS=40,
    NEO_WORLD_ORDER=80, NEO_WORLD_DRAWS=96, NEO_WORLD_BOXES=48 };
typedef enum { NEO_WORLD_SCRIPT, NEO_WORLD_RUNNER, NEO_WORLD_SOLDIER, NEO_WORLD_THROW,
    NEO_WORLD_SPLASH, NEO_WORLD_GACTOR } neo_world_kind_t;
/* depth: original +10 (creation depth; ROM 004192 deletes following deeper
 * nodes with their parent). */
typedef struct { uint8_t kind,index,depth; } neo_world_slot_t;
/* Dynamic collision object owned by a generic actor (RAM 10C06E + 64*i):
 * local position block relative to its parent (+00..+25). */
typedef struct {
    bool used;int16_t owner;
    neo_contact_actor_t block;   /* x,y,vx,vy,parent,projected,cached,mark,correction */
} neo_world_dyn_t;
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
    uint8_t follow_x,follow_y; /* +7A/+7B (0xFF while the player is controllable) */
    bool inactive;             /* RAM 10C33C bit clear: the player is dying (its
                                * own update clears it before the others run) */
    bool unregistered;         /* +50 clear: no hurtbox registered (004C24) */
} neo_world_player_t;
typedef struct {
    neo_script_t script;
    neo_soldier_t soldiers[NEO_WORLD_SOLDIERS];
    neo_soldier_spawn_t pending[NEO_WORLD_SOLDIERS];bool constructed[NEO_WORLD_SOLDIERS];
    neo_throw_actor_t throws[NEO_WORLD_THROWS];uint8_t throw_palettes[NEO_WORLD_THROWS][4];
    neo_world_splash_t splashes[NEO_WORLD_SPLASHES];
    neo_gactor_t gactors[NEO_WORLD_GACTORS];
    neo_world_dyn_t dyn[NEO_KDYN_COUNT];
    neo_contact_parent_t dyn_parent[NEO_KDYN_COUNT];  /* projected blocks as parents */
    /* ROM 004C24 registry of the generic actors, double buffered: owners
     * registered during this update / queried during this update. */
    struct { uint8_t owner; uint16_t xbins; uint8_t ybins; } boxes[NEO_WORLD_BOXES],last_boxes[NEO_WORLD_BOXES];
    unsigned box_count,last_box_count;
    uint16_t view[4];
    neo_world_slot_t order[NEO_WORLD_ORDER];unsigned order_count;
    neo_kscene_t kernel;
    neo_contact_parent_t terrain,root;
    neo_rng_t rng;
    uint16_t grid[128];unsigned grid_cursor;
    uint8_t epoch;
    uint16_t video_phase;          /* RAM 10000E, set by the caller per update */
    int32_t scroll_x;              /* RAM 10E97C: -camera, Q16.16 */
    int32_t scroll_y;              /* RAM 10E980: scene object y (0x01000000 at the
                                    * base height; the caller sets it per update) */
    /* Optional trace hook, called before every actor update (tests). */
    void (*trace)(void *context,uint8_t kind,uint8_t index);void *trace_context;
    neo_world_draw_t draws[NEO_WORLD_DRAWS];unsigned draw_count;
    /* Counters for the caller and the documentation of the gaps. */
    unsigned ticks,spawned,kills,player_hits,external_spawns,unsupported,overflow;
    uint32_t external_constructor;  /* last skipped spawn-table entry */
    uint16_t hit_x;                 /* projected x of the last attacker that hit the player */
    int32_t camera_lock;            /* RAM 69BC high word set by 0C7B0C; -1 = free (0C7C12) */
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
/* Attack row (ROM 0049E8 format) of an outside attacker against generic
 * actor `index`, as registered during the previous update (ROM 004762). */
bool neo_world_hit_actor(neo_world_t *world,unsigned index,const neo_contact_actor_t *attacker,uint8_t a5e,const int16_t row[7]);
/* True once the script has passed the camera-1920 lock (2D6A1A): the end of
 * the imported records. */
bool neo_world_region_clear(const neo_world_t *world);
unsigned neo_world_alive(const neo_world_t *world);
#endif
