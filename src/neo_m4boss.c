#include "neo_region.h"
/* Mission 4 boss 02267E (script order 0BB028, camera 5552): manual
 * translation of its parts, one by one; ROM labels are provenance only.
 *
 * Debris 0259F2 / 025A5E (created by the boss's parts, capture T):
 * 0259F2: palettes released (009F88), depth +66 / +1E = E0EC, palette 0xF0
 * (009EA8, D1 = 0), program 246AD2 at speed 0x100, +15 bit 2, and bit 0 (the
 * facing) when the random number's bit 0 is clear (0029C6); then 025A42
 * every update: 01ABC8, the program (004188 at its end), deleted outside the
 * rectangle E007C (024978 -> 019504).
 * 025A5E: the same with palette 0x276, +30 = 1000, +34 = C000 and program
 * 234D04; then 025AAA: 01AC38 (moves by +30 / +34), the program (its end
 * falls to the cartridge check 025ABA and 004188), 024978. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static const uint32_t routines[]={0x259f2,0x25a42,0x25a5e,0x25aaa,0x25b6e,0x25baa,0x25be2,0x25c1e,0x25cde,0x25d1a,0x25dc2,0x25e18};
bool neo_m4boss_routine(uint32_t r) {
    for (unsigned i=0;i<sizeof(routines)/sizeof(routines[0]);i++) if (routines[i]==r) return true;
    return false;
}
/* 025B6E / 025BE2 / 025CDE / 025DC2: palettes released, depth 1301,
 * palette 0xFC, +15 bit 2, the program at speed 0x100. */
static bool flash_start(neo_gactor_t *a,neo_genv_t *e,uint32_t program,uint32_t next) {
    SB(0x1d,0);                                                         /* 009F88 */
    SW(0x66,0x1301);SW(0x1e,0x1301);ga_palette(a,0xfc);SB(0x15,B(0x15)|0x04u);
    if (!ga_anim_start(a,e,program,0x100)) return false;
    SL(0,next);return true;
}
ga_result_t neo_m4boss_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x259f2:
        SB(0x1d,0);                                                     /* 009F88 */
        SW(0x66,0xe0ec);SW(0x1e,0xe0ec);
        ga_palette(a,0xf0);
        if (!ga_anim_start(a,e,0x246ad2,0x100)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)|0x04u);
        if (!(ga_rand(e)&1u)) SB(0x15,B(0x15)|0x01u);
        SL(0,0x25a42);
        /* fall through */
    case 0x25a42: {
        ga_project(a,e);                                                /* 01ABC8 */
        int st=ga_anim_step(a,e,NULL);                                  /* 00427A */
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;                                       /* 004188 */
        break;
    }
    case 0x25a5e:
        SB(0x1d,0);                                                     /* 009F88 */
        SW(0x66,0xe0ec);SW(0x1e,0xe0ec);
        ga_palette(a,0x276);
        SL(0x30,0x1000);SL(0x34,0xc000);
        SB(0x15,B(0x15)|0x04u);
        if (!ga_anim_start(a,e,0x234d04,0x100)) return GA_UNSUPPORTED;
        SL(0,0x25aaa);
        /* fall through */
    case 0x25aaa: {
        ga_move(a,e);                                                   /* 01AC38 */
        int st=ga_anim_step(a,e,NULL);
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;                                       /* 025ABA -> 004188 */
        break;
    }
    /* The flashes made by the boss's program calls (025B4E / 025BC2 / 025CBE
     * at -48, -30 through 01ACF2; 025D32..025DAA with +90 = 0..5): 01ABC8
     * and the program to its end (004188), no cull, except 025DC2: placed
     * on its creator (01AD6E), program +90 of the table E06DA, 024978. */
    case 0x25b6e: if (!flash_start(a,e,0x234c06,0x25baa)) return GA_UNSUPPORTED; /* fall through */
    case 0x25baa: goto flash;
    case 0x25be2: if (!flash_start(a,e,0x234bfc,0x25c1e)) return GA_UNSUPPORTED; /* fall through */
    case 0x25c1e: goto flash;                                           /* its end: the cartridge check 025C2E -> 004188 */
    case 0x25cde: if (!flash_start(a,e,0x234c62,0x25d1a)) return GA_UNSUPPORTED; /* fall through */
    case 0x25d1a:
    flash: {
        ga_project(a,e);
        int st=ga_anim_step(a,e,NULL);
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x25dc2: {
        SB(0x1d,0);ga_palette(a,0xfc);                                  /* 009F88, 009EA8 */
        SW(0x66,0x1301);SW(0x1e,0x1301);SB(0x15,B(0x15)|0x04u);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        ga_place(a,e,0,0,p?(p->index>=0?NEO_GA_TAG(p->index):L(0x0c)+0x28u):0);   /* 01AD6E: +38 = A0 + 28 (a creator the family test reads from RAM: its address) */
        const uint8_t *t=ga_rom(0xe06dau+(uint32_t)(int32_t)(int16_t)(uint16_t)(W(0x90)<<2));   /* (a1, d0.w) */
        if (!t) {ga_fail(e,0xe06da);return GA_UNSUPPORTED;}
        if (!ga_anim_start(a,e,(uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3],0x100)) return GA_UNSUPPORTED;
        SL(0,0x25e18);
    }   /* fall through */
    case 0x25e18: {
        ga_project(a,e);
        int st=ga_anim_step(a,e,NULL);
        if (st<0) return GA_UNSUPPORTED;
        if (st) return GA_DELETE;                                       /* 025E28 -> 004188 */
        break;
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    if (ga_cull_rect(a,e,0xe007c)) return GA_DELETE;                    /* 024978 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
