#include "neo_region.h"
#include "neo_soldier_ai.h"
/* Mission 2 stage 3: the creature 080170 thrown by the soldiers 0678B6 of
 * +9C = 8 (callback 06899A -> 0689D2 of neo_rebel3.c, B54), its egg 0808AA
 * and its pieces 0805D4 / 080634 / 08070C. Manual translation of ROM
 * 080170..080A60; ROM labels are provenance only.
 *
 * 080170 (004C1C, 080804: the thrower's +6E/+70/+72 to +7C/+7E/+80, +4E by
 * 2D4874; hurtbox E4960, palette 0x836): with +7C = 0 (the only case
 * observed) 08023E: program 2801CC, +6E = rank 2D488E << 8, +86 / +88 by
 * 2D48DA / 2D48F4, 080A08 (+82), heading +8A by the facing, +84 =
 * 0029F2(8), +8C toward the nearest player (0366AC), sound 1166, then the
 * flight 0802CE: 080942 (every +82 updates a new aim +8C, 080A08), the turn
 * +8B every 8 updates, 0809D6 (+30/+34 = 002E7C(+6E, +8A), halved below
 * Y 0x140 while heading up), 01AC38, 08086A (facing by +30), 00427A; +88
 * once 080A3C; at +86 = 0 bit 3 of +5F (harmless), below -24 deleted;
 * 01945C (E4958). Its bite (0049E8, E496C): palette 0x837, program 2802C2
 * (its call 08088E lays the egg 0808AA), sound 1165, 080390 until the end.
 * Killed (004C24 / +5F bit 7, 0803A8): two 08070C, two 080634 and one
 * 0805D4 (reserve 0x2A, a tenth of its speed), 08083C (the item +7E/+80 by
 * 08CDB2), program 2803BC, 08052E until the end. 080546 ends with the
 * cartridge check (10F00A/10F00C, 2FFFE0, not reproduced) and 004188.
 * Pieces: speed 002E7C(0x40000, 0x30 + rand & 0x1F) with a drag toward
 * (0, -2) (0806A8) or a swing (080786: +8D += 8, X = 002E7C(0xC000) + +76,
 * +76 -= +76 >> 5); after +86 updates +5F bit 3, 0x10 more, or 019504.
 * Egg 0808AA: program 280342, 0143DC until +15 bit 6, then 28034C.
 * Paths without MAME evidence stop explicitly (ga_fail): 080170 with +7C
 * (0801D0..08023A, the RAM 10E99E wait) and the other entries of
 * 080A62..0822B2. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,n) ((B(o)>>(n))&1u)
#define SET(o,n) SB(o,B(o)|(1u<<(n)))
#define CLR(o,n) SB(o,B(o)&~(1u<<(n)))
#define START(t,s) do { if (!ga_anim_start(a,e,(t),(s))) return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { r=ga_anim_step(a,e,call); if (r<0) return GA_UNSUPPORTED; } while (0)
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {   /* 0040BC with D0 */
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
/* 0366AC: angle toward the nearest player (+0x20 above its +40); D0 is
 * left as it was (*d0) when no player is in play. */
