#define MSX_SOLDIER_PROGRAMS 1
#include "neo_world.h"
#include <string.h>
#include "neo_throw.h"
#include "neo_hit.h"
#include "neo_collision.h"
#include "neo_region.h"
#include "soldier_ai_tables.h"
/* See neo_world.h. Every routine executed here is one of the translated and
 * fixture-verified pieces; this file only orders them as the original actor
 * list does and connects their inputs/outputs. */
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
enum { TERRAIN_TAG=0x10c72e, ROOT_TAG=0x10e968, SCENE_Y=0x01000000 };
enum { SOLDIER_CONSTRUCTOR=0xaf17e, SCRIPT_TAG=0x101414 };
#define RUNNER_TAG(i) (0x50000000u|(uint32_t)(i))
/* Observed settings of every capture: RAM 100088 = 2, 10FD8B = 3, one player.
 * RAM 10FD8F (BLOOD) is w->blood: 040F98 asks 01B2BE for the splash palette
 * (src/neo_blood.c: 037A ON, 03BA OFF). */
enum { MODE88=2, RANK=3, SPLASH_PROGRAM=0x2402fa };
static uint16_t HI(int32_t v) {return (uint16_t)((uint32_t)v>>16);}
/* The programs by tag: a binary search over an index sorted once (the
 * first entry of a tag, as the linear scan it replaces). */
static uint16_t program_order[COUNT(soldier_programs)];
static bool programs_sorted;
static const neo_anim_program_t *program(uint32_t tag) {
    enum {N=COUNT(soldier_programs)};
    if (!programs_sorted) {
        _Static_assert(N<=65535,"program index is 16 bits");
        for (unsigned i=0;i<N;i++) {      /* stable insertion: equal tags keep their order */
            unsigned j=i;
            while (j && soldier_programs[program_order[j-1]].tag>soldier_programs[i].tag) {program_order[j]=program_order[j-1];j--;}
            program_order[j]=(uint16_t)i;
        }
        programs_sorted=true;
    }
    unsigned lo=0,hi=N;                                /* first tag >= tag */
    while (lo<hi) {unsigned m=(lo+hi)/2;if (soldier_programs[program_order[m]].tag<tag) lo=m+1; else hi=m;}
    return lo<N && soldier_programs[program_order[lo]].tag==tag?&soldier_programs[program_order[lo]].program:NULL;
}
static uint8_t kobj(uint32_t tag) {return tag==TERRAIN_TAG?NEO_KOBJ_TERRAIN:tag==ROOT_TAG?NEO_KOBJ_ROOT:NEO_KOBJ_NONE;}
static uint32_t ktag(uint8_t obj) {return obj==NEO_KOBJ_TERRAIN?TERRAIN_TAG:obj==NEO_KOBJ_ROOT?ROOT_TAG:0;}
/* ---- per-update context ---------------------------------------------------- */
typedef struct {
    neo_world_t *w;
    neo_hurtbox_t player;bool player_valid,player_hittable;
    neo_control_frame_t players;
    neo_cull_bounds_t bounds;
    neo_soldier_env_t env;
    unsigned hits;
    int current;                 /* order index of the running actor */
    uint16_t draw_palette;bool draw_flash;uint8_t draw_kind;
    neo_genv_t genv;
    neo_m2boss_env_t m2boss;     /* the Mission 2 boss's RAM (neo_m2boss.c) */
    const neo_script_env_t *se;  /* the script environment of this tick */
    int sub_owner;               /* generic actor running a sub-script (-1) */
} tick_t;
static void emit(void *context,const neo_anim_draw_t *d) {
    tick_t *t=context;neo_world_t *w=t->w;
    if (w->draw_count==NEO_WORLD_DRAWS) return;
    w->draws[w->draw_count++]=(neo_world_draw_t){d->descriptor,d->x,d->y,d->attributes,t->draw_palette,t->draw_flash,t->draw_kind,0xff,0xff,d->z};
}
static bool insert(neo_world_t *w,unsigned at,neo_world_slot_t slot) {
    if (w->order_count==NEO_WORLD_ORDER) return false;
    memmove(&w->order[at+1],&w->order[at],(w->order_count-at)*sizeof(slot));
    w->order[at]=slot;w->order_count++;return true;
}
static void remove_at(neo_world_t *w,unsigned at) {
    memmove(&w->order[at],&w->order[at+1],(w->order_count-at-1)*sizeof(w->order[0]));w->order_count--;
}
static int find(const neo_world_t *w,uint8_t kind,unsigned index) {
    for (unsigned i=0;i<w->order_count;i++) if (w->order[i].kind==kind && w->order[i].index==index) return (int)i;
    return -1;
}
/* ---- script callbacks ------------------------------------------------------ */
static uint16_t script_rng(void *context) {tick_t *t=context;return neo_rng_next(&t->w->rng);}
static void script_runner(void *context,int creator,unsigned index) {
    tick_t *t=context;neo_world_t *w=t->w;
    int at=creator<0?find(w,NEO_WORLD_SCRIPT,0):find(w,NEO_WORLD_RUNNER,(unsigned)creator);
    if (at<0 || !insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_RUNNER,(uint8_t)index,(uint8_t)(w->order[at].depth+1u)})) w->overflow++;
}
static void script_camera_lock(void *context,uint16_t x,const uint16_t bounds[4]) {
    tick_t *t=context;t->w->camera_lock=(int32_t)x;
    if (t->w->camera_native) neo_camera_lock(&t->w->camera,x,bounds);
}
static void script_camera_release(void *context) {
    tick_t *t=context;t->w->camera_lock=-1;
    if (t->w->camera_native) neo_camera_release(&t->w->camera);
}
static void script_autoscroll(void *context,bool start) {
    tick_t *t=context;
    if (!t->w->camera_native) return;
    if (start) neo_camera_autoscroll(&t->w->camera); else neo_camera_autoscroll_stop(&t->w->camera);
}
static neo_gactor_t *ga_insert(tick_t *t,int at,uint32_t routine,uint32_t creator_tag,uint8_t v68);
enum { BOSS_CONSTRUCTOR=0xd5976 };
static void boss_spawn(tick_t *t,int at,const neo_script_spawn_t *sp);
/* The spawn-table constructors script_spawn runs natively (the rest are
 * counted in external_spawns and never created). Items 08C374/08C37C are
 * native only past the region (ga_insert). */
/* Spawn-table constructors the world stops explicitly (counted in
 * vehicle_stops, logged): none left (the Camel Slug 03458E and the SV-001
 * 092A32 are native). */
bool neo_world_spawn_stop(uint32_t constructor) {
    (void)constructor;
    return false;
}
bool neo_world_spawn_native(uint32_t constructor) {
    uint32_t r=constructor&0xffffffu;
    if (neo_world_spawn_stop(r)) return false;
    return neo_region_routine(r) || r==BOSS_CONSTRUCTOR || r==0x5486au || r==SOLDIER_CONSTRUCTOR || r==0x4115au;
}
static void script_spawn(void *context,const neo_script_spawn_t *sp) {
    tick_t *t=context;neo_world_t *w=t->w;
    /* A sub-script actor (generic actor 019E82) creates after itself. */
    bool sub=sp->creator<0 && t->sub_owner>=0;
    int at=sub?find(w,NEO_WORLD_GACTOR,(unsigned)t->sub_owner):sp->creator<0?find(w,NEO_WORLD_SCRIPT,0):find(w,NEO_WORLD_RUNNER,(unsigned)sp->creator);
    uint32_t routine=sp->constructor&0xffffffu;
    if (routine!=SOLDIER_CONSTRUCTOR && at>=0 && neo_region_routine(routine) && !neo_world_spawn_stop(routine)) {
        /* 01A10A: 0040BC after the creator, words to +6E.., +69 group,
         * +68 variant, +28/+2C at the scene root (01ABB6). */
        neo_gactor_t *a=ga_insert(t,at,routine,sub?NEO_GA_TAG((unsigned)t->sub_owner):sp->creator<0?SCRIPT_TAG:RUNNER_TAG(sp->creator),0);
        if (!a) return;
        for (unsigned k=0;k<sp->word_count;k++) ga_sw(a,0x6eu + 2u*k,sp->words[k]);
        ga_sb(a,0x69,sp->group69);ga_sb(a,0x68,sp->variant68);
        ga_sl(a,0x28,(uint32_t)sp->x);ga_sl(a,0x2c,(uint32_t)sp->y);ga_sl(a,0x38,ROOT_TAG);ga_sw(a,0x4c,0);
        ga_project(a,&t->genv);
        return;
    }
    if (routine==BOSS_CONSTRUCTOR) {boss_spawn(t,at,sp);return;}
    if (routine==0x5486au) {
        /* Spawn-table entry 05486A: RAM 10C44E = FF, 10C44F = its group;
         * the actor then leaves (004188 after the cartridge check). */
        w->ga_flags.dome_signal=0xff;w->ga_flags.dome_group=sp->group69;return;
    }
    if (routine==0x1913cu) {
        /* Spawn-table entry 01913C (uncounted): the mission timer 1013DE back
         * to its start value 003586; the actor then leaves (004188 after the
         * cartridge check). The player's HUD applies it (neo_hud.c). */
        w->timer_resets++;return;
    }
    if (routine!=SOLDIER_CONSTRUCTOR && routine!=0x4115au) {w->external_spawns++;w->external_constructor=sp->constructor;return;}
    unsigned i;for (i=0;i<NEO_WORLD_SOLDIERS && w->soldiers[i].active;i++) {}
    if (i==NEO_WORLD_SOLDIERS || at<0 || !insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_SOLDIER,(uint8_t)i,(uint8_t)(w->order[at].depth+1u)})) {w->overflow++;return;}
    neo_soldier_spawn_t *p=&w->pending[i];memset(p,0,sizeof(*p));
    p->x=sp->x;p->y=sp->y;p->variant68=sp->variant68;p->group69=sp->group69;p->constructor=routine==SOLDIER_CONSTRUCTOR?0:routine;
    for (unsigned k=0;k<6 && k<sp->word_count;k++) p->words[k]=sp->words[k];
    /* The node exists from now on; its constructor runs when the list
     * traversal reaches it (after the creator has finished its update). */
    memset(&w->soldiers[i],0,sizeof(w->soldiers[i]));w->soldiers[i].active=true;w->constructed[i]=false;
    w->spawned++;
}
static void sound_request(neo_world_t *w,uint16_t word) {
    if (w->sound_count<NEO_WORLD_SOUNDS) w->sounds[w->sound_count++]=word;else w->sound_dropped++;
}
/* ---- soldier events -------------------------------------------------------- */
static void place(neo_contact_actor_t *a,const neo_contact_actor_t *source,int dx,int dy,
    const neo_contact_parent_t *scene,uint8_t epoch) {
    /* ROM 01B220 -> 01ACF2/01ABCC: projected/cached position plus offset,
     * relative to the scene root, then projection with a fresh history. */
    neo_contact_actor_t seed=*source;
    seed.projected_x=(int32_t)((uint32_t)seed.projected_x+((uint32_t)dx<<16));
    seed.projected_y=(int32_t)((uint32_t)seed.projected_y+((uint32_t)dy<<16));
    seed.cached_x=(int32_t)((uint32_t)seed.cached_x+((uint32_t)dx<<16));
    seed.cached_y=(int32_t)((uint32_t)seed.cached_y+((uint32_t)dy<<16));
    /* neo_throw_spawn applies 01ACF2 with the (0,32) offset of 03BB8E; undo
     * that fixed offset so the same verified placement serves both callers. */
    seed.projected_y=(int32_t)((uint32_t)seed.projected_y-0x200000u);
    seed.cached_y=(int32_t)((uint32_t)seed.cached_y-0x200000u);
    neo_throw_spawn(a,&seed,scene,epoch);
}
/* 004C24 on the player (+5A/+5B): the highest kind of the update wins; +5B
 * bit 7 when the player's projected x is left of the attacker's +44. */
