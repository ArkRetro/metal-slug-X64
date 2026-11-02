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
 * - 03A6EA (the stage-2 gate, made by the map command 0B7326 at (0xA70,
 *   0x2C0) of the root): 01AB98 on the root 10E968 from its +28/+2C words,
 *   the overlay 03A82E (0040BC reserve 0x1C, 01B220), layer 0, palette
 *   0x445, +15 bit 0 clear, program 23CDD0, RAM 10C358 = 0; 03A75A waits
 *   for 10C358 (01ABC8, 00427A every update). 03A776: 0B7350 (the map
 *   patch 0B706C through 0C70FC at block (0xA2, 0x2F), sound 103B, the
 *   exit 0B6ED2 at (0xAA0, 0x280) of the root with the rectangle 0B7348
 *   and +72 = 0xAC0), the dust 03A464 (reserve 0x2A, 01B220 (-8, -64)),
 *   +6E = +2C, +30 = 0, +34 = 4.0; 03A7BA (01AC38, 00427A) until +2C has
 *   risen 0x50 px; 03A7DE/03A7E8: one update 0x100 * sine 0029FC[+70 << 7]
 *   up (+2C restored after 00427A), then 03A768 (01ABC8, 00427A) for good.
 * - 03A82E (overlay): layer 0, palette 0x44B, +15 bit 2 set, bit 0 clear,
 *   program 23CDDC; 03A86A: 01ABC8, 00427A.
 * - 03A464 (dust): palette 0x204, program 22D372; 03A488: 01436A, 00427A,
 *   004188 at the program's end (after the cartridge check, not
 *   reproduced).
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
static neo_gactor_t *spawn_reserve(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);                                /* 01B220 */
    return c;
}
/* 0B7350: map patch 0B706C (0C70FC), sound 103B, the exit 0B6ED2. */
static void gate_open(neo_gactor_t *a,neo_genv_t *e) {
    const int16_t d[6]={0,0,8,0xa,0xa2,0x2f};
    if (e->map_patch) e->map_patch(e->ctx,0x10c72e,0xb706c,d);
    if (e->sound) e->sound(e->ctx,0x103b);                            /* 0120F8 */
    neo_gactor_t *x=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0xb6ed2,0x1c):e->spawn?e->spawn(e->ctx,a,0xb6ed2,false):NULL;
    if (x) {ga_sl(x,0x38,0x10e968);ga_sl(x,0x28,0xaa00000);ga_sl(x,0x2c,0x2800000);ga_sl(x,0x6e,0xb7348);ga_sw(x,0x72,0xac0);}
}
static ga_result_t gate_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x3a6ea:
        ga_place(a,e,(int16_t)W(0x28),(int16_t)W(0x2c),0x10e968u);   /* 01AB98 */
        spawn_reserve(a,e,0x3a82e,0x1c,0,0);
        SW(0x66,0);SW(0x1e,0);ga_palette(a,0x445);SB(0x15,B(0x15)&0xfeu);
        START(0x23cdd0);
        if (e->flags) e->flags->signal358=0;
        SL(0,0x3a75a);
        /* fall through */
    case 0x3a75a:
        if (e->flags && e->flags->signal358) SL(0,0x3a776);
        /* fall through */
    case 0x3a768:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x3a776:
        gate_open(a,e);
        spawn_reserve(a,e,0x3a464,0x2a,-8,-64);
        SW(0x6e,W(0x2c));SL(0x30,0);SL(0x34,0x40000);SL(0,0x3a7ba);
        /* fall through */
    case 0x3a7ba:
        ga_move(a,e);                                                 /* 01AC38 */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if ((int16_t)(W(0x2c)-W(0x6e))>=0x50) SL(0,0x3a7de);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x3a7de:
        SB(0x70,0);SL(0,0x3a7e8);
        /* fall through */
    case 0x3a7e8: {
        uint16_t d0=(uint16_t)(W(0x70)+1u);SW(0x70,d0);
        if (d0<0x19u) SL(0,0x3a768);
        const uint8_t *t=ga_rom(0x29fcu+(uint16_t)((uint16_t)(d0<<7)&0xffu));
        if (!t) {ga_fail(e,0x29fc);return GA_UNSUPPORTED;}
        uint32_t y=L(0x2c);
        SL(0x2c,y+((uint32_t)(int32_t)(int16_t)(t[0]<<8|t[1])<<8));
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        SL(0x2c,y);
        if (r<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x3a82e:
        SW(0x66,0);SW(0x1e,0);ga_palette(a,0x44b);
        SB(0x15,(B(0x15)|4u)&0xfeu);START(0x23cddc);SL(0,0x3a86a);
        /* fall through */
    case 0x3a86a:
        ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x3a464:
        ga_palette(a,0x204);START(0x22d372);SL(0,0x3a488);
        /* fall through */
    case 0x3a488: {
        ga_stationary(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- Signs 0B829C / 0B833A (script records of sections 2 and 3, uncounted) ----
 * - 0B829C: floor 0146E8, layer 0, palette 0x474, +6E & 3, the collision
 *   stamp B8454[+6E] (0C88B0: templates B8464/B8492), program B84AC[+6E];
 *   0B8302: 01ABC8, 00427A; +70 nonzero and below the camera progress
 *   10E99E (unsigned) or outside B8444 (0C8BEC): 0B843C (0041AC).
 * - 0B833A: palette 0x4B8 (+70 = 0) or 0x4C3, layer 0, +6E & 0xF (0 from
 *   9 on); 0B8384: program B84BC[+6E]; 0B83A4: 01ABC8, 00427A; a player in
 *   B844C (01AFFC) wiggles it (0B83F0: B84E0[+6E], back to 0B8384 at its
 *   end); +72 below the progress or outside B8444: 0B843C. */
static uint32_t rl(uint32_t at) {const uint8_t *p=ga_rom(at);return p?((uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]):0;}
static bool sign_gone(neo_gactor_t *a,neo_genv_t *e,uint16_t limit) {
    return (limit && limit<e->camera) || ga_outside_view(a,e,0xb8444);
}
static ga_result_t sign_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0xb829c:
        ga_floor(a,e);SW(0x66,0);SW(0x1e,0);ga_palette(a,0x474);
        SW(0x6e,W(0x6e)&3u);
        if (e->stamp) e->stamp(e->ctx,a,rl(0xb8454u+4u*W(0x6e)));
        START(rl(0xb84acu+4u*W(0x6e)));SL(0,0xb8302);
        /* fall through */
    case 0xb8302:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (sign_gone(a,e,W(0x70))) SL(0,0xb843c);
        return GA_MORE;
    case 0xb833a:
        ga_palette(a,W(0x70)?0x4c3:0x4b8);SW(0x66,0);SW(0x1e,0);
        SW(0x6e,W(0x6e)&0xfu);if (W(0x6e)>=9u) SW(0x6e,0);
        /* fall through */
    case 0xb8384:
        START(rl(0xb84bcu+4u*W(0x6e)));SL(0,0xb83a4);
        /* fall through */
    case 0xb83a4:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (!ga_players_in(a,e,0xb844c)) SL(0,0xb83f0);
        if (sign_gone(a,e,W(0x72))) SL(0,0xb843c);
        return GA_MORE;
    case 0xb83f0:
        START(rl(0xb84e0u+4u*W(0x6e)));SL(0,0xb8410);
        /* fall through */
    case 0xb8410:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xb8384);
        if (ga_outside_view(a,e,0xb8444)) SL(0,0xb843c);
        return GA_MORE;
    case 0xb843c:
        return GA_DELETE_TREE;                                          /* 0041AC */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool sign_routine(uint32_t r) {
    return r==0xb829cu || r==0xb8302u || r==0xb833au || r==0xb8384u || r==0xb83a4u || r==0xb83f0u || r==0xb8410u || r==0xb843cu;
}
bool neo_m2gate_routine(uint32_t r) {
    return r==0x3a6eau || r==0x3a75au || r==0x3a768u || r==0x3a776u || r==0x3a7bau || r==0x3a7deu || r==0x3a7e8u ||
        r==0x3a82eu || r==0x3a86au || r==0x3a464u || r==0x3a488u;
}
bool neo_m2props_routine(uint32_t r) {
    return neo_m2gate_routine(r) || sign_routine(r) || r==0x6851au || r==0x685c8u || r==0x6875au || r==0x6878eu || r==0x685feu || r==0x6865cu || r==0x3a892u || r==0x52a5cu || r==0x52a74u;
}
ga_result_t neo_m2props_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (neo_m2gate_routine(L(0))) return gate_tick(a,e);
    if (sign_routine(L(0))) return sign_tick(a,e);
    if (L(0)==0x3a892u) {if (e->flags) e->flags->signal358=1;return GA_DELETE;}
    if (L(0)==0x52a5cu) {if (!W(0x6e)) SW(0x6e,e->camera+1u);SL(0,0x52a74);}
    if (L(0)==0x52a74u) return W(0x6e)>e->camera?GA_MORE:GA_DELETE;
    return bags_tick(a,e);
}
