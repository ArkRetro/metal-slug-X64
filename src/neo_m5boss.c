#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 5 boss 0A0C00 (script record 2DF66C, camera 2560) and its pieces:
 * manual translation, routine by routine; ROM labels are provenance only
 * (docs/MISION5.md, "El jefe"; MAME run4 replay from sec08_274387, capture B).
 * - 0A0C00 (the controller, invisible): the music 059F76 (2C), the pieces of
 *   0A26CE (0A1830, five 0A14FE with +85 = 0..4, the body 0A167C), +62 by
 *   0A2312, health +4E = rank 2C2B08, the next threshold +7E = rank 2C2B6A,
 *   the target player +6E (01B04A), palettes 107 / 149, the smoke 01D86C
 *   (01DD50, rectangle 40, 48, 3F, 1F), the fires of 0A29CE; rises
 *   (0A0C9A..0A0D1C: the flames 0A2AE8 and 0A2B08), then hovers (0A0D54 /
 *   0A0D80: +30 eased, 0A2560 the target, 0A24C6 the next attack
 *   0A2D46[+8A]) and attacks by orders +60 to the body (0A0DA6, 0A0EC6,
 *   0A10BE, 0A0FA4: the body's +61 answers). Common tail 0A11E2 / 0A11F0 /
 *   0A11FA: the speed (0A241A / 0A244C), 01AC38, the damage +80 of the
 *   pieces, death at +4E < 0, the bob +87, the flash +8C / +8D, the phase
 *   +89 at +7E (0A2926 at the second), the cell map 0A2E40 (0140C0).
 *   Death 0A12C0..0A1466: 013F1E, 018F92, the pieces 0A18D8 / 0A1756, the
 *   order FD to the pieces, the fires 0A2AE8 / 0A2958, 01DA40 lists, the
 *   rise, 009BB0, 0C8022, the fires 0A29CE, 018F40, 004188.
 * - 0A14FE (x5, hurtboxes on the body): the damage to the controller (+80,
 *   +8C, +90), its own stages +86 (ranks 2C2C90 / 2C2CF2 / 2C2D24) with
 *   the explosion 0A2718 and the sound 1021; 0A25E0 the bob.
 * - 0A167C (the body): follows the orders +60 (0A2584: programs 0A2D6A),
 *   +61 = FF at a program's end (0A25EA), order 4 turns +88 to the
 *   controller's (0A2514) and gives +86. Its program calls: 0A27E0 /
 *   0A2856 (the shots 0A192E / 0A1958), 0A28EA / 0A28FC (0A1E92 / 0A1EA6),
 *   0A290E (0A1F0E), 0A264E / 0A2656 / 0A265E (+14 = +19 / +1A / +1B).
 * - 0A1830 / 0A18D8 (the lights), 0A1756 (the falling piece of the death:
 *   at y 20 the explosion 01C71E), 0A192E / 0A1958 (shots: 0049E8 row
 *   0A2BBA, a bounce with +92 bit 7, 01C706 / 0A14F6 at their end),
 *   0A1B3A / 0A1BEC / 0A1C0C / 0A1C4C / 0A1D20 (the beam: 0049E8 row 0A2BCA
 *   along the angle until y 200), 0A1DF2 / 0A1E7E, 0A1E92 / 0A1EA6 /
 *   0A1F0E (reparented to the controller), 0A1F98 / 0A203A (debris of the
 *   lists 01D764), 0A2134 -> 099090 (the burnt soldiers flung by the death:
 *   palette 87C, +5F bit 6 set: no hurtbox, the 0406B6 branch stops
 *   explicitly), the fires 0A21A8 / 0A21CE / 0A21F0.
 * 0A1C8C and 0A1DCE are referenced nowhere in the ROM (dead code). The
 * cartridge checks (RAM 10F00C + 3EF) end in 004188, as in neo_m5enemy.c. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define S16(v) ((int16_t)(uint16_t)(v))
#define S8(v) ((int8_t)(uint8_t)(v))
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { if (((r)=ga_anim_step(a,e,boss_call))<0) return GA_UNSUPPORTED; } while (0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static uint8_t rb(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return p[0];}
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2);}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}   /* 0120F8 (012192: | 8000) */
static ga_result_t done(neo_genv_t *e) {return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
/* 0260C8 (a type-2 table takes a random number). */
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;if (!c) ga_fail(e,0x4000cu);return c;}
/* 0040BC with the reserve D0. */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
/* 0040BC + 01B220 (dx, dy) when it gave a node. */
static neo_gactor_t *child_from(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,int16_t dx,int16_t dy) {
    neo_gactor_t *c=child(a,e,routine,reserve);
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
/* 01AD6E: (dx, dy) relative to the creator's block. */
static void on_creator(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {neo_gactor_t *p=creator(a,e);ga_place(a,e,dx,dy,p?NEO_GA_TAG(p->index):0);}
/* 01DA40: the explosion list emitter 01D764 (+86 = list) at the node. */
static void k1da40(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764,0x1c);
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
/* 01DD50: the smoke 01D86C (D0 = 1C, 01B220 (0, 0)) with +94 = 0 and the rectangle +6E..+74. */
static void k1dd50(neo_gactor_t *a,neo_genv_t *e,uint16_t d0,uint16_t d1,uint16_t d2,uint16_t d3) {
    neo_gactor_t *c=child_from(a,e,0x1d86c,0x1c,0,0);
    if (!c) return;
    ga_sb(c,0x94,0);ga_sw(c,0x6e,d0);ga_sw(c,0x70,d1);ga_sw(c,0x72,d2);ga_sw(c,0x74,d3);
}
/* 01DBAE: D0 pieces 01D31C (D0 = 2A, 01B220 (0, 18)). */
static void k1dbae(neo_gactor_t *a,neo_genv_t *e,int16_t n) {
    do child_from(a,e,0x1d31c,0x2a,0,0x18); while (--n>0);
}
/* 018F92: mission complete (as neo_m4boss.c). */
static void k18f92(neo_gactor_t *a,neo_genv_t *e) {
    neo_ga_flags_t *f=e->flags;
    if (!f) {ga_fail(e,0x18f92);return;}
    f->kill_request=0xff;f->flash_request=0xff;f->player_guard=1;
    if (e->vehicle) for (unsigned p=0;p<2;p++) {
        uint8_t *n=e->vehicle->player[p];
        if (!e->vehicle->exists[p] || !n) continue;
        n[0x84]=0xff;n[0x85]=0xff;n[0x5f]|=0x40u;n[0x9a]&=(uint8_t)~4u;
    }
    f->mission_end=0xff;f->end_kind=B(0x5e);f->player_handlers=2;
}
static void flash(neo_genv_t *e,uint8_t on) {if (e->flags) e->flags->flash=on; else ga_fail(e,0x9bb0);}   /* 009BB0: RAM 109944 */
static void shake(neo_genv_t *e) {if (e->shake_pulse) e->shake_pulse(e->ctx); else ga_fail(e,0xc8022);}   /* 0C8022 */
/* 01AEDA: carry when `node` is a player node whose bit is set in 10C33C. */
static bool live(const neo_genv_t *e,uint32_t node) {
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
static uint16_t px(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2x:e->players.p1x;}
static uint16_t py(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2y:e->players.p1y;}
/* 01AF9A: the nearest live player's node (player 1 when none); carry (true) when none. */
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
/* 01B14A / 01B162: the angle to the centre of the first +50 rectangle of a player node (as neo_m4boss.c). */
static bool aim(neo_gactor_t *a,neo_genv_t *e,uint32_t node,uint8_t *angle) {
    const uint8_t *n=NULL;
    if (e->vehicle) n=node==P1_NODE?e->vehicle->player[0]:node==P2_NODE?e->vehicle->player[1]:NULL;
    if (!n) {ga_fail(e,0x1b162);return false;}
    int16_t x=(int16_t)(n[0x3c]<<8|n[0x3d]),y=(int16_t)(n[0x40]<<8|n[0x41]);
    uint32_t d=(uint32_t)n[0x50]<<24|(uint32_t)n[0x51]<<16|(uint32_t)n[0x52]<<8|n[0x53];
    if (d) {
        const uint8_t *r=ga_rom(d+2u);
        if (!r) {ga_fail(e,d);return false;}
        int16_t r0=(int16_t)(r[0]<<8|r[1]),r1=(int16_t)(r[2]<<8|r[3]),r2=(int16_t)(r[4]<<8|r[5]),r3=(int16_t)(r[6]<<8|r[7]);
        int16_t dx=(int16_t)((r2>>1)+r0),dy=(int16_t)((r3>>1)+r1);
        if (B(0x15)&1u) dx=(int16_t)-dx;
        if (B(0x15)&2u) dy=(int16_t)-dy;
        x=(int16_t)(x+dx);y=(int16_t)(y+dy);
    }
    *angle=(uint8_t)neo_angle((int16_t)(x-S16(W(0x3c))),(int16_t)(y-S16(W(0x40))));
    return true;
}
/* 01AD8C: +1E follows +66 - y when +66 is 8xxx. */
static void depth(neo_gactor_t *a) {uint16_t d0=W(0x66);if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));}
static void release(neo_gactor_t *a) {SB(0x1d,0);}                    /* 009F88 */
static void palettes3(neo_gactor_t *a,uint16_t p) {ga_palette(a,p);ga_palette(a,(uint16_t)(p+1u));ga_palette(a,(uint16_t)(p+2u));}
/* ---- shared helpers of the pieces -------------------------------------------------- */
/* 0A22FC: the rank table 0A2xxx[+89 & 3]. */
static uint16_t k22fc(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {return rank(a,e,rl(e,list+4u*(B(0x89)&3u)));}
static void k2312(neo_gactor_t *a,neo_genv_t *e) {SL(0x62,(uint32_t)(int32_t)S16(k22fc(a,e,0xa2be2))<<8);}
static void k2324(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *p=creator(a,e);if (p) ga_sb(p,0x61,B(0x61));}
/* 0A2330: +88 = the angle to the target +6E (58 above its feet). */
static uint8_t k2330(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t n=L(0x6e);
    int16_t d0=(int16_t)(px(e,n)-W(0x3c)),d1=(int16_t)(py(e,n)-0x58u-W(0x40));
    uint8_t g=neo_angle(d0,d1);
    SB(0x88,g);return g;
}
/* 0A2354: +88 rounded by the table 0A2CF2[+8A][+89]. */
static uint16_t k2354(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d1=B(0x88),d2=B(0x8a)&3u;
    if (d2!=3u) {
        uint16_t d4=rw(e,rl(e,0xa2cf2u+4u*d2)+2u*(B(0x89)&3u));
        if (!d4) d1=S16(d1)<0x38?0x30:S16(d1)>0x48?0x50:0x40;
        else if (d4==1u) d1=S16(d1)<0x28?0x20:S16(d1)>0x58?0x60:(uint16_t)(((d1-0x28u)&0xf0u)+0x30u);
        else d1=S16(d1)<0x24?0x20:S16(d1)>0x5c?0x60:(uint16_t)(((d1-0x24u)&0xf8u)+0x28u);
    }
    SB(0x88,d1);return d1;
}
/* 0A2460..0A24C4: +30 eased by +62 / 16 (the carry is not used). */
static void ease(neo_gactor_t *a,int kind) {
    uint32_t d0,d1;
    if (kind==0) {
        d0=L(0x30);if (d0&0x80000000u) d0=0u-d0;
        d1=L(0x62)>>4;
        if ((int32_t)d1>(int32_t)d0) {SL(0x30,0);return;}
        if (!(L(0x30)&0x80000000u)) kind=1; else kind=2;
    }
    if (kind==1) {                                                         /* 0A2488 */
        d0=L(0x62);d1=d0>>4;SL(0x30,L(0x30)-d1);d0=0u-d0;
        if ((int32_t)d0>(int32_t)L(0x30)) SL(0x30,d0);
        return;
    }
    d0=L(0x62);d1=d0>>4;SL(0x30,L(0x30)+d1);                              /* 0A24A8 */
    if ((int32_t)d0<(int32_t)L(0x30)) SL(0x30,d0);
}
static void k241a(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x=S16(W(0x3c));
    if (x<0x20) {ease(a,2);return;}
    if (x>0x100) {ease(a,1);return;}
    uint16_t d0=k2330(a,e),d1=k2354(a,e);
    d0&=0x7fu;d1&=0x7fu;
    if (S16(d1)>S16(d0)) ease(a,2); else if (S16(d1)<S16(d0)) ease(a,1); else ease(a,0);
}
static void k244c(neo_gactor_t *a) {
    int16_t x=S16(W(0x3c));
    ease(a,x<0x20?2:x>0x100?1:0);
}
/* 0A24C6: at the end of +82 the attack 0A2D46[+8A] (unless no player). */
static void k24c6(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x82,W(0x82)-1u);
    if (S16(W(0x82))>0) return;
    SW(0x82,0);
    uint32_t n;
    if (!nearest(a,e,&n)) SL(0,rl(e,0xa2d46u+4u*(B(0x8a)&3u)));
}
/* 0A24F8: carry when the creator's order +60 is FD (the controller dies). */
static bool k24f8(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *p=creator(a,e);return p && ga_b(p,0x60)==0xfdu;}
/* 0A2514: +88 one step towards the creator's (20..60); carry when equal. */
static bool k2514(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    if (!p) return false;
    uint16_t d0=ga_b(p,0x88);
    if ((uint8_t)d0==B(0x88)) return true;
    d0&=0x7fu;
    if (S16(d0)<0x20) d0=0x20;
    if (S16(d0)>0x60) d0=0x60;
    if (S8(d0)>=S8(B(0x88))) SB(0x88,B(0x88)+1u); else SB(0x88,B(0x88)-1u);
    return false;
}
/* 0A2560: the target +6E; carry when it is gone and there is no player. */
static bool k2560(neo_gactor_t *a,neo_genv_t *e) {
    if (live(e,L(0x6e))) return false;
    uint32_t n;
    if (k1b04a(e,&n)) return true;
    SL(0x6e,n);return false;
}
/* 0A2584: the controller's order +60 (programs 0A2D6A). */
static bool k2584(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    if (!p) return true;
    SB(0x92,ga_b(p,0x92));
    uint8_t d0=ga_b(p,0x60);
    if ((d0&0x80u) || d0==B(0x60)) return true;
    SB(0x60,d0);d0&=0xfu;
    if (d0!=5u && d0!=4u) {SB(0x86,0);SB(0x88,0x20);}
    if (!ga_anim_start(a,e,rl(e,0xa2d6au+4u*d0),0x100)) return false;
    SB(0x61,0);
    return true;
}
static int boss_step(neo_gactor_t *a,neo_genv_t *e);
/* 0A25EA: the creator's bob +87 (and +8D) added to y for the program step;
 * +61 = FF at its end. Carry (1) when +61 is FF; -1 unsupported. */
static int k25ea(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    if (!p) return -1;
    uint8_t g=ga_b(p,0x87);SB(0x87,g);
    uint16_t d0=(uint16_t)(ga_sine(g)-8);
    SB(0x8d,ga_b(p,0x8d));
    d0=(uint16_t)(B(0x8d)?S16(d0)>>6:S16(d0)>>7);
    SW(0x40,W(0x40)+d0);
    int r=boss_step(a,e);
    if (r<0) return -1;
    if (r) SB(0x61,0xff);
    SW(0x40,W(0x40)-d0);
    return B(0x61)==0xffu;
}
static int k25e0(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *p=creator(a,e);if (!p) return -1;SB(0x17,ga_b(p,0x17));return k25ea(a,e);}
/* 0A2680: every (10000E & mask) == 0 one of the sounds 1051 / 1052. */
static void k2680(neo_gactor_t *a,neo_genv_t *e,uint16_t mask) {
    (void)a;
    if (e->video&mask) return;
    sound(e,(ga_rand(e)&1u)?0x1052:0x1051);
}
static void k26b4(neo_gactor_t *a,neo_genv_t *e) {SB(0x8a,k22fc(a,e,0xa2bf2));SW(0x82,k22fc(a,e,0xa2c02));}
/* 0A26CE: 0A1830, five 0A14FE (+85 = 0..4), 0A167C (D0 = E). */
static void k26ce(neo_gactor_t *a,neo_genv_t *e) {
    child(a,e,0xa1830,0xe);
    uint8_t d0=0xff;SB(0x85,d0);
    do {neo_gactor_t *c=child(a,e,0xa14fe,0xe);d0++;if (c) ga_sb(c,0x85,d0);} while (S8(d0)<4);
    child(a,e,0xa167c,0xe);
}
/* 0A2718: the explosion of a piece at 0A2DAA[+85]: 01DBAE (3) and 01C71E. */
static void k2718(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t t=0xa2daau+4u*(B(0x85)&7u);
    uint16_t d1=rw(e,t),d2=rw(e,t+2u);
    SW(0x28,W(0x28)+d1);SW(0x2c,W(0x2c)+d2);
    SW(0x66,0xe0ed);SW(0x1e,0xe0ed);ga_project(a,e);
    k1dbae(a,e,3);
    child_from(a,e,0x1c71e,0x2a,0,0);
    SW(0x28,W(0x28)-d1);SW(0x2c,W(0x2c)-d2);
    SW(0x66,0x8820);SW(0x1e,0x8820);ga_project(a,e);
}
/* 0A2A58: one or two fires 0A21A8 (D0 = 1C, 01B220 (0, 0)) at x +/- d0 * 16. */
static void k2a58(neo_gactor_t *a,neo_genv_t *e,uint16_t d0,uint16_t d1) {
    d0&=0x7fu;
    neo_gactor_t *c=child_from(a,e,0xa21a8,0x1c,0,0);
    uint16_t d2=d1;d1=(d0&1u)?0xfff0:0xfff4;d0=(uint16_t)(d0<<4);
    if (c) {ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+d0));ga_sw(c,0x2c,d1);ga_sw(c,0x82,d2);}
    if (!d0) return;
    c=child_from(a,e,0xa21a8,0x1c,0,0);
    if (c) {ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)-d0));ga_sw(c,0x2c,d1);ga_sw(c,0x82,d2);}
}
static void k2ae8(neo_gactor_t *a,neo_genv_t *e) {for (uint16_t d0=0;d0<=8u;d0++) k2a58(a,e,d0,0xffff);}
/* 0A2B08: one or two fires 0A21CE at x +/- d0 * 32, y -10 - d0 * 4. */
static void k2b08(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    d0&=0x7fu;
    neo_gactor_t *c=child_from(a,e,0xa21ce,0x1c,0,0);
    d0=(uint16_t)(d0<<2);uint16_t d1=(uint16_t)(0xfff0u-d0);d0=(uint16_t)(d0<<3);
    if (c) {ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+d0));ga_sw(c,0x2c,d1);}
    if (!d0) return;
    c=child_from(a,e,0xa21ce,0x1c,0,0);
    if (c) {ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)-d0));ga_sw(c,0x2c,d1);}
}
/* 0A29CE: three fires 0A21F0 (D0 = 2A). */
static void k29ce(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=child_from(a,e,0xa21f0,0x2a,0,0);
    if (c) ga_sw(c,0x2c,0);
    c=child_from(a,e,0xa21f0,0x2a,-0x20,0);
    if (c) {ga_sw(c,0x2c,0xfff8);ga_sw(c,0x82,5);}
    c=child_from(a,e,0xa21f0,0x2a,0x18,0);
    if (c) {ga_sw(c,0x2c,0xfffc);ga_sw(c,0x82,0xa);}
}
/* 0A2958: the burnt soldiers 0A2134 by x, the lists 0A2DF4 / 0A2E06. */
static void k2958(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x3c))<=0xc0) child_from(a,e,0xa2134,0xe,0x40,0x3c);
    if (S16(W(0x3c))>=0x80) child_from(a,e,0xa2134,0xe,-0x60,0x60);
    k1da40(a,e,0xa2df4);k1da40(a,e,0xa2e06);
}
/* 0A2292: palettes 11E..120 (18C..18E with +92 bit 7), +14 = +19. */
static void k2292(neo_gactor_t *a) {palettes3(a,BIT(0x92,7)?0x18c:0x11e);SB(0x14,B(0x19));}
/* The three palette byte steps of the pieces (+8F 0..2, +14 = +19[+8F]). */
static void cycle(neo_gactor_t *a) {
    SB(0x8f,B(0x8f)+1u);
    if (S8(B(0x8f))>=3) SB(0x8f,0);
    SB(0x14,B(0x19u+(B(0x8f)&3u)));
}
/* 0A27E0 / 0A281E: a shot 0A192E / 0A1958 (D0 = 1C, 01B220 (8, 10)). */
static neo_gactor_t *shot(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {
    neo_gactor_t *c=child_from(a,e,routine,0x1c,8,0x10);
    neo_gactor_t *p=creator(a,e);
    if (c && p) {
        if (routine==0xa192e) ga_sb(c,0x89,ga_b(p,0x89));
        ga_sb(c,0x88,ga_b(p,0x88));ga_sb(c,0x92,B(0x92));
    }
    return c;
}
/* The program calls of the pieces. */
static bool boss_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0xa264e: SB(0x14,B(0x19));return true;
    case 0xa2656: SB(0x14,B(0x1a));return true;
    case 0xa265e: SB(0x14,B(0x1b));return true;
    case 0xa27e0: shot(a,e,0xa192e);return true;
    case 0xa2856: {                                                         /* the fan 0A2D1A[+8B] */
        uint8_t d0=B(0x8b);SB(0x8b,d0+1u);
        if (S8(d0)>=3) {d0=0;SB(0x8b,0);}
        uint32_t t=rl(e,0xa2d1au+4u*(d0&3u));
        int16_t n=S16(rw(e,t));t+=2;
        do {
            neo_gactor_t *c=shot(a,e,0xa1958);
            uint16_t d1=rw(e,t);t+=2;
            if (c) ga_sb(c,0x88,(uint8_t)d1);
        } while (--n>0);
        return true;
    }
    case 0xa28ea: child(a,e,0xa1e92,0x2a);return true;
    case 0xa28fc: child(a,e,0xa1ea6,0x2a);return true;
    case 0xa290e: {neo_gactor_t *c=child(a,e,0xa1f0e,0x1c);if (c) ga_sb(c,0x92,B(0x92));return true;}
    }
    return false;
}
static int boss_step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,boss_call);}
/* ---- the controller 0A0C00 ---------------------------------------------------------- */
static ga_result_t controller_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c;uint32_t n;uint8_t d0b=0;
    switch (L(0)) {
    case 0xa0c00:
        c=child(a,e,0x59f76,0);if (c) ga_sw(c,0x72,0x2c);                 /* 059F4E: music 2C */
        SB(0x87,2);
        k26ce(a,e);k2312(a,e);SB(0x17,0);
        SW(0x4e,rank(a,e,0x2c2b08));SW(0x7e,rank(a,e,0x2c2b6a));
        k1b04a(e,&n);SL(0x6e,n);
        ga_palette(a,0x107);ga_flash_palette(a,0x149);
        SW(0x66,0x8820);SW(0x1e,0x8820);SET(0x92,0);
        k1dd50(a,e,0x40,0x48,0x3f,0x1f);
        SB(0x91,0);k29ce(a,e);SW(0x82,0x3c);SL(0,0xa0c9a);
        /* fall through */
    case 0xa0c9a:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) {SB(0x91,1);SL(0,0xa0cc4);}
        k2680(a,e,0x3f);return done(e);
    case 0xa0cc4:
        SB(0x91,0);k2ae8(a,e);SW(0x82,0x74);SL(0,0xa0cd8);
        /* fall through */
    case 0xa0cd8:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) SL(0,0xa0cfc);
        k2680(a,e,0x3f);return done(e);
    case 0xa0cfc:
        for (uint16_t i=0;i<5u;i++) k2b08(a,e,i);
        SL(0x34,0x10000);SL(0,0xa0d1c);
        /* fall through */
    case 0xa0d1c:
        ga_project(a,e);ga_move(a,e);
        if (S16(W(0x2c))>=-0x28) {SL(0x34,0);SL(0,0xa0d9c);SB(0x91,1);CLR(0x92,0);}
        k2680(a,e,0x1f);
        goto t123c;
    case 0xa0d9c: hover:
        k2312(a,e);k26b4(a,e);
        /* fall through */
    case 0xa0d54:
        SL(0,0xa0d5a);
        /* fall through */
    case 0xa0d5a:
        ga_project(a,e);
        SL(0x30,L(0x30)-(uint32_t)((int32_t)L(0x30)>>3));
        if (!k2560(a,e)) SL(0,0xa0d80);
        k24c6(a,e);goto t11fa;
    case 0xa0d80:
        SL(0,0xa0d86);
        /* fall through */
    case 0xa0d86:
        if (k2560(a,e)) SL(0,0xa0d54);
        k24c6(a,e);goto t11e2;
    /* attack 0 */
    case 0xa0da6:
        CLR(0x92,7);SB(0x60,1);SB(0x61,0);SL(0,0xa0dbc);
        /* fall through */
    case 0xa0dbc:
        if (!B(0x61)) goto t11e2;
        sound(e,0x1074);SB(0x60,4);SB(0x61,0);SL(0,0xa0de2);
        /* fall through */
    case 0xa0de2:
        if (!B(0x61)) goto t11e2;
        sound(e,0x10d0);child_from(a,e,0xa1df2,0x1c,0,0);                   /* 0A27BC */
        SW(0x82,k22fc(a,e,0xa2c12));SL(0,0xa0e0e);
        /* fall through */
    case 0xa0e0e:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) SL(0,0xa0e2c);
        goto t11f0;
    case 0xa0e2c:
        sound(e,0x1098);SB(0x8d,0x30);SB(0x87,0xc2);                       /* 0A2666 */
        child(a,e,0xa1c0c,0x1c);child(a,e,0xa1c4c,0x1c);child(a,e,0xa1b3a,0xe);child(a,e,0xa1d20,0xe);child(a,e,0xa1bec,0x1c);   /* 0A2898 */
        SB(0x60,5);SB(0x61,0);SW(0x82,0x3c);SL(0,0xa0e54);
        /* fall through */
    case 0xa0e54:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) SL(0,0xa0e72);
        goto t11f0;
    case 0xa0e72:
        SB(0x88,0x20);sound(e,0x1074);SB(0x60,4);SB(0x61,0);SL(0,0xa0e92);
        /* fall through */
    case 0xa0e92:
        if (!B(0x61)) {ga_project(a,e);goto t11f0;}
        SB(0x60,2);SB(0x61,0);SL(0,0xa0eb4);
        /* fall through */
    case 0xa0eb4:
        if (B(0x61)) goto hover;
        ga_project(a,e);goto t11f0;
    /* attack 1 */
    case 0xa0ec6:
        CLR(0x92,7);CLR(0x92,6);SB(0x60,8);SB(0x61,0);SL(0,0xa0ee2);
        /* fall through */
    case 0xa0ee2:
        if (!B(0x61)) goto t11e2;
        SB(0x84,k22fc(a,e,0xa2c22));SW(0x76,k22fc(a,e,0xa2c42));
        /* fall through */
    case 0xa0f06:
        SB(0x60,BIT(0x84,0)?0xb:0xa);SB(0x61,0);
        sound(e,0x1097);SW(0x82,k22fc(a,e,0xa2c32));SL(0,0xa0f44);
        /* fall through */
    case 0xa0f44:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<0 && B(0x61)) {
            SL(0,0xa0f82);SB(0x84,B(0x84)-1u);
            if (S8(B(0x84))>0) SL(0,0xa0f06);
            k241a(a,e);
        }
        goto t11f0;
    case 0xa0f82:
        SB(0x60,9);SB(0x61,0);SL(0,0xa0f92);
        /* fall through */
    case 0xa0f92:
        if (B(0x61)) goto hover;
        ga_project(a,e);goto t11f0;
    /* attack 3 */
    case 0xa0fa4:
        SET(0x92,7);SB(0x60,8);SB(0x61,0);SL(0,0xa0fba);
        /* fall through */
    case 0xa0fba:
        if (!B(0x61)) goto t11e2;
        SB(0x84,k22fc(a,e,0xa2ca2));SW(0x76,k22fc(a,e,0xa2cc2));
        /* fall through */
    case 0xa0fde: {
        uint32_t p=L(0x7a);
        for (unsigned guard=0;;guard++) {
            if (!p || guard) p=0x2c3f1eu+4u*(k22fc(a,e,0xa2cd2)&3u);
            d0b=rb(e,p);p++;
            if (!(d0b&0x80u) || e->unsupported || guard>64) break;
        }
        if (d0b) SET(0x92,6); else CLR(0x92,6);
        SL(0x7a,p);
        SB(0x60,BIT(0x84,0)?0xb:0xa);SB(0x61,0);
        sound(e,0x1097);SW(0x82,k22fc(a,e,0xa2cb2));SL(0,0xa105e);
    }   /* fall through */
    case 0xa105e:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<0 && B(0x61)) {
            SL(0,0xa109c);SB(0x84,B(0x84)-1u);
            if (S8(B(0x84))>0) SL(0,0xa0fde);
            k241a(a,e);
        }
        goto t11f0;
    case 0xa109c:
        SB(0x60,9);SB(0x61,0);SL(0,0xa10ac);
        /* fall through */
    case 0xa10ac:
        if (B(0x61)) goto hover;
        ga_project(a,e);goto t11f0;
    /* attack 2 */
    case 0xa10be:
        CLR(0x92,7);CLR(0x92,6);SB(0x60,8);SB(0x61,0);SL(0,0xa10da);
        /* fall through */
    case 0xa10da:
        if (!B(0x61)) goto t11e2;
        sound(e,0x10d0);child_from(a,e,0xa1e7e,0x1c,0,0);                   /* 0A2798 */
        SW(0x82,k22fc(a,e,0xa2c82));SL(0,0xa1106);
        /* fall through */
    case 0xa1106:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) SL(0,0xa1124);
        goto t11e2;
    case 0xa1124:
        SB(0x84,k22fc(a,e,0xa2c52));SB(0x8b,k22fc(a,e,0xa2c92));SW(0x76,k22fc(a,e,0xa2c72));
        /* fall through */
    case 0xa1148:
        SB(0x60,BIT(0x84,0)?0xd:0xc);SB(0x61,0);
        sound(e,0x1097);SW(0x82,k22fc(a,e,0xa2c62));SL(0,0xa1186);
        /* fall through */
    case 0xa1186:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<0 && B(0x61)) {
            SL(0,0xa11c0);SB(0x84,B(0x84)-1u);
            if (S8(B(0x84))>0) SL(0,0xa1148);
        }
        goto t11e2;
    case 0xa11c0:
        SB(0x60,9);SB(0x61,0);SL(0,0xa11d0);
        /* fall through */
    case 0xa11d0:
        if (B(0x61)) goto hover;
        ga_project(a,e);goto t11f0;
    /* death */
    case 0xa12c0:
        if (e->dyn_release) e->dyn_release(e->ctx,a);                      /* 013F1E */
        k18f92(a,e);
        sound(e,0x1020);sound(e,0x10d0|0x8000u);sound(e,0x1098|0x8000u);sound(e,0x10d0|0x8000u);sound(e,0x1097|0x8000u);   /* 012192 */
        child_from(a,e,0xa18d8,0xe,0,0);
        c=child_from(a,e,0xa1756,0xe,0,0x78);
        if (c) ga_sb(c,0x60,B(0x60));
        SB(0x60,0xfd);SB(0x61,0);SB(0x91,0);
        k2ae8(a,e);k2958(a,e);k1da40(a,e,0xa2dbe);
        SL(0x30,0);SL(0x34,0);SW(0x82,0x78);SL(0x34,0xfffff000u);SET(0x5f,7);
        START(0x2a2768);flash(e,1);
        SL(0,0xa13ac);goto t13cc;
    case 0xa13ac:
        SL(0,0xa13b6);goto t13cc;
    case 0xa13b6:
        flash(e,0);shake(e);SL(0,0xa13cc);
        /* fall through */
    case 0xa13cc: t13cc:
        SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) {sound(e,0x1026);shake(e);SL(0x34,0xffff0000u);SL(0,0xa13f8);}
        /* fall through */
    case 0xa13f8:
        ga_project(a,e);ga_move(a,e);
        if (boss_step(a,e)<0) return GA_UNSUPPORTED;
        if (S16(W(0x40))<=0x70) {SB(0x91,1);SL(0,0xa142a);}
        k2680(a,e,0x1f);return done(e);
    case 0xa142a:
        SB(0x91,0);k29ce(a,e);SW(0x82,0x1e);SL(0,0xa143e);
        /* fall through */
    case 0xa143e:
        ga_project(a,e);SW(0x82,W(0x82)-1u);
        if (S16(W(0x82))<=0) {
            SB(0x91,1);
            if (e->flags) e->flags->stage_done=1; else STOP(0x18f40);        /* 018F40 */
            SL(0,0xa1466);
        }
        return done(e);
    case 0xa1466:return GA_DELETE;                                        /* cartridge check, 004188 */
    case 0xa14f6:return GA_DELETE_TREE;                                   /* 0041AC */
    default:STOP(L(0));
    }
