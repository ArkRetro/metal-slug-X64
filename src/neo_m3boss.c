#include "neo_region.h"
/* Mission 3: the final boss 04A2C2 (script record 2DB5E4, camera 6496),
 * manual translation in pieces (ROM labels as provenance;
 * docs/JEFE_MISION3.md, "Implementación propuesta").
 * Piece 1, the skeleton (fixture m3boss0, 144410..144886):
 * - Body 04A2C2: music 0x28 (059F4E), torso 04B252 and legs 04AF02/04B0E6
 *   (0040BC, reserve 0xE), health 2C1380, smoke 01DD10, the walk-in
 *   (04D66E, +60 bit 0), the decision 04DA38 (rank tables of P2 bank 1,
 *   order bytes 04DC54), the steps left/right 04A488/04A5DE, the stop
 *   04A668 with the footsteps (+60 bits 2/4), the crouch 04A74A/04A7CC
 *   with the second choice 04DC5C, standing up 04A89E, the return to x
 *   0x100 04A94C; phase 04D6F2 (+A4), hurtbox 04E008 (E108A/E103A/E0E8A),
 *   dynamic map 04E04E (E104A, map E0D00).
 * - Legs and torso: programs by +9D with the handshake 04DDD4 (+9E); the
 *   torso passes its pose +98 to the body and makes the arm 04B3BC, the
 *   missile pod 04B920, the hatch 04C38A (and its floor 04C69E, map E0E36
 *   while the body's +9D is 7) and the mortar 04CF2C; their waits on the
 *   handshake 04DF02 at the pose points E0F9A (04D9DE).
 * - Footsteps 04E2EA/04E3BE (callbacks 04E296/04E2C0) and dust
 *   04AD12/04AD94 (04E520/04E53C).
 * Explicit stops: the attacks (arm 04B52C, pod 04BA16, hatch 04C4D2, mortar
 * 04CF92), the death 04A9D4 and the leg/torso deaths 04B01E/04B208 until
 * their pieces. The cartridge check before some deletions (10F00A/10F00C)
 * is not reproduced (as in neo_m3mid.c). */
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
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2);}
static uint8_t rb(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;}return p[0];}
/* 0260C8 (type 2 tables are random: the boss's rank words). */
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    if (t[0]!=2u) return ga_rank_word(a,e,t);
    unsigned coop=e->players.active_mask==3u;
    unsigned rk=e->players.mode88==2?e->players.rank:3u;
    unsigned mask=(1u<<((rk&7u)+1u))-1u,row=0,count=0;
    for (unsigned b=t[1]&mask&0xffu;b;b&=b-1u) row++;
    for (unsigned b=t[1];b;b&=b-1u) count++;
    if (!row) {ga_fail(e,0x26118);return 0;}
    unsigned d0=((row-1u)*2u+coop)*6u+B(0x68);
    unsigned o=2u+(unsigned)t[2+d0]*16u+count*12u+(ga_rand(e)&0xeu);
    return rw(e,table+o);
}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;if (!c) ga_fail(e,0x4000cu);return c;}
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
static void place(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {   /* 01AD6E */
    neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;ga_place(a,e,dx,dy,p?NEO_GA_TAG(p->index):0);
}
static bool step(neo_gactor_t *a,neo_genv_t *e,ga_call_t call,bool *fin) {     /* 00427A: carry = finished */
    int r=ga_anim_step(a,e,call);if (r<0) return false;*fin=r!=0;return !e->unsupported;
}
/* 0044F6: the creator's flash timer (+17). */
static void flash_share(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);if (!p) return;
    uint8_t d0=ga_b(p,0x17);if (!d0) return;
    if (!(d0&0x80u)) d0++;
    SB(0x17,d0);
}
/* 01AEB0 / 0AAA88 for player p. */
static bool active(neo_genv_t *e,unsigned p) {return (e->players.active_mask>>p)&1u;}
static bool riding(neo_genv_t *e,unsigned p) {
    return e->vehicle && e->vehicle->slots && e->vehicle->exists[p] && neo_vslot_node(&e->vehicle->slots->slot[p]);
}
/* 01B04A: player 1, 2 or (both live) one at random; false when none. */
static bool k1b04a(neo_genv_t *e,unsigned *p) {
    unsigned d0=e->players.active_mask&3u;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    *p=d0==2u?1u:0u;
    return d0!=0;
}
/* 059F4E: the music after the delay of 059F76 (neo_m2boss.c runs it). */
static void music(neo_gactor_t *a,neo_genv_t *e,uint16_t id) {
    neo_gactor_t *c=child(a,e,0x59f76,0);
    if (c) ga_sw(c,0x72,id);
}
/* 01DD10: the damage smoke 01D86C (+70 = parameter). */
static void smoke(neo_gactor_t *a,neo_genv_t *e,uint16_t d0) {
    neo_gactor_t *c=child(a,e,0x1d86c,0x1c);
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    ga_sb(c,0x94,0);ga_sw(c,0x6e,0);ga_sw(c,0x70,d0);ga_sw(c,0x72,0x7f);ga_sw(c,0x74,0x1f);
}
/* ---- body helpers -------------------------------------------------------- */
static void k4d66e(neo_gactor_t *a,unsigned field,bool walk_in) {     /* 04D66E (+3C, doubled) / 04D6B6 (+28) */
    int32_t d0=(int32_t)L(0x62);
    if (walk_in && BIT(0x60,0)) d0+=d0;
    int32_t d1=(int32_t)((uint32_t)d0>>3);
    if (S16(W(field))>S16(W(0x8a))) {
        d0=-d0;SL(0x30,(int32_t)L(0x30)-d1);
        if (d0>(int32_t)L(0x30)) SL(0x30,d0);
    } else {
        SL(0x30,(int32_t)L(0x30)+d1);
        if (d0<(int32_t)L(0x30)) SL(0x30,d0);
    }
}
static void k4d6f2(neo_gactor_t *a,neo_genv_t *e) {SB(0xa4,S16(rank(a,e,0x2c13e2))>=S16(W(0x4e))?1:0);}
static void k4e008(neo_gactor_t *a,neo_genv_t *e) {                       /* hurtbox by pose */
    for (uint32_t t=0xe108a;!e->unsupported;t+=4) {
        if (S16(W(0x98))>S16(rw(e,t))) continue;
        uint32_t d0=rl(e,0xe103au+4u*rw(e,t+2));
        if (d0==0xe0e8au) d0=rl(e,d0+4u*B(0xa3));
        SL(0x50,d0);return;
    }
}
static void k4e04e(neo_gactor_t *a,neo_genv_t *e) {                       /* the dynamic map by pose */
    for (uint32_t t=0xe104a;!e->unsupported;t+=4) {
        if (S16(W(0x98))>S16(rw(e,t))) continue;
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe0d00,0,S16(rw(e,t+2)));   /* 0140C4 */
        return;
    }
}
static bool k4dd86(neo_genv_t *e) {for (unsigned p=0;p<2;p++) if (active(e,p) && riding(e,p)) return true;return false;}
static void k4ae6a(neo_gactor_t *a,neo_genv_t *e) {SB(0xa5,0);if (active(e,0)) SET(0xa5,0);if (active(e,1)) SET(0xa5,1);}
static bool k4ae98(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned p=0;p<2;p++) if (active(e,p)!=(bool)BIT(0xa5,p)) return true;
    return false;
}
static uint8_t order(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    uint16_t d0=rank(a,e,table);
    if (S16(d0)>=8) d0=0;
    return rb(e,0x4dc54u+d0);
}
static const uint8_t *player(neo_genv_t *e,unsigned p) {
    const uint8_t *n=e->vehicle?e->vehicle->player[p]:NULL;
    if (!n) ga_fail(e,0x1b04a);
    return n;
}
static bool k4da38(neo_gactor_t *a,neo_genv_t *e) {                       /* carry: order 8 or 4 */
    unsigned p;
    if (B(0x9e)) return false;
    if (!k1b04a(e,&p)) return false;
    if (W(0x84)) {SW(0x84,W(0x84)-1u);return false;}
    SW(0x84,W(0x80));
    if (B(0xa1)) {SB(0xa1,0);SB(0x9d,3);SB(0x9e,0xff);return false;}
    k1b04a(e,&p);SL(0x6e,p?0x101574u:0x1014c4u);
    const uint8_t *n=player(e,p);if (!n) return false;
    int16_t px=(int16_t)(n[0x3c]<<8|n[0x3d]),py=(int16_t)(n[0x40]<<8|n[0x41]);
    uint8_t d0;
    if (S16(W(0x3c)-0x40u)<=px) d0=S16(W(0x40)+0x60u)>py?4:5;
    else {
        bool v=riding(e,p);
        d0=order(a,e,v?(B(0xa4)?0x2c15a4u:0x2c1556u):(B(0xa4)?0x2c1640u:0x2c15f2u));
        if (d0==4) {SET(0xa0,7);SET(0xa0,4);}
    }
    if (d0==7 && k4dd86(e)) return false;
    if (d0==4) {SB(0xa1,1);SW(0x26,rank(a,e,B(0xa4)?0x2c17e0u:0x2c17c6u));SW(0x7e,W(0x26));}
    SB(0x9d,d0);SB(0x9e,0xff);
    if (d0==8) {SB(0x9e,0);return true;}
    if (d0==4) {SET(0xa0,4);SB(0x9e,0);return true;}
    return false;
}
static void k4dc5c(neo_gactor_t *a,neo_genv_t *e) {                       /* the second choice, crouched */
    if (!BIT(0xa0,3)) return;
    unsigned p=L(0x6e)==0x1014c4u?0u:1u;
    bool v=riding(e,p);
    uint8_t d0=order(a,e,v?(B(0xa4)?0x2c16dcu:0x2c168eu):(B(0xa4)?0x2c1778u:0x2c172au));
    if (d0==4 || d0==8) d0=7;
    if (d0==7) {if (v || !k4dd86(e)) SET(0xa0,6);return;}
    SB(0x9d,d0);SB(0x9e,0xff);
}
static int32_t toward(neo_gactor_t *a,unsigned field) {int32_t d0=(int32_t)L(0x62);return S16(W(field))<S16(W(0x8a))?-d0:d0;}
/* ---- callbacks --------------------------------------------------------------- */
static bool call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    neo_gactor_t *c;
    switch (routine) {
    case 0x4e27e: if (!(c=creator(a,e))) return false;ga_sb(c,0x60,(uint8_t)(ga_b(c,0x60)|0x40u));return true;
    case 0x4e28a: if (!(c=creator(a,e))) return false;ga_sb(c,0x60,(uint8_t)(ga_b(c,0x60)&~0x40u));return true;
    case 0x4e296: case 0x4e2c0: {
        uint8_t bit=routine==0x4e296?4u:0x10u;
        if (!(c=creator(a,e))) return false;
        if (ga_b(c,0x60)&bit) {child(a,e,routine==0x4e296?0x4e2ea:0x4e3be,0xe);ga_sb(c,0x60,(uint8_t)(ga_b(c,0x60)&~bit));}
        return !e->unsupported;
    }
    case 0x4e520: case 0x4e53c: {
        neo_gactor_t *d=child(a,e,routine==0x4e520?0x4ad12:0x4ad94,0x2a);
        if (d) ga_place_at(d,a,e,routine==0x4e520?0x40:-0x38,routine==0x4e520?0x10:0x30);
        return !e->unsupported;
    }
    }
    return false;
}
#define STEP(f) do { if (!step(a,e,call,&(f))) return GA_UNSUPPORTED; } while (0)
static ga_result_t hit_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);                        /* 004C24 */
    if (BIT(0x5f,7)) SL(0,0x4a9d4);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* the shared tail of 04A4C2 / 04A618 / 04A57C / 04A866 / 04A8CA / 04A974 */
