#define MSX_SOLDIER_PROGRAMS 1
#include "neo_world.h"
#include <string.h>
#include "neo_throw.h"
#include "soldier_ai_tables.h"
/* See neo_world.h. Every routine executed here is one of the translated and
 * fixture-verified pieces; this file only orders them as the original actor
 * list does and connects their inputs/outputs. */
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
enum { TERRAIN_TAG=0x10c72e, ROOT_TAG=0x10e968, SCENE_Y=0x01000000 };
enum { SOLDIER_CONSTRUCTOR=0xaf17e, REGION_CLEAR=0x2d651a };
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
    neo_hurtbox_t player;bool player_valid;
    neo_control_frame_t players;
    neo_cull_bounds_t bounds;
    neo_soldier_env_t env;
    unsigned hits;
    int current;                 /* order index of the running actor */
    uint16_t draw_palette;bool draw_flash;uint8_t draw_kind;
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
    if (at<0 || !insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_RUNNER,(uint8_t)index})) w->overflow++;
}
static void script_spawn(void *context,const neo_script_spawn_t *sp) {
    tick_t *t=context;neo_world_t *w=t->w;
    if (sp->constructor!=SOLDIER_CONSTRUCTOR) {w->external_spawns++;w->external_constructor=sp->constructor;return;}
    unsigned i;for (i=0;i<NEO_WORLD_SOLDIERS && w->soldiers[i].active;i++) {}
    int at=sp->creator<0?find(w,NEO_WORLD_SCRIPT,0):find(w,NEO_WORLD_RUNNER,(unsigned)sp->creator);
    if (i==NEO_WORLD_SOLDIERS || at<0 || !insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_SOLDIER,(uint8_t)i})) {w->overflow++;return;}
    neo_soldier_spawn_t *p=&w->pending[i];memset(p,0,sizeof(*p));
    p->x=sp->x;p->y=sp->y;p->variant68=sp->variant68;p->group69=sp->group69;
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
        if (i==NEO_WORLD_THROWS || at<0 || !insert(w,(unsigned)at+1,(neo_world_slot_t){NEO_WORLD_THROW,(uint8_t)i})) {w->overflow++;break;}
        neo_contact_actor_t body;neo_throw_spawn(&body,&e->actor,&w->root,w->epoch);
        neo_anim_render_t graphics;memset(&graphics,0,sizeof(graphics));graphics.attributes=body.flags15;
        neo_throw_actor_start(&w->throws[i],&body,0,0xff,&graphics);
        break;
    }
    case NEO_SOLDIER_EVENT_DEATH_CHILD: { /* 040ED0: immediate child 040F88 */
        unsigned i;for (i=0;i<NEO_WORLD_SPLASHES && w->splashes[i].active;i++) {}
        if (i==NEO_WORLD_SPLASHES || !insert(w,(unsigned)t->current+1,(neo_world_slot_t){NEO_WORLD_SPLASH,(uint8_t)i})) {w->overflow++;break;}
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
    if (!t->player_valid) return false;
    int first=-1;
    uint8_t type=neo_hit_query(b,soldier_throw_attack,&t->player,1,(uint8_t)soldier_throw_attack[4],(uint8_t)soldier_throw_attack[5],&first);
    if (type && first>=0 && soldier_throw_attack[6]) {t->hits++;t->w->hit_x=HI(b->projected_x);}
    return type!=0;
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
        throw_palette,NULL,t,throw_air,throw_hit};
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
/* ---- public ------------------------------------------------------------------ */
static void scene_at(neo_world_t *w,int32_t x,bool reset) {
    neo_contact_parent_t *p[2]={&w->terrain,&w->root};
    for (unsigned i=0;i<2;i++) {
        p[i]->previous_x=reset?x:p[i]->x;p[i]->previous_y=SCENE_Y;p[i]->x=x;p[i]->y=SCENE_Y;p[i]->correction=0;
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
    w->epoch=1;
    scene_at(w,scroll_x,true);
    w->order[w->order_count++]=(neo_world_slot_t){NEO_WORLD_SCRIPT,0};
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
    uint16_t px=pl?pl->px:0,py=pl?pl->py:0;
    t.player=(neo_hurtbox_t){px,py,pl?pl->flags15:0,soldier_player_hurtbox.type,0,
        {soldier_player_hurtbox.rect[0],soldier_player_hurtbox.rect[1],soldier_player_hurtbox.rect[2],soldier_player_hurtbox.rect[3]},pl && pl->dead};
    t.players=(neo_control_frame_t){0,0,0,0,px,py,0,0,(uint8_t)(t.player_valid?1:0),MODE88,RANK,t.player_valid,w->grid};
    t.bounds=(neo_cull_bounds_t){false,0,0,0,0,0,256,320,512};
    neo_soldier_env_t *e=&t.env;
    e->kernel=&w->kernel;e->terrain=w->terrain;e->root=w->root;e->epoch=w->epoch;e->players=t.players;
    e->grid=w->grid;e->rng=&w->rng;e->targets=t.player_valid?&t.player:NULL;e->target_count=t.player_valid?1:0;
    e->bounds=t.bounds;e->camera_word=HI(w->terrain.x);e->program=program;e->emit=emit;e->emit_context=&t;
    e->event=soldier_event;e->event_context=&t;e->blood=false;
    /* Mission actor (0186A0): grid decay 00469C, group recount 019E40. */
    neo_grid_decay(w->grid,&w->grid_cursor);
    memset(w->script.groups,0,sizeof(w->script.groups));
    for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++)
        if (w->soldiers[i].active && w->constructed[i] && w->soldiers[i].group69<9u) w->script.groups[w->soldiers[i].group69]++;
    neo_script_env_t se={camera,w->terrain.x,w->terrain.y,t.player_valid,script_rng,script_spawn,script_runner,NULL,&t};
    for (unsigned at=0;at<w->order_count;) {
        neo_world_slot_t slot=w->order[at];t.current=(int)at;bool keep=true;
        if (w->trace) w->trace(w->trace_context,slot.kind,slot.index);
        switch (slot.kind) {
        case NEO_WORLD_SCRIPT: neo_script_tick(&w->script,&se);break;
        case NEO_WORLD_RUNNER: keep=neo_script_tick_runner(&w->script,slot.index,&se);break;
        case NEO_WORLD_SOLDIER: keep=run_soldier(&t,slot.index);break;
        case NEO_WORLD_THROW: keep=run_throw(&t,slot.index);break;
        case NEO_WORLD_SPLASH: keep=run_splash(&t,slot.index);break;
        }
        /* The running actor may have inserted others after itself. */
        int now=find(w,slot.kind,slot.index);
        if (now<0) now=(int)at;
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
bool neo_world_region_clear(const neo_world_t *w) {return neo_script_reached(&w->script,REGION_CLEAR);}
unsigned neo_world_alive(const neo_world_t *w) {
    unsigned n=0;
    for (unsigned i=0;i<NEO_WORLD_SOLDIERS;i++) if (w->soldiers[i].active && w->constructed[i] && w->soldiers[i].group69<9u) n++;
    return n;
}
