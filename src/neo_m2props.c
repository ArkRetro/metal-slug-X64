#include "neo_region.h"
/* Mission 2 stage 2 props, manual translation (ROM labels as provenance).
 * - 06851A (script): +15 bit 0 = (+6E != 0), program 26939A/2693A6 by it,
 *   layer E06C, palettes released (009F88), palette 0xF5, flash palette
 *   0xF6, hurtbox E1AA2, health 2D296A (0260C8), floor 0146E8, +60 = 0 and
 *   the overlay 0685FE (0040BC, 01B220). 0685C8: 01436A, 00427A, 004C24
 *   (carry: +5F bit 7 -> 06870A, else +5A = 0), the dynamic object E1B6A
 *   (0140C0) and the cull 0688D6 (E1A5E: +60 = FF, 004188).
 *   06870A: +60 = FF, 013F1E, debris E1BF2/E1C04 (01DA40), explosion
 *   01C71E 32 px up (0040BC, 01ACF2), sound 1035; next update 06875A:
 *   layer 0, program 2693B2, 019DFC, row E1B2E (0049E8), then 06878E
 *   (01436A, 00427A: at its end 004188; else 0688D6).
 * - 0685FE (overlay): layer 0, palettes as above, program 269382/26938E by
 *   +15 bit 0; 06865C: 01436A, 0044F6 (+17 from the creator), 00427A and
 *   004188 (after the cartridge check, not reproduced) once the creator's
 *   +60 is set.
 * - 03A892: RAM 10C358 = 1 (read by 03A75A) and 004188.
 * - 052A5C (stage 3): +6E = camera progress 10E99E + 1 when it is 0;
 *   052A74 waits until the progress reaches +6E and leaves (004188). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
/* 01DA40: debris emitter 01D764 with the list `list`, +66/+1E = 8800. */
static void debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=ga_spawn_from(a,e,0x1d764,0,0);
    if (!c) return;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);ga_sw(c,0x66,0x8800);ga_sw(c,0x1e,0x8800);
}
static void palettes(neo_gactor_t *a) {SB(0x1d,0);ga_palette(a,0xf5);ga_flash_palette(a,0xf6);}   /* 009F88, 009EA8, 004520 */
/* 0688D6 */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    if (e->unsupported) return GA_UNSUPPORTED;
    if (ga_cull(a,e,0xe1a5e)) {SB(0x60,0xff);return GA_DELETE;}
    return GA_MORE;
}
static ga_result_t bags_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x6851a: {
        SB(0x15,(B(0x15)&0xfeu)|(W(0x6e)?1u:0u));
        START((B(0x15)&1u)?0x2693a6:0x26939a);
        SW(0x66,0xe06c);SW(0x1e,0xe06c);palettes(a);SL(0x50,0xe1aa2);
        const uint8_t *t=ga_rom(0x2d296a);
        if (!t) {ga_fail(e,0x2d296a);return GA_UNSUPPORTED;}
        SW(0x4e,ga_rank_word(a,e,t));ga_floor(a,e);SB(0x60,0);
        ga_spawn_from(a,e,0x685fe,0,0);SL(0,0x685c8);
    }   /* fall through */
    case 0x685c8:
        ga_stationary(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5f)&0x80u) goto broken;
            SB(0x5a,0);
        }
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe1b6a,0,0);
        return tail(a,e);
    broken: {
        SB(0x60,0xff);
        if (e->dyn_release) e->dyn_release(e->ctx,a);
        debris(a,e,0xe1bf2);debris(a,e,0xe1c04);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x1c71e,false):NULL;
        if (c) ga_place_at(c,a,e,0,0x20);
        if (e->sound) e->sound(e->ctx,0x1035);
        SL(0,0x6875a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x6875a:
        SW(0x66,0);SW(0x1e,0);START(0x2693b2);SB(0x69,0xff);
        ga_attack(a,e,0xe1b2e);SL(0,0x6878e);
        /* fall through */
    case 0x6878e:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return tail(a,e);
    case 0x685fe:
        SW(0x66,0);SW(0x1e,0);palettes(a);
        START((B(0x15)&1u)?0x26938e:0x269382);SL(0,0x6865c);
        /* fall through */
    case 0x6865c: {
        ga_stationary(a,e);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x6866e);return GA_UNSUPPORTED;}
        uint8_t d=ga_b(p,0x17);                                       /* 0044F6 */
        if (d) {if (!(d&0x80u)) d++;SB(0x17,d);}
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        return ga_b(p,0x60)?GA_DELETE:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_m2props_routine(uint32_t r) {
    return r==0x6851au || r==0x685c8u || r==0x6875au || r==0x6878eu || r==0x685feu || r==0x6865cu || r==0x3a892u || r==0x52a5cu || r==0x52a74u;
}
ga_result_t neo_m2props_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x3a892u) {if (e->flags) e->flags->signal358=1;return GA_DELETE;}
    if (L(0)==0x52a5cu) {if (!W(0x6e)) SW(0x6e,e->camera+1u);SL(0,0x52a74);}
    if (L(0)==0x52a74u) return W(0x6e)>e->camera?GA_MORE:GA_DELETE;
    return bags_tick(a,e);
}