static void player_hit(tick_t *t,int32_t projected_x,int32_t cached_x,uint8_t kind) {
    neo_world_t *w=t->w;
    if (!t->hits || kind>w->hit_kind) {w->hit_kind=kind;w->hit_right=(int16_t)(uint16_t)(t->player.x-(uint16_t)HI(cached_x))<0;}
    t->hits++;w->hit_x=HI(projected_x);
}
static void soldier_event(void *context,const neo_soldier_event_t *e) {
    tick_t *t=context;neo_world_t *w=t->w;
    switch (e->kind) {
    case NEO_SOLDIER_EVENT_SOUND:sound_request(w,e->value);break;
    case NEO_SOLDIER_EVENT_KNIFE_HIT:
        /* The soldier's 0049E8 row (value: its kind +4). */
        if (t->current>=0 && w->order[t->current].kind==NEO_WORLD_SOLDIER) {
            const neo_soldier_t *so=&w->soldiers[w->order[t->current].index];
            player_hit(t,so->b.projected_x,so->b.cached_x,(uint8_t)e->value);
        } else t->hits++;
        break;
    case NEO_SOLDIER_EVENT_THROW: { /* 03BB8E: 00411C inserts after the script actor */
        unsigned i;for (i=0;i<NEO_WORLD_THROWS && w->throws[i].active;i++) {}
        int at=find(w,NEO_WORLD_SCRIPT,0);
        if (i==NEO_WORLD_THROWS || at<0 || !insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_THROW,(uint8_t)i,1})) {w->overflow++;break;}
        /* neo_throw_spawn applies the (0,32) offset of 03BB8E; 03BBBC uses 24. */
        neo_contact_actor_t seed=e->actor;int dy=(int)(e->value>>8)-0x20;
        seed.projected_y=(int32_t)((uint32_t)seed.projected_y+((uint32_t)dy<<16));
        seed.cached_y=(int32_t)((uint32_t)seed.cached_y+((uint32_t)dy<<16));
        neo_contact_actor_t body;neo_throw_spawn(&body,&seed,&w->root,w->epoch);
        neo_anim_render_t graphics;memset(&graphics,0,sizeof(graphics));graphics.attributes=body.flags15;
        neo_throw_actor_start(&w->throws[i],&body,(uint16_t)(e->value&0xffu),0xff,&graphics);
        break;
    }
    case NEO_SOLDIER_EVENT_DEATH_CHILD: { /* 040ED0: immediate child 040F88 */
        unsigned i;for (i=0;i<NEO_WORLD_SPLASHES && w->splashes[i].active;i++) {}
        if (i==NEO_WORLD_SPLASHES || !insert(w,(unsigned)t->current+1,(neo_world_slot_t){NEO_WORLD_SPLASH,(uint8_t)i,(uint8_t)(w->order[t->current].depth+1u)})) {w->overflow++;break;}
        neo_world_splash_t *s=&w->splashes[i];memset(s,0,sizeof(*s));
        /* 01B242 copies flip bits 0..2; 01B220 negates the offset per flip. */
        uint8_t flags=e->actor.flags15&7u;
        place(&s->body,&e->actor,(flags&1u)?-8:8,0,&w->root,w->epoch);
        s->body.flags15=flags;s->body.field66=e->actor.field66;s->body.word1e=e->actor.word1e;
        s->active=true;
        break;
    }
    default:break;
    }
}
/* ---- thrown object services ------------------------------------------------ */
static const uint16_t throw_palette_ids[]={0,0x35d,0x364,0x365,0x36c};
static bool throw_palette(void *context,uint16_t asset,bool replace,uint8_t *handle) {
    (void)context;(void)replace;
    for (uint8_t i=1;i<COUNT(throw_palette_ids);i++) if (throw_palette_ids[i]==asset) {*handle=i;return true;}
    return false;
}
static void throw_release(void *context) {(void)context;}
/* The throw services' context is the world: neo_throw_actor keeps it as the
 * owner of the palettes it took and checks it on every later update, so it
 * must not be the address of a per-update (stack) tick_t. The tick of the
 * running update is reached through throw_tick_ctx (set by run_throw). */
static tick_t *throw_tick_ctx;
static bool throw_air(void *context,neo_contact_actor_t *b,const neo_throw_frame_t *f) {
    neo_world_t *w=context;(void)f;
    /* 03BD3A acceleration, then 0143E4: air request, 013968 sweep, commit. */
    neo_throw_accelerate(b);
    neo_contact_request_t q=neo_contact_prepare_air(b);
    neo_kreq_t kq={q.x,q.y,q.dx,q.dy,kobj(q.parent_tag),0,q.mode8,0,0,0,0};
    neo_kres_t k;if (!kq.parent || !neo_kernel_sweep(&w->kernel,&kq,&k)) return false;
    neo_contact_result_t r={k.x,k.y,k.vx,k.vy,ktag(k.parent),k.kind,k.flags};
    neo_contact_commit(b,&r,r.parent_tag==TERRAIN_TAG?&w->terrain:&w->root,w->epoch);
    return true;
}
static bool throw_hit(void *context,const neo_contact_actor_t *b) {
    tick_t *t=throw_tick_ctx;(void)context;
    if (!t->player_hittable) return false;
    int first=-1;
    uint8_t type=neo_hit_query(b,soldier_throw_attack,&t->player,1,(uint8_t)soldier_throw_attack[4],(uint8_t)soldier_throw_attack[5],&first);
    if (type && first>=0 && soldier_throw_attack[6]) player_hit(t,b->projected_x,b->cached_x,(uint8_t)soldier_throw_attack[4]);
    return type!=0;
}
/* ---- generic actors (neo_gactor.h) ------------------------------------------- */
static int order_of_gactor(const neo_world_t *w,const neo_gactor_t *a) {return find(w,NEO_WORLD_GACTOR,(unsigned)a->index);}
/* Free node as ROM 004126 left it (003F24, 01AB60, 0164B8, 009EA0 clear the
 * fields below; the others keep the previous occupant's values, zero here),
 * then 0040BC: +0C creator, +10 depth, +68 from the creator. */
static neo_gactor_t *ga_alloc(neo_world_t *w,uint32_t routine,uint32_t creator_tag,uint8_t v68,uint8_t depth) {
    unsigned i;for (i=0;i<NEO_WORLD_GACTORS && w->gactors[i].active;i++) {}
    if (i==NEO_WORLD_GACTORS) {w->overflow++;return NULL;}
    neo_gactor_t *a=&w->gactors[i];memset(a,0,sizeof(*a));a->index=(int16_t)i;a->active=true;
    ga_sl(a,0,routine);ga_sl(a,0x0c,creator_tag);ga_sw(a,0x10,depth);
    ga_sb(a,0x12,0xff);ga_sb(a,0x13,0xff);ga_sw(a,0x1e,0xffff);ga_sb(a,0x17,0xff);ga_sw(a,0x58,0xffff);
    ga_sb(a,0x69,0xff);ga_sb(a,0x6a,0xff);ga_sb(a,0x6b,0xff);ga_sb(a,0x68,v68);
    memset(a->written,0,sizeof(a->written));
    return a;
}
bool neo_world_parent(const neo_world_t *w,uint32_t tag,neo_contact_parent_t *out) {
    if (tag==TERRAIN_TAG) {*out=w->terrain;return true;}
    if (tag==ROOT_TAG) {*out=w->root;return true;}
    if (tag>=NEO_DYN_TAG(0) && tag<NEO_DYN_TAG(NEO_KDYN_COUNT)) {*out=w->dyn_parent[(tag-NEO_DYN_TAG(0))/64u];return true;}
    if ((tag&0xff000000u)==NEO_GA_TAG_BASE && (tag&0xffffu)<NEO_WORLD_GACTORS) {
        const neo_gactor_t *a=&w->gactors[tag&0xffffu];
        *out=(neo_contact_parent_t){tag,(int32_t)ga_l(a,0x3c),(int32_t)ga_l(a,0x40),(int32_t)ga_l(a,0x44),(int32_t)ga_l(a,0x48),ga_b(a,0x4d)};
        return true;
    }
    if (NEO_BOSS_IS_TAG(tag) && NEO_BOSS_TAG_INDEX(tag)<NEO_BOSS_NODES) {
        const neo_boss_node_t *n=&w->boss.nodes[NEO_BOSS_TAG_INDEX(tag)];
        *out=(neo_contact_parent_t){tag,(int32_t)neo_boss_l(n,0x3c),(int32_t)neo_boss_l(n,0x40),(int32_t)neo_boss_l(n,0x44),(int32_t)neo_boss_l(n,0x48),neo_boss_b(n,0x4d)};
        return true;
    }
    /* Layer objects 10D7B6/10E8C6 scroll with the foreground once the middle
     * bank runs at full speed (camera >= 1023, docs/MISSION1_REST.md). */
    if (tag==0x10d7b6u || tag==0x10e8c6u) {*out=w->terrain;out->tag=tag;return true;}
    return false;
}
static bool g_parent(void *ctx,uint32_t tag,neo_contact_parent_t *out) {return neo_world_parent(((tick_t *)ctx)->w,tag,out);}
static const neo_anim_program_t *g_program(uint32_t tag) {return program(tag);}
const neo_anim_program_t *neo_world_program(uint32_t tag) {return program(tag);}
static neo_gactor_t *ga_insert(tick_t *t,int at,uint32_t routine,uint32_t creator_tag,uint8_t v68) {
    neo_world_t *w=t->w;
    if (at<0) {w->overflow++;return NULL;}
    /* A family not ported yet: the actor is not created (counted). */
    if (!neo_region_routine(routine)) {w->external_spawns++;w->external_constructor=routine;return NULL;}
    uint8_t depth=(uint8_t)(w->order[at].depth+1u);
    neo_gactor_t *a=ga_alloc(w,routine,creator_tag,v68,depth);
    if (!a) return NULL;
    if (!insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_GACTOR,(uint8_t)a->index,depth})) {a->active=false;w->overflow++;return NULL;}
    return a;
}
static neo_gactor_t *g_spawn(void *ctx,neo_gactor_t *creator,uint32_t routine,bool after_script) {
    tick_t *t=ctx;neo_world_t *w=t->w;
    int at=after_script?find(w,NEO_WORLD_SCRIPT,0):(creator?order_of_gactor(w,creator):t->current);
    /* 0040BC copies +68 from its A6: the creator, or the script actor for
     * 00411C (+68 = 0). */
    return ga_insert(t,at,routine,after_script?SCRIPT_TAG:(creator?NEO_GA_TAG(creator->index):0),!after_script && creator?ga_b(creator,0x68):0);
}
static void g_sound(void *ctx,uint16_t id) {sound_request(((tick_t *)ctx)->w,id);}
static void g_camera_mode(void *ctx,uint8_t mode) {tick_t *t=ctx;if (t->w->camera_native) neo_camera_set_auto(&t->w->camera,mode);}
static void g_camera_rise(void *ctx,int32_t vy) {tick_t *t=ctx;if (t->w->camera_native) t->w->camera.auto_vy=vy;}
static void g_shake(void *ctx) {tick_t *t=ctx;if (t->w->camera_native) neo_camera_shake_start(&t->w->camera);}
static bool g_map_swap(void *ctx) {tick_t *t=ctx;if (t->w->swap_step) return false;t->w->swap_step=1;return true;}
/* 012022: RAM 10B954 against 10B95C; the only call in Mission 1 is the
 * banners' constructor at camera 1600, where the trace reads 0x88 and 0x40
 * (bge: +15 bit 2 clear). */
static bool g_front_layer(void *ctx) {(void)ctx;return false;}
static void g_external(void *ctx,uint32_t constructor) {tick_t *t=ctx;t->w->external_spawns++;t->w->external_constructor=constructor;}
static bool g_creator_alive(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;uint32_t c=ga_l(a,0x0c);
    if (!c) return true;
    if ((c&0xff000000u)==NEO_GA_TAG_BASE) return (c&0xffffu)<NEO_WORLD_GACTORS && t->w->gactors[c&0xffffu].active && !t->w->gactors[c&0xffffu].placeholder;
    return true;
}
static neo_gactor_t *g_previous(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;neo_world_t *w=t->w;int at=order_of_gactor(w,a);
    if (at<=0 || w->order[at-1].kind!=NEO_WORLD_GACTOR) return NULL;
    return &w->gactors[w->order[at-1].index];
}
static neo_gactor_t *g_actor(void *ctx,uint32_t tag) {
    tick_t *t=ctx;
    if ((tag&0xff000000u)==NEO_GA_TAG_BASE && (tag&0xffffu)<NEO_WORLD_GACTORS && t->w->gactors[tag&0xffffu].active) return &t->w->gactors[tag&0xffffu];
    return NULL;
}
static neo_gactor_t *g_creator(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;uint32_t c=ga_l(a,0x0c);
    if ((c&0xff000000u)==NEO_GA_TAG_BASE && (c&0xffffu)<NEO_WORLD_GACTORS) return &t->w->gactors[c&0xffffu];
    return NULL;
}
/* ROM 01ABCC on a dynamic object's block. */
static void dyn_project(neo_world_t *w,tick_t *t,unsigned i) {
    neo_world_dyn_t *d=&w->dyn[i];neo_contact_parent_t p;
    bool has=d->block.parent_tag && g_parent(t,d->block.parent_tag,&p);
    neo_contact_project(&d->block,has?&p:NULL,w->epoch);
    neo_kdyn_t *k=&w->kernel.dyn[i];
    k->pos=(neo_kobj_t){d->block.projected_x,d->block.projected_y,d->block.cached_x,d->block.cached_y};
    w->dyn_parent[i]=(neo_contact_parent_t){NEO_DYN_TAG(i),d->block.projected_x,d->block.projected_y,d->block.cached_x,d->block.cached_y,d->block.correction};
    k->chain[0]=d->block.parent_tag;k->chain[1]=k->chain[2]=0;
    if ((k->chain[0]&0xff000000u)==NEO_GA_TAG_BASE) {
        const neo_gactor_t *o=&w->gactors[k->chain[0]&0xffffu];k->chain[1]=ga_l(o,0x38);
        if ((k->chain[1]&0xff000000u)==NEO_GA_TAG_BASE) k->chain[2]=ga_l(&w->gactors[k->chain[1]&0xffffu],0x38);
    }
}
static void g_dyn_set(void *ctx,neo_gactor_t *a,uint32_t map,int16_t dx,int16_t dy) {
    tick_t *t=ctx;neo_world_t *w=t->w;
    const uint8_t *m=ga_rom(map);
    if (!m) {ga_fail(&t->genv,map);return;}
    unsigned i=ga_b(a,0x6a);
    if (i==0xffu) { /* 0140D2: first free slot from 7 down */
        int k;for (k=NEO_KDYN_COUNT-1;k>=0 && (w->kernel.dyn[k].map || w->kernel.dyn[k].countdown);k--) {}
        if (k<0) return;
        i=(unsigned)k;ga_sb(a,0x6a,(uint8_t)i);
        w->kernel.dyn[i].map=m;
        neo_world_dyn_t *d=&w->dyn[i];memset(d,0,sizeof(*d));d->used=true;d->owner=a->index;
        d->block.x=(int32_t)(((uint32_t)(int32_t)dx<<16)+ga_l(a,0x28));d->block.y=(int32_t)(((uint32_t)(int32_t)dy<<16)+ga_l(a,0x2c));
        d->block.parent_tag=ga_l(a,0x38);d->block.mark=0;
    } else {
        if (i>=NEO_KDYN_COUNT) return;
        w->kernel.dyn[i].map=m;
        neo_world_dyn_t *d=&w->dyn[i];
        d->block.x=(int32_t)(((uint32_t)(int32_t)dx<<16)+ga_l(a,0x28));d->block.y=(int32_t)(((uint32_t)(int32_t)dy<<16)+ga_l(a,0x2c));
        d->block.vx=(int32_t)ga_l(a,0x30);d->block.vy=(int32_t)ga_l(a,0x34);d->block.parent_tag=ga_l(a,0x38);
    }
    dyn_project(w,t,i);
    neo_kernel_dyn_update(&w->kernel,i);
}
static void g_dyn_release(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;neo_world_t *w=t->w;unsigned i=ga_b(a,0x6a);
    if (i>=NEO_KDYN_COUNT) return;
    w->kernel.dyn[i].map=NULL;w->kernel.dyn[i].countdown=2;ga_sb(a,0x6a,0xff);
    dyn_project(w,t,i);
}
/* ROM 004126 -> 0164D0: a deleted node releases its dynamic object. */
static void ga_kill(tick_t *t,neo_gactor_t *a) {
    /* 003F90: palettes, dynamic object (0164D0), +6B/+58, +68/+69. */
    if (ga_b(a,0x6a)!=0xffu) g_dyn_release(t,a);
    for (unsigned i=0;i<NEO_WORLD_SUBSCRIPTS;i++) if (t->w->subs[i].used && t->w->subs[i].owner==a->index) t->w->subs[i].used=false;
    ga_sb(a,0x6b,0xff);ga_sw(a,0x58,0xffff);ga_sb(a,0x68,0);ga_sb(a,0x69,0xff);
    a->active=false;
}
static void g_marker(void *ctx,int32_t x,int32_t y,uint8_t h,uint8_t id) {
    tick_t *t=ctx;neo_world_t *w=t->w;unsigned l=t->genv.video&1u;
    if (w->marker_count[l]>=8u) return; /* 040114: 0x50 bytes */
    w->markers[l][w->marker_count[l]++]=(neo_marker_t){x,y,h,id};
}
/* 004E76: register (or with a null descriptor unregister) the actor's solid
 * body in the table 106756 (slot index at +6B, first free slot for a new
 * body; a full table leaves the body unregistered). */
