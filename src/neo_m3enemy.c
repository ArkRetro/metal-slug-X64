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
 * the deletion on genuine hardware). Explicit stops: 040920 (a grounded
 * death with +74 in the air: the block to the scene root), the crouch entry
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
 * Explicit stop: the water splash 03C962 of 03C5C6 (not ported). */
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
/* 04097A */
static void k4097a(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x5f,6)) {SET(0x5f,6);SB(0x6e,0);}
    SL(0x50,0);CLR(0x5f,7);
    SB(0x1d,0);ga_palette(a,neo_blood_palette(NEO_BLOOD_SOLDIER_DEATH,e->blood));   /* 0405EA: 009F88, 01B2BE */
    SL(0x50,0);SB(0x69,0xff);                                                    /* 019DFC */
    uint16_t d6=ga_probe_vertical_d6(a,e,-1);
    SB(0x72,(d6&8u)?0xff:0);
    if (d6&8u) {SW(0x66,0xfff2);SW(0x1e,0xfff2);}
    SB(0x70,BIT(0x15,6)?0xff:0);
    SL(0x76,0);SL(0x62,0);SB(0x71,0);SB(0x6f,0);SB(0x74,0);
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
static int start(neo_gactor_t *a,neo_genv_t *e,uint32_t program,uint32_t routine) {
    if (!ga_anim_start(a,e,program,0x100)) return D_STOP;
    SL(0,routine);return D_MORE;
}
/* 0406B6 (crouch false) / 0407CA (crouch true). */
static int dispatch(neo_gactor_t *a,neo_genv_t *e,bool crouch) {
    if (!B(0x5a)) return D_MORE;
    k4097a(a,e);
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
    }
    ga_fail(e,0x40be6);return D_STOP;   /* 040BE6: D1's upper byte */
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
        if (B(0x74)) {ga_fail(e,0x40920);return false;}
        uint16_t d6=BIT(0x15,5) && B(0x72)?ga_air_400a(a,e):ga_air(a,e);
        if (d6&0x200u) SET(0x15,5);
        if (d6&1u) SB(0x72,(d6&8u)?0xff:0);
    }
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
/* 03C55C: water (class 5) at y-1, y+7, y+15 of the node (013CF8). */
static bool water(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned i=0;i<3;i++)
        if (neo_kernel_point_class(e->kernel,(int32_t)L(0x3c),(int32_t)(L(0x40)-0x10000u+0x80000u*i),5,0,NULL,NULL,NULL)) return true;
    return false;
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
            if (water(a,e)) {ga_fail(e,0x3c962);return GA_UNSUPPORTED;}   /* 03C5C6: the splash 03C962 */
            SB(0x82,0);
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
    case 0x8ae70:
        k1b1dc(a);SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x37c);
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
            if (water(a,e)) {ga_fail(e,0x3c962);return GA_UNSUPPORTED;}   /* 03C5C6: the splash 03C962 */
            SB(0x85,0);
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
    (void)a;(void)parameter;
    if (kind==NEO_ANIM_SPAWN_THROW) {ga_fail(e,routine?routine:0x3bb8e);return false;}   /* the grenade 03BB8E: not ported here */
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
        if ((r=ga_anim_step(a,e,w_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8dad0);
        /* 08DBC4 compares +90 as a word: its upper byte is D0's after
         * 00427A (not modelled). */
        if (!B(0x90)) {ga_fail(e,0x8dbc8);return GA_UNSUPPORTED;}
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
/* ---- dispatch ----------------------------------------------------------------- */
static const uint32_t deaths[]={0x40dc0,0x40df0,0x40d06,0x40db4,0x40ae0};
static const uint32_t splashes[]={0x40f88,0x40f60,0x40f74,0x40fd4};
static const uint32_t grenadier[]={0x8a732,0x8a788,0x8a798,0x8a7ae,0x8a838,0x8a84e,0x8a878,0x8a88e,0x8aa6e,0x8aaaa,
    0x8ab70,0x8aba0,0x8ac54,0x8ad18,0x8ae70,0x8aea6};
static bool in(const uint32_t *k,unsigned n,uint32_t r) {for (unsigned i=0;i<n;i++) if (k[i]==r) return true;return false;}
#define IN(t,r) in(t,sizeof t/sizeof t[0],r)
static const uint32_t windowed[]={0x8da12,0x8da34,0x8dad0,0x8dae4,0x8db2a,0x8db40,0x8db66,0x8db7c,0x8db98,0x8dbac,0x8dbe8,0x8dbfe,0xb05fe,0xb0626,0xb0632};
static bool shielded(uint32_t r) {return (r>=0x9ae1e && r<0x9cb70) || r==0x9ba2a;}
bool neo_m3enemy_routine(uint32_t r) {return IN(deaths,r) || IN(splashes,r) || IN(grenadier,r) || shielded(r) || IN(windowed,r);}
ga_result_t neo_m3enemy_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (IN(deaths,r)) return death_tick(a,e);
    if (IN(splashes,r)) return splash_tick(a,e);
    if (IN(grenadier,r)) return grenadier_tick(a,e);
    if (shielded(r)) return shield_tick(a,e);
    if (IN(windowed,r)) return window_tick(a,e);
    ga_fail(e,r);return GA_UNSUPPORTED;
}
