#include "neo_soldier_ai.h"
#include "neo_soldier.h"
#include "neo_soldier_action.h"
#include "neo_hit.h"
#include "neo_motion.h"
#include "neo_target.h"
#include <stddef.h>
#include <string.h>
#include "soldier_ai_tables.h"
/* Diagnostics: source line of the last explicit stop (no behaviour). */
unsigned neo_soldier_unsupported_line;uint32_t neo_soldier_unsupported_detail;
#define UNSUPPORTED_AT() (neo_soldier_unsupported_line=__LINE__,c->unsupported=true)

/* Native translation of the rebel soldier routines. Every routine below is
 * labelled with its original ROM entry; the code follows the instruction
 * order of the original, including writes later overwritten in the same
 * update. Nothing here decodes or executes 68000 instructions. */

#define TERRAIN_TAG 0x10c72eu
#define ROOT_TAG 0x10e968u
static int32_t S32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static int16_t S16(uint16_t v) {return (int16_t)(v<=0x7fffu?(int)v:(int)v-65536);}
static uint16_t HI(int32_t v) {return (uint16_t)((uint32_t)v>>16);}
static uint32_t ASR(uint32_t v,unsigned n) {return (uint32_t)neo_asr(S32(v),n);}

uint8_t neo_soldier_b(const neo_soldier_t *s,unsigned o) {return s->locals[o-0x6e];}
uint16_t neo_soldier_w(const neo_soldier_t *s,unsigned o) {return (uint16_t)(s->locals[o-0x6e]<<8|s->locals[o-0x6d]);}
uint32_t neo_soldier_l(const neo_soldier_t *s,unsigned o) {return (uint32_t)neo_soldier_w(s,o)<<16|neo_soldier_w(s,o+2);}
void neo_soldier_set_b(neo_soldier_t *s,unsigned o,uint8_t v) {s->locals[o-0x6e]=v;}
void neo_soldier_set_w(neo_soldier_t *s,unsigned o,uint16_t v) {s->locals[o-0x6e]=(uint8_t)(v>>8);s->locals[o-0x6d]=(uint8_t)v;}
void neo_soldier_set_l(neo_soldier_t *s,unsigned o,uint32_t v) {neo_soldier_set_w(s,o,(uint16_t)(v>>16));neo_soldier_set_w(s,o+2,(uint16_t)v);}
#define B(o) neo_soldier_b(s,o)
#define W(o) neo_soldier_w(s,o)
#define L(o) neo_soldier_l(s,o)
#define SB(o,v) neo_soldier_set_b(s,o,(uint8_t)(v))
#define SW(o,v) neo_soldier_set_w(s,o,(uint16_t)(v))
#define SL(o,v) neo_soldier_set_l(s,o,(uint32_t)(v))

#define START(t) do { if (!anim_start(c,(t))) return NEO_SOLDIER_UNSUPPORTED; } while (0)
typedef struct {
    neo_soldier_t *s;neo_soldier_env_t *e;
    bool unsupported;
} ctx_t;

/* ---- scene objects -------------------------------------------------------- */
static uint8_t kobj(uint32_t tag) {
    if (tag>=NEO_DYN_TAG(0) && tag<NEO_DYN_TAG(NEO_KDYN_COUNT) && !((tag-NEO_DYN_TAG(0))&63u)) return (uint8_t)(NEO_KOBJ_DYN0+(tag-NEO_DYN_TAG(0))/64u);
    return tag==TERRAIN_TAG?NEO_KOBJ_TERRAIN:tag==ROOT_TAG?NEO_KOBJ_ROOT:tag?0xff:NEO_KOBJ_NONE;
}
static uint32_t ktag(uint8_t id) {
    if (id>=NEO_KOBJ_DYN0 && id<NEO_KOBJ_DYN0+NEO_KDYN_COUNT) return NEO_DYN_TAG(id-NEO_KOBJ_DYN0);
    return id==NEO_KOBJ_TERRAIN?TERRAIN_TAG:id==NEO_KOBJ_ROOT?ROOT_TAG:0;
}
static const neo_contact_parent_t *parent_of(const neo_soldier_env_t *e,uint32_t tag) {
    if (e->dyn && tag>=NEO_DYN_TAG(0) && tag<NEO_DYN_TAG(NEO_KDYN_COUNT)) return &e->dyn[(tag-NEO_DYN_TAG(0))/64u];
    return tag==TERRAIN_TAG?&e->terrain:tag==ROOT_TAG?&e->root:NULL;
}
static void project(ctx_t *c) {neo_contact_project(&c->s->b,parent_of(c->e,c->s->b.parent_tag),c->e->epoch);}
/* Kernel call followed by the 014492 commit. */
static bool sweep(ctx_t *c,const neo_contact_request_t *r,uint32_t mode4004,uint8_t b4008,uint8_t b4009,uint8_t b400a,
    neo_kres_t *out) {
    uint8_t parent=kobj(r->parent_tag);
    if (parent==0xff) {UNSUPPORTED_AT();return false;}
    neo_kreq_t q={r->x,r->y,r->dx,r->dy,parent,mode4004,b4008,b4009,b400a,0,0};
    neo_kres_t k;
    if (!neo_kernel_sweep(c->e->kernel,&q,&k)) {UNSUPPORTED_AT();return false;}
    neo_contact_result_t res={k.x,k.y,k.vx,k.vy,ktag(k.parent),k.kind,k.flags};
    neo_contact_commit(&c->s->b,&res,parent_of(c->e,res.parent_tag),c->e->epoch);
    if (out) *out=k;
    return true;
}
/* ROM 01456A: ground step (prepare 0145BC..01469E, kernel, commit). */
static uint16_t ground(ctx_t *c,uint32_t *kind) {
    neo_contact_actor_t *a=&c->s->b;
    neo_contact_request_t r=neo_contact_prepare_ground(a);
    uint32_t mode=r.mode;
    neo_kres_t k;
    if (!sweep(c,&r,mode,r.mode8,0xff,0,&k)) return 0;
    if (kind) *kind=k.kind;
    return k.flags;
}
/* ROM 0143E4/01444A: air step with direct velocities. */
static uint16_t air(ctx_t *c,uint8_t b400a) {
    neo_contact_request_t r=neo_contact_prepare_air(&c->s->b);
    neo_kres_t k;
    if (!sweep(c,&r,0,r.mode8,0,b400a,&k)) return 0;
    return k.flags;
}
/* ROM 0143DC: extra gravity, then 0143E4. */
static uint16_t air_drop(ctx_t *c) {c->s->b.vy=S32((uint32_t)c->s->b.vy+0xffffb000u);return air(c,0);}
/* ROM 01436A: zero-length query, commit, restore velocity. */
static void stationary(ctx_t *c) {
    neo_contact_actor_t *a=&c->s->b;int32_t vx=a->vx,vy=a->vy;
    neo_contact_request_t r={a->x,a->y,0,0,a->parent_tag,0,a->flags15&32u?0:255,0,0,0};
    sweep(c,&r,0,r.mode8,0,0,NULL);
    a->vx=vx;a->vy=vy;
}
/* Kernel query without commit (0148C2, 014850, 01495C). */
static bool query(ctx_t *c,int32_t x,int32_t y,int32_t dx,int32_t dy,uint8_t b4009,neo_kres_t *k) {
    neo_contact_actor_t *a=&c->s->b;uint8_t parent=kobj(a->parent_tag);
    if (parent==0xff) {UNSUPPORTED_AT();return false;}
    neo_kreq_t q={x,y,dx,dy,parent,0,(uint8_t)(a->flags15&32u?0:255),b4009,0,0,0};
    if (!neo_kernel_sweep(c->e->kernel,&q,k)) {UNSUPPORTED_AT();return false;}
    return true;
}
/* ROM 0148C2: segment (d0,d1)->(d2,d3) relative to the actor. */
static uint16_t probe_segment(ctx_t *c,int16_t x0,int16_t y0,int16_t x1,int16_t y1) {
    neo_contact_actor_t *a=&c->s->b;neo_kres_t k;
    int32_t dx=(int32_t)((uint32_t)(int32_t)(x1-x0)<<16),dy=(int32_t)((uint32_t)(int32_t)(y1-y0)<<16);
    if (!query(c,S32((uint32_t)a->x+((uint32_t)(int32_t)x0<<16)),S32((uint32_t)a->y+((uint32_t)(int32_t)y0<<16)),dx,dy,0,&k)) return 0;
    return k.flags;
}
/* ROM 014850: forward ground-follow probe, then a downward one. */
static uint16_t probe_forward(ctx_t *c,int16_t distance) {
    neo_contact_actor_t *a=&c->s->b;neo_kres_t k;
    int32_t d3=(int32_t)((uint32_t)(int32_t)distance<<16);
    if (!query(c,a->x,a->y,d3,0,0xff,&k)) return 0;
    if (k.flags&1u) return k.flags;
    if (d3>=0) d3=S32(0u-(uint32_t)d3);
    d3=S32((uint32_t)d3-0x80000u);
    neo_kres_t k2;
    neo_kreq_t q={k.x,k.y,0,d3,k.parent,0,(uint8_t)(a->flags15&32u?0:255),0xff,0,0,0};
    if (!neo_kernel_sweep(c->e->kernel,&q,&k2)) {UNSUPPORTED_AT();return 0;}
    return k2.flags;
}
/* ROM 01495C: vertical probe by d0 pixels. Returns carry, d6 in *flags. */
static bool probe_vertical(ctx_t *c,int16_t d0,uint16_t *flags) {
    neo_contact_actor_t *a=&c->s->b;neo_kres_t k;
    if (!query(c,a->x,a->y,0,(int32_t)((uint32_t)(int32_t)d0<<16),0,&k)) return false;
    if (flags) *flags=k.flags;
    return (k.flags&1u)!=0;
}

