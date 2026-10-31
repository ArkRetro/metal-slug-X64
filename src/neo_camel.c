#include "neo_region.h"
#include "neo_blood.h"
/* Camel riders 09F682 (Mission 1 camera 2184, script record 2D6D24), manual
 * translation of ROM 09F682..0A0AE8 with the rider's death hand-over to the
 * swordsman deaths 09E920/09EAB8 (neo_swords.c) through 09F562..09F5F8; ROM
 * labels are provenance only.
 *
 * Three nodes per rider: the camel 09F682 (invisible state machine with the
 * walk/trot programs), the rider 09FDFE/09FEAA (0040BC after the camel: it
 * follows the camel's offsets +88/+89, shows the program of the camel's
 * state +8B and carries the hurtbox) and the camel's head 09FF26/09FF92
 * (0040BC after the rider: hurtbox A0B02 while the rider's +8F is set).
 *
 * Camel fields: +6E node of its player (0A0576: 01AEDA / 01B04A), +72 speed
 * (rank 2D30A0 << 8), +7A motion routine (A039E pace, A03D6 keep away,
 * A043A turn back at the edge), +7E shot timer (rank 2D311C / 2D31E0 /
 * 2D317E), +80 life time (spawn word +76, 0 = 7FFF; 0A0296 entries keep
 * none), +82 pace timer, +84 stop point (spawn word +74), +86 shot bursts
 * (0A0296), +87 shots of a burst (rank 2D3242), +8A close count, +8B state
 * for the rider (FF none, 0 walk, 1 aim, 2/3 shoot, 4 lower, 5/6 turn, 7..10
 * swap side, B/C stop, D lose the head), +8C spawn word +72 (0A0664),
 * +8D trot pose (0A0774, offsets A0B6A -> +88/+89), +90 flags (bit 0 facing,
 * 1 far side, 2 rider up, 3 head program, 4 head hit, 5 headless, 6 pace
 * direction, 7 0A0296 entry).
 * Camel states: entry walk 09F7C4/09F7F4 (A0532 at +84) or wait 09F730/
 * 09F752 (A055C) and stop 09F76A/09F782 (the rider's program ends: +61),
 * walk 09F846/09F878, keep away 09F90A/09F940 (A0508 six times close),
 * stand 09F968/09F9A6, turn 09F9FA/09FA28, swap side 09FA48/09FAC0, lose the
 * head 09FADE/09FB2C (sound 108E), aim 09FB48/09FBA6, shoot 09FBC2/09FBD2/
 * 09FC0A, lower 09FC4C/09FC6C, leave 09FC9C/09FCA6/09FCE4/09FCFE (019DFC,
 * 6 px per update, also when +5F bit 7 is set); tails 09FD0E (life time,
 * A072A), 09FD3A (+5F bit 7), 09FD4A (cull A0B1E) and 09FD60 (+5F bit 7,
 * 004188; the cartridge check before it falls through to the deletion on
 * genuine hardware and is not reproduced). 03C532 -> 019240: RAM 10C2F6 = 0
 * (Mission 1) clears the carry: no water ripple 03C62A.
 * Program callbacks: A0774 (trot pose), A0AB6/A0ABE (+90 bit 2), A0AC6/
 * A0AD8 (+90 bit 3 of the rider and the camel), A09D0/A0A02 (sound 10FF and
 * the shot A0034 at (-27,31) / (-32,31)), A0A34/A0A5C (the case A020A),
 * A0A68 (three or four pieces A016A).
 * Shot A0034 (rank 2D30BA, attack row A0B0E, explosion A00E8 with program
 * 22E504), pieces A016A / case A020A (speeds A0B2E / A0B5E): 09FDF6 = 0041AC.
 *
 * Rider death (09FF0A): the camel's +5F bit 7 is set, then 09F562 (the
 * sword 09EE34 at (0,16), neo_swords.c) or 09F58C (+96 bit 0, headless),
 * 009F88, the palette by 01B2BE at 09F598 (neo_blood.c: ON 0x3FE, OFF
 * 0x0B8), the facing by +5B and 09E920 or 09EAB8 at random.
 *
 * Explicit stops: none known in the code; 0AAA88 (the vehicle the player
 * rides, 0A0664) answers 0: the port has no player vehicles. */
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
#define FACING BIT(0x15,0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint32_t rl(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]:0;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
static bool cm_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
#define STEP(r) do { r=ga_anim_step(a,e,cm_call); if (r<0) return GA_UNSUPPORTED; } while (0)
/* ---- players --------------------------------------------------------------- */
static uint16_t px(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2x:e->players.p1x;}
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
/* 01B04A: player 1, 2 or (both live) one at random; carry when none. */
static bool k1b04a(neo_genv_t *e,uint32_t *node) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    *node=d0==2u?P2_NODE:P1_NODE;
    return d0==0;
}
static uint16_t absw(uint16_t d) {return (d&0x8000u)?(uint16_t)(0u-d):d;}
/* ---- camel services -------------------------------------------------------- */
/* 0A0576: keep the player +6E while live, else 01B04A. */
static void ka0576(neo_gactor_t *a,neo_genv_t *e) {
    if (player_live(e,L(0x6e))) return;
    uint32_t n;
    if (!k1b04a(e,&n)) SL(0x6e,n);
}
static void ground(neo_gactor_t *a,neo_genv_t *e) {uint32_t k;ga_ground(a,e,&k);}       /* 01456A */
static void gravity_air(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}   /* 0143DC */
/* 0A0334: air, or stand (01B1DC + 01436A). */
static void ka0334(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) gravity_air(a,e);
    else {SL(0x30,0);SL(0x34,0);SL(0x62,0);ga_stationary(a,e);}
}
/* 0A047E: program speed from the walking speed. */
static void ka047e(neo_gactor_t *a) {
    uint32_t d0=L(0x62);
    if (d0&0x80000000u) d0=0u-d0;
    uint16_t w=(uint16_t)(d0>>8);
    if (S16(w)<0x100) w=0x100;
    if (S16(w)>0x400) w=0x400;
    SW(0x26,w);
}
/* 0A05F0: carry when the player +6E is nearer than rank 2D3306. */
static bool ka05f0(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (nearest(a,e,&n)) return false;
    uint16_t d0=rank(a,e,0x2d3306);
    uint32_t d2=L(0x6e);
    if (!d2) return false;
    return absw((uint16_t)(W(0x3c)-px(e,d2)))<d0;
}
/* 0A0630: carry when the nearest player is nearer than rank 2D3320; the
 * node in *node (player 1 when none). */
