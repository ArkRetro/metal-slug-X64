#include "neo_region.h"
/* Mission 6 stage 1 (the coast) props, manual translation (ROM labels as
 * provenance; MAME run4 replay from sec09_292683, captures-m6/fam/R,
 * docs/MISION6.md "Fase 2").
 * - 0BD71E (call of the camera list 0BCA00, head): the rafts 0BD822 /
 *   0BD836 / 0BD84A at (010A / 0270 / 02F0, 0020) of the root 10E968 with
 *   +74 = 0 / 4 / 8, then three 0BDA4E at the same points (0040BC, D0 = 1C).
 * - 0BD822 / 0BD836 / 0BD84A (the raft): its float 0BD8F4 / 0BD906 / 0BD918
 *   (0040BC, D0 = 1C; +38 = the root; 0BD84A also copies its +74), +66 =
 *   +1E = 0, +30 = 0, +34 = 1, +15 bit 0 clear, +6E = 0340, +72 = 0.
 *   0BD898: 0BD9D4 counts the players in play inside 0BDA46 (01B332: +72 =
 *   2 per player), +34 = 0 / +1 / -1 to bring +2C to 0BDA0C[+72] (0020,
 *   001C, 0018: the raft sinks under its riders), 01AC38, 004188 once
 *   10E99E >= +6E.
 * - 0BD8F4 / 0BD906 / 0BD918 (the float): palette 4CB / 4CC / 4CD, +6E =
 *   the creator's +6E, program 2A953E, +66 = +1E = 0, +15 bit 0 clear, +76 =
 *   0. 0BD95E: the bobbing table 0BDA12 (updates, dy) by +74 (+76 counts
 *   the updates; at the entry's count +74 steps 4, & 1C), 01AD6E at (0, dy)
 *   of the raft, 01ABC8, the cell map 0BDA32 (0140C0: nine cells 0B, the
 *   floor the player stands on), 00427A, 004188 once 10E99E >= +6E.
 * - 0BDA4E (the raft's picture): palette 509, program 2A954E, +66 = +1E =
 *   0, +15 bit 0 clear, +6E = 0340, +15 bit 2 set; then 0BD508.
 * - 0BD416 / 0BD444 / 0BD472 / 0BD4A6 / 0BD4D4 (decorations of the list
 *   calls 0BD2FA, 0BD386, 0BD3B6, 0BD3E6; +28/+2C/+38/+6E from the call):
 *   programs 2A946C / 2A947C / 2A948C / 2A949C / 2A94B4, palettes 4D4 /
 *   4D5 / 50B / 4DF / 11D, layers FFF1 (0BD472: 1111 and 012022; 0BD4D4: 0
 *   with +15 bit 0 clear). 0BD508 -> 0BD50E: 01ABC8, 00427A, 004188 once
 *   10E99E >= +6E.
 * - 0BDA8E (list call, progress 32): 0BDAB8 at (01B0, 0060) of the root:
 *   program 2A95A4, +4E = rank 2D3FEE (0260C8); 0BDADE: 01ABC8, 00427A. */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,(uint32_t)(v))
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
static uint16_t rw(uint32_t at) {const uint8_t *p=ga_rom(at);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static bool rom_ok(neo_genv_t *e,uint32_t at) {if (ga_rom(at)) return true;ga_fail(e,at);return false;}
/* 01B332 (01AEB0, 01AF3C): player p in play and inside the rectangle `rect`
 * (x, y, w, h; both edges in) of the actor's projected point. */
static bool player_in(neo_gactor_t *a,neo_genv_t *e,unsigned p,uint32_t rect) {
    if (!((e->players.active_mask>>p)&1u)) return false;
    int16_t d1=(int16_t)(uint16_t)((p?e->players.p2y:e->players.p1y)-W(0x40)),d2=(int16_t)rw(rect+2),d3=(int16_t)(uint16_t)((uint16_t)d2+rw(rect+6));
    if (B(0x15)&2u) d1=(int16_t)-d1;
    if (d1<d2 || d1>d3) return false;
    d1=(int16_t)(uint16_t)((p?e->players.p2x:e->players.p1x)-W(0x3c));d2=(int16_t)rw(rect);d3=(int16_t)(uint16_t)((uint16_t)d2+rw(rect+4));
    if (B(0x15)&1u) d1=(int16_t)-d1;
    return d1>=d2 && d1<=d3;
}
static ga_result_t until(neo_gactor_t *a,neo_genv_t *e) {   /* 10E99E >= +6E: 004188 */
    if (e->unsupported) return GA_UNSUPPORTED;
    return e->camera>=W(0x6e)?GA_DELETE:GA_MORE;
}
static ga_result_t step(neo_gactor_t *a,neo_genv_t *e) {     /* 00427A */
    return ga_anim_step(a,e,NULL)<0?GA_UNSUPPORTED:GA_MORE;
}
static bool start(neo_gactor_t *a,neo_genv_t *e,uint32_t program,uint16_t palette,uint16_t layers,bool palette_first) {
    if (palette_first) ga_palette(a,palette);
    if (!ga_anim_start(a,e,program,0x100)) return false;
    if (!palette_first) ga_palette(a,palette);
    SW(0x66,layers);SW(0x1e,layers);
    return true;
}
static ga_result_t m6props_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    /* ---- 0BD822 / 0BD836 / 0BD84A: the raft ---- */
    case 0xbd822: case 0xbd836: case 0xbd84a: {
        uint32_t f=L(0)==0xbd822u?0xbd8f4u:L(0)==0xbd836u?0xbd906u:0xbd918u;
        neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,f,0x1c):e->spawn?e->spawn(e->ctx,a,f,false):NULL;
        if (c) {
            if (L(0)==0xbd84au) ga_sw(c,0x74,W(0x74));
            ga_sl(c,0x38,0x10e968u);
        }
        SW(0x66,0);SW(0x1e,0);SL(0x30,0);SL(0x34,0x10000);SB(0x15,B(0x15)&0xfeu);SW(0x6e,0x340);SW(0x72,0);SL(0,0xbd898);
    }   /* fall through */
    case 0xbd898: {
        SW(0x72,0);                                                          /* 0BD9D4 */
        for (unsigned p=0;p<2;p++) if (player_in(a,e,p,0xbda46)) SW(0x72,W(0x72)+2u);
        if (!rom_ok(e,0xbda0cu+W(0x72))) return GA_UNSUPPORTED;
        int16_t d1=(int16_t)rw(0xbda0cu+W(0x72)),d2=(int16_t)W(0x2c);
        SL(0x34,d1==d2?0u:d1>d2?0x10000u:0xffff0000u);
        ga_move(a,e);                                                        /* 01AC38 */
        return until(a,e);
    }
    /* ---- 0BD8F4 / 0BD906 / 0BD918: the float ---- */
    case 0xbd8f4: case 0xbd906: case 0xbd918: {
        ga_palette(a,L(0)==0xbd8f4u?0x4cb:L(0)==0xbd906u?0x4cc:0x4cd);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xbd926);return GA_UNSUPPORTED;}
        SW(0x6e,ga_w(p,0x6e));
        if (!ga_anim_start(a,e,0x2a953e,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0);SW(0x1e,0);SB(0x15,B(0x15)&0xfeu);SW(0x76,0);SL(0,0xbd95e);
    }   /* fall through */
    case 0xbd95e: {
        uint16_t i=W(0x74);
        if (!rom_ok(e,0xbda12u+i)) return GA_UNSUPPORTED;
        int16_t n=(int16_t)rw(0xbda12u+i),d2=(int16_t)(W(0x76)+1u);
        if (n<=d2) {d2=0;SW(0x74,(W(0x74)+4u)&0x1cu);}
        SW(0x76,d2);
        int16_t dy=(int16_t)rw(0xbda14u+W(0x74));
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xbd9a2);return GA_UNSUPPORTED;}
        ga_place(a,e,0,dy,NEO_GA_TAG(p->index));                             /* 01AD6E */
        ga_project(a,e);                                                     /* 01ABC8 */
        if (!e->dyn_set) {ga_fail(e,0x140c0);return GA_UNSUPPORTED;}
        e->dyn_set(e->ctx,a,0xbda32,0,0);                                    /* 0140C0 */
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        return until(a,e);
    }
    /* ---- decorations ---- */
    case 0xbda4e:
        if (!start(a,e,0x2a954e,0x509,0,true)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)&0xfeu);SW(0x6e,0x340);SB(0x15,B(0x15)|4u);
        goto deco;
    case 0xbd416: if (!start(a,e,0x2a946c,0x4d4,0xfff1,false)) return GA_UNSUPPORTED;goto deco;
    case 0xbd444: if (!start(a,e,0x2a947c,0x4d5,0xfff1,false)) return GA_UNSUPPORTED;goto deco;
    case 0xbd472:
        if (!start(a,e,0x2a948c,0x50b,0x1111,false)) return GA_UNSUPPORTED;
        if (!e->front_layer) {ga_fail(e,0x12022);return GA_UNSUPPORTED;}
        if (e->front_layer(e->ctx)) SB(0x15,B(0x15)|4u); else SB(0x15,B(0x15)&0xfbu);   /* 012022 */
        goto deco;
    case 0xbd4a6: if (!start(a,e,0x2a949c,0x4df,0xfff1,false)) return GA_UNSUPPORTED;goto deco;
    case 0xbd4d4:
        if (!start(a,e,0x2a94b4,0x11d,0,false)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)&0xfeu);
    deco:
        SL(0,0xbd50e);
        /* fall through */
    case 0xbd50e:
        ga_project(a,e);
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        return until(a,e);
    /* ---- 0BDAB8 ---- */
    case 0xbdab8: {
        if (!ga_anim_start(a,e,0x2a95a4,0x100)) return GA_UNSUPPORTED;
        const uint8_t *t=ga_rom(0x2d3fee);
        if (!t) {ga_fail(e,0x2d3fee);return GA_UNSUPPORTED;}
        SW(0x4e,ga_rank_word(a,e,t));SL(0,0xbdade);
    }   /* fall through */
    case 0xbdade:
        ga_project(a,e);
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_m6props_routine(uint32_t r) {
    static const uint32_t k[]={0xbd822,0xbd836,0xbd84a,0xbd898,0xbd8f4,0xbd906,0xbd918,0xbd95e,0xbda4e,0xbd416,0xbd444,0xbd472,0xbd4a6,0xbd4d4,0xbd50e,
        0xbdab8,0xbdade};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
ga_result_t neo_m6props_tick(neo_gactor_t *a,neo_genv_t *e) {return m6props_tick(a,e);}
