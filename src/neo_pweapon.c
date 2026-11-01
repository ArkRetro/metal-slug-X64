#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4), neo_grid_splat (00505A) */
/* The player's weapons other than the pistol, the H, the C and the R
 * (those are neo_bullet.c / neo_chaser.c): manual translations of their
 * projectiles; ROM labels are provenance only. The player's fire callback
 * 0C4372 (gameplay.c) makes, after the list head (00411C), the flash of
 * table 0C432A (0C58D2, neo_chaser.c) and the projectile of table 0C42AA
 * with +6E = the 0C4244 angle, +70 = player, +5E the player's, position and
 * velocity of 0C482E.
 *
 * Weapon 0D (table 0C42AA: 08606A, sound 115A, cap 2, 30 rounds; the
 * player fires it with the C's programs, tables 0C3C10/0C3C50/0C3C90):
 * Ball 08606A: sound 116A, layer E000, palette 822, +61 = 3 (no attack for
 * three updates), 03C526 (water ripple 1). Level (+6E & 7F = 0): program
 * 282C18, +34 = 0, speed 0x1C000; up: 282BF0, +34 = 0x38000, 0x10000; down
 * (+6E bit 7): +75 = FF and 086172. The speed, negated facing left (+15
 * bit 0), is added to the inherited +30; with +30 nonzero, +73 = FF when
 * +30 has the facing's sign (spl). 08613A: +34 -= 1.0, 0143E4; +76 = D6
 * bit 9; 00427A; landed (+15 bit 6): +73 = 0 rolls (0861C6), else bursts
 * (086216). 086172 (down): +15 bit 5, program 282C22, +30 = 0, +34 = -6.0;
 * 08619E: 0143DC, +76, 00427A, landed: 086304.
 * Roll 0861C6: program 282C2C, +62 = +30; 0861E2: 0865F4 (on the floor
 * 01456A, else 08660E), +76; off the floor: 086124 (282C18, 08613A);
 * 08661C slope pose +72 by D7; program end: 086216.
 * Burst 086216: sound 1168, program 282DCC, +62 = 0, the glow 08651C
 * (0040BC + 01B220); 086260: +62 += (+-0x100000 - +62) / 2 by the facing,
 * 0865F4, +76, 08661C, 00427A; +5F bit 0: 086304.
 * Common tail 0862AE: after +61 updates, the contact row E4E44 (0049E8);
 * a hit or +76: 086304; out of E4E2C (019504): 0C0DFC and delete; once
 * (+74), 03C5CC (water: the splash 03C962, explicit stop); 00505A.
 * Explosion 086304: 0C0DFC id 0D; the down ball (+75) flips y and takes
 * 28311A; else 28311A when the velocity angle (002EB4, +0x10, >> 5) is 2
 * (up), 2830DA otherwise; 08637A: 01ABC8, 00427A until its end. The
 * programs CALL 0866A6 three times (payload dx, dy, program, row): the
 * blast 08642E after the list head, +5E copied, at the offset (mirrored by
 * +15), 01AC94, the program; 08642E: palette 828, layer 1001; 086450:
 * 01ABC8, 0049E8 with +6E once; 086466: 01ABC8, 00427A until its end or
 * out of E4E3C.
 * Glow 08651C: program 282EE6 (SWITCH +72), palette 204, +72 of the ball;
 * 08654A: the ball's layer +1E, 01ABC8, 00427A until its end.
 *
 * Weapons 02 / 07 (table 0C42AA: 0644C4 / 0646B0, sounds 112F, caps 1,
 * 30 / 15 rounds; programs of the tables 0C3C10.. at 0BFFD8/0C0018/0C0058):
 * one blast, +72 = 0 / 1 (2 / 3: the +93 = 2 constructors 0646C8 / 0646E0
 * of table 0C42EA, palette 86C, not fired by the port), palette 39B, sound
 * 10F6, layer 1110 (mission 9: 012022, explicit stop), program E13F2[+72]
 * (level) or E1402[+72]; 064556: 01ABC8, the row E1412 / E1422 / E1432
 * (down: +15 bit 1 toggled) [+72] once (0049E8), the white screen (009BB0
 * FF) for one update (0645D2: 0); 0645E0: the velocity decays by 1/8,
 * 01AC38, 00427A; at its end 0C0DFC with the id E1442[+72] and delete.
 *
 * Weapons 04 / 09 (table 0C42AA: 0A71F8 / 0A716E, sounds 112C, caps 1,
 * 30 rounds; programs of 0C00D8/0C0118/0C0158): the flame. 0A71F8: palette
 * 384, program 2A655A, +7A = 0; 0A716E: 876, 2A696E, +7A = FF; +7B = 0
 * (FF in the +93 = 2 constructors 0A7226 / 0A719C of table 0C42EA, not
 * fired by the port). 0A727E: sound 10F3, layer 1110, +15 bit 5, velocity
 * polar(0xC0000, +6E), +78/+79 = 0, +72 = 12 updates, 01ABC8. 0A72CE: +74 =
 * +34, 0143E4, row 0A7146 (0049E8); in the air +30 loses 1/8 (1/16 with
 * +7B), on the floor +34 = +74 and +30 tends to +-0xC0000 by 1/8 (+6E = 0 /
 * 0x80 by the facing); +34 tends to 0xC0000 - |+30| by 1/16 (with +7B: the
 * floor clears +15 bit 1, +5F bit 0 skips it); out of 0A7166 (019504) or
 * +72 run out: 0A73B4; 00427A; +78 -= 0xA0, a borrow makes a child
 * (0A748E: the first 0A74E4 with 0040BC reserve 0xE, then the puffs 0A751C
 * reserve 0x2A; +7A, +7B, +6E, +72 copied, 01ACF2 (0, 0), 01AC94).
 * 0A73B4: +5F bit 7, 0C0DFC (id 9 with +7A, else 4); 0A73DA: the velocity
 * loses 1/16, 0143E4, no drawing, deleted once no deeper node follows
 * (00408C). 0A74E4: program 2A6574 / 2A676C (vertical) at 0x300;
 * 0A751C: a random flip (0029C6 < 0), program 2A6666 / 2A6988 (+7A, 0x380)
 * or 2A6868 / 2A6A8E (vertical); both 0A7596: palette 384 / 876 (+7A),
 * layer 1110 (110F with +7B), +7C/+80 = -polar(0x100000, the angle of the
 * velocity words) (0 when both are 0), the velocity halves plus two random
 * words * 2, 01ABC8; 0A7654: towards the node before it (+04) plus +7C/+80
 * (1/16 of the distance, 1/16 of the difference), 01AC38, every fourth
 * update the row 0A7156, out of 0A7166 delete, 00427A until its end;
 * 0A7756: the program speed +26 tends to 0x80 (0x180 once the flame has
 * +5F bit 7) by 1/8.
 *
 * The paths that would run the cartridge check (2FFFE0) end in 004188 on
 * genuine hardware: delete. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
/* 0C0DFC with the projectile's player (+70) and the weapon id. */
static bool release(neo_gactor_t *a,neo_genv_t *e,uint8_t id) {
    if (W(0x70)>=2u) {ga_fail(e,0xc0e04);return false;}           /* trap #15 */
    if (!e->pickup || !e->pickup->weapons) {ga_fail(e,0xc0dfc);return false;}
    marco_ammo_t m=marco_weapons_ammo(e->pickup->weapons,W(0x70));
    marco_ammo_release(&m,id);marco_weapons_set_ammo(e->pickup->weapons,W(0x70),&m);
    return true;
}
/* 03C5CC: water (class 5) at y-1, y+7, y+15 (03C55C, 013CF8): the splash
 * 03C962 is not translated (explicit stop). Carry: water. */