static void aim(neo_gactor_t *a,neo_genv_t *e,uint16_t *d0) {
    uint8_t mask=e->players.active_mask&3u;
    if (!mask) return;
    uint16_t px=e->players.p1x,py=e->players.p1y;
    if (mask==2u) {px=e->players.p2x;py=e->players.p2y;}
    else if (mask==3u) {      /* 01AF9E: the horizontally nearest, P1 on ties */
        int16_t d1=(int16_t)(e->players.p1x-W(0x3c)),d2=(int16_t)(e->players.p2x-W(0x3c));
        if (d1<0) d1=(int16_t)-d1;
        if (d2<0) d2=(int16_t)-d2;
        if (d2<d1) {px=e->players.p2x;py=e->players.p2y;}
    }
    int16_t dx=(int16_t)(px-W(0x3c)),dy=(int16_t)(py+0x20u-W(0x40));
    *d0=(uint16_t)((*d0&0xff00u)|neo_angle(dx,dy));
}
static bool low(neo_gactor_t *a) {return S16(W(0x40))<0x140;}           /* 0809C0: carry */
/* 080A08: +82 = rank 2D48C0, doubled one time in five, less rand & 7 (D0). */
static uint16_t k80a08(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x82,rank(a,e,0x2d48c0));
    if (!ga_rand_range(e,5)) SW(0x82,W(0x82)<<1);
    uint16_t d0=ga_rand(e)&7u;
    SW(0x82,W(0x82)-d0);return d0;
}
/* 080A3C: +8C turned by 0029F2(rank 2D490E) away from straight down. */
static void k80a3c(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d0=(uint8_t)ga_rand_range(e,rank(a,e,0x2d490e));
    uint8_t d2=((uint8_t)(B(0x8c)+0x40u)&0x80u)?0xffu:0u;
    d0=(uint8_t)((d0^d2)-d2);SB(0x8c,B(0x8c)+d0);
}
/* 0809D6 */
static void k809d6(neo_gactor_t *a) {
    uint32_t d0=L(0x6e);
    if (low(a) && (B(0x8a)&0x80u)) d0>>=1;
    uint32_t vx,vy;ga_polar(d0,B(0x8a),&vx,&vy);SL(0x30,vx);SL(0x34,vy);
}
/* 080942 */
static void k80942(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x82,W(0x82)-1u);
    if (S16(W(0x82))<=0) {
        uint16_t d0=k80a08(a,e);
        if (low(a)) SB(0x8c,0x40);
        else {aim(a,e,&d0);SB(0x8c,d0);}
        SB(0x8c,B(0x8c)+(uint8_t)((ga_rand(e)&0xfu)-7u));
    }
    SW(0x84,W(0x84)+1u);
    if (!(W(0x84)&7u)) SB(0x8b,((uint8_t)(B(0x8c)-B(0x8a))&0x80u)?0xfau:6u);
    uint8_t d1=(uint8_t)(B(0x8b)+(uint8_t)(ga_rand_range(e,3)-1u));
    SB(0x8a,B(0x8a)+d1);
    k809d6(a);
}
/* 08086A: the facing by the sign of +30 (kept at 0). */
static void k8086a(neo_gactor_t *a) {
    if (!W(0x30)) return;
    if (W(0x30)&0x8000u) CLR(0x15,0); else SET(0x15,0);
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 08088E: the egg 0808AA (reserve 0x1C) at the creature (01ACF2). */
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x8088eu) return false;
    neo_gactor_t *c=spawn(a,e,0x808aa,0x1c);
    if (c) ga_place_at(c,a,e,0,0);
    return true;
}
/* A piece of 0803A8 (reserve 0x2A, 01B220 at (0, 0), +30/+34 >> 3). */
static neo_gactor_t *piece(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    neo_gactor_t *c=spawn(a,e,routine,0x2a);
    if (!c) return NULL;
    ga_place_from(c,a,e,0,0);
    ga_sl(c,0x30,(uint32_t)((int32_t)L(0x30)>>3));ga_sl(c,0x34,(uint32_t)((int32_t)L(0x34)>>3));
    return c;
}
/* BCHG 0 of the new node's +15 (cleared when its node was freed, 003F24,
 * and set from the creature's by 01B242). */
