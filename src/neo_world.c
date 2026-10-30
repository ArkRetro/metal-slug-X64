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
/* Observed settings of every capture: RAM 100088 = 2, 10FD8B = 3, one player;
 * RAM 10FD8F = 0 (01B2BE carry set: splash palette 03BA). */
enum { MODE88=2, RANK=3, SPLASH_PALETTE=0x3ba, SPLASH_PROGRAM=0x2402fa };
static uint16_t HI(int32_t v) {return (uint16_t)((uint32_t)v>>16);}
static const neo_anim_program_t *program(uint32_t tag) {
    for (unsigned i=0;i<COUNT(soldier_programs);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i].program;
    return NULL;
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
} tick_t;
static void emit(void *context,const neo_anim_draw_t *d) {
    tick_t *t=context;neo_world_t *w=t->w;
    if (w->draw_count==NEO_WORLD_DRAWS) return;
    w->draws[w->draw_count++]=(neo_world_draw_t){d->descriptor,d->x,d->y,d->attributes,t->draw_palette,t->draw_flash,t->draw_kind};
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
    tick_t *t=context;(void)bounds;t->w->camera_lock=(int32_t)x;
}
static void script_camera_release(void *context) {tick_t *t=context;t->w->camera_lock=-1;}
static neo_gactor_t *ga_insert(tick_t *t,int at,uint32_t routine,uint32_t creator_tag,uint8_t v68);
static void script_spawn(void *context,const neo_script_spawn_t *sp) {
    tick_t *t=context;neo_world_t *w=t->w;
    int at=sp->creator<0?find(w,NEO_WORLD_SCRIPT,0):find(w,NEO_WORLD_RUNNER,(unsigned)sp->creator);
    uint32_t routine=sp->constructor&0xffffffu;
    if (routine!=SOLDIER_CONSTRUCTOR && at>=0 && neo_region_routine(routine)) {
        /* 01A10A: 0040BC after the creator, words to +6E.., +69 group,
         * +68 variant, +28/+2C at the scene root (01ABB6). */
        neo_gactor_t *a=ga_insert(t,at,routine,sp->creator<0?SCRIPT_TAG:RUNNER_TAG(sp->creator),0);
        if (!a) return;
        for (unsigned k=0;k<sp->word_count;k++) ga_sw(a,0x6eu + 2u*k,sp->words[k]);
        ga_sb(a,0x69,sp->group69);ga_sb(a,0x68,sp->variant68);
        ga_sl(a,0x28,(uint32_t)sp->x);ga_sl(a,0x2c,(uint32_t)sp->y);ga_sl(a,0x38,ROOT_TAG);ga_sw(a,0x4c,0);
        ga_project(a,&t->genv);
        return;
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
static void soldier_event(void *context,const neo_soldier_event_t *e) {
    tick_t *t=context;neo_world_t *w=t->w;
    switch (e->kind) {
    case NEO_SOLDIER_EVENT_KNIFE_HIT:
        t->hits++;
        if (t->current>=0 && w->order[t->current].kind==NEO_WORLD_SOLDIER) w->hit_x=HI(w->soldiers[w->order[t->current].index].b.projected_x);
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
static bool throw_air(void *context,neo_contact_actor_t *b,const neo_throw_frame_t *f) {
    tick_t *t=context;neo_world_t *w=t->w;(void)f;
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
    tick_t *t=context;
    if (!t->player_hittable) return false;
    int first=-1;
    uint8_t type=neo_hit_query(b,soldier_throw_attack,&t->player,1,(uint8_t)soldier_throw_attack[4],(uint8_t)soldier_throw_attack[5],&first);
    if (type && first>=0 && soldier_throw_attack[6]) {t->hits++;t->w->hit_x=HI(b->projected_x);}
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
static bool g_parent(void *ctx,uint32_t tag,neo_contact_parent_t *out) {
    tick_t *t=ctx;neo_world_t *w=t->w;
    if (tag==TERRAIN_TAG) {*out=w->terrain;return true;}
    if (tag==ROOT_TAG) {*out=w->root;return true;}
    if (tag>=NEO_DYN_TAG(0) && tag<NEO_DYN_TAG(NEO_KDYN_COUNT)) {*out=w->dyn_parent[(tag-NEO_DYN_TAG(0))/64u];return true;}
    if ((tag&0xff000000u)==NEO_GA_TAG_BASE && (tag&0xffffu)<NEO_WORLD_GACTORS) {
        const neo_gactor_t *a=&w->gactors[tag&0xffffu];
        *out=(neo_contact_parent_t){tag,(int32_t)ga_l(a,0x3c),(int32_t)ga_l(a,0x40),(int32_t)ga_l(a,0x44),(int32_t)ga_l(a,0x48),ga_b(a,0x4d)};
        return true;
    }
    return false;
}
static const neo_anim_program_t *g_program(uint32_t tag) {return program(tag);}
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
    return ga_insert(t,at,routine,after_script?SCRIPT_TAG:(creator?NEO_GA_TAG(creator->index):0),creator?ga_b(creator,0x68):0);
}
static void g_sound(void *ctx,uint16_t id) {(void)ctx;(void)id;}
static bool g_creator_alive(void *ctx,neo_gactor_t *a) {
    tick_t *t=ctx;uint32_t c=ga_l(a,0x0c);
    if (!c) return true;
    if ((c&0xff000000u)==NEO_GA_TAG_BASE) return (c&0xffffu)<NEO_WORLD_GACTORS && t->w->gactors[c&0xffffu].active && !t->w->gactors[c&0xffffu].placeholder;
    return true;
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
    ga_sb(a,0x6b,0xff);ga_sw(a,0x58,0xffff);ga_sb(a,0x68,0);ga_sb(a,0x69,0xff);
    a->active=false;
}
static void g_marker(void *ctx,int32_t x,int32_t y,uint8_t h,uint8_t id) {
    tick_t *t=ctx;neo_world_t *w=t->w;unsigned l=t->genv.video&1u;
    if (w->marker_count[l]>=8u) return; /* 040114: 0x50 bytes */
    w->markers[l][w->marker_count[l]++]=(neo_marker_t){x,y,h,id};
}
static void g_solid(void *ctx,neo_gactor_t *a,uint32_t desc) {(void)ctx;ga_sb(a,0x6b,desc?0:0xff);}
static void g_stamp(void *ctx,neo_gactor_t *a,uint32_t templ) {
    (void)ctx;neo_collision_vehicle_wreckage((int32_t)ga_l(a,0x28),(int32_t)ga_l(a,0x2c),(uint8_t)(templ==0x29c26u?3u:1u));
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
static uint8_t g_attack(void *ctx,neo_gactor_t *a,const int16_t row[7]) {
    tick_t *t=ctx;neo_world_t *w=t->w;neo_contact_actor_t c=ga_contact(a);
    if (row[5]==0) { /* class 0: the player */
        if (!t->player_hittable) return 0;
        int first=-1;uint8_t type=neo_hit_query(&c,row,&t->player,1,(uint8_t)row[4],0,&first);
        if (type && first>=0 && row[6]) {t->hits++;w->hit_x=HI(c.projected_x);}
        return type;
    }
    uint8_t best=0;
    for (unsigned at=0;at<w->order_count;at++) {
        if (w->order[at].kind==NEO_WORLD_SOLDIER && w->constructed[w->order[at].index]) {
            neo_soldier_t *s=&w->soldiers[w->order[at].index];neo_hurtbox_t hb;
            if (neo_soldier_hurtbox(s,&hb) && neo_soldier_receive(s,&c,ga_b(a,0x5e),row,w->grid) && hb.type>best) best=hb.type;
        }
    }
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
static int run_gactor(tick_t *t,unsigned i) {
    neo_world_t *w=t->w;neo_gactor_t *a=&w->gactors[i];
    uint8_t group=ga_b(a,0x69);
    unsigned before=w->draw_count;
    t->draw_kind=NEO_WORLD_GACTOR;t->draw_flash=false;t->draw_palette=0;
    ga_result_t r=neo_region_tick(a,&t->genv);
    for (unsigned k=before;k<w->draw_count;k++) {
        uint8_t h=(uint8_t)(w->draws[k].attributes>>8);
        w->draws[k].palette=h && h<=4u?a->palette[h-1u]:a->palette[0];
        w->draws[k].flash=(ga_b(a,0x17)&1u) && !(ga_b(a,0x17)&0x80u);
    }
    if (group<9u && ga_b(a,0x69)>=9u) w->kills++;
    if (r==GA_UNSUPPORTED || t->genv.unsupported) {
        w->unsupported++;w->unsupported_routine=t->genv.unsupported?t->genv.unsupported_at:ga_l(a,0);t->genv.unsupported=0;
        w->draw_count=before;a->active=false;return 0;
    }
    if (r==GA_DELETE) {ga_kill(t,a);return 0;}
    if (r==GA_DELETE_TREE) {ga_kill(t,a);return 2;}
    return 1;
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
    t->draw_palette=SPLASH_PALETTE;t->draw_flash=false;t->draw_kind=NEO_WORLD_SPLASH;
    neo_anim_result_t r=neo_anim_step(&s->anim,&s->render,emit,t,1024);
    if (r!=NEO_ANIM_MORE) {s->active=false;return false;}
    return true;
}
static bool run_throw(tick_t *t,unsigned i) {
    neo_world_t *w=t->w;neo_throw_actor_t *o=&w->throws[i];
    neo_throw_services_t services={program(0x23f3f0),program(0x22d2a6),soldier_throw_flight_rect,soldier_throw_burst_rect,
        throw_palette,throw_release,t,throw_air,throw_hit};
    /* The effect phase projects against the object's current parent. */
    const neo_contact_parent_t *scene=o->body.parent_tag==TERRAIN_TAG?&w->terrain:&w->root;
    neo_throw_frame_t frame={scene,&t->players,&t->bounds,w->video_phase,0,w->epoch,false};
    unsigned before=w->draw_count;
    t->draw_kind=NEO_WORLD_THROW;t->draw_flash=false;t->draw_palette=0;
    unsigned events;neo_throw_result_t r=neo_throw_actor_step(o,&frame,&services,emit,t,&events);
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
        default:break;
        }
        remove_at(w,at+1u);
    }
}
/* ---- public ------------------------------------------------------------------ */
static void scene_at(neo_world_t *w,int32_t x,bool reset) {
    neo_contact_parent_t *p[2]={&w->terrain,&w->root};
    for (unsigned i=0;i<2;i++) {
        p[i]->previous_x=reset?x:p[i]->x;p[i]->previous_y=reset?w->scroll_y:p[i]->y;p[i]->x=x;p[i]->y=w->scroll_y;p[i]->correction=0;
    }
    w->kernel.terrain=(neo_kobj_t){w->terrain.x,w->terrain.y,w->terrain.previous_x,w->terrain.previous_y};
    w->kernel.root=(neo_kobj_t){w->root.x,w->root.y,w->root.previous_x,w->root.previous_y};
    w->scroll_x=x;
}
void neo_world_init(neo_world_t *w,int32_t scroll_x) {
    memset(w,0,sizeof(*w));
    neo_script_init(&w->script,mission1_script,sizeof(mission1_script),MISSION1_SCRIPT_BASE);
    memcpy(w->rng.words,mission1_handoff_rng,sizeof(w->rng.words));w->rng.cursor=MISSION1_HANDOFF_RNG_CURSOR;
    w->terrain.tag=TERRAIN_TAG;w->root.tag=ROOT_TAG;
    w->kernel.group_map=soldier_group_map;
    w->epoch=1;w->camera_lock=-1;w->scroll_y=SCENE_Y;
    scene_at(w,scroll_x,true);
    w->order[w->order_count++]=(neo_world_slot_t){NEO_WORLD_SCRIPT,0,0};
    /* RAM 10E9D2..10E9D8 as observed in the region (0C8BEC). */
    w->view[0]=0;w->view[1]=319;w->view[2]=256;w->view[3]=511;
}
unsigned neo_world_tick(neo_world_t *w,int32_t scroll_x,const neo_world_player_t *pl) {
    tick_t t;memset(&t,0,sizeof(t));t.w=w;
    w->ticks++;
    w->epoch=w->epoch==1?2:1;               /* RAM 10C33A */
    scene_at(w,scroll_x,false);
    uint16_t camera=(uint16_t)((0u-(uint32_t)scroll_x)>>16); /* RAM 10E99E */
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
    e->event=soldier_event;e->event_context=&t;e->blood=false;e->dyn=w->dyn_parent;
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
    g->creator_alive=g_creator_alive;g->creator=g_creator;g->stamp=g_stamp;
    g->marker=g_marker;g->marker_seq=&w->marker_seq;
    w->marker_count[g->video&1u]=0;                          /* 0401E8 */
    e->markers=w->markers[(g->video&1u)^1u];e->marker_count=w->marker_count[(g->video&1u)^1u];
    memcpy(g->view,w->view,sizeof(g->view));g->flag448=0;
    /* Hurtboxes registered during the previous update are the ones queried
     * now (ROM 004C24 double buffer). */
    memcpy(w->last_boxes,w->boxes,sizeof(w->boxes));w->last_box_count=w->box_count;w->box_count=0;
    /* End of the camera update (ROM 01406A): dynamic collision objects. */
    neo_kernel_dyn_begin(&w->kernel);
    for (int i=NEO_KDYN_COUNT-1;i>=0;i--) {
        if (w->kernel.dyn[i].countdown) {w->kernel.dyn[i].countdown--;continue;}
        dyn_project(w,&t,(unsigned)i);
        if (w->kernel.dyn[i].map) neo_kernel_dyn_update(&w->kernel,(unsigned)i);
    }
    /* Mission actor (0186A0): grid decay 00469C, group recount 019E40. */
    neo_grid_decay(w->grid,&w->grid_cursor);
    memset(w->script.groups,0,sizeof(w->script.groups));
    for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++)
        if (w->soldiers[i].active && w->constructed[i] && w->soldiers[i].group69<9u) w->script.groups[w->soldiers[i].group69]++;
    for (unsigned i=0;i<NEO_WORLD_GACTORS;i++)
        if (w->gactors[i].active && ga_b(&w->gactors[i],0x69)<9u) w->script.groups[ga_b(&w->gactors[i],0x69)]++;
    neo_script_env_t se={camera,w->terrain.x,w->terrain.y,
        {{t.player_valid,pl?pl->follow_x:0,pl?pl->follow_y:0,px,py},{false,0,0,0,0}},
        script_rng,script_spawn,script_runner,script_camera_lock,script_camera_release,&t};
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
        case NEO_WORLD_SCRIPT: neo_script_tick(&w->script,&se);break;
        case NEO_WORLD_RUNNER: keep=neo_script_tick_runner(&w->script,slot.index,&se);break;
        case NEO_WORLD_SOLDIER: keep=run_soldier(&t,slot.index);break;
        case NEO_WORLD_THROW: keep=run_throw(&t,slot.index);break;
        case NEO_WORLD_SPLASH: keep=run_splash(&t,slot.index);break;
        case NEO_WORLD_GACTOR: {int r=run_gactor(&t,slot.index);keep=r==1;tree=r==2;break;}
        }
        /* The running actor may have inserted others after itself. */
        int now=find(w,slot.kind,slot.index);
        if (now<0) now=(int)at;
        if (tree) delete_tree(&t,(unsigned)now);
        if (!keep && slot.kind==NEO_WORLD_GACTOR && (unsigned)now+1u<w->order_count && w->order[now+1].depth>slot.depth) {
            /* 004126: a node followed by deeper ones stays as a placeholder. */
            neo_gactor_t *g=&w->gactors[slot.index];
            g->active=true;g->placeholder=true;ga_sl(g,0,0x3f1e);keep=true;
        }
        if (!keep) {remove_at(w,(unsigned)now);at=(unsigned)now;}
        else at=(unsigned)now+1u;
    }
    w->player_hits+=t.hits;
    return t.hits;
}
bool neo_world_shot(neo_world_t *w,int32_t previous_x,int32_t x,int32_t y,bool left) {
    neo_contact_actor_t b;memset(&b,0,sizeof(b));
    b.projected_x=(int32_t)((uint32_t)x+(uint32_t)w->terrain.x);b.projected_y=(int32_t)((uint32_t)y+(uint32_t)w->terrain.y);
    b.cached_x=(int32_t)((uint32_t)previous_x+(uint32_t)w->terrain.x);b.cached_y=b.projected_y;
    b.flags15=(uint8_t)(32u|(left?1u:0u));
    bool hit=false;
    for (unsigned at=0;at<w->order_count;at++) {
        if (w->order[at].kind!=NEO_WORLD_SOLDIER) continue;
        unsigned i=w->order[at].index;
        if (!w->constructed[i]) continue;
        hit|=neo_soldier_receive(&w->soldiers[i],&b,1,soldier_pistol_attacks[0],w->grid);
    }
    return hit;
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
bool neo_world_region_clear(const neo_world_t *w) {return neo_script_reached(&w->script,MISSION1_SCRIPT_END);}
unsigned neo_world_alive(const neo_world_t *w) {
    unsigned n=0;
    for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++) if (w->soldiers[i].active && w->constructed[i] && w->soldiers[i].group69<9u) n++;
    return n;
}