static bool water(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned i=0;i<3;i++)
        if (neo_kernel_point_class(e->kernel,(int32_t)L(0x3c),(int32_t)(L(0x40)-0x10000u+0x80000u*i),5,0,NULL,NULL,NULL)) {ga_fail(e,0x3c962);return true;}
    return false;
}
/* 0866A6: a blast 08642E of the explosion programs 2830DA / 28311A. */
static bool dblast(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy,uint32_t program,uint32_t row) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8642e,true):NULL;
    if (!c) return true;
    ga_sb(c,0x5e,B(0x5e));ga_sl(c,0x6e,row);
    if (B(0x15)&1u) dx=(int16_t)-dx;
    if (B(0x15)&2u) dy=(int16_t)-dy;
    ga_place_at(c,a,e,dx,dy);
    ga_sb(c,0x15,(uint8_t)(B(0x15)&3u));                               /* 01AC94 */
    return ga_anim_start(c,e,program,0x100);                           /* 004260 */
}
/* The CALLs 0866A6 of 2830DA (dx 0x10/0x20/0x30, dy 0) and 28311A (dx 0,
 * dy -8/8/0x18): programs 22CDD4 / 22CE6C / 22CF2C, rows E4E54 / E4E64 /
 * E4E74. The converted CALL keeps only dx: the vertical calls are told
 * apart by their order. */
