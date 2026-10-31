#include "neo_region.h"
#include "neo_blood.h"
/* Swordsmen 09D846 / 09D862 / 09D876 (Mission 1 cameras 2556..2789 and the
 * ones the helicopter 04857C drops) and their thrown knife 09ECCC, manual
 * translation; ROM labels are provenance only.
 *
 * Constructors: 09EEB8 (09D846/09D862: spawn words +6E..+76 -> flags +96/+98,
 * +8A timer, +8C = FFFF) or 09EF6A (09D876: target point +7E, item +8C/+8E,
 * knives +95), then 09D89A: floor, projection, facing +97, palette by
 * 01B2BE (09EFE4: 0x3FE BLOOD ON, 0x0B8 OFF; src/neo_blood.c), hurtbox
 * 09F610, rank words +88/+82/+84, nearest player +6E (01AF9A: the player
 * node 1014C4/101574 is stored as in the original).
 * States: patrol 09D964, idle 09D9C0/09DA2E/09DA3A, approach 09DB44, chase
 * 09DC5C, run 09DD12, jump attack 09DDCE, flee 09DE84, landing 09DF5E,
 * guard 09DFB2, turn 09E00A, knife throw 09E062 (program callbacks 09F508
 * knife 09ECCC, 09F536), slash 09E122..09E1FE (attack row 09F62C), hop
 * 09E25A/09E250, fall 09E34C. Common tails 09E77E/09E79C/09E7AC (004C24:
 * any hit kills, item 09F53E, death by the hit type or a random one of
 * 09F66C) and 09E81C (cull 09F64C). Deaths 09E842/09E8C2/09E920/09EAB8 (splash
 * 01C97A, neo_effects.c). 09EC34 and 09ECC4 end in 004188/0041AC (the
 * cartridge check of 09EC34 falls through to the deletion on genuine
 * hardware).
 *
 * Explicit stop: 09E382 (flag +96 bit 3, spawn word +72 of 09D846/09D862 non
 * zero: the jump to a fixed point 0DD8/0EE0 through 0B64BA, not recorded). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define CHG(o,b) SB(o,B(o)^(1u<<(b)))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { r=ga_anim_step(a,e,sw_call); if (r<0) return GA_UNSUPPORTED; } while (0)
#define FACING BIT(0x15,0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static bool sw_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint32_t rl(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]:0;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static void gravity_air(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}   /* 0143DC */
/* ---- players (RAM 10C33C mask, nodes 1014C4 / 101574) ---------------------- */
static uint16_t px(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2x:e->players.p1x;}
static uint16_t py(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2y:e->players.p1y;}
/* 01AF9A: the nearest live player's node; carry (true) when none (player 1). */
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
/* 01AEDA: carry when `node` is a player node whose bit is set in 10C33C. */
static bool player_live(const neo_genv_t *e,uint32_t node) {
    unsigned d0;
    if (node==P1_NODE) d0=0; else if (node==P2_NODE) d0=1; else return false;
    return (e->players.active_mask>>d0)&1u;
}
/* 09F07A: keep the player +6E while live, else the nearest. */
static uint32_t k9f07a(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n=L(0x6e);
    if (player_live(e,n)) return n;
    nearest(a,e,&n);SL(0x6e,n);return n;
}
/* 01B04A: player 1, 2 or (both live) one at random; carry when none. */
static bool k1b04a(neo_genv_t *e,uint32_t *node) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    *node=d0==2u?P2_NODE:P1_NODE;
    return d0==0;
}
/* 01B08A: carry when the player is behind. */
static bool behind(neo_gactor_t *a,const neo_genv_t *e,uint32_t node) {
    int16_t d0=S16((uint16_t)(W(0x3c)-px(e,node)));
    return FACING?d0>0:d0<0;
}
static uint16_t absw(uint16_t d) {return (d&0x8000u)?(uint16_t)(0u-d):d;}
static int16_t k9f094(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {return S16(absw((uint16_t)(W(0x3c)-px(e,n))));}
static int16_t k9f0a8(neo_gactor_t *a,const neo_genv_t *e,uint32_t n) {return S16(absw((uint16_t)(W(0x40)-py(e,n))));}
/* ---- shared helpers -------------------------------------------------------- */
/* 09EFC8: ground or air motion. */
static void k9efc8(neo_gactor_t *a,neo_genv_t *e) {if (B(0x15)&0x40u) {uint32_t k;ga_ground(a,e,&k);} else gravity_air(a,e);}
/* 09EFE4: palette by 01B2BE (both signs of +8C ask the same resource). */
static void k9efe4(neo_gactor_t *a,neo_genv_t *e) {ga_palette(a,neo_blood_palette(NEO_BLOOD_SWORDSMAN,e->blood));}
/* 09F432: reach words +82/+84. */
static void k9f432(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x98,7)) {
        SW(0x82,0x20);SW(0x84,0x20);
        SW(0x82,W(0x82)+(uint16_t)((ga_rand(e)&8u)-4u));
        return;
    }
    SW(0x82,rank(a,e,BIT(0x96,7)?0x2d241a:0x2d23e6));
    SW(0x84,rank(a,e,0x2d23cc));
    SW(0x82,W(0x82)+(uint16_t)((ga_rand(e)&0x1cu)-0xeu));
}
/* Walking speed from 2D2244 / 2D2400, << shift, towards the facing. */
static uint32_t speed(neo_gactor_t *a,neo_genv_t *e,unsigned shift) {
    uint32_t d0=(uint32_t)(int32_t)S16(rank(a,e,BIT(0x96,7)?0x2d2400:0x2d2244))<<shift;
    return FACING?d0:0u-d0;
}
/* 09F0BC: carry: turn towards a player behind (timer +93). */
static bool k9f0bc(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x96,7) && !BIT(0x96,5)) return false;
    SB(0x93,B(0x93)-1u);
    if ((int8_t)B(0x93)>0) return false;
    SB(0x93,0);
    return behind(a,e,k9f07a(a,e));
}
/* 09F0FA: carry when the ground ahead ends (014850 carry clear). */
static bool k9f0fa(neo_gactor_t *a,neo_genv_t *e) {
    int16_t d0=S16(W(0x62));
    if (!d0) d0=FACING?2:-2;
    d0=(int16_t)(d0*2);
    bool c;
    if (!BIT(0x15,5)) c=ga_probe_forward(a,e,d0);
    else {CLR(0x15,5);c=ga_probe_forward(a,e,d0);SET(0x15,5);}
    return !c;
}
static bool k9f156(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x98,3)) return false;
    uint32_t n=k9f07a(a,e);
    if (behind(a,e,n)) return false;
    if (S16((uint16_t)(W(0x40)-py(e,n)))<0x10) return false;
    return k9f094(a,e,n)<=0x30;
}
static bool k9f196(neo_gactor_t *a) {return BIT(0x98,5);}
/* 09F1AC: target X (+7E, screen-relative with +96 bit 6) against +3C. */
static int16_t k9f1ac(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=W(0x7e);
    if (BIT(0x96,6)) {neo_contact_parent_t r;if (ga_parent(e,0x10e968u,&r)) d0=(uint16_t)(d0+(uint16_t)((uint32_t)r.x>>16));}  /* 01AD7C */
    return S16((uint16_t)(d0-W(0x3c)));
}
static bool k9f1c8(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x96,7) || BIT(0x96,5)) return false;
    int16_t d=k9f1ac(a,e);
    return d<0?!FACING:FACING;
}
static bool k9f208(neo_gactor_t *a) {return BIT(0x96,7) && !BIT(0x96,4) && !B(0x95) && S16(W(0x88))<=0;}
static bool k9f23a(neo_gactor_t *a) {return BIT(0x5f,0);}
static bool k9f250(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x96,7)) return false;
    uint32_t n=k9f07a(a,e);
    if (behind(a,e,n)) return false;
    return !(k9f094(a,e,n)<S16(W(0x82)));
}
static bool k9f280(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x96,7)) return false;
    uint32_t n=k9f07a(a,e);
    if (behind(a,e,n)) return false;
    if (k9f0a8(a,e,n)>0x20) return false;
    return !(k9f094(a,e,n)>S16(W(0x84)));
}
static bool k9f2bc(neo_gactor_t *a) {
    if (BIT(0x96,7)) return false;
    int16_t x=S16(W(0x3c));
    return x<0x20 || x>0x120;
}
static bool k9f2e6(neo_gactor_t *a) {return FACING?S16(W(0x3c))<0x40:S16(W(0x3c))>0x100;}
static bool on_view(neo_gactor_t *a) {int16_t x=S16(W(0x3c));return x>=0x10 && x<=0x130;}
static bool k9f314(neo_gactor_t *a,neo_genv_t *e) {
    if (!on_view(a) || !BIT(0x98,7)) return false;
    return !ga_players_in(a,e,0x9f654);
}
static bool k9f348(neo_gactor_t *a,neo_genv_t *e) {
    if (!on_view(a) || BIT(0x98,7)) return false;
    if (S16(W(0x88))>0) return false;
    if (k9f2bc(a)) return false;
    uint32_t n=k9f07a(a,e);
    if (behind(a,e,n)) return false;
    return !(k9f094(a,e,n)>S16(W(0x82)));
}
static void k9f39a(neo_gactor_t *a,neo_genv_t *e) {ga_project(a,e);if (k9f1ac(a,e)>=0) SET(0x15,0);}
/* 09F3B0: throw distance +86. */
static void k9f3b0(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n=k9f07a(a,e);int16_t d0;
    if (behind(a,e,n)) d0=0x30;
    else {
        d0=S16((uint16_t)(px(e,n)-W(0x3c)));
        if (!FACING) d0=(int16_t)-d0;
        if (d0<0x30) d0=0x30;
        if (d0>S16(W(0x82))) d0=S16(W(0x82));
    }
    if (!FACING) d0=(int16_t)-d0;
    SW(0x86,(uint16_t)d0);
}
static void k9f412(neo_gactor_t *a,neo_genv_t *e) {if (BIT(0x98,4) && !(ga_rand(e)&1u)) CHG(0x98,5);}
static void k9f4ac(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x98,6) || !BIT(0x98,7)) return;
    uint32_t n=k9f07a(a,e);
    if (behind(a,e,n)) return;
    int16_t d1=k9f094(a,e,n);
    int16_t d0=S16(rank(a,e,BIT(0x96,7)?0x2d241a:0x2d23e6));
    if (d0>d1) return;
    CLR(0x98,7);k9f432(a,e);
}
/* 09F53E: the item +8C (08CDB2: after the script actor, +6E = +8C, +70 =
 * +8E, +72/+7F/+8B = 0, 16 px below). */
