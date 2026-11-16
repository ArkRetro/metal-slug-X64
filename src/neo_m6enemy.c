#include "neo_region.h"
#include "neo_target.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 6 stage 2 (the factory) enemies, manual translation (ROM labels as
 * provenance; MAME run4 replay from sec10_304271, captures-m6/fam/G,
 * docs/MISION6.md "Etapa 2").
 *
 * ---- The jumper 086712 / 08671A / 086728 (script records from 2E03DE) ----
 * Entries: 086712, 08671A (+8B bit 3: no drop from above), 086728 (+8B bit 1:
 * walks to the x +7E, then waits for the player). 087838 (the script words):
 * +8C = (+78 != 0), +6E != 0 -> +8B bit 0, +7E = +70 (+ the root's +14 word
 * with +72 != 0, 01AD7C), +84 = +74 low byte (the palette set), +85 = +76 low
 * byte (the shot's palette), +6E..+78 cleared.
 * 086732: palette 087E1E[+84], flash palette 0882 (004520), layers 8820,
 * 01B1DC, 0143E4, +4E = rank 2D2C68, +80 = rank 2D2D2C (the shot delay), +76
 * = rank 2D2CCA << 8 (the walk speed), hurtbox 087C92, +6E = 01AF9A (its
 * player), 0878D2 (+7A = 2D2FA0 + 8 rank 2D2F52: the sight rectangle); from
 * above (+40 > 0200, not bit 3) 086868: the drop (+34 = -rank 2D2F1E << 8,
 * hurtbox 087C9E, 2835CE; 087B30 fires 087BB8 on the way down); +8C: the
 * wake 0867E8 (sound 116B, 28315A, palette 087E24[+84], +14 = +19); else
 * 0146E8 and 01B284 (lying, +6C = 3: 086938) or the walk 086A1C.
 * Fields: +7A the sight rectangle, +6E the player, +76 the speed, +62 the
 * walk speed, +80 the shot delay, +82 a step count, +86 a sight delay, +87
 * the turn delay, +88 the shots left, +89 the shot number, +8B bits: 0 from
 * the script, 1 the walker 086728, 2 it reached +7E, 3 no drop, 4 the player
 * within x of the rectangle (or none), 5 inside it (or none), 6 / 7 the jump
 * kind (086D4C); +8D on the ground (087C86).
 * The common tail 08712E / 08713E / 087184: 086EB4 when it leaves the
 * ground, +87 and +80 count down, 004C24 (+5A = 0 without a kill), +5F bit 7
 * -> the death 0871AA, x < FFA0 or outside 087CEE (01945C) -> 0873C0.
 * The deaths: kind 7 (08728C, the blast: +30 += 002E7C of a random speed at
 * +5B << 4, 2839CA / 283B7A + 283BF8 with the pieces 087C0E) or 0871D4
 * (2838F0 / 283740 with palette 087E2A[+84] in the air or by 01B206); 0873C0
 * is the cartridge check before 004188 (not reproduced: on genuine hardware
 * it ends in 004188).
 * Program calls: 087B84 (the shot 087460 at (-2C, 11)), 087C0E (one to six
 * pieces 087716), 087C86 (+8D).
 * The shots 087458 / 087460 (by 087BB8 / 087B84): angle 087D06 / 087D0E /
 * 087D16 [+89 & 7], +4E = rank 2D2E0A, speed rank 2D2E24, boxes 087CAA /
 * 087CB6, layers E0EF, palette C1 / 189 (+85), 283CCC, 004C1C, +80 = rank
 * 2D2E3E; 087520: 0143E4, then 087598 (speed rank 2D2E58, +80 = rank
 * 2D2EBC; every rank 2D2EA2 updates it turns 4 toward the player, 002EB4)
 * and 087640 (283E30); 0049E8 087CDE or a hit -> 087674 (C2 / 18A,
 * 283EBE); outside 087CF6 (019504) -> 0041AC.
 * 0876CC (the muzzle of 087BB8): layers E0ED, palette C0, 283674.
 * 087716 (the pieces): velocities 087D1E[random 0..3][+89 & 7], layers 1001,
 * palette C3, 283F4C (speed 80 half the time), 087892; on the ground 0877DE
 * (+30 random, 283FB6); +88 = 0 -> 0041AC; outside 087CFE -> 0041AC. */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,(uint32_t)(v))
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
#define BIT(o,b) ((ga_b(a,o)>>(b))&1u)
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define S16(v) ((int16_t)(uint16_t)(v))
#define S32(v) ((int32_t)(uint32_t)(v))
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static uint16_t rw(uint32_t at) {const uint8_t *p=ga_rom(at);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t at) {return (uint32_t)rw(at)<<16|rw(at+2);}
static bool rom_ok(neo_genv_t *e,uint32_t at) {if (ga_rom(at)) return true;ga_fail(e,at);return false;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t t) {   /* 0260C8 */
    if (!rom_ok(e,t)) return 0;
    return ga_rank_word(a,e,ga_rom(t));
}
static uint16_t px(const neo_genv_t *e,uint32_t n) {return n==P2_NODE?e->players.p2x:e->players.p1x;}
static uint16_t py(const neo_genv_t *e,uint32_t n) {return n==P2_NODE?e->players.p2y:e->players.p1y;}
/* 01AEDA: carry (true) while the player node is in play. */
static bool live(const neo_genv_t *e,uint32_t n) {
    if (n!=P1_NODE && n!=P2_NODE) return false;
    return (e->players.active_mask>>(n==P2_NODE?1u:0u))&1u;
}
/* 01AF9A: the nearest player in play (P1 on ties or with none; carry true when none). */
static bool nearest(neo_gactor_t *a,const neo_genv_t *e,uint32_t *n) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m) {*n=P1_NODE;return true;}
    if (m==1u) {*n=P1_NODE;return false;}
    if (m==2u) {*n=P2_NODE;return false;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    *n=S16(d2)<S16(d1)?P2_NODE:P1_NODE;
    return false;
}
/* 01AF66: carry (true) when the node's x is outside the rectangle's x span (mirrored by +15 bit 0). */
static bool outside_x(neo_gactor_t *a,const neo_genv_t *e,uint32_t n,uint32_t rect) {
    int16_t d1=S16(px(e,n)-W(0x3c)),d2=S16(rw(rect)),d3=S16(rw(rect)+rw(rect+4));
    if (BIT(0x15,0)) d1=(int16_t)-d1;
    return d1<d2 || d1>d3;
}
/* 01AF3C: carry (true) when the node is outside the rectangle (y by +15 bit 1, then 01AF66). */
static bool outside(neo_gactor_t *a,const neo_genv_t *e,uint32_t n,uint32_t rect) {
    int16_t d1=S16(py(e,n)-W(0x40)),d2=S16(rw(rect+2)),d3=S16(rw(rect+2)+rw(rect+6));
    if (BIT(0x15,1)) d1=(int16_t)-d1;
    if (d1<d2 || d1>d3) return true;
    return outside_x(a,e,n,rect);
}
/* 01AF34: carry (true) when the player node is in play and off the ground (its +15 bit 6 clear). */
static bool live_air(const neo_genv_t *e,uint32_t n) {
    if (!live(e,n)) return false;
    return !(e->player_flags15[n==P2_NODE?1:0]&0x40u);
}
/* 01B08A: carry (true) when the node is behind (by +15 bit 0). */
static bool behind(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {
    int16_t d0=S16(W(0x3c)-px(e,n));
    return BIT(0x15,0)?d0>0:d0<0;
}
/* 01B206: carry for the hit kinds of the table 01B1F2 (ROM bytes). */
static bool flinch(neo_gactor_t *a,neo_genv_t *e) {
    static const uint8_t t[20]={0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,0,0,0};
    if (B(0x5a)>=20u) {ga_fail(e,0x1b206);return false;}
    return t[B(0x5a)]!=0;
}
static void clear_speed(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}   /* 01B1DC */
static void fall(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}   /* 0143DC */
static void layers(neo_gactor_t *a,uint16_t v) {SW(0x66,v);SW(0x1e,v);}
static void z_from_y(neo_gactor_t *a) {if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));}   /* 01AD8C */
static void snd(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}   /* 0120F8 */
static neo_gactor_t *kid(neo_gactor_t *a,neo_genv_t *e,uint32_t r,uint16_t reserve,int16_t dx,int16_t dy) {   /* 0040BC + 01B220 */
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,r,reserve):e->spawn?e->spawn(e->ctx,a,r,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
/* 087E1E / 087E24 / 087E2A: palettes by +84. */
static bool jp_palette(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    uint32_t at=table+2u*B(0x84);
    if (!rom_ok(e,at)) return false;
    ga_palette(a,rw(at));return true;                                   /* 009EA8 (D1 = 0) */
}
static bool jp_start(neo_gactor_t *a,neo_genv_t *e,uint32_t p) {return ga_anim_start(a,e,p,0x100);}   /* 004256 */
static bool jp_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static int jp_step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,jp_call);}   /* 00427A */
/* 0878D2: +7A = 2D2FA0 + 8 rank 2D2F52. */
static void jp_sight(neo_gactor_t *a,neo_genv_t *e) {SL(0x7a,0x2d2fa0u+(uint16_t)(rank(a,e,0x2d2f52)<<3));}
/* 0878F0: a new player when its one left. */
static void jp_target(neo_gactor_t *a,neo_genv_t *e) {
    if (live(e,L(0x6e))) return;
    uint32_t n;nearest(a,e,&n);SL(0x6e,n);
}
/* 08790A: +8B bits 5 / 4. */
static void jp_look(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n=L(0x6e);
    if (live(e,n)) {
        uint32_t r=L(0x7a);
        if (!rom_ok(e,r)) return;
        if (outside(a,e,n,r)) {
            CLR(0x8b,5);
            if (outside_x(a,e,n,r)) {CLR(0x8b,4);return;}
            SET(0x8b,4);return;
        }
    }
    SET(0x8b,5);SET(0x8b,4);
}
static bool jp_no_floor(neo_gactor_t *a,neo_genv_t *e) {return !ga_probe_forward(a,e,(int16_t)W(0x62));}   /* 087954 */
/* 087964: carry at the screen edge it walks to. */
static bool jp_edge(neo_gactor_t *a) {
    int16_t d0=S16(W(0x62)),d1=S16(W(0x3c));
    if (!d0) return false;
    return d0>=0?d1>0x110:d1<0x30;
}
static bool jp_9b0(neo_gactor_t *a,neo_genv_t *e) {   /* 0879B0 */
    SL(0x62,0u-L(0x62));
    bool c=jp_edge(a);
    SL(0x62,0u-L(0x62));
    if (c) return false;
    if (!BIT(0x8b,5)) return false;
    uint16_t d0=(uint16_t)(W(0x3c)-px(e,L(0x6e)));
    if (d0&0x8000u) d0=(uint16_t)(0u-d0);
    return S16(d0)<0x28;
}
static bool jp_9fe(neo_gactor_t *a) {return BIT(0x8b,4) || BIT(0x5f,0);}   /* 0879FE */
static bool jp_a1e(neo_gactor_t *a,neo_genv_t *e) {   /* 087A1E */
    if (!BIT(0x8b,4) || BIT(0x5f,0) || jp_edge(a)) return true;
    uint16_t d0=(uint16_t)(W(0x3c)-px(e,L(0x6e)));
    if (d0&0x8000u) d0=(uint16_t)(0u-d0);
    return S16(d0)>0x38;
}
static bool jp_a64(neo_gactor_t *a,neo_genv_t *e) {   /* 087A64 */
    if (!BIT(0x8b,4)) return jp_no_floor(a,e);
    if (BIT(0x8b,5)) return false;
    return S16(py(e,L(0x6e)))<S16(W(0x40));
}
static bool jp_aa0(neo_gactor_t *a,neo_genv_t *e) {   /* 087AA0 */
    if (!BIT(0x8b,4)) return jp_no_floor(a,e);
    if (BIT(0x8b,5)) return false;
    if (S16(py(e,L(0x6e)))<=S16(W(0x40))) return false;
    return !live_air(e,L(0x6e));
}
static bool jp_af0(neo_gactor_t *a,neo_genv_t *e) {   /* 087AF0 */
    if (S16(W(0x80))>0) return false;
    int16_t d0=S16(W(0x3c));
    if (d0<0x18 || d0>0x128) return false;
    if (S16(W(0x40))>0x1e0) return false;
    uint32_t n;
    return !nearest(a,e,&n);
}
static bool jp_ae6(neo_gactor_t *a,neo_genv_t *e) {return BIT(0x8b,5) && jp_af0(a,e);}   /* 087AE6 */
static bool jp_b30(neo_gactor_t *a,neo_genv_t *e) {return S16(W(0x40))<=0x1e0 && jp_af0(a,e);}   /* 087B30 */
static bool jp_b3a(neo_gactor_t *a) {   /* 087B3A */
    uint16_t d0=(uint16_t)(W(0x3c)-W(0x7e));
    if (d0&0x8000u) d0=(uint16_t)(0u-d0);
    if (S16(d0)>8) return false;
    SET(0x8b,2);return true;
}
static bool jp_b64(neo_gactor_t *a,neo_genv_t *e) {   /* 087B64 */
    if ((int8_t)B(0x87)>0) return false;
    SB(0x87,0);jp_target(a,e);
    return behind(a,e,L(0x6e));
}
/* 087BB8: the muzzle 0876CC and the shot 087458 at (-3, 10). */
static void jp_bb8(neo_gactor_t *a,neo_genv_t *e) {
    kid(a,e,0x876cc,0x2a,0,0);
    neo_gactor_t *c=kid(a,e,0x87458,0x1c,-3,0x10);
    if (c) {ga_sb(c,0x89,B(0x89));ga_sb(c,0x85,B(0x85));}
}
/* 0878A2: +30 / +34 = cos / sin (002A7C / 0029FC) of +8A times +76. */
static void sh_speed(neo_gactor_t *a) {
    uint8_t g=B(0x8a);int16_t v=S16(W(0x76));
    SL(0x30,(uint32_t)((int32_t)ga_cosine(g)*v));SL(0x34,(uint32_t)((int32_t)ga_sine(g)*v));
}
/* 087892 */
static void jp_rise(neo_gactor_t *a,neo_genv_t *e) {ga_air(a,e);SL(0x34,L(0x34)+L(0x72));}
static bool jp_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x87b84: {                                                      /* the shot 087460 at (-2C, 11) */
        neo_gactor_t *c=kid(a,e,0x87460,0x1c,-0x2c,0x11);
        if (c) {ga_sb(c,0x89,B(0x89));ga_sb(c,0x85,B(0x85));}
        return true;
    }
    case 0x87c0e: {                                                      /* one to six pieces 087716 */
        uint16_t d0=(uint16_t)(ga_rand_range(e,6)+1u);                   /* 0029F2 */
        uint16_t reserve=0x1c;
        for (;;) {
            neo_gactor_t *c=kid(a,e,0x87716,reserve,0,0);
            d0--;
            if (c) {ga_sb(c,0x89,(uint8_t)d0);if (d0&1u) ga_sb(c,0x88,1);}
            if (S16(d0)<=0) break;
            reserve=0x2a;
        }
        return true;
    }
    case 0x87c86: SB(0x8d,BIT(0x15,6)?0xff:0);return true;
    }
    return false;
}
#define JP_HIT 0x87c92u
static bool jp_routine(uint32_t r);
/* 087184 */
static ga_result_t jp_t3(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x3c))<S16(0xffa0)) SL(0,0x873c0);
    if (ga_cull(a,e,0x87cee)) SL(0,0x873c0);                             /* 01945C */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 08713E */