/* ---- animation ------------------------------------------------------------ */
static bool anim_start(ctx_t *c,uint32_t tag) {
    const neo_anim_program_t *p=c->e->program?c->e->program(tag):NULL;
    if (!p) {UNSUPPORTED_AT();return false;}
    neo_anim_start(&c->s->anim,p,0x100);return true;
}
static void event(ctx_t *c,neo_soldier_event_kind_t kind,uint16_t value,const neo_contact_actor_t *seed) {
    if (!c->e->event) return;
    neo_soldier_event_t ev={kind,value,{0}};if (seed) ev.actor=*seed;
    c->e->event(c->e->event_context,&ev);
}
static bool field_read(void *context,uint16_t field,uint8_t *value) {
    ctx_t *c=context;neo_soldier_t *s=c->s;
    if (field>=0x6e && field<0x6e + NEO_SOLDIER_LOCALS) {*value=B(field);return true;}
    return false;
}
static bool field_write(void *context,uint16_t field,unsigned size,uint32_t value) {
    ctx_t *c=context;neo_soldier_t *s=c->s;
    if (field>=0x6e && field+size<=0x6e + NEO_SOLDIER_LOCALS) {
        if (size==1) SB(field,value); else if (size==2) SW(field,value); else SL(field,value);
        return true;
    }
    if (field==0x34 && size==4) {s->b.vy=S32(value);return true;}
    return false;
}
static void sound(void *context,uint16_t id) {event(context,NEO_SOLDIER_EVENT_SOUND,id,NULL);}
static uint8_t hit_player(ctx_t *c,const int16_t *attack,bool damage);
static bool call(void *context,uint32_t routine,uint16_t parameter,neo_anim_render_t *render) {
    ctx_t *c=context;neo_soldier_t *s=c->s;
    switch (routine) {
    case 0x40948: /* random(parameter) -> +6F */
        SB(0x6f,(uint16_t)(((uint32_t)neo_rng_next(c->e->rng)*parameter)>>16));return true;
    case 0x40956:s->b.flags15|=32u;render->attributes|=32u;return true;
    case 0x4095e: {
        int32_t v=(int32_t)((uint32_t)parameter<<16);if (!(s->b.flags15&1u)) v=S32(0u-(uint32_t)v);
        s->b.vx=v;s->b.scalar62=v;return true;
    }
    case 0x23e418:s->b.flags15&=254u;render->attributes&=0xfffeu;return true;
    case 0x23e420:case 0x23e42e: {
        uint16_t y=(uint16_t)(HI(s->b.y)+(routine==0x23e420?0x24u:0u-0x24u));
        s->b.y=(int32_t)(((uint32_t)y<<16)|((uint32_t)s->b.y&0xffffu));project(c);
        render->x=HI(s->b.projected_x);render->y=HI(s->b.projected_y);render->correction=s->b.correction;
        return true;
    }
    case 0xaef14: /* knife: 0049E8 with ROM 0AE980, sound 10FA on any hit. */
        if (hit_player(c,soldier_attack_knife,true)) event(c,NEO_SOLDIER_EVENT_SOUND,0x10fa,NULL);
        return true;
    default:return false;
    }
}
static bool anim_event(void *context,neo_anim_kind_t kind,uint32_t asset,neo_anim_render_t *render) {
    ctx_t *c=context;neo_soldier_t *s=c->s;
    if (kind==NEO_ANIM_REPLACE_PALETTE) {s->palette_resource=(uint16_t)asset;return true;}
    if (kind==NEO_ANIM_SPAWN_THROW) {
        /* 03BB8E: deferred 03BC28 actor offset (0,32), +6E=0; seed built by
         * the existing native neo_throw_spawn in the owner. */
        /* 03BBBC / 03BBEA: same object with +6E=1 / 2 at (0,24) / (0,32).
         * Event value: +6E in the low byte, vertical offset in the high byte. */
        uint16_t field6e=asset==0x3bbbcu?1u:asset==0x3bbeau?2u:0u;
        uint16_t dy=asset==0x3bbbcu?0x18u:0x20u;
        neo_contact_actor_t seed=s->b;seed.flags15=(uint8_t)render->attributes;
        event(c,NEO_SOLDIER_EVENT_THROW,(uint16_t)(field6e|dy<<8),&seed);return true;
    }
    return false;
}
/* ROM 00427A. Returns true when the program signals completion (carry). */
static bool anim_step(ctx_t *c) {
    neo_soldier_t *s=c->s;
    s->render.x=HI(s->b.projected_x);s->render.y=HI(s->b.projected_y);
    s->render.attributes=(uint16_t)((s->render.attributes&0xff00u)|s->b.flags15);
    s->render.correction=s->b.correction;s->render.z=s->b.word1e;
    neo_anim_actor_t actor={anim_event,field_read,field_write,sound,call,c};
    neo_anim_result_t r=neo_anim_step_actor(&s->anim,&s->render,c->e->emit,c->e->emit_context,1024,&actor);
    s->b.flags15=(uint8_t)s->render.attributes;
    if (r!=NEO_ANIM_MORE && r!=NEO_ANIM_COMPLETE) {
        const neo_anim_command_t *cmd=s->anim.program && s->anim.cursor<s->anim.program->count?&s->anim.program->commands[s->anim.cursor]:NULL;
        neo_soldier_unsupported_detail=(uint32_t)r<<28|(cmd?(cmd->asset&0xffffffu):0xffffffu);
        UNSUPPORTED_AT();return false;
    }
    return r==NEO_ANIM_COMPLETE;
}

