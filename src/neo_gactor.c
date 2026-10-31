#include "neo_gactor.h"
#include <string.h>
#include "neo_target.h"
#include "neo_cull.h"
/* Shared services of the generic actor (see neo_gactor.h). Each function is
 * the native counterpart of one original helper, named by its ROM label. */
static int32_t S32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
#define TERRAIN_TAG 0x10c72eu
#define ROOT_TAG 0x10e968u
#define DYN_TAG(i) (0x10c06eu+64u*(unsigned)(i))
void ga_fail(neo_genv_t *e,uint32_t at) {if (!e->unsupported) e->unsupported_at=at;e->unsupported++;}
neo_contact_actor_t ga_contact(const neo_gactor_t *a) {
    neo_contact_actor_t c;
    c.x=S32(ga_l(a,0x28));c.y=S32(ga_l(a,0x2c));c.vx=S32(ga_l(a,0x30));c.vy=S32(ga_l(a,0x34));
    c.scalar62=S32(ga_l(a,0x62));c.projected_x=S32(ga_l(a,0x3c));c.projected_y=S32(ga_l(a,0x40));
    c.cached_x=S32(ga_l(a,0x44));c.cached_y=S32(ga_l(a,0x48));c.parent_tag=ga_l(a,0x38);
    c.kind_angle=ga_w(a,0x6c);c.field66=ga_w(a,0x66);c.word1e=ga_w(a,0x1e);
    c.flags15=ga_b(a,0x15);c.flags5f=ga_b(a,0x5f);c.timer5a=ga_b(a,0x5a);c.mark=ga_b(a,0x4c);c.correction=ga_b(a,0x4d);
    return c;
}
void ga_set_contact(neo_gactor_t *a,const neo_contact_actor_t *c) {
    /* Only changed fields are stored, so the written-byte mask (stale-field
     * tracking) reflects what the translated routine actually changed. */
    #define PUTL(o,v) do { if (ga_l(a,o)!=(uint32_t)(v)) ga_sl(a,o,(uint32_t)(v)); } while (0)
    #define PUTW(o,v) do { if (ga_w(a,o)!=(uint16_t)(v)) ga_sw(a,o,(uint16_t)(v)); } while (0)
    #define PUTB(o,v) do { if (ga_b(a,o)!=(uint8_t)(v)) ga_sb(a,o,(uint8_t)(v)); } while (0)
    PUTL(0x28,c->x);PUTL(0x2c,c->y);PUTL(0x30,c->vx);PUTL(0x34,c->vy);
    PUTL(0x62,c->scalar62);PUTL(0x3c,c->projected_x);PUTL(0x40,c->projected_y);
    PUTL(0x44,c->cached_x);PUTL(0x48,c->cached_y);PUTL(0x38,c->parent_tag);
    PUTW(0x6c,c->kind_angle);PUTW(0x66,c->field66);PUTW(0x1e,c->word1e);
    PUTB(0x15,c->flags15);PUTB(0x5f,c->flags5f);PUTB(0x5a,c->timer5a);PUTB(0x4c,c->mark);PUTB(0x4d,c->correction);
    #undef PUTL
    #undef PUTW
    #undef PUTB
}
bool ga_parent(neo_genv_t *e,uint32_t tag,neo_contact_parent_t *out) {
    if (!tag) return false;
    if (!e->parent || !e->parent(e->ctx,tag,out)) {ga_fail(e,0x1abcc);return false;}
    return true;
}
void ga_project(neo_gactor_t *a,neo_genv_t *e) {
    neo_contact_actor_t c=ga_contact(a);neo_contact_parent_t p;
    bool has=ga_parent(e,c.parent_tag,&p);
    if (c.parent_tag && !has) return;
    neo_contact_project(&c,has?&p:NULL,e->epoch);ga_set_contact(a,&c);
}
void ga_place(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy,uint32_t parent) {
    ga_sl(a,0x28,(uint32_t)(int32_t)dx<<16);ga_sl(a,0x2c,(uint32_t)(int32_t)dy<<16);ga_sl(a,0x38,parent);ga_sb(a,0x4c,0);
    ga_project(a,e);
}
void ga_place_from(neo_gactor_t *a,const neo_gactor_t *s,neo_genv_t *e,int16_t dx,int16_t dy) {
    /* 01B242: layer words and flip bits 0..2 from the source. */
    ga_sw(a,0x1e,ga_w(s,0x1e));ga_sw(a,0x66,ga_w(s,0x66));
    ga_sb(a,0x15,(uint8_t)(ga_b(a,0x15)|(ga_b(s,0x15)&7u)));
    if (ga_b(a,0x15)&1u) dx=(int16_t)-dx;
    if (ga_b(a,0x15)&2u) dy=(int16_t)-dy;
    /* 01ACF2: relative to the scene root (RAM 10E968). */
    neo_contact_parent_t root;
    if (!ga_parent(e,ROOT_TAG,&root)) return;
    uint32_t ox=(uint32_t)(int32_t)dx<<16,oy=(uint32_t)(int32_t)dy<<16;
    uint32_t d0=ox+ga_l(s,0x3c)-(uint32_t)root.x,d1=oy+ga_l(s,0x40)-(uint32_t)root.y;
    uint32_t d2=ox+ga_l(s,0x44)-(uint32_t)root.previous_x,d3=oy+ga_l(s,0x48)-(uint32_t)root.previous_y;
    ga_sl(a,0x28,d0);ga_sl(a,0x2c,d1);ga_sl(a,0x30,d0-d2);ga_sl(a,0x34,d1-d3);ga_sl(a,0x38,ROOT_TAG);
    ga_project(a,e);
}
void ga_move(neo_gactor_t *a,neo_genv_t *e) {
    ga_sl(a,0x28,ga_l(a,0x28)+ga_l(a,0x30));ga_sl(a,0x2c,ga_l(a,0x2c)+ga_l(a,0x34));
    ga_project(a,e);
}
static uint8_t kobj(uint32_t tag) {
    if (tag>=DYN_TAG(0) && tag<DYN_TAG(NEO_KDYN_COUNT) && !((tag-DYN_TAG(0))&63u)) return (uint8_t)(NEO_KOBJ_DYN0+(tag-DYN_TAG(0))/64u);
    return tag==TERRAIN_TAG || tag==0x10d7b6u?NEO_KOBJ_TERRAIN:tag==ROOT_TAG?NEO_KOBJ_ROOT:tag?0xff:NEO_KOBJ_NONE;
}
static uint32_t terrain_tag=TERRAIN_TAG;
static uint32_t ktag(uint8_t id) {
    if (id>=NEO_KOBJ_DYN0 && id<NEO_KOBJ_DYN0+NEO_KDYN_COUNT) return DYN_TAG(id-NEO_KOBJ_DYN0);
    return id==NEO_KOBJ_TERRAIN?terrain_tag:id==NEO_KOBJ_ROOT?ROOT_TAG:0;
}
static void self(neo_gactor_t *a,neo_genv_t *e) {
    terrain_tag=e->terrain_tag?e->terrain_tag:TERRAIN_TAG;
    uint8_t d=ga_b(a,0x6a);e->kernel->self_dyn=d<NEO_KDYN_COUNT?(uint8_t)(d+1):0;e->kernel->self_block=NEO_GA_TAG(a->index);
}
static uint8_t sweep_400b,sweep_400d;   /* RAM 10C00B/10C00D for the next sweep (ga_*_flags) */
static bool sweep(neo_gactor_t *a,neo_genv_t *e,neo_contact_actor_t *c,const neo_contact_request_t *r,
    uint32_t mode,uint8_t b4008,uint8_t b4009,neo_kres_t *out) {
    uint8_t b400b=sweep_400b,b400d=sweep_400d;sweep_400b=sweep_400d=0;
    uint8_t parent=kobj(r->parent_tag);
    int32_t x=r->x,y=r->y,dx=r->dx,dy=r->dy;
    if (parent==0xff) {
        /* An actor's position block as A0 (the camel rider 09FEAA dying on
         * its camel): 01AC4C converts to absolute coordinates (A1 = 0) with
         * the block's previous and current positions, as for any object. */
        neo_contact_parent_t p;
        if ((r->parent_tag&0xff000000u)!=NEO_GA_TAG_BASE || !ga_parent(e,r->parent_tag,&p)) {ga_fail(e,0x13968);return false;}
        int32_t ex=(int32_t)((uint32_t)x+(uint32_t)dx+(uint32_t)p.x),ey=(int32_t)((uint32_t)y+(uint32_t)dy+(uint32_t)p.y);
        x=(int32_t)((uint32_t)x+(uint32_t)p.previous_x);y=(int32_t)((uint32_t)y+(uint32_t)p.previous_y);
        dx=(int32_t)((uint32_t)ex-(uint32_t)x);dy=(int32_t)((uint32_t)ey-(uint32_t)y);
        parent=NEO_KOBJ_NONE;
    }
    self(a,e);
    neo_kreq_t q={x,y,dx,dy,parent,mode,b4008,b4009,0,b400b,b400d};
    neo_kres_t k;
    if (!neo_kernel_sweep(e->kernel,&q,&k)) {ga_fail(e,0x13968);return false;}
    neo_contact_result_t res={k.x,k.y,k.vx,k.vy,kobj(r->parent_tag)==k.parent?r->parent_tag:ktag(k.parent),k.kind,k.flags};
    neo_contact_parent_t p;bool has=res.parent_tag?ga_parent(e,res.parent_tag,&p):false;
    if (res.parent_tag && !has) return false;
    neo_contact_commit(c,&res,has?&p:NULL,e->epoch);
    /* 014506..014516: after the projection, a body on the ground (+15 bit
     * 6) updates its depth word (01AD8C). */
    if ((c->flags15&0x40u) && (c->field66&0xf000u)==0x8000u) c->word1e=(uint16_t)(c->field66-(uint16_t)((uint32_t)c->projected_y>>16));
    if (out) *out=k;
    return true;
}
uint16_t ga_air_kind(neo_gactor_t *a,neo_genv_t *e,uint32_t *kind) {
    neo_contact_actor_t c=ga_contact(a);
    neo_contact_request_t r=neo_contact_prepare_air(&c);
    neo_kres_t k;
    if (!sweep(a,e,&c,&r,0,r.mode8,0,&k)) {ga_set_contact(a,&c);return 0;}
    ga_set_contact(a,&c);if (kind) *kind=k.kind;return k.flags;
}
uint16_t ga_air(neo_gactor_t *a,neo_genv_t *e) {return ga_air_kind(a,e,NULL);}
uint16_t ga_air_flags(neo_gactor_t *a,neo_genv_t *e,uint8_t b400b,uint8_t b400d,uint32_t *kind) {
    sweep_400b=b400b;sweep_400d=b400d;uint16_t f=ga_air_kind(a,e,kind);sweep_400b=sweep_400d=0;return f;
}
uint16_t ga_ground_flags(neo_gactor_t *a,neo_genv_t *e,uint8_t b400b,uint8_t b400d,uint32_t *kind) {
    sweep_400b=b400b;sweep_400d=b400d;uint16_t f=ga_ground(a,e,kind);sweep_400b=sweep_400d=0;return f;
}
uint16_t ga_air_d4(neo_gactor_t *a,neo_genv_t *e,uint16_t *d4) {
    neo_contact_actor_t c=ga_contact(a);
    neo_contact_request_t r=neo_contact_prepare_air(&c);
    neo_kres_t k;
    if (!sweep(a,e,&c,&r,0,r.mode8,0,&k)) {ga_set_contact(a,&c);return 0;}
    ga_set_contact(a,&c);if (d4) *d4=k.w4068;return k.flags;
}
uint16_t ga_ground(neo_gactor_t *a,neo_genv_t *e,uint32_t *kind) {
    neo_contact_actor_t c=ga_contact(a);
    neo_contact_request_t r=neo_contact_prepare_ground(&c);
    neo_kres_t k;
    if (!sweep(a,e,&c,&r,r.mode,r.mode8,0xff,&k)) {ga_set_contact(a,&c);return 0;}
    ga_set_contact(a,&c);if (kind) *kind=k.kind;return k.flags;
}
void ga_stationary(neo_gactor_t *a,neo_genv_t *e) {
    neo_contact_actor_t c=ga_contact(a);int32_t vx=c.vx,vy=c.vy;
    neo_contact_request_t r={c.x,c.y,0,0,c.parent_tag,0,(uint8_t)(c.flags15&32u?0:255),0,0,0};
    sweep(a,e,&c,&r,0,r.mode8,0,NULL);
    c.vx=vx;c.vy=vy;ga_set_contact(a,&c);
}
void ga_floor(neo_gactor_t *a,neo_genv_t *e) {
    neo_contact_actor_t c=ga_contact(a);
    uint8_t parent=kobj(c.parent_tag);
    if (parent==0xff) {
        /* A0 = another actor's position block (the civilians 031F52 placed
         * on their leader by 01AD6E): 013DB2 reads its +14/+18 (actor
         * +3C/+40); the same point relative to the scene root. */
        neo_contact_parent_t p,r;
        if (!ga_parent(e,c.parent_tag,&p) || !ga_parent(e,ROOT_TAG,&r)) {ga_fail(e,0x146e8);return;}
        c.x=(int32_t)((uint32_t)c.x+(uint32_t)p.x-(uint32_t)r.x);c.y=(int32_t)((uint32_t)c.y+(uint32_t)p.y-(uint32_t)r.y);
        c.parent_tag=ROOT_TAG;parent=NEO_KOBJ_ROOT;
    }
    self(a,e);
    neo_kfloor_t fl;
    if (!neo_kernel_floor(e->kernel,c.x,c.y,parent,(int32_t)0xff000000,(uint8_t)(c.flags15&32u?0:255),0,0,&fl)) {ga_fail(e,0x146e8);return;}
    ga_sw(a,0x6c,(uint16_t)(fl.kind>>8));
    ga_sl(a,0x28,(uint32_t)fl.x);ga_sl(a,0x2c,(uint32_t)fl.y);ga_sl(a,0x30,0);ga_sl(a,0x34,0);ga_sl(a,0x38,parent==fl.parent?c.parent_tag:ktag(fl.parent));  /* a layer object (10D7B6) stays itself */
    ga_project(a,e);
    ga_sb(a,0x15,(uint8_t)(ga_b(a,0x15)|64u));
}
/* 01495C: vertical probe by `distance` pixels from the actor (4009 = 0);
 * carry when the sweep meets a surface. */
