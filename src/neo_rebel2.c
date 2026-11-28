#include "neo_region.h"
/* Mission 2 rebels 0613CA / 0613D8 / 0613E6 (stages 2 and 3: the most
 * frequent spawns of the mission). Manual translation of ROM 0613CA..062634;
 * ROM labels are provenance only.
 *
 * One family, variant in +96 bit 0/1/2 (constructors 062114 / 062190 /
 * 06224E: script words +6E.. moved to +76/+90/+92/+9B/+9C, rank words
 * 2D4A60.. by 0260C8, +8A/+8C jittered by the RNG). Palettes 0x1F6/0x1F7,
 * flash 0x811, solid body E1262 (004E76), hurtbox from the programs.
 * Behaviour: idle 06173C (program 265C8A), keep the nearest player in +6E
 * (01AF9A / 01AEDA), walk towards it to +94 (0615A6) or away to +8C
 * (061670), turn (061808), shoot (061AE4, callback 062422: the shot 061E8C
 * after the node, 0040BC), variant 2 jumps by the table 2D4CCC (06243E /
 * 06247E, 0619BA). +9C: a flash 0620AA first (0614C0 waits for its +60);
 * +9B: a drop 061B70. Every update ends in 061BE8: +84 count, cull E121E
 * (01945C), +9D count, hurtbox (004C24: the spark 0682F0 after the script
 * actor, 0625EC) and the death by +5A (061C6E / 061D46 / 061E16 / 061E2E,
 * the item +90/+92 by 08CDB2 from the program callback 0624C4).
 * The cartridge check of 06201C (10F00A/10F00C, 2FFFE0) before 004188 is
 * not reproduced; 061E16 (kind 0x0A: pieces 068134) stops explicitly. */
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
#define STEP(r) do { r=ga_anim_step(a,e,call); if (r<0) return GA_UNSUPPORTED; } while (0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static uint32_t rl(uint32_t address) {return (uint32_t)rw(address)<<16|rw(address+2);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
static void fall(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}   /* 0143DC */
/* 0623FE: on the ground stand (01436A), else fall without +30. */
static void k623fe(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x15,6)) ga_stationary(a,e); else {SL(0x30,0);fall(a,e);}
}
/* 0623DA: on the ground walk (01456A), else fall. */
static void k623da(neo_gactor_t *a,neo_genv_t *e) {
    if (BIT(0x15,6)) ga_ground(a,e,NULL); else fall(a,e);
}
/* ---- players ---------------------------------------------------------------- */
static uint32_t pxl(const neo_genv_t *e,uint32_t node) {
    return node==P2_NODE?(uint32_t)e->players.p2x<<16|e->player_low[1]:(uint32_t)e->players.p1x<<16|e->player_low[0];
}
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
/* 01AEDA: carry when `node` is a live player node. */
static bool player_live(const neo_genv_t *e,uint32_t node) {
    unsigned d0;
    if (node==P1_NODE) d0=0; else if (node==P2_NODE) d0=1; else return false;
    return (e->players.active_mask>>d0)&1u;
}
/* 062318: +6E = the nearest player; carry when none. */
static bool k62318(neo_gactor_t *a,neo_genv_t *e) {uint32_t n;bool c=nearest(a,e,&n);SL(0x6e,n);return c;}
/* 062338: the player of +6E when live (carry clear), else player 1 (carry). */
static bool k62338(neo_gactor_t *a,neo_genv_t *e,uint32_t *node) {
    uint32_t n=L(0x6e);
    if (!(n&0x80000000u) && player_live(e,n)) {*node=n;return false;}
    *node=P1_NODE;return true;
}
/* 062362: carry when no player at all; *lost (D0 negative) when +6E was
 * taken again. */
