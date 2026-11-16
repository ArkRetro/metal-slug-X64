#include "neo_region.h"
#include "neo_blood.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
#include "neo_target.h"
/* Mission 3 path enemies, manual translation; ROM labels are provenance only
 * (docs/MISION3.md "Paso 2", docs/MISION3_ACTORES.md).
 *
 * Soldier death on a generic node (0406B6 / 0407B6), shared by the Mission 3
 * enemies that end in it (08A732 ...); the same routines the rebel soldier
 * runs (neo_soldier_ai.c) on the generic record:
 * - 0406B6: nothing while +5A is zero; else 04097A (+5F bit 6 / +6E, +50,
 *   +5F bit 7, the palette of 0405EA by 01B2BE: 20E / 22F, 019DFC, 01495C
 *   with D0 = -1: D6 bit 3 -> +72 = FF and +66/+1E = FFF2, +70 = +15 bit 6,
 *   +76/+62/+71/+6F/+74 = 0), +5A = 0 and the kind +5A through the grounded
 *   table 0406EA or the air table 040750. 0407B6: +73 = FF first.
 * - Entries: 040DC0 (2404FE), 040DD8 (240454), 040A10 (24103C), 040A40
 *   (+76 = rand << 2 by +5B, +34 = 0, 240E8E), 040A7C (splash 040F60, 240DC0 /
 *   240AE6 by the sign of rand, +30 = rand << 2 by +5B, +34 = 0x100000, +15
 *   bit 6 clear, 040AE0 and its body now), 040AF6 (splash 040F60, 240D2E /
 *   240C6E by +5B against the facing, +34 = 0x20000), 040B46 (240A3E /
 *   240AE6 with +72 / 240C76, +34 = 0x20000), 040BA8 (240D3C / 240C76),
 *   040C8E (thrown up and back, 240DEE, 040D06), 040D66 (splash 040F88 unless
 *   +6E, 240588 / 2406EC, 040DB4); each then runs 040DF0.
 * - 040DF0: 0405CE (deleted out of x -20..15F or y < E0), 040852 (ground:
 *   +62 eases to +76, 01456A, D6 & 208 -> +72 = FF, a slope 7/9 with +15 bit 5
 *   leaves the ground and sets +74; air: +34 eases to -0.75, +30 to +76,
 *   0143CC with +15 bit 5 and +72 else 0143E4, D6 bit 9 -> +15 bit 5, bit 0 ->
 *   +72 by bit 3; +70 = +15 bit 6), 00427A (the end deletes), the +71 / +5F
 *   bit 3 blink count. 040D06: the speed +26 by rand in the air, +76 = 0 and
 *   0x100 on the ground, +50 = 24057C while falling, 040A00 (004C24 unless
 *   +73) -> 040DC0. 040DB4: 040A00, 0406B6, 040DF0.
 * - Splashes 040F88 / 040F60 / 040F74 (0040BC D0 = 2A, 01B220 (8, 0) with
 *   the creator's facing flipped): programs 2402FA / 24039C / 2403EE, 040F98
 *   (palette 37A / 3BA by 01B2BE, +66/+1E = E0ED), 040FD4: 01ABC8, 00427A.
 * The cartridge checks before 004188 are not reproduced (they fall through to
 * the deletion on genuine hardware). 040920 (+74: a body that left
 * the ground on a slope 7 / 9) re-expresses its block in the scene root 10E968 (01AC4C) and moves (01AC38) without
 * a sweep. Explicit stop: the crouch entry
 * 040BE6 (its sign test reads the upper byte of D1, not set by the routine).
 *
 * Grenadier 08A732 (script records of sections 2 and 4) and its pieces:
 * - 08A732: +66/+1E = 8820, palette A4, faces right unless +6E, 0146E8, +4E
 *   = 1, +50 = 08AF76, +72 = 28 below 0A, +7E = rank 2D21E2, program 285864.
 * - 08A7AE (idle): 01436A, 0143DC in the air, 00427A; with +7E = 0 and the
 *   node at x 0..140, y 100..200 -> 08A878 (throw: 2858FE, its callback
 *   08A9A4, then 08A788: a new +7E); else with +70 = 0 the target 08AA14
 *   behind -> 08A838 (turn: 2858A6, then 08A798). Every state ends in
 *   08A8B8: +7E count, 01945C (08AF92) deletes, 004C24 and a hit (+5F bit 7):
 *   019DFC, the helmet 08AA6E (D0 = 1C, (-10, 18)) and 0406B6.
 * - 08A9A4 (program callback): unless outside 08AF92 (019504), sound 10A4, the
 *   shell 08AC54 (D0 = 1C, (-18, 1A), +72/+74 of the thrower) and the flash
 *   08AB70 (D0 = 1C, (0, 0)).
 * - 08AA14: with +74, a player riding a vehicle (0AAA88 nonzero) and live
 *   (01AEB0), player 1 first; else 01AF9A.
 * - 08AA6E (helmet): +34 = 0x22222, +7A = -0x2468, palette A4, 285A0C;
 *   08AAAA: 0143E4, deleted on the ground, else +34 += +7A; 00427A; 01945C.
 * - 08AB70 (flash): 8820, palette A5, 2859BA; 08ABA0: 01ABC8, 00427A (end
 *   deletes), deleted outside 08AF92 (019504).
 * - 08AC54 (shell): E0EF, palette A5, 285AB2, +15 bit 5, 01ABC8; to the
 *   target of 08AA14 (or itself): x distance by the facing, at most rank
 *   2D21C8 and at least 40, y + 10, 03B332 with gravity -0x53A0 over +72
 *   updates; +76 = 0, +7A = gravity, +81 = 0, facing right; 08AD18: +30 +=
 *   +76, +34 += +7A, 0143E4, 01945C (08AF9A) deletes, the water test 03C5C6
 *   once (+82), on a surface (+15 bit 7) -> 08AE46 (or deleted in water);
 *   else the attack row 08AF82 (0049E8) -> 08AE46; else +80 = the angle of
 *   (+30, +34) >> 3 (08AF5A, 002EB4), 00427A, +81 -= 38 and on the borrow
 *   the smoke 08AE70 (D0 = 1C, (0, 0)). 08AE46: sound 1022, 009F88, +66/+1E
 *   = 1001, +50 = 0, the explosion 01C706 (jmp).
 * - 08AE70 (smoke): 01B1DC, 1001, palette 37C, 285BBE; 08AEA6: 01AC38,
 *   00427A (end deletes), 01945C (08AF92).
 * Water: 03C5C6, the splash 03C962 (neo_gate.c, family m3splash). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,n) ((B(o)>>(n))&1u)
#define SET(o,n) SB(o,B(o)|(1u<<(n)))
#define CLR(o,n) SB(o,B(o)&~(1u<<(n)))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static int16_t S16(uint16_t v) {return (int16_t)v;}
static int32_t S32(uint32_t v) {return (int32_t)v;}
static uint32_t ASR(uint32_t v,unsigned n) {return (uint32_t)(S32(v)>>n);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
/* 0260C8. Type 2 (0261A4..026188): the row byte picks a list of eight words
 * after the index (12 bytes per bit of the mask byte), one taken at random
 * (0029C6 & E). */
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    if (t[0]!=2u) return ga_rank_word(a,e,t);
    unsigned coop=e->players.active_mask==3u;
    unsigned rk=e->players.mode88==2?e->players.rank:3u;
    unsigned mask=(1u<<((rk&7u)+1u))-1u,row=0,bits=0;
    for (unsigned b=t[1]&mask&0xffu;b;b&=b-1u) row++;
    for (unsigned b=t[1];b;b&=b-1u) bits++;
    if (!row) {ga_fail(e,0x26118);return 0;}                          /* TRAP #15 */
    if (B(0x68)>=12u) {ga_fail(e,0x2615e);return 0;}
    unsigned index=t[2+((row-1u)*2u+coop)*6u+B(0x68)];
    const uint8_t *w=ga_rom(table+2u+12u*bits+16u*index);
    if (!w) {ga_fail(e,table);return 0;}
    unsigned o=ga_rand(e)&0xeu;
    return (uint16_t)(w[o]<<8|w[o+1]);
}
/* 0040BC with its reserve D0, then 01B220 (dx, dy). */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* ---- soldier death on a generic node (0406B6) ------------------------------- */
/* 040ED0 / 040F04: the splash with the facing flipped for 0040BC / 01B220. */
static void splash(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    SB(0x15,B(0x15)^1u);child(a,e,routine,0x2a,8,0);SB(0x15,B(0x15)^1u);
}
/* 04097A; returns the D1 its 01495C leaves (040BE6 reads it). */
static uint32_t k4097a(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x5f,6)) {SET(0x5f,6);SB(0x6e,0);}
    SL(0x50,0);CLR(0x5f,7);
    SB(0x1d,0);ga_palette(a,neo_blood_palette(NEO_BLOOD_SOLDIER_DEATH,e->blood));   /* 0405EA: 009F88, 01B2BE */
    SL(0x50,0);SB(0x69,0xff);                                                    /* 019DFC */
    uint32_t d1;uint16_t d6=ga_probe_vertical_d6_d1(a,e,-1,&d1);
    SB(0x72,(d6&8u)?0xff:0);
    if (d6&8u) {SW(0x66,0xfff2);SW(0x1e,0xfff2);}
    SB(0x70,BIT(0x15,6)?0xff:0);
    SL(0x76,0);SL(0x62,0);SB(0x71,0);SB(0x6f,0);SB(0x74,0);
    return d1;
}
/* +5B against the facing (sne / eor.b / bmi). */
static bool against(neo_gactor_t *a) {return (int8_t)(B(0x5b)^(BIT(0x15,0)?0xffu:0u))<0;}
static uint32_t rand_x4(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t d0=(uint32_t)ga_rand(e)<<2;
    return (B(0x5b)&0x80u)?0u-d0:d0;
}
/* Program callbacks of the death programs: 040948 (+6F = random(parameter),
 * 0029F2), 040956 (+15 bit 5), 04095E (+30 = +62 = parameter.0 by the facing). */
static bool d_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x40948:SB(0x6f,ga_rand_range(e,parameter));return true;
    case 0x40956:SET(0x15,5);return true;
    case 0x4095e: {
        uint32_t v=(uint32_t)parameter<<16;
        if (!BIT(0x15,0)) v=0u-v;
        SL(0x30,v);SL(0x62,v);return true;
    }
    }
    return false;
}
enum { D_MORE, D_DELETE, D_STOP };
static int body(neo_gactor_t *a,neo_genv_t *e);
/* 040C8E */
static bool k40c8e(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t d0=((uint32_t)ga_rand(e)<<2)-0x10000u;
    if (B(0x5b)&0x80u) d0=0u-d0;
    SL(0x76,d0);SL(0x30,d0);
    SL(0x34,((uint32_t)ga_rand(e)<<3)+0x40000u);
    neo_rng_result_t r=neo_rng_next_result(e->rng);
    if (r.mixed_word&0x8000u) SB(0x15,B(0x15)^1u);
    SW(0x26,(r.value&0x3fu)+0x80u);
    if (!ga_anim_start(a,e,0x240dee,0x100)) return false;
    CLR(0x15,6);SL(0,0x40d06);return true;
}
/* 040846 from another module (neo_m5mech.c: the pilot 0D447C at 0D475A). */
void neo_m3enemy_death_40846(neo_gactor_t *a,neo_genv_t *e) {k4097a(a,e);SL(0,0x40c8e);}
static int start(neo_gactor_t *a,neo_genv_t *e,uint32_t program,uint32_t routine) {
    if (!ga_anim_start(a,e,program,0x100)) return D_STOP;
    SL(0,routine);return D_MORE;
}
/* 0406B6 (crouch false) / 0407CA (crouch true). */
static int dispatch(neo_gactor_t *a,neo_genv_t *e,bool crouch) {
    if (!B(0x5a)) return D_MORE;
    uint32_t d1=k4097a(a,e);
    uint8_t kind=B(0x5a);SB(0x5a,0);
    if (crouch) SB(0x5b,0);
    if (kind>=0x14) kind=0x0e;
    static const uint16_t ground[0x14]={0xdc0,0xc8e,0xdc0,0xb46,0xb46,0xba8,0xd66,0xa40,0xdc0,0xc8e,0xaf6,0xa7c,0xdd8,0xdd8,0xdc0,0xdc0,0xdc0,0xdc0,0xdc0,0xdc0};
    static const uint16_t air[0x14]={0xdc0,0xc8e,0xdc0,0xb46,0xb46,0xdc0,0xa10,0xa40,0xdc0,0xdc0,0xaf6,0xa7c,0xdc0,0xdc0,0xdc0,0xdc0,0xdc0,0xdc0,0xdc0,0xdc0};
    static const uint16_t low[0x14]={0xa28,0xa28,0xa28,0xb46,0xb46,0xbe6,0xa28,0xa40,0xa28,0xa28,0xaf6,0xa7c,0xa28,0xa28,0xa28,0xa28,0xa28,0xa28,0xa28,0xa28};
    uint16_t entry=crouch?low[kind]:BIT(0x15,6)?ground[kind]:air[kind];
    switch (entry) {
    case 0xdc0:return start(a,e,0x2404fe,0x40df0);
    case 0xdd8:return start(a,e,0x240454,0x40df0);
    case 0xa28:return start(a,e,0x240f8e,0x40df0);
    case 0xa10:return start(a,e,0x24103c,0x40df0);
    case 0xa40:SL(0x76,rand_x4(a,e));SL(0x34,0);return start(a,e,0x240e8e,0x40df0);
    case 0xa7c: {
        splash(a,e,0x40f60);
        if (!ga_anim_start(a,e,(ga_rand(e)&0x8000u)?0x240ae6:0x240dc0,0x100)) return D_STOP;
        SL(0x30,rand_x4(a,e));SL(0x34,0x100000);CLR(0x15,6);SL(0,0x40ae0);
        if (S32(L(0x34))>=0) SL(0x34,L(0x34)-ASR(L(0x34),4));
        return body(a,e);
    }
    case 0xaf6:
        splash(a,e,0x40f60);
        if (!ga_anim_start(a,e,against(a)?0x240c6e:0x240d2e,0x100)) return D_STOP;
        SL(0x34,0x20000);CLR(0x15,6);SL(0,0x40df0);return D_MORE;
    case 0xb46:
        if (!ga_anim_start(a,e,!against(a)?0x240a3e:B(0x72)?0x240ae6:0x240c76,0x100)) return D_STOP;
        SL(0x34,0x20000);SL(0,0x40df0);return D_MORE;
    case 0xba8:return start(a,e,against(a)?0x240c76:0x240d3c,0x40df0);
    case 0xc8e:return k40c8e(a,e)?D_MORE:D_STOP;
    case 0xd66:
        if (!B(0x6e)) splash(a,e,0x40f88);
        return start(a,e,against(a)?0x2406ec:0x240588,0x40db4);
    case 0xbe6: {   /* 040BE6: D0.w = +5B (high byte 0 from kind * 4), D1.b = seq of +15 bit 0, eor.w; bmi */
        uint16_t d0w=B(0x5b),d1w=(uint16_t)((d1&0xff00u)|(BIT(0x15,0)?0u:0xffu));
        return start(a,e,((d0w^d1w)&0x8000u)?0x241056:0x2410e2,0x40df0);
    }
    }
    ga_fail(e,(uint32_t)entry|0x40000u);return D_STOP;
}
/* 040852 */
static bool motion(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x15,6)) {
        SL(0x62,L(0x62)+ASR(L(0x76)-L(0x62),2));
        uint16_t d6=ga_ground(a,e,NULL);
        if (d6&0x208u) SB(0x72,0xff);
        if (BIT(0x15,5) && (B(0x6c)==7u || B(0x6c)==9u)) {CLR(0x15,6);SB(0x74,0xff);}
    } else {
        SL(0x34,L(0x34)+ASR(0xfff40000u-L(0x34),5));
        SL(0x30,L(0x30)+ASR(L(0x76)-L(0x30),5));
        SL(0x62,L(0x30));
        if (B(0x74)) {ga_to_root(a,e);ga_move(a,e);goto out;}                  /* 040920: 01AC4C to 10E968, 01AC38 */
        uint16_t d6=BIT(0x15,5) && B(0x72)?ga_air_400a(a,e):ga_air(a,e);
        if (d6&0x200u) SET(0x15,5);
        if (d6&1u) SB(0x72,(d6&8u)?0xff:0);
    }
out:
    SB(0x70,BIT(0x15,6)?0xff:0);
    return true;
}
/* 040DF0 */
static int body(neo_gactor_t *a,neo_genv_t *e) {
    if ((uint16_t)(W(0x3c)+0x20u)>=0x180u || W(0x40)<0xe0u) return D_DELETE;   /* 0405CE */
    if (!motion(a,e)) return D_STOP;
    int r=ga_anim_step(a,e,d_call);
    if (r<0) return D_STOP;
    if (r) return D_DELETE;
    uint8_t t=B(0x71);
    if (!BIT(0x5f,3)) {
        if (t) {t--;if (!t) {SET(0x5f,3);t=0x1e;}SB(0x71,t);}
        return D_MORE;
    }
    if (!t) return D_DELETE;
    SB(0x71,t-1u);return D_MORE;
}
/* 040A00: carry = a hit (004C24) unless +73. */
static bool k40a00(neo_gactor_t *a,neo_genv_t *e) {return B(0x73)?false:ga_register(a,e,NULL,NULL);}
static ga_result_t result(neo_gactor_t *a,neo_genv_t *e,int r) {
    (void)a;
    if (r==D_STOP || e->unsupported) return GA_UNSUPPORTED;
    return r==D_DELETE?GA_DELETE:GA_MORE;
}
/* 0406B6 from a family (its JMP / BSR). */
static ga_result_t death(neo_gactor_t *a,neo_genv_t *e) {return result(a,e,dispatch(a,e,false));}
static ga_result_t death_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x40dc0:return result(a,e,start(a,e,0x2404fe,0x40df0));
    case 0x40c8e:return k40c8e(a,e)?GA_MORE:GA_UNSUPPORTED;                  /* 040846's state (the jeep's turret 0A3BA4) */
    case 0x40ae0:
        if (S32(L(0x34))>=0) SL(0x34,L(0x34)-ASR(L(0x34),4));
        break;
    case 0x40d06:
        if (!BIT(0x15,6)) {
            uint16_t d0=(uint16_t)((ga_rand(e)&0xfu)-0xau+W(0x26));
            if (S16((uint16_t)(d0-0x20u))<0) d0=0x20;
            SW(0x26,d0);
        } else {SL(0x76,0);SW(0x26,0x100);}
        if (S32(L(0x34))<0) SL(0x50,0x24057c);
        if (k40a00(a,e)) SL(0,0x40dc0);
        break;
    case 0x40db4: {
        k40a00(a,e);
        int r=dispatch(a,e,false);
        if (r!=D_MORE) return result(a,e,r);
        break;
    }
    }
    return result(a,e,body(a,e));
}
static ga_result_t splash_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x40f88:START(0x2402fa);goto set;
    case 0x40f60:START(0x24039c);goto set;
    case 0x40f74:START(0x2403ee);
    set:
        ga_palette(a,neo_blood_palette(NEO_BLOOD_SPLASH,e->blood));
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0,0x40fd4);
        /* fall through */
    case 0x40fd4: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- players ------------------------------------------------------------------ */
static uint16_t px(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2x:e->players.p1x;}
static uint16_t py(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2y:e->players.p1y;}
/* 01AF9A: the nearest live player's node; carry (true) when none. */
static bool nearest(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m) {*node=P1_NODE;return true;}
    if (m==1u) {*node=P1_NODE;return false;}
    if (m==2u) {*node=P2_NODE;return false;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    *node=S16(d2)<S16(d1)?P2_NODE:P1_NODE;
    return false;
}
/* 0AAA88: nonzero when player p exists and holds a vehicle slot. */
static bool riding(neo_genv_t *e,unsigned p) {
    if (!e->vehicle || !e->vehicle->slots) return false;
    return e->vehicle->exists[p] && neo_vslot_node(&e->vehicle->slots->slot[p]) && e->vehicle->slots->slot[p].b[0x21];
}
/* 08AA14: carry (true) when none. */
static bool k8aa14(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {
    if (W(0x74)) for (unsigned p=0;p<2;p++)
        if (riding(e,p) && ((e->players.active_mask>>p)&1u)) {*node=p?P2_NODE:P1_NODE;return false;}   /* 01AEB0 */
    return nearest(a,e,node);
}
/* ---- grenadier 08A732 --------------------------------------------------------- */
static bool g_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x8a9a4u) return false;
    if (ga_cull_rect(a,e,0x8af92)) return true;                   /* 019504 */
    sound(e,0x10a4);
    neo_gactor_t *c=child(a,e,0x8ac54,0x1c,-0x18,0x1a);
    if (c) {ga_sw(c,0x72,W(0x72));ga_sw(c,0x74,W(0x74));}
    child(a,e,0x8ab70,0x1c,0,0);
    return true;
}
static int g_step(neo_gactor_t *a,neo_genv_t *e) {
    ga_stationary(a,e);
    if (!BIT(0x15,6)) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}   /* 0143DC */
    return ga_anim_step(a,e,g_call);
}
/* 08A8B8 */
static ga_result_t g_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x7e)) SW(0x7e,W(0x7e)-1u);
    if (ga_cull(a,e,0x8af92)) return GA_DELETE;
    ga_register(a,e,NULL,NULL);
    if (BIT(0x5f,7)) {
        SB(0x69,0xff);                                              /* 019DFC */
        child(a,e,0x8aa6e,0x1c,-0x10,0x18);
        return death(a,e);
    }
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 03B332: speeds to cover D0/D1 in D3 updates under D2. */
static void launch(int16_t d0,int16_t d1,uint32_t d2,uint16_t d3,uint32_t *vx,uint32_t *vy) {
    uint32_t g=(d2&0x80000000u)?0u-d2:d2;
    if ((int16_t)d3<2) d3=2;
    uint16_t q=(uint16_t)(0x10000u/d3);
    uint32_t x=(uint32_t)((int32_t)d0*(int16_t)q),y=(uint32_t)((int32_t)d1*(int16_t)q);
    uint32_t t=(g&0xffffu)*d3+(((g>>16)*d3)<<16);
    t=(uint32_t)((int32_t)t>>1);
    if (d2&0x80000000u) t=0u-t;
    *vx=x;*vy=y-t;
}
static ga_result_t grenadier_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x8a732:
        SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0xa4);
        if (W(0x6e)) CLR(0x15,0); else SET(0x15,0);
        ga_floor(a,e);
        SW(0x4e,1);SL(0x50,0x8af76);
        if (W(0x72)<0x0au) SW(0x72,0x28);
        /* fall through */
    case 0x8a788:
        SW(0x7e,rank(a,e,0x2d21e2));
        /* fall through */
    case 0x8a798:
        START(0x285864);SL(0,0x8a7ae);
        /* fall through */
    case 0x8a7ae: {
        if (g_step(a,e)<0) return GA_UNSUPPORTED;
        int16_t x=S16(W(0x3c)),y=S16(W(0x40));
        if (!W(0x7e) && x<=0x140 && x>=0 && y<=0x200 && y>=0x100) {SL(0,0x8a878);return g_tail(a,e);}
        uint32_t n;
        if (!W(0x70) && !k8aa14(a,e,&n)) {
            int16_t d0=S16((uint16_t)(px(e,n)-W(0x3c)));
            if (!BIT(0x15,0)) d0=(int16_t)-d0;
            if (d0<0) SL(0,0x8a838);
        }
        return g_tail(a,e);
    }
    case 0x8a838:START(0x2858a6);SL(0,0x8a84e);
        /* fall through */
    case 0x8a84e:
        if ((r=g_step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8a798);
        return g_tail(a,e);
    case 0x8a878:START(0x2858fe);SL(0,0x8a88e);
        /* fall through */
    case 0x8a88e:
        if ((r=g_step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8a788);
        return g_tail(a,e);
    case 0x8aa6e:
        SL(0x34,0x22222);SL(0x7a,0xffffdb98u);SL(0x30,0);ga_palette(a,0xa4);
        START(0x285a0c);SL(0,0x8aaaa);
        /* fall through */
    case 0x8aaaa:
        ga_air(a,e);
        if (BIT(0x15,6)) return GA_DELETE;
        SL(0x34,L(0x34)+L(0x7a));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0x8af92)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8ab70:
        SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0xa5);
        START(0x2859ba);SL(0,0x8aba0);
        /* fall through */
    case 0x8aba0:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        if (ga_cull_rect(a,e,0x8af92)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8ac54: {
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);ga_palette(a,0xa5);
        START(0x285ab2);SET(0x15,5);ga_project(a,e);
        int16_t far=S16(rank(a,e,0x2d21c8));
        uint32_t n;int16_t tx,ty;
        if (k8aa14(a,e,&n)) {tx=S16(W(0x3c));ty=S16(W(0x40));} else {tx=S16(px(e,n));ty=S16(py(e,n));}
        int16_t d0=(int16_t)(tx-S16(W(0x3c)));
        if (!BIT(0x15,0)) d0=(int16_t)-d0;
        if (d0>far) d0=far;
        if (d0<0x40) d0=0x40;
        if (!BIT(0x15,0)) d0=(int16_t)-d0;
        int16_t d1=(int16_t)(ty+0x10-S16(W(0x40)));
        uint32_t vx,vy;launch(d0,d1,0xffffac60u,W(0x72),&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0x76,0);SL(0x7a,0xffffac60u);SB(0x81,0);CLR(0x15,0);
        SL(0,0x8ad18);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x8ad18:
        SL(0x30,L(0x30)+L(0x76));SL(0x34,L(0x34)+L(0x7a));
        ga_air(a,e);
        if (ga_cull(a,e,0x8af9a)) return GA_DELETE;
        if (!B(0x82)) {
            SB(0x82,neo_gate_splash(a,e,0)?0xff:0);                      /* 03C5C6 (the splash 03C962), SCS */
        }
        if (BIT(0x15,7)) {
            if (B(0x82)) return GA_DELETE;
        } else {
            if (!ga_attack(a,e,0x8af82)) {
                SB(0x80,neo_angle((int16_t)ASR(L(0x30),12),(int16_t)ASR(L(0x34),12))>>3);  /* 08AF5A: 002EB4, lsr.b #3 */
                if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
                uint8_t t=B(0x81);SB(0x81,t-0x38u);
                if (t<0x38u) child(a,e,0x8ae70,0x1c,0,0);
                return e->unsupported?GA_UNSUPPORTED:GA_MORE;
            }
        }
        /* 08AE46 */
        sound(e,0x1022);SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);
        SL(0,0x1c706);
        return neo_effects_tick(a,e);                                   /* jmp 01C706 */
    case 0x8ae70:k1b1dc(a);goto ae76;
    case 0x8ae76:                                                        /* the box 0391EA's smoke keeps its speeds */
    ae76:
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x37c);
        START(0x285bbe);SL(0,0x8aea6);
        /* fall through */
    case 0x8aea6:
        ga_move(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        if (ga_cull(a,e,0x8af92)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- shield soldiers 09AE1E / 09AE2C / 09BA2A --------------------------------- */
static uint16_t absw(uint16_t d) {return (d&0x8000u)?(uint16_t)(0u-d):d;}
/* 01B08A: carry when the player is behind. */
static bool behind(neo_gactor_t *a,const neo_genv_t *e,uint32_t node) {
    int16_t d0=S16((uint16_t)(W(0x3c)-px(e,node)));
    return BIT(0x15,0)?d0>0:d0<0;
}
static neo_gactor_t *sh_child(neo_gactor_t *a,neo_genv_t *e) {return e->actor?e->actor(e->ctx,L(0x72)):NULL;}
static neo_gactor_t *sh_creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static void solid(neo_gactor_t *a,neo_genv_t *e,uint32_t desc) {if (e->solid) e->solid(e->ctx,a,desc);}   /* 004E76 */
static void dyn_release(neo_gactor_t *a,neo_genv_t *e) {if (e->dyn_release) e->dyn_release(e->ctx,a);}   /* 013F1E */
/* 01AD52: re-parent the position block (+28..+37) to the scene root
 * 10E968 (01AC4C), then project (01ABC8); as neo_heli.c. */
static void to_root(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t tag=L(0x38);
    if (tag!=0x10e968u) {
        neo_contact_parent_t op,np;bool has_old=tag && ga_parent(e,tag,&op),has_new=ga_parent(e,0x10e968u,&np);
        uint32_t d0=L(0x28),d1=L(0x2c),d2=L(0x30),d3=L(0x34);
        d2+=d0;d3+=d1;
        if (has_old) {d0+=(uint32_t)op.previous_x;d1+=(uint32_t)op.previous_y;d2+=(uint32_t)op.x;d3+=(uint32_t)op.y;}
        if (has_new) {d0-=(uint32_t)np.previous_x;d1-=(uint32_t)np.previous_y;d2-=(uint32_t)np.x;d3-=(uint32_t)np.y;}
        d2-=d0;d3-=d1;
        SL(0x28,d0);SL(0x2c,d1);SL(0x30,d2);SL(0x34,d3);SL(0x38,0x10e968u);
    }
    ga_project(a,e);
}
/* 01B206: carry for the hit kinds of table 01B1F2. */
static bool flinch(neo_gactor_t *a,neo_genv_t *e) {
    static const uint8_t t[20]={0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0};
    if (B(0x5a)>=20u) {ga_fail(e,0x1b206);return false;}
    return t[B(0x5a)]!=0;
}
/* 09CB70 */
static void k9cb70(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x90,0);
    uint16_t d0=W(0x6e);
    if (S16(d0)>=2) d0=ga_rand(e);
    if (d0&1u) SET(0x90,5); else CLR(0x90,5);
    SW(0x7e,W(0x70));
    if (W(0x70)) SET(0x90,0);
    if (W(0x72)!=0xffffu) {SW(0x80,W(0x72));SW(0x82,W(0x74));SET(0x90,6);}
    SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);
}
/* 09D310: the body hurtbox by +90 bit 4. */
static void k9d310(neo_gactor_t *a) {SL(0x50,BIT(0x90,4)?0x9d636:0x9d62a);}
/* 09CBE2 */
static bool k9cbe2(neo_gactor_t *a,neo_genv_t *e) {
    ga_floor(a,e);k1b1dc(a);ga_air(a,e);
    if (S16(W(0x3c))<0x100) SET(0x15,0);
    if (!BIT(0x90,6)) {ga_palette(a,0x238);ga_flash_palette(a,0x38c);}
    else {ga_palette(a,0x3c7);ga_flash_palette(a,0x171);}
    SW(0x66,0x8820);SW(0x1e,0x8820);SB(0x8e,0);SW(0x4e,1);k9d310(a);
    neo_gactor_t *c=child(a,e,0x9c46c,0x0e,0,0);
    if (c) {ga_sb(c,0x90,B(0x90));SL(0x72,NEO_GA_TAG(c->index));}
    SW(0x7a,rank(a,e,0x2d043c));
    return neo_gate_ripple(a,e,2);                                   /* 03C52C */
}
/* 09CCA0: the jump arc; carry on the ground. */
static bool k9cca0(neo_gactor_t *a,neo_genv_t *e) {ga_air(a,e);SL(0x34,L(0x34)+L(0x6e));return BIT(0x15,6);}
/* 09CCC6 */
static void k9ccc6(neo_gactor_t *a,neo_genv_t *e) {
    SL(0x30,0);SL(0x34,0);ga_air(a,e);
    if (BIT(0x15,6)) ga_ground(a,e,NULL);
    else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);if (BIT(0x15,6)) {SL(0x30,0);SL(0x34,0);}}
}
/* 09CD02 */
static void k9cd02(neo_gactor_t *a,neo_genv_t *e) {
    ga_air(a,e);
    if (BIT(0x15,6)) SL(0x34,0);
    else {SL(0x34,L(0x34)+0xffffb000u);if (BIT(0x15,6)) SL(0x34,0);}
    SL(0x30,0);
}
/* 09CD36: walking speed and +26 from rank 2D0244. */
static void k9cd36(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=rank(a,e,0x2d0244),d1=d0;
    uint32_t v=(uint32_t)(int32_t)S16(d0);
    if (!BIT(0x15,0)) v=0u-v;
    SL(0x62,v<<8);
    d1=(uint16_t)((d1&0x700u)>>1);
    if (!d1) d1=0x80;
    SW(0x26,d1);
}
/* 09CD6E: the bash rows while +8F. */
static void k9cd6e(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x8f)) return;
    uint32_t row=B(0x8f)==1u?(BIT(0x90,4)?0x9d762:0x9d752):(BIT(0x90,4)?0x9d782:0x9d772);
    ga_attack(a,e,row);
}
/* 09CDB8: carry on a hit (+86 = +5A) or with the shield hit (+17 from it). */
static bool k9cdb8(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) {SB(0x86,B(0x5a));return true;}
    if (!BIT(0x90,1)) return false;
    neo_gactor_t *c=sh_child(a,e);
    if (c) SB(0x17,ga_b(c,0x17));
    return true;
}
/* 09CDF2: carry (and +90 bit 1) when the shield was hit, unless +8E. */
static bool k9cdf2(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=B(0x8e)?NULL:sh_child(a,e);
    if (c && (ga_b(c,0x90)&2u)) {SET(0x90,1);return true;}
    CLR(0x90,1);return false;
}
/* 09CE20: the shield as a solid body while it has a hurtbox. */
static void k9ce20(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x90,4)) return;
    neo_gactor_t *c=sh_child(a,e);
    solid(a,e,c && ga_l(c,0x50)?0x9d792:0);
}
static void k9ce50(neo_gactor_t *a,neo_genv_t *e) {SB(0x1d,0);ga_palette(a,e->blood?0x39f:0x3a3);}   /* 009F88, 01B2BE */
static int16_t k9ce82(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {return S16(absw((uint16_t)(W(0x3c)-px(e,n))));}
/* 09CE96: carry when +90 bit 2 and the player is out of -30..40 in y. */
static bool k9ce96(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {
    if (!BIT(0x90,2)) return false;
    int16_t d0=S16((uint16_t)(py(e,n)-W(0x40)));
    return d0>0x40 || d0<-0x30;
}
static bool k9cec4(neo_gactor_t *a) {
    int16_t x=S16(W(0x3c));
    if (x<=0x20 && BIT(0x15,0)) return true;
    return x>=0x120 && !BIT(0x15,0);
}
static bool k9cef8(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;if (nearest(a,e,&n)) return true;
    if (k9ce96(a,e,n)) return k9ce82(a,e,n)<=4;
    return k9ce82(a,e,n)<=S16(W(0x7c));
}
static bool k9cf32(neo_gactor_t *a) {if (!BIT(0x5f,0)) return false;SB(0x8b,0x1e);return true;}
static bool k9cf50(neo_gactor_t *a,neo_genv_t *e) {
    if (k9cec4(a)) return true;
    uint32_t n;if (nearest(a,e,&n)) return true;
    if (BIT(0x5f,0)) return true;
    if (k9ce96(a,e,n)) return true;
    return k9ce82(a,e,n)>S16(W(0x7c));
}
static bool k9cf8c(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;if (nearest(a,e,&n)) return false;
    if (k9ce96(a,e,n)) return k9ce82(a,e,n)>4;
    return (int16_t)(k9ce82(a,e,n)-0x10)>S16(W(0x7c));
}
static bool k9cfca(neo_gactor_t *a,neo_genv_t *e) {
    if (k9cec4(a)) return false;
    uint32_t n;if (nearest(a,e,&n)) return false;
    if (k9ce96(a,e,n)) return false;
    return !((int16_t)(k9ce82(a,e,n)+8)>S16(W(0x7c)));
}
/* 09CFFE: every 15 updates, carry when a player is behind (09D094: the one
 * of two that is 30 below or more and 20 away). +76 keeps a player node. */
static bool k9cffe(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x89,B(0x89)+1u);
    if ((int8_t)B(0x89)<0x0f) return false;
    SB(0x89,0x0f);
    bool l1=e->players.active_mask&1u,l2=(e->players.active_mask>>1)&1u;
    uint32_t a0;
    if (l1) {
        SL(0x76,P1_NODE);
        if (!l2) return behind(a,e,P1_NODE);
        /* 09D062: both live. */
        if (behind(a,e,P2_NODE)) {SL(0x76,P2_NODE);if (behind(a,e,P1_NODE)) return true;a0=P1_NODE;}
        else {SL(0x76,P2_NODE);if (!behind(a,e,P1_NODE)) return false;a0=P1_NODE;}
        /* 09D094 (A1 is overwritten by A0: both tests read the same node). */
        if (S16((uint16_t)(py(e,a0)-W(0x40)))>-0x30) return false;
        if (k9ce82(a,e,a0)<=0x20) return false;
        return behind(a,e,a0);
    }
    if (!l2) return false;
    return behind(a,e,P2_NODE);
}
/* 09D140: carry after 6 updates in the air. */
static bool k9d140(neo_gactor_t *a) {
    if (BIT(0x15,6)) {SB(0x88,0);return false;}
    SB(0x88,B(0x88)+1u);
    if ((int8_t)B(0x88)>6) {SB(0x88,6);return true;}
    return false;
}
/* 09D178: carry when on screen (x 18..128) and a player lives. */
static bool k9d178(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x=S16(W(0x3c));
    if (x<0x18 || x>0x128) return false;
    uint32_t n;return !nearest(a,e,&n);
}
/* 01AFFC with its A0: the nearest player inside the rectangle. */
static bool pick(neo_gactor_t *a,neo_genv_t *e,uint32_t rect,uint16_t *x) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return false;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    neo_target_t t=neo_target_pick(&c,&e->players,q);
    if (t.valid) *x=t.x;
    return t.valid;
}
static bool k9d1a6(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x90,5) || S16(W(0x7a))>0) return false;
    uint16_t x;if (!pick(a,e,0x9d7a0,&x)) return false;
    return !((int16_t)(S16(absw((uint16_t)(W(0x3c)-x)))-0x10)>S16(W(0x7c)));
}
static bool k9d19e(neo_gactor_t *a,neo_genv_t *e) {return k9d178(a,e) && k9d1a6(a,e);}
static bool k9d1e4(neo_gactor_t *a,neo_genv_t *e) {
    if (!k9d178(a,e) || BIT(0x90,5) || S16(W(0x7a))>0) return false;
    return !ga_players_in(a,e,0x9d7a0);
}
static bool k9d20c(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x90,2)) return false;
    if (!ga_players_in(a,e,0x9d7b0)) {SET(0x90,3);return true;}
    if (!ga_players_in(a,e,0x9d7b8)) {CLR(0x90,3);return true;}
    return false;
}
static bool k9d250(neo_gactor_t *a,neo_genv_t *e) {return !ga_players_in(a,e,BIT(0x90,3)?0x9d7b0:0x9d7b8);}
static bool k9d272(neo_gactor_t *a,neo_genv_t *e) {
    if (!k9d178(a,e)) return false;
    if (S16(W(0x7a))<=0) return k9d250(a,e);
    return false;
}
static bool k9d288(neo_gactor_t *a,neo_genv_t *e) {return k9d178(a,e) && S16(W(0x7a))<=0;}
static bool k9d2b0(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x90,5) || S16(W(0x7a))>0) return false;
    return !ga_players_in(a,e,0x9d7a8);
}
static bool k9d2a8(neo_gactor_t *a,neo_genv_t *e) {return k9d178(a,e) && k9d2b0(a,e);}
static bool k9d2de(neo_gactor_t *a) {
    int16_t x=S16(W(0x3c));
    if (x<0x128 && x>0x18) return false;
    SB(0x8c,B(0x8c)+1u);
    if ((int8_t)B(0x8c)<0x1e) return false;
    SB(0x8a,6);return true;
}
/* 09D470: carry = remove (outside -40..1C0 or 01945C with 09D7C8). */
static bool k9d470(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x=S16(W(0x3c));
    if (x<-0x40 || x>0x1c0) return true;
    return ga_cull(a,e,0x9d7c8);
}
/* 09D496: +90 bit 4 asks 044930 (not ported: explicit stop). */
static bool k9d496(neo_gactor_t *a,neo_genv_t *e) {if (BIT(0x90,4)) ga_fail(e,0x44930);return false;}
/* 09D5BA / 09D5EE: the item +80/+82 once (08CDB2 at (0, 4) / (0, 10)). */
static void k9d5ba(neo_gactor_t *a,neo_genv_t *e,int16_t dy,bool rise) {
    if (!BIT(0x90,6) || BIT(0x90,7)) return;
    if (W(0x80)<0x5au) {
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
        if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x82));ga_sw(c,0x6e,W(0x80));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,dy);
            if (rise) ga_sl(c,0x34,0x80000);}
    }
    SET(0x90,7);
}
/* 09D4AE / 09D508 / 09D562: sound 10FF, the flash (D0 = 2A) and the shot
 * (D0 = 1C) with +90. */
static void shot(neo_gactor_t *a,neo_genv_t *e,uint32_t flash,uint32_t bullet,int16_t dx,int16_t dy) {
    sound(e,0x10ff);
    child(a,e,flash,0x2a,0,0);
    neo_gactor_t *c=child(a,e,bullet,0x1c,dx,dy);
    if (c) ga_sb(c,0x90,B(0x90));
}
static bool sh_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    static const uint32_t box[][3]={{0x9d310,0x9d62a,0x9d636},{0x9d330,0x9d65a,0x9d666},{0x9d350,0x9d68a,0x9d696},{0x9d370,0x9d6a2,0x9d6ae},
        {0x9d390,0x9d6d2,0x9d6de},{0x9d3b0,0x9d6ea,0x9d6f6},{0x9d3d0,0x9d71a,0x9d726},{0x9d3f0,0x9d642,0x9d64e},{0x9d410,0x9d672,0x9d67e},
        {0x9d430,0x9d6ba,0x9d6c6},{0x9d450,0x9d702,0x9d70e}};
    for (unsigned i=0;i<sizeof box/sizeof box[0];i++) if (box[i][0]==routine) {SL(0x50,box[i][BIT(0x90,4)?2:1]);return true;}
    switch (routine) {
    case 0x9d4ae:shot(a,e,0x9cae8,0x9c744,-0x1e,0x1c);return true;
    case 0x9d508:shot(a,e,0x9cafc,0x9c76c,-0x02,0x33);return true;
    case 0x9d562:shot(a,e,0x9cb10,0x9c780,-0x05,0);return true;
    }
    return false;
}
/* 9CDF2, 9CE20, 9CD02, 00427A: the common step. */
static int sh_std(neo_gactor_t *a,neo_genv_t *e) {k9cdf2(a,e);k9ce20(a,e);k9cd02(a,e);return ga_anim_step(a,e,sh_call);}
/* 09B538 */
static ga_result_t k9b538(neo_gactor_t *a,neo_genv_t *e) {solid(a,e,0);dyn_release(a,e);SL(0,0x9b550);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
/* 09B47A */
static ga_result_t t47a(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x7a,W(0x7a)-1u);
    if (k9cdb8(a,e)) SB(0x5a,0);
    if (BIT(0x5f,7)) return k9b538(a,e);
    if (k9d470(a,e)) SL(0,0x9c3d4);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 09B6E6 / 09BC30 (+90 bit 3): the raised shield's collision map by the facing. */
static void raised(neo_gactor_t *a,neo_genv_t *e,uint32_t self) {
    if (L(0)!=self) {dyn_release(a,e);return;}
    if (BIT(0x90,3) && e->dyn_set) e->dyn_set(e->ctx,a,BIT(0x15,0)?0x9d7f6:0x9d7e8,0,0);
}
#define SSTART(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STD() do { if ((r=sh_std(a,e))<0) return GA_UNSUPPORTED; } while (0)
#define ANIM() do { if ((r=ga_anim_step(a,e,sh_call))<0) return GA_UNSUPPORTED; } while (0)
#define B3(x,y) (BIT(0x90,3)?(x):(y))
static ga_result_t shield_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    for (unsigned guard=0;guard<8;guard++) switch (L(0)) {
    /* ---- constructors ---- */
    case 0x9ae1e:k9cb70(a,e);SET(0x90,2);goto ae30;
    case 0x9ae2c:k9cb70(a,e);
    ae30:
        if (!k9cbe2(a,e)) return GA_UNSUPPORTED;
        SW(0x7c,rank(a,e,0x2d02de));
        if (BIT(0x90,5)) SW(0x7c,0x34);
        k1b1dc(a);ga_air(a,e);
        if (B(0x6c)==3u) {SL(0,0x9af78);continue;}                    /* 01B284 */
        SL(0,BIT(0x90,0)?0x9b044:0x9ae74);continue;
    case 0x9ba2a:k9cb70(a,e);goto ba;
    case 0x9ba40:k9cb70(a,e);SET(0x90,4);SET(0x90,2);SET(0x90,3);goto ba;
    case 0x9ba68:k9cb70(a,e);SET(0x90,2);SET(0x90,3);goto ba;
    case 0x9ba8a:k9cb70(a,e);SET(0x90,2);
    ba:
        if (!k9cbe2(a,e)) return GA_UNSUPPORTED;
        SL(0,BIT(0x90,0)?0x9bdaa:0x9baa2);continue;
    /* ---- 09AE2C states ---- */
    case 0x9ae74:SB(0x8c,0);k1b1dc(a);SW(0x26,0x100);SSTART(0x2a037a);SL(0,0x9ae9a);
        /* fall through */
    case 0x9ae9a:
        STD();
        if (BIT(0x90,0)) {
            if ((int8_t)B(0x8b)>0) SB(0x8b,B(0x8b)-1u);
            else {
                if (k9d2de(a)) SL(0,0x9b044);
                if (k9cfca(a,e)) SL(0,0x9b13a);
                if (k9cf8c(a,e)) SL(0,0x9b044);
            }
        }
        if (k9cffe(a,e)) SL(0,0x9afac);
        if (k9d20c(a,e)) SL(0,0x9b61c);
        if (k9d19e(a,e)) SL(0,0x9b1b4);
        if (k9d2a8(a,e)) SL(0,0x9b2ec);
        if (k9d140(a)) SL(0,0x9b000);
        SW(0x7a,W(0x7a)-1u);
        if (k9cdb8(a,e)) {
            if (BIT(0x5f,7)) return k9b538(a,e);
            SB(0x5a,0);SL(0,0x9b40e);
            if (flinch(a,e)) SL(0,0x9b42e);
        }
        if (k9d470(a,e)) SL(0,0x9c3d4);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9af78:SSTART(0x2a037a);SL(0,0x9af8e);
        /* fall through */
    case 0x9af8e:
        k9cd02(a,e);
        if (B(0x6c)!=3u) SL(0,0x9ae74);
        ANIM();return t47a(a,e);
    case 0x9afac:SB(0x89,0);SW(0x26,0x100);k1b1dc(a);SSTART(0x2a04e2);SL(0,0x9afd2);
        /* fall through */
    case 0x9afd2:STD();if (r) SL(0,0x9ae74);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b000:SL(0x6e,0xffff8000u);SW(0x26,0x100);SSTART(0x2a0562);SL(0,0x9b024);
        /* fall through */
    case 0x9b024:k9cdf2(a,e);k9ce20(a,e);if (k9cca0(a,e)) SL(0,0x9ae74);ANIM();return t47a(a,e);
    case 0x9b044:SSTART(0x2a03c6);k9cd36(a,e);SL(0,0x9b05e);
        /* fall through */
    case 0x9b05e:
        k9cdf2(a,e);k9ce20(a,e);k9ccc6(a,e);ANIM();
        if ((int8_t)B(0x8a)>0) SB(0x8a,B(0x8a)-1u);
        else {
            if (k9d1a6(a,e)) SL(0,0x9b0da);
            if (k9d2b0(a,e)) SL(0,0x9b0da);
            if (k9cef8(a,e)) SL(0,0x9b0da);
            if (k9cf32(a)) SL(0,0x9b0da);
            if (k9cffe(a,e)) SL(0,0x9b0da);
        }
        if (k9d140(a)) SL(0,0x9b000);
        return t47a(a,e);
    case 0x9b0da:k1b1dc(a);SW(0x26,0x100);SSTART(0x2a044e);SL(0,0x9b0fc);
        /* fall through */
    case 0x9b0fc:
        k9cd02(a,e);ANIM();
        if (r) {if (B(0x8b)==0x1eu) {SB(0x8b,0x1d);SL(0,0x9afac);} else SL(0,0x9ae74);}
        if (k9d140(a)) SL(0,0x9b000);
        return t47a(a,e);
    case 0x9b13a:SSTART(0x2a048c);k9cd36(a,e);SL(0x62,0u-L(0x62));SL(0,0x9b158);
        /* fall through */
    case 0x9b158:
        k9cdf2(a,e);k9ce20(a,e);k9ccc6(a,e);ANIM();
        if (k9cf50(a,e)) SL(0,0x9ae74);
        if (k9d19e(a,e)) SL(0,0x9b1b4);
        if (k9d2a8(a,e)) SL(0,0x9b2ec);
        if (k9cffe(a,e)) SL(0,0x9afac);
        if (k9d140(a)) SL(0,0x9b000);
        return t47a(a,e);
    case 0x9b1b4:k1b1dc(a);SW(0x26,0x100);k1b1dc(a);SB(0x87,rank(a,e,0x2d03f2));SSTART(0x2a05c8);SL(0,0x9b1ec);
        /* fall through */
    case 0x9b1ec:STD();if (r) SL(0,0x9b21a);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b21a:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d049e));SSTART(0x2a0654);SL(0,0x9b246);
        /* fall through */
    case 0x9b246:
        STD();
        if (r && S16(W(0x7a))<=0) {SL(0,0x9b21a);SB(0x87,B(0x87)-1u);if ((int8_t)B(0x87)<=0) SL(0,0x9b292);}
        if (k9d140(a)) SL(0,0x9b000);
        return t47a(a,e);
    case 0x9b292:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d043c));SSTART(0x2a069e);SL(0,0x9b2be);
        /* fall through */
    case 0x9b2be:STD();if (r) SL(0,0x9ae74);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b2ec:k1b1dc(a);SW(0x26,0x100);SB(0x87,1);SSTART(0x2a0afc);SL(0,0x9b314);
        /* fall through */
    case 0x9b314:STD();if (r) SL(0,0x9b342);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b342:SW(0x26,0x100);SW(0x7a,0x0a);SSTART(0x2a0b64);SL(0,0x9b364);
        /* fall through */
    case 0x9b364:
        STD();
        if (r && S16(W(0x7a))<=0) {SL(0,0x9b342);SB(0x87,B(0x87)-1u);if ((int8_t)B(0x87)<=0) SL(0,0x9b3b4);}
        k9cd6e(a,e);
        if (k9d140(a)) SL(0,0x9b000);
        return t47a(a,e);
    case 0x9b3b4:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d04e8));SSTART(0x2a0cc8);SL(0,0x9b3e0);
        /* fall through */
    case 0x9b3e0:STD();if (r) SL(0,0x9ae74);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b40e:k1b1dc(a);SW(0x26,0x100);SSTART(0x2a0e04);SL(0,0x9b450);continue;
    case 0x9b42e:k1b1dc(a);SW(0x26,0x100);SSTART(0x2a0d20);SL(0,0x9b450);
        /* fall through */
    case 0x9b450:STD();if (r) SL(0,0x9ae74);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    /* ---- the shield broken (set by 09C46C) ---- */
    case 0x9b4a4:
        k1b1dc(a);dyn_release(a,e);SW(0x26,0x100);
        if (BIT(0x90,6)) k9d5ba(a,e,0x10,true);
        SSTART(0x23de6a);solid(a,e,0);SL(0,0x9b4e4);
        /* fall through */
    case 0x9b4e4:k9cd02(a,e);ANIM();if (r) SL(0,0x9b500);SL(0x50,0);return t47a(a,e);
    case 0x9b500:
        if (BIT(0x90,4)) {ga_fail(e,0x4611c);return GA_UNSUPPORTED;}   /* jmp 04611C */
        CLR(0x5f,7);SB(0x5a,0);SB(0x17,0xff);SL(0x50,0x9d62a);solid(a,e,0);
        /* jmp 0B0626: the node becomes a soldier (the world moves it). */
        if (!e->become_soldier || !e->become_soldier(e->ctx,a,0xb0626)) {ga_fail(e,0xb0626);return GA_UNSUPPORTED;}
        return GA_MORE;
    /* ---- death ---- */
    case 0x9b550:
        if (BIT(0x90,6)) {k9d5ba(a,e,4,false);k9ce50(a,e);SL(0,0x9b59a);continue;}
        SB(0x17,0xff);SB(0x5a,B(0x86));
        if (BIT(0x90,4) && B(0x5a)) SB(0x73,0xff);                     /* 0407B6 */
        {int d=dispatch(a,e,false);
         if (d!=D_MORE) return result(a,e,d);}
        if (L(0)==0x9b550) {ga_fail(e,0x9b592);return GA_UNSUPPORTED;}   /* jmp (a6) on itself */
        return death_tick(a,e);                                          /* 09B592: jmp (a6) */
    case 0x9b59a:
        child(a,e,0x9b5fc,0x1c,0,0x18);
        SSTART(0x2a0ffa);SL(0,0x9b5d6);
        /* fall through */
    case 0x9b5d6:
        ga_project(a,e);ANIM();
        if (r) SL(0,0x9c3d4);
        if (k9d470(a,e)) SL(0,0x9c3d4);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9b5fc:ga_palette(a,0x20d);SSTART(0x2a1000);SL(0,0x9b5d6);continue;
    /* ---- the raised shield (09AE2C) ---- */
    case 0x9b61c:k1b1dc(a);SSTART(B3(0x2a0714,0x2a076c));SW(0x26,0x100);SL(0,0x9b65c);
        /* fall through */
    case 0x9b65c:STD();if (r) SL(0,0x9b68a);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b68a: {
        neo_gactor_t *c=child(a,e,0x9c6b0,0x0e,0,0);
        if (c) ga_sb(c,0x90,B(0x90));
        SSTART(B3(0x2a07c4,0x2a0852));SL(0,0x9b6e6);
    }   /* fall through */
    case 0x9b6e6:
        STD();
        if (!k9d250(a,e)) SL(0,0x9b79c);
        if (k9d272(a,e)) SL(0,0x9b804);
        if (k9d140(a)) SL(0,0x9b000);
        SW(0x7a,W(0x7a)-1u);
        if (k9cdb8(a,e)) {SB(0x5a,0);SL(0,0x9b98a);if (flinch(a,e)) SL(0,0x9b9c2);}
        if (BIT(0x5f,7)) return k9b538(a,e);
        if (k9d470(a,e)) SL(0,0x9c3d4);
        raised(a,e,0x9b6e6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9b79c:SSTART(B3(0x2a08e0,0x2a0938));SL(0,0x9b7d0);
        /* fall through */
    case 0x9b7d0:
        k9cdf2(a,e);k9ce20(a,e);k9cd02(a,e);ANIM();ANIM();
        if (r) SL(0,0x9ae74);
        if (k9d140(a)) SL(0,0x9b000);
        return t47a(a,e);
    case 0x9b804:SW(0x26,0x100);SB(0x87,rank(a,e,0x2d03f2));SSTART(B3(0x2a0990,0x2a09c4));SL(0,0x9b84e);
        /* fall through */
    case 0x9b84e:STD();if (r) SL(0,0x9b87c);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b87c:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d049e));SSTART(B3(0x2a09f8,0x2a0a46));SL(0,0x9b8c6);
        /* fall through */
    case 0x9b8c6:
        STD();
        if (r && S16(W(0x7a))<=0) {SL(0,0x9b87c);SB(0x87,B(0x87)-1u);if ((int8_t)B(0x87)<=0) SL(0,0x9b912);}
        if (k9d140(a)) SL(0,0x9b000);
        return t47a(a,e);
    case 0x9b912:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d043c));SSTART(B3(0x2a0a94,0x2a0ac8));SL(0,0x9b95c);
        /* fall through */
    case 0x9b95c:STD();if (r) SL(0,0x9b68a);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    case 0x9b98a:SW(0x26,0x100);SSTART(B3(0x2a0e24,0x2a0e4e));SL(0,0x9b9fc);continue;
    case 0x9b9c2:SW(0x26,0x100);SSTART(B3(0x2a0d80,0x2a0dc2));SL(0,0x9b9fc);
        /* fall through */
    case 0x9b9fc:STD();if (r) SL(0,0x9b68a);if (k9d140(a)) SL(0,0x9b000);return t47a(a,e);
    /* ---- 09BA2A states ---- */
    case 0x9baa2:SW(0x26,0x100);SSTART(0x2a037a);SL(0,0x9babe);
        /* fall through */
    case 0x9babe:
        STD();
        if (k9d1e4(a,e)) SL(0,0x9be6c);
        if (k9d2a8(a,e)) SL(0,0x9c11e);
        if (BIT(0x90,2)) SL(0,0x9bb66);
        if (k9cffe(a,e)) SL(0,0x9bd5c);
        if (k9d140(a)) SL(0,0x9c23a);
        if (k9d496(a,e)) SL(0,0x9c292);
        SW(0x7a,W(0x7a)-1u);
        if (k9cdb8(a,e)) {SB(0x5a,0);SL(0,0x9c370);if (flinch(a,e)) SL(0,0x9c38a);}
        if (BIT(0x5f,7)) return k9b538(a,e);
        if (k9d470(a,e)) SL(0,0x9c3d4);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9bb66:k1b1dc(a);SSTART(B3(0x2a0714,0x2a076c));SW(0x26,0x100);SL(0,0x9bba6);
        /* fall through */
    case 0x9bba6:STD();if (r) SL(0,0x9bbd4);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9bbd4: {
        neo_gactor_t *c=child(a,e,0x9c6b0,0x0e,0,0);
        if (c) ga_sb(c,0x90,B(0x90));
        SSTART(B3(0x2a07c4,0x2a0852));SL(0,0x9bc30);
    }   /* fall through */
    case 0x9bc30:
        STD();
        if (k9d2a8(a,e)) SL(0,0x9bcf4);
        if (k9d288(a,e)) SL(0,0x9bf98);
        if (k9d140(a)) SL(0,0x9c23a);
        if (k9d496(a,e)) SL(0,0x9c292);
        SW(0x7a,W(0x7a)-1u);
        if (k9cdb8(a,e)) {SB(0x5a,0);SL(0,0x9c2d0);if (flinch(a,e)) SL(0,0x9bcf4);}
        if (BIT(0x5f,7)) return k9b538(a,e);
        if (k9d470(a,e)) SL(0,0x9c3d4);
        raised(a,e,0x9bc30);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9bcf4:SSTART(B3(0x2a08e0,0x2a0938));SL(0,0x9bd28);
        /* fall through */
    case 0x9bd28:
        k9cdf2(a,e);k9ce20(a,e);k9cd02(a,e);ANIM();ANIM();
        if (r) SL(0,0x9baa2);
        if (k9d140(a)) SL(0,0x9c23a);
        return t47a(a,e);
    case 0x9bd5c:SB(0x89,0);SW(0x26,0x100);SSTART(0x2a04e2);SL(0,0x9bd7c);
        /* fall through */
    case 0x9bd7c:STD();if (r) SL(0,0x9baa2);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9bdaa:k9cd36(a,e);SSTART(0x2a03c6);SL(0,0x9bdc4);
        /* fall through */
    case 0x9bdc4: {
        k9cdf2(a,e);k9ce20(a,e);k9ccc6(a,e);ANIM();
        /* 01AD7C: + the scene root's +14 word (RAM 10E97C). */
        neo_contact_parent_t root;int16_t d0=S16(W(0x7e));
        if (ga_parent(e,0x10e968u,&root)) d0=(int16_t)(d0+(int16_t)((uint32_t)root.x>>16));
        if (!BIT(0x15,0)) {if (d0>=S16(W(0x3c))) SL(0,0x9be1e);}
        else if (d0<=S16(W(0x3c))) SL(0,0x9be1e);
        if (k9d140(a)) SL(0,0x9c23a);
        return t47a(a,e);
    }
    case 0x9be1e:k1b1dc(a);CLR(0x90,0);SW(0x26,0x100);SSTART(0x2a044e);SL(0,0x9be46);
        /* fall through */
    case 0x9be46:k9cd02(a,e);ANIM();if (r) SL(0,0x9baa2);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9be6c:SW(0x26,0x100);SB(0x87,rank(a,e,0x2d03f2));SSTART(0x2a05c8);SL(0,0x9be98);
        /* fall through */
    case 0x9be98:STD();if (r) SL(0,0x9bec6);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9bec6:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d049e));SSTART(0x2a0654);SL(0,0x9bef2);
        /* fall through */
    case 0x9bef2:
        STD();
        if (r && S16(W(0x7a))<=0) {SL(0,0x9bec6);SB(0x87,B(0x87)-1u);if ((int8_t)B(0x87)<=0) SL(0,0x9bf3e);}
        if (k9d140(a)) SL(0,0x9c23a);
        return t47a(a,e);
    case 0x9bf3e:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d043c));SSTART(0x2a069e);SL(0,0x9bf6a);
        /* fall through */
    case 0x9bf6a:STD();if (r) SL(0,0x9baa2);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9bf98:SW(0x26,0x100);SB(0x87,rank(a,e,0x2d03f2));SSTART(B3(0x2a0990,0x2a09c4));SL(0,0x9bfe2);
        /* fall through */
    case 0x9bfe2:STD();if (r) SL(0,0x9c010);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9c010:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d049e));SSTART(B3(0x2a09f8,0x2a0a46));SL(0,0x9c05a);
        /* fall through */
    case 0x9c05a:
        STD();
        if (r && S16(W(0x7a))<=0) {SL(0,0x9c010);SB(0x87,B(0x87)-1u);if ((int8_t)B(0x87)<=0) SL(0,0x9c0a6);}
        if (k9d140(a)) SL(0,0x9c23a);
        return t47a(a,e);
    case 0x9c0a6:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d043c));SSTART(B3(0x2a0a94,0x2a0ac8));SL(0,0x9c0f0);
        /* fall through */
    case 0x9c0f0:STD();if (r) SL(0,0x9bbd4);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9c11e:SW(0x26,0x100);SB(0x87,1);SSTART(0x2a0afc);SL(0,0x9c140);
        /* fall through */
    case 0x9c140:STD();if (r) SL(0,0x9c16e);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9c16e:SW(0x26,0x100);SW(0x7a,0x0a);SSTART(0x2a0b64);SL(0,0x9c190);
        /* fall through */
    case 0x9c190:
        STD();
        if (r && S16(W(0x7a))<=0) {SL(0,0x9c16e);SB(0x87,B(0x87)-1u);if ((int8_t)B(0x87)<=0) SL(0,0x9c1e0);}
        k9cd6e(a,e);
        if (k9d140(a)) SL(0,0x9c23a);
        return t47a(a,e);
    case 0x9c1e0:SW(0x26,0x100);SW(0x7a,rank(a,e,0x2d04e8));SSTART(0x2a0cc8);SL(0,0x9c20c);
        /* fall through */
    case 0x9c20c:STD();if (r) SL(0,0x9baa2);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9c23a:SL(0x6e,0xffff8000u);SW(0x26,0x100);SSTART(0x2a0562);SL(0,0x9c25e);
        /* fall through */
    case 0x9c25e:
        k9cdf2(a,e);k9ce20(a,e);
        if (k9cca0(a,e)) SL(0,BIT(0x90,0)?0x9bdaa:0x9baa2);
        ANIM();return t47a(a,e);
    case 0x9c292:k9cd36(a,e);SSTART(0x2a03c6);SL(0,0x9c2ac);
        /* fall through */
    case 0x9c2ac:k9cdf2(a,e);k9ce20(a,e);k9ccc6(a,e);ANIM();if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9c2d0:SW(0x26,0x100);SSTART(B3(0x2a0e24,0x2a0e4e));SL(0,0x9c342);continue;
    case 0x9c308:SW(0x26,0x100);SSTART(B3(0x2a0d80,0x2a0dc2));SL(0,0x9c342);
        /* fall through */
    case 0x9c342:STD();if (r) SL(0,0x9bbd4);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9c370:SW(0x26,0x100);SSTART(0x2a0e04);SL(0,0x9c3a6);continue;
    case 0x9c38a:SW(0x26,0x100);SSTART(0x2a0d20);SL(0,0x9c3a6);
        /* fall through */
    case 0x9c3a6:STD();if (r) SL(0,0x9baa2);if (k9d140(a)) SL(0,0x9c23a);return t47a(a,e);
    case 0x9c3d4:return GA_DELETE;                                      /* 004188 (after the cartridge check) */
    case 0x9c464:return GA_DELETE_TREE;                                 /* 0041AC */
    /* ---- the shield 09C46C ---- */
    case 0x9c46c: {
        if (!BIT(0x90,6)) {ga_palette(a,0x238);ga_flash_palette(a,0x38c);}
        else {ga_palette(a,0x3c7);ga_flash_palette(a,0x171);}
        SW(0x4e,rank(a,e,0x2d01e2));
        SL(0x50,BIT(0x90,4)?0x9d64e:0x9d642);                           /* 09D3F0 */
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);                      /* 01AD6E */
        SSTART(0x2a0eda);SL(0,0x9c4de);
    }   /* fall through */
    case 0x9c4de: {
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x9c4de);return GA_UNSUPPORTED;}
        uint8_t d0=ga_b(p,0x8d);
        if (d0!=B(0x8d)) {
            SB(0x8d,d0);
            static const uint32_t poses[14]={0x2a0eda,0x2a0ee2,0x2a061e,0x2a06e4,0x2a0b34,0x2a0c24,0x2a0cfa,0x2a0746,0x2a079e,0x2a0eea,0x2a0ef2,0x2a0912,0x2a096a,0x2a0efa};
            if ((d0&0xfu)>=14u) {ga_fail(e,0x9d804);return GA_UNSUPPORTED;}
            SSTART(poses[d0&0xfu]);
        }
        ga_project(a,e);
        SB(0x90,ga_b(p,0x90));SW(0x26,ga_w(p,0x26));
        if (ga_b(p,0x15)&1u) SET(0x15,0); else CLR(0x15,0);
        ANIM();
        if (ga_b(p,0x5f)&0x80u) {SL(0x50,0);SL(0,0x9c5ba);}
        else {
            if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SET(0x90,1);} else CLR(0x90,1);
            if (BIT(0x5f,7)) {
                ga_sl(p,0,0x9b4a4);
                to_root(a,e);                                            /* 01AD52 */
                SL(0x50,0);SL(0,0x9c5ba);
            }
        }
        if (!ga_creator_alive(a,e)) SL(0,0x9c464);                       /* 00409A */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x9c5ba:
        SB(0x5a,0);SB(0x17,0xff);SL(0x50,0);SW(0x26,0x100);
        SW(0x28,W(0x28)+0xfff8u);SW(0x2c,W(0x2c)+0x12u);
        SL(0x34,0x7ae14);SL(0x6e,0xffff62b7u);SL(0x30,0x3d70a);
        SSTART(0x2a0e78);SL(0,0x9c60c);
        /* fall through */
    case 0x9c60c:if (k9cca0(a,e)) SL(0,0x9c636);ANIM();goto c620;
    case 0x9c636:SL(0x34,0x99999);SL(0x6e,0xfffe147bu);SL(0x30,0x33333);SSTART(0x2a0e78);SL(0,0x9c664);
        /* fall through */
    case 0x9c664:if (k9cca0(a,e)) SL(0,0x9c67a);ANIM();goto c620;
    case 0x9c67a:SET(0x5f,3);SW(0x7a,0x1e);SL(0,0x9c68c);
        /* fall through */
    case 0x9c68c:
        ga_project(a,e);ANIM();
        SW(0x7a,W(0x7a)-1u);
        if (S16(W(0x7a))<=0) SL(0,0x9c464);
    c620:
        if (ga_cull_rect(a,e,0x9d7d0)) SL(0,0x9c464);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- the raised shield's overlay 09C6B0 ---- */
    case 0x9c6b0: {
        ga_palette(a,BIT(0x90,6)?0x3c7:0x238);
        SSTART(B3(0x2a0810,0x2a089e));
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);                      /* 01AD6E */
        SL(0,0x9c718);
    }   /* fall through */
    case 0x9c718: {
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x9c718);return GA_UNSUPPORTED;}
        SW(0x1e,ga_w(p,0x1e));
        if (ga_l(p,0)!=0x9b6e6u && ga_l(p,0)!=0x9bc30u) return GA_DELETE_TREE;   /* 09C464 */
        ga_project(a,e);ANIM();
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* ---- shots 09C744 / 09C76C / 09C780 ---- */
    case 0x9c744:SB(0x84,BIT(0x15,0)?0:0x80);SW(0x30,0x100);SW(0x34,0);goto c7aa;
    case 0x9c76c:SW(0x30,0);SW(0x34,0x100);SB(0x84,0x40);goto c7aa;
    case 0x9c780: {
        SB(0x84,0xc0);CLR(0x15,0);
        ga_players_in(a,e,0x9d7c0);                                     /* 01AFFC (then 01AF9A; A0 unused) */
        SW(0x30,0);SW(0x34,0xff00);
    }
    c7aa: {
        int16_t d0=S16(rank(a,e,0x2d0390)),d1=S16(W(0x30)),d2=S16(W(0x34));
        if (!BIT(0x15,0)) d1=(int16_t)-d1;
        SL(0x30,(uint32_t)((int32_t)d0*d1));SL(0x34,(uint32_t)((int32_t)d0*d2));
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);ga_palette(a,0x22);
        SSTART(0x2a0f26);SL(0,0x9c80c);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x9c80c: {
        ga_air(a,e);
        uint32_t program;
        if (ga_attack(a,e,BIT(0x90,4)?0x9d742:0x9d732)) {
            SB(0x1d,0);ga_palette(a,0x202);
            SW(0x28,W(0x28)+(uint16_t)((ga_rand(e)&0xfu)-7u));SW(0x2c,W(0x2c)+(uint16_t)((ga_rand(e)&0xfu)-7u));
            SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
            SSTART(0x22e504);SL(0,0x9c998);return GA_MORE;
        }
        if (!B(0x85)) {
            SB(0x85,neo_gate_splash(a,e,0)?0xff:0);                      /* 03C5C6 (the splash 03C962), SCS */
        }
        if (!BIT(0x15,7)) {
            ANIM();
            if (ga_cull_rect(a,e,0x9d7d0)) SL(0,0x9c464);
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        if (B(0x85)) return GA_DELETE;
        /* 09C87E */
        SB(0x1d,0);ga_palette(a,0x202);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        if ((int8_t)(B(0x84)-0x40u)>=0) SET(0x15,0);
        if (BIT(0x5f,0)) {SW(0x2c,W(0x2c)+(uint16_t)((ga_rand(e)&0xfu)-7u));program=0x2a109a;}
        else if (BIT(0x5f,1)) {SW(0x28,W(0x28)+(uint16_t)((ga_rand(e)&0xfu)-7u));SET(0x15,1);program=0x2a1034;}
        else {
            SW(0x28,W(0x28)+(uint16_t)((ga_rand(e)&0xfu)-7u));
            if (B(0x6d)!=0x40u) SW(0x2c,W(0x2c)+(uint16_t)((ga_rand(e)&0xfu)-7u));
            program=0x2a1034;
        }
        SSTART(program);SL(0,0x9c998);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x9c998:
        ga_project(a,e);ANIM();
        if (r) return GA_DELETE;
        if (W(0x3c)>=0x140u) return GA_DELETE;
        if ((uint16_t)(W(0x40)-0x100u)>=0x100u) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- muzzle flashes ---- */
    case 0x9cae8:SSTART(0x2a0f40);goto cb24;
    case 0x9cafc:SSTART(0x2a0f7e);goto cb24;
    case 0x9cb10:SSTART(0x2a0fbc);
    cb24:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_palette(a,0x37a);SL(0,0x9cb44);
        /* fall through */
    case 0x9cb44:
        ga_project(a,e);ANIM();
        if (r) SL(0,0x9c464);
        if (ga_cull_rect(a,e,0x9d7d0)) SL(0,0x9c464);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    default:ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- window soldiers 08DA12 / 08DA34 ------------------------------------------ */
/* 01B0BE: carry when no live player is in front; A0 = the first in front. */
static bool k1b0be(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {
    for (unsigned p=0;p<2;p++) {
        uint32_t n=p?P2_NODE:P1_NODE;
        if (((e->players.active_mask>>p)&1u) && !behind(a,e,n)) {*node=n;return false;}
    }
    return true;
}
/* 08D940: carry with the nearest player at dx -20..1F, dy 0..3F. */
static bool k8d940(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;if (nearest(a,e,&n)) return false;
    uint16_t d0=(uint16_t)(px(e,n)-W(0x3c)),d1=(uint16_t)(py(e,n)-W(0x40));
    if (d1>=0x40u) return false;
    return (uint16_t)(d0+0x20u)<0x40u;
}
/* 08D91E */
static void k8d91e(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x90) && !BIT(0x15,6)) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);} else ga_stationary(a,e);
}
/* 08D972: 004C24, a hit dies by 0406B6 (JSR: the update goes on). */
static int k8d972(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) return dispatch(a,e,false);
    return D_MORE;
}
/* 08D880: 01945C with 08D878 deletes (after the cartridge check). */
static ga_result_t w_tail(neo_gactor_t *a,neo_genv_t *e,int d) {
    if (d==D_STOP) return GA_UNSUPPORTED;
    if (d==D_DELETE) return GA_DELETE;
    if (ga_cull(a,e,0x8d878)) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 08DC20 */
static void k8dc20(neo_gactor_t *a) {
    if (!B(0x8f)) {
        int16_t d0=(int16_t)(0x140-(int16_t)(W(0x6e)<<3));
        if ((int16_t)(d0-S16(W(0x3c)))>=0) SB(0x8f,1);
        return;
    }
    if (W(0x8a)) SW(0x8a,W(0x8a)-1u);
    if (W(0x8c)) SW(0x8c,W(0x8c)-1u);
}
/* 08D9D0 / 08D988 / 08D9E2 */
static uint32_t k8d9d0(neo_gactor_t *a,neo_genv_t *e,bool *carry) {uint32_t n=0;*carry=k1b0be(a,e,&n);return *carry?0:n;}
static bool w_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind==NEO_ANIM_SPAWN_THROW) {   /* the grenades 03BBBC (23D976) / 03BC28 (28ABD4): the world's thrown object */
        if (!e->spawn_throw || !e->spawn_throw(e->ctx,a,routine?routine:0x3bb8e)) {ga_fail(e,routine?routine:0x3bb8e);return false;}
        return true;
    }
    return false;
}
static ga_result_t window_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r,d;
    for (unsigned guard=0;guard<4;guard++) switch (L(0)) {
    case 0x8da12:SB(0x90,0);SL(0x76,0x23d62a);SL(0x7a,0x23d976);SL(0x82,0xb05fe);goto c56;
    case 0x8da34:SB(0x90,1);SL(0x76,0x28ab8c);SL(0x7a,0x28abd4);SL(0x82,0xb0626);
    c56: {
        ga_palette(a,0x20e);
        if (!neo_gate_ripple(a,e,2)) return GA_UNSUPPORTED;            /* 03C52C */
        SB(0x8f,0);
        uint16_t d0=W(0x70);
        d0=d0?(uint16_t)(((uint32_t)rank(a,e,0x2d3c16)*d0)>>8):0xffffu;
        SW(0x8a,d0);
        uint32_t v=((uint32_t)rank(a,e,0x2d3b54)*W(0x72))>>8;
        SW(0x72,v>=0x100u?0xffu:v);
        SW(0x8c,W(0x74)?W(0x74):0xffffu);
        SW(0x66,0x8820);SW(0x1e,0x8820);
    }   /* fall through */
    case 0x8dad0:SSTART(L(0x76));SL(0,0x8dae4);
        /* fall through */
    case 0x8dae4:
        k8dc20(a);k8d91e(a,e);
        if (ga_anim_step(a,e,w_call)<0) return GA_UNSUPPORTED;
        if (!B(0x90)) {
            /* 08D988 */
            if (!(e->video&0x1fu)) {bool c;uint32_t n=k8d9d0(a,e,&c);if (c) SB(0x15,B(0x15)^1u);SL(0x7e,n);}
            if (k8d940(a,e)) SL(0,0x8dbe8);
            uint16_t g=0;
            {uint16_t y=(uint16_t)(W(0x40)-0x100u),x=(uint16_t)(W(0x3c)-0x20u);   /* 00510C */
             if (y<0x100u && x<0x200u) {
                 if (!e->grid) {ga_fail(e,0x510c);return GA_UNSUPPORTED;}
                 g=e->grid[(((x>>1)&0xf0u)+((y>>4)&0x0eu))/2u];if (g>=0x100u) g=0xff;
             }}
            if (g) SL(0,0xb05fe);
            if (!BIT(0x15,6)) SL(0,0x8db2a);
        } else {bool c;uint32_t n=k8d9d0(a,e,&c);SL(0x7a,c?0x23e82e:0x28abd4);SL(0x7e,n);}   /* 08D9E2 */
        if (!W(0x8a) && (uint16_t)(W(0x3c)-0x30u)<0xf0u && L(0x7e)) {SW(0x8a,W(0x72));SL(0,0x8db98);}   /* 08DC64 */
        if (!W(0x8c)) SL(0,L(0x82));                                     /* 08DA04 */
        d=k8d972(a,e);return w_tail(a,e,d);
    case 0x8db2a:SSTART(0x28ac56);SL(0,0x8db40);
        /* fall through */
    case 0x8db40:
        k8d91e(a,e);
        if ((r=ga_anim_step(a,e,w_call))<0) return GA_UNSUPPORTED;
        if (r && BIT(0x15,6)) SL(0,0x8db66);
        d=k8d972(a,e);return w_tail(a,e,d);
    case 0x8db66:SSTART(0x28acce);SL(0,0x8db7c);
        /* fall through */
    case 0x8db7c:
        k8d91e(a,e);
        if ((r=ga_anim_step(a,e,w_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8dad0);
        d=k8d972(a,e);return w_tail(a,e,d);
    case 0x8db98:SSTART(L(0x7a));SL(0,0x8dbac);
        /* fall through */
    case 0x8dbac:
        k8dc20(a);k8d91e(a,e);
        {uint16_t t24=W(0x24);
         if ((r=ga_anim_step(a,e,w_call))<0) return GA_UNSUPPORTED;
         if (r) SL(0,0x8dad0);
         /* 08DBC4 compares +90 as a word: its upper byte is D0's after
          * 00427A, which a WAIT leaves as +26 (004328) when the timer +24
          * was not negative (with +26 >= 100 the test never holds). A
          * program end (004352) leaves D0 as 08D91E's (0143E4 / 01436A:
          * position words, not zero in practice: the check is skipped, an
          * assumption); a negative timer is an explicit stop. */
         if (!B(0x90) && !r) {
            if (S16(t24)<0) {ga_fail(e,0x8dbc8);return GA_UNSUPPORTED;}
            if (!(W(0x26)>>8) && k8d940(a,e)) SL(0,0x8dbe8);
         }}
        d=k8d972(a,e);return w_tail(a,e,d);
    case 0x8dbe8:SSTART(0x28ad18);SL(0,0x8dbfe);
        /* fall through */
    case 0x8dbfe:
        k8d91e(a,e);
        if (ga_anim_step(a,e,w_call)<0) return GA_UNSUPPORTED;
        if (!k8d940(a,e)) SL(0,0x8dad0);
        d=k8d972(a,e);return w_tail(a,e,d);
    case 0xb05fe:case 0xb0626:case 0xb0632:
        /* The node becomes a regular soldier (the world moves it). */
        if (!e->become_soldier || !e->become_soldier(e->ctx,a,L(0))) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        return GA_MORE;
    default:ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- soldier family 05C53C / 05D226 / 05D916 (parachute) ------------------------ */
/* 05C53C..05E918 (script records of section 2; tables 05E918..05EB40 and the
 * rank tables 2CFD00..2CFF9C). One record layout for three behaviours:
 * - 05D916 / 05D924 (parachutist): 05E186 (+7E = +76 >> 4), +15 bit 5 clear /
 *   set, +4E = 1, +50 = 05E918+C, palette 238, 8820, +34 = +62 = -(rank
 *   2CFF70 << 8), +6E = 01AF9A, +88 = 0, +8B = 8, +8C = F0, the canopy 05DDA4
 *   (D0 = E, (0, 0)); 05D9AC: +7C = rank 2CFF0E, +86 = 0, 26333E; 05D9D8:
 *   05E19C (swing: +87 -= 2, +30 = cos(+87) * B0 with +7E added to the low
 *   word, +34 = +62, 0143E4; carry on the ground) -> 05DA68, the aim 05E388
 *   (+88 steps to the target's angle every 4th video frame) -> 05DA16 (+86 =
 *   1, the shot 05E7D8, a program of 05EA38 by +88 >> 1, back to 05D9AC);
 *   05DA68: lands (+86 = 3, 263498) -> 05C5B4 (the walking soldier). Tail
 *   05DBC6: 05E6D2, 004C24, a hit -> +86 = 2 and 05DC32 (the helmet 05DD2A
 *   with +30 = 8000 by +5B, 0406B6 and the routine it leaves); deleted
 *   (05DC7A, next update) out of x -20..160, y <= D0 or 01945C 05E9A0.
 * - 05DDA4 (canopy): palette 38D, +50 = 05E954, +4E = 14, 01AD6E, 2634E0;
 *   05DDE0 / 05DE22 follow the creator's +86 (0: 2634E0, 1: 05EA5C by its
 *   +88), depth = creator's - 1 (- 2 while 05DDE0); 05DEDA: creator's +86 =
 *   3 -> 05DE84 (2638B0), 2 -> 05DE98 (26395C), a hit passes +5A and +5F bit 7
 *   to the creator; 05DEA8: +5F bit 3, 01AD52, +50 = 0, the program to its
 *   end; the creator gone deletes the tree (00409A).
 * - 05C53C..05C5A0 / 05C5B4 (walking soldier, +8B/+8C the aim step and mask):
 *   05E10E takes the script words (+6E nonzero: +93 bit 4; +85/+84 the rows
 *   of 2CFE40 / 2CFE70), 05E0B8 (faces the screen centre), 05E0C8 (8820,
 *   palette 238, +4E = 1, +50 = 05E918, +7C = rank 2CFD00, 03C52C).
 *   States 05C694 (idle) .. 05D0DA (walk, run, turn, jump 05CA76/05CB06,
 *   fall 05CD08/05CD4E, aim 05CD90..05D0DA with the programs of 05E9B8 /
 *   05E9F8 and the shot 05E800, crouch-fire 05D100..05D200 when the target
 *   crouches, 01AE00). Tails 05DAB4 / 05DB4C: 05E6D2, +7C count, +91 by +5F
 *   bit 0, 05E7A8 (RAM 10C2F6 = 9 and x 820..880: 05D732), 004C24, a hit ->
 *   05DB18 / 05DB92 (helmet (0, 18) / (0, 10), then 0406B6 / 0407CA and the
 *   routine it leaves), 01945C 05E998 -> 05DC7A.
 * - 05D226 / 05D234 (05E164: +80 = +6E, +82 = +74): walks to x +80 (01AD7C),
 *   then jumps (+93 bit 2, 05D390) or stands, fires (05D406.. / 05D596..)
 *   and at 05D732 throws the helmet and becomes a soldier (JMP 0B0632).
 * - 05D76A (05E17C): hops by the rows of 2CFEAA (rank 2CFE90) and fires.
 * - Shots: 05E800 (+72 = the offset row 05EA80 / 05EAC0 by +89, the flash
 *   05DF3E and the bullet 05DFC2 through 01B242 + 01ACF2; the bullet's sweep
 *   014A02 from (+2C + F) hitting -> 05DD12 at once), 05E8B8 (05DF3E and
 *   05DF9A at (-20, E) by 01B220). 05DF9A / 05DFC2: speed rank 2CFDDE (along
 *   the facing / at angle +89 * 16), +7A = 96, palette 37B, E0EF, +15 bit 5,
 *   sound 1064, 263A00; 05E03E: 0143E4, the water test 03C5C6 once (+92), a
 *   surface -> 05DD12 (deleted in water), +7A out or the attack row 05E960 ->
 *   05DD12 (1001, 009F88, JMP 01C706), 019504 05E9A8 deletes the tree.
 * - 05DD2A (helmet): +34 = 4CCCC, +76 = -7AE1, +30 = -CCCC, palette 238,
 *   263AD0, falls to the ground (05DD0A) or out of 05E9A8. 05DF3E (flash):
 *   palette 204, 1001, 263A2E.
 * Explicit stops: 01AE00 without the players'
 * nodes (e->vehicle), 0AAA88 with a slot of kind 0 (TRAP #15). */
static uint32_t rl(neo_genv_t *e,uint32_t at) {
    const uint8_t *t=ga_rom(at);
    if (!t) {ga_fail(e,at);return 0;}
    return (uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3];
}
static uint16_t rw(neo_genv_t *e,uint32_t at) {
    const uint8_t *t=ga_rom(at);
    if (!t) {ga_fail(e,at);return 0;}
    return (uint16_t)(t[0]<<8|t[1]);
}
/* 01AEDA */
static bool live(const neo_genv_t *e,uint32_t node,unsigned *p) {
    unsigned d0=node==P2_NODE?1u:0u;
    if (p) *p=d0;
    if (node!=P1_NODE && node!=P2_NODE) return false;
    return (e->players.active_mask>>d0)&1u;
}
/* 01B04A */
static uint32_t k1b04a(neo_genv_t *e) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    return d0==2u?P2_NODE:P1_NODE;
}
/* 01AFFC with its A0 (P1 when none); carry (true) when none. */
static bool k1affc(neo_gactor_t *a,neo_genv_t *e,uint32_t rect,uint32_t *node) {
    const uint8_t *r=ga_rom(rect);
    *node=P1_NODE;
    if (!r) {ga_fail(e,rect);return true;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    neo_target_t t=neo_target_pick(&c,&e->players,q);
    if (t.valid && t.player) *node=P2_NODE;
    return !t.valid;
}
/* 01AF3C: carry (true) when the node is outside the oriented rectangle. */
static bool k1af3c(neo_gactor_t *a,neo_genv_t *e,uint32_t node,uint32_t rect) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return true;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    int16_t d1=(int16_t)(py(e,node)-W(0x40));
    if (BIT(0x15,1)) d1=(int16_t)-d1;
    if (d1<q[1] || d1>(int16_t)(q[1]+q[3])) return true;
    d1=(int16_t)(px(e,node)-W(0x3c));
    if (BIT(0x15,0)) d1=(int16_t)-d1;
    return d1<q[0] || d1>(int16_t)(q[0]+q[2]);
}
/* 01B12E */
static uint8_t k1b12e(neo_gactor_t *a,const neo_genv_t *e,uint32_t node) {
    return neo_angle((int16_t)(px(e,node)-W(0x3c)),(int16_t)(py(e,node)-W(0x40)));
}
/* 01AE00: carry when the node is a live player on the ground holding down
 * (0CA2CE word +0 bit 1). */
static bool k1ae00(neo_genv_t *e,uint32_t node,bool *stop) {
    if (node!=P1_NODE && node!=P2_NODE) return false;
    unsigned p=node==P2_NODE;
    if (!e->vehicle || !e->vehicle->player[p]) {ga_fail(e,0x1ae00);*stop=true;return false;}
    if (!e->vehicle->exists[p]) return false;
    if (!(e->vehicle->player[p][0x15]&0x40u)) return false;
    return (e->vehicle->input[p][0]>>1)&1u;
}
/* 0AAA88 for player p: the kind (+21) of its vehicle slot, 0 without one. */
static uint8_t vkind(neo_genv_t *e,unsigned p) {
    if (!e->vehicle || !e->vehicle->slots || !e->vehicle->exists[p] || !neo_vslot_node(&e->vehicle->slots->slot[p])) return 0;
    uint8_t k=e->vehicle->slots->slot[p].b[0x21];
    if (!k) ga_fail(e,0xaaacc);                                     /* TRAP #15 */
    return k;
}
static int16_t dxw(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {return S16(absw((uint16_t)(W(0x3c)-px(e,n))));}   /* 05E770 */
/* 05E784: carry when the node is out of y -10..30 of the actor. */
static bool k5e784(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {int16_t d=(int16_t)(py(e,n)-W(0x40));return d>0x30 || d<-0x10;}
/* 01B242: depth words and flips (bits 0..2 set) to the child. */
static void k1b242(neo_gactor_t *c,const neo_gactor_t *a) {
    ga_sw(c,0x1e,W(0x1e));ga_sw(c,0x66,W(0x66));
    ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|(B(0x15)&7u)));
}
/* 05E10E / 05E164 / 05E17C / 05E186 */
static void k5e126(neo_gactor_t *a,neo_genv_t *e,bool take) {
    if (take) {
        uint16_t d0=W(0x70),d1=W(0x72);
        SB(0x85,d0);SB(0x84,d1);
    }
    for (unsigned o=0x6e;o<0x7a;o+=2) SW(o,0);
    uint32_t n;nearest(a,e,&n);SL(0x6e,n);
}
static void k5e10e(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x93,7)) {k5e126(a,e,false);return;}
    if (W(0x6e)) SET(0x93,4);
    SET(0x93,7);k5e126(a,e,true);
}
static void k5e164(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x93,7)) {k5e126(a,e,false);return;}
    SW(0x80,W(0x6e));SB(0x82,W(0x74));
    SET(0x93,7);k5e126(a,e,true);
}
static void k5e17c(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x93,7)) {k5e126(a,e,false);return;}
    SET(0x93,7);k5e126(a,e,true);
}
static void k5e186(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x93,7)) {k5e126(a,e,false);return;}
    SB(0x7e,(uint8_t)(S16(W(0x76))>>4));
    k5e10e(a,e);
}
/* 05E0C8 */
static bool k5e0c8(neo_gactor_t *a,neo_genv_t *e) {
    k1b1dc(a);SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0x238);
    SW(0x4e,1);SL(0x50,0x5e918);SW(0x7c,rank(a,e,0x2cfd00));
    return neo_gate_ripple(a,e,2);                                   /* 03C52C */
}
/* 05E0B8 */
static bool k5e0b8(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x3c))<=0xa0) SB(0x15,B(0x15)^1u);
    return k5e0c8(a,e);
}
/* 05E19C: the swing under the canopy; carry on the ground. */
static bool k5e19c(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x87,B(0x87)-2u);
    uint32_t d1=(uint32_t)((int32_t)ga_cosine(B(0x87))*0xb0);
    d1=(d1&0xffff0000u)|((d1+W(0x7e))&0xffffu);
    SL(0x30,d1);SL(0x34,L(0x62));ga_air(a,e);
    return BIT(0x15,6);
}
/* 05E20A: the fall. */
static void k5e20a(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x34))>=0) SL(0x34,0xffff0000u);
    SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                        /* 0143DC */
    if (BIT(0x15,6)) {SL(0x30,0);SL(0x34,0);}
}
/* 05E1E4 (walk) / 05E234 (stand) */
static void k5e1e4(neo_gactor_t *a,neo_genv_t *e) {
    ga_stationary(a,e);
    if (!BIT(0x15,6)) {k5e20a(a,e);return;}
    ga_ground(a,e,NULL);
    if (!L(0x30)) SET(0x5f,0);
}
static void k5e234(neo_gactor_t *a,neo_genv_t *e) {ga_stationary(a,e);if (!BIT(0x15,6)) k5e20a(a,e);}
/* 05E244: carry after 6 updates in the air. */
static bool k5e244(neo_gactor_t *a) {
    if (BIT(0x15,6)) {SB(0x8f,0);return false;}
    SB(0x8f,B(0x8f)+1u);return (int8_t)B(0x8f)>=6;
}
/* 05E26C: carry after F blocked updates (+5F bit 0). */
static bool k5e26c(neo_gactor_t *a) {
    if (!BIT(0x5f,0)) {SB(0x90,0);return false;}
    SB(0x91,0x1e);SB(0x90,B(0x90)+1u);
    if ((int8_t)B(0x90)<0x0f) return false;
    SB(0x90,0x0f);return true;
}
/* 05E2E4: carry when on screen (x 18..128) with a live player. */
static bool k5e2e4(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x=S16(W(0x3c));
    if (x<0x18 || x>0x128) return false;
    uint32_t n;return !nearest(a,e,&n);
}
/* 05E482 */
static bool k5e482(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d0=k1b12e(a,e,L(0x6e));
    return (uint8_t)((d0+B(0x8b))&B(0x8c))==d0;
}
/* 05E4CC: carry when the target +6E is inside the rectangle of 2CFE40 by +85. */
static bool k5e4cc(neo_gactor_t *a,neo_genv_t *e) {return !k1af3c(a,e,L(0x6e),rl(e,0x2cfe40u+4u*(B(0x85)&3u)));}
static bool k5e454(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;if (nearest(a,e,&n)) return true;
    if (!k5e4cc(a,e)) return false;
    if (!k5e784(a,e,L(0x6e))) return true;
    return k5e482(a,e);
}
static int k5e2a0(neo_gactor_t *a,neo_genv_t *e) {
    if (k5e454(a,e)) return 0;
    if (B(0x8c)==0xc0u) return 1;
    if (!k5e4cc(a,e)) return 1;
    k5e482(a,e);
    return (ga_rand(e)&1u)?2:1;
}
static bool k5e30a(neo_gactor_t *a,neo_genv_t *e) {
    if (!k5e2e4(a,e)) return false;
    uint32_t n=L(0x6e);
    if ((int8_t)B(0x8d)<=5) {
        if (S16(W(0x7c))>0) return false;
        SW(0x7c,0);
        if (!k5e4cc(a,e)) return false;
    }
    if (!BIT(0x93,4)) return true;
    if (!k5e784(a,e,n)) return true;
    return k5e482(a,e);
}
static bool k5e35c(neo_gactor_t *a,neo_genv_t *e) {return S16(W(0x7c))<=0 && k5e2e4(a,e);}   /* 05E35C / 05E372 */
/* 05E388: the parachutist's aim (+88 0..10 to the target's angle). */
static bool k5e388(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n=L(0x6e);
    if (!n) return false;
    unsigned d0=((k1b12e(a,e,n)+8u)>>4)&0xfu;
    int d1;
    if (!d0) d0=8;
    else if (d0<4u) {if (B(0x88)==0x10u) return false;d1=1;goto step;}
    else if (d0<8u) {if (!B(0x88)) return false;d1=-1;goto step;}
    else d0-=8u;
    d0*=2u;
    if ((int8_t)d0==(int8_t)B(0x88)) {
        if (S16(W(0x7c))>0 || !k5e2e4(a,e)) return false;
        int16_t y=S16(W(0x40));
        return y<=0x1e0 && y>=0x100;
    }
    if ((int8_t)d0>=(int8_t)B(0x88)) {if (B(0x88)==0x10u) return false;d1=1;}
    else {if (!B(0x88)) return false;d1=-1;}
step:
    if (!(e->video&3u)) SB(0x88,B(0x88)+(unsigned)d1);
    return false;
}
static bool k5e4a8(neo_gactor_t *a,neo_genv_t *e) {return !k5e4cc(a,e) && dxw(a,e,L(0x6e))>0xc0;}
static bool k5e6ec(neo_gactor_t *a) {
    int16_t x=S16(W(0x3c));
    if (x<=0x20 && BIT(0x93,0)) return true;
    return x>=0x120 && !BIT(0x93,0);
}
static int16_t row2cfe70(neo_gactor_t *a,neo_genv_t *e,unsigned o) {return S16(rw(e,0x2cfe70u+8u*(B(0x84)&3u)+o));}
static bool k5e4f0(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (k5e6ec(a) || k1affc(a,e,0x5e988,&n) || k5e784(a,e,n)) return false;
    return dxw(a,e,n)<row2cfe70(a,e,0);
}
static bool k5e536(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (k1affc(a,e,0x5e988,&n) || k5e784(a,e,n)) return false;
    return dxw(a,e,n)<row2cfe70(a,e,2);
}
static bool k5e574(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x5f,0)) {SB(0x91,0x1e);return false;}
    uint32_t n;
    if (k1affc(a,e,0x5e988,&n)) return false;
    return dxw(a,e,n)<row2cfe70(a,e,4);
}
static bool k5e5ce(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (nearest(a,e,&n)) return false;
    if (!k1affc(a,e,0x5e978,&n)) return false;
    if (!k1affc(a,e,0x5e980,&n)) return true;
    return behind(a,e,L(0x6e));                                      /* 01B08A */
}
static bool k5e5ba(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x8e,B(0x8e)+1u);
    if ((int8_t)B(0x8e)<0x1e) return false;
    SB(0x8e,0x1e);return k5e5ce(a,e);
}
/* 05E60C: carry when the sweep (±8, 0) from 18 lower meets nothing (014906). */
static bool k5e60c(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x2c,W(0x2c)+0x18u);SW(0x40,W(0x40)+0x18u);
    uint16_t d6=ga_sweep_probe(a,e,S16(W(0x62))<0?-8:8,0);
    SW(0x2c,W(0x2c)-0x18u);SW(0x40,W(0x40)-0x18u);
    return !((d6>>1)&1u);
}
static bool k5e65e(neo_gactor_t *a,neo_genv_t *e) {uint32_t n;return BIT(0x93,3) && !k1affc(a,e,0x5e990,&n);}
static bool k5e67e(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (nearest(a,e,&n) || k5e784(a,e,n)) return false;
    return dxw(a,e,n)<=0x30;
}
static void k5e6d2(neo_gactor_t *a,neo_genv_t *e) {if (!live(e,L(0x6e),NULL)) SL(0x6e,k1b04a(e));}
static void k5e720(neo_gactor_t *a) {if (BIT(0x15,0)) CLR(0x93,0); else SET(0x93,0);}
static void k5e73a(neo_gactor_t *a) {
    uint32_t d0=L(0x62);
    if (!BIT(0x93,0)) {if (S32(d0)<0) d0=0u-d0;}
    else if (S32(d0)>=0) d0=0u-d0;
    SL(0x62,d0);
    d0=ASR(d0,1);if (S32(d0)<0) d0=0u-d0;
    SW(0x26,d0>>8);
}
static bool k5e7a8(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->pickup || e->pickup->mission!=9u) return false;
    int16_t x=S16(W(0x28));
    return x>0x820 && x<0x880;
}
/* 05E81A..05E8B6 with A0 = the offset table. */
static void shot_from(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    uint32_t row=table+4u*(B(0x89)&0xfu);
    SL(0x72,row);
    int16_t dx=S16(rw(e,row)),dy=S16(rw(e,row+2u));
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x5df3e,0x1c):e->spawn?e->spawn(e->ctx,a,0x5df3e,false):NULL;
    if (c) {k1b242(c,a);ga_place_at(c,a,e,dx,dy);}
    c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x5dfc2,0x1c):e->spawn?e->spawn(e->ctx,a,0x5dfc2,false):NULL;
    if (!c) return;
    SW(0x2c,W(0x2c)+0x0fu);SW(0x40,W(0x40)+0x0fu);
    uint16_t d6=ga_sweep_place_d0(a,e,c,dx,(int16_t)(dy-0x0f),0xff);
    SW(0x2c,W(0x2c)-0x0fu);SW(0x40,W(0x40)-0x0fu);
    if (d6&0xffu) ga_sl(c,0,0x5dd12);
    ga_sb(c,0x89,B(0x89));
    k1b242(c,a);ga_place_at(c,a,e,dx,dy);
}
static void k5e800(neo_gactor_t *a,neo_genv_t *e) {shot_from(a,e,BIT(0x15,0)?0x5eac0:0x5ea80);}
static void k5e7d8(neo_gactor_t *a,neo_genv_t *e) {SB(0x89,(B(0x88)>>1)+8u);shot_from(a,e,0x5eb00);}
static bool p_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x5e8b8:child(a,e,0x5df3e,0x1c,-0x20,0x0e);child(a,e,0x5df9a,0x1c,-0x20,0x0e);return true;
    case 0x5e906:SW(0x7c,rank(a,e,0x2cfd62));return true;
    case 0x5e7ec:SB(0x89,BIT(0x15,0)?0:8);k5e800(a,e);return true;
    }
    return false;
}
/* 05DAB4 (crouch false) / 05DB4C (crouch true) */
static ga_result_t p_tail(neo_gactor_t *a,neo_genv_t *e,bool crouch) {
    k5e6d2(a,e);
    SW(0x7c,W(0x7c)-1u);if (S16(W(0x7c))<0) SW(0x7c,0);
    if (!crouch) {
        if (BIT(0x5f,0)) SB(0x91,0x1e);
        if (k5e7a8(a,e)) SL(0,0x5d732);
    }
    ga_register(a,e,NULL,NULL);
    if (BIT(0x5f,7)) SL(0,crouch?0x5db92:0x5db18); else SB(0x5a,0);
    if (ga_cull(a,e,0x5e998)) SL(0,0x5dc7a);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 05DBC6 */
static ga_result_t p_tail_air(neo_gactor_t *a,neo_genv_t *e) {
    k5e6d2(a,e);
    ga_register(a,e,NULL,NULL);
    if (BIT(0x5f,7)) {SB(0x86,2);SL(0,0x5dc32);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
    SB(0x5a,0);
    int16_t x=S16(W(0x3c));
    if (x<=-0x20 || x>=0x160 || S16(W(0x40))<=0xd0) SL(0,0x5dc7a);
    if (ga_cull(a,e,0x5e9a0)) SL(0,0x5dc7a);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 05DB18 / 05DB92 / 05DC32: the helmet, then JSR 0406B6 / 0407CA and JMP
 * (A6): the routine the death left runs in the same update. */
static ga_result_t p_death(neo_gactor_t *a,neo_genv_t *e,int16_t dy,bool crouch,bool air,uint32_t self) {
    neo_gactor_t *c=child(a,e,0x5dd2a,0x1c,0,dy);
    if (air && c) ga_sl(c,0x30,(B(0x5b)&0x80u)?0x8000u:0xffff8000u);
    SL(0,self);
    int d=dispatch(a,e,crouch);
    if (d!=D_MORE) return result(a,e,d);
    uint32_t r=L(0);
    if (r!=0x40df0 && r!=0x40ae0 && r!=0x40d06 && r!=0x40db4) {ga_fail(e,r);return GA_UNSUPPORTED;}   /* +5A zero: JMP (A6) loops */
    return death_tick(a,e);
}
#define PS(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define PA() do { if ((r=ga_anim_step(a,e,p_call))<0) return GA_UNSUPPORTED; } while (0)
#define GO(x) do { SL(0,(x));goto again; } while (0)
static ga_result_t para_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;unsigned guard=0;
again:
    if (++guard>12) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    if (e->unsupported) return GA_UNSUPPORTED;
    switch (L(0)) {
    /* ---- walking soldier entries ---- */
    case 0x5c53c:k5e10e(a,e);SB(0x8c,0xf0);SB(0x8b,0x08);ga_floor(a,e);CLR(0x93,3);goto c5f0;
    case 0x5c550:k5e10e(a,e);SB(0x8c,0xc0);SB(0x8b,0x20);goto c622;
    case 0x5c564:k5e10e(a,e);SB(0x8c,0xe0);SB(0x8b,0x10);goto c622;
    case 0x5c578:k5e10e(a,e);SB(0x8c,0xf0);SB(0x8b,0x08);goto c622;
    case 0x5c58c:k5e10e(a,e);SB(0x8c,0xf0);SB(0x8b,0x08);SET(0x93,3);ga_floor(a,e);goto c5f0;
    case 0x5c5a0:k5e10e(a,e);SB(0x8c,0xf0);SB(0x8b,0x08);SET(0x93,3);goto c632;
    case 0x5c5b4:
        SB(0x1d,0);SB(0x8c,0xf0);SB(0x8b,0x08);CLR(0x93,3);           /* 009F88 */
        if (!k5e0c8(a,e)) return GA_UNSUPPORTED;
        goto c5f4;
    c5f0:
        if (!k5e0b8(a,e)) return GA_UNSUPPORTED;
    c5f4: {
        uint32_t n;nearest(a,e,&n);SL(0x6e,n);
        k1b1dc(a);ga_air(a,e);
        if (B(0x6c)==3u) GO(0x5c7b4);                                   /* 01B284 */
        GO(BIT(0x93,4)?0x5c7e8:0x5c694);
    }
    c622:CLR(0x93,3);
    c632: {
        ga_floor(a,e);
        if (!k5e0b8(a,e)) return GA_UNSUPPORTED;
        uint32_t n;nearest(a,e,&n);SL(0x6e,n);
        PS(0x2639a4);SL(0,0x5c65c);
    }   /* fall through */
    case 0x5c65c:
        k5e234(a,e);PA();
        if (r) SL(0,0x5c694);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5c682:SL(0x50,0x5e918);SL(0x6e,k1b04a(e));
        /* fall through */
    case 0x5c694:SL(0x30,0);SL(0x34,0);k5e720(a);SB(0x93,B(0x93)^1u);PS(0x262ad8);SL(0,0x5c6bc);
        /* fall through */
    case 0x5c6bc:
        k5e234(a,e);PA();
        if (!BIT(0x93,4)) {if (k5e5ba(a,e)) SL(0,0x5cc94);}
        else if (B(0x91)) {SB(0x91,B(0x91)-1u);if (k5e5ba(a,e)) SL(0,0x5cc94);}
        else {
            unsigned p;
            if (!live(e,L(0x6e),&p)) SL(0x6e,k1b04a(e));
            else {
                if (vkind(e,p)!=4u) {
                    int k=k5e2a0(a,e);
                    if (k==1) SL(0,0x5c870); else if (k==2) SL(0,0x5c91c);
                }
                if (k5e4a8(a,e)) SL(0,0x5c7e8);
                if (k5e65e(a,e)) SL(0,0x5cb06);
                if (k5e5ba(a,e)) SL(0,0x5cc94);
                if (k5e4f0(a,e)) SL(0,0x5c9c0);
                if (k5e536(a,e)) SL(0,0x5cb72);
            }
        }
        if (k5e30a(a,e)) SL(0,0x5cd90);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5c7b4:PS(0x262ad8);SL(0,0x5c7ca);
        /* fall through */
    case 0x5c7ca:
        k5e234(a,e);
        if (B(0x6c)!=3u) SL(0,0x5c694);
        PA();
        return p_tail(a,e,false);
    case 0x5c7e8:k5e720(a);SL(0x62,0x40000);k5e73a(a);PS(0x262b1a);SL(0,0x5c80e);
        /* fall through */
    case 0x5c80e:
        k5e1e4(a,e);PA();
        if (k5e26c(a)) SL(0,0x5cc3c);
        if (k5e454(a,e)) SL(0,0x5cc3c);
        if (k5e5ba(a,e)) SL(0,0x5cc3c);
        if (k5e4f0(a,e)) SL(0,0x5cc3c);
        if (k5e536(a,e)) SL(0,0x5cc3c);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5c870:
        k5e720(a);
        if (k5e6ec(a)) GO(0x5c91c);
        SL(0x62,0x10000);k5e73a(a);PS(0x262bcc);SL(0,0x5c89e);
        /* fall through */
    case 0x5c89e:
        k5e1e4(a,e);PA();
        if (k5e26c(a)) {SL(0,0x5c91c);if (k5e60c(a,e)) SL(0,0x5ca76);}
        if (k5e454(a,e)) SL(0,0x5c694);
        if (k5e5ba(a,e)) SL(0,0x5cc94);
        if (k5e4f0(a,e)) SL(0,0x5c9c0);
        if (k5e6ec(a)) SL(0,0x5c91c);
        if (k5e536(a,e)) SL(0,0x5cb72);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5c91c:
        k5e720(a);SB(0x93,B(0x93)^1u);
        if (k5e6ec(a)) GO(0x5c870);
        SL(0x62,0x10000);k5e73a(a);PS(0x262c18);SL(0,0x5c950);
        /* fall through */
    case 0x5c950:
        k5e1e4(a,e);PA();
        if (k5e26c(a)) SL(0,0x5c870);
        if (k5e454(a,e)) SL(0,0x5c694);
        if (k5e5ba(a,e)) SL(0,0x5cc94);
        if (k5e4f0(a,e)) SL(0,0x5c9c0);
        if (k5e6ec(a)) SL(0,0x5c870);
        if (k5e536(a,e)) SL(0,0x5cb72);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5c9c0: {
        k5e720(a);SB(0x93,B(0x93)^1u);
        if (k5e6ec(a)) GO(0x5c694);
        uint32_t n;k1affc(a,e,0x5e988,&n);SL(0x6e,n);
        SL(0x62,0x10000);k5e73a(a);SL(0x62,0u-L(0x62));PS(0x262c18);SL(0,0x5ca06);
    }   /* fall through */
    case 0x5ca06:
        k5e1e4(a,e);PA();
        if (k5e26c(a)) SL(0,0x5cd90);
        if (k5e30a(a,e)) SL(0,0x5c694);
        if (k5e5ba(a,e)) SL(0,0x5cc94);
        if (!k5e4f0(a,e)) SL(0,0x5c694);
        if (k5e6ec(a)) SL(0,0x5c694);
        if (k5e536(a,e)) SL(0,0x5cb72);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5ca76:
        SL(0x34,0x80000);SL(0x76,0xffff3334u);
        SL(0x30,BIT(0x15,0)?0x19999u:0xfffe6667u);
        if (!(ga_rand(e)&1u)) SW(0x30,W(0x30)+(uint16_t)(S16(W(0x30))>>1));
        SW(0x7e,W(0x30));PS(0x262ec8);SL(0,0x5cad8);
        /* fall through */
    case 0x5cad8:
        SL(0x34,L(0x34)+L(0x76));SW(0x30,W(0x7e));ga_air(a,e);PA();
        if (BIT(0x15,6)) SL(0,0x5cd4e);
        return p_tail(a,e,false);
    case 0x5cb06:
        SL(0x34,0x66666);SL(0x76,0xfffeb852u);SL(0x30,BIT(0x15,0)?0xfffb3334u:0x4ccccu);
        PS(0x262ec8);SL(0,0x5cb4a);
        /* fall through */
    case 0x5cb4a:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x76));PA();
        if (BIT(0x15,6)) SL(0,0x5cd4e);
        return p_tail(a,e,false);
    case 0x5cb72: {
        SL(0x30,0);SL(0x34,0);k5e720(a);SB(0x93,B(0x93)^1u);
        if (k5e6ec(a)) GO(0x5cc94);
        if (k5e5ce(a,e)) GO(0x5cbde);
        uint32_t n;k1affc(a,e,0x5e988,&n);SL(0x6e,n);
        PS(0x262f26);SL(0,0x5cbb8);
    }   /* fall through */
    case 0x5cbb8:
        k5e234(a,e);PA();
        if (r) SL(0,0x5cbde);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5cbde:k5e720(a);SL(0x62,0x40000);k5e73a(a);PS(0x262b1a);SL(0,0x5cc04);
        /* fall through */
    case 0x5cc04:
        k5e1e4(a,e);PA();
        if (!k5e574(a,e)) SL(0,0x5cc3c);
        if (k5e6ec(a)) SL(0,0x5cc3c);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5cc3c:
        if (!live(e,L(0x6e),NULL)) GO(0x5c682);
        SL(0x30,0);SL(0x34,0);SB(0x90,0);PS(0x262b98);SL(0,0x5cc6e);
        /* fall through */
    case 0x5cc6e:
        k5e234(a,e);PA();
        if (r) SL(0,0x5c694);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5cc94: {
        SB(0x8d,B(0x8d)+1u);SL(0x30,0);SL(0x34,0);
        uint32_t n;uint16_t y=nearest(a,e,&n)?W(0x40):py(e,n);
        PS(y<W(0x40)?0x262f26:0x262fc4);SL(0,0x5cce2);
    }   /* fall through */
    case 0x5cce2:
        k5e234(a,e);PA();
        if (r) SL(0,0x5c694);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5cd08:SL(0x50,0x5e918);SL(0x30,ASR(L(0x30),1));PS(0x262ec8);SL(0,0x5cd30);
        /* fall through */
    case 0x5cd30:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);PA();                  /* 0143DC */
        if (!k5e244(a)) SL(0,0x5cd4e);
        return p_tail(a,e,false);
    case 0x5cd4e:k1b1dc(a);PS(0x262f06);SL(0,0x5cd6a);
        /* fall through */
    case 0x5cd6a:
        k5e234(a,e);PA();
        if (r) SL(0,0x5c694);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    /* ---- aim and fire ---- */
    case 0x5cd90: {
        uint32_t n=L(0x6e);
        if (BIT(0x93,3)) {
            bool stop=false;
            if (k1ae00(e,n,&stop) && py(e,n)==W(0x40)) GO(0x5d100);
            if (stop) return GA_UNSUPPORTED;
        }
        SB(0x8d,0);SL(0x30,0);
        SB(0x8a,k1b12e(a,e,n)+B(0x8b));
        uint8_t d1;
        if (!k5e784(a,e,n)) {SB(0x8a,S16(W(0x3c))>S16(px(e,n))?0x80:0);d1=0x80;}
        else d1=B(0x8c);
        uint8_t d0=(uint8_t)((B(0x8a)&d1)>>4);
        SB(0x8a,d0);SB(0x89,d0);
        if ((int8_t)d0>8) {SET(0x93,1);PS(0x263082);} else {CLR(0x93,1);PS(0x263062);}
        SL(0,0x5ce54);
    }   /* fall through */
    case 0x5ce54:
        k5e234(a,e);PA();
        if (r) SL(0,0x5ce7a);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5ce7a:
        SL(0x30,0);SL(0x34,0);
        if (!BIT(0x93,1)) {PS(0x2630e2);if (!BIT(0x15,0)) SB(0x8a,8u-B(0x8a));}
        else {PS(0x263170);SB(0x8a,B(0x8a)-9u);if (BIT(0x15,0)) SB(0x8a,6u-B(0x8a));}
        SB(0x8a,B(0x8a)*2u);SB(0x88,0);SL(0,0x5cef4);
        /* fall through */
    case 0x5cef4:
        k5e234(a,e);PA();
        if ((B(0x8a)&0x1fu)==B(0x88)) SL(0,0x5cf2a);
        SB(0x88,B(0x88)+1u);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5cf2a:SL(0x30,0);SL(0x34,0);SB(0x88,B(0x88)-1u);SB(0x83,rank(a,e,0x2cfdc4));
        /* fall through */
    case 0x5cf46: {
        unsigned d0=B(0x89)&0xfu;
        PS(rl(e,(BIT(0x15,0)?0x5e9f8u:0x5e9b8u)+4u*d0));
        k5e800(a,e);SW(0x7c,rank(a,e,0x2cfd62));SL(0,0x5cfa2);
    }   /* fall through */
    case 0x5cfa2:
        k5e234(a,e);PA();
        if (r || (int8_t)B(0x83)>1) {
            if (S16(W(0x7c))<=0) {
                SL(0,0x5cf46);SB(0x83,B(0x83)-1u);
                if ((int8_t)B(0x83)<=0) SL(0,0x5cff0);
            }
        }
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5cff0: {
        SL(0x30,0);SL(0x34,0);
        uint8_t d0;int8_t c;
        if (!BIT(0x93,1)) {PS(0x2630e2);d0=0x10;c=8;} else {PS(0x263170);d0=0x0c;c=6;}
        if ((int8_t)B(0x88)>=c) {SB(0x88,d0-B(0x88));SB(0x15,B(0x15)^1u);}
        SL(0,0x5d052);
    }   /* fall through */
    case 0x5d052:
        k5e234(a,e);PA();
        if (!B(0x88)) SL(0,0x5d08e);
        if (!(e->video&1u)) SB(0x88,B(0x88)-1u);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    case 0x5d08e:SL(0x30,0);SL(0x34,0);SW(0x7c,rank(a,e,0x2cfd00));PS(BIT(0x93,1)?0x2630c2:0x2630a2);SL(0,0x5d0da);
        /* fall through */
    case 0x5d0da:
        k5e234(a,e);PA();
        if (r) SL(0,0x5c682);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,false);
    /* ---- crouched fire ---- */
    case 0x5d100:SL(0x50,0x5e93c);PS(0x262dcc);SL(0,0x5d11e);
        /* fall through */
    case 0x5d11e:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d144);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,true);
    case 0x5d144:SL(0x30,0);SL(0x34,0);SB(0x83,rank(a,e,0x2cfdc4));
        /* fall through */
    case 0x5d15c:SW(0x7c,rank(a,e,0x2cfd62));SL(0x50,0x5e948);PS(0x262e32);SL(0,0x5d18a);
        /* fall through */
    case 0x5d18a:
        k5e234(a,e);PA();
        if (r && S16(W(0x7c))<=0) {
            SB(0x83,B(0x83)-1u);
            if ((int8_t)B(0x83)>0) SL(0,0x5d15c);
            else {SL(0,0x5d1e2);SW(0x7c,rank(a,e,0x2cfd00));}
        }
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,true);
    case 0x5d1e2:SL(0x30,0);SL(0x34,0);PS(0x262e58);SL(0,0x5d200);
        /* fall through */
    case 0x5d200:
        k5e234(a,e);PA();
        if (r) SL(0,0x5c682);
        if (k5e244(a)) SL(0,0x5cd08);
        return p_tail(a,e,true);
    /* ---- 05D226: walk to +80, jump or stand, fire, become a soldier ---- */
    case 0x5d226:k5e164(a,e);SET(0x93,2);goto d238;
    case 0x5d234:k5e164(a,e);
    d238:
        ga_floor(a,e);
        if (!k5e0b8(a,e)) return GA_UNSUPPORTED;
        if (!W(0x80)) GO(BIT(0x93,2)?0x5d406:0x5d596);
        if (B(0x82)) SW(0x7c,B(0x82));
        ga_stationary(a,e);
        GO(k5e244(a)?0x5d27a:0x5d302);
    case 0x5d27a:SL(0x30,ASR(L(0x30),1));SL(0x50,0x5e918);PS(0x262ec8);SL(0,0x5d2a2);
        /* fall through */
    case 0x5d2a2:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);PA();
        if (!k5e244(a)) SL(0,0x5d2c0);
        return p_tail(a,e,false);
    case 0x5d2c0:k1b1dc(a);PS(0x262f06);SL(0,0x5d2dc);
        /* fall through */
    case 0x5d2dc:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d302);
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,false);
    case 0x5d302:k5e720(a);SL(0x62,0x40000);k5e73a(a);SL(0x50,0x5e918);PS(0x262b1a);SL(0,0x5d330);
        /* fall through */
    case 0x5d330: {
        k5e1e4(a,e);PA();
        /* 01AD7C: + the scene root's +14 word (RAM 10E97C). */
        neo_contact_parent_t root;int16_t d0=S16(W(0x80));
        if (ga_parent(e,0x10e968u,&root)) d0=(int16_t)(d0+(int16_t)((uint32_t)root.x>>16));
        if (!BIT(0x15,0)) {if (d0>=S16(W(0x3c))) SL(0,0x5d390);}
        else if (d0<=S16(W(0x3c))) SL(0,0x5d390);
        
        if (k5e26c(a)) SL(0,0x5d390);
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,false);
    }
    case 0x5d390:
        if (!BIT(0x93,2)) GO(0x5d552);
        SL(0x34,0xccccc);SL(0x76,0xfffae148u);SL(0x30,0xfff9999au);PS(0x262dcc);SL(0,0x5d3c8);
        /* fall through */
    case 0x5d3c8:
        ga_air(a,e);
        if (!BIT(0x15,6)) SL(0x34,L(0x34)+L(0x76)); else k1b1dc(a);
        PA();
        if (r && !W(0x30)) SL(0,0x5d406);
        return p_tail(a,e,false);
    case 0x5d406:SL(0x30,0);SL(0x34,0);SL(0x50,0x5e93c);PS(0x262e26);SL(0,0x5d42c);
        /* fall through */
    case 0x5d42c:
        k5e234(a,e);PA();
        if (k5e67e(a,e)) SL(0,0x5d502);
        if (k5e35c(a,e)) SL(0,0x5d464);
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,true);
    case 0x5d464:SL(0x30,0);SL(0x34,0);SB(0x83,rank(a,e,0x2cfdc4));
        /* fall through */
    case 0x5d47c:SW(0x7c,rank(a,e,0x2cfd62));SL(0x50,0x5e948);PS(0x262e32);SL(0,0x5d4aa);
        /* fall through */
    case 0x5d4aa:
        k5e234(a,e);PA();
        if (r && S16(W(0x7c))<=0) {
            SB(0x83,B(0x83)-1u);
            if ((int8_t)B(0x83)>0) SL(0,0x5d47c);
            else {SL(0,0x5d406);SW(0x7c,rank(a,e,0x2cfd00));}
        }
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,true);
    case 0x5d502:SL(0x30,0);SL(0x34,0);SL(0x50,0x5e918);PS(0x262e58);SL(0,0x5d528);
        /* fall through */
    case 0x5d528:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d732);
        else if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,true);
    case 0x5d552:SL(0x30,0);SL(0x34,0);PS(0x262c64);SL(0,0x5d570);
        /* fall through */
    case 0x5d570:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d596);
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,false);
    case 0x5d596:SL(0x30,0);SL(0x34,0);SL(0x50,0x5e930);PS(0x262cac);SL(0,0x5d5bc);
        /* fall through */
    case 0x5d5bc:
        k5e234(a,e);PA();
        if (k5e67e(a,e)) SL(0,0x5d6e2);
        if (k5e35c(a,e)) SL(0,0x5d5f4);
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,false);
    case 0x5d5f4:SL(0x30,0);SL(0x34,0);SL(0x50,0x5e918);SB(0x83,rank(a,e,0x2cfdc4));PS(0x262d2c);SL(0,0x5d63e);goto d63e;
    case 0x5d628:PS(0x262d86);SL(0,0x5d63e);
        /* fall through */
    case 0x5d63e:
    d63e:
        k5e234(a,e);PA();
        if (r && S16(W(0x7c))<=0) {
            SB(0x83,B(0x83)-1u);
            if ((int8_t)B(0x83)>0) SL(0,0x5d628);
            else {SL(0,0x5d696);SW(0x7c,rank(a,e,0x2cfd00));}
        }
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,false);
    case 0x5d696:SL(0x30,0);SL(0x34,0);SL(0x50,0x5e930);PS(0x262da8);SL(0,0x5d6bc);
        /* fall through */
    case 0x5d6bc:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d596);
        if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,false);
    case 0x5d6e2:SL(0x30,0);SL(0x34,0);SL(0x50,0x5e918);PS(0x262cee);SL(0,0x5d708);
        /* fall through */
    case 0x5d708:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d732);
        else if (k5e244(a)) SL(0,0x5d27a);
        return p_tail(a,e,false);
    case 0x5d732:
        SL(0x50,0x5e918);child(a,e,0x5dd2a,0x1c,0,0x18);SL(0,0x5d764);
        /* fall through */
    case 0x5d764:
        /* JMP 0B0632: the node becomes a soldier (the world moves it). */
        if (!e->become_soldier || !e->become_soldier(e->ctx,a,0xb0632)) {ga_fail(e,0xb0632);return GA_UNSUPPORTED;}
        return GA_MORE;
    /* ---- 05D76A: hops ---- */
    case 0x5d76a:
        k5e17c(a,e);ga_floor(a,e);
        if (!k5e0b8(a,e)) return GA_UNSUPPORTED;
        /* fall through */
    case 0x5d778: {
        uint32_t row=0x2cfeaau+12u*(rank(a,e,0x2cfe90)&3u);
        SL(0x34,rl(e,row));SL(0x76,rl(e,row+4u));
        uint32_t d0=rl(e,row+8u);if (!BIT(0x15,0)) d0=0u-d0;
        SL(0x30,d0);PS(0x262ec8);SL(0,0x5d7ce);
    }   /* fall through */
    case 0x5d7ce:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x76));PA();
        if (BIT(0x15,6)) SL(0,0x5d7f6);
        return p_tail(a,e,false);
    case 0x5d7f6:k1b1dc(a);PS(0x262f06);SL(0,0x5d812);
        /* fall through */
    case 0x5d812:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d83e);
        if (k5e244(a)) SL(0,0x5d778);
        return p_tail(a,e,false);
    case 0x5d83e:
        k1b1dc(a);SW(0x7a,rank(a,e,0x2cfef4));
        if (k5e2e4(a,e)) {
            uint16_t d0=ga_rand(e)&0xffu,d1=W(0x7a)&0xffu;
            if (S16(d1)>S16(d0)) GO(0x5d8ca);
        }
        SW(0x7a,rank(a,e,0x2cfeda));PS(0x262ad8);SL(0,0x5d89a);
        /* fall through */
    case 0x5d89a:
        k5e234(a,e);PA();
        SW(0x7a,W(0x7a)-1u);if (S16(W(0x7a))<=0) SL(0,0x5d778);
        if (k5e244(a)) SL(0,0x5d778);
        return p_tail(a,e,false);
    case 0x5d8ca:
        k1b1dc(a);SB(0x89,BIT(0x15,0)?0:8);k5e800(a,e);PS(0x26328e);SL(0,0x5d8fe);
        /* fall through */
    case 0x5d8fe:
        k5e234(a,e);PA();
        if (r) SL(0,0x5d778);
        return p_tail(a,e,false);
    /* ---- 05D916: parachutist ---- */
    case 0x5d916:k5e186(a,e);CLR(0x15,5);goto d92e;
    case 0x5d924:k5e186(a,e);SET(0x15,5);
    d92e: {
        SW(0x4e,1);SL(0x50,0x5e924);ga_palette(a,0x238);SW(0x66,0x8820);SW(0x1e,0x8820);
        uint32_t d0=0u-((uint32_t)(int32_t)S16(rank(a,e,0x2cff70))<<8);
        SL(0x34,d0);SL(0x62,d0);
        uint32_t n;nearest(a,e,&n);SL(0x6e,n);
        SB(0x88,0);SB(0x8b,8);SB(0x8c,0xf0);
        child(a,e,0x5dda4,0x0e,0,0);
    }   /* fall through */
    case 0x5d9ac:SW(0x7c,rank(a,e,0x2cff0e));SB(0x86,0);PS(0x26333e);SL(0,0x5d9d8);
        /* fall through */
    case 0x5d9d8:
        if (k5e19c(a,e)) SL(0,0x5da68);
        PA();
        SW(0x7c,W(0x7c)-1u);if (S16(W(0x7c))<0) SW(0x7c,0);
        if (k5e388(a,e)) {SB(0x86,1);SL(0,0x5da16);}
        return p_tail_air(a,e);
    case 0x5da16:
        SB(0x86,1);k5e7d8(a,e);
        PS(rl(e,0x5ea38u+4u*((B(0x88)>>1)&0xfu)));SL(0,0x5da46);
        /* fall through */
    case 0x5da46:
        if (k5e19c(a,e)) SL(0,0x5da68);
        PA();
        if (r) SL(0,0x5d9ac);
        return p_tail_air(a,e);
    case 0x5da68:
        if ((int8_t)B(0x88)>8) SET(0x15,0);
        k1b1dc(a);SB(0x86,3);PS(0x263498);SL(0,0x5da9a);
        /* fall through */
    case 0x5da9a:
        ga_air(a,e);PA();
        if (r) SL(0,0x5c5b4);
        return p_tail_air(a,e);
    /* ---- deaths and deletion ---- */
    case 0x5db18:case 0x5db42:return p_death(a,e,0x18,false,false,0x5db42);
    case 0x5db92:case 0x5dbbc:return p_death(a,e,0x10,true,false,0x5dbbc);
    case 0x5dc32:case 0x5dc70:return p_death(a,e,0x18,false,true,0x5dc70);
    case 0x5dc7a:return GA_DELETE;                                      /* 004188 (after the cartridge check) */
    case 0x5dd0a:return GA_DELETE_TREE;                                 /* 0041AC */
    /* ---- pieces ---- */
    case 0x5dd12:SW(0x66,0x1001);SW(0x1e,0x1001);SB(0x1d,0);SL(0,0x1c706);return neo_effects_tick(a,e);   /* JMP 01C706 */
    case 0x5dd2a:
        SL(0x34,0x4cccc);SL(0x76,0xffff851fu);SL(0x30,0xffff3334u);ga_palette(a,0x238);PS(0x263ad0);SL(0,0x5dd66);
        /* fall through */
    case 0x5dd66:
        ga_air(a,e);
        if (!BIT(0x15,6)) SL(0x34,L(0x34)+L(0x76)); else SL(0,0x5dd0a);
        PA();
        if (ga_cull_rect(a,e,0x5e9a8)) SL(0,0x5dd0a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x5dda4: {
        ga_palette(a,0x38d);SL(0x50,0x5e954);SW(0x4e,0x14);
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);                      /* 01AD6E */
    }   /* fall through */
    case 0x5ddca:PS(0x2634e0);SL(0,0x5dde0);
        /* fall through */
    case 0x5dde0: {
        if (!ga_creator_alive(a,e)) return GA_DELETE_TREE;              /* 00409A, JMP 0041AC */
        ga_project(a,e);
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x5ddf0);return GA_UNSUPPORTED;}
        SW(0x1e,ga_w(p,0x1e)-1u);
        if (ga_b(p,0x86)==1u) SL(0,0x5de22);
        SB(0x88,ga_b(p,0x88));SW(0x1e,W(0x1e)-1u);
        PA();
        goto deda;
    }
    case 0x5de22: {
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x5de22);return GA_UNSUPPORTED;}
        PS(rl(e,0x5ea5cu+4u*((ga_b(p,0x88)>>1)&0xfu)));SL(0,0x5de4c);
    }   /* fall through */
    case 0x5de4c: {
        if (!ga_creator_alive(a,e)) return GA_DELETE_TREE;
        ga_project(a,e);
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x5de5c);return GA_UNSUPPORTED;}
        SW(0x1e,ga_w(p,0x1e)-1u);
        if (!ga_b(p,0x86)) SL(0,0x5ddca);
        PA();
        goto deda;
    }
    deda: {
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x5deda);return GA_UNSUPPORTED;}
        if (ga_b(p,0x86)==3u) {SL(0,0x5de84);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
        if (ga_b(p,0x86)==2u) {SL(0,0x5de98);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
        ga_register(a,e,NULL,NULL);
        if (BIT(0x5f,7)) {ga_sb(p,0x5a,B(0x5a));ga_sb(p,0x5f,ga_b(p,0x5f)|0x80u);SL(0,0x5de98);}
        SB(0x5a,0);
        if (!ga_creator_alive(a,e)) SL(0,0x5dd0a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x5de84:PS(0x2638b0);goto dea8;
    case 0x5de98:PS(0x26395c);
    dea8:
        SET(0x5f,3);to_root(a,e);SL(0x50,0);SL(0,0x5dec2);              /* 01AD52 */
        /* fall through */
    case 0x5dec2:
        ga_project(a,e);PA();
        if (r) SL(0,0x5dd0a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x5df3e:
        ga_palette(a,0x204);SW(0x66,0x1001);SW(0x1e,0x1001);PS(0x263a2e);SL(0,0x5df6e);
        /* fall through */
    case 0x5df6e:
        ga_project(a,e);PA();
        if (r) SL(0,0x5dd0a);
        if (ga_cull_rect(a,e,0x5e9a8)) SL(0,0x5dd0a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x5df9a: {
        uint32_t d0=(uint32_t)(int32_t)S16(rank(a,e,0x2cfdde))<<8;
        if (!BIT(0x15,0)) d0=0u-d0;
        SL(0x30,d0);SW(0x34,0);
        goto dff8;
    }
    case 0x5dfc2: {
        int16_t d0=S16(rank(a,e,0x2cfdde));
        uint8_t ang=(uint8_t)((B(0x89)&0xfu)<<4);
        SL(0x30,(uint32_t)((int32_t)ga_cosine(ang)*d0));SL(0x34,(uint32_t)((int32_t)ga_sine(ang)*d0));
    }
    dff8:
        SW(0x7a,0x96);ga_palette(a,0x37b);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        sound(e,0x1064);PS(0x263a00);SL(0,0x5e03e);
        /* fall through */
    case 0x5e03e:
        ga_air(a,e);
        if (!B(0x92)) {
            SB(0x92,neo_gate_splash(a,e,0)?0xff:0);                      /* 03C5C6 (the splash 03C962), SCS */
        }
        if (BIT(0x15,7)) {
            if (B(0x92)) return GA_DELETE;
            GO(0x5dd12);
        }
        PA();
        SW(0x7a,W(0x7a)-1u);if (S16(W(0x7a))<=0) SL(0,0x5dd12);
        if (ga_attack(a,e,0x5e960)) SL(0,0x5dd12);
        if (ga_cull_rect(a,e,0x5e9a8)) SL(0,0x5dd0a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
#undef PS
#undef PA
#undef GO
/* ---- flying machine 04E990 and its pieces ------------------------------------- */
/* 04E990..04FCA0 (script records 2DAB30/2DAB44/2DAB52, section 2; tables
 * 0E10A2..0E11AA, rank tables 2D36A4..2D3840):
 * - 04E990 / 04E998 (+8E bit 1: no floor piece): 009F88, 6001, palette A6,
 *   flash palette AA, +50 = 0E10DE, 247730, +82 = B0 + random(30), +4E =
 *   rank 2D36A4, +72 = rank 2D3768, the pieces 04EF88 (pilot), 04EE18, 04F2DC
 *   and 04EE88 (the floor: dynamic map 0E112E), sound 105E. 04EA86: 04FADE
 *   (x speed toward the target / patrol between +6E and +70 once the pilot
 *   is hit: +8E bit 0), 01AC38, 01AD8C, the program; while +61 is clear and
 *   a player lives, every +72 updates the bomb 04F358 and the missiles
 *   04F464 (23, 16) and 04F44E (1D, 7); 004C24, a hit -> 04EB36 (explosions
 *   01C706 every 4 updates until x >= B0, 04EBCE, 10 updates, 04EC0C: six
 *   pieces 04EC8C, the explosion 01C71E (0, -8), the attack row 0E111E, +8E
 *   bit 1, +8F = FF, sound 1021, 04EC78 falls); 04F7E8 (01945C 0E10C6: +60
 *   = FF and deleted).
 * - Pieces read the creator: +8C (the frame of 247730, offsets 0E1140), +8F
 *   (04F960: deleted), +60 (04F94C: deleted), +17 (0044F6). 04EF88 (pilot,
 *   palette A7 / B9 by 01B2BE, +50 = 0E10EA): a hit sets the creator's +8E
 *   bit 0, 24790C, then 04F152 / 04F1C8 (hit again) with +50 = 0E10F6.
 * - Missiles 04F44E / 04F464 (8802 / 6000, sound 1067): 004C1C, palette A9,
 *   247C50, +50 = 0E1102, flash 78, +A8 = 300, +74 = C, +4E = rank 2D3706,
 *   01AD52, the path 0E1168 (021288 with its calls 04F97A..04F9B6), +76 =
 *   01B04A; at the path's end 04F5FE (247D16, +72 = rank 2D382C, 04F9FA
 *   steers +84 by 5 toward the target (y + 10), speed rank 2D37CA by 002E7C,
 *   0143E4); they explode (+17 = FF, sound 1022, 01C706) on +5F bit 0, the
 *   attack row 0E110E, a hit or the creator hit; smoke 04F720 every 4
 *   updates (-10, 0); 01945C 0E10CE deletes.
 * - 04F358 (bomb, E0ED, palette A8, 247A76 at (0, 4)), 04EC8C (pieces:
 *   0E10A2 / 0E10BE by +88, 01DEA2, random speeds, gravity -4444).
 * The cartridge checks before 004188 are not reproduced. */
static uint8_t rb8(neo_genv_t *e,uint32_t at) {const uint8_t *t=ga_rom(at);if (!t) {ga_fail(e,at);return 0;}return t[0];}
static neo_gactor_t *spawn_plain(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
static void depth8000(neo_gactor_t *a) {if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));}   /* 01AD8C */
static void flash_share(neo_gactor_t *a,neo_genv_t *e) {                                  /* 0044F6 */
    neo_gactor_t *p=sh_creator(a,e);
    if (!p) {ga_fail(e,0x44f6);return;}
    uint8_t f=ga_b(p,0x17);
    if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}
}
/* 04FADE */
static void k4fade(neo_gactor_t *a,neo_genv_t *e) {
    int32_t v=S32(L(0x30));
    if (B(0x61)) {
        if (S16(W(0x3c))<0x90) {v+=0x2000;if (v>0x60000) v=0x60000;}
        else if (v>0x30000) v-=0x4000; else v+=0x2000;
        SL(0x30,(uint32_t)v);return;
    }
    int16_t d0;
    if (!BIT(0x8e,0)) {
        uint32_t n;
        if (nearest(a,e,&n)) d0=0xa0;
        else d0=S16(px(e,n))>0xc0?0xc0:S16(px(e,n));
        d0=(int16_t)(d0+(int16_t)((ga_rand(e)&7u)-4u));
    } else {
        SW(0x6e,0x20);SW(0x70,0xc0);
        if (!W(0x86)) {
            if (S16(absw((uint16_t)(W(0x82)-W(0x3c))))<=5) {
                int16_t r=(int16_t)(ga_rand_range(e,0x30)+0x10u);
                if (S16(W(0x82))>=0xa0) r=(int16_t)-r;
                SW(0x82,(uint16_t)(r+0xa0));SW(0x86,0x10);
            }
        } else SW(0x86,W(0x86)-1u);
        d0=S16(W(0x82));
    }
    int16_t x=S16(W(0x3c));
    if (d0>=x) {
        if (S16(W(0x70))>=x) {v+=0x2000;if (v>=0x30000) v=0x30000;}
        else {v-=0x2000;if (v<=0) v=0;}
    } else {
        if (S16(W(0x6e))<=x) {v-=0x2000;if (v<=(int32_t)0xfffd0000) v=(int32_t)0xfffd0000;}
        else {v+=0x2000;if (v>=0) v=0;}
    }
    SL(0x30,(uint32_t)v);
}
/* 04F7E8 / 04F88E / 04F8AC: 01945C deletes (+60 = FF first but in 04F8AC). */
static bool pod_cull(neo_gactor_t *a,neo_genv_t *e,uint32_t rect,bool mark) {
    if (!ga_cull(a,e,rect)) return false;
    if (mark) SB(0x60,0xff);
    return true;
}
static ga_result_t pod_end(neo_gactor_t *a,neo_genv_t *e,uint32_t rect,bool mark) {
    if (pod_cull(a,e,rect,mark)) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04F960 (creator +8F) / 04F94C (creator +60). */
static int creator_byte(neo_gactor_t *a,neo_genv_t *e,unsigned o) {
    neo_gactor_t *p=sh_creator(a,e);
    if (!p) {ga_fail(e,0x4f94c);return -1;}
    return ga_b(p,o);
}
static int16_t offset8c(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=sh_creator(a,e);
    if (!p) {ga_fail(e,0x4eea2);return 0;}
    return S16(rw(e,0xe1140u+2u*ga_b(p,0x8c)));
}
static void path_move4(neo_gactor_t *a,uint16_t d0,uint8_t d1) {
    int32_t d2=(int32_t)S16(d0)*ga_sine(d1),d3=(int32_t)S16(d0)*ga_cosine(d1);
    SL(0x30,(uint32_t)d3);SL(0x34,(uint32_t)d2);
    SL(0x28,L(0x28)+(uint32_t)d3);SL(0x2c,L(0x2c)+(uint32_t)d2);
}
/* The path calls of 0E1168. */
static bool pod_path_call(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    switch (routine) {
    case 0x4f97a:return ga_anim_start(a,e,0x247afe,0x100);
    case 0x4f98c:return ga_anim_start(a,e,0x247b18,0x100);
    case 0x4f99e:SW(0xa8,0x500);return ga_anim_start(a,e,0x247baa,0x100);
    case 0x4f9b6:SW(0xa8,0x500);return ga_anim_start(a,e,0x247c50,0x100);
    case 0x4f9ce:return ga_anim_start(a,e,0x247c6a,0x100);
    case 0x4f9e0:return ga_anim_start(a,e,0x247cd4,0x100);
    case 0x4f9f2:SW(0xa8,0x180);return true;
    }
    ga_fail(e,routine);return false;
}
/* 021288 (as neo_heli.c), op 8 calling the routines above. Carry: ended. */
static bool pod_path(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    uint16_t d5=W(0xa8);
    for (unsigned guard=0;guard<256;guard++) {
        uint32_t at=L(0xa0);
        if (!ga_rom(at)) {ga_fail(e,at);return false;}
        uint8_t op=rb8(e,at);
        if (op>=0x0b) {ga_fail(e,0x212a4);return false;}
        switch (op) {
        case 0:SW(0xaa,(uint16_t)rb8(e,at+1)<<8);SL(0xa0,at+2);break;
        case 1:case 2: {
            if (!d5) return false;
            uint32_t d0=op==1?((uint32_t)rw(e,at+2)<<16):((uint32_t)rw(e,at+4)<<8);
            d0-=L(0xa4)&0xffffff00u;
            if (d0<0x1000000u && d5>=(uint16_t)(d0>>8)) {
                uint16_t step=(uint16_t)(d0>>8);
                d5=(uint16_t)(d5-step);
                SB(0xa6,0);SW(0xa4,0);
                path_move4(a,step,B(0xaa));
                if (op==1) SL(0xa0,at+4);
                else {
                    SB(0xaa,B(0xaa)+rb8(e,at+1));
                    if (B(0xaa)==rb8(e,at+2)) SL(0xa0,at+6);
                }
                break;
            }
            SL(0xa4,L(0xa4)+((uint32_t)d5<<8));
            path_move4(a,d5,B(0xaa));
            return false;
        }
        case 3:
            SW(0xa4,W(0xa4)+1u);
            if (rw(e,at+2)>W(0xa4)) return false;
            SW(0xa4,0);SL(0xa0,at+4);
            return false;
        case 4:SL(0xa0,rl(e,at+2));break;
        case 5: {uint16_t o=rw(e,at+2);if (o>=0xb0) {ga_fail(e,0x21396);return false;}SL(o,rl(e,at+4));SL(0xa0,at+8);break;}
        case 6: {uint16_t o=rw(e,at+4);if (o>=0xb0) {ga_fail(e,0x21376);return false;}SW(o,rw(e,at+2));SL(0xa0,at+6);break;}
        case 7: {uint16_t o=rw(e,at+2);if (o>=0xb0) {ga_fail(e,0x21356);return false;}SB(o,rb8(e,at+1));SL(0xa0,at+4);break;}
        case 8:
            SL(0xa0,at+6);
            if (!pod_path_call(a,e,rl(e,at+2))) return false;
            break;
        case 9:sound(e,rw(e,at+2));SL(0xa0,at+4);break;
        case 10:path_move4(a,d5,B(0xaa));return true;
        }
    }
    ga_fail(e,0x21296);return false;
}
/* The missiles' common tail (04F53E.. / 04F664..). */
/* `homing`: 04F6F2 sets 01C706 for the creator's hit and runs 04FA8E all the
 * same (no BRA, unlike 04F5EA). */
static ga_result_t missile_tail(neo_gactor_t *a,neo_genv_t *e,bool homing) {
    if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
    bool boom=BIT(0x5f,0) || ga_attack(a,e,0xe110e),smoke=false;
    if (!boom && ga_register(a,e,NULL,NULL)) {
        if (BIT(0x5f,7)) boom=true; else SB(0x5a,0);
    }
    if (!boom) {
        int c=creator_byte(a,e,0x5f);
        if (c<0) return GA_UNSUPPORTED;
        boom=(c&0x80)!=0;smoke=homing;
    }
    if (boom) {SB(0x17,0xff);sound(e,0x1022);SL(0,0x1c706);}
    if (!boom || smoke) {                                             /* 04FA8E */
        SW(0x74,W(0x74)-1u);
        if (!W(0x74)) {
            neo_gactor_t *c=spawn_plain(a,e,0x4f720,0x2a);
            if (c) {ga_place_at(c,a,e,-0x10,0);ga_sl(c,0x30,L(0x30));}
            SW(0x74,4);
        }
    }
    return pod_end(a,e,0xe10ce,false);
}
static ga_result_t pod_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r,c;
    switch (L(0)) {
    case 0x4e990:SB(0x8e,0);goto p9a2;
    case 0x4e998:SB(0x8e,0);SET(0x8e,1);
    p9a2:
        SB(0x1d,0);SW(0x66,0x6001);SW(0x1e,0x6001);ga_palette(a,0xa6);ga_flash_palette(a,0xaa);
        SL(0x50,0xe10de);START(0x247730);
        SB(0x61,0);SB(0x60,0);SB(0x8f,0);
        SW(0x82,ga_rand_range(e,0x30)+0xb0u);SW(0x86,0);
        ga_project(a,e);
        SW(0x4e,rank(a,e,0x2d36a4));SW(0x72,rank(a,e,0x2d3768));
        spawn_plain(a,e,0x4ef88,0x1c);spawn_plain(a,e,0x4ee18,0x1c);spawn_plain(a,e,0x4f2dc,0x1c);
        if (!BIT(0x8e,1)) spawn_plain(a,e,0x4ee88,0x1c);
        sound(e,0x105e);SL(0,0x4ea86);
        /* fall through */
    case 0x4ea86: {
        k4fade(a,e);ga_move(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        uint32_t n;
        if (!B(0x61) && !nearest(a,e,&n)) {
            SW(0x72,W(0x72)-1u);
            if (!W(0x72)) {
                SW(0x72,rank(a,e,0x2d3768));
                spawn_plain(a,e,0x4f358,0x1c);
                neo_gactor_t *m=spawn_plain(a,e,0x4f464,0x1c);
                if (m) ga_place_at(m,a,e,0x23,0x16);
                m=spawn_plain(a,e,0x4f44e,0x1c);
                if (m) ga_place_at(m,a,e,0x1d,7);
            }
        }
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0x4eb36);
        return pod_end(a,e,0xe10c6,true);
    }
    case 0x4eb36:SL(0x50,0);SB(0x61,0xff);SW(0x72,0);SL(0,0x4eb4e);
        /* fall through */
    case 0x4eb4e:
        if (!W(0x72)) {
            SW(0x72,4);
            neo_gactor_t *x=spawn_plain(a,e,0x1c706,0x2a);
            if (x) {
                int16_t dx=(int16_t)((ga_rand(e)&0x3fu)-0x20u),dy=(int16_t)(ga_rand(e)&0x1fu);
                ga_place(x,e,dx,dy,NEO_GA_TAG(a->index));                 /* 01AD6E */
            }
        } else SW(0x72,W(0x72)-1u);
        k4fade(a,e);ga_move(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (S16(W(0x3c))>=0xb0) SL(0,0x4ebce);
        return pod_end(a,e,0xe10c6,true);
    case 0x4ebce:SW(0x72,0x10);SL(0,0x4ebda);
        /* fall through */
    case 0x4ebda:
        k4fade(a,e);ga_move(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (!W(0x72)) SL(0,0x4ec0c); else SW(0x72,W(0x72)-1u);
        return pod_end(a,e,0xe10c6,true);
    case 0x4ec0c: {
        for (int d7=5;d7>=0;d7--) {
            neo_gactor_t *p=spawn_plain(a,e,0x4ec8c,0x2a);
            if (p) ga_sw(p,0x88,(uint16_t)d7);
        }
        neo_gactor_t *x=spawn_plain(a,e,0x1c71e,0x2a);
        if (x) ga_place(x,e,0,-8,NEO_GA_TAG(a->index));
        ga_attack(a,e,0xe111e);
        SET(0x8e,1);SB(0x8f,0xff);sound(e,0x1021);SL(0,0x4ec78);
    }   /* fall through */
    case 0x4ec78:
        SL(0x30,L(0x30)-0x10000u);ga_move(a,e);
        return pod_end(a,e,0xe10d6,true);
    case 0x4ec8c: {
        SB(0x1d,0);
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,-8,p?NEO_GA_TAG(p->index):0);                     /* 01AD6E */
        unsigned k=W(0x88);
        ga_palette(a,rb8(e,0xe10beu+k)?0x20b:0x20a);
        START(rl(e,0xe10a2u+4u*k));
        if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);                       /* 01DEA2 */
        SL(0x34,0x80000);SL(0x7e,0xffffbbbcu);SL(0x30,0x20000);
        uint16_t d=ga_rand(e);
        uint32_t v=(uint32_t)(d&7u)<<15;if (!(d&1u)) v=0u-v;
        SL(0x30,v);
        SL(0x34,L(0x34)-((uint32_t)(ga_rand(e)&0xfu)<<15));
        if (S16(W(0x34))>0xfb) SW(0x34,0xfb);
        if (S32(L(0x7e))>(int32_t)0xfffff000) SL(0x7e,0xfffff000u);
        SET(0x15,5);SL(0,0x4ed6c);
    }   /* fall through */
    case 0x4ed6c:
        ga_move(a,e);SL(0x34,L(0x34)+L(0x7e));
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return pod_end(a,e,0xe10c6,true);
    case 0x4ee18: {
        SB(0x1d,0);SW(0x66,0x6001);SW(0x1e,0x6001);ga_palette(a,0xa6);ga_flash_palette(a,0xaa);
        START(0x2477c2);
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);SL(0,0x4ee62);
    }   /* fall through */
    case 0x4ee62:
        ga_project(a,e);flash_share(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        goto alive;
    case 0x4ee88: {
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);SW(0x8a,0);SL(0,0x4eea0);
    }   /* fall through */
    case 0x4eea0: {
        int16_t d0=offset8c(a,e);
        SW(0x8a,(uint16_t)d0);SW(0x2c,W(0x2c)+(uint16_t)d0);ga_project(a,e);
        if ((c=creator_byte(a,e,0x8f))<0 || creator_byte(a,e,0x8e)<0) return GA_UNSUPPORTED;
        if (c || (creator_byte(a,e,0x8e)&2)) {dyn_release(a,e);return GA_DELETE;}   /* 013F1E */
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe112e,0,0);                /* 0140C0 */
        SW(0x2c,W(0x2c)-W(0x8a));
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4ef88: {
        SB(0x1d,0);SW(0x66,0x6001);SW(0x1e,0x6001);SL(0x50,0xe10ea);
        ga_palette(a,e->blood?0xa7:0xb9);                               /* 01B2BE */
        START(0x247854);SB(0x61,0);SB(0x60,0);SW(0x4e,1);
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);SL(0,0x4f004);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4f004:
        if ((c=creator_byte(a,e,0x8c))<0) return GA_UNSUPPORTED;
        SB(0x8c,c);ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if ((c=creator_byte(a,e,0x8f))<0) return GA_UNSUPPORTED;
        if (c) return GA_DELETE;
        if (BIT(0x5f,7)) SL(0,0x4f04c);
        goto gone;
    case 0x4f04c: {
        START(0x24790c);
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x4f05c);return GA_UNSUPPORTED;}
        ga_sb(p,0x8e,ga_b(p,0x8e)|1u);
        SL(0x50,0);sound(e,0x103f);SL(0,0x4f07e);
    }   /* fall through */
    case 0x4f07e: {
        int16_t d0=offset8c(a,e);uint32_t keep=L(0x2c);
        SW(0x2c,W(0x2c)+(uint16_t)d0);ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x4f152);
        SL(0x2c,keep);
        goto alive;
    }
    case 0x4f152:START(0x2478b0);SB(0x5a,0);CLR(0x5f,7);SW(0x4e,1);SL(0x50,0xe10f6);SL(0,0x4f180);
        /* fall through */
    case 0x4f180:
        if ((c=creator_byte(a,e,0x8c))<0) return GA_UNSUPPORTED;
        SB(0x8c,c);ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0x4f1c8);
        goto alive;
    case 0x4f1c8:START(0x247986);SB(0x5a,0);CLR(0x5f,7);SW(0x4e,1);SL(0,0x4f1ee);
        /* fall through */
    case 0x4f1ee: {
        int16_t d0=offset8c(a,e);uint32_t keep=L(0x2c);
        SW(0x2c,W(0x2c)+(uint16_t)d0);ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x4f152);
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);CLR(0x5f,7);SL(0,0x4f1c8);}
        SL(0x2c,keep);
        goto alive;
    }
    case 0x4f2dc: {
        SB(0x1d,0);SW(0x66,0x6001);SW(0x1e,0x6001);ga_palette(a,0xab);START(0x2479c4);SB(0x61,0);SB(0x60,0);
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);SL(0,0x4f32e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4f32e:
        if ((c=creator_byte(a,e,0x8c))<0) return GA_UNSUPPORTED;
        SB(0x8c,c);ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        goto alive;
    case 0x4f358: {
        SB(0x1d,0);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_palette(a,0xa8);START(0x247a76);SB(0x61,0);SB(0x60,0);
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,4,p?NEO_GA_TAG(p->index):0);SL(0,0x4f3aa);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4f3aa:
        if (B(0x60)) {SL(0x30,L(0x30)-0x12000u);if (S32(L(0x30))<(int32_t)0xfff60000) SL(0x30,0xa0000);}   /* 04FC78 */
        ga_move(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4f44e:SW(0x66,0x8802);SW(0x1e,0x8802);SW(0x88,0);goto f480;
    case 0x4f464:SW(0x66,0x6000);SW(0x1e,0x6000);SW(0x88,1);sound(e,0x1067);
    f480:
        SB(0x5e,B(0x5e)|0x80u);                                          /* 004C1C */
        SB(0x1d,0);ga_palette(a,0xa9);START(0x247c50);SL(0x50,0xe1102);ga_flash_palette(a,0x78);
        SB(0x61,0);SB(0x60,0);SW(0xa8,0x300);SW(0x74,0x0c);SB(0x90,0);
        SW(0x4e,rank(a,e,0x2d3706));
        to_root(a,e);                                                    /* 01AD52 */
        SL(0xa0,0xe1168);SW(0xa4,0);SB(0xa6,0);
        SL(0x76,k1b04a(e));SL(0,0x4f50e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4f50e:
        if (pod_path(a,e)) {SB(0x60,0xff);CLR(0x15,6);CLR(0x15,7);SL(0,0x4f5fe);}
        if (e->unsupported) return GA_UNSUPPORTED;
        if (!B(0x61)) SW(0xa8,W(0xa8)-8u);
        return missile_tail(a,e,false);
    case 0x4f5fe:START(0x247d16);SW(0x84,0);SB(0x8d,0);SL(0x30,0);SL(0x34,0);SW(0x72,rank(a,e,0x2d382c));SL(0,0x4f636);
        /* fall through */
    case 0x4f636: {
        uint32_t n;
        if (!nearest(a,e,&n) && W(0x72)) {                              /* 04F9FA */
            SW(0x72,W(0x72)-1u);
            uint16_t ang=neo_angle((int16_t)(px(e,n)-W(0x3c)),(int16_t)(py(e,n)+0x10u-W(0x40)));
            int16_t d0=(int16_t)(ang-W(0x84));
            if (d0<0) {d0=(int16_t)-d0;SW(0x84,W(0x84)+(d0>=0x80?5u:0xfffbu));}
            else if (d0) SW(0x84,W(0x84)+(d0>=0x80?0xfffbu:5u));
            SW(0x84,W(0x84)&0xffu);
        }
        uint32_t vx,vy;
        ga_polar((uint32_t)((int32_t)S16(rank(a,e,0x2d37ca))<<8),(uint8_t)W(0x84),&vx,&vy);   /* 002E7C */
        SL(0x30,vx);SL(0x34,vy);
        ga_air(a,e);
        if (!B(0x90) && S16(W(0x40))>=0x160) {SB(0x61,0xff);SW(0x66,0x8802);SW(0x1e,0x8802);}
        return missile_tail(a,e,true);
    }
    case 0x4f720:
        SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x37c);START(0x247da8);
        if (!(ga_rand(e)&1u)) SET(0x15,0);
        SB(0x61,0);SB(0x60,0);
        SL(0x30,ASR(L(0x30),1)+0xfff60000u);SL(0x34,0);SL(0x7a,0xffffa000u);SL(0,0x4f794);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4f794:
        if (!B(0x61)) {
            neo_gactor_t *p=sh_creator(a,e);
            if (!p) {ga_fail(e,0x4f79c);return GA_UNSUPPORTED;}
            if (!ga_b(p,0x60)) SW(0x1e,ga_w(p,0x1e));
            else {SB(0x61,0xff);SW(0x66,0x1001);SW(0x1e,0x1001);}
        }
        SL(0x30,L(0x30)+L(0x7a));ga_move(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return pod_end(a,e,0xe10c6,true);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
alive:                                                                   /* 04F960 */
    if ((c=creator_byte(a,e,0x8f))<0) return GA_UNSUPPORTED;
    if (c) return GA_DELETE;
gone:                                                                    /* 04F94C */
    if ((c=creator_byte(a,e,0x60))<0) return GA_UNSUPPORTED;
    if (c) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- hovering gunship 097FCC and its pieces ------------------------------------ */
/* 097FCC..099070 (script records of sections 2 and 4; tables 0E608A..0E61BC,
 * rank tables 2D2040/2D2104/2D2166). The script words: +6E base x, +70 timer
 * (0: none), +72 missile speed, +74 missile retarget period, +76 offset row
 * of 0E619C (through +7C), +78 count of ships, +7A bombs (0: 1).
 * - 097FCC: sound 10BA, +8C bit 1, 0985A0 (ships 2..+78 as siblings 097FE4
 *   with the words and +69, +7C = index; TRAP #15 past 5). 097FE8: the
 *   hurtbox node 098E4A, 09849C (+82 = +6E + rand & F, +84 = +70 (+ (rand >>
 *   4) & F), +7C = +72, +88 = +74, +86 = +76, +8A = +7A or 1, +76 = +7C, +92 =
 *   rand; the rotor 098886; +4E / +7A / +7E = ranks 2D2040 / 2D2104 / 2D2166;
 *   palette 23B, flash 30C, 8838, faces left (+15 bit 0), +50 = 0E609A, the
 *   exhaust 01DCCE (-10), +78 = +4E / 2, 098664).
 * - 098012 (hover): with bombs left (+8A) and not ready (+8D = 0) ->
 *   098114; else the program 29EE20 / 29ED1A (+8D), +80 = +82; 09805A: with +8D and a target the count +80 -> +8C bit 4 (bomb);
 *   bit 3 (timer +84 out) -> 09816E (leave); else 09865C (+92 -= 8: a new
 *   point 098664 on the borrow), 098704 (toward +72/+74 at +7A, doubled while
 *   +91, eased 1/8), 09861E (the bank angle +8E and pose +8F; 00427A).
 *   09809C: the bomb 09879C (+60 / +8D = FF when 0040BC made none), +8A--,
 *   program by +8D; 0980F0: +60 -> 098012 at once, else 098750 (slow);
 *   09810E: level (+8F + 1) & F <= 2 -> 098142 (29EF26, back to 098012 at
 *   its end). The programs 29ED1A / 29EE20 / 29EF26 set +8D = FF at their
 *   end (ready to bomb). 09816E: +30/+34 +=
 *   (-800 / -400) << 8 (x >= A0 flips x), +72/+74 = (800 / 200) + rand &
 *   1FF -> 0981EC (098770: eased 1/16).
 * - 098664: the target's x +- C8 (against RAM 10E9B8) and y +- 60 (against
 *   180), or (100, 180) without players; + rand (0..1F - F) + the row 0E619C
 *   [+76], clamped 09901E (x 30..170, y 130..230); +8C bit 0 (a hit from
 *   the front, 098E62) -> +91 |= 7.
 * - Tail 09842C: +84 -> bit 3, +91--, 004C24 (+91 |= 3 on a hit), +4E <= +78
 *   -> +91 |= 3, +5F bit 7 -> 0981F8, 01945C 0E60BE -> 09841E (bit 2, deleted).
 * - 0981F8 (death): rand >= 0: the debris 01DA40 0E614A (E0ED), +72/+74 =
 *   polar(rand & 1FF + 600, angle to (130, 80)) >> 8, 098288 (098770; y <
 *   120 -> 0982A4: sound 1023, bit 2, 098C58: six 098CB6 after the script
 *   actor at (rand & F - 7, rand >> 8 & F - 7) and 098E18; deleted); rand <
 *   0: 098346 (sound 1023, bit 2, 1001, 01C71E, debris 0E60CE, the pieces
 *   098C0C / 098C20 / 098C34 / 098C0C, the last flipped; deleted).
 * - 098886 (rotor): 01AD6E (0, 0), 01ACA2, 29EF7C, palette 23B, flash 30C;
 *   099050 (creator gone or its bit 2) deletes, +1E = creator's, 0044F6.
 * - 098E4A (hurtbox, +50 = 0E60B2): a hit sets +5A = 0 and, from the front
 *   ((+5B + 1) & 7 < 3), the creator's +8C bit 0.
 * - 09879C (bomb): 01AD6E (8, -10), 01ACA2, 29F032 (its call 098FE6: the
 *   missile 098974 after the script actor with the ship's +86/+88/+7C),
 *   palette 23C; its end sets the creator's +60.
 * - 098974 (missile): 29F06C, palette 23C, +34 = -((y - 100) << 16 >> 5),
 *   +50 = 0E60A6, E0EF, +7C += rand & 7F - 40 (min 40), the target 098B32
 *   (01B04A); 0989DE: +86 count, every +88 updates the target's (x, y + 10)
 *   -> 098BCA (speed +7C, eased 1/16); then 098A12 (angle of (+30, +34) >>
 *   11 folded to 40 +- 20) -> 098A62 (speed 80000, eased 1/8). 098A96: +90
 *   -= 40 -> smoke 098EA2 (20, 0); 004C24; the row 0E608A or +5F bit 7
 *   (sound 1022) -> 098AFE (009F88, +17 = 0, JMP 01C706); 019504 0E60C6.
 * - 098C0C / 098C20 / 098C34: programs 29F090 / 29F0E6 / 29F13C, palette
 *   23B, JMP 01D51A (neo_effects_frag_launch). 098CB6: +80 = random(7),
 *   program 0E6120, palette 0E613C, speeds (rand & FFF - 800) << 8, +72 =
 *   F000 + rand & 1FF - FF, +74 = 0; 098D48 eases (1/16, 1/32), 019504
 *   0E60C6. 098E18: 22CF2C, palette 204, +30 = -20000 -> 098D24.
 * - 098EA2 (smoke): 1001, palette 37C, 29F192, rand < 0 flips, +30 = min(0,
 *   +30 / 2 - A0000); 098F06: +30 -= 8000, 01AC38, deleted at the end or
 *   x <= -20.
 * The cartridge checks before 004188 are not reproduced. */
static neo_gactor_t *spawn_script(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return e->spawn?e->spawn(e->ctx,a,routine,true):NULL;}   /* 00411C */
/* 01DA40: the debris emitter 01D764 with the list `list` (+86). */
static neo_gactor_t *debris40(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764,0x1c,0,0);
    if (!c) return NULL;
    if (BIT(0x15,2)) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
    return c;
}
/* 01DCCE: the exhaust 01D86C with +70 = d0. */
static void exhaust(neo_gactor_t *a,neo_genv_t *e,int16_t d0) {
    neo_gactor_t *c=child(a,e,0x1d86c,0x1c,0,0);
    if (!c) return;
    ga_sb(c,0x94,1);ga_sw(c,0x6e,0);ga_sw(c,0x70,(uint16_t)d0);ga_sw(c,0x72,0x3f);ga_sw(c,0x74,0x1f);
}
/* 099050: carry (true) when the creator is gone or its +8C bit 2 is set. */
static bool gs_gone(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t **p) {
    *p=NULL;
    if (!ga_creator_alive(a,e)) return true;
    *p=sh_creator(a,e);
    if (!*p) {ga_fail(e,0x99050);return true;}
    return (ga_b(*p,0x8c)>>2)&1u;
}
/* 01ACA2: the creator's flip bits 0..1. */
static void k1aca2(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *p=sh_creator(a,e);SB(0x15,p?(ga_b(p,0x15)&3u):0u);}
/* 098B6E (nearest) / 098B32 (01B04A): carry (true) with a target in +6E. */
static bool gs_target(neo_gactor_t *a,neo_genv_t *e,bool random) {
    uint32_t n=L(0x6e);
    if (n && live(e,n,NULL)) return true;
    if (random) {
        unsigned d0=e->players.active_mask&0xffu;
        if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
        if (!d0) {SL(0x6e,0);return false;}
        SL(0x6e,d0==2u?P2_NODE:P1_NODE);return true;
    }
    if (nearest(a,e,&n)) {SL(0x6e,0);return false;}
    SL(0x6e,n);return true;
}
/* 098664 (and 0986A0 from 0986F4) */
static void k98664(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x,y;
    if (gs_target(a,e,false)) {
        uint32_t n=L(0x6e);
        int16_t tx=S16(px(e,n)),ty=S16(py(e,n));
        x=(int16_t)(tx+(tx<=S16(e->ref69b8)?0xc8:-0xc8));
        y=(int16_t)(ty+(ty<=0x180?0x60:-0x60));
    } else {x=0x100;y=0x180;}
    SW(0x72,(uint16_t)x);SW(0x74,(uint16_t)y);
    uint16_t r=ga_rand(e);
    int16_t d0=(int16_t)((r&0x1fu)-0x0f),d1=(int16_t)(((r>>8)&0x1fu)-0x0f);
    uint32_t row=0xe619cu+4u*W(0x76);
    d0=(int16_t)(d0+S16(rw(e,row))+x);d1=(int16_t)(d1+S16(rw(e,row+2u))+y);
    if (d0<0x30) d0=0x30;                                            /* 09901E */
    if (d0>0x170) d0=0x170;
    if (d1<0x130) d1=0x130;
    if (d1>0x230) d1=0x230;
    SW(0x72,(uint16_t)d0);SW(0x74,(uint16_t)d1);
    if (BIT(0x8c,0)) {CLR(0x8c,0);SB(0x91,B(0x91)|7u);}
}
/* 09865C */
static void k9865c(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t v=B(0x92);SB(0x92,v-8u);
    if (v<8u) k98664(a,e);
}
/* 098704: toward +72/+74 at +7A (x2 while +91), eased 1/8. */
static void k98704(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t ang=neo_angle((int16_t)(W(0x72)-W(0x3c)),(int16_t)(W(0x74)-W(0x40)));
    uint32_t sp=(uint32_t)(int32_t)S16(W(0x7a));
    if (B(0x91)) sp=(uint32_t)(int32_t)(int16_t)(W(0x7a)*2u);
    uint32_t vx,vy;ga_polar(sp<<8,ang,&vx,&vy);
    SL(0x30,L(0x30)+ASR(vx-L(0x30),3));SL(0x34,L(0x34)+ASR(vy-L(0x34),3));
    ga_move(a,e);
}
/* 098750 */
static void k98750(neo_gactor_t *a,neo_genv_t *e) {
    SL(0x30,L(0x30)+ASR(0u-L(0x30),3));SL(0x34,L(0x34)+ASR(0u-L(0x34),3));ga_move(a,e);
}
/* 098770 */
static void k98770(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t tx=(uint32_t)(int32_t)S16(W(0x72))<<8,ty=(uint32_t)(int32_t)S16(W(0x74))<<8;
    SL(0x30,L(0x30)+ASR(tx-L(0x30),4));SL(0x34,L(0x34)+ASR(ty-L(0x34),4));ga_move(a,e);
}
/* 09861E: the bank angle +8E eases (1/8) to 002EB4 of (+-80 by the sign of
 * +30, +34 >> 10) and +8F = (+8E + 4) >> 3 (the pose); 00427A. */
static int k9861e(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x=S16(W(0x30))<0?-0x80:0x80;
    int16_t y=(int16_t)(uint16_t)(S32(L(0x34))>>10);
    int8_t d=(int8_t)(uint8_t)(neo_angle(x,y)-B(0x8e));
    d=(int8_t)(d>>3);
    SB(0x8e,B(0x8e)+(uint8_t)d);
    SB(0x8f,(uint8_t)((uint8_t)(B(0x8e)+4u)>>3));
    return ga_anim_step(a,e,NULL);
}
/* 09842C */
static ga_result_t gs_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x84)) {SW(0x84,W(0x84)-1u);if (!W(0x84)) SET(0x8c,3);}
    if (B(0x91)) SB(0x91,B(0x91)-1u);
    if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SB(0x91,B(0x91)|3u);}
    if (W(0x4e)<=W(0x78)) SB(0x91,B(0x91)|3u);
    if (BIT(0x5f,7)) SL(0,0x981f8);
    if (ga_cull(a,e,0xe60be)) SL(0,0x9841e);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static void gs_program(neo_gactor_t *a,neo_genv_t *e) {ga_anim_start(a,e,B(0x8d)?0x29ed1a:0x29ee20,0x100);}
static bool bomb_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x98fe6u) return false;
    neo_gactor_t *m=spawn_script(a,e,0x98974),*p=sh_creator(a,e);
    if (m && p) {ga_sw(m,0x86,ga_w(p,0x86));ga_sw(m,0x88,ga_w(p,0x88));ga_sw(m,0x7c,ga_w(p,0x7c));}
    if (m) ga_place_from(m,a,e,0,0);
    return true;
}
/* 098A12: the missile's gliding angle. */
static uint8_t k98a12(neo_gactor_t *a) {
    int16_t x=(int16_t)((int16_t)(uint16_t)(S32(L(0x30))>>8)>>3),y=(int16_t)((int16_t)(uint16_t)(S32(L(0x34))>>8)>>3);
    uint8_t d0=neo_angle(x,y),d1=(d0&0x80u)?0xff:0;
    d0=(uint8_t)((d0^d1)-d1);
    d0=(uint8_t)(d0+0x40);
    uint8_t d2=(d0&0x80u)?0xff:0;
    d0=(uint8_t)((d0^d2)-d2);
    d0=(uint8_t)(d0-0x40);
    if (d0>0x20u) d0=0x20;
    d0=(uint8_t)(d0+0x40);
    d0=(uint8_t)((d0^d2)-d2);
    d0=(uint8_t)(d0-0x40);
    return (uint8_t)((d0^d1)-d1);
}
static ga_result_t gunship_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;
    switch (L(0)) {
    case 0x97fcc:
        sound(e,0x10ba);SET(0x8c,1);
        if (W(0x78)>5u) {ga_fail(e,0x985aa);return GA_UNSUPPORTED;}     /* TRAP #15 */
        for (uint16_t i=1;i<W(0x78);i++) {
            neo_gactor_t *c=spawn_plain(a,e,0x97fe4,0x0e);
            if (!c) continue;
            for (unsigned o=0x6e;o<0x78;o+=2) ga_sw(c,o,W(o));
            ga_sw(c,0x7a,W(0x7a));ga_sb(c,0x69,B(0x69));ga_sw(c,0x7c,i);
            ga_place_at(c,a,e,0,0);
        }
        SW(0x7c,0);
        goto fe8;
    case 0x97fe4:
    fe8:
        child(a,e,0x98e4a,0x1c,0,0);
        {   /* 09849C */
            uint16_t d0=W(0x6e),d1=W(0x70),d7=W(0x7c);
            SW(0x82,d0);SW(0x84,d1);SW(0x7c,W(0x72));SW(0x88,W(0x74));SW(0x86,W(0x76));
            SW(0x8a,W(0x7a)?W(0x7a):1u);SW(0x76,d7);
            uint16_t r0=ga_rand(e);SB(0x92,r0);SW(0x82,W(0x82)+(r0&0xfu));
            d1=(uint16_t)((r0>>4)&0xfu);
            if (W(0x84)) SW(0x84,W(0x84)+d1);
            child(a,e,0x98886,0x1c,0,0);
            SW(0x4e,rank(a,e,0x2d2040));SW(0x7a,rank(a,e,0x2d2104));SW(0x7e,rank(a,e,0x2d2166));
            ga_palette(a,0x23b);ga_flash_palette(a,0x30c);SW(0x66,0x8838);SW(0x1e,0x8838);SET(0x15,0);
            SL(0x50,0xe609a);exhaust(a,e,-0x10);SW(0x78,W(0x4e)>>1);
            k98664(a,e);
        }
        SL(0,0x98012);
        /* fall through */
    case 0x98012:
        if (W(0x8a) && !B(0x8d)) {SL(0,0x98114);goto s114;}
        gs_program(a,e);SW(0x80,W(0x82));SL(0,0x9805a);
        /* fall through */
    case 0x9805a:
        if (B(0x8d) && L(0x6e)) {SW(0x80,W(0x80)-1u);if (S16(W(0x80))<0) SET(0x8c,4);}
        if (BIT(0x8c,4)) {
            neo_gactor_t *b=spawn_plain(a,e,0x9879c,0x0e);
            SB(0x60,b?0:0xff);SB(0x8d,b?0:0xff);                        /* scc: carry clear = no node */
            SW(0x8a,W(0x8a)-1u);CLR(0x8c,4);gs_program(a,e);SL(0,0x980f0);
            goto s0f0;
        }
        if (BIT(0x8c,3)) goto s16e;
        k9865c(a,e);k98704(a,e);
        if (k9861e(a,e)<0) return GA_UNSUPPORTED;
        return gs_tail(a,e);
    case 0x980f0:
    s0f0:
        if (B(0x60)) {SL(0,0x98012);return gunship_tick(a,e);}
        if (BIT(0x8c,3)) goto s16e;
        k98750(a,e);
        if (k9861e(a,e)<0) return GA_UNSUPPORTED;
        return gs_tail(a,e);
    case 0x98114:
    s114:
        if (BIT(0x8c,3)) goto s16e;
        k98750(a,e);
        if (k9861e(a,e)<0) return GA_UNSUPPORTED;
        if (((B(0x8f)+1u)&0xfu)<=2u) SL(0,0x98142);
        return gs_tail(a,e);
    case 0x98142:START(0x29ef26);SL(0,0x98158);
        /* fall through */
    case 0x98158:
        k98750(a,e);
        if ((r=k9861e(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x98012);
        return gs_tail(a,e);
    s16e: {
        gs_program(a,e);
        int16_t d0=-0x800,d1=-0x400,d2=0x800,d3=0x200;
        if (S16(W(0x3c))>=0xa0) {d0=(int16_t)-d0;d2=(int16_t)-d2;}
        SL(0x30,L(0x30)+((uint32_t)(int32_t)d0<<8));SL(0x34,L(0x34)+((uint32_t)(int32_t)d1<<8));
        d2=(int16_t)(d2+(ga_rand(e)&0x1ffu));d3=(int16_t)(d3+(ga_rand(e)&0x1ffu));
        SW(0x72,(uint16_t)d2);SW(0x74,(uint16_t)d3);SL(0,0x981ec);
    }   /* fall through */
    case 0x981ec:
        k98770(a,e);
        if (k9861e(a,e)<0) return GA_UNSUPPORTED;
        return gs_tail(a,e);
    case 0x981f8: {
        gs_program(a,e);
        if (ga_rand(e)&0x8000u) {
            /* 098346 */
            sound(e,0x1023);SET(0x8c,2);SW(0x66,0x1001);SW(0x1e,0x1001);
            child(a,e,0x1c71e,0x1c,0,0);debris40(a,e,0xe60ce);
            child(a,e,0x98c0c,0x2a,0,0);child(a,e,0x98c20,0x2a,0,0);child(a,e,0x98c34,0x2a,0,0);
            neo_gactor_t *c=child(a,e,0x98c0c,0x2a,0,0);
            if (c) ga_sb(c,0x15,ga_b(c,0x15)^1u);
            return GA_DELETE;
        }
        neo_gactor_t *d=debris40(a,e,0xe614a);
        if (d) {ga_sw(d,0x66,0xe0ed);ga_sw(d,0x1e,0xe0ed);}
        uint8_t ang=neo_angle((int16_t)(0x130-S16(W(0x3c))),(int16_t)(0x80-S16(W(0x40))));
        uint32_t sp=(uint32_t)(int32_t)(int16_t)((ga_rand(e)&0x1ffu)+0x600u);
        uint32_t vx,vy;ga_polar(sp<<8,ang,&vx,&vy);
        SW(0x72,(uint16_t)(vx>>8));SW(0x74,(uint16_t)(vy>>8));SL(0,0x98288);
    }   /* fall through */
    case 0x98288:
        k98770(a,e);
        if (k9861e(a,e)<0) return GA_UNSUPPORTED;
        if (S16(W(0x40))<0x120) SL(0,0x982a4);
        if (ga_cull(a,e,0xe60be)) SL(0,0x9841e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x982a4:
        sound(e,0x1023);SET(0x8c,2);
        for (unsigned i=0;i<6;i++) {                                   /* 098C58 */
            neo_gactor_t *c=spawn_script(a,e,0x98cb6);
            uint16_t r0=ga_rand(e);
            int16_t dx=(int16_t)((r0&0xfu)-7u),dy=(int16_t)(((r0>>8)&0xfu)-7u);
            if (c) ga_place_from(c,a,e,dx,dy);
        }
        {neo_gactor_t *c=spawn_script(a,e,0x98e18);if (c) ga_place_from(c,a,e,0,0);}
        return GA_DELETE;
    case 0x9841e:SET(0x8c,2);return GA_DELETE;
    /* ---- pieces ---- */
    case 0x9879c:
        p=sh_creator(a,e);ga_place(a,e,8,-0x10,p?NEO_GA_TAG(p->index):0);k1aca2(a,e);
        START(0x29f032);ga_palette(a,0x23c);SL(0,0x987d0);
        /* fall through */
    case 0x987d0:
        if (gs_gone(a,e,&p)) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,bomb_call))<0) return GA_UNSUPPORTED;
        if (r) {ga_sb(p,0x60,0xff);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x98886:
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);k1aca2(a,e);
        START(0x29ef7c);ga_palette(a,0x23b);ga_flash_palette(a,0x30c);SL(0,0x988c4);
        /* fall through */
    case 0x988c4:
        if (gs_gone(a,e,&p)) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
        SW(0x1e,ga_w(p,0x1e));flash_share(a,e);ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x98974: {
        START(0x29f06c);ga_palette(a,0x23c);
        SL(0x34,0u-(((uint32_t)(uint16_t)(W(0x40)-0x100u)<<16)>>5));    /* 098B1C */
        SL(0x50,0xe60a6);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SL(0x6e,0);
        SW(0x7c,W(0x7c)+(uint16_t)((ga_rand(e)&0x7fu)-0x40u));
        if (S16(W(0x7c))<0) SW(0x7c,0x40);
        SW(0x80,0);gs_target(a,e,true);SL(0,0x989de);
    }   /* fall through */
    case 0x989de:
        SW(0x86,W(0x86)-1u);
        if (S16(W(0x86))<0 || !L(0x6e)) goto a12;
        SW(0x80,W(0x80)-1u);
        if (S16(W(0x80))<0) {
            SW(0x80,W(0x88));gs_target(a,e,true);
            if (L(0x6e)) {SW(0x72,px(e,L(0x6e)));SW(0x74,py(e,L(0x6e))+0x10u);}   /* 098BAA */
        }
        {   /* 098BCA */
            uint8_t ang=neo_angle((int16_t)(W(0x72)-W(0x3c)),(int16_t)(W(0x74)-W(0x40)));
            uint32_t vx,vy;ga_polar((uint32_t)(int32_t)S16(W(0x7c))<<8,ang,&vx,&vy);
            SL(0x30,L(0x30)+ASR(vx-L(0x30),4));SL(0x34,L(0x34)+ASR(vy-L(0x34),4));ga_move(a,e);
        }
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        goto a96;
    a12:SB(0x8f,k98a12(a));SL(0,0x98a62);
        /* fall through */
    case 0x98a62: {
        uint32_t vx,vy;ga_polar(0x80000,B(0x8f),&vx,&vy);
        SL(0x30,L(0x30)+ASR(vx-L(0x30),3));SL(0x34,L(0x34)+ASR(vy-L(0x34),3));ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
    }
    a96: {
        uint8_t v=B(0x90);SB(0x90,v-0x40u);
        if (v<0x40u) child(a,e,0x98ea2,0x2a,0x20,0);                     /* 098FBE */
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        bool boom=ga_attack(a,e,0xe608a)!=0;
        if (!boom && BIT(0x5f,7)) {sound(e,0x1022);boom=true;}
        if (boom) {SL(0x50,0);SL(0x62,0);SB(0x1d,0);SB(0x17,0);SL(0,0x1c706);return neo_effects_tick(a,e);}   /* 098AFE */
        if (ga_cull_rect(a,e,0xe60c6)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x98c0c:START(0x29f090);goto c44;
    case 0x98c20:START(0x29f0e6);goto c44;
    case 0x98c34:START(0x29f13c);
    c44:ga_palette(a,0x23b);return neo_effects_frag_launch(a,e,0);          /* JMP 01D51A */
    case 0x98cb6: {
        uint16_t k=ga_rand_range(e,7);SW(0x80,k);
        START(rl(e,0xe6120u+4u*k));ga_palette(a,rw(e,0xe613cu+2u*k));
        SL(0x30,(uint32_t)(int32_t)(int16_t)((ga_rand(e)&0xfffu)-0x800u)<<8);
        SL(0x34,(uint32_t)(int32_t)(int16_t)((ga_rand(e)&0xfffu)-0x800u)<<8);
        goto d24;
    }
    case 0x98e18:START(0x22cf2c);ga_palette(a,0x204);SL(0x30,0xfffe0000u);SL(0x34,0);
    d24:SW(0x72,0xf000);SW(0x74,0);SW(0x72,W(0x72)+(uint16_t)((ga_rand(e)&0x1ffu)-0xffu));SL(0,0x98d48);
        /* fall through */
    case 0x98d48: {
        uint32_t tx=(uint32_t)(int32_t)S16(W(0x72))<<8,ty=(uint32_t)(int32_t)S16(W(0x74))<<8;
        SL(0x30,L(0x30)+ASR(tx-L(0x30),4));SL(0x34,L(0x34)+ASR(ty-L(0x34),5));ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,0xe60c6)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x98e4a:
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);SL(0x50,0xe60b2);SL(0,0x98e62);
        /* fall through */
    case 0x98e62:
        if (gs_gone(a,e,&p)) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
        ga_project(a,e);
        if (ga_register(a,e,NULL,NULL)) {
            SB(0x5a,0);
            if (((B(0x5b)+1u)&7u)<3u) ga_sb(p,0x8c,ga_b(p,0x8c)|1u);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x98ea2: {
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x37c);START(0x29f192);
        if (ga_rand(e)&0x8000u) SET(0x15,0);
        int32_t d0=S32(ASR(L(0x30),1)+0xfff60000u);
        SL(0x30,d0<0?(uint32_t)d0:0u);SL(0x34,0);SL(0,0x98f06);
    }   /* fall through */
    case 0x98f06:
        SL(0x30,L(0x30)+0xffff8000u);ga_move(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        if (S16(W(0x3c))<=-0x20) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- molotov throwers 0569B4 / 0569D0 / 0569E6 -------------------------------- */
/* 0558C0..056EEE (script records of section 4; tables 05594C..055A48 and
 * 056A18, rank tables 2D0D42..2D0E3E): rebels who throw molotovs. 056922
 * (+93 = kind): the hurtbox follower 056E5A, 055B7E (+74 >= 3 -> rand & 1),
 * +80 = rank 2D0D42 << 8 (run speed), +88/+8C/+8E = ranks 2D0E26 / 2D0D90 /
 * 2D0D76 (TRAP #15 unless +88 > +8C and +88 > +8E), 055BAE (the target +84:
 * the nearest player in 055964 or 05596C within 30 below, else -1), palette
 * 9F, 8820, +4E = 1, 0146E8, +92 = 0, +50 = 05597C, 24AC4C, 01436A, 00427A,
 * faces right under x A0. 0569B4: +74 = +6E, +78 = 056A18[+70], 056230;
 * 0569D0: +78 = 056A18[+6E], 056620; 0569E6: +90 = +28 (home), +74 = +6E,
 * +76 = +70, +78 = 056A18[+72], 0568B2.
 * States: 056230 (stand: 055E28 -> run 056012, 055D4E turn / 055DB2, 055DF6
 * back off, 055F08 -> 055F9E, the ground 01456A -> 05616C fall, 055EC8 ->
 * 055F26 step up / 0561B2), 056012 (run), 05608C / 0560EE / 05611C (search),
 * 0561B2 / 0561F4, 056484 (throw: the shield 056D76, 24B730 -> 056D76's +60
 * counts the hits; 0562D8 / 05631A / 0564F2 / 05644A / 056374 / 0563AE /
 * 0563E8), 056620 (kind 1: idle 24AC56 -> 056542 fall / 0563E8 / 0564F2 /
 * 056588 / 0565E6), 056690 .. 05682E (kind 2: patrol around +90 within +76,
 * 0568B2 / 05686C / 0567CE / 056720 / 05675A / 056794 / 0566F2). Tail
 * 055AD6: 01945C 05594C -> +5F bit 7, +4E = 0, deleted. A hit (004C24 carry)
 * -> 056A28: 01436A, 00427A, the helmet 056A6A (-9, 1D), +5F bit 7, +4E = 0,
 * JMP 0406B6.
 * Program calls: 055A6C (+62 = +-+80), 055A82 / 055A8A (+5F bit 6), 055A92
 * (+97 = +34 < 0), 055A9C (+96 = grounded), 055AA8 (+9B = facing), 055ABA
 * (row 055A38 -> sound 10F9), 055A48 / 055A50 (x -= 10 / 9 by the facing),
 * 056CD0 (the flash 056C94 and the molotov 056B00 at (-1F, 1F) / (-1D, 12)
 * by +92), 01ADB2 (+15 bit 1 flipped).
 * Pieces: 056E5A (hurtbox +50 = creator's +78; a hit from the front sets
 * the creator's +9A), 056D76 (shield while the creator is in 0564BE; +98 =
 * 1E per hit, creator +60 while it counts), 056B00 (molotov: palettes 22 /
 * 178 / 179, 24BA1A, E0EF, sound 10FF, +50 = 0559DC, speed rank 2D0D5C; the
 * probe 0149CA (0, 3), +5F bit 0, the ground or its row 055A18 / 055A28 ->
 * the fire 01C950; 01945C 055954), 056C94 (flash A2, 24B966), 056A6A (helmet
 * 9F, 24B9B8, +-8000 / 48000, bounces to 14000). */
static bool th_target(neo_gactor_t *a,neo_genv_t *e) {                                   /* 055CDE */
    uint32_t n=L(0x84);
    return !(n&0x80000000u) && live(e,n,NULL);
}
static void k55bae(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x95,0);SB(0x94,0);
    uint32_t n;
    if (k1affc(a,e,0x55964,&n) && k1affc(a,e,0x5596c,&n)) {SL(0x84,0xffffffffu);return;}
    if ((int16_t)(py(e,n)-W(0x40))>0x30) n=0xffffffffu;
    SL(0x84,n);
}
/* 055D22: carry (true) without a target or with it ahead. */
static bool k55d22(neo_gactor_t *a,neo_genv_t *e) {
    if (!th_target(a,e)) return true;
    int16_t d0=(int16_t)(px(e,L(0x84))-W(0x3c));
    if (!BIT(0x15,0)) d0=(int16_t)-d0;
    return d0>=0;
}
/* 055D4E: 0 none / far, 1 / 2 the side. */
static int k55d4e(neo_gactor_t *a,neo_genv_t *e) {
    if (!th_target(a,e)) {SB(0x94,0);return 0;}
    int16_t d1=(int16_t)(px(e,L(0x84))-W(0x3c));
    if ((uint16_t)absw((uint16_t)d1)>W(0x8c)) {SB(0x94,0);return 0;}
    if (B(0x94)) return 2;
    if (BIT(0x15,0)) d1=(int16_t)-d1;
    SB(0x95,0xff);
    return d1<0?1:2;
}
static bool k55d9c(neo_gactor_t *a,neo_genv_t *e) {return (ga_rand_range(e,rank(a,e,0x2d0daa))&0xffu)!=0;}
/* 055EC8: 0149CA at (+-10, 8) then (+-10, 20): 0 / 1 / 2. */
static int k55ec8(neo_gactor_t *a,neo_genv_t *e) {
    int16_t dx=BIT(0x15,0)?0x10:-0x10;
    if (!ga_probe_point(a,e,dx,8)) return 0;
    return ga_probe_point(a,e,dx,0x20)?2:1;
}
static bool k55db2(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t x=W(0x3c);
    if (x>0x30u) {
        if (x>=0x110u && BIT(0x15,0)) goto edge;
    } else if (!BIT(0x15,0)) goto edge;
    if (k55ec8(a,e)==2) goto edge;
    return false;
edge:SB(0x94,0xff);return true;
}
static bool k55df6(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x95) || !th_target(a,e)) return false;
    return (uint16_t)absw((uint16_t)(px(e,L(0x84))-W(0x3c)))>=W(0x8e);
}
static bool k55e28(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x93) || !th_target(a,e)) return false;
    uint16_t d=absw((uint16_t)(px(e,L(0x84))-W(0x3c)));
    return d<=W(0x88) && d>W(0x8c);
}
static bool k55e64(neo_gactor_t *a,neo_genv_t *e) {uint32_t n;return k1affc(a,e,0x55974,&n);}   /* carry (true): no player in 055974 */
static bool k55e6e(neo_gactor_t *a,neo_genv_t *e) {
    (void)e;
    SW(0x8a,W(0x8a)-1u);
    if (S16(W(0x8a))>=0) return false;
    SW(0x8a,0);
    int16_t x=S16(W(0x3c));
    return x>0x10 && x<0x130;
}
static void k55e9a(neo_gactor_t *a) {
    SW(0x28,W(0x28)+(BIT(0x15,0)?0xfff8u:8u));
    if (!BIT(0x15,0)) SW(0x28,W(0x28)&0xfff8u); else SW(0x28,W(0x28)|7u);
}
static bool k55f08(neo_gactor_t *a) {return !B(0x92) && B(0x9a);}
static uint8_t k55c68(neo_gactor_t *a,neo_genv_t *e,bool *stop) {
    if (W(0x74)!=2u) return (uint8_t)W(0x74);
    return k1ae00(e,L(0x84),stop)?1u:0u;
}
static bool k55c92(neo_gactor_t *a,neo_genv_t *e) {
    bool stop=false;uint8_t d0=k55c68(a,e,&stop);
    if (stop) return false;
    uint32_t prog=d0==B(0x92)?0x24b426u:d0==0?0x24b548u:0x24b58au;
    return ga_anim_start(a,e,prog,0x100);
}
static void k55d02(neo_gactor_t *a) {SL(0x30,BIT(0x15,0)?0x3c000u:0xfffc4000u);SL(0x34,0x40000);}
static bool th_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x55a6c:SL(0x62,BIT(0x15,0)?L(0x80):0u-L(0x80));return true;
    case 0x55a82:SET(0x5f,6);return true;
    case 0x55a8a:CLR(0x5f,6);return true;
    case 0x55a92:SB(0x97,S16(W(0x34))<0?0xff:0);return true;
    case 0x55a9c:SB(0x96,BIT(0x15,6)?0xff:0);return true;
    case 0x55aa8:SB(0x9b,BIT(0x15,0));return true;
    case 0x55aba:if (ga_attack(a,e,0x55a38)) sound(e,0x10f9);return true;
    case 0x55a48:case 0x55a50: {
        int16_t d0=routine==0x55a48?-10:-9;
        if (BIT(0x15,0)) d0=(int16_t)-d0;
        SW(0x28,W(0x28)+(uint16_t)d0);ga_project(a,e);return true;
    }
    case 0x56cd0: {
        int16_t dx=B(0x92)?-0x1d:-0x1f,dy=B(0x92)?0x12:0x1f;
        child(a,e,0x56c94,0x2a,dx,dy);child(a,e,0x56b00,0x2a,dx,dy);return true;
    }
    case 0x1adb2:SB(0x15,B(0x15)^2u);return true;
    }
    return false;
}
/* 055AD6 */
static ga_result_t th_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0x5594c)) {SET(0x5f,7);SW(0x4e,0);return GA_DELETE;}
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 056A28: a hit (004C24 carry). */
static ga_result_t th_hit(neo_gactor_t *a,neo_genv_t *e) {
    ga_stationary(a,e);
    if (ga_anim_step(a,e,th_call)<0) return GA_UNSUPPORTED;
    child(a,e,0x56a6a,0x2a,-9,0x1d);
    SET(0x5f,7);SW(0x4e,0);
    return death(a,e);
}
#define TS(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define TA() do { if ((r=ga_anim_step(a,e,th_call))<0) return GA_UNSUPPORTED; } while (0)
#define HIT() do { if (ga_register(a,e,NULL,NULL)) return th_hit(a,e); } while (0)
#define TGO(x) do { SL(0,(x));goto again; } while (0)
static ga_result_t thrower_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;unsigned guard=0;
again:
    if (++guard>12) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    if (e->unsupported) return GA_UNSUPPORTED;
    switch (L(0)) {
    case 0x569b4:case 0x569d0:case 0x569e6: {
        uint32_t self=L(0);uint16_t sel;
        SB(0x93,self==0x569b4?0:self==0x569d0?1:2);
        spawn_plain(a,e,0x56e5a,0x1c);
        if (W(0x74)>=3u) SW(0x74,ga_rand(e)&1u);                            /* 055B7E */
        SL(0x80,(uint32_t)(int32_t)S16(rank(a,e,0x2d0d42))<<8);              /* 055B98 */
        SW(0x88,rank(a,e,0x2d0e26));SW(0x8c,rank(a,e,0x2d0d90));SW(0x8e,rank(a,e,0x2d0d76));   /* 055BF8 */
        if (W(0x88)<=W(0x8c) || W(0x88)<=W(0x8e)) {ga_fail(e,0x55c34);return GA_UNSUPPORTED;}   /* TRAP #15 */
        k55bae(a,e);
        ga_palette(a,0x9f);SW(0x66,0x8820);SW(0x1e,0x8820);SW(0x4e,1);ga_floor(a,e);
        SB(0x92,0);SL(0x50,0x5597c);SB(0x94,0);SB(0x99,0);SB(0x9a,0);
        TS(0x24ac4c);ga_stationary(a,e);TA();
        if (S16(W(0x3c))<0xa0) SET(0x15,0);
        if (self==0x569b4) {SW(0x74,W(0x6e));sel=W(0x70);}
        else if (self==0x569d0) sel=W(0x6e);
        else {SW(0x90,W(0x28));SW(0x74,W(0x6e));SW(0x76,W(0x70));sel=W(0x72);}
        SL(0x78,rl(e,0x56a18u+(uint16_t)(sel<<2)));
        SL(0,self==0x569b4?0x56230:self==0x569d0?0x56620:0x568b2);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* ---- kind 0 ---- */
    case 0x56230:TS(0x24ac56);SL(0,0x56246);
        /* fall through */
    case 0x56246:
        if (!th_target(a,e)) TGO(0x5611c);
        if (!B(0x95) && k55e28(a,e)) TGO(0x56012);
        r=k55d4e(a,e);
        if (r==1) TGO(0x55f64);
        if (r==2 && k55db2(a,e)) TGO(0x55f64);
        if (k55df6(a,e)) {SB(0x95,0);SB(0x94,0);if (!k55d22(a,e)) TGO(0x55f64);}
        HIT();
        if (k55f08(a)) TGO(0x55f9e);
        ga_ground(a,e,NULL);
        if (!BIT(0x15,6)) SL(0,0x5616c);
        r=k55ec8(a,e);
        if (r==1) TGO(0x55f26);
        if (r==2) TGO(0x561b2);
        TA();return th_tail(a,e);
    case 0x56012:
        if (!k55d22(a,e)) TGO(0x55f64);
        SW(0x8a,rank(a,e,0x2d0dc4));TS(0x24ad2a);SL(0,0x56040);
        /* fall through */
    case 0x56040:
        if (!th_target(a,e)) TGO(0x5611c);
        if (!k55e64(a,e)) TGO(0x55fd8);
        r=k55d4e(a,e);
        if (r==1) TGO(0x55f64);
        if (r==2) TGO(0x56230);
        if (k55e6e(a,e)) TGO(0x560ee);
        if (k55f08(a)) TGO(0x55f9e);
        HIT();ga_stationary(a,e);TA();return th_tail(a,e);
    case 0x5608c:TS(k55d9c(a,e)?0x24adf2:0x24aed2);k55bae(a,e);SL(0,0x560c2);
        /* fall through */
    case 0x560c2:
        HIT();
        if (!k55e64(a,e)) TGO(0x55fd8);
        ga_stationary(a,e);TA();if (r) SL(0,0x56012);return th_tail(a,e);
    case 0x560ee:if (!k55c92(a,e)) return GA_UNSUPPORTED;SL(0,0x560f8);
        /* fall through */
    case 0x560f8:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x5608c);return th_tail(a,e);
    case 0x5611c:SB(0x95,0);SB(0x94,0);TS(0x24ad2a);SL(0,0x5613a);
        /* fall through */
    case 0x5613a:
        k55bae(a,e);
        if (!(L(0x84)&0x80000000u)) {if (!k55d22(a,e)) TGO(0x55f64);TGO(0x56230);}
        HIT();ga_stationary(a,e);TA();return th_tail(a,e);
    case 0x5616c:TS(0x24ad9a);k55d02(a);SL(0,0x56186);
        /* fall through */
    case 0x56186:HIT();ga_air(a,e);SL(0x34,L(0x34)+0xffff8000u);TA();if (r) SL(0,0x56230);return th_tail(a,e);
    case 0x561b2:SB(0x95,0);SB(0x94,0);TS(0x24b680);SL(0,0x561d0);
        /* fall through */
    case 0x561d0:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x561f4);return th_tail(a,e);
    case 0x561f4:TS(0x24ad2a);SL(0,0x5620a);
        /* fall through */
    case 0x5620a:
        k55bae(a,e);
        if (th_target(a,e)) TGO(0x56012);
        HIT();ga_stationary(a,e);TA();return th_tail(a,e);
    case 0x562d8:TS(0x24b616);SL(0,0x562ee);
        /* fall through */
    case 0x562ee:
        HIT();
        if (k55f08(a)) TGO(0x564f2);
        ga_stationary(a,e);TA();if (r) SL(0,0x564f2);return th_tail(a,e);
    case 0x5631a:TS(k55d9c(a,e)?0x24b106:0x24b1f6);k55bae(a,e);SL(0,0x56350);
        /* fall through */
    case 0x56350:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56484);return th_tail(a,e);
    case 0x56374:TS(0x24b680);SL(0,0x5638a);
        /* fall through */
    case 0x5638a:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56620);return th_tail(a,e);
    case 0x563ae:TS(0x24b680);SL(0,0x563c4);
        /* fall through */
    case 0x563c4:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x564f2);return th_tail(a,e);
    case 0x563e8:k55e9a(a);TS(0x24b810);SL(0,0x56402);
        /* fall through */
    case 0x56402:
        HIT();ga_project(a,e);TA();
        if (r) {
            SL(0,0x56620);
            if (!k55d22(a,e) && (uint16_t)absw((uint16_t)(px(e,L(0x84))-W(0x3c)))>=0x60u) SL(0,0x563ae);
        }
        return th_tail(a,e);
    case 0x5644a:TS(0x24b50a);SL(0,0x56460);
        /* fall through */
    case 0x56460:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56620);return th_tail(a,e);
    case 0x56484:SW(0x8a,rank(a,e,0x2d0dc4));TS(0x24b730);spawn_plain(a,e,0x56d76,0x1c);SB(0x60,0);SL(0,0x564be);
        /* fall through */
    case 0x564be:
        if (!k55d22(a,e)) TGO(0x5644a);
        if (k55e6e(a,e)) {if (!B(0x60)) TGO(0x562d8);SW(0x8a,0);}
        HIT();ga_stationary(a,e);TA();return th_tail(a,e);
    case 0x564f2:TS(0x24b7ae);SL(0,0x56508);
        /* fall through */
    case 0x56508:
        HIT();ga_stationary(a,e);TA();
        if (r) {if (B(0x99)) {SB(0x99,0);SL(0,0x5631a);} else SL(0,0x56484);}
        return th_tail(a,e);
    case 0x56542:TS(0x24ad9a);k55d02(a);SL(0,0x5655c);
        /* fall through */
    case 0x5655c:HIT();ga_air(a,e);SL(0x34,L(0x34)+0xffff8000u);TA();if (r) SL(0,0x56620);return th_tail(a,e);
    case 0x56588:TS(0x24b5cc);SB(0x98,0);SL(0,0x565a2);
        /* fall through */
    case 0x565a2:
        if (B(0x98)) {SB(0x98,B(0x98)-1u);if (!B(0x98)) TGO(0x56620);}
        HIT();ga_stationary(a,e);TA();
        if (r) {SB(0x98,0x28);TS(0x24ad2a);}
        return th_tail(a,e);
    case 0x565e6:TS(0x24b8b6);SL(0,0x565fc);
        /* fall through */
    case 0x565fc:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56620);return th_tail(a,e);
    /* ---- kind 1 ---- */
    case 0x56620:TS(0x24ac56);SL(0,0x56636);
        /* fall through */
    case 0x56636:
        HIT();ga_ground(a,e,NULL);
        if (!BIT(0x15,6)) SL(0,0x56542);
        r=k55ec8(a,e);
        if (r==1) {if (k55d22(a,e)) {k55e9a(a);TGO(0x564f2);}TGO(0x563e8);}
        if (k55f08(a)) TGO(0x56588);
        if (!k55e64(a,e)) TGO(0x565e6);
        TA();return th_tail(a,e);
    /* ---- kind 2 ---- */
    case 0x56690:TS(k55d9c(a,e)?0x24adf2:0x24aed2);k55bae(a,e);SL(0,0x566c6);
        /* fall through */
    case 0x566c6:
        HIT();
        if (!k55e64(a,e)) TGO(0x5675a);
        ga_stationary(a,e);TA();if (r) SL(0,0x567ce);return th_tail(a,e);
    case 0x566f2:if (!k55c92(a,e)) return GA_UNSUPPORTED;SL(0,0x566fc);
        /* fall through */
    case 0x566fc:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56690);return th_tail(a,e);
    case 0x56720:TS(0x24b680);SL(0,0x56736);
        /* fall through */
    case 0x56736:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x567ce);return th_tail(a,e);
    case 0x5675a:TS(0x24b8b6);SL(0,0x56770);
        /* fall through */
    case 0x56770:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x567ce);return th_tail(a,e);
    case 0x56794:TS(0x24b5cc);SL(0,0x567aa);
        /* fall through */
    case 0x567aa:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x567ce);return th_tail(a,e);
    case 0x567ce:
        if (!k55d22(a,e)) TGO(0x56720);
        SW(0x8a,rank(a,e,0x2d0dc4));TS(0x24ad2a);SL(0,0x567fc);
        /* fall through */
    case 0x567fc:
        if (k55e6e(a,e)) TGO(0x566f2);
        if (!k55e64(a,e)) TGO(0x5675a);
        HIT();
        if (k55f08(a)) TGO(0x56794);
        ga_stationary(a,e);TA();return th_tail(a,e);
    case 0x5682e:k55e9a(a);TS(0x24b810);SL(0,0x56848);
        /* fall through */
    case 0x56848:HIT();ga_project(a,e);TA();if (r) SL(0,0x568b2);return th_tail(a,e);
    case 0x5686c:TS(0x24ad9a);k55d02(a);SL(0,0x56886);
        /* fall through */
    case 0x56886:HIT();ga_air(a,e);SL(0x34,L(0x34)+0xffff8000u);TA();if (r) SL(0,0x568b2);return th_tail(a,e);
    case 0x568b2:TS(0x24ac56);SL(0,0x568c8);
        /* fall through */
    case 0x568c8:
        HIT();ga_ground(a,e,NULL);
        if (!BIT(0x15,6)) SL(0,0x5686c);
        r=k55ec8(a,e);
        if (r==1) TGO(0x5682e);
        if (r==2) TGO(0x567ce);
        TA();
        if ((uint16_t)absw((uint16_t)(W(0x28)-W(0x90)))>=W(0x76)) SL(0,0x567ce);
        return th_tail(a,e);
    /* ---- shared states of kind 0 ---- */
    case 0x55f26:k55e9a(a);TS(0x24b810);SL(0,0x55f40);
        /* fall through */
    case 0x55f40:HIT();ga_project(a,e);TA();if (r) SL(0,0x56230);return th_tail(a,e);
    case 0x55f64:TS(0x24b680);SL(0,0x55f7a);
        /* fall through */
    case 0x55f7a:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56230);return th_tail(a,e);
    case 0x55f9e:TS(0x24b5cc);SL(0,0x55fb4);
        /* fall through */
    case 0x55fb4:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56012);return th_tail(a,e);
    case 0x55fd8:TS(0x24b8b6);SL(0,0x55fee);
        /* fall through */
    case 0x55fee:HIT();ga_stationary(a,e);TA();if (r) SL(0,0x56012);return th_tail(a,e);
    /* ---- pieces ---- */
    case 0x56e5a: {
        neo_gactor_t *p=sh_creator(a,e);
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);
        if (!p) {ga_fail(e,0x56e64);return GA_UNSUPPORTED;}
        SL(0x50,ga_l(p,0x78));
        if (!L(0x50)) return GA_DELETE;
        SW(0x4e,1);SL(0,0x56e7e);
    }   /* fall through */
    case 0x56e7e: {
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x56e7e);return GA_UNSUPPORTED;}
        if (!ga_w(p,0x4e)) return GA_DELETE;
        k1aca2(a,e);ga_project(a,e);ga_sb(p,0x9a,0);
        if (!ga_b(p,0x92) && ga_register(a,e,NULL,NULL)) {
            SW(0x4e,1);CLR(0x5f,7);
            uint8_t d0=(uint8_t)(B(0x5b)+1u);
            if (BIT(0x15,0)) d0=(uint8_t)(d0+8u);
            if ((d0&0xfu)<=2u) ga_sb(p,0x9a,0xff);
        }
        SB(0x5a,0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x56d76: {
        neo_gactor_t *p=sh_creator(a,e);
        SL(0x50,0x559e8);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);SL(0,0x56d8e);
    }   /* fall through */
    case 0x56d8e: {
        neo_gactor_t *p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x56d8e);return GA_UNSUPPORTED;}
        if (ga_l(p,0)!=0x564beu) return GA_DELETE;
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SB(0x98,0x1e);}
        if (B(0x98)) {SB(0x98,B(0x98)-1u);ga_sb(p,0x60,B(0x98)?0xff:0);}
        ga_project(a,e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x56b00: {
        ga_palette(a,0x22);ga_palette(a,0x178);ga_palette(a,0x179);SET(0x15,5);
        TS(0x24ba1a);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);sound(e,0x10ff);SL(0x50,0x559dc);
        uint32_t v=(uint32_t)(int32_t)S16(rank(a,e,0x2d0d5c))<<8;
        SL(0x30,BIT(0x15,0)?v:0u-v);SL(0x7c,0x55a18);SL(0,0x56b8e);
    }   /* fall through */
    case 0x56b8e:
        if (ga_probe_point(a,e,0,3)) goto fire;
        ga_air(a,e);
        if (ga_cull(a,e,0x55954)) return GA_DELETE;
        if (BIT(0x5f,0) || BIT(0x15,6)) goto fire;
        if (ga_attack(a,e,L(0x7c))) goto fire;
        SL(0x7c,0x55a28);
        if (ga_anim_step(a,e,th_call)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    fire:
        child(a,e,0x1c950,0x2a,0,0);
        return GA_DELETE;
    case 0x56c94:ga_palette(a,0xa2);TS(0x24b966);SL(0,0x56cb8);
        /* fall through */
    case 0x56cb8:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,th_call))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x56a6a:
        ga_palette(a,0x9f);TS(0x24b9b8);
        SL(0x30,BIT(0x15,0)?0xffff8000u:0x8000u);SL(0x34,0x48000);SL(0,0x56aac);
        /* fall through */
    case 0x56aac:
        ga_air(a,e);
        if (ga_cull(a,e,0x5595c)) return GA_DELETE;
        if (BIT(0x15,6)) {
            SET(0x5f,3);
            uint32_t d0=ASR(0u-L(0x34),1);
            if (d0<0x14000u) return GA_DELETE;
            SL(0x34,d0);
        } else SL(0x34,L(0x34)+0xffff6000u);
        if (ga_anim_step(a,e,th_call)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
#undef TS
#undef TA
#undef HIT
#undef TGO
/* ---- flying bomber 084588 ------------------------------------------------------ */
/* 083B30..08513C (record 2DB248 of section 4, a runner's record; tables
 * 0E4C58..0E4D04, rank tables 2D5432..2D5502). 084588: the target +7A =
 * 01B04A (none: the constructor runs again next update), 084D4A (+6E / +70
 * words: +6E = 0 faces right, +84 = +70 << 3, +4E = rank 2D549A, +6E = rank
 * 2D54B4 << 8), palette 244, flash 850, 8800, +50 = 0E4C70, +30 = +-+6E (x
 * >= A0 flips), sound 105D, 281FFC, the rotor 083B30. 08460E: 01AC38,
 * 01AD8C, 00427A; with the target live, when |tx - x| - +84 and |t+44 -
 * +44| - +84 differ in sign (the ship crosses the drop distance) and x in
 * A..136 -> 0846CE (the bomb 08471A, then 0846DA); 004C24 (+5A = 6 without
 * +5F bit 7: sound 108D), +5F bit 7 -> 0846DA (sound 1021, the helmet
 * 084948, 019DFC, 084E74: 01C71E and two 08497E, the last flipped; deleted),
 * 01945C 0E4C58.
 * - 083B30 (rotor): palette 220, 28207A, 01AD6E (0, 0), 01ACA2, +1E = the
 *   creator's; deleted with the creator (00409A).
 * - 08471A (bomb): palette BD, 2824F4, E0EF, 08508E (+34 = rank 2D54CE <<
 *   8; the time to fall 100 - y below (084FAE, 084FD6: |d0| * |d0| / d1 / 2
 *   negated) -> +7E gravity; +30 from the target's x over that time
 *   (085026 scaled by rank 2D54E8 + 100 ahead when below, 084FF2)), +8A = 0,
 *   +82 = A; 08475E: +34 += +7E, +34 >= 0: 01AC38, else 1001 and 0143E4;
 *   00427A; +8A += 002F1A(+30 >> 8, +34 >> 8) and every 1200 the trail
 *   0848EE; +82 counts down to the row 0E4CC4; 01945C 0E4C58; on the ground
 *   -> 08480E (palette 113, 24855C, 1001, sound 1021, deleted at the end).
 * - 0848EE (trail): palette BD, 282518, +66/+1E = the creator's - 1.
 * - 084948: 0405EA, +5A = 5, +15 bit 6, +5B bit 7 facing left, JMP 0406B6.
 * - 08497E (debris): +15 bit 5, 1001, palette 245, +78 = 4, rand & 7 == 0 ->
 *   084ABE (slide: +62 = +-20000 at least, 28236E, 01456A, a hop 084A02 by
 *   rand & F == 0 or leaving the ground) else a row of 0E4CD4 (rand & 3): +34
 *   / +72, +6E gravity, +30 = +-speed, +76 = rand & 1F + 18; 084A50: 0143E4,
 *   grounded -> 084A82 (282388, 01436A, then 084A2E: +34 = -+72 / 2, +78
 *   bounces); 084B86 (+76 <= F: +5F bit 3); 084B46 (+76 <= 0 or x <= -30 or
 *   019504 0E4C68: deleted, 0041AC).
 * The 083C8A / 083C98 variants (083D80.., bomb-drop soldier 0842A8 -> 0AF17E,
 * smoke 084B9C) are not reached by the record and are explicit stops. */
/* 084FD6: the 32-bit quotient d0 / d1 (two divu.w). */
static uint32_t k84fd6(uint32_t d0,uint16_t d1) {
    if (!d1) return d0;
    uint32_t hi=(d0>>16)/d1,r=(d0>>16)%d1,lo=((r<<16)|(d0&0xffffu))/d1;
    return hi<<16|(lo&0xffffu);
}
/* 084FAE: -(|d0| * |d0| / d1) / 2 (d1 = 0 -> 1, negative flips d0). */
static uint32_t k84fae(int16_t d0,int16_t d1) {
    uint16_t u=(uint16_t)(d0<0?-d0:d0);
    if (!d1) d1=1; else if (d1<0) {u=(uint16_t)(0u-u);d1=(int16_t)-d1;}
    uint32_t q=(uint32_t)u*u;
    q=k84fd6(q,(uint16_t)d1);
    return 0u-(q>>1);
}
/* 084FF2: d0 / d1 with signs (the quotient negated when they differ). */
static uint32_t k84ff2(uint32_t d0,int16_t d1) {
    bool neg=(d0&0x80000000u)!=0;
    if (neg) d0=0u-d0;
    if (!d1) d1=1; else if (d1<0) {d1=(int16_t)-d1;neg=!neg;}
    d0=k84fd6(d0,(uint16_t)d1);
    return neg?0u-d0:d0;
}
/* 085026: |d0| (1 for 0) when |d1| >= |d0|; else d2 + (table 085070 of (|d1|
 * << 16) / |d0|) * d2 >> 16. */
static uint16_t k85026(neo_genv_t *e,uint16_t d0,int16_t d1,uint16_t d2) {
    int16_t x=(int16_t)d0;
    if (!x) x=1; else if (x<0) x=(int16_t)-x;
    if (d1<0) d1=(int16_t)-d1;
    if (d1>=x) return (uint16_t)x;                                   /* blt: the table only below */
    uint32_t q=((uint32_t)(uint16_t)d1<<16)/(uint16_t)x;
    uint16_t v=0x4cc2;
    for (uint32_t at=0x85070;;at+=4u) {
        uint16_t d3=rw(e,at);
        if (!d3) {v=0x4cc2;break;}
        v=rw(e,at+2u);
        if ((uint16_t)q>=d3) break;
    }
    return (uint16_t)((((uint32_t)v*d2)>>16)+d2);
}
/* 002F1A: |d0|, |d1|: the larger + the smaller / 2. */
static uint16_t k2f1a(int16_t d0,int16_t d1) {
    uint16_t a0=(uint16_t)(d0<0?-d0:d0),a1=(uint16_t)(d1<0?-d1:d1);
    return a1>a0?(uint16_t)((a0>>1)+a1):(uint16_t)(a0+(a1>>1));
}
/* 084E74: the explosion and two debris, the second flipped. */
static void k84e74(neo_gactor_t *a,neo_genv_t *e) {
    child(a,e,0x1c71e,0x1c,0,0);child(a,e,0x8497e,0x2a,0,0);
    neo_gactor_t *c=child(a,e,0x8497e,0x1c,0,0);
    /* BCHG #0 on the fresh node after 01B242 gave it the creator's bit 0: the opposite facing. */
    if (c) ga_sb(c,0x15,(uint8_t)((ga_b(c,0x15)&~1u)|((B(0x15)&1u)^1u)));
}
/* 084B86 / 084B46: the end sets 084B7E (0041AC next update). */
static ga_result_t deb_tail(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x76,W(0x76)-1u);
    if (S16(W(0x76))<=0x0f) SET(0x5f,3);
    if (S16(W(0x76))<=0 || S16(W(0x3c))<=-0x30 || ga_cull_rect(a,e,0xe4c68)) SL(0,0x84b7e);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- Mission 4: the flyers 083C8A / 08391C of the same family -------------------
 * (script records of section 1; MAME captures B and C from sec05_149829,
 * docs/MISION4.md "Fase 2"; tables 0E4C58..0E4CD4, rank tables 2D5432..).
 * - 083C8A: 084CDA (+84/+86/+88 = the record's words +6E/+70/+72, +8C..+97
 *   = its +74..+7F: the rider's soldier words; +4E = rank 2D5466, +6E =
 *   rank 2D5480 << 8), +99 = 0 (083C98, +99 = 1 with a target 01B04A, is
 *   not reached: explicit stop). 083CC8: faces right from x < A0 (else +6E
 *   negated), palette 244, flash 850, 8800, +50 = 0E4C70, 0146E8, 01B1DC,
 *   sound 105D, 2820F8, the rider 08443E (D0 = 1C, 01B220) and the rotor
 *   084098 (D0 = 0E), +30 = +6E / 2. 083D80: 084E2C (0143DC; landing halves
 *   +30, +34 = -(+34 before) / 4, sound 1060), 00427A, +30 += (+6E - +30) /
 *   8; on the ground, when its x of the root (01AC4C) passes +84 -> 083E2C
 *   (+60 = 1: the rotor leaves; 282154, the rotor 0841A0, +62 = +30).
 *   083E5E: 01456A, 00427A; at the program's end or off the ground ->
 *   083F28; +62 += (+6E - +62) / 8. Tail 083E9E: +5F bit 0 -> 08404E;
 *   004C24 (+5A = 6 without +5F bit 7: sound 108D); 0049E8 0E4C94 hits ->
 *   08404E; +5F bit 7 -> 084010; 004188 outside 0E4C58.
 * - 083F28: sound 105C, the jumping rider 0842A8 at (18, 8) with +30 = -(+30
 *   / 4), +30 /= 2, +34 = ((rand & 300) + 400) << 8, y + 20, +15 bit 5,
 *   E0EF, 2821AC. 083FAE: 0143DC, 00427A; the ground or 0049E8 0E4CB4 ->
 *   08407E; 004C24, +5F bit 7 -> 084032; 004188 outside 0E4C58.
 * - 084010: the dying rider 0843F8; 084032: sound 1021, 019DFC, 084E74,
 *   the item 084F74 (08CDB2 kind +86, +70 = +88, at (0, 8)), deleted.
 *   08404E: +5B bit 7 when +30 >= 0, 0843F8; 08407E: sound 1021, 019DFC,
 *   084EE2 (the explosion 084B9C and two debris 08497E, the last flipped),
 *   deleted.
 * - 08391C: 084C9A (+86/+88 = the words +6E/+70, +4E = rank 2D5432, +6E =
 *   rank 2D544C << 8), as 083CC8 with sound 105E, 281FFC, the rotor 083B30
 *   and the trail 083B92. 0839D2: 084D94 (0143DC; landing on a slope facing
 *   it (D7 against 4000) restores the position: -> 083A8E), 00427A, +30 +=
 *   (+6E - +30) / 16, the tail of 083E9E with 083A8E / 083A72 (as 08407E /
 *   084032).
 * - 083B92: 01AD6E at (+-14, 0); 083BB0: 01ABC8, every fourth frame the
 *   creator's +1E and the smoke 01C82A (D0 = 2A); deleted with the creator.
 * - 08443E (rider): 01AD6E, palette 177, 2823B2; 08446C: the creator's
 *   +1E; its +61 -> 0844C2; 00427A while the creator is on the ground, else
 *   the program again; 0844C2: 01AD52, 2823E0, deleted at its end.
 * - 084098 / 0841A0 (rotors): 8800, palette 220, 282126 / 282182, 01AD6E,
 *   01ACA2; deleted when the creator's +60 is set or with the creator.
 * - 0842A8 (the rider jumping off): 8820, 282362, palette 20E, +34 = ((rand
 *   & 300) + 200) << 8; 0842EE: 0143DC, 00427A; at its end the soldier
 *   0AF17E (0040BC, D0 = 1C) with the creator's +8C..+97 as its words;
 *   084344: 0143DC, the soldier at its place (01ACF2, 01AC94) with its
 *   speeds, deleted.
 * - 0843F8: 8820, 0405EA, the creator's +5B, +5A = 5 and +15 bit 6 on even
 *   frames (10000F bit 0), 7 on odd ones, JMP 0406B6.
 * - 084B9C: palette 204, 28240A, 01DEA2, +50 = 0; 084BD0: 01ABC8, 00427A,
 *   deleted at its end or outside 0E4C60 (019504); 0049E8 with the row +76.
 */
/* 084E2C / 084D94: 0143DC and the landing; *kind = D7 of the sweep. */
static void fl_land(neo_gactor_t *a,neo_genv_t *e,uint32_t *kind) {
    SL(0x72,L(0x34));SB(0x98,B(0x15));
    SL(0x34,L(0x34)+0xffffb000u);
    if (kind) ga_air_kind(a,e,kind); else ga_air(a,e);
}
static void fl_bounce(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x15,6) && !BIT(0x98,6)) {SL(0x34,0u-ASR(L(0x72),2));SL(0x30,ASR(L(0x30),1));sound(e,0x1060);}
}
/* 084EE2 */
static void k84ee2(neo_gactor_t *a,neo_genv_t *e) {
    child(a,e,0x84b9c,0x1c,0,0);child(a,e,0x8497e,0x2a,0,0);
    neo_gactor_t *c=child(a,e,0x8497e,0x1c,0,0);
    /* BCHG #0 on the fresh node after 01B242 gave it the creator's bit 0: the opposite facing. */
    if (c) ga_sb(c,0x15,(uint8_t)((ga_b(c,0x15)&~1u)|((B(0x15)&1u)^1u)));
}
/* 084F74 */
static void k84f74(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x86)==0xffffu) return;
    ga_project(a,e);
    if (W(0x86)<0x5au) {                                                  /* 08CDB2 */
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
        if (c) {
            ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x88));ga_sw(c,0x6e,W(0x86));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
            ga_place_at(c,a,e,0,8);ga_sl(c,0x30,0);ga_sl(c,0x34,0);
        }
    }
    SW(0x86,0xffff);
}
/* The calls of the program 2821AC: 08512E (+9B = +9A / 2 rounded up) and
 * 08513E (+9A + 1). */
static bool fl_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)e;(void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x8512eu) {int8_t v=(int8_t)B(0x9a);SB(0x9b,(uint8_t)((v>>1)+(v&1)));return true;}
    if (routine==0x8513eu) {SB(0x9a,B(0x9a)+1u);return true;}
    return false;
}
/* The tail 083E9E / 0839F4: 0 go on, 1 deleted. */
static int fl_tail(neo_gactor_t *a,neo_genv_t *e,uint32_t crash,uint32_t killed,bool mark60) {
    if (BIT(0x5f,0)) SL(0,crash);
    if (ga_register(a,e,NULL,NULL)) {
        if (!BIT(0x5f,7) && B(0x5a)==6u) sound(e,0x108d);
        SB(0x5a,0);
    }
    if (ga_attack(a,e,0xe4c94)) {if (mark60) SB(0x60,1);SL(0,crash);}
    if (BIT(0x5f,7)) {if (mark60) SB(0x60,1);SL(0,killed);}
    return ga_cull(a,e,0xe4c58)?1:0;
}
static ga_result_t bomber_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;
    switch (L(0)) {
    case 0x83c8a: {                                                          /* 084CDA */
        uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72);uint32_t d3=L(0x74),d4=L(0x78),d5=L(0x7c);
        for (unsigned o=0x6e;o<0x80;o+=2) SW(o,0);
        SW(0x84,d0);SW(0x86,d1);SW(0x88,d2);SL(0x8c,d3);SL(0x90,d4);SL(0x94,d5);
        SW(0x4e,rank(a,e,0x2d5466));SL(0x6e,(uint32_t)(int32_t)S16(rank(a,e,0x2d5480))<<8);
        SB(0x99,0);
        uint32_t v=L(0x6e);
        if (S16(W(0x3c))<0xa0) SET(0x15,0); else v=0u-v;
        SL(0x6e,v);
        ga_palette(a,0x244);ga_flash_palette(a,0x850);SW(0x66,0x8800);SW(0x1e,0x8800);SL(0x50,0xe4c70);
        ga_floor(a,e);k1b1dc(a);SB(0x61,0);sound(e,0x105d);START(0x2820f8);
        child(a,e,0x8443e,0x1c,0,0);spawn_plain(a,e,0x84098,0x0e);
        SL(0x30,ASR(L(0x6e),1));SL(0,0x83d80);
    }   /* fall through */
    case 0x83d80:
        fl_land(a,e,NULL);fl_bounce(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SL(0x30,L(0x30)+ASR(L(0x6e)-L(0x30),3));
        if (BIT(0x15,6)) {
            if (B(0x99)) {ga_fail(e,0x83df8);return GA_UNSUPPORTED;}
            neo_gactor_t t=*a;to_root(&t,e);                                 /* 01AC4C to the root 10E968 */
            int16_t x=S16(ga_w(&t,0x28));
            if (!BIT(0x15,0)?x<=S16(W(0x84)):x>=S16(W(0x84))) {SB(0x60,1);SL(0,0x83e2c);}
        }
        goto fl_end;
    case 0x83e2c:
        START(0x282154);spawn_plain(a,e,0x841a0,0x0e);SB(0x60,0);SL(0x62,L(0x30));SL(0,0x83e5e);
        /* fall through */
    case 0x83e5e:
        ga_ground(a,e,NULL);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) {SB(0x60,1);SL(0,0x83f28);}
        if (!BIT(0x15,6)) {SB(0x60,1);SL(0,0x83f28);}
        SL(0x62,L(0x62)+ASR(L(0x6e)-L(0x62),3));
    fl_end:
        if (fl_tail(a,e,0x8404e,0x84010,true)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x83f28: {
        sound(e,0x105c);
        neo_gactor_t *c=child(a,e,0x842a8,0x1c,0x18,8);
        SL(0x30,ASR(L(0x30),1));
        if (c) ga_sl(c,0x30,0u-ASR(L(0x30),1));
        SL(0x34,(uint32_t)(int32_t)(int16_t)((ga_rand(e)&0x300u)+0x400u)<<8);
        SW(0x2c,W(0x2c)+0x20u);SET(0x15,5);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);START(0x2821ac);SL(0,0x83fae);
    }   /* fall through */
    case 0x83fae:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                            /* 0143DC */
        if (ga_anim_step(a,e,fl_call)<0) return GA_UNSUPPORTED;
        if (BIT(0x15,6)) SL(0,0x8407e);
        if (ga_attack(a,e,0xe4cb4)) SL(0,0x8407e);
        ga_register(a,e,NULL,NULL);SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0x84032);
        if (ga_cull(a,e,0xe4c58)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x84010:child(a,e,0x843f8,0x1c,0,0);
        /* fall through */
    case 0x84032: case 0x83a72:
        sound(e,0x1021);SB(0x69,0xff);k84e74(a,e);k84f74(a,e);   /* 019DFC: +69 = FF */
        return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
    case 0x8404e:
        if (S16(W(0x30))>=0) SET(0x5b,7);
        child(a,e,0x843f8,0x1c,0,0);
        /* fall through */
    case 0x8407e: case 0x83a8e:
        sound(e,0x1021);SB(0x69,0xff);k84ee2(a,e);              /* 019DFC: +69 = FF */
        return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
    case 0x8391c: {                                                          /* 084C9A */
        uint16_t d0=W(0x6e),d1=W(0x70);SW(0x6e,0);SW(0x70,0);SW(0x86,d0);SW(0x88,d1);
        SW(0x4e,rank(a,e,0x2d5432));SL(0x6e,(uint32_t)(int32_t)S16(rank(a,e,0x2d544c))<<8);
        uint32_t v=L(0x6e);
        if (S16(W(0x3c))<0xa0) SET(0x15,0); else v=0u-v;
        SL(0x6e,v);
        ga_palette(a,0x244);ga_flash_palette(a,0x850);SW(0x66,0x8800);SW(0x1e,0x8800);SL(0x50,0xe4c70);
        ga_floor(a,e);k1b1dc(a);sound(e,0x105e);START(0x281ffc);
        spawn_plain(a,e,0x83b30,0x0e);child(a,e,0x83b92,0x1c,0,0);
        SL(0x30,ASR(L(0x6e),1));SL(0,0x839d2);
    }   /* fall through */
    case 0x839d2: {
        uint32_t x=L(0x28),y=L(0x2c),tag=L(0x38),kind=0;                    /* 084D94 */
        fl_land(a,e,&kind);
        if (BIT(0x15,6)) {
            uint16_t d7=(uint16_t)kind;
            if (!BIT(0x15,0)?d7<0x4000u:S16(d7)>0x4000) {SL(0x38,tag);SL(0x2c,y);SL(0x28,x);ga_project(a,e);SL(0,0x83a8e);}
            else fl_bounce(a,e);
        }
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SL(0x30,L(0x30)+ASR(L(0x6e)-L(0x30),4));
        if (fl_tail(a,e,0x83a8e,0x83a72,false)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x83b92:
        p=sh_creator(a,e);ga_place(a,e,BIT(0x15,0)?-0x14:0x14,0,p?NEO_GA_TAG(p->index):0);SL(0,0x83bb0);
        /* fall through */
    case 0x83bb0:
        ga_project(a,e);
        if (!(e->video&3u)) {
            p=sh_creator(a,e);if (p) SW(0x1e,ga_w(p,0x1e));
            child(a,e,0x1c82a,0x2a,0,0);
        }
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8443e:
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);ga_palette(a,0x177);START(0x2823b2);SL(0,0x8446c);
        /* fall through */
    case 0x8446c:
        ga_project(a,e);
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x84472);return GA_UNSUPPORTED;}
        SW(0x1e,ga_w(p,0x1e));
        if (ga_b(p,0x61)) SL(0,0x844c2);
        if (ga_b(p,0x15)&0x40u) {if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;}
        else START(0x2823b2);
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x844c2:
        to_root(a,e);START(0x2823e0);SL(0,0x844de);
        /* fall through */
    case 0x844de:
        ga_project(a,e);
        p=sh_creator(a,e);if (p) SW(0x1e,ga_w(p,0x1e));
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x84098: case 0x841a0: {
        bool second=L(0)==0x841a0u;
        SB(0x1d,0);SW(0x66,0x8800);SW(0x1e,0x8800);ga_palette(a,0x220);START(second?0x282182:0x282126);
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);k1aca2(a,e);SL(0,second?0x841e6:0x840de);
    }   /* fall through */
    case 0x840de: case 0x841e6:
        ga_project(a,e);
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x840e4);return GA_UNSUPPORTED;}
        SW(0x1e,ga_w(p,0x1e));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_b(p,0x60)) return GA_DELETE;
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x842a8:
        SW(0x66,0x8820);SW(0x1e,0x8820);START(0x282362);ga_palette(a,0x20e);
        SL(0x34,(uint32_t)(int32_t)(int16_t)((ga_rand(e)&0x300u)+0x200u)<<8);SL(0,0x842ee);
        /* fall through */
    case 0x842ee: {
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                            /* 0143DC */
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (!r) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        /* 084300: the soldier 0AF17E with the creator's +8C..+97 as its
         * words +6E..+79. The world does not create a soldier from a generic
         * node yet: counted there (external), and 084344 then has no node. */
        neo_gactor_t *c=spawn_plain(a,e,0xaf17e,0x1c);
        p=sh_creator(a,e);
        if (c && p) for (unsigned o=0;o<12;o+=2) ga_sw(c,0x6eu+o,ga_w(p,0x8cu+o));
        SL(0x7a,c?NEO_GA_TAG(c->index):0);SL(0,0x84344);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x84344: {
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                            /* 0143DC */
        neo_gactor_t *c=L(0x7a) && e->actor?e->actor(e->ctx,L(0x7a)):NULL;
        if (c) {ga_place_at(c,a,e,0,0);ga_sb(c,0x15,B(0x15)&3u);ga_sl(c,0x30,L(0x30));ga_sl(c,0x34,L(0x34));}   /* 01ACF2, 01AC94 */
        return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
    }
    case 0x843f8:
        SW(0x66,0x8820);SW(0x1e,0x8820);SB(0x1d,0);ga_palette(a,neo_blood_palette(NEO_BLOOD_SOLDIER_DEATH,e->blood));   /* 0405EA */
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x8440a);return GA_UNSUPPORTED;}
        SB(0x5b,ga_b(p,0x5b));
        if (!(e->video&1u)) {SB(0x5a,5);SET(0x15,6);} else SB(0x5a,7);
        return death(a,e);                                                   /* JMP 0406B6 */
    case 0x84b9c:
        ga_palette(a,0x204);START(0x28240a);
        if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);                          /* 01DEA2 */
        CLR(0x15,1);SL(0x50,0);SL(0,0x84bd0);
        /* fall through */
    case 0x84bd0:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        if (ga_cull_rect(a,e,0xe4c60)) return GA_DELETE;
        if (L(0x76)) ga_attack(a,e,L(0x76));
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x84588: {
        unsigned d0=e->players.active_mask&0xffu;
        if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
        if (!d0) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        SL(0x7a,d0==2u?P2_NODE:P1_NODE);
        uint16_t w6e=W(0x6e),w70=W(0x70);SW(0x6e,0);SW(0x70,0);                   /* 084D4A */
        if (!w6e) SET(0x15,0);
        SW(0x84,(uint16_t)(w70<<3));SW(0x4e,rank(a,e,0x2d549a));
        SL(0x6e,(uint32_t)(int32_t)S16(rank(a,e,0x2d54b4))<<8);
        ga_palette(a,0x244);ga_flash_palette(a,0x850);SW(0x66,0x8800);SW(0x1e,0x8800);SL(0x50,0xe4c70);
        SL(0x30,S16(W(0x3c))<0xa0?L(0x6e):0u-L(0x6e));
        sound(e,0x105d);START(0x281ffc);spawn_plain(a,e,0x83b30,0x0e);SL(0,0x8460e);
    }   /* fall through */
    case 0x8460e: {
        ga_move(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        uint32_t n=L(0x7a);
        if (live(e,n,NULL)) {
            int16_t d0=S16(absw((uint16_t)(px(e,n)-W(0x3c))));
            bool d1=(int16_t)(d0-S16(W(0x84)))<0;
            unsigned pi=n==P2_NODE;
            if (!e->vehicle || !e->vehicle->player[pi]) {ga_fail(e,0x84642);return GA_UNSUPPORTED;}   /* the player's +44 */
            uint16_t cx=(uint16_t)(e->vehicle->player[pi][0x44]<<8|e->vehicle->player[pi][0x45]);
            d0=S16(absw((uint16_t)(cx-W(0x44))));
            bool d2=(int16_t)(d0-S16(W(0x84)))<0;
            int16_t x=S16(W(0x3c));
            if (d1!=d2 && x>0x0a && x<0x136) SL(0,0x846ce);
        }
        if (ga_register(a,e,NULL,NULL)) {
            if (!BIT(0x5f,7) && B(0x5a)==6u) sound(e,0x108d);
            SB(0x5a,0);
        }
        if (BIT(0x5f,7)) SL(0,0x846da);
        if (ga_cull(a,e,0xe4c58)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x846ce:spawn_plain(a,e,0x8471a,0x0e);SL(0,0x846da);return e->unsupported?GA_UNSUPPORTED:GA_MORE;   /* 084F50 */
    case 0x846da:
        sound(e,0x1021);child(a,e,0x84948,0x1c,0,0);SB(0x69,0xff);k84e74(a,e);
        return GA_DELETE;
    case 0x83b30:
        SB(0x1d,0);ga_palette(a,0x220);START(0x28207a);
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);k1aca2(a,e);SL(0,0x83b6a);
        /* fall through */
    case 0x83b6a:
        ga_project(a,e);
        p=sh_creator(a,e);if (p) SW(0x1e,ga_w(p,0x1e));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8471a: {
        SB(0x1d,0);ga_palette(a,0xbd);START(0x2824f4);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
        /* 08508E */
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x850f0);return GA_UNSUPPORTED;}
        uint32_t t=ga_l(p,0x7a);
        int16_t dy=(int16_t)(rank(a,e,0x2d54e8)+0x100u-W(0x40));
        int16_t sp=S16(rank(a,e,0x2d54ce));SL(0x34,(uint32_t)(int32_t)sp<<8);
        uint32_t g=k84fae(sp,dy);SL(0x7e,g);
        uint32_t ag=(g&0x80000000u)?0u-g:g;
        uint16_t div=ag<0x10000u?(uint16_t)ag:0xffffu;
        uint16_t steps=(uint16_t)(L(0x34)/div);
        uint16_t ahead=(uint16_t)(rank(a,e,0x2d54e8)+0x100u);
        int16_t d1=(int16_t)(py(e,t)-ahead);
        int16_t d0=(int16_t)(ahead-W(0x40));
        uint16_t time=d0<0?steps:k85026(e,(uint16_t)d0,d1,steps);
        uint32_t tx=((uint32_t)px(e,t)<<16)|(t==P2_NODE?e->player_low[1]:e->player_low[0]);
        SL(0x30,k84ff2(tx-L(0x3c),(int16_t)time));
        SW(0x8a,0);SW(0x82,0x0a);SL(0,0x8475e);
    }   /* fall through */
    case 0x8475e:
        SL(0x34,L(0x34)+L(0x7e));
        if (S32(L(0x34))>=0) ga_move(a,e);
        else {SW(0x66,0x1001);SW(0x1e,0x1001);ga_air(a,e);}
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SW(0x8a,W(0x8a)+k2f1a((int16_t)(uint16_t)(S32(L(0x30))>>8),(int16_t)(uint16_t)(S32(L(0x34))>>8)));
        if (S16(W(0x8a))>=0x1200) {
            SW(0x8a,W(0x8a)-0x1200u);
            neo_gactor_t *c=spawn_plain(a,e,0x848ee,0x2a);
            if (c) ga_place_at(c,a,e,0,0);
        }
        if (!W(0x82)) ga_attack(a,e,0xe4cc4); else SW(0x82,W(0x82)-1u);
        if (ga_cull(a,e,0xe4c58)) return GA_DELETE;
        if (BIT(0x15,6)) SL(0,0x8480e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8480e:
        SB(0x1d,0);ga_palette(a,0x113);START(0x24855c);SW(0x66,0x1001);SW(0x1e,0x1001);sound(e,0x1021);SL(0,0x8484e);
        /* fall through */
    case 0x8484e:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x848ee:SB(0x1d,0);ga_palette(a,0xbd);START(0x282518);SL(0,0x84918);
        /* fall through */
    case 0x84918:
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x84918);return GA_UNSUPPORTED;}
        SW(0x66,ga_w(p,0x66)-1u);SW(0x1e,ga_w(p,0x1e)-1u);ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x84948:
        SW(0x66,0x8820);SW(0x1e,0x8820);SB(0x1d,0);ga_palette(a,neo_blood_palette(NEO_BLOOD_SOLDIER_DEATH,e->blood));   /* 0405EA */
        SB(0x5a,5);SET(0x15,6);
        if (!BIT(0x15,0)) SB(0x5b,B(0x5b)|0x80u);
        return death(a,e);                                                 /* JMP 0406B6 */
    case 0x8497e: {
        SET(0x15,5);SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x245);SB(0x78,4);
        if (!(ga_rand(e)&7u)) goto abe;
        uint32_t row=0xe4cd4u+12u*(ga_rand(e)&3u);
        uint32_t d1=rl(e,row),d0=rl(e,row+8u);
        SL(0x6e,rl(e,row+4u));
        if (!BIT(0x15,0)) d0=0u-d0;
        SL(0x30,d0);SL(0x34,d1);SL(0x72,d1);
        SW(0x76,(ga_rand(e)&0x1fu)+0x18u);
        goto a3a;
    }
    case 0x84a02:SL(0x34,0x2aaaa);SL(0x6e,0xffff1c72u);goto a3a;         /* +30 kept */
    case 0x84a2e:SL(0x34,0u-ASR(L(0x72),1));
    a3a:START(0x28236e);SL(0,0x84a50);
        /* fall through */
    case 0x84a50:
        SL(0x72,L(0x34));ga_air(a,e);SL(0x34,L(0x34)+L(0x6e));
        if (BIT(0x15,6)) SL(0,0x84a82);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return deb_tail(a,e);
    case 0x84a82:
        SB(0x78,B(0x78)-1u);
        if ((int8_t)B(0x78)<0) goto abe;
        START(0x282388);SL(0,0x84aa0);
        /* fall through */
    case 0x84aa0:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x84a2e);
        return deb_tail(a,e);
    abe: {
        uint32_t d0=L(0x30);k1b1dc(a);
        if (S32(d0)<0x20000 && S32(d0)>(int32_t)0xfffe0000) d0=S32(d0)>=0?0x20000u:0xfffe0000u;
        SL(0x62,d0);START(0x28236e);SL(0,0x84b12);
    }   /* fall through */
    case 0x84b12:
        ga_ground(a,e,NULL);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SW(0x76,W(0x76)-1u);
        if (S16(W(0x76))<=0x0f) SET(0x5f,3);
        if (!(ga_rand(e)&0xfu)) SL(0,0x84a02);
        if (!BIT(0x15,6)) SL(0,0x84a02);
        if (S16(W(0x76))<=0 || S16(W(0x3c))<=-0x30 || ga_cull_rect(a,e,0xe4c68)) SL(0,0x84b7e);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x84b7e:return GA_DELETE_TREE;                                 /* 0041AC */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- oil drum 0391EA ------------------------------------------------------------ */
/* 0391EA (script records 2DA6B4 / 2DA800 of section 2): 039502 (re-parented
 * to the layer 10C72E at its own +28/+2C words), FFF1, 23C658 (its call
 * 039296: four smokes 08AE76 at (-28, 50) / (28, 50) / (-28, 18) / (28, 18)
 * with the speeds (-1.5, 1.5) / (1.5, 1.5) / (-1.5, -1.5) / (1.5, -1.5)),
 * palette 4AD, faces left, +50 = 03954E, +4E = 1; 039232: 01ABC8, 00427A,
 * 004C24: a hit of kind 13 -> 039260 (23C6B4, 01AC38, +30 -= 5000, +34 -=
 * A00); 039370: deleted at x <= -60. */
static bool box_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x39296u) return false;
    static const int16_t at[4][4]={{-0x28,0x50,-0x18,0x18},{0x28,0x50,0x18,0x18},{-0x28,0x18,-0x18,-0x18},{0x28,0x18,0x18,-0x18}};
    for (unsigned i=0;i<4;i++) {
        neo_gactor_t *c=child(a,e,0x8ae76,0x1c,at[i][0],at[i][1]);
        if (c) {ga_sl(c,0x30,(uint32_t)(int32_t)at[i][2]<<12);ga_sl(c,0x34,(uint32_t)(int32_t)at[i][3]<<12);}
    }
    return true;
}
static ga_result_t box_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x391ea:
        ga_place(a,e,S16(W(0x28)),S16(W(0x2c)),0x10c72eu);                /* 039502 */
        SW(0x66,0xfff1);SW(0x1e,0xfff1);START(0x23c658);ga_palette(a,0x4ad);CLR(0x15,0);
        SL(0x50,0x3954e);SW(0x4e,1);SL(0,0x39232);
        /* fall through */
    case 0x39232:
        ga_project(a,e);
        if (ga_anim_step(a,e,box_call)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) {if (B(0x5a)==0x13u) SL(0,0x39260);SB(0x5a,0);}
        break;
    case 0x39260:START(0x23c6b4);SL(0,0x39276);
        /* fall through */
    case 0x39276:
        ga_move(a,e);
        if ((r=ga_anim_step(a,e,box_call))<0) return GA_UNSUPPORTED;
        SL(0x30,L(0x30)+0xffffb000u);SL(0x34,L(0x34)+0xfffff600u);
        break;
    default:ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    if (S16(W(0x3c))<=-0x60) return GA_DELETE;                           /* 039370 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- armed truck 02B202 --------------------------------------------------------- */
/* 02AE46..02C5D6 (script records of sections 2 and 4; tables 02C42A..02C5D6,
 * rank tables 2CF330..2CF61E). 02B202: 02BDA6 (+80 = +74 the camera x that
 * turns it back, +86 = +6E, +7E = +70 the target x, +90 = +72; +6E..+74 = 0),
 * +91 bits 0 and 2, palette 244, 236D00 (its call 02BDFC: +72 = +76 >> 5),
 * 8800, faces right, +50 = 02C42A, +62 = (rank 2CF5CA & 7FFF) << 8, +4E =
 * rank 2CF330, +30 = +-+62 toward +7E, the gunner 02B61C (+91 bit 0) or the
 * rider 02B670. 02B2EA: once the camera passes +80 the target is -40 (it
 * leaves); 02B316: 01AC38, 01AD8C, 00427A, +30 eases by +62 / 8 toward
 * +-+62 (clamped), |x - +7E| < 8 clears +91 bit 2, with bit 0 the dynamic
 * map 02C5BA / 02C5C8 (0140C0, by bit 2). Tail 02AEAC: +61 = 0, 004C24,
 * +5A = 0, +5F bit 7 -> 02AE46 (sound 1021, 019DFC, 236D88: its call 02C3B4
 * makes 01C71E and two tyres 02BBBA, the second flipped; 02BE44 = 01B1DC +
 * 0143E4; the end -> 02B138), 01945C 02C4AE -> 02B138 (deleted).
 * - 02B61C (gunner): palette 245, 23739E, 01AD6E, 881C; 02B656: 02BEF2 (+61
 *   to the creator, +91 from it, the creator's +60 & F taken once: +60 = F
 *   there), 01AD8C, D0 == 9 -> 02B5DC (02BE98 selects the program 02C4BE
 *   [+60], a tyre 02BBBA, then 02B138; 01AD8C has overwritten D0 with the
 *   depth word, so the test never holds), 00427A, 00409A. The mounted gunner
 *   02B39E.. (programs 02C4BE by the creator's +60, missiles 02B83E /
 *   02B976) is another record's child, not reached here.
 * - 02B670 (rider): palette 177, 23787C; the creator's +60 == 9 -> 02B6E4
 *   (01AD52, 2378AA, deleted at its end); 019504 02C4AE, 00409A.
 * - 02BBBA (tyre): as the bomber's debris 08497E with +8F bounces, +82 life,
 *   rows 02C58A, programs 2373CC / 2373E6, 019504 02C4A6, 02B1CC (0041AC). */
static bool tr_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x2bdfc:SL(0x72,ASR(L(0x76),5));return true;
    case 0x2c3b4: {
        child(a,e,0x1c71e,0x1c,0,0);child(a,e,0x2bbba,0x2a,0,0);
        neo_gactor_t *c=child(a,e,0x2bbba,0x1c,0,0);
        if (c) ga_sb(c,0x15,ga_b(c,0x15)^1u);
        return true;
    }
    case 0x2c422:return true;
    }
    return false;
}
/* 02BEF2 (and 02BE98 with `program`): the creator's +60 & F once (-1: F). */
static int tr_sync(neo_gactor_t *a,neo_genv_t *e,bool program) {
    ga_project(a,e);
    neo_gactor_t *p=sh_creator(a,e);
    if (!p) {ga_fail(e,0x2bef2);return -1;}
    ga_sb(p,0x61,B(0x61));SB(0x91,ga_b(p,0x91));
    if (program) SW(0x1e,ga_w(p,0x1e));
    unsigned d0=ga_b(p,0x60)&0xfu;
    if (d0==0xfu) return 0xf;
    ga_sb(p,0x60,0xf);SB(0x60,d0);
    if (program && !ga_anim_start(a,e,rl(e,0x2c4beu+4u*d0),0x100)) return -1;
    return (int)d0;
}
/* 02AEAC */
static ga_result_t tr_tail(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x61,0);ga_register(a,e,NULL,NULL);SB(0x5a,0);
    if (BIT(0x5f,7)) SL(0,0x2ae46);
    if (ga_cull(a,e,0x2c4ae)) SL(0,0x2b138);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 02C18E / 02BD70 */
static ga_result_t tyre_tail(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x82,W(0x82)-1u);
    if (S16(W(0x82))<=0x0f) SET(0x5f,3);
    if (S16(W(0x82))<=0 || S16(W(0x3c))<=-0x30 || ga_cull_rect(a,e,0x2c4a6)) SL(0,0x2b1cc);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t truck_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;
    switch (L(0)) {
    case 0x2b202: {
        SW(0x80,W(0x74));SB(0x91,0);SW(0x86,W(0x6e));SW(0x7e,W(0x70));SB(0x90,W(0x72));   /* 02BDA6 */
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);
        SET(0x91,0);SET(0x91,2);ga_palette(a,0x244);START(0x236d00);ga_project(a,e);
        SW(0x66,0x8800);SW(0x1e,0x8800);SET(0x15,0);SB(0x61,0);SL(0x50,0x2c42a);
        SL(0x62,(uint32_t)(rank(a,e,0x2cf5ca)&0x7fffu)<<8);SW(0x4e,rank(a,e,0x2cf330));
        uint32_t d0=L(0x62);
        if (S16(W(0x3c))>S16(W(0x7e))) d0=0u-d0;
        SL(0x30,d0);
        child(a,e,BIT(0x91,0)?0x2b61c:0x2b670,0x0e,0,0);
        SL(0,0x2b2ea);
    }   /* fall through */
    case 0x2b2ea:
        if (W(0x7e)!=0xffc0u && W(0x80) && S16(W(0x80))<S16(e->camera)) {SW(0x7e,0xffc0);SL(0,0x2b316);}
        /* fall through */
    case 0x2b316: {
        ga_move(a,e);depth8000(a);
        if (ga_anim_step(a,e,tr_call)<0) return GA_UNSUPPORTED;
        uint32_t d0=L(0x62),d1=d0>>3;
        if (S16(W(0x3c))>S16(W(0x7e))) {
            d0=0u-d0;SL(0x30,L(0x30)-d1);
            if (S32(d0)>S32(L(0x30))) SL(0x30,d0);
        } else {
            SL(0x30,L(0x30)+d1);
            if (S32(d0)<S32(L(0x30))) SL(0x30,d0);
        }
        if (BIT(0x91,2) && S16(absw((uint16_t)(W(0x3c)-W(0x7e))))<8) CLR(0x91,2);
        if (BIT(0x91,0) && e->dyn_set) e->dyn_set(e->ctx,a,BIT(0x91,2)?0x2c5bau:0x2c5c8u,0,0);   /* 02C170 */
        return tr_tail(a,e);
    }
    case 0x2ae46:sound(e,0x1021);CLR(0x91,7);SB(0x69,0xff);START(0x236d88);SL(0,0x2ae72);
        /* fall through */
    case 0x2ae72:
        k1b1dc(a);ga_air(a,e);
        if ((r=ga_anim_step(a,e,tr_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x2b138);
        if (ga_cull(a,e,0x2c4ae)) SL(0,0x2b138);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x2b138:case 0x2b1d4:return GA_DELETE;
    case 0x2b1cc:return GA_DELETE_TREE;
    case 0x2b61c:
        ga_palette(a,0x245);START(0x23739e);
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);
        SW(0x66,0x881c);SW(0x1e,0x881c);SL(0,0x2b656);
        /* fall through */
    case 0x2b656:
        if ((r=tr_sync(a,e,false))<0) return GA_UNSUPPORTED;
        depth8000(a);
        /* 02B660 compares D0 after 01AD8C, which leaves +66 (- y when its
         * high nibble is 8) there, not 02BEF2's +60: never 9 for this gunner. */
        r=(W(0x66)&0xf000u)==0x8000u?(int)(uint16_t)(W(0x66)-W(0x40)):(int)W(0x66);
        if (r==9) {
            if (tr_sync(a,e,true)<0) return GA_UNSUPPORTED;                  /* 02BE98 */
            child(a,e,0x2bbba,0x1c,0,0);SL(0,0x2b138);
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        if (ga_anim_step(a,e,tr_call)<0) return GA_UNSUPPORTED;
        if (!ga_creator_alive(a,e)) SL(0,0x2b138);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x2b670:
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);
        ga_palette(a,0x177);START(0x23787c);SL(0,0x2b69e);
        /* fall through */
    case 0x2b69e:
        ga_project(a,e);
        if (ga_anim_step(a,e,tr_call)<0) return GA_UNSUPPORTED;
        p=sh_creator(a,e);
        if (p && ga_b(p,0x60)==9u) SL(0,0x2b6e4);
        if (ga_cull_rect(a,e,0x2c4ae)) SL(0,0x2b1cc);
        if (!ga_creator_alive(a,e)) SL(0,0x2b1cc);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x2b6e4:to_root(a,e);START(0x2378aa);SL(0,0x2b700);
        /* fall through */
    case 0x2b700:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,tr_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x2b1cc);
        if (ga_cull_rect(a,e,0x2c4ae)) SL(0,0x2b1cc);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- tyres ---- */
    case 0x2bbba: {
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x245);SB(0x8f,4);
        if (!(ga_rand(e)&7u)) goto slide;
        uint32_t row=0x2c58au+12u*(ga_rand(e)&3u);
        uint32_t d1=rl(e,row),d0=rl(e,row+8u);
        SL(0x6e,rl(e,row+4u));
        if (!BIT(0x15,0)) d0=0u-d0;
        SL(0x30,d0);SL(0x34,d1);SL(0x76,d1);
        SW(0x82,(ga_rand(e)&0x1fu)+0x18u);
        goto c6e;
    }
    case 0x2bc36:SL(0x34,0x2aaaa);SL(0x6e,0xffff1c72u);goto c6e;
    case 0x2bc62:SL(0x34,0u-ASR(L(0x76),1));
    c6e:START(0x2373cc);SL(0,0x2bc84);
        /* fall through */
    case 0x2bc84:
        SL(0x76,L(0x34));ga_air(a,e);SL(0x34,L(0x34)+L(0x6e));
        if (BIT(0x15,6)) SL(0,0x2bcac);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return tyre_tail(a,e);
    case 0x2bcac:
        SB(0x8f,B(0x8f)-1u);
        if ((int8_t)B(0x8f)<0) goto slide;
        START(0x2373e6);SL(0,0x2bcca);
        /* fall through */
    case 0x2bcca:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x2bc62);
        return tyre_tail(a,e);
    slide: {
        uint32_t d0=L(0x30);k1b1dc(a);
        if (S32(d0)<0x20000 && S32(d0)>(int32_t)0xfffe0000) d0=S32(d0)>=0?0x20000u:0xfffe0000u;
        SL(0x62,d0);START(0x2373cc);SL(0,0x2bd3c);
    }   /* fall through */
    case 0x2bd3c:
        ga_ground(a,e,NULL);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0x0f) SET(0x5f,3);
        if (!(ga_rand(e)&0xfu)) SL(0,0x2bc36);
        if (!BIT(0x15,6)) SL(0,0x2bc36);
        if (S16(W(0x82))<=0 || S16(W(0x3c))<=-0x30 || ga_cull_rect(a,e,0x2c4a6)) SL(0,0x2b1cc);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- floating platform 08AFB6 ---------------------------------------------------- */
/* 08AFB6..08B55C (record 2DAC6A of section 2; tables 0E5392..0E54A0). 08B372
 * takes the record words: +34 = -(+6E or 100) << 8 (it rises), +70 / +72 =
 * -+70 / -+72 (the heights that end it and that start its wave +7E), +80 =
 * +74 (sinks instead of fading), +74 = +76 or 400 (the wobble step), +7A =
 * (+78 or 4) << 8 (its amplitude). 08AFB6: +6E = 0, +7F bits 2 and 3;
 * 08AFD0: +6E = 1; 08AFDE: +6E = 1 with the bits. 08AFF8: +7E = 0, the floor
 * 08B3E6 (+81 = FF when none, +6E copied), faces right under x A0, palette
 * 0E53BA[+6E], 0800, +30 = 0, program 0E5392[+6E]. 08B090: 08B31A (the
 * wobble: +30 = (sin(+76 + +74) - sin(+76)) * +7A, 01AC38 with bit 3 else
 * 0143E4), 00427A; without bit 2 the probe 01495C up 400 px (its distance
 * >= +72: +7E = 1, >= +70: 08B138; the ground: 08B18E), with it the y of the
 * scene root (01AC4C) negated against +72 / +70; +81 -> 08B138; 01945C 0E53F2
 * -> deleted. 08B138: +80 -> 08B144 (sinks with 0E539A: not reached, an
 * explicit stop), else 08B18E (+7F bit 1): +6E = 0: 285D3A and +7C = 10
 * then 8 blinking updates (+5F bit 3) and deleted; +6E = 1: 01AD52, 285D68,
 * drifts (+30 -> +-20000 by 1/8, +34 -> 40000 by 1/16).
 * - 08B3E6 (floor): program 0E53A2[+6E], palette 0E53BE[+6E], 8848, 01AD6E,
 *   01ACA2; 08B43E: 00409A, the creator's +7F bit 0 once -> 0E53AA, bit 1 ->
 *   08B4A8 (0E53B2, 01AD52, 01B1DC; 08B4D6: the end -> 013F1E, the debris
 *   0E53FA and 0E540C, deleted); else 01ABC8, 08B52A (0140C4: the map
 *   0E53E6[creator +7E] at (+-+82, +83), set by its programs), 01AD8C,
 *   00427A. */
static int16_t sin_b(neo_genv_t *e,uint8_t i) {return S16(rw(e,0x29fcu+2u*i));}
/* 08B31A */
static void k8b31a(neo_gactor_t *a,neo_genv_t *e) {
    int16_t d1=sin_b(e,B(0x76));
    SW(0x76,W(0x76)+W(0x74));
    int16_t d2=sin_b(e,B(0x76));
    SL(0x30,(uint32_t)((int32_t)(int16_t)(d2-d1)*S16(W(0x7a))));
    if (BIT(0x7f,3)) ga_move(a,e); else ga_air(a,e);
}
/* 08B120 */
static ga_result_t raft_end(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0xe53f2)) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t raft_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;
    switch (L(0)) {
    case 0x8afb6:case 0x8afd0:case 0x8afde: {
        uint32_t self=L(0);
        uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72),d3=W(0x74),d4=W(0x76),d5=W(0x78);   /* 08B372 */
        for (unsigned o=0x6e;o<0x7a;o+=2) SW(o,0);
        if (!d0) d0=0x100;
        SL(0x34,0u-((uint32_t)(int32_t)S16(d0)<<8));
        SW(0x70,0u-d1);SW(0x72,0u-d2);SB(0x80,d3);
        SW(0x74,d4?d4:0x400u);
        SW(0x7a,(uint16_t)((d5?d5:4u)<<8));
        SW(0x6e,self==0x8afb6?0:1);
        if (self!=0x8afd0) {SET(0x7f,2);SET(0x7f,3);}
        SB(0x7e,0);
        neo_gactor_t *c=child(a,e,0x8b3e6,0x0e,0,0);
        SB(0x81,c?0:0xff);
        if (c) ga_sw(c,0x6e,W(0x6e));
        ga_project(a,e);
        if (S16(W(0x3c))<0xa0) SET(0x15,0);
        ga_palette(a,rw(e,0xe53bau+2u*W(0x6e)));SW(0x66,0x800);SW(0x1e,0x800);SL(0x30,0);
        START(rl(e,0xe5392u+4u*W(0x6e)));SL(0,0x8b090);
    }   /* fall through */
    case 0x8b090:
        k8b31a(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (!BIT(0x7f,2)) {
            int16_t dy;
            if (ga_probe_vertical_dy(a,e,-0x400,&dy)) {
                if (dy>=S16(W(0x72))) SB(0x7e,1);
                if (dy>=S16(W(0x70))) SL(0,0x8b138);
            }
            if (BIT(0x15,6)) SL(0,0x8b18e);
        } else {
            /* 01AC4C to the scene root: the y long less the parents' previous y. */
            neo_contact_parent_t op,np;uint32_t y=L(0x2c),tag=L(0x38);
            if (tag!=0x10e968u) {
                if (tag && ga_parent(e,tag,&op)) y+=(uint32_t)op.previous_y;
                if (ga_parent(e,0x10e968u,&np)) y-=(uint32_t)np.previous_y;
            }
            int16_t d1=(int16_t)(0u-(y>>16));
            if (d1>=S16(W(0x72))) SB(0x7e,1);
            if (d1>=S16(W(0x70))) SL(0,0x8b138);
        }
        if (B(0x81)) SL(0,0x8b138);
        return raft_end(a,e);
    case 0x8b138:
        if (B(0x80)) {ga_fail(e,0x8b144);return GA_UNSUPPORTED;}
        /* fall through */
    case 0x8b18e:
        SET(0x7f,1);
        if (W(0x6e)==0) {START(0x285d3a);SW(0x7c,0x10);SL(0,0x8b1c4);goto c4;}
        to_root(a,e);START(0x285d68);SL(0x30,0);SL(0,0x8b214);
        /* fall through */
    case 0x8b214: {
        uint32_t d0=BIT(0x15,0)?0x20000u:0xfffe0000u;
        SL(0x30,L(0x30)+ASR(d0-L(0x30),3));SL(0x34,L(0x34)+ASR(0x40000u-L(0x34),4));
        ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return raft_end(a,e);
    }
    case 0x8b1c4:
    c4:
        SW(0x7c,W(0x7c)-1u);
        if (S16(W(0x7c))<0) {
            if (BIT(0x5f,3)) return GA_DELETE;
            SET(0x5f,3);SW(0x7c,8);
        }
        k8b31a(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return raft_end(a,e);
    /* ---- floor ---- */
    case 0x8b3e6:
        START(rl(e,0xe53a2u+4u*W(0x6e)));ga_palette(a,rw(e,0xe53beu+2u*W(0x6e)));
        SW(0x66,0x8848);SW(0x1e,0x8848);
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);k1aca2(a,e);SL(0,0x8b43e);
        /* fall through */
    case 0x8b43e:
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x8b452);return GA_UNSUPPORTED;}
        if (!BIT(0x7f,0) && (ga_b(p,0x7f)&1u)) {SET(0x7f,0);START(rl(e,0xe53aau+4u*W(0x6e)));}
        if (ga_b(p,0x7f)&2u) {
            START(rl(e,0xe53b2u+4u*W(0x6e)));to_root(a,e);k1b1dc(a);SL(0,0x8b4d6);
            goto d6;
        }
        ga_project(a,e);
        /* 08B52A: 0140C4 with the map 0E53E6[creator +7E] at (+-+82, +83). */
        {uint32_t map=rl(e,0xe53e6u+(uint8_t)(ga_b(p,0x7e)<<2));   /* 08B52E..08B540: the byte offset (+7E * 4) & FF */
         int16_t dx=(int8_t)B(0x82),dy=(int8_t)B(0x83);
         if (BIT(0x15,0)) dx=(int16_t)-dx;
         if (e->dyn_set) e->dyn_set(e->ctx,a,map,dx,dy);}
        depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8b4d6:
    d6:
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x8b4d6);return GA_UNSUPPORTED;}
        ga_project(a,e);
        {uint32_t map=rl(e,0xe53e6u+(uint8_t)(ga_b(p,0x7e)<<2));   /* 08B52E..08B540: the byte offset (+7E * 4) & FF */
         int16_t dx=(int8_t)B(0x82),dy=(int8_t)B(0x83);
         if (BIT(0x15,0)) dx=(int16_t)-dx;
         if (e->dyn_set) e->dyn_set(e->ctx,a,map,dx,dy);}
        depth8000(a);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8b502);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8b502:dyn_release(a,e);debris40(a,e,0xe53fa);debris40(a,e,0xe540c);return GA_DELETE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- dispatch ----------------------------------------------------------------- */
static const uint32_t deaths[]={0x40dc0,0x40df0,0x40d06,0x40db4,0x40ae0,0x40c8e};
static const uint32_t splashes[]={0x40f88,0x40f60,0x40f74,0x40fd4};
static const uint32_t grenadier[]={0x8a732,0x8a788,0x8a798,0x8a7ae,0x8a838,0x8a84e,0x8a878,0x8a88e,0x8aa6e,0x8aaaa,
    0x8ab70,0x8aba0,0x8ac54,0x8ad18,0x8ae70,0x8ae76,0x8aea6};
static bool in(const uint32_t *k,unsigned n,uint32_t r) {for (unsigned i=0;i<n;i++) if (k[i]==r) return true;return false;}
#define IN(t,r) in(t,sizeof t/sizeof t[0],r)
static const uint32_t windowed[]={0x8da12,0x8da34,0x8dad0,0x8dae4,0x8db2a,0x8db40,0x8db66,0x8db7c,0x8db98,0x8dbac,0x8dbe8,0x8dbfe,0xb05fe,0xb0626,0xb0632};
static bool shielded(uint32_t r) {return (r>=0x9ae1e && r<0x9cb70) || r==0x9ba2a;}
/* ---- rider vehicle 027F54 --------------------------------------------------------- */
/* 027F54..02916A (record 2DA4A6 of section 2 at camera 1264: an armed car
 * with a rider; tables 029084..02916A, ranks 2CCFF8..2CD19E). 028FB2 takes
 * the record words: +6E != 0 -> +7D bit 5 (it idles instead of driving),
 * +70 -> bit 3 (the bomb 0286F8 starts 8 px lower), +74 -> bit 2 (palette
 * 3CC instead of 2F2), +7C = +72 (the rider's mode: 0 stays on, 1 jumps off,
 * 2 random), +7E = +76 / +80 = +78 (the item 08CDB2 dropped by 029010, -1
 * none); +6E..+78 = 0. Constructor: 0146E8, 01ABC8, 8810, flash 305, +4E =
 * rank 2CCFF8, +76 = rank 2CD0BC, x >= A0 -> bit 6 (the far screen range of
 * 028D52), +50 = 029084, solid 0290E8, exhaust 01DC8E (0), 0143E4; +6C == 3
 * -> 02802C (235BAA until +6C changes), else 028052: with bit 5 the idle
 * 028118 (235BAA, 01436A; 028DA8: +76 expired with a player on screen ->
 * the burst 02834A; 028DEA: +7B expired and no player in front -> the turn
 * 02818E: 235B1A, +7B = rank 2CD16A, +26 from rank 2CD184, then 028118),
 * else the drive 0280B6 (sound 109D, 235A96 / 235AD8 by 028D24 = facing !=
 * bit 7, +62 = -+1 << 16, +26 by 028D04 from the +78 counter; 0146A8; a
 * contact, +30 = 0 or the screen edge 028D52 -> 02829E (bit 7 flipped) /
 * 0282A4 (235BE8 / 235C30 brake, 109D stopped, +72 = -+62 >> 5; 028300
 * slows to 028118); 028DA8 / 028DEA -> 0282A4; row 0290A8). 02820A: 028DA8
 * -> 02834A, bit 5 -> 028118, else 235C78 / 235D58 (+72 = +-0x10000 >> 5)
 * and 028272 accelerates to 028052. 02834A: 235E38 then 028386: +7A = rank
 * 2CD11E bursts of +76 = rank 2CD138 (235F3E when +7A > 1 and +76 <= 3C,
 * else 235EAA; 0283F0 counts them down, 02839C re-ranks +76; then 028118
 * with +76 = rank 2CD0BC). 028450 (235FB6, unreached) and the flinch 02849A
 * (235FE0, 109D stopped) go back to 02820A. Tails 0285F6 (004C24: a hit of
 * 01B206 -> 02849A), 028650 and 028614 (+5F bit 7 -> 0284F2, or 02858C for
 * a hit of 01B206; 01945C 0290FE -> 028660 deleted). Death 0284F2: 01C71E
 * at (-18, 0), +60 = 1 (the rider clears it), 23606E; 02853E waits for +60
 * = 0 -> 028572 (23611C) / 02858C (236170) -> 0285A2: 019DFC, 0285B4 until
 * bit 0 of +7D is clear -> 0285EA (028EAA: sound 1033, 01C71E, 16 x 01D31C)
 * -> 028660. Program calls: 028E16 (1033, debris 01DA40 029112), 028E42 /
 * 028E5A (carry tests), 028E62 (028C76), 028E86 (028CA4, D0 = 2A), 028EAA,
 * 028EDE (the rider 0287FA, D0 = 0E, +7C copied), 028F08 (1067, the bomb
 * 0286F8 at (-1E, 2C), the shell 0287AC at (0, A)), 028F5E (10AA, 028C3E
 * and 028A88 at (-10, 10)), 029010 (the item), 029034 (+5F bit 3), 02903C
 * (8820; bit 0 of +7D: the wreck stamp 0C88B0 029124 / 029146 by bit 1 and
 * the row 0290D8; solid released).
 * - 0287FA (rider): 01AD6E (+-4, 28) on the creator, palette 21A, faces the
 *   nearest player (01AF9A), +7C (2: random bit): nonzero -> +7C = 0, bit 4,
 *   23638A then 2363D8 then 0288D2 (028BBC, 236356); zero -> +7C = 1, 236314;
 *   028930 (028E2C: 01ABC8, +1E = the creator's - 1) -> 028948: flipped,
 *   +34 = 44444, +6E = FFFF6E5E, +30 = +-11111, 8820, the creator's +60 = 0,
 *   JMP 0B0626 (+7C == 1) / 0B0632: a soldier. Tail 028A5C: 004C24, +5F bit
 *   7 -> 0289A6 (23643A / 2364D4 by the facing, 028BA8 / 028BBC by bit 4,
 *   the creator's +60 = 0; 028A32: +1E + 1, ends or the creator gone ->
 *   028660); 00409A -> 028660.
 * - 0286F8 (bomb): +30 = +-rank 2CD05A << 8, bit 3 -> y + 8, 236244, palette
 *   3CF, E0EF, +15 bit 5; 028762: 0143E4, row 0290C8 or +15 bit 7 -> 028B8C
 *   (009F88, 1001, +50 = 0, JMP 01C706), 019504 0290F6 -> 0286F0.
 * - 0287AC (shell): palette 208, facing flipped, 1001, 236272; 0287E2 ends
 *   -> 0286F0.
 * - 028A88 (engine): E0EF, +15 bit 5, palettes 216 / 3D2 / 367, +30 =
 *   +-10000, +4E = 1, 236758; 028AFA: 0143E4, +30 += +30 >> 5, +34 += +6E,
 *   +14 = the byte +19 / +1A / +1B by 10000E & 3, row 0290B8 / +15 bit 7 /
 *   +5F bit 7 -> 028B8C, 019504 0290F6 -> 0286F0.
 * - 028BA8 / 028BBC (helmet pieces 23660C / 2366B2): palette 21A, +34 =
 *   99999, +6E = FFFF0A3E, +30 = CCCC, E0ED; 028C04: 0143E4, +34 += +6E, +15
 *   bit 7 or 019504 0290F6 -> 0286F0.
 * - 028C3E (23657A on the creator, palette 218, 1001), 028C76 (2361DE, 348,
 *   E0ED), 028CA4 (22D372, 204, E0ED): 028CD8 ends or 019504 -> 0286F0.
 * The cartridge checks before 004188 / 0041AC are not reproduced. Proven by
 * the family m3car (capture A of captures-m3s2c: the idle car at 1264, its
 * flinch, death and rider); the drive, burst and bomb paths are literal. */
static bool cv(uint32_t r) {return r>=0x27f54 && r<0x28d04;}
/* 028FB2 */
static void cv_fb2(neo_gactor_t *a) {
    SB(0x7d,0);
    if (W(0x6e)) SET(0x7d,5);
    if (W(0x70)) SET(0x7d,3);
    if (W(0x74)) SET(0x7d,2);
    SB(0x7c,W(0x72));SW(0x7e,W(0x76));SW(0x80,W(0x78));
    SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);SW(0x76,0);SW(0x78,0);
}
/* 01DC8E: the exhaust 01D86C with +70 = d0 and +94 = 0. */
static void exhaust8e(neo_gactor_t *a,neo_genv_t *e,int16_t d0) {
    neo_gactor_t *c=child(a,e,0x1d86c,0x1c,0,0);
    if (!c) return;
    ga_sb(c,0x94,0);ga_sw(c,0x6e,0);ga_sw(c,0x70,(uint16_t)d0);ga_sw(c,0x72,0x3f);ga_sw(c,0x74,0x1f);
}
/* 028D04: +26 from the +78 counter. */
static void cv_d04(neo_gactor_t *a) {
    uint16_t d0=W(0x78);SW(0x78,d0+1u);d0>>=4;
    if (S16(d0)>3) d0=3;
    SW(0x26,(uint16_t)((d0+1u)<<7));
}
/* 028D24: carry when the facing differs from +7D bit 7. */
static bool cv_d24(neo_gactor_t *a) {return BIT(0x15,0)!=BIT(0x7d,7);}
/* 028D52: carry at the screen edge of the drive (bit 6: the far range). */
static bool cv_d52(neo_gactor_t *a) {
    int16_t x=S16(W(0x3c));bool left=S16(W(0x30))<0;
    if (!BIT(0x7d,6)) return left?x<0x28:x>0x90;
    return left?x<0xb0:x>0x118;
}
/* 028DA8: carry when +76 has expired with a live player and x in 10..120. */
static bool cv_da8(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x76,W(0x76)-1u);
    if (S16(W(0x76))>0) return false;
    SW(0x76,0);
    uint32_t n;
    if (nearest(a,e,&n)) return false;
    int16_t x=S16(W(0x3c));
    if (x<0x10 || x>0x120) return false;
    return true;
}
/* 028DEA: +7B counts down; carry when no live player is in front (01B0BE). */
static bool cv_dea(neo_gactor_t *a,neo_genv_t *e) {
    if ((int8_t)B(0x7b)>0) {SB(0x7b,B(0x7b)-1u);return false;}
    uint32_t n;
    if (nearest(a,e,&n)) return false;
    return k1b0be(a,e,&n);
}
/* 028E2C: 01ABC8 and the depth one above the creator's. */
static void cv_e2c(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    neo_gactor_t *p=sh_creator(a,e);
    if (!p) {ga_fail(e,0x28e2c);return;}
    SW(0x1e,ga_w(p,0x1e)-1u);
}
/* 028EAA: sound 1033, 01C71E and 01DBAA (16 x 01D31C at (0, 18)). */
static void cv_eaa(neo_gactor_t *a,neo_genv_t *e) {
    sound(e,0x1033);child(a,e,0x1c71e,0x1c,0,0);
    for (unsigned i=0;i<16;i++) child(a,e,0x1d31c,0x2a,0,0x18);
}
/* 029010 -> 08CDB2: the item +7E / +80 after the script actor at (0, 0). */
static void cv_010(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t kind=W(0x7e);
    if (kind==0xffffu || kind>=0x5au) return;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (!c) return;
    ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x80));ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
    ga_place_at(c,a,e,0,0);
}
static bool cv_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x28e16:sound(e,0x1033);debris40(a,e,0x29112);return true;
    case 0x28e42:case 0x28e5a:return true;                          /* carry tests: no effect on the node */
    case 0x28e62:child(a,e,0x28c76,0x1c,0,0);return true;
    case 0x28e86:child(a,e,0x28ca4,0x2a,0,0);return true;
    case 0x28eaa:cv_eaa(a,e);return true;
    case 0x28ede: {neo_gactor_t *c=child(a,e,0x287fa,0x0e,0,0);if (c) ga_sb(c,0x7c,B(0x7c));return true;}
    case 0x28f08:sound(e,0x1067);child(a,e,0x286f8,0x1c,-0x1e,0x2c);child(a,e,0x287ac,0x1c,0,0x0a);return true;
    case 0x28f5e:sound(e,0x10aa);child(a,e,0x28c3e,0x1c,0,0);child(a,e,0x28a88,0x1c,-0x10,0x10);return true;
    case 0x29010:cv_010(a,e);return true;
    case 0x29034:SET(0x5f,3);return true;
    case 0x2903c:
        SW(0x66,0x8820);SW(0x1e,0x8820);
        if (BIT(0x7d,0)) {
            if (e->stamp) e->stamp(e->ctx,a,BIT(0x7d,1)?0x29146u:0x29124u);   /* 0C88B0 */
            ga_attack(a,e,0x290d8);
        }
        solid(a,e,0);
        return true;
    }
    return false;
}
/* 028614 */
static ga_result_t cv_tail614(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x5f,7)) {SL(0,0x284f2);if (flinch(a,e)) SL(0,0x2858c);}
    SB(0x5a,0);
    if (ga_cull(a,e,0x290fe)) SL(0,0x28660);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 0285F6 */
static ga_result_t cv_tail5f6(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) {if (flinch(a,e)) SL(0,0x2849a);SB(0x5a,0);}
    return cv_tail614(a,e);
}
/* 028650 */
static ga_result_t cv_tail650(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    return cv_tail614(a,e);
}
/* 028A5C */
static ga_result_t cv_taila5c(neo_gactor_t *a,neo_genv_t *e) {
    ga_register(a,e,NULL,NULL);
    if (BIT(0x5f,7)) {SL(0,0x289a6);SB(0x5a,0);}
    if (!ga_creator_alive(a,e)) SL(0,0x28660);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 0146A8: ground step; a non-flat contact restores the position and sets
 * +5F bit 0 (as neo_armored.c). */
static void k146a8(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t x=L(0x28),y=L(0x2c),p=L(0x38),kind=0;
    ga_ground(a,e,&kind);
    if (BIT(0x15,6) && (uint16_t)kind!=0x4000u) {SL(0x38,p);SL(0x2c,y);SL(0x28,x);SET(0x5f,0);ga_project(a,e);}
}
static ga_result_t cv_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;uint32_t n;
again:
    switch (L(0)) {
    case 0x27f54:
        cv_fb2(a);CLR(0x7d,0);CLR(0x7d,1);ga_floor(a,e);ga_project(a,e);k1b1dc(a);
        SW(0x66,0x8810);SW(0x1e,0x8810);ga_palette(a,BIT(0x7d,2)?0x3cc:0x2f2);ga_flash_palette(a,0x305);
        SW(0x4e,rank(a,e,0x2ccff8));SW(0x76,rank(a,e,0x2cd0bc));
        if (S16(W(0x3c))>=0xa0) SET(0x7d,6);
        CLR(0x7d,7);SL(0x50,0x29084);solid(a,e,0x290e8);exhaust8e(a,e,0);ga_air(a,e);
        if (B(0x6c)!=3u) {SL(0,0x28052);goto again;}                /* 01B284 */
        START(0x235baa);SL(0,0x2802c);
        /* fall through */
    case 0x2802c:
        ga_air(a,e);
        if (B(0x6c)!=3u) SL(0,0x28052);
        if (ga_anim_step(a,e,cv_call)<0) return GA_UNSUPPORTED;
        ga_register(a,e,NULL,NULL);
        return cv_tail614(a,e);
    case 0x28052:
        if (BIT(0x7d,5)) {SL(0,0x28118);goto again;}
        sound(e,0x109d);k1b1dc(a);
        if (cv_d24(a)) {START(0x235ad8);SL(0x62,0x10000);} else {START(0x235a96);SL(0x62,0xffff0000u);}
        SW(0x78,0);cv_d04(a);SL(0,0x280b6);
        /* fall through */
    case 0x280b6:
        k146a8(a,e);
        if (BIT(0x5f,0)) SL(0,0x2829e);
        if (!L(0x30)) SL(0,0x2829e);
        if (ga_anim_step(a,e,cv_call)<0) return GA_UNSUPPORTED;
        if (cv_d52(a)) SL(0,0x2829e);
        if (cv_da8(a,e)) SL(0,0x282a4);
        if (cv_dea(a,e)) SL(0,0x282a4);
        ga_attack(a,e,0x290a8);
        return cv_tail5f6(a,e);
    case 0x28118:
        k1b1dc(a);SW(0x26,0x100);START(0x235baa);SL(0,0x2813a);
        /* fall through */
    case 0x2813a:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) {if (BIT(0x7d,5)) START(0x235baa); else SL(0,0x2820a);}
        if (cv_da8(a,e)) SL(0,0x2834a);
        if (cv_dea(a,e)) SL(0,0x2818e);
        return cv_tail5f6(a,e);
    case 0x2818e:
        k1b1dc(a);sound(e,0x109d);SB(0x7b,rank(a,e,0x2cd16a));
        SW(0x26,(uint16_t)(((rank(a,e,0x2cd184)&3u)+1u)<<7));START(0x235b1a);SL(0,0x281dc);
        /* fall through */
    case 0x281dc:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x28118);
        ga_attack(a,e,0x290a8);ga_register(a,e,NULL,NULL);SB(0x5a,0);
        return cv_tail614(a,e);
    case 0x2820a: {
        SW(0x26,0x100);
        if (cv_da8(a,e)) {SL(0,0x2834a);goto again;}                 /* bcs 02834A */
        if (BIT(0x7d,5)) {SL(0,0x28118);goto again;}
        uint32_t d0;
        if (cv_d24(a)) {START(0x235d58);d0=0x10000;} else {START(0x235c78);d0=0xffff0000u;}
        if (BIT(0x15,0)) d0=0u-d0;
        SL(0x72,ASR(d0,5));SL(0,0x28272);
    }   /* fall through */
    case 0x28272:
        k146a8(a,e);SL(0x62,L(0x62)+L(0x72));
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x28052);
        ga_attack(a,e,0x290a8);
        return cv_tail5f6(a,e);
    case 0x2829e:
        SB(0x7d,B(0x7d)^0x80u);
        /* fall through */
    case 0x282a4: {
        uint16_t d0=W(0x62);
        if (BIT(0x15,0)) d0=0u-d0;
        if (S16(d0)>0) START(0x235c30); else START(0x235be8);
        SW(0x78,0);SL(0x72,0u-ASR(L(0x62),5));sound(e,0x8000u|0x109d);SL(0,0x28300);   /* 012192 */
    }   /* fall through */
    case 0x28300:
        k146a8(a,e);
        if (L(0x72)) {if (L(0x62)) SL(0x62,L(0x62)+L(0x72)); else SL(0x72,0);} else SL(0x62,0);
        cv_d04(a);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x28118);
        cv_da8(a,e);
        return cv_tail5f6(a,e);
    case 0x2834a:
        k1b1dc(a);SW(0x26,0x100);START(0x235e38);SL(0,0x2836c);
        /* fall through */
    case 0x2836c:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x28386);
        return cv_tail650(a,e);
    case 0x28386:
        SW(0x26,0x100);SB(0x7a,rank(a,e,0x2cd11e));
        /* fall through */
    case 0x2839c:
        SW(0x76,rank(a,e,0x2cd138));
        if ((int8_t)B(0x7a)>1 && S16(W(0x76))<=0x3c) START(0x235f3e); else START(0x235eaa);
        k1b1dc(a);SL(0,0x283f0);
        /* fall through */
    case 0x283f0:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r && (int8_t)B(0x7a)<=1) {SL(0,0x28118);SW(0x76,rank(a,e,0x2cd0bc));}
        if ((int8_t)B(0x7a)>0) {
            SW(0x76,W(0x76)-1u);
            if (S16(W(0x76))<=0) {
                SB(0x7a,B(0x7a)-1u);
                if ((int8_t)B(0x7a)>0) SL(0,0x2839c);
            }
        }
        return cv_tail650(a,e);
    case 0x28450:
        SW(0x26,0x100);k1b1dc(a);sound(e,0x8000u|0x109d);START(0x235fb6);SL(0,0x2847c);
        /* fall through */
    case 0x2847c:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x2820a);
        cv_da8(a,e);
        return cv_tail5f6(a,e);
    case 0x2849a:
        SW(0x26,0x100);k1b1dc(a);sound(e,0x8000u|0x109d);START(0x235fe0);SL(0,0x284c6);
        /* fall through */
    case 0x284c6:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x2820a);
        cv_da8(a,e);
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        return cv_tail614(a,e);
    /* ---- death ---- */
    case 0x284f2:
        SW(0x26,0x100);k1b1dc(a);child(a,e,0x1c71e,0x1c,-0x18,0);SB(0x60,1);START(0x23606e);SL(0,0x2853e);
        /* fall through */
    case 0x2853e:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r && !B(0x60)) SL(0,0x28572);
        if (ga_cull(a,e,0x290fe)) SL(0,0x28660);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x28572:SW(0x26,0x100);START(0x23611c);goto a2;
    case 0x2858c:SW(0x26,0x100);START(0x236170);
    a2: SB(0x69,0xff);k1b1dc(a);SL(0,0x285b4);                       /* 019DFC */
        /* fall through */
    case 0x285b4:
        ga_stationary(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r && !BIT(0x7d,0)) SL(0,0x285ea);
        if (ga_cull(a,e,0x290fe)) SL(0,0x28660);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x285ea:cv_eaa(a,e);SL(0,0x28660);return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x28660:return GA_DELETE;
    case 0x286f0:return GA_DELETE_TREE;
    /* ---- bomb and shell ---- */
    case 0x286f8: {
        uint32_t d0=(uint32_t)rank(a,e,0x2cd05a)<<8;
        if (!BIT(0x15,0)) d0=0u-d0;
        SL(0x30,d0);SL(0x34,0);
        if (BIT(0x7d,3)) SW(0x2c,W(0x2c)+8u);
        START(0x236244);ga_palette(a,0x3cf);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);SL(0,0x28762);
    }   /* fall through */
    case 0x28762:
        ga_air(a,e);
        if (ga_anim_step(a,e,cv_call)<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,0x290c8)) SL(0,0x28b8c);
        if (BIT(0x15,7)) SL(0,0x28b8c);
        if (ga_cull_rect(a,e,0x290f6)) SL(0,0x286f0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x287ac:
        ga_palette(a,0x208);SB(0x15,B(0x15)^1u);SW(0x66,0x1001);SW(0x1e,0x1001);START(0x236272);SL(0,0x287e2);
        /* fall through */
    case 0x287e2:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x286f0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- rider ---- */
    case 0x287fa: {
        p=sh_creator(a,e);
        int16_t dx=(p && (ga_b(p,0x15)&1u))?4:-4;
        ga_place(a,e,dx,0x28,p?NEO_GA_TAG(p->index):0);                 /* 01AD6E */
        ga_palette(a,0x21a);
        if (!nearest(a,e,&n)) {CLR(0x15,0);if (px(e,n)>=W(0x3c)) SET(0x15,0);}
        uint8_t d0=B(0x7c);
        if (d0==2u) d0=(uint8_t)(ga_rand(e)&1u);
        if (d0) {SB(0x7c,0);SET(0x7d,4);START(0x23638a);SL(0,0x28886);}
        else {SB(0x7c,1);CLR(0x7d,4);START(0x236314);SL(0,0x28930);}
        goto again;
    }
    case 0x28886:
        cv_e2c(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x2889e);
        return cv_taila5c(a,e);
    case 0x2889e:
        CLR(0x7d,4);START(0x2363d8);SL(0,0x288ba);
        /* fall through */
    case 0x288ba:
        cv_e2c(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x288d2);
        return cv_taila5c(a,e);
    case 0x288d2:
        child(a,e,0x28bbc,0x1c,0,0);CLR(0x7d,4);START(0x236356);SL(0,0x28930);
        /* fall through */
    case 0x28930:
        cv_e2c(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x28948);
        return cv_taila5c(a,e);
    case 0x28948: {
        SB(0x15,B(0x15)^1u);SL(0x34,0x44444);SL(0x6e,0xffff6e5eu);
        SL(0x30,BIT(0x15,0)?0x11111u:0xfffeeeefu);
        SW(0x66,0x8820);SW(0x1e,0x8820);
        p=sh_creator(a,e);
        if (p) ga_sb(p,0x60,0);
        /* JMP 0B0626 / 0B0632: the node becomes a soldier (the world moves it).
         * Its block +38 is the car's: the world's soldiers move against the
         * kernel's objects only, so the record is re-based on the scene root
         * first (the car stands still until it is deleted; the original keeps
         * the pointer, to a dead node after 028660). */
        to_root(a,e);
        uint32_t entry=B(0x7c)==1u?0xb0626u:0xb0632u;
        if (!e->become_soldier || !e->become_soldier(e->ctx,a,entry)) {ga_fail(e,entry);return GA_UNSUPPORTED;}
        return GA_MORE;
    }
    case 0x289a6:
        if (BIT(0x15,0)) START(0x23643a); else START(0x2364d4);        /* 028E42 */
        child(a,e,BIT(0x7d,4)?0x28ba8:0x28bbc,0x1c,0,0);
        p=sh_creator(a,e);
        if (p) ga_sb(p,0x60,0);
        SL(0,0x28a32);
        /* fall through */
    case 0x28a32:
        cv_e2c(a,e);SW(0x1e,W(0x1e)+1u);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x28660);
        if (!ga_creator_alive(a,e)) SL(0,0x28660);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- pieces ---- */
    case 0x28a88:
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);ga_palette(a,0x216);ga_palette(a,0x3d2);ga_palette(a,0x367);
        SL(0x30,BIT(0x15,0)?0x10000u:0xffff0000u);SL(0x34,0);SW(0x4e,1);START(0x236758);SL(0,0x28afa);
        /* fall through */
    case 0x28afa: {
        static const uint8_t t[4]={0x19,0x1a,0x19,0x1b};               /* 02910E */
        ga_air(a,e);SL(0x30,L(0x30)+ASR(L(0x30),5));SL(0x34,L(0x34)+L(0x6e));
        SB(0x14,B(t[e->video&3u]));
        if (ga_anim_step(a,e,cv_call)<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,0x290b8)) SL(0,0x28b8c);
        if (BIT(0x15,7)) SL(0,0x28b8c);
        ga_register(a,e,NULL,NULL);SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0x28b8c);
        if (ga_cull_rect(a,e,0x290f6)) SL(0,0x286f0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x28b8c:SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);SL(0,0x1c706);return neo_effects_tick(a,e);   /* 009F88, JMP 01C706 */
    case 0x28ba8:START(0x23660c);goto bcc;
    case 0x28bbc:START(0x2366b2);
    bcc:ga_palette(a,0x21a);SL(0x34,0x99999);SL(0x6e,0xffff0a3eu);SL(0x30,0xcccc);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0,0x28c04);
        /* fall through */
    case 0x28c04:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x6e));
        if (BIT(0x15,7)) SL(0,0x286f0);
        if (ga_anim_step(a,e,cv_call)<0) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,0x290f6)) SL(0,0x286f0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x28c3e:
        ga_palette(a,0x218);SW(0x66,0x1001);SW(0x1e,0x1001);START(0x23657a);
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);goto cd2;
    case 0x28c76:ga_palette(a,0x348);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);START(0x2361de);goto cd2;
    case 0x28ca4:ga_palette(a,0x204);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);START(0x22d372);
    cd2:SL(0,0x28cd8);
        /* fall through */
    case 0x28cd8:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,cv_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x286f0);
        if (ga_cull_rect(a,e,0x290f6)) SL(0,0x286f0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- armed jeep 0A2F2E --------------------------------------------------------- */
/* 0A2F2E..0A41E2 (records 2DAC58 of section 2 at camera 3296 and 2DB29E of
 * section 4; tables 0A41E2..0A4402, ranks 2CD236..2CD452). 0A3DB0 takes the
 * record words: +8E = +74 / +90 = +76 (the item 08CDB2 of 0A3E76, -1 none),
 * +6E != 0 -> +96 bit 6 (it idles instead of driving), +70 != 0 -> bit 3
 * (palette 21C instead of 21B), +95 = +72 (the rider's mode at the death:
 * 0 jumps off as a soldier, 1 another entry, 2 random); +6E..+76 = 0. The
 * entries set +96 bits 0..2 (0A2F2E none; 0A2F48 bits 0 and 2; 0A2F62 bits
 * 0..2; 0A2F7C bit 2): bit 0 the wreck stamp 0C88B0 (0A42C4 / 0A42E6 by bit
 * 1, 0A4154), bit 2 no gunner. Constructor 0A2F92: 0146E8, palette, 01ABC8,
 * flash 306, 8800, +86 = rank 2CD32A (the burst delay), +88 = rank 2CD38C,
 * +8B = 0, +4E = 64, +50 = 0A41E2, solid 0A428E, the turret 0A3A0A (D0 = 0E,
 * +7A; +96 / +95 copied) and the gunner 0A38F4 (D0 = 0E, +76; +96 copied),
 * x > E0: faces left, +82 = E0, else faces right, bit 7, +82 = 60; exhaust
 * 01DC8E (0); bit 6 -> 0A322A else 0A3116. The children's programs come from
 * the tables 0A436A (turret) / 0A4396 (gunner) by index (0A3EF0: 004256 on
 * the child); 0A3F22 sets the turret's state. 0A3116 (drive): sound 109E,
 * +62 = +-30000 by bit 7, +8A = 0, 0A3F6C (facing != bit 7) selects 2A3ACA
 * (gunner 2, turret 1) or 2A39BE (gunner 1, turret 1), +26 by 0A3FE2 from
 * the +8A counter; 0A31D6: 0146A8, a contact, +30 = 0 or the edge 0A3FA0
 * (+82 against x) -> 0A32A8 (bit 7 flipped) / 0A32AE (brake: +6E = -+62 >>
 * 4, 109E stopped; 0A32CE adds +6E to +62 until its high bytes clear: 2A3B80,
 * gunner 3, turret 2; then 0A322A), 0A3FC0 (+86 expired with x <= 120) ->
 * 0A32AE, row 0A425E. 0A322A (idle): +84 = F, 2A392A (gunner 0, turret 0);
 * 0A3274: 0A3F52 (+30/+34/+6E/+72 = 0, 0143E4), without bit 6 +84 counts
 * down to 0A3352, 0A3FC0 -> 0A33C4. 0A3352: bit 6 -> 0A322A, 0A3FC0 ->
 * 0A33C4, else 2A3CBC (gunner 4, turret 3) and 0A33A2 -> 0A3116 at its end,
 * row 0A425E. Burst 0A33C4: +8B != 0 -> 0A344A; +84 = 32, bit 4, 2A3E92
 * (gunner 5, turret 5); 0A341E counts +84 down to 0A343A (+8B = rank
 * 2CD3D6) -> 0A344A: +88 = rank 2CD38C, 2A40FA (gunner 8, turret 6) or with
 * bit 4 2A426A (gunner 9, turret 6); 0A34DE: at its end with +8B <= 0 ->
 * 0A352C; +88 counts down, then +8B, back to 0A344A. 0A352C: +8B = 0, +86 =
 * rank 2CD32A, bit 4 clear -> 0A322A, else 2A4056 (gunner 7, turret 0) and
 * 0A3590 -> 0A322A. 0A35A8 (the gunner dead: gunner D, turret 8, 2A440A, +76
 * = 0), 0A35E8 (hit: gunner A, turret 8, 2A4350), 0A3624 (hit of 01B206:
 * gunner B, turret 8, 2A440A) -> 0A365C: 109E stopped; 0A3672 -> 0A3352 at
 * its end, +15 bit 6 clear -> 0A30B2, 004C24. 0A30B2 (in the air): gunner 0,
 * turret 0, 2A392A; 0A30F6: 0143DC, +15 bit 6 -> 0A322A. Tails 0A37C2 (+15
 * bit 6 clear -> 0A30B2; 004C24 -> 0A35E8 / 0A3624 by 01B206), 0A383C
 * (004C24 only) and 0A37F6 (+5F bit 7 -> 0A36AC / 0A36F4 by 01B206, or
 * 0A378A in the air; 01945C 0A42A4 -> 0A385C deleted). Death 0A36AC (turret
 * state 0A3A54, gunner C, turret A, 2A4578) / 0A36F4 (turret 0A3BA4, gunner
 * D, turret A, 2A45FE) -> 0A3738: +92 = 0, 019DFC, +50 = 0; 0A3752 (+92
 * counts down) ends with +96 bit 0 clear -> 0A37BA (0A407A: 1033, 01C71E, 16
 * x 01D31C) -> 0A385C. 0A378A: turret 0A3BA4, 019DFC, the item 0A3E76 and
 * 0A407A at once. Program calls: 0A3E76 (item), 0A3F32 / 0A3F4A (carry
 * tests), 0A404A (01C71E, 1001), 0A407A, 0A40AE (1064, the piece 0A3BD6 at
 * (-28, 30) and the shell 0A3C24 at (-28, 3E)), 0A4106 (1064, 0A3BC2),
 * 0A4134 (palette 20E), 0A4144 (21A), 0A4154 (8820, the stamp, row 0A427E,
 * solid released), 0A419C (the shell 0A3C58 preset with D0: +8D = D0 << 3,
 * +84 = 258, +62 = 400, +8C = 5, +86 = 1E).
 * - 0A38F4 (gunner): palette by bit 3, 01AD6E (0, 0), flash 306, +4E = rank
 *   2CD236, 2A3958; 0A3932: 0A3EC0 (01ABC8, the creator's facing and +1E),
 *   +26 = the creator's, 00427A, +93 = 1: rows 0A421E / 0A422E, 2: 0A423E /
 *   0A424E (unless +5F bit 7), 004C24 clears +5F bit 7; +4E <= 0 (unless +60
 *   is 33 or 77): +60 = 33, sound 102D, the creator -> 0A35A8; +60 == 77 ->
 *   0A39FE (the creator's +76 = 0, deleted); 00409A -> 0A38EC.
 * - 0A3A0A (turret): +94 = FF, palette 21D, 2A3990, 01AD6E; 0A3A3E: 0A3E28
 *   (+28 = +-10, +2C = 18 on the creator, 01ABC8, +1E = the creator's - 1),
 *   +26 = the creator's, 00427A (no tail). 0A3A54 (the rider at the death):
 *   +95 (2: random bit) -> 2A473A / 2A478E; 0A3AA0 ends -> 0A3B50 (8820, y +
 *   8; +95 = 0: flipped, +34 = 22222, +72 = FFFFB72F, +30 = FFFEEEEF, 0405EA,
 *   JMP 0B0626; else 0405EA, y + 4, JMP 0AF14C: not a soldier entry of this
 *   port, an explicit stop); 004C24, +5F bit 7 -> 0A3ACC (2A45AC; 0A3AE2
 *   until the creator's +92 is 0 -> 0A3B02: 0405EA, 2404FE, 0A3B24 ends or
 *   019504 0A42BC -> 0A38EC). 0A3BA4: E0ED, 040846 (04097A, then 040C8E: not
 *   ported, an explicit stop).
 * - 0A3BC2 / 0A3BD6 (pieces 2A47C4 / 2A4866 flipped): palette 208, 1001;
 *   0A3C0C ends -> 0A38EC.
 * - 0A3C24 (shell): +8D = C, +62 = rank 2CD438, +8C = 0A4308[+62 >> 8], +86
 *   = 0A4318[+62 >> 8], +84 = 5A, palette 223, +4E = 5, E0EF, +15 bit 5,
 *   004C1C, 2A48FE, 0A4006 (+30/+34 = cos/sin(+8D << 3) * +62); 0A3C9E: +86
 *   expired -> 0A3D26 (homing: +86 = 258, +7E = 01B04A; 0A3D3C: every +8C
 *   updates +8D steps toward the angle 01B12E >> 3 and 0A4006), +84 expired
 *   -> 0A3CB2 (no more homing), 0143E4, +15 bit 7 / row 0A426E / +5F bit 7
 *   -> 0A3D86 (009F88, 1001, sound 1022, +50 = 0, JMP 01C706), 019504 0A42B4
 *   -> 0A38EC.
 * The cartridge checks before 004188 / 0041AC are not reproduced. Proven by
 * the family m3jeep (capture A of captures-m3s2c: the idle jeep at 3296, its
 * bursts, shells, the gunner's death, the jeep's death and its rider); the
 * drive and the entries 0A2F48.. are literal. */
static bool jp(uint32_t r) {return r>=0xa2f2e && r<0xa3db0;}
static uint8_t rb(neo_genv_t *e,uint32_t at) {const uint8_t *t=ga_rom(at);if (!t) {ga_fail(e,at);return 0;}return t[0];}
/* 0A3DB0 */
static void jp_db0(neo_gactor_t *a) {
    SB(0x96,0);SW(0x8e,W(0x74));SW(0x90,W(0x76));
    if (W(0x6e)) SET(0x96,6);
    if (W(0x70)) SET(0x96,3);
    SB(0x95,W(0x72));
    SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);SW(0x76,0);
}
/* 0A3EF0: the program `table`[d0 & F] on the child +o (nothing without one). */
static bool jp_prog(neo_gactor_t *a,neo_genv_t *e,unsigned o,uint32_t table,unsigned d0) {
    if (!L(o)) return true;
    neo_gactor_t *c=e->actor?e->actor(e->ctx,L(o)):NULL;
    if (!c) return true;                                             /* a deleted child: the write goes nowhere */
    uint32_t t=rl(e,table+4u*(d0&0xfu));
    if (!t) return true;
    return ga_anim_start(c,e,t,0x100);
}
#define JPROG(g,t) do { if (!jp_prog(a,e,0x76,0xa4396,(g)) || !jp_prog(a,e,0x7a,0xa436a,(t))) return GA_UNSUPPORTED; } while (0)
/* 0A3F22: the turret's state. */
static void jp_turret_state(neo_gactor_t *a,neo_genv_t *e,uint32_t state) {
    if (!L(0x7a)) return;
    neo_gactor_t *c=e->actor?e->actor(e->ctx,L(0x7a)):NULL;
    if (c) ga_sl(c,0,state);
}
/* 0A3F52 */
static void jp_f52(neo_gactor_t *a,neo_genv_t *e) {SL(0x30,0);SL(0x34,0);SL(0x6e,0);SL(0x72,0);ga_air(a,e);}
/* 0A3F6C: carry when the facing differs from +96 bit 7. */
static bool jp_f6c(neo_gactor_t *a) {return BIT(0x15,0)!=BIT(0x96,7);}
/* 0A3FA0: the edge +82 passed (unsigned x). */
static bool jp_fa0(neo_gactor_t *a) {
    uint16_t x=W(0x3c),d0=W(0x82);
    return BIT(0x96,7)?d0<x:x<d0;
}
/* 0A3FC0: carry when +86 has expired with x <= 120. */
static bool jp_fc0(neo_gactor_t *a) {
    SW(0x86,W(0x86)-1u);
    if (S16(W(0x86))>0) return false;
    SW(0x86,0);
    return S16(W(0x3c))<=0x120;
}
/* 0A3FE2: +26 from the +8A counter. */
static void jp_fe2(neo_gactor_t *a) {
    uint8_t d0=B(0x8a);SB(0x8a,d0+1u);d0>>=4;
    if ((int8_t)d0>3) d0=3;
    SW(0x26,(uint16_t)(((d0&3u)+1u)<<7));
}
/* 0A4006: +30/+34 from the angle +8D (steps of 8) and the speed +62. */
static void jp_006(neo_gactor_t *a) {
    uint8_t angle=(uint8_t)((B(0x8d)&0x1fu)<<3);
    int32_t s=S16(W(0x62));
    SL(0x30,(uint32_t)((int32_t)ga_cosine(angle)*s));SL(0x34,(uint32_t)((int32_t)ga_sine(angle)*s));
}
/* 0A407A: sound 1033, 01C71E and 01DBAA. */
static void jp_07a(neo_gactor_t *a,neo_genv_t *e) {
    sound(e,0x1033);child(a,e,0x1c71e,0x1c,0,0);
    for (unsigned i=0;i<16;i++) child(a,e,0x1d31c,0x2a,0,0x18);
}
/* 0A3E76 -> 08CDB2: the item +8E / +90 at (+-10, 20) after the script actor. */
static void jp_e76(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t kind=W(0x8e);
    if (kind==0xffffu || kind>=0x5au) return;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (!c) return;
    ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x90));ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
    ga_place_at(c,a,e,BIT(0x15,0)?-0x10:0x10,0x20);
}
/* 0A3E28: on the creator at (+-10, 18), one depth above it (+94 = FF). */
static void jp_e28(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=sh_creator(a,e);
    if (!p) {ga_fail(e,0xa3e28);return;}
    if (ga_b(p,0x15)&1u) {SET(0x15,0);SW(0x28,0xfff0);} else {CLR(0x15,0);SW(0x28,0x10);}
    SW(0x2c,0x18);ga_project(a,e);
    SW(0x1e,ga_w(p,0x1e)-(B(0x94)==0xffu?1u:0u));
}
/* 0A3EC0: 01ABC8, the creator's facing and depth. */
static void jp_ec0(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    neo_gactor_t *p=sh_creator(a,e);
    if (!p) {ga_fail(e,0xa3ec0);return;}
    if (ga_b(p,0x15)&1u) SET(0x15,0); else CLR(0x15,0);
    SW(0x1e,ga_w(p,0x1e));
}
/* 0405EA: the soldier death palette (009F88, 01B2BE). */
static void k405ea(neo_gactor_t *a,neo_genv_t *e) {SB(0x1d,0);ga_palette(a,neo_blood_palette(NEO_BLOOD_SOLDIER_DEATH,e->blood));}
static bool jp_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0xa3e76:jp_e76(a,e);return true;
    case 0xa3f32:case 0xa3f4a:return true;                          /* carry tests: no effect on the node */
    case 0xa404a: {neo_gactor_t *c=child(a,e,0x1c71e,0x1c,0,0);if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}return true;}
    case 0xa407a:jp_07a(a,e);return true;
    case 0xa40ae:sound(e,0x1064);child(a,e,0xa3bd6,0x1c,-0x28,0x30);child(a,e,0xa3c24,0x1c,-0x28,0x3e);return true;
    case 0xa4106:sound(e,0x1064);child(a,e,0xa3bc2,0x1c,0,0);return true;
    case 0xa4134:ga_palette(a,0x20e);return true;
    case 0xa4144:ga_palette(a,0x21a);return true;
    case 0xa4154:
        SW(0x66,0x8820);SW(0x1e,0x8820);
        if (BIT(0x96,0)) {
            if (e->stamp) e->stamp(e->ctx,a,BIT(0x96,1)?0xa42e6u:0xa42c4u);   /* 0C88B0 */
            ga_attack(a,e,0xa427e);
        }
        solid(a,e,0);
        return true;
    case 0xa419c: {
        neo_gactor_t *c=child(a,e,0xa3c58,0x1c,0,0);
        if (!c) return true;
        ga_sb(c,0x8d,(uint8_t)(parameter<<3));ga_sw(c,0x84,0x258);ga_sw(c,0x62,0x400);ga_sb(c,0x8c,5);ga_sw(c,0x86,0x1e);
        return true;
    }
    }
    return false;
}
/* 0A37F6 */
static ga_result_t jp_tail7f6(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x5f,7)) {
        SL(0,0xa36ac);
        if (flinch(a,e)) SL(0,0xa36f4);
        if (!BIT(0x15,6)) SL(0,0xa378a);
    }
    if (ga_cull(a,e,0xa42a4)) SL(0,0xa385c);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 0A37C2 */
static ga_result_t jp_tail7c2(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) SL(0,0xa30b2);
    if (ga_register(a,e,NULL,NULL)) {SL(0,0xa35e8);if (flinch(a,e)) SL(0,0xa3624);SB(0x5a,0);}
    return jp_tail7f6(a,e);
}
/* 0A383C */
static ga_result_t jp_tail83c(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) SL(0,0xa30b2);
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    return jp_tail7f6(a,e);
}
static ga_result_t jeep_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p,*c;
again:
    switch (L(0)) {
    case 0xa2f2e:jp_db0(a);CLR(0x96,0);CLR(0x96,1);CLR(0x96,2);goto f92;
    case 0xa2f48:jp_db0(a);SET(0x96,0);CLR(0x96,1);SET(0x96,2);goto f92;
    case 0xa2f62:jp_db0(a);SET(0x96,0);SET(0x96,1);SET(0x96,2);goto f92;
    case 0xa2f7c:jp_db0(a);CLR(0x96,0);CLR(0x96,1);SET(0x96,2);
    f92:
        ga_floor(a,e);k1b1dc(a);ga_palette(a,BIT(0x96,3)?0x21c:0x21b);ga_project(a,e);ga_flash_palette(a,0x306);
        SW(0x66,0x8800);SW(0x1e,0x8800);SW(0x86,rank(a,e,0x2cd32a));SW(0x88,rank(a,e,0x2cd38c));SB(0x8b,0);
        SW(0x4e,0x64);SL(0x50,0xa41e2);solid(a,e,0xa428e);
        c=child(a,e,0xa3a0a,0x0e,0,0);
        if (c) {SL(0x7a,NEO_GA_TAG(c->index));ga_sb(c,0x96,B(0x96));ga_sb(c,0x95,B(0x95));}
        if (!BIT(0x96,2)) {
            c=child(a,e,0xa38f4,0x0e,0,0);
            if (c) {SL(0x76,NEO_GA_TAG(c->index));ga_sb(c,0x96,B(0x96));}
        }
        if (S16(W(0x3c))>0xe0) {CLR(0x15,0);CLR(0x96,7);SW(0x82,0xe0);} else {SET(0x15,0);SET(0x96,7);SW(0x82,0x60);}
        exhaust8e(a,e,0);
        if (BIT(0x96,6)) {SL(0,0xa322a);goto again;}
        SL(0,0xa3116);goto again;
    case 0xa30b2:
        k1b1dc(a);JPROG(0,0);START(0x2a392a);SL(0,0xa30f6);
        /* fall through */
    case 0xa30f6:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                        /* 0143DC */
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        if (BIT(0x15,6)) SL(0,0xa322a);
        if (ga_register(a,e,NULL,NULL)) {SL(0,0xa35e8);if (flinch(a,e)) SL(0,0xa3624);SB(0x5a,0);}   /* 0A37D2 */
        return jp_tail7f6(a,e);
    case 0xa3116:
        k1b1dc(a);
        if (BIT(0x96,6)) {SL(0,0xa322a);goto again;}
        sound(e,0x109e);k1b1dc(a);SL(0x62,BIT(0x96,7)?0x30000u:0xfffd0000u);SB(0x8a,0);
        if (jp_f6c(a)) {JPROG(2,1);START(0x2a3aca);} else {JPROG(1,1);START(0x2a39be);}
        jp_fe2(a);SL(0,0xa31d6);
        /* fall through */
    case 0xa31d6:
        k146a8(a,e);
        if (BIT(0x5f,0)) SL(0,0xa32ae);
        if (!L(0x30)) SL(0,0xa32ae);
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        if (jp_fa0(a)) SL(0,0xa32a8);
        if (jp_fc0(a)) SL(0,0xa32ae);
        ga_attack(a,e,0xa425e);
        return jp_tail7c2(a,e);
    case 0xa322a:
        k1b1dc(a);SW(0x84,0xf);JPROG(0,0);START(0x2a392a);SL(0,0xa3274);
        /* fall through */
    case 0xa3274:
        jp_f52(a,e);
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        if (!BIT(0x96,6)) {SW(0x84,W(0x84)-1u);if (!W(0x84)) SL(0,0xa3352);}
        if (jp_fc0(a)) SL(0,0xa33c4);
        return jp_tail7c2(a,e);
    case 0xa32a8:
        SB(0x96,B(0x96)^0x80u);
        /* fall through */
    case 0xa32ae:
        SB(0x8a,0);SL(0x6e,0u-ASR(L(0x62),4));sound(e,0x8000u|0x109e);SL(0,0xa32ce);   /* 012192 */
        /* fall through */
    case 0xa32ce:
        k146a8(a,e);SL(0x62,L(0x62)+L(0x6e));
        if (L(0x6e) && !(L(0x62)&0xffffff00u)) {
            SL(0x6e,0);SL(0x62,0);JPROG(3,2);START(0x2a3b80);
        }
        jp_fe2(a);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa322a);
        jp_fc0(a);
        return jp_tail7c2(a,e);
    case 0xa3352:
        if (BIT(0x96,6)) {SL(0,0xa322a);goto again;}
        if (jp_fc0(a)) {SL(0,0xa33c4);goto again;}
        JPROG(4,3);START(0x2a3cbc);SL(0,0xa33a2);
        /* fall through */
    case 0xa33a2:
        jp_f52(a,e);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa3116);
        ga_attack(a,e,0xa425e);
        return jp_tail7c2(a,e);
    /* ---- bursts ---- */
    case 0xa33c4:
        k1b1dc(a);
        if (B(0x8b)) {SL(0,0xa344a);goto again;}
        SW(0x84,0x32);SET(0x96,4);JPROG(5,5);START(0x2a3e92);SL(0,0xa341e);
        /* fall through */
    case 0xa341e:
        jp_f52(a,e);
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        SW(0x84,W(0x84)-1u);
        if (!W(0x84)) SL(0,0xa343a);
        return jp_tail83c(a,e);
    case 0xa343a:
        SB(0x8b,rank(a,e,0x2cd3d6));
        /* fall through */
    case 0xa344a:
        SW(0x88,rank(a,e,0x2cd38c));
        if (!BIT(0x96,4)) {JPROG(8,6);START(0x2a40fa);} else {JPROG(9,6);START(0x2a426a);}
        SL(0,0xa34de);
        /* fall through */
    case 0xa34de:
        jp_f52(a,e);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r && (int8_t)B(0x8b)<=0) SL(0,0xa352c);
        if ((int8_t)B(0x8b)>0) {
            SW(0x88,W(0x88)-1u);
            if (S16(W(0x88))<=0) {
                SB(0x8b,B(0x8b)-1u);
                if ((int8_t)B(0x8b)>0) SL(0,0xa344a);
            }
        }
        return jp_tail83c(a,e);
    case 0xa352c:
        SB(0x8b,0);SW(0x86,rank(a,e,0x2cd32a));
        if (!BIT(0x96,4)) {SL(0,0xa322a);goto again;}
        CLR(0x96,4);JPROG(7,0);START(0x2a4056);SL(0,0xa3590);
        /* fall through */
    case 0xa3590:
        jp_f52(a,e);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa322a);
        return jp_tail7c2(a,e);
    /* ---- the gunner dead, hits ---- */
    case 0xa35a8:JPROG(0xd,8);START(0x2a440a);SL(0x76,0);goto c65c;
    case 0xa35e8:JPROG(0xa,8);START(0x2a4350);goto c65c;
    case 0xa3624:JPROG(0xb,8);START(0x2a440a);
    c65c:
        k1b1dc(a);sound(e,0x8000u|0x109e);SL(0,0xa3672);
        /* fall through */
    case 0xa3672:
        jp_f52(a,e);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa3352);
        jp_fc0(a);
        if (!BIT(0x15,6)) SL(0,0xa30b2);
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        return jp_tail7f6(a,e);
    /* ---- death ---- */
    case 0xa36ac:jp_turret_state(a,e,0xa3a54);JPROG(0xc,0xa);START(0x2a4578);goto c738;
    case 0xa36f4:jp_turret_state(a,e,0xa3ba4);JPROG(0xd,0xa);START(0x2a45fe);
    c738:
        SB(0x92,0);k1b1dc(a);SB(0x69,0xff);SL(0x50,0);SL(0,0xa3752);          /* 019DFC */
        /* fall through */
    case 0xa3752:
        jp_f52(a,e);
        if ((int8_t)B(0x92)>0) SB(0x92,B(0x92)-1u);                          /* 0A4036 */
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r && !BIT(0x96,0)) SL(0,0xa37ba);
        if (ga_cull(a,e,0xa42a4)) SL(0,0xa385c);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa378a:
        jp_turret_state(a,e,0xa3ba4);SB(0x92,0);SL(0x50,0);k1b1dc(a);SB(0x69,0xff);
        jp_e76(a,e);jp_07a(a,e);SL(0,0xa385c);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa37ba:jp_07a(a,e);return e->unsupported?GA_UNSUPPORTED:GA_DELETE;   /* bra 0A385C */
    case 0xa385c:return GA_DELETE;
    case 0xa38ec:return GA_DELETE_TREE;
    /* ---- gunner ---- */
    case 0xa38f4:
        ga_palette(a,BIT(0x96,3)?0x21c:0x21b);
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);
        ga_flash_palette(a,0x306);SW(0x4e,rank(a,e,0x2cd236));START(0x2a3958);SL(0,0xa3932);
        /* fall through */
    case 0xa3932:
        jp_ec0(a,e);
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0xa3936);return GA_UNSUPPORTED;}
        SW(0x26,ga_w(p,0x26));
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        if (!BIT(0x5f,7)) {
            if (B(0x93)==1u) {ga_attack(a,e,0xa421e);ga_attack(a,e,0xa422e);}
            if (B(0x93)==2u) {ga_attack(a,e,0xa423e);ga_attack(a,e,0xa424e);}
        }
        if (ga_register(a,e,NULL,NULL)) {CLR(0x5f,7);SB(0x5a,0);}
        if (B(0x60)!=0x33u && B(0x60)!=0x77u && S16(W(0x4e))<=0) {
            SB(0x60,0x33);sound(e,0x102d);
            p=sh_creator(a,e);
            if (p) ga_sl(p,0,0xa35a8);
        }
        if (B(0x60)==0x77u) SL(0,0xa39fe);
        if (!ga_creator_alive(a,e)) SL(0,0xa38ec);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa39fe:
        p=sh_creator(a,e);
        if (p) ga_sl(p,0x76,0);
        return GA_DELETE_TREE;
    /* ---- turret and rider ---- */
    case 0xa3a0a:
        SB(0x94,0xff);ga_palette(a,0x21d);START(0x2a3990);
        p=sh_creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);SL(0,0xa3a3e);
        /* fall through */
    case 0xa3a3e:
        jp_e28(a,e);
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0xa3a42);return GA_UNSUPPORTED;}
        SW(0x26,ga_w(p,0x26));
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa3a54: {
        uint8_t d0=B(0x95);
        if ((int8_t)d0>=2) d0=(uint8_t)ga_rand(e);
        d0&=1u;SB(0x95,d0);
        if (!d0) START(0x2a473a); else START(0x2a478e);
        SL(0,0xa3aa0);
    }   /* fall through */
    case 0xa3aa0:
        jp_e28(a,e);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa3b50);
        ga_register(a,e,NULL,NULL);
        if (BIT(0x5f,7)) SL(0,0xa3acc);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa3acc:START(0x2a45ac);SL(0,0xa3ae2);
        /* fall through */
    case 0xa3ae2:
        jp_e28(a,e);
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0xa3ae6);return GA_UNSUPPORTED;}
        if (!ga_b(p,0x92)) SL(0,0xa3b02);
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa3b02:k405ea(a,e);k1b1dc(a);START(0x2404fe);SL(0,0xa3b24);
        /* fall through */
    case 0xa3b24:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa38ec);
        if (ga_cull_rect(a,e,0xa42bc)) SL(0,0xa38ec);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa3b50:
        SW(0x66,0x8820);SW(0x1e,0x8820);SW(0x2c,W(0x2c)+8u);
        if (!B(0x95)) {
            SB(0x15,B(0x15)^1u);SL(0x34,0x22222);SL(0x72,0xffffb72fu);SL(0x30,0xfffeeeefu);k405ea(a,e);
            /* JMP 0B0626: the node becomes a soldier (the world moves it); its
             * block +38 (the jeep's) is re-based on the scene root first, as the
             * car's rider (028948). */
            to_root(a,e);
            if (!e->become_soldier || !e->become_soldier(e->ctx,a,0xb0626)) {ga_fail(e,0xb0626);return GA_UNSUPPORTED;}
            return GA_MORE;
        }
        k405ea(a,e);SW(0x2c,W(0x2c)+4u);to_root(a,e);
        /* JMP 0AF14C: a soldier of 0AEF30 with +81 = E2 (neo_soldier_become). */
        if (!e->become_soldier || !e->become_soldier(e->ctx,a,0xaf14c)) {ga_fail(e,0xaf14c);return GA_UNSUPPORTED;}
        return GA_MORE;
    case 0xa3ba4:
        jp_e28(a,e);
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        k4097a(a,e);SL(0,0x40c8e);return e->unsupported?GA_UNSUPPORTED:GA_MORE;   /* 040846: 04097A, then 040C8E */
    /* ---- pieces ---- */
    case 0xa3bc2:START(0x2a47c4);goto bec;
    case 0xa3bd6:SB(0x15,B(0x15)^1u);START(0x2a4866);
    bec:ga_palette(a,0x208);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0xa3c0c);
        /* fall through */
    case 0xa3c0c:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,jp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa38ec);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- shell ---- */
    case 0xa3c24: {
        SB(0x8d,0xc);
        uint16_t d0=rank(a,e,0x2cd438);SW(0x62,d0);d0>>=8;
        SB(0x8c,rb(e,0xa4308u+d0));SW(0x86,rw(e,0xa4318u+2u*d0));
    }   /* fall through */
    case 0xa3c58:
        SW(0x84,0x5a);ga_palette(a,0x223);SW(0x4e,5);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        SB(0x5e,B(0x5e)|0x80u);START(0x2a48fe);jp_006(a);SL(0,0xa3c9e);         /* 004C1C */
        /* fall through */
    case 0xa3c9e:
        SW(0x86,W(0x86)-1u);
        if (S16(W(0x86))<=0) SL(0,0xa3d26);
        /* fall through */
    case 0xa3cb2:
    cb2:
        SW(0x84,W(0x84)-1u);
        if (S16(W(0x84))<=0) SL(0,0xa3cb2);
        ga_air(a,e);
        if (BIT(0x15,7)) SL(0,0xa3d86);
        if (ga_anim_step(a,e,jp_call)<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,0xa426e)) SL(0,0xa3d86);
        ga_register(a,e,NULL,NULL);
        if (BIT(0x5f,7)) SL(0,0xa3d86);
        if (ga_cull_rect(a,e,0xa42b4)) SL(0,0xa38ec);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xa3d26:
        SW(0x86,0x258);SL(0x7e,k1b04a(e));SL(0,0xa3d3c);
        /* fall through */
    case 0xa3d3c:
        SW(0x86,W(0x86)+1u);
        if ((int8_t)(uint8_t)W(0x86)>(int8_t)B(0x8c)) {
            SW(0x86,0);
            uint8_t d0=(uint8_t)((uint8_t)(k1b12e(a,e,L(0x7e))+8u)>>3);
            if (d0!=B(0x8d)) {
                SB(0x8d,(uint8_t)(B(0x8d)+(d0<B(0x8d)?0xffu:1u)));
                SB(0x8d,B(0x8d)&0x1fu);
            } else SB(0x8d,B(0x8d)&0x1fu);
            jp_006(a);
        }
        goto cb2;
    case 0xa3d86:SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);sound(e,0x1022);SL(0x50,0);SL(0,0x1c706);return neo_effects_tick(a,e);   /* 009F88, JMP 01C706 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- birds 03ED00..03F1D4 ------------------------------------------------------- */
/* Decorative birds of Mission 3 (records 2DAC.. of section 2 with 03EE8A;
 * tables 0E08B8 (cull) and 0E08C0 (random states)). Record words: +6E bit 0
 * the facing (03FAF2), +70 the speed (03FB10: +30 = +70 << 8 toward the
 * facing; 03FB2C: +76 = the camera progress 10E99E + +70 << 8, 03FB40: carry
 * once the progress reaches it), +72 the palette variant of other entries.
 * 03EE8A: no layer word, palette 2F8, 03EEB4 (23FEA0, 01ABC8, 01AD8C,
 * 00427A; at its end with +70 = 0 a player within 60 px (03FC26: x - P1/P2
 * +3C < 60) takes off (03EFC0), else 03F138 picks 03EEB4 / 03EF24 (23FEAC)
 * / 03EF5A (hop: +6E = random bit, +30 = +-8000, 23FE6C, 03FA46 moves +28/+2C
 * by +30/+34) from 0E08C0; with +70 != 0 the progress 03FB40 decides).
 * 03EFC0: y + 4, +74 = 1E + rand(10); 03EFF8: +88 = rand(20) + 10 (the
 * angle); 03F028: +80 = rand(100) + 300 (the speed), 002E7C -> +30/+34, faces
 * right, 23FECC; 03F084: 03FA46 until +74 expires -> 8838, +30 = +80 << 8,
 * +34 = 0, 03ED8E (flapping: +86 = E, +74 = 8 + rand(8), 23FECC; 03EDBE:
 * 03FA74 adds +84/+86 << 8 to +30/+34; y >= 1D0 or +74 expired -> 03EE00:
 * +86 = -E, 23FEFA, 03EE30 until y < 160 or +74 expired -> 03ED8E). 03FADA:
 * 01945C 0E08B8 -> 0041AC. Other entries: 03ED00 (palette 328, 23FE3E,
 * 03ED3A: 03FABA moves +28 by +30), 03ED56 (8838, 2F8, 23FECC -> 03ED8E),
 * 03EE72 (01AB98 at (40, 150) then 03EE8A), 03F0D0 (2F8, 23FEA0; 03F10E:
 * 03FC26 -> 03EFC0), 03F152 (01AB98 (40, 150), +70 = 1000, +6E = 1, 2F9,
 * 23FF04; 03F1BE: 03FABA). The cartridge checks are not reproduced. Proven
 * by the family m3bird (capture A of captures-m3s2c: two 03EE8A at 3296). */
static bool bd(uint32_t r) {return r>=0x3ed00 && r<0x3f1da;}
static void bd_af2(neo_gactor_t *a) {if (W(0x6e)&1u) SET(0x15,0); else CLR(0x15,0);}   /* 03FAF2 */
static void bd_b10(neo_gactor_t *a) {uint32_t d0=(uint32_t)(int32_t)S16(W(0x70))<<8;SL(0x30,BIT(0x15,0)?d0:0u-d0);}   /* 03FB10 */
static void bd_b2c(neo_gactor_t *a,neo_genv_t *e) {SL(0x76,((uint32_t)e->camera<<16|e->camera_low)+((uint32_t)(int32_t)S16(W(0x70))<<8));}   /* 03FB2C */
static bool bd_b40(neo_gactor_t *a,neo_genv_t *e) {return W(0x70) && ((uint32_t)e->camera<<16|e->camera_low)>=L(0x76);}   /* 03FB40 */
static void bd_a46(neo_gactor_t *a) {SL(0x28,L(0x28)+L(0x30));SL(0x2c,L(0x2c)+L(0x34));}   /* 03FA46 */
static void bd_a74(neo_gactor_t *a) {bd_a46(a);SL(0x30,L(0x30)+((uint32_t)(int32_t)S16(W(0x84))<<8));SL(0x34,L(0x34)+((uint32_t)(int32_t)S16(W(0x86))<<8));}   /* 03FA74 */
/* 03FC26: carry when a player's x is less than d0 behind (x - +3C of P1 / P2). */
static bool bd_c26(neo_gactor_t *a,neo_genv_t *e,int16_t d0) {
    return S16((uint16_t)(W(0x3c)-e->players.p1x))<d0 || S16((uint16_t)(W(0x3c)-e->players.p2x))<d0;
}
/* 03FADA: 01945C 0E08B8 -> 0041AC. */
static ga_result_t bd_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0xe08b8)) return GA_DELETE_TREE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t bird_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x3ed00:
        SW(0x66,0);SW(0x1e,0);bd_af2(a);bd_b10(a);ga_palette(a,0x328);START(0x23fe3e);SL(0,0x3ed3a);return GA_MORE;
    case 0x3ed3a:
        SL(0x28,L(0x28)+L(0x30));                                        /* 03FABA */
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return bd_tail(a,e);
    case 0x3ed56:
        SW(0x66,0x8838);SW(0x1e,0x8838);bd_af2(a);bd_b10(a);ga_palette(a,0x2f8);START(0x23fecc);SL(0,0x3ed8e);
        /* fall through */
    case 0x3ed8e:
        SW(0x86,0xe);SW(0x74,8u+ga_rand_range(e,8));START(0x23fecc);SL(0,0x3edbe);
        /* fall through */
    case 0x3edbe:
        ga_project(a,e);depth8000(a);bd_a74(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (S16(W(0x40))>=0x1d0) {SL(0,0x3ee00);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
        SW(0x74,W(0x74)-1u);
        if (!(W(0x74)>0u)) SL(0,0x3ee00);
        return bd_tail(a,e);
    case 0x3ee00:
        SW(0x86,0xfff2);SW(0x74,8u+ga_rand_range(e,8));START(0x23fefa);SL(0,0x3ee30);
        /* fall through */
    case 0x3ee30:
        ga_project(a,e);depth8000(a);bd_a74(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (S16(W(0x40))<0x160) {SL(0,0x3ed8e);return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
        SW(0x74,W(0x74)-1u);
        if (!(W(0x74)>0u)) SL(0,0x3ed8e);
        return bd_tail(a,e);
    case 0x3ee72:ga_place(a,e,0x40,0x150,0);                            /* 01AB98 */
        /* fall through */
    case 0x3ee8a:
        SW(0x66,0);SW(0x1e,0);bd_af2(a);bd_b2c(a,e);ga_palette(a,0x2f8);SL(0,0x3eeb4);return GA_MORE;
    case 0x3eeb4:START(0x23fea0);SL(0,0x3eeca);
        /* fall through */
    case 0x3eeca:
        ga_project(a,e);depth8000(a);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) {
            if (!W(0x70) ? bd_c26(a,e,0x60) : bd_b40(a,e)) SL(0,0x3efc0);
            else SL(0,rl(e,0xe08c0u+4u*(ga_rand(e)&3u)));                /* 03F138 */
        }
        return bd_tail(a,e);
    case 0x3ef24:START(0x23feac);SL(0,0x3ef3a);
        /* fall through */
    case 0x3ef3a:
        ga_project(a,e);depth8000(a);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,rl(e,0xe08c0u+4u*(ga_rand(e)&3u)));                   /* 03F138 */
        return bd_tail(a,e);
    case 0x3ef5a:
        SW(0x6e,ga_rand_range(e,2));SL(0x30,W(0x6e)?0x8000u:0xffff8000u);bd_af2(a);START(0x23fe6c);SL(0,0x3ef9a);
        /* fall through */
    case 0x3ef9a:
        ga_project(a,e);depth8000(a);bd_a46(a);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x3eeb4);
        return bd_tail(a,e);
    case 0x3efc0:
        SW(0x2c,W(0x2c)+4u);SW(0x74,0x1eu+ga_rand_range(e,0x10));ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SL(0,0x3eff8);return bd_tail(a,e);
    case 0x3eff8:
        SB(0x88,ga_rand_range(e,0x20)+0x10u);ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SL(0,0x3f028);return bd_tail(a,e);
    case 0x3f028: {
        SW(0x80,ga_rand_range(e,0x100)+0x300u);
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)S16(W(0x80))<<8,B(0x88),&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SET(0x15,0);START(0x23fecc);ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SL(0,0x3f084);return bd_tail(a,e);
    }
    case 0x3f084:
        ga_project(a,e);depth8000(a);bd_a46(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SW(0x74,W(0x74)-1u);
        if (!(W(0x74)>0u)) {SW(0x66,0x8838);SW(0x1e,0x8838);SL(0x30,(uint32_t)(int32_t)S16(W(0x80))<<8);SL(0x34,0);SL(0,0x3ed8e);}
        return bd_tail(a,e);
    case 0x3f0d0:
        SW(0x66,0);SW(0x1e,0);bd_af2(a);bd_b10(a);ga_palette(a,0x2f8);SL(0,0x3f0f8);return GA_MORE;
    case 0x3f0f8:START(0x23fea0);SL(0,0x3f10e);
        /* fall through */
    case 0x3f10e:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (bd_c26(a,e,0x60)) SL(0,0x3efc0);
        return bd_tail(a,e);
    case 0x3f152:
        ga_place(a,e,0x40,0x150,0);SW(0x70,0x1000);SW(0x6e,1);
        SW(0x66,0);SW(0x1e,0);bd_af2(a);bd_b10(a);ga_palette(a,0x2f9);START(0x23ff04);SL(0,0x3f1be);
        /* fall through */
    case 0x3f1be:
        ga_project(a,e);depth8000(a);SL(0x28,L(0x28)+L(0x30));          /* 03FABA */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return bd_tail(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- sandbags 0214EA / 0216BA ----------------------------------------------------- */
/* 0214EA..021A2A (record of section 2 at camera 1264 with +70 / +72 the
 * item 08CDB2 dropped by 0219CC at (0, 0), +6E the facing; tables 021A02
 * (cull) and 021A0A (the broken hurtbox)). 0214EA: 009F88, palette 219,
 * 8820, +4E = 1, 230238, 0146E8; 02153C: 01ABC8, 01AD8C, 00427A, 004C24: a
 * hit with +5F bit 7 throws a piece 0218C4 (D0 = 2A; +5B bit 7 flips it) and
 * -> 0215A8 (23035E, sound 103F; 0215C8 ends -> 0215EA: 004C1C, +74 = 6,
 * +50 = 021A0A, 2303E4 / 021612: 2303CA, +5F bit 7 clear, +4E = 1; 021634:
 * each hit with +5F bit 7 while +74 > 0 counts it down with a piece, back
 * to 021612; at 0 the item 0219CC: its +12/+13 = 20 and +34 = 20000). 0216BA
 * (palettes 3D5 and 219, 2303F0; 02171A -> 021790: without blood (01B2BE)
 * +14 = +19, +50 = 0, 230438, 103F; 0217EA: 23052C / 021812: 230512; 021834
 * as 021634 with the piece's facing by +5B bit 7). 0218C4 (piece): E0ED,
 * 230538, palette 37A (blood) / 3BA; 021910 ends -> 004188. 0219B0: 01945C
 * 021A02 -> +60 = FF, 004188. The cartridge check before 004188 is not
 * reproduced. Proven by the family m3sack (capture A of captures-m3s2c: the
 * 0214EA at 1264, seven pieces, the item). */
static bool sk(uint32_t r) {return r>=0x214ea && r<0x219b0;}
/* 0219B0 */
static ga_result_t sk_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0x21a02)) {SB(0x60,0xff);return GA_DELETE;}
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* The piece 0218C4 (D0 = 2A) with its facing by +5B bit 7 (set, or copied when `copy`). */
static void sk_piece(neo_gactor_t *a,neo_genv_t *e,bool copy) {
    neo_gactor_t *c=child(a,e,0x218c4,0x2a,0,0);
    if (!c) return;
    if (BIT(0x5b,7)) ga_sb(c,0x15,ga_b(c,0x15)|1u);
    else if (copy) ga_sb(c,0x15,ga_b(c,0x15)&~1u);
}
/* 0219CC -> 08CDB2: the item +70 / +72 at (0, 0); its +12/+13 = 20, +34 = 20000. */
static void sk_item(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t kind=W(0x70);
    if (kind>=0x5au) return;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (!c) return;
    ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x72));ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
    ga_place_at(c,a,e,0,0);
    ga_sb(c,0x12,0x20);ga_sb(c,0x13,0x20);ga_sl(c,0x34,0x20000);
}
static ga_result_t sack_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x214ea:
        SB(0x1d,0);ga_palette(a,0x219);SW(0x66,0x8820);SW(0x1e,0x8820);SW(0x4e,1);START(0x230238);
        if (W(0x6e)) SET(0x15,0);
        ga_floor(a,e);SL(0,0x2153c);return GA_MORE;
    case 0x2153c:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) {
            if (BIT(0x5f,7)) {sk_piece(a,e,false);SL(0,0x215a8);}
            SB(0x5a,0);
        }
        return sk_tail(a,e);
    case 0x215a8:START(0x23035e);sound(e,0x103f);SL(0,0x215c8);
        /* fall through */
    case 0x215c8:
        ga_project(a,e);depth8000(a);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x215ea);
        return sk_tail(a,e);
    case 0x215ea:SB(0x5e,B(0x5e)|0x80u);SW(0x74,6);SL(0x50,0x21a0a);START(0x2303e4);goto c622;   /* 004C1C */
    case 0x21612:START(0x2303ca);
    c622:CLR(0x5f,7);SW(0x4e,1);SL(0,0x21634);
        /* fall through */
    case 0x21634:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (W(0x74) && ga_register(a,e,NULL,NULL)) {
            SB(0x5a,0);
            if (BIT(0x5f,7)) {
                SW(0x74,W(0x74)-1u);sk_piece(a,e,false);SL(0,0x21612);
                if (!W(0x74)) sk_item(a,e);
            }
        }
        return sk_tail(a,e);
    case 0x216ba:
        SB(0x1d,0);ga_palette(a,0x3d5);ga_palette(a,0x219);SW(0x66,0x8820);SW(0x1e,0x8820);SW(0x4e,1);START(0x2303f0);
        if (W(0x6e)) SET(0x15,0);
        ga_floor(a,e);SL(0,0x2171a);return GA_MORE;
    case 0x2171a:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) {
            if (BIT(0x5f,7)) {sk_piece(a,e,true);SL(0,0x21790);}
            SB(0x5a,0);
        }
        return sk_tail(a,e);
    case 0x21790:
        if (!e->blood) SB(0x14,B(0x19));                                   /* 01B2BE */
        SL(0x50,0);START(0x230438);sound(e,0x103f);SL(0,0x217c8);
        /* fall through */
    case 0x217c8:
        ga_project(a,e);depth8000(a);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x217ea);
        return sk_tail(a,e);
    case 0x217ea:SB(0x5e,B(0x5e)|0x80u);SW(0x74,6);SL(0x50,0x21a0a);START(0x23052c);goto c822;   /* 004C1C */
    case 0x21812:START(0x230512);
    c822:CLR(0x5f,7);SW(0x4e,1);SL(0,0x21834);
        /* fall through */
    case 0x21834:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (W(0x74) && ga_register(a,e,NULL,NULL)) {
            SB(0x5a,0);
            if (BIT(0x5f,7)) {
                SW(0x74,W(0x74)-1u);sk_piece(a,e,true);SL(0,0x21812);
                if (!W(0x74)) sk_item(a,e);
            }
        }
        return sk_tail(a,e);
    case 0x218c4:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);START(0x230538);ga_palette(a,e->blood?0x37a:0x3ba);SL(0,0x21910);
        /* fall through */
    case 0x21910:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- camp 01F1D4 ------------------------------------------------------------------- */
/* 01F1D4..01F7FC (record of section 2 at camera 1264; tables 0DFDD2 (the
 * +6E words of 01F7FC), hurtboxes 0DFDDA / 0DFDE6, cull rectangles 0DFDF2 /
 * 0DFDFA). 01F1D4 (the group): +60 / +61 = 0, the soldiers 01F386 at (-40,
 * 0) and 01F2D6 at (0, 0) and the stove 01F562 at (-20, 0) (0040BC D0 = 1C,
 * 01ACF2); 01F236: +61 = FF (the stove exploded) -> 004188, else +6E = 0 each
 * update (01F7FC from a child's program: +6E = 0DFDD2[rand & 3] of its
 * creator). 01F2D6 / 01F386 (soldiers, palette 20E, 8844, 22F81A / 22F868):
 * 01ABC8, 01AD8C, 00427A, 004C24: a hit with +5F bit 7 dies by 0406B6 (JSR),
 * else when the group's +61 is FF the state 0B05FE (01F386 flipped): a
 * soldier of the world (the routine 0B05FE of a generic node: window_tick); 01F386 with the group's +6E == 2 -> 01F450 (22F89C,
 * back to 01F3BC at its end); 01945C 0DFDFA -> 01F75A (004188). 01F562
 * (stove, palette 22E, 8844): its pot 01F4DA (palette 208, 8843, 22F9D8, +26
 * = 80 / 100 by the stove's +6E; the stove's +60 = FF -> 01F6E0: +5F bit 3,
 * 14 updates, 004188), 22F8C2; 01F5C8: the group's +6E 1 / 2 -> +6E, 01F636
 * (22F98A / 22F8F6, back to 01F5B2 at its end); a hit with +5F bit 7 ->
 * 01F6C0: +60 = FF, the group's +61 = FF, sound 109F, JMP 01C706; 01945C
 * 0DFDF2 -> 01F7EA (the group's +61 = FF, 0041AC). 01F81A (JMP 003C64, the
 * reset trap) is an explicit stop. The cartridge checks are not reproduced.
 * Proven by the family m3camp (capture A of captures-m3s2c). */
static bool cp(uint32_t r) {return r>=0x1f1d4 && r<0x1f7fc;}
/* 0040BC (D0 = 1C) placed by 01ACF2. */
static neo_gactor_t *child_at(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,0x1c):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_at(c,a,e,dx,dy);
    return c;
}
static bool cp_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x1f7fc) {
        neo_gactor_t *p=sh_creator(a,e);
        uint16_t v=rw(e,0xdfdd2u+2u*(ga_rand(e)&3u));
        if (p) ga_sw(p,0x6e,v);
        return true;
    }
    if (routine==0x1f81a) {ga_fail(e,0x3c64);return true;}             /* JMP 003C64: the reset trap */
    return false;
}
/* 004C24 of the soldiers: a hit with +5F bit 7 dies (0406B6, JSR); else the
 * group's +61 = FF makes a soldier (0B05FE, `flip` for 01F386). Returns the
 * 0406B6 result or -1 without a hit. */
static int cp_hit(neo_gactor_t *a,neo_genv_t *e,bool flip) {
    if (ga_register(a,e,NULL,NULL)) {
        if (BIT(0x5f,7)) return dispatch(a,e,false);
        SB(0x5a,0);return D_MORE;
    }
    neo_gactor_t *p=sh_creator(a,e);
    if (p && ga_b(p,0x61)==0xffu) {if (flip) SB(0x15,B(0x15)^1u);SL(0,0xb05fe);}
    return D_MORE;
}
static ga_result_t cp_soldier_tail(neo_gactor_t *a,neo_genv_t *e,int d) {
    if (d==D_STOP) return GA_UNSUPPORTED;
    if (d==D_DELETE) return GA_DELETE;
    if (ga_cull(a,e,0xdfdfa)) SL(0,0x1f75a);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t camp_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;
    switch (L(0)) {
    case 0x1f1d4:
        SB(0x60,0);SB(0x61,0);
        child_at(a,e,0x1f386,-0x40,0);child_at(a,e,0x1f562,-0x20,0);child_at(a,e,0x1f2d6,0,0);
        SL(0,0x1f236);
        /* fall through */
    case 0x1f236:
        if (B(0x61)==0xffu) return GA_DELETE;
        SW(0x6e,0);return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1f2d6:
        ga_floor(a,e);SB(0x1d,0);ga_palette(a,0x20e);SB(0x60,0);SB(0x61,0);SW(0x4e,1);SW(0x66,0x8844);SW(0x1e,0x8844);
        START(0x22f81a);SL(0,0x1f322);
        /* fall through */
    case 0x1f322:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,cp_call)<0) return GA_UNSUPPORTED;
        return cp_soldier_tail(a,e,cp_hit(a,e,false));
    case 0x1f386:
        ga_floor(a,e);SB(0x1d,0);ga_palette(a,0x20e);SB(0x60,0);SB(0x61,0);SW(0x4e,1);SW(0x66,0x8844);SW(0x1e,0x8844);
        /* fall through */
    case 0x1f3bc:START(0x22f868);SL(0,0x1f3d2);
        /* fall through */
    case 0x1f3d2:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,cp_call)<0) return GA_UNSUPPORTED;
        p=sh_creator(a,e);
        if (p && ga_w(p,0x6e)==2u) SL(0,0x1f450);
        return cp_soldier_tail(a,e,cp_hit(a,e,true));
    case 0x1f450:START(0x22f89c);SL(0,0x1f466);
        /* fall through */
    case 0x1f466:
        ga_project(a,e);depth8000(a);
        if ((r=ga_anim_step(a,e,cp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x1f3bc);
        return cp_soldier_tail(a,e,cp_hit(a,e,true));
    case 0x1f4da:
        SB(0x1d,0);ga_palette(a,0x208);SW(0x66,0x8843);SW(0x1e,0x8843);START(0x22f9d8);SL(0,0x1f510);
        /* fall through */
    case 0x1f510:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,cp_call)<0) return GA_UNSUPPORTED;
        SW(0x26,0x80);
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x1f528);return GA_UNSUPPORTED;}
        if (ga_w(p,0x6e)) SW(0x26,0x100);
        if (ga_b(p,0x60)==0xffu) SL(0,0x1f6e0);
        if (ga_cull(a,e,0xdfdf2)) SL(0,0x1f75a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1f562:
        ga_floor(a,e);SB(0x1d,0);ga_palette(a,0x22e);SB(0x60,0);SB(0x61,0);SW(0x4e,1);SW(0x66,0x8844);SW(0x1e,0x8844);
        child_at(a,e,0x1f4da,0,0);
        /* fall through */
    case 0x1f5b2:START(0x22f8c2);SL(0,0x1f5c8);
        /* fall through */
    case 0x1f5c8:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,cp_call)<0) return GA_UNSUPPORTED;
        p=sh_creator(a,e);
        if (!p) {ga_fail(e,0x1f5da);return GA_UNSUPPORTED;}
        {uint16_t d0=ga_w(p,0x6e);if (d0==1u || d0==2u) {SW(0x6e,d0);SL(0,0x1f636);}}
        goto c5fc;
    case 0x1f636:
        if (W(0x6e)==1u) START(0x22f98a); else START(0x22f8f6);
        SL(0,0x1f66a);
        /* fall through */
    case 0x1f66a:
        ga_project(a,e);depth8000(a);
        if ((r=ga_anim_step(a,e,cp_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x1f5b2);
    c5fc:
        if (ga_register(a,e,NULL,NULL)) {if (BIT(0x5f,7)) SL(0,0x1f6c0); else SB(0x5a,0);}
        if (ga_cull(a,e,0xdfdf2)) SL(0,0x1f7ea);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1f6c0:
        SB(0x60,0xff);
        p=sh_creator(a,e);
        if (p) ga_sb(p,0x61,0xff);
        sound(e,0x109f);SL(0,0x1c706);return neo_effects_tick(a,e);       /* JMP 01C706 */
    case 0x1f6e0:SW(0x26,0x100);SET(0x5f,3);SW(0x70,0x14);SL(0,0x1f6f8);
        /* fall through */
    case 0x1f6f8:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,cp_call)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0xdfdf2)) SL(0,0x1f7ea);
        SW(0x70,W(0x70)-1u);
        if (!W(0x70)) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1f730:
        ga_project(a,e);depth8000(a);
        if (ga_anim_step(a,e,cp_call)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0xdfdf2)) SL(0,0x1f7ea);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x1f75a:return GA_DELETE;
    case 0x1f7ea:
        p=sh_creator(a,e);
        if (p) ga_sb(p,0x61,0xff);
        return GA_DELETE_TREE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- the soldiers' grenade 03C306 ------------------------------------------------ */
/* 03C306 (03C2DE from the soldier programs 23DA62 / 24124C: 00411C after the
 * script actor, 01ACF2 (0, 2C), facing bits 0..1 of the soldier): +5F bit 6,
 * +15 bit 5, E0EF, palette 20E, 23F2A0, +34 = 20000, +62 = +-60000 by the
 * facing, +30 = 0, +50 = 03C2D2, +6E = 1. 03C376: +30 eases to +62 by 1/16,
 * the probe 01495C (-40) sets the fall (+34 += ((dy + 18 or -40) << 16 >> 7)
 * - +34 >> 3), +6E = 0 / 1 / 2 by +34's word + 4 (negative / < 8 / else),
 * water (03C5CC: 03C55C under it -> the splash 03C962, 009F88, E0ED, state
 * 01C706), 0143E4 with +30 kept (+70), 00427A, 019504
 * 03BC18 -> 004188, the row 03C2C2 or +5F bit 0 -> 03C4F8 (sound 1022,
 * 009F88, 1001, JMP 01C706), 004C24: a hit clears +5A, +34 + 4 and +30 = 0.
 * Translated without a MAME trace of it (no capture reached 03C2DE: a
 * literal translation, docs/MISION3.md); its sprites (23F2A0) are not
 * exported yet. */
static bool dg(uint32_t r) {return r==0x3c306 || r==0x3c376 || r==0x3c4f8;}
static ga_result_t grenade_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x3c306:
        SET(0x5f,6);SET(0x15,5);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);ga_palette(a,0x20e);START(0x23f2a0);
        SL(0x34,0x20000);SL(0x62,BIT(0x15,0)?0x60000u:0xfffa0000u);SL(0x30,0);SL(0x50,0x3c2d2);SB(0x6e,1);SL(0,0x3c376);
        /* fall through */
    case 0x3c376: {
        SL(0x30,L(0x30)+ASR(L(0x62)-L(0x30),4));
        int16_t dy=0;uint16_t d0=ga_probe_vertical_dy(a,e,-0x40,&dy)?(uint16_t)(dy+0x18):0xffc0u;   /* 01495C */
        uint32_t d0l=ASR((uint32_t)d0<<16,7);
        SL(0x34,L(0x34)+d0l-ASR(L(0x34),3));
        int16_t v=(int16_t)(W(0x34)+4u);
        if (v<0) SB(0x6e,0); else if ((uint16_t)v<8u) SB(0x6e,1); else SB(0x6e,2);
        if (neo_gate_splash(a,e,1)) {SB(0x1d,0);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0,0x1c706);return GA_MORE;}   /* 03C5CC (the splash 03C962): 009F88, 01C706 */
        {uint32_t v30=L(0x30);ga_air(a,e);SL(0x30,v30);}
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,0x3bc18)) return GA_DELETE;
        if (ga_attack(a,e,0x3c2c2)) SL(0,0x3c4f8);
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SW(0x34,W(0x34)+4u);SL(0x30,0);}
        if (BIT(0x5f,0)) SL(0,0x3c4f8);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x3c4f8:sound(e,0x1022);SB(0x1d,0);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x1c706);return neo_effects_tick(a,e);   /* 009F88, JMP 01C706 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool para(uint32_t r) {return r>=0x5c53c && r<0x5e0b8;}
static bool pod(uint32_t r) {return r>=0x4e990 && r<0x4f7e8;}
static bool gunship(uint32_t r) {return r>=0x97fcc && r<0x99070;}
static bool thrower(uint32_t r) {return r>=0x55f26 && r<0x56eee;}
static bool bomber(uint32_t r) {return r>=0x8391c && r<=0x84bd0;}   /* 08391C (Mission 4) .. 084BD0 inclusive */
static bool box(uint32_t r) {return r==0x391ea || r==0x39232 || r==0x39260 || r==0x39276;}
static const uint32_t rafts[]={0x8afb6,0x8afd0,0x8afde,0x8b090,0x8b138,0x8b18e,0x8b214,0x8b1c4,0x8b3e6,0x8b43e,0x8b4d6,0x8b502};
static const uint32_t trucks[]={0x2b202,0x2b2ea,0x2b316,0x2ae46,0x2ae72,0x2b138,0x2b1d4,0x2b1cc,0x2b61c,0x2b656,0x2b670,0x2b69e,0x2b6e4,0x2b700,0x2bbba,0x2bc36,0x2bc62,0x2bc84,0x2bcac,0x2bcca,0x2bd3c};
bool neo_m3enemy_routine(uint32_t r) {return IN(deaths,r) || IN(splashes,r) || IN(grenadier,r) || shielded(r) || IN(windowed,r) || para(r) || pod(r) || gunship(r) || thrower(r) || bomber(r) || box(r) || IN(trucks,r) || IN(rafts,r) || cv(r) || jp(r) || bd(r) || sk(r) || cp(r) || dg(r);}
ga_result_t neo_m3enemy_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (IN(deaths,r)) return death_tick(a,e);
    if (IN(splashes,r)) return splash_tick(a,e);
    if (IN(grenadier,r)) return grenadier_tick(a,e);
    if (shielded(r)) return shield_tick(a,e);
    if (IN(windowed,r)) return window_tick(a,e);
    if (para(r)) return para_tick(a,e);
    if (pod(r)) return pod_tick(a,e);
    if (gunship(r)) return gunship_tick(a,e);
    if (thrower(r)) return thrower_tick(a,e);
    if (bomber(r)) return bomber_tick(a,e);
    if (box(r)) return box_tick(a,e);
    if (IN(trucks,r)) return truck_tick(a,e);
    if (IN(rafts,r)) return raft_tick(a,e);
    if (cv(r)) return cv_tick(a,e);
    if (jp(r)) return jeep_tick(a,e);
    if (bd(r)) return bird_tick(a,e);
    if (sk(r)) return sack_tick(a,e);
    if (cp(r)) return camp_tick(a,e);
    if (dg(r)) return grenade_tick(a,e);
    ga_fail(e,r);return GA_UNSUPPORTED;
}
