#include "neo_region.h"
#include "neo_vslot.h"
/* Mission 5 sewer actors, manual translation (ROM labels as provenance; MAME
 * run4 replay from sec08_274387, capture X, docs/MISION5.md).
 * - 095C98 (script, the crank): palette 4F, +8F = +6E * 2 + +70, layers
 *   8810, 0146E8, the child 095F8C (D0 = 1C) at (+74, +76) by 01ACF2 with
 *   the words +6E, +72..+82 (+78 << 3 first), program 29C9D4. 095D6E:
 *   004C24, 01436A, 01AD8C, 00427A, 0C8BEC (095948) -> 004188; +95 set (the
 *   child is done): 095C38 (once, +98: the explosion 01C706 (D0 = 2A) at
 *   (28, 8), sound 106A); else 0AA606 (0AA616 with 0C069E: the rectangle
 *   095938, or 095940 and D1 = FF with +70). The grab (capture C):
 *   095DCE (+84, 095B64 = 0AA760 seat 0 / 47, +86, 0AA83A class 5, the
 *   input word's bit 4 cleared), 095E08 (the turning player 096140,
 *   program 29CA7A, +50 = 095950 / 09595C), 095E58..095F8A (A adds 9 to
 *   +88, 095B98 turns the wheel 095A94[+91], 0C05EC / 004C24, let go by
 *   095C76 (class 3 / 5) or a hit (class 0)); the wheel's call 095A74 makes
 *   the splash 0959B0 (palette 204, program 29C98C) after the script actor.
 * - 095F8C (its child): 01ABC8 and the cell map 095B54[+72] by 0140C0;
 *   095FAE: the items 08C374 of the words +7C..+82 by 08CD70 at
 *   (C + 18 * i, 10), life FFFF, +8B = 1; 095FFC: 01ABC8; creator gone ->
 *   0041AC; no children (00408C) -> 0960A6: the creator's +95, 004188;
 *   else +8C runs to +8A (the crank's +96 adds C & F8), up to +78, along
 *   095B44[+6E] while the map is placed (095F98).
 * - 089948 / 08995A / 08996A (script soldiers on the floor, the wall
 *   089D92 and the ceiling with +15 bit 1): 08A080 (the guns 08A264,
 *   one or two), 08A0CE (ranks 2D2A1C / 2D2C06 / 2D2A7E, palette C4 and the
 *   list +72 = E5372 by 009D1E, +7C random, +80 layers), the table E534A.
 *   Walking 0899A6 / 089A8C / 089B16 (the floor, the wall, the ceiling) to
 *   08A480's target, idle, turning 089BB0, aiming 089DB4 (01AFFC by
 *   E5312[+86]), firing (E5362 by 08A264's report +89), hit 089E34, burnt
 *   089ED6 (+5A = 7), past +82 08A00A; the tail 08A1BA (01945C E530A,
 *   10C700 by 08A654, 004C24).
 * - 08A264 (the gun): on the creator (01AD6E), +15 and the hurtbox
 *   E52A6[+76][+70 & 1] from it; hit (004C24) by a player in front: the
 *   creator's +89 = 1 / 2. */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,v)
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
#define BIT(o,n) ((B(o)>>(n))&1u)
#define S16(v) ((int16_t)(uint16_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if ((r=ga_anim_step(a,e,sewer_call))<0) return GA_UNSUPPORTED; } while (0)
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;} return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2u);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {   /* 0260C8 */
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* 0040BC with its reserve D0. */
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
static ga_result_t done(neo_genv_t *e,ga_result_t r) {return e->unsupported?GA_UNSUPPORTED:r;}
/* 01AD8C: +1E follows +66 - y when +66 is 8xxx. */
static void depth(neo_gactor_t *a) {
    uint16_t d0=W(0x66);
    if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
static void k8a618(neo_gactor_t *a,neo_genv_t *e);
/* The soldiers' program calls (A0: the parameter word after the call). */
static bool sewer_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x8a66c: SB(0x8c,parameter>>8);k8a618(a,e);return true;        /* move.b (a0) */
    case 0x8a674:
        SB(0x1d,0);ga_palette(a,0xc5);                                   /* 009F88, 009EA8 */
        SB(0x15,B(0x15)&~2u);
        if (W(0x76)==2u) SW(0x2c,W(0x2c)-0x20u);
        return true;
    case 0x8a6a0: SB(0x15,B(0x15)&~2u);return true;
    case 0x8a6a8: SB(0x15,B(0x15)|2u);return true;
    case 0x8a6b0: {
        uint32_t d0=L(parameter);
        if (d0&0x80000000u) d0=0u-d0;
        SW(0x26,(uint16_t)((d0<<7)>>16));
        return true;
    }
    case 0x8a6c6:
        SB(0x15,B(0x15)^2u);
        SW(0x76,W(0x76)==0u?2:0);
        return true;
    case 0x95a74: {                                                      /* the wheel's program: the splash 0959B0 */
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x959b0,true):NULL;   /* 00411C after the script actor, D0 = 2A */
        if (c) ga_place_at(c,a,e,0,0);                                     /* 01ACF2 */
        return true;
    }
    case 0x8a6e8: {                                                      /* 08A5D4: A80000 */
        uint32_t d0=0xa80000u;
        if (BIT(0x15,1)) d0=0u-d0;
        d0-=0xffc00000u;
        SL(0x34,(uint32_t)((int32_t)d0>>4));SL(0x30,0);
        return true;
    }
    }
    return false;
}
/* ---- the crank 095C98 and its child 095F8C ---------------------------------------- */
static uint16_t rng(void *ctx) {return ga_rand((neo_genv_t *)ctx);}
/* 095D2E..095D68: the idle crank. */
static bool crank_idle(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x5a,0);SB(0x94,0);SB(0x96,0);SB(0x97,0);
    SW(0x84,0xffff);SW(0x86,0xffff);SW(0x4e,0);SB(0x5f,B(0x5f)&0x7fu);SL(0x50,0);
    if (!ga_anim_start(a,e,0x29c9d4,0x100)) return false;
    SL(0,0x95d6e);
    return true;
}
static uint16_t node_word(neo_genv_t *e,const neo_gactor_t *a) {return e->node_word?e->node_word(e->ctx,a):(uint16_t)(0x8000u|(uint16_t)a->index);}
/* The bytes the slot services write into the node directly, written again through the actor (as neo_tank.c). */
static void touched(neo_gactor_t *a) {static const uint8_t o[]={0x4e,0x4f,0x5e,0x5f};for (unsigned i=0;i<sizeof(o);i++) ga_sb(a,o[i],a->m[o[i]]);}
static unsigned rider(neo_gactor_t *a) {return W(0x84)&1u;}
/* 095B64: 0AA760 for the player +84 with the seat (0 or 47 by +70, 0); carry
 * (true) when bound, *d0 = the player's +6E; else +94 = 0. +95 set: no carry. */