static ga_result_t jp_t2(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x87)) SB(0x87,B(0x87)-1u);
    SW(0x80,W(0x80)-1u);
    if (S16(W(0x80))<0) SW(0x80,0);
    if (ga_register(a,e,NULL,NULL) && !BIT(0x5f,7)) SB(0x5a,0);          /* 004C24 */
    if (BIT(0x5f,7)) SL(0,0x871aa);
    return jp_t3(a,e);
}
/* 08712E */
static ga_result_t jp_t1(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) SL(0,0x86eb4);
    return jp_t2(a,e);
}
#define START(p) do {if (!jp_start(a,e,(p))) return GA_UNSUPPORTED;} while (0)
#define STEP() jp_step(a,e)
static ga_result_t jp_tick(neo_gactor_t *a,neo_genv_t *e) {
    int done;
    switch (L(0)) {
    case 0x86712: case 0x8671a: case 0x86728: {
        uint32_t r=L(0);
        SB(0x8c,W(0x78)?0xff:0);                                         /* 087838 */
        if (W(0x6e)) SET(0x8b,0);
        uint16_t d0=W(0x70);
        if (W(0x72)) {
            neo_contact_parent_t root;
            if (!ga_parent(e,0x10e968u,&root)) {ga_fail(e,0x1ad7c);return GA_UNSUPPORTED;}
            d0=(uint16_t)(d0+(uint16_t)((uint32_t)root.x>>16));          /* 01AD7C: + the root's +14 word */
        }
        SW(0x7e,d0);SB(0x84,(uint8_t)W(0x74));SB(0x85,(uint8_t)W(0x76));
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);SW(0x76,0);SW(0x78,0);
        if (r==0x8671au) SET(0x8b,3);
        if (r==0x86728u) SET(0x8b,1);
        if (!jp_palette(a,e,0x87e1e)) return GA_UNSUPPORTED;              /* 086732 */
        ga_flash_palette(a,0x882);                                       /* 004520 */
        layers(a,0x8820);clear_speed(a);ga_air(a,e);
        SW(0x4e,rank(a,e,0x2d2c68));SW(0x80,rank(a,e,0x2d2d2c));
        SL(0x76,(uint32_t)((int32_t)S16(rank(a,e,0x2d2cca))<<8));
        SL(0x50,JP_HIT);
        uint32_t n;nearest(a,e,&n);SL(0x6e,n);
        jp_sight(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (!BIT(0x8b,3) && S16(W(0x40))>0x200) goto drop;
        if (B(0x8c)) {                                                   /* 0867E8 */
            snd(e,0x116b);
            START(0x28315a);
            if (!jp_palette(a,e,0x87e24)) return GA_UNSUPPORTED;
            SB(0x14,B(0x19));SL(0x50,0);clear_speed(a);
            SL(0,0x86836);
            goto s86836;
        }
        ga_floor(a,e);                                                   /* 0146E8 */
        goto s867da;
    }
    s867da:
        if (B(0x6c)==3u) goto s86938;                                    /* 01B284 */
        goto s86a1c;
    case 0x86836: s86836:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x86850);
        return jp_t3(a,e);
    case 0x86850:
        SB(0x8c,0);ga_stationary(a,e);
        if (!BIT(0x15,6)) goto s86eb4;
        goto s867da;
    drop: {                                                              /* 086868 */
        SL(0x34,0u-((uint32_t)(int32_t)S16(rank(a,e,0x2d2f1e))<<8));
        SL(0x50,0x87c9e);
        if (BIT(0x8b,0)) SET(0x15,5);
        if (BIT(0x8b,1)) {
            uint32_t d0=L(0x76);
            if (S16(W(0x3c))>S16(W(0x7e))) d0=0u-d0;
            SL(0x30,d0);
        }
        SW(0x80,rank(a,e,0x2d2f38));
        START(0x2835ce);
        SL(0,0x868dc);
    }   /* fall through */
    case 0x868dc:
        if (BIT(0x8b,1) && jp_b3a(a)) SL(0x30,0);
        ga_air(a,e);z_from_y(a);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (jp_b30(a,e)) {
            jp_bb8(a,e);SB(0x89,B(0x89)+1u);SW(0x80,rank(a,e,0x2d2f38));
        }
        if (BIT(0x15,6)) SL(0,0x86f04);
        return jp_t2(a,e);
    s86938:
        SL(0x50,JP_HIT);clear_speed(a);START(0x28320a);SL(0,0x8695c);
        /* fall through */
    case 0x8695c:
        ga_stationary(a,e);
        if (!BIT(0x15,6)) {
            if (S32(L(0x34))>S32(0xfffe0000u)) SL(0x34,0xfffe0000u);
            fall(a,e);
        } else {SL(0x30,0);SL(0x34,0);}
        if (STEP()<0) return GA_UNSUPPORTED;
        if (B(0x6c)!=3u) SL(0,0x86a1c);                                  /* 01B284 */
        return jp_t1(a,e);
    case 0x869ac:
        SL(0x50,JP_HIT);START(0x2833fc);SL(0,0x869ca);
        /* fall through */
    case 0x869ca:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x86938);
        return jp_t1(a,e);
    case 0x869e4:
        SL(0x50,JP_HIT);START(0x2834e6);SL(0,0x86a02);
        /* fall through */
    case 0x86a02:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x86938);
        return jp_t1(a,e);
    case 0x86a1c: s86a1c:
        SL(0x50,JP_HIT);
        if (BIT(0x5f,0)) SB(0x86,0x10);
        if (BIT(0x8b,1)) goto s87066;
        SW(0x82,0);clear_speed(a);
        {uint32_t d0=L(0x76);if (!BIT(0x15,0)) d0=0u-d0;SL(0x62,d0);}
        START(0x28320a);SL(0,0x86a72);
        /* fall through */
    case 0x86a72:
        ga_stationary(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        jp_target(a,e);jp_look(a,e);
        if (S16(W(0x3c))>0x110) SL(0,0x86cf8);
        if (!B(0x86)) {if (!BIT(0x8b,4)) SL(0,0x86b52);}                 /* 087998 */
        else SB(0x86,B(0x86)-1u);
        if (jp_9b0(a,e)) SL(0,0x86c3a);
        if (jp_a64(a,e)) SL(0,0x86d08);
        if (jp_aa0(a,e)) SL(0,0x86cd8);
        if (S16(W(0x3c))<0x30) SL(0,0x86cf8);
        if (jp_ae6(a,e)) SL(0,0x86f4c);
        if (jp_b64(a,e)) SL(0,0x86b0e);
        return jp_t1(a,e);
    case 0x86b0e: s86b0e:
        SL(0x50,JP_HIT);START(0x2833fc);SB(0x87,6);SL(0,0x86b32);
        /* fall through */
    case 0x86b32:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) {SB(0x15,B(0x15)^1u);SL(0,0x86a1c);}
        return jp_t1(a,e);
    case 0x86b52: s86b52:
        SL(0x50,JP_HIT);SW(0x82,(uint16_t)(W(0x82)<<1));
        {uint32_t d0=L(0x76);if (!BIT(0x15,0)) d0=0u-d0;SL(0x62,d0);}
        START(0x2832b0);SL(0,0x86b8e);
        /* fall through */
    case 0x86b8e:
        ga_ground(a,e,NULL);
        if (STEP()<0) return GA_UNSUPPORTED;
        jp_look(a,e);
        if (jp_edge(a)) SL(0,0x86a1c);
        SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))>=0) return jp_t1(a,e);
        SW(0x82,0);
        if (jp_9fe(a)) SL(0,0x86a1c);
        if (jp_a64(a,e)) SL(0,0x86d08);
        if (jp_aa0(a,e)) SL(0,0x86cd8);
        if (jp_ae6(a,e)) SL(0,0x86f4c);
        if (jp_b64(a,e)) SL(0,0x86b0e);
        return jp_t1(a,e);
    case 0x86c08: {
        SL(0x50,JP_HIT);
        if (BIT(0x8b,1)) goto s87066;
        uint32_t d0=L(0x76);if (BIT(0x15,0)) d0=0u-d0;SL(0x62,d0);
        if (jp_edge(a)) goto s86b52;
        goto s86c5c;
    }
    case 0x86c3a: {
        if (ga_rand(e)&1u) {CLR(0x8b,7);SET(0x8b,6);goto s86d14;}       /* 086CE8 */
        uint32_t d0=L(0x76);if (BIT(0x15,0)) d0=0u-d0;SL(0x62,d0);
    }
    s86c5c:
        SL(0x50,JP_HIT);START(0x283356);SL(0,0x86c7a);
        /* fall through */
    case 0x86c7a:
        ga_ground(a,e,NULL);
        if (STEP()<0) return GA_UNSUPPORTED;
        jp_look(a,e);
        if (jp_edge(a)) SL(0,0x86a1c);
        SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))>=0) return jp_t1(a,e);
        SW(0x82,0);
        if (jp_a1e(a,e)) SL(0,0x86a1c);
        if (jp_ae6(a,e)) SL(0,0x86f4c);
        if (jp_b64(a,e)) SL(0,0x86b0e);
        return jp_t1(a,e);
    case 0x86cd8: CLR(0x8b,7);CLR(0x8b,6);goto s86d14;
    case 0x86ce8: CLR(0x8b,7);SET(0x8b,6);goto s86d14;
    case 0x86cf8: SET(0x8b,7);SET(0x8b,6);goto s86d14;
    case 0x86d08: SET(0x8b,7);CLR(0x8b,6);
    s86d14:
        SL(0x50,JP_HIT);START(0x283540);SL(0,0x86d32);
        /* fall through */
    case 0x86d32:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x86d4c);
        return jp_t1(a,e);
    case 0x86d4c: {                                                      /* the jump kinds */
        uint16_t d0=ga_rand(e);
        bool f=BIT(0x15,0)!=0;
        if (BIT(0x8b,6)) {
            if (BIT(0x8b,7)) {SL(0x34,0x66666);SL(0x72,0xffff92c6u);SL(0x30,f?0x66666u:0xfff9999au);}
            else {SL(0x34,0x66666);SL(0x72,0xffff5c29u);SL(0x30,f?0xfffd999au:0x26666u);}
        } else if (BIT(0x8b,7)) {
            SET(0x15,5);
            if (d0&1u) {SL(0x34,0x66666);SL(0x72,0xfffeb852u);SL(0x30,0);}
            else {SL(0x34,0x44444);SL(0x72,0xffff6e5eu);SL(0x30,f?0x11111u:0xfffeeeefu);}
        } else {
            CLR(0x15,5);
            if (d0&1u) {SL(0x34,0x8cccc);SL(0x72,0xffff8f5du);SL(0x30,f?0x13333u:0xfffecccdu);}
            else {SL(0x34,0x99999);SL(0x72,0xffff851fu);SL(0x30,f?0xccccu:0xffff3334u);}
        }
        goto s86eb4;
    }
    case 0x86eb4: s86eb4:
        if (S32(L(0x72))>=0) SL(0x72,0xffffb000u);
        START(0x2835ce);SL(0x50,0x87c9e);SL(0,0x86ee6);
        /* fall through */
    case 0x86ee6:
        jp_rise(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (BIT(0x15,6)) SL(0,0x86f04);
        return jp_t2(a,e);
    case 0x86f04:
        SL(0x72,0);clear_speed(a);CLR(0x15,5);SL(0x50,JP_HIT);START(0x2836b2);SL(0,0x86f32);
        /* fall through */
    case 0x86f32:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x86a1c);
        return jp_t1(a,e);
    case 0x86f4c:
        SB(0x89,0);SL(0x50,JP_HIT);SB(0x88,(uint8_t)rank(a,e,0x2d2d8e));START(0x283458);SL(0,0x86f7e);
        /* fall through */
    case 0x86f7e:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x86f98);
        return jp_t1(a,e);
    case 0x86f98:
        SB(0x88,B(0x88)-1u);snd(e,0x10d1);SW(0x80,rank(a,e,0x2d2df0));START(0x2834e6);SL(0,0x86fcc);
        /* fall through */
    case 0x86fcc:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) {
            if ((int8_t)B(0x88)>0) {SB(0x89,B(0x89)+1u);if (S16(W(0x80))<=0) SL(0,0x86f98);}
            else SL(0,0x87008);
        }
        return jp_t1(a,e);
    case 0x87008: {
        SW(0x82,(uint16_t)((ga_rand(e)&0x1cu)+8u));
        jp_sight(a,e);
        uint32_t n;nearest(a,e,&n);SL(0x6e,n);
        SW(0x80,rank(a,e,0x2d2d2c));START(0x283534);SL(0,0x8704c);
    }   /* fall through */
    case 0x8704c:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x86c08);
        return jp_t1(a,e);
    case 0x87066: s87066:
        SL(0x50,JP_HIT);
        if (BIT(0x8b,2)) {START(0x28320a);SL(0,0x8708e);goto s8708e;}
        if (S16(W(0x3c))>S16(W(0x7e))) {if (BIT(0x15,0)) goto s86b0e;}
        else if (!BIT(0x15,0)) goto s86b0e;
        {uint32_t d0=L(0x76);if (!BIT(0x15,0)) d0=0u-d0;SL(0x62,d0);}
        START(0x2832b0);SL(0,0x87110);
        goto s87110;
    case 0x8708e: s8708e:
        ga_stationary(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (jp_af0(a,e)) SL(0,0x86f4c);                                  /* 087B2E */
        if (jp_b64(a,e)) SL(0,0x86b0e);
        return jp_t1(a,e);
    case 0x87110: s87110:
        ga_ground(a,e,NULL);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (jp_b3a(a)) SL(0,0x87066);
        return jp_t1(a,e);
    /* ---- the deaths ---- */
    case 0x871aa:
        if (B(0x5a)==7u) goto s8728c;
        if (BIT(0x15,6) && !(flinch(a,e) && (int16_t)(int8_t)(uint8_t)ga_rand(e)<0)) {   /* 0871D4 */
            layers(a,0x8820);snd(e,0x1142);clear_speed(a);START(0x283740);SL(0,0x87206);
            goto s87206;
        }
        layers(a,0x8820);snd(e,0x1142);clear_speed(a);                  /* 087220 */
        SB(0x1d,0);                                                      /* 009F88 */
        if (!jp_palette(a,e,0x87e2a)) return GA_UNSUPPORTED;
        START(0x2838f0);SL(0,0x87272);
        /* fall through */
    case 0x87272: case 0x87206: s87206:
        ga_stationary(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x873c0);
        return jp_t3(a,e);
    s8728c: {
        uint32_t d0=((uint32_t)(ga_rand(e)&0x7fffu)+0x8000u)<<2;
        uint32_t vx,vy;ga_polar(d0,(uint8_t)(B(0x5b)<<4),&vx,&vy);       /* 002E7C */
        SL(0x30,L(0x30)+vx);
        if (BIT(0x15,6)) {
            layers(a,0x8820);snd(e,0x1142);START(0x2839ca);SB(0x1d,0);ga_palette(a,0x852);SL(0,0x872fa);
            goto s872fa;
        }
        if (!jp_call(a,e,NEO_ANIM_CALL,0x87c0e,0)) return GA_UNSUPPORTED;    /* 08731E */
        snd(e,0x1142);layers(a,0x8820);START(0x283b7a);SB(0x1d,0);ga_palette(a,0x852);SL(0,0x87362);
        goto s87362;
    }
    case 0x872fa: s872fa:
    case 0x8739c:
        SL(0x30,L(0x30)-(uint32_t)(S32(L(0x30))>>3));fall(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x873c0);
        return jp_t3(a,e);
    case 0x87362: s87362:
        SL(0x30,L(0x30)-(uint32_t)(S32(L(0x30))>>4));fall(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (!BIT(0x15,6)) return jp_t3(a,e);
        START(0x283bf8);SL(0,0x8739c);
        SL(0x30,L(0x30)-(uint32_t)(S32(L(0x30))>>3));fall(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x873c0);
        return jp_t3(a,e);
    case 0x873c0: return GA_DELETE;                                      /* the cartridge check, then 004188 */
    case 0x87450: return GA_DELETE_TREE;                                 /* 0041AC */
    /* ---- the shots 087458 / 087460 ---- */
    case 0x87458: case 0x87460: {
        uint32_t t=L(0)==0x87458u?0x87d06u:BIT(0x15,0)?0x87d16u:0x87d0eu;
        if (!rom_ok(e,t)) return GA_UNSUPPORTED;
        SB(0x8a,ga_rom(t)[B(0x89)&7u]);
        SW(0x4e,rank(a,e,0x2d2e0a));SW(0x76,rank(a,e,0x2d2e24));
        SL(0x50,0x87caa);SL(0x54,0x87cb6);layers(a,0xe0ef);SET(0x15,5);
        ga_palette(a,B(0x85)?0x189:0xc1);
        START(0x283ccc);
        SB(0x5e,B(0x5e)|0x80u);                                          /* 004C1C */
        SW(0x80,rank(a,e,0x2d2e3e));sh_speed(a);SL(0,0x87520);
    }   /* fall through */
    case 0x87520:
        ga_air(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        SW(0x80,W(0x80)-1u);
        if (S16(W(0x80))<=0) SL(0,0x87598);
    s87540: {
        uint32_t n;
        if (nearest(a,e,&n)) SL(0,0x87640);
        if (!e->attack) {ga_fail(e,0x49e8);return GA_UNSUPPORTED;}
        if (ga_attack(a,e,0x87cde)) SL(0,0x87674);                       /* 0049E8 */
        ga_register(a,e,NULL,NULL);SB(0x5a,0);                           /* 004C24 */
        if (BIT(0x5f,7)) SL(0,0x87674);
    }
    s87582:
        if (ga_cull_rect(a,e,0x87cf6)) SL(0,0x87450);                    /* 019504 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x87598:
        SW(0x76,rank(a,e,0x2d2e58));SW(0x80,rank(a,e,0x2d2ebc));SL(0,0x875be);
        /* fall through */
    case 0x875be: {
        ga_air(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        SW(0x80,W(0x80)-1u);
        if (S16(W(0x80))<=0) SL(0,0x87640);
        SB(0x88,B(0x88)-1u);
        if ((int8_t)B(0x88)>0) goto s87540;
        SB(0x88,(uint8_t)rank(a,e,0x2d2ea2));
        uint32_t n;nearest(a,e,&n);
        int8_t g=(int8_t)neo_angle(S16(px(e,n)-W(0x3c)),S16((uint16_t)(py(e,n)+0x14u)-W(0x40)));   /* 002EB4 */
        if (g!=(int8_t)B(0x8a)) {
            SB(0x8a,B(0x8a)+(g>=(int8_t)B(0x8a)?4u:0xfcu));
            sh_speed(a);
        }
        goto s87540;
    }
    case 0x87640:
        SL(0x50,0);START(0x283e30);SL(0,0x8765a);
        /* fall through */
    case 0x8765a:
        ga_air(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x87450);
        goto s87582;
    case 0x87674:
        ga_palette(a,B(0x85)?0x18a:0xc2);START(0x283ebe);SL(0,0x876b2);
        /* fall through */
    case 0x876b2: case 0x876fc:
        ga_project(a,e);                                                 /* 01ABC8 */
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x87450);
        goto s87582;
    case 0x876cc:
        layers(a,0xe0ed);ga_palette(a,0xc0);START(0x283674);SL(0,0x876fc);
        ga_project(a,e);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x87450);
        goto s87582;
    /* ---- the pieces 087716 ---- */
    case 0x87716: {
        uint32_t t=rl(0x87d1eu+4u*(ga_rand(e)&3u));
        uint32_t at=t+12u*(B(0x89)&7u);
        if (!rom_ok(e,0x87d1e) || !rom_ok(e,at)) return GA_UNSUPPORTED;
        SL(0x34,rl(at));SL(0x72,rl(at+4));
        uint32_t d0=rl(at+8);if (!BIT(0x15,0)) d0=0u-d0;SL(0x30,d0);
        layers(a,0x1001);SET(0x15,5);ga_palette(a,0xc3);START(0x283f4c);
        if (ga_rand(e)&1u) SW(0x26,0x80);
        SL(0,0x877a0);
    }   /* fall through */
    case 0x877a0:
        jp_rise(a,e);
        if (STEP()<0) return GA_UNSUPPORTED;
        if (BIT(0x15,6)) {SL(0,0x877de);if (!B(0x88)) SL(0,0x87450);}
    s877c8:
        if (ga_cull_rect(a,e,0x87cfe)) SL(0,0x87450);                    /* 019504 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x877de: {
        uint32_t d0=ga_rand(e)&0xfu;
        if (d0&1u) d0=0u-d0;
        SL(0x30,d0<<12);START(0x283fb6);SL(0,0x87812);
    }   /* fall through */
    case 0x87812:
        ga_air(a,e);
        if (!BIT(0x15,6)) SL(0x30,0);
        if ((done=STEP())<0) return GA_UNSUPPORTED;
        if (done) SL(0,0x87450);
        goto s877c8;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool jp_routine(uint32_t r) {
    static const uint32_t k[]={0x86712,0x8671a,0x86728,0x86836,0x86850,0x868dc,0x8695c,0x869ac,0x869ca,0x869e4,0x86a02,0x86a1c,0x86a72,
        0x86b0e,0x86b32,0x86b52,0x86b8e,0x86c08,0x86c3a,0x86c7a,0x86cd8,0x86ce8,0x86cf8,0x86d08,0x86d32,0x86d4c,0x86eb4,0x86ee6,0x86f04,
        0x86f32,0x86f4c,0x86f7e,0x86f98,0x86fcc,0x87008,0x8704c,0x87066,0x8708e,0x87110,0x871aa,0x87206,0x87272,0x872fa,0x87362,0x8739c,
        0x873c0,0x87450,0x87458,0x87460,0x87520,0x87598,0x875be,0x87640,0x8765a,0x87674,0x876b2,0x876cc,0x876fc,0x87716,0x877a0,0x877de,
        0x87812};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
/* ---- 0BE460: the platform of the jumpers (script records at camera 1040) ----
 * 0BE460: program 2A9626, palette 508, layers 0001, +72 = FF (riders' map), +73 = 0. 0BE498: 01AC38; with +72 set and +2C <=
 * +6E, +72 = 0; with +73 clear and +70 >= +2C: +2C = +70, +34 = 0, +72 = FF, program 2A9650 (every update it stays there);
 * the cell map 0BE338 (+72 set: group 3) or 0BE34C (0140C4 at (0, -1)), 00427A; outside 0BE526 (01945C) -> 004188. */
static ga_result_t pf_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0xbe460:
        if (!jp_start(a,e,0x2a9626)) return GA_UNSUPPORTED;
        ga_palette(a,0x508);layers(a,1);SB(0x72,0xff);SB(0x73,0);SL(0,0xbe498);
        /* fall through */
    case 0xbe498:
        ga_move(a,e);                                                    /* 01AC38 */
        if (B(0x72) && S16(W(0x2c))<=S16(W(0x6e))) SB(0x72,0);
        if (!B(0x73) && S16(W(0x70))>=S16(W(0x2c))) {
            SW(0x2c,W(0x70));SL(0x34,0);SB(0x72,0xff);
            if (!jp_start(a,e,0x2a9650)) return GA_UNSUPPORTED;
        }
        if (!e->dyn_set) {ga_fail(e,0x140c4);return GA_UNSUPPORTED;}
        e->dyn_set(e->ctx,a,B(0x72)?0xbe338u:0xbe34cu,0,-1);             /* 0140C4 */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0xbe526)) SL(0,0xbe520);                         /* 01945C */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0xbe520: return GA_DELETE;                                      /* 004188 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool pf_routine(uint32_t r) {return r==0xbe460u || r==0xbe498u || r==0xbe520u;}
/* ---- the end of stage 2: 0BE8A2 (made by the list call 0BE888 at progress 3952) and 0BE8F6 (script, camera 4240) ----
 * 0BE8A2: 10C716 = 0; 0BE8AE: with 10C716 set, the stage exit 0B6ED2 (0040BC, D0 = 1C) at (11E0, 0038) of the root
 * 10E968, +6E = 0BE89A, +72 = 1220, then 004188. 0BE8F6: 10C716 = 1, 004188. */
static ga_result_t ex_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->flags) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    switch (L(0)) {
    case 0xbe8f6: e->flags->m6_exit=1;return GA_DELETE;
    case 0xbe8a2: e->flags->m6_exit=0;SL(0,0xbe8ae);
        /* fall through */
    case 0xbe8ae: {
        if (!e->flags->m6_exit) return GA_MORE;
        neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0xb6ed2,0x1c):e->spawn?e->spawn(e->ctx,a,0xb6ed2,false):NULL;
        if (c) {ga_sl(c,0x38,0x10e968u);ga_sl(c,0x28,0x11e00000u);ga_sl(c,0x2c,0x00380000u);ga_sl(c,0x6e,0x0be89au);ga_sw(c,0x72,0x1220);}
        return GA_DELETE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool ex_routine(uint32_t r) {return r==0xbe8a2u || r==0xbe8aeu || r==0xbe8f6u;}
bool neo_m6enemy_routine(uint32_t r) {return jp_routine(r) || pf_routine(r) || ex_routine(r);}
ga_result_t neo_m6enemy_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (jp_routine(L(0))) return jp_tick(a,e);
    if (pf_routine(L(0))) return pf_tick(a,e);
    if (ex_routine(L(0))) return ex_tick(a,e);
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