static void g_solid(void *ctx,neo_gactor_t *a,uint32_t desc) {
    neo_world_t *w=((tick_t *)ctx)->w;
    if (!desc) {ga_sb(a,0x6b,0xff);return;}
    uint8_t slot=ga_b(a,0x6b);
    if (slot==0xffu) {
        for (slot=0;slot<NEO_WORLD_SOLIDS && w->solids[slot].actor>=0;slot++) {}
        if (slot==NEO_WORLD_SOLIDS) return;                            /* 004EB6 */
        ga_sb(a,0x6b,slot);
    } else if (slot>=NEO_WORLD_SOLIDS) {                               /* 004EC0: TRAP #15 */
        w->solid_unsupported++;ga_fail(&((tick_t *)ctx)->genv,0x4ec0);return;
    }
    w->solids[slot].actor=(int16_t)a->index;w->solids[slot].desc=desc;
}
static void g_stamp(void *ctx,neo_gactor_t *a,uint32_t templ) {
    if (templ==0x29bf4u || templ==0x29c26u) {neo_collision_vehicle_wreckage((int32_t)ga_l(a,0x28),(int32_t)ga_l(a,0x2c),(uint8_t)(templ==0x29c26u?3u:1u));return;}
    const uint8_t *t=ga_rom(templ);
    if (!t) {ga_fail(&((tick_t *)ctx)->genv,templ);return;}
    neo_collision_stamp((int32_t)ga_l(a,0x28),(int32_t)ga_l(a,0x2c),t);
}
/* ROM 0C70FC, collision part (0C72EC): the sub-map's cells over the whole
 * stage grid of the terrain layer 10C72E (the ring window is not modelled;
 * the patch is made on screen). Block row d5 is counted from the ROM top
 * row of a 32-row grid (Mission 2 stage 1); others stop explicitly. */
static void g_map_patch(void *ctx,uint32_t layer,uint32_t submap,const int16_t d[6]) {
    neo_genv_t *e=&((tick_t *)ctx)->genv;
    const uint8_t *s=ga_rom(submap);
    if (!s) {ga_fail(e,submap);return;}
    uint32_t c=(uint32_t)s[4]<<24|(uint32_t)s[5]<<16|(uint32_t)s[6]<<8|s[7];
    if (layer!=0x10c72eu || !c) return;                            /* tiles only (the renderer's) */
    if (c==0xffffffffu || d[0] || d[1] || neo_collision_rows()!=32u) {ga_fail(e,0xc72ec);return;}
    unsigned h=(unsigned)(s[10]<<8|s[11]);
    const uint8_t *cells=ga_rom(c);
    if (!cells || (unsigned)d[3]!=h) {ga_fail(e,c);return;}
    neo_collision_patch(cells,(unsigned)d[2],h,2*d[4],2*d[5]+1);
}
/* ROM 004C24: the actor is registered when one of its rectangles is inside
 * the registry area (x -32..383, y 264..503 projected); the query (0049E8 ->
 * 004762) reads the target's current position and descriptor. The original
 * visits slots in its bin-word order; the port visits them in registration
 * order (the slot numbers include the player and the outside actors). */
/* Registry window clip of ROM 004C24/0049E8 (x -32..383, y 264..503
 * projected): false when the rectangle is outside, else its 32-pixel bins. */
static bool clip_bins(int16_t x,int16_t y,int16_t w,int16_t h,uint16_t *xbins,uint8_t *ybins) {
    x=(int16_t)(x+0x20);
    if (x<0) {w=(int16_t)(w+x);if (w<0) return false;x=0;}
    if (x>0x17f) return false;
    if (x+w>0x17f) w=(int16_t)(0x17f-x);
    if (!w) return false;
    y=(int16_t)(y-0x108);
    if (y<0) {h=(int16_t)(h+y);if (h<0) return false;y=0;}
    if (y>0xef) return false;
    if (y+h>0xef) h=(int16_t)(0xef-y);
    if (!h) return false;
    for (int b=x>>5;b<=(x+w)>>5;b++) *xbins|=(uint16_t)(1u<<b);
    for (int b=y>>5;b<=(y+h)>>5;b++) *ybins|=(uint8_t)(1u<<b);
    return true;
}
/* 004C24: the next registry slot (none past 0x40). */
static void reg_add(neo_world_t *w,uint8_t kind,unsigned index,uint16_t xb,uint8_t yb) {
    if (w->reg_count>=COUNT(w->regs)) return;
    w->regs[w->reg_count].kind=kind;w->regs[w->reg_count].index=(uint8_t)index;
    w->regs[w->reg_count].xbins=xb;w->regs[w->reg_count].ybins=yb;w->reg_count++;
}
static bool ga_rect_bins(const neo_gactor_t *a,const uint8_t *r,uint16_t *xbins,uint8_t *ybins) {
    int16_t x=(int16_t)(r[0]<<8|r[1]),y=(int16_t)(r[2]<<8|r[3]),w=(int16_t)(r[4]<<8|r[5]),h=(int16_t)(r[6]<<8|r[7]);
    if (ga_b(a,0x15)&1u) x=(int16_t)-(x+w);
    x=(int16_t)(x+(int16_t)ga_w(a,0x3c));
    if (ga_b(a,0x15)&2u) y=(int16_t)-(y+h);
    y=(int16_t)(y+(int16_t)ga_w(a,0x40));
    return clip_bins(x,y,w,h,xbins,ybins);
}
static void g_register(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;neo_world_t *w=t->w;const uint8_t *d=ga_rom(ga_l(a,0x50));
    if (!d) {ga_fail(&t->genv,ga_l(a,0x50));return;}
    bool any=false;uint16_t xb=0;uint8_t yb=0;
    for (unsigned at=2;(d[at]<<8|d[at+1])!=0x8000;at+=8) any|=ga_rect_bins(a,d+at,&xb,&yb);
    if (any && w->box_count<NEO_WORLD_BOXES) {w->boxes[w->box_count].owner=(uint8_t)a->index;w->boxes[w->box_count].xbins=xb;w->boxes[w->box_count].ybins=yb;w->box_count++;}
    if (any) reg_add(w,NEO_WORLD_GACTOR,(unsigned)a->index,xb,yb);
}
/* Bins of an attack rectangle (0049E8); false when outside the window. */
static bool attack_bins(const neo_contact_actor_t *a,const int16_t at[7],uint16_t *xb,uint8_t *yb) {
    int16_t x=at[0],y=at[1];
    if (a->flags15&1u) x=(int16_t)-(x+at[2]);
    x=(int16_t)(x+(int16_t)HI(a->projected_x));
    if (a->flags15&2u) y=(int16_t)-(y+at[3]);
    y=(int16_t)(y+(int16_t)HI(a->projected_y));
    *xb=0;*yb=0;
    return clip_bins(x,y,at[2],at[3],xb,yb);
}
/* ROM 004762 + 004892..00499A on a generic actor hit by attack row `at`. */
static bool ga_receive(neo_gactor_t *g,const neo_contact_actor_t *a,uint8_t a5e,const int16_t at[7],uint16_t *grid) {
    const uint8_t *d=ga_rom(ga_l(g,0x50));
    if (!d) return false;
    uint16_t x=(uint16_t)at[0],y=(uint16_t)at[1];
    if (a->flags15&1u) x=(uint16_t)(0u-(uint16_t)(x+(uint16_t)at[2]));
    x=(uint16_t)(x+HI(a->projected_x));
    if (a->flags15&2u) y=(uint16_t)(0u-(uint16_t)(y+(uint16_t)at[3]));
    y=(uint16_t)(y+HI(a->projected_y));
    neo_hit_rectangle_t q={x,y,(uint16_t)at[2],(uint16_t)at[3]};
    uint16_t ix=0,iy=0;bool hit=false;
    for (unsigned o=2;(d[o]<<8|d[o+1])!=0x8000 && !hit;o+=8) {
        neo_hit_rectangle_t local={(uint16_t)(d[o]<<8|d[o+1]),(uint16_t)(d[o+2]<<8|d[o+3]),(uint16_t)(d[o+4]<<8|d[o+5]),(uint16_t)(d[o+6]<<8|d[o+7])};
        hit=neo_hit_rectangle(&q,ga_w(g,0x3c),ga_w(g,0x40),ga_b(g,0x15),&local,&ix,&iy);
    }
    if (!hit) return false;
    uint8_t type=d[0];
    uint8_t kind=(uint8_t)at[4];
    if (kind>=20u || !soldier_hit_matrix[type&7u][kind]) return true;
    if (kind>ga_b(g,0x5a)) {
        ga_sb(g,0x5a,kind);ga_sb(g,0x17,(uint8_t)(ga_b(g,0x17)|6u));
        uint8_t f=0;
        if ((int16_t)(uint16_t)(ga_w(g,0x3c)-HI(a->cached_x))<0) f|=0x80u;
        if ((int16_t)(uint16_t)(ga_w(g,0x40)-HI(a->cached_y))<0) f|=0x40u;
        uint32_t dx=(uint32_t)a->projected_x-(uint32_t)a->cached_x-ga_l(g,0x3c)+ga_l(g,0x44);
        uint32_t dy=(uint32_t)a->projected_y-(uint32_t)a->cached_y-ga_l(g,0x40)+ga_l(g,0x48);
        f|=(uint8_t)((neo_angle((int16_t)(dx>>16),(int16_t)(dy>>16))>>4)&15u);
        ga_sb(g,0x5b,f);ga_sb(g,0x5c,(uint8_t)((int16_t)ix>>1));ga_sb(g,0x5d,(uint8_t)((int16_t)iy>>1));
    }
    if (!(ga_b(g,0x5f)&0xc0u)) {
        uint16_t dmg=(uint16_t)at[6];
        const uint8_t *mul=ga_l(g,0x54)?ga_rom(ga_l(g,0x54)):NULL;
        if (mul) dmg=(uint16_t)(((uint32_t)(mul[2*kind]<<8|mul[2*kind+1])*dmg)>>8);
        if (dmg) {
            uint16_t h=ga_w(g,0x4e);
            if (!(h>dmg)) {ga_sb(g,0x5f,(uint8_t)(ga_b(g,0x5f)|0x80u));if (at[5]==1 && grid) neo_grid_splat(grid,HI(a->projected_x),HI(a->projected_y));h=0;}
            else h=(uint16_t)(h-dmg);
            ga_sw(g,0x4e,h);
        }
    }
    ga_sb(g,0x5e,(uint8_t)(((a5e&3u)<<2)|(ga_b(g,0x5e)&0xf3u)));
    return true;
}
static bool ga_class(const neo_gactor_t *g,unsigned cls) {
    const uint8_t *d=ga_rom(ga_l(g,0x50));
    return d && ((d[1]>>cls)&1u);
}
static bool boss_shot(neo_world_t *w,const neo_contact_actor_t *a,const int16_t at[7]);
/* 004762..004952 without side effects, for one registered body: the
 * narrow phase with the descriptor's rectangles, the hit matrix 004582, the
 * +54 multiplier of the kind and +5F bits 7/6; its +4E and what 01B162 reads. */
