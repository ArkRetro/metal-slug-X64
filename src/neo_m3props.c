#include "neo_region.h"
/* Mission 3 wagon boxes and their map stamper, manual translation (ROM
 * labels as provenance; MAME run4 tap, docs/MISION3.md "Fase 3").
 * - 0B918C / 0B91AE (calls of the camera list 0B8AC8 / 0B8B0C): 039382 /
 *   03938A (+6E = 0 / 1) after the mission actor (0040BC, D0 = 1C) with +28
 *   = 0770 / 0870 and +2C = 0040 (neo_world).
 * - 039390: 01AB98 on 10C72E at (+28, +2C), layer 8834, palette 04BF
 *   (009EA8), +15 bit 0 clear, program 23C762 (004256, 0x100), +26 = 0,
 *   +4E = 1, hurtbox 039542; 0393DC: 01ABC8, 00427A and, with +3C < 0x148,
 *   004C24: a hit (carry) goes to 039406: sound 1069, +26 = 0x100; 03941C:
 *   01ABC8, 00427A; at its end 039436: the stamper of table 039572[+6E]
 *   (0040BC, D0 = 1C) and 039458 (01ABC8, 00427A). Every state ends in
 *   039468: 004188 once +3C <= -0x40 (after the cartridge check 039472,
 *   not reproduced).
 * - 0B911E / 0B9140: +78 = 0x74 / 0x84, +7A = 0x0B, +7C = +7E = 4, program
 *   0B9090, sub-map 0B85EA, +80 = 1, +76 = 0, then 0B8FA8: --+80; at 0 the
 *   next 8-byte record of the program (+1 = length): FF -> 004188; 1 -> +80
 *   = its timer and 0C70FC on 10C72E with D0/D1 = its source block, D2/D3 =
 *   +7C/+7E, D4/D5 = +78/+7A. The last record of 0B9090 leaves the 0B
 *   ledge at the rows the player climbs onto the wagon (x 1864..1903 and
 *   2120..2159): the 145 mutable cells of the pack are these six stamps. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,0x1c):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
/* 039468 */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    if (e->unsupported) return GA_UNSUPPORTED;
    return (int16_t)W(0x3c)<=-0x40?GA_DELETE:GA_MORE;
}
static ga_result_t step(neo_gactor_t *a,neo_genv_t *e,int *r) {
    ga_project(a,e);
    *r=ga_anim_step(a,e,NULL);
    return *r<0?GA_UNSUPPORTED:GA_MORE;
}
/* 0B8FA8 */
static ga_result_t stamper(neo_gactor_t *a,neo_genv_t *e) {
    int16_t t=(int16_t)(W(0x80)-1u);
    if (t>0) {SW(0x80,t);return GA_MORE;}
    const uint8_t *p=ga_rom(L(0x6e));
    if (!p) {ga_fail(e,L(0x6e));return GA_UNSUPPORTED;}
    uint16_t d3=W(0x76);d3=(uint16_t)(d3+p[d3+1u]);
    if (p[d3]==0xffu) return GA_DELETE;
    if (p[d3]!=1u) {ga_fail(e,0xb902a);return GA_UNSUPPORTED;}                  /* TRAP #15 */
    SW(0x76,d3);SW(0x80,p[d3+2u]<<8|p[d3+3u]);
    const int16_t d[6]={(int16_t)(p[d3+4u]<<8|p[d3+5u]),(int16_t)(p[d3+6u]<<8|p[d3+7u]),(int16_t)W(0x7c),(int16_t)W(0x7e),(int16_t)W(0x78),(int16_t)W(0x7a)};
    if (e->map_patch) e->map_patch(e->ctx,0x10c72eu,L(0x72),d);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t m3props_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x39382: SW(0x6e,0);goto box;
    case 0x3938a: SW(0x6e,1);
    box:
        ga_place(a,e,(int16_t)W(0x28),(int16_t)W(0x2c),0x10c72eu);
        SW(0x66,0x8834);SW(0x1e,0x8834);ga_palette(a,0x4bf);SB(0x15,B(0x15)&0xfeu);
        if (!ga_anim_start(a,e,0x23c762,0x100)) return GA_UNSUPPORTED;
        SW(0x26,0);SW(0x4e,1);SL(0x50,0x39542);SL(0,0x393dc);
        /* fall through */
    case 0x393dc:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        if ((int16_t)W(0x3c)<0x148 && ga_register(a,e,NULL,NULL)) SL(0,0x39406);
        return tail(a,e);
    case 0x39406:
        if (e->sound) e->sound(e->ctx,0x1069);
        SW(0x26,0x100);SL(0,0x3941c);
        /* fall through */
    case 0x3941c:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        if (r) SL(0,0x39436);
        return tail(a,e);
    case 0x39436:
        spawn(a,e,W(0x6e)?0xb9140u:0xb911eu);SL(0,0x39458);
        /* fall through */
    case 0x39458:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        return tail(a,e);
    case 0xb9140: SW(0x78,0x84);goto ledge;
    case 0xb911e: SW(0x78,0x74);
    ledge:
        SW(0x7a,0xb);SW(0x7c,4);SW(0x7e,4);SL(0x72,0xb85ea);SL(0x6e,0xb9090);SW(0x80,1);SW(0x76,0);SL(0,0xb8fa8);
        /* fall through */
    case 0xb8fa8:
        return stamper(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_m3props_routine(uint32_t r) {
    return r==0x39382u || r==0x3938au || r==0x393dcu || r==0x39406u || r==0x3941cu || r==0x39436u || r==0x39458u ||
        r==0xb911eu || r==0xb9140u || r==0xb8fa8u;
}
ga_result_t neo_m3props_tick(neo_gactor_t *a,neo_genv_t *e) {return m3props_tick(a,e);}
