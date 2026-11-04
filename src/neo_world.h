#ifndef MSX_NEO_WORLD_H
#define MSX_NEO_WORLD_H
/* Stage 3 tower pieces remembered for the renderer (81 in run4, up to the boss). */
enum { NEO_TOWER_PIECES=128 };
#include <stdbool.h>
#include <stdint.h>
#include "neo_script.h"
#include "neo_soldier_ai.h"
#include "neo_throw_actor.h"
#include "neo_gactor.h"
#include "neo_camera.h"
#include "neo_train.h"
#include "neo_boss.h"
#include "neo_blood.h"
#include "neo_vslot.h"
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
enum { NEO_WORLD_SOLDIERS=12, NEO_WORLD_THROWS=8, NEO_WORLD_SPLASHES=8, NEO_WORLD_GACTORS=96,
    NEO_WORLD_ORDER=160, NEO_WORLD_DRAWS=160, NEO_WORLD_BOXES=48, NEO_WORLD_SOUNDS=16 };
typedef enum { NEO_WORLD_SCRIPT, NEO_WORLD_RUNNER, NEO_WORLD_SOLDIER, NEO_WORLD_THROW,
    NEO_WORLD_SPLASH, NEO_WORLD_GACTOR, NEO_WORLD_BOSS } neo_world_kind_t;
/* depth: original +10 (creation depth; ROM 004192 deletes following deeper
 * nodes with their parent). */
