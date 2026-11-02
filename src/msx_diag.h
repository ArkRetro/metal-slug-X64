#ifndef MSX_DIAG_H
#define MSX_DIAG_H
/* Switchable diagnostic mode (docs/PORT_STATUS.md, "Instrumentación").
 * MSX_DIAG=1 (reference cartridges, mslugx-diag.z64): logic/render time per
 * frame, heap and scene-cache peaks, pool occupancy against capacity with
 * the allocations each pool refused, the explicit stops with their routine,
 * and a ring of the last MSX_DIAG_RING events dumped to the ISViewer log on
 * an assertion, an exception, the first explicit stop of each kind or the
 * pad combo C-left + C-right. MSX_DIAG=0 (mslugx-explorer.z64, host tests):
 * every hook below is an empty inline, nothing is compiled in.
 *
 * The hooks only read the simulation and write their own counters: the
 * game never reads them back (make check runs the gameplay test with
 * MSX_DIAG=1 and requires the same output). */
#include <stdint.h>
#include <stdbool.h>
#ifndef MSX_DIAG
#define MSX_DIAG 0
#endif
typedef enum {
    MSX_POOL_ORDER,    /* world node list (neo_world_t.order, 160) */
    MSX_POOL_SOLDIER,  /* rebel soldiers (12) */
    MSX_POOL_THROW,    /* thrown objects (8) */
    MSX_POOL_SPLASH,   /* death splashes (8) */
    MSX_POOL_GACTOR,   /* generic actors: effects, enemies, bullets... (96) */
    MSX_POOL_BOSS,     /* boss nodes (96) */
    MSX_POOL_DRAW,     /* world draw list (160) */
    MSX_POOL_BOX,      /* generic actor hurtbox registry (48) */
    MSX_POOL_REG,      /* 004C24 slot registry (64) */
    MSX_POOL_SOUND,    /* sound requests per frame (16) */
    MSX_POOL_SHOT,     /* player shot requests per update (8) */
    MSX_POOL_SUB,      /* sub-scripts (4) */
    MSX_POOL_SOLID,    /* solid bodies 106756 (16) */
    MSX_POOL_DYN,      /* kernel dynamic objects (8) */
    MSX_POOL_BULLET,   /* player pistol bullets (16) */
    MSX_POOL_COUNT
} msx_diag_pool_t;
typedef enum {
    MSX_EV_NONE,
    MSX_EV_BOOT,      /* a=0 */
    MSX_EV_STAGE,     /* stage loaded: a=mission, b=stage, c=descriptor */
    MSX_EV_FLOW,      /* mission end state: a=state, b=mission */
    MSX_EV_RESET,     /* game reset / restart: a=cause */
    MSX_EV_SPAWN,     /* script record: a=created, b=camera, c=constructor */
    MSX_EV_SOLDIER,   /* soldiers spawned: b=total, a=alive */
    MSX_EV_KILL,      /* enemies defeated: b=total */
    MSX_EV_DEATH,     /* player death: a=kind, b=deaths */
    MSX_EV_WEAPON,    /* weapon change: a=id, b=rounds */
    MSX_EV_STOP,      /* explicit stop: a=msx_diag_stop_t, b=count, c=routine */
    MSX_EV_POOL,      /* pool refused an allocation: a=pool, b=failures */
    MSX_EV_SLOW,      /* slow frame: b=cpu ms, c=logic us<<16|render us/16 */
    MSX_EV_BOSS,      /* boss: a=phase, b=hp/100 */
    MSX_EV_MOUNT,     /* vehicle: a=1 mount 0 dismount */
    MSX_EV_CHECKPOINT,/* b=checkpoint */
    MSX_EV_COMBO,     /* dump requested from the pad */
    MSX_EV_COUNT
} msx_diag_event_t;
typedef enum {
    MSX_STOP_WORLD,     /* neo_world unsupported routine */
    MSX_STOP_BOSS,      /* boss node */
    MSX_STOP_DEATH,     /* neo_soldier_death_stops */
    MSX_STOP_WEAPON,    /* weapon shot constructor */
    MSX_STOP_GRENADE,   /* bomb type */
    MSX_STOP_MOUNT,     /* mounted player */
    MSX_STOP_VEHICLE,   /* script vehicle */
    MSX_STOP_ITEM,      /* item pickup */
    MSX_STOP_HUD,       /* HUD */
    MSX_STOP_SCRIPT,    /* level script halted (unknown opcode) */
    MSX_STOP_SKIPPED,   /* script record of an unported constructor skipped */
    MSX_STOP_EXTERNAL,  /* spawn of an unported family (not created) */
    MSX_STOP_CAMERA,    /* camera event not supported */
    MSX_STOP_MARCO,     /* Marco program stopped */
    MSX_STOP_SOLID,     /* 004E76 trap */
    MSX_STOP_COUNT
} msx_diag_stop_t;
#if MSX_DIAG
/* Portable core (msx_diag_core.c; also linked into host tests). */
void msx_diag_event(msx_diag_event_t type,unsigned a,unsigned b,uint32_t c);
void msx_diag_pool_fail(msx_diag_pool_t pool);
void msx_diag_pool_sample(msx_diag_pool_t pool,unsigned used);
void msx_diag_stop(msx_diag_stop_t kind,unsigned count,uint32_t routine);
void msx_diag_dump(const char *reason);
void msx_diag_check_failed(const char *file,int line,const char *expr);
void msx_diag_set_frame(uint32_t frame);
/* Cheap consistency checks (pool indices, list links), diagnostic builds only. */
#define MSX_DIAG_CHECK(expr) ((expr)?(void)0:msx_diag_check_failed(__FILE__,__LINE__,#expr))
#define MSX_DIAG_POOL_FAIL(pool) msx_diag_pool_fail(pool)
#else
static inline void msx_diag_event(msx_diag_event_t type,unsigned a,unsigned b,uint32_t c) {(void)type;(void)a;(void)b;(void)c;}
static inline void msx_diag_pool_fail(msx_diag_pool_t pool) {(void)pool;}
static inline void msx_diag_pool_sample(msx_diag_pool_t pool,unsigned used) {(void)pool;(void)used;}
static inline void msx_diag_stop(msx_diag_stop_t kind,unsigned count,uint32_t routine) {(void)kind;(void)count;(void)routine;}
static inline void msx_diag_dump(const char *reason) {(void)reason;}
static inline void msx_diag_set_frame(uint32_t frame) {(void)frame;}
#define MSX_DIAG_CHECK(expr) ((void)0)
#define MSX_DIAG_POOL_FAIL(pool) ((void)0)
#endif
/* Console side (msx_diag.c, N64 only). */
#if MSX_DIAG && defined(N64)
void msx_diag_init(void);
/* Once per frame after the simulation steps: pool occupancy, stops and the
 * game events (compared with the previous frame); pad combos. */
void msx_diag_game(const void *game,uint16_t buttons_c);
/* From msx_perf_frame_end: this frame's simulation, render build and total. */
void msx_diag_frame(uint32_t logic_ticks,uint32_t render_ticks,uint32_t cpu_ticks,uint32_t asset_ticks);
/* Heap sample at a stage change (also every 30 frames). */
void msx_diag_heap(const char *where);
#else
static inline void msx_diag_init(void) {}
static inline void msx_diag_game(const void *game,uint16_t buttons_c) {(void)game;(void)buttons_c;}
static inline void msx_diag_frame(uint32_t logic_ticks,uint32_t render_ticks,uint32_t cpu_ticks,uint32_t asset_ticks) {(void)logic_ticks;(void)render_ticks;(void)cpu_ticks;(void)asset_ticks;}
static inline void msx_diag_heap(const char *where) {(void)where;}
#endif
#endif