static bool candidate(const uint8_t *d,uint16_t bx,uint16_t by,uint8_t b15,const uint8_t *mul,uint8_t f5f,
                      const neo_hit_rectangle_t *q,const int16_t row[7]) {
    if (!d || !((d[1]>>(unsigned)row[5])&1u)) return false;
    uint16_t ix=0,iy=0;bool hit=false;
    for (unsigned o=2;(d[o]<<8|d[o+1])!=0x8000 && !hit;o+=8) {
        neo_hit_rectangle_t local={(uint16_t)(d[o]<<8|d[o+1]),(uint16_t)(d[o+2]<<8|d[o+3]),(uint16_t)(d[o+4]<<8|d[o+5]),(uint16_t)(d[o+6]<<8|d[o+7])};
        hit=neo_hit_rectangle(q,bx,by,b15,&local,&ix,&iy);
    }
    uint8_t kind=(uint8_t)row[4];
    if (!hit || kind>=20u || !soldier_hit_matrix[d[0]&7u][kind]) return false;
    if (mul && !(mul[2*kind]|mul[2*kind+1])) return false;
    return !(f5f&0xc0u);
}
static void attack_targets(tick_t *t,neo_gactor_t *a,const neo_contact_actor_t *c,const int16_t row[7]) {
    neo_world_t *w=t->w;neo_ga_target_t *out=t->genv.target;
    memset(out,0,2*sizeof(*out));
    uint16_t qx;uint8_t qy;
    if (!attack_bins(c,row,&qx,&qy)) return;                       /* 004BA4 */
    uint16_t x=(uint16_t)row[0],y=(uint16_t)row[1];
    if (c->flags15&1u) x=(uint16_t)(0u-(uint16_t)(x+(uint16_t)row[2]));
    x=(uint16_t)(x+HI(c->projected_x));
    if (c->flags15&2u) y=(uint16_t)(0u-(uint16_t)(y+(uint16_t)row[3]));
    y=(uint16_t)(y+HI(c->projected_y));
    neo_hit_rectangle_t q={x,y,(uint16_t)row[2],(uint16_t)row[3]};
    uint16_t hi=0,lo=0xffff;
    static const uint8_t group[4]={16,0,48,32};
    for (unsigned gi=0;gi<4;gi++) for (unsigned sl=group[gi];sl<group[gi]+16u;sl++) {
        if (sl>=w->last_reg_count) continue;
        const uint16_t xb=w->last_regs[sl].xbins;const uint8_t yb=w->last_regs[sl].ybins;
        if (!(xb&qx) || !(yb&qy)) continue;
        unsigned i=w->last_regs[sl].index;uint16_t h;neo_ga_target_t tg={true,0,0,true,{0,0,0,0},0,0};
        const uint8_t *d=NULL;
        switch (w->last_regs[sl].kind) {
        case NEO_WORLD_GACTOR: {
            neo_gactor_t *g=&w->gactors[i];
            if (g==a || !g->active || g->placeholder || !ga_l(g,0x50)) continue;
            d=ga_rom(ga_l(g,0x50));
            if (!candidate(d,ga_w(g,0x3c),ga_w(g,0x40),ga_b(g,0x15),ga_l(g,0x54)?ga_rom(ga_l(g,0x54)):NULL,ga_b(g,0x5f),&q,row)) continue;
            h=ga_w(g,0x4e);tg.x=ga_w(g,0x3c);tg.y=ga_w(g,0x40);tg.y_low=ga_w(g,0x42);tg.x_low=ga_w(g,0x3e);
            break;
        }
        case NEO_WORLD_SOLDIER: {
            neo_soldier_t *s=&w->soldiers[i];neo_hurtbox_t hb;
            if (!s->active || !w->constructed[i] || !neo_soldier_hurtbox(s,&hb)) continue;
            uint8_t sd[12]={hb.type,(uint8_t)(1u<<hb.klass),(uint8_t)((uint16_t)hb.rect[0]>>8),(uint8_t)hb.rect[0],(uint8_t)((uint16_t)hb.rect[1]>>8),(uint8_t)hb.rect[1],
                (uint8_t)((uint16_t)hb.rect[2]>>8),(uint8_t)hb.rect[2],(uint8_t)((uint16_t)hb.rect[3]>>8),(uint8_t)hb.rect[3],0x80,0};
            if (!candidate(sd,hb.x,hb.y,hb.flags15,NULL,hb.dead?0x80u:0,&q,row)) continue;
            h=s->health;tg.x=hb.x;tg.y=hb.y;tg.y_low=(uint16_t)s->b.projected_y;tg.x_low=(uint16_t)s->b.projected_x;d=sd;
            for (unsigned k=0;k<4;k++) tg.rect[k]=hb.rect[k];
            break;
        }
        case NEO_WORLD_BOSS: {
            const neo_boss_node_t *n=&w->boss.nodes[i];
            if (!w->boss_active || !n->routine || !neo_boss_l(n,0x50)) continue;
            d=neo_boss_rom(neo_boss_l(n,0x50),12);
            const uint8_t *mul=neo_boss_l(n,0x54)?neo_boss_rom(neo_boss_l(n,0x54),40):NULL;
            if (!candidate(d,neo_boss_w(n,0x3c),neo_boss_w(n,0x40),n->f[0x15],mul,n->f[0x5f],&q,row)) continue;
            h=neo_boss_w(n,0x4e);tg.x=neo_boss_w(n,0x3c);tg.y=neo_boss_w(n,0x40);tg.y_low=neo_boss_w(n,0x42);tg.x_low=neo_boss_w(n,0x3e);
            break;
        }
        default: continue;
        }
        if (w->last_regs[sl].kind!=NEO_WORLD_SOLDIER) for (unsigned k=0;k<4;k++) tg.rect[k]=(int16_t)(d[2+2*k]<<8|d[3+2*k]);
        if (h>=hi) {hi=h;out[0]=tg;}                                   /* 004932..004942 */
        if (h<=lo) {lo=h;out[1]=tg;}                                   /* 004946..004952 */
    }
}
static uint8_t g_attack(void *ctx,neo_gactor_t *a,const int16_t row[7]) {
    tick_t *t=ctx;neo_world_t *w=t->w;neo_contact_actor_t c=ga_contact(a);
    if (t->genv.want_targets && row[5]!=0) attack_targets(t,a,&c,row);
    if (row[5]==0) { /* class 0: the player */
        if (!t->player_hittable) return 0;
        int first=-1;uint8_t type=neo_hit_query(&c,row,&t->player,1,(uint8_t)row[4],0,&first);
        if (type && first>=0 && row[6]) player_hit(t,c.projected_x,c.cached_x,(uint8_t)row[4]);
        return type;
    }
    uint8_t best=0;
    for (unsigned at=0;at<w->order_count;at++) {
        if (w->order[at].kind==NEO_WORLD_SOLDIER && w->constructed[w->order[at].index]) {
            neo_soldier_t *s=&w->soldiers[w->order[at].index];neo_hurtbox_t hb;
            if (neo_soldier_hurtbox(s,&hb) && neo_soldier_receive(s,&c,ga_b(a,0x5e),row,w->grid) && hb.type>best) best=hb.type;
        }
    }
    /* The boss's hurtboxes (registered apart, see boss_shot) take the
     * player's bullets (neo_bullet.c) like neo_world_shot_row's. */
    if (w->boss_active && (neo_bullet_routine(ga_l(a,0)) || neo_chaser_attacker(ga_l(a,0))) && !t->genv.want_targets && boss_shot(w,&c,row) && !best) best=1;
    uint16_t qx;uint8_t qy;
    if (!attack_bins(&c,row,&qx,&qy)) return best;
    for (unsigned b=0;b<w->last_box_count;b++) {
        neo_gactor_t *g=&w->gactors[w->last_boxes[b].owner];
        if (!(w->last_boxes[b].xbins&qx) || !(w->last_boxes[b].ybins&qy)) continue;
        if (g==a || !g->active || !ga_l(g,0x50) || !ga_class(g,(unsigned)row[5])) continue;
        if (ga_receive(g,&c,ga_b(a,0x5e),row,w->grid)) {uint8_t ty=ga_rom(ga_l(g,0x50))[0];if (ty>best) best=ty;}
    }
    return best;
}
static void delete_tree(tick_t *t,unsigned at);
static int run_gactor(tick_t *t,unsigned i) {
    neo_world_t *w=t->w;neo_gactor_t *a=&w->gactors[i];
    uint8_t group=ga_b(a,0x69);
    unsigned before=w->draw_count;
    t->draw_kind=NEO_WORLD_GACTOR;t->draw_flash=false;t->draw_palette=0;
    /* RAM the Mission 2 boss 026254 reads and writes (neo_m2boss.c): camera
     * 699A, point 69BA, the mission actor's speeds, the tower mode 10C354;
     * one frame per logic update (003B4C). */
    bool m2=neo_m2boss_routine(ga_l(a,0));
    if (m2) {
        neo_m2boss_env_t *m=&t->m2boss;
        m->camera_dy=w->camera_native?w->camera.dy:0;m->point69ba=w->script.w69ba;
        m->speed[0]=w->camera.speed_x;m->speed[1]=w->camera.speed_y;m->tower_mode=w->tower.mode;m->frame_step=1;
        t->genv.m2boss=m;
    }
    ga_result_t r=neo_region_tick(a,&t->genv);
    if (m2) {
        neo_m2boss_env_t *m=&t->m2boss;
        w->script.w69ba=m->point69ba;w->camera.speed_x=m->speed[0];w->camera.speed_y=m->speed[1];w->tower.mode=m->tower_mode;
        t->genv.m2boss=NULL;
    }
    for (unsigned k=before;k<w->draw_count;k++) {
        uint8_t h=(uint8_t)(w->draws[k].attributes>>8);
        w->draws[k].palette=h && h<=6u?a->palette[h-1u]:a->palette[0];
        w->draws[k].flash=(ga_b(a,0x17)&1u) && !(ga_b(a,0x17)&0x80u);
        w->draws[k].zoom_x=ga_b(a,0x12);w->draws[k].zoom_y=ga_b(a,0x13);
    }
    if (group<9u && ga_b(a,0x69)>=9u) w->kills++;
    if ((r==GA_UNSUPPORTED || t->genv.unsupported) && neo_item_pickup(t->genv.unsupported_at)) {
        /* Explicit stop: an item taken by the player (its pickup routine
         * serves the original player's weapons/points, not ported). */
        w->item_pickups++;w->item_pickup_routine=t->genv.unsupported_at;t->genv.unsupported=0;
        w->draw_count=before;a->active=false;return 0;
    }
    if (r==GA_UNSUPPORTED || t->genv.unsupported) {
        w->unsupported++;w->unsupported_routine=t->genv.unsupported?t->genv.unsupported_at:ga_l(a,0);t->genv.unsupported=0;
        /* The stopped actor goes with its descendants (0041AC): a child
         * must not outlive it (the Camel Slug stopped at 004ED2 left its
         * rider graphic 034F1A, a second Marco, drawn where it stopped). */
        {int at=find(w,NEO_WORLD_GACTOR,(unsigned)a->index);if (at>=0) delete_tree(t,(unsigned)at);}
        w->draw_count=before;a->active=false;return 0;
    }
    if (r==GA_DELETE) {ga_kill(t,a);return 0;}
    if (r==GA_DELETE_TREE) {ga_kill(t,a);return 2;}
    return 1;
}
/* ---- Mission 1 boss (neo_boss) ------------------------------------------------- */
/* The boss services answer from the native world. Context: the running tick. */
static tick_t *boss_tick_ctx;
static uint8_t boss_kobj(uint32_t tag) {
    if (tag==0x10d7b6u || tag==0x10e8c6u) return NEO_KOBJ_TERRAIN;  /* see g_parent */
    if (tag>=NEO_DYN_TAG(0) && tag<NEO_DYN_TAG(NEO_KDYN_COUNT) && !((tag-NEO_DYN_TAG(0))&63u)) return (uint8_t)(NEO_KOBJ_DYN0+(tag-NEO_DYN_TAG(0))/64u);
    return tag==TERRAIN_TAG?NEO_KOBJ_TERRAIN:tag==ROOT_TAG?NEO_KOBJ_ROOT:tag?0xff:NEO_KOBJ_NONE;
}
static uint32_t boss_ktag(uint8_t id) {
    if (id>=NEO_KOBJ_DYN0 && id<NEO_KOBJ_DYN0+NEO_KDYN_COUNT) return NEO_DYN_TAG(id-NEO_KOBJ_DYN0);
    return ktag(id);
}
static uint16_t b_random(void *c) {(void)c;return neo_rng_next(&boss_tick_ctx->w->rng);}
/* ROM 0040BC: the native pool is the boss's own node array; the image is
 * what ROM 004126 leaves in a freed node (the previous occupant's other
 * bytes are not tracked: zero, see docs/MISION1_COMPLETA.md). */