/* ---- hits ----------------------------------------------------------------- */
uint8_t neo_hit_query(const neo_contact_actor_t *a,const int16_t rect[4],const neo_hurtbox_t *targets,
    unsigned count,uint8_t kind,uint8_t klass,int *first_hit) {
    (void)kind;
    /* 0049E8..004A56: query rectangle in projected words, oriented. */
    uint16_t x=(uint16_t)rect[0],y=(uint16_t)rect[1];
    if (a->flags15&1u) x=(uint16_t)(0u-(uint16_t)(x+(uint16_t)rect[2]));
    x=(uint16_t)(x+HI(a->projected_x));
    if (a->flags15&2u) y=(uint16_t)(0u-(uint16_t)(y+(uint16_t)rect[3]));
    y=(uint16_t)(y+HI(a->projected_y));
    /* 004A58..004AAA: an attack outside the registry window finds nothing. */
    int16_t cx=S16((uint16_t)(x+0x20u)),w=rect[2],cy=S16((uint16_t)(y-0x108u)),h=rect[3];
    if (cx<0) {if ((int16_t)(w+cx)<0) return 0; w=(int16_t)(w+cx);cx=0;}
    if (cx>0x17f) return 0;
    if (cx+w>0x17f) w=(int16_t)(0x17f-cx);
    if (!w) return 0;
    if (cy<0) {if ((int16_t)(h+cy)<0) return 0; h=(int16_t)(h+cy);cy=0;}
    if (cy>0xef) return 0;
    if (cy+h>0xef) h=(int16_t)(0xef-cy);
    if (!h) return 0;
    neo_hit_rectangle_t q={x,y,(uint16_t)rect[2],(uint16_t)rect[3]};
    uint8_t best=0;
    for (unsigned i=0;i<count;i++) {
        const neo_hurtbox_t *t=&targets[i];
        if (t->klass!=klass) continue;
        neo_hit_rectangle_t local={(uint16_t)t->rect[0],(uint16_t)t->rect[1],(uint16_t)t->rect[2],(uint16_t)t->rect[3]};
        uint16_t ix,iy;
        if (!neo_hit_rectangle(&q,t->x,t->y,t->flags15,&local,&ix,&iy)) continue;
        if (t->type>=best) best=t->type;
        if (first_hit && *first_hit<0) *first_hit=(int)i;
    }
    return best;
}
/* 0049E8 by the soldier against the player-side (class 0) hurtboxes. */
static uint8_t hit_player(ctx_t *c,const int16_t *attack,bool damage) {
    int first=-1;
    uint8_t type=neo_hit_query(&c->s->b,attack,c->e->targets,c->e->target_count,(uint8_t)attack[4],0,&first);
    if (damage && first>=0 && attack[6]) event(c,NEO_SOLDIER_EVENT_KNIFE_HIT,(uint16_t)attack[6],NULL);
    return type;
}
bool neo_soldier_hurtbox(const neo_soldier_t *s,neo_hurtbox_t *out) {
    /* 040A00: a dying soldier registers only while +73 is zero. */
    if ((s->routine==0x40db4 || s->routine==0x40df0) && B(0x73)) return false;
    const int16_t *d=soldier_hurtbox(s->anim.field50_tag);
    if (!d) return false;
    *out=(neo_hurtbox_t){HI(s->b.projected_x),HI(s->b.projected_y),s->b.flags15,5,1,{d[0],d[1],d[2],d[3]},
        (s->b.flags5f&0xc0u)!=0};
    return true;
}
uint8_t neo_angle(int16_t x,int16_t y) {
    /* 002EB4: halve, sign-fold, log/atan tables at ROM 002C7C/002D7C. */
    uint16_t d0=(uint16_t)neo_asr(x,1),d1=(uint16_t)neo_asr(y,1);
    uint8_t s2=(d0&0x8000u)?0xff:0,s3=(d1&0x8000u)?0xff:0;
    d0=(uint16_t)((d0&0xff00u)|(uint8_t)(((uint8_t)d0^s2)-s2));
    d1=(uint16_t)((d1&0xff00u)|(uint8_t)(((uint8_t)d1^s3)-s3));
    uint8_t r;
    if (!(d0&0xffu)) r=0x40;
    else if (!(d1&0xffu)) r=0;
    else {
        uint8_t a=soldier_angle_log[d1&0xffu],b=soldier_angle_log[d0&0xffu];
        if (a>=b) {uint8_t i=(uint8_t)~(uint8_t)(a-b);r=(uint8_t)(0x40u-soldier_angle_atan[i]);}
        else r=soldier_angle_atan[(uint8_t)(a-b)];
    }
    s3^=s2;r=(uint8_t)((r^s3)-s3);
    return (uint8_t)(r+(s2&0x80u));
}
bool neo_soldier_receive(neo_soldier_t *s,const neo_contact_actor_t *a,uint8_t a5e,const int16_t at[7],uint16_t *grid) {
    neo_hurtbox_t box;
    if (!s->active || !neo_soldier_hurtbox(s,&box) || at[5]!=1) return false;
    uint16_t x=(uint16_t)at[0],y=(uint16_t)at[1];
    if (a->flags15&1u) x=(uint16_t)(0u-(uint16_t)(x+(uint16_t)at[2]));
    x=(uint16_t)(x+HI(a->projected_x));
    if (a->flags15&2u) y=(uint16_t)(0u-(uint16_t)(y+(uint16_t)at[3]));
    y=(uint16_t)(y+HI(a->projected_y));
    neo_hit_rectangle_t q={x,y,(uint16_t)at[2],(uint16_t)at[3]};
    neo_hit_rectangle_t local={(uint16_t)box.rect[0],(uint16_t)box.rect[1],(uint16_t)box.rect[2],(uint16_t)box.rect[3]};
    uint16_t ix,iy;
    if (!neo_hit_rectangle(&q,box.x,box.y,box.flags15,&local,&ix,&iy)) return false;
    uint8_t kind=(uint8_t)at[4];
    if (kind>=20u || !soldier_hit_matrix[5][kind]) return true;
    if (kind>s->b.timer5a) { /* 004892..00491A */
        s->b.timer5a=kind;s->anim.flash|=6u;s->f5b=0;
        if (S16((uint16_t)(HI(s->b.projected_x)-HI(a->cached_x)))<0) s->f5b|=0x80u;
        if (S16((uint16_t)(HI(s->b.projected_y)-HI(a->cached_y)))<0) s->f5b|=0x40u;
        uint32_t dx=(uint32_t)a->projected_x-(uint32_t)a->cached_x-(uint32_t)s->b.projected_x+(uint32_t)s->b.cached_x;
        uint32_t dy=(uint32_t)a->projected_y-(uint32_t)a->cached_y-(uint32_t)s->b.projected_y+(uint32_t)s->b.cached_y;
        s->f5b|=(uint8_t)((neo_angle(S16(HI((int32_t)dx)),S16(HI((int32_t)dy)))>>4)&15u);
        s->f5c=(uint8_t)neo_asr(S16(ix),1);s->f5d=(uint8_t)neo_asr(S16(iy),1);
    }
    if (!(s->b.flags5f&0xc0u) && at[6]) { /* 00491E..00499A */
        uint16_t h=(uint16_t)(s->health-(uint16_t)at[6]);
        if (!(s->health>(uint16_t)at[6])) {s->b.flags5f|=128u;if (grid) neo_grid_splat(grid,HI(a->projected_x),HI(a->projected_y));h=0;}
        s->health=h;
    }
    s->f5e=(uint8_t)(((a5e&3u)<<2)|(s->f5e&0xf3u));
    return true;
}
void neo_grid_splat(uint16_t *g,uint16_t px,uint16_t py) {
    /* 00505A..005106 */
    uint16_t y=(uint16_t)(py-0x100u);if (y>=0x100u) return;
    unsigned d0=(y>>4)&14u;
    uint16_t x=(uint16_t)(px-0x20u);if (x>=0x200u) return;
    unsigned d1=(x>>1)&0xf0u;
    #define AT(off) g[((int)(d0+d1)+(off))/2]
    AT(0)=(uint16_t)(AT(0)+0x100u);
    if (d0>=2) {
        AT(-2)=(uint16_t)(AT(-2)+0xc0u);
        if (d1>=0x10) AT(-0x12)=(uint16_t)(AT(-0x12)+0x80u);
        if (d1<0xf0) AT(0x0e)=(uint16_t)(AT(0x0e)+0x80u);
    }
    if (d1>=0x10) AT(-0x10)=(uint16_t)(AT(-0x10)+0xc0u);
    if (d1<0xf0) AT(0x10)=(uint16_t)(AT(0x10)+0xc0u);
    if (d0<0x0e) {
        AT(2)=(uint16_t)(AT(2)+0xc0u);
        if (d1>=0x10) AT(-0x0e)=(uint16_t)(AT(-0x0e)+0x80u);
        if (d1<0xf0) AT(0x12)=(uint16_t)(AT(0x12)+0x80u);
    }
    #undef AT
}
void neo_grid_decay(uint16_t *g,unsigned *cursor) {
    /* 00469C: LSR.W one row of 16 words, then advance (wrap at 0x1068E6). */
    for (unsigned i=0;i<16;i++) g[*cursor*16+i]>>=1;
    *cursor=(*cursor+1)&7u;
}
static uint16_t grid_lookup(const uint16_t *grid,uint16_t x,uint16_t y) {
    /* 00510C */
    y=(uint16_t)(y-0x100u);if (y>=0x100u) return 0;
    x=(uint16_t)(x-0x20u);if (x>=0x200u) return 0;
    unsigned off=((x>>1)&0xf0u)+((y>>4)&14u);
    uint16_t v=grid[off/2];return v>=0x100u?0xffu:v;
}

