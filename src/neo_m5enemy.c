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
 * - The flash 08EFFA (palette 208, program 22D264, at +8B / +8C - 8) and the
 *   shell 08F058 (speeds 08FD5C[+88][+8A], palette 373, layers E0EF, program
 *   28C32C; 08F114: 0143E4, +8A = its angle >> 3, the row 08FC46 -> 08EFDE,
 *   outside 08FC74 (019504) -> 08EFD6; landed -> 08F1E0).
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
    c=child(a,e,(B(0x93)&0x20u)?0x8f1a0u:0x8f058u,0x1c);
    if (!c) return;
    ga_sb(c,0x8a,B(0x6e));
    ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+(uint16_t)d1));ga_sw(c,0x2c,(uint16_t)(ga_w(c,0x2c)+(uint16_t)d2));
    ga_sb(c,0x93,0);ga_sb(c,0x94,0);
    if (B(0x94)&1u) ga_sb(c,0x94,(uint8_t)(ga_b(c,0x94)|1u));
    ga_sb(c,0x88,B(0x88));
}
/* 08F1E0 / 08EFDE */
static ga_result_t burst(neo_gactor_t *a,neo_genv_t *e,bool loud) {
    if (loud) sound(e,0x1025);
    SB(0x1d,0);                                                          /* 009F88 */
    SW(0x66,0xe0ed);SW(0x1e,0xe0ed);SL(0x50,0);
    SL(0,0x1c706);
    return neo_effects_tick(a,e);                                        /* jmp 01C706 */
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
/* ---- the flash 08EFFA and the shells 08F058 / 08F1A0 ---------------------------------- */
static ga_result_t shell_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x8effa:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        ga_palette(a,0x208);
        SW(0x28,W(0x28)+(uint16_t)(int16_t)(int8_t)B(0x8b));
        SW(0x2c,W(0x2c)+(uint16_t)((int16_t)(int8_t)B(0x8c)-8));
        START(0x22d264);
        SL(0,0x8f040);
        /* fall through */
    case 0x8f040:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0 || e->unsupported) return GA_UNSUPPORTED;
        if (r) SL(0,0x8efd6);
        return GA_MORE;
    case 0x8efd6:
        return GA_DELETE_TREE;                                           /* 0041AC */
    case 0x8efde:
        return burst(a,e,false);
    case 0x8f1e0:
        return burst(a,e,true);
    case 0x8f058: {
        uint32_t at=rl(e,0x8fd5cu+4u*(B(0x88)&7u))+6u*(B(0x8a)&0xfu);
        int16_t d4=0x20,d0=S16(rw(e,at));
        if (d0<0) d4=-0x20;
        if (B(0x94)&1u) d0=(int16_t)(d0-d4);
        SL(0x30,(uint32_t)(int32_t)d0<<8);
        SL(0x76,(uint32_t)(int32_t)S16(rw(e,at+2u))<<8);
        SL(0x34,(uint32_t)(int32_t)S16(rw(e,at+4u))<<8);
        uint8_t n=B(0x8a)&0xfu;
        if (!n || n>=8u) SB(0x15,B(0x15)|0x20u);
        goto fly;}
    case 0x8f1a0: {
        uint8_t angle=(uint8_t)((B(0x8a)&0xfu)<<4);
        int32_t d1=(int32_t)ga_cosine(angle)*0xc00,d2=(int32_t)ga_sine(angle)*0xc00;
        SL(0x30,(uint32_t)d1);SL(0x34,(uint32_t)d2);
        SL(0x76,(uint32_t)-(d2>>5));
        SB(0x93,B(0x93)|0x20u);}
    fly:
        ga_palette(a,0x373);
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
        SB(0x15,B(0x15)&0xfeu);
        START(0x28c32c);
        SL(0,0x8f114);
        /* fall through */
    case 0x8f114:
        ga_air(a,e);                                                     /* 0143E4 */
        if (B(0x15)&0x40u) SL(0,0x8f1e0);
        if (B(0x5f)&1u) SL(0,0x8f1e0);
        SB(0x8a,(uint16_t)neo_angle((int16_t)(S16(W(0x30))>>4),(int16_t)(S16(W(0x34))>>4))>>3);
        SL(0x34,L(0x34)+L(0x76));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_attack(a,e,(B(0x93)&0x20u)?0x8fc56u:0x8fc46u)) SL(0,0x8efde);   /* 0049E8 */
        if (ga_cull_rect(a,e,0x8fc74)) SL(0,0x8efd6);                    /* 019504 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool tank_routine(uint32_t r) {
    return r==0x8dcb2u || r==0x8dd34u || r==0x8dda2u || r==0x8dd6eu || r==0x8ddd4u || r==0x8ddfau || r==0x8de46u || r==0x8de84u || r==0x8dec2u ||
           r==0x8dedeu || r==0x8df16u || r==0x8df42u || r==0x8df76u || r==0x8df92u || r==0x8dfa6u || r==0x8e000u || r==0x8e034u || r==0x8e06eu ||
           r==0x8e08au || r==0x8e0d4u || r==0x8e104u || r==0x8e128u || r==0x8e144u || r==0x8ef3eu;
}
static bool turret_routine(uint32_t r) {
    return r==0x8eb18u || r==0x8eb3cu || r==0x8eb58u || r==0x8eb8eu || r==0x8ebb6u || r==0x8ec3au || r==0x8ec54u || r==0x8eccau || r==0x8ecf4u ||
           r==0x8ed22u || r==0x8ed3cu || r==0x8edacu || r==0x8edc6u || r==0x8edfcu || r==0x8ee2cu || r==0x8ee62u || r==0x8ee78u || r==0x8eebcu ||
           r==0x8eeeau || r==0x8ef46u;
}
static bool shell_routine(uint32_t r) {return r==0x8effau || r==0x8f040u || r==0x8efd6u || r==0x8efdeu || r==0x8f1e0u || r==0x8f058u || r==0x8f1a0u || r==0x8f114u;}
bool neo_m5enemy_routine(uint32_t r) {return tank_routine(r) || turret_routine(r) || shell_routine(r);}
ga_result_t neo_m5enemy_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (tank_routine(r)) return tank_tick(a,e);
    if (turret_routine(r)) return turret_tick(a,e);
    return shell_tick(a,e);
}