static bool k95b64(neo_gactor_t *a,neo_genv_t *e,uint16_t *d0) {
    if (B(0x95)) return false;
    neo_vehicle_env_t *v=e->vehicle;unsigned p=rider(a);uint16_t p6e=0;
    neo_vslot_player_t pl={v->exists[p],v->player[p]};
    bool bound=neo_vslot_bind(v->slots,p,a->m,node_word(e,a),&pl,e->video,W(0x70)?0x47:0,0,&p6e);
    touched(a);
    if (bound) {if (d0) *d0=p6e;return true;}
    SB(0x94,0);return false;
}
/* 095C76: 0AA7FC kind 3 (or 5 with +70), +00 = 095D2E. */
static void k95c76(neo_gactor_t *a,neo_genv_t *e) {
    neo_vslot_exit(e->vehicle->slots,rider(a),a->m,node_word(e,a),W(0x70)?5:3);touched(a);
    SL(0,0x95d2e);
}
/* 0C05EC: player p's +84 nonzero (d1), +9A bit 2 (d2), +84 >= 10 and odd
 * (d3); all clear for a player who does not exist (0040AC) or with +5F bit 7. */
static void k0c05ec(neo_genv_t *e,unsigned p,bool *d1,bool *d2,bool *d3) {
    const uint8_t *m=e->vehicle->exists[p&1u]?e->vehicle->player[p&1u]:NULL;
    *d1=*d2=*d3=false;
    if (!m || (m[0x5f]&0x80u)) return;
    uint16_t w84=(uint16_t)(m[0x84]<<8|m[0x85]);
    *d1=w84!=0;*d2=(m[0x9a]&4u)!=0;*d3=w84>=0x10u && (w84&1u);
}
/* 095B98: +92 counts down; at 0 the wheel steps by +93 (095A94[+91]), at
 * step 4 turning forwards +96 (the child's +8A) and sound 108B; the next
 * count from 095B14 / 095B24 / 095B34 (by +86 < 2 and +88 >= 28) while +97,
 * else 2. */
