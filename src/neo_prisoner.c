#include "neo_region.h"
/* Prisoner of the rest of Mission 1 (spawn-table entry 099862, camera
 * 2336..), manual translation; ROM labels are provenance only.
 *
 * 099862 sits tied (program 29F6D0: +80 = wriggle index, callback 099BE6
 * walks +62 and spends the budget +76) until it is hit (004C24 carry: 099922)
 * or a player touches it (099C3C: rectangle 099D7E, 09AB38 counts the rescue
 * in RAM 10C5FC/10C600). Released (099922/099956) it wriggles by the table
 * 099D86 and every +7A updates (rank table 2D0BB8) gives one of its +72 items
 * (099CEE -> 099D12 -> 08CDB2: item 08C374 after the script actor, kind from
 * the list 2D0BEC by +70 and the RNG); touched, it salutes (0999C2: program
 * 29F9AC, whose callback 099CCC throws the remaining gift 099C9C), jumps
 * (29FA22/29FA40, air 0143E4) and runs (099A8A: speed 2D0B9E, program 29FA78)
 * until it hits a wall (+5F bit 0: +7A = 30 updates more) or leaves the view
 * (099AF8: rectangles 099D76/099D6E). With its budget +76 spent while tied
 * (099A22) it throws one item and jumps. Every path that would reach the
 * cartridge check (2FFFE0) ends in 004188 (delete) on genuine hardware: that
 * is what is translated. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static bool prisoner_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void gravity_air(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}   /* 0143DC */
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 099BB2: stand (01436A), then the floor step or gravity. */
static void k99bb2(neo_gactor_t *a,neo_genv_t *e) {
    ga_stationary(a,e);
    if (B(0x15)&0x40u) {uint32_t kind;ga_ground(a,e,&kind);}
    else {gravity_air(a,e);if (B(0x15)&0x40u) {SL(0x30,0);SL(0x34,0);}}
}
/* 099C26: +62 towards the facing. */
static void k99c26(neo_gactor_t *a) {uint32_t d0=L(0x62);if (BIT(0x15,0)) d0=0u-d0;SL(0x62,d0);}
/* 099BE6: wriggle step on the floor, spending the budget +76. */
static void k99be6(neo_gactor_t *a,neo_genv_t *e) {
    if (!(B(0x15)&0x40u)) return;
    k99c26(a);
    uint32_t kind;ga_ground(a,e,&kind);
    uint32_t d0=L(0x62);if (d0&0x80000000u) d0=0u-d0;
    SL(0x76,L(0x76)-d0);
    if (L(0x76)&0x80000000u) SL(0x76,0);
    SL(0x62,0);SL(0x30,0);SL(0x34,0);
}
/* 099D12 -> 08CDB2: one item 08C374 after the script actor (kind from the
 * list [2D0BEC + 4*+70] by the RNG; kinds >= 0x5A give none: the original
 * returns the dummy 101624). */
static neo_gactor_t *k99d12(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t r=ga_rand(e)&7u;
    uint32_t list=rl(0x2d0becu+(uint32_t)(int32_t)(int16_t)(uint16_t)(W(0x70)<<2));
    const uint8_t *p=ga_rom(list+r);
    if (!p) {ga_fail(e,list+r);return NULL;}
    uint16_t d2=p[0]&0x7fu;
    int16_t d0=0xc,d1=S16(W(0x7e));
    uint16_t d3=W(0x7c);
    if (!BIT(0x15,0)) d0=(int16_t)-d0;
    if (d2>=0x5au) return NULL;
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
    if (!c) return NULL;
    ga_sb(c,0x8b,0);ga_sw(c,0x70,d3);ga_sw(c,0x6e,d2);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
    ga_place_at(c,a,e,d0,d1);                                       /* 01ACF2 */
    ga_sb(c,0x12,0x18);ga_sb(c,0x13,0x18);
    return c;
}
/* 099C9C: the item thrown upwards and forwards by +72. */
static void k99c9c(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=k99d12(a,e);
    uint32_t d0=(uint32_t)(W(0x72)&7u)<<15;
    if (c) ga_sl(c,0x34,0x30000u+d0);
    if (!BIT(0x15,0)) d0=0u-d0;
    if (c) ga_sl(c,0x30,d0);
}
/* 099CCC: the remaining gift (one item) at the end of the salute. */
static void k99ccc(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x72))<=0) return;
    SW(0x7e,8);SW(0x72,1);k99c9c(a,e);SW(0x72,0);
}
/* 099CEE: one more item while +72 lasts; carry (true) when none is left. */
static bool k99cee(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x7e,8);
    if (S16(W(0x72))<=0) return true;
    (void)k99d12(a,e);SW(0x72,W(0x72)-1u);
    return false;
}
/* 01AF3C against player p: carry (true) when outside the rectangle. */
static bool outside(neo_gactor_t *a,neo_genv_t *e,unsigned p,uint32_t rect) {
    int16_t px=(int16_t)(p?e->players.p2x:e->players.p1x),py=(int16_t)(p?e->players.p2y:e->players.p1y);
    int16_t d1=(int16_t)(py-S16(W(0x40))),d2=S16(rw(rect+2)),d3=(int16_t)(d2+S16(rw(rect+6)));
    if (BIT(0x15,1)) d1=(int16_t)-d1;
    if (d1<d2 || d1>d3) return true;
    d1=(int16_t)(px-S16(W(0x3c)));d2=S16(rw(rect));d3=(int16_t)(d2+S16(rw(rect+4)));
    if (BIT(0x15,0)) d1=(int16_t)-d1;
    return d1<d2 || d1>d3;
}
/* 099C3C: carry when touched (+5F bit 0) or a player is in 099D7E
 * (09AB38: rescue counted for that player). */
