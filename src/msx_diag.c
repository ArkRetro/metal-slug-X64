/* Diagnostic mode, console side (msx_diag.h): frame timing split, heap and
 * scene-cache peaks, pool sampling from the world, stop and event
 * observation, the report lines, the pad combos and the dumps on assertion
 * and exception. Compiled to nothing unless MSX_DIAG. */
#include "msx_diag.h"
#if MSX_DIAG
#include <libdragon.h>
#include <malloc.h>
#include <stdarg.h>
#include "msx_diag_core.h"
#include "gameplay.h"
#include "scene_assets.h"
#include "neo_soldier_ai.h"
_Static_assert(NEO_WORLD_ORDER==160 && NEO_WORLD_SOLDIERS==12 && NEO_WORLD_THROWS==8 && NEO_WORLD_SPLASHES==8 && NEO_WORLD_GACTORS==96 &&
    NEO_BOSS_NODES==96 && NEO_WORLD_DRAWS==160 && NEO_WORLD_BOXES==48 && NEO_WORLD_SOUNDS==16 && NEO_WORLD_SUBSCRIPTS==4 &&
    NEO_WORLD_SOLIDS==16 && NEO_KDYN_COUNT==8 && MSX_BULLETS==8,"msx_diag_pool_capacity (msx_diag_core.c) follows the pools");
