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
enum { NEO_KOBJ_NONE, NEO_KOBJ_TERRAIN, NEO_KOBJ_ROOT };
typedef struct {
    int32_t x,y,prev_x,prev_y; /* object +14/+18/+1C/+20, Q16.16 */
} neo_kobj_t;
typedef struct {
    neo_kobj_t terrain,root;
    const uint8_t *group_map; /* 8-byte table referenced by RAM 10C00E */
    /* RAM 10C018 is global scratch written by 013C1A/013DB2 and read by later
     * sweeps; it is kept here instead of being reset per call. */
    uint16_t w4018;
    /* RAM 10C068 is written only by the surface response 012EE2 and returned
     * in D4.w by every sweep, so it also persists between calls. */
    uint16_t w4068;
} neo_kscene_t;
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
/* ROM 012938 classification of the terrain cell at local pixel (x,y). */
uint32_t neo_kernel_cell_class(const neo_kscene_t *scene,uint8_t parent,uint16_t x,uint16_t y,
    uint8_t b4008,uint8_t b400d,uint16_t *flags,uint32_t d7_in);
#endif
