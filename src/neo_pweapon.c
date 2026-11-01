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
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_pweapon_routine(uint32_t r) {
    return r==0x8606a || r==0x86124 || r==0x8613a || r==0x8619e || r==0x861c6 || r==0x861e2 || r==0x86216 ||
        r==0x86260 || r==0x86304 || r==0x8637a || r==0x8642e || r==0x86450 || r==0x86466 || r==0x8651c || r==0x8654a;
}
/* Attackers of the player's side (their rows may hit the boss). */
bool neo_pweapon_attacker(uint32_t r) {
    return r==0x8613a || r==0x8619e || r==0x861e2 || r==0x86260 || r==0x86450;
}
ga_result_t neo_pweapon_tick(neo_gactor_t *a,neo_genv_t *e) {return pweapon_tick(a,e);}