enum { WINDOW=300, HEAP_EVERY=30, SLOW_US=25000, SLOWEST=5, STOPS_EVERY=6 };
typedef struct { uint32_t frame,cpu,logic,render,asset;int camera; } slow_t;
static struct {
    /* report window */
    unsigned frames,slow;uint64_t logic_sum,render_sum,cpu_sum;uint32_t logic_max,render_max,cpu_max;
    unsigned windows;
    slow_t slowest[SLOWEST];
    /* heap (KB) and scene cache */
    int heap_used,heap_peak_boot,heap_peak_game,heap_free_min,arena_peak;
    unsigned slots,slots_cap,slots_peak;
    /* game observation */
    bool primed,in_game;int camera;
    uint16_t mission,stage;uint8_t mission_end,phase,weapon,boss_phase;
    unsigned spawned,kills,deaths,mounts,dismounts,boss_hp;int checkpoint;bool boss;
    unsigned stop_seen[MSX_STOP_COUNT];
    /* pad combos and the selected entity (order index, -1 none) */
    uint16_t buttons;int selected;
} d={.heap_free_min=0x7fffffff,.selected=-1};
static void heap_sample(void) {
    heap_stats_t h;sys_get_heap_stats(&h);
    struct mallinfo m=mallinfo();
    int used=h.used/1024,free_kb=(h.total-h.used)/1024,arena=(int)(m.arena/1024);
    d.heap_used=used;
    if (d.in_game) {if (used>d.heap_peak_game) d.heap_peak_game=used;if (free_kb<d.heap_free_min) d.heap_free_min=free_kb;}
    else if (used>d.heap_peak_boot) d.heap_peak_boot=used;
    if (arena>d.arena_peak) d.arena_peak=arena;
}
void msx_diag_heap(const char *where) {(void)where;heap_sample();}
/* ---- fatal paths ------------------------------------------------------------ */
static void fatal_dump(const char *reason) {
    if (msx_diag.fatal) return;
    msx_diag.fatal=true;msx_diag_dump(reason);
}
/* assertf (libdragon and the port): linked with --wrap=debug_assert_func_f. */
void __real_debug_assert_func_f(const char *file,int line,const char *func,const char *failedexpr,const char *msg,...);
void __wrap_debug_assert_func_f(const char *file,int line,const char *func,const char *failedexpr,const char *msg,...) {
    char text[256]="";
    if (msg) {va_list args;va_start(args,msg);vsnprintf(text,sizeof(text),msg,args);va_end(args);}
    fatal_dump("assertion");
    __real_debug_assert_func_f(file,line,func,failedexpr,msg?"%s":NULL,text);
    abort();
}
/* assert(): newlib's __assert_func goes through this pointer (debug_init). */
extern void (*__assert_func_ptr)(const char *file,int line,const char *func,const char *failedexpr);
static void (*assert_next)(const char *file,int line,const char *func,const char *failedexpr);
static void diag_assert(const char *file,int line,const char *func,const char *failedexpr) {
    fatal_dump("assert");
    if (assert_next) assert_next(file,line,func,failedexpr);
    abort();
}
static void diag_exception(exception_t *ex) {
    fatal_dump("exception");
    exception_default_handler(ex);
}
void msx_diag_init(void) {
    register_exception_handler(diag_exception);
    assert_next=__assert_func_ptr;__assert_func_ptr=diag_assert;
    msx_diag_event(MSX_EV_BOOT,0,0,0);
    heap_sample();
    debugf("MSX DIAG on: ring=%u events, report every %u frames, dump: assertion, exception, first stop of each kind, C-left+C-right\n",MSX_DIAG_RING,WINDOW);
}
/* ---- per-frame game observation --------------------------------------------- */
static unsigned count_active_soldiers(const neo_world_t *w) {unsigned n=0;for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++) n+=w->soldiers[i].active;return n;}
static void sample_pools(const msx_game_t *g) {
    const neo_world_t *w=&g->world;unsigned n;
    msx_diag_pool_sample(MSX_POOL_ORDER,w->order_count);
    msx_diag_pool_sample(MSX_POOL_SOLDIER,count_active_soldiers(w));
    n=0;for (unsigned i=0;i<NEO_WORLD_THROWS;i++) n+=w->throws[i].active;msx_diag_pool_sample(MSX_POOL_THROW,n);
    n=0;for (unsigned i=0;i<NEO_WORLD_SPLASHES;i++) n+=w->splashes[i].active;msx_diag_pool_sample(MSX_POOL_SPLASH,n);
    n=0;for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) n+=w->gactors[i].active;msx_diag_pool_sample(MSX_POOL_GACTOR,n);
    n=0;if (w->boss_active) for (unsigned i=0;i<NEO_BOSS_NODES;i++) n+=w->boss.nodes[i].routine!=0;msx_diag_pool_sample(MSX_POOL_BOSS,n);
    msx_diag_pool_sample(MSX_POOL_DRAW,w->draw_count);
    msx_diag_pool_sample(MSX_POOL_BOX,w->box_count>w->last_box_count?w->box_count:w->last_box_count);
    msx_diag_pool_sample(MSX_POOL_REG,w->reg_count>w->last_reg_count?w->reg_count:w->last_reg_count);
    msx_diag_pool_sample(MSX_POOL_SOUND,w->sound_count);
    msx_diag_pool_sample(MSX_POOL_SHOT,w->shot_request_count);
    n=0;for (unsigned i=0;i<NEO_WORLD_SUBSCRIPTS;i++) n+=w->subs[i].used;msx_diag_pool_sample(MSX_POOL_SUB,n);
    n=0;for (unsigned i=0;i<NEO_WORLD_SOLIDS;i++) n+=w->solids[i].actor>=0;msx_diag_pool_sample(MSX_POOL_SOLID,n);
    n=0;for (unsigned i=0;i<NEO_KDYN_COUNT;i++) n+=w->dyn[i].used;msx_diag_pool_sample(MSX_POOL_DYN,n);
    n=0;for (unsigned i=0;i<MSX_BULLETS;i++) n+=g->bullets[i].active;msx_diag_pool_sample(MSX_POOL_BULLET,n);
}
static void observe_stops(const msx_game_t *g) {
    const neo_world_t *w=&g->world;
    unsigned world_only=w->unsupported-w->boss_unsupported;
    msx_diag_stop(MSX_STOP_WORLD,world_only,world_only?w->unsupported_routine:0);
    msx_diag_stop(MSX_STOP_BOSS,w->boss_unsupported,w->boss_unsupported_routine);
    msx_diag_stop(MSX_STOP_DEATH,neo_soldier_death_stops,neo_soldier_death_stop_kinds);
    msx_diag_stop(MSX_STOP_WEAPON,g->weapon_stops,g->weapon_stop_constructor);
    msx_diag_stop(MSX_STOP_GRENADE,g->grenade_stops,g->grenade_stops?w->weapons.b[10]:0);
    msx_diag_stop(MSX_STOP_MOUNT,g->mount_stops,g->mount_stop);
    msx_diag_stop(MSX_STOP_VEHICLE,w->vehicle_stops,w->vehicle_stop_constructor);
    msx_diag_stop(MSX_STOP_ITEM,w->item_pickups,w->item_pickup_routine);
    msx_diag_stop(MSX_STOP_HUD,g->hud.unsupported,0);
    msx_diag_stop(MSX_STOP_SCRIPT,w->script.halted,w->script.halted?w->script.halt_address:0);
    msx_diag_stop(MSX_STOP_SKIPPED,w->script.skipped,w->script.skipped_address);
    msx_diag_stop(MSX_STOP_EXTERNAL,w->external_spawns,w->external_constructor);
    msx_diag_stop(MSX_STOP_CAMERA,w->camera.unsupported+w->camera_events_unsupported,w->camera.unsupported_at);
    msx_diag_stop(MSX_STOP_SOLID,w->solid_unsupported,0x4ec0);
}
static void observe_events(const msx_game_t *g) {
    const neo_world_t *w=&g->world;
    if (!d.primed) {
        d.primed=true;d.mission=g->flow.mission;d.stage=g->flow.stage;d.mission_end=g->mission_end;d.phase=g->life.phase;
        d.weapon=marco_weapons_id(&w->weapons,0);d.spawned=w->spawned;d.kills=g->kills;d.deaths=g->deaths;
        d.mounts=g->mounts;d.dismounts=g->dismounts;d.checkpoint=g->checkpoint;
        return;
    }
    if (g->flow.mission!=d.mission || g->flow.stage!=d.stage) {
        msx_diag_event(MSX_EV_STAGE,g->flow.mission+1u,g->flow.stage,g->flow.descriptor);
        d.mission=g->flow.mission;d.stage=g->flow.stage;heap_sample();
    }
    if (g->mission_end!=d.mission_end) {msx_diag_event(MSX_EV_FLOW,g->mission_end,g->flow.mission+1u,(uint32_t)g->camera);d.mission_end=g->mission_end;}
    for (unsigned i=0;i<w->spawn_log_count;i++)
        msx_diag_event(MSX_EV_SPAWN,w->spawn_log[i].created,neo_camera_progress(&w->camera),w->spawn_log[i].constructor);
    if (w->spawned!=d.spawned) {msx_diag_event(MSX_EV_SOLDIER,count_active_soldiers(w),w->spawned,(uint32_t)g->camera);d.spawned=w->spawned;}
    if (g->kills!=d.kills) {msx_diag_event(MSX_EV_KILL,0,g->kills,(uint32_t)g->camera);d.kills=g->kills;}
    if (g->life.phase!=d.phase) {
        if (g->life.phase==MARCO_DYING) msx_diag_event(MSX_EV_DEATH,w->hit_kind,g->deaths,(uint32_t)g->camera);
        d.phase=g->life.phase;
    }
    if (g->deaths!=d.deaths) d.deaths=g->deaths;
    uint8_t weapon=marco_weapons_id(&w->weapons,0);
    if (weapon!=d.weapon) {msx_diag_event(MSX_EV_WEAPON,weapon,marco_weapons_rounds(&w->weapons,0),(uint32_t)g->camera);d.weapon=weapon;}
    if (g->mounts!=d.mounts) {msx_diag_event(MSX_EV_MOUNT,1,g->mounts,w->vslots.slot[0].b[0x21]);d.mounts=g->mounts;}
    if (g->dismounts!=d.dismounts) {msx_diag_event(MSX_EV_MOUNT,0,g->dismounts,w->vslots.slot[0].b[0x1d]);d.dismounts=g->dismounts;}
    if (g->checkpoint!=d.checkpoint) {msx_diag_event(MSX_EV_CHECKPOINT,0,(unsigned)g->checkpoint,(uint32_t)g->camera);d.checkpoint=g->checkpoint;}
    if (w->boss_active!=d.boss) {d.boss=w->boss_active;msx_diag_event(MSX_EV_BOSS,d.boss?0xff:0,0,(uint32_t)g->camera);}
    if (w->boss_active) {
        uint8_t phase=neo_boss_phase(&w->boss);unsigned hp=neo_boss_health(&w->boss);
        if (phase!=d.boss_phase || hp/1000!=d.boss_hp/1000) {msx_diag_event(MSX_EV_BOSS,phase,hp/100,w->boss_hits);d.boss_phase=phase;d.boss_hp=hp;}
    }
}
static void print_slowest(void);
static void print_selected(const msx_game_t *g) {
    const neo_world_t *w=&g->world;
    if (d.selected<0 || (unsigned)d.selected>=w->order_count) {d.selected=-1;return;}
    neo_world_slot_t s=w->order[d.selected];
    if (s.kind==NEO_WORLD_GACTOR) {
        const neo_gactor_t *a=&w->gactors[s.index];
        debugf("MSX DIAG entity #%d gactor %u routine=%06lx x=%d y=%d +5F=%02x +68=%02x +69=%02x creator=%08lx\n",d.selected,s.index,(unsigned long)ga_l(a,0),
            (int16_t)ga_w(a,0x3c),(int16_t)ga_w(a,0x40),ga_b(a,0x5f),ga_b(a,0x68),ga_b(a,0x69),(unsigned long)ga_l(a,0x0c));
    } else if (s.kind==NEO_WORLD_SOLDIER) {
        const neo_soldier_t *so=&w->soldiers[s.index];
        debugf("MSX DIAG entity #%d soldier %u routine=%06lx x=%d y=%d health=%u\n",d.selected,s.index,(unsigned long)so->routine,
            (int16_t)(so->b.projected_x>>16),(int16_t)(so->b.projected_y>>16),so->health);
    } else debugf("MSX DIAG entity #%d kind=%u index=%u depth=%u\n",d.selected,s.kind,s.index,s.depth);
}
void msx_diag_game(const void *game,uint16_t buttons) {
    const msx_game_t *g=game;
    if (!d.in_game) {d.in_game=true;heap_sample();}
    d.camera=g->camera;
    sample_pools(g);
    observe_stops(g);
    observe_events(g);
    /* Pad (diagnostic builds only; the C buttons are unused by the game):
     * C-left+C-right dumps the ring, C-up+C-down selects the next entity. */
    uint16_t pressed=buttons&~d.buttons;d.buttons=buttons;
    if ((buttons&3u)==3u && (pressed&3u)) {msx_diag_event(MSX_EV_COMBO,0,0,0);msx_diag_dump("pad");print_slowest();}
    if ((buttons&12u)==12u && (pressed&12u)) {
        d.selected=g->world.order_count?(d.selected+1)%(int)g->world.order_count:-1;
        print_selected(g);
    }
}
/* ---- per-frame timing and the report ---------------------------------------- */
static void slowest_insert(uint32_t cpu,uint32_t logic,uint32_t render,uint32_t asset) {
    unsigned at=SLOWEST;
    while (at && d.slowest[at-1].cpu<cpu) at--;
    if (at==SLOWEST) return;
    for (unsigned i=SLOWEST-1;i>at;i--) d.slowest[i]=d.slowest[i-1];
    d.slowest[at]=(slow_t){msx_diag.frame,cpu,logic,render,asset,d.camera};
}
static void print_slowest(void) {
    char line[320];int n=snprintf(line,sizeof(line),"MSX DIAG slowest (frame:cpu/logic/render/asset us@camera):");
    for (unsigned i=0;i<SLOWEST && d.slowest[i].cpu && n<(int)sizeof(line);i++) n+=snprintf(line+n,sizeof(line)-n," %lu:%lu/%lu/%lu/%lu@%d",
        (unsigned long)d.slowest[i].frame,(unsigned long)d.slowest[i].cpu,(unsigned long)d.slowest[i].logic,(unsigned long)d.slowest[i].render,(unsigned long)d.slowest[i].asset,d.slowest[i].camera);
    debugf("%s\n",line);
}
void msx_diag_frame(uint32_t logic_ticks,uint32_t render_ticks,uint32_t cpu_ticks,uint32_t asset_ticks) {
    uint32_t logic=TICKS_TO_US(logic_ticks),render=TICKS_TO_US(render_ticks),cpu=TICKS_TO_US(cpu_ticks),asset=TICKS_TO_US(asset_ticks);
    msx_diag_set_frame(msx_diag.frame+1u);
    d.frames++;d.logic_sum+=logic;d.render_sum+=render;d.cpu_sum+=cpu;
    if (logic>d.logic_max) d.logic_max=logic;
    if (render>d.render_max) d.render_max=render;
    if (cpu>d.cpu_max) d.cpu_max=cpu;
    if (d.in_game) {
        slowest_insert(cpu,logic,render,asset);
        if (cpu>=SLOW_US) {d.slow++;msx_diag_event(MSX_EV_SLOW,0,cpu/1000u,(logic/16u>0xffffu?0xffffu:logic/16u)<<16|(render/16u>0xffffu?0xffffu:render/16u));}
    }
    if (!(msx_diag.frame%HEAP_EVERY)) {
        heap_sample();
        unsigned resident,capacity;scene_assets_slots(&resident,&capacity);
        d.slots=resident;d.slots_cap=capacity;if (resident>d.slots_peak) d.slots_peak=resident;
    }
    if (d.frames<WINDOW) return;
    /* One report per 300 frames (~5 s): timing split, memory, pools; stops
     * and the slowest frames every 30 s. */
    debugf("MSX DIAG frame=%lu logic=%lu/%lu render=%lu/%lu cpu=%lu/%lu us slow=%u heap used=%d peak=%d boot_peak=%d free_min=%d arena=%d KB slots=%u/%u peak=%u ring=%lu\n",
        (unsigned long)msx_diag.frame,(unsigned long)(d.logic_sum/d.frames),(unsigned long)d.logic_max,(unsigned long)(d.render_sum/d.frames),(unsigned long)d.render_max,
        (unsigned long)(d.cpu_sum/d.frames),(unsigned long)d.cpu_max,d.slow,d.heap_used,d.heap_peak_game,d.heap_peak_boot,d.heap_free_min==0x7fffffff?-1:d.heap_free_min,d.arena_peak,
        d.slots,d.slots_cap,d.slots_peak,(unsigned long)msx_diag.ring.total);
    msx_diag_dump_pools();
    if (!(++d.windows%STOPS_EVERY)) {msx_diag_dump_stops();print_slowest();}
    for (unsigned i=0;i<MSX_POOL_COUNT;i++) {msx_diag.pool[i].peak_window=0;msx_diag.pool[i].fails_window=0;}
    d.frames=d.slow=0;d.logic_sum=d.render_sum=d.cpu_sum=0;d.logic_max=d.render_max=d.cpu_max=0;
}
#else
typedef int msx_diag_unused_t;
#endif
