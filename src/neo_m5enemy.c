#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 5 enemies, manual translation (ROM labels as provenance).
 *
 * ---- the street's tank 08DCB2 (script record 2DCF1A) and its turret 08EB18 ----
 * MAME captures: captures-m5/fam/T (run4 replay, killed at once) and fam/K (the
 * same replay without fire in 248100..249600: it drives, turns, aims and fires).
 * - 08DCB2: 08F1EE (+93 = +94 = 0; +70 = 0: facing left, +93 bit 7; +88 = +6E,
 *   +89 = +72, the four words cleared), 08F2A2 (0146E8, +96 = +15, palette
 *   08FC8C[+89 & 3] and flash 306, layers 8800, solid body 08FC66 by 004E76,
 *   hurtbox 08FC0A, the turret 08EB18 by 0040BC (D0 = E) at (0, 0) with the
 *   tank's +93 / +89 / +88), +72 = rank 2CED2C << 8 (ground speed), +7E = rank
 *   2CED8E (updates before a shot), +4E = rank 2CECCA.
 * - +8D is the order the turret follows (table 08FEF0): 0 idle, 1 aim, 2 / 3
 *   fire, 4 back to the front, 5 wait, 7 destroyed.
 * - 08DD34 (+95 = 0): 01B1DC, program 28AEF0, +8F = rank 2CEE34; 08DD6E: 08F4CE,
 *   00427A; unless +6C = 3 (01B284): +8F runs out -> 08DEDE; 08F752 -> 08DF42.
 *   08DDA2 (+95 set) the same without the count.
 * - 08DEDE: program 28B13A / 28B250 (08F4A0: facing against +93 bit 7); 08DF16:
 *   08F51C (a wall: +93 bit 7 toggled, 08DE84), 00427A, its end -> 08DDFA.
 * - 08DDFA: +62 = -+72 (or +72 with +93 bit 7), program 28AD3A / 28ADF4;
 *   08DE46: 08F51C, 00427A, 08F57C (the screen's sides 30 / 110 or the target
 *   08F5DE) and 08F752 -> 08DE84.
 * - 08DE84: 01B1DC, program 28B03C / 28AF3E; 08DEC2: 08F4CE, 00427A, its end ->
 *   08DD34.
 * - 08DF42: 01B1DC, +7A = the nearest player (01AF9A), +8D = 1, program 28AEF0;
 *   08DF76: once the turret says it is aimed (+8E) -> 08DF92: +8F = rank 2CEE1A
 *   shots; each one (08DFB4): +8D = 2 / 3 by turns (+92), +7E = rank 2CEE00,
 *   program 28B366; 08E000: the turret's shot done (+8E): +8D = 5; +7E runs out
 *   -> 08DFA6 (the next shot, or 08E034: +7E = rank 2CED8E + 1E, +8D = 4,
 *   program 28AEF0; 08E06E: the turret back (+8E) -> 08E08A -> 08DD34 / 08DEDE).
 * - Every state but the last ones ends in 08E096: 08E15A (the sound 108A when it
 *   lands, +86 the pause), 004C24 (a hit: +5A = 0, +93 bit 6 for the turret's
 *   program 28BCDC), +5F bit 7 -> 08E0D4, outside 08FC74 (01945C) or left of
 *   -80 (08F432) -> 08E128. The moving states attack with the row 08FC36
 *   (08F3B6; 08FC22 with +93 bit 4).
 * - 08E0D4: +17 = 0, 019DFC, +8D = 7, 004E76 (0), program 28B5CC (its calls
 *   08FB56: sound 1033, the debris lists 08FC94 and 08FCA6 with the layers 1001;
 *   08FB82: sound 1021, 08FCA6 and the blast 01C71E); 08E104: 08F4CE, 00427A, its
 *   end or 08F432 -> 08E128: +7E = 3C, +50 = 0; 08E144 counts and 08EF3E: 0041AC.
 * - 08F4CE / 08F51C run once per video frame (RAM 10000E low byte against +91):
 *   01436A, then in the air 0143DC (08F4CE: never rising slower than -2 px) or
 *   on the ground 0146A8 (08F51C; carry = +5F bit 0).
 * - The turret 08EB18: palettes, +88 &= 3, at (0, 0) of the tank (01AD6E).
 *   Every update: its own +8D = 7 -> 08EEBC; 08F88A (the tank's +8D changed: the
 *   state of table 08FEF0 / 08FF14 (+93 bit 1) / 08FF38 (+93 bit 5) entered at
 *   once, jmp (a1)); 08F3E2 (01ABC8, the tank's +1E, its program's offset +8B /
 *   +8C, a layer behind for +8A = 0 or >= 8, 0044F6); 00427A.
 *   08EB3C: +8A = 8, program 28B6F6 (the pose by +8A). 08EB8E: aims every 4
 *   updates (64 once aimed): 08F456, 08F98E (the angle of tables 08FE9C / 08FEBE
 *   by the target's x distance, mirrored by 08FEE0, 0 / 8 when the target is
 *   level, 08F836) one step of +8A per call, the tank's +8E = 1 when there.
 *   08ECCA: program 28B77C[+8A], 08FA4E (sound 1064, the flash 08EFFA and the
 *   shell 08F058 / 08F1A0 at the muzzle 08FD1C[+8A]), the program's end: the
 *   tank's +8E = 1. 08ED22: +8A back to 8 a step every 4 updates. 08EDAC and
 *   08EDFC (+8A = 4 / C): waits. 08EE62: program 28C10E a layer behind.
 *   08EE9C: the tank's +93 bit 6: program 28BCDC.
 *   08EEBC: thrown off (+34 = 6.4 px, +76 = gravity FFFF92C6, +30 = FFFFBBBC,
 *   layers 1001); 08EEEA: 0143E4; landed or +5F bit 0 -> 08F1E0 (sound 1025,
 *   08EFDE: 009F88, layers E0ED, +50 = 0, jmp 01C706); +40 <= C0 -> 08EF46;
 *   08F432 -> 08EFD6 (0041AC).
 * - The flash 08EFFA, the shell 08F058 and their ends 08EFD6 / 08EFDE / 08F1E0
 *   are the ones of neo_effects.c (the Mission 1 boss's turret fires the same
 *   ones); the family m5tank is their capture. The shell 08F1A0 of the +93
 *   bit 5 tanks is an explicit stop.
 *
 * 08EF46 is the cartridge check (RAM 10F00C + 3EF, on a carry the test of
 * 2FFFE0, branch to 08DC92 when it fails): not reproduced, as in neo_armored.c;
 * on the original board it ends in 004188. */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,v)
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define P1_NODE 0x1014c4u
#define P2_NODE 0x101574u
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint8_t rb(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;} return p[0];}
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;} return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2u);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {   /* 0260C8 */
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    uint16_t v=ga_rank_word(a,e,t);
    if (t[0]!=2u) return v;
    /* 02614A: type 2: the byte picks a group of 8 words after the byte rows (12 per bit of the mask), one of them at random (0029C6). */
    unsigned bits=0;for (unsigned b=t[1];b;b&=b-1u) bits++;
    return rw(e,table+2u+((uint32_t)v<<4)+bits*12u+(ga_rand(e)&0xeu));
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* 0040BC with its reserve D0 and 01B220 at (0, 0). */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,0,0);
    return c;
}
/* 01DA40: the emitter 01D764 of the debris list (0040BC with D0 = 1C, 01B220 at
 * (0, 0), +15 bit 2 from the caller, +86 = the list). */
static neo_gactor_t *debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764,0x1c);
    if (!c) return NULL;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
    return c;
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 0146A8: ground step; a non-flat contact restores the position and sets +5F
 * bit 0 (as neo_armored.c). */