bool ga_probe_vertical(neo_gactor_t *a,neo_genv_t *e,int16_t distance) {
    neo_contact_actor_t c=ga_contact(a);
    uint8_t parent=kobj(c.parent_tag);
    if (parent==0xff) {ga_fail(e,0x1495c);return false;}
    self(a,e);
    neo_kreq_t q={c.x,c.y,0,(int32_t)((uint32_t)(int32_t)distance<<16),parent,0,(uint8_t)(c.flags15&32u?0:255),0,0,0,0};
    neo_kres_t k;
    if (!neo_kernel_sweep(e->kernel,&q,&k)) {ga_fail(e,0x1495c);return false;}
    return (k.flags&1u)!=0;
}
bool ga_probe_forward(neo_gactor_t *a,neo_genv_t *e,int16_t distance) {
    /* 014850: forward probe (4009 = FF) by `distance` pixels; without a hit,
     * a downward one from its end by |distance| + 8 (carry on either hit). */
    neo_contact_actor_t c=ga_contact(a);
    uint8_t parent=kobj(c.parent_tag);
    if (parent==0xff) {ga_fail(e,0x14850);return false;}
    self(a,e);
    uint8_t b4008=(uint8_t)(c.flags15&32u?0:255);
    int32_t d3=(int32_t)((uint32_t)(int32_t)distance<<16);
    neo_kreq_t q={c.x,c.y,d3,0,parent,0,b4008,0xff,0,0,0};
    neo_kres_t k;
    if (!neo_kernel_sweep(e->kernel,&q,&k)) {ga_fail(e,0x14850);return false;}
    if (k.flags&1u) return true;
    if (d3>=0) d3=S32(0u-(uint32_t)d3);
    d3=S32((uint32_t)d3-0x80000u);
    neo_kreq_t q2={k.x,k.y,0,d3,k.parent,0,b4008,0xff,0,0,0};
    neo_kres_t k2;
    if (!neo_kernel_sweep(e->kernel,&q2,&k2)) {ga_fail(e,0x14850);return false;}
    return (k2.flags&1u)!=0;
}
bool ga_anim_start(neo_gactor_t *a,neo_genv_t *e,uint32_t tag,uint16_t speed) {
    const neo_anim_program_t *p=e->program?e->program(tag):NULL;
    if (!p) {ga_fail(e,tag);return false;}
    a->program=p;a->program_tag=tag;a->cursor=0;
    ga_sw(a,0x24,0);ga_sw(a,0x26,speed);ga_sb(a,0x18,0);
    return true;
}
/* +18 is the program's own counter (neo_anim_state_t field18) while it runs:
 * the program's byte commands on +18 (SET_BYTE/DEC_BYTE/COND_JUMP) see it. */
