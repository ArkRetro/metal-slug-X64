#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
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
 * Piece 2, the flamethrower 04B52C (orders 0/1; fixtures m3boss1, m3boss3):
 *   aim 04D798 (tables 04D91A/04D92A, 04D8C6) and turn 04D768, flame emitter
 *   04B79C (program 245BA0) and segments 04B87E (row E0F4A), fireball
 *   04CAB4 (03B36E parabola, 04E5D0/04E962 fixed point, 002F1A, trails
 *   04CD32, puddle 04CCE4).
 * Piece 3, the missile pod 04BA16 (order 2; m3boss3): open, aim 04D94A,
 *   volleys of 3 missiles 04C194 (04E154, points 04E19E), close.
 * Piece 4, the hatch 04C4D2 (order 7; m3boss2): shots 04C7B6 and 04C840
 *   (callbacks 04CE78/04CE98), its floor 04C69E (map E0E36).
 * Piece 5, the mortar 04CF92 (order 5; m3boss4): bombs 04D1DA (up, then
 *   thrown at a player; registered, one shot destroys them, 04D608).
 * Piece 6, the death 04A9D4..04ACDC (m3boss5): explosions 01C71E and lists
 *   01DA40 (E0C42..E0CAE, pieces 04E558/04E568), stand up, +9D 7F (legs
 *   04B01E/04B208 fall, torso and pieces deleted), the drag left 04AB10 and
 *   01DB74; 018F92 at the start and 018F40 out of view (piece 7: the
 *   mission end of gameplay.c, B24).
 * Order 6 of the pod (G15; the vehicle tables of 04DA38/04DC5C): aim 04BAB2
 *   to the player's side, open 04BB88, sweeps 04BCA2..04BD32 (patterns
 *   2C25E2/2C2602) with the missiles 04C220 (CALL 04E140 of 245DE6), then
 *   one volley of order 2 (04BF66). 04E1E6 (the pieces 04C2F6/04C30A) has
 *   no caller in the ROM.
 * The cartridge check before some deletions (10F00A/10F00C) is not
 * reproduced (as in neo_m3mid.c). */
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
/* ---- arithmetic of the arm and the pod ----------------------------------------- */
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static uint16_t absw(uint16_t v) {return S16(v)<0?(uint16_t)-v:v;}
/* 002F1A: |dx| + |dy| with the smaller halved (words). */
static uint16_t k2f1a(uint16_t d0,uint16_t d1) {
    d0=absw(d0);d1=absw(d1);
    if (d1>d0) return (uint16_t)((d0>>1)+d1);
    return (uint16_t)(d0+(d1>>1));
}
/* 04E57A: the unsigned 64-bit product of d0 and d1 (d1:d0). */
static uint64_t k4e57a(uint32_t d0,uint32_t d1) {return (uint64_t)d0*d1;}
/* 04E5D0: the signed 16.16 product. Both below 0x10000: mulu.w and >>16
 * (negated after: towards zero); else bits 16..47 of the 64-bit product
 * negated as a whole (04E5BE; TRAP #15 when its top bit is set). */
static uint32_t k4e5d0(neo_genv_t *e,uint32_t d0,uint32_t d1) {
    bool neg=false;
    if ((int32_t)d0<0) {d0=(uint32_t)-d0;neg=!neg;}
    if ((int32_t)d1<0) {d1=(uint32_t)-d1;neg=!neg;}
    if (d0<0x10000u && d1<0x10000u) {uint32_t r=(d0*d1)>>16;return neg?(uint32_t)-r:r;}
    uint64_t q=k4e57a(d0,d1);
    if (q>>63) {ga_fail(e,0x4e5c2);return 0;}
    if (neg) q=(uint64_t)-q;
    return (uint32_t)(q>>16);
}
/* 04E648: (d0<<16)/d1 by 48 restoring steps (d0 = 0: 0; d1 = 0: TRAP #15). */
static uint32_t k4e648(neo_genv_t *e,uint32_t d0,uint32_t d1) {
    if (!d0) return 0;
    if (!d1) {ga_fail(e,0x4e578);return 0;}
    uint32_t d2=d1,d3=0,q=d0<<16;uint16_t w=(uint16_t)(d0>>16);
    for (unsigned i=0;i<48;i++) {
        unsigned x=q>>31;q<<=1;
        unsigned x2=w>>15;w=(uint16_t)(w<<1|x);
        d3=d3<<1|x2;
        if (d3>=d2) {q=(q&0xffff0000u)|(uint16_t)(q+1u);d3-=d2;}
    }
    return q;
}
/* 04E962: the signed form of 04E648. */
static uint32_t k4e962(neo_genv_t *e,uint32_t d0,uint32_t d1) {
    bool neg=false;
    if ((int32_t)d0<0) {d0=(uint32_t)-d0;neg=!neg;}
    if ((int32_t)d1<0) {d1=(uint32_t)-d1;neg=!neg;}
    uint32_t r=k4e648(e,d0,d1);return neg?(uint32_t)-r:r;
}
/* 03B3D0: d7 / d0, signed, towards zero (two divu.w by the low word of |d0|). */
static int32_t k3b3d0(int32_t d7,int32_t d0) {
    bool neg=d0<0;uint32_t n=(uint32_t)d7,d=(uint32_t)d0;
    if (neg) d=(uint32_t)-d;
    if (d7<0) {n=(uint32_t)-n;neg=!neg;}
    if (d) {uint16_t dw=(uint16_t)d;uint32_t hi=(n>>16)/dw,r=(n>>16)%dw,lo=((r<<16)|(n&0xffffu))/dw;n=(hi<<16)|(lo&0xffffu);}
    return neg?-(int32_t)n:(int32_t)n;
}
/* 03B36E: the parabola to (dx, dy) at the speed d2 and the angle d3:
 * velocity (d0, d1) and accelerations (d2, d3). */
static void k3b36e(int16_t dx,int16_t dy,uint32_t speed,uint8_t angle,uint32_t out[4]) {
    uint32_t vx,vy;ga_polar(speed,angle,&vx,&vy);
    int32_t d0=dx?dx:1,d1=dy;
    int16_t x=(int16_t)(uint16_t)((int32_t)vx>>8),y=(int16_t)(uint16_t)((int32_t)vy>>8);
    int32_t d4=-(k3b3d0((int32_t)x*x,d0)>>1);
    int32_t d5=(k3b3d0((int32_t)x*(int16_t)d1,d0)>>1)-y;
    d5=(int32_t)(int16_t)(uint16_t)d5*x;
    out[0]=(uint32_t)(int32_t)x<<8;out[1]=(uint32_t)(int32_t)y<<8;out[2]=(uint32_t)d4;out[3]=(uint32_t)k3b3d0(d5,d0);
}
/* A table of (limit, value) words read until d0 <= limit (signed), or, when
 * stop, until the limit 0xFF (04D812, 04D892, 04D998). */