static bool b_allocate(void *c,uint16_t reserve,uint8_t image[NEO_BOSS_FIELDS]) {
    neo_world_t *w=c;unsigned used=0;
    for (unsigned i=0;i<NEO_BOSS_NODES;i++) used+=w->boss.nodes[i].routine!=0;
    if (NEO_BOSS_NODES-used<=reserve) return false;
    memset(image,0,NEO_BOSS_FIELDS);
    image[0x12]=image[0x13]=0xff;image[0x17]=0xff;image[0x1e]=image[0x1f]=0xff;image[0x58]=image[0x59]=0xff;
    image[0x69]=image[0x6a]=image[0x6b]=0xff;
    return true;
}
/* ROM 009EA8: one palette slot per logical resource, reference counted. */
static bool b_palette(void *c,uint16_t resource,uint8_t *slot) {
    neo_world_t *w=c;int free_slot=-1;
    for (unsigned i=1;i<32;i++) {
        if (w->boss_palette_refs[i] && w->boss_palette[i]==resource) {w->boss_palette_refs[i]++;*slot=(uint8_t)i;return true;}
        if (!w->boss_palette_refs[i] && free_slot<0) free_slot=(int)i;
    }
    if (free_slot<0) return false;
    w->boss_palette[free_slot]=resource;w->boss_palette_refs[free_slot]=1;*slot=(uint8_t)free_slot;return true;
}
static void b_palette_release(void *c,const uint8_t *slots,unsigned count) {
    neo_world_t *w=c;
    for (unsigned i=0;i<count;i++) if (slots[i]<32 && w->boss_palette_refs[slots[i]]) w->boss_palette_refs[slots[i]]--;
}
static void b_sound(void *c,uint16_t id) {sound_request(c,id);}
static bool b_scene(void *c,uint32_t tag,neo_contact_parent_t *out) {
    neo_world_t *w=c;
    if (tag==ROOT_TAG) {*out=w->root;return true;}
    if (tag==TERRAIN_TAG || tag==0x10d7b6u || tag==0x10e8c6u) {*out=w->terrain;out->tag=tag;return true;}
    return false;
}
static bool b_sweep(void *c,const neo_contact_request_t *q,neo_boss_sweep_t *out) {
    neo_world_t *w=c;uint8_t parent=boss_kobj(q->parent_tag);
    if (parent==0xff) return false;
    neo_kreq_t kq={q->x,q->y,q->dx,q->dy,parent,0,q->mode8,0,0,0,0};neo_kres_t k;
    if (!neo_kernel_sweep(&w->kernel,&kq,&k)) return false;
    *out=(neo_boss_sweep_t){k.x,k.y,k.vx,k.vy,boss_ktag(k.parent),k.flags,(uint16_t)(k.kind>>8)};
    return true;
}
static bool b_floor(void *c,int32_t x,int32_t y,uint32_t parent_tag,neo_boss_sweep_t *out) {
    neo_world_t *w=c;uint8_t parent=boss_kobj(parent_tag);
    if (parent==0xff) return false;
    neo_kfloor_t fl;
    if (!neo_kernel_floor(&w->kernel,x,y,parent,(int32_t)0xff000000,255,0,0,&fl)) return false;
    *out=(neo_boss_sweep_t){fl.x,fl.y,0,0,boss_ktag(fl.parent),0,(uint16_t)(fl.kind>>8)};
    return true;
}
/* ROM 0140C0 / 013F1E: object platforms 0E93B2/0E94AC/0E95A6 as kernel
 * dynamic objects (the soldiers' and generic actors' collision sees them). */
static bool b_platform(void *c,int16_t node,uint8_t slot,uint32_t table,const int32_t pos[4],uint32_t parent,uint8_t *slot_out) {
    neo_world_t *w=c;tick_t *t=boss_tick_ctx;
    const uint8_t *m=neo_boss_rom(table,10);
    if (!m) return false;
    unsigned i=slot;
    if (i==0xffu) {
        int k;for (k=NEO_KDYN_COUNT-1;k>=0 && (w->kernel.dyn[k].map || w->kernel.dyn[k].countdown);k--) {}
        if (k<0) {*slot_out=0xff;return true;}
        i=(unsigned)k;memset(&w->dyn[i],0,sizeof(w->dyn[i]));w->dyn[i].used=true;w->dyn[i].owner=(int16_t)(0x4000|node);
    }
    if (i>=NEO_KDYN_COUNT) return false;
    w->kernel.dyn[i].map=m;
    neo_world_dyn_t *d=&w->dyn[i];
    d->block.x=pos[0];d->block.y=pos[1];d->block.vx=pos[2];d->block.vy=pos[3];d->block.parent_tag=parent;
    dyn_project(w,t,i);neo_kernel_dyn_update(&w->kernel,i);
    *slot_out=(uint8_t)i;return true;
}
static void b_platform_release(void *c,int16_t node,uint8_t slot) {
    neo_world_t *w=c;(void)node;
    if (slot>=NEO_KDYN_COUNT) return;
    w->kernel.dyn[slot].map=NULL;w->kernel.dyn[slot].countdown=2;dyn_project(w,boss_tick_ctx,slot);
}
/* ROM 004C24: rectangles of the node's +50 descriptor inside the registry
 * window; queried by the player's shots during the next update. */
static bool b_hurtbox(void *c,int16_t node,uint8_t *offset,uint8_t *index,bool *kill_all) {
    neo_world_t *w=c;const neo_boss_node_t *n=&w->boss.nodes[node];
    const uint8_t *d=neo_boss_rom(neo_boss_l(n,0x50),12);
    *offset=0;*index=0;*kill_all=false;
    if (!d) return false;
    bool any=false;
    for (unsigned at=2;(d[at]<<8|d[at+1])!=0x8000;at+=8) {
        if (!neo_boss_rom(neo_boss_l(n,0x50)+at,10)) return false;
        int16_t x=(int16_t)(d[at]<<8|d[at+1]),y=(int16_t)(d[at+2]<<8|d[at+3]),rw=(int16_t)(d[at+4]<<8|d[at+5]),rh=(int16_t)(d[at+6]<<8|d[at+7]);
        if (n->f[0x15]&1u) x=(int16_t)-(x+rw);
        x=(int16_t)(x+(int16_t)neo_boss_w(n,0x3c));
        if (n->f[0x15]&2u) y=(int16_t)-(y+rh);
        y=(int16_t)(y+(int16_t)neo_boss_w(n,0x40));
        uint16_t xb=0;uint8_t yb=0;any|=clip_bins(x,y,rw,rh,&xb,&yb);
    }
    if (any && w->boss_box_count<NEO_BOSS_NODES) w->boss_boxes[w->boss_box_count++]=node;
    if (any) {
        uint16_t xb=0;uint8_t yb=0;
        for (unsigned at=2;(d[at]<<8|d[at+1])!=0x8000;at+=8) {
            int16_t x=(int16_t)(d[at]<<8|d[at+1]),y=(int16_t)(d[at+2]<<8|d[at+3]),rw=(int16_t)(d[at+4]<<8|d[at+5]),rh=(int16_t)(d[at+6]<<8|d[at+7]);
            if (n->f[0x15]&1u) x=(int16_t)-(x+rw);
            x=(int16_t)(x+(int16_t)neo_boss_w(n,0x3c));
            if (n->f[0x15]&2u) y=(int16_t)-(y+rh);
            y=(int16_t)(y+(int16_t)neo_boss_w(n,0x40));
            clip_bins(x,y,rw,rh,&xb,&yb);
        }
        reg_add(w,NEO_WORLD_BOSS,(unsigned)node,xb,yb);
    }
    /* 004BAC: after the mission-complete request everything registered dies. */
    *kill_all=w->mission_step!=0;
    return true;
}
/* ROM 0049E8 with a boss attack row (x,y,w,h,kind,class,damage,next):
 * class 0 rows against the player. */
static bool b_attack(void *c,int16_t node,uint32_t rows,uint16_t *result) {
    neo_world_t *w=c;tick_t *t=boss_tick_ctx;const neo_boss_node_t *n=&w->boss.nodes[node];
    *result=0;
    for (unsigned k=0;k<8;k++) {
        const uint8_t *r=neo_boss_rom(rows+16u*k,16);
        if (!r) return false;
        int16_t row[7];for (unsigned i=0;i<7;i++) row[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
        if (row[5]==0 && t->player_hittable) {
            neo_contact_actor_t a;memset(&a,0,sizeof(a));
            a.projected_x=(int32_t)neo_boss_l(n,0x3c);a.projected_y=(int32_t)neo_boss_l(n,0x40);
            a.cached_x=(int32_t)neo_boss_l(n,0x44);a.cached_y=(int32_t)neo_boss_l(n,0x48);a.flags15=n->f[0x15];
            int first=-1;uint8_t type=neo_hit_query(&a,row,&t->player,1,(uint8_t)row[4],0,&first);
            if (type && first>=0 && row[6]) player_hit(t,a.projected_x,a.cached_x,(uint8_t)row[4]);
            if (type>*result) *result=type;
        }
        if ((uint16_t)(r[14]<<8|r[15])==0xffffu) return true;
    }
    return true;
}
static bool b_bounds(void *c,neo_cull_bounds_t *out) {(void)c;*out=boss_tick_ctx->bounds;return true;}
static void b_draw(void *c,int16_t node,const neo_anim_draw_t *d) {
    neo_world_t *w=c;
    if (w->draw_count==NEO_WORLD_DRAWS) return;
    const neo_boss_node_t *n=&w->boss.nodes[node];
    uint8_t slot=(uint8_t)(d->attributes>>8);
    w->draws[w->draw_count++]=(neo_world_draw_t){d->descriptor,d->x,d->y,d->attributes,slot<32?w->boss_palette[slot]:0,
        (n->f[0x17]&1u) && !(n->f[0x17]&0x80u),NEO_WORLD_BOSS,0xff,0xff,d->z};
}
/* Generic actors the boss creates or becomes: the native generic families
 * continue them from the seed image; the others are counted (not drawn). */
static bool b_effect(void *c,const neo_boss_effect_t *e) {
    neo_world_t *w=c;tick_t *t=boss_tick_ctx;
    if (e->kind==NEO_BOSS_EFFECT_DELETE_TREE) return true;
    /* 02A18A is JMP 0041AC: a converted node that deletes itself. */
    if (e->kind==NEO_BOSS_EFFECT_CONVERT && e->label==0x2a18au) return true;
    if (!e->has_image || !neo_region_routine(e->label)) {w->boss_external_effects++;w->external_constructor=e->label;return true;}
    int at=find(w,NEO_WORLD_BOSS,0);
    neo_gactor_t *a=ga_insert(t,at,e->label,0,e->image[0x68]);
    if (!a) return true;
    for (unsigned o=0x10;o<NEO_BOSS_FIELDS;o++) if (o<0x20 || o>=0x24) ga_sb(a,o,e->image[o]);
    ga_sl(a,0,e->label);ga_sw(a,0x10,a->m[0x10]);
    /* Palette slots of the boss allocator -> native handles (ga_palette):
     * +19..+1C hold the node's references, +14/+16 the draw/flash slot. */
    uint8_t count=e->image[0x1d]<4u?e->image[0x1d]:4u;
    for (unsigned i=0;i<count;i++) {uint8_t s=e->image[0x19+i];a->palette[i]=s<32?w->boss_palette[s]:0;ga_sb(a,0x19+i,(uint8_t)(i+1u));}
    /* A draw/flash slot the node no longer references (the shell releases
     * its three palettes before converting into the explosion 01C71E, which
     * then allocates 0x204 into +19) keeps its resource behind handle 5/6:
     * the original draws with the slot's last contents until the allocator
     * reuses it, and the +19.. list stays as the original's (the explosion's
     * descriptors select palette 0 = +19). */
    static const uint8_t refs[2]={0x14,0x16};
    for (unsigned r=0;r<2;r++) {
        uint8_t s=e->image[refs[r]],h=0;
        for (unsigned i=0;i<count;i++) if (e->image[0x19+i]==s) h=(uint8_t)(i+1u);
        if (!h && s && s<32) {a->palette[4+r]=w->boss_palette[s];h=(uint8_t)(5+r);}
        ga_sb(a,refs[r],h);
    }
    ga_sb(a,0x1d,count);
    /* The node's creator is a boss node: no generic creator to follow. */
    ga_sl(a,0x0c,0);
    return true;
}
static void b_mission(void *c,int16_t node,uint8_t step) {neo_world_t *w=c;(void)node;if (step>w->mission_step) w->mission_step=step;}
static void boss_spawn(tick_t *t,int at,const neo_script_spawn_t *sp) {
    neo_world_t *w=t->w;
    if (w->boss_active || at<0) {w->overflow++;return;}
    neo_boss_services_t sv={w,b_random,b_allocate,b_palette,b_palette_release,b_sound,b_scene,b_sweep,b_floor,
        b_platform,b_platform_release,b_hurtbox,b_attack,b_bounds,b_draw,b_effect,b_mission};
    neo_boss_init(&w->boss,&sv);
    /* 01A10A/01A136: a fresh node (0040BC), the record words at +6E.., +69
     * group, +68 variant, +28/+2C in the scene root (01ABB6), projected. */
    uint8_t image[NEO_BOSS_FIELDS];b_allocate(w,0,image);
    image[0x10]=0;image[0x11]=(uint8_t)(w->order[at].depth+1u);
    for (unsigned k=0;k<sp->word_count && 0x6eu+2u*k+1u<NEO_BOSS_FIELDS;k++) {image[0x6e + 2*k]=(uint8_t)(sp->words[k]>>8);image[0x6f + 2*k]=(uint8_t)sp->words[k];}
    image[0x69]=sp->group69;image[0x68]=sp->variant68;
    uint32_t v[3]={(uint32_t)sp->x,(uint32_t)sp->y,ROOT_TAG};
    for (unsigned k=0;k<3;k++) for (unsigned b=0;b<4;b++) image[(k==2?0x38:0x28+4*k)+b]=(uint8_t)(v[k]>>(24-8*b));
    neo_contact_actor_t a;memset(&a,0,sizeof(a));
    a.x=sp->x;a.y=sp->y;a.parent_tag=ROOT_TAG;
    neo_contact_project(&a,&w->root,w->epoch);
    uint32_t p[4]={(uint32_t)a.projected_x,(uint32_t)a.projected_y,(uint32_t)a.cached_x,(uint32_t)a.cached_y};
    for (unsigned k=0;k<4;k++) for (unsigned b=0;b<4;b++) image[0x3c+4*k+b]=(uint8_t)(p[k]>>(24-8*b));
    image[0x4c]=a.mark;image[0x4d]=a.correction;
    if (neo_boss_spawn(&w->boss,image)<0 || !insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_BOSS,0,(uint8_t)(w->order[at].depth+1u)})) {w->overflow++;return;}
    w->boss_active=true;w->spawned++;
}
static bool run_boss(tick_t *t) {
    neo_world_t *w=t->w;neo_boss_t *b=&w->boss;
    b->env.epoch=w->epoch;b->env.facing606=w->flag606;b->env.rank=RANK;b->env.players88=MODE88;
    b->env.alive_mask=t->players.active_mask;b->env.p1_node=t->player_valid;b->env.p2_node=false;
    b->env.p1x=(int16_t)t->player.x;b->env.p1y=(int16_t)t->player.y;b->env.p2x=b->env.p2y=0;
    b->env.mission_over=w->mission_step!=0;b->env.frame=t->genv.video;
    boss_tick_ctx=t;
    w->boss_box_count=0;
    /* neo_boss_tick, continuing after a node that stops on an untranslated
     * path: that node is removed (logged), the others keep running. */
    /* List order at the start of the pass: when a node is freed during its
     * update, 00424A follows stale +8 links (a converted shell takes its
     * trails with it), i.e. the next node of this order still linked. */
    int16_t order[NEO_BOSS_NODES];uint8_t pos[NEO_BOSS_NODES];unsigned count=0;
    for (int16_t k=b->first;k>=0 && count<NEO_BOSS_NODES;k=b->nodes[k].next) {pos[k]=(uint8_t)count;order[count++]=k;}
    bool listed[NEO_BOSS_NODES]={false};
    for (unsigned k=0;k<count;k++) listed[order[k]]=true;
    for (int16_t i=b->first;i>=0;) {
        int16_t next=-1;uint32_t routine=b->nodes[i].routine;
        neo_boss_result_t r=neo_boss_step(b,i);
        if (r==NEO_BOSS_UNSUPPORTED) {
            w->boss_unsupported++;w->boss_unsupported_label=b->unsupported_label;w->boss_unsupported_routine=routine;
            w->unsupported++;w->unsupported_routine=b->unsupported_label;
            neo_boss_stop(b,i);
        }
        if (b->nodes[i].routine) next=b->nodes[i].next;
        else if (listed[i]) for (unsigned k=pos[i]+1u;k<count;k++) if (b->nodes[order[k]].routine) {next=order[k];break;}
        i=next;
    }
    boss_tick_ctx=NULL;
    /* The block stays in the list while one boss node lives. */
    return b->first>=0;
}
/* Player shot against the boss nodes registered during the previous update
 * (004762 narrow phase with the attack row, then 004892..0049E0). */