static bool dball_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    if (kind!=NEO_ANIM_CALL || routine!=0x866a6u || (a->program_tag!=0x2830dau && a->program_tag!=0x28311au)) return false;
    static const uint32_t prog[3]={0x22cdd4,0x22ce6c,0x22cf2c},row[3]={0xe4e54,0xe4e64,0xe4e74};
    static const int16_t vdy[3]={-8,8,0x18};
    bool level=a->program_tag==0x2830dau;
    unsigned i;
    if (level) {
        if (parameter!=0x10u && parameter!=0x20u && parameter!=0x30u) {ga_fail(e,0x866a6);return true;}
        i=parameter/0x10u-1u;
    } else {
        static struct {const neo_gactor_t *a;unsigned n;} seen[8];
        unsigned k,free=8;
        if (parameter) {ga_fail(e,0x866a6);return true;}
        for (k=0;k<8 && seen[k].a!=a;k++) if (!seen[k].a && free==8) free=k;
        if (k==8) {if (free==8) {ga_fail(e,0x866a6);return true;} k=free;seen[k].a=a;seen[k].n=0;}
        i=seen[k].n++;
        if (seen[k].n>=3u) seen[k].a=NULL;
    }
    return dblast(a,e,level?(int16_t)(0x10*(i+1u)):0,level?0:vdy[i],prog[i],row[i]);
}
/* 08661C: the roll pose +72 by the floor's angle word (D7). */
static void slope(neo_gactor_t *a,uint16_t d7) {
    uint8_t d0=2;
    if (B(0x15)&0x40u) {
        if ((int16_t)d7<=0x1000) d0=2;
        else if (d7<=0x2b00u) d0=0;
        else if (d7<=0x3b00u) d0=1;
        else if (d7<=0x4500u) d0=2;
        else if (d7<=0x5500u) d0=3;
        else if (d7<=0x7000u) d0=4;
        else d0=2;
        if (B(0x15)&1u) d0=(uint8_t)(4u-d0);
    }
    SB(0x72,d0);
}
/* 0865F4: on the floor 01456A, else 08660E (+34 -= 1.0, 0143E4). D6 and
 * the D7 word out. */