static bool k62362(neo_gactor_t *a,neo_genv_t *e,bool *lost) {
    uint32_t n;*lost=false;
    if (!k62338(a,e,&n)) return false;
    if (k62318(a,e)) return true;
    *lost=true;return false;
}
/* 06237C + SWAP: whole pixels of |player X - X| (longs). */
static uint16_t distance(neo_gactor_t *a,neo_genv_t *e,uint32_t node) {
    uint32_t d=pxl(e,node)-L(0x3c);
    if (d&0x80000000u) d=0u-d;
    return (uint16_t)(d>>16);
}
static uint16_t player_distance(neo_gactor_t *a,neo_genv_t *e) {uint32_t n;k62338(a,e,&n);return distance(a,e,n);}
/* 06238C: carry when the player is behind (facing bit 0 against the side). */
static bool behind(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n;
    if (k62338(a,e,&n)) return false;
    bool right=(int32_t)pxl(e,n)>(int32_t)L(0x3c);
    return right!=(bool)BIT(0x15,0);
}
/* 0625CC: carry when the screen X is outside 8..0x138. */
static bool off_screen(neo_gactor_t *a) {return S16(W(0x3c))<8 || S16(W(0x3c))>0x138;}
/* 0625B0: carry when on screen and the fire delay +84 is over. */
static bool may_fire(neo_gactor_t *a) {return !off_screen(a) && S16(W(0x84))<=0;}
static uint32_t facing(neo_gactor_t *a,uint32_t v,bool when_set) {return BIT(0x15,0)==(unsigned)when_set?v:0u-v;}
/* ---- callbacks ------------------------------------------------------------ */
/* 0624C4: the item +90 (kind) / +92 dropped by 08CDB2 (kind < 0x5A, after
 * the script actor, at (0, 8)), its speed cleared; +90 = FFFF. */
static void drop_item(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x90)==0xffffu) return;
    ga_project(a,e);
    if (W(0x90)<0x5au) {
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
        if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x92));ga_sw(c,0x6e,W(0x90));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
            ga_place_at(c,a,e,0,8);ga_sl(c,0x30,0);ga_sl(c,0x34,0);}
    }
    SW(0x90,0xffff);
}
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    if (routine==0x62422u) {                                           /* 062422 */
        sound(e,0x1162);
        if (e->spawn) e->spawn(e->ctx,a,0x61e8cu,false);
        return true;
    }
    if (routine==0x624c4u) {drop_item(a,e);return true;}
    return false;
}
/* 03B332: launch speeds (+30, +34) to cover D0/D1 in D3 updates under the
 * gravity D2. */