static bool boss_shot(neo_world_t *w,const neo_contact_actor_t *a,const int16_t at[7]) {
    bool any=false;
    for (unsigned k=0;k<w->last_boss_box_count;k++) {
        int16_t i=w->last_boss_boxes[k];const neo_boss_node_t *n=&w->boss.nodes[i];
        if (!n->routine || !neo_boss_l(n,0x50)) continue;
        const uint8_t *d=neo_boss_rom(neo_boss_l(n,0x50),12);
        if (!d || !((d[1]>>(unsigned)at[5])&1u)) continue;
        uint16_t x=(uint16_t)at[0],y=(uint16_t)at[1];
        if (a->flags15&1u) x=(uint16_t)(0u-(uint16_t)(x+(uint16_t)at[2]));
        x=(uint16_t)(x+HI(a->projected_x));
        if (a->flags15&2u) y=(uint16_t)(0u-(uint16_t)(y+(uint16_t)at[3]));
        y=(uint16_t)(y+HI(a->projected_y));
        neo_hit_rectangle_t q={x,y,(uint16_t)at[2],(uint16_t)at[3]};
        uint16_t ix=0,iy=0;bool hit=false;
        for (unsigned o=2;(d[o]<<8|d[o+1])!=0x8000 && !hit;o+=8) {
            neo_hit_rectangle_t local={(uint16_t)(d[o]<<8|d[o+1]),(uint16_t)(d[o+2]<<8|d[o+3]),(uint16_t)(d[o+4]<<8|d[o+5]),(uint16_t)(d[o+6]<<8|d[o+7])};
            hit=neo_hit_rectangle(&q,neo_boss_w(n,0x3c),neo_boss_w(n,0x40),n->f[0x15],&local,&ix,&iy);
        }
        if (!hit) continue;
        any=true;
        uint8_t kind=(uint8_t)at[4];
        if (kind>=20u || !soldier_hit_matrix[d[0]&7u][kind]) continue;
        neo_boss_hit_t h={a->projected_x,a->projected_y,a->cached_x,a->cached_y,kind,(uint8_t)at[5],1,(uint16_t)at[6],(int16_t)ix,(int16_t)iy,false,0};
        const uint8_t *mul=neo_boss_l(n,0x54)?neo_boss_rom(neo_boss_l(n,0x54)+2u*kind,2):NULL;
        if (mul) {h.scale_row=true;h.scale=(uint16_t)(mul[0]<<8|mul[1]);}
        if (neo_boss_receive(&w->boss,i,&h)) w->boss_hits++;
    }
    return any;
}
/* ---- actors ------------------------------------------------------------------ */
static bool run_soldier(tick_t *t,unsigned i) {
    neo_world_t *w=t->w;neo_soldier_t *s=&w->soldiers[i];
    t->env.event_context=t;
    if (!w->constructed[i]) {
        if (!neo_soldier_construct(s,&w->pending[i],&t->env)) {w->unsupported++;w->unsupported_routine=SOLDIER_CONSTRUCTOR;s->active=false;return false;}
        w->constructed[i]=true;return true;
    }
    uint8_t group=s->group69;
    /* 004BAC while RAM 1067E2 is set (collapse of the domed building): the
     * soldier's registration (040A00 -> 004C24) kills it instead. Applied
     * before its update; the original does it inside, at the registration. */
    neo_hurtbox_t kill_box;
    if (w->kill_active && neo_soldier_hurtbox(s,&kill_box)) {s->health=0;s->b.flags5f|=0x80u;s->b.timer5a=6;w->killed_all++;}
    t->draw_palette=s->palette_resource;
    t->draw_kind=NEO_WORLD_SOLDIER;
    t->draw_flash=false;
    unsigned before=w->draw_count;
    neo_soldier_result_t r=neo_soldier_tick(s,&t->env);
    /* Palette/flash as left by this update (004392 reads +17 after the step). */
    for (unsigned k=before;k<w->draw_count;k++) {
        w->draws[k].palette=s->palette_resource;
        w->draws[k].flash=(s->anim.flash&1u) && !(s->anim.flash&0x80u);
    }
    if (group<9u && s->group69>=9u) w->kills++;
    if (r==NEO_SOLDIER_UNSUPPORTED) {w->unsupported++;w->unsupported_routine=s->routine;w->draw_count=before;s->active=false;return false;}
    if (r==NEO_SOLDIER_DELETED) {s->active=false;return false;}
    /* 040A00 -> 004C24: the soldier's slot in the registry. */
    neo_hurtbox_t hb;
    if (neo_soldier_hurtbox(s,&hb)) {
        int16_t x=hb.rect[0],y=hb.rect[1];
        if (hb.flags15&1u) x=(int16_t)-(x+hb.rect[2]);
        if (hb.flags15&2u) y=(int16_t)-(y+hb.rect[3]);
        uint16_t xb=0;uint8_t yb=0;
        if (clip_bins((int16_t)(x+(int16_t)hb.x),(int16_t)(y+(int16_t)hb.y),hb.rect[2],hb.rect[3],&xb,&yb)) reg_add(w,NEO_WORLD_SOLDIER,i,xb,yb);
    }
    return true;
}
static bool run_splash(tick_t *t,unsigned i) {
    neo_world_t *w=t->w;neo_world_splash_t *s=&w->splashes[i];
    if (!s->started) { /* 040F88 */
        neo_anim_start(&s->anim,program(SPLASH_PROGRAM),0x100);
        s->anim.flash=0xff;s->body.field66=0xe0ed;s->body.word1e=0xe0ed;s->started=true;
    }
    /* 040FD4: 01ABC8 projection, 00427A; finished -> 004188. */
    neo_contact_project(&s->body,&w->root,w->epoch);
    s->render.x=HI(s->body.projected_x);s->render.y=HI(s->body.projected_y);
    s->render.attributes=(uint16_t)((s->render.attributes&0xff00u)|s->body.flags15);
    s->render.z=s->body.word1e;s->render.correction=s->body.correction;
    t->draw_palette=neo_blood_palette(NEO_BLOOD_SPLASH,w->blood);t->draw_flash=false;t->draw_kind=NEO_WORLD_SPLASH;
    neo_anim_result_t r=neo_anim_step(&s->anim,&s->render,emit,t,1024);
    if (r!=NEO_ANIM_MORE) {s->active=false;return false;}
    return true;
}
static bool run_throw(tick_t *t,unsigned i) {
    neo_world_t *w=t->w;neo_throw_actor_t *o=&w->throws[i];
    neo_throw_services_t services={program(0x23f3f0),program(0x22d2a6),soldier_throw_flight_rect,soldier_throw_burst_rect,
        throw_palette,throw_release,w,throw_air,throw_hit};
    /* The effect phase projects against the object's current parent. */
    const neo_contact_parent_t *scene=o->body.parent_tag==TERRAIN_TAG?&w->terrain:&w->root;
    neo_throw_frame_t frame={scene,&t->players,&t->bounds,w->video_phase,0,w->epoch,false};
    unsigned before=w->draw_count;
    t->draw_kind=NEO_WORLD_THROW;t->draw_flash=false;t->draw_palette=0;
    throw_tick_ctx=t;
    unsigned events;neo_throw_result_t r=neo_throw_actor_step(o,&frame,&services,emit,t,&events);
    throw_tick_ctx=NULL;
    if (events&NEO_THROW_AUDIO_1027) sound_request(w,0x1027); /* 03BE52 burst on terrain contact */
    for (unsigned k=before;k<w->draw_count;k++) {
        uint8_t handle=(uint8_t)(w->draws[k].attributes>>8);
        w->draws[k].palette=handle<COUNT(throw_palette_ids)?throw_palette_ids[handle]:0;
    }
    if (r==NEO_THROW_UNSUPPORTED) {w->unsupported++;w->unsupported_routine=0x3bd3a;w->draw_count=before;o->active=false;return false;}
    return r==NEO_THROW_MORE;
}
/* ROM 0041AC: the following nodes deeper than this one are deleted with it
 * (without running their own exit code). */
static void delete_tree(tick_t *t,unsigned at) {
    neo_world_t *w=t->w;uint8_t depth=w->order[at].depth;
    while (at+1u<w->order_count && w->order[at+1u].depth>depth) {
        neo_world_slot_t s=w->order[at+1u];
        switch (s.kind) {
        case NEO_WORLD_SOLDIER: w->soldiers[s.index].active=false;break;
        case NEO_WORLD_THROW: w->throws[s.index].active=false;break;
        case NEO_WORLD_SPLASH: w->splashes[s.index].active=false;break;
        case NEO_WORLD_GACTOR: ga_kill(t,&w->gactors[s.index]);break;
        case NEO_WORLD_RUNNER: w->script.runners[s.index].active=false;break;
        case NEO_WORLD_BOSS: w->boss_active=false;break;
        default:break;
        }
        remove_at(w,at+1u);
    }
}
/* Sub-script actor 019E72/019E82 (a generic actor) running the record at
 * ROM `record` with the level-script interpreter; true once its op 3 ran. */