/* ---- control -------------------------------------------------------------- */
static neo_control_t ctl_load(const neo_soldier_t *s) {
    neo_control_t c={S32(L(0x84)),S32(L(0x88)),W(0x8c),W(0x70),W(0x74),W(0x92),W(0x8e),W(0x90),W(0x6e),
        s->b.flags15,s->b.flags5f,B(0x7c),B(0x80),s->variant68};
    return c;
}
static void ctl_store(neo_soldier_t *s,const neo_control_t *c) {
    SL(0x84,c->target84);SL(0x88,c->velocity88);SW(0x8c,c->phase8c);SW(0x70,c->timer70);SW(0x74,c->counter74);
    SW(0x92,c->delay92);SW(0x8e,c->target8e);SW(0x90,c->target90);SW(0x6e,c->selector6e);
    s->b.flags5f=c->flags5f;SB(0x7c,c->flags7c);SB(0x80,c->selected80);
}
static neo_control_frame_t frame(ctx_t *c) {
    neo_control_frame_t f=c->e->players;
    f.x=HI(c->s->b.projected_x);f.y=HI(c->s->b.projected_y);
    f.origin_x=HI(c->e->terrain.x);f.origin_y=HI(c->e->terrain.y);
    f.delay_grid=c->e->grid;
    return f;
}
/* ROM 0AE9C0 */
static void target(ctx_t *c) {
    neo_control_t k=ctl_load(c->s);neo_control_frame_t f=frame(c);
    neo_control_target(&k,&f);ctl_store(c->s,&k);
}
/* ROM 0AEB5E..0AEDA6: prefix, then the two forward probes. */
static void prefix(ctx_t *c) {
    neo_soldier_t *s=c->s;neo_control_t k=ctl_load(s);neo_control_frame_t f=frame(c);
    if (!neo_control_prefix(&k,&f,c->e->rng)) {UNSUPPORTED_AT();return;}
    ctl_store(s,&k);
    uint16_t p1=probe_segment(c,0,16,(s->b.flags15&1u)?26:-26,36);
    uint16_t p2=probe_forward(c,(s->b.flags15&1u)?26:-26);
    k=ctl_load(s);neo_control_finish(&k,p1,p2);ctl_store(s,&k);
}
/* ROM 0AEB40 */
static void delay_update(ctx_t *c,uint16_t value,uint16_t x,uint16_t y) {
    neo_soldier_t *s=c->s;
    if (value<=W(0x92)) return;
    SW(0x92,value);SW(0x8e,(uint16_t)(x-HI(c->e->terrain.x)));SW(0x90,(uint16_t)(y-HI(c->e->terrain.y)));
}
/* ---- predicates ----------------------------------------------------------- */
static bool pred_contact(const neo_soldier_t *s) { /* 0AF02A */
    if ((s->b.flags5f&32u) && (B(0x7c)&16u) && (uint8_t)s->b.kind_angle==0x40u) return true;
    if (B(0x7c)&(32u|64u)) return false;
    return (B(0x7c)&4u)!=0;
}
static bool pred_wall(const neo_soldier_t *s) { /* 0AF072 */
    if (!(W(0x78)&1u) || !(B(0x7c)&32u)) return false;
    bool neg=(S16(HI(s->b.scalar62)))<0,left=!(s->b.flags15&1u);
    return (neg^left)==0;
}
static bool pred_grid(ctx_t *c) { /* 0AF0A8 */
    neo_soldier_t *s=c->s;
    if (!(B(0x7c)&2u) || (uint8_t)s->b.kind_angle!=0x40u) return false;
    return grid_lookup(c->e->grid,HI(s->b.projected_x),(uint16_t)(HI(s->b.projected_y)+0x20u))>=0x40u;
}
static bool pred_screen(const neo_soldier_t *s) { /* 0AF0E4 */
    return (uint16_t)(HI(s->b.projected_x)-0x18u)<0x110u && (B(0x80)&0x80u);
}
static bool pred_region(ctx_t *c) { /* 0AEDA8 */
    if (!(neo_soldier_b(c->s,0x7c)&8u)) return false;
    return hit_player(c,soldier_attack_sense,false)==5u;
}
/* Scene-root-relative position of the actor (040200/040128). */
static void root_relative(ctx_t *c,int32_t *x,int32_t *y) {
    const neo_contact_parent_t *p=parent_of(c->e,c->s->b.parent_tag);
    uint32_t d0=(uint32_t)c->s->b.x,d1=(uint32_t)c->s->b.y;
    if (p) {d0+=(uint32_t)p->x;d1+=(uint32_t)p->y;}
    *x=S32(d0-(uint32_t)c->e->root.x);*y=S32(d1-(uint32_t)c->e->root.y);
}
/* ROM 040200: the marker the actor is crossing this update; -1 for none.
 * *flag: 1 when below the marker's top half, 2 above (040274). */
static int marker_cross(ctx_t *c,uint8_t *flag) {
    const neo_soldier_env_t *e=c->e;
    int32_t x,y;root_relative(c,&x,&y);
    uint32_t best=0xffffffffu;int32_t at=0;const neo_marker_t *m=NULL;
    for (unsigned i=0;i<e->marker_count;i++) { /* 04015C */
        const neo_marker_t *k=&e->markers[i];
        uint32_t dy=(uint32_t)y-(uint32_t)k->y;
        if (dy>=((uint32_t)k->height<<16)) continue;
        int32_t px=S32((uint32_t)k->x-(dy>>3));
        uint32_t d=(uint32_t)(x-px);if (S32(d)<0) d=0u-d;
        if (best>d) {best=d;at=px;m=k;}
    }
    if (S32(best)<0 || !m) return -1;
    const neo_contact_parent_t *p=parent_of(e,c->s->b.parent_tag);
    uint32_t d7=(uint32_t)c->s->b.vx;
    if (p) d7+=(uint32_t)p->x-(uint32_t)p->previous_x;
    d7=d7-(uint32_t)e->root.x+(uint32_t)e->root.previous_x+(uint32_t)x;
    if (S32(((uint32_t)x-(uint32_t)at)^(d7-(uint32_t)at))>=0) return -1;
    uint32_t d1=((uint32_t)y-(uint32_t)m->y)*2u;
    int8_t r=(int8_t)(uint8_t)((uint8_t)(d1>>16)-m->height);
    *flag=(uint8_t)((r<0?0xffu:0u)+2u);
    return m->id;
}
/* ROM 0AF108 */
static void pred_object(ctx_t *c) {
    neo_soldier_t *s=c->s;uint8_t flag=0;
    int id=marker_cross(c,&flag);
    if (id<0) return;
    SB(0x7e,(uint8_t)id);
    if ((s->b.flags5f&32u) && (flag&1u)) {s->routine=0xb00c0;return;}
    if ((s->b.flags5f&16u) && (flag&2u)) s->routine=0xb01ce;
}
/* ROM 0402E6: follow marker `id` (stairs); returns the remaining height
 * word (negative: none / above). */
static int16_t marker_follow(ctx_t *c,uint8_t id) {
    neo_soldier_t *s=c->s;neo_contact_actor_t *a=&s->b;
    if ((a->field66&0xf000u)==0x8000u) a->word1e=(uint16_t)(a->field66-HI(a->projected_y)); /* 01AD8C */
    const neo_marker_t *m=NULL;
    for (unsigned i=0;i<c->e->marker_count && !m;i++) if (c->e->markers[i].id==id) m=&c->e->markers[i];
    if (!m) {stationary(c);return (int16_t)0x8000;}
    /* 01AC4C: re-parent the block to the scene root. */
    const neo_contact_parent_t *op=parent_of(c->e,a->parent_tag),*np=&c->e->root;
    uint32_t d0=(uint32_t)a->x,d1=(uint32_t)a->y,d2=(uint32_t)a->vx,d3=(uint32_t)a->vy;
    if (a->parent_tag!=ROOT_TAG) {
        d2+=d0;d3+=d1;
        if (op) {d0+=(uint32_t)op->previous_x;d1+=(uint32_t)op->previous_y;d2+=(uint32_t)op->x;d3+=(uint32_t)op->y;}
        d0-=(uint32_t)np->previous_x;d1-=(uint32_t)np->previous_y;d2-=(uint32_t)np->x;d3-=(uint32_t)np->y;
        d2-=d0;d3-=d1;
    }
    uint32_t d4=(uint32_t)(S32(d1+d3-(uint32_t)m->y)>>3);
    d2=(uint32_t)m->x-d0-d4;
    a->x=S32(d0);a->y=S32(d1);a->vx=S32(d2);a->vy=S32(d3);a->parent_tag=ROOT_TAG;
    air(c,0);
    uint32_t h=(uint32_t)m->height<<16;
    uint32_t r=h+(uint32_t)m->y+(uint32_t)c->e->root.y-(uint32_t)a->projected_y;
    if (S32(r)>S32(h)) r=0xffffffffu;
    return (int16_t)(uint16_t)(r>>16);
}
static uint8_t choose(ctx_t *c) { /* 0AEE0C */
    neo_soldier_t *s=c->s;neo_control_t k=ctl_load(s);neo_control_frame_t f=frame(c);
    uint8_t r=neo_soldier_action_choose(&k,&s->b,&f,B(0x81),c->e->rng);ctl_store(s,&k);return r;
}
static uint32_t action_routine(uint8_t action) { /* table 0AF494 */
    static const uint32_t table[8]={0,0xaf918,0xaf92c,0xaf940,0xaf998,0xaf99a,0xaf99a,0xaf99a};
    return table[action&7u];
}
static bool facing_away(const neo_soldier_t *s,uint16_t *distance) {
    uint16_t d=(uint16_t)(HI(S32(L(0x84)))-HI(s->b.projected_x));
    uint16_t o=(uint16_t)(d^((s->b.flags15&1u)?0u:0xffffu));
    if (distance) *distance=(uint16_t)(d&0x8000u?0u-d:d);
    return (o&0x8000u)!=0;
}