t11e2:
    ga_project(a,e);k241a(a,e);goto t11fa;
t11f0:
    ga_project(a,e);k244c(a);
t11fa:
    ga_move(a,e);
    if (W(0x80)) {SW(0x4e,W(0x4e)-W(0x80));SW(0x80,0);}
    if (S16(W(0x4e))<0) {SET(0x5f,7);SB(0x5e,B(0x90));}
    if (BIT(0x5f,7)) SL(0,0xa12c0);
    SB(0x87,B(0x87)+4u);
t123c:
    if (B(0x8c)) {SB(0x17,B(0x8c));SB(0x8c,0);if (B(0x8d)==0x30u) SB(0x87,0xc2);}
    if (S8(B(0x17))>0) SB(0x17,B(0x17)-1u);
    if (B(0x8d)) SB(0x8d,B(0x8d)-1u);
    if (S16(W(0x7e))>=S16(W(0x4e))) {
        if (!B(0x89)) SW(0x7e,rank(a,e,0x2c2bcc));
        else {
            SW(0x7e,0xff00);
            child_from(a,e,0xa203a,0x1c,0,0x96);k1da40(a,e,0xa2de2);    /* 0A2926 */
        }
        SB(0x89,B(0x89)+1u);k2312(a,e);k26b4(a,e);
    }
    SB(0x90,0);
    if (e->dyn_set && !e->unsupported) e->dyn_set(e->ctx,a,0xa2e40,0,0); /* 0A2674: 0140C0 */
    return done(e);
}
/* ---- the pieces ---------------------------------------------------------------------- */
static ga_result_t piece_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *p;uint16_t d0,d1;uint32_t n;uint8_t g;
    switch (L(0)) {
    /* 0A14FE: the hurtboxes */
    case 0xa14fe:
        on_creator(a,e,0,0);SB(0x17,0);
        SW(0x4e,rank(a,e,0x2c2c2e));SW(0x80,W(0x4e));SW(0x7e,rank(a,e,0x2c2c90));
        ga_palette(a,0x107);ga_flash_palette(a,0x149);SW(0x66,0x8820);SW(0x1e,0x8820);
        START(rl(e,0xa2d56u+4u*(B(0x85)&7u)));SL(0,0xa1578);
        /* fall through */
    case 0xa1578:
        if (k24f8(a,e)) return GA_DELETE;
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        if (ga_b(p,0x60)==0xfeu) SB(0x86,3);
        if (!(ga_b(p,0x92)&1u) && ga_register(a,e,NULL,NULL)) {
            SB(0x5a,0);
            ga_sb(p,0x8c,B(0x17));ga_sb(p,0x90,(uint8_t)(ga_b(p,0x90)|B(0x5e)));SB(0x5e,0);
            d0=W(0x80);d1=W(0x4e);SW(0x80,d1);d0=(uint16_t)(d0-d1);
            if (S16(d0)>S16(ga_w(p,0x80))) ga_sw(p,0x80,d0);
            if (S8(B(0x86))<3 && S16(d1)<=S16(W(0x7e))) {
                SB(0x86,B(0x86)+1u);
                SW(0x7e,B(0x86)==1u?rank(a,e,0x2c2cf2):rank(a,e,0x2c2d24));
                ga_sb(p,0x8d,0x30);
                if (S8(B(0x8d))<=0x24) {SB(0x8d,0x30);k2718(a,e);}
                sound(e,0x1021);
            }
            if (BIT(0x5f,7)) {SW(0x4e,0x64);SW(0x80,0x64);CLR(0x5f,7);}
        }
        if (B(0x8d)) SB(0x8d,B(0x8d)-1u);
        ga_project(a,e);
        if (k25e0(a,e)<0) return GA_UNSUPPORTED;
        return done(e);
    /* 0A167C: the body */
    case 0xa167c:
        on_creator(a,e,0,0x68);
        ga_palette(a,0x107);ga_flash_palette(a,0x149);SW(0x66,0x8820);SW(0x1e,0x8820);SW(0x1e,W(0x1e)-1u);
        START(0x2a2882);
        /* fall through */
    case 0xa16c0:
        SL(0,0xa16c6);
        /* fall through */
    case 0xa16c6:
        if (k24f8(a,e)) return GA_DELETE;
        ga_project(a,e);
        if (!k2584(a,e)) return GA_UNSUPPORTED;
        if (k25e0(a,e)<0) return GA_UNSUPPORTED;
        k2324(a,e);
        if (B(0x60)==4u) SL(0,0xa16f2);
        return done(e);
    case 0xa16f2:
        SL(0,0xa16f8);
        /* fall through */
    case 0xa16f8:
        if (k24f8(a,e)) return GA_DELETE;
        ga_project(a,e);
        SB(0x61,k2514(a,e)?0xff:0);k2324(a,e);
        g=(uint8_t)((uint8_t)(B(0x88)-0x20u)>>2);SB(0x86,g);
        p=creator(a,e);if (p) ga_sb(p,0x86,g);
        if (!k2584(a,e)) return GA_UNSUPPORTED;
        if (k25e0(a,e)<0) return GA_UNSUPPORTED;
        if (B(0x60)!=4u) SL(0,0xa16c0);
        return done(e);
    /* 0A1756: the falling piece of the death */
    case 0xa1756:
        ga_palette(a,0x107);ga_flash_palette(a,0x149);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        SL(0x34,0x44444);SL(0x76,0xffffb72fu);
        SL(0x30,BIT(0x15,0)?0xfffddddeu:0x22222u);
        START(S8(B(0x60))>=8?0x2a2c8a:0x2a2c7e);SL(0,0xa17e2);
        /* fall through */
    case 0xa17e2:
        SL(0x34,L(0x34)+L(0x76));ga_project(a,e);ga_move(a,e);STEP(r);
        if (S16(W(0x2c))<=0x20) {release(a);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SL(0,0x1c71e);}
        if (S16(W(0x40))<=0x80) SL(0,0xa1466);
        return done(e);
    /* 0A1830 / 0A18D8: the lights */
    case 0xa1830:
        on_creator(a,e,0,0);ga_palette(a,0x108);SW(0x66,0x8820);SW(0x1e,0x8820);START(0x2a38ce);SL(0,0xa186a);
        /* fall through */
    case 0xa186a:
        if (k24f8(a,e)) return GA_DELETE;
        ga_project(a,e);
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        if (ga_b(p,0x8d)) {if (!B(0x8d)) {START(0x2a38fc);SB(0x8d,1);}}
        else if (B(0x8d)) {START(0x2a38ce);SB(0x8d,0);}
        if (S16(ga_w(p,0x2c))>=-0x28) STEP(r);
        return done(e);
    case 0xa18d8:
        on_creator(a,e,0,0);ga_palette(a,0x108);SW(0x66,0x8820);SW(0x1e,0x8820);START(0x2a38fc);SL(0,0xa1912);
        /* fall through */
    case 0xa1912:
        ga_project(a,e);
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        if (S16(ga_w(p,0x2c))>-0x30) STEP(r);
        return done(e);
    /* 0A192E / 0A1958: the shots */
    case 0xa192e:
        if (S8(B(0x89))>=1) {
            if (nearest(a,e,&n)) g=0x40;
            else if (!aim(a,e,n,&g)) return GA_UNSUPPORTED;
            SB(0x88,g&0xf8u);
        }
        /* fall through */
    case 0xa1958: {
        if (BIT(0x92,7)) SB(0x88,0x40);
        g=B(0x88);
        int16_t c1=ga_cosine(g),s1=ga_sine(g);
        p=creator(a,e);neo_gactor_t *q=p?creator(p,e):NULL;
        if (!q) return GA_UNSUPPORTED;
        int16_t v=S16(ga_w(q,0x76));
        SL(0x30,(uint32_t)((int32_t)c1*v));SL(0x34,(uint32_t)((int32_t)s1*v));
        SL(0x62,(uint32_t)((int32_t)v<<8));
        SW(0x28,W(0x28)+(uint16_t)(c1>>7));SW(0x2c,W(0x2c)+(uint16_t)(s1>>7));
        SET(0x15,5);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
        k2292(a);SW(0x82,0);START(0x2a3816);SL(0,0xa19ea);
    }   /* fall through */
    case 0xa19ea:
        ga_project(a,e);
        if (!W(0x82)) ga_move(a,e);
        else {SW(0x82,W(0x82)-1u);if (!W(0x82)) START(0x2a3816);}
        STEP(r);
        cycle(a);
        if (BIT(0x92,7)) {
            int16_t y=S16(W(0x2c));
            if (y>=(BIT(0x92,6)?0xb0:0x98)) {
                CLR(0x92,7);START(0x2a386c);SW(0x82,k22fc(a,e,0xa2ce2));SL(0x34,0);
                uint32_t v=L(0x62);
                if (nearest(a,e,&n)) {if (S16(W(0x30))<0) v=0u-v;}
                else if (S16((uint16_t)(px(e,n)-W(0x3c)))<0) v=0u-v;
                SL(0x30,v);
            }
        }
        if (ga_attack(a,e,0xa2bba)) SL(0,0xa1b14);
        if (S16(W(0x40))>=0x1e8) SL(0,0xa1b1e);
        if (e->flags && e->flags->mission_end) SL(0,0xa1b1e);              /* 019424 */
        if (ga_cull_rect(a,e,0xa2bda)) SL(0,0xa14f6);
        return done(e);
    case 0xa1b14:
        sound(e,0x1022);
        /* fall through */
    case 0xa1b1e:
        release(a);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);
        SL(0,0x1c706);return neo_effects_tick(a,e);
    /* 0A1B3A / 0A1BEC / 0A1C0C / 0A1C4C: the beam's pieces */
    case 0xa1b3a:
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);START(0x2a3172);d0=0;d1=0x68;goto t1b5c;
    case 0xa1bec:
        SW(0x66,0x8820);SW(0x1e,0x8820);START(0x2a3458);d0=0;d1=0x68;goto t1b5c;
    case 0xa1c0c: case 0xa1c4c: {
        bool second=L(0)==0xa1c4cu;
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);START(second?0x2a359a:0x2a3572);
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        uint32_t t=0xa2e18u+8u*((ga_b(p,0x86)>>2)&7u)+(second?4u:0u);
        d0=rw(e,t);d1=(uint16_t)(rw(e,t+2u)+0x68u);
        goto t1b5c;
    }
    t1b5c:
        on_creator(a,e,S16(d0),S16(d1));
        palettes3(a,0x11e);SB(0x14,B(0x19));CLR(0x15,0);SL(0,0xa1b9e);
        /* fall through */
    case 0xa1b9e:
        if (k24f8(a,e)) return GA_DELETE;
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        SB(0x86,ga_b(p,0x86)>>2);
        if (S16(ga_w(p,0x82))<=0) return GA_DELETE;
        ga_project(a,e);
        if (k25ea(a,e)<0) return GA_UNSUPPORTED;
        cycle(a);
        return done(e);
    /* 0A1D20: the beam */
    case 0xa1d20:
        START(0x2a38c2);SL(0,0xa1d36);
        /* fall through */
    case 0xa1d36:
        if (k24f8(a,e)) return GA_DELETE;
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        if (S16(ga_w(p,0x82))<=0) return GA_DELETE;
        on_creator(a,e,0,0x58);ga_project(a,e);
        g=(uint8_t)(ga_b(p,0x88)&0xfcu);
        if (g!=B(0x88)) {
            SB(0x88,g);
            SL(0x72,(uint32_t)((int32_t)ga_cosine(g)*0x1000));SL(0x76,(uint32_t)((int32_t)ga_sine(g)*0x1000));
        }
        SL(0x30,L(0x72));SL(0x34,L(0x76));
        for (unsigned guard=0;;guard++) {
            ga_attack(a,e,0xa2bca);ga_move(a,e);STEP(r);
            if (S16(W(0x40))>=0x200) break;
            if (guard>4096) STOP(0xa1dae);
        }
        return done(e);
    /* 0A1DF2 / 0A1E7E */
    case 0xa1df2: case 0xa1e7e:
        START(L(0)==0xa1df2u?0x2a2cd8:0x2a2c96);
        on_creator(a,e,0,0x68);palettes3(a,0x11e);SB(0x14,B(0x19));CLR(0x15,0);
        SW(0x66,0x8820);SW(0x1e,0x8820);SL(0,0xa1e56);
        /* fall through */
    case 0xa1e56:
        if (k24f8(a,e)) return GA_DELETE;
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        SB(0x86,ga_b(p,0x86));
        if (S16(ga_w(p,0x82))<=0) return GA_DELETE;
        ga_project(a,e);
        if (k25ea(a,e)<0) return GA_UNSUPPORTED;
        return done(e);
    /* 0A1E92 / 0A1EA6 */
    case 0xa1e92: case 0xa1ea6:
        START(L(0)==0xa1e92u?0x2a35c8:0x2a3642);
        ga_palette(a,0x135);SW(0x66,0x8820);SW(0x1e,0x8820);
        on_creator(a,e,0,0);
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        SL(0x0c,ga_l(p,0x0c));CLR(0x15,0);SL(0,0xa1ef0);
        /* fall through */
    case 0xa1ef0:
        if (k24f8(a,e)) return GA_DELETE;
        ga_project(a,e);
        if ((r=k25ea(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa1466);
        return done(e);
    /* 0A1F0E */
    case 0xa1f0e:
        on_creator(a,e,0,0);k2292(a);
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        SL(0x0c,ga_l(p,0x0c));SB(0x8f,0);CLR(0x15,0);SW(0x66,0x8820);SW(0x1e,0x8820);
        START(0x2a36f8);SL(0,0xa1f52);
        /* fall through */
    case 0xa1f52:
        if (k24f8(a,e)) return GA_DELETE;
        ga_project(a,e);cycle(a);
        if ((r=k25ea(a,e))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0xa1466);
        return done(e);
    /* 0A1F98 (list 01D764): gone with a dead controller, else the debris 0A203A */
    case 0xa1f98:
        p=creator(a,e);p=p?creator(p,e):NULL;
        if (!p) return GA_UNSUPPORTED;
        if (ga_b(p,0x5f)&0x80u) return GA_DELETE;                          /* cartridge check, 004188 */
        /* fall through */
    case 0xa203a:
        ga_floor(a,e);SL(0x30,0);SL(0x34,0);SL(0x62,0);                    /* 0146E8, 01B1DC */
        SW(0x6e,ga_rand(e)&0xfu);
        {uint32_t v=0x10000u;if (ga_rand(e)&0x80u) {v=0u-v;SET(0x15,0);}SL(0x62,v);}
        SW(0x66,0x8800);SW(0x1e,0x8800);ga_palette(a,0x4c2);START(0x23bffc);ga_project(a,e);
        SL(0,0xa20ac);
        /* fall through */
    case 0xa20ac:
        if (S16(W(0x2c))>S16(W(0x6e))) {
            if (BIT(0x15,6)) {
                uint32_t kind=0;ga_ground(a,e,&kind);
                if ((uint16_t)(kind>>16)==5u) SW(0x6e,W(0x2c));
            } else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}          /* 0143DC */
            goto t2112;
        }
        SL(0x34,0);SL(0x30,L(0x62));START(0x23bfba);SL(0,0xa2106);
        /* fall through */
    case 0xa2106:
        ga_project(a,e);ga_move(a,e);
    t2112:
        depth(a);STEP(r);
        if (ga_cull_rect(a,e,0xa2bda)) SL(0,0xa14f6);
        return done(e);
    /* 0A2134: the burnt soldiers */
    case 0xa2134:
        SET(0x5f,6);ga_project(a,e);
        d1=(uint16_t)((ga_rand(e)&0x1fu)+0xfffcu);
        d0=0x100;
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        if (S16(ga_w(p,0x3c))>S16(W(0x3c))) d0=(uint16_t)(0u-d0);
        SW(0x6e,0xffff);SW(0x70,d0);SW(0x72,0xffff);SW(0x74,0);SW(0x76,1);SW(0x78,d1);
        SW(0x66,0x8800);SW(0x1e,0x8800);ga_palette(a,0x87c);SL(0,0x99090);
        return done(e);
    /* 0A21A8 / 0A21CE / 0A21F0: the fires */
    case 0xa21a8:
        if (!e->front_layer) STOP(0x12022);
        if (e->front_layer(e->ctx)) SET(0x15,2); else CLR(0x15,2);         /* 012022 */
        SW(0x66,0xffff);SW(0x1e,0xffff);depth(a);SB(0x91,0);n=0x22df54;goto t2206;
    case 0xa21ce:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);depth(a);SB(0x91,1);n=0x22e1f0;goto t2206;
    case 0xa21f0:
        SW(0x66,0x1001);SW(0x1e,0x1001);SB(0x91,0);n=0x22e750;
    t2206:
        if (S16(W(0x82))<0) SW(0x82,ga_rand(e)&0xfu);
        SL(0x6e,n);START(n);
        {uint16_t s=neo_gate_splash_palette(e);if (!s) STOP(0x19240);ga_palette(a,s);}
        SL(0,0xa2240);
        /* fall through */
    case 0xa2240:
        p=creator(a,e);if (!p) return GA_UNSUPPORTED;
        if (ga_b(p,0x91)) SB(0x91,ga_b(p,0x91));
        ga_project(a,e);
        if (S16(W(0x82))>=0) {SW(0x82,W(0x82)-1u);return done(e);}
        STEP(r);
        if (r) {if (B(0x91)) return GA_DELETE;START(L(0x6e));}
        return done(e);
    case 0xa1466:return GA_DELETE;                                        /* cartridge check, 004188 */
    case 0xa14f6:return GA_DELETE_TREE;                                   /* 0041AC */
    default:STOP(L(0));
    }
}
/* ---- 099090: the burnt soldier (0A2134) ---------------------------------------------- */
/* 09954A: the throw 0995A6[+6E] (random when negative): +34, +78, +30 by the facing. */
static void k9954a(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=W(0x6e);
    if (S16(d0)<0) d0=ga_rand(e);
    uint32_t t=0x995a6u+12u*(d0&3u);
    SL(0x34,rl(e,t));SL(0x78,rl(e,t+4u));
    uint32_t v=rl(e,t+8u);
    if (!BIT(0x15,0)) v=0u-v;
    SL(0x30,v);
}
static ga_result_t burnt_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x99090: {
        uint16_t s=neo_gate_splash_palette(e);if (!s) STOP(0x19240);ga_palette(a,s);   /* 019240 */
        SB(0x14,B(0x19));
        if (S16(W(0x70))>=0) SET(0x15,0);
        uint16_t d0=W(0x72);if (S16(d0)<0) d0=ga_rand(e);
        SW(0x72,d0&1u);
        if (W(0x76)) goto t9910c;
        START(rl(e,0x995d6u+4u*W(0x72)));SL(0,0x990f2);
    }   /* fall through */
    case 0x990f2:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x9910c);
        goto t92ac;
    case 0x9910c: t9910c:
        k9954a(a,e);START(rl(e,0x995deu+4u*W(0x72)));SL(0,0x99130);
        /* fall through */
    case 0x99130:
        if (W(0x74) && S16(W(0x34))<0) {SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0,0x99152);}
        /* fall through */
    case 0x99152:
        SL(0x34,L(0x34)+L(0x78));ga_air(a,e);STEP(r);
        if (BIT(0x15,6)) SL(0,0x9917a);
        goto t92ac;
    case 0x9917a:
        SB(0x14,B(0x1a));START(0x29f3dc);SL(0,0x99196);
        /* fall through */
    case 0x99196:
        ga_stationary(a,e);depth(a);STEP(r);
        if (r) SL(0,0x991b6);
        goto t92ce;
    case 0x991b6:
        SB(0x14,B(0x19));START(0x29f2b6);SL(0,0x991d2);
        /* fall through */
    case 0x991d2:
        ga_stationary(a,e);depth(a);STEP(r);
        if (r) SL(0,0x991f2);
        goto t92ce;
    case 0x991f2:
        SL(0x30,(uint32_t)((int32_t)S16(W(0x70))<<8));SL(0x34,0);START(0x29f23e);SL(0,0x99218);
        /* fall through */
    case 0x99218:
        ga_air(a,e);STEP(r);
        if (ga_register(a,e,NULL,NULL)) SL(0,0x99238);
        goto t92ce;
    case 0x99238:
        START(0x29f286);SL(0,0x9924e);
        /* fall through */
    case 0x9924e:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x99268);
        goto t92ce;
    case 0x99268:
        release(a);ga_palette(a,0x20c);START(0x22e750);SL(0,0x99292);
        /* fall through */
    case 0x99292:
        ga_stationary(a,e);STEP(r);
        if (r) SL(0,0x992e4);
        goto t92ce;
    case 0x992e4:return GA_DELETE;                                        /* cartridge check, 004188 */
    default:STOP(L(0));
    }