typedef struct {neo_gactor_t *a;neo_genv_t *e;ga_call_t call;bool fail;neo_anim_state_t *s;uint32_t missing;} anim_ctx_t;
static bool ev(void *context,neo_anim_kind_t kind,uint32_t asset,neo_anim_render_t *render) {
    anim_ctx_t *c=context;(void)render;
    if (kind==NEO_ANIM_REPLACE_PALETTE) {c->a->palette[0]=(uint16_t)asset;return true;}
    if (c->call && c->call(c->a,c->e,kind,asset,0)) return true;
    c->missing=asset;return false;
}
static bool rd(void *context,uint16_t field,uint8_t *value) {
    anim_ctx_t *c=context;if (field>=NEO_GA_BYTES) return false;*value=field==0x18 && c->s?c->s->field18:c->a->m[field];return true;
}
static bool wr(void *context,uint16_t field,unsigned size,uint32_t value) {
    anim_ctx_t *c=context;
    if (field+size>NEO_GA_BYTES) return false;
    if (size==1) ga_sb(c->a,field,(uint8_t)value); else if (size==2) ga_sw(c->a,field,(uint16_t)value); else ga_sl(c->a,field,value);
    if (c->s && field<=0x18 && field+size>0x18) c->s->field18=c->a->m[0x18];
    return true;
}
static void snd(void *context,uint16_t id) {anim_ctx_t *c=context;if (c->e->sound) c->e->sound(c->e->ctx,id);}
/* A callback reads and writes the node: the program's timer +24, speed +26,
 * flash +17 and counter +18 are in the node while it runs. */