static void k9f53e(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x8c)==0xffffu || W(0x8c)>=0x5au) return;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x8e));ga_sw(c,0x6e,W(0x8c));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,0x10);}
}
/* 09EEB8 (09D846/09D862) and 09EF6A (09D876): spawn words to flags. */
static void k9ef52(neo_gactor_t *a) {SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);SW(0x76,0);}
static void k9eeb8(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x6e)) {SET(0x98,5);if (W(0x6e)!=1u) SET(0x98,4);}
    if (W(0x70)) SET(0x98,3);
    if (W(0x72)) {
        if (W(0x72)==1u) SET(0x15,0);
        SET(0x96,3);SB(0x5e,B(0x5e)|0x80u);SET(0x15,2);              /* 004C1C */
    }
    uint16_t d0=W(0x74);
    if (d0) {
        if (d0==1u) SET(0x96,2);
        if (!(ga_rand(e)&1u)) SET(0x96,2);
    }
    if (W(0x76)) {SW(0x8a,W(0x76));SET(0x98,2);}
    SW(0x8c,0xffff);k9ef52(a);
}
static void k9ef6a(neo_gactor_t *a) {
    SET(0x96,7);SET(0x98,5);
    SW(0x7e,W(0x70));
    if (!W(0x70)) SET(0x96,5);
    if (W(0x6e)) SET(0x96,6);
    SB(0x95,(uint8_t)W(0x72));SW(0x8c,W(0x74));SW(0x8e,W(0x76));
    k9ef52(a);
}
/* ---- common tails ---------------------------------------------------------- */
static ga_result_t t81c(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x3c))<(int16_t)0xffa0) SL(0,0x9ec34);
    if (ga_cull(a,e,0x9f64c)) SL(0,0x9ec34);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t t7ac(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x88,W(0x88)-1u);
    if (ga_register(a,e,NULL,NULL)) {
        k9f53e(a,e);
        SL(0,rl(0x9f66cu+4u*(ga_rand(e)&3u)));                          /* 09F5FA */
        uint8_t d0=B(0x5a);
        if (d0==2u) SL(0,0x9e8c2);
        if (d0==1u || d0==0x0cu || d0==0x0du || d0==9u) SL(0,0x9e920);
        if (!(B(0x15)&0x40u)) SL(0,0x9eab8);
    }
    return t81c(a,e);
}
static ga_result_t t79c(neo_gactor_t *a,neo_genv_t *e) {if (!(B(0x15)&0x40u)) SL(0,0x9e34c);return t7ac(a,e);}
static ga_result_t t77e(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x98,2)) {SW(0x8a,W(0x8a)-1u);if (S16(W(0x8a))<=0) SL(0,0x9de84);}
    return t79c(a,e);
}
/* Death start: 019DFC, sound, splash 01C97A (09E842/09E920). */
static void splash(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x1c97a,false):NULL;
    if (c) ga_place_from(c,a,e,0,0);
}
static ga_result_t sword_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);int r;
    for (unsigned guard=0;guard<64;guard++) switch (pc) {
    case 0x9d846: k9eeb8(a,e);CLR(0x98,7);pc=0x9d89a;continue;
    case 0x9d862: k9eeb8(a,e);CLR(0x98,7);SET(0x98,6);pc=0x9d89a;continue;
    case 0x9d876: k9ef6a(a);CLR(0x98,7);k9f39a(a,e);pc=0x9d89a;continue;
    case 0x9d89a: {
        ga_floor(a,e);ga_project(a,e);
        if (S16(W(0x3c))<0xa0) SET(0x97,0);
        /* 03C52C -> 019240: RAM 10C2F6 = 0 (Mission 1) clears the carry: no 03C62A. */
        k9efe4(a,e);
        SW(0x66,0x8820);SW(0x1e,0x8820);SL(0x50,0x9f610);
        uint16_t d0=rank(a,e,BIT(0x96,7)?0x2d244e:0x2d2308);
        if (S16(d0)<=5) SET(0x98,0);
        SW(0x88,d0);k9f432(a,e);
        uint32_t n;nearest(a,e,&n);SL(0x6e,n);
        if (B(0x6c)==3u) {pc=0x9db0e;continue;}                       /* 01B284 */
        if (BIT(0x96,3)) {pc=0x9e382;continue;}
    }   /* fall through */
    case 0x9d928:
        SET(0x97,1);k9f432(a,e);
        SL(0x62,BIT(0x97,0)?0x8000u:0xffff8000u);SB(0x92,0x0f);
        START(0x2a1200);SL(0,0x9d964);
        /* fall through */
    case 0x9d964:
        ga_stationary(a,e);
        if (k9f0fa(a,e)) {
            if (!k9f196(a)) {CHG(0x97,0);SB(0x92,0x0f);SL(0x62,0u-L(0x62));}
            else SL(0,0x9e25a);
        }
        k9efc8(a,e);
        SB(0x92,B(0x92)-1u);
        if ((int8_t)B(0x92)<=0) {CHG(0x97,0);SB(0x92,0x0f);SL(0x62,0u-L(0x62));}
        STEP(r);
        pc=0x9da3a;continue;
    case 0x9d9c0:
        if (k9f314(a,e)) {pc=0x9e122;continue;}
        if (k9f348(a,e)) {pc=0x9e062;continue;}
        if (!BIT(0x96,7)) {pc=0x9d928;continue;}
        CLR(0x97,1);k9f432(a,e);
        SL(0x62,speed(a,e,8));START(0x2a1166);SL(0,0x9da2e);
        /* fall through */
    case 0x9da2e:
        ga_stationary(a,e);STEP(r);
        /* fall through */
    case 0x9da3a: {
        uint32_t n;
        if (!nearest(a,e,&n)) {
            if (k9f0bc(a,e)) {SL(0,0x9e00a);pc=0x9dad2;continue;}
            if (k9f0fa(a,e)) {if (k9f196(a)) SL(0,0x9e25a);pc=0x9dad2;continue;}
            if (k9f2bc(a)) SL(0,0x9dd12);
            if (k9f156(a,e)) SL(0,0x9e250);
            if ((int8_t)B(0x90)<0) {if (k9f250(a,e)) SL(0,0x9db44);}
            else SB(0x90,B(0x90)-1u);
            if (k9f280(a,e)) {
                SL(0,0x9ddce);
                if (BIT(0x98,6)) {SET(0x98,7);k9f432(a,e);SL(0,0x9db44);}
            }
            pc=0x9dad2;continue;
        }
        pc=0x9daee;continue;
    }
    case 0x9dad2:
        if (k9f348(a,e)) SL(0,0x9e062);
        if (k9f314(a,e)) SL(0,0x9e122);
        /* fall through */
    case 0x9daee:
        if (k9f208(a)) SL(0,0x9e040);
        if (k9f1c8(a,e)) SL(0,0x9dc5c);
        return t77e(a,e);
    case 0x9db0e:
        START(0x2a1166);SL(0,0x9db24);
        /* fall through */
    case 0x9db24:
        ga_stationary(a,e);STEP(r);
        if (B(0x6c)!=3u) SL(0,0x9d9c0);                                  /* 01B284 */
        return t77e(a,e);
    case 0x9db44: {
        k9f432(a,e);SL(0x62,speed(a,e,8));
        ga_stationary(a,e);
        if (k9f0fa(a,e)) {pc=0x9d9c0;continue;}
        START(BIT(0x97,1)?0x2a124c:0x2a1242);SL(0,0x9dbbe);
    }   /* fall through */
    case 0x9dbbe: {
        k9efc8(a,e);STEP(r);k9f4ac(a,e);
        if (k9f23a(a)) SL(0,0x9dfb2);
        uint32_t n;
        if (nearest(a,e,&n)) {SL(0,0x9dfb2);return t77e(a,e);}
        if (k9f314(a,e)) SL(0,0x9e122);
        if (k9f348(a,e)) SL(0,0x9dfb2);
        if (k9f0bc(a,e)) {SL(0,0x9dfb2);return t77e(a,e);}
        if (k9f0fa(a,e)) {SL(0,0x9dfb2);if (k9f196(a)) SL(0,0x9e25a);return t77e(a,e);}
        if (k9f156(a,e)) SL(0,0x9e250);
        if (!k9f250(a,e)) SL(0,0x9dfb2);
        return t77e(a,e);
    }
    case 0x9dc5c: {
        uint32_t d0=speed(a,e,8);
        if (BIT(0x96,4)) d0+=d0;
        SL(0x62,d0);START(BIT(0x97,1)?0x2a124c:0x2a1242);SL(0,0x9dcd0);
    }   /* fall through */
    case 0x9dcd0:
        k9efc8(a,e);STEP(r);
        if (k9f0fa(a,e)) SL(0,0x9e25a);
        if (!k9f1c8(a,e)) {SET(0x96,5);SL(0,0x9dfb2);}
        else if (k9f23a(a)) SL(0,0x9dfb2);
        return t77e(a,e);
    case 0x9dd12:
        if (S16(W(0x3c))>0xa0?FACING:!FACING) {pc=0x9e00a;continue;}
        SL(0x62,speed(a,e,9));
        SB(0x91,(ga_rand(e)&7u)+8u);
        START(0x2a1242);SL(0,0x9dd90);
        /* fall through */
    case 0x9dd90:
        k9efc8(a,e);STEP(r);
        if (k9f0fa(a,e)) SL(0,0x9e25a);
        if (k9f23a(a)) SL(0,0x9dfb2);
        SB(0x91,B(0x91)-1u);
        if ((int8_t)B(0x91)<=0) SL(0,0x9dfb2);
        return t77e(a,e);
    case 0x9ddce:
        if (k9f2e6(a)) {
            SL(0x34,0xb3333);SL(0x72,0xfffee148u);SL(0x30,FACING?0x4ccccu:0xfffb3334u);START(0x2a12ee);
        } else {
            SL(0x34,0xaaaaa);SL(0x72,0xfffe38e4u);SL(0x30,FACING?0xfffaaaabu:0x55555u);START(0x2a12ca);
        }
        SL(0,0x9de5c);
        /* fall through */
    case 0x9de5c:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x72));STEP(r);
        if (B(0x15)&0x40u) SL(0,0x9df5e);
        return t7ac(a,e);
    case 0x9de84:
        SB(0x99,0x1e);
        if (!FACING) {pc=0x9e00a;continue;}
        SL(0x62,speed(a,e,9));
        START(BIT(0x97,1)?0x2a124c:0x2a1242);SL(0,0x9defe);
        /* fall through */
    case 0x9defe:
        k9efc8(a,e);STEP(r);
        if (!BIT(0x5f,3)) {
            if (!BIT(0x5f,0)) SB(0x99,0x1e);
            else {SB(0x99,B(0x99)-1u);if ((int8_t)B(0x99)<0) {SET(0x5f,3);SB(0x99,0x1e);}}
        } else {SB(0x99,B(0x99)-1u);if ((int8_t)B(0x99)<0) SL(0,0x9ec34);}
        if (k9f0fa(a,e)) SL(0,0x9e25a);
        return t79c(a,e);
    case 0x9df5e:
        if (!BIT(0x98,1)) START(0x2a1778); else {CLR(0x98,1);START(0x2a178e);}
        SL(0,0x9df98);
        /* fall through */
    case 0x9df98:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x9d9c0);
        return t7ac(a,e);
    case 0x9dfb2:
        if (BIT(0x98,0)) {CLR(0x98,0);pc=0x9d9c0;continue;}
        SB(0x90,0x0f);START(0x2a1382);SL(0,0x9dfe2);
        /* fall through */
    case 0x9dfe2:
        ga_stationary(a,e);STEP(r);
        if (r) {SL(0,0x9d9c0);if (k9f0bc(a,e)) SL(0,0x9e00a);}
        return t7ac(a,e);
    case 0x9e00a:
        SB(0x93,6);START(0x2a11d0);SL(0,0x9e026);
        /* fall through */
    case 0x9e026:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x9d9c0);
        return t7ac(a,e);
    case 0x9e040:
        SW(0x88,0x7fff);CLR(0x96,5);CLR(0x96,6);SET(0x96,4);SW(0x7e,0x200);
        pc=0x9e00a;continue;
    case 0x9e062:
        if (BIT(0x96,7)) {if (!B(0x95)) {pc=0x9e040;continue;}SB(0x95,B(0x95)-1u);}
        k9f412(a,e);k9f3b0(a,e);
        START(0x2a148a);SL(0,0x9e0b2);
        /* fall through */
    case 0x9e0b2:
        if (!(B(0x15)&0x40u)) gravity_air(a,e); else ga_stationary(a,e);
        STEP(r);
        if (r) {
            k9f432(a,e);SW(0x88,rank(a,e,BIT(0x96,7)?0x2d2434:0x2d22a6));
            uint32_t n;k1b04a(e,&n);SL(0x6e,n);
            SL(0,0x9d9c0);
            if (!(B(0x15)&0x40u)) SL(0,0x9e34c);
        }
        return t7ac(a,e);
    case 0x9e122:
        if (BIT(0x96,7)) {if (!B(0x95)) {pc=0x9e040;continue;}SB(0x95,B(0x95)-1u);}
        k9f412(a,e);
        SW(0x88,rank(a,e,BIT(0x96,7)?0x2d24b0:0x2d236a));
        START(BIT(0x97,1)?0x2a13d4:0x2a13ca);SL(0,0x9e194);
        /* fall through */
    case 0x9e194:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x9e1ae);
        return t77e(a,e);
    case 0x9e1ae:
        START(0x2a1430);SL(0,0x9e1c4);
        /* fall through */
    case 0x9e1c4:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x9e1e8);
        ga_attack(a,e,0x9f62c);
        return t77e(a,e);
    case 0x9e1e8:
        START(0x2a1446);SL(0,0x9e1fe);
        /* fall through */
    case 0x9e1fe:
        ga_stationary(a,e);STEP(r);
        if (r) {
            if (BIT(0x98,6)) CLR(0x98,7);
            k9f432(a,e);SW(0x88,rank(a,e,BIT(0x96,7)?0x2d2434:0x2d22a6));SL(0,0x9d9c0);
        }
        return t77e(a,e);
    case 0x9e250: SET(0x15,5);pc=0x9e268;continue;
    case 0x9e25a:
        if (!(ga_rand(e)&1u)) {pc=0x9e2da;continue;}
        /* fall through */
    case 0x9e268:
        START(0x2a154c);SL(0,0x9e27e);
        /* fall through */
    case 0x9e27e:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x9e298);
        return t7ac(a,e);
    case 0x9e298:
        START(0x2a159e);SL(0x34,0x80000);SL(0x72,0xfffe0000u);SL(0x30,FACING?0x10000u:0xffff0000u);
        pc=0x9e318;continue;
    case 0x9e2da:
        START(0x2a15be);SL(0x34,0xa0000);SL(0x72,0xfffec000u);SL(0x30,FACING?0x8000u:0xffff8000u);
        /* fall through */
    case 0x9e318: SL(0,0x9e31e);
        /* fall through */
    case 0x9e31e:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x72));STEP(r);
        if (B(0x15)&0x40u) {CLR(0x15,5);SL(0,0x9df5e);}
        return t7ac(a,e);
    case 0x9e34c:
        START(0x2a12ca);SL(0,0x9e362);
        /* fall through */
    case 0x9e362:
        gravity_air(a,e);STEP(r);
        if (B(0x15)&0x40u) SL(0,0x9df5e);
        return t7ac(a,e);
    case 0x9e382:
        /* The jump to the fixed point 0DD8/0EE0 (0B64BA): not recorded. */
        ga_fail(e,0x9e382);return GA_UNSUPPORTED;
    /* ---- deaths ---- */
    case 0x9e842:
        SB(0x69,0xff);sound(e,0x103e);splash(a,e);                      /* 019DFC */
        START(BIT(0x96,0)?0x2a1892:0x2a17c2);SL(0,0x9e8a8);
        /* fall through */
    case 0x9e8a8: case 0x9e906: case 0x9ea9e: case 0x9eade:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x9ec34);
        return t81c(a,e);
    case 0x9e8c2:
        SB(0x69,0xff);sound(e,0x1041);
        START(BIT(0x96,0)?0x2a1a32:0x2a1962);SL(0,0x9e906);
        pc=0x9e906;continue;
    case 0x9e920:
        if (B(0x5b)&0x80u) SET(0x15,0); else CLR(0x15,0);               /* 09F060 */
        SB(0x69,0xff);splash(a,e);sound(e,0x103e);
        SL(0x34,0x66666);SL(0x72,0xffff5c29u);SL(0x30,FACING?0xfffe6667u:0x19999u);
        START(BIT(0x96,0)?0x2a1b7c:0x2a1b02);SL(0,0x9e9b8);
        /* fall through */
    case 0x9e9b8:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x72));STEP(r);
        if (B(0x15)&0x40u) SL(0,0x9e9e0);
        return t81c(a,e);
    case 0x9e9e0:
        SL(0x34,0x2aaaa);SL(0x72,0xffff1c72u);SL(0x30,FACING?0xfffe0000u:0x20000u);
        START(BIT(0x96,0)?0x2a1bce:0x2a1b54);SL(0,0x9ea42);
        /* fall through */
    case 0x9ea42:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x72));STEP(r);
        if (B(0x15)&0x40u) SL(0,0x9ea6a);
        return t81c(a,e);
    case 0x9ea6a:
        START(BIT(0x96,0)?0x2a1be4:0x2a1b6a);SL(0,0x9ea9e);
        pc=0x9ea9e;continue;
    case 0x9eab8:
        SB(0x69,0xff);sound(e,0x103e);
        START(0x2a1bf6);SL(0,0x9eade);
        pc=0x9eade;continue;
    case 0x9ec34: return GA_DELETE;
    default: ga_fail(e,pc);return GA_UNSUPPORTED;
    }
    ga_fail(e,pc);return GA_UNSUPPORTED;
}
/* 09ECCC: the thrown knife (09F508), speed from the thrower's +86. */
static ga_result_t knife_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x9eccc: {
        uint16_t d0=rank(a,e,0x2d2512)&7u,d1=d0;
        SL(0x34,rl(0x2d253cu+8u*d0));SL(0x72,rl(0x2d253cu+8u*d0+4u));
        int16_t div=S16(rw(0x2d252cu+2u*d1));if (!div) div=1;
        neo_gactor_t *t=e->creator?e->creator(e->ctx,a):NULL;
        int32_t dd=(int32_t)S16(t?ga_w(t,0x86):0)<<8,q=dd/div;
        uint32_t v=(q<-32768 || q>32767)?(uint32_t)(int32_t)(int16_t)(uint16_t)dd:(uint32_t)q;  /* divs.w overflow keeps d0 */
        v=(uint32_t)(int32_t)(int16_t)(uint16_t)v<<5;
        SL(0x30,(v<<2)+v);
        ga_palette(a,0x16c);ga_palette(a,0x16d);ga_palette(a,0x16e);
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        START(0x2a1c84);SL(0,0x9ed76);
    }   /* fall through */
    case 0x9ed76: {
        ga_air(a,e);SL(0x34,L(0x34)+L(0x72));
        if (S16(W(0x40))>=0x1d0 && !(L(0x34)&0x80000000u)) SL(0x34,0);
        STEP(r);
        if (B(0x15)&0x40u) SL(0,0x9edf0);
        if (ga_attack(a,e,0x9f61c)) SL(0,0x9ecc4);
        uint8_t d=B(0x94),n=(uint8_t)(d+1u);
        SB(0x14,B(0x19u+(d&3u)));
        if (n>=3u) n=0;
        SB(0x94,n);
        return t81c(a,e);
    }
    case 0x9edf0:
        SB(0x1d,0);ga_palette(a,0x3fe);                                 /* 009F88 */
        START(0x2a1dbc);SL(0,0x9ee1a);
        /* fall through */
    case 0x9ee1a:
        ga_project(a,e);STEP(r);
        if (r) SL(0,0x9ecc4);
        return t81c(a,e);
    case 0x9ecc4: return GA_DELETE_TREE;                                  /* 0041AC */
    case 0x9ec34: return GA_DELETE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* 09EE34: the camel rider's dropped sword (09F562, neo_camel.c): thrown up
 * and back, palette 0x3FE, program 2A1CEE; 0041AC on the ground. */