/* ---- death ---------------------------------------------------------------- */
static void death_setup(ctx_t *c) { /* 04097A */
    neo_soldier_t *s=c->s;
    if (!(s->b.flags5f&64u)) {s->b.flags5f|=64u;SB(0x6e,0);}
    s->anim.field50_tag=0;s->b.flags5f&=127u;
    /* 0405EA: palette resource 0x22F, or the 01B2BE alternative. */
    s->palette_resource=c->e->blood?0x22f:0x22f;
    s->anim.field50_tag=0;s->group69=0xff;
    uint16_t flags=0;probe_vertical(c,-1,&flags);
    SB(0x72,(flags&8u)?0xff:0);
    if (flags&8u) {s->b.field66=0xfff2;s->b.word1e=0xfff2;}
    SB(0x70,(s->b.flags15&64u)?0xff:0);
    SL(0x76,0);s->b.scalar62=0;SB(0x71,0);SB(0x6f,0);SB(0x74,0);
}
static bool death_dispatch(ctx_t *c,bool crouch) { /* 0406B6 / 0407CA */
    neo_soldier_t *s=c->s;uint8_t kind=s->b.timer5a;
    if (!kind) return true;
    death_setup(c);
    kind=s->b.timer5a;s->b.timer5a=0;
    if (crouch) s->f5b=0; /* 0407DA writes a word over +5A/+5B */
    bool grounded=(s->b.flags15&64u)!=0;
    /* Dispatch tables: 0406EA (grounded), 040750 (air), 0407F4 (crouch).
     * Entries: 040DC0/040DD8/040A28 start a program and continue in 040DF0;
     * 040D66 (grounded pistol) and 040A10 (air pistol) are the observed kills;
     * the remaining entries (040C8E, 040B46, 040BA8, 040A40, 040AF6, 040A7C,
     * 040BE6) are not translated and stop explicitly. */
    uint32_t program=0;
    if (crouch) {
        if (kind==0||kind==1||kind==2||kind==6||kind==8||kind==9||(kind>=0xc&&kind<0x14)) program=0x240f8eu; /* 040A28 */
    } else if (grounded) {
        if (kind==6) { /* 040D66 */
            if (!B(0x6e)) { /* 040ED0: child 040F88 at offset (8,0) */
                neo_contact_actor_t seed=s->b;seed.flags15^=1u;
                event(c,NEO_SOLDIER_EVENT_DEATH_CHILD,0,&seed);
            }
            uint8_t d=s->f5b;bool facing=(s->b.flags15&1u)!=0;
            bool same=(int8_t)(d^(facing?0xffu:0u))>=0;
            if (!anim_start(c,same?0x240588u:0x2406ecu)) return false;
            s->routine=0x40db4;return true;
        }
        if (kind==0||kind==2||kind==8||kind==0xe||kind==0xf||(kind>=0x10&&kind<0x14)) program=0x2404feu; /* 040DC0 */
        else if (kind==0xc||kind==0xd) program=0x240454u; /* 040DD8 */
    } else {
        if (kind==6) program=0x24103cu; /* 040A10 */
        else if (kind==0||kind==2||kind==5||kind==8||kind==9||(kind>=0xc&&kind<0x14)) program=0x2404feu; /* 040DC0 */
    }
    if (program) {if (!anim_start(c,program)) return false;s->routine=0x40df0;return true;}
    UNSUPPORTED_AT();return false;
}
static bool death_motion(ctx_t *c) { /* 040852 */
    neo_soldier_t *s=c->s;
    if (s->b.flags15&64u) {
        s->b.scalar62=S32((uint32_t)s->b.scalar62+ASR(L(0x76)-(uint32_t)s->b.scalar62,2));
        uint16_t f=ground(c,NULL);
        if (f&0x208u) SB(0x72,0xff);
        if ((s->b.flags15&32u) && ((uint8_t)(s->b.kind_angle>>8)==7u || (uint8_t)(s->b.kind_angle>>8)==9u)) {
            s->b.flags15&=191u;SB(0x74,0xff);
        }
    } else {
        s->b.vy=S32((uint32_t)s->b.vy+ASR(0xfff40000u-(uint32_t)s->b.vy,5));
        s->b.vx=S32((uint32_t)s->b.vx+ASR(L(0x76)-(uint32_t)s->b.vx,5));
        s->b.scalar62=s->b.vx;
        if (B(0x74)) {UNSUPPORTED_AT();return false;}
        uint16_t f=(s->b.flags15&32u) && B(0x72)?air(c,0xff):air(c,0);
        if (f&0x200u) s->b.flags15|=32u;
        if (f&1u) SB(0x72,(f&8u)?0xff:0);
    }
    SB(0x70,(s->b.flags15&64u)?0xff:0);
    return true;
}
static neo_soldier_result_t death_tick(ctx_t *c) { /* 040DB4 / 040DF0 */
    neo_soldier_t *s=c->s;
    if (s->routine==0x40db4) {
        /* 040A00 registers the hurtbox only when +73 is zero (owner side). */
        if (!death_dispatch(c,false)) return NEO_SOLDIER_UNSUPPORTED;
    }
    /* 0405CE: off-screen removal */
    uint16_t x=(uint16_t)(HI(s->b.projected_x)+0x20u);
    if (x>=0x180u || HI(s->b.projected_y)<0xe0u) return NEO_SOLDIER_DELETED;
    if (!death_motion(c)) return NEO_SOLDIER_UNSUPPORTED;
    if (anim_step(c)) return NEO_SOLDIER_DELETED;
    if (c->unsupported) return NEO_SOLDIER_UNSUPPORTED;
    if (!(s->b.flags5f&8u)) {
        uint8_t t=B(0x71);
        if (t) {t--;if (!t) {s->b.flags5f|=8u;t=0x1e;} SB(0x71,t);}
        return NEO_SOLDIER_MORE;
    }
    uint8_t t=B(0x71);
    if (!t) return NEO_SOLDIER_DELETED;
    SB(0x71,t-1u);return NEO_SOLDIER_MORE;
}

/* ---- common tails ---------------------------------------------------------- */
static neo_soldier_result_t tail(ctx_t *c,bool crouch) { /* 0B051E / 0B0504 */
    neo_soldier_t *s=c->s;
    if (s->b.flags5f&128u) {if (!death_dispatch(c,crouch)) return NEO_SOLDIER_UNSUPPORTED;}
    target(c);
    neo_cull_bounds_t bounds=c->e->bounds;
    if (neo_cull_outside(&s->b,&bounds,soldier_cull_rect)) return NEO_SOLDIER_DELETED;
    prefix(c);
    return c->unsupported?NEO_SOLDIER_UNSUPPORTED:NEO_SOLDIER_MORE;
}
static neo_soldier_result_t retreat_tail(ctx_t *c) { /* 0B08EE */
    neo_soldier_t *s=c->s;
    neo_cull_bounds_t bounds=c->e->bounds;
    if (neo_cull_rectangle_outside(&s->b,&bounds,soldier_retreat_rect)) return NEO_SOLDIER_DELETED;
    /* 01903A: each live player's +3C/+40 (when its +7A/+7B is set) over the
     * script point 69B8/69BA; a missing player copies the other. */
    const neo_soldier_env_t *e=c->e;
    uint16_t p1x=e->point[0],p1y=e->point[1],p2x=e->point[0],p2y=e->point[1];
    if (e->player_state[0].alive) {if (e->player_state[0].follow7a) p1x=e->player_state[0].x;if (e->player_state[0].follow7b) p1y=e->player_state[0].y;}
    if (e->player_state[1].alive) {if (e->player_state[1].follow7a) p2x=e->player_state[1].x;if (e->player_state[1].follow7b) p2y=e->player_state[1].y;}
    if (!e->player_state[1].alive) {p2x=p1x;p2y=p1y;}
    if (!e->player_state[0].alive) {p1x=p2x;p1y=p2y;}
    uint16_t t84=HI(S32(L(0x84)));
    uint16_t d1=(uint16_t)neo_asr(S16(p1y),1);if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    uint16_t d0=(uint16_t)(p1x-t84);if (d0&0x8000u) d1=(uint16_t)(0u-d1);
    d0=(uint16_t)(d0+d1+0x8000u);
    /* 019504 leaves the projected-x fraction in D0.h and the high word of
     * (cached x - projected x) in D2.h; 01903A only writes the low words, so
     * SWAP+ASR.L #8 below shift those words into the result. */
    uint32_t px=(uint32_t)s->b.projected_x,cx=(uint32_t)s->b.cached_x;
    if (bounds.has_parent) {px-=(uint32_t)bounds.x;cx-=(uint32_t)bounds.previous_x;}
    uint32_t a=ASR(((uint32_t)d0<<16)|(px&0xffffu),8);
    uint16_t d3=(uint16_t)neo_asr(S16(p2y),1);if (d3&0x8000u) d3=(uint16_t)(0u-d3);
    uint16_t d2=(uint16_t)(p2x-t84);if (d2&0x8000u) d3=(uint16_t)(0u-d3);
    d2=(uint16_t)(d2+d3+0x8000u);
    uint32_t b=ASR(((uint32_t)d2<<16)|((cx-px)>>16),8)+a;
    if (W(0x92)) {
        uint16_t d=(uint16_t)(W(0x8e)+c->e->camera_word-HI(s->b.projected_x)+0x8000u);
        b+=ASR((uint32_t)d<<16,6);
    }
    b=ASR(b,6);
    uint32_t phase=(uint16_t)(W(0x8c)+0x77u);SW(0x8c,phase);
    uint32_t v=L(0x88);
    if (S32(v)>=0) phase=0u-phase;
    phase<<=1;
    uint32_t d1l=ASR(v,6);d1l=(d1l&0xffff0000u)|((d1l+phase)&0xffffu);
    b-=d1l;b+=v;SL(0x88,b);
    int64_t sum=(int64_t)S32(b)+S32(L(0x84));
    if (sum>=-2147483648LL && sum<=2147483647LL) SL(0x84,(uint32_t)sum);
    if (s->b.flags5f&128u) {if (!death_dispatch(c,false)) return NEO_SOLDIER_UNSUPPORTED;}
    prefix(c);
    return c->unsupported?NEO_SOLDIER_UNSUPPORTED:NEO_SOLDIER_MORE;
}

