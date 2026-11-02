#include <string.h>
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
        if (e->flags) {                                                 /* 09AB38 */
            if (e->flags->rescued[p]<99u) e->flags->rescued[p]++;
            e->flags->special[p]++;
        }
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
static bool prisoner2_routine(uint32_t r) {
    static const uint32_t list[]={0x99dcc,0x99de2,0x99e1c,0x99e48,0x99e64,0x99e7a,0x99e06,0x99f64,0x99f80,0x99f9c,0x99fd8,0x9a000,0x9a042,
        0x9a06e,0x9a0f6,0x9a11c,0x9a142,0x9a160,0x9a17e,0x9a1fc,0x9a230,0x9a278,0x9a290,0x9a2d8,0x9a322,0x9a346,0x9a364,
        0x9a376,0x9a396,0x9a3b2,0x9a3d4,0x9a3f2,0x9a438,0x9a454,0x9a46e,0x9a4a2,0x9a4d2,0x9a4fa,0x9a516,0x9a534,0x9a552,
        0x9a58e,0x9a5aa,0x9a97c,0x9a9ac,0x9a5ee};
    for (unsigned i=0;i<sizeof(list)/sizeof(list[0]);i++) if (list[i]==r) return true;
    return false;
}
/* ---- Prisoners 099DCC/099E06 (spawn-table entries of the whole mission,
 * 099DCC..09A5EE; the boss's hatch also drops three 099DCC) -------------
 * Constructor 09AC16: +72 faces left, +6E = 3 is drawn by 0029F2(3), +78
 * puts it behind (+15 bit 5), 01436A, palettes 0x382/0x3FA, depth words
 * 8820 (03C526 asks 019240 for a mission bonus: RAM 10C2F6 = 0 in Mission
 * 1, nothing is created). 099DCC hangs tied (29FAD8) until hit (09AD2A ->
 * 004C24; +88 counts the flinch of 09AD1C), struggles free (29FB34) and is
 * released (099FD8); 099E06 sits tied on a chair (29FCBE), hit it falls
 * (29FDE6) and leaves its rope 09A97C. Released (099FD8 -> 09A042): palettes
 * 0x381/0x3FA, hurtbox 09AD4C, wanders (29FE52, +62 = 1.5 px) for +7A
 * updates (+6E = 0: 15; else 7FFF facing by +6E) and stops (29FED0) at a
 * wall (+5F bit 0) or when the count runs out; falls (29FBEC) off ledges;
 * at an object-list marker (040200) climbs (bit 0: 29FFE4/29FFFA, 0402E6,
 * then jumps off 2A0074/2A011C) or goes down (bit 1: 2A0128/2A01D0/2A024A).
 * Touched by a player (09AB9A: rectangle 09ADE0) it is rescued (09AAD6),
 * turns (29FE52 for 4 or 5 updates), salutes (29FEEC, whose callback 09ACD6
 * gives the item +74 (life +76) through 08CDB2 placed by 014A02), waves
 * (2A02B6) and runs off left (2A0260) until a wall (+5F bit 0), 30 more
 * updates. Outside the view (0C8BEC: 09ADD0) it ends (09A5EE: the cartridge
 * check, then 004188 on genuine hardware: delete). */
static uint16_t k9ac86_ground_or_air(neo_gactor_t *a,neo_genv_t *e) {   /* 09AC86 */
    if (B(0x15)&0x40u) {uint32_t kind;ga_ground(a,e,&kind);return 0;}
    gravity_air(a,e);
    if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));          /* 01AD8C */
    if (B(0x15)&0x40u) {SL(0x30,0);SL(0x34,0);}
    return 0;
}
static void k9ac16(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x72)) SET(0x15,0);
    if (W(0x6e)==3u) SW(0x6e,ga_rand_range(e,3));
    if (W(0x78)) SET(0x15,5);
    ga_stationary(a,e);
    ga_palette(a,0x382);ga_palette(a,0x3fa);SB(0x14,B(0x19));
    SW(0x66,0x8820);SW(0x1e,0x8820);
    /* 03C526 -> 019240: the ripple 03C62A (none in Mission 1, RAM 10C2F6 = 0). */
    neo_gate_ripple(a,e,1);
}
/* 09AD2A: the flinch count +88 (09AD1C) runs down instead of registering;
 * otherwise 004C24 (carry: hit). */
static bool k9ad2a(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x88)) {SW(0x88,W(0x88)-1u);if (!W(0x88)) CLR(0x15,5);return false;}
    return ga_register(a,e,NULL,NULL);
}
/* 09AB9A: a live player inside 09ADE0 (09AB7E: +5F bit 7 (dying) clear,
 * the port's live player; 01AF3C). RAM
 * 10C303 (018F16) reads FF in every recorded update (91 in fam2): touching
 * is enabled. Both players inside: the nearer by X (not reached, one
 * player in the port). */
