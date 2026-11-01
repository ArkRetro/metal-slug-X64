#include "neo_region.h"
/* Mission 2 stage 3: the drone 03670E (+96 = 4) and its bomb 0369AC,
 * manual translation (ROM labels as provenance).
 * - 03670E: +76/+74/+77/+78 = 0, palettes 0x142/0x143, program 23BA74,
 *   layer E0ED, health 2D0C7C (0260C8), hurtbox 035A84, 01B1DC, 01ABC8,
 *   00427A; speed +70 = 2D0CAE << 8, +30 = +70 (negated right of x 0xA0).
 * - 0367B6: +78 = FF once x - 0x28 is below 0xF0 (unsigned); +74 counts
 *   down; 035AF0 (004C24: +9F = +5A, +5A = 0; +5F bit 7 -> 0368DA); with
 *   the nearest player (01AF9A) it turns 0x30 px past it, and above it
 *   (|dx| <= 4, +74 = 0) drops the bomb (0368C2 path, when +78 and +77)
 *   or arms +74 = 2D0CF8 / 2 and +77 = FF. 03685A: +34 = 0x18000 towards
 *   y 0x100 + +6E (none within 4 px), 01AC38, 00427A, cull 035A54
 *   (01945C, 0041AC).
 * - 03689C: +14 = +19, program 23BB52, 019DFC, 0366DA (the bomb 0369AC
 *   20 px up once, +76), 0368C2: 035AF0 (-> 0368DA), 01AC38, 00427A, cull.
 * - 0368DA: +14 = +19, 0366DA, program 23BBD0, 019DFC, sound 10F0;
 *   03690A: 01ABC8, 00427A; at its end 004188 (after the cartridge check,
 *   not reproduced), else the cull.
 * - 0369AC (bomb): 01B1DC, program 23BC72, palette 0x143, layer 8820;
 *   0369E2: 0143DC; on the floor (+15 bit 6) or when row 035AC8 hits
 *   (0049E8) -> 036A0C: palettes released, palette 0x4BB, program 23BCF2,
 *   +61 = FF; 036A3A: row 035AB8 while +61, 01ABC8, 00427A, 004188 at its
 *   end. Otherwise 00427A and the cull. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define S16(v) ((int16_t)(uint16_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static bool rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table,uint16_t *out) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return false;}
    *out=ga_rank_word(a,e,t);return true;
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 035AF0: carry = +5F bit 7. */
static bool k35af0(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) {SB(0x9f,B(0x5a));SB(0x5a,0);}
    return (B(0x5f)&0x80u)!=0;
}
/* 01AF9A: the nearest live player's X; carry (true) when none. */
static bool nearest_x(neo_gactor_t *a,neo_genv_t *e,uint16_t *x) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m) return true;
    if (m==1u) {*x=e->players.p1x;return false;}
    if (m==2u) {*x=e->players.p2x;return false;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    *x=S16(d2)<S16(d1)?e->players.p2x:e->players.p1x;
    return false;
}
/* 0366DA */
static void bomb(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x76)) ga_spawn_from(a,e,0x369ac,0,(int16_t)-0x14);
    SB(0x76,0xff);
}
/* 03623E */
static ga_result_t cull(neo_gactor_t *a,neo_genv_t *e) {
    if (e->unsupported) return GA_UNSUPPORTED;
    return ga_cull(a,e,0x35a54)?GA_DELETE_TREE:GA_MORE;
}
static ga_result_t drone_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint16_t v;
    switch (L(0)) {
    case 0x3670e:
        SB(0x96,4);SB(0x76,0);SW(0x74,0);SB(0x77,0);SB(0x78,0);
        ga_palette(a,0x142);ga_palette(a,0x143);START(0x23ba74);
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        if (!rank(a,e,0x2d0c7c,&v)) return GA_UNSUPPORTED;
        SW(0x4e,v);SL(0x50,0x35a84);k1b1dc(a);ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (!rank(a,e,0x2d0cae,&v)) return GA_UNSUPPORTED;
        SL(0x70,(uint32_t)(int32_t)S16(v)<<8);
        SL(0x30,S16(W(0x3c))>0xa0?0u-L(0x70):L(0x70));SL(0,0x367b6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x367b6: {
        if ((uint16_t)(W(0x3c)-0x28u)<0xf0u) SB(0x78,0xff);
        if (W(0x74)) SW(0x74,W(0x74)-1u);
        if (k35af0(a,e)) goto dying;
        uint16_t px;
        if (!nearest_x(a,e,&px)) {
            uint16_t d0=(uint16_t)(W(0x3c)-px);
            if (!(L(0x30)&0x80000000u)) {if (S16(d0)>=0x30) SL(0x30,0u-L(0x70));}
            else if (S16(d0)<=-0x30) SL(0x30,L(0x70));
            if (!W(0x74)) {
                uint16_t m=(d0&0x8000u)?(uint16_t)(0u-d0):d0;
                if (m<=4u) {
                    if (B(0x78) && B(0x77)) goto drop;
                    if (!rank(a,e,0x2d0cf8,&v)) return GA_UNSUPPORTED;
                    SW(0x74,v>>1);SB(0x77,0xff);
                }
            }
        }
        SL(0x34,0);
        uint16_t d0=(uint16_t)(W(0x40)-0x100u-W(0x6e)),d1=(d0&0x8000u)?(uint16_t)(0u-d0):d0;
        if (d1>4u) SL(0x34,(d0&0x8000u)?0x18000u:0xfffe8000u);
        ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return cull(a,e);
    }
    drop:
        SB(0x14,B(0x19));START(0x23bb52);SB(0x69,0xff);bomb(a,e);SL(0,0x368c2);
        /* fall through */
    case 0x368c2:
        if (k35af0(a,e)) goto dying;
        ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return cull(a,e);
    dying:
        SB(0x14,B(0x19));bomb(a,e);START(0x23bbd0);SB(0x69,0xff);
        if (e->sound) e->sound(e->ctx,0x10f0);
        SL(0,0x3690a);
        /* fall through */
    case 0x3690a:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return cull(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static ga_result_t bomb_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x369ac:
        k1b1dc(a);START(0x23bc72);ga_palette(a,0x143);SW(0x66,0x8820);SW(0x1e,0x8820);SL(0,0x369e2);
        /* fall through */
    case 0x369e2:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                      /* 0143DC */
        if (!(B(0x15)&0x40u) && !ga_attack(a,e,0x35ac8)) {
            if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
            return cull(a,e);
        }
        if (e->unsupported) return GA_UNSUPPORTED;
        SB(0x1d,0);ga_palette(a,0x4bb);START(0x23bcf2);SB(0x61,0xff);SL(0,0x36a3a);
        /* fall through */
    case 0x36a3a:
        if (B(0x61)) ga_attack(a,e,0x35ab8);
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_drone_routine(uint32_t r) {
    return r==0x3670eu || r==0x367b6u || r==0x368c2u || r==0x3690au || r==0x369acu || r==0x369e2u || r==0x36a3au;
}
ga_result_t neo_drone_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r>=0x369acu) return bomb_tick(a,e);
    return drone_tick(a,e);
}
