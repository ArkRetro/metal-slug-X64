#include "neo_region.h"
/* Mission 2 stage 2: the soldier holding a hostage, manual translation of
 * ROM 06835C and of the hostage part of 021B46..022258 (ROM labels as
 * provenance only).
 * - 06835C (script): +8A = 2D27E2 (0260C8) << 8; the holder 0683FA
 *   (0040BC, neo_rebel3.c) with +69, +8A, +86 = this node, +6E, +70 = +72,
 *   +74, +76, +7A = +78, placed 0x20 px ahead (01ACF2); here +72 = +70,
 *   +74 = FFFF, +62 = +8A, +7E = +69, 019DFC (+69 = FF) and 021B46.
 * - 021B46: layer 8820, palette 0x1F, hurtbox E005E, floor 0146E8, facing
 *   by +6E (+62 negated with it), program 230984, +81 = 2, +4E = 1,
 *   +80 = 0. 021BB2: 01456A, 00427A; +80 = 1 (the holder died, 068F4A)
 *   -> 021E9A; 004C24 (carry: +5A = 0x10 -> 021C0A, not observed; else
 *   +4E = 1; +5F bit 7 and +5A cleared).
 * - 021E9A: program 230654 (01436A, 00427A) then 021ECC: program 23066A,
 *   facing flipped, +62 = +72 << 8 by facing; 021F02 walks (02223C:
 *   01456A on the floor, else 0143DC) until +5F bit 0 (-> 021F22, not
 *   observed).
 * - 0220BC (every state's tail): 01945C with E004E: +60 = FF and 004188
 *   (after the cartridge check, not reproduced). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,n) ((B(o)>>(n))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { r=ga_anim_step(a,e,NULL); if (r<0) return GA_UNSUPPORTED; } while (0)
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
static void walker(neo_gactor_t *a,neo_genv_t *e) {                    /* 02223C */
    if (BIT(0x15,6)) ga_ground(a,e,NULL); else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {               /* 0220BC */
    if (ga_cull(a,e,0xe004e)) {SB(0x60,0xff);return GA_DELETE;}
    return GA_MORE;
}
static ga_result_t hostage(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x6835c: {
        const uint8_t *t=ga_rom(0x2d27e2);
        if (!t) STOP(0x2d27e2);
        SL(0x8a,(uint32_t)ga_rank_word(a,e,t)<<8);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x683fa,false):NULL;
        if (c) {
            ga_sb(c,0x69,B(0x69));ga_sl(c,0x8a,L(0x8a));ga_sl(c,0x86,NEO_GA_TAG((unsigned)a->index));
            ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x70,W(0x72));
        }
        SW(0x72,W(0x70));
        if (c) ga_sw(c,0x74,W(0x74));
        SW(0x74,0xffff);
        if (c) {ga_sw(c,0x76,W(0x76));ga_sw(c,0x7a,W(0x78));ga_place_at(c,a,e,W(0x6e)?-0x20:0x20,0);}
        SL(0x62,L(0x8a));SW(0x7e,B(0x69));SB(0x69,0xff);              /* 019DFC */
        SB(0x1d,0);SL(0x50,0xe005e);SW(0x66,0x8820);SW(0x1e,0x8820);   /* 021B46 */
        ga_palette(a,0x1f);ga_floor(a,e);SB(0x61,0);
        if (!W(0x6e)) {SB(0x15,B(0x15)|1u);SL(0x62,0u-L(0x62));}
        START(0x230984);SB(0x81,2);SW(0x4e,1);SB(0x80,0);SL(0,0x21bb2);
        return GA_MORE;
    }
    case 0x21bb2:
        ga_ground(a,e,NULL);STEP(r);
        if (B(0x80)==1u) SL(0,0x21e9a);
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5a)==0x10u) STOP(0x21c0a);
            SW(0x4e,1);SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);
        }
        return tail(a,e);
    case 0x21e9a:START(0x230654);SL(0,0x21eb0);/* fall through */
    case 0x21eb0:
        ga_stationary(a,e);STEP(r);
        if (r>0) SL(0,0x21ecc);
        return tail(a,e);
    case 0x21ecc: {
        START(0x23066a);SB(0x15,B(0x15)^1u);
        uint32_t v=(uint32_t)(int32_t)(int16_t)W(0x72)<<8;
        SL(0x62,BIT(0x15,0)?v:0u-v);SL(0,0x21f02);
    }   /* fall through */
    case 0x21f02:
        walker(a,e);STEP(r);
        if (BIT(0x5f,0)) STOP(0x21f22);
        return tail(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static const uint32_t STATES[]={0x6835c,0x21bb2,0x21e9a,0x21eb0,0x21ecc,0x21f02};
bool neo_hostage_routine(uint32_t r) {
    for (unsigned i=0;i<sizeof(STATES)/sizeof(STATES[0]);i++) if (STATES[i]==r) return true;
    return false;
}
ga_result_t neo_hostage_tick(neo_gactor_t *a,neo_genv_t *e) {return hostage(a,e);}