static void state_out(anim_ctx_t *c) {
    if (!c->s) return;
    ga_sw(c->a,0x24,c->s->timer);ga_sw(c->a,0x26,c->s->speed);ga_sb(c->a,0x17,c->s->flash);ga_sb(c->a,0x18,c->s->field18);
}
static void state_in(anim_ctx_t *c) {
    if (!c->s) return;
    c->s->timer=ga_w(c->a,0x24);c->s->speed=ga_w(c->a,0x26);c->s->flash=ga_b(c->a,0x17);c->s->field18=ga_b(c->a,0x18);
}
static bool cl(void *context,uint32_t routine,uint16_t parameter,neo_anim_render_t *render) {
    anim_ctx_t *c=context;
    /* ... and the attributes +15 (facing: 286AE8). */
    state_out(c);if (render) ga_sb(c->a,0x15,(uint8_t)render->attributes);
    bool ok=c->call && c->call(c->a,c->e,NEO_ANIM_CALL,routine,parameter);
    /* The callback may change the draw palette +14 (025192 cycles it). */
    state_in(c);if (render) render->attributes=(uint16_t)(ga_b(c->a,0x14)<<8|ga_b(c->a,0x15));
    if (ok) return true;
    c->missing=routine;return false;
}
int ga_anim_step(neo_gactor_t *a,neo_genv_t *e,ga_call_t call) {
    neo_anim_state_t s={a->program,ga_l(a,0x50),a->cursor,ga_w(a,0x24),ga_w(a,0x26),ga_b(a,0x17),ga_b(a,0x18)};
    neo_anim_render_t r;memset(&r,0,sizeof(r));
    /* 0043D4: a descriptor's palette nibble >= 4 draws with +14 (high byte
     * of the attribute word); 0..3 select +19..+1C. */
    r.x=ga_w(a,0x3c);r.y=ga_w(a,0x40);r.attributes=(uint16_t)(ga_b(a,0x14)<<8|ga_b(a,0x15));r.z=ga_w(a,0x1e);r.correction=ga_b(a,0x4d);
    for (unsigned i=0;i<4;i++) r.palettes[i]=ga_b(a,0x19+i);
    r.flash_palette=ga_b(a,0x16);
    anim_ctx_t c={a,e,call,false,&s,0};
    neo_anim_actor_t actor={ev,rd,wr,snd,cl,&c};
    neo_anim_result_t res=neo_anim_step_actor(&s,&r,e->emit,e->emit_ctx,1024,&actor);
    a->program=s.program;a->cursor=s.cursor;ga_sl(a,0x50,s.field50_tag);ga_sw(a,0x24,s.timer);ga_sw(a,0x26,s.speed);
    ga_sb(a,0x17,s.flash);ga_sb(a,0x18,s.field18);
    ga_sb(a,0x15,(uint8_t)r.attributes);
    /* An untranslated program callback stops at its own address. */
    if (res!=NEO_ANIM_MORE && res!=NEO_ANIM_COMPLETE) {ga_fail(e,c.missing?c.missing:0x427a);return -1;}
    return res==NEO_ANIM_COMPLETE;
}
void ga_palette(neo_gactor_t *a,uint16_t resource) {
    uint8_t slot=ga_b(a,0x1d);
    if (slot>=4u) return; /* original traps */
    /* Native handle = slot+1 (the original allocator's slot numbers are not
     * reproduced); the renderer resolves it through a->palette[]. */
    a->palette[slot]=resource;ga_sb(a,0x19+slot,(uint8_t)(slot+1u));ga_sb(a,0x14,(uint8_t)(slot+1u));ga_sb(a,0x1d,(uint8_t)(slot+1u));
}
void ga_flash_palette(neo_gactor_t *a,uint16_t resource) {
    uint8_t keep=ga_b(a,0x14);ga_palette(a,resource);
    ga_sb(a,0x16,ga_b(a,0x14));ga_sb(a,0x17,0);ga_sb(a,0x14,keep);
}
uint8_t ga_attack(neo_gactor_t *a,neo_genv_t *e,uint32_t row) {
    const uint8_t *r=ga_rom(row);
    if (!r || !e->attack) {ga_fail(e,row);return 0;}
    int16_t w[7];for (unsigned i=0;i<7;i++) w[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    if (w[5]<0) return 0;
    uint8_t best=e->attack(e->ctx,a,w);
    if ((int16_t)(r[14]<<8|r[15])>=0) ga_fail(e,row); /* several (class,damage) pairs: not observed */
    return best;
}
uint8_t ga_attack_targets(neo_gactor_t *a,neo_genv_t *e,uint32_t row) {
    memset(e->target,0,sizeof(e->target));
    e->want_targets=true;uint8_t r=ga_attack(a,e,row);e->want_targets=false;
    return r;
}
bool ga_register(neo_gactor_t *a,neo_genv_t *e,int16_t *dx,int16_t *dy) {
    if (!ga_l(a,0x50)) return false;
    /* 004BAC: while RAM 1067E2 is set every body whose descriptor class is
     * not 1 dies instead of registering. */
    if (e->kill_all) {
        const uint8_t *d=ga_rom(ga_l(a,0x50));
        if (d && d[1]!=1u) {ga_sw(a,0x4e,0);ga_sb(a,0x5f,(uint8_t)(ga_b(a,0x5f)|0x80u));ga_sb(a,0x5a,6);goto hit;}
    }
    if (e->register_box) e->register_box(e->ctx,a);
hit:
    if (!ga_b(a,0x5a)) return false;
    if (dx) *dx=(int16_t)((int8_t)ga_b(a,0x5c)*2);
    if (dy) *dy=(int16_t)((int8_t)ga_b(a,0x5d)*2);
    return true;
}
bool ga_players_in(neo_gactor_t *a,neo_genv_t *e,uint32_t rect) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return true;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    return !neo_target_pick(&c,&e->players,q).valid;
}
bool ga_outside_view(neo_gactor_t *a,neo_genv_t *e,uint32_t rect) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return false;}
    uint32_t d0=(uint32_t)((uint32_t)r[0]<<24|r[1]<<16|r[2]<<8|r[3]),d1=(uint32_t)((uint32_t)r[4]<<24|r[5]<<16|r[6]<<8|r[7]);
    uint8_t f=ga_b(a,0x15);
    uint16_t lo=(uint16_t)d0,hi;
    if (f&2u) lo=(uint16_t)(0u-(uint16_t)(lo+(uint16_t)d1));
    int32_t dy=S32(ga_l(a,0x40)-ga_l(a,0x48));
    if (dy>0) {if ((int16_t)(uint16_t)(lo+ga_w(a,0x40))>(int16_t)e->view[3]) return true;}
    else if (dy<0) {if ((int16_t)(uint16_t)(lo+(uint16_t)d1+ga_w(a,0x40))<(int16_t)e->view[2]) return true;}
    lo=(uint16_t)(d0>>16);hi=(uint16_t)(d1>>16);
    if (f&1u) lo=(uint16_t)(0u-(uint16_t)(lo+hi));
    int32_t dx=S32(ga_l(a,0x3c)-ga_l(a,0x44));
    if (dx>0) {if ((int16_t)(uint16_t)(lo+ga_w(a,0x3c))>(int16_t)e->view[1]) return true;}
    else if (dx<0) {if ((int16_t)(uint16_t)(lo+hi+ga_w(a,0x3c))<(int16_t)e->view[0]) return true;}
    return false;
}
uint16_t ga_rank_word(const neo_gactor_t *a,const neo_genv_t *e,const uint8_t *t) {
    /* 0260C8: [type][rank mask] then rows; rank RAM 10FD8B unless mode 2. */
    unsigned type=t[0],mask_bits=t[1];
    unsigned coop=e->players.active_mask==3u;
    unsigned rank=e->players.mode88==2?e->players.rank:3u;
    unsigned mask=(1u<<((rank&7u)+1u))-1u;
    /* The ROM lookup table (soldier_rank_lookup) is the bit count of the
     * index for all 256 entries. */
    unsigned row=0;for (unsigned b=mask_bits&mask&0xffu;b;b&=b-1u) row++;
    if (!row) return 0;
    unsigned d0=(row-1u)*2u+coop,v=ga_b(a,0x68);
    if (type==2) return t[2+d0*6u+v];
    unsigned o=2+(d0*12u+v*2u);
    return (uint16_t)(t[o]<<8|t[o+1]);
}

