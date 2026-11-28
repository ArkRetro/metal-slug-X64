#include "neo_region.h"
/* Mission 5 metro: the train 051344 (script record 2DDF60, camera 528) and its
 * children, manual translation of ROM 050FB4..052856 (ROM labels as provenance).
 *
 * - 051344: +70 updates of 01436A first; then 050FCC (palettes 10F, 116, 115,
 *   110; +14 = +19), program 248CD6, layers 5, 0146E8, the wheels 0515CE and the
 *   bogies 051688 / 051690 (0040BC, reserve 1C), +4E = rank 2D388E (health),
 *   +8C = (+4E & 3FFF) / 5, +8A = +4E - +8C (the next damage stage), +30 = +72 =
 *   -(rank 2D38F0 << 8), +50 = 050D76, +90 = +4E, the sound 1078.
 * - 051430 every update: +A2 (the cooldown of the sound 107C) counts down;
 *   004C24 (a hit: 05100C, the flash list 050F04 / 050F22 by stage; +9C / +9D =
 *   +5A / +5B, +5A = 0); 05117A (the flash list) and 05112E (palette cycles of
 *   +9F = 4 / 2); +A1 -> +14; below stage 5 051220 (the push of the hit by the
 *   weapon's kind +9C: ranks 2D390A.., 2 px right from x 180) and the cell map
 *   050D82 at (-78, 0) (0140C4); 0511D2 (+4E under +8A: the next stage, program
 *   050ECC[stage]); stage 6 falls (+34 - 5000); 01AC38; 00427A: its end ->
 *   0514EA (009BB0(1), the sound 1034, the pieces 0525EE, the debris 050F62,
 *   +9B = FF, the item 08CDB2 at (-20, 58) of the kind +6E; 051530: 009BB0(0),
 *   019DFC, 004188); left of x -80: 013F1E, 019DFC, the sound 1078 stopped,
 *   0041AC.
 * - The calls of its programs: 0517D0 / 0518C4 / 051A1E / 051B20 (the sound
 *   1030, the panels 051772 and the blasts 01C706 / 01C6EE after the script
 *   actor), 0521B4 (the sparks 051DF2), 0521D0 (stage 6: the jump), 0521F2 /
 *   0522E4 / 0523D6 (blasts 01C6EE), 0524C8 (the smoke 051C00, +9F = 3),
 *   0524E0 (six blasts 01C71E), 0525C6 (the piece 052128), 052840 (one update a
 *   4 px bob: 0527A2).
 *
 * 08EF46-style cartridge checks (RAM 10F00C + 3EF, on a carry the test of
 * 2FFFE0) are not reproduced: MAME's protection patch leaves their bne as NOPs
 * and every one ends in 004188 (as neo_m5enemy.c). */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,v)
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static int16_t S16v(uint16_t v) {return (int16_t)v;}
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;} return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2u);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {   /* 0260C8 */
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* 0040BC with its reserve D0 (no placement). */
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
/* 00411C after the script actor 101414, 01ACF2 at (dx, dy), layers 28. */
static void blast(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,routine,true):NULL;
    if (!c) return;
    ga_place_at(c,a,e,dx,dy);ga_sw(c,0x66,0x28);ga_sw(c,0x1e,0x28);
}
/* 01AD6E: the position block on the creator's at (dx, dy). */
static void on_creator(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {
    neo_gactor_t *p=creator(a,e);ga_place(a,e,dx,dy,p?NEO_GA_TAG(p->index):0);
}
/* 050FB4: layers = the creator's + D0. */
static void layers(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    neo_gactor_t *p=creator(a,e);
    if (!p) {ga_fail(e,0x50fb4);return;}
    SW(0x1e,ga_w(p,0x1e)+d0);SW(0x66,ga_w(p,0x66)+d0);
}
static void pals(neo_gactor_t *a,const uint16_t r[4]) {for (unsigned i=0;i<4;i++) ga_palette(a,r[i]);SB(0x14,B(0x19));}
static const uint16_t P_FCC[4]={0x10f,0x116,0x115,0x110},P_032[4]={0x110,0x111,0x116,0x10f},P_082[4]={0x110,0x115,0x116,0x10f},P_0D4[4]={0x116,0x115,0x10f,0x111};
/* 051032 / 051082 / 0510D4 (009F88, four palettes, +14 = +19): +9F = 1 / 2 / 4. */
static void k51032(neo_gactor_t *a) {SL(0x7a,0);SB(0x1d,0);pals(a,P_032);SB(0x9f,1);}
static void k51082(neo_gactor_t *a) {SB(0x1d,0);pals(a,P_082);SB(0xa0,0);SB(0x9f,2);}
static void k510d4(neo_gactor_t *a) {SB(0x1d,0);pals(a,P_0D4);SB(0xa0,0);SB(0x9f,4);}
/* 05100C: the flash list of the stage (050F04 below 3, else 050F22). */
static void k5100c(neo_gactor_t *a) {
    if (L(0x7a)) return;
    SL(0x7a,S16v(W(0x8e))>=3?0x50f22u:0x50f04u);SB(0x98,1);
}
/* 05112E: the palette cycles of +9F = 4 (+19..+1C by turns) and 2 (table 051126). */
static void k5112e(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x9f)==4u) {SB(0xa0,B(0xa0)+1u);SB(0x14,B(0x19u+(B(0xa0)&3u)));return;}
    if (B(0x9f)==2u) {
        SB(0xa0,B(0xa0)+1u);
        uint8_t d0=B(0xa0);
        if (d0&0x80u) {SB(0xa0,0);d0=0;}
        const uint8_t *t=ga_rom(0x51126u+(d0&3u));
        if (!t) {ga_fail(e,0x51126);return;}
        SB(0x14,B(0x19u+t[0]));
    }
}
/* 05117A: the flash list +7A (pairs palette, time; FF ends). */
static void k5117a(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x8e)>=5u || B(0x9f) || !L(0x7a)) return;
    SB(0x98,B(0x98)-1u);
    if (B(0x98)) return;
    uint32_t p=L(0x7a);const uint8_t *t=ga_rom(p);
    if (!t) {ga_fail(e,p);return;}
    if (t[0]&0x80u) {SL(0x7a,0);SB(0x14,B(0x19));return;}
    SB(0x14,B(0x19u+((t[0]-1u)&3u)));SB(0x98,t[1]>>1);SL(0x7a,p+2u);
}
/* 0511D2: +4E under +8A: the next stage and its program 050ECC[stage]. */
static bool k511d2(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x8e)>=5u) return true;
    if (W(0x8a)<W(0x4e)) return true;
    SW(0x8a,W(0x8a)-W(0x8c));SW(0x8e,W(0x8e)+1u);
    if (!ga_anim_start(a,e,rl(e,0x50eccu+4u*W(0x8e)),0x100)) return false;
    if (W(0x8e)>=4u) SW(0x8a,0);
    return true;
}
/* 051220: a hit's push by the weapon's kind +9C. */
static void k51220(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x9d)&0x80u) return;
    if (B(0x5f)&0x80u) return;
    uint16_t d0=W(0x4e),d1=W(0x90);
    SW(0x90,d0);
    if (d1==d0) return;
    if (S16v(W(0x3c))>=0x180) SW(0x28,W(0x28)+2u);
    else {
        static const struct {uint8_t kind;uint32_t table;} k[]={{1,0x2d3924},{7,0x2d393e},{9,0x2d3958},{0xa,0x2d3972},{0xb,0x2d3972},{0xe,0x2d398c},{0xc,0x2d39a6}};
        uint32_t v=(uint32_t)(int32_t)S16v(rank(a,e,0x2d390a))<<8;
        for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (B(0x9c)==k[i].kind) v=(uint32_t)(int32_t)S16v(rank(a,e,k[i].table))<<8;
        SL(0x28,L(0x28)+v);SB(0x9c,0);
    }
    if (!B(0xa2)) {sound(e,0x107c);SB(0xa2,3);}
}
/* 0525EE: the eight pieces 051FFE / 051FF4 (program +82, speed +30 / +34) at 01B220 (dx, dy). */
static void k525ee(neo_gactor_t *a,neo_genv_t *e) {
    static const struct {uint32_t r;int16_t dx,dy;uint32_t prog,vx,vy;} t[]={
        {0x51ffe,-0x2c,0x55,0x2492f2,0xffff8000u,0x40000},{0x51ffe,-0x7b,0x27,0x249398,0xffff0000u,0x74000},
        {0x51ffe,0x34,0x71,0x249420,0x8000,0x74000},{0x51ff4,-0x49,0x19,0x2494b2,0xffffa000u,0x60000},
        {0x51ff4,0x27,0x69,0x2494b2,0x2000,0x40000},{0x51ffe,-0x29,0x2f,0x2494f4,0xfffec000u,0x48000},
        {0x51ffe,0x49,0x79,0x249518,0x10000,0x70000}};
    for (unsigned i=0;i<sizeof t/sizeof t[0];i++) {
        neo_gactor_t *c=spawn(a,e,t[i].r,0x1c);
        if (!c) continue;
        ga_place_from(c,a,e,t[i].dx,t[i].dy);ga_sl(c,0x82,t[i].prog);ga_sl(c,0x30,t[i].vx);ga_sl(c,0x34,t[i].vy);
    }
}
/* The panels 051772 of 0517D0 / 0518C4 / 051A1E / 051B20: (+92, +94, +96). */
typedef struct {int16_t x,y;uint16_t n;} panel_t;
static void panels(neo_gactor_t *a,neo_genv_t *e,const panel_t *p,unsigned n) {
    sound(e,0x1030);
    for (unsigned i=0;i<n;i++) {
        neo_gactor_t *c=spawn(a,e,0x51772,0x2a);
        if (!c) continue;
        ga_sw(c,0x92,(uint16_t)p[i].x);ga_sw(c,0x94,(uint16_t)p[i].y);ga_sw(c,0x96,p[i].n);
    }
}
static bool train_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x51032: k51032(a);return true;
    case 0x51082: k51082(a);return true;
    case 0x510d4: k510d4(a);return true;
    case 0x13f1e:
        if (e->dyn_release) e->dyn_release(e->ctx,a);
        return true;
    case 0x517d0: {
        static const panel_t p[]={{-0x69,0x30,1},{-0x69,0x30,2},{-0x69,0x30,3},{-0x69,0x30,4}};
        panels(a,e,p,4);blast(a,e,0x1c706,-0x48,0xe);blast(a,e,0x1c6ee,-0x29,0x43);
        return true;}
    case 0x518c4: {
        static const panel_t p[]={{-0x69,0x30,0},{-0x59,0x28,7},{-0x58,0x2b,4},{-0x38,0x40,5},{-0x38,0x33,5},{-0x20,0x30,6},{-0x12,0x30,6}};
        panels(a,e,p,7);blast(a,e,0x1c706,-0x60,0x46);blast(a,e,0x1c6ee,-9,0x4b);
        return true;}
    case 0x51a1e: {
        static const panel_t p[]={{9,0x40,5},{9,0x34,5},{0x24,0x40,5}};
        panels(a,e,p,3);blast(a,e,0x1c706,-0x68,0x2e);blast(a,e,0x1c706,-0x30,0x3e);blast(a,e,0x1c6ee,-0x19,0x13);
        return true;}
    case 0x51b20: {
        static const panel_t p[]={{0x24,0x34,5},{0x3f,0x2e,6}};
        panels(a,e,p,2);blast(a,e,0x1c6ee,-0x30,0x2b);blast(a,e,0x1c706,-0x10,0x1a);blast(a,e,0x1c6ee,0x40,0x3b);
        return true;}
    case 0x521b4:
        if (!spawn(a,e,0x51df2,0)) SB(0x99,0xff);
        return true;
    case 0x521d0:
        SW(0x8e,6);sound(e,0x1078|0x8000u);                              /* 012192 */
        SL(0x30,0xfffec000u);SL(0x34,0x6c000);
        return true;
    case 0x521f2: {
        static const int16_t p[][2]={{-0x77,0x48},{-0x64,0x40},{-0x3e,0x48},{1,0x55},{0x1d,0x53}};
        for (unsigned i=0;i<5;i++) blast(a,e,0x1c6ee,p[i][0],p[i][1]);
        return true;}
    case 0x522e4: {
        static const int16_t p[][2]={{-0x75,0x2d},{-0x5a,0x3a},{-0x39,0x73},{0x18,0x94},{0x26,0x7b}};
        for (unsigned i=0;i<5;i++) blast(a,e,0x1c6ee,p[i][0],p[i][1]);
        return true;}
    case 0x523d6: {
        static const int16_t p[][2]={{-0x83,0x3e},{-0x6f,0x3c},{-0x4c,0x4f},{-0x17,0x72},{7,0x74}};
        for (unsigned i=0;i<5;i++) blast(a,e,0x1c6ee,p[i][0],p[i][1]);
        return true;}
    case 0x524c8:
        spawn(a,e,0x51c00,0);SB(0x9f,3);
        return true;
    case 0x524e0: {
        static const int16_t p[][2]={{-0x55,0x1d},{-0x3b,0x56},{4,0x56},{0x29,0x87},{-0x74,0x26},{0xa,0x5e}};
        for (unsigned i=0;i<6;i++) {neo_gactor_t *c=spawn(a,e,0x1c71e,0x1c);if (c) ga_place_from(c,a,e,p[i][0],p[i][1]);}
        return true;}
    case 0x525c6: {
        neo_gactor_t *c=spawn(a,e,0x52128,0x1c);
        if (c) ga_place_from(c,a,e,-0x48,0x28);
        return true;}
    case 0x52840:
        SW(0x2c,W(0x2c)-4u);spawn(a,e,0x527a2,0);
        return true;
    }
    return false;
}
/* 051C00 / 051DF2 / 051E7E..: the creator's palette select +A1 by the counter +9A. */
static void select_a1(neo_gactor_t *p,uint8_t from) {ga_sb(p,0xa1,ga_b(p,from));}
static ga_result_t train_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p,*c;int r;
    for (;;) switch (L(0)) {
    /* the train */
    case 0x51344:
        SW(0x70,W(0x70)-1u);
        if (!(W(0x70)&0x8000u)) {ga_stationary(a,e);return GA_MORE;}   /* 01436A */
        pals(a,P_FCC);                                                   /* 050FCC */
        START(0x248cd6);
        SW(0x66,5);SW(0x1e,5);
        ga_floor(a,e);                                                   /* 0146E8 */
        SW(0x8e,0);SL(0x7a,0);SB(0x9b,0);SB(0x9f,0);SB(0xa1,0);SB(0xa2,0);
        spawn(a,e,0x515ce,0x1c);spawn(a,e,0x51688,0x1c);spawn(a,e,0x51690,0x1c);
        SW(0x4e,rank(a,e,0x2d388e));
        SW(0x8c,(uint16_t)((W(0x4e)&0x3fffu)/5u));
        SW(0x8a,W(0x4e)-W(0x8c));
        SL(0x72,0u-((uint32_t)(int32_t)S16v(rank(a,e,0x2d38f0))<<8));
        SL(0x30,L(0x72));SL(0x34,0);SL(0x50,0x50d76);
        SW(0x90,W(0x4e));
        sound(e,0x1078);
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x51430);
        /* fall through */
    case 0x51430:
        if (B(0xa2)) SB(0xa2,B(0xa2)-1u);
        if (ga_register(a,e,NULL,NULL)) {k5100c(a);SB(0x9c,B(0x5a));SB(0x9d,B(0x5b));SB(0x5a,0);}   /* 004C24 */
        k5117a(a,e);k5112e(a,e);
        if (B(0xa1)) {SB(0x14,B(0xa1));SB(0xa1,0);}
        if (W(0x8e)<5u) k51220(a,e);
        if (!k511d2(a,e)) return GA_UNSUPPORTED;
        if (W(0x8e)<5u && e->dyn_set) e->dyn_set(e->ctx,a,0x50d82,-0x78,0);   /* 0140C4 */
        if (W(0x8e)==6u) SL(0x34,L(0x34)+0xffffb000u);
        ga_move(a,e);                                                    /* 01AC38 */
        if ((r=ga_anim_step(a,e,train_call))<0) return GA_UNSUPPORTED;
        if (r) {
            if (e->flags) e->flags->flash=1;                             /* 009BB0(1) */
            sound(e,0x1034);
            k525ee(a,e);
            c=spawn(a,e,0x1d764,0x1c);                                   /* 01DA40 */
            if (c) {ga_place_from(c,a,e,0,0);if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));ga_sl(c,0x86,0x50f62);}
            SB(0x9b,0xff);
            if (W(0x6e)<0x5au) {                                         /* 08CDB2 */
                c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
                if (c) {ga_sb(c,0x8b,0);ga_sw(c,0x70,0);ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,-0x20,0x58);}
            }
            SL(0,0x51530);
            return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        }
        if (S16v(W(0x3c))>=-0x80) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        if (e->dyn_release) e->dyn_release(e->ctx,a);                    /* 013F1E */
        SB(0x69,0xff);                                                   /* 019DFC */
        sound(e,0x1078|0x8000u);                                         /* 012192 */
        return GA_DELETE_TREE;                                           /* 0041AC */
    case 0x51530:
        if (e->flags) e->flags->flash=0;                                 /* 009BB0(0) */
        SB(0x69,0xff);                                                   /* 019DFC */
        return GA_DELETE;                                                /* 004188 */
    /* the wheels */
    case 0x515ce:
        on_creator(a,e,0,0);pals(a,P_FCC);layers(a,e,0x1e);
        SB(0x9f,0);SW(0x8e,0xffff);
        SL(0,0x515f8);
        /* fall through */
    case 0x515f8:
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        if (ga_b(p,0x9b)) return GA_DELETE;
        if (ga_w(p,0x8e)!=W(0x8e)) {SW(0x8e,ga_w(p,0x8e));START(rl(e,0x50ee8u+4u*W(0x8e)));}
        if (ga_b(p,0x9f)!=B(0x9f)) {
            unsigned d0=ga_b(p,0x9f)&0xfu;
            if (d0==4u) k510d4(a);
            if (d0==2u) k51082(a);
            if (d0==1u) k51032(a);
        }
        SB(0x14,ga_b(p,0x14));
        ga_project(a,e);
        if (ga_anim_step(a,e,train_call)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the bogies */
    case 0x51688:
    case 0x51690:
        on_creator(a,e,L(0)==0x51688u?-0x49:0x49,9);
        layers(a,e,0xffff);
        ga_palette(a,0x112);
        START(0x248f0a);
        SL(0,0x516c8);
        /* fall through */
    case 0x516c8:
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        if (ga_w(p,0x8e)>=6u) return GA_DELETE;
        ga_project(a,e);
        if (ga_anim_step(a,e,train_call)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the panels */
    case 0x51772:
        on_creator(a,e,S16v(W(0x92)),S16v(W(0x94)));
        ga_palette(a,0x10f);
        layers(a,e,0x3c);
        START(rl(e,0x24922cu+4u*W(0x96)));
        SL(0,0x517b8);
        /* fall through */
    case 0x517b8:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,train_call))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the smoke 051C00: a blast 01C706 every other update from the list 050F50 */
    case 0x51c00:
        SL(0x76,0x50f50);SB(0x9a,0xff);
        on_creator(a,e,0,0);layers(a,e,0x14);
        SL(0,0x51c2a);
        /* fall through */
    case 0x51c2a: {
        ga_project(a,e);
        SB(0x9a,B(0x9a)+1u);
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        if (B(0x9a)==0u || B(0x9a)==8u) select_a1(p,0x1a);
        if (B(0x9a)==1u || B(0x9a)==9u) select_a1(p,0x19);
        if (!(B(0x9a)&1u)) return GA_MORE;
        uint32_t q=L(0x76);const uint8_t *t=ga_rom(q);
        if (!t) {ga_fail(e,q);return GA_UNSUPPORTED;}
        if (t[1]&0x80u) {ga_sb(p,0x99,0xff);return GA_DELETE;}
        SL(0x76,q+2u);
        c=spawn(a,e,0x1c706,0x2a);
        if (c) {ga_place_at(c,a,e,(int8_t)t[0],(int8_t)t[1]);ga_sw(c,0x1e,W(0x1e));ga_sw(c,0x66,W(0x66));}
        sound(e,0x1022);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
    /* the spark 051D70 of 051DF2: the creator's creator bobs 8 px for an update */
    case 0x51d70:
        on_creator(a,e,S16v(W(0x92)),S16v(W(0x94)));
        ga_palette(a,0x1d);
        START(0x24c3aa);
        SB(0x9a,0);
        p=creator(a,e);p=p?creator(p,e):NULL;
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        ga_sw(p,0x2c,ga_w(p,0x2c)-8u);select_a1(p,0x1a);
        SL(0,0x51dbc);
        return GA_MORE;
    case 0x51dbc:
        if (!B(0x9a)) {
            SB(0x9a,0xff);
            p=creator(a,e);p=p?creator(p,e):NULL;
            if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
            ga_sw(p,0x2c,ga_w(p,0x2c)+8u);select_a1(p,0x19);
        }
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,train_call))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the sparks 051DF2: 051D70 every other update from 050F40, a pause, then blasts 01C71E from 050F48 */
    case 0x51df2:
        on_creator(a,e,0,0);layers(a,e,0x14);
        SL(0x76,0x50f40);SB(0x9a,1);
        SL(0,0x51e1c);
        /* fall through */
    case 0x51e1c: {
        ga_project(a,e);
        SB(0x9a,B(0x9a)-1u);
        if (B(0x9a)) return GA_MORE;
        SB(0x9a,2);
        uint32_t q=L(0x76);const uint8_t *t=ga_rom(q);
        if (!t) {ga_fail(e,q);return GA_UNSUPPORTED;}
        if (t[1]&0x80u) {SB(0x9a,0xf);SL(0,0x51e7e);continue;}
        SL(0x76,q+2u);
        c=spawn(a,e,0x51d70,0x2a);
        if (c) {ga_sw(c,0x92,(uint16_t)(int16_t)(int8_t)t[0]);ga_sw(c,0x94,(uint16_t)(int16_t)(int8_t)t[1]);ga_sw(c,0x1e,W(0x1e));ga_sw(c,0x66,W(0x66));}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
    case 0x51e7e:
        ga_project(a,e);
        SB(0x9a,B(0x9a)-1u);
        if (B(0x9a)) return GA_MORE;
        layers(a,e,0x28);
        SL(0x76,0x50f48);SB(0x9a,0xff);
        SL(0,0x51eaa);
        /* fall through */
    case 0x51eaa: {
        uint32_t q=L(0x76);const uint8_t *t=ga_rom(q);
        if (!t) {ga_fail(e,q);return GA_UNSUPPORTED;}
        if (t[1]&0x80u) SL(0,0x51f10);
        else {
            SL(0x76,q+2u);
            c=spawn(a,e,0x1c71e,0x2a);
            if (c) {ga_place_at(c,a,e,(int8_t)t[0],(int8_t)t[1]);ga_sw(c,0x1e,W(0x1e));ga_sw(c,0x66,W(0x66));}
            sound(e,0x1022);
            if (!B(0x9a)) {p=creator(a,e);if (p) select_a1(p,0x1a);}
        }}
        /* fall through */
    case 0x51f10:
        ga_project(a,e);
        SB(0x9a,B(0x9a)+1u);
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        if (B(0x9a)==0u) ga_sw(p,0x2c,ga_w(p,0x2c)-6u);
        if (B(0x9a)==1u) ga_sw(p,0x2c,ga_w(p,0x2c)-2u);
        if (B(0x9a)==2u) ga_sw(p,0x2c,ga_w(p,0x2c)+4u);
        if (B(0x9a)==3u) {ga_sb(p,0x99,0xff);return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the pieces 051FF4 (+9E = 1: no floor) / 051FFE */
    case 0x51ff4:
    case 0x51ffe:
        SB(0x9e,L(0)==0x51ff4u?1:0);
        ga_palette(a,0x110);
        START(L(0x82));
        layers(a,e,0x32);
        SB(0x9a,0x64);
        SL(0,0x52034);
        /* fall through */
    case 0x52034:
        SB(0x9a,B(0x9a)-1u);
        if (!B(0x9a)) return GA_DELETE;
        SL(0x86,L(0x34));
        if (!B(0x9e)) {
            SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                    /* 0143DC */
            if (B(0x15)&0x40u) {
                uint32_t d0=L(0x86);if (d0&0x80000000u) d0=0u-d0;
                SL(0x34,(uint32_t)((int32_t)d0>>1));SB(0x5f,B(0x5f)|8u);
            }
        } else {
            if (S16v(W(0x40))<0xe0) return GA_DELETE;
            SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);                   /* 01AC38 */
        }
        if (ga_anim_step(a,e,train_call)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the piece 052128 */
    case 0x52128:
        ga_palette(a,0x110);
        START(0x24924c);
        layers(a,e,0x32);
        SB(0x9a,0x3c);SL(0x30,0xfffe0000u);SL(0x34,0x58000);
        SL(0,0x5216a);
        /* fall through */
    case 0x5216a:
        SB(0x9a,B(0x9a)-1u);
        if (!B(0x9a)) return GA_DELETE;
        SL(0x86,L(0x34));
        SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);
        if (ga_anim_step(a,e,train_call)<0) return GA_UNSUPPORTED;
        if (S16v(W(0x40))<=0x130) {
            uint32_t d0=L(0x86);if (d0&0x80000000u) d0=0u-d0;
            SL(0x34,(uint32_t)((int32_t)d0>>1));SB(0x5f,B(0x5f)|8u);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the bob 0527A2 of 052840: the creator 4 px back down next update */
    case 0x527a2:
        SL(0,0x527aa);
        return GA_MORE;
    case 0x527aa:
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        ga_sw(p,0x2c,ga_w(p,0x2c)+4u);
        return GA_DELETE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
bool neo_m5train_routine(uint32_t r) {
    switch (r) {
    case 0x51344: case 0x51430: case 0x51530: case 0x515ce: case 0x515f8: case 0x51688: case 0x51690: case 0x516c8: case 0x51772:
    case 0x517b8: case 0x51c00: case 0x51c2a: case 0x51d70: case 0x51dbc: case 0x51df2: case 0x51e1c: case 0x51e7e: case 0x51eaa:
    case 0x51f10: case 0x51ff4: case 0x51ffe: case 0x52034: case 0x52128: case 0x5216a: case 0x527a2: case 0x527aa: return true;
    }
    return false;
}
ga_result_t neo_m5train_tick(neo_gactor_t *a,neo_genv_t *e) {return train_tick(a,e);}