/* A player shot waiting for its list insertion (neo_world_player_shot). */
typedef struct { uint32_t routine;int32_t x,y,vx,vy;uint8_t angle,player,flags15;bool grenade; bool actor;uint8_t f5e; bool laser; } neo_world_shot_request_t;
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
    uint32_t program;   /* 2402FA (040F88, kind 6) or 24039C (040F60, kinds 0A/0B) */
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
    uint16_t z;                /* +1E: the sprite list is sorted by it (0113EA) */
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
    /* Item pickup boxes RAM 10C5EC/10C5F4 from outside (a recording); NULL:
     * the port's player keeps the standing box (neo_world_tick). */
    const uint16_t *pickup_input;
    neo_world_dyn_t dyn[NEO_KDYN_COUNT];
    neo_contact_parent_t dyn_parent[NEO_KDYN_COUNT];  /* projected blocks as parents */
    /* ROM 004C24 registry of the generic actors, double buffered: owners
     * registered during this update / queried during this update. */
    struct { uint8_t owner; uint16_t xbins; uint8_t ybins; } boxes[NEO_WORLD_BOXES],last_boxes[NEO_WORLD_BOXES];
    unsigned box_count,last_box_count;
    /* The same registry in its slot order (004C24 numbers the bodies as
     * they register, at most 0x40): soldiers, generic actors and boss
     * nodes, for the A0/A1 targets of 0049E8 (004B06 visits the slots
     * 16..31, 0..15, 48..63, 32..47). */
    struct { uint8_t kind,index; uint16_t xbins; uint8_t ybins; } regs[64],last_regs[64];
    unsigned reg_count,last_reg_count;
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
    unsigned timer_resets;          /* spawn-table entries 01913C (mission timer reset) */
    uint8_t arrow;                  /* RAM 10C44A: GO arrow direction (0529C0, read by 05287C) */
    /* Level-script records executed (op 7, script_spawn), in order, for the
     * reference log (actor.c drains them): created = the world made the
     * actor (or applied its effect), 0 = dropped. */
    struct {uint32_t record,constructor;uint8_t created;} spawn_log[16];unsigned spawn_log_count,spawn_log_dropped;
    /* Spawn-table vehicles stopped explicitly (none left: the Camel Slug
     * 03458E and the SV-001 092A32 are native, neo_slug.c / neo_tank.c). */
    unsigned vehicle_stops;uint32_t vehicle_stop_constructor;
    uint16_t hit_x;                 /* projected x of the last attacker that hit the player */
    uint8_t hit_kind;bool hit_right; /* the update's player hit: +5A kind, +5B bit 7 (attacker right) */
    int32_t camera_lock;            /* RAM 69BC high word set by 0C7B0C; -1 = free (0C7C12) */
    /* Native mission camera (neo_camera.h) driven by the caller before each
     * tick (neo_world_camera); when set, RAM 10E99E is its progress and the
     * script's stop/release/autoscroll go to it. Off in the closed-loop
     * world tests, which take the scroll from the captures. */
    neo_camera_t camera;bool camera_native;
    neo_train_t train;           /* Misión 3: capa del tren 10D7B6 (neo_train.c) */
    bool script_skip_unported;   /* reference cartridge: neo_script_env_t.skip_unported */
    /* Stage 3 tower writer 0B7C38 (created by 0B7C20; 10C6B2 counter
     * cleared by 0B7B00, 10C350 sequence, 10C354 mode): gameplay.c. */
    /* types[k]: piece type of the k-th piece (10C6B2 = 3k), drawn by the
     * renderer from the templates of map 0B740A (scene_render_set_tower);
     * drawn: pieces 0..drawn-1 placed. */
    struct { bool on; uint16_t counter; uint8_t table,at; int16_t mode; unsigned pieces,drawn; uint8_t types[NEO_TOWER_PIECES]; } tower;
    /* Stage of a mission pack (neo_world_init_stage); NULL camera = Mission 1's
     * list 0B57BE. */
    const uint8_t *stage_camera;uint16_t stage_camera_size;uint32_t stage_camera_base;unsigned camera_events_unsupported;
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
    bool front_window;              /* 012022: RAM 10B954 < 10B95C (signed), the stage's sprite windows (gameplay.c msx_draw_order) */
    uint8_t flag606;                /* RAM 10C606 */
    unsigned boss_unsupported,boss_external_effects;uint32_t boss_unsupported_label,boss_unsupported_routine;
    unsigned boss_hits;
    uint32_t unsupported_routine;   /* last soldier routine that stopped */
    unsigned item_pickups;uint32_t item_pickup_routine; /* items whose pickup stopped explicitly (neo_item.c) */
    /* The players' weapon block (RAM 10C718), player 1's weapon fields and
     * the score/pickup state the items serve (neo_item.c); the caller (the
     * port's player) reads and updates them. Pointers are set every tick. */
    marco_weapons_t weapons;marco_weapon_player_t weapon_player;neo_pickup_env_t pickup;
    /* Vehicle slots RAM 10C608/10C62C and records 10C2E8/10C2EC (neo_vslot.c):
     * free at the start (0AA4F6); player 1 accepts or drops its binding every
     * update (0C3352 -> 0AA458). The pickups and the HUD read player 1's. */
    neo_vslots_t vslots;
    /* What the player's vehicles read (neo_genv_t.vehicle): player 1's node
     * record 1014C4 (the port's player keeps it, gameplay.c), player 2's
     * (absent), the input words 10E9EC +0 new / +2 held (0CA1B8) and the
     * environment the world hands to the Camel Slug (set every tick). The
     * world no longer runs player 1's 0AA458: the port's player does, after
     * the actors (the original player 1014C4 runs after the Camel). */
    uint8_t player_node[2][0xc0];
    uint16_t pad[2];
    neo_vehicle_env_t vehicle_env;
    /* RAM 10F000, the original's debug-flag word (cleared at 002488): bit 7
     * keeps a vehicle's rider when the vehicle is destroyed (03519E and the
     * other vehicles). 0 in the game; the reference build sets bit 7. */
    uint8_t dip10f000;
    /* Player shots made by the player's update (neo_world_player_shot):
     * inserted after the list head (00411C, A6 = 101414) at the end of the
     * next tick, so their constructor runs on the update after, as in the
     * original. */
    neo_world_shot_request_t shot_requests[8];unsigned shot_request_count,shot_dropped;
    /* Camera-list calls that create an actor after the mission actor (0040BC
     * from the camera step; Mission 3 wagon boxes 0B918C/0B91AE): made at
     * the end of the next tick, like the shots. */
    uint32_t camera_calls[4];unsigned camera_call_count;
    unsigned player_shots;
    /* RAM 10C340..10C34F: the players' beam blocks of weapons 06/0B
     * (neo_world_laser, neo_pweapon.c); beams made (01E19E). */
    uint8_t laser[16];unsigned lasers;
    /* Mission 3 mid-boss 0A4A94 (neo_m3mid.c): the layer object 10E8C6
     * (RAM 10E8C6..10E94D) it sets up and moves, RAM 10E959 (bit 7: the
     * camera moves the layer by its own +8/+C, 0C82F4), and what the
     * renderer draws of it: the map 0C73CE drew (+2E then) and the patches
     * 0A6D7C applied since (0C714E: sub-map, d0..d5). */
    uint8_t m3layer[0x88];uint8_t m3layer959;bool m3layer_used;
    uint32_t m3layer_map;unsigned m3patch_count;
    /* The layer's 19 x 10 blocks as 0C714E left them (tile << 16 |
     * attribute; column major), drawn by actor.c. */
    uint32_t m3grid[19*10];
    /* Domed building (054904) and its collapse: script flags, player lock,
     * kill-all, white flash and the map swap of 0B595A (0B5972 turns the
     * flash on, 0B5982 runs 0B58CC on the next update and turns it off). */
    neo_ga_flags_t ga_flags;
    bool kill_active;               /* RAM 1067E2 */
    bool shots_clear;               /* RAM 1067E4 (0046E0: copy of the 004C06 request 1067E5) */
    uint8_t swap_step;
    /* Map patches 0C70FC applied this stage (sub-map, layer object), for the
     * renderer: their tile part replaces the drawn blocks (B37). */
    uint32_t map_patch[16][2];uint8_t map_patches;
    unsigned killed_all;            /* soldiers killed by 004BAC */
    /* RAM 10FD8F, arcade soft-DIP option 6 "BLOOD" (01B2BE: carry clear when
     * nonzero). neo_world_init sets MSX_BLOOD (default 1: ON, the Japanese
     * header default); the US/EU headers default it to OFF. */
    bool blood;
} neo_world_t;
/* Starts the region script at its first record with the RNG ring dumped at
 * the mission handoff (data/soldier_ai_tables.json). */