t92ac:
    if (!BIT(0x5f,6)) {if (ga_register(a,e,NULL,NULL)) STOP(0x406b6);}  /* 0406B6: not on this path (0A2134 sets +5F bit 6) */
    else SB(0x5a,0);
t92ce:
    if (ga_cull(a,e,0x9959e)) SL(0,0x992e4);                              /* 01945C */
    return done(e);
}
static bool burnt_routine(uint32_t r) {
    switch (r) {
    case 0x99090: case 0x990f2: case 0x9910c: case 0x99130: case 0x99152: case 0x9917a: case 0x99196: case 0x991b6:
    case 0x991d2: case 0x991f2: case 0x99218: case 0x99238: case 0x9924e: case 0x99268: case 0x99292: case 0x992e4: return true;
    }
    return false;
}
static bool controller_routine(uint32_t r) {return (r>=0xa0c00u && r<0xa14feu) || r==0xa14f6u;}
bool neo_m5boss_routine(uint32_t r) {return (r>=0xa0c00u && r<0xa22fcu) || burnt_routine(r);}
ga_result_t neo_m5boss_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (burnt_routine(r)) return burnt_tick(a,e);
    if (controller_routine(r) && r!=0xa1466u) return controller_tick(a,e);
    return piece_tick(a,e);
}