static void launch(int16_t d0,int16_t d1,uint32_t d2,uint16_t d3,uint32_t *vx,uint32_t *vy) {
    uint32_t g=(d2&0x80000000u)?0u-d2:d2;
    if (S16(d3)<2) d3=2;
    uint16_t q=(uint16_t)(0x10000u/d3);
    uint32_t x=(uint32_t)((int32_t)d0*(int16_t)q),y=(uint32_t)((int32_t)d1*(int16_t)q);
    uint32_t t=(g&0xffffu)*d3+(((g>>16)*d3)<<16);
    t=(uint32_t)((int32_t)t>>1);
    if (d2&0x80000000u) t=0u-t;
    *vx=x;*vy=y-t;
}
/* 0625EC: the hit spark 0682F0 after the script actor (+9D: one per 0x18). */
static void spark(neo_gactor_t *a,neo_genv_t *e) {
    if ((int8_t)B(0x9d)>0) return;
    SB(0x9d,0x18);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x682f0u,true):NULL;
    uint16_t r=ga_rand(e);
    int16_t dx=(int16_t)((r&7u)-3u),dy=(int16_t)(((r>>8)&7u)+8u);
    if (c) {ga_place_at(c,a,e,dx,dy);ga_sl(c,0x34,0);}
}
/* ---- constructors ------------------------------------------------------------- */
static void k62114(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72),d3=W(0x74),d4=W(0x76);
    for (unsigned o=0x6e;o<=0x76;o+=2) SW(o,0);
    SB(0x96,0);SW(0x76,d0);SW(0x90,d1);SW(0x92,d2);SB(0x9b,d3?0xff:0);SB(0x9c,d4?0xff:0);
    SW(0x4e,rank(a,e,0x2d4a60));SW(0x88,rank(a,e,0x2d4bba));
    SL(0x72,(uint32_t)(int32_t)S16(rank(a,e,0x2d4ac2))<<8);
}
static void k62190(neo_gactor_t *a,neo_genv_t *e,bool jumper) {
    uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72),d3=W(0x74);
    for (unsigned o=0x6e;o<=0x74;o+=2) SW(o,0);
    SB(0x96,0);SW(0x90,d0);SW(0x92,d1);SB(0x9b,d2?0xff:0);SB(0x9c,d3?0xff:0);
    SW(0x4e,rank(a,e,0x2d4a60));
    if (jumper) {SB(0x97,rank(a,e,0x2d4c98));SB(0x98,rank(a,e,0x2d4cb2));}
    SW(0x88,rank(a,e,0x2d4bba));SW(0x8a,rank(a,e,0x2d4c1c));SW(0x8c,rank(a,e,0x2d4b3e));SW(0x8e,rank(a,e,0x2d4b24));
    if (!jumper) SL(0x72,(uint32_t)(int32_t)S16(rank(a,e,0x2d4ac2))<<8);
    uint16_t r=ga_rand(e);
    SW(0x8a,W(0x8a)+(uint16_t)((r&7u)-3u));
    SW(0x8c,W(0x8c)+(uint16_t)(((r>>8)&7u)-3u));
}
/* 06243E / 06247E: a jump of the table 2D4CCC (+34, +7A, +30 by facing). */
static unsigned jump(neo_gactor_t *a,neo_genv_t *e,bool back) {
    unsigned d0=back?B(0x98):B(0x97);
    if (back) {if (d0&0x80u) d0=(ga_rand(e)&3u)+4u;d0&=7u;}
    else {if (d0&0x80u) d0=ga_rand(e);d0&=3u;}
    uint32_t t=0x2d4cccu+12u*d0;
    SL(0x34,rl(t));SL(0x7a,rl(t+4));
    SL(0x30,facing(a,rl(t+8),!back));
    return d0;
}
/* ---- the actor -------------------------------------------------------------- */
enum { TAIL=1, DONE, DELETE };
static ga_result_t rebel(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);int r;bool lost;uint16_t d0;
    for (;;) switch (pc) {
    case 0x613ca:k62114(a,e);SET(0x96,0);pc=0x613f0;break;
    case 0x613d8:k62190(a,e,false);SET(0x96,1);pc=0x613f0;break;
    case 0x613e6:k62190(a,e,true);SET(0x96,2);pc=0x613f0;break;
    case 0x613f0: {
        SW(0x66,0x8820);SW(0x1e,0x8820);
        ga_palette(a,0x1f6);ga_palette(a,0x1f7);SB(0x14,B(0x19));
        ga_flash_palette(a,0x811);
        if (e->solid) e->solid(e->ctx,a,0xe1262);
        SW(0x84,W(0x88));
        k62318(a,e);
        uint32_t n;
        if (!k62338(a,e,&n)) {if ((int32_t)pxl(e,n)>(int32_t)L(0x3c)) SET(0x15,0);}
        else if (S16(W(0x3c))<0xa0) SET(0x15,0);
        k1b1dc(a);
        if (B(0x9b)) {pc=0x61b70;break;}
        ga_floor(a,e);
        if (B(0x9c)) {pc=0x61496;break;}
        pc=BIT(0x96,0)?0x61536:0x6173c;break;
    }
    case 0x61496: {
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x620aau,false):NULL;
        SB(0x60,c?0:0xff);
        if (c) ga_place_at(c,a,e,0,0);
        k1b1dc(a);SL(0,0x614c0);pc=0x614c0;break;
    }
    case 0x614c0:
        k623fe(a,e);
        if (B(0x60)) SL(0,0x614e4);
        if (ga_cull(a,e,0xe121e)) return GA_DELETE;
        return GA_MORE;
    case 0x614e4:START(0x265c8a);SW(0x82,0x14);k1b1dc(a);SL(0,0x61506);pc=0x61506;break;
    case 0x61506:
        k623fe(a,e);STEP(r);
        SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) SL(0,BIT(0x96,0)?0x61536:0x6173c);
        pc=TAIL;break;
    case 0x61536:START(0x265f36);SL(0x62,facing(a,L(0x72),true));SL(0,0x61560);pc=0x61560;break;
    case 0x61560: case 0x6187e: {
        k623da(a,e);STEP(r);
        uint32_t v=L(0x30);if (v&0x80000000u) v=0u-v;
        uint32_t left=L(0x76)-v;SL(0x76,left);
        if ((int32_t)left<=0 && BIT(0x15,6)) SL(0,pc==0x61560?0x6173c:0x61ae4);
        if (pc==0x61560 && !v && BIT(0x15,6)) SL(0,0x6173c);
        pc=TAIL;break;
    }
    case 0x615a6: {
        if (BIT(0x96,5)) {pc=0x6173c;break;}
        CLR(0x96,3);
        if (BIT(0x96,2)) {pc=0x618ae;break;}
        uint16_t d1=(uint16_t)(W(0x8a)-(ga_rand(e)&7u));
        if (S16(d1)<=S16(W(0x8e))) d1=(uint16_t)(W(0x8e)+4u);
        SW(0x94,d1);
        SL(0x62,facing(a,L(0x72),true));
        START(0x265f36);SL(0,0x6160c);pc=0x6160c;break;
    }
    case 0x6160c:
        k623da(a,e);STEP(r);
        if (BIT(0x15,6)) {
            if (k62362(a,e,&lost)) {SL(0,0x61b38);pc=TAIL;break;}
            if (lost) {SL(0,0x6173c);pc=TAIL;break;}
            if (S16(player_distance(a,e))<=S16(W(0x94))) SL(0,0x6173c);
            if (may_fire(a)) SL(0,0x61ae4);
        }
        pc=TAIL;break;
    case 0x61670:
        if (BIT(0x96,5)) {pc=0x6173c;break;}
        CLR(0x96,3);
        if (BIT(0x96,2)) {pc=0x618ca;break;}
        START(0x265fe2);SL(0x62,facing(a,L(0x72),false));SL(0,0x616b4);pc=0x616b4;break;
    case 0x616b4:
        k623da(a,e);STEP(r);
        if (BIT(0x15,6)) {
            if (k62362(a,e,&lost)) {SL(0,0x61b38);pc=TAIL;break;}
            if (lost) {SL(0,0x6173c);pc=TAIL;break;}
            if (S16(player_distance(a,e))>S16(W(0x8c))) SL(0,0x6173c);
            if (may_fire(a)) SL(0,0x61ae4);
            if (S16(W(0x3c))<=0x28 || S16(W(0x3c))>=0x118) {SET(0x96,3);SL(0,0x6173c);}
        }
        pc=TAIL;break;
    case 0x6173c:
        SB(0x9a,(ga_rand(e)&0xfu)==0?0xff:0);
        START(0x265c8a);k1b1dc(a);SL(0,0x61766);pc=0x61766;break;
    case 0x61766:
        k623fe(a,e);STEP(r);
        if (k62362(a,e,&lost)) {SL(0,0x61b38);pc=TAIL;break;}
        if (!((B(0x96)&0x21u)|B(0x9a))) {
            d0=player_distance(a,e);
            if (!BIT(0x96,3) && S16(d0)<=S16(W(0x8e))) SL(0,0x61670);
            if (S16(d0)>S16(W(0x8a))) SL(0,0x615a6);
        }
        if (behind(a,e)) SL(0,0x61808);
        if (may_fire(a)) {SL(0,0x61ae4);pc=TAIL;break;}
        if (S16(W(0x84))<=0) {
            SB(0x9a,0);CLR(0x96,5);
            if (BIT(0x96,3)) SL(0,0x61848);
        }
        pc=TAIL;break;
    case 0x61808:CLR(0x96,3);START(0x266666);k1b1dc(a);SL(0,0x6182a);pc=0x6182a;break;
    case 0x6182a:
        k623fe(a,e);STEP(r);
        if (r>0) {SB(0x15,B(0x15)^1u);SL(0,0x6173c);}
        pc=TAIL;break;
    case 0x61848:
        CLR(0x96,3);SL(0x62,facing(a,L(0x72),true));SW(0x76,0x20);
        START(0x265f36);SL(0,0x6187e);pc=0x6187e;break;
    case 0x618ae: case 0x618ca: {
        bool back=pc==0x618ca;
        if (k62362(a,e,&lost)) {SL(0,0x61b38);pc=TAIL;break;}
        unsigned i=jump(a,e,back);
        uint16_t d2=rw(0x2d4d2cu+2u*i);
        if (!back) {
            if (!BIT(0x96,4)) {
                if (!BIT(0x15,0)) d2=(uint16_t)(0u-d2);
                d2=(uint16_t)(d2+W(0x3c));
                uint32_t n;k62338(a,e,&n);
                uint16_t d1=(uint16_t)(d2-(uint16_t)(pxl(e,n)>>16));
                if (d1&0x8000u) d1=(uint16_t)(0u-d1);
                if (S16(d1)<=S16(W(0x8e))) {SET(0x96,5);pc=0x6173c;break;}
            }
        } else {
            if (BIT(0x15,0)) d2=(uint16_t)(0u-d2);
            uint16_t d1=(uint16_t)(W(0x3c)+d2);
            if (S16(d1)<=0x28 || S16(d1)>=0x118) {SET(0x96,4);pc=0x615a6;break;}
        }
        uint16_t t=rw(0x2d4d3cu+2u*i);
        SW(0x86,(uint16_t)((t>>3)+((t>>2)&1u)));
        START(0x26608e);SL(0,0x619a0);pc=0x619a0;break;
    }
    case 0x619a0:
        ga_stationary(a,e);STEP(r);
        if (r>0) SL(0,0x619ba);
        pc=TAIL;break;
    case 0x619ba:START(0x2660c8);SW(0x82,W(0x86));SL(0x7e,L(0x30));SL(0,0x619dc);pc=0x619dc;break;
    case 0x619dc:
        SL(0x34,L(0x34)+L(0x7a));ga_air(a,e);
        if (!L(0x30)) SL(0x30,L(0x7e));
        STEP(r);
        SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) {if ((int8_t)B(0x99)<7) SB(0x99,B(0x99)+1u);SW(0x82,W(0x86));}
        if (BIT(0x15,6)) SL(0,0x61a2e);
        pc=TAIL;break;
    case 0x61a2e:START(0x26611e);k1b1dc(a);SL(0,0x61a4a);pc=0x61a4a;break;
    case 0x61a4a:
        k623fe(a,e);STEP(r);
        if (r>0) {
            SL(0,0x6173c);
            if (k62362(a,e,&lost)) {SL(0,0x61b38);pc=TAIL;break;}
            d0=player_distance(a,e);
            if (S16(d0)<=S16(W(0x8e))) {
                SL(0,0x61670);
                if (BIT(0x96,4)) {
                    SL(0,0x615a6);
                    if (!(ga_rand(e)&1u)) SET(0x96,5);
                    pc=TAIL;break;
                }
            }
            CLR(0x96,4);
            if (S16(d0)>S16(W(0x8a))) SL(0,0x615a6);
            if (may_fire(a)) SL(0,0x61ae4);
            if (behind(a,e)) SL(0,0x61808);
        }
        pc=TAIL;break;
    case 0x61ae4:CLR(0x96,5);START(0x266190);k1b1dc(a);SL(0,0x61b06);pc=0x61b06;break;
    case 0x61b06:
        k623fe(a,e);STEP(r);
        if (r>0) {SW(0x84,W(0x88));SL(0,0x6173c);}
        if (off_screen(a)) {SW(0x84,W(0x88));SL(0,0x6173c);}
        pc=TAIL;break;
    case 0x61b38:START(0x265e62);k1b1dc(a);SL(0,0x61b54);pc=0x61b54;break;
    case 0x61b54:
        k623fe(a,e);STEP(r);
        if (!k62318(a,e)) SL(0,0x6173c);
        pc=TAIL;break;
    case 0x61b70:START(0x266162);SL(0,0x61b86);pc=0x61b86;break;
    case 0x61b86:
        fall(a,e);STEP(r);
        if (BIT(0x15,6)) SL(0,0x61ba6);
        pc=TAIL;break;
    case 0x61ba6:START(0x26611e);SL(0,0x61bbc);pc=0x61bbc;break;
    case 0x61bbc:
        k623fe(a,e);STEP(r);
        if (r>0) SL(0,BIT(0x96,0)?0x61536:0x6173c);
        pc=TAIL;break;
    case TAIL:                                                         /* 061BE8 */
        if (S16(W(0x84))>0) SW(0x84,W(0x84)-1u);
        if (ga_cull(a,e,0xe121e)) return GA_DELETE;
        if ((int8_t)B(0x9d)>0) SB(0x9d,B(0x9d)-1u);
        if (ga_register(a,e,NULL,NULL)) spark(a,e);
        if (BIT(0x5f,7)) {
            uint8_t k=B(0x5a);uint32_t to=0x61c6e;
            if (k==0x0au) to=0x61e16;
            if (k==0x0bu) to=0x61e2e;
            if (k==7u) to=0x61d46;
            SL(0,to);
            if (e->solid) e->solid(e->ctx,a,0);
        }
        SB(0x5a,0);
        return GA_MORE;
    /* ---- deaths ---- */
    case 0x61c6e: case 0x61d46: {
        bool burn=pc==0x61d46;
        sound(e,0x1163);SB(0x69,0xff);k1b1dc(a);                       /* 0120F8, 019DFC */
        if (!burn && BIT(0x15,6) && (e->video&1u)) {pc=0x61d12;break;}
        if (burn) {ga_palette(a,0x84a);START(0x26684a);} else START(0x266302);
        int16_t v=(int16_t)((ga_rand(e)&0xfu)+0x1fu);
        if ((int8_t)B(0x5b)<0) v=(int16_t)-v;
        uint32_t vx,vy;launch(v,0,0xffff4000u,0xf,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SB(0x17,0xff);
        SL(0,burn?0x61db8:0x61ce8);pc=burn?0x61db8:0x61ce8;break;
    }
    case 0x61ce8: case 0x61db8:
        fall(a,e);STEP(r);
        SL(0x30,L(0x30)-(uint32_t)((int32_t)L(0x30)>>5));
        if (BIT(0x15,6)) SL(0,pc==0x61ce8?0x61d12:0x61de2);
        pc=DONE;break;
    case 0x61d12: case 0x61de2:
        START(pc==0x61d12?0x2663f8:0x2668c8);k1b1dc(a);
        SL(0,pc==0x61d12?0x61d2e:0x61dfe);pc=pc==0x61d12?0x61d2e:0x61dfe;break;
    case 0x61d2e: case 0x61dfe:
        k623fe(a,e);STEP(r);
        if (r>0) SL(0,0x6201c);
        pc=DONE;break;
    case 0x61e16:ga_fail(e,0x624fe);return GA_UNSUPPORTED;            /* pieces 068134 */
    case 0x61e2e:
        START(0x2665ec);
        sound(e,0x1163);
        { /* 01AD52: the position block to the scene root, then 01ABC8 */
            uint32_t tag=L(0x38);
            if (tag!=0x10e968u) {
                neo_contact_parent_t op,np;bool has_old=tag && ga_parent(e,tag,&op),has_new=ga_parent(e,0x10e968u,&np);
                uint32_t x0=L(0x28),y0=L(0x2c),x1=L(0x30),y1=L(0x34);
                x1+=x0;y1+=y0;
                if (has_old) {x0+=(uint32_t)op.previous_x;y0+=(uint32_t)op.previous_y;x1+=(uint32_t)op.x;y1+=(uint32_t)op.y;}
                if (has_new) {x0-=(uint32_t)np.previous_x;y0-=(uint32_t)np.previous_y;x1-=(uint32_t)np.x;y1-=(uint32_t)np.y;}
                x1-=x0;y1-=y0;
                SL(0x28,x0);SL(0x2c,y0);SL(0x30,x1);SL(0x34,y1);SL(0x38,0x10e968u);
            }
            ga_project(a,e);
        }
        k1b1dc(a);drop_item(a,e);SB(0x17,0xff);SL(0,0x61e64);pc=0x61e64;break;
    case 0x61e64:
        ga_project(a,e);STEP(r);
        if (r>0) SL(0,0x6201c);
        pc=DONE;break;
    case DONE:                                                         /* 061E7A */
        if (ga_cull(a,e,0xe121e)) return GA_DELETE;
        return GA_MORE;
    case 0x6201c:return GA_DELETE;                                     /* 004188 */
    default:ga_fail(e,pc);return GA_UNSUPPORTED;
    }
}
/* 061E8C: the shot of 062422, from (-0x14, 0x15) of its creator (01AD6E);
 * speeds by rank (2D4B58, range 2D4C66 in +76), the attack row E1252
 * (0049E8) while flying, hurtbox E1246, cull E1226 (019504). */
