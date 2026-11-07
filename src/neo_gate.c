#include "neo_region.h"
/* Mission 2 stage 1: the gate 03882A/03889C and the water ripple 03C62A,
 * manual translation (ROM labels as provenance).
 * - 03882A (script, +6E = 1 at x 558): ripple 03C52C (+6E = 2), layer
 *   8800, palettes 0x3CA/0x3D8, floor 0146E8, health 2D39C0 (0260C8),
 *   hurtbox 038A26, program 23C55A, flash palette 0x3CB.
 * - 03889C: 01436A (+15 bit 6) or 0143DC, 00427A, the dynamic collision
 *   object 038AD8 (0140C0, +70 = 0); left of x 0x148 the cull 038A52
 *   (019504: outside -> 004188 after the cartridge check, not reproduced)
 *   and 004C24 (carry: +5F bit 7 -> 038994; +5A = 0).
 * - 038994: sound 1021, row 038A32, 013F1E, debris 038A62/038A74/038A86
 *   (01DA40), then by +6E (table 038A5A): 0 -> 0389E6; 1 -> 038A0C: the
 *   white screen 0AE1DC (0AE220: +6E = 1, +70 = 0, after the script actor),
 *   row 038A42 and 0B6E06 (map patches 0B6E4A/0B6E74 through 0C70FC: the
 *   flag gate_patched; marker 0B6F6A by 0B6F40; the exit 0B6ED2).
 *   0389E6: +76 = 8, then 0389F2 every update: 0049E8 with +72 nine times,
 *   004188.
 * - 03C62A (03C520/03C526/03C52C/03C532 with +6E = 0..3, when 019240 has
 *   a palette for the mission RAM 10C2F6): layer 1001, 012022, palette by
 *   mission, +15 bit 1 clear. 03C656: water (class 5, 013CF8) under the
 *   creator 2 px up, from its +38; found: +38/+28/+2C there (+2C | 7FFFF,
 *   and 8 px lower when also water), program 03C67E[+6E], +12 = FF,
 *   03C77E. 03C6EA follows the creator (+70 = water under it, +28),
 *   01ABC8, +12 by the height over the creator, +1E. 03C7E6 (03C7C6 /
 *   03C7D6 when the creator moves), 03C820 (+26 random), 03C85E/03C87E
 *   (03C84E) and back to 03C656. Tail 03C898: +15 bit 0 of the creator,
 *   012022, deleted with the creator (00409A; cartridge check not
 *   reproduced). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x12022u) return false;
    bool front=e->front_layer && e->front_layer(e->ctx);
    SB(0x15,front?(B(0x15)|4u):(B(0x15)&0xfbu));
    return true;
}
static void front(neo_gactor_t *a,neo_genv_t *e) {call(a,e,NEO_ANIM_CALL,0x12022u,0);}  /* 012022 */
/* 019240: the ripple palette of mission RAM 10C2F6 (carry clear: none). */
static uint16_t ripple_palette(const neo_genv_t *e) {
    switch (e->pickup?e->pickup->mission:0u) {
    case 1: return 0x14b;case 4: return 0x14c;
    default: return 0;   /* 01937A: missions 0, 2, 3 */
    }
}
static const uint32_t ripple_idle[4]={0x23f868,0x23f8d0,0x23f960,0x23fa2c};   /* 03C67E */
static const uint32_t ripple_still[4]={0x23f746,0x23f774,0x23f7a2,0x23f7d0};  /* 03C7C6 */
static const uint32_t ripple_moving[4]={0x23f746,0x23f774,0x23f7a2,0x23f7fe}; /* 03C7D6 */
static const uint32_t ripple_settle[4]={0x23f868,0x23f892,0x23f90e,0x23f9b2}; /* 03C84E */
bool neo_gate_ripple(neo_gactor_t *a,neo_genv_t *e,uint16_t kind) {
    if (e->pickup && e->pickup->mission>4u) {ga_fail(e,0x19240);return false;}   /* 013D0A */
    if (!ripple_palette(e)) return true;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x3c62a,false):NULL;  /* 0040BC */
    if (c) ga_sw(c,0x6e,kind);
    return true;
}
/* 013CF8, class 5 under (x,y) of the object `tag`. */
static bool water(neo_genv_t *e,uint32_t tag,int32_t x,int32_t y,uint32_t *otag,int32_t *ox,int32_t *oy) {
    uint8_t p=ga_kobj(tag),o=0;
    if (p==0xffu) return false;
    if (!neo_kernel_point_class_in(e->kernel,p,x,y,5,0,ox,oy,&o)) return false;
    *otag=ga_ktag(o);return true;
}
static int32_t object_x(neo_genv_t *e,uint32_t tag) {
    if (!tag) return 0;
    neo_contact_parent_t p;if (!ga_parent(e,tag,&p)) {ga_fail(e,0x3c71c);return 0;}
    return p.x;
}
static bool alive(const neo_gactor_t *c) {return c && ga_l(c,0)!=0x3f1eu;}   /* 0040AC */
/* 03C6EA */
static void follow(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *c) {
    if (alive(c)) {
        uint32_t t;int32_t x,y;
        bool w=water(e,ga_l(c,0x38),(int32_t)ga_l(c,0x28),(int32_t)(ga_l(c,0x2c)-0x20000u),&t,&x,&y);
        SB(0x70,w?0xff:0);
        if (w) SL(0x28,(uint32_t)x+(uint32_t)object_x(e,t)-(uint32_t)object_x(e,L(0x38)));
    }
    ga_project(a,e);
    uint8_t d1=0xff;
    if (c) {
        uint32_t d0=L(0x40)-ga_l(c,0x40)+0x20000u;
        if (d0<0xa0000u && ga_b(c,0x6d)!=0x40u) d1=(uint8_t)((d0-0xa0000u)<<4>>16);
        SW(0x1e,ga_w(c,0x1e));
    }
    SB(0x12,d1);
}
static ga_result_t ripple_tail(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *c) {   /* 03C898 */
    if (c) SB(0x15,(B(0x15)&0xfeu)|(ga_b(c,0x15)&1u));
    front(a,e);
    return ga_creator_alive(a,e)?GA_MORE:GA_DELETE;
}
static ga_result_t ripple_probe(neo_gactor_t *a,neo_genv_t *e) {   /* 03C656 */
    neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;
    if (!c) {ga_fail(e,0x3c656);return GA_UNSUPPORTED;}
    uint32_t t;int32_t x,y;
    if (water(e,ga_l(c,0x38),(int32_t)ga_l(c,0x28),(int32_t)(ga_l(c,0x2c)-0x20000u),&t,&x,&y)) {
        SL(0x38,t);SL(0x28,(uint32_t)x);y=(int32_t)((uint32_t)y|0x7ffffu);SL(0x2c,(uint32_t)y);
        uint32_t t2;int32_t x2,y2;
        if (water(e,t,x,(int32_t)((uint32_t)y+0x80000u),&t2,&x2,&y2)) {SL(0x38,t2);SL(0x28,(uint32_t)x2);SL(0x2c,(uint32_t)y2);}
        START(ripple_idle[W(0x6e)&3u]);
        SB(0x12,0xff);SL(0,0x3c77e);
    }
    if (e->unsupported) return GA_UNSUPPORTED;
    return ripple_tail(a,e,c);
}
static ga_result_t ripple_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;
    int r;
    switch (L(0)) {
    case 0x3c62a: {
        SW(0x66,0x1001);SW(0x1e,0x1001);front(a,e);
        ga_palette(a,ripple_palette(e));SB(0x15,B(0x15)&0xfdu);SL(0,0x3c656);
    }   /* fall through */
    case 0x3c656: return ripple_probe(a,e);
    case 0x3c77e:
        follow(a,e,c);
        if ((r=ga_anim_step(a,e,call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,B(0x70)?0x3c7e6:0x3c656);
        if (c) SB(0x15,(B(0x15)&0xfeu)|(ga_b(c,0x15)&1u));
        front(a,e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x3c7e6: {
        const uint32_t *t=ripple_still;
        if (alive(c) && ga_l(c,0x30)) t=ripple_moving;
        START(t[W(0x6e)&3u]);SL(0,0x3c820);
    }   /* fall through */
    case 0x3c820:
        follow(a,e,c);
        if (ga_anim_step(a,e,call)<0) return GA_UNSUPPORTED;
        SW(0x26,(ga_rand(e)&0x7fu)+0x40u);
        if (!B(0x70)) SL(0,0x3c85e);
        if (e->unsupported) return GA_UNSUPPORTED;
        return ripple_tail(a,e,c);
    case 0x3c85e:
        START(ripple_settle[W(0x6e)&3u]);SL(0,0x3c87e);
        /* fall through */
    case 0x3c87e:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x3c656);
        return ripple_probe(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 01DA40: debris emitter 01D764 with the list `list`, +66/+1E = 8800. */
static void debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=ga_spawn_from(a,e,0x1d764,0,0);
    if (!c) return;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);ga_sw(c,0x66,0x8800);ga_sw(c,0x1e,0x8800);
}
/* 0B6E06: map patches (0B6E4A, 0B6E74), marker 0B6F40, exit 0B6ED2. */
static void patch(neo_genv_t *e,uint32_t layer,uint32_t submap,int16_t d2,int16_t d3,int16_t d4,int16_t d5) {
    const int16_t d[6]={0,0,d2,d3,d4,d5};
    if (e->map_patch) e->map_patch(e->ctx,layer,submap,d);
}
static void stage_exit(neo_gactor_t *a,neo_genv_t *e) {
    patch(e,0x10c72e,0xb6bec,7,7,0x21,0xa);                            /* 0B6E4A */
    patch(e,0x10c72e,0xb6c02,7,4,0x21,3);patch(e,0x10e83e,0xb6c18,7,4,0x21,3);   /* 0B6E74 */
    if (e->flags) e->flags->gate_patched=1;
    neo_gactor_t *m=e->spawn?e->spawn(e->ctx,a,0xb6f6a,false):NULL;
    if (m) {ga_sl(m,0x28,0x2600000);ga_sl(m,0x2c,0x800000);ga_sl(m,0x38,0x10e968);}
    neo_gactor_t *x=e->spawn?e->spawn(e->ctx,a,0xb6ed2,false):NULL;
    if (x) {ga_sl(x,0x38,0x10e968);ga_sl(x,0x28,0x2800000);ga_sl(x,0x2c,0x380000);ga_sl(x,0x6e,0xb6eca);ga_sw(x,0x72,0x2a0);}
}
static ga_result_t gate_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x3882a: {
        if (!neo_gate_ripple(a,e,2)) return GA_UNSUPPORTED;           /* 03C52C */
        SW(0x66,0x8800);SW(0x1e,0x8800);
        ga_palette(a,0x3ca);ga_palette(a,0x3d8);
        SB(0x15,B(0x15)&0xfeu);ga_floor(a,e);
        const uint8_t *t=ga_rom(0x2d39c0);
        if (!t) {ga_fail(e,0x2d39c0);return GA_UNSUPPORTED;}
        SW(0x4e,ga_rank_word(a,e,t));SL(0x50,0x38a26);
        START(0x23c55a);ga_flash_palette(a,0x3cb);SL(0,0x3889c);
    }   /* fall through */
    case 0x3889c:
        if (B(0x15)&0x40u) ga_stationary(a,e);
        else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}             /* 0143DC */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (!W(0x70) && e->dyn_set) e->dyn_set(e->ctx,a,0x38ad8,0,0);
        if (e->unsupported) return GA_UNSUPPORTED;
        if ((int16_t)(W(0x3c)-0x148u)>=0) return GA_MORE;
        if (ga_cull_rect(a,e,0x38a52)) return GA_DELETE;
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5f)&0x80u) SL(0,0x38994);
            SB(0x5a,0);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x38994:
        if (e->sound) e->sound(e->ctx,0x1021);
        SL(0x72,0x38a32);
        if (!W(0x70) && e->dyn_release) e->dyn_release(e->ctx,a);
        debris(a,e,0x38a62);debris(a,e,0x38a74);debris(a,e,0x38a86);
        if (W(0x6e)==1u) {
            neo_gactor_t *f=e->spawn?e->spawn(e->ctx,a,0xae1dc,true):NULL;   /* 0AE220 */
            if (f) {ga_sb(f,0x70,0);ga_sw(f,0x6e,1);}
            SL(0x72,0x38a42);stage_exit(a,e);
        } else if (W(0x6e)) {ga_fail(e,0x389e0);return GA_UNSUPPORTED;}
        SW(0x76,8);SL(0,0x389f2);
        /* fall through */
    case 0x389f2:
        ga_attack(a,e,L(0x72));
        if (e->unsupported) return GA_UNSUPPORTED;
        SW(0x76,W(0x76)-1u);
        return (int16_t)W(0x76)<0?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- the splash 03C962 (03C5C6..03C5D8, family m3splash) ---------------
 * 03C55C: water (class 5, 013CF8) at the node's +3C/+40 - 1, + 7 or + 15
 * px; found, 8 px higher while there is water (relative to the object of
 * the cell), then y = (y & ~7FFFF) - 1: the surface. 03C5DE (+6E = 0..3):
 * found, the splash 03C962 after the script actor (00411C, reserve 0x2A)
 * at that point of that object (01AB9C: whole pixels, +4C = 0) with the
 * caller's +6E; carry for the caller. 03C962: 012022, the palette D1 of
 * 019240 (mission 1: 151, 4: 152), program 03C952[+6E], +26 = (4 - +6E)
 * * 4 + 100, no speed; 03C9BA each update: 01ABC8, 03C55C (none: 004188),
 * +28/+2C/+38 the surface, 00427A; at its end the cartridge check (004188
 * on genuine hardware: delete). */
