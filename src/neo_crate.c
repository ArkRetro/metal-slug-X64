#include "neo_region.h"
/* Supply crate 08C154/08C15E/08C168/08C172 (+88 = 3/2/1/0), Mission 2
 * stage 3; manual translation (ROM labels as provenance).
 * - 08C17C: program E57FA[+88], palette E55B6[+88], +4E = 10, layer 8820,
 *   hurtbox E5634, floor 0146E8 when +88 < 2, dynamic object E5640
 *   (0140C0), +62 = 0.
 * - 08C1F2: 004C24, +5A = 0; +5F bit 7 -> 08C24E. Otherwise 01436A and the
 *   depth 01AD8C (+88 >= 2) or the walker 08CCD4, 00427A, 0140C0 again, and
 *   004188 outside the view rectangle E562C (0C8BEC).
 * - 08C24E: 013F1E, program E580A[+88], 019DFC (+69 = FF) for +88 = 1/3;
 *   +88 0/1: 08C2B2 (08CCD4, 00427A; at the end 004188 after the cartridge
 *   check 08C2C0, not reproduced); +88 2/3: 08C356 (01AD8C, 01436A,
 *   00427A; at the end 004188). The broken program E580A[1] calls 2869A4:
 *   the item 08C374 after the script actor. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static uint32_t rl(uint32_t at) {const uint8_t *p=ga_rom(at);return p?((uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]):0;}
static uint16_t rw(uint32_t at) {const uint8_t *p=ga_rom(at);return p?(uint16_t)(p[0]<<8|p[1]):0;}
/* 01AD8C */
static void depth(neo_gactor_t *a) {
    uint16_t d0=W(0x66);
    if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));
}
/* 08CCD4: ground step (01456A) on the floor, else gravity + air (0143DC). */
static void walker(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&0x40u) {uint32_t kind;ga_ground(a,e,&kind);}
    else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
/* 2869A4 (program callback of the broken crate): the item 08C374 after
 * the script actor (00411C) with the crate's +6E/+70/+72, at its place
 * (01ACF2). */
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x2869a4u) return false;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (!c) return true;
    ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x70,W(0x70));ga_sw(c,0x72,W(0x72));
    ga_place_at(c,a,e,0,0);
    return true;
}
static void dyn(neo_gactor_t *a,neo_genv_t *e) {if (e->dyn_set) e->dyn_set(e->ctx,a,0xe5640,0,0);}
bool neo_crate_routine(uint32_t r) {
    return r==0x8c154u || r==0x8c15eu || r==0x8c168u || r==0x8c172u || r==0x8c1f2u || r==0x8c2b2u || r==0x8c356u;
}
ga_result_t neo_crate_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x8c154: case 0x8c15e: case 0x8c168: case 0x8c172: {
        uint16_t v=(uint16_t)(3u-((L(0)-0x8c154u)/10u));SW(0x88,v);
        if (!ga_rom(0xe57fau+4u*v)) {ga_fail(e,0xe57fa);return GA_UNSUPPORTED;}
        if (!ga_anim_start(a,e,rl(0xe57fau+4u*v),0x100)) return GA_UNSUPPORTED;
        ga_palette(a,rw(0xe55b6u+2u*v));
        SW(0x4e,10);SW(0x66,0x8820);SW(0x1e,0x8820);SL(0x50,0xe5634);
        if (v<2u) ga_floor(a,e);
        dyn(a,e);SL(0x62,0);SL(0,0x8c1f2);
    }   /* fall through */
    case 0x8c1f2:
        ga_register(a,e,NULL,NULL);SB(0x5a,0);
        if (!(B(0x5f)&0x80u)) {
            if (W(0x88)>=2u) {ga_stationary(a,e);depth(a);}
            else walker(a,e);
            if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
            dyn(a,e);
            if (e->unsupported) return GA_UNSUPPORTED;
            return ga_outside_view(a,e,0xe562c)?GA_DELETE:GA_MORE;
        }
        if (e->dyn_release) e->dyn_release(e->ctx,a);
        {
            uint16_t v=W(0x88)&3u;
            if (!ga_anim_start(a,e,rl(0xe580au+4u*v),0x100)) return GA_UNSUPPORTED;
            if (v==1u || v==3u) SB(0x69,0xff);
            if (v>=2u) {SL(0,0x8c356);goto still;}
            SL(0,0x8c2b2);
        }
        /* fall through */
    case 0x8c2b2:
        walker(a,e);
        if ((r=ga_anim_step(a,e,call))<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    case 0x8c356:
    still:
        depth(a);ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,call))<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