static uint16_t lookup(neo_genv_t *e,uint32_t t,int16_t d0,bool stop) {
    for (unsigned i=0;i<64;i++,t+=4) {
        uint16_t v=rw(e,t);
        if (d0<=S16(v) || (stop && v==0xffu)) return rw(e,t+2);
    }
    ga_fail(e,t);return 0;
}
static const uint8_t *target(neo_gactor_t *a,neo_genv_t *e) {return player(e,L(0x6e)==0x1014c4u?0u:1u);}
/* 0AAA88 for player p: the kind (+21) of its vehicle slot, 0 without one. */
static uint8_t vkind(neo_genv_t *e,unsigned p) {
    if (!e->vehicle || !e->vehicle->slots || !e->vehicle->exists[p] || !neo_vslot_node(&e->vehicle->slots->slot[p])) return 0;
    uint8_t k=e->vehicle->slots->slot[p].b[0x21];
    if (!k) ga_fail(e,0xaaacc);                                     /* TRAP #15 */
    return k;
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
    case 0x4ceb8: {                                                    /* 04CEB8: the fireball 04CAB4 */
        if (W(0x86)) return true;
        sound(e,0x1065);
        neo_gactor_t *b=child(a,e,0x4cab4,0x1c);
        if (!b) return !e->unsupported;
        ga_sw(b,0x8c,W(0x8c));ga_sb(b,0xa2,B(0xa2));ga_sl(b,0x6e,L(0x6e));ga_sw(b,0x72,W(0x72));ga_sw(b,0x74,W(0x74));
        uint32_t t=0x4cf18u+(uint32_t)(B(0x9a)>>1)*4u;
        ga_place_at(b,a,e,S16(rw(e,t)),S16(rw(e,t+2)));
        return !e->unsupported;
    }
    case 0x4e140: case 0x4e154: {                                      /* 04E140 / 04E154: one missile 04C220 / 04C194 */
        neo_gactor_t *m=child(a,e,routine==0x4e140?0x4c220:0x4c194,0x1c);
        if (m) {
            ga_sb(m,0x9a,B(0x9a));
            uint32_t t=0x4e19eu+(uint32_t)B(0x9a)*8u;
            ga_sb(m,0xa2,(uint8_t)rw(e,t+4));ga_place_at(m,a,e,S16(rw(e,t)),S16(rw(e,t+2)));
        }
        SB(0x9e,0xff);sound(e,0x1062);
        return !e->unsupported;
    }
    case 0x4ce78: case 0x4ce98: {                                      /* the hatch shots 04C7B6 / 04C840 */
        neo_gactor_t *s=child(a,e,routine==0x4ce78?0x4c7b6:0x4c840,0x1c);
        if (s) ga_place_at(s,a,e,routine==0x4ce78?-0x38:-0x34,-0x44);
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
/* ---- the death (piece 6) --------------------------------------------------------- */
/* 01DA40: the explosion list emitter 01D764 (+86 = list), at the node (01B220). */
static void k1da40(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764,0x1c);
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
/* 018F92: mission complete (as neo_m2boss.c: kill-all and flash requests,
 * 0C09D8, RAM 10C300 = FF, 10C304 = +5E, handlers 0C0538). */
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
static void shake(neo_genv_t *e) {if (e->shake) e->shake(e->ctx);}  /* 0C801A */
/* 04A9D4: sound 1026, 018F92, the floor off (013F1E), +61 = FF, the
 * explosion 01C71E at (0, 0x96), lists E0C9C, E0C42/E0C54/E0C66 over 0x80
 * updates; it stands up if crouched (+9D 3), then +9D = 7F (the legs fall,
 * the torso goes) and it drags itself left (04AB10: 04AD0C, 2 px per update,
 * 16-update pauses, lists E0C78/E0CAE, 1032) shaking (+7A), and accelerates
 * away: at x <= 0x10 the list E0C8A, the debris 01DB74 and 1033; out of view
 * (01945C E0E82) 018F40 and the deletion (after the cartridge check). */
static ga_result_t body_death(neo_gactor_t *a,neo_genv_t *e) {
    bool f;
    switch (L(0)) {
    case 0x4a9d4: {
        sound(e,0x1026);k18f92(a,e);
        if (e->dyn_release) e->dyn_release(e->ctx,a);                  /* 013F1E */
        SB(0x61,0xff);SW(0x88,0x80);
        neo_gactor_t *x=child(a,e,0x1c71e,0x2a);
        if (x) ga_place_at(x,a,e,0,0x96);
        k1da40(a,e,0xe0c9c);
        SL(0x50,0);SET(0x60,3);SET(0x60,5);SL(0,0x4aa3a);
    }
        /* fall through */
    case 0x4aa3a:
        ga_project(a,e);STEP(f);
        if (!W(0x88)) SL(0,0x4aaae);
        else {
            SW(0x88,W(0x88)-1u);
            if (W(0x88)==0x40) k1da40(a,e,0xe0c42);
            if (W(0x88)==0x28) k1da40(a,e,0xe0c54);
            if (W(0x88)==0x10) k1da40(a,e,0xe0c66);
        }
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);                    /* 004C24 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4aaae:
        SB(0x9e,0);
        if (B(0xa1)) {SB(0x9e,0xff);SB(0x9d,3);SB(0xa1,0);SW(0x88,0);}
        else SW(0x88,0x10);
        SL(0,0x4aade);
        /* fall through */
    case 0x4aade:
        ga_project(a,e);STEP(f);
        if (!B(0x9e)) {
            if (!W(0x88)) {SB(0x9d,0x7f);SL(0,0x4ab10);}
            else SW(0x88,W(0x88)-1u);
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4ab10:
        SW(0x86,0);CLR(0x60,7);SL(0x7a,0x20000);
        /* fall through */
    case 0x4ab22: {
        k1da40(a,e,0xe0c78);k1da40(a,e,0xe0cae);shake(e);sound(e,0x1032);
        uint16_t d0=rw(e,0x4ad0cu+(uint32_t)(uint16_t)(W(0x86)*2u));
        if (d0==0xffffu) SW(0x8c,0x7fff);
        else SW(0x8c,W(0x3c)-d0);
        SW(0x86,W(0x86)+1u);SL(0x30,0);SL(0,0x4ab82);
    }
        /* fall through */
    case 0x4ab82:
        if (!BIT(0x60,7) && S16(W(0x3c))<=0x10) {
            SET(0x60,7);k1da40(a,e,0xe0c8a);
            uint32_t x=L(0x3c),y=L(0x40);
            SL(0x3c,x+0x200000u);SL(0x40,y+0x800000u);
            for (unsigned i=0;i<16;i++) {                              /* 01DB74 */
                neo_gactor_t *d=child(a,e,0x1d3d2,0x2a);
                if (d) ga_place_from(d,a,e,0,8);
            }
            SL(0x40,y);SL(0x3c,x);
            shake(e);sound(e,0x1033);
        }
        if (W(0x8c)==0x7fff) SL(0x30,L(0x30)-0x2000u);
        else {SL(0x30,0xfffe0000u);if (S16(W(0x8c))>=S16(W(0x3c))) SL(0,0x4accc);}
        SL(0x7a,-(int32_t)L(0x7a));SL(0x34,L(0x7a));
        ga_move(a,e);STEP(f);
        if (ga_cull(a,e,0xe0e82)) {                                    /* 01945C */
            if (e->flags) e->flags->stage_done=1;                      /* 018F40 */
            return GA_DELETE;                                          /* the cartridge check, 004188 */
        }
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4accc:
        SW(0x84,0x10);SL(0x30,0);SL(0,0x4acdc);
        /* fall through */
    case 0x4acdc:
        SL(0x7a,-(int32_t)L(0x7a));SL(0x34,L(0x7a));
        ga_move(a,e);STEP(f);
        if (!W(0x84)) SL(0,0x4ab22);
        else SW(0x84,W(0x84)-1u);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
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
    return body_death(a,e);                                            /* 04A9D4: piece 6 */
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
static bool k4d9e4(neo_gactor_t *a,neo_genv_t *e,bool *fin);
static bool k4d9de(neo_gactor_t *a,neo_genv_t *e,bool *fin) {flash_share(a,e);return k4d9e4(a,e,fin);}
static bool k4d9e4(neo_gactor_t *a,neo_genv_t *e,bool *fin) {
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
    case 0x4b01e: case 0x4b208: START(second?0x2459dc:0x2459d0);SL(0,second?0x4b21e:0x4b034);
        /* fall through */
    case 0x4b034: case 0x4b21e:                                        /* the legs fall (body +9D 7F) */
        ga_project(a,e);flash_share(a,e);STEP(f);
        if (second) {if (!(p=creator(a,e))) return GA_UNSUPPORTED;ga_sw(p,0x98,W(0x98));}
        if (ga_cull(a,e,0xe0e82)) return second?GA_DELETE_TREE:GA_DELETE;   /* 01945C; 0041AC / the cartridge check, 004188 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    default: STOP(L(0));
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
/* ---- the flamethrower arm (orders 0/1) --------------------------------------------- */
/* 04D768: +9A one step towards +9C; carry (true) when there. */
static bool k4d768(neo_gactor_t *a) {
    int8_t d0=(int8_t)B(0x9a),t=(int8_t)B(0x9c);
    if (d0==t) return true;
    SB(0x9a,d0>=t?d0-1:d0+1);
    return false;
}
/* 04D718: the body's phase into +A4; carry in phase 2. */
static bool k4d718(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *g=grand(a,e);uint8_t v=g&&ga_b(g,0xa4)?1u:0u;SB(0xa4,v);return v!=0;}
/* The pose point E0F9A[+98] of the torso (the creator), into +98. */
static void pose_point(neo_gactor_t *a,neo_genv_t *e,int16_t *x,int16_t *y) {
    neo_gactor_t *c=creator(a,e);uint16_t pose=c?ga_w(c,0x98):0;SW(0x98,pose);
    uint32_t t=(uint32_t)(0xe0f9a+(int16_t)(uint16_t)(pose<<2));
    *x=S16(W(0x3c)+rw(e,t));*y=S16(W(0x40)+rw(e,t+2));
}
/* 04D798: the arm's aim: the landing point +72/+74 of the fireball (order
 * 0: a random distance 0x10..0xAF, else the player's), its angle +A2, the
 * target angle +9C (table 04D8C6) and the distance +8C. */
static void k4d798(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x,y;pose_point(a,e,&x,&y);
    uint16_t d0;
    if (!B(0x9d)) {uint16_t d1=(uint16_t)(ga_rand_range(e,0xa0)+0x10u);d0=(uint16_t)(x-d1);}
    else {const uint8_t *n=target(a,e);if (!n) return;d0=(uint16_t)(x-(n[0x3c]<<8|n[0x3d]));}
    d0=absw(d0);
    uint32_t t=k4d718(a,e)?0x4d92au:0x4d91au;
    SW(0x72,(uint16_t)(x-lookup(e,t,S16(d0),true)));SW(0x74,0x120);
    uint8_t an=(uint8_t)-neo_angle(S16(W(0x72)-(uint16_t)x),S16(0x120-(uint16_t)y));
    SB(0xa2,an);SB(0x9c,(uint8_t)lookup(e,0x4d8c6,an,false));
    SW(0x8c,absw((uint16_t)(x-W(0x72))));
}
static ga_result_t arm_attack(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *g;
    switch (L(0)) {
    case 0x4b52c:
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        if (!ga_b(g,0xa4)) {SB(0xa4,0);SW(0x80,rank(a,e,0x2c21ae));SW(0x82,rank(a,e,0x2c21e2));}
        else {SB(0xa4,1);SW(0x80,rank(a,e,0x2c21c8));SW(0x82,rank(a,e,0x2c2244));}
        /* fall through */
    case 0x4b58e:
        if (!W(0x82)) goto aim_back;
        START(0x2459f4);SW(0x88,3);SW(0x84,W(0x80));k4d798(a,e);SL(0,0x4b5bc);
        /* fall through */
    case 0x4b5bc:
        ga_project(a,e);
        if (!W(0x88)) {
            SW(0x88,3);
            if (k4d768(a)) {if (W(0x84)) k4d798(a,e);else SL(0,0x4b658);}
        } else SW(0x88,W(0x88)-1u);
        if (W(0x84)) SW(0x84,W(0x84)-1u);
        if (!(g=creator(a,e))) return GA_UNSUPPORTED;
        SW(0x98,ga_w(g,0x98));
        START(S16(ga_w(g,0x98))>=0x25?0x2459e8:0x2459f4);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) {SB(0x61,0xff);SL(0,0x4b732);}
        break;
    case 0x4b658:
        SB(0x60,0);SB(0x61,0);
        if (!child(a,e,0x4b79c,0xe)) SB(0x60,0xff);                    /* 0040BC without room */
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x4b680);
        /* fall through */
    case 0x4b680:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (B(0x60)) SL(0,0x4b6ae);
        if (k4d74a(a,e)) {SB(0x61,0xff);SL(0,0x4b732);}
        break;
    case 0x4b6ae:
        START(0x245a48);SW(0x86,0);SL(0,0x4b6c8);
        /* fall through */
    case 0x4b6c8:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;                    /* CALL 04CEB8 */
        if (f) {
            SW(0x86,W(0x86)+1u);
            if (S16(W(0x86))>3) {
                unsigned p;SET(0x61,0);
                if (!k1b04a(e,&p)) SL(0,0x4b732);
                else {SW(0x82,W(0x82)-1u);SL(0,0x4b58e);}
            } else START(0x245a48);
        }
        if (k4d74a(a,e)) {SB(0x61,0xff);SL(0,0x4b732);}
        break;
    case 0x4b732:
    aim_back:
        SW(0x88,4);SB(0x9c,0);START(0x2459f4);SL(0,0x4b752);
        /* fall through */
    case 0x4b752:
        ga_project(a,e);
        if (!W(0x88)) {
            SW(0x88,4);
            if (k4d768(a)) {if (!(g=grand(a,e))) return GA_UNSUPPORTED;ga_sb(g,0x9e,0);SL(0,0x4b40c);}
        } else SW(0x88,W(0x88)-1u);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) SB(0x61,0xff);
        break;
    default: STOP(L(0));
    }
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04B79C: the flame emitter (program 245BA0 sets +60 and +9F). */
static ga_result_t emitter_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *c;
    if (L(0)==0x4b79c) {
        SB(0x1d,0);ga_palette(a,0xbb);SW(0x66,0xe06f);SW(0x1e,0xe06f);SB(0x61,0);SB(0x60,0);SB(0x9f,0);
        place(a,e,0,0);START(0x245ba0);SL(0,0x4b7ec);
    }
    if (!(c=creator(a,e))) return GA_UNSUPPORTED;
    SB(0x9a,ga_b(c,0x9a));
    ga_project(a,e);
    if (!k4d9e4(a,e,&f)) return GA_UNSUPPORTED;
    if (!B(0x61) && B(0x60)) {ga_sb(c,0x60,0xff);SB(0x61,0xff);}
    if (B(0x9f)) {
        neo_gactor_t *s=child(a,e,0x4b87e,0x1c);
        uint16_t pose=ga_w(c,0x98);SW(0x98,pose);
        uint32_t t=(uint32_t)(0xe0f9a+(int16_t)(uint16_t)(pose<<2));
        int16_t dx=S16(rw(e,t)-0x10u),dy=S16(rw(e,t+2)-2u*B(0x9a));
        if (s) ga_place_at(s,a,e,dx,dy);
    }
    if (ga_b(c,0x61)&1u) return GA_DELETE;                             /* 004188 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04B87E: one flame segment, one update (the cartridge check, then 004188). */
static ga_result_t segment_tick(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);ga_attack(a,e,0xe0f4a);
    return e->unsupported?GA_UNSUPPORTED:GA_DELETE;
}
/* 04CAB4: the fireball: a parabola 03B36E to (+72, +74), a trail 04CD32
 * every 0x1200 of path, the explosion 01C706 on a hit (or when the arm's +61
 * bit 1 is set) and the puddle 04CCE4 on landing. */
static ga_result_t ball_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;neo_gactor_t *c;
    switch (L(0)) {
    case 0x4cab4: {
        SB(0x1d,0);ga_palette(a,0xbd);START(0x247656);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SET(0x15,5);
        uint32_t d0=k4e962(e,(uint32_t)k2f1a((uint16_t)(W(0x72)-W(0x3c)),(uint16_t)(W(0x74)-W(0x40)))<<16,0x1000000u);
        if (!(c=creator(a,e))) return GA_UNSUPPORTED;
        uint16_t v=rank(a,e,ga_b(c,0xa4)?0x2c214cu:0x2c20eau);
        uint32_t d2=k4e5d0(e,d0,(uint32_t)(int32_t)S16(v)<<8),o[4];
        k3b36e(S16(W(0x72)-W(0x3c)),S16(0x120u-W(0x40)),d2,B(0xa2),o);
        SL(0x30,o[0]);SL(0x34,o[1]);SL(0x76,o[2]);SL(0x7a,o[3]);SW(0x96,0);SW(0x84,0);SL(0,0x4cb74);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4cb74:
        if ((int32_t)L(0x30)>=0) {SL(0x30,0);SL(0x76,0);SL(0,0x4cb96);}
        else SL(0x30,L(0x30)+L(0x76));
        /* fall through */
    case 0x4cb96: break;
    default: STOP(L(0));
    }
    SW(0x88,W(0x1e));
    SL(0x34,L(0x34)+L(0x7a));
    SW(0x96,W(0x96)+k2f1a((uint16_t)((int32_t)L(0x30)>>8),(uint16_t)((int32_t)L(0x34)>>8)));
    ga_air(a,e);
    if (!step(a,e,call,&f)) return GA_UNSUPPORTED;
    if (S16(W(0x96))>=0x1200) {
        SW(0x96,W(0x96)-0x1200u);
        neo_gactor_t *t=child(a,e,0x4cd32,0x2a);
        if (t) ga_place_at(t,a,e,0,0);
    }
    if (!W(0x86)) {if (ga_attack(a,e,0xe0f5a)) {SL(0,0x1c706);sound(e,0x1021);}}
    else SW(0x86,W(0x86)-1u);
    if (BIT(0x15,6)) SL(0,0x4cce4);
    if (!(c=creator(a,e))) return GA_UNSUPPORTED;
    if (ga_b(c,0x61)&2u) SL(0,0x1c706);
    if (ga_cull(a,e,0xe0e72)) return GA_DELETE;                        /* 04E092, the cartridge check, 004188 */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04CCE4: the puddle (program 24855C, sound 1025). */
static ga_result_t puddle_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;
    if (L(0)==0x4cce4) {SW(0x66,0x1001);SW(0x1e,0x1001);SB(0x1d,0);ga_palette(a,0x113);START(0x24855c);SL(0,0x4cd1a);}
    ga_project(a,e);
    if (!step(a,e,call,&f)) return GA_UNSUPPORTED;
    if (f) return GA_DELETE;
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04CD32: a trail piece; its depth counts down in the ball's +88. */
static ga_result_t trail_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;neo_gactor_t *c;
    if (L(0)==0x4cd32) {SB(0x1d,0);ga_palette(a,0xbd);START(0x24767a);SL(0x30,0);SL(0x34,0);SL(0,0x4cd64);}
    if (!(c=creator(a,e))) return GA_UNSUPPORTED;
    ga_sw(c,0x88,(uint16_t)(ga_w(c,0x88)-1u));SW(0x1e,ga_w(c,0x88));
    ga_project(a,e);
    if (!step(a,e,call,&f)) return GA_UNSUPPORTED;
    if (f) return GA_DELETE;                                           /* the cartridge check, 004188 */
    if (ga_cull(a,e,0xe0e72)) return GA_DELETE;                        /* 04E07A */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* ---- the missile pod (order 2) ----------------------------------------------------- */
/* 04D94A: the pod's target angle +9C (table 04D9BA) to the player (+18 lower). */
static void k4d94a(neo_gactor_t *a,neo_genv_t *e) {
    int16_t x,y;pose_point(a,e,&x,&y);
    const uint8_t *n=target(a,e);if (!n) return;
    uint8_t an=neo_angle(S16((uint16_t)(n[0x3c]<<8|n[0x3d])-(uint16_t)x),S16((uint16_t)((n[0x40]<<8|n[0x41])+0x18u)-(uint16_t)y));
    SB(0x9c,(uint8_t)lookup(e,0x4d9ba,an,false));
}
static uint16_t open_speed(neo_gactor_t *a,neo_genv_t *e) {neo_gactor_t *g=grand(a,e);return g && ga_b(g,0xa4)==1?0x400:0x200;}
static ga_result_t pod_attack(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *g;
    switch (L(0)) {
    case 0x4ba16:
        START(0x2460d8);sound(e,0x1075);SB(0x9e,0);SW(0x26,open_speed(a,e));SL(0,0x4ba5c);
        /* fall through */
    case 0x4ba5c:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (f) SL(0,B(0x9d)==6?0x4ba94:0x4beec);
        if (k4d74a(a,e)) SB(0x61,0xff);
        break;
    case 0x4beec:
        START(0x245efc);SB(0x14,B(0x1a));SW(0x26,1);SW(0x88,2);SL(0,0x4bf14);
        /* fall through */
    case 0x4bf14:
        ga_project(a,e);
        if (!W(0x88)) {
            if (W(0x26)==0x100) {sound(e,0x10a1);SL(0,0x4bf80);}
            else {SW(0x88,2);SW(0x26,W(0x26)<<1);}
        } else SW(0x88,W(0x88)-1u);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) SB(0x61,0xff);
        break;
    case 0x4bf80: {
        SW(0x88,4);
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        bool two=ga_b(g,0xa4)!=0;
        SB(0xa4,two?1:0);SW(0x80,rank(a,e,two?0x2c23a4u:0x2c2372u));
        uint8_t k=vkind(e,L(0x6e)==0x1014c4u?0u:1u);
        SW(0x82,rank(a,e,k?(two?0x2c2518u:0x2c24b6u):(two?0x2c2594u:0x2c257au)));
        SW(0x86,0);
    }
        /* fall through */
    case 0x4c060:
    k4c060:
        START(0x245efc);SW(0x84,0);SL(0,0x4c07a);
        /* fall through */
    case 0x4c07a:
        ga_project(a,e);
        k4d94a(a,e);
        if (!W(0x88)) {k4d768(a);SW(0x88,4);}
        else SW(0x88,W(0x88)-1u);
        if (S16(W(0x84))>=S16(W(0x80))) SL(0,0x4c0d2);
        else SW(0x84,W(0x84)+1u);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) {SB(0x61,0xff);SL(0,0x4be04);}
        break;
    case 0x4c0d2:
        START(0x245df2);SW(0x84,2);SL(0,0x4c0ee);
        /* fall through */
    case 0x4c0ee:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;                    /* CALL 04E154 */
        if (f) {
            if (!W(0x84)) {
                SW(0x86,W(0x86)+1u);
                if (S16(W(0x86))>=S16(W(0x82))) SL(0,0x4be04);
                else {
                    if (!(g=grand(a,e))) return GA_UNSUPPORTED;
                    bool two=ga_b(g,0xa4)!=0;
                    SB(0xa4,two?1:0);SW(0x80,rank(a,e,two?0x2c2408u:0x2c23d6u));SL(0,0x4c060);
                }
            } else {START(0x245df2);SW(0x84,W(0x84)-1u);}
        }
        if (k4d74a(a,e)) {SB(0x61,0xff);SL(0,0x4be04);}
        break;
    case 0x4be04:
        START(0x245efc);SW(0x26,0x100);SW(0x88,0);SL(0,0x4be26);
        /* fall through */
    case 0x4be26:
        ga_project(a,e);
        if (!W(0x88)) {
            if (W(0x26)==1) {SB(0x9e,0xff);SL(0,0x4ba94);}
            else {SW(0x88,0);SW(0x26,W(0x26)>>1);}
        } else SW(0x88,W(0x88)-1u);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) SB(0x61,0xff);
        break;
    case 0x4ba94:
        SW(0x26,0x100);START(0x245d92);
        if (!B(0x9e)) {                                                /* 04BAB2 (order 6): 9 or 0 by the side of a player */
            unsigned p;SB(0x9a,2);
            int16_t x,y;pose_point(a,e,&x,&y);
            k1b04a(e,&p);
            const uint8_t *n=player(e,p);if (!n) return GA_UNSUPPORTED;
            uint16_t an=neo_angle(S16((uint16_t)(n[0x3c]<<8|n[0x3d])-(uint16_t)x),S16((uint16_t)(n[0x40]<<8|n[0x41])-(uint16_t)y))&0xffu;   /* 01B12E */
            SB(0x9c,S16(an)>0x80?0:8);
        } else SB(0x9c,2);
        SB(0x14,B(0x1a));SW(0x88,0);SL(0,0x4bb2c);
        /* fall through */
    case 0x4bb2c:
        ga_project(a,e);
        if (!W(0x88)) {
            SW(0x88,0);
            if (k4d768(a)) SL(0,B(0x9e)?0x4be74:0x4bb88);
        } else SW(0x88,W(0x88)-1u);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) {SB(0x61,0xff);SB(0x9e,0xff);SB(0x9c,2);}
        break;
    case 0x4bb88:                                                      /* order 6: open (245EFC, +26 from 1 to 0x100) */
        START(0x245efc);SW(0x26,1);SW(0x88,0);SL(0,0x4bbaa);
        /* fall through */
    case 0x4bbaa:
        ga_project(a,e);
        if (!W(0x88)) {
            if (W(0x26)==0x100) {sound(e,0x10a1);SL(0,0x4bbfc);}
            else {SW(0x88,0);SW(0x26,W(0x26)<<1);}
        } else SW(0x88,W(0x88)-1u);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) SB(0x61,0xff);
        break;
    case 0x4bbfc: {                                                    /* the wait +80 and the sweeps +82 */
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        bool two=ga_b(g,0xa4)!=0;
        SB(0xa4,two?1:0);SW(0x80,rank(a,e,two?0x2c2340u:0x2c230eu));SW(0x82,rank(a,e,two?0x2c2454u:0x2c243au));
        SW(0x84,0);SL(0,0x4bc68);
    }
        /* fall through */
    case 0x4bc68:
        ga_project(a,e);
        if (S16(W(0x84))>=S16(W(0x80))) SL(0,0x4bca2);
        else SW(0x84,W(0x84)+1u);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (k4d74a(a,e)) {SB(0x61,0xff);SL(0,0x4be04);}
        break;
    case 0x4bca2: {                                                    /* one of 8 sweep patterns (2C25E2 / 2C2602) */
        SW(0x84,0);
        uint16_t d0=(uint16_t)(ga_rand_range(e,8)<<2);
        SL(0x92,rl(e,(B(0xa4)?0x2c2602u:0x2c25e2u)+d0));
    }
        /* fall through */
    case 0x4bcd2:
        SB(0x9b,rb(e,L(0x92)+(uint32_t)(uint16_t)(W(0x84)*2u)));
        START(B(0x9b)==B(0x9a)?0x245fec:0x245de6);                     /* 245DE6: CALL 04E140 */
        SB(0x9c,B(0x9c)?0:8);SB(0x9f,0);SL(0,0x4bd32);
        /* fall through */
    case 0x4bd32:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (f) {
            SB(0x9f,B(0x9f)^1u);
            if (k4d768(a)) {
                if (!BIT(0x9f,1)) {SET(0x9f,1);SB(0x9c,B(0x9c)?0:8);SB(0x9b,rb(e,L(0x92)+(uint32_t)(uint16_t)(W(0x84)*2u)+1u));}
                else {SW(0x84,W(0x84)+1u);SL(0,S16(W(0x84))>=S16(W(0x82))?0x4bf66:0x4bcd2);}
            }
            START(B(0x9b)==B(0x9a)?0x245fec:0x245de6);
        }
        if (k4d74a(a,e)) {SB(0x61,0xff);SL(0,0x4be04);}
        break;
    case 0x4bf66:                                                      /* then one volley of order 2 */
        SW(0x88,4);SW(0x80,0x10);SW(0x82,1);SW(0x86,0);
        goto k4c060;
    case 0x4be74:
        START(0x2461a2);SB(0x14,B(0x19));sound(e,0x1075);SW(0x26,open_speed(a,e));SL(0,0x4bebc);
        /* fall through */
    case 0x4bebc:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (f) {if (!(g=grand(a,e))) return GA_UNSUPPORTED;ga_sb(g,0x9e,0);SL(0,0x4b97e);}
        if (k4d74a(a,e)) SB(0x61,0xff);
        break;
    default: STOP(L(0));
    }
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04C194: a missile in a straight line (speed 2C22DA/2C22F4, angle +A2 less
 * 4 per missile left); 01C6EE on a hit or when the pod's +61 is set. */
