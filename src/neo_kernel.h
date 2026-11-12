#ifndef MSX_NEO_KERNEL_H
#define MSX_NEO_KERNEL_H
#include <stdbool.h>
#include <stdint.h>
/* Native translation of the original static-terrain collision kernel:
 * ROM 013968 (sweep), 013776/013214/01351A (cell stepping), 012938 (cell
 * classification), 0129EC/012AD4/012CBA/012E64 (per-shape tables), 012EE2
 * (surface response) and 013DB2 (floor search). Objects are native structs;
 * no CPU address is dereferenced. Scope: the collision-object list observed
 * in the playable Mission 1 region (one cell-map terrain object, the scene
 * root and no dynamic platform). Unsupported contexts fail explicitly. */
enum { NEO_KOBJ_NONE, NEO_KOBJ_TERRAIN, NEO_KOBJ_ROOT, NEO_KOBJ_DYN0, NEO_KDYN_COUNT=8 };
/* Mission 3: the train layer 10D7B6 as the second terrain object of 01382A. */
enum { NEO_KOBJ_TRAIN=NEO_KOBJ_DYN0+NEO_KDYN_COUNT };
/* An actor's position block (actor +28) as the query's parent (Mission 5: the pilot 0D447C of the metro machine
 * 0D3B36 dies on the machine's block, 04097A's 01495C): its +14/+18/+1C/+20 = the actor's +3C/+40/+44/+48, set per
 * query by neo_gactor.c (neo_kscene_t node). */
enum { NEO_KOBJ_NODE=NEO_KOBJ_TRAIN+1 };
typedef struct {
    int32_t x,y,prev_x,prev_y; /* object +14/+18/+1C/+20, Q16.16 */
} neo_kobj_t;
/* Dynamic collision object (RAM 10C06E + 64*i): a small cell map attached to
 * an actor (ROM 0140C0), rebuilt every update by ROM 01406A/013F82. */
typedef struct {
    const uint8_t *map;        /* +26: dx,dy,w,h (cells), flag, group, 4 bytes, w*h cells */
    neo_kobj_t pos;            /* +14..+20 */
    int32_t bbox[4];           /* +2C..+38: x0,y0,x1,y1 (projected, Q16.16) */
    uint16_t countdown;        /* +2A: updates left before the slot is reused */
    uint32_t chain[3];         /* tags of +10, its +10, ... (01429C exclusion) */
} neo_kdyn_t;
typedef struct {
    neo_kobj_t terrain,root;
    const uint8_t *group_map; /* 8-byte table referenced by RAM 10C00E */
    /* The terrain layer's 64x64 cell ring (layer object +88, read by
     * 0C79FE) when known (the family tests of the vertical stages feed the
     * bytes the original read); NULL: the recovered grid. */
    const uint8_t *ring;
    /* RAM 10C018 is global scratch written by 013C1A/013DB2 and read by later
     * sweeps; it is kept here instead of being reset per call. */
    uint16_t w4018;
    /* RAM 10C068 is written only by the surface response 012EE2 and returned
     * in D4.w by every sweep, so it also persists between calls. */
    uint16_t w4068;
    /* 0138A0 (sweeps with RAM 10C00D set by 0128E8: the players and the
     * vehicles they ride): the screen box RAM 10C01C..10C023 copied from the
     * rider's record (10C2E8: ROM 016DBE x 1C, y 0, w 108, h 1E0), RAM
     * 10C00C (set by 0537FE, cleared at the mission start 0180CC: 0 in the
     * Mission 1 dumps, vertical bound on) and the shake RAM 10E9CC. */
    uint16_t config[4]; uint8_t b400c; uint32_t shake_y;
    neo_kdyn_t dyn[NEO_KDYN_COUNT];
    uint8_t bins_x[18],bins_y[16],solid; /* RAM 10C26E, 10C280, 10C290 */
    /* The actor running a query (A6): its own dynamic object (+6A; stored
     * as index+1, 0 = none) and the tag of its position block (actor +28)
     * are excluded by 01424C. */
    uint8_t self_dyn; uint32_t self_block;
    /* Mission 3 (docs/MISION3.md): 10D7B6 in the static list 013810 with
     * its map 0B8566 (+2E: +C nonzero, +10 = 0x11, +14 = 7; layer +34 = 19)
     * and its column origin +32 (neo_train.c). Off: the list stops at the
     * terrain as in Missions 1 and 2. */
    bool train_on; neo_kobj_t train; int16_t train_origin;
    /* The train's RAM ring (10D7B6 +88) as the original read it (the
     * family tests); NULL: the ROM map through neo_kernel_train_ring. */
    const uint8_t *train_ring;
    neo_kobj_t node;   /* NEO_KOBJ_NODE */
    /* Mission 6 stage 1: 10D7B6 is the bridge (map 0BC6E6, +C = 0: no
     * vertical test in 01384E) with its ring as the loaders left it
     * (neo_m6bridge.h); train_on with train = its position. NULL: the train. */
    const struct neo_m6ring *bridge_ring; const struct neo_m6map *bridge_map;
} neo_kscene_t;
/* Cell of the train map 0B8566 (ROM 0C72EC layout: 784 columns, 48 rows,
 * row 0 at the bottom); 0 outside. */