static bool walk(neo_gactor_t *a,neo_genv_t *e,bool approach) {
    bool f;
    if (approach) k4d66e(a,0x28,false);                                /* 04D6B6 */
    k4e008(a,e);ga_move(a,e);
    if (!step(a,e,call,&f)) return false;
    k4e04e(a,e);
    return !e->unsupported;
}
static ga_result_t body_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;
    switch (L(0)) {
    case 0x4a2c2:
        music(a,e,0x28);SB(0x1d,0);                                    /* 009F88 */
        SW(0x66,0xfff1);SW(0x1e,0xfff1);
        child(a,e,0x4b252,0xe);child(a,e,0x4af02,0xe);child(a,e,0x4b0e6,0xe);
        SB(0x9d,3);SB(0x9e,0);SB(0xa1,0);SB(0x61,0);SB(0x60,0);SW(0x98,0);SB(0xa4,0);
        SET(0x60,0);SET(0x5f,6);
        ga_project(a,e);START(0x2451dc);
        SW(0x4e,rank(a,e,0x2c1380));SW(0x8a,0x100);SL(0x62,0x8000);
        smoke(a,e,0x78);SL(0,0x4a380);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4a380:
        if (BIT(0xa0,6)) {SB(0x9d,7);SB(0x9e,0xff);}
        SB(0xa0,0);SW(0x8a,0x100);SL(0x30,toward(a,0x3c));
        SW(0x80,rank(a,e,B(0xa4)?0x2c14a6:0x2c1444));SW(0x84,W(0x80));
        SL(0,0x4a3f0);
        /* fall through */
    case 0x4a3f0:
        k4d6f2(a,e);k4d66e(a,0x3c,true);k4e008(a,e);ga_move(a,e);STEP(f);k4e04e(a,e);
        if (!BIT(0x60,0)) {
            if (k4da38(a,e)) {
                if (B(0x9d)==8) SL(0,0x4a488);
                if (B(0x9d)==4) {SET(0xa0,3);SL(0,0x4a668);}
            }
        } else if (S16(W(0x3c))<=0x100) {CLR(0x60,0);ga_flash_palette(a,0x148);CLR(0x5f,6);}
        return hit_tail(a,e);
    case 0x4a488:
        if (S16(W(0x3c)-0x30u)<=0x30) goto k4a512;
        SW(0x8a,W(0x28)-0x30u);
        goto side;
    case 0x4a5de:
        if (S16(W(0x3c)+0x30u)>=0x100) goto k4a94c;
        SW(0x8a,W(0x28)+0x30u);
    side:
        SL(0x30,0x30000);
        if (S16(W(0x8a))<S16(W(0x28))) SL(0x30,-(int32_t)L(0x30));
        SL(0,L(0)==0x4a488?0x4a4c2:0x4a618);
        /* fall through */
    case 0x4a4c2: case 0x4a618: {
        if (!walk(a,e,false)) return GA_UNSUPPORTED;
        int16_t d=S16(W(0x28)-W(0x8a));if (d<0) d=(int16_t)-d;
        if (d<=3) SL(0,0x4a668);
        return hit_tail(a,e);
    }
    k4a512:
        SW(0x8a,W(0x28));
        if (!(ga_rand(e)&1u)) SET(0xa0,1);
        SET(0xa0,2);
        SW(0x88,rank(a,e,B(0xa4)?0x2c1814:0x2c17fa));
        SL(0x30,toward(a,0x28));SL(0,0x4a57c);
        /* fall through */
    case 0x4a57c:
        if (!walk(a,e,true)) return GA_UNSUPPORTED;
        if (!W(0x88)) SL(0,BIT(0xa0,1)?0x4a5de:0x4a94c);
        else SW(0x88,W(0x88)-1u);
        return hit_tail(a,e);
    case 0x4a668:
        SW(0x8a,W(0x28));
        if (BIT(0xa0,3)) {
            SW(0x88,rank(a,e,B(0xa4)?0x2c1814:0x2c17fa));SW(0x82,rank(a,e,B(0xa4)?0x2c1848:0x2c182e));
        } else {
            SW(0x88,rank(a,e,B(0xa4)?0x2c187c:0x2c1862));SW(0x82,rank(a,e,B(0xa4)?0x2c18b0:0x2c1896));
        }
        SL(0x30,toward(a,0x28));
        CLR(0x60,1);SET(0x60,2);SET(0x60,4);k4ae6a(a,e);SL(0,0x4a74a);
        /* fall through */
    case 0x4a74a:
        if (!BIT(0x60,2) && !BIT(0x60,4)) {
            if (W(0x82)) SW(0x82,W(0x82)-1u);
            else if (!BIT(0x60,1)) {if (BIT(0x60,6)) {SET(0x60,3);SET(0x60,5);SET(0x60,1);}}
            else if (!BIT(0x60,3) && !BIT(0x60,5)) {SB(0x9d,4);SB(0xa1,1);SB(0x9e,0xff);SL(0,0x4a7cc);}
        }
        if (!walk(a,e,true)) return GA_UNSUPPORTED;
        return hit_tail(a,e);
    case 0x4a7cc:
        if (!B(0x9e)) {
            if (k4ae98(a,e)) SET(0xa5,7);
            if (W(0x88)) SW(0x88,W(0x88)-1u);
            else if (BIT(0xa5,7)) {SET(0xa0,5);CLR(0xa0,7);SET(0xa0,3);SL(0,0x4a89e);}
            else if (BIT(0xa0,7)) {CLR(0xa0,7);SET(0xa0,5);k4dc5c(a,e);}
            else if (!BIT(0xa0,5)) {SET(0xa0,5);if (rank(a,e,0x2c1508)) k4dc5c(a,e);}
            else SL(0,0x4a89e);
        }
        if (!walk(a,e,true)) return GA_UNSUPPORTED;
        return hit_tail(a,e);
    case 0x4a89e:
        SB(0x9d,3);SB(0xa1,0);SB(0x9e,0xff);SL(0x30,toward(a,0x28));SL(0,0x4a8ca);
        /* fall through */
    case 0x4a8ca:
        if (!walk(a,e,true)) return GA_UNSUPPORTED;
        if (!B(0x9e)) SL(0,BIT(0xa0,3)?0x4a94c:!BIT(0xa0,2)?0x4a488:BIT(0xa0,1)?0x4a5de:0x4a94c);
        return hit_tail(a,e);
    k4a94c:
    case 0x4a94c:
        SW(0x88,0);SL(0x30,0x30000);SW(0x8a,0x100);
        if (S16(W(0x8a))<S16(W(0x3c))) SL(0x30,-(int32_t)L(0x30));
        SL(0,0x4a974);
        /* fall through */
    case 0x4a974:
        if (!walk(a,e,false)) return GA_UNSUPPORTED;
        if (S16(W(0x8a)-W(0x3c))<0) {if (!((int32_t)L(0x30)<0)) SL(0,0x4a380);}
        else if ((int32_t)L(0x30)<0) SL(0,0x4a380);
        return hit_tail(a,e);
    }
    STOP(L(0));                                                        /* 04A9D4 death: piece 6 */
}
/* ---- legs, torso ------------------------------------------------------------- */
/* 04DDD4: legs (+A1 1/2) and torso (+A1 0) follow the body's order. */
static bool k4ddd4(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);if (!p) return false;
    SL(0x6e,ga_l(p,0x6e));
    if (ga_b(p,0x9e)!=0xffu) return false;
    uint8_t o=ga_b(p,0x9d);
    if (!B(0xa1)) {
        if (o==3 || o==4) {
            if (B(0x9d)!=o) {ga_sb(p,0x9e,0x7f);SB(0x9e,0);SB(0x9d,o);return true;}
            ga_sb(p,0x9e,0);
        }
        return false;
    }
    if (B(0xa1)==1 || B(0xa1)==2) {
        if ((o==3 || o==4) && B(0x9d)!=o) {SB(0x9d,o);return true;}
    }
    return false;
}
/* 04DF02: the torso's children (+A1 3..6) take their orders. */
static bool k4df02(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *t=creator(a,e),*p=t?creator(t,e):NULL;if (!p) return false;
    SL(0x6e,ga_l(p,0x6e));
    if (ga_b(p,0x9e)!=0xffu) return false;
    uint8_t o=ga_b(p,0x9d),r=B(0xa1);
    bool take=(r==3 && (o==0 || o==1)) || (r==4 && (o==6 || o==2)) || (r==5 && o==7) || (r==6 && o==5);
    if (!take) return false;
    ga_sb(p,0x9e,0x7f);SB(0x9d,o);return true;
}
static bool k4d74a(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *t=creator(a,e),*p=t?creator(t,e):NULL;return p && ga_b(p,0x61);}
static neo_gactor_t *grand(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *t=creator(a,e);return t?creator(t,e):NULL;}
/* 04D9DE: the program step at the torso's pose point E0F9A[+98]. */
static bool k4d9de(neo_gactor_t *a,neo_genv_t *e,bool *fin) {
    flash_share(a,e);
    neo_gactor_t *c=creator(a,e);if (!c) return false;
    uint32_t x=L(0x3c),y=L(0x40);
    uint16_t pose=ga_w(c,0x98);SW(0x98,pose);
    uint32_t t=(uint32_t)(0xe0f9a+(int16_t)(uint16_t)(pose<<2));
    SW(0x3c,W(0x3c)+rw(e,t));SW(0x40,W(0x40)+rw(e,t+2));
    bool ok=step(a,e,call,fin);
    SL(0x40,y);SL(0x3c,x);
    return ok;
}
static ga_result_t leg_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f,second=L(0)>=0x4b0e6u;
    neo_gactor_t *p;
    switch (L(0)) {
    case 0x4af02: case 0x4b0e6:
        SB(0x1d,0);ga_palette(a,0xba);ga_palette(a,0x157);SB(0x14,B(0x19));
        SW(0x66,second?0x8800:0xe0dc);SW(0x1e,W(0x66));
        ga_flash_palette(a,0x148);place(a,e,0,0);START(second?0x245294:0x2451e8);
        if (second) SET(0x15,2);
        SB(0xa1,second?2:1);SB(0x9d,3);SL(0,second?0x4b1b8:0x4afce);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4af72: case 0x4b15c:
        START(second?0x2458c8:0x24585a);
        if (!(p=creator(a,e))) return GA_UNSUPPORTED;
        SW(0x26,ga_b(p,0xa4)==1?0x200:0x100);SL(0,second?0x4b1b8:0x4afce);
        break;
    case 0x4afaa: case 0x4b194:
        START(second?0x24571e:0x24569c);
        if (!(p=creator(a,e))) return GA_UNSUPPORTED;
        SW(0x26,ga_w(p,0x26));SL(0,second?0x4b1b8:0x4afce);
        break;
    case 0x4afce: case 0x4b1b8: break;
    default: STOP(L(0));                                               /* 04B01E / 04B208: piece 6 */
    }
    ga_project(a,e);flash_share(a,e);STEP(f);
    if (k4ddd4(a,e)) {
        if (B(0x9d)==3) SL(0,second?0x4b15c:0x4af72);
        if (B(0x9d)==4) SL(0,second?0x4b194:0x4afaa);
    }
    if (!(p=creator(a,e))) return GA_UNSUPPORTED;
    if (ga_b(p,0x9d)==0x7fu) SL(0,second?0x4b208:0x4b01e);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t torso_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;neo_gactor_t *p;
    switch (L(0)) {
    case 0x4b252:
        SB(0x1d,0);ga_palette(a,0xba);SW(0x66,0x9100);SW(0x1e,0x9100);ga_flash_palette(a,0x148);place(a,e,0,0);
        child(a,e,0x4b3bc,0xe);child(a,e,0x4b920,0xe);child(a,e,0x4c38a,0xe);child(a,e,0x4cf2c,0xe);
        START(0x245340);SB(0xa1,0);SB(0x9d,3);SB(0x9e,0);SB(0x9f,0);SL(0,0x4b352);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4b2f6:
        START(0x245936);
        if (!(p=creator(a,e))) return GA_UNSUPPORTED;
        SW(0x26,ga_b(p,0xa4)==1?0x200:0x100);SL(0,0x4b352);
        break;
    case 0x4b32e:
        START(0x2457a0);
        if (!(p=creator(a,e))) return GA_UNSUPPORTED;
        SW(0x26,ga_w(p,0x26));SL(0,0x4b352);
        break;
    case 0x4b352: break;
    default: STOP(L(0));
    }
    ga_project(a,e);flash_share(a,e);STEP(f);
    if (!(p=creator(a,e))) return GA_UNSUPPORTED;
    if (B(0x9e)) {ga_sb(p,0x9e,0);SB(0x9e,0);}
    if (k4ddd4(a,e)) {
        if (B(0x9d)==4) SL(0,0x4b32e);
        if (B(0x9d)==3) SL(0,0x4b2f6);
    }
    ga_sw(p,0x98,W(0x98));
    if (ga_b(p,0x9d)==0x7fu) return GA_DELETE;                         /* 004188 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- arm, pod, hatch, floor, mortar (waiting) ---------------------------------- */