void neo_world_init(neo_world_t *world,int32_t scroll_x);
/* Stage of a mission pack (docs/MISION2.md "Fase 2", 0183D0): the script
 * section of the descriptor's +8 entry and the camera command list +0 from
 * its first command; the RNG ring, weapons and score carry over from the
 * previous stage (the original never resets them between stages). */
typedef struct {
    const uint8_t *script;uint16_t script_size;uint32_t script_base;
    const uint8_t *camera;uint16_t camera_size;uint32_t camera_base;
} neo_world_stage_t;
void neo_world_init_stage(neo_world_t *world,int32_t scroll_x,const neo_world_stage_t *stage);
/* One original logic update. Returns the number of hits on the player
 * (knife 0AEF14 and thrown-object flight attack ROM 0E0894). */
unsigned neo_world_tick(neo_world_t *world,int32_t scroll_x,const neo_world_player_t *player);
/* Position block of the object a +38 parent tag names (terrain, scene root,
 * layer objects, dynamic objects, generic actors, boss nodes): current and
 * previous projected position; false for an unknown tag. */
bool neo_world_parent(const neo_world_t *world,uint32_t tag,neo_contact_parent_t *out);
/* A shot of the player's weapon as a native bullet (neo_bullet.c, ROM
 * 01B6DC/01B716/01B750/01B782): fields the creator writes (0C4434: +6E
 * angle, +70 player; +28/+2C at the scene root, Q16.16, y up). False when
 * the request list is full. */
bool neo_world_player_shot(neo_world_t *world,uint32_t constructor,int32_t x,int32_t y,uint8_t angle,uint8_t player);
/* A grenade of the player (0C47BA: 01BF46, or 01BF10 when the player's +90
 * is FF): +6E player, +15 facing (01AC94), +28/+2C at the scene root,
 * +30/+34 the player's motion (0C4834). */