static void k146a8(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t x=L(0x28),y=L(0x2c),p=L(0x38),kind=0;
    ga_ground(a,e,&kind);
    if ((B(0x15)&64u) && (uint16_t)kind!=0x4000u) {SL(0x38,p);SL(0x2c,y);SL(0x28,x);SB(0x5f,B(0x5f)|1u);ga_project(a,e);}
}
static void k143dc(neo_gactor_t *a,neo_genv_t *e) {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
static uint16_t px(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2x:e->players.p1x;}
static uint16_t py(const neo_genv_t *e,uint32_t node) {return node==P2_NODE?e->players.p2y:e->players.p1y;}
/* 01AF9A: the nearest live player's node (player 1 when none). */
static uint32_t nearest(neo_gactor_t *a,neo_genv_t *e) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m || m==1u) return P1_NODE;
    if (m==2u) return P2_NODE;
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    return S16(d2)<S16(d1)?P2_NODE:P1_NODE;
}
/* 01B29C: octagonal distance. */
static uint16_t distance(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1) {
    uint16_t d0=(uint16_t)(x0-x1),d1=(uint16_t)(y0-y1);
    if (d0&0x8000u) d0=(uint16_t)(0u-d0);
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (S16(d0)>S16(d1)) return (uint16_t)(d0+(d1>>1));
    return (uint16_t)((d0>>1)+d1);
}
/* ---- the tank 08DCB2 ---------------------------------------------------------------- */
/* 08F38E */
static void tank_palettes(neo_gactor_t *a,neo_genv_t *e) {
    ga_palette(a,rw(e,0x8fc8cu+2u*(B(0x89)&3u)));
    ga_flash_palette(a,0x306);
}
/* 08F4A0: carry when it faces against its direction +93 bit 7. */
static bool tank_reversed(neo_gactor_t *a) {return ((B(0x93)>>7)&1u)!=(B(0x15)&1u);}
/* 091A20: the same for the tank 08FF7E (+96 bit 7). */
static bool tank_reversed_96(neo_gactor_t *a) {return ((B(0x96)>>7)&1u)!=(B(0x15)&1u);}
/* 08F4CE */
static void tank_stand(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d0=(uint8_t)e->video;
    if (d0==B(0x91)) return;
    SB(0x91,d0);
    ga_stationary(a,e);                                                  /* 01436A */
    if (!(B(0x15)&0x40u)) {
        if ((int32_t)L(0x34)>(int32_t)0xfffe0000u) SL(0x34,0xfffe0000u);
        k143dc(a,e);
    } else {SL(0x30,0);SL(0x34,0);}
}
/* 08F51C: carry = +5F bit 0. */
static bool tank_drive(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d0=(uint8_t)e->video;
    if (d0==B(0x91)) return false;
    SB(0x91,d0);
    ga_stationary(a,e);                                                  /* 01436A */
    if (B(0x15)&0x40u) k146a8(a,e);
    else {
        k143dc(a,e);
        if (B(0x15)&0x40u) {SL(0x30,0);SL(0x34,0);}
    }
    return (B(0x5f)&1u)!=0;
}
/* 08F432 */
static bool tank_gone(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0x8fc74)) return true;                               /* 01945C */
    return S16(W(0x3c))<-0x80;
}
/* 08F3B6 */
static void tank_attack(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x93)&0x20u) return;
    ga_attack(a,e,(B(0x93)&0x10u)?0x8fc22u:0x8fc36u);                    /* 0049E8 */
}
/* 08F5DE */
static bool tank_passed(neo_gactor_t *a,neo_genv_t *e) {
    if (!(B(0x93)&4u) || !L(0x7a)) return false;
    uint16_t d0=px(e,L(0x7a));
    if (!(B(0x93)&0x80u)) {
        d0+=8u;
        if (S16(d0)<S16(W(0x3c))) return false;
    } else {
        d0-=8u;
        if (S16(d0)>S16(W(0x3c))) return false;
    }
    SB(0x93,B(0x93)^0x80u);
    return true;
}
/* 08F57C (08F636: no carry) */
static bool tank_turn(neo_gactor_t *a,neo_genv_t *e) {
    if (S16(W(0x3c))<=0x30 && !(B(0x93)&0x80u)) {SB(0x93,B(0x93)|0x80u);return true;}
    if (S16(W(0x3c))>=0x110 && (B(0x93)&0x80u)) {SB(0x93,B(0x93)&0x7fu);return true;}
    return tank_passed(a,e);
}
/* 08F752: carry when +7E has run out, it is on screen (20..120) and the nearest
 * player is inside the reach 08FCF8[+88] for the height difference. */