static int k9ab9a(neo_gactor_t *a,neo_genv_t *e) {
    int found=-1;
    for (unsigned p=0;p<2;p++) {
        if (!(e->players.active_mask&(1u<<p))) continue;              /* 01AEB0 */
        if (outside(a,e,p,0x9ade0)) continue;
        if (found>=0) {ga_fail(e,0x9abd6);return -1;}
        found=(int)p;
    }
    return found;
}
/* 09AAD6: the rescue (RAM 10C301 (018F24) reads 0 in the traces). */
static void k9aad6(neo_gactor_t *a,neo_genv_t *e,unsigned p) {
    if (p>1u || !(e->players.active_mask&(1u<<p)) || !e->flags) return; /* 01AEB0 */
    e->flags->tally[p]++;                                            /* 0C8CC2 */
    e->flags->rescued[p]++;if (e->flags->rescued[p]>=100u) e->flags->rescued[p]=99u;
    if (W(0x74)==0xfffeu) e->flags->gifts[p]++;
}
/* 09ACD6 (program callback): the item +74 (life +76) after the script
 * actor (08CDB2), then moved 16 px forward and 8 down by 014A02. */
static void k9acd6(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x74)==0xffffu) return;
    uint16_t kind=W(0x74);
    neo_gactor_t *c=NULL;
    static neo_gactor_t dummy;                     /* 101624: kinds >= 0x5A */
    if (kind<0x5au) {
        c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
        if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x76));ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,0);}
    }
    if (!c) {memset(&dummy,0,sizeof(dummy));c=&dummy;}
    ga_sweep_place(a,e,c,(int16_t)(BIT(0x15,0)?0x10:-0x10),8);
    SW(0x74,0xffff);
}
/* 0C8BEC against 09ADD0 (the routine then reaches 09A5EE). */
static ga_result_t k9a5d8(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_outside_view(a,e,0x9add0)) SL(0,0x9a5ee);
    return GA_MORE;
}
/* 09A5C2: touched -> rescued (09A17E); then 09A5D8. */
static ga_result_t k9a5c2(neo_gactor_t *a,neo_genv_t *e) {
    int p=k9ab9a(a,e);
    if (e->unsupported) return GA_UNSUPPORTED;
    if (p>=0) {SB(0x83,(uint8_t)p);SL(0,0x9a17e);}
    return k9a5d8(a,e);
}
static int step2(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,prisoner_call);}
#define STEP2(r) do { if (((r)=step2(a,e))<0) return GA_UNSUPPORTED; } while (0)
/* 09A230/09A290: face the rescuer (01AEB0 +83). */
static void face_rescuer(neo_gactor_t *a,neo_genv_t *e) {
    unsigned p=B(0x83)&1u;
    if (!(e->players.active_mask&(1u<<p))) return;
    int16_t px=(int16_t)(p?e->players.p2x:e->players.p1x);
    if (S16(W(0x3c))>px) CLR(0x15,0); else SET(0x15,0);
}
static ga_result_t prisoner2_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    for (;;) switch (L(0)) {
    case 0x99de2:                                    /* 099DE2: no 01B1DC, +86 bit 0 set */
    case 0x99dcc:
        if (L(0)==0x99dccu) {k1b1dc(a);k9ac16(a,e);} else {k9ac16(a,e);SB(0x86,B(0x86)|1u);}
        START(0x29fad8);SL(0,0x99e48);
        /* fall through */
    case 0x99e48:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (k9ad2a(a,e)) SL(0,0x99e64);
        return k9a5d8(a,e);
    case 0x99e64:
        START(0x29fb34);SL(0,0x99e7a);
        /* fall through */
    case 0x99e7a:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (r) SL(0,0x99fd8);
        return k9a5d8(a,e);
    case 0x99e06:
        k1b1dc(a);k9ac16(a,e);
        START(0x29fcbe);SL(0,0x99f64);
        /* fall through */
    case 0x99f64:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (k9ad2a(a,e)) SL(0,0x99f80);
        return k9a5d8(a,e);
    case 0x99f80:
        k1b1dc(a);START(0x29fde6);SL(0,0x99f9c);
        /* fall through */
    case 0x99f9c:
        ga_air(a,e);STEP2(r);
        if (r) {
            /* 0040BC: the rope 09A97C where the chair was (01B220). */
            (void)ga_spawn_from(a,e,0x9a97c,0,0);
            SL(0,0x99fd8);
        }
        return k9a5d8(a,e);
    case 0x99e1c:                                    /* 09A842's prisoner: released at once */
        k1b1dc(a);k9ac16(a,e);SL(0,0x99fd8);
        /* fall through */
    case 0x99fd8:
        SB(0x1d,0);ga_palette(a,0x381);ga_palette(a,0x3fa);            /* 009F88, 009EA8 */
        SB(0x14,B(0x19));
        /* fall through */
    case 0x9a000:
        SL(0x50,0x9ad4c);SB(0x5a,0);CLR(0x5f,7);
        if (!W(0x6e)) SW(0x7a,0xf);
        else {if ((uint8_t)W(0x6e)==1u) CLR(0x15,0); else SET(0x15,0);SW(0x7a,0x7fff);}
        /* fall through */
    case 0x9a042:
        START(0x29fe52);
        SL(0x62,BIT(0x15,0)?0x18000u:(uint32_t)-0x18000);
        SL(0,0x9a06e);
        /* fall through */
    case 0x9a06e: {
        if (BIT(0x86,0)) ga_stationary(a,e);
        else {
            k9ac86_ground_or_air(a,e);
            if (!(B(0x15)&0x40u)) SL(0,0x9a0f6);
            else {
                if (BIT(0x5f,0)) SL(0,0x9a142);
                if (!B(0x6e)) {uint16_t t=W(0x7a);SW(0x7a,t-1u);if (!t) SL(0,0x9a142);}
            }
        }
        uint8_t flag=0;int id=ga_marker_cross(a,e,&flag);             /* 040200 */
        if (e->unsupported) return GA_UNSUPPORTED;
        if (id>=0) {
            SB(0x84,(uint8_t)id);
            if (flag&1u) SL(0,0x9a396);
            if (flag&2u) SL(0,0x9a4fa);
        }
        STEP2(r);
        return k9a5c2(a,e);
    }
    case 0x9a0f6:
        SL(0x50,0x9ad4c);START(0x29fbec);SL(0x30,0);SL(0x34,0);SL(0,0x9a11c);
        /* fall through */
    case 0x9a11c:
        gravity_air(a,e);
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));       /* 01AD8C */
        if (B(0x15)&0x40u) SL(0,0x9a042);
        STEP2(r);
        return k9a5c2(a,e);
    case 0x9a142:
        SL(0x62,0);SL(0x30,0);START(0x29fed0);SL(0,0x9a160);
        /* fall through */
    case 0x9a160:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (r) {SW(0x7a,0x1e);SL(0,0x9a042);}
        return k9a5c2(a,e);
    case 0x9a17e:
        if (e->sound) e->sound(e->ctx,0x113a);                         /* 0120F8 */
        k9aad6(a,e,B(0x83));
        SL(0x50,0);START(0x29fe52);
        if (BIT(0x86,0)) {SET(0x15,0);SL(0x62,0xfff00000u);SW(0x7a,5);}
        else {
            if (S16(W(0x3c))>0xc0) {SET(0x15,0);SL(0x62,0xfff90000u);} else {CLR(0x15,0);SL(0x62,0x70000u);}
            SW(0x7a,4);
        }
        SL(0,0x9a1fc);
        /* fall through */
    case 0x9a1fc:
        k9ac86_ground_or_air(a,e);STEP2(r);
        SW(0x7a,W(0x7a)-1u);
        if (!W(0x7a)) {SB(0x15,B(0x15)^1u);SL(0,S16(W(0x74))<0?0x9a290:0x9a230);}
        return k9a5d8(a,e);
    case 0x9a230:
        face_rescuer(a,e);START(0x29feec);SL(0x30,0);SL(0x62,0);SL(0,0x9a278);
        /* fall through */
    case 0x9a278:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (r) SL(0,0x9a290);
        return k9a5d8(a,e);
    case 0x9a290:
        face_rescuer(a,e);START(0x2a02b6);SL(0x62,0);SL(0x30,0);SL(0,0x9a2d8);
        /* fall through */
    case 0x9a2d8:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (r) {
            SL(0,0x9a322);
            /* +74 = FFFE: the prisoner hands its gift to the rescuer
             * (0DDCDA: follows the player node), not translated. */
            if (W(0x74)==0xfffeu && (e->players.active_mask&(1u<<(B(0x83)&1u)))) {ga_fail(e,0xddcda);return GA_UNSUPPORTED;}
        }
        return k9a5d8(a,e);
    case 0x9a322:
        START(0x2a0260);CLR(0x15,0);SL(0x62,0xfffc0000u);SL(0,0x9a346);
        /* fall through */
    case 0x9a346:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (BIT(0x5f,0)) SL(0,0x9a364);
        return k9a5d8(a,e);
    case 0x9a364:
        SW(0x7a,0x1e);SET(0x5f,3);SL(0,0x9a376);
        /* fall through */
    case 0x9a376:
        k9ac86_ground_or_air(a,e);STEP2(r);
        SW(0x7a,W(0x7a)-1u);
        if (S16(W(0x7a))<=0) SL(0,0x9a5ee);
        return k9a5d8(a,e);
    case 0x9a396:
        k1b1dc(a);START(0x29ffe4);SL(0,0x9a3b2);
        /* fall through */
    case 0x9a3b2:
        SW(0x34,0);(void)ga_marker_follow(a,e,B(0x84));STEP2(r);
        if (r) SL(0,0x9a3d4);
        return k9a5c2(a,e);
    case 0x9a3d4:
        START(0x29fffa);SL(0x34,0x15500);SL(0,0x9a3f2);
        /* fall through */
    case 0x9a3f2: {
        int16_t h=ga_marker_follow(a,e,B(0x84));
        if (h<0) SL(0,0x9a000);
        else if ((uint16_t)h<0x24u) SL(0,0x9a438);
        STEP2(r);
        if (r) START(0x29fffa);
        return k9a5c2(a,e);
    }
    case 0x9a438:
        k1b1dc(a);START(0x2a0074);SL(0,0x9a454);
        /* fall through */
    case 0x9a454:
        ga_air(a,e);STEP2(r);
        if (r) SL(0,0x9a46e);
        return k9a5c2(a,e);
    case 0x9a46e:
        SB(0x82,0xc);START(0x2a011c);SL(0x34,0x4cccc);SL(0x7e,0xffff0a3eu);SL(0x30,0xfffe6667u);SL(0,0x9a4a2);
        /* fall through */
    case 0x9a4a2:
        if (L(0)==0x9a4a2) {
            uint32_t d0=L(0x30);
            if (d0) {
                d0=(uint32_t)((int32_t)d0>>4);
                uint32_t d1=d0?0u:0xffffffffu;
                d0=(d0^d1)-d1;
                SL(0x30,L(0x30)-d0);
            }
            SB(0x82,B(0x82)-1u);
            if ((int8_t)B(0x82)<=0) {SL(0x30,0);SL(0,0x9a4d2);}
        }
        /* fall through */
    case 0x9a4d2:
        SL(0x34,L(0x34)+L(0x7e));
        ga_air(a,e);STEP2(r);
        if (B(0x15)&0x40u) SL(0,0x9a000);
        return k9a5c2(a,e);
    case 0x9a4fa:
        k1b1dc(a);START(0x2a0128);SL(0,0x9a516);
        /* fall through */
    case 0x9a516:
        (void)ga_marker_follow(a,e,B(0x84));STEP2(r);
        if (r) SL(0,0x9a534);
        return k9a5c2(a,e);
    case 0x9a534:
        START(0x2a01d0);SL(0x34,0xfffeab00u);SL(0,0x9a552);
        /* fall through */
    case 0x9a552: {
        int16_t h=ga_marker_follow(a,e,B(0x84));
        STEP2(r);
        if (r) SL(0,0x9a534);
        if (h<0) SL(0,0x9a0f6);
        if (!(B(0x15)&0x40u)) SL(0,0x9a58e);
        return k9a5c2(a,e);
    }
    case 0x9a58e:
        k1b1dc(a);START(0x2a024a);SL(0,0x9a5aa);
        /* fall through */
    case 0x9a5aa:
        k9ac86_ground_or_air(a,e);STEP2(r);
        if (r) SL(0,0x9a000);
        return k9a5c2(a,e);
    /* 09A97C: the rope left by 099E06 (palette 0x382, program 29FE0A). */
    case 0x9a97c:
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0x382);START(0x29fe0a);SL(0,0x9a9ac);
        /* fall through */
    case 0x9a9ac:
        ga_project(a,e);STEP2(r);
        if (r) SL(0,0x9a5ee);
        return GA_MORE;
    case 0x9a5ee:
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
    if (routine==0x9acd6u) {k9acd6(a,e);return true;}
    if (routine==0x9acbau) {SW(0x2c,W(0x2c)+0x24u);ga_project(a,e);return true;}   /* 09ACBA */
    if (routine==0x9acc8u) {SW(0x2c,W(0x2c)-0x24u);ga_project(a,e);return true;}   /* 09ACC8 */
    return false;
}
bool neo_prisoner_routine(uint32_t r) {
    return r==0x99862 || r==0x998e8 || r==0x99922 || r==0x99956 || r==0x999ac || r==0x999c2 || r==0x999dc ||
        r==0x999f6 || r==0x99a0e || r==0x99a22 || r==0x99a70 || r==0x99a8a || r==0x99ac6 || r==0x99b22 || prisoner2_routine(r);
}
ga_result_t neo_prisoner_tick(neo_gactor_t *a,neo_genv_t *e) {return prisoner2_routine(L(0))?prisoner2_tick(a,e):prisoner_tick(a,e);}