bool ga_cull(neo_gactor_t *a,neo_genv_t *e,uint32_t rect) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return false;}
    uint16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(uint16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    return neo_cull_outside(&c,&e->bounds,q);
}
bool ga_cull_rect(neo_gactor_t *a,neo_genv_t *e,uint32_t rect) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return false;}
    uint16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(uint16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    return neo_cull_rectangle_outside(&c,&e->bounds,q);
}
uint16_t ga_rand(neo_genv_t *e) {return neo_rng_next(e->rng);}
uint16_t ga_rand_range(neo_genv_t *e,uint16_t range) {return (uint16_t)(((uint32_t)neo_rng_next(e->rng)*range)>>16);}
int16_t ga_sine(uint8_t angle) {const uint8_t *t=ga_rom(0x29fcu+2u*angle);return t?(int16_t)(t[0]<<8|t[1]):0;}
int16_t ga_cosine(uint8_t angle) {const uint8_t *t=ga_rom(0x29fcu+0x80u+2u*angle);return t?(int16_t)(t[0]<<8|t[1]):0;}
void ga_polar(uint32_t speed,uint8_t angle,uint32_t *vx,uint32_t *vy) {
    /* 002E7C: d0 = speed>>8 (arithmetic); d1 = d0.w*sin, d0 = d0.w*cos (muls.w). */
    int32_t d0=S32(speed)>>8;
    int16_t w=(int16_t)(uint16_t)d0;
    *vy=(uint32_t)((int32_t)w*ga_sine(angle));
    *vx=(uint32_t)((int32_t)w*ga_cosine(angle));
}
void ga_place_at(neo_gactor_t *a,const neo_gactor_t *s,neo_genv_t *e,int16_t dx,int16_t dy) {
    neo_contact_parent_t root;
    if (!ga_parent(e,ROOT_TAG,&root)) return;
    uint32_t ox=(uint32_t)(int32_t)dx<<16,oy=(uint32_t)(int32_t)dy<<16;
    uint32_t d0=ox+ga_l(s,0x3c)-(uint32_t)root.x,d1=oy+ga_l(s,0x40)-(uint32_t)root.y;
    uint32_t d2=ox+ga_l(s,0x44)-(uint32_t)root.previous_x,d3=oy+ga_l(s,0x48)-(uint32_t)root.previous_y;
    ga_sl(a,0x28,d0);ga_sl(a,0x2c,d1);ga_sl(a,0x30,d0-d2);ga_sl(a,0x34,d1-d3);ga_sl(a,0x38,ROOT_TAG);
    ga_project(a,e);
}
neo_gactor_t *ga_spawn_from(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
bool ga_creator_alive(neo_gactor_t *a,neo_genv_t *e) {return !e->creator_alive || e->creator_alive(e->ctx,a);}
/* ROM 040200: the object-list marker (stairs, ledges) the actor crosses this
 * update (scene-root-relative position, 04015C: the nearest by X among the
 * markers whose height band holds Y); -1 for none. *flag: 1 below the
 * marker's top half, 2 above (040274). */
int ga_marker_cross(neo_gactor_t *a,neo_genv_t *e,uint8_t *flag) {
    neo_contact_parent_t root,p;
    if (!ga_parent(e,ROOT_TAG,&root)) return -1;
    uint32_t tag=ga_l(a,0x38);bool has=tag && ga_parent(e,tag,&p);
    if (tag && !has) return -1;
    uint32_t x=ga_l(a,0x28),y=ga_l(a,0x2c);
    if (has) {x+=(uint32_t)p.x;y+=(uint32_t)p.y;}
    x-=(uint32_t)root.x;y-=(uint32_t)root.y;
    uint32_t best=0xffffffffu;uint32_t at=0;const neo_marker_t *m=NULL;
    for (unsigned i=0;i<e->marker_count;i++) {
        const neo_marker_t *k=&e->markers[i];
        uint32_t dy=y-(uint32_t)k->y;
        if (dy>=((uint32_t)k->height<<16)) continue;
        uint32_t px=(uint32_t)k->x-(dy>>3);
        uint32_t d=x-px;if ((int32_t)d<0) d=0u-d;
        if (best>d) {best=d;at=px;m=k;}
    }
    if ((int32_t)best<0 || !m) return -1;
    uint32_t d7=ga_l(a,0x30);
    if (has) d7+=(uint32_t)p.x-(uint32_t)p.previous_x;
    d7=d7-(uint32_t)root.x+(uint32_t)root.previous_x+x;
    if ((int32_t)((x-at)^(d7-at))>=0) return -1;
    uint32_t d1=(y-(uint32_t)m->y)*2u;
    int8_t r=(int8_t)(uint8_t)((uint8_t)(d1>>16)-m->height);
    *flag=(uint8_t)((r<0?0xffu:0u)+2u);
    return m->id;
}
/* ROM 0402E6: follow marker `id` (the depth word 01AD8C, re-parent to the
 * scene root 01AC4C, X along the marker's slope, 0143E4); the height left to
 * climb (negative: none, or above). */
int16_t ga_marker_follow(neo_gactor_t *a,neo_genv_t *e,uint8_t id) {
    if ((ga_w(a,0x66)&0xf000u)==0x8000u) ga_sw(a,0x1e,(uint16_t)(ga_w(a,0x66)-ga_w(a,0x40)));
    const neo_marker_t *m=NULL;
    for (unsigned i=0;i<e->marker_count && !m;i++) if (e->markers[i].id==id) m=&e->markers[i];
    if (!m) {ga_stationary(a,e);return (int16_t)0x8000;}
    neo_contact_parent_t root,op;
    if (!ga_parent(e,ROOT_TAG,&root)) return (int16_t)0x8000;
    uint32_t tag=ga_l(a,0x38);
    uint32_t d0=ga_l(a,0x28),d1=ga_l(a,0x2c),d2=ga_l(a,0x30),d3=ga_l(a,0x34);
    if (tag!=ROOT_TAG) {
        bool has=tag && ga_parent(e,tag,&op);
        d2+=d0;d3+=d1;
        if (has) {d0+=(uint32_t)op.previous_x;d1+=(uint32_t)op.previous_y;d2+=(uint32_t)op.x;d3+=(uint32_t)op.y;}
        d0-=(uint32_t)root.previous_x;d1-=(uint32_t)root.previous_y;d2-=(uint32_t)root.x;d3-=(uint32_t)root.y;
        d2-=d0;d3-=d1;
    }
    uint32_t d4=(uint32_t)((int32_t)(d1+d3-(uint32_t)m->y)>>3);
    d2=(uint32_t)m->x-d0-d4;
    ga_sl(a,0x28,d0);ga_sl(a,0x2c,d1);ga_sl(a,0x30,d2);ga_sl(a,0x34,d3);ga_sl(a,0x38,ROOT_TAG);
    ga_air(a,e);
    uint32_t h=(uint32_t)m->height<<16;
    uint32_t r=h+(uint32_t)m->y+(uint32_t)root.y-ga_l(a,0x40);
    if ((int32_t)r>(int32_t)h) r=0xffffffffu;
    return (int16_t)(uint16_t)(r>>16);
}
/* ROM 014A02: kernel sweep (013968, 10C004 = 0, 10C008 = FF, 10C009 = 0)
 * from the actor's block by (dx,dy) pixels; the end point and its object
 * become `child`'s +28/+2C/+38 (not projected). */
void ga_sweep_place(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *child,int16_t dx,int16_t dy) {
    (void)ga_sweep_place_d0(a,e,child,dx,dy,0);
}
uint16_t ga_sweep_place_d0(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *child,int16_t dx,int16_t dy,uint8_t d0) {
    uint32_t tag=ga_l(a,0x38);uint8_t parent=kobj(tag);
    int32_t x=(int32_t)ga_l(a,0x28),y=(int32_t)ga_l(a,0x2c),ddx=(int32_t)((uint32_t)(int32_t)dx<<16),ddy=(int32_t)((uint32_t)(int32_t)dy<<16);
    if (parent==0xff) {
        /* An actor's position block (the turret on its vehicle): 01AC4C
         * converts to absolute coordinates, as in sweep(). */
        neo_contact_parent_t p;
        if ((tag&0xff000000u)!=NEO_GA_TAG_BASE || !ga_parent(e,tag,&p)) {ga_fail(e,0x14a02);return 0;}
        int32_t ex=(int32_t)((uint32_t)x+(uint32_t)ddx+(uint32_t)p.x),ey=(int32_t)((uint32_t)y+(uint32_t)ddy+(uint32_t)p.y);
        x=(int32_t)((uint32_t)x+(uint32_t)p.previous_x);y=(int32_t)((uint32_t)y+(uint32_t)p.previous_y);
        ddx=(int32_t)((uint32_t)ex-(uint32_t)x);ddy=(int32_t)((uint32_t)ey-(uint32_t)y);
        parent=NEO_KOBJ_NONE;
    }
    self(a,e);
    neo_kreq_t q={x,y,ddx,ddy,parent,0,(uint8_t)(d0?0:0xff),0,0,0,0};
    neo_kres_t k;
    if (!neo_kernel_sweep(e->kernel,&q,&k)) {ga_fail(e,0x13968);return 0;}
    ga_sl(child,0x28,(uint32_t)k.x);ga_sl(child,0x2c,(uint32_t)k.y);ga_sl(child,0x38,kobj(tag)==k.parent?tag:ktag(k.parent));
    return (uint16_t)(k.flags&0x15u);                               /* 014A48: D6 & 15 */
}
