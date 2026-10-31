#include "neo_region.h"
/* Village porter 03264E (Mission 1 camera 440, script record 2D642E, group
 * 9 not counted), manual translation of ROM 03264E..032938 with the program
 * callbacks of its bank programs (238632..2386EE, 238AF0..238BAA); ROM
 * labels are provenance only.
 *
 * 03264E is the waiting figure (palette 0x92, program 2389CE: it paces by
 * the callbacks 238B20/238B70/238AF0/238B08/238B78, +7E its offset): spawn
 * word +6E plus the camera progress (RAM 10E99E) is the camera at which it
 * sends the porter 0327EC (0040BC) from the screen edge (+70: left, x =
 * -0x40, else 0x180; 01ACF2), then 08CCD4 motion until +80 (the porter
 * arrived: 23867C at 0x28 px) turns it to 0327B0 (program 2386F0, 01436A).
 * The porter (palette 0x114, program 237F36, 2.5 px per update towards it)
 * hands over an item 08C374 kind 0x30 (238632 -> 08CD70: 0040BC after the
 * porter, +70 = FFFF, the item and its creator kept in +72/+76); while the
 * item (or anything deeper) follows it in the list (00408C) it waits, then
 * +83 of both (032840). 3290E clears +80..+89. Both are deleted by the cull
 * 032646 (004188; the cartridge check before it is not reproduced). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static void k3290e(neo_gactor_t *a) {for (unsigned o=0x80;o<=0x89;o++) SB(o,0);}
/* 08CCD4: ground step on the floor, else gravity + air. */
static void k8ccd4(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&0x40u) {uint32_t kind;ga_ground(a,e,&kind);}
    else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    neo_gactor_t *p=NULL;
    switch (routine) {
    /* the waiting figure */
    case 0x238af0: SB(0x88,0);if (!(ga_rand(e)&3u)) SB(0x88,0xff);return true;
    case 0x238b08: SL(0x62,BIT(0x15,0)?0xffff0000u:0x10000u);return true;
    case 0x238b20:
        SB(0x87,0);
        if ((int16_t)W(0x7e)>=3) {if (BIT(0x15,0)) SB(0x87,0xff);}
        else if ((int16_t)W(0x7e)<=-3) {if (!BIT(0x15,0)) SB(0x87,0xff);}
        else if (!(ga_rand(e)&3u)) SB(0x87,0xff);
        return true;
    case 0x238b70: SB(0x15,B(0x15)^1u);return true;
    case 0x238b78: SW(0x7e,(L(0x62)&0x80000000u)?W(0x7e)+1u:W(0x7e)-1u);return true;
    case 0x238b8e: SB(0x87,0);if ((W(0x70)!=0)!=(BIT(0x15,0)!=0)) SB(0x87,0xff);return true;
    /* the porter */
    case 0x238632: {
        uint16_t d5=W(0x70)?1u:0u;
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x8c374,false):NULL;   /* 08CD70 */
        if (c) {
            ga_sb(c,0x8b,0);ga_sw(c,0x70,0xffff);ga_sw(c,0x6e,0x30);ga_sw(c,0x72,d5);ga_sb(c,0x7f,0);
            ga_place_at(c,a,e,0,0);
            SL(0x72,NEO_GA_TAG(c->index));SL(0x76,ga_l(c,0x0c));
        }
        SB(0x82,0xff);return true;
    }
    case 0x238666: p=creator(a,e);if (p) ga_sb(p,0x81,0xff);return true;
    case 0x238670: p=creator(a,e);if (p) SB(0x81,ga_b(p,0x81));return true;
    case 0x23867c: {
        p=creator(a,e);if (!p) return true;
        uint16_t d7=(uint16_t)(ga_w(p,0x3c)-W(0x3c));
        if (d7&0x8000u) d7=(uint16_t)(0u-d7);
        if ((int16_t)d7<0x28) {ga_sb(p,0x80,0xff);SB(0x80,0xff);SL(0x62,0);}
        return true;
    }
    case 0x2386a4: SL(0x62,W(0x70)?0x3c000u:0xfffc8000u);return true;
    case 0x2386c2: p=creator(a,e);if (p) ga_sb(p,0x84,0xff);return true;
    case 0x2386cc: p=creator(a,e);if (p) ga_sb(p,0x84,0);return true;
    case 0x2386d6: SB(0x85,(ga_rand(e)&3u)?0:0xff);return true;
    case 0x2386e6: p=creator(a,e);if (p) ga_sb(p,0x86,0xff);return true;
    }
    return false;
}
#define STEP() do { if (ga_anim_step(a,e,call)<0) return GA_UNSUPPORTED; } while (0)
static ga_result_t culled(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0x32646)) return GA_DELETE;                         /* 004188 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
ga_result_t neo_porter_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x3264e:
        SB(0x69,0xff);                                                  /* 019DFC */
        SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0x92);START(0x2389ce);
        k3290e(a);SW(0x7e,0);SW(0x6e,W(0x6e)+e->camera);
        if (W(0x70)) SB(0x15,B(0x15)|1u);
        ga_floor(a,e);SL(0,0x326a8);
        /* fall through */
    case 0x326a8:
        if (W(0x6e)!=0xffffu && e->camera>W(0x6e)) {
            neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x327ec,false):NULL;
            uint16_t d0=(uint16_t)((W(0x70)?0xffc0u:0x180u)-W(0x3c));
            if (c) {ga_place_at(c,a,e,(int16_t)d0,0x10);ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x70,W(0x70));}
            SL(0,0x326fe);
        }
        /* fall through */
    case 0x326fe:
        if (!B(0x80)) {k8ccd4(a,e);STEP();return culled(a,e);}
        START(0x2386f0);SL(0x62,0);SL(0,0x327ca);
        /* fall through */
    case 0x327ca:
        ga_stationary(a,e);STEP();return culled(a,e);
    case 0x327ec:
        SW(0x66,0x8800);SW(0x1e,0x8800);ga_palette(a,0x114);START(0x237f36);
        ga_floor(a,e);k3290e(a);SL(0x62,0xfffd8000u);
        if (W(0x70)) {SB(0x15,B(0x15)|1u);SL(0x62,0u-L(0x62));}
        SL(0,0x32840);
        /* fall through */
    case 0x32840:
        if (B(0x82) && !(e->has_children && e->has_children(e->ctx,a))) {
            SB(0x83,0xff);
            neo_gactor_t *p=creator(a,e);if (p) ga_sb(p,0x83,0xff);
            SL(0,0x32864);
        }
        /* fall through */
    case 0x32864:
        k8ccd4(a,e);STEP();return culled(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_porter_routine(uint32_t r) {
    return r==0x3264e || r==0x326a8 || r==0x326fe || r==0x327ca || r==0x327ec || r==0x32840 || r==0x32864;
}