bool neo_world_player_grenade(neo_world_t *world,uint32_t constructor,int32_t x,int32_t y,int32_t vx,int32_t vy,bool left,uint8_t player);
/* A child of the player's fire callback 0C4372 made after the list head
 * (00411C) with the creator's fields: +6E angle (0C4244), +70 player word,
 * +15, +5E, +28/+2C at the scene root, +30/+34 (the C weapon: the muzzle
 * flash 0C58D2 placed by 01ACF2, the shell 063F32 by 0C482E). A shell
 * 063F32 the world cannot insert gives its projectile back (0C4526). */
bool neo_world_player_actor(neo_world_t *world,uint32_t constructor,int32_t x,int32_t y,int32_t vx,int32_t vy,uint8_t angle,uint8_t flags15,uint8_t f5e,uint8_t player);
/* 01E19E + 01E166 of the beam call 0C46D6 (weapons 06/0B, gameplay.c):
 * the player's block in `laser` gets the point (x, y: the fire program's
 * point plus +72/+74, facing forward, y up), the direction (+92: 0 level,
 * 1 up, FF down) and the life 4; with no beam alive (+6) the sound 10F8
 * (unless the other player's +7) and the beam 01E22C after the player
 * (0040BC reserve 0xE; inserted here after the list head with the next
 * shots, placed by 01B220 (0, 0) on the player's node record). */
void neo_world_laser(neo_world_t *world,uint8_t player,int16_t x,int16_t y,uint8_t direction);
/* Player pistol shot segment against the registered soldiers (ROM 0049E8 with
 * attack row ROM 01B4C2: kind 6, class 1, damage 10). Coordinates are
 * scene-local Q16.16 of the bullet before and after its move. Returns true
 * when a soldier consumed the bullet. */
bool neo_world_shot(neo_world_t *world,int32_t previous_x,int32_t x,int32_t y,bool left);
/* Same with pistol attack row `row` (0..15, ROM 01B4C2 + 16*row: the
 * bullet's direction) and a segment (previous_x,previous_y)->(x,y). */
/* An actor of the mission actor (map command routine): list head, +28/+2C. */
bool neo_world_mission_actor(neo_world_t *world,uint32_t routine,int32_t x,int32_t y);
bool neo_world_shot_row(neo_world_t *world,int32_t previous_x,int32_t previous_y,int32_t x,int32_t y,unsigned row,bool left);
/* 0049E8 for the player (A6 = 1014C4) with the attack row `row` at its
 * origin (x, y) (+15 bit 0 = left): the soldiers and the class-matching
 * generic actors take it (damage row[6], none for a query row); returns the
 * highest hurtbox type hit (D0), 0 for none. The knife: 0C3744 (query
 * 2D0AE4) and 0C419E (cut 2D0AF4). */
uint8_t neo_world_player_attack(neo_world_t *w,int32_t x,int32_t y,bool left,const int16_t row[7]);
/* Attack row (ROM 0049E8 format) of an outside attacker against generic
 * actor `index`, as registered during the previous update (ROM 004762). */
bool neo_world_hit_actor(neo_world_t *world,unsigned index,const neo_contact_actor_t *attacker,uint8_t a5e,const int16_t row[7]);
/* True once the script has passed the camera-1920 lock (2D6A1A): the end of
 * the imported records. */
bool neo_world_region_clear(const neo_world_t *world);
/* A spawn-table constructor the script creates natively (script_spawn). */
bool neo_world_spawn_stop(uint32_t constructor);
bool neo_world_spawn_native(uint32_t constructor);
unsigned neo_world_alive(const neo_world_t *world);
/* Mission actor's camera step (ROM 018D96..0C81C0, neo_camera_update) with
 * the player's projected words; enables the native camera. The caller then
 * ticks the world with neo_camera_root_x/neo_camera_root_y as the scroll. */
void neo_world_camera(neo_world_t *world,const neo_world_player_t *player);
/* Native animation program of a ROM program tag (tools/extract_soldier_programs.py). */
const neo_anim_program_t *neo_world_program(uint32_t tag);
#endif