uint8_t neo_kernel_train_cell(int column,int row);
/* 0C79FE on the train layer's ring: local pixel (x, y), column origin +32. */
uint8_t neo_kernel_train_ring(uint16_t x,uint16_t y,int16_t origin);
typedef struct {
    int32_t x,y,dx,dy;          /* d0..d3 at entry, relative to `parent` */
    uint8_t parent;             /* NEO_KOBJ_* (A0) */
    uint32_t mode4004;
    uint8_t b4008,b4009,b400a,b400b,b400d;
} neo_kreq_t;
typedef struct {
    int32_t x,y,vx,vy;          /* d0..d3 at exit */
    uint8_t parent;             /* A0 at exit */
    uint16_t flags;             /* D6.w = RAM 10C012 */
    uint32_t kind;              /* D7: group<<16 | angle word */
    uint16_t w4068;             /* D4.w */
} neo_kres_t;
/* Returns false only for a context outside the recovered object list. */
bool neo_kernel_sweep(neo_kscene_t *scene,const neo_kreq_t *request,neo_kres_t *result);
/* ROM 013DB2 floor search: d3 = negative search depth (Q16.16). */
typedef struct { int32_t x,y; uint8_t parent; uint32_t kind; int32_t d6; bool found; } neo_kfloor_t;
bool neo_kernel_floor(neo_kscene_t *scene,int32_t x,int32_t y,uint8_t parent,int32_t depth,
    uint8_t b4008,uint8_t b400a,uint8_t b400d,neo_kfloor_t *result);
/* 013968 with player 1's screen box of 0128E8 (ROM 016DBE), for its
 * sweeps 0143EE/014594 (neo_kernel.c). */
bool neo_kernel_player_sweep(neo_kscene_t *scene,const neo_kreq_t *request,neo_kres_t *result);
/* Player 1's air sweep 0143EE/0128E8/013968 from (x,y) with (vx,vy), all
 * relative to the terrain: true on a floor of the terrain (D6 bit 0), with
 * the swept position and velocity (neo_kernel.c). */
typedef struct { int32_t x,y,vx,vy; uint16_t flags; uint32_t kind; } neo_kland_t;
bool neo_kernel_air_landing(neo_kscene_t *scene,int32_t x,int32_t y,int32_t vx,int32_t vy,neo_kland_t *out);
/* ROM 01406A start: clear the dynamic-object bins (before the per-object
 * updates of the same tick). */
void neo_kernel_dyn_begin(neo_kscene_t *scene);
/* ROM 013F82: bounding box and bins of dynamic object i (its position must
 * already be projected for this tick). */
void neo_kernel_dyn_update(neo_kscene_t *scene,unsigned i);
/* ROM 013CF8 at the absolute point (x,y) (A0 = 0): true when a cell of the
 * static terrain there belongs to `group` (012938 high word). A dynamic
 * object whose map is of that group sets *unknown (013CF8 then depends on
 * RAM 10C008 and the bins of the previous query). */
bool neo_kernel_point_group(const neo_kscene_t *scene,int32_t x,int32_t y,uint16_t group,bool *unknown);
/* ROM 012938 classification of the terrain cell at local pixel (x,y). */
uint32_t neo_kernel_cell_class(const neo_kscene_t *scene,uint8_t parent,uint16_t x,uint16_t y,
    uint8_t b4008,uint8_t b400d,uint16_t *flags,uint32_t d7_in);
/* ROM 013CF8: first collision object with a cell of class `cls` under the
 * absolute point; the point relative to it. */
bool neo_kernel_point_class(neo_kscene_t *scene,int32_t x,int32_t y,uint16_t cls,uint8_t b4008,
    int32_t *ox,int32_t *oy,uint8_t *object);
bool neo_kernel_point_class_in(neo_kscene_t *scene,uint8_t parent,int32_t x,int32_t y,uint16_t cls,uint8_t b4008,
    int32_t *ox,int32_t *oy,uint8_t *object);
/* ROM 013D5E: true when a collision object has a solid cell (012938's D7
 * word nonzero) under the point (x,y) of `parent`. */
bool neo_kernel_point_solid(neo_kscene_t *scene,uint8_t parent,int32_t x,int32_t y,uint8_t b4008);
/* ROM 013C1A: surface probe under (x,y) of `parent`; D5 (the surface word
 * before 012E64), D6 (height), D7 and the object (10C060). */
bool neo_kernel_probe(neo_kscene_t *scene,int32_t x,int32_t y,uint8_t parent,uint8_t b4008,uint8_t b400a,uint8_t b400d,
    uint16_t *d5,uint32_t *d6,uint32_t *d7,uint8_t *object);
/* Stair/ledge marker of the object list RAM 10C3A2/10C3F4 (registered by ROM
 * 040110: position relative to the scene root, height, id; queried by
 * 040200/0402E6). */
typedef struct { int32_t x,y; uint8_t height,id; } neo_marker_t;
#endif