/* ---- the routine family ---------------------------------------------------- */
static void gravity_blend(neo_soldier_t *s) { /* shared by the air states */
    s->b.vx=S32((uint32_t)s->b.vx+ASR((uint32_t)s->b.scalar62-(uint32_t)s->b.vx,5));
    s->b.vy=S32((uint32_t)s->b.vy+ASR(0xfff00000u-(uint32_t)s->b.vy,5));
}
static int32_t facing_value(const neo_soldier_t *s,int32_t v) {return (s->b.flags15&1u)?v:S32(0u-(uint32_t)v);}
static uint16_t speed_from(int32_t v) {
    uint32_t m=(uint32_t)v;if (v<0) m=0u-m;
    return (uint16_t)(((m<<6)>>16)+0x80u);
}
/* AF65A..AF6BA initial movement, AF4F6..AF56C reposition, etc. return false on
 * an unsupported path (state is then in an unspecified partial update). */
static neo_soldier_result_t run(ctx_t *c) {
    neo_soldier_t *s=c->s;neo_contact_actor_t *a=&s->b;
    for (unsigned guard=0;guard<8;guard++) switch (s->routine) {
    case 0xaf65a: { /* walk start */
        START(0x23d510);
        neo_soldier_motion_seed_t seed=neo_soldier_seed_from_rng(c->e->rng,a->flags15,(int16_t)W(0x92));
        a->scalar62=seed.value_62;s->anim.speed=seed.value_26;s->routine=0xaf6c0;
    }   /* fall through */
    case 0xaf6c0: {
        if (!(a->flags15&64u)) {s->routine=0xafca4;continue;}
        uint16_t f=ground(c,NULL);
        if ((f&0x200u) && (W(0x78)&1u)) s->routine=0xaf7f6;
        anim_step(c);
        uint16_t d=(uint16_t)(HI(S32(L(0x84)))-HI(a->projected_x));
        if (S16((uint16_t)(d^HI(a->scalar62)))<0) {s->routine=0xaf890;if (B(0x7c)&2u) s->routine=0xaf8d4;}
        if ((uint8_t)(a->kind_angle>>8)==3u) s->routine=0xaf890;
        if (pred_contact(s)) s->routine=0xafbc0;
        if (pred_grid(c)) s->routine=0xafd94;
        if (a->flags5f&1u) s->routine=pred_wall(s)?0xaf7b0:0xafc68;
        if (pred_region(c)) s->routine=0xb0336;
        if (B(0x7c)&1u) {uint32_t r=action_routine(choose(c));if (r) s->routine=r;}
        pred_object(c);
        if (!W(0x74)) s->routine=0xb063e;
        return tail(c,false);
    }
    case 0xaf890:START(0x23d594);s->routine=0xaf8a6; /* fall through */
    case 0xaf8a6:
        a->scalar62=neo_soldier_damp_motion(a->scalar62);
        if (!(a->flags15&64u)) {s->routine=0xafce2;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xaf380;
        return tail(c,false);
    case 0xaf8d4:START(0x23ddb4);s->routine=0xaf8ea; /* fall through */
    case 0xaf8ea:
        if (!(a->flags15&64u)) {s->routine=0xafce2;continue;}
        a->scalar62=S32((uint32_t)a->scalar62-ASR((uint32_t)a->scalar62,2));
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xaf65a;
        return tail(c,false);
    case 0xaf380:START(0x23d5e2);a->scalar62=0;s->routine=0xaf39e; /* fall through */
    case 0xaf39e: {
        if (!(a->flags15&64u)) {s->routine=0xafce2;continue;}
        ground(c,NULL);anim_step(c);
        if ((uint8_t)(a->kind_angle>>8)!=3u) {
            uint16_t distance;bool away=facing_away(s,&distance);
            if (distance>=0x10u) s->routine=0xaf4f6;
            if (!away) {if (distance>=0x30u) s->routine=0xaf65a;}
            else {if (distance>=0x30u) s->routine=0xafc68;if (distance>=0x48u) s->routine=0xaf8d4;}
            if (pred_contact(s)) s->routine=0xafbc0;
            if (pred_grid(c)) s->routine=0xafd94;
        }
        if (pred_region(c)) s->routine=0xb0336;
        {uint32_t r=action_routine(choose(c));if (r) s->routine=r;}
        if (!(B(0x7c)&1u)) s->routine=0xaf4b4;
        pred_object(c);
        if (pred_screen(s)) s->routine=0xb0396;
        if (!W(0x74)) s->routine=0xb063e;
        return tail(c,false);
    }
    case 0xaf4b4:START(0x23ddd6);a->scalar62=0;s->routine=0xaf4d2; /* fall through */
    case 0xaf4d2:
        if (!(a->flags15&64u)) {s->routine=0xafce2;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xaf380;
        return tail(c,false);
    case 0xaf4f6: {
        uint16_t d=(uint16_t)(HI(S32(L(0x84)))-HI(a->projected_x));
        uint16_t o=(uint16_t)(d^((a->flags15&1u)?0u:0xffffu));
        if (!anim_start(c,(o&0x8000u)?0x23e0da:0x23e01e)) return NEO_SOLDIER_UNSUPPORTED;
        int32_t base=neo_asr(S32(L(0x84)-(uint32_t)a->projected_x),5);
        int32_t random=(int32_t)(neo_rng_next(c->e->rng)&0x7fffu)-0x4000;
        a->scalar62=S32((uint32_t)base+(uint32_t)random);s->anim.speed=speed_from(a->scalar62);
        s->routine=0xaf572;
    }   /* fall through */
    case 0xaf572: {
        if (!(a->flags15&64u)) {s->routine=0xafca4;continue;}
        uint32_t kind=0;ground(c,&kind);
        s->render.variant61=0;
        uint16_t angle=(uint16_t)((uint16_t)kind-0x2800u);
        if (angle>=0x3000u) {
            angle=(uint16_t)(angle-0x1800u);
            uint16_t o=(uint16_t)(angle^((a->flags15&1u)?0u:0xffffu));
            s->render.variant61=(o&0x8000u)?2:1;
        }
        if (anim_step(c)) s->routine=0xaf380;
        uint16_t distance;
        if (facing_away(s,&distance) && distance>=0x30u) s->routine=0xaf8d4;
        if (pred_region(c)) s->routine=0xb0336;
        {uint32_t r=action_routine(choose(c));if (r) s->routine=r;}
        if (pred_grid(c)) s->routine=0xafd94;
        if ((a->flags5f&1u) && pred_wall(s)) s->routine=0xaf7f6;
        pred_object(c);
        if (!W(0x74)) s->routine=0xb063e;
        return tail(c,false);
    }
    case 0xaf918:
        START(0x23d7ee);
        SW(0x70,W(0x72));a->scalar62=0;s->routine=0xaf964; /* fall through */
    case 0xaf964:
        stationary(c);
        if (a->flags15&64u) ground(c,NULL); else air_drop(c);
        if (anim_step(c)) s->routine=0xaf380;
        return tail(c,false);
    case 0xb0336:
        START(0x23ef98);a->scalar62=0;s->routine=0xb0354; /* fall through */
    case 0xb0354:
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xaf380;
        if (!(a->flags15&64u)) s->routine=0xafce2;
        return tail(c,false);
    case 0xafbc0: /* jump */
        a->vy=0x8a000;a->scalar62=facing_value(s,0x28000);a->vx=a->scalar62;
        START(0x23e1da);s->routine=0xafbf8; /* fall through */
    case 0xafbf8:
        gravity_blend(s);air(c,0);anim_step(c);
        if (a->flags15&64u) s->routine=0xafd52;
        return tail(c,false);
    case 0xafc68: /* jump back */
        START(0x23e2ea);
        {int32_t v=0x30000;if (a->flags15&1u) v=S32(0u-(uint32_t)v);a->scalar62=v;a->vx=v;}
        a->vy=0x20000;s->routine=0xafd10;continue;
    case 0xafca4: { /* left the ground while walking */
        if ((B(0x7c)&2u) && pred_contact(s)) {s->routine=0xafbc0;continue;}
        if (pred_grid(c)) {s->routine=0xafd94;continue;}
        uint16_t r=(uint16_t)(neo_rng_next(c->e->rng)&3u);
        if (r==1u) {s->routine=0xafd94;continue;}
        if (r==2u) {s->routine=0xafbc0;continue;}
        a->vx=a->scalar62;goto fall_start;
    }
    /* ---- stairs/ledges of the object list (0B00C0..0B0332) ---- */
    case 0xb00c0:a->flags15&=(uint8_t)~1u;START(0x23e43c);s->routine=0xb00dc; /* fall through */
    case 0xb00dc:
        a->vy=0;marker_follow(c,B(0x7e));
        if (anim_step(c)) s->routine=0xb0102;
        return tail(c,false);
    case 0xb0102:START(0x23e47c);a->vy=0x15555;s->routine=0xb0120; /* fall through */
    case 0xb0120: {
        int16_t h=marker_follow(c,B(0x7e));
        if (h<0) s->routine=0xafce2;
        else if ((uint16_t)h<0x24u) s->routine=0xb0168;
        if (anim_step(c)) START(0x23e47c);
        return tail(c,false);
    }
    case 0xb0168:START(0x23e4ee);a->vx=0;a->vy=0x30000;s->routine=0xb018e; /* fall through */
    case 0xb018e:
        a->vy=0;marker_follow(c,B(0x7e));
        if (anim_step(c)) {a->vx=S32(0xfffe0000u);a->scalar62=S32(0xfffe0000u);a->vy=0x20000;s->routine=0xafd94;}
        return tail(c,false);
    case 0xb01ce:a->flags15&=(uint8_t)~1u;START(0x23e5b8);a->vy=0;s->routine=0xb01f2; /* fall through */
    case 0xb01f2:
        marker_follow(c,B(0x7e));
        if (anim_step(c)) s->routine=0xb0210;
        return tail(c,false);
    case 0xb0210:START(0x23e6ae);a->vy=S32(0xfffeaaabu);s->routine=0xb022e; /* fall through */
    case 0xb022e:
        if (marker_follow(c,B(0x7e))<0) s->routine=0xafce2;
        if (anim_step(c)) START(0x23e6ae);
        if (a->flags15&64u) s->routine=0xb0272;
        return tail(c,false);
    case 0xb0272:START(0x23e71a);a->vx=0;a->vy=0;s->routine=0xb0298; /* fall through */
    case 0xb0298:
        air(c,0);
        if (anim_step(c)) s->routine=0xaf380;
        return tail(c,false);
    case 0xb0396: /* idle on a vehicle (+76 = 3/4): random program of 0B037E */
        START(soldier_idle_programs[(neo_rng_next(c->e->rng)&0xcu)>>2]);a->scalar62=0;s->routine=0xb03c0; /* fall through */
    case 0xb03c0: {
        if (!(a->flags15&64u)) {s->routine=0xafce2;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xb0396;
        if (B(0x80)&0x80u) return tail(c,false);
        const neo_soldier_env_t *e=c->e;
        if (e->player_state[0].vehicle || e->player_state[1].vehicle) return tail(c,false);   /* 0CA2F8 */
        for (unsigned p=0;p<2;p++) if (e->player_state[p].alive && !e->player_state[p].follow7a) return tail(c,false);
        neo_control_frame_t f=frame(c);
        if (neo_target_pick(a,&f,soldier_alert_rect).valid) s->routine=0xb0466;          /* 01AFFC */
        if (grid_lookup(e->grid,HI(a->projected_x),(uint16_t)(HI(a->projected_y)+0x10u))) s->routine=0xb0466; /* 00510C */
        return tail(c,false);
    }
    case 0xb0466: /* alerted: jump down */
        event(c,NEO_SOLDIER_EVENT_SOUND,0x104a,NULL);
        START(0x23de6a);
        if (!(B(0x7c)&1u)) a->flags15^=1u;
        s->routine=0xb0496; /* fall through */
    case 0xb0496: {
        if (!(a->flags15&64u)) air_drop(c); else stationary(c);
        if (anim_step(c)) {
            /* 00505A at (+0x40,+0x20), (-0x40,+0x20), (0,+0x20) of the
             * projected position. */
            uint16_t x=HI(a->projected_x),y=(uint16_t)(HI(a->projected_y)+0x20u);
            neo_grid_splat(c->e->grid,(uint16_t)(x+0x40u),y);neo_grid_splat(c->e->grid,(uint16_t)(x-0x40u),y);neo_grid_splat(c->e->grid,x,y);
            s->routine=0xaf65a;
        }
        return tail(c,false);
    }
    case 0xafce2:
        a->vx=0;
    fall_start:
        START(0x23e2ea);
        a->scalar62=0;a->vy=0;s->routine=0xafd10; /* fall through */
    case 0xafd10:
        gravity_blend(s);air(c,0);
        if (anim_step(c) && (a->flags15&64u)) s->routine=0xafd52;
        return tail(c,false);
    case 0xafd52:
        START(0x23dfe4);a->scalar62=0;s->routine=0xafd70; /* fall through */
    case 0xafd70:
        if (!(a->flags15&64u)) {s->routine=0xafce2;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xaf380;
        return tail(c,false);
    case 0xafd94: /* drop to a crouch */
        START(0x23d692);
        a->vx=a->scalar62;a->scalar62=0;s->routine=0xafdb8; /* fall through */
    case 0xafdb8:
        gravity_blend(s);air(c,0);
        if (anim_step(c) && (a->flags15&64u)) s->routine=0xafdfa;
        return tail(c,true);
    case 0xafdfa:
        START(0x23d712);
        SW(0x82,(uint16_t)((neo_rng_next(c->e->rng)&0x3fu)+0x10u));a->scalar62=0;s->routine=0xafe2a; /* fall through */
    case 0xafe2a:
        if (!(a->flags15&64u)) {s->routine=0xb0056;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xafe4e;
        return tail(c,true);
    case 0xafe4e:
        if (pred_grid(c)) SW(0x82,(uint16_t)(W(0x82)+(neo_rng_next(c->e->rng)&0x1fu)));
        START(0x23dfd2);a->scalar62=0;s->routine=0xafe82; /* fall through */
    case 0xafe82:
        if (!(a->flags15&64u)) {s->routine=0xb0056;continue;}
        ground(c,NULL);anim_step(c);
        { uint16_t d=(uint16_t)(HI(S32(L(0x84)))-HI(a->projected_x));if (d&0x8000u) d=(uint16_t)(0u-d);
          if (d>=0x10u) s->routine=0xaff16; }
    crouch_decisions: {
        uint8_t act=choose(c);
        if (act) s->routine=act==1u?0xaffcc:0xb0014;
        uint16_t t=(uint16_t)(W(0x82)-1u);SW(0x82,t);
        if (t&0x8000u) s->routine=0xb0014;
        if (!(B(0x7c)&1u)) s->routine=0xaff82;
        if (!W(0x74)) s->routine=0xb0764;
        if ((uint8_t)a->kind_angle!=0x40u) s->routine=0xb0014;
        return tail(c,true);
    }
    case 0xaff16: {
        START(0x23df8e);
        int32_t v=neo_asr(S32(L(0x84)-(uint32_t)a->projected_x),5);
        if (v>0x18000) v=0x18000; else if (v<-0x18000) v=-0x18000;
        a->scalar62=v;s->routine=0xaff5e;
    }   /* fall through */
    case 0xaff5e:
        if (!(a->flags15&64u)) {s->routine=0xb0056;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xafe4e;
        goto crouch_decisions;
    case 0xaff82:
        if (B(0x80)&0x80u) {s->routine=0xb0014;continue;}
        START(0x23f0fc);a->scalar62=0;s->routine=0xaffa8; /* fall through */
    case 0xaffa8:
        if (!(a->flags15&64u)) {s->routine=0xb0056;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xafe4e;
        return tail(c,true);
    case 0xaffcc:
        START(0x23d8c0);
        SW(0x70,W(0x72));a->scalar62=0;s->routine=0xafff0; /* fall through */
    case 0xafff0:
        if (!(a->flags15&64u)) {s->routine=0xb0056;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xafe4e;
        return tail(c,true);
    case 0xb0014:
        START(0x23db7e);a->scalar62=0;s->routine=0xb0032; /* fall through */
    case 0xb0032:
        if (!(a->flags15&64u)) {s->routine=0xb0056;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xaf380;
        return tail(c,false);
    case 0xb0056:
        START(0x23d6ec);
        a->vx=a->scalar62;a->scalar62=0;a->vy=0;s->routine=0xb0082; /* fall through */
    case 0xb0082:
        gravity_blend(s);air(c,0);anim_step(c);
        if (a->flags15&64u) s->routine=0xafdfa;
        return tail(c,false);
    case 0xb063e: { /* retreat */
        SL(0x88,(uint32_t)((neo_rng_next(c->e->rng)&0xffu)-0x80u));
        if (!anim_start(c,(B(0x7c)&0x80u)?0x23e394:0x23d510)) return NEO_SOLDIER_UNSUPPORTED;
        int32_t v=(int32_t)(neo_rng_next(c->e->rng)&0xffffu)+0x40000;
        a->scalar62=facing_value(s,v);s->anim.speed=speed_from(a->scalar62);
        SL(0x84,(uint32_t)a->projected_x);s->routine=0xb06c4;
    }   /* fall through */
    case 0xb06c4: {
        if (!(a->flags15&64u)) {s->routine=0xb083e;continue;}
        if (a->flags5f&1u) {s->routine=0xb079e;continue;}
        uint32_t d0=L(0x84)-(uint32_t)a->projected_x+((uint32_t)a->scalar62<<3);
        if (S32(d0^(uint32_t)a->scalar62)<0) {
            if (!(neo_rng_next(c->e->rng)&7u)) {s->routine=0xafbc0;continue;}
            a->scalar62=S32(0u-(uint32_t)a->scalar62);a->flags15^=1u;
        }
        uint16_t x=(uint16_t)(HI(a->projected_x)+((a->flags15&1u)?0x40u:0xffc0u)),y=HI(a->projected_y);
        delay_update(c,grid_lookup(c->e->grid,x,y),x,y);
        ground(c,NULL);anim_step(c);
        if (pred_contact(s)) s->routine=0xafbc0;
        return retreat_tail(c);
    }
    case 0xb0750: /* idle variants entered with +74 exhausted */
        START(0x23db34);s->routine=0xb077a;continue;
    case 0xb0764:
        START(0x23db7e);s->routine=0xb077a; /* fall through */
    case 0xb077a:
        if (!(a->flags15&64u)) {s->routine=0xb083e;continue;}
        stationary(c);
        if (anim_step(c)) s->routine=0xb063e;
        return retreat_tail(c);
    case 0xb079e: /* +5F bit 0: back off one way */
        START(0x23edf4);
        a->scalar62=facing_value(s,0x10000);SL(0x84,(uint32_t)a->projected_x);s->routine=0xb07d0; /* fall through */
    case 0xb07d0: {
        uint16_t x=(uint16_t)(((uint16_t)HI(a->scalar62)<<2)+HI(a->projected_x)),y=HI(a->projected_y);
        delay_update(c,0xff,x,y);
        if ((int16_t)((uint16_t)(HI(S32(L(0x84)))-HI(a->projected_x))^(uint16_t)HI(a->scalar62))<0)
            a->scalar62=S32(0u-(uint32_t)a->scalar62);
        if (!(a->flags15&64u)) {s->routine=0xb083e;continue;}
        ground(c,NULL);
        if (!(a->flags5f&1u)) {
            if ((int16_t)HI(a->scalar62)>=0) a->flags15|=1u; else a->flags15&=254u;
            s->routine=0xb063e;
        }
        anim_step(c);
        return retreat_tail(c);
    }
    case 0xb083e:
        a->scalar62=facing_value(s,0x28000);a->vx=a->scalar62;
        START(0x23e1da);s->routine=0xb086e; /* fall through */
    case 0xb086e:
        gravity_blend(s);air(c,0);anim_step(c);
        if (a->flags15&64u) s->routine=0xb08ac;
        return retreat_tail(c);
    case 0xb08ac:
        START(0x23dfe4);a->scalar62=0;s->routine=0xb08ca; /* fall through */
    case 0xb08ca:
        if (!(a->flags15&64u)) {s->routine=0xafce2;continue;}
        ground(c,NULL);
        if (anim_step(c)) s->routine=0xb063e;
        return retreat_tail(c);
    case 0x40db4:case 0x40df0:return death_tick(c);
    default:return NEO_SOLDIER_UNSUPPORTED;
    }
    return NEO_SOLDIER_UNSUPPORTED;
}
neo_soldier_result_t neo_soldier_tick(neo_soldier_t *s,neo_soldier_env_t *e) {
    ctx_t c={s,e,false};
    neo_soldier_result_t r=run(&c);
    if (c.unsupported) return NEO_SOLDIER_UNSUPPORTED;
    if (r==NEO_SOLDIER_DELETED) s->active=false;
    return r;
}

/* ---- constructor ------------------------------------------------------------ */
static uint16_t table_word(const uint16_t *table,const neo_control_frame_t *f,uint8_t variant) { /* 0260C8 type 1 */
    unsigned rank=f->mode88==2?f->rank:3u;
    unsigned mask=(1u<<((rank&7u)+1u))-1u;
    unsigned row=soldier_rank_lookup[0xffu&mask];
    unsigned coop=f->active_mask==3u;
    return table[((row-1u)*2u+coop)*6u+variant];
}
bool neo_soldier_construct(neo_soldier_t *s,const neo_soldier_spawn_t *sp,neo_soldier_env_t *e) {
    /* +76 modes 1..4 (0AF1EE): 1 ground, 2 run in (0AFF16), 3/4 idle on a
     * vehicle facing right/left (0B0396). 0/5/6/7 and +78 bit 0 (01495C
     * probe) are not used by the region script. */
    if (sp->words[4]<1u || sp->words[4]>4u || (sp->words[5]&1u) || sp->variant68>=12u) return false;
    neo_soldier_t backup=*s;
    memset(s,0,sizeof(*s));
    ctx_t c={s,e,false};
    /* Fresh node (003F24/01AB6E): position block, +15/+5F/+60/+61, +6E..+AF zero,
     * +1E=FFFF; flash 0xFF as in every observed construction. */
    s->active=true;s->anim.flash=0xff;s->b.word1e=0xffff;
    s->variant68=sp->variant68;s->group69=sp->group69;
    for (unsigned i=0;i<6;i++) SW(0x6e + 2*i,sp->words[i]);
    /* 01ABB6: placed relative to the scene root, mark/correction cleared. */
    s->b.x=sp->x;s->b.y=sp->y;s->b.parent_tag=ROOT_TAG;s->b.mark=0;s->b.correction=0;
    project(&c);
    /* 0AEF30 */
    neo_control_frame_t f=frame(&c);
    uint16_t d0=table_word(soldier_timer70_table,&f,s->variant68);
    if (W(0x70)) d0=(uint16_t)(((uint32_t)d0*W(0x70))>>8); else d0=(uint16_t)(d0>>8);
    SW(0x70,d0);
    d0=table_word(soldier_timer72_table,&f,s->variant68);
    uint16_t d1=W(0x72)?W(0x72):0x1e;
    SW(0x72,(uint16_t)(((uint32_t)d0*d1)>>8));
    SW(0x72,(uint16_t)(W(0x72)+(neo_rng_next(e->rng)&7u)));
    if (!W(0x74)) SW(0x74,0x5a);
    s->palette_resource=0x20e;
    s->b.field66=0x8820;s->b.word1e=0x8820;
    /* +76==1: 0146E8 floor search, then projection. */
    neo_kfloor_t fl;
    if (!neo_kernel_floor(e->kernel,s->b.x,s->b.y,kobj(s->b.parent_tag),(int32_t)0xff000000,
        (uint8_t)(s->b.flags15&32u?0:255),0,0,&fl)) {*s=backup;return false;}
    s->b.kind_angle=(uint16_t)(fl.kind>>8);s->b.x=fl.x;s->b.y=fl.y;s->b.vx=0;s->b.vy=0;s->b.parent_tag=ktag(fl.parent);
    project(&c);s->b.flags15|=64u;
    project(&c);
    if (S16(HI(s->b.projected_x))<0xa0) s->b.flags15|=1u;
    SW(0x84,(uint16_t)(neo_asr(S16((uint16_t)(HI(s->b.projected_x)-0xa0u)),2)+0xa0));
    SW(0x86,neo_rng_next(e->rng));
    target(&c);prefix(&c);
    SB(0x81,2);
    s->routine=(s->b.flags15&64u)?((uint8_t)(s->b.kind_angle>>8)==3u?0xaf380:0xaf65a):0xafce2;
    if (sp->words[4]==2u) s->routine=0xaff16;
    else if (sp->words[4]==3u) {s->b.flags15&=(uint8_t)~1u;s->routine=0xb0396;}
    else if (sp->words[4]==4u) {s->b.flags15|=1u;s->routine=0xb0396;}
    if (c.unsupported) {*s=backup;return false;}
    return true;
}