static ga_result_t piece_init(neo_gactor_t *a,neo_genv_t *e,uint16_t pal2,uint16_t depth,int16_t dx,int16_t dy,uint8_t role) {
    SB(0x1d,0);
    if (role!=5) ga_palette(a,0xba);
    if (pal2) ga_palette(a,pal2);
    SW(0x66,depth);SW(0x1e,depth);ga_flash_palette(a,0x148);place(a,e,dx,dy);
    SB(0xa1,role);SB(0x9d,3);SB(0x61,0);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t arm_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *p,*g;
    switch (L(0)) {
    case 0x4b3bc: piece_init(a,e,0,0x9110,-0xe,0x33,3);SL(0,0x4b40c);return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4b40c: SB(0x9a,0);START(0x2459f4);SL(0,0x4b426);
        /* fall through */
    case 0x4b426: break;
    default: STOP(L(0));                                               /* 04B52C: the flamethrower, piece 2 */
    }
    ga_project(a,e);
    if (!(p=creator(a,e))) return GA_UNSUPPORTED;
    if (!ga_b(p,0x9f)) {
        START(S16(ga_w(p,0x98))>=0x25?0x2459e8:0x2459f4);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
    }
    if (k4df02(a,e)) SL(0,0x4b52c);
    if (k4d74a(a,e)) SB(0x61,0xff);
    if ((g=grand(a,e)) && ga_b(g,0x9d)==0x7fu) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t pod_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *p,*g;
    switch (L(0)) {
    case 0x4b920: piece_init(a,e,0xbc,0x9100,-0xe,0x1b,4);SL(0,0x4b97e);return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4b97e: START(0x245d86);SB(0x14,B(0x19));SL(0,0x4b99a);
        /* fall through */
    case 0x4b99a: break;
    default: STOP(L(0));                                               /* 04BA16: the missiles, piece 5 */
    }
    ga_project(a,e);
    if (!(p=creator(a,e))) return GA_UNSUPPORTED;
    SW(0x98,ga_w(p,0x98));
    START(S16(ga_w(p,0x98))>=0x25?0x2459e8:0x245d86);
    if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
    if (k4df02(a,e)) SL(0,0x4ba16);
    if (k4d74a(a,e)) SB(0x61,0xff);
    if ((g=grand(a,e)) && ga_b(g,0x9d)==0x7fu) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t hatch_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *g;
    switch (L(0)) {
    case 0x4c38a: piece_init(a,e,0xbc,0x90f0,0,0,5);child(a,e,0x4c69e,0x1c);SL(0,0x4c3ea);return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4c3ea:
        SB(0xa3,0);
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        ga_sb(g,0xa3,B(0xa3));START(0x2459e8);SL(0,0x4c412);
        /* fall through */
    case 0x4c412: break;
    default: STOP(L(0));                                               /* 04C4D2: the hatch, piece 4 */
    }
    ga_project(a,e);
    if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
    if (k4df02(a,e)) SL(0,0x4c4d2);
    if ((g=grand(a,e)) && ga_b(g,0x9d)==0x7fu) {SB(0x9d,0x7f);return GA_DELETE;}
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t floor_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *h,*b;
    if (L(0)==0x4c69e) {place(a,e,0,0);START(0x2451dc);SL(0,0x4c6c2);}
    ga_project(a,e);
    if (!(h=creator(a,e)) || !(b=grand(h,e))) return GA_UNSUPPORTED;
    if (ga_b(b,0x9d)==7) {
        if (ga_b(h,0x61)) {if (e->dyn_release) e->dyn_release(e->ctx,a);return GA_DELETE;}
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe0e36,0,(int8_t)ga_b(h,0x9f));   /* 0140C4 */
    } else if (e->dyn_release) e->dyn_release(e->ctx,a);                  /* 013F1E */
    if (ga_b(h,0x9d)==0x7fu) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t mortar_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x4cf2c: place(a,e,0,-0x30);SB(0xa1,6);SB(0x9d,3);SB(0x61,0);SW(0x86,0);SL(0,0x4cf56);return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4cf56: SL(0,0x4cf5c);
        /* fall through */
    case 0x4cf5c: break;
    default: STOP(L(0));                                               /* 04CF92: the mortar, piece 3 */
    }
    ga_project(a,e);
    if (k4df02(a,e)) SL(0,0x4cf92);
    if (!W(0x86)) SB(0x9f,0);
    if (k4d74a(a,e)) {SB(0x61,0xff);return GA_DELETE;}
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- footsteps and dust ----------------------------------------------------------- */
static ga_result_t foot_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f,rear=L(0)>=0x4e3beu;neo_gactor_t *b;
    uint8_t bit=rear?0x20u:0x08u;
    if (L(0)==0x4e2ea || L(0)==0x4e3be) {
        SB(0x1d,0);ga_palette(a,0x130);SW(0x66,rear?0x87e0:0xe0e4);SW(0x1e,W(0x66));place(a,e,0,0);
        if (!(b=grand(a,e))) return GA_UNSUPPORTED;
        START((ga_b(b,0xa0)&8u)?((ga_b(b,0xa0)&0x10u)?(rear?0x247598:0x247034):(rear?0x2470f6:0x246b4c)):(rear?0x24741e:0x246eb2));
        if (rear) SET(0x15,2);
        SB(0x9e,0);SL(0,rear?0x4e450:0x4e376);
    }
    if (!(b=grand(a,e))) return GA_UNSUPPORTED;
    if (ga_b(b,0x60)&bit) SB(0x9e,0xff);
    ga_project(a,e);STEP(f);
    if (f && (ga_b(b,0x60)&bit)) {ga_sb(b,0x60,(uint8_t)(ga_b(b,0x60)&~bit));return GA_DELETE;}   /* 004188 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t dust_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;
    if (L(0)==0x4ad12 || L(0)==0x4ad94) {
        bool rear=L(0)==0x4ad94;
        SB(0x1d,0);SW(0x66,rear?0x87d0:0xe0e4);SW(0x1e,W(0x66));ga_palette(a,0xf0);START(0x246ad2);
        if (rear) SET(0x15,2);
        if (!(ga_rand(e)&1u)) SET(0x15,0);
        uint32_t vx,vy;ga_polar(0xa0000,(uint8_t)((ga_rand(e)&0x3fu)+0x60u),&vx,&vy);   /* 002E7C */
        SL(0x30,vx);SL(0x34,vy);SB(0x12,0xf);SB(0x13,0xf);SW(0x26,0x200);SL(0,0x4ae1c);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    if (B(0x12)!=0xffu) {
        SB(0x12,B(0x12)+0x30u);
        unsigned s=B(0x13)+0x30u;SB(0x13,s);
        if (s>0xffu) {SB(0x12,0xff);SB(0x13,0xff);}
    }
    int32_t d0=-(int32_t)L(0x34);d0>>=4;SL(0x34,(int32_t)L(0x34)+d0);
    ga_move(a,e);STEP(f);
    if (f) return GA_DELETE;
    if (ga_cull(a,e,0xe0e72)) return GA_DELETE;                        /* 04E07A */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static bool in(uint32_t r,const uint32_t *s,unsigned n) {for (unsigned i=0;i<n;i++) if (s[i]==r) return true;return false;}