static bool tank_reach(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x7e,W(0x7e)-1u);
    if (S16(W(0x7e))>0) return false;
    SW(0x7e,0);
    if (S16(W(0x3c))<0x20 || S16(W(0x3c))>0x120) return false;
    uint32_t node=nearest(a,e);
    uint16_t d0=(uint16_t)(py(e,node)-W(0x40));
    if (S16(d0)<=0) d0=(uint16_t)(0u-d0);
    if (S16(d0)>0x80) d0=0x80;
    d0>>=5;
    uint16_t d1=rb(e,rl(e,0x8fcf8u+4u*(B(0x88)&3u))+d0);
    int16_t dx=S16((uint16_t)(px(e,node)-W(0x3c)));
    return !(dx<-(int16_t)d1) && !(dx>(int16_t)d1);
}
/* 08E15A */
static void tank_land(neo_gactor_t *a,neo_genv_t *e) {
    if (!(B(0x15)&0x40u)) {
        if (W(0x86)) SW(0x86,W(0x86)-1u);
    } else {
        if (!(B(0x96)&0x40u) && !W(0x86)) sound(e,0x108a);
        SW(0x86,5);
    }
    SB(0x96,B(0x15));
}
static bool tank_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    neo_gactor_t *c;
    switch (routine) {
    case 0x8fb56:
        sound(e,0x1033);
        debris(a,e,0x8fc94);c=debris(a,e,0x8fca6);
        if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
        return true;
    case 0x8fb82:
    case 0x8fbc6:
        sound(e,0x1021);
        debris(a,e,0x8fca6);c=child(a,e,0x1c71e,0x1c);
        if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
        return true;
    }
    return false;
}
static ga_result_t tank_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;neo_gactor_t *c;
    for (;;) switch (L(0)) {
    case 0x8dcb2:
        SB(0x93,0);SB(0x94,0);                                           /* 08F1EE */
        if (!W(0x70)) {SB(0x15,B(0x15)|1u);SB(0x93,B(0x93)|0x80u);}
        SB(0x88,W(0x6e));SB(0x89,W(0x72));
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);
        ga_floor(a,e);                                                   /* 08F2A2: 0146E8 */
        SB(0x96,B(0x15));
        tank_palettes(a,e);
        SW(0x66,0x8800);SW(0x1e,0x8800);
        if (!(B(0x93)&0x20u)) {
            if (e->solid) e->solid(e->ctx,a,0x8fc66);                    /* 004E76 */
            SL(0x50,0x8fc0a);
        } else SL(0x50,0x8fc16);
        c=child(a,e,0x8eb18,0xe);
        if (c) {ga_sb(c,0x93,B(0x93));ga_sb(c,0x89,B(0x89));ga_sb(c,0x88,B(0x88));}
        SL(0x72,(uint32_t)(int32_t)S16(rank(a,e,0x2ced2c))<<8);
        SW(0x7e,rank(a,e,0x2ced8e));
        SW(0x4e,rank(a,e,0x2cecca));
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x8dd34);continue;
    case 0x8dd34:
        if (B(0x95)) {SL(0,0x8dda2);continue;}
        /* fall through */
    case 0x8dda2: {
        bool counted=L(0)==0x8dd34u;
        k1b1dc(a);SB(0x8d,0);SB(0x8f,rank(a,e,0x2cee34));
        START(0x28aef0);
        SL(0,counted?0x8dd6eu:0x8ddd4u);continue;}
    case 0x8dd6e:
    case 0x8ddd4: {
        bool counted=L(0)==0x8dd6eu;
        tank_stand(a,e);
        if (ga_anim_step(a,e,tank_call)<0) return GA_UNSUPPORTED;
        if (B(0x6c)!=3u) {                                               /* 01B284 */
            if (counted) {
                SB(0x8f,B(0x8f)-1u);
                if ((int8_t)B(0x8f)<=0) SL(0,0x8dede);
            }
            if (tank_reach(a,e)) SL(0,0x8df42);
        }
        goto tail;}
    case 0x8ddfa: {
        SB(0x8d,0);
        uint32_t d0=L(0x72);
        if (!(B(0x93)&0x80u)) d0=0u-d0;
        SL(0x62,d0);
        START(tank_reversed(a)?0x28adf4u:0x28ad3au);
        SL(0,0x8de46);}
        /* fall through */
    case 0x8de46:
        if (tank_drive(a,e)) {SB(0x93,B(0x93)^0x80u);SL(0,0x8de84);}
        if (ga_anim_step(a,e,tank_call)<0) return GA_UNSUPPORTED;
        if (tank_turn(a,e)) SL(0,0x8de84);
        if (tank_reach(a,e)) SL(0,0x8de84);
        tank_attack(a,e);
        goto tail;
    case 0x8de84:
        k1b1dc(a);SB(0x8d,0);
        START(tank_reversed(a)?0x28af3eu:0x28b03cu);
        SL(0,0x8dec2);
        /* fall through */
    case 0x8dec2:
        tank_stand(a,e);
        if ((r=ga_anim_step(a,e,tank_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8dd34);
        tank_attack(a,e);
        goto tail;
    case 0x8dede:
        SB(0x8d,0);
        START(tank_reversed(a)?0x28b250u:0x28b13au);
        SL(0,0x8df16);
        /* fall through */
    case 0x8df16:
        if (tank_drive(a,e)) {SB(0x93,B(0x93)^0x80u);SL(0,0x8de84);}
        if ((r=ga_anim_step(a,e,tank_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8ddfa);
        tank_attack(a,e);
        goto tail;
    case 0x8df42:
        k1b1dc(a);
        SL(0x7a,nearest(a,e));                                           /* 01AF9A */
        SB(0x8d,1);SB(0x8f,0);SB(0x8e,0);
        START(0x28aef0);
        SL(0,0x8df76);
        /* fall through */
    case 0x8df76:
        tank_stand(a,e);
        if (ga_anim_step(a,e,tank_call)<0) return GA_UNSUPPORTED;
        if (B(0x8e)) SL(0,0x8df92);
        goto tail;
    case 0x8df92:
        SB(0x8f,rank(a,e,0x2cee1a));
        goto shot;
    case 0x8dfa6:
        SB(0x8f,B(0x8f)-1u);
        if ((int8_t)B(0x8f)<=0) {SL(0,0x8e034);continue;}
    shot:
        if (!B(0x92)) {SB(0x8d,2);SB(0x92,0xff);} else {SB(0x8d,3);SB(0x92,0);}
        SB(0x8e,0);
        SW(0x7e,rank(a,e,0x2cee00));
        START(0x28b366);
        SL(0,0x8e000);
        /* fall through */
    case 0x8e000:
        tank_stand(a,e);
        if (ga_anim_step(a,e,tank_call)<0) return GA_UNSUPPORTED;
        if (B(0x8e)) {SB(0x8e,0);SB(0x8d,5);}
        SW(0x7e,W(0x7e)-1u);
        if (S16(W(0x7e))<=0) SL(0,0x8dfa6);
        goto tail;
    case 0x8e034:
        SW(0x7e,rank(a,e,0x2ced8e)+0x1eu);
        SB(0x8f,0);SB(0x8e,0);SB(0x8d,4);
        START(0x28aef0);
        SL(0,0x8e06e);
        /* fall through */
    case 0x8e06e:
        tank_stand(a,e);
        if (ga_anim_step(a,e,tank_call)<0) return GA_UNSUPPORTED;
        if (B(0x8e)) SL(0,0x8e08a);
        goto tail;
    case 0x8e08a:
        SL(0,B(0x95)?0x8dd34u:0x8dedeu);continue;
    tail:                                                                /* 08E096 */
        tank_land(a,e);
        SB(0x93,B(0x93)&0xbfu);
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SB(0x93,B(0x93)|0x40u);}   /* 004C24 */
        if (B(0x5f)&0x80u) SL(0,0x8e0d4);
        if (tank_gone(a,e)) SL(0,0x8e128);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8e0d4:
        SB(0x17,0);SB(0x69,0xff);                                        /* 019DFC */
        SB(0x8d,7);
        if (e->solid) e->solid(e->ctx,a,0);                              /* 004E76 */
        START(0x28b5cc);
        SL(0,0x8e104);
        /* fall through */
    case 0x8e104:
        tank_stand(a,e);
        if ((r=ga_anim_step(a,e,tank_call))<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x8e128);
        if (tank_gone(a,e)) SL(0,0x8e128);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8e128:
        SW(0x7e,0x3c);SL(0x50,0);SB(0x69,0xff);SB(0x8d,7);
        SL(0,0x8e144);
        /* fall through */
    case 0x8e144:
        SW(0x7e,W(0x7e)-1u);
        if (S16(W(0x7e))<=0) SL(0,0x8ef3e);
        return GA_MORE;
    case 0x8ef3e:
        return GA_DELETE_TREE;                                           /* 0041AC */
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* ---- the turret 08EB18 ---------------------------------------------------------------- */
/* 08F88A: carry with the state of the tank's new order. */
static bool turret_order(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *p,uint32_t *next) {
    uint32_t table=0x8fef0;
    if (B(0x93)&2u) table=0x8ff14;
    if (B(0x93)&0x20u) table=0x8ff38;
    uint8_t d0=ga_b(p,0x8d);
    if (d0==B(0x8d)) {SB(0x8d,d0);return false;}
    SB(0x8d,d0);
    uint32_t t=rl(e,table+4u*(d0&15u));
    if (!t) return false;
    *next=t;return true;
}
/* 08F3E2 */
static void turret_follow(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *p) {
    ga_project(a,e);
    SW(0x1e,ga_w(p,0x1e));
    int16_t d0=(int8_t)ga_b(p,0x8b);
    if (B(0x15)&1u) d0=(int16_t)-d0;
    SW(0x28,d0);
    SW(0x2c,(int16_t)(int8_t)ga_b(p,0x8c));
    uint8_t n=B(0x8a)&0xfu;
    if (!n || n>=8u) SW(0x1e,W(0x1e)-1u);
    uint8_t f=ga_b(p,0x17);                                              /* 0044F6 */
    if (f) {
        if (!(f&0x80u)) f++;
        SB(0x17,f);
    }
}
/* 08F456 */
static void turret_range(neo_gactor_t *a,neo_genv_t *e) {
    if (!(B(0x93)&8u)) return;
    uint16_t d0=distance(W(0x3c),W(0x40),px(e,L(0x7a)),py(e,L(0x7a)));
    uint8_t d1=0;
    if (S16(d0)>=0x70) d1=1;
    if (S16(d0)>=0xb0) d1=2;
    SB(0x88,d1&3u);
}
/* 08F836 */
static uint8_t turret_level(neo_gactor_t *a,neo_genv_t *e,uint8_t d0) {
    SB(0x6e,d0);
    int16_t d=S16((uint16_t)(py(e,L(0x7a))-W(0x40)));
    if (d<-8 || d>0x28) return B(0x6e);
    d=S16((uint16_t)(px(e,L(0x7a))-W(0x3c)));
    if (B(0x15)&1u) d=(int16_t)-d;
    return d>0?0:8;
}
/* 08F98E: one step of +8A towards the target's angle; carry when there. */
static bool turret_aim(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t node=L(0x7a);
    uint16_t d0=(uint16_t)(px(e,node)-W(0x3c));
    if (S16(d0)<=-0x100) d0=0xff00;
    if (S16(d0)>=0x100) d0=0x100;
    d0+=0x100u;d0>>=4;
    uint8_t d=rb(e,(S16(py(e,node))>S16(W(0x40))?0x8fe9cu:0x8febeu)+d0);
    if (B(0x15)&1u) d=rb(e,0x8fee0u+d);
    d=turret_level(a,e,d)&0xfu;
    if (d==B(0x8a)) return true;
    d=(uint8_t)(d-B(0x8a));
    SB(0x8a,B(0x8a)+((int8_t)d>0?1u:0xffu));
    return false;
}
/* 08FA4E: the flash 08EFFA and the shell 08F058 / 08F1A0. */
static void turret_fire(neo_gactor_t *a,neo_genv_t *e) {
    sound(e,0x1064);
    uint8_t d0=B(0x8a);
    if (B(0x15)&1u) d0=rb(e,0x8fee0u+(d0&0xfu));
    SB(0x6e,d0&0xfu);
    neo_gactor_t *c=child(a,e,0x8effa,0x1c);
    uint32_t at=0x8fd1cu+4u*(B(0x8a)&0xfu);
    int16_t d1=S16(rw(e,at)),d2=S16(rw(e,at+2u));
    if (B(0x15)&1u) d1=(int16_t)-d1;
    if (c) {ga_sb(c,0x8a,B(0x8a));ga_sb(c,0x8b,(uint8_t)d1);ga_sb(c,0x8c,(uint8_t)d2);}
    if (B(0x93)&0x20u) {ga_fail(e,0x8f1a0);return;}                      /* the shell of the +93 bit 5 tanks: not a Mission 5 record */
    c=child(a,e,0x8f058,0x1c);
    if (!c) return;
    ga_sb(c,0x8a,B(0x6e));
    ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+(uint16_t)d1));ga_sw(c,0x2c,(uint16_t)(ga_w(c,0x2c)+(uint16_t)d2));
    ga_sb(c,0x93,0);ga_sb(c,0x94,0);
    if (B(0x94)&1u) ga_sb(c,0x94,(uint8_t)(ga_b(c,0x94)|1u));
    ga_sb(c,0x88,B(0x88));
}
static ga_result_t turret_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint32_t next=0;
    neo_gactor_t *p=creator(a,e);
    if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    for (;;) switch (L(0)) {
    case 0x8eb18:
        tank_palettes(a,e);
        SB(0x88,B(0x88)&3u);SB(0x8d,0);
        ga_place(a,e,0,0,NEO_GA_TAG(p->index));                          /* 01AD6E */
        if (B(0x93)&2u) {SL(0,0x8edfc);continue;}
        /* fall through */
    case 0x8eb3c:
        SB(0x8a,8);
        START(0x28b6f6);
        SL(0,0x8eb58);continue;
    case 0x8eb8e:
        SB(0x8f,0);SL(0x7a,ga_l(p,0x7a));SB(0x8e,0);
        START(0x28b6f6);
        SL(0,0x8ebb6);continue;
    case 0x8ec3a:
    case 0x8ed22:
    case 0x8edac:
        SB(0x8f,0);
        START(0x28b6f6);
        SL(0,L(0)==0x8ec3au?0x8ec54u:L(0)==0x8ed22u?0x8ed3cu:0x8edc6u);continue;
    case 0x8edfc:
        SB(0x8f,0);SB(0x8a,(B(0x93)&1u)?4:0xc);
        START(0x28b6f6);
        SL(0,0x8ee2c);continue;
    case 0x8ecca:
        START(rl(e,0x28b77cu+4u*(B(0x8a)&0xfu)));
        turret_fire(a,e);
        SL(0,0x8ecf4);continue;
    case 0x8ee62:
        START(0x28c10e);
        SL(0,0x8ee78);continue;
    case 0x8eb58:
    case 0x8ebb6:
    case 0x8ec54:
    case 0x8ecf4:
    case 0x8ed3c:
    case 0x8edc6:
    case 0x8ee2c:
    case 0x8ee78: {
        uint32_t state=L(0);
        if (B(0x8d)==7u) {SL(0,0x8eebc);continue;}
        if (turret_order(a,e,p,&next)) {SL(0,next);continue;}           /* jmp (a1) */
        if (e->unsupported) return GA_UNSUPPORTED;
        turret_follow(a,e,p);
        switch (state) {
        case 0x8ebb6:
            if (!B(0x8e)) SB(0x8f,(B(0x8f)+1u)&3u); else SB(0x8f,(B(0x8f)+1u)&0x3fu);
            if (!B(0x8f)) {
                turret_range(a,e);
                ga_sb(p,0x8e,0);
                if (turret_aim(a,e)) {ga_sb(p,0x8e,1);SB(0x8e,1);} else SB(0x8e,0);
            }
            break;
        case 0x8ec54:
            SB(0x8f,(B(0x8f)+1u)&3u);
            if (!B(0x8f)) {
                uint8_t d0=ga_b(p,0x8a);
                if (d0==B(0x8a)) ga_sb(p,0x8e,1);
                else {SB(0x8a,B(0x8a)+((int8_t)d0>=(int8_t)B(0x8a)?1u:0xffu));ga_sb(p,0x8e,0);}
            }
            break;
        case 0x8ed3c:
            SB(0x8f,(B(0x8f)+1u)&3u);
            if (!B(0x8f)) {
                if (B(0x8a)==8u) ga_sb(p,0x8e,1);
                else SB(0x8a,B(0x8a)+((int8_t)B(0x8a)>8?0xffu:1u));
            }
            break;
        case 0x8ee78:
            SW(0x1e,W(0x1e)-1u);
            break;
        }
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        if (state==0x8ee78u) return GA_MORE;
        if (state==0x8ecf4u) {
            if (r) ga_sb(p,0x8e,1);
            return GA_MORE;
        }
        if (r) START(0x28b6f6);
        if (ga_b(p,0x93)&0x40u) START(0x28bcdc);                         /* 08EE9C */
        return GA_MORE;}
    case 0x8eebc:
        SB(0x17,0);
        SL(0x34,0x66666);SL(0x76,0xffff92c6u);SL(0x30,0xffffbbbcu);
        SW(0x66,0x1001);SW(0x1e,0x1001);
        SL(0,0x8eeea);
        /* fall through */
    case 0x8eeea:
        SL(0x34,L(0x34)+L(0x76));
        ga_air(a,e);                                                     /* 0143E4 */
        if (B(0x15)&0x40u) SL(0,0x8f1e0);
        if (B(0x5f)&1u) SL(0,0x8f1e0);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (S16(W(0x40))<=0xc0) SL(0,0x8ef46);
        if (tank_gone(a,e)) SL(0,0x8efd6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x8ef46:
        return GA_DELETE;                                                /* the cartridge check, then 004188 */
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* ---- the tank on its treads 08FF7E (street record 2DCFA4, sewer) and its turret 090A62 ---- */
/* MAME captures: captures-m5/fam/T (street) and fam/W (sewer).
 * - 08FF7E: 0913DE (+96 = 0; +70 = 0: facing left, +96 bit 7; +93 = +6E, +94 = +72, the three words cleared), +96
 *   bit 0 (08FF8C, the entry without it, is in no record), 0146E8, +97 = +15, the palette 091DE0[+94 & 3] and the
 *   flash 306, +5F bits 4 and 5, +90 = 8, +92 = FF, hurtbox 091CC8, solid body 091D98, layers 8800, +6E = rank
 *   2CED2C << 8, +80 = rank 2CED8E, +4E = rank 2CECCA, the tread node 014AE8 in +7A and the turret 090A62 (D0 = E)
 *   with its +96 / +93 / +94.
 * - 091416, once per video frame (RAM 10000E low byte against +8D): +8E counts, +96 bit 5 = 0918FC (pushed by a
 *   solid, +15 bit 3, with a player ahead of its motion, inside the body 091E90[+92] when both play), +82 / +84 the
 *   position, the treads 014C6A on the ground or 014C2E with the gravity FFFF6000 in the air, and +90 = the slope
 *   (091A72: the angle's high byte + 20 clamped to 0..40, mirrored when it faces right, / 4). 091494 is the same
 *   keeping +90.
 * - 0919F4: the pose of the slope +90: 0, 8 or 10 -> 0, 1, 2; anything else is carry and 3 (the tilting state
 *   090698). Every state picks its program by that pose from a table of four (the fourth is the empty 28C440) and
 *   takes the hurtbox 091E80[pose] and the solid body 091E90[pose] (091AB6).
 * - +60 is the order the turret follows (table 0924B0): 0 idle, 1 tilting, 2 aim, 3 / 4 fire, 5 back to the front,
 *   6 wait, 8 destroyed; +61 the turret's answer.
 * - 090042 idle (+8C = rank 2CEE34 updates -> 0903C4; 0917FC -> 0904B0); 0903C4 starting (091898: against a wall
 *   for A updates near the screen's sides, 1E in the middle -> turns, 0902E4; its end -> 090124); 090124 driving
 *   (+62 = +-+6E; +96 bit 5, the sides 30 / 110 or 0917FC -> 0902E4); 0902E4 stopping (-> 090042); 0904B0 aims
 *   (+76 = 01AF9A, order 2; the turret aimed -> 090538: +8C = rank 2CEE1A shots, each 090548 with the order 3 / 4 by
 *   turns (+95) and +80 = rank 2CEE00 updates, order 6 once the turret fired; 09060E: order 5, +80 = rank 2CED8E +
 *   1E; the turret back -> 0903C4); 090698 tilting and moving, 090772 tilting and waiting (program 28C442, hurtbox
 *   091CEC, body 091DC2).
 * - 0917FC: unless it is level (+92 = 1) in the stage RAM 10C2F6 = 5: +80 runs out, on screen (20..120) and the
 *   nearest player inside the reach 091E5C[+93][height difference / 20].
 * - Every state ends in 0908A6 (004C24: +5A = 0, +96 bit 6 for the turret) and 0908C0 (090A1E: the sound 108A when
 *   it lands; +5F bit 7 -> 0908E4; outside 091DD0 or left of -30 -> 09094C).
 * - 0908E4: destroyed (order 8, program 091E4C[pose]: its calls 091BF0 / 091C28 / 091C78); 09094C: 3C updates and
 *   0041AC.
 * - The turret 090A62: +8A = 0923C0[+93 & 3] (the flight time of its shell), at (0, 0) of the tank; every update
 *   takes the tank's order (jmp of 0924B0 at once), 0914BC (the tank's +1E, its +90, the offset +88 / +89 of the
 *   tank's program, a layer behind for +91 = 0 or >= 8, 0044F6) and the program of the pose (the tank's +90).
 *   090C20 aims: 09174E every 4 updates when +96 bit 1 (the launch 03B332 to the target under FFFFE000 in +8A
 *   updates, twice: from the centre and from the muzzle 091ED0[+92][angle]; its speeds stay in +30 / +34 for the
 *   shell, the angle in +8F) and one step of +91 towards it (09153A; level with the target: 0 or 8), the tank's
 *   +61 = 1 when there. 090D8C fires (091B02: sound 1064, the flash 091290 and the shell 0912EE). 090E10: +91
 *   back to 8. 0911DE: thrown off, as the turret 08EEBC.
 *   091638 reads the target's x through A0 after 002EB4, which leaves A0 at its table 002D7C unless one of the
 *   halved components has a zero low byte: the word at ROM 002DB8, translated as it is.
 * 090986 is the cartridge check (as 08EF46): it ends in 004188. 0907FC is referenced nowhere in the ROM. */
static const uint32_t ST_IDLE[4]={0x28ca64,0x28c9c8,0x28ca16,0x28c440};      /* 090096, 0900EE, 090506, 090666 */
static const uint32_t ST_DRIVE[4]={0x28c6e0,0x28c56c,0x28c626,0x28c440};     /* 090174, 090216 */
static const uint32_t ST_DRIVE_R[4]={0x28c90e,0x28c79a,0x28c854,0x28c440};   /* 0901B6, 090258 */
static const uint32_t ST_STOP_R[4]={0x28ccae,0x28cab2,0x28cbb0,0x28c440};    /* 090330 */
static const uint32_t ST_STOP[4]={0x28cfa8,0x28cdac,0x28ceaa,0x28c440};      /* 090372 */
static const uint32_t ST_START[4]={0x28d2d2,0x28d0a6,0x28d1bc,0x28c440};     /* 090408 */
static const uint32_t ST_START_R[4]={0x28d614,0x28d3e8,0x28d4fe,0x28c440};   /* 09044A */
static const uint32_t ST_FIRE[4]={0x28d7ca,0x28d72a,0x28d77a,0x28c440};      /* 0905AC */
static const uint32_t ST_DEAD[4]={0x28e146,0x28def2,0x28e01c,0x28e270};      /* 091E4C */
static const uint32_t SU_POSE[4]={0x28e4b8,0x28e3ac,0x28e432,0x28c440};      /* 090AC8 and its eleven copies */
static const uint32_t SU_FIRE[4]={0x28efc6,0x28e53e,0x28ea82,0x28e53e};      /* 091EC0 */
static const uint32_t SU_OFF[4]={0x2905dc,0x2901a0,0x2903be,0x28c440};       /* 0910DA, 09116E */
static const uint32_t SU_HIT[4]={0x28fd6e,0x28f50a,0x28f93c,0x28c440};       /* 0911CC */
/* 0919F4: carry while the slope +90 is between two poses. */
static bool st_pose(neo_gactor_t *a,uint8_t *d0) {
    if (B(0x90)&7u) {*d0=3;return true;}
    *d0=(uint8_t)(((B(0x90)&0x1fu)>>3)&3u);return false;
}
/* 091512 */
static void st_palettes(neo_gactor_t *a,neo_genv_t *e) {
    ga_palette(a,rw(e,0x91de0u+2u*(B(0x94)&3u)));
    ga_flash_palette(a,0x306);
}
/* 091AB6 */
static void st_body(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t i=4u*(B(0x92)&3u);
    SL(0x50,rl(e,0x91e80u+i));
    if (e->solid) e->solid(e->ctx,a,rl(e,0x91e90u+i));
}
/* 091ADA */
static void st_attack(neo_gactor_t *a,neo_genv_t *e) {
    ga_attack(a,e,rl(e,((B(0x96)&1u)?0x91ea0u:0x91eb0u)+4u*(B(0x92)&3u)));
}
/* 01AF3C: carry (true) when the player's node is outside the oriented rectangle. */
static bool st_outside(neo_gactor_t *a,neo_genv_t *e,uint32_t node,uint32_t rect) {
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=S16(rw(e,rect+2u*i));
    int16_t d1=S16((uint16_t)(py(e,node)-W(0x40)));
    if (B(0x15)&2u) d1=(int16_t)-d1;
    if (d1<q[1] || d1>(int16_t)(q[1]+q[3])) return true;
    d1=S16((uint16_t)(px(e,node)-W(0x3c)));
    if (B(0x15)&1u) d1=(int16_t)-d1;
    return d1<q[0] || d1>(int16_t)(q[0]+q[2]);
}
/* 091978 */
static bool st_ahead(neo_gactor_t *a,neo_genv_t *e,uint32_t node) {
    if (S16(px(e,node))<=S16(W(0x3c))) return (L(0x62)&0x80000000u)!=0;
    return !(L(0x62)&0x80000000u);
}
/* 0918FC */
static bool st_blocked(neo_gactor_t *a,neo_genv_t *e) {
    if (!(B(0x15)&8u)) return false;
    unsigned m=e->players.active_mask;
    if (!(m&1u)) return st_ahead(a,e,P2_NODE);                          /* 01AEB0 */
    if (!(m&2u)) return st_ahead(a,e,P1_NODE);
    uint32_t rect=rl(e,0x91e90u+4u*(B(0x92)&3u))+4u;                    /* 0918E4 */
    if (!st_outside(a,e,P1_NODE,rect) && st_ahead(a,e,P1_NODE)) return true;
    if (st_outside(a,e,P2_NODE,rect)) return false;
    return st_ahead(a,e,P2_NODE);
}
/* 091416 (keep: 091494, +90 kept). False on an explicit stop. */
static bool st_step(neo_gactor_t *a,neo_genv_t *e,bool keep) {
    uint8_t d0=(uint8_t)e->video,k90=B(0x90);
    if (d0==B(0x8d)) return true;
    SB(0x8d,d0);SB(0x8e,B(0x8e)+1u);
    if (st_blocked(a,e)) SB(0x96,B(0x96)|0x20u); else SB(0x96,B(0x96)&0xdfu);
    SW(0x82,W(0x3c));SW(0x84,W(0x40));
    int32_t r;
    if (B(0x15)&0x40u) r=neo_tread_step_node(a,e,3,L(0x7a));             /* 014C6A */
    else {
        SL(0x34,L(0x34)+0xffff6000u);
        r=neo_tread_step_node(a,e,1,L(0x7a));                            /* 014C2E */
        if (r>=0 && (B(0x15)&0x40u)) {SL(0x30,0);SL(0x34,0);}
    }
    if (r<0) return false;
    uint8_t b=(uint8_t)((((uint16_t)r)>>8)+0x20u);                       /* 091A72 */
    if (b&0x80u) b=0;
    uint16_t w=b;
    if (w>0x40u) w=0x40;
    if (!(B(0x15)&1u)) w=(uint16_t)(0x40u-w);
    SB(0x90,keep?k90:(uint8_t)(w>>2));
    return true;
}
/* 0914A2 */
static void st_halt(neo_gactor_t *a) {
    SL(0x62,0);SL(0x30,0);
    if (B(0x15)&0x40u) SL(0x34,0);
}
/* 0917FC */
static bool st_reach(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x92)!=1u && (e->pickup?e->pickup->mission:0u)==5u) return false;
    SW(0x80,W(0x80)-1u);
    if (S16(W(0x80))>0) return false;
    SW(0x80,0);
    if (S16(W(0x3c))<0x20 || S16(W(0x3c))>0x120) return false;
    uint32_t node=nearest(a,e);
    uint16_t d0=(uint16_t)(py(e,node)-W(0x40));
    if (d0&0x8000u) d0=(uint16_t)(0u-d0);
    if (S16(d0)>0x80) d0=0x80;
    d0>>=5;
    uint16_t d1=rb(e,rl(e,0x91e5cu+4u*(B(0x93)&3u))+d0);
    int16_t dx=S16((uint16_t)(px(e,node)-W(0x3c)));
    return !(dx<-(int16_t)d1) && !(dx>(int16_t)d1);
}
/* 091898 */
static bool st_wall(neo_gactor_t *a) {
    if (!(B(0x5f)&1u)) {SB(0x8b,0);return false;}
    SB(0x8b,B(0x8b)+1u);
    int8_t d0=0xa;
    if (S16(W(0x3c))>0x30 && S16(W(0x3c))<0x110) d0=0x1e;
    if (!(d0<(int8_t)B(0x8b))) return false;
    SB(0x5f,B(0x5f)&0xfeu);
    return true;
}
/* 091A4E */
static bool st_gone(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_cull(a,e,0x91dd0)) return true;                               /* 01945C */
    return S16(W(0x3c))<-0x30;
}
/* 090A1E */
static void st_land(neo_gactor_t *a,neo_genv_t *e) {
    if (!(B(0x15)&0x40u)) {
        if (W(0x86)) SW(0x86,W(0x86)-1u);
    } else {
        if (!(B(0x97)&0x40u) && !W(0x86)) sound(e,0x108a);
        SW(0x86,5);
    }
    SB(0x97,B(0x15));
}
static bool st_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    neo_gactor_t *c;
    switch (routine) {
    case 0x91bf0:
        sound(e,0x1033);
        c=debris(a,e,0x91de8);
        if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
        c=debris(a,e,0x91dfa);
        if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
        return true;
    case 0x91c28:
    case 0x91c78:
        sound(e,0x1021);
        c=debris(a,e,0x91dfa);
        if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
        c=child(a,e,0x1c71e,0x1c);
        if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
        return true;
    }
    return false;
}
/* The program of the pose D0 with its hurtbox and body. */
static bool st_prog(neo_gactor_t *a,neo_genv_t *e,const uint32_t *t,uint8_t d0) {
    SB(0x92,d0);
    if (!ga_anim_start(a,e,t[d0&3u],0x100)) return false;
    st_body(a,e);return true;
}
#define ST_TILT() do { if (st_pose(a,&d0)) {SL(0,0x90698);goto again;} } while (0)
#define ST_PROG(t) do { if (!st_prog(a,e,(t),d0)) return GA_UNSUPPORTED; } while (0)
#define ST_STEP(keep) do { if (!st_step(a,e,(keep))) return GA_UNSUPPORTED; } while (0)
#define ST_ANIM() do { if ((r=ga_anim_step(a,e,st_call))<0) return GA_UNSUPPORTED; } while (0)
static ga_result_t st_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint8_t d0;neo_gactor_t *c;bool rev;
again:
    switch (L(0)) {
    case 0x8ff7e:
        SB(0x96,0);                                                      /* 0913DE */
        if (!W(0x70)) {SB(0x15,B(0x15)|1u);SB(0x96,B(0x96)|0x80u);}
        SB(0x93,W(0x6e));SB(0x94,W(0x72));
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);
        SB(0x96,B(0x96)|1u);
        ga_floor(a,e);                                                   /* 0146E8 */
        SB(0x97,B(0x15));
        st_palettes(a,e);
        SB(0x5f,B(0x5f)|0x30u);                                          /* 091AA8 */
        SB(0x90,8);SB(0x92,0xff);
        SL(0x50,0x91cc8);
        if (e->solid) e->solid(e->ctx,a,0x91d98);                        /* 004E76 */
        SW(0x66,0x8800);SW(0x1e,0x8800);
        SL(0x6e,(uint32_t)(int32_t)S16(rank(a,e,0x2ced2c))<<8);
        SW(0x80,rank(a,e,0x2ced8e));
        SW(0x4e,rank(a,e,0x2cecca));
        c=neo_tread_spawn(a,e);                                          /* 014AE8 */
        SL(0x7a,c?NEO_GA_TAG(c->index):0);
        c=child(a,e,0x90a62,0xe);
        if (c) {ga_sb(c,0x96,B(0x96));ga_sb(c,0x93,B(0x93));ga_sb(c,0x94,B(0x94));}
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x90042);
        /* fall through */
    case 0x90042:
        ST_TILT();
        st_halt(a);SB(0x8b,0);SB(0x60,0);
        SB(0x8c,rank(a,e,0x2cee34));
        ST_TILT();ST_PROG(ST_IDLE);
        SL(0,0x900ac);
        /* fall through */
    case 0x900ac:
        ST_STEP(false);
        if (st_pose(a,&d0)) {SL(0,0x90772);goto again;}
        if (d0!=B(0x92)) {ST_TILT();ST_PROG(ST_IDLE);}
        ST_ANIM();
        SB(0x8c,B(0x8c)-1u);
        if ((int8_t)B(0x8c)<=0) SL(0,0x903c4);
        if (st_reach(a,e)) SL(0,0x904b0);
        goto tail;
    case 0x90124: {
        SB(0x60,0);
        uint32_t v=L(0x6e);
        if (!(B(0x96)&0x80u)) v=0u-v;
        SL(0x62,v);
        rev=tank_reversed_96(a);
        ST_TILT();ST_PROG(rev?ST_DRIVE_R:ST_DRIVE);
        SL(0,0x901cc);}
        /* fall through */
    case 0x901cc:
        ST_STEP(false);
        ST_TILT();
        if (d0!=B(0x92)) {rev=tank_reversed_96(a);ST_TILT();ST_PROG(rev?ST_DRIVE_R:ST_DRIVE);}
        ST_ANIM();
        if (B(0x96)&0x20u) {SB(0x96,B(0x96)^0x80u);SL(0,0x902e4);}      /* 0919A4 */
        else if (S16(W(0x3c))<0x30 && !(B(0x96)&0x80u)) {SB(0x96,B(0x96)|0x80u);SL(0,0x902e4);}
        else if (S16(W(0x3c))>0x110 && (B(0x96)&0x80u)) {SB(0x96,B(0x96)&0x7fu);SL(0,0x902e4);}
        st_attack(a,e);
        if (st_reach(a,e)) SL(0,0x902e4);
        goto tail;
    case 0x902e4:
        ST_TILT();
        SB(0x60,0);st_halt(a);SB(0x8b,0);
        rev=tank_reversed_96(a);
        ST_TILT();ST_PROG(rev?ST_STOP_R:ST_STOP);
        SL(0,0x90388);
        /* fall through */
    case 0x90388:
        ST_STEP(false);
        if (st_pose(a,&d0) || d0!=B(0x92)) SL(0,0x90042);
        ST_ANIM();
        if (r) SL(0,0x90042);
        st_attack(a,e);
        goto tail;
    case 0x903c4:
        SB(0x60,0);
        ST_TILT();
        rev=tank_reversed_96(a);
        ST_TILT();ST_PROG(rev?ST_START_R:ST_START);
        SL(0,0x90460);
        /* fall through */
    case 0x90460:
        ST_STEP(false);
        if (st_wall(a)) {SB(0x96,B(0x96)^0x80u);SL(0,0x902e4);}
        if (st_pose(a,&d0)) SL(0,0x90772);
        else if (d0!=B(0x92)) SL(0,0x90042);
        ST_ANIM();
        if (r) SL(0,0x90124);
        st_attack(a,e);
        goto tail;
    case 0x904b0:
        st_halt(a);SB(0x8b,0);
        ST_TILT();
        SL(0x76,nearest(a,e));                                           /* 01AF9A */
        SB(0x60,2);SB(0x8c,0);SB(0x61,0);
        ST_TILT();ST_PROG(ST_IDLE);
        SL(0,0x9051c);
        /* fall through */
    case 0x9051c:
        ST_STEP(true);
        ST_ANIM();
        if (B(0x61)) SL(0,0x90538);
        goto tail;
    case 0x90538:
        SB(0x8c,rank(a,e,0x2cee1a));
        /* fall through */
    case 0x90548:
        if (!B(0x95)) {SB(0x60,3);SB(0x95,0xff);} else {SB(0x60,4);SB(0x95,0);}
        SB(0x61,0);
        SW(0x80,rank(a,e,0x2cee00));
        ST_TILT();ST_PROG(ST_FIRE);
        SL(0,0x905c2);
        /* fall through */
    case 0x905c2:
        ST_STEP(true);
        ST_ANIM();
        if (B(0x61)) {SB(0x61,0);SB(0x60,6);}
        SW(0x80,W(0x80)-1u);
        if (S16(W(0x80))<=0) {
            SB(0x8c,B(0x8c)-1u);
            SL(0,(int8_t)B(0x8c)>0?0x90548u:0x9060eu);
        }
        goto tail;
    case 0x9060e:
        SW(0x80,rank(a,e,0x2ced8e)+0x1eu);
        SB(0x8c,0);SB(0x61,0);
        ST_TILT();
        SB(0x60,5);
        ST_TILT();ST_PROG(ST_IDLE);
        SL(0,0x9067c);
        /* fall through */
    case 0x9067c:
        ST_STEP(true);
        ST_ANIM();
        if (B(0x61)) SL(0,0x903c4);
        goto tail;
    case 0x90698: {
        uint32_t v=L(0x6e);
        if (!(B(0x96)&0x80u)) v=0u-v;
        SL(0x62,v);
        SB(0x60,1);
        SL(0x50,0x91cec);
        if (e->solid) e->solid(e->ctx,a,0x91dc2);
        START(0x28c442);
        SL(0,0x906da);}
        /* fall through */
    case 0x906da:
        ST_STEP(false);
        if (!st_pose(a,&d0)) SL(0,0x90124);
        ST_ANIM();
        SB(0x96,B(0x96)&0xbfu);
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);                      /* 004C24 */
        SW(0x80,W(0x80)-1u);
        if (B(0x96)&0x20u) {SB(0x96,B(0x96)^0x80u);SL(0,0x90772);}
        else if (S16(W(0x3c))<0x10 && !(B(0x96)&0x80u)) {SB(0x96,B(0x96)|0x80u);SL(0,0x90772);}
        else if (S16(W(0x3c))>0x130 && (B(0x96)&0x80u)) {SB(0x96,B(0x96)&0x7fu);SL(0,0x90772);}
        st_attack(a,e);
        goto tail2;
    case 0x90772:
        st_halt(a);SB(0x8b,0);SB(0x60,1);
        SL(0x50,0x91cec);
        if (e->solid) e->solid(e->ctx,a,0x91dc2);
        SB(0x8c,(uint8_t)rank(a,e,0x2cee34)>>1);
        START(0x28c442);
        SL(0,0x907ba);
        /* fall through */
    case 0x907ba:
        ST_STEP(false);
        if (!st_pose(a,&d0)) SL(0,0x90042);
        ST_ANIM();
        SB(0x96,B(0x96)&0xbfu);
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        SW(0x80,W(0x80)-1u);
        SB(0x8c,B(0x8c)-1u);
        if ((int8_t)B(0x8c)<=0) SL(0,0x90698);
        goto tail2;
    tail:                                                                /* 0908A6 */
        SB(0x96,B(0x96)&0xbfu);
        if (ga_register(a,e,NULL,NULL)) {SB(0x5a,0);SB(0x96,B(0x96)|0x40u);}
    tail2:                                                               /* 0908C0 */
        st_land(a,e);
        if (B(0x5f)&0x80u) SL(0,0x908e4);
        if (st_gone(a,e)) SL(0,0x9094c);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x908e4:
        SB(0x17,0);
        k1b1dc(a);
        SB(0x69,0xff);                                                   /* 019DFC */
        if (e->solid) e->solid(e->ctx,a,0);
        SB(0x60,8);
        st_pose(a,&d0);
        START(ST_DEAD[d0&3u]);
        SL(0,0x90928);
        /* fall through */
    case 0x90928:
        ST_STEP(false);
        ST_ANIM();
        if (r) SL(0,0x9094c);
        if (st_gone(a,e)) SL(0,0x9094c);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x9094c:
        SW(0x80,0x3c);SL(0x50,0);SB(0x69,0xff);SB(0x60,8);
        SL(0,0x90968);
        /* fall through */
    case 0x90968:
        SW(0x80,W(0x80)-1u);
        if (S16(W(0x80))<=0) SL(0,0x9097e);
        return GA_MORE;
    case 0x9097e:
        return GA_DELETE_TREE;                                           /* 0041AC */
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* ---- its turret 090A62, the flash 091290 and the shell 0912EE ---- */
/* 03B332: speeds to cover D0/D1 in D3 updates under D2 (as neo_m3enemy.c). */
static void su_launch(int16_t d0,int16_t d1,uint32_t d2,uint16_t d3,uint32_t *vx,uint32_t *vy) {
    uint32_t g=(d2&0x80000000u)?0u-d2:d2;
    if ((int16_t)d3<2) d3=2;
    uint16_t q=(uint16_t)(0x10000u/d3);
    uint32_t x=(uint32_t)((int32_t)d0*(int16_t)q),y=(uint32_t)((int32_t)d1*(int16_t)q);
    uint32_t t=(g&0xffffu)*d3+(((g>>16)*d3)<<16);
    t=(uint32_t)((int32_t)t>>1);
    if (d2&0x80000000u) t=0u-t;
    *vx=x;*vy=y-t;
}
/* 091638: the angle (0..F) of the speeds D0 / D1. */
static uint8_t su_angle(neo_gactor_t *a,neo_genv_t *e,uint32_t node,uint32_t d0,uint32_t d1) {
    int16_t x=S16((uint16_t)((d0<<3)>>16)),y=S16((uint16_t)((d1<<3)>>16));
    uint16_t ang=neo_angle(x,y);                                         /* 002EB4 */
    /* A0 after 002EB4: the target still, or the table 002D7C. */
    bool kept=!((uint16_t)(x>>1)&0xffu) || !((uint16_t)(y>>1)&0xffu);
    uint16_t d2=(uint16_t)((kept?px(e,node):rw(e,0x2d7cu+0x3cu))-W(0x3c));
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    if (S16(d2)<0x30) {
        if (ang<0x30u) ang=0x30;
        else if (ang>0x50u) ang=0x50;
    }
    ang=(uint16_t)((ang+8u)&0xf0u);
    ang=(uint8_t)(ang+(uint8_t)((uint8_t)(B(0x92)<<5)-0x20u));
    if (ang>0x80u) ang=ang>=0xc0u?0:0x80;
    ang=(ang>>4)&0xfu;
    if (B(0x15)&1u) ang=rb(e,0x92490u+ang);
    return (uint8_t)ang;
}
/* 09160A: the muzzle of the angle. */
static void su_muzzle(neo_gactor_t *a,neo_genv_t *e,uint8_t d0,int16_t *d2,int16_t *d3) {
    uint32_t at=rl(e,0x91ed0u+4u*(B(0x92)&3u))+4u*(d0&0xfu);
    *d2=S16(rw(e,at));*d3=S16(rw(e,at+2u));
    if (B(0x15)&1u) *d2=(int16_t)-*d2;
}
/* 09174E */
static void su_aim(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t node=L(0x76),vx,vy;
    if (!node) return;
    uint16_t n=B(0x8a)&0x7fu;
    int16_t dx=S16((uint16_t)(px(e,node)-W(0x3c))),dy=S16((uint16_t)(py(e,node)+0x10u-W(0x40))),d2,d3;
    su_launch(dx,dy,0xffffe000u,n,&vx,&vy);vy<<=1;
    su_muzzle(a,e,su_angle(a,e,node,vx,vy),&d2,&d3);
    su_launch((int16_t)(dx-d2),(int16_t)(dy-d3),0xffffe000u,n,&vx,&vy);vy<<=1;
    SL(0x30,vx);SL(0x34,vy);
    uint16_t d0=su_angle(a,e,node,vx,vy);
    int16_t x=S16((uint16_t)(px(e,node)-W(0x3c)));                       /* 0916C4 */
    if (d0==4u) {
        uint16_t lo=(B(0x15)&1u)?3:5,hi=(B(0x15)&1u)?5:3;
        if (x<=-0x10) d0=lo;
        if (x>=0x10) d0=hi;
    }
    if (d0==0xcu) {
        uint16_t lo=(B(0x15)&1u)?0xb:0xd,hi=(B(0x15)&1u)?0xd:0xb;
        if (x<=-0x10) d0=lo;
        if (x>=0x10) d0=hi;
    }
    if (S16(py(e,node))<S16(W(0x40))) d0=(0x10u-d0)&0xfu;
    SB(0x8f,d0);
}
/* 09153A: one step of +91 towards +8F (091578: 0 or 8 when it is level with a target more than 30 away); carry
 * when there. */
