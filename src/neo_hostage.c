#include "neo_region.h"
/* Mission 2 stage 2: the soldier holding a hostage and the miners, manual
 * translation of ROM 06835C and of 021A1C..022258 (ROM labels as provenance
 * only).
 * - 06835C (script): +8A = 2D27E2 (0260C8) << 8; the holder 0683FA
 *   (0040BC, neo_rebel3.c) with +69, +8A, +86 = this node, +6E, +70 = +72,
 *   +74, +76, +7A = +78, placed 0x20 px ahead (01ACF2); here +72 = +70,
 *   +74 = FFFF, +62 = +8A, +7E = +69, 019DFC (+69 = FF) and 021B46.
 * - 021B46: layer 8820, palette 0x1F, hurtbox E005E, floor 0146E8, facing
 *   by +6E (+62 negated with it), program 230984, +81 = 2, +4E = 1,
 *   +80 = 0. 021BB2: 01456A, 00427A; +80 = 1 (the holder died, 068F4A)
 *   -> 021E9A; 004C24 (carry: +5A = 0x10 -> 021C0A, not observed; else
 *   +4E = 1; +5F bit 7 and +5A cleared).
 * - 021E9A: program 230654 (01436A, 00427A) then 021ECC: program 23066A,
 *   facing flipped, +62 = +72 << 8 by facing; 021F02 walks (02223C:
 *   01456A on the floor, else 0143DC) until +5F bit 0 (-> 021F22, not
 *   observed).
 * - 0220BC (every state's tail): 01945C with E004E: +60 = FF and 004188
 *   (after the cartridge check, not reproduced).
 * The miners (script records of section 2, uncounted): 021A1C (+81 = 0)
 * and 021A26 (+81 = 1; words +70/+72/+74 move to +72/+74/+76). 021A3E:
 * 009F88, layer 8820, palette 0x1F, +61 = 0, facing by +6E, floor 0146E8,
 * one of the digs 23058A/2305CC/230608 by 0029C6 & 3 (0, 1, else).
 * - 021AE6 (dig, 021AD0 restarts 23058A): 01436A, 01AD8C, 00427A; with
 *   +81 = 1 a player inside E0056 (022178: 01AFFC) startles it (021E24);
 *   with +81 = 0 a camera progress (10E99E) at or past +70 (signed, +70 nonzero)
 *   sends it away (021F4E). The dig's callback 022162 (0029C6 & 3 = 0)
 *   switches to the pause 021D98 (2308CE): the same tests before 00427A,
 *   at its end 021AD0 again (+61 = 0) or away (+61 set).
 * - 021E24: 2306EE, then 021E5C: +61 = FF, 23072C (callback 0221C2: the
 *   item +74 (life +76) by 08CDB2 0x18 px ahead, +12/+13 = 4, +34 =
 *   0x90000, and the sparkle 02206E there), then the pause 021D98.
 * - 021F4E: 2306AC, +62 = +72 << 8 (facing right when positive), 021F84
 *   walks with 01456A.
 * - 022196 (dig callback): the dust 021F96 (layer 8820, palette 0x204,
 *   22E504) 0x18 px ahead; 02206E: layer 1001, palette 0xAD, 22DF54. Both
 *   follow the root (01ABC8, 01AD8C) and leave at the end of their program
 *   (004188 after the cartridge check, not reproduced). */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,n) ((B(o)>>(n))&1u)