static const uint32_t BODY[]={0x4a2c2,0x4a380,0x4a3f0,0x4a488,0x4a4c2,0x4a57c,0x4a5de,0x4a618,0x4a668,0x4a74a,0x4a7cc,0x4a89e,0x4a8ca,0x4a94c,0x4a974,0x4a9d4};
static const uint32_t LEGS[]={0x4af02,0x4af72,0x4afaa,0x4afce,0x4b01e,0x4b034,0x4b0e6,0x4b15c,0x4b194,0x4b1b8,0x4b208,0x4b21e};
static const uint32_t TORSO[]={0x4b252,0x4b2f6,0x4b32e,0x4b352};
static const uint32_t ARM[]={0x4b3bc,0x4b40c,0x4b426,0x4b52c};
static const uint32_t POD[]={0x4b920,0x4b97e,0x4b99a,0x4ba16};
static const uint32_t HATCH[]={0x4c38a,0x4c3ea,0x4c412,0x4c4d2};
static const uint32_t MORTAR[]={0x4cf2c,0x4cf56,0x4cf5c,0x4cf92};
#define IN(r,s) in((r),(s),sizeof(s)/sizeof((s)[0]))
bool neo_m3boss_routine(uint32_t r) {
    return IN(r,BODY) || IN(r,LEGS) || IN(r,TORSO) || IN(r,ARM) || IN(r,POD) || IN(r,HATCH) || IN(r,MORTAR) ||
        r==0x4c69e || r==0x4c6c2 || r==0x4e2ea || r==0x4e376 || r==0x4e3be || r==0x4e450 || r==0x4ad12 || r==0x4ad94 || r==0x4ae1c;
}
ga_result_t neo_m3boss_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (IN(r,BODY)) return body_tick(a,e);
    if (IN(r,LEGS)) return leg_tick(a,e);
    if (IN(r,TORSO)) return torso_tick(a,e);
    if (IN(r,ARM)) return arm_tick(a,e);
    if (IN(r,POD)) return pod_tick(a,e);
    if (IN(r,HATCH)) return hatch_tick(a,e);
    if (IN(r,MORTAR)) return mortar_tick(a,e);
    if (r==0x4c69e || r==0x4c6c2) return floor_tick(a,e);
    if (r>=0x4e2ea && r<=0x4e450) return foot_tick(a,e);
    if (r==0x4ad12 || r==0x4ad94 || r==0x4ae1c) return dust_tick(a,e);
    STOP(r);
}