static bool su_turn(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t d0=B(0x8f);
    if (B(0x92)==1u) {
        uint32_t node=L(0x76);
        uint16_t d1=(uint16_t)(px(e,node)-W(0x3c));
        if (d1&0x8000u) d1=(uint16_t)(0u-d1);
        if (S16(d1)>0x30) {
            int16_t dy=S16((uint16_t)(py(e,node)-W(0x40)));
            if (dy>=-8 && dy<=0x28) {
                int16_t dx=S16((uint16_t)(px(e,node)-W(0x3c)));
                if (B(0x15)&1u) dx=(int16_t)-dx;
                d0=dx<0?8:0;
            }
        }
    }
    d0&=0xfu;
    if (d0==B(0x91)) return true;
    d0=(uint8_t)(d0-B(0x91));
    SB(0x91,B(0x91)+((int8_t)d0>0?1u:0xffu));
    return false;
}
/* 0914BC */
static void su_follow(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *p) {
    SW(0x1e,ga_w(p,0x1e));
    SB(0x90,ga_b(p,0x90));
    int16_t d0=(int8_t)ga_b(p,0x88);
    if (B(0x15)&1u) d0=(int16_t)-d0;
    SW(0x28,d0);
    SW(0x2c,(int16_t)(int8_t)ga_b(p,0x89));
    ga_project(a,e);
    uint8_t n=B(0x91)&0xfu;
    if (!n || n>=8u) SW(0x1e,W(0x1e)-1u);
    uint8_t f=ga_b(p,0x17);                                              /* 0044F6 */
    if (f) {
        if (!(f&0x80u)) f++;
        SB(0x17,f);
    }
}
/* 091B02: the flash 091290 and the shell 0912EE. */
static void su_fire(neo_gactor_t *a,neo_genv_t *e) {
    sound(e,0x1064);
    uint8_t d0=B(0x91);
    if (B(0x15)&1u) d0=rb(e,0x92490u+(d0&0xfu));
    SB(0x7e,d0&0xfu);
    int16_t d2,d3;
    neo_gactor_t *c=child(a,e,0x91290,0x1c);
    su_muzzle(a,e,B(0x91),&d2,&d3);
    if (c) {ga_sb(c,0x91,B(0x91));ga_sb(c,0x88,(uint8_t)d2);ga_sb(c,0x89,(uint8_t)d3);ga_sb(c,0x92,B(0x92));}
    c=child(a,e,0x912ee,0x1c);
    if (!c) return;
    ga_sb(c,0x91,B(0x7e));
    ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+(uint16_t)d2));ga_sw(c,0x2c,(uint16_t)(ga_w(c,0x2c)+(uint16_t)d3));
    ga_sb(c,0x92,B(0x92));
    d0=B(0x91);
    if (B(0x92)==1u && (!d0 || d0==8u)) {                                /* 0915E6 */
        uint32_t d1=0x50000;
        if (!rb(e,0x92490u+(d0&0xfu))) d1=0u-d1;
        ga_sl(c,0x30,d1);ga_sl(c,0x34,0x10000);
    } else {ga_sl(c,0x30,L(0x30));ga_sl(c,0x34,L(0x34));}
}
/* The tank's order changed: the state of table 0924B0, entered at once (jmp (a1)). */
static bool su_order(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *p,uint32_t *next) {
    uint8_t d0=ga_b(p,0x60);
    if (d0==B(0x60)) {SB(0x60,d0);return false;}
    if (B(0x60)==8u) {*next=0x911de;return true;}
    SB(0x60,d0);
    *next=rl(e,0x924b0u+4u*(d0&0xfu));
    return true;
}
/* The tank's slope and the program of its pose: 0, 1 while it tilts (090BBE), -1 on an explicit stop. */
static int su_prog(neo_gactor_t *a,neo_genv_t *e,neo_gactor_t *p,const uint32_t *t) {
    uint8_t d0;
    SB(0x90,ga_b(p,0x90));
    if (st_pose(a,&d0)) return 1;
    SB(0x92,d0);
    return ga_anim_start(a,e,t[d0&3u],0x100)?0:-1;
}
#define SU_ORDER() do { if (su_order(a,e,p,&next)) {SL(0,next);goto again;} if (e->unsupported) return GA_UNSUPPORTED; } while (0)
#define SU_PROG(t) do { int q=su_prog(a,e,p,(t));if (q<0) return GA_UNSUPPORTED;if (q) {SL(0,0x90bbe);goto again;} } while (0)
#define SU_ANIM() do { if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED; } while (0)
static ga_result_t su_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint32_t next=0;uint8_t d0;
    neo_gactor_t *p=NULL;