static void k95b98(neo_gactor_t *a,neo_genv_t *e) {
    SB(0x92,B(0x92)-1u);
    if (B(0x92)) return;
    uint8_t d0=(uint8_t)((B(0x91)+B(0x93))&0xfu);SB(0x91,d0);
    if (!ga_anim_start(a,e,rl(e,0x95a94u+4u*d0),0x100)) return;
    if (B(0x91)==4u && !(B(0x93)&0x80u)) {SB(0x96,0xff);sound(e,0x108b);}
    if (!B(0x97)) {SB(0x92,2);return;}
    uint32_t t0,t1;
    if (W(0x86)<2u) {t0=0x95b24;t1=0x95b14;} else {t0=0x95b34;t1=0x95b24;}
    if (S16(W(0x88))>=0x28) t0=t1;
    const uint8_t *t=ga_rom(t0+B(0x91));
    if (!t) {ga_fail(e,t0);return;}
    SB(0x92,t[0]>>1);
}
/* 095C38: once (+98), the explosion 01C706 (D0 = 2A) at (28, 8), sound 106A. */
static void k95c38(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x98)) return;
    SB(0x98,0xff);
    neo_gactor_t *c=spawn(a,e,0x1c706,0x2a);
    if (c) ga_place_from(c,a,e,0x28,8);
    sound(e,0x106a);
}
/* 095F98: the cell map 095B54[+72] by 0140C0. */
static void crank_map(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t map=rl(e,0x95b54u+4u*W(0x72));
    if (e->dyn_set && !e->unsupported) e->dyn_set(e->ctx,a,map,0,0);
}
static ga_result_t crank_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x95c98: {
        ga_palette(a,0x4f);
        SB(0x8f,(uint8_t)(W(0x6e)*2u+W(0x70)));
        SW(0x66,0x8810);SW(0x1e,0x8810);
        ga_floor(a,e);
        neo_gactor_t *c=spawn(a,e,0x95f8c,0x1c);
        SW(0x78,(uint16_t)(W(0x78)<<3));
        if (c) {
            ga_place_at(c,a,e,S16(W(0x74)),S16(W(0x76)));                 /* 01ACF2 */
            static const uint8_t f[]={0x6e,0x72,0x74,0x76};
            for (unsigned i=0;i<sizeof f;i++) ga_sw(c,f[i],W(f[i]));
            for (unsigned o=0x78;o<=0x82;o+=2) ga_sw(c,o,W(o));
        }
        SB(0x95,0);
        if (!crank_idle(a,e)) return GA_UNSUPPORTED;
    }   /* fall through */
    case 0x95d6e: wait: {
        ga_register(a,e,NULL,NULL);                                      /* 004C24 */
        SB(0x5a,0);SL(0x30,0);
        ga_stationary(a,e);depth(a);STEP();
        if (ga_outside_view(a,e,0x95948)) return done(e,GA_DELETE);     /* 0C8BEC -> 004188 */
        if (B(0x95)) {k95c38(a,e);return done(e,GA_MORE);}
        if (!e->vehicle || !e->vehicle->slots) {ga_fail(e,0xaa606);return GA_UNSUPPORTED;}
        neo_vehicle_env_t *v=e->vehicle;
        int16_t rect[4];uint32_t at=W(0x70)?0x95940u:0x95938u;
        for (unsigned i=0;i<4;i++) rect[i]=S16(rw(e,at+2u*i));
        neo_vslot_player_t pl[2]={{v->exists[0],v->player[0]},{v->exists[1],v->player[1]}};
        neo_vslot_grab_t g={W(0x70)!=0,{v->input[0][0],v->input[1][0]}};
        int p=neo_vslot_select_grab(v->slots,a->m,pl,rect,v->halted!=0,&g,rng,e);
        if (p<0) return done(e,GA_MORE);
        SW(0x84,(uint16_t)p);                                            /* 095DCE */
        uint16_t d0;
        if (!k95b64(a,e,&d0)) return done(e,GA_MORE);
        SW(0x86,d0);
        neo_vslot_mount(v->slots,rider(a),a->m,5,0,0,0);touched(a);      /* 0AA83A */
        v->input[rider(a)][0]&=0xffefu;                                  /* 0CA2CE: andi.w #FFEF, (a0) */
        SL(0,0x95e08);
        return done(e,GA_MORE);
    }
    case 0x95e08: {
        neo_gactor_t *c=spawn(a,e,0x96140,0x1c);
        if (c) ga_sw(c,0x70,W(0x70));
        SB(0x91,0);SB(0x92,0);SW(0x88,0);SB(0x94,0xff);
        if (!ga_anim_start(a,e,0x29ca7a,0x100)) return GA_UNSUPPORTED;
        SL(0x50,W(0x70)?0x9595cu:0x95950u);
        SL(0,0x95e58);
    }   /* fall through */
    case 0x95e58: {
        if (!e->vehicle || !e->vehicle->slots) {ga_fail(e,0x95e58);return GA_UNSUPPORTED;}
        neo_vehicle_env_t *v=e->vehicle;
        if (B(0x95)) {k95c76(a,e);if (!crank_idle(a,e)) return GA_UNSUPPORTED;goto wait;}
        if (!(v->dip10f000&0x80u)) {
            bool d1,d2,d3;k0c05ec(e,W(0x84),&d1,&d2,&d3);
            if (d1) {SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);}                  /* 095EA4 */
            else if (ga_register(a,e,NULL,NULL)) {                        /* 004C24 */
                neo_vslot_exit(v->slots,rider(a),a->m,node_word(e,a),0);touched(a);
                if (!crank_idle(a,e)) return GA_UNSUPPORTED;
                goto wait;
            }
        }
        uint16_t pad=v->input[rider(a)][0];                              /* 0CA2CE */
        if (pad&0x10u) {
            SB(0x97,0xf);SB(0x93,1);
            if (!B(0x92)) {SB(0x92,1);SW(0x88,0);}
            SW(0x88,W(0x88)+9u);
        }
        if (W(0x88)) SW(0x88,W(0x88)-1u);
        if (B(0x97)) {
            SB(0x97,B(0x97)-1u);
            if (!B(0x97)) {SW(0x88,0);if (B(0x91)<=7u) SB(0x93,0xff);}
        }
        if (B(0x92)) k95b98(a,e);
        if (!W(0x88) && !B(0x91) && B(0x92)) {
            if (!ga_anim_start(a,e,0x29ca7a,0x100)) return GA_UNSUPPORTED;
            SB(0x92,0);
        }
        if (v->input[rider(a)][0]&0xffefu) k95c76(a,e);
        SL(0x30,0);
        ga_stationary(a,e);depth(a);STEP();
        if (!k95b64(a,e,NULL)) SL(0,0x95d2e);
        return done(e,GA_MORE);
    }
    case 0x95d2e:
        if (!crank_idle(a,e)) return GA_UNSUPPORTED;
        goto wait;
    case 0x959b0:   /* the splash of the wheel (095A74): palette 204, program 29C98C */
        ga_palette(a,0x204);
        START(0x29c98c);
        SL(0,0x959d4);
        /* fall through */
    case 0x959d4:
        ga_project(a,e);STEP();
        if (r) return done(e,GA_DELETE);                                 /* 00427A carry: 10F00C + 3EF, 004188 */
        return done(e,GA_MORE);
    case 0x96140: {   /* the turning player (its creator the crank) */
        neo_gactor_t *c=creator(a,e);
        int16_t d0=0;
        if (W(0x70)) {SB(0x15,B(0x15)|1u);d0=0x47;}
        ga_place(a,e,d0,0,c?NEO_GA_TAG(c->index):0);                     /* 01AD6E */
        uint16_t w86=c?ga_w(c,0x86):0;SW(0x86,w86);SB(0x90,w86);
        uint16_t pal=rw(e,0x34330u+2u*W(0x86));
        ga_palette(a,pal);ga_palette(a,pal);                             /* 009EA8 D1 = 1, twice */
        /* 009CCE (+1A, 0, 8): palette fade RAM 1068EA (no node byte), not reproduced (as neo_slug.c). */
        SW(0x84,c?ga_w(c,0x84):0);SB(0x91,0x63);
        SL(0,0x961ce);
    }   /* fall through */
    case 0x961ce: {
        neo_gactor_t *c=creator(a,e);
        if (!c || !ga_b(c,0x94)) return done(e,GA_DELETE);              /* 004188 */
        SB(0x14,B(0x19));
        bool d1,d2,d3;
        if (!e->vehicle) {ga_fail(e,0xc05ec);return GA_UNSUPPORTED;}
        k0c05ec(e,W(0x84),&d1,&d2,&d3);
        if (d1 && d2 && d3) SB(0x14,B(0x1a));
        uint8_t d0=ga_b(c,0x91);
        if (d0!=B(0x91)) {
            d0&=0xfu;SB(0x91,d0);
            if (!ga_anim_start(a,e,rl(e,0x95ad4u+4u*d0),0x100)) return GA_UNSUPPORTED;
        }
        SW(0x1e,ga_w(c,0x1e));
        ga_project(a,e);STEP();
        return done(e,GA_MORE);
    }
    case 0x95f8c:
        SL(0,0x95fae);
        ga_project(a,e);crank_map(a,e);
        return done(e,GA_MORE);
    case 0x95fae:
        ga_project(a,e);
        for (unsigned i=0;i<4;i++) {                                     /* 08CD70 */
            uint16_t kind=W(0x7c+2u*i);
            if (kind>=0x5au) continue;
            neo_gactor_t *c=spawn(a,e,0x8c374,0x1c);
            if (!c) continue;
            ga_sb(c,0x8b,1);ga_sw(c,0x70,0xffff);ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
            ga_place_at(c,a,e,(int16_t)(0xc+0x18*i),0x10);
        }
        SW(0x8a,0);SW(0x8c,0);SL(0,0x95ffc);
        /* fall through */
    case 0x95ffc: {
        ga_project(a,e);
        if (!ga_creator_alive(a,e)) return done(e,GA_DELETE_TREE);       /* 00409A -> 0041AC */
        if (!e->has_children || !e->has_children(e->ctx,a)) {            /* 00408C */
            neo_gactor_t *c=creator(a,e);
            if (c) ga_sb(c,0x95,0xff);
            SL(0x30,0);
            return done(e,GA_DELETE);
        }
        neo_gactor_t *c=creator(a,e);
        if (c && ga_b(c,0x96)) {ga_sb(c,0x96,0);SW(0x8a,(W(0x8a)+0xcu)&0xf8u);}
        int16_t d0=S16(rw(e,0x95b44u+4u*W(0x6e))),d1=S16(rw(e,0x95b46u+4u*W(0x6e)));
        uint16_t d2=W(0x8a);
        if (d2!=W(0x8c)) {
            if (S16(d2)>S16(W(0x8c))) {
                SW(0x8c,W(0x8c)+2u);
                if (d2<W(0x8c)) SW(0x8c,d2);                             /* bcc: d2 >= +8C */
            } else SW(0x8c,W(0x8c)-1u);
        }
        d2=W(0x8c);
        if (d2>=W(0x78)) SL(0,0x960a6);
        uint32_t x=L(0x28),y=L(0x2c);
        SW(0x28,W(0x28)+(uint16_t)(d0*S16(d2)));SW(0x2c,W(0x2c)+(uint16_t)(d1*S16(d2)));
        crank_map(a,e);
        SL(0x2c,y);SL(0x28,x);
        return done(e,GA_MORE);
    }
    case 0x960a6: {
        neo_gactor_t *c=creator(a,e);
        if (c) ga_sb(c,0x95,0xff);
        SL(0x30,0);
        return done(e,GA_DELETE);
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* ---- the sewer soldiers 089948 / 08995A / 08996A and their guns 08A264 -------- */
/* 08A654: carry when RAM 10C700 is not right of +3C. */
static bool k8a654(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->flags) {ga_fail(e,0x8a654);return false;}
    return S16(e->flags->limit700)<=S16(W(0x3c));
}
/* 08A618: a new +8C takes the palette word +72[+8C] into the slot +14 (009D1E). */
static void k8a618(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x8c)==B(0x8d)) return;
    SB(0x8d,B(0x8c));
    uint16_t d1=rw(e,L(0x72)+2u*B(0x8c));
    uint8_t s=B(0x14);if (s>=1u && s<=6u) a->palette[s-1u]=d1;
}
static void k8a60e(neo_gactor_t *a,neo_genv_t *e) {SB(0x8c,0);SB(0x8d,0xff);k8a618(a,e);}
/* 08A5DC: the routine of the table at D0 (08A5D4 is never 0). */
static void k8a5dc(neo_gactor_t *a,neo_genv_t *e,uint32_t table,uint16_t d0) {
    uint32_t t=rl(e,table+4u*d0);
    SL(0,t);
}
static void k8a4f6(neo_gactor_t *a,uint32_t to) {if (!B(0x85)) SL(0,to);}
static uint16_t k8a502(neo_gactor_t *a,neo_genv_t *e) {return rank(a,e,B(0x88)?0x2d2b42u:0x2d2ae0u);}
static void k8a528(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=(uint16_t)(k8a502(a,e)+W(0x7e));
    if (!BIT(0x15,0)) d0=(uint16_t)-d0;
    SL(0x6e,(uint32_t)((int32_t)S16(d0)<<8));
}
/* 08A546 / 08A590: the move 0143E4 kept when it stuck (+5F bit 1), else undone. */
static bool try_air(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t x=L(0x28),y=L(0x2c),p=L(0x38);
    ga_air(a,e);
    if (BIT(0x5f,1)) return true;
    SL(0x38,p);SL(0x2c,y);SL(0x28,x);
    ga_project(a,e);
    return false;
}
static void k89d92(neo_gactor_t *a) {
    if (S16(W(0x2c))>0xa0) SW(0x2c,0xa0);
    if (S16(W(0x2c))<0x40) SW(0x2c,0x40);
}
static void k8a646(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffff8000u);ga_air(a,e);}
/* 01AF9A: the nearest live player (false: none). */
static bool nearest(neo_gactor_t *a,neo_genv_t *e,unsigned *p) {
    unsigned m=e->players.active_mask&3u;
    if (!m) return false;
    if (m!=3u) {*p=m==2u;return true;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)-d1;
    if (d2&0x8000u) d2=(uint16_t)-d2;
    *p=S16(d2)<S16(d1);
    return true;
}
/* 08A480: the signed distance to walk (0 within C). */
static int16_t k8a480(neo_gactor_t *a,neo_genv_t *e) {
    unsigned p;uint16_t d0;
    if (!nearest(a,e,&p)) d0=W(0x3c);
    else {
        d0=p?e->players.p2x:e->players.p1x;
        if (W(0x76)==2u) d0=(uint16_t)(d0+((e->player_flags15[p]&1u)?0x10u:0xfff0u));
    }
    d0=(uint16_t)(d0+W(0x7c));
    if (S16(d0)<0x20) d0=0x20;
    if (S16(d0)>0x120) d0=0x120;
    d0=(uint16_t)(d0-W(0x3c));
    if (!BIT(0x15,0)) d0=(uint16_t)-d0;
    uint16_t d1=(d0&0x8000u)?(uint16_t)-d0:d0;
    if (d1<0xcu) d0=0;
    return S16(d0);
}
static void k89a68(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x88) || !W(0x7a)) return;
    if (!ga_rand_range(e,W(0x7a))) SL(0,0x89a38);
}
/* 08A3D4 / 08A3F6 */
static void k8a3d4(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t t=rl(e,0xe5312u+B(0x86));
    if (!e->unsupported && !ga_players_in(a,e,t)) SL(0,0x89db4);
}
static void k8a3f6(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x78,W(0x78)-1u);
    if (W(0x78)&0x8000u) k8a5dc(a,e,0xe5356u,W(0x76));
    if (W(0x76)==2u) return;
    uint16_t d1=W(0x3c);
    bool near=false;
    for (unsigned p=0;p<2 && !near;p++) {
        uint16_t d0=(uint16_t)((p?e->players.p2x:e->players.p1x)-d1);
        if (d0&0x8000u) d0=(uint16_t)-d0;
        if (d0<=0x10u) near=true;
    }
    if (!near) return;
    if (ga_players_in(a,e,0xe5342)) return;
    if (S16(W(0x3c))>=0x18 && S16(W(0x3c))<=0x128) SL(0,0x8a00a);
}
static void k8a37c(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x8e)) return;
    if (!B(0x88)) k8a3d4(a,e); else k8a3f6(a,e);
    uint8_t d0=B(0x89);
    if (!d0) return;
    uint32_t was=L(0);
    k8a5dc(a,e,0xe5362u,d0==2u?3u:W(0x76));
    if (L(0)!=was) SB(0x8a,3);
}
static ga_result_t soldier_tail(neo_gactor_t *a,neo_genv_t *e) {     /* 08A1BA */
    SB(0x89,0);
    if (ga_cull(a,e,0xe530a)) return done(e,GA_DELETE_TREE);
    bool c=k8a654(a,e);SB(0x8e,c?0xff:0);
    if (c) {SB(0x8a,1);SW(0x66,0);SW(0x1e,0);} else {SW(0x66,W(0x80));SW(0x1e,W(0x80));}
    depth(a);
    if (!B(0x8a)) ga_register(a,e,NULL,NULL); else SB(0x8a,B(0x8a)-1u);
    if (BIT(0x5f,7)) {SL(0,0x89e34);if (B(0x5a)==7u) SL(0,0x89ed6);}
    SB(0x5a,0);
    if (B(0x85)) SB(0x85,B(0x85)-1u);
    if (S16(W(0x28))>S16(W(0x82))) SL(0,0x8a00a);
    k8a618(a,e);
    return done(e,GA_MORE);
}
static ga_result_t soldier_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;int16_t d;
    switch (L(0)) {
    case 0x89948: case 0x8995a: case 0x8996a: {
        uint16_t kind=L(0)==0x89948u?0:L(0)==0x8995au?1:2;
        uint16_t d1=W(0x6e);SW(0x76,kind);                               /* 08A080 */
        if (d1) {
            neo_gactor_t *c=spawn(a,e,0x8a264,0xe);
            if (c) ga_sb(c,0x70,0);
            if (W(0x76)==1u) {c=spawn(a,e,0x8a264,0xe);if (c) ga_sb(c,0x70,1);}
        }
        SL(0x72,0xe5372);
        if (kind==0) ga_floor(a,e);
        else if (kind==1) k89d92(a);
        else {
            SB(0x15,B(0x15)|2u);
            SL(0x30,0);SL(0x34,0x4000000);
            if (!try_air(a,e)) {ga_fail(e,0x89980);return GA_UNSUPPORTED;}   /* TRAP #F */
        }
        SW(0x4e,rank(a,e,0x2d2a1c));k1b1dc(a);                          /* 08A0CE */
        SB(0x87,0);SB(0x85,0);SB(0x88,0);SB(0x89,0);SB(0x8a,0);SB(0x8b,0);
        SB(0x8e,k8a654(a,e)?0xff:0);
        SW(0x7c,ga_rand_range(e,8)-4u);
        ga_palette(a,0xc4);
        SB(0x15,B(0x15)&~1u);
        SW(0x7a,rank(a,e,0x2d2c06));
        SB(0x86,(uint8_t)(rank(a,e,0x2d2a7e)*4u));
        k8a60e(a,e);
        if (S16(W(0x3c))>=0xa0) SB(0x15,B(0x15)&~1u); else SB(0x15,B(0x15)|1u);
        SW(0x80,W(0x76)==1u?0x1110:0x8820);
        SW(0x66,0);SW(0x1e,0);
        SW(0x7e,ga_rand_range(e,(uint16_t)(k8a502(a,e)>>2)));
        SW(0x82,0x7fff);
        if (!e->pickup) {ga_fail(e,0x8a1a8);return GA_UNSUPPORTED;}
        if (e->pickup->mission==8u) SW(0x82,0x8c0);
        k8a5dc(a,e,0xe534a,W(0x76));
        return done(e,GA_MORE);
    }
    case 0x899a6:
        START(0x284c18);k8a528(a,e);SL(0,0x899c0);
        /* fall through */
    case 0x899c0: {
        uint32_t kind;
        k89a68(a,e);d=k8a480(a,e);
        if (!d) SL(0,0x899f8);
        if (d<0) k8a4f6(a,0x89bb0);
        SL(0x62,L(0x6e));ga_ground(a,e,&kind);STEP();
        goto walked;
    }
    case 0x899f8:
        START(0x284cb2);k1b1dc(a);SL(0,0x89a14);
        /* fall through */
    case 0x89a14:
        ga_stationary(a,e);STEP();
        k89a68(a,e);
        if (k8a480(a,e)) SL(0,0x899a6);
        goto walked;
    case 0x89a38:
        START(0x284d36);SL(0,0x89a4e);
        /* fall through */
    case 0x89a4e:
        ga_stationary(a,e);STEP();
        if (r) SL(0,0x899a6);
        goto walked;
    case 0x89a8c:
        START(0x284df0);k8a528(a,e);SL(0,0x89aa6);
        /* fall through */
    case 0x89aa6:
        d=k8a480(a,e);
        if (!d) SL(0,0x89ada);
        if (d<0) k8a4f6(a,0x89bb0);
        SL(0x30,L(0x6e));ga_air(a,e);STEP();
        goto walked;
    case 0x89ada:
        START(0x284e7c);k1b1dc(a);SL(0,0x89af6);
        /* fall through */
    case 0x89af6:
        ga_stationary(a,e);STEP();
        if (k8a480(a,e)) SL(0,0x89a8c);
        goto walked;
    case 0x89b16:
        START(0x284c26);k8a528(a,e);SL(0,0x89b30);
        /* fall through */
    case 0x89b30:
        d=k8a480(a,e);
        if (!d) SL(0,0x89b6c);
        if (d<0) k8a4f6(a,0x89bb0);
        SL(0x30,L(0x6e));
        SW(0x34,8);                                                      /* 08A590 */
        if (!try_air(a,e)) SL(0,0x89bfe);
        STEP();
        goto walked;
    case 0x89b6c:
        START(0x284cb2);k1b1dc(a);SL(0,0x89b88);
        /* fall through */
    case 0x89b88:
        ga_stationary(a,e);STEP();
        if (k8a480(a,e)) SL(0,0x89b16);
        goto walked;
    case 0x89bb0:
        START(rl(e,0xe5282u+4u*W(0x76)));SL(0,0x89bd2);
        /* fall through */
    case 0x89bd2:
        ga_stationary(a,e);STEP();
        if (r) {SB(0x15,B(0x15)^1u);SB(0x85,0xf);k8a5dc(a,e,0xe534a,W(0x76));}
        goto walked;
    case 0x89bfe:
        START(0x284f08);goto fall;
    case 0x89c12:
        START(0x284f68);
    fall:
        SB(0x8b,0);SB(0x15,B(0x15)|0x20u);SL(0,0x89c36);
        /* fall through */
    case 0x89c36:
        if (!B(0x8b)) ga_stationary(a,e); else k8a646(a,e);
        STEP();
        if (r) {SB(0x15,B(0x15)&~0x20u);k8a5dc(a,e,0xe534a,W(0x76));}
        return soldier_tail(a,e);
    case 0x89c6e:
        START(0x285088);SL(0x30,0);SL(0x34,0);SW(0x80,0x8820);SB(0x15,B(0x15)|0x20u);SL(0,0x89cb6);
        /* fall through */
    case 0x89cb6:
        k8a646(a,e);STEP();
        if (BIT(0x15,6)) {SB(0x15,B(0x15)&~0x20u);SL(0,0x89cda);}
        return soldier_tail(a,e);
    case 0x89cda:
        k1b1dc(a);START(0x285026);SW(0x76,0);SL(0,0x89cfc);
        /* fall through */
    case 0x89cfc:
        ga_stationary(a,e);STEP();
        if (r) k8a5dc(a,e,0xe534a,W(0x76));
        return soldier_tail(a,e);
    case 0x89d1e: SL(0x34,0x40000);goto rise;
    case 0x89d2a: SL(0x34,0xfffc0000u);
    rise:
        START(0x284df0);k8a528(a,e);SB(0x84,6);SL(0,0x89d56);
        /* fall through */
    case 0x89d56:
        SL(0x30,L(0x6e));ga_air(a,e);k89d92(a);ga_project(a,e);STEP();
        SB(0x84,B(0x84)-1u);
        if (B(0x84)&0x80u) {k1b1dc(a);k8a5dc(a,e,0xe534a,W(0x76));}
        return soldier_tail(a,e);
    case 0x89db4:
        START(rl(e,0xe526au+4u*W(0x76)));SB(0x88,1);SW(0x78,0);SL(0,0x89de0);
        /* fall through */
    case 0x89de0: {
        ga_stationary(a,e);STEP();
        uint16_t n=(uint16_t)(W(0x78)+1u);SW(0x78,n);
        uint8_t q=(uint8_t)(n/6u+1u);
        if (q>5u) {SW(0x78,rank(a,e,0x2d2ba4));k8a5dc(a,e,0xe534a,W(0x76));}
        else {SB(0x8c,q);k8a618(a,e);}
        return soldier_tail(a,e);
    }
    case 0x89e34:
        if (!e->blood) {SL(0x72,0xe5382);SB(0x8d,0xff);}                 /* 01B2BE */
        sound(e,0x1144);
        SW(0x66,0x8800);SW(0x1e,0x8800);
        if (!B(0x8b)) START(rl(e,0xe525eu+4u*W(0x76))); else START(0x28538a);
        k1b1dc(a);SL(0,0x89ea6);
        /* fall through */
    case 0x89ea6:
        ga_stationary(a,e);STEP();
        if (r) return done(e,GA_DELETE_TREE);                            /* 0041AC */
        if (B(0x8c)) SB(0x8c,B(0x8c)-(uint8_t)(e->video&1u));
        k8a618(a,e);
        return done(e,GA_MORE);
    case 0x89ed6:
        sound(e,0x1144);
        SB(0x1d,0);ga_palette(a,0x878);                                  /* 009F88, 009EA8 */
        SW(0x66,0x8800);SW(0x1e,0x8800);
        if (W(0x76)==0u) {
            START(0x2854f2);
            uint32_t d0=((uint32_t)(ga_rand(e)&0x7fffu)+0x8000u)<<2,vx,vy;
            ga_polar(d0,(uint8_t)(B(0x5b)<<4),&vx,&vy);
            SL(0x30,L(0x30)+vx);SL(0x34,0);SL(0,0x89f56);
        } else {
            if (W(0x76)==1u) {START(0x285602);SW(0x2c,W(0x2c)+0x18u);SL(0x34,0);}
            else if (W(0x76)==2u) {START(0x28563a);SB(0x15,B(0x15)&~2u);SL(0x34,0xfffc0000u);}
            else {ga_fail(e,0x89f08);return GA_UNSUPPORTED;}
            SL(0,0x89fbe);
            goto burnt_fall;
        }
        /* fall through */
    case 0x89f56:
        SL(0x30,L(0x30)-(uint32_t)((int32_t)L(0x30)>>3));
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                        /* 0143DC */
        STEP();
        if (r) return done(e,GA_DELETE_TREE);
        return done(e,GA_MORE);
    case 0x89fbe:
    burnt_fall:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                        /* 0143DC */
        STEP();
        if (BIT(0x15,6)) SL(0,0x89fdc);
        return done(e,GA_MORE);
    case 0x89fdc:
        START(0x285722);SL(0,0x89ff2);
        /* fall through */
    case 0x89ff2:
        ga_stationary(a,e);STEP();
        if (r) return done(e,GA_DELETE_TREE);
        return done(e,GA_MORE);
    case 0x8a00a:
        sound(e,0x10f1);sound(e,0x1144);
        START(rl(e,0xe5276u+4u*W(0x76)));SL(0,0x8a040);
        /* fall through */
    case 0x8a040:
        ga_stationary(a,e);STEP();
        if (r) return done(e,GA_DELETE_TREE);
        if (B(0x87) && S16(W(0x3c))>=-4 && S16(W(0x3c))<=0x144) ga_attack(a,e,0xe52fa);   /* 0049E8 */
        return done(e,GA_MORE);
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
walked:
    k8a37c(a,e);
    return soldier_tail(a,e);
}
static ga_result_t gun_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x8a264: {
        SW(0x6e,0xffff);
        neo_gactor_t *p=creator(a,e);ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);   /* 01AD6E */
        SL(0,0x8a27a);
    }   /* fall through */
    case 0x8a27a: {
        neo_gactor_t *c=creator(a,e);
        if (!c) {ga_fail(e,0x8a27a);return GA_UNSUPPORTED;}
        if (ga_b(c,0x8e)) return done(e,GA_MORE);
        SB(0x15,ga_b(c,0x15));
        uint16_t d0=ga_w(c,0x76);
        if (d0!=W(0x6e)) {
            SW(0x6e,d0);
            uint32_t t=rl(e,0xe52a6u+4u*d0);
            SL(0x50,rl(e,t+4u*(B(0x70)&1u)));
        }
        ga_project(a,e);
        if (!ga_register(a,e,NULL,NULL)) return done(e,GA_MORE);         /* 004C24 */
        SB(0x5a,0);
        unsigned p=((B(0x5e)>>2)-1u)&1u;
        if (!((e->players.active_mask>>p)&1u)) return done(e,GA_MORE);   /* 01AEB0 */
        uint16_t dx=(uint16_t)((p?e->players.p2x:e->players.p1x)-W(0x3c));
        if (!(e->player_flags15[p]&1u)) dx=(uint16_t)-dx;
        if (ga_w(c,0x76)!=2u && S16(dx)<0x10) return done(e,GA_MORE);
        uint8_t s=(uint8_t)((B(0x5b)+(ga_w(c,0x76)==2u?5u:1u))&7u);
        if (s<3u) ga_sb(c,0x89,B(0x70)?2:1);
        return done(e,GA_MORE);
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static const uint32_t crank_r[]={0x959b0,0x959d4,0x95c98,0x95d2e,0x95d6e,0x95e08,0x95e58,0x95f8c,0x95fae,0x95ffc,0x960a6,0x96140,0x961ce};
static const uint32_t soldier_r[]={0x89948,0x8995a,0x8996a,0x899a6,0x899c0,0x899f8,0x89a14,0x89a38,0x89a4e,0x89a8c,0x89aa6,0x89ada,
    0x89af6,0x89b16,0x89b30,0x89b6c,0x89b88,0x89bb0,0x89bd2,0x89bfe,0x89c12,0x89c36,0x89c6e,0x89cb6,0x89cda,0x89cfc,0x89d1e,0x89d2a,
    0x89d56,0x89db4,0x89de0,0x89e34,0x89ea6,0x89ed6,0x89f56,0x89fbe,0x89fdc,0x89ff2,0x8a00a,0x8a040};
static bool in(const uint32_t *k,unsigned n,uint32_t r) {for (unsigned i=0;i<n;i++) if (k[i]==r) return true;return false;}
#define IN(t,r) in(t,sizeof t/sizeof t[0],r)
bool neo_m5sewer_routine(uint32_t r) {return IN(crank_r,r) || IN(soldier_r,r) || r==0x8a264u || r==0x8a27au;}
ga_result_t neo_m5sewer_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (IN(crank_r,r)) return crank_tick(a,e);
    if (r==0x8a264u || r==0x8a27au) return gun_tick(a,e);
    return soldier_tick(a,e);
}