static bool ka0630(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {
    if (nearest(a,e,node)) return false;
    uint16_t d0=rank(a,e,0x2d3320);
    return absw((uint16_t)(W(0x3c)-px(e,*node)))<d0;
}
/* 0A039E: pace back and forth (+82 updates each way). */
static void ka039e(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x82,W(0x82)-1u);
    if (S16(W(0x82))<0) {
        uint32_t d0=(uint32_t)((int32_t)L(0x72)>>1);
        CHG(0x90,6);
        if (!BIT(0x90,6)) d0=0u-d0;
        SL(0x62,d0);SW(0x82,0x0f);
    }
    ground(a,e);
}
/* 0A03D6: turn at the screen edges; standing, walk away from the nearest
 * player unless the player +6E is near (0A05F0). */
static void ka03d6(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t v=L(0x62);
    if (v) {
        if (v&0x80000000u) {if (S16(W(0x3c))<0x20) SL(0x62,0u-L(0x62));}
        else if (S16(W(0x3c))>0x120) SL(0x62,0u-L(0x62));
    } else {
        uint32_t n;
        if (!nearest(a,e,&n)) {
            uint32_t d0=L(0x72);
            if (S16(W(0x3c))<S16(px(e,n))) d0=0u-d0;
            SL(0x62,d0);
            if (!ka05f0(a,e)) SL(0x62,0u-L(0x62));
        }
    }
    ground(a,e);
}
/* 0A043A: walk back towards the middle for 0x1E updates, then pace. */
static void ka043a(neo_gactor_t *a,neo_genv_t *e) {
    if (!W(0x82)) {
        SW(0x82,0x1e);
        uint32_t d0=L(0x72);
        if (S16(W(0x3c))>0xa0) d0=0u-d0;
        SL(0x62,d0);ka047e(a);
    }
    SW(0x82,W(0x82)-1u);
    if (S16(W(0x82))<=0) SL(0x7a,0xa039e);
    ground(a,e);
}
/* 0A0356: air, or the motion routine +7A on the ground. */
static bool ka0356(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x15,6)) {gravity_air(a,e);return true;}
    if (BIT(0x90,1)) {SL(0x62,0);ga_stationary(a,e);return true;}
    uint32_t r=L(0x7a);
    if (!r) {ga_stationary(a,e);return true;}
    switch (r) {
    case 0xa039e: ka039e(a,e);break;
    case 0xa03d6: ka03d6(a,e);break;
    case 0xa043a: ka043a(a,e);break;
    default: ga_fail(e,r);return false;
    }
    ka047e(a);return true;
}
/* 0A04A8: carry when walking out past the screen edges (not 0A0296). */
static bool ka04a8(neo_gactor_t *a) {
    if (BIT(0x90,7) || !L(0x62)) return false;
    if (L(0x62)&0x80000000u) return S16(W(0x3c))<=0x20;
    return S16(W(0x3c))>=0x120;
}
/* 0A04E2: carry when the nearest player is close or the player +6E far. */
static bool ka04e2(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x90,7)) return false;
    uint32_t n;
    if (ka0630(a,e,&n)) return true;
    return !ka05f0(a,e);
}
/* 0A0508: carry after six close updates in a row. */
static bool ka0508(neo_gactor_t *a,neo_genv_t *e) {
    if (ka04e2(a,e)) SB(0x8a,B(0x8a)+1u); else SB(0x8a,0);
    return (int8_t)B(0x8a)>=6;
}
/* 0A0532: carry once past the stop point +84 in the walking direction. */
static bool ka0532(neo_gactor_t *a) {
    int16_t x=S16(W(0x3c)),s=S16(W(0x84));
    return (W(0x62)&0x8000u)?s>=x:x>=s;
}
/* 0A059A: shot timer +7E inside the screen, then carry when the player +6E
 * is nearer than rank 2D3306. */