static void flip(neo_gactor_t *a,neo_gactor_t *c) {if (c) ga_sb(c,0x15,(uint8_t)((B(0x15)&7u)^1u));}
static void nudge(neo_gactor_t *c,uint16_t r) {if (c) ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+(uint16_t)((r&0xfu)-7u)));}
/* 08083C: the item +7E (kind) / +80 by 08CDB2 at (0, 4), then +30/+34 = 0. */
static void k8083c(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x7e)==0xffffu) return;
    if (W(0x7e)<0x5au) {
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
        if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x80));ga_sw(c,0x6e,W(0x7e));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,4);}
    }
    SL(0x30,0);SL(0x34,0);
}
/* 0806A8 / 080786 tail: +86 updates, +5F bit 3, 0x10 more; 019504. */
static ga_result_t piece_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x5f,3)) {
        SW(0x86,W(0x86)-1u);
        if (W(0x86)&0x8000u) {SET(0x5f,3);SW(0x86,0x10);}
    } else {
        SW(0x86,W(0x86)-1u);
        if (W(0x86)&0x8000u) return GA_DELETE;                          /* 080546 */
    }
    return ga_cull_rect(a,e,0xe4958)?GA_DELETE:GA_MORE;
}
static ga_result_t creature(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    for (int guard=0;guard<8;guard++) {
        switch (L(0)) {
        case 0x80170: {
            SB(0x5e,B(0x5e)|0x80u);                                      /* 004C1C */
            uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72);                     /* 080804 */
            SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x7c,d0);SW(0x7e,d1);SW(0x80,d2);
            SW(0x4e,rank(a,e,0x2d4874));
            SL(0x50,0xe4960);SW(0x66,0x8820);SW(0x1e,0x8820);SET(0x15,5);CLR(0x15,1);
            SW(0x66,0xe0ef);SW(0x1e,0xe0ef);ga_palette(a,0x836);
            if (W(0x7c)) STOP(0x801d0);
            START(0x2801cc,0x100);
            SL(0x6e,(uint32_t)((int32_t)S16(rank(a,e,0x2d488e))<<8));
            SW(0x86,rank(a,e,0x2d48da));SW(0x88,rank(a,e,0x2d48f4));
            k80a08(a,e);
            SB(0x8a,BIT(0x15,0)?0u:0x80u);
            uint16_t v=ga_rand_range(e,8)&7u;SW(0x84,v);SB(0x8b,0);
            aim(a,e,&v);SB(0x8c,v);SB(0x5a,0);
            sound(e,0x1166);SL(0,0x802ce);
        }   /* fall through */
        case 0x802ce:
            k80942(a,e);ga_move(a,e);k8086a(a);STEP(r);
            if (S16(W(0x88))>0) {
                SW(0x88,W(0x88)-1u);
                if (S16(W(0x88))<=0) {k80a3c(a,e);k80a08(a,e);}
            }
            SW(0x86,W(0x86)-1u);
            if (!W(0x86)) SET(0x5f,3);
            else if (S16(W(0x86))<=-25) return GA_DELETE;                  /* 080546 */
            if (ga_cull(a,e,0xe4958)) return GA_DELETE;                      /* 01945C */
            if (!BIT(0x5f,3) && ga_attack(a,e,0xe496c)) {                    /* 0049E8 */
                ga_palette(a,0x837);START(0x2802c2,0x100);sound(e,0x1165);SB(0x17,0xff);
                SL(0,0x80390);continue;
            }
            if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);                      /* 004C24 */
            if (!(B(0x5f)&0x80u)) return GA_MORE;
            {                                                                /* 0803A8 */
                neo_gactor_t *c=piece(a,e,0x8070c);
                uint16_t rnd=ga_rand(e);
                nudge(c,(uint16_t)(rnd>>8));
                c=piece(a,e,0x8070c);nudge(c,rnd&0xffu);flip(a,c);
                piece(a,e,0x80634);c=piece(a,e,0x80634);flip(a,c);
                piece(a,e,0x805d4);
                k8083c(a,e);
                ga_palette(a,0x837);START(0x2803bc,0x100);sound(e,0x1165);SB(0x17,0xff);
                SL(0,0x8052e);
            }   /* fall through */
        case 0x8052e: case 0x80390: case 0x8092a:
            ga_project(a,e);STEP(r);
            if (r>0) SL(0,0x80546);
            return GA_MORE;
        case 0x80546:return GA_DELETE;                                       /* 004188 */
        case 0x805d4: case 0x80634: {
            bool first=L(0)==0x805d4u;
            SW(0x66,0xe0ee);SW(0x1e,0xe0ee);ga_palette(a,0x837);
            uint16_t rnd=ga_rand(e);
            START(first?0x2804a0:0x280436,(uint16_t)((rnd&0xffu)+0x100u));
            uint32_t vx,vy;ga_polar(0x40000,(uint8_t)((rnd&0x1fu)+0x30u),&vx,&vy);
            SL(0x30,L(0x30)+vx);SL(0x34,L(0x34)+vy);
            if (first && !(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);           /* 01DEA2 */
            SB(0x8e,2);SW(0x86,(ga_rand(e)&0xfu)+0x10u);SL(0,0x806a8);
        }   /* fall through */
        case 0x806a8:
            SL(0x30,L(0x30)+(uint32_t)((int32_t)(0u-L(0x30))>>4));
            SL(0x34,L(0x34)+(uint32_t)((int32_t)(0xfffe0000u-L(0x34))>>4));
            ga_move(a,e);STEP(r);
            return piece_tail(a,e);
        case 0x8070c: {
            SW(0x66,0xe0ee);SW(0x1e,0xe0ee);ga_palette(a,0x837);
            uint16_t rnd=ga_rand(e);
            START(0x28050a,(uint16_t)((rnd&0xffu)+0x80u));
            SB(0x8d,rnd>>8);SB(0x8e,2);
            SL(0x72,(uint32_t)(int32_t)(int16_t)(ga_rand(e)&0x7fffu)+0xffff4000u);
            SL(0x76,L(0x30));
            SW(0x86,(ga_rand(e)&0xfu)+0x10u);SL(0,0x80786);
        }   /* fall through */
        case 0x80786: {
            SL(0x34,L(0x34)+(uint32_t)((int32_t)(L(0x72)-L(0x34))>>4));
            SB(0x8d,B(0x8d)+8u);
            uint32_t vx,vy;ga_polar(0xc000,B(0x8d),&vx,&vy);
            SL(0x76,L(0x76)-(uint32_t)((int32_t)L(0x76)>>5));
            SL(0x30,vx+L(0x76));
            ga_move(a,e);STEP(r);
            return piece_tail(a,e);
        }
        case 0x808aa:
            SW(0x66,0xe0ee);SW(0x1e,0xe0ee);ga_palette(a,0x837);
            START(0x280342,0x100);k1b1dc(a);SL(0,0x808e0);
            /* fall through */
        case 0x808e0:
            SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);STEP(r);               /* 0143DC */
            if (BIT(0x15,6)) SL(0,0x8090e);
            return ga_cull(a,e,0xe4958)?GA_DELETE:GA_MORE;
        case 0x8090e:
            START(0x28034c,0x100);k1b1dc(a);SL(0,0x8092a);continue;
        default:STOP(L(0));
        }
    }
    STOP(L(0));
}
static const uint32_t STATES[]={0x80170,0x802ce,0x80390,0x8052e,0x80546,0x805d4,0x80634,0x806a8,0x8070c,0x80786,0x808aa,0x808e0,0x8090e,0x8092a};
bool neo_m2creature_routine(uint32_t r) {
    for (unsigned i=0;i<sizeof(STATES)/sizeof(STATES[0]);i++) if (STATES[i]==r) return true;
    return false;
}
ga_result_t neo_m2creature_tick(neo_gactor_t *a,neo_genv_t *e) {return creature(a,e);}