static uint16_t step(neo_gactor_t *a,neo_genv_t *e,uint16_t *d7) {
    uint32_t kind=0;uint16_t d6;
    if (B(0x15)&0x40u) d6=ga_ground(a,e,&kind);
    else {SL(0x34,L(0x34)+0xffff0000u);d6=ga_air_kind(a,e,&kind);}
    *d7=(uint16_t)kind;return d6;
}
/* 0862AE: 0 = go on, 1 = 086304 now, -1 = delete (released). */
static int tail(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x61)) {
        if (ga_attack(a,e,0xe4e44)) return 1;
    } else SB(0x61,B(0x61)-1u);
    if (B(0x76)) return 1;
    if (ga_cull_rect(a,e,0xe4e2c)) return release(a,e,0x0d)?-1:-2;
    if (!B(0x74)) SB(0x74,water(a,e)?0xffu:0u);
    if (e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));              /* 00505A */
    return 0;
}
/* (A plain block: `continue` re-dispatches 086304 in the same update.) */
#define TAIL_OR(next) { int t_=tail(a,e); \
    if (e->unsupported || t_==-2) return GA_UNSUPPORTED; \
    if (t_<0) return GA_DELETE; \
    if (t_>0) {SL(0,0x86304);continue;} \
    return GA_MORE; }