static bool ka059a(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x3c))<0x20 || S16(W(0x3c))>0x120) return false;
    SW(0x7e,W(0x7e)-1u);
    if (S16(W(0x7e))>0) return false;
    SW(0x7e,0);
    uint16_t d0=rank(a,e,0x2d3306);
    uint32_t d2=L(0x6e);
    if (!d2) return false;
    return absw((uint16_t)(W(0x3c)-px(e,d2)))<d0;
}
/* 0A0664: carry to swap sides (spawn word +72 bits by the player's
 * vehicle: 0AAA88, none in the port). */
static bool ka0664(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (ka0630(a,e,&n)) return false;
    if (!ka059a(a,e)) return false;
    /* 01AEDA on +6E, then 0AAA88: the port's players ride nothing (d0 = 0). */
    if (BIT(0x8c,1)) return !BIT(0x90,1);
    return BIT(0x90,1);
}
/* 0A06E4: carry when facing away from the player. */
static bool ka06e4(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (!ka0630(a,e,&n)) n=L(0x6e);
    if (!n) return false;
    if (S16(W(0x3c))>S16(px(e,n))) return BIT(0x90,0);
    return !BIT(0x90,0);
}
/* 0A072A: the head was hit (bit 4) and the camel still has it (bit 5). */
static bool ka072a(neo_gactor_t *a) {return !BIT(0x90,5) && BIT(0x90,4);}
/* 0A074A: on the far side and outside the middle of the screen. */
static bool ka074a(neo_gactor_t *a) {
    if (!BIT(0x90,1)) return false;
    return S16(W(0x3c))<0x20 || S16(W(0x3c))>0x120;
}
/* 0A0774: trot pose +8D by the walking direction, offsets A0B6A. */
static void ka0774(neo_gactor_t *a) {
    uint32_t d0=L(0x30);
    if (FACING) d0=0u-d0;
    if (d0&0x80000000u) {SB(0x8d,B(0x8d)+1u);if ((int8_t)B(0x8d)>=0x0c) SB(0x8d,0);}
    else {SB(0x8d,B(0x8d)-1u);if ((int8_t)B(0x8d)<0) SB(0x8d,0x0b);}
    const uint8_t *t=ga_rom(0xa0b6au+2u*(B(0x8d)&0x7fu));
    if (t) {SB(0x88,t[0]);SB(0x89,t[1]);}
}
/* 0A023E / 0A0296: spawn words to fields. */
static void ka027e(neo_gactor_t *a) {SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);SW(0x76,0);}
static void ka023e(neo_gactor_t *a) {
    if (!W(0x6e)) {SET(0x15,0);SET(0x90,0);}
    if (W(0x70)) SET(0x90,1);
    SW(0x84,W(0x74));SB(0x8c,W(0x72));
    uint16_t d0=W(0x76);
    if (!d0) d0=0x7fff;
    SW(0x80,d0);ka027e(a);
}
static void ka0296(neo_gactor_t *a) {
    SW(0x84,W(0x74));
    if (!W(0x6e)) {SET(0x15,0);SET(0x90,0);}
    if (W(0x70)) SET(0x90,1);
    SB(0x86,W(0x76));SB(0x8c,W(0x72));ka027e(a);
}
/* 0A02D0: floor, projection, the rider 09FDFE, palette 0x117. */
static void ka02d0(neo_gactor_t *a,neo_genv_t *e) {
    ga_floor(a,e);ga_project(a,e);
    SW(0x66,0x8820);SW(0x1e,0x8820);
    ga_spawn_from(a,e,0x9fdfe,0,0);
    SB(0x8b,0xff);SB(0x61,0);ka0576(a,e);
    ga_palette(a,0x117);SL(0x7a,0xa03d6);SB(0x8f,0);
}
static void state(neo_gactor_t *a,uint8_t s) {SB(0x8b,s);SB(0x61,0);}
static ga_result_t tail_d4a(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0xa0b1e)) SL(0,0x9fd60);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t tail_d3a(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x5f,7)) SL(0,0x9fca6);
    return tail_d4a(a,e);
}
static ga_result_t tail_d0e(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x90,7)) {SW(0x80,W(0x80)-1u);if (S16(W(0x80))<=0) SL(0,0x9fc9c);}
    if (ka072a(a)) SL(0,0x9fade);
    return tail_d3a(a,e);
}
/* 09F8CC / 09F9B4: the choices of the walk and stand states. */
static void choose(neo_gactor_t *a,neo_genv_t *e,bool side) {
    uint32_t n;
    if (nearest(a,e,&n)) return;
    if (ka059a(a,e)) SL(0,0x9fb48);
    if (ka0664(a,e)) SL(0,0x9fa48);
    if (ka06e4(a,e)) SL(0,0x9f9fa);
    if (side && ka074a(a)) SL(0,0x9fa48);
}
static ga_result_t camel_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);int r;
    for (unsigned guard=0;guard<64;guard++) switch (pc) {
    case 0x9f682:
        ka023e(a);ka02d0(a,e);
        /* 03C532 -> 019240: RAM 10C2F6 = 0 (Mission 1) clears the carry: no 03C62A. */
        SW(0x7e,rank(a,e,0x2d311c));
        SL(0x72,(uint32_t)(int32_t)S16(rank(a,e,0x2d30a0))<<8);
        SL(0x7a,0xa03d6);
        pc=BIT(0x90,1)?0x9f730:0x9f7c4;continue;
    case 0x9f6ca: {
        ka0296(a);ka02d0(a,e);
        SW(0x7e,rank(a,e,0x2d33b6));
        SL(0x72,(uint32_t)(int32_t)S16(rank(a,e,0x2d333a))<<8);
        uint16_t d1=W(0x84),d0=W(0x3c);
        if (S16(d0)>0xa0) d1=(uint16_t)(0u-d1);
        SW(0x84,d0+d1);
        SL(0x7a,0xa039e);SET(0x90,7);
        pc=BIT(0x90,1)?0x9f730:0x9f7c4;continue;
    }
    case 0x9f730:
        START(0x2a1ea6);SW(0x66,0x1110);SW(0x1e,0x1110);SL(0,0x9f752);
        /* fall through */
    case 0x9f752:
        if (S16(W(0x84))>=S16(W(0x3c))) {state(a,0x0c);SL(0,0x9f76a);}
        /* fall through */
    case 0x9f76a:
        if (B(0x61)) {state(a,9);SL(0,0x9f782);pc=0x9f782;continue;}
        ka0334(a,e);STEP(r);CLR(0x90,2);
        goto f7a6;
    case 0x9f782:
        ka0334(a,e);STEP(r);SET(0x90,2);
    f7a6:
        if (B(0x61)) {SW(0x66,0x8820);SW(0x1e,0x8820);SL(0,0x9f846);}
        return tail_d3a(a,e);
    case 0x9f7c4: {
        uint32_t d0=L(0x72);
        if (S16(W(0x84))<S16(W(0x3c))) d0=0u-d0;
        SL(0x62,d0);START(0x2a1dd8);ka047e(a);SL(0,0x9f7f4);
    }   /* fall through */
    case 0x9f7f4:
        if (ka0532(a)) {state(a,0x0b);SL(0,0x9f82a);if (!ka0356(a,e)) return GA_UNSUPPORTED;}
        else if (!BIT(0x15,6)) gravity_air(a,e); else ground(a,e);
        goto f82e;
    case 0x9f82a:
        if (!ka0356(a,e)) return GA_UNSUPPORTED;
    f82e:
        STEP(r);
        if (B(0x61)) SL(0,0x9f846);
        return tail_d3a(a,e);
    case 0x9f846:
        if (BIT(0x90,1)) {pc=0x9f968;continue;}
        state(a,0);START(0x2a1dd8);SL(0x7a,0xa039e);SL(0,0x9f878);
        /* fall through */
    case 0x9f878: {
        ka0576(a,e);
        if (!ka0356(a,e)) return GA_UNSUPPORTED;
        if (ka04a8(a) && L(0x7a)!=0xa043au) {SW(0x82,0);SL(0x7a,0xa043a);}
        if (L(0x7a)!=0xa043au) {
            uint32_t n;
            if (ka04e2(a,e)) SL(0,0x9f90a);
            if (nearest(a,e,&n)) SL(0,0x9f968);
        }
    }
    f8cc:
        STEP(r);choose(a,e,false);
        return tail_d0e(a,e);
    case 0x9f90a:
        if (BIT(0x90,1)) {pc=0x9f968;continue;}
        state(a,0);START(0x2a1dd8);SL(0x62,0);SL(0x7a,0xa03d6);SL(0,0x9f940);
        /* fall through */
    case 0x9f940:
        ka0576(a,e);
        if (!ka0356(a,e)) return GA_UNSUPPORTED;
        if (BIT(0x5f,0)) SL(0x62,0u-L(0x62));
        if (ka0508(a,e)) SL(0,0x9f846);
        goto f8cc;
    case 0x9f968:
        state(a,0);START(BIT(0x90,1)?0x2a1ea6:0x2a1e56);SL(0,0x9f9a6);
        /* fall through */
    case 0x9f9a6:
        ka0576(a,e);ka0334(a,e);STEP(r);choose(a,e,true);
        return tail_d0e(a,e);
    case 0x9f9fa:
        CHG(0x90,0);state(a,BIT(0x90,5)?6:5);SL(0,0x9fa28);
        /* fall through */
    case 0x9fa28:
        ka0576(a,e);
        if (!ka0356(a,e)) return GA_UNSUPPORTED;
        STEP(r);
        if (B(0x61)) SL(0,0x9f846);
        return tail_d0e(a,e);
    case 0x9fa48:
        if (!BIT(0x90,1)) {state(a,BIT(0x90,5)?0x0a:9);START(0x2a1ef6);}
        else {state(a,BIT(0x90,5)?8:7);START(0x2a1f34);}
        SL(0,0x9fac0);
        /* fall through */
    case 0x9fac0:
        ka0334(a,e);STEP(r);
        if (r) {CHG(0x90,1);SL(0,0x9f846);}
        return tail_d3a(a,e);
    case 0x9fade:
        sound(e,0x108e);state(a,0x0d);SET(0x90,5);
        START(BIT(0x90,1)?0x2a1ea6:0x2a1e56);SL(0,0x9fb2c);
        /* fall through */
    case 0x9fb2c:
        ka0334(a,e);STEP(r);
        if (B(0x61)) SL(0,0x9f846);
        return tail_d3a(a,e);
    case 0x9fb48:
        if (BIT(0x90,7)) {
            if (!W(0x86)) {pc=0x9fc9c;continue;}
            if (S16(W(0x86))>0) {SW(0x86,W(0x86)-1u);if (!W(0x86)) {pc=0x9fc9c;continue;}}
        }
        state(a,1);START(BIT(0x90,1)?0x2a1ea6:0x2a1e56);SL(0,0x9fba6);
        /* fall through */
    case 0x9fba6:
        ka0334(a,e);STEP(r);
        if (B(0x61)) SL(0,0x9fbc2);
        return tail_d0e(a,e);
    case 0x9fbc2:
        SB(0x87,rank(a,e,0x2d3242));
        /* fall through */
    case 0x9fbd2:
        SW(0x7e,rank(a,e,0x2d31e0));
        state(a,BIT(0x87,0)?3:2);SL(0,0x9fc0a);
        /* fall through */
    case 0x9fc0a:
        ka0334(a,e);STEP(r);
        if ((int8_t)B(0x87)>1) {
            SW(0x7e,W(0x7e)-1u);
            if (S16(W(0x7e))<=1) {SB(0x87,B(0x87)-1u);SL(0,0x9fbd2);}
        } else if (B(0x61)) SL(0,0x9fc4c);
        return tail_d0e(a,e);
    case 0x9fc4c:
        SW(0x7e,rank(a,e,0x2d317e));state(a,4);SL(0,0x9fc6c);
        /* fall through */
    case 0x9fc6c:
        ka0334(a,e);STEP(r);
        if (B(0x61)) SL(0,BIT(0x90,1)?0x9fa48:0x9f846);
        return tail_d0e(a,e);
    case 0x9fc9c:
        state(a,0);
        /* fall through */
    case 0x9fca6:
        SW(0x66,0x8820);SW(0x1e,0x8820);
        SB(0x69,0xff);                                                  /* 019DFC */
        SL(0x62,FACING?0x60000u:0xfffa0000u);
        START(0x2a1f72);SL(0,0x9fce4);
        /* fall through */
    case 0x9fce4:
        if (BIT(0x5f,7)) {state(a,0xff);SL(0,0x9fcfe);}
        /* fall through */
    case 0x9fcfe:
        ground(a,e);STEP(r);
        return tail_d4a(a,e);
    default: ga_fail(e,pc);return GA_UNSUPPORTED;
    }
    ga_fail(e,pc);return GA_UNSUPPORTED;
}
/* ---- rider ------------------------------------------------------------------ */
/* 0A098C: the rider's program for the camel state +8B. */
static bool ka098c(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t t=rl((BIT(0x90,1)?0xa0bbau:0xa0b82u)+4u*(B(0x8b)&0x0fu));
    return ga_anim_start(a,e,t,0x100);
}
/* 0A07D0: follow the camel's trot offsets, layer, flags and state. */
static bool ka07d0(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);
    if (!c) {ga_fail(e,0xa07d0);return false;}
    int16_t d0=(int8_t)ga_b(c,0x88),d1=(int8_t)ga_b(c,0x89);
    if (ga_b(c,0x15)&1u) d0=(int16_t)-d0;
    SW(0x28,(uint16_t)d0);SW(0x2c,(uint16_t)d1);
    ga_project(a,e);
    SW(0x1e,ga_w(c,0x1e));SB(0x90,ga_b(c,0x90));
    uint32_t box;
    if (!BIT(0x90,2)) {SW(0x1e,W(0x1e)-1u);box=0xa0aea;} else box=0xa0af6;
    uint8_t s=ga_b(c,0x8b);
    if ((s&0x80u) || s==0x0du || s==0x0bu || s==0x0cu) box=0;
    SL(0x50,box);
    if (s!=B(0x8b)) {SB(0x8b,s);if (!ka098c(a,e)) return false;}
    return true;
}
/* 09F562 / 09F58C .. 09F5F8: the rider dies as a swordsman. */
static void rider_death(neo_gactor_t *a,neo_genv_t *e,bool sword) {
    if (sword) ga_spawn_from(a,e,0x9ee34,0,0x10);                     /* 09F562 */
    else SET(0x96,0);                                                  /* 09F58C */
    SB(0x1d,0);                                                        /* 009F88 */
    ga_palette(a,neo_blood_palette(NEO_BLOOD_CAMEL_RIDER,e->blood));   /* 09F598 */
    if (B(0x5b)&0x80u) SET(0x15,0); else CLR(0x15,0);
    SL(0,(ga_rand(e)&1u)?0x9eab8:0x9e920);
}
static ga_result_t rider_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x9fdfe: {
        neo_gactor_t *c=creator(a,e);
        ga_place(a,e,0,0,c?NEO_GA_TAG(c->index):0);                    /* 01AD6E */
        uint32_t n;
        if (!nearest(a,e,&n) && c) {
            if (px(e,n)<W(0x3c)) {CLR(0x15,0);ga_sb(c,0x90,ga_b(c,0x90)&~1u);}
            else {SET(0x15,0);ga_sb(c,0x90,ga_b(c,0x90)|1u);}
        }
        ga_spawn_from(a,e,0x9ff26,0,0);
        SW(0x4e,1);SW(0x66,0x8820);SW(0x1e,0x8820);
        ga_palette(a,0x3fe);ga_palette(a,0x118);SB(0x14,B(0x19));
        START(0x2a21c4);SL(0,0x9feaa);
    }   /* fall through */
    case 0x9feaa: {
        if (!ka07d0(a,e)) return GA_UNSUPPORTED;
        if (!(B(0x8b)&0x80u)) {
            STEP(r);
            if (r) {neo_gactor_t *c=creator(a,e);if (c) ga_sb(c,0x61,0xff);}
            if (ga_register(a,e,NULL,NULL) && !BIT(0x5f,7)) SB(0x5a,0);
        }
        neo_gactor_t *c=creator(a,e);
        if (c && (ga_b(c,0x5f)&0x80u)) SL(0,0x9fd60);
        if (BIT(0x5f,7)) {
            if (c) ga_sb(c,0x5f,ga_b(c,0x5f)|0x80u);
            rider_death(a,e,!BIT(0x90,5));
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x9fd60:
        SET(0x5f,7);
        return GA_DELETE;                                               /* 004188 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- head ------------------------------------------------------------------- */
static void head_program(neo_gactor_t *a,neo_genv_t *e,uint32_t near,uint32_t far) {
    if (!ga_anim_start(a,e,BIT(0x90,1)?far:near,0x100)) ga_fail(e,near);
}
/* 0A0866: follow the rider; programs on its flag changes; hurtbox. */
static void ka0866(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    neo_gactor_t *c=creator(a,e);
    if (!c) {ga_fail(e,0xa0866);return;}
    if (ga_b(c,0x15)&1u) SET(0x15,0); else CLR(0x15,0);
    SW(0x1e,ga_w(c,0x1e)-1u);
    uint8_t d0=(uint8_t)(ga_b(c,0x90)^B(0x90));
    SB(0x90,ga_b(c,0x90));SB(0x8f,ga_b(c,0x8f));
    if (d0&0x20u) head_program(a,e,0x2a25a0,0x2a25ce);
    else if (d0&6u) {                                                  /* 0A0920 */
        if (!BIT(0x90,5)) head_program(a,e,0x2a24ec,0x2a251e);
        else head_program(a,e,0x2a25a0,0x2a25ce);
    }
    SL(0x50,(!BIT(0x90,5) && B(0x8f))?0xa0b02u:0u);
}
static ga_result_t head_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x9ff26: {
        neo_gactor_t *c=creator(a,e);
        ga_place(a,e,0,0,c?NEO_GA_TAG(c->index):0);                    /* 01AD6E */
        SW(0x4e,rank(a,e,0x2d303e));SW(0x66,0x8820);SW(0x1e,0x8820);
        ga_palette(a,0x3fe);
        START(BIT(0x90,1)?0x2a251e:0x2a24ec);SB(0x60,0);SL(0,0x9ff92);
    }   /* fall through */
    case 0x9ff92: {
        ka0866(a,e);
        if (BIT(0x90,3)) STEP(r);
        if (ga_register(a,e,NULL,NULL)) {
            if (!BIT(0x90,3)) SB(0x5a,0);
            else if (!BIT(0x90,5)) {
                SB(0x5a,0);
                if (!B(0x60)) START(BIT(0x90,1)?0x2a2578:0x2a2550);
            }
            if (BIT(0x90,3) && BIT(0x5f,7)) {
                neo_gactor_t *c=creator(a,e),*cc=c?creator(c,e):NULL;
                if (cc) ga_sb(cc,0x90,ga_b(cc,0x90)|0x10u);
            }
        }
        neo_gactor_t *c=creator(a,e);
        if (c && (ga_b(c,0x5f)&0x80u)) SL(0,0x9fd60);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x9fd60:
        SET(0x5f,7);
        return GA_DELETE;                                               /* 004188 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- shot, pieces, case ---------------------------------------------------- */
static ga_result_t shot_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0xa0034: {
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                               /* 01B1DC */
        uint32_t d0=(uint32_t)(int32_t)S16(rank(a,e,0x2d30ba))<<8;
        if (!FACING) d0=0u-d0;
        SL(0x30,d0);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        ga_palette(a,0x22);START(0x2a263e);SL(0,0xa0090);
    }   /* fall through */
    case 0xa0090: {
        ga_air(a,e);
        if (BIT(0x15,7)) SL(0,0xa00e8);
        STEP(r);
        if (ga_attack(a,e,0xa0b0e)) SL(0,0xa00e8);
        if (ga_cull_rect(a,e,0xa0b26)) SL(0,0x9fdf6);
        neo_gactor_t *c=creator(a,e);
        if (c && (ga_b(c,0x5f)&0x80u)) return GA_DELETE_TREE;          /* bne 09FDF6 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0xa00e8: {
        SB(0x1d,0);                                                     /* 009F88 */
        ga_palette(a,0x202);
        uint16_t d0=ga_rand(e);
        SW(0x28,W(0x28)+(uint16_t)((d0&0x0fu)-7u));
        SW(0x2c,W(0x2c)+(uint16_t)(((d0>>8)&0x0fu)-7u));
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);START(0x22e504);SL(0,0xa013c);
    }   /* fall through */
    case 0xa013c:
        ga_project(a,e);STEP(r);
        if (r) return GA_DELETE_TREE;                                   /* 09FDF6 */
        if (W(0x3c)>=0x140u) return GA_DELETE_TREE;
        if ((uint16_t)(W(0x40)-0x100u)>=0x100u) return GA_DELETE_TREE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9fdf6: return GA_DELETE_TREE;                                /* 0041AC */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static ga_result_t piece_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint32_t table=0;
    switch (L(0)) {
    case 0xa016a:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_palette(a,0x117);START(0x2a2658);
        table=0xa0b2eu+12u*(B(0x8e)&3u);
        goto a1a4;
    case 0xa020a:
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x118);START(0x2a26fe);
        table=0xa0b5e;
    a1a4: {
        SL(0x34,rl(table));SL(0x76,rl(table+4u));
        uint32_t d0=rl(table+8u);
        if (!FACING) d0=0u-d0;
        SL(0x30,d0);SET(0x15,5);SL(0,0xa01d0);
    }   /* fall through */
    case 0xa01d0:
        SL(0x34,L(0x34)+L(0x76));ga_air(a,e);STEP(r);
        if (BIT(0x15,6)) SL(0,0x9fdf6);
        if (ga_cull_rect(a,e,0xa0b26)) SL(0,0x9fdf6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9fdf6: return GA_DELETE_TREE;                                /* 0041AC */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- program callbacks ------------------------------------------------------ */
static bool cm_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0xa0774: ka0774(a);return true;
    case 0xa0ab6: SET(0x90,2);return true;
    case 0xa0abe: CLR(0x90,2);return true;
    case 0xa0ac6: case 0xa0ad8: {
        neo_gactor_t *c=creator(a,e);
        uint8_t m=(uint8_t)(1u<<3);
        if (routine==0xa0ac6) {if (c) ga_sb(c,0x90,ga_b(c,0x90)&~m);CLR(0x90,3);}
        else {if (c) ga_sb(c,0x90,ga_b(c,0x90)|m);SET(0x90,3);}
        return true;
    }
    case 0xa09d0: case 0xa0a02:
        sound(e,0x10ff);
        ga_spawn_from(a,e,0xa0034,routine==0xa09d0?-0x1b:-0x20,0x1f);
        return true;
    case 0xa0a34: ga_spawn_from(a,e,0xa020a,-0x14,0x20);return true;
    case 0xa0a5c: {
        neo_gactor_t *c=ga_spawn_from(a,e,0xa020a,-0x14,0x20);
        if (c) ga_sb(c,0x15,ga_b(c,0x15)^2u);
        return true;
    }
    case 0xa0a68: {
        uint16_t d0=ga_rand(e);
        SB(0x8e,d0);
        for (unsigned n=(d0&1u)+3u;n>0;n--) {
            neo_gactor_t *c=ga_spawn_from(a,e,0xa016a,8,0x30);
            if (c) ga_sb(c,0x8e,B(0x8e));
            SB(0x8e,B(0x8e)+1u);
        }
        return true;
    }
    }
    return false;
}
static bool camel(uint32_t r) {
    static const uint32_t own[]={0x9f682,0x9f6ca,0x9f730,0x9f752,0x9f76a,0x9f782,0x9f7c4,0x9f7f4,0x9f82a,0x9f846,0x9f878,
        0x9f90a,0x9f940,0x9f968,0x9f9a6,0x9f9fa,0x9fa28,0x9fa48,0x9fac0,0x9fade,0x9fb2c,0x9fb48,0x9fba6,0x9fbc2,0x9fbd2,
        0x9fc0a,0x9fc4c,0x9fc6c,0x9fc9c,0x9fca6,0x9fce4,0x9fcfe};
    for (unsigned i=0;i<sizeof(own)/sizeof(own[0]);i++) if (own[i]==r) return true;
    return false;
}
/* Which family member a node is: its routine, or for 09FD60 / 09FDF6 (shared
 * tails) the member that set it (+50 / +7A tell them apart is not needed:
 * the tails are identical). */
bool neo_camel_routine(uint32_t r) {
    return camel(r) || r==0x9fd60 || r==0x9fdfe || r==0x9feaa || r==0x9ff26 || r==0x9ff92 || r==0xa0034 || r==0xa0090 ||
        r==0xa00e8 || r==0xa013c || r==0x9fdf6 || r==0xa016a || r==0xa020a || r==0xa01d0;
}
ga_result_t neo_camel_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (camel(r)) return camel_tick(a,e);
    if (r==0x9fd60) {SET(0x5f,7);return GA_DELETE;}                    /* 004188 */
    if (r==0x9fdfe || r==0x9feaa) return rider_tick(a,e);
    if (r==0x9ff26 || r==0x9ff92) return head_tick(a,e);
    if (r==0xa016a || r==0xa020a || r==0xa01d0) return piece_tick(a,e);
    return shot_tick(a,e);
}