again:
    if (L(0)<0x911deu && L(0)!=0x90986u && L(0)!=0x90a16u) {
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
    }
    switch (L(0)) {
    case 0x90a62:
        st_palettes(a,e);
        SB(0x60,0);
        SB(0x93,B(0x93)&3u);
        SB(0x8a,rb(e,0x923c0u+(B(0x93)&3u)));
        ga_place(a,e,0,0,NEO_GA_TAG(p->index));                          /* 01AD6E */
        /* fall through */
    case 0x90a8e:
        SB(0x91,8);
        SU_PROG(SU_POSE);
        SL(0,0x90ade);
        /* fall through */
    case 0x90ade:
    case 0x90fc6:
        SU_ORDER();
        su_follow(a,e,p);
        if (st_pose(a,&d0)) {SL(0,0x90bbe);goto again;}
        if (d0!=B(0x92)) {SB(0x92,d0);SU_PROG(SU_POSE);}
        SU_ANIM();
        if (r) SU_PROG(SU_POSE);
        goto hit;
    case 0x90bbe:
        SB(0x91,8);
        START(0x28e28a);
        SL(0,0x90bda);
        /* fall through */
    case 0x90bda:
        SU_ORDER();
        su_follow(a,e,p);
        if (!st_pose(a,&d0)) {SL(0,0x90a8e);goto again;}
        SU_ANIM();
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x90c20:
        SB(0x8c,0);
        SL(0x76,ga_l(p,0x76));
        su_aim(a,e);
        SU_PROG(SU_POSE);
        SL(0,0x90c7c);
        /* fall through */
    case 0x90c7c:
        SB(0x8c,(B(0x8c)+1u)&3u);
        if (!B(0x8c)) {
            if (B(0x96)&2u) su_aim(a,e);                                 /* 091742 */
            ga_sb(p,0x61,su_turn(a,e)?1:0);
        }
        SU_ORDER();
        su_follow(a,e,p);
        if (st_pose(a,&d0)) {SL(0,0x90bbe);goto again;}
        if (d0!=B(0x92)) {SB(0x92,d0);SU_PROG(SU_POSE);}
        SU_ANIM();
        if (r) SU_PROG(SU_POSE);
        goto hit;
    case 0x90d8c:
        SU_PROG(SU_FIRE);
        su_fire(a,e);
        SL(0,0x90dc4);
        /* fall through */
    case 0x90dc4:
        SU_ORDER();
        su_follow(a,e,p);
        SU_ANIM();
        if (r) ga_sb(p,0x61,1);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x90e10:
        SB(0x8c,0);
        SU_PROG(SU_POSE);
        SL(0,0x90e5e);
        /* fall through */
    case 0x90e5e:
        SB(0x8c,(B(0x8c)+1u)&3u);
        if (!B(0x8c)) {
            if (B(0x91)==8u) ga_sb(p,0x61,1);
            else SB(0x91,B(0x91)+((int8_t)B(0x91)>=8?0xffu:1u));
        }
        SU_ORDER();
        su_follow(a,e,p);
        if (st_pose(a,&d0)) {SL(0,0x90bbe);goto again;}
        if (d0!=B(0x92)) {SB(0x92,d0);SU_PROG(SU_POSE);}
        SU_ANIM();
        if (r) SU_PROG(SU_POSE);
        goto hit;
    case 0x90f78:
        SB(0x8c,0);
        SU_PROG(SU_POSE);
        SL(0,0x90fc6);
        goto again;
    case 0x910a6:
        SU_PROG(SU_OFF);
        SL(0,0x910f0);
        /* fall through */
    case 0x910f0:
        SU_ORDER();
        su_follow(a,e,p);
        if (st_pose(a,&d0)) {SL(0,0x90bbe);goto again;}
        if (d0!=B(0x92)) {SB(0x92,d0);SU_PROG(SU_OFF);}
        SW(0x1e,W(0x1e)-1u);
        SU_ANIM();
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    hit:                                                                 /* 09118A */
        if (ga_b(p,0x96)&0x40u) SU_PROG(SU_HIT);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x911de:
        SB(0x17,0);
        SL(0x34,0x66666);SL(0x72,0xffff92c6u);SL(0x30,0xffffbbbcu);
        SW(0x66,0x1001);SW(0x1e,0x1001);
        SL(0,0x9120c);
        /* fall through */
    case 0x9120c:
        ga_air(a,e);                                                     /* 0143E4 */
        SL(0x34,L(0x34)+L(0x72));
        if (B(0x15)&0x40u) SL(0,0x91270);
        if (B(0x5f)&1u) SL(0,0x91270);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (S16(W(0x40))<=0xc0) SL(0,0x90986);
        if (ga_creator_alive(a,e)) SL(0,0x9127a);                        /* 00409A: carry while the tank node lives */
        if (st_gone(a,e)) SL(0,0x90a16);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x91270:
        sound(e,0x1025);
        /* fall through */
    case 0x9127a:
        SW(0x66,0x1001);SW(0x1e,0x1001);SL(0x50,0);
        SL(0,0x1c706);return neo_effects_tick(a,e);                      /* JMP 01C706 */
    case 0x90986:
        return GA_DELETE;                                                /* the cartridge check, then 004188 */
    case 0x90a16:
        return GA_DELETE_TREE;                                           /* 0041AC */
    /* the flash */
    case 0x91290:
        SW(0x66,0x1001);SW(0x1e,0x1001);
        ga_palette(a,0x208);
        SW(0x28,W(0x28)+(uint16_t)(int16_t)(int8_t)B(0x88));
        SW(0x2c,W(0x2c)+(uint16_t)((int16_t)(int8_t)B(0x89)-8));
        START(0x22d264);
        SL(0,0x912d6);
        /* fall through */
    case 0x912d6:
        ga_project(a,e);
        SU_ANIM();
        if (r) SL(0,0x90a16);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* the shell */
    case 0x912ee: {
        SB(0x15,B(0x15)|0x20u);
        ga_project(a,e);
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        int16_t x0=S16(ga_w(p,0x3c)),x1=S16(W(0x3c));
        uint32_t v=L(0x30);
        if (v && ((v&0x80000000u)?x1>x0:x0>x1)) SL(0x30,0u-v);
        ga_palette(a,0x373);
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
        SB(0x15,B(0x15)&0xfeu);
        START(0x2907fa);
        SL(0,0x9137c);}
        /* fall through */
    case 0x9137c:
        SL(0x34,L(0x34)+0xffffc000u);
        ga_air(a,e);                                                     /* 0143E4 */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,0x91cf8)) SL(0,0x9127a);                       /* 0049E8 */
        if (B(0x15)&0x40u) SL(0,0x91270);
        if (B(0x5f)&1u) SL(0,0x91270);
        if (ga_cull_rect(a,e,0x91dd8)) SL(0,0x90a16);                    /* 019504 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool st_routine(uint32_t r) {
    switch (r) {
    case 0x8ff7e: case 0x90042: case 0x900ac: case 0x90124: case 0x901cc: case 0x902e4: case 0x90388: case 0x903c4: case 0x90460:
    case 0x904b0: case 0x9051c: case 0x90538: case 0x90548: case 0x905c2: case 0x9060e: case 0x9067c: case 0x90698: case 0x906da:
    case 0x90772: case 0x907ba: case 0x908e4: case 0x90928: case 0x9094c: case 0x90968: case 0x9097e: return true;
    }
    return false;
}
static bool su_routine(uint32_t r) {
    switch (r) {
    case 0x90a62: case 0x90a8e: case 0x90ade: case 0x90fc6: case 0x90bbe: case 0x90bda: case 0x90c20: case 0x90c7c: case 0x90d8c:
    case 0x90dc4: case 0x90e10: case 0x90e5e: case 0x90f78: case 0x910a6: case 0x910f0: case 0x911de: case 0x9120c: case 0x91270:
    case 0x9127a: case 0x90986: case 0x90a16: case 0x91290: case 0x912d6: case 0x912ee: case 0x9137c: return true;
    }
    return false;
}
static bool tank_routine(uint32_t r) {
    return r==0x8dcb2u || r==0x8dd34u || r==0x8dda2u || r==0x8dd6eu || r==0x8ddd4u || r==0x8ddfau || r==0x8de46u || r==0x8de84u || r==0x8dec2u ||
           r==0x8dedeu || r==0x8df16u || r==0x8df42u || r==0x8df76u || r==0x8df92u || r==0x8dfa6u || r==0x8e000u || r==0x8e034u || r==0x8e06eu ||
           r==0x8e08au || r==0x8e0d4u || r==0x8e104u || r==0x8e128u || r==0x8e144u || r==0x8ef3eu;
}
/* 08EEBC is entered from the turret's own update (never a node's routine at its dispatch here); as a label it is also
 * where the Mission 1 boss's turret jumps (neo_boss.c, convert), which the world keeps as it was. */
static bool turret_routine(uint32_t r) {
    return r==0x8eb18u || r==0x8eb3cu || r==0x8eb58u || r==0x8eb8eu || r==0x8ebb6u || r==0x8ec3au || r==0x8ec54u || r==0x8eccau || r==0x8ecf4u ||
           r==0x8ed22u || r==0x8ed3cu || r==0x8edacu || r==0x8edc6u || r==0x8edfcu || r==0x8ee2cu || r==0x8ee62u || r==0x8ee78u ||
           r==0x8eeeau || r==0x8ef46u;
}
bool neo_m5enemy_routine(uint32_t r) {return tank_routine(r) || turret_routine(r) || st_routine(r) || su_routine(r);}
ga_result_t neo_m5enemy_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (st_routine(r)) return st_tick(a,e);
    if (su_routine(r)) return su_tick(a,e);
    return tank_routine(r)?tank_tick(a,e):turret_tick(a,e);
}
