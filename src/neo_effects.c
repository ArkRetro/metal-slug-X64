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
/* ---- the generic fire 08FA4E (the boss turret's gun 0D6168) -------------- */
/* 002EB4: angle (0..0xFF) of the vector (d0, d1) with the tables 002C7C
 * (logarithm) and 002D7C (arctangent); 0x40 when x is 0 after halving. */
static uint8_t k2eb4(neo_genv_t *e,int16_t x,int16_t y) {
    const uint8_t *t=ga_rom(0x2c7c);
    if (!t) {ga_fail(e,0x2c7c);return 0;}
    uint16_t d0=(uint16_t)(x>>1),d1=(uint16_t)(y>>1);
    uint8_t d2=(d0&0x8000u)?0xff:0,d3=(d1&0x8000u)?0xff:0,r;
    d0=(uint16_t)((d0&0xff00u)|(uint8_t)(((uint8_t)d0^d2)-d2));
    d1=(uint16_t)((d1&0xff00u)|(uint8_t)(((uint8_t)d1^d3)-d3));
    d0&=0xffu;
    if (!d0) r=0x40;
    else {
        d1&=0xffu;
        if (!d1) r=0;
        else {
            unsigned v=(unsigned)t[d1]-(unsigned)t[d0];
            if (v>0xffu) r=t[0x100+(v&0xffu)];                                   /* borrow */
            else r=(uint8_t)(0x40u-t[0x100+(uint8_t)~v]);
        }
    }
    d3^=d2;r=(uint8_t)((r^d3)-d3);r=(uint8_t)(r+(d2&0x80u));
    return r;
}
/* 08EFFA / 08F040: the muzzle flash (program 22D264, palette 0x208) at the
 * offset +8B/+8C (-8 in y); its end -> 08EFD6 (0041AC). 08F058 / 08F114:
 * the projectile: velocity, gravity and rise from the rows 08FD5C[+88 & 7]
 * by the direction +8A (x 0x20 back with +94 bit 0), +15 bit 5 for the
 * directions 0 and 8..15, palette 0x373, program 28C32C; each update 0143E4,
 * the ground or +5F bit 0 -> 08F1E0 (sound 1025, 08EFDE), +8A = 002EB4 of
 * the velocity >> 3, gravity, 0427A, attack row 08FC46 (08FC56 with +93 bit
 * 5) -> 08EFDE on a hit, outside 08FC74 (019504) -> 08EFD6. 08EFDE: +66/+1E
 * = E0ED, +50 = 0, the explosion 01C706 in place. Translated from the ROM:
 * no capture holds the turret's fire. */
static ga_result_t fire_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x8effa:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_palette(a,0x208);
        SW(0x28,W(0x28)+(uint16_t)(int16_t)(int8_t)B(0x8b));SW(0x2c,W(0x2c)+(uint16_t)((int16_t)(int8_t)B(0x8c)-8));
        if (!ga_anim_start(a,e,0x22d264,0x100)) return GA_UNSUPPORTED;
        SL(0,0x8f040);
        /* fall through */
    case 0x8f040:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r>0) SL(0,0x8efd6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8efd6: return GA_DELETE_TREE;
    case 0x8f058: {
        uint32_t t=0;const uint8_t *p=ga_rom(0x8fd5cu+(B(0x88)&7u)*4u);
        if (p) t=(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];
        const uint8_t *q=t?ga_rom(t+(B(0x8a)&15u)*6u):NULL;
        if (!q) {ga_fail(e,0x8fd5c);return GA_UNSUPPORTED;}
        int16_t vx=(int16_t)(q[0]<<8|q[1]),g=(int16_t)(q[2]<<8|q[3]),vy=(int16_t)(q[4]<<8|q[5]),d4=0x20;
        if (vx<0) d4=-0x20;
        if (B(0x94)&1u) vx=(int16_t)(vx-d4);
        SL(0x30,(uint32_t)((int32_t)vx*256));SL(0x76,(uint32_t)((int32_t)g*256));SL(0x34,(uint32_t)((int32_t)vy*256));
        uint8_t d=B(0x8a)&15u;
        if (!d || d>=8u) SB(0x15,B(0x15)|0x20u);
        ga_palette(a,0x373);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x15,B(0x15)&~1u);
        if (!ga_anim_start(a,e,0x28c32c,0x100)) return GA_UNSUPPORTED;
        SL(0,0x8f114);
    }   /* fall through */
    case 0x8f114: {
        ga_air(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        if ((B(0x15)&0x40u) || (B(0x5f)&1u)) SL(0,0x8f1e0);
        SB(0x8a,(uint8_t)(k2eb4(e,(int16_t)((int16_t)W(0x30)>>4),(int16_t)((int16_t)W(0x34)>>4))>>3));   /* asr.w #4, 002EB4, lsr.w #3 */
        SL(0x34,L(0x34)+L(0x76));
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,(B(0x93)&0x20u)?0x8fc56u:0x8fc46u)) SL(0,0x8efde);
        if (ga_cull_rect(a,e,0x8fc74)) SL(0,0x8efd6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x8f1e0:
        if (e->sound) e->sound(e->ctx,0x1025);
        /* fall through */
    case 0x8efde:
        SB(0x1d,0);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0x50,0);              /* 009F88 */
        SL(0,0x1c706);
        return neo_effects_tick(a,e);                                   /* jmp 01C706 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool fire_routine(uint32_t r) {return r==0x8effa || r==0x8f040 || r==0x8efd6 || r==0x8f058 || r==0x8f114 || r==0x8f1e0 || r==0x8efde;}
bool neo_effects_routine(uint32_t r) {
    return fire_routine(r) || simple_of(r) || r==0x1c9e6 || r==0x1c97a || r==0x1d764 || r==0x1d7b8 || (r>=0x1d86c && r<=0x1d93c) || r==0x1d944 || r==0x1d958 || r==0x1d9c6;
}
ga_result_t neo_effects_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (fire_routine(r)) return fire_tick(a,e);
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
