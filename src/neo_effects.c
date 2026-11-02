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
static int32_t S32(uint32_t v) {return (int32_t)v;}
/* ---- one-shot effects ------------------------------------------------------- */
typedef struct { uint32_t routine,program,rect; uint16_t palette; } fx_t;
/* Constructors reached in the region: program (004256), cull rectangle (+82,
 * 019504), palette 0x204 (01C814) and the random flip of 01DEA2. */
static const fx_t simple[]={
    {0x1c6ee,0x22cdd4,0x1dec0,0x204},{0x1c706,0x22ce6c,0x1deb8,0x204},{0x1c71e,0x22cf2c,0x1dec8,0x204},
    {0x1c736,0x22cf2c,0x1dec8,0x204},{0x1c74e,0x22d264,0x1dec0,0x204},{0x1c766,0x22d27a,0x1dec0,0x204},
    {0x1c77e,0x22d290,0x1dec8,0x204},{0x1c796,0x22e48a,0x1def0,0x204},
    /* 01C7AE: palette 0x26C, then 01C7F8 (the treasure 0995EC, neo_m2traps.c). */
    {0x1c7ae,0x22cf2c,0x1dec8,0x26c},
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
/* ---- explosion fragments 01D31C.. and debris 01CE08.. --------------------- */
/* 01D31C/01D37C/01D3D2/01D428/01D488/01D4DA: a fragment (program and
 * palette by a random bit or the table 01E008), then the launch at a random
 * angle 01D5EC, the flight 01D6E4 (0143E4 with the gravity +7E; each landing
 * halves the speeds and counts +92 down to the end 01D93C; +5F bit 3 once it
 * bounced) until 01D93C or outside 01DEF0 (019504). 01D5AA (no program of
 * its own) is not reached in Mission 1 and is not translated. */
typedef struct { uint32_t routine; uint8_t clear62,tail; uint32_t prog0,prog1; uint16_t pal0,pal1; } frag_t;
static const frag_t frags[]={
    {0x1d31c,0,0,0x22daee,0x22dc2c,0x277,0x278},{0x1d37c,1,1,0x22daee,0x22dc2c,0x277,0x278},
    {0x1d3d2,1,2,0x22daee,0x22dc2c,0x277,0x278},{0x1d428,0,0,0x22d5bc,0x22d626,0x20a,0x20a},
    {0x1d488,0,0,0x22d8ea,0x22d940,0x29e,0x29e},{0x1d4da,0,0,0,0,0xf7,0xf7},
};
static const frag_t *frag_of(uint32_t r) {
    for (unsigned i=0;i<sizeof(frags)/sizeof(frags[0]);i++) if (frags[i].routine==r) return &frags[i];
    return NULL;
}
static uint32_t rom_l(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]:0;}
static ga_result_t frag_tick(neo_gactor_t *a,neo_genv_t *e) {
    const frag_t *f=frag_of(L(0));
    if (f) {
        uint16_t d0;
        if (f->clear62) {SL(0x62,0);d0=ga_rand(e);}
        else {d0=ga_rand(e);SW(0x62,(uint16_t)((d0&0x700u)+0x100u));}
        uint32_t prog;uint16_t pal;
        if (f->routine==0x1d4da) {prog=rom_l(0x1e008u+4u*(d0&7u));pal=f->pal0;}
        else if (!(d0&2u)) {prog=f->prog0;pal=f->pal0;} else {prog=f->prog1;pal=f->pal1;}
        if (!prog || !ga_anim_start(a,e,prog,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,pal);
        /* 01D51A / 01D54A / 01D578 */
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SB(0x92,1);SB(0x93,1);
        d0=ga_rand(e);
        if (f->tail==0) SW(0x8a,(uint16_t)((d0&0x3eu)+0x20u));
        else if (f->tail==1) SW(0x8a,(uint16_t)((d0&0x3eu)+0x50u));
        else SW(0x8a,(uint16_t)(((d0&0x3eu)-0x10u)&0xffu));
        /* 01D5EC */
        if (S16(W(0x62))<=0) SW(0x62,0x200);
        uint16_t d3=ga_rand(e);
        uint8_t ang=(uint8_t)W(0x8a);
        int16_t d1=ga_sine(ang),d2=ga_cosine(ang);
        d3=(uint16_t)((d3&7u)+1u);
        int16_t m=(int16_t)(uint16_t)((int32_t)d1*(int16_t)d3);     /* muls.w, low word */
        m=(int16_t)(m>>2);                                           /* asr.w #2 */
        int32_t vy=(int32_t)m*S16(W(0x62)),vx=(int32_t)d2*S16(W(0x62));
        SL(0x34,(uint32_t)vy);SL(0x30,(uint32_t)vx);
        d0=ga_rand(e);
        if ((int8_t)(uint8_t)d0<=0) {
            int16_t k=(int16_t)((ga_rand(e)&3u)+2u);
            uint32_t d1l=(uint32_t)(S32(L(0x30))>>8);                       /* move.l, asr.l #8 */
            int16_t v=(int16_t)(uint16_t)d1l;
            int16_t q=(int16_t)(uint16_t)((int32_t)v*k);SW(0x30,(uint16_t)(int16_t)(q>>1));
            k=(int16_t)((ga_rand(e)&3u)+2u);
            /* move.w $34(a6),d1 keeps the upper word of D1, asr.l #8 */
            d1l=(uint32_t)(S32((d1l&0xffff0000u)|W(0x34))>>8);
            v=(int16_t)(uint16_t)d1l;
            q=(int16_t)(uint16_t)((int32_t)v*k);SW(0x34,(uint16_t)(int16_t)(q>>1));
        }
        int32_t c=S32(L(0x34));if (c>0x100000) c=0x100000;if (c<0) c=0;SL(0x34,(uint32_t)c);
        c=S32(L(0x30));if (c>0x80000) c=0x80000;if (c<-0x80000) c=-0x80000;SL(0x30,(uint32_t)c);
        SL(0x7a,L(0x34));SL(0x7e,0xffffb000u);SB(0x15,B(0x15)|0x20u);
        SL(0,0x1d6e4);
    }
    /* 01D6E4 */
    ga_air(a,e);
    if (B(0x15)&0x80u) {
        SL(0x30,(uint32_t)(S32(L(0x30))>>2));
        uint32_t d0=(uint32_t)(S32(L(0x7a))>>1);SL(0x34,d0);SL(0x7a,d0);
        SB(0x92,B(0x92)-1u);
        if ((int8_t)B(0x92)<=0) SL(0,0x1d93c);
    } else SL(0x34,L(0x34)+L(0x7e));
    if (B(0x93)!=B(0x92)) SB(0x5f,B(0x5f)|8u);
    if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;            /* 01B1EC: carry clear */
    if (ga_cull_rect(a,e,0x1def0)) SL(0,0x1d93c);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 01CE08..01D078: a debris piece, (d1 = +91 index, d0 = palette and its
 * base: 01D084 +0, 01D09C 0x106, 01D0B4 0x879, 01D0CC +0, 01D0E4 +0x200);
 * 01D0F8: program 01DF20 and cull rectangle 01DF94 by +91, random flip
 * 01DEA2, random launch, then 01D1B0: 0143E4 with the gravity +7E until it
 * lands (01B1DC, 004188), its program ends (004188) or it leaves +82
 * (019504). */
static bool debris_entry(uint32_t r,uint8_t *d1,uint16_t *pal) {
    static const struct { uint16_t lo,hi; uint16_t d0; uint8_t d1,base; } runs[]={
        /* routine range (low 16 bits), d0, first d1, base: 0 = d0+0x200, 1 = d0, 2 = 0x106, 3 = 0x879 */
        {0xce08,0xce20,0x00a,0x11,0},{0xce2c,0xce44,0x00b,0x14,0},{0xce50,0xce5c,0x168,0x15,1},
        {0xce68,0xce98,0x167,0x0f,1},{0xcea4,0xcf04,0x09e,0x06,0},{0xcf10,0xcf34,0x0f7,0x0b,1},
        {0xcf94,0xcfd0,0x106,0x17,2},{0xcfdc,0xd000,0x167,0x0b,3},{0xd00c,0xd030,0x144,0x0b,1},{0xd03c,0xd078,0x144,0x00,1},
    };
    if ((r>>16)!=1u) return false;
    uint16_t w=(uint16_t)r;
    for (unsigned i=0;i<sizeof(runs)/sizeof(runs[0]);i++) if (w>=runs[i].lo && w<=runs[i].hi && !((w-runs[i].lo)%12u)) {
        *d1=(uint8_t)(runs[i].d1+(w-runs[i].lo)/12u);
        *pal=runs[i].base==0?(uint16_t)(runs[i].d0+0x200u):runs[i].base==1?runs[i].d0:runs[i].base==2?0x106:0x879;
        return true;
    }
    /* 01CF40..01CF88: one entry each (01D0E4: +0x200). */
    static const struct { uint16_t w,d0; uint8_t d1; } one[]={
        {0xcf40,0x88,0x10},{0xcf4c,0x77,0x11},{0xcf58,0x77,0x13},{0xcf64,0x28,0x10},{0xcf70,0x28,0x13},{0xcf7c,0x77,0x13},{0xcf88,0x78,0x15},
    };
    for (unsigned i=0;i<sizeof(one)/sizeof(one[0]);i++) if (w==one[i].w) {*d1=one[i].d1;*pal=(uint16_t)(one[i].d0+0x200u);return true;}
    return false;
}
static ga_result_t debris_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d1;uint16_t pal;
    if (debris_entry(L(0),&d1,&pal)) {
        SB(0x91,(uint8_t)(d1<<2));ga_palette(a,pal);
        uint16_t i=(uint16_t)(B(0x91)&0xfcu);
        SL(0x82,rom_l(0x1df94u+i));
        if (!ga_anim_start(a,e,rom_l(0x1df20u+i),0x100)) return GA_UNSUPPORTED;
        if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);                     /* 01DEA2 */
        SL(0x34,0x80000);SL(0x7e,0xffffbbbcu);SL(0x30,0x20000);
        uint16_t r=ga_rand(e);uint32_t d0=(uint32_t)(r&7u)<<15;
        SL(0x30,(r&1u)?d0:0u-d0);
        r=ga_rand(e);SL(0x34,L(0x34)-((uint32_t)(r&15u)<<15));
        if (S16(W(0x34))>0xfb) SW(0x34,0xfb);
        if (S32(L(0x7e))>-0x1000) SL(0x7e,0xfffff000u);
        SB(0x15,B(0x15)|0x20u);SL(0x50,0);SB(0x12,0xff);SB(0x13,0xff);
        SL(0,0x1d1b0);
    }
    /* 01D1B0 */
    ga_air(a,e);
    if (B(0x15)&0x40u) {SL(0x30,0);SL(0x34,0);SL(0x62,0);return GA_DELETE;}   /* 01B1DC, 004188 */
    SL(0x34,L(0x34)+L(0x7e));
    int st=ga_anim_step(a,e,NULL);                                  /* 01B1EC: carry clear */
    if (st<0) return GA_UNSUPPORTED;
    if (st) return GA_DELETE;
    if (ga_cull_rect(a,e,L(0x82))) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 01E058: program 22EA20 (palette 0x143) in place until its end (01E07C). */
static ga_result_t flash_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x1e058) {
        if (!ga_anim_start(a,e,0x22ea20,0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x143);SL(0,0x1e07c);
    }
    ga_project(a,e);
    int st=ga_anim_step(a,e,NULL);
    if (st<0) return GA_UNSUPPORTED;
    return st?GA_DELETE:GA_MORE;
}
static bool frag_routine(uint32_t r) {return frag_of(r) || r==0x1d6e4;}
static bool debris_routine(uint32_t r) {uint8_t d1;uint16_t pal;return debris_entry(r,&d1,&pal) || r==0x1d1b0;}
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
    return fire_routine(r) || simple_of(r) || r==0x1c9e6 || r==0x1c97a || r==0x1d764 || r==0x1d7b8 || (r>=0x1d86c && r<=0x1d93c) || r==0x1d944 || r==0x1d958 || r==0x1d9c6 || frag_routine(r) || debris_routine(r) || r==0x1e058 || r==0x1e07c;
}
ga_result_t neo_effects_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (fire_routine(r)) return fire_tick(a,e);
    if (simple_of(r) || r==0x1c9e6 || r==0x1c97a) return simple_tick(a,e);
    if (r==0x1d764 || r==0x1d7b8) return smoke_tick(a,e);
    if (r==0x1d944 || r==0x1d958 || r==0x1d9c6) return particle_tick(a,e);
    if (frag_routine(r)) return frag_tick(a,e);
    if (debris_routine(r)) return debris_tick(a,e);
    if (r==0x1e058 || r==0x1e07c) return flash_tick(a,e);
    if (r==0x1d93c) {
        /* 0041AC from a particle or the emitter: delete with the following
         * deeper nodes. */
        return GA_DELETE_TREE;
    }
    return exhaust_tick(a,e);
}