static ga_result_t missile_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;neo_gactor_t *c;
    if (!(c=creator(a,e))) return GA_UNSUPPORTED;
    switch (L(0)) {
    case 0x4c194: {
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x1d,0);ga_palette(a,0xbb);START(0x24683a);
        uint16_t v=rank(a,e,ga_b(c,0xa4)?0x2c22f4u:0x2c22dau);
        uint8_t an=(uint8_t)(B(0xa2)-(uint8_t)((ga_w(c,0x84)-1u)<<2));
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)S16(v)<<8,an,&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0,0x4c298);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4c220: {                                                    /* order 6: palette 202, program 246794 */
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x1d,0);ga_palette(a,0x202);START(0x246794);
        uint16_t v=rank(a,e,ga_b(c,0xa4)?0x2c22c0u:0x2c22a6u);
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)S16(v)<<8,B(0xa2),&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0,0x4c298);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4c298: break;
    default: STOP(L(0));
    }
    ga_move(a,e);
    if (!step(a,e,call,&f)) return GA_UNSUPPORTED;
    if (ga_attack(a,e,0xe0f8a)) {if (e->kill_children) e->kill_children(e->ctx,a);SL(0,0x1c6ee);sound(e,0x109f);}   /* 004192 */
    if (ga_b(c,0x61)) {if (e->kill_children) e->kill_children(e->ctx,a);SL(0,0x1c6ee);}
    if (ga_cull(a,e,0xe0e72)) return GA_DELETE_TREE;                   /* 04E092, 0041AC */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t arm_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *p,*g;
    switch (L(0)) {
    case 0x4b3bc: piece_init(a,e,0,0x9110,-0xe,0x33,3);SL(0,0x4b40c);return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4b40c: SB(0x9a,0);START(0x2459f4);SL(0,0x4b426);
        /* fall through */
    case 0x4b426: break;
    default: return arm_attack(a,e);                                   /* 04B52C: the flamethrower (piece 2) */
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
    default: return pod_attack(a,e);                                   /* 04BA16: the missiles (piece 3) */
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
/* ---- the hatch (order 7) ------------------------------------------------------------ */
/* 04C4D2: open (246262), +84 = 2C25AE/2C25C8 shots 04C7B6 (program 2463EA,
 * then 2463EE; callback 04CE78), 16 shots 04C840 (246480/246484, 04CE98),
 * close (246326); +A3 to the body every update; the stop sound 10EF
 * (012192) when the pieces stop. */
static ga_result_t hatch_attack(neo_gactor_t *a,neo_genv_t *e) {
    bool f=false;neo_gactor_t *g;
    switch (L(0)) {
    case 0x4c4d2: START(0x246262);SL(0,0x4c4e8);
        /* fall through */
    case 0x4c4e8:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (f) SL(0,0x4c51a);
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        ga_sb(g,0xa3,B(0xa3));
        if (k4d74a(a,e)) SB(0x61,0xff);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4c51a:
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        SW(0x84,rank(a,e,ga_b(g,0xa4)?0x2c25c8u:0x2c25aeu));START(0x2463ea);SL(0,0x4c566);
        /* fall through */
    case 0x4c566: case 0x4c5e6: {
        bool second=L(0)==0x4c5e6;
        SW(0x86,0xe06f);ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (f) {
            SW(0x84,W(0x84)-1u);
            if (!W(0x84)) SL(0,second?0x4c64a:0x4c5ca);
            else START(second?0x246484:0x2463ee);
        }
        if (k4d74a(a,e)) {sound(e,0x10ef|0x8000u);SL(0,0x4c64a);SB(0x61,0xff);}   /* 012192 */
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        ga_sb(g,0xa3,B(0xa3));
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4c5ca: START(0x246480);SW(0x84,0x10);SL(0,0x4c5e6);
        return hatch_attack(a,e);
    case 0x4c64a: START(0x246326);SL(0,0x4c660);
        /* fall through */
    case 0x4c660:
        ga_project(a,e);
        if (!k4d9de(a,e,&f)) return GA_UNSUPPORTED;
        if (f) {if (!(g=grand(a,e))) return GA_UNSUPPORTED;ga_sb(g,0x9e,0);SL(0,0x4c3ea);}
        if (!(g=grand(a,e))) return GA_UNSUPPORTED;
        ga_sb(g,0xa3,B(0xa3));
        if (k4d74a(a,e)) SB(0x61,0xff);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
}
/* 04C7B6 / 04C840: a hatch shot (programs 246694 / 2465DA, speed -0xE000 /
 * -0xA0000 per update, rows E0F6A / E0F5A unless +9F); its depth counts down
 * in the hatch's +86; deleted at the end of its program (after the cartridge
 * check) or out of view (04E07A). */
static ga_result_t hatch_shot_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;neo_gactor_t *c;bool big=L(0)>=0x4c840u;
    if (L(0)==0x4c7b6 || L(0)==0x4c840) {
        SB(0x1d,0);ga_palette(a,0xbd);SW(0x66,0xe06f);SW(0x1e,0xe06f);START(big?0x2465da:0x246694);
        SL(0x30,big?0xfff60000u:0xffff2000u);SL(0x34,0);SB(0x9f,0);SL(0,big?0x4c88c:0x4c802);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    if (!(c=creator(a,e))) return GA_UNSUPPORTED;
    SW(0x1e,ga_w(c,0x86));ga_sw(c,0x86,(uint16_t)(ga_w(c,0x86)-1u));
    ga_move(a,e);
    if (!step(a,e,call,&f)) return GA_UNSUPPORTED;
    if (f) return GA_DELETE;
    if (!B(0x9f)) ga_attack(a,e,big?0xe0f5a:0xe0f6a);
    if (ga_cull(a,e,0xe0e72)) return GA_DELETE;                        /* 04E07A */
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
    default: return hatch_attack(a,e);                                 /* 04C4D2: order 7 (piece 4) */
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
    case 0x4cf92: {                                                    /* 04CF92: a volley (order 5) */
        neo_gactor_t *g=grand(a,e);if (!g) return GA_UNSUPPORTED;
        SB(0xa4,ga_b(g,0xa4));
        bool r=k4dd86(e),p2=B(0xa4)!=0;
        SW(0x88,rank(a,e,p2?(r?0x2c18e4u:0x2c1cdau):(r?0x2c18cau:0x2c1cc0u)));
        SW(0x82,rank(a,e,p2?(r?0x2c1930u:0x2c1d26u):(r?0x2c18feu:0x2c1cf4u)));
        int16_t d7=S16(rank(a,e,p2?(r?0x2c19f6u:0x2c1decu):(r?0x2c19c4u:0x2c1dbau)));
        uint16_t d6=0;
        do {
            if (S16(W(0x86))>=S16(W(0x82))) break;
            neo_gactor_t *b=child(a,e,0x4d1da,0x1c),*c;
            if (b) {
                if (!(c=creator(a,e))) return GA_UNSUPPORTED;
                uint16_t pose=ga_w(c,0x98);SW(0x98,pose);
                uint32_t t=(uint32_t)(0xe0f9a+(int16_t)(uint16_t)(pose<<2));
                ga_place_at(b,a,e,S16(rw(e,t)),S16(rw(e,t+2)));
                ga_sw(b,0x86,d6);SW(0x86,W(0x86)+1u);
            }
            d6++;
        } while (--d7>=0);
        SL(0,0x4d0e0);
    }
        /* fall through */
    case 0x4d0e0: {
        ga_project(a,e);
        if (!W(0x88)) {neo_gactor_t *g=grand(a,e);if (!g) return GA_UNSUPPORTED;ga_sb(g,0x9e,0);SL(0,0x4cf56);}
        else SW(0x88,W(0x88)-1u);
        if (!W(0x86)) SB(0x9f,0);
        if (k4d74a(a,e)) {SB(0x61,0xff);return GA_DELETE;}             /* the cartridge check, 004188 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    default: STOP(L(0));
    }
    ga_project(a,e);
    if (k4df02(a,e)) SL(0,0x4cf92);
    if (!W(0x86)) SB(0x9f,0);
    if (k4d74a(a,e)) {SB(0x61,0xff);return GA_DELETE;}
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
/* 04D608: a bomb's hit: row E0F7A, or the mortar's +61/+9F (every bomb
 * explodes), or its registration 004C24 (one shot, health 2C1D58/2C1962);
 * the explosion 01C6EE with sound 1022. */
static void k4d608(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=creator(a,e);if (!c) return;
    bool boom=ga_attack(a,e,0xe0f7a)!=0 || ga_b(c,0x61) || ga_b(c,0x9f);
    if (!boom) {
        if (!ga_register(a,e,NULL,NULL)) return;
        if (!BIT(0x5f,7)) {SB(0x5a,0);return;}
    } else ga_sb(c,0x9f,0xff);
    ga_sw(c,0x86,(uint16_t)(ga_w(c,0x86)-1u));sound(e,0x1022);SL(0,0x1c6ee);
}
static uint16_t bomb_rank(neo_gactor_t *a,neo_genv_t *e,uint32_t ride0,uint32_t walk0,uint32_t ride1,uint32_t walk1) {
    neo_gactor_t *c=creator(a,e);if (!c) return 0;
    bool r=k4dd86(e);
    return rank(a,e,ga_b(c,0xa4)?(r?ride1:walk1):(r?ride0:walk0));
}
/* 04D1DA: a mortar bomb: up with gravity (04D284) to y 0x1E0, then +84
 * throws at a player (01B04A, 01B12E at +40 + 0x10, speed +8E) with waits
 * +88 (2C1AF8.. tables); 04E092 out of view. */
static ga_result_t bomb_tick(neo_gactor_t *a,neo_genv_t *e) {
    bool f;neo_gactor_t *c;
    switch (L(0)) {
    case 0x4d1da: {
        SB(0x1d,0);ga_palette(a,0xbb);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);START(0x2464ba);
        SET(0x5e,7);                                                   /* 004C1C */
        uint32_t t=0x4d1b2u+(uint32_t)(uint16_t)(W(0x86)<<3);
        SL(0x30,rl(e,t));SL(0x34,0);SL(0x28,L(0x28)+rl(e,t+4));
        SW(0x4e,rank(a,e,k4dd86(e)?0x2c1962u:0x2c1d58u));
        SL(0x50,0xe0f3e);SET(0x5f,6);
        if (!W(0x86)) sound(e,0x109a);
        SL(0,0x4d284);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x4d284:
        SL(0x34,L(0x34)+0x4000u);SL(0x30,L(0x30)-(uint32_t)((int32_t)L(0x30)>>8));
        ga_move(a,e);
        if (!step(a,e,call,&f)) return GA_UNSUPPORTED;
        if (S16(W(0x40))>=0x1e0) SL(0,0x4d2ce);
        k4d608(a,e);
        if (ga_cull(a,e,0xe0e72)) {if (!(c=creator(a,e))) return GA_UNSUPPORTED;ga_sw(c,0x86,(uint16_t)(ga_w(c,0x86)-1u));return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x4d2ce:
        SW(0x88,bomb_rank(a,e,0x2c1af8,0x2c1eee,0x2c1b6a,0x2c1f60));
        SW(0x84,bomb_rank(a,e,0x2c1a28,0x2c1e1e,0x2c1a42,0x2c1e38));
        SL(0x30,0);SL(0x34,0);SB(0x60,0);CLR(0x5f,6);SL(0,0x4d390);
        /* fall through */
    case 0x4d390:
        ga_move(a,e);
        if (!W(0x84)) {}
        else if (W(0x88)) {
            SW(0x88,W(0x88)-1u);
            if (B(0x60) && (L(0x30) || L(0x34)) &&
                (S16(W(0x3c))<=0x10 || S16(W(0x3c))>=0x120 || S16(W(0x40))<=0x138 || S16(W(0x40))>=0x1e8)) SW(0x88,0xffff);
        } else {
            SB(0x60,(B(0x60)+1u)&1u);
            if (!B(0x60)) {SW(0x88,bomb_rank(a,e,0x2c1af8,0x2c1eee,0x2c1b6a,0x2c1f60));SL(0x30,0);SL(0x34,0);}
            else {
                SW(0x88,bomb_rank(a,e,0x2c1bdc,0x2c1fd2,0x2c1c4e,0x2c2044));
                SW(0x8e,bomb_rank(a,e,0x2c1a5c,0x2c1e52,0x2c1aaa,0x2c1ea0));
                unsigned p;if (!k1b04a(e,&p)) {ga_fail(e,0x4d4d4);return GA_UNSUPPORTED;}
                const uint8_t *n=player(e,p);if (!n) return GA_UNSUPPORTED;
                uint16_t px=(uint16_t)(n[0x3c]<<8|n[0x3d]),py=(uint16_t)((n[0x40]<<8|n[0x41])+0x10u);
                uint8_t an=neo_angle(S16(px-W(0x3c)),S16(py-W(0x40)));      /* 01B12E */
                uint32_t vx,vy;ga_polar((uint32_t)((int32_t)S16(W(0x8e))<<8),an,&vx,&vy);   /* 002E7C */
                SL(0x30,vx);SL(0x34,vy);SW(0x84,W(0x84)-1u);
            }
        }
        if (!step(a,e,call,&f)) return GA_UNSUPPORTED;
        k4d608(a,e);
        if (ga_cull(a,e,0xe0e72)) {if (!(c=creator(a,e))) return GA_UNSUPPORTED;ga_sw(c,0x86,(uint16_t)(ga_w(c,0x86)-1u));return GA_DELETE;}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    STOP(L(0));
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
static const uint32_t BODY[]={0x4a2c2,0x4a380,0x4a3f0,0x4a488,0x4a4c2,0x4a57c,0x4a5de,0x4a618,0x4a668,0x4a74a,0x4a7cc,0x4a89e,0x4a8ca,0x4a94c,0x4a974,0x4a9d4,
    0x4aa3a,0x4aaae,0x4aade,0x4ab10,0x4ab22,0x4ab82,0x4accc,0x4acdc};
static const uint32_t LEGS[]={0x4af02,0x4af72,0x4afaa,0x4afce,0x4b01e,0x4b034,0x4b0e6,0x4b15c,0x4b194,0x4b1b8,0x4b208,0x4b21e};
static const uint32_t TORSO[]={0x4b252,0x4b2f6,0x4b32e,0x4b352};
static const uint32_t ARM[]={0x4b3bc,0x4b40c,0x4b426,0x4b52c,0x4b58e,0x4b5bc,0x4b658,0x4b680,0x4b6ae,0x4b6c8,0x4b732,0x4b752};
static const uint32_t POD[]={0x4b920,0x4b97e,0x4b99a,0x4ba16,0x4ba5c,0x4ba94,0x4bb2c,0x4bb88,0x4bbaa,0x4bbfc,0x4bc68,0x4bca2,0x4bcd2,
    0x4bd32,0x4be04,0x4be26,0x4be74,0x4bebc,0x4beec,0x4bf14,0x4bf66,0x4bf80,0x4c060,0x4c07a,0x4c0d2,0x4c0ee};
static const uint32_t SHOTS[]={0x4b79c,0x4b7ec,0x4b87e,0x4cab4,0x4cb74,0x4cb96,0x4cce4,0x4cd1a,0x4cd32,0x4cd64,0x4c194,0x4c220,0x4c298};
static const uint32_t HATCH[]={0x4c38a,0x4c3ea,0x4c412,0x4c4d2,0x4c4e8,0x4c51a,0x4c566,0x4c5ca,0x4c5e6,0x4c64a,0x4c660};
static const uint32_t MORTAR[]={0x4cf2c,0x4cf56,0x4cf5c,0x4cf92,0x4d0e0};
#define IN(r,s) in((r),(s),sizeof(s)/sizeof((s)[0]))
bool neo_m3boss_routine(uint32_t r) {
    return IN(r,BODY) || IN(r,LEGS) || IN(r,SHOTS) || IN(r,TORSO) || IN(r,ARM) || IN(r,POD) || IN(r,HATCH) || IN(r,MORTAR) ||
        r==0x4c69e || r==0x4c6c2 || r==0x4d1da || r==0x4d284 || r==0x4d2ce || r==0x4d390 || r==0x4c7b6 || r==0x4c802 || r==0x4c840 || r==0x4c88c || r==0x4e2ea || r==0x4e376 || r==0x4e3be || r==0x4e450 || r==0x4ad12 || r==0x4ad94 || r==0x4ae1c || r==0x4e558 || r==0x4e568;
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
    if (r==0x4b79c || r==0x4b7ec) return emitter_tick(a,e);
    if (r==0x4b87e) return segment_tick(a,e);
    if (r==0x4cab4 || r==0x4cb74 || r==0x4cb96) return ball_tick(a,e);
    if (r==0x4cce4 || r==0x4cd1a) return puddle_tick(a,e);
    if (r==0x4cd32 || r==0x4cd64) return trail_tick(a,e);
    if (r==0x4c194 || r==0x4c220 || r==0x4c298) return missile_tick(a,e);
    if (r==0x4c69e || r==0x4c6c2) return floor_tick(a,e);
    if (r==0x4d1da || r==0x4d284 || r==0x4d2ce || r==0x4d390) return bomb_tick(a,e);
    if (r==0x4c7b6 || r==0x4c802 || r==0x4c840 || r==0x4c88c) return hatch_shot_tick(a,e);
    if (r>=0x4e2ea && r<=0x4e450) return foot_tick(a,e);
    if (r==0x4e558 || r==0x4e568) {                                    /* list pieces: sound 1022, jmp 01C706 / 01C6EE */
        sound(e,0x1022);SL(0,r==0x4e558?0x1c706:0x1c6ee);return neo_effects_tick(a,e);
    }
    if (r==0x4ad12 || r==0x4ad94 || r==0x4ae1c) return dust_tick(a,e);
    STOP(r);
}