static ga_result_t drop_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x9ee34:
        ga_palette(a,0x3fe);
        SL(0x34,0x66666);SL(0x72,0xffff5c29u);SL(0x30,FACING?0x19999u:0xfffe6667u);
        SET(0x5f,3);START(0x2a1cee);SL(0,0x9ee8c);
        /* fall through */
    case 0x9ee8c:
        ga_air(a,e);SL(0x34,L(0x34)+L(0x72));STEP(r);
        if (B(0x15)&0x40u) SL(0,0x9ecc4);
        SW(0x26,W(0x26)-4u);
        return t81c(a,e);
    case 0x9ecc4: return GA_DELETE_TREE;                                  /* 0041AC */
    case 0x9ec34: return GA_DELETE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool sw_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x9f4a4u) {SET(0x5f,3);return true;}
    if (routine==0x9f508u) {                                            /* knife 09ECCC */
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x9eccc,false):NULL;
        if (c) ga_place_from(c,a,e,-16,0x18);
        SET(0x96,0);return true;
    }
    if (routine==0x9f536u) {CLR(0x96,0);return true;}
    return false;
}
static bool drop(uint32_t r) {return r==0x9ee34 || r==0x9ee8c;}
static bool knife(uint32_t r) {return r==0x9eccc || r==0x9ed76 || r==0x9edf0 || r==0x9ee1a || r==0x9ecc4;}
bool neo_swords_routine(uint32_t r) {
    static const uint32_t own[]={0x9d846,0x9d862,0x9d876,0x9d964,0x9d9c0,0x9da2e,0x9db24,0x9db44,0x9dbbe,0x9dc5c,0x9dcd0,
        0x9dd12,0x9dd90,0x9ddce,0x9de5c,0x9de84,0x9defe,0x9df5e,0x9df98,0x9dfb2,0x9dfe2,0x9e00a,0x9e026,0x9e040,0x9e062,
        0x9e0b2,0x9e122,0x9e194,0x9e1ae,0x9e1c4,0x9e1e8,0x9e1fe,0x9e250,0x9e25a,0x9e27e,0x9e298,0x9e31e,0x9e34c,0x9e362,
        0x9e842,0x9e8a8,0x9e8c2,0x9e906,0x9e920,0x9e9b8,0x9e9e0,0x9ea42,0x9ea6a,0x9ea9e,0x9eab8,0x9eade,0x9ec34};
    if (knife(r) || drop(r)) return true;
    for (unsigned i=0;i<sizeof(own)/sizeof(own[0]);i++) if (own[i]==r) return true;
    return false;
}
ga_result_t neo_swords_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (knife(r)) return knife_tick(a,e);
    if (drop(r)) return drop_tick(a,e);
    return sword_tick(a,e);
}
