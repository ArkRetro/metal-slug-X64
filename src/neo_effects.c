#include "neo_region.h"
#include "neo_blood.h"
/* Visual effects of the region (ROM 01C6E8..01DF20), manual translation:
 * one-shot effects (01C6EE.. constructors, routine 01C9E6), smoke emitters
 * (01DA40 -> 01D764/01D7B8 with the emission lists) and the vehicle exhaust
 * emitter (01DC8E -> 01D86C..01D93C) with its particles (01D944/01D958 ->
 * 01D9C6). The copy-protection check that ends several of these routines
 * (writes to 2FFFE0, branch to 01C6E8 on a failed check) is not reproduced:
 * on the original board it falls through to the deletion. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static int16_t S16(uint16_t v) {return (int16_t)v;}
/* ---- one-shot effects ------------------------------------------------------- */
typedef struct { uint32_t routine,program,rect; uint16_t palette; } fx_t;
/* Constructors reached in the region: program (004256), cull rectangle (+82,
 * 019504), palette 0x204 (01C814) and the random flip of 01DEA2. */
static const fx_t simple[]={
    {0x1c6ee,0x22cdd4,0x1dec0,0x204},{0x1c706,0x22ce6c,0x1deb8,0x204},{0x1c71e,0x22cf2c,0x1dec8,0x204},
    {0x1c736,0x22cf2c,0x1dec8,0x204},{0x1c74e,0x22d264,0x1dec0,0x204},{0x1c766,0x22d27a,0x1dec0,0x204},
    {0x1c77e,0x22d290,0x1dec8,0x204},{0x1c796,0x22e48a,0x1def0,0x204},
};
static const fx_t *simple_of(uint32_t r) {
    for (unsigned i=0;i<sizeof(simple)/sizeof(simple[0]);i++) if (simple[i].routine==r) return &simple[i];
    return NULL;
}
static ga_result_t simple_tick(neo_gactor_t *a,neo_genv_t *e) {
    const fx_t *f=simple_of(L(0));
    if (L(0)==0x1c97au) {
        /* 01C97A: blood splash of the swordsmen 09D846.. (palette by 01B2BE
         * at 01C996), mirrored, rectangle 01DEF0. */
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        if (!ga_anim_start(a,e,0x22e97e,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,neo_blood_palette(NEO_BLOOD_SPLASH_1C97A,e->blood));
        SB(0x15,B(0x15)^1u);SL(0x82,0x1def0);
        SB(0x15,B(0x15)&~2u);SL(0x50,0);SL(0,0x1c9e6);
    } else if (f) {
        if (!ga_anim_start(a,e,f->program,0x100)) return GA_UNSUPPORTED;
        SL(0x82,f->rect);ga_palette(a,f->palette);
        if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);   /* 01DEA2 */
        SB(0x15,B(0x15)&~2u);SL(0x50,0);SL(0,0x1c9e6);
    }
    /* 01C9E6 */
    ga_project(a,e);
    int r=ga_anim_step(a,e,NULL);
    if (r<0) return GA_UNSUPPORTED;
    if (r) return GA_DELETE;
    if (ga_cull_rect(a,e,L(0x82))) return GA_DELETE;
    return GA_MORE;
}
/* ---- smoke emitter 01D764 --------------------------------------------------- */
/* 01DA7C: one emission from the list at +86. */
static void emit_one(neo_gactor_t *a,neo_genv_t *e) {
    const uint8_t *l=ga_rom(L(0x86));
    if (!l) {ga_fail(e,L(0x86));return;}
    #define LW(o) ((uint16_t)(l[o]<<8|l[(o)+1]))
    uint16_t d0=(uint16_t)(ga_rand_range(e,LW(6))+LW(4));
    if (B(0x15)&1u) d0=(uint16_t)(0u-d0);
    SW(0x30,d0);
    d0=(uint16_t)(ga_rand_range(e,LW(10))+LW(8));
    if (B(0x15)&2u) d0=(uint16_t)(0u-d0);
    SW(0x34,d0);
    uint32_t routine=(uint32_t)l[14]<<24|(uint32_t)l[15]<<16|(uint32_t)l[16]<<8|l[17];
    if (l[13]) {
        const uint8_t *t=ga_rom(routine+(ga_rand(e)&0x3cu));
        if (!t) {ga_fail(e,routine);return;}
        routine=(uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3];
    }
    #undef LW
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (!c) return;
    ga_sw(c,0x1e,W(0x1e));
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_place_at(c,a,e,S16(W(0x30)),S16(W(0x34)));
    neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
    ga_sb(c,0x12,p?ga_b(p,0x12):0xff);ga_sb(c,0x13,p?ga_b(p,0x13):0xff);
}
static ga_result_t smoke_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x1d764) {
        const uint8_t *l=ga_rom(L(0x86));
        if (!l) return GA_UNSUPPORTED;
        SB(0x90,l[1]);SW(0x8e,(uint16_t)(l[2]<<8|l[3]));
        if (l[0]) {neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);} /* 01AD6E */
        ga_project(a,e);
        if (B(0x90)==0xffu) {
            while (W(0x8e)) {emit_one(a,e);SW(0x8e,W(0x8e)-1u);}
            return GA_DELETE;
        }
        SL(0,0x1d7b8);
        /* falls into 01D7B8 */
    }
    ga_project(a,e);
    SB(0x90,B(0x90)-1u);
    if (!B(0x90)) {const uint8_t *l=ga_rom(L(0x86));if (l) SB(0x90,l[1]);emit_one(a,e);}
    SW(0x8e,W(0x8e)-1u);
    return W(0x8e)?GA_MORE:GA_DELETE;
}
/* ---- exhaust emitter 01D86C and its particles ------------------------------- */
/* 01DD90: follow the creator; returns creator +4E - own +4E (signed). */
static int32_t follow(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
    if (!p) return 0;
    SB(0x12,ga_b(p,0x12));SB(0x13,ga_b(p,0x13));
    SB(0x15,(ga_b(p,0x15)&1u)?(B(0x15)|1u):(B(0x15)&~1u));
    SB(0x5f,B(0x5f)&0x7fu);
    if (!B(0x94) && (ga_b(p,0x5f)&0x80u)) SB(0x5f,B(0x5f)|0x80u);
    return (int32_t)S16(ga_w(p,0x4e))-(int32_t)S16(W(0x4e));
}
/* 01DE0E / 01DDE8 */
static void particle(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    neo_gactor_t *c=ga_spawn_from(a,e,routine,0,0);
    if (!c) return;
    ga_sw(c,0x8c,0x1e);
    uint16_t d1=W(0x72),d0=(uint16_t)(ga_rand(e)&d1);
    d0=(uint16_t)(d0-(d1>>1));
    if (B(0x12)!=0xffu) d0=(uint16_t)(S16((uint16_t)((int32_t)S16(d0)*(int32_t)(B(0x12)+1u)))>>8); /* muls.w, asr.w #8 */
    ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+d0));
    d1=W(0x74);d0=(uint16_t)((ga_rand(e)&d1)+8u);
    if (B(0x13)!=0xffu) d0=(uint16_t)(S16((uint16_t)((int32_t)S16(d0)*(int32_t)(B(0x13)+1u)))>>8);
    ga_sw(c,0x2c,(uint16_t)(ga_w(c,0x2c)+d0));
    ga_sb(c,0x12,B(0x12));ga_sb(c,0x13,B(0x13));
}
/* Common tail 01D8B2: dead flag or dead creator end the emitter. */
static ga_result_t exhaust_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x5f)&0x80u) SL(0,0x1d934);
    if (!ga_creator_alive(a,e)) SL(0,0x1d934);
    return GA_MORE;
}
static ga_result_t exhaust_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned guard=0;guard<4;guard++) switch (L(0)) {
    case 0x1d86c: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        ga_place(a,e,S16(W(0x6e)),S16(W(0x70)),p?NEO_GA_TAG(p->index):0); /* 01AD6E */
        SW(0x4e,(p?ga_w(p,0x4e):0)>>1);
        if (!ga_anim_start(a,e,0x22eac2,0x100)) return GA_UNSUPPORTED;
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SL(0,0x1d8a4);
    } /* fall through */
    case 0x1d8a4:
        if (follow(a,e)<=0) SL(0,0x1d8d4);
        return exhaust_tail(a,e);
    case 0x1d8d4:
        SW(0x4e,W(0x4e)>>1);SL(0,0x1d8e4); /* fall through */
    case 0x1d8e4:
        if (follow(a,e)<=0) SL(0,0x1d908);
        SB(0x8c,B(0x8c)+1u);
        if (!(B(0x8c)&7u)) particle(a,e,0x1d944);
        return exhaust_tail(a,e);
    case 0x1d908:
        follow(a,e);
        SB(0x8c,B(0x8c)+1u);
        if (!(B(0x8c)&15u)) particle(a,e,0x1d958);
        else if (!(B(0x8c)&3u)) particle(a,e,0x1d944);
        return exhaust_tail(a,e);
    case 0x1d934:return GA_DELETE;
    case 0x1d93c:return GA_DELETE_TREE;
    default:return GA_UNSUPPORTED;
    }
    return GA_UNSUPPORTED;
}
static ga_result_t particle_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x1d944 || L(0)==0x1d958) {
        if (!ga_anim_start(a,e,L(0)==0x1d944?0x22e7f2:0x22e8ae,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x20d);
        SW(0x66,0x1001);SW(0x1e,0x1001);
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                 /* 01B1DC */
        uint32_t d0=(ga_rand(e)&0xff00u)+0x4000u;
        SL(0x34,d0);SL(0x7e,0u-(d0>>6));
        SW(0x8a,0);
        if (S16(W(0x8c))<=10) SW(0x8c,10);
        SL(0,0x1d9c6);
    }
    if (L(0)==0x1d93c) return GA_DELETE_TREE;
    /* 01D9C6 */
    uint16_t r=ga_rand(e),d0=(uint16_t)(r&0x3ffu);
    if (S16(r)<=0) d0=(uint16_t)(0u-d0);
    SW(0x8a,W(0x8a)+d0);
    int16_t s=ga_sine(B(0x8a));
    uint32_t v=(uint32_t)(int32_t)s;                       /* ext.l */
    v=(v&0xffff0000u)|(uint16_t)(v<<8);                   /* lsl.w #8 */
    v=(v&0xffff0000u)|(uint16_t)(v<<1);                   /* add.w d0,d0 */
    SL(0x30,v);
    ga_air(a,e);
    SL(0x34,L(0x34)+L(0x7e));
    int st=ga_anim_step(a,e,NULL);
    if (st<0) return GA_UNSUPPORTED;
    if (st) SL(0,0x1d93c);
    SW(0x8c,W(0x8c)-1u);
    if (S16(W(0x8c))<=0) SL(0,0x1d93c);
    if (ga_cull_rect(a,e,0x1def8)) SL(0,0x1d93c);
    return GA_MORE;
}
bool neo_effects_routine(uint32_t r) {
    return simple_of(r) || r==0x1c9e6 || r==0x1c97a || r==0x1d764 || r==0x1d7b8 || (r>=0x1d86c && r<=0x1d93c) || r==0x1d944 || r==0x1d958 || r==0x1d9c6;
}
ga_result_t neo_effects_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (simple_of(r) || r==0x1c9e6 || r==0x1c97a) return simple_tick(a,e);
    if (r==0x1d764 || r==0x1d7b8) return smoke_tick(a,e);
    if (r==0x1d944 || r==0x1d958 || r==0x1d9c6) return particle_tick(a,e);
    if (r==0x1d93c) {
        /* 0041AC from a particle or the emitter: delete with the following
         * deeper nodes. */
        return GA_DELETE_TREE;
    }
    return exhaust_tick(a,e);
}