#define S16(v) ((int16_t)(uint16_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { r=ga_anim_step(a,e,NULL); if (r<0) return GA_UNSUPPORTED; } while (0)
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
static void walker(neo_gactor_t *a,neo_genv_t *e) {                    /* 02223C */
    if (BIT(0x15,6)) ga_ground(a,e,NULL); else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {               /* 0220BC */
    if (ga_cull(a,e,0xe004e)) {SB(0x60,0xff);return GA_DELETE;}
    return GA_MORE;
}
/* 01AD8C: +1E follows +66 - y when +66 is 8xxx. */
static void depth(neo_gactor_t *a) {
    uint16_t d0=W(0x66);
    if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));
}
/* 0x18 px ahead (022196/0221C2: D0 = E8 when +15 bit 0 is clear). */
static int16_t ahead(const neo_gactor_t *a) {return BIT(0x15,0)?0x18:-0x18;}
static bool miner_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x22162u:                                                      /* 022162 */
        if (!(ga_rand(e)&3u)) SL(0,0x21d98);
        return true;
    case 0x22196u: {                                                    /* 022196: 0040BC, 01ACF2 */
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x21f96,false):NULL;
        if (c) ga_place_at(c,a,e,ahead(a),0);
        return true;
    }
    case 0x221c2u: {                                                    /* 0221C2 */
        int16_t d0=ahead(a);uint16_t kind=W(0x74),life=W(0x76);
        if (kind<0x5au) {                                               /* 08CDB2: after the script actor */
            neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
            if (c) {
                ga_sb(c,0x8b,0);ga_sw(c,0x70,life);ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
                ga_place_at(c,a,e,d0,0);ga_sb(c,0x12,4);ga_sb(c,0x13,4);ga_sl(c,0x34,0x90000);
            }
        }
        neo_gactor_t *s2=e->spawn?e->spawn(e->ctx,a,0x2206e,false):NULL;
        if (s2) ga_place_at(s2,a,e,d0,0);
        return true;
    }
    }
    return false;
}
/* 022178: carry when a player is inside E0056 (01AFFC: carry when none). */
static bool startled(neo_gactor_t *a,neo_genv_t *e) {return !ga_players_in(a,e,0xe0056);}
/* 021B14/021DE6: +70 nonzero and the camera progress at or past it. */
static bool leave(neo_gactor_t *a,neo_genv_t *e) {return W(0x70) && S16(W(0x70))<=S16(e->camera);}
#define MSTEP(r) do { r=ga_anim_step(a,e,miner_call); if (r<0) return GA_UNSUPPORTED; } while (0)
static ga_result_t miner(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    for (;;) switch (L(0)) {
    case 0x21a26:
        SB(0x81,1);SW(0x76,W(0x74));SW(0x74,W(0x72));SW(0x72,W(0x70));
        /* fall through */
    case 0x21a1c:
        if (L(0)==0x21a1cu) SB(0x81,0);
        SB(0x1d,0);SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0x1f);SB(0x61,0);  /* 021A3E */
        if (W(0x6e)) SB(0x15,B(0x15)|1u);
        ga_floor(a,e);
        r=ga_rand(e)&3;
        START(r==0?0x23058a:r==1?0x2305cc:0x230608);
        SL(0,0x21ae6);return GA_MORE;
    case 0x21ad0:
        START(0x23058a);SL(0,0x21ae6);
        /* fall through */
    case 0x21ae6:
        ga_stationary(a,e);depth(a);MSTEP(r);
        if (B(0x81)==1u) {if (startled(a,e)) SL(0,0x21e24);}
        else if (leave(a,e)) SL(0,0x21f4e);
        return tail(a,e);
    case 0x21d98:
        START(0x2308ce);SL(0,0x21dae);
        /* fall through */
    case 0x21dae:
        ga_stationary(a,e);depth(a);
        if (B(0x81)==1u) {if (!B(0x61) && startled(a,e)) SL(0,0x21e24);}
        else if (!B(0x81) && leave(a,e)) SL(0,0x21f4e);
        MSTEP(r);
        if (r) SL(0,B(0x61)?0x21f4e:0x21ad0);
        return tail(a,e);
    case 0x21e24:
        START(0x2306ee);SL(0,0x21e3a);
        /* fall through */
    case 0x21e3a:
        ga_stationary(a,e);depth(a);MSTEP(r);
        if (r) SL(0,0x21e5c);
        return tail(a,e);
    case 0x21e5c:
        SB(0x61,0xff);START(0x23072c);SL(0,0x21e78);
        /* fall through */
    case 0x21e78:
        ga_stationary(a,e);depth(a);MSTEP(r);
        if (r) SL(0,0x21d98);
        return tail(a,e);
    case 0x21f4e: {
        START(0x2306ac);
        uint32_t v=(uint32_t)(int32_t)(int16_t)W(0x72)<<8;
        SL(0x62,v);
        if (v&0x80000000u) SB(0x15,B(0x15)&0xfeu); else SB(0x15,B(0x15)|1u);
        SL(0,0x21f84);
    }   /* fall through */
    case 0x21f84:
        ga_ground(a,e,NULL);MSTEP(r);
        return tail(a,e);
    case 0x21f96:                                                       /* the dust */
        SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0x204);START(0x22e504);SL(0,0x21fc8);
        return GA_MORE;
    case 0x21fc8:
        ga_project(a,e);depth(a);MSTEP(r);
        return r?GA_DELETE:GA_MORE;
    case 0x2206e:                                                       /* the sparkle */
        SW(0x66,0x1001);SW(0x1e,0x1001);ga_palette(a,0xad);START(0x22df54);SL(0,0x2209e);
        /* fall through */
    case 0x2209e:
        ga_project(a,e);depth(a);MSTEP(r);
        return r?GA_DELETE:GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static const uint32_t MINER[]={0x21a1c,0x21a26,0x21ad0,0x21ae6,0x21d98,0x21dae,0x21e24,0x21e3a,0x21e5c,0x21e78,0x21f4e,0x21f84,
    0x21f96,0x21fc8,0x2206e,0x2209e};
