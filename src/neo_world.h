#ifndef MSX_NEO_WORLD_H
#define MSX_NEO_WORLD_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_script.h"
#include "neo_soldier_ai.h"
#include "neo_throw_actor.h"
#include "neo_gactor.h"
#include "neo_camera.h"
#include "neo_boss.h"
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
    NEO_WORLD_ORDER=80, NEO_WORLD_DRAWS=96, NEO_WORLD_BOXES=48, NEO_WORLD_SOUNDS=16 };
typedef enum { NEO_WORLD_SCRIPT, NEO_WORLD_RUNNER, NEO_WORLD_SOLDIER, NEO_WORLD_THROW,
    NEO_WORLD_SPLASH, NEO_WORLD_GACTOR, NEO_WORLD_BOSS } neo_world_kind_t;
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
    uint8_t zoom_x,zoom_y;     /* generic actors: +12/+13 (0xFF = full size) */
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
/* Sub-script actors 019E72 created by generic actors (robed swordsman
 * 0A8C46/0A8C62): the same interpreter over another record. */
enum { NEO_WORLD_SUBSCRIPTS=4, NEO_WORLD_SOLIDS=16 };
typedef struct { bool used; int16_t owner; neo_script_t script; } neo_world_sub_t;
typedef struct {
    neo_script_t script;
    neo_world_sub_t subs[NEO_WORLD_SUBSCRIPTS];
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
    /* Object lists RAM 43F4/43A2 (written when the counter RAM 10000F bit 0
     * is clear / set, cleared by 0401E8 before the actors run; the other one
     * is queried) and the marker id counter RAM 10C446. */
    neo_marker_t markers[2][8];uint8_t marker_count[2],marker_seq;
    neo_world_slot_t order[NEO_WORLD_ORDER];unsigned order_count;
    neo_kscene_t kernel;
    neo_contact_parent_t terrain,root;
    neo_rng_t rng;
    uint16_t grid[128];unsigned grid_cursor;
    /* RAM 106756..1067D6: solid bodies registered by 004E76 (actor, body
     * descriptor; the slot index is the actor's +6B), read by the player's
     * push-out 004ED2. actor = gactor index, -1 = free slot. */
    struct { int16_t actor; uint32_t desc; } solids[NEO_WORLD_SOLIDS];
    unsigned solid_unsupported;     /* 004E76 traps (+6B >= 16) */
    uint8_t epoch;
    uint16_t video_phase;          /* RAM 10000E, set by the caller per update */
    int32_t scroll_x;              /* RAM 10E97C: -camera, Q16.16 */
    int32_t scroll_y;              /* RAM 10E980: scene object y (0x01000000 at the
                                    * base height; the caller sets it per update) */
    /* Optional trace hook, called before every actor update (tests). */
    void (*trace)(void *context,uint8_t kind,uint8_t index);void *trace_context;
    neo_world_draw_t draws[NEO_WORLD_DRAWS];unsigned draw_count;
    /* Sound requests (original 0120F8 words, e.g. 0x1041 from animation
     * command 0x30, 0x10FA knife hit, 0x1027 thrown-object burst) in emission
     * order. Appended by every tick and never cleared here: the consumer
     * drains the list (sound_count=0); extra requests count in sound_dropped. */
    uint16_t sounds[NEO_WORLD_SOUNDS];unsigned sound_count,sound_dropped;
    /* Counters for the caller and the documentation of the gaps. */
    unsigned ticks,spawned,kills,player_hits,external_spawns,unsupported,overflow;
    uint32_t external_constructor;  /* last skipped spawn-table entry */
    uint16_t hit_x;                 /* projected x of the last attacker that hit the player */
    int32_t camera_lock;            /* RAM 69BC high word set by 0C7B0C; -1 = free (0C7C12) */
    /* Native mission camera (neo_camera.h) driven by the caller before each
     * tick (neo_world_camera); when set, RAM 10E99E is its progress and the
     * script's stop/release/autoscroll go to it. Off in the closed-loop
     * world tests, which take the scroll from the captures. */
    neo_camera_t camera;bool camera_native;
    /* Optional extra target of the player's shots (the boss, neo_boss). */
    bool (*shot_hook)(void *context,const neo_contact_actor_t *bullet,const int16_t row[7]);void *shot_context;
    /* Mission 1 boss (neo_boss, script record 2D70FC): its nodes run as one
     * block at the main node's list position. Palette slots of its
     * allocations (009EA8) map to logical resources for drawing; its
     * hurtboxes registered during the previous update take the player's
     * shots; its generic actors (explosions, debris, smoke) join the world. */
    neo_boss_t boss;bool boss_active;
    uint16_t boss_palette[32];uint8_t boss_palette_refs[32];
    int16_t boss_boxes[NEO_BOSS_NODES],last_boss_boxes[NEO_BOSS_NODES];unsigned boss_box_count,last_boss_box_count;
    uint8_t mission_step;           /* 018F92 (1) / 018F40 (2) requested by the boss */
    uint8_t flag606;                /* RAM 10C606 */
    unsigned boss_unsupported,boss_external_effects;uint32_t boss_unsupported_label,boss_unsupported_routine;
    unsigned boss_hits;
    uint32_t unsupported_routine;   /* last soldier routine that stopped */
    unsigned item_pickups;uint32_t item_pickup_routine; /* items taken: explicit stops (neo_item.c) */
    /* Domed building (054904) and its collapse: script flags, player lock,
     * kill-all, white flash and the map swap of 0B595A (0B5972 turns the
     * flash on, 0B5982 runs 0B58CC on the next update and turns it off). */
    neo_ga_flags_t ga_flags;
    bool kill_active;               /* RAM 1067E2 */
    uint8_t swap_step;
    unsigned killed_all;            /* soldiers killed by 004BAC */
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
/* Same with pistol attack row `row` (0..15, ROM 01B4C2 + 16*row: the
 * bullet's direction) and a segment (previous_x,previous_y)->(x,y). */
bool neo_world_shot_row(neo_world_t *world,int32_t previous_x,int32_t previous_y,int32_t x,int32_t y,unsigned row,bool left);
/* Attack row (ROM 0049E8 format) of an outside attacker against generic
 * actor `index`, as registered during the previous update (ROM 004762). */
bool neo_world_hit_actor(neo_world_t *world,unsigned index,const neo_contact_actor_t *attacker,uint8_t a5e,const int16_t row[7]);
/* True once the script has passed the camera-1920 lock (2D6A1A): the end of
 * the imported records. */
bool neo_world_region_clear(const neo_world_t *world);
unsigned neo_world_alive(const neo_world_t *world);
/* Mission actor's camera step (ROM 018D96..0C81C0, neo_camera_update) with
 * the player's projected words; enables the native camera. The caller then
 * ticks the world with neo_camera_root_x/neo_camera_root_y as the scroll. */
void neo_world_camera(neo_world_t *world,const neo_world_player_t *player);
#endif