static ga_result_t pweapon_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (int guard=0;guard<4;guard++) switch (L(0)) {
    /* ---- weapon 0D: the ball 08606A ---- */
    case 0x8606a: {
        sound(e,0x116a);SW(0x66,0xe000);SW(0x1e,0xe000);ga_palette(a,0x822);
        SB(0x61,3);
        if (!neo_gate_ripple(a,e,1)) return GA_UNSUPPORTED;            /* 03C526 */
        uint32_t d0;
        if (!(B(0x6e)&0x7fu)) {START(0x282c18);SL(0x34,0);d0=0x1c000;}
        else if (!(B(0x6e)&0x80u)) {START(0x282bf0);SL(0x34,0x38000);d0=0x10000;}
        else {
            SB(0x75,0xff);
            SB(0x15,B(0x15)|0x20u);START(0x282c22);SL(0x30,0);SL(0x34,0xfffa0000u);SL(0,0x8619e);
            continue;
        }
        SB(0x75,0);
        uint32_t d1=0;
        if (B(0x15)&1u) {d0=0u-d0;d1=0xffffffffu;}
        uint32_t d2=L(0x30);
        if (d2) {d1^=d2;SB(0x73,(d1&0x80000000u)?0u:0xffu);d0+=d2;}
        SL(0x30,d0);SL(0,0x8613a);
        continue;
    }
    case 0x86124:
        START(0x282c18);SL(0,0x8613a);
        /* fall through */
    case 0x8613a: {
        SL(0x34,L(0x34)+0xffff0000u);                                   /* 08660E */
        uint16_t d6=ga_air(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        SB(0x76,(d6&0x200u)?0xffu:0u);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (B(0x15)&0x40u) SL(0,B(0x73)?0x86216u:0x861c6u);
        TAIL_OR(0)
    }
    case 0x8619e: {
        SL(0x34,L(0x34)+0xffffb000u);                                   /* 0143DC */
        uint16_t d6=ga_air(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        SB(0x76,(d6&0x200u)?0xffu:0u);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (B(0x15)&0x40u) SL(0,0x86304);
        TAIL_OR(0)
    }
    case 0x861c6:
        START(0x282c2c);SL(0x62,L(0x30));SL(0,0x861e2);
        /* fall through */
    case 0x861e2: {
        uint16_t d7;uint16_t d6=step(a,e,&d7);
        if (e->unsupported) return GA_UNSUPPORTED;
        SB(0x76,(d6&0x200u)?0xffu:0u);
        if (!(B(0x15)&0x40u)) SL(0,0x86124);
        slope(a,d7);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x86216);
        TAIL_OR(0)
    }
    case 0x86216:
        sound(e,0x1168);START(0x282dcc);SL(0x62,0);
        ga_spawn_from(a,e,0x8651c,0,0);                                 /* 0040BC, 01B220 */
        SL(0,0x86260);
        /* fall through */
    case 0x86260: {
        uint32_t d0=(B(0x15)&1u)?0xfff00000u:0x100000u;
        d0-=L(0x62);SL(0x62,L(0x62)+(uint32_t)((int32_t)d0>>1));
        uint16_t d7;uint16_t d6=step(a,e,&d7);
        if (e->unsupported) return GA_UNSUPPORTED;
        SB(0x76,(d6&0x200u)?0xffu:0u);
        slope(a,d7);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (B(0x5f)&1u) SL(0,0x86304);
        TAIL_OR(0)
    }
    case 0x86304: {
        if (!release(a,e,0x0d)) return GA_UNSUPPORTED;
        if (B(0x75)) {SB(0x15,B(0x15)|2u);START(0x28311a);}
        else {
            uint8_t d0=(uint8_t)(neo_angle((int16_t)W(0x30),(int16_t)W(0x34))+0x10u);
            START((d0>>5)==2u?0x28311a:0x2830da);
        }
        SL(0,0x8637a);
    }   /* fall through */
    case 0x8637a: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,dball_call);
        if (r<0 || e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    case 0x8642e:
        ga_palette(a,0x828);SW(0x66,0x1001);SW(0x1e,0x1001);SL(0,0x86450);
        return GA_MORE;
    case 0x86450:
        ga_project(a,e);ga_attack(a,e,L(0x6e));
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x86466);
        /* fall through */
    case 0x86466: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return ga_cull_rect(a,e,0xe4e3c)?GA_DELETE:GA_MORE;
    }
    case 0x8651c: {
        START(0x282ee6);ga_palette(a,0x204);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x8651c);return GA_UNSUPPORTED;}
        SB(0x72,ga_b(p,0x72));SL(0,0x8654a);
    }   /* fall through */
    case 0x8654a: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x8654a);return GA_UNSUPPORTED;}
        SW(0x1e,ga_w(p,0x1e));
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    /* ---- weapons 02 / 07: the blast 0644C4 / 0646B0 ---- */
    case 0x644c4: case 0x646b0: {
        SB(0x72,L(0)==0x646b0u?1u:0u);ga_palette(a,0x39b);
        sound(e,0x10f6);SW(0x66,0x1110);SW(0x1e,0x1110);
        if (!e->pickup) {ga_fail(e,0x644ee);return GA_UNSUPPORTED;}
        if (e->pickup->mission==9u) {ga_fail(e,0x12022);return GA_UNSUPPORTED;}
        static const uint32_t level[4]={0x2686dc,0x2687e4,0x2688ec,0x2689f4},vertical[4]={0x268760,0x268868,0x268970,0x268a78};
        START(((B(0x6e)&0x7fu)?vertical:level)[B(0x72)&3u]);
        SL(0,0x64556);
        return GA_MORE;
    }
    case 0x64556: {
        ga_project(a,e);
        uint32_t row;
        if (!(B(0x6e)&0x7fu)) row=0xe1412;
        else if (!(B(0x6e)&0x80u)) row=0xe1422;
        else {SB(0x15,B(0x15)^2u);row=0xe1432;}
        const uint8_t *t=ga_rom(row+4u*(B(0x72)&3u));
        if (!t) {ga_fail(e,row);return GA_UNSUPPORTED;}
        ga_attack(a,e,(uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3]);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (e->flags) e->flags->flash=0xff;                            /* 009BB0 */
        SL(0,0x645d2);
        goto decay;
    }
    case 0x645d2:
        if (e->flags) e->flags->flash=0;                               /* 009BB0 */
        SL(0,0x645e0);
        /* fall through */
    case 0x645e0:
    decay: {
        SL(0x30,L(0x30)-(uint32_t)((int32_t)L(0x30)>>3));
        SL(0x34,L(0x34)-(uint32_t)((int32_t)L(0x34)>>3));
        ga_move(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (!r) return GA_MORE;
        const uint8_t *id=ga_rom(0xe1442);
        if (!id) {ga_fail(e,0xe1442);return GA_UNSUPPORTED;}
        return release(a,e,id[B(0x72)&3u])?GA_DELETE:GA_UNSUPPORTED;
    }
    /* ---- weapons 04 / 09: the flame 0A71F8 / 0A716E ---- */
    case 0xa71f8: case 0xa716e: {
        bool nine=L(0)==0xa716eu;
        ga_palette(a,nine?0x876:0x384);START(nine?0x2a696e:0x2a655a);
        SB(0x7a,nine?0xffu:0u);SB(0x7b,0);
        sound(e,0x10f3);SW(0x66,0x1110);SW(0x1e,0x1110);SB(0x15,B(0x15)|0x20u);
        uint32_t vx,vy;ga_polar(0xc0000,B(0x6e),&vx,&vy);SL(0x30,vx);SL(0x34,vy);
        SB(0x78,0);SB(0x79,0);SW(0x72,12);
        ga_project(a,e);SL(0,0xa72ce);
        return GA_MORE;
    }
    case 0xa72ce: {
        SL(0x74,L(0x34));
        ga_air(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        ga_attack(a,e,0xa7146);
        if (e->unsupported) return GA_UNSUPPORTED;
        bool floor=B(0x15)&0x40u,lift=true;
        if (!floor) {
            uint32_t d0=(uint32_t)((int32_t)L(0x30)>>(B(0x7b)?4:3));SL(0x30,L(0x30)-d0);
        } else {
            SL(0x34,L(0x74));
            uint32_t d0=0xc0000;uint16_t d2=0;
            if (B(0x15)&1u) {d2=0x80;d0=0u-d0;}
            d0=(uint32_t)((int32_t)(d0-L(0x30))>>3);SL(0x30,L(0x30)+d0);SW(0x6e,d2);
        }
        if (B(0x7b)) {
            if (B(0x15)&0x40u) SB(0x15,B(0x15)&0xfdu);
            else if (B(0x5f)&1u) lift=false;
        }
        if (lift) {
            uint32_t d1=L(0x30);
            if (!(d1&0x80000000u)) d1=0u-d1;
            d1+=0xc0000u;d1-=L(0x34);SL(0x34,L(0x34)+(uint32_t)((int32_t)d1>>4));
        }
        if (ga_cull_rect(a,e,0xa7166)) SL(0,0xa73b4);
        uint16_t t=W(0x72);SW(0x72,t-1u);
        if (!t) SL(0,0xa73b4);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        uint8_t b78=B(0x78);SB(0x78,b78-0xa0u);
        if (b78<0xa0u) {                                                 /* 0A748E */
            bool first=!(B(0x79)&0x80u);SB(0x79,B(0x79)|0x80u);
            neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,first?0xa74e4u:0xa751cu,false):NULL;
            if (c) {
                ga_sb(c,0x7a,B(0x7a));ga_sb(c,0x7b,B(0x7b));ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x72,W(0x72));
                ga_place_at(c,a,e,0,0);ga_sb(c,0x15,(uint8_t)(B(0x15)&3u));   /* 01ACF2, 01AC94 */
            }
        }
        return GA_MORE;
    }
    case 0xa73b4:
        SB(0x5f,B(0x5f)|0x80u);
        if (!release(a,e,B(0x7a)?9u:4u)) return GA_UNSUPPORTED;
        SL(0,0xa73da);
        /* fall through */
    case 0xa73da:
        SL(0x30,L(0x30)-(uint32_t)((int32_t)L(0x30)>>4));
        SL(0x34,L(0x34)-(uint32_t)((int32_t)L(0x34)>>4));
        ga_air(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (!e->has_children) {ga_fail(e,0xa73f4);return GA_UNSUPPORTED;}
        return e->has_children(e->ctx,a)?GA_MORE:GA_DELETE;            /* 00408C */
    case 0xa74e4: case 0xa751c: {
        if (L(0)==0xa74e4u) {
            if (!ga_anim_start(a,e,(B(0x6e)&0x40u)?0x2a676c:0x2a6574,0x300)) return GA_UNSUPPORTED;
        } else {
            if (ga_rand(e)&0x8000u) SB(0x15,B(0x15)^1u);
            bool v=B(0x6e)&0x40u;
            uint32_t prog=v?(B(0x7a)?0x2a6a8e:0x2a6868):(B(0x7a)?0x2a6988:0x2a6666);
            if (!ga_anim_start(a,e,prog,!v && B(0x7a)?0x380:0x300)) return GA_UNSUPPORTED;
        }
        ga_palette(a,B(0x7a)?0x876:0x384);
        SW(0x66,B(0x7b)?0x110f:0x1110);SW(0x1e,B(0x7b)?0x110f:0x1110);
        uint32_t d0=0,d1=0;
        if (W(0x30) || W(0x34)) {
            uint8_t ang=(uint8_t)neo_angle((int16_t)W(0x30),(int16_t)W(0x34));
            ga_polar(0x100000,ang,&d0,&d1);d0=0u-d0;d1=0u-d1;
        }
        SL(0x7c,d0);SL(0x80,d1);
        SL(0x30,(uint32_t)((int32_t)L(0x30)>>1));SL(0x34,(uint32_t)((int32_t)L(0x34)>>1));
        SL(0x30,L(0x30)+((uint32_t)(int32_t)(int16_t)ga_rand(e)<<1));
        SL(0x34,L(0x34)+((uint32_t)(int32_t)(int16_t)ga_rand(e)<<1));
        ga_project(a,e);SL(0,0xa7654);
        return GA_MORE;
    }
    case 0xa7654: {
        neo_gactor_t *p=e->previous?e->previous(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xa7654);return GA_UNSUPPORTED;}
        uint32_t d0=(uint32_t)((int32_t)(ga_l(p,0x3c)+L(0x7c)-L(0x3c))>>4);
        SL(0x30,L(0x30)+(uint32_t)((int32_t)(d0-L(0x30))>>4));
        d0=(uint32_t)((int32_t)(ga_l(p,0x40)+L(0x80)-L(0x40))>>4);
        SL(0x34,L(0x34)+(uint32_t)((int32_t)(d0-L(0x34))>>4));
        ga_move(a,e);
        uint16_t t=(uint16_t)(W(0x72)+1u);SW(0x72,t);
        if (!(t&3u)) {ga_attack(a,e,0xa7156);if (e->unsupported) return GA_UNSUPPORTED;}
        if (ga_cull_rect(a,e,0xa7166)) return GA_DELETE;
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        /* 0A7756: the program speed +26 tends to 0x80 (0x180 once the
         * flame, +0C, has +5F bit 7) by 1/8. */
        neo_gactor_t *f=e->creator?e->creator(e->ctx,a):NULL;
        if (!f) {ga_fail(e,0xa7756);return GA_UNSUPPORTED;}
        uint16_t target=(ga_b(f,0x5f)&0x80u)?0x180:0x80;
        SW(0x26,W(0x26)+(uint16_t)((int16_t)(target-W(0x26))>>3));
        return GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_pweapon_routine(uint32_t r) {
    return r==0x8606a || r==0x86124 || r==0x8613a || r==0x8619e || r==0x861c6 || r==0x861e2 || r==0x86216 ||
        r==0x86260 || r==0x86304 || r==0x8637a || r==0x8642e || r==0x86450 || r==0x86466 || r==0x8651c || r==0x8654a ||
        r==0x644c4 || r==0x646b0 || r==0x64556 || r==0x645d2 || r==0x645e0 ||
        r==0xa71f8 || r==0xa716e || r==0xa72ce || r==0xa73b4 || r==0xa73da || r==0xa74e4 || r==0xa751c || r==0xa7654;
}
/* Attackers of the player's side (their rows may hit the boss). */
bool neo_pweapon_attacker(uint32_t r) {
    return r==0x8613a || r==0x8619e || r==0x861e2 || r==0x86260 || r==0x86450 || r==0x64556 || r==0xa72ce || r==0xa7654;
}
ga_result_t neo_pweapon_tick(neo_gactor_t *a,neo_genv_t *e) {return pweapon_tick(a,e);}