static ga_result_t shot(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t pc=L(0);int r;
    for (;;) switch (pc) {
    case 0x61e8c: {
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_palette(a,0x1f7);SET(0x15,5);SL(0x50,0xe1246);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        SB(0x15,(uint8_t)(p?(ga_b(p,0x15)&3u):0u));                    /* 01ACA2 */
        int16_t dx=-0x14;if (BIT(0x15,0)) dx=0x14;
        ga_place(a,e,dx,0x15,p?NEO_GA_TAG(p->index):0);                /* 01AD6E */
        START(0x266686);
        SL(0x30,facing(a,(uint32_t)(int32_t)S16(rank(a,e,0x2d4b58))<<8,true));
        SL(0x34,0);
        SW(0x76,rank(a,e,0x2d4c66));
        SB(0x61,0);SL(0,0x61f26);pc=0x61f26;break;
    }
    case 0x61f26: {
        uint32_t d=0x6000u-L(0x34);
        if (!(d&0x80000000u)) SL(0x34,L(0x34)+(uint32_t)((int32_t)d>>3));
        ga_air(a,e);
        if (!B(0x61)) {
            uint32_t v=L(0x30);if (v&0x80000000u) v=0u-v;
            uint32_t left=L(0x76)-v;SL(0x76,left);
            SB(0x61,(int32_t)left<=0?0xff:0);
        }
        if (BIT(0x5f,0) || !L(0x34)) {SB(0x61,1);SL(0x30,0);SL(0x34,0);SL(0,0x61f84);pc=0x61f84;break;}
        pc=0x61f8a;break;
    }
    case 0x61f84:ga_project(a,e);pc=0x61f8a;break;
    case 0x61f8a:
        STEP(r);
        if (r>0) {START(0x266780);SL(0,0x61fc2);pc=0x61fc2;break;}
        if (ga_attack(a,e,0xe1252)) return GA_DELETE;
        pc=0x61fec;break;
    case 0x61fc2:
        ga_move(a,e);STEP(r);
        if (r>0) return GA_DELETE;
        SL(0x30,L(0x30)-(uint32_t)((int32_t)L(0x30)>>4));
        if (B(0x61)&0x80u) SL(0x34,L(0x34)+0x1000u);
        pc=0x61fec;break;
    case 0x61fec:
        if (ga_register(a,e,NULL,NULL) && (B(0x5a)==0x0au || B(0x5a)==0x0bu)) return GA_DELETE;
        if (ga_cull_rect(a,e,0xe1226)) return GA_DELETE;
        return GA_MORE;
    default:ga_fail(e,pc);return GA_UNSUPPORTED;
    }
}
/* 0620AA: the flash before a +9C rebel (palette 0xB0, program 266A38: its
 * byte +60 raises the creator's +60). */
static bool flash_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)a;(void)e;(void)kind;(void)routine;(void)parameter;return false;
}
static ga_result_t flash(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x620aau) {
        SB(0x1d,0);                                                    /* 009F88 */
        SW(0x66,0x8821);SW(0x1e,0x8821);ga_palette(a,0xb0);
        if (!ga_anim_start(a,e,0x266a38,0x100)) return GA_UNSUPPORTED;
        SB(0x60,0);SL(0,0x620e6);return GA_MORE;
    }
    ga_project(a,e);
    int r=ga_anim_step(a,e,flash_call);
    if (r<0) return GA_UNSUPPORTED;
    if (r>0) return GA_DELETE;
    if (B(0x60)) {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (p) ga_sb(p,0x60,1);
        SB(0x60,0);
    }
    return GA_MORE;
}
bool neo_rebel2_routine(uint32_t r) {return (r>=0x613cau && r<0x62114u);}
ga_result_t neo_rebel2_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (r==0x620aau || r==0x620e6u) return flash(a,e);
    if (r>=0x61e8cu && r<0x6201cu) return shot(a,e);
    return rebel(a,e);
}