static bool g_subscript(void *ctx,neo_gactor_t *a,uint32_t record) {
    tick_t *t=ctx;neo_world_t *w=t->w;
    if (!t->se) return false;
    neo_world_sub_t *sub=NULL;
    for (unsigned i=0;i<NEO_WORLD_SUBSCRIPTS && !sub;i++) if (w->subs[i].used && w->subs[i].owner==a->index) sub=&w->subs[i];
    if (!sub) {
        for (unsigned i=0;i<NEO_WORLD_SUBSCRIPTS && !sub;i++) if (!w->subs[i].used) sub=&w->subs[i];
        const uint8_t *code=ga_rom(record);
        if (!sub || !code) {ga_fail(&t->genv,record);return false;}
        sub->used=true;sub->owner=a->index;
        neo_script_init(&sub->script,code,0x80,record);
    }
    /* RAM 10C326..: the live counts of this tick. */
    memcpy(sub->script.groups,w->script.groups,sizeof(sub->script.groups));
    int saved=t->sub_owner;t->sub_owner=a->index;
    neo_script_tick(&sub->script,t->se);
    t->sub_owner=saved;
    if (sub->script.halted) {ga_fail(&t->genv,sub->script.halt_address);return false;}
    return sub->script.finished;
}
/* ROM 004192: the running actor deletes the deeper nodes that follow it. */
static bool g_has_children(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;int at=find(t->w,NEO_WORLD_GACTOR,(unsigned)a->index);
    return at>=0 && (unsigned)at+1u<t->w->order_count && t->w->order[at+1].depth>t->w->order[at].depth;
}
static void g_kill_children(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;int at=find(t->w,NEO_WORLD_GACTOR,(unsigned)a->index);
    if (at>=0) delete_tree(t,(unsigned)at);
}
/* ---- public ------------------------------------------------------------------ */
static void scene_at(neo_world_t *w,int32_t x,bool reset) {
    neo_contact_parent_t *p[2]={&w->terrain,&w->root};
    for (unsigned i=0;i<2;i++) {
        p[i]->previous_x=reset?x:p[i]->x;p[i]->previous_y=reset?w->scroll_y:p[i]->y;p[i]->x=x;p[i]->y=w->scroll_y;p[i]->correction=0;
    }
    w->kernel.terrain=(neo_kobj_t){w->terrain.x,w->terrain.y,w->terrain.previous_x,w->terrain.previous_y};
    w->kernel.root=(neo_kobj_t){w->root.x,w->root.y,w->root.previous_x,w->root.previous_y};
    w->kernel.ring=neo_collision_ring(w->terrain.x,w->terrain.y);
    /* RAM 10C00E (camera op 0x50): the cell group map of 012938. Mission 1
     * keeps its table; Mission 2's 0B7000 maps group 6 to 0x0A (ROM bytes
     * 00 01 02 03 04 05 0A 07). */
    static const uint8_t mission2_group_map[8]={0,1,2,3,4,5,10,7};
    w->kernel.group_map=w->camera.l400e==0x0B7000u?mission2_group_map:soldier_group_map;
    w->scroll_x=x;
}
void neo_world_init(neo_world_t *w,int32_t scroll_x) {
    memset(w,0,sizeof(*w));
    neo_script_init(&w->script,mission1_script,sizeof(mission1_script),MISSION1_SCRIPT_BASE);
    memcpy(w->rng.words,mission1_handoff_rng,sizeof(w->rng.words));w->rng.cursor=MISSION1_HANDOFF_RNG_CURSOR;
    w->terrain.tag=TERRAIN_TAG;w->root.tag=ROOT_TAG;
    for (unsigned i=0;i<NEO_WORLD_SOLIDS;i++) w->solids[i].actor=-1;  /* 004624: table 106756 cleared */
    w->kernel.group_map=soldier_group_map;
    w->epoch=1;w->camera_lock=-1;w->scroll_y=SCENE_Y;
    w->flag606=0xff;   /* RAM 10C606 when the boss spawns (both recorded fights) */
    /* 0C0E20 (new game) and 0C0E6E (player 1's first life); RAM 10C2F6 reads 0 in Mission 1. */
    marco_weapons_new_game(&w->weapons);w->weapon_player.exists=true;
    neo_vslot_reset_all(&w->vslots);w->vslots.record[0]=w->vslots.record[1]=NEO_VSLOT_RECORD;
    marco_weapons_life(&w->weapons,0,&w->weapon_player);w->pickup.mission=0;
    scene_at(w,scroll_x,true);
    w->order[w->order_count++]=(neo_world_slot_t){NEO_WORLD_SCRIPT,0,0};
    /* RAM 10E9D2..10E9D8 as observed in the region (0C8BEC). */
    w->view[0]=0;w->view[1]=319;w->view[2]=256;w->view[3]=511;
    w->blood=MSX_BLOOD!=0;  /* RAM 10FD8F */
}
unsigned neo_world_tick(neo_world_t *w,int32_t scroll_x,const neo_world_player_t *pl) {
    tick_t t;memset(&t,0,sizeof(t));t.w=w;
    w->ticks++;
    w->epoch=w->epoch==1?2:1;               /* RAM 10C33A */
    scene_at(w,scroll_x,false);
    uint16_t camera=w->camera_native?neo_camera_progress(&w->camera):(uint16_t)((0u-(uint32_t)scroll_x)>>16); /* RAM 10E99E */
    w->draw_count=0;
    /* Player as the others see it (registered during its previous update). */
    t.player_valid=pl && pl->alive;
    t.player_hittable=t.player_valid && !pl->unregistered;
    uint16_t px=pl?pl->px:0,py=pl?pl->py:0;
    t.player=(neo_hurtbox_t){px,py,pl?pl->flags15:0,soldier_player_hurtbox.type,0,
        {soldier_player_hurtbox.rect[0],soldier_player_hurtbox.rect[1],soldier_player_hurtbox.rect[2],soldier_player_hurtbox.rect[3]},pl && pl->dead};
    /* ROM 01903A: a player with +7A/+7B clear is replaced by the script's
     * point RAM 69B8/69BA. */
    uint16_t tx=pl && !pl->follow_x?w->script.w69b8:px,ty=pl && !pl->follow_y?w->script.w69ba:py;
    t.players=(neo_control_frame_t){0,0,0,0,tx,ty,0,0,(uint8_t)(t.player_valid && !pl->inactive?1:0),MODE88,RANK,t.player_valid,w->grid};
    t.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};
    neo_soldier_env_t *e=&t.env;
    e->kernel=&w->kernel;e->terrain=w->terrain;e->root=w->root;e->epoch=w->epoch;e->players=t.players;
    e->grid=w->grid;e->rng=&w->rng;e->targets=t.player_hittable?&t.player:NULL;e->target_count=t.player_hittable?1:0;
    e->bounds=t.bounds;e->camera_word=HI(w->terrain.x);e->program=program;e->emit=emit;e->emit_context=&t;
    e->event=soldier_event;e->event_context=&t;e->blood=w->blood;e->dyn=w->dyn_parent;
    e->player_state[0].alive=t.player_valid;e->player_state[0].follow7a=pl?pl->follow_x:0xff;
    e->player_state[0].follow7b=pl?pl->follow_y:0xff;e->player_state[0].x=px;e->player_state[0].y=py;
    e->point[0]=w->script.w69b8;e->point[1]=w->script.w69ba;
    neo_genv_t *g=&t.genv;
    g->kernel=&w->kernel;g->epoch=w->epoch;g->rng=&w->rng;g->players=t.players;g->bounds=t.bounds;
    /* RAM 10000E as the actors read it: the counter has already advanced for
     * this update (video_phase is the value before it). */
    g->video=(uint16_t)(w->video_phase+1u);g->camera=camera;g->scroll_x=w->terrain.x;g->scroll_y=w->terrain.y;g->grid=w->grid;g->ctx=&t;
    g->parent=g_parent;g->program=g_program;g->emit=emit;g->emit_ctx=&t;g->spawn=g_spawn;g->sound=g_sound;
    g->attack=g_attack;g->register_box=g_register;g->dyn_set=g_dyn_set;g->dyn_release=g_dyn_release;g->solid=g_solid;
    g->creator_alive=g_creator_alive;g->creator=g_creator;g->actor=g_actor;g->previous=g_previous;g->stamp=g_stamp;g->map_patch=g_map_patch;
    g->marker=g_marker;g->marker_seq=&w->marker_seq;
    g->groups=w->script.groups;g->flag606=&w->flag606;g->external=g_external;
    /* Pickup state (neo_item.c); RAM 10C590 counts the logic updates. */
    w->pickup.weapons=&w->weapons;w->pickup.player[0]=t.player_valid?&w->weapon_player:NULL;w->pickup.player[1]=NULL;
    w->pickup.slot[0]=w->vslots.slot[0].b;w->pickup.slot[1]=NULL;w->pickup.frame++;g->pickup=&w->pickup;
    /* The player's vehicles (Camel Slug 03458E): slots, the players' node
     * records, input words. Player 1's 0C3352 -> 0AA458 is the port
     * player's, after this tick (the original 1014C4 runs after the Camel). */
    {
        neo_vehicle_env_t *v=&w->vehicle_env;unsigned grenades=v->grenades;uint32_t last=v->grenade_last;
        memset(v,0,sizeof(*v));v->grenades=grenades;v->grenade_last=last;
        v->slots=&w->vslots;v->player[0]=w->player_node[0];v->player[1]=w->player_node[1];
        v->exists[0]=t.player_valid;v->exists[1]=false;
        v->input[0][0]=w->pad[0];v->input[0][1]=w->pad[1];v->dip10f000=w->dip10f000;
        for (unsigned i=0;i<NEO_WORLD_SOLIDS;i++) if (w->solids[i].actor>=0) {
            v->solids++;v->solid_node[i]=w->gactors[w->solids[i].actor].m;v->solid_desc[i]=w->solids[i].desc;
        }
        g->vehicle=v;
    }
    w->kernel.shake_y=w->camera_native?(uint32_t)w->camera.shake_applied:0;   /* RAM 10E9CC (0138A0) */
    /* RAM 10C5EC/10C5F4: item pickup boxes of the players (the original
     * player's code writes them). The port's player keeps the standing box
     * of the captures (FFF8,0000,0010,0020 in 160 of 175 RAM dumps; the
     * others are the tank and one more pose the port has not); no player 2. */
    for (unsigned i=0;i<8;i++) g->pickup_box[i]=0x8000u;
    if (t.player_valid) {g->pickup_box[0]=0xfff8u;g->pickup_box[1]=0;g->pickup_box[2]=0x10;g->pickup_box[3]=0x20;}
    if (w->pickup_input) memcpy(g->pickup_box,w->pickup_input,sizeof(g->pickup_box));
    g->terrain_tag=camera>=1023u?0x10d7b6u:0x10c72eu;
    g->flags=&w->ga_flags;g->camera_mode=g_camera_mode;g->camera_rise=g_camera_rise;g->shake=g_shake;
    g->map_swap=g_map_swap;g->front_layer=g_front_layer;g->kill_children=g_kill_children;g->has_children=g_has_children;g->subscript=g_subscript;
    w->marker_count[g->video&1u]=0;                          /* 0401E8 */
    e->markers=w->markers[(g->video&1u)^1u];e->marker_count=w->marker_count[(g->video&1u)^1u];
    g->markers=e->markers;g->marker_count=e->marker_count;
    memcpy(g->view,w->view,sizeof(g->view));g->flag448=0;g->blood=w->blood;
    /* Hurtboxes registered during the previous update are the ones queried
     * now (ROM 004C24 double buffer). */
    memcpy(w->last_boxes,w->boxes,sizeof(w->boxes));w->last_box_count=w->box_count;w->box_count=0;
    memcpy(w->last_regs,w->regs,sizeof(w->regs));w->last_reg_count=w->reg_count;w->reg_count=0;
    /* End of the camera update (ROM 01406A): dynamic collision objects. */
    neo_kernel_dyn_begin(&w->kernel);
    for (int i=NEO_KDYN_COUNT-1;i>=0;i--) {
        if (w->kernel.dyn[i].countdown) {w->kernel.dyn[i].countdown--;continue;}
        dyn_project(w,&t,(unsigned)i);
        if (w->kernel.dyn[i].map) neo_kernel_dyn_update(&w->kernel,(unsigned)i);
    }
    /* Mission actor (0186A0): grid decay 00469C, group recount 019E40. */
    neo_grid_decay(w->grid,&w->grid_cursor);
    w->kill_active=w->ga_flags.kill_request!=0;w->ga_flags.kill_request=0;   /* 0046D4 */
    w->shots_clear=w->ga_flags.flash_request!=0;w->ga_flags.flash_request=0;  /* 0046E0 */
    /* 004736: free the slots whose actor no longer holds them (+6B). */
    for (unsigned i=0;i<NEO_WORLD_SOLIDS;i++) {
        int16_t o=w->solids[i].actor;
        if (o>=0 && ga_b(&w->gactors[o],0x6b)!=i) w->solids[i].actor=-1;
    }
    g->kill_all=w->kill_active;g->clear_shots=w->shots_clear;
    /* Map swap actor 0B595A (inserted after the mission actor). */
    if (w->swap_step==2) {w->ga_flags.collapsed=1;w->ga_flags.flash=0;w->swap_step=3;}
    else if (w->swap_step==1) {w->ga_flags.flash=1;w->swap_step=2;}
    memset(w->script.groups,0,sizeof(w->script.groups));
    for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++)
        if (w->soldiers[i].active && w->constructed[i] && w->soldiers[i].group69<9u) w->script.groups[w->soldiers[i].group69]++;
    for (unsigned i=0;i<NEO_WORLD_GACTORS;i++)
        if (w->gactors[i].active && ga_b(&w->gactors[i],0x69)<9u) w->script.groups[ga_b(&w->gactors[i],0x69)]++;
    if (w->boss_active) for (int16_t i=w->boss.first;i>=0;i=w->boss.nodes[i].next)
        if (w->boss.nodes[i].routine && w->boss.nodes[i].routine!=0x3f1eu && w->boss.nodes[i].f[0x69]<9u) w->script.groups[w->boss.nodes[i].f[0x69]]++;
    memcpy(w->last_boss_boxes,w->boss_boxes,sizeof(w->boss_boxes));w->last_boss_box_count=w->boss_box_count;
    neo_script_env_t se={camera,w->terrain.x,w->terrain.y,
        {{t.player_valid,pl?pl->follow_x:0,pl?pl->follow_y:0,px,py},{false,0,0,0,0}},
        script_rng,script_spawn,script_runner,script_camera_lock,script_camera_release,script_autoscroll,&t};
    t.se=&se;t.sub_owner=-1;
    for (unsigned at=0;at<w->order_count;) {
        neo_world_slot_t slot=w->order[at];t.current=(int)at;bool keep=true,tree=false;
        if (slot.kind==NEO_WORLD_GACTOR && w->gactors[slot.index].placeholder) {
            /* 00417E: freed once no deeper node follows it. */
            if (at+1u<w->order_count && w->order[at+1u].depth>slot.depth) {at++;continue;}
            w->gactors[slot.index].placeholder=false;w->gactors[slot.index].active=false;
            remove_at(w,at);continue;
        }
        if (w->trace) w->trace(w->trace_context,slot.kind,slot.index);
        /* The running actor (A6) for the dynamic-object exclusions of 01424C:
         * only generic actors own dynamic objects or carry others. */
        w->kernel.self_dyn=0;w->kernel.self_block=0x60000000u|(uint32_t)slot.kind<<8|slot.index;
        switch (slot.kind) {
        case NEO_WORLD_SCRIPT: if (!w->ga_flags.mission_end) neo_script_tick(&w->script,&se);break;   /* 018F92 deletes the script actor (0041D0) */
        case NEO_WORLD_RUNNER: keep=neo_script_tick_runner(&w->script,slot.index,&se);break;
        case NEO_WORLD_SOLDIER: keep=run_soldier(&t,slot.index);break;
        case NEO_WORLD_THROW: keep=run_throw(&t,slot.index);break;
        case NEO_WORLD_SPLASH: keep=run_splash(&t,slot.index);break;
        case NEO_WORLD_GACTOR: {int r=run_gactor(&t,slot.index);keep=r==1;tree=r==2;break;}
        case NEO_WORLD_BOSS: keep=run_boss(&t);if (!keep) w->boss_active=false;break;
        }
        /* The running actor may have inserted others after itself. */
        int now=find(w,slot.kind,slot.index);
        if (now<0) now=(int)at;
        if (tree) delete_tree(&t,(unsigned)now);
        if (!keep && slot.kind==NEO_WORLD_GACTOR && (unsigned)now+1u<w->order_count && w->order[now+1].depth>slot.depth) {
            /* 004126: a node followed by deeper ones stays as a placeholder. */
            neo_gactor_t *g=&w->gactors[slot.index];
            g->active=true;g->placeholder=true;ga_sl(g,0,0x3f1e);keep=true;
        } else if (!keep && slot.kind!=NEO_WORLD_GACTOR && (unsigned)now+1u<w->order_count && w->order[now+1].depth>slot.depth) {
            /* 004126 for any node (a script runner whose soldier follows
             * it): the slot stays as a placeholder generic actor, so a tree
             * deletion (0041AC) before it stops at its depth. */
            neo_gactor_t *g=ga_alloc(w,0x3f1e,0,0,slot.depth);
            if (g) {g->placeholder=true;w->order[now].kind=NEO_WORLD_GACTOR;w->order[now].index=(uint8_t)g->index;keep=true;}
        }
        if (!keep) {remove_at(w,(unsigned)now);at=(unsigned)now;}
        else at=(unsigned)now+1u;
    }
    /* Player shots of this update (00411C after the list head). */
    for (unsigned i=0;i<w->shot_request_count;i++) {
        neo_gactor_t *a=ga_insert(&t,find(w,NEO_WORLD_SCRIPT,0),w->shot_requests[i].routine,SCRIPT_TAG,0);
        const neo_world_shot_request_t *q=&w->shot_requests[i];
        if (!a) {
            w->shot_dropped++;
            if (q->routine==0x63f32u && q->player<2u) {                  /* 0C4526: 0C0DFC */
                marco_ammo_t m=marco_weapons_ammo(&w->weapons,q->player);marco_ammo_release(&m,0x0c);marco_weapons_set_ammo(&w->weapons,q->player,&m);
            }
            continue;
        }
        if (q->actor) {
            ga_sb(a,0x6e,q->angle);ga_sw(a,0x70,q->player);ga_sb(a,0x15,q->flags15);ga_sb(a,0x5e,q->f5e);
            ga_sl(a,0x28,(uint32_t)q->x);ga_sl(a,0x2c,(uint32_t)q->y);ga_sl(a,0x30,(uint32_t)q->vx);ga_sl(a,0x34,(uint32_t)q->vy);ga_sl(a,0x38,ROOT_TAG);
            w->player_shots++;continue;
        }
        if (q->grenade) {ga_sw(a,0x6e,q->player);ga_sb(a,0x15,q->flags15);ga_sl(a,0x30,(uint32_t)q->vx);ga_sl(a,0x34,(uint32_t)q->vy);}
        else {ga_sb(a,0x6e,q->angle);ga_sw(a,0x70,q->player);}
        ga_sb(a,0x5e,0);
        ga_sl(a,0x28,(uint32_t)w->shot_requests[i].x);ga_sl(a,0x2c,(uint32_t)w->shot_requests[i].y);ga_sl(a,0x38,ROOT_TAG);
        w->player_shots++;
    }
    w->shot_request_count=0;
    w->player_hits+=t.hits;
    return t.hits;
}
bool neo_world_player_shot(neo_world_t *w,uint32_t constructor,int32_t x,int32_t y,uint8_t angle,uint8_t player) {
    if (w->shot_request_count>=sizeof(w->shot_requests)/sizeof(w->shot_requests[0])) {w->shot_dropped++;return false;}
    w->shot_requests[w->shot_request_count++]=(neo_world_shot_request_t){constructor,x,y,0,0,angle,player,0,false,false,0};
    return true;
}
bool neo_world_player_grenade(neo_world_t *w,uint32_t constructor,int32_t x,int32_t y,int32_t vx,int32_t vy,bool left,uint8_t player) {
    if (w->shot_request_count>=sizeof(w->shot_requests)/sizeof(w->shot_requests[0])) {w->shot_dropped++;return false;}
    w->shot_requests[w->shot_request_count++]=(neo_world_shot_request_t){constructor,x,y,vx,vy,0,player,(uint8_t)(left?1u:0u),true,false,0};
    return true;
}
bool neo_world_player_actor(neo_world_t *w,uint32_t constructor,int32_t x,int32_t y,int32_t vx,int32_t vy,uint8_t angle,uint8_t flags15,uint8_t f5e,uint8_t player) {
    if (w->shot_request_count>=sizeof(w->shot_requests)/sizeof(w->shot_requests[0])) {w->shot_dropped++;return false;}
    w->shot_requests[w->shot_request_count++]=(neo_world_shot_request_t){constructor,x,y,vx,vy,angle,player,flags15,false,true,f5e};
    return true;
}
bool neo_world_shot_row(neo_world_t *w,int32_t previous_x,int32_t previous_y,int32_t x,int32_t y,unsigned rowi,bool left) {
    neo_contact_actor_t b;memset(&b,0,sizeof(b));
    b.projected_x=(int32_t)((uint32_t)x+(uint32_t)w->terrain.x);b.projected_y=(int32_t)((uint32_t)y+(uint32_t)w->terrain.y);
    b.cached_x=(int32_t)((uint32_t)previous_x+(uint32_t)w->terrain.x);b.cached_y=(int32_t)((uint32_t)previous_y+(uint32_t)w->terrain.y);
    b.flags15=(uint8_t)(32u|(left?1u:0u));
    const int16_t *row=soldier_pistol_attacks[rowi&15u];
    bool hit=false;
    for (unsigned at=0;at<w->order_count;at++) {
        if (w->order[at].kind!=NEO_WORLD_SOLDIER) continue;
        unsigned i=w->order[at].index;
        if (!w->constructed[i]) continue;
        hit|=neo_soldier_receive(&w->soldiers[i],&b,1,row,w->grid);
    }
    /* The same attack row (0049E8) against the generic actors registered
     * with class 1 during the previous update (vehicles, bomber, ...). */
    uint16_t qx;uint8_t qy;
    if (attack_bins(&b,row,&qx,&qy)) for (unsigned k=0;k<w->last_box_count;k++) {
        neo_gactor_t *g=&w->gactors[w->last_boxes[k].owner];
        if (!(w->last_boxes[k].xbins&qx) || !(w->last_boxes[k].ybins&qy)) continue;
        if (!g->active || g->placeholder || !ga_l(g,0x50) || !ga_class(g,(unsigned)row[5])) continue;
        hit|=ga_receive(g,&b,1,row,w->grid);
    }
    if (w->boss_active) hit|=boss_shot(w,&b,row);
    if (w->shot_hook) hit|=w->shot_hook(w->shot_context,&b,row);
    return hit;
}
uint8_t neo_world_player_attack(neo_world_t *w,int32_t x,int32_t y,bool left,const int16_t row[7]) {
    neo_contact_actor_t b;memset(&b,0,sizeof(b));
    b.projected_x=b.cached_x=(int32_t)((uint32_t)x+(uint32_t)w->terrain.x);
    b.projected_y=b.cached_y=(int32_t)((uint32_t)y+(uint32_t)w->terrain.y);
    b.flags15=(uint8_t)(32u|(left?1u:0u));
    uint8_t best=0;
    for (unsigned at=0;at<w->order_count;at++) {
        if (w->order[at].kind!=NEO_WORLD_SOLDIER || !w->constructed[w->order[at].index]) continue;
        neo_soldier_t *s=&w->soldiers[w->order[at].index];neo_hurtbox_t hb;
        if (neo_soldier_hurtbox(s,&hb) && neo_soldier_receive(s,&b,1,row,w->grid) && hb.type>best) best=hb.type;
    }
    uint16_t qx;uint8_t qy;
    if (!attack_bins(&b,row,&qx,&qy)) return best;
    for (unsigned k=0;k<w->last_box_count;k++) {
        neo_gactor_t *g=&w->gactors[w->last_boxes[k].owner];
        if (!(w->last_boxes[k].xbins&qx) || !(w->last_boxes[k].ybins&qy)) continue;
        if (!g->active || g->placeholder || !ga_l(g,0x50) || !ga_class(g,(unsigned)row[5])) continue;
        if (ga_receive(g,&b,1,row,w->grid)) {uint8_t ty=ga_rom(ga_l(g,0x50))[0];if (ty>best) best=ty;}
    }
    return best;
}
bool neo_world_shot(neo_world_t *w,int32_t previous_x,int32_t x,int32_t y,bool left) {
    return neo_world_shot_row(w,previous_x,y,x,y,0,left);
}
bool neo_world_hit_actor(neo_world_t *w,unsigned index,const neo_contact_actor_t *attacker,uint8_t a5e,const int16_t row[7]) {
    if (index>=NEO_WORLD_GACTORS || row[5]<0) return false;
    neo_gactor_t *g=&w->gactors[index];
    bool registered=false;uint16_t qx;uint8_t qy;
    if (!attack_bins(attacker,row,&qx,&qy)) return false;
    for (unsigned b=0;b<w->last_box_count;b++)
        registered|=w->last_boxes[b].owner==index && (w->last_boxes[b].xbins&qx) && (w->last_boxes[b].ybins&qy);
    if (!registered || !g->active || !ga_l(g,0x50) || !ga_class(g,(unsigned)row[5])) return false;
    return ga_receive(g,attacker,a5e,row,w->grid);
}
void neo_world_init_stage(neo_world_t *w,int32_t scroll_x,const neo_world_stage_t *st) {
    neo_rng_t rng=w->rng;marco_weapons_t weapons=w->weapons;neo_pickup_env_t pickup=w->pickup;neo_ga_flags_t flags=w->ga_flags;
    neo_world_init(w,scroll_x);
    w->rng=rng;w->weapons=weapons;w->pickup=pickup;
    memcpy(w->ga_flags.rescued,flags.rescued,sizeof(flags.rescued));memcpy(w->ga_flags.special,flags.special,sizeof(flags.special));memcpy(w->ga_flags.gifts,flags.gifts,sizeof(flags.gifts));
    neo_script_init(&w->script,st->script,st->script_size,st->script_base);
    w->stage_camera=st->camera;w->stage_camera_size=st->camera_size;w->stage_camera_base=st->camera_base;
}
void neo_world_camera(neo_world_t *w,const neo_world_player_t *pl) {
    if (!w->camera_native && w->stage_camera) {
        /* 0C8774 with 69D0 = 0 (a stage start): the whole list from its
         * first command; its point (op 34) sets 69B8/69BA. */
        neo_camera_init(&w->camera,w->stage_camera,w->stage_camera_size,w->stage_camera_base);w->camera_native=true;
        neo_camera_commands(&w->camera,&w->script.w69b8,&w->script.w69ba);
    }
    if (!w->camera_native) {
        const uint8_t *list=ga_rom(0x0B57BE);
        if (!list) return;
        neo_camera_init(&w->camera,list,0xB2,0x0B57BE);w->camera_native=true;
        /* State of the first mission-actor update (data/camera_fixture.json.gz):
         * point 69B8/69BA = 128/384 and the list's first wait (944) armed. */
        w->script.w69b8=0x80;w->script.w69ba=0x180;
        neo_camera_commands(&w->camera,&w->script.w69b8,&w->script.w69ba);
    }
    neo_camera_player_t p[2]={{pl && pl->alive,pl?pl->follow_x:0,pl?pl->follow_y:0,pl?pl->px:0,pl?pl->py:0},{false,0,0,0,0}};
    neo_camera_update(&w->camera,p,true,&w->script.w69b8,&w->script.w69ba);
}
bool neo_world_region_clear(const neo_world_t *w) {return neo_script_reached(&w->script,MISSION1_REGION_END);}
unsigned neo_world_alive(const neo_world_t *w) {
    unsigned n=0;
    for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++) if (w->soldiers[i].active && w->constructed[i] && w->soldiers[i].group69<9u) n++;
    return n;
}