static ga_result_t hostage(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    for (unsigned i=0;i<sizeof(MINER)/sizeof(MINER[0]);i++) if (MINER[i]==L(0)) return miner(a,e);
    switch (L(0)) {
    case 0x6835c: {
        const uint8_t *t=ga_rom(0x2d27e2);
        if (!t) STOP(0x2d27e2);
        SL(0x8a,(uint32_t)ga_rank_word(a,e,t)<<8);
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x683fa,false):NULL;
        if (c) {
            ga_sb(c,0x69,B(0x69));ga_sl(c,0x8a,L(0x8a));ga_sl(c,0x86,NEO_GA_TAG((unsigned)a->index));
            ga_sw(c,0x6e,W(0x6e));ga_sw(c,0x70,W(0x72));
        }
        SW(0x72,W(0x70));
        if (c) ga_sw(c,0x74,W(0x74));
        SW(0x74,0xffff);
        if (c) {ga_sw(c,0x76,W(0x76));ga_sw(c,0x7a,W(0x78));ga_place_at(c,a,e,W(0x6e)?-0x20:0x20,0);}
        SL(0x62,L(0x8a));SW(0x7e,B(0x69));SB(0x69,0xff);              /* 019DFC */
        SB(0x1d,0);SL(0x50,0xe005e);SW(0x66,0x8820);SW(0x1e,0x8820);   /* 021B46 */
        ga_palette(a,0x1f);ga_floor(a,e);SB(0x61,0);
        if (!W(0x6e)) {SB(0x15,B(0x15)|1u);SL(0x62,0u-L(0x62));}
        START(0x230984);SB(0x81,2);SW(0x4e,1);SB(0x80,0);SL(0,0x21bb2);
        return GA_MORE;
    }
    case 0x21bb2:
        ga_ground(a,e,NULL);STEP(r);
        if (B(0x80)==1u) SL(0,0x21e9a);
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5a)==0x10u) STOP(0x21c0a);
            SW(0x4e,1);SB(0x5f,B(0x5f)&0x7fu);SB(0x5a,0);
        }
        return tail(a,e);
    case 0x21e9a:START(0x230654);SL(0,0x21eb0);/* fall through */
    case 0x21eb0:
        ga_stationary(a,e);STEP(r);
        if (r>0) SL(0,0x21ecc);
        return tail(a,e);
    case 0x21ecc: {
        START(0x23066a);SB(0x15,B(0x15)^1u);
        uint32_t v=(uint32_t)(int32_t)(int16_t)W(0x72)<<8;
        SL(0x62,BIT(0x15,0)?v:0u-v);SL(0,0x21f02);
    }   /* fall through */
    case 0x21f02:
        walker(a,e);STEP(r);
        if (BIT(0x5f,0)) STOP(0x21f22);
        return tail(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static const uint32_t STATES[]={0x6835c,0x21bb2,0x21e9a,0x21eb0,0x21ecc,0x21f02};
bool neo_hostage_routine(uint32_t r) {
    for (unsigned i=0;i<sizeof(STATES)/sizeof(STATES[0]);i++) if (STATES[i]==r) return true;
    for (unsigned i=0;i<sizeof(MINER)/sizeof(MINER[0]);i++) if (MINER[i]==r) return true;
    return false;
}
ga_result_t neo_hostage_tick(neo_gactor_t *a,neo_genv_t *e) {return hostage(a,e);}