static bool k99c3c(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x5f,0)) return true;
    for (unsigned p=0;p<2;p++) {
        if (!(e->players.active_mask&(1u<<p))) continue;              /* 01AEB0 */
        if (outside(a,e,p,0x99d7e)) continue;
        if (e->flags && e->flags->rescued[p]<99u) e->flags->rescued[p]++; /* 09AB38 */
        return true;
    }
    return false;
}
/* 01AF9A: the nearest live player's X (one player in the port's world:
 * player 1); false when none. */
static bool nearest_x(neo_genv_t *e,int16_t *x) {
    unsigned m=e->players.active_mask&3u;
    if (!m) return false;
    if (m==3u) {
        /* Both alive: the nearer one (not reached: the port has one player). */
        ga_fail(e,0x1af9a);return false;
    }
    *x=(int16_t)(m&1u?e->players.p1x:e->players.p2x);
    return true;
}
/* 099AF8: leave outside the rectangles 099D76 (019504) / 099D6E (01945C). */
static ga_result_t k99af8(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull_rect(a,e,0x99d76)) SL(0,0x99b22);
    if (ga_cull(a,e,0x99d6e)) SL(0,0x99b22);
    return GA_MORE;
}
static int step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,prisoner_call);}
static ga_result_t prisoner_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;int16_t px;
    for (;;) switch (L(0)) {
    case 0x99862:
        if (W(0x6e)) {SET(0x15,0);SL(0x62,0xffff8000u);} else {CLR(0x15,0);SL(0x62,0x8000);}
        SL(0x76,(uint32_t)W(0x74)<<16);
        ga_floor(a,e);
        SL(0x50,0x99d62);SW(0x66,0x8820);SW(0x1e,0x8820);
        ga_palette(a,0x9b);
        SW(0x7c,rank(a,e,0x2d0bd2));
        SL(0x62,0);
        START(0x29f6d0);
        SL(0,0x998e8);
        /* fall through */
    case 0x998e8:
        k99bb2(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) SL(0,0x99922);
        if (k99c3c(a,e)) SL(0,0x999c2);
        if (!L(0x76)) SL(0,0x99a22);
        return k99af8(a,e);
    case 0x99922:
        SW(0x7a,rank(a,e,0x2d0bb8));
        START(rl(0x99d86u+4u*(B(0x80)&0x1fu)));
        SL(0,0x99956);
        /* fall through */
    case 0x99956:
        k99bb2(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        SW(0x7a,W(0x7a)-1u);
        if (S16(W(0x7a))<=0) {
            SW(0x7a,rank(a,e,0x2d0bb8));
            if (k99cee(a,e)) SL(0,0x999ac);
        }
        if (k99c3c(a,e)) SL(0,0x999c2);
        if (!L(0x76)) SL(0,0x99a22);
        return k99af8(a,e);
    case 0x999ac:
        if (nearest_x(e,&px)) {if ((int16_t)(px-S16(W(0x3c)))<=0) SET(0x15,0); else CLR(0x15,0);}
        else if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x999dc);continue;
    case 0x999c2:
        if (S16(W(0x3c))<=0xa0) SET(0x15,0); else CLR(0x15,0);
        /* fall through */
    case 0x999dc:
        SL(0x62,0);START(0x29f9ac);SL(0,0x999f6);
        /* fall through */
    case 0x999f6:
        k99bb2(a,e);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x99a0e);
        return k99af8(a,e);
    case 0x99a0e:
        START(0x29fa22);
        goto jump;
    case 0x99a22:
        SW(0x7e,0x20);SW(0x72,1);k99c9c(a,e);
        START(0x29fa40);
    jump:
        /* 099A42: faces the nearest player (01AF9A without a carry test). */
        if (!nearest_x(e,&px)) return GA_UNSUPPORTED;
        if ((int16_t)(px-S16(W(0x3c)))<=0) SET(0x15,0); else CLR(0x15,0);
        k1b1dc(a);
        SL(0,0x99a70);
        /* fall through */
    case 0x99a70:
        ga_air(a,e);
        if ((r=step(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x99a8a);
        return k99af8(a,e);
    case 0x99a8a: {
        uint32_t d0=(uint32_t)(int32_t)S16(rank(a,e,0x2d0b9e))<<8;
        if (!BIT(0x15,0)) d0=0u-d0;
        SL(0x62,d0);SW(0x7a,0x1e);START(0x29fa78);SL(0,0x99ac6);
    }   /* fall through */
    case 0x99ac6:
        k99bb2(a,e);
        if (step(a,e)<0) return GA_UNSUPPORTED;
        if (BIT(0x5f,0)) {
            SET(0x5f,3);SW(0x7a,W(0x7a)-1u);
            if (S16(W(0x7a))<=0) SL(0,0x99b22);
        }
        return k99af8(a,e);
    case 0x99b22:
        return GA_DELETE;                                           /* 004188 */
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool prisoner_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x99be6u) {k99be6(a,e);return true;}
    if (routine==0x99c26u) {k99c26(a);return true;}
    if (routine==0x99ccc) {k99ccc(a,e);return true;}
    return false;
}
bool neo_prisoner_routine(uint32_t r) {
    return r==0x99862 || r==0x998e8 || r==0x99922 || r==0x99956 || r==0x999ac || r==0x999c2 || r==0x999dc ||
        r==0x999f6 || r==0x99a0e || r==0x99a22 || r==0x99a70 || r==0x99a8a || r==0x99ac6 || r==0x99b22;
}
ga_result_t neo_prisoner_tick(neo_gactor_t *a,neo_genv_t *e) {return prisoner_tick(a,e);}