static bool surface(neo_genv_t *e,int32_t x,int32_t y,int32_t *ox,int32_t *oy,uint32_t *otag) {
    int32_t lx=0,ly=0;uint8_t o=0;bool found=false;
    for (unsigned i=0;i<3 && !found;i++)
        found=neo_kernel_point_class(e->kernel,x,(int32_t)((uint32_t)y-0x10000u+0x80000u*i),5,0,&lx,&ly,&o);
    if (!found) return false;
    for (;;) {
        int32_t nx,ny;uint8_t no;
        ly=(int32_t)((uint32_t)ly+0x80000u);
        if (!neo_kernel_point_class_in(e->kernel,o,lx,ly,5,0,&nx,&ny,&no)) break;
        lx=nx;ly=ny;o=no;
    }
    *ox=lx;*oy=(int32_t)(((uint32_t)ly&0xfff80000u)-1u);*otag=ga_ktag(o);
    return true;
}
static uint16_t splash_palette(const neo_genv_t *e) {   /* 019240 D1 */
    switch (e->pickup?e->pickup->mission:0u) {
    case 1: return 0x151;case 4: return 0x152;
    default: return 0;
    }
}
bool neo_gate_splash(neo_gactor_t *a,neo_genv_t *e,uint16_t kind) {
    int32_t x,y;uint32_t t;
    if (!surface(e,(int32_t)L(0x3c),(int32_t)L(0x40),&x,&y,&t)) return false;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x3c962,true):NULL;
    if (c) {
        ga_sl(c,0x28,(uint32_t)x&0xffff0000u);ga_sl(c,0x2c,(uint32_t)y&0xffff0000u);ga_sl(c,0x38,t);ga_sw(c,0x4c,0);
        ga_project(c,e);ga_sw(c,0x6e,kind);
    }
    return true;
}
static const uint32_t splash_program[4]={0x22e064,0x22e0f2,0x22df54,0x22e1f0};   /* 03C952 */
static ga_result_t splash_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x3c962u) {
        front(a,e);
        uint16_t p=splash_palette(e);
        if (!p) {ga_fail(e,0x19240);return GA_UNSUPPORTED;}
        ga_palette(a,p);START(splash_program[W(0x6e)&3u]);
        SW(0x26,(uint16_t)((4u-W(0x6e))*4u+0x100u));SL(0x30,0);SL(0x34,0);SL(0,0x3c9ba);
    }
    ga_project(a,e);
    int32_t x,y;uint32_t t;
    if (!surface(e,(int32_t)L(0x3c),(int32_t)L(0x40),&x,&y,&t)) return GA_DELETE;
    SL(0x28,(uint32_t)x);SL(0x2c,(uint32_t)y);SL(0x38,t);
    int r=ga_anim_step(a,e,NULL);
    if (r<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:GA_MORE;
}
bool neo_gate_routine(uint32_t r) {
    return r==0x3882au || r==0x3889cu || r==0x38994u || r==0x389f2u ||
        r==0x3c62au || r==0x3c656u || r==0x3c77eu || r==0x3c7e6u || r==0x3c820u || r==0x3c85eu || r==0x3c87eu || r==0x3c962u || r==0x3c9bau;
}
ga_result_t neo_gate_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0x3c962u || r==0x3c9bau) return splash_tick(a,e);
    if (r>=0x3c62au) return ripple_tick(a,e);
    return gate_tick(a,e);
}
