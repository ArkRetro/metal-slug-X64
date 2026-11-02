#include "neo_region.h"
/* Mission 2 stage-3 objects the level script places (B31), manual
 * translation (ROM labels as provenance).
 *
 * 0995EC: the hidden treasure (record 2D89EE, words +6E points table,
 *   +70 facing, +72 health). 0146E8; +15 bit 0 when +70 = 0; hurtbox +50 =
 *   099838, +4E = +72, palette 0x104, layer 1001, program 29F57A.
 *   09963E: 01ABC8, 00427A; 09964A: 004C24 (hit: 099684), +5A = 0, +5F bit
 *   7 -> 0996E4; 09966E: 01945C rectangle 099828 -> 09972E (the cartridge
 *   check, not reproduced, then 004188) at the next update.
 *   099684: program 29F586 (the hit), 09969A until its end -> 0996B2,
 *   both through 09964A. 0996B2: program 29F5D8 (open), 0996C8: 01ABC8,
 *   00427A, 0997BE: player 2 then player 1 in play (01AEB0) inside the
 *   rectangle 099830 (01AF3C): +74 = that player, 08CECC with the points
 *   of 099854 (+6E = 0) or 099858 -> 0996E4. 0996E4: program 29F602,
 *   0996FA until its end, then the row 099844 (0049E8), palettes released
 *   (009F88), sound 1022 and the node becomes the explosion 01C7AE.
 *   08CECC: 057948 (BCD sum into the score of the player), sound 1121, the
 *   score label 08D330 after the script actor (00411C) with +6E = the
 *   value, +8A = the player, at the node (01ACF2).
 *
 * 032DE2: the soldier wrapped as a mummy (records 2D8C68/2D8C76, words
 *   +6E gift, +70 its life). +78 = +69, 019DFC; +74/+76 = +6E/+70 and
 *   +6E/+70/+72/+7A = 0; palette 0x105, layer 8820, program 239492, hurtbox
 *   032DD6, +62 = 0, +4E = 0x64, 0146E8. 032E54: 08CCD4, 004C24: a hit by
 *   the mummies' breath (+5A = 0x10) restores +69 = +78 and goes to 032F2E;
 *   any hit leaves +5A = 0, +4E = 0x64, +5F bit 7 clear (it cannot die);
 *   00427A; 01945C rectangle 032DCE -> 004188 (after the cartridge check).
 *   032F2E: the flash 021CB6 (0040BC reserve 0x1C, 01ACF2), +80 = 2;
 *   032F54: 08CCD4, 00427A, at +80 = 3 (raised by the flash's callback
 *   022230) 068E94 (palettes 0x3FC/0x3FD/0xF2, flash 0xAC, +60/+61/+6E/+9C
 *   = 0, +94 = 0x38 - 0029F2(0x18), +4E = 0260C8(2D2780), +8A =
 *   0260C8(2D27E2) << 8, the soldier 067AB4 of neo_rebel3.c); 00427A again
 *   and the cull. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define S16(v) ((int16_t)(uint16_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP(r) do { if (((r)=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED; } while (0)
static uint32_t rlong(neo_genv_t *e,uint32_t at) {
    const uint8_t *p=ga_rom(at);
    if (!p) {ga_fail(e,at);return 0;}
    return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];
}
static bool rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table,uint16_t *out) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return false;}
    *out=ga_rank_word(a,e,t);return true;
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
/* 08CCD4: ground step on the floor, else gravity + air. */
static void k8ccd4(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&0x40u) {uint32_t kind;ga_ground(a,e,&kind);}
    else {SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);}
}
/* 01AF3C: carry (true) when player p is outside rectangle `rect` (x,y,w,h)
 * relative to the node, mirrored by its flips. */
static bool player_outside(neo_gactor_t *a,neo_genv_t *e,unsigned p,uint32_t rect) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return true;}
    int16_t rx=(int16_t)(r[0]<<8|r[1]),ry=(int16_t)(r[2]<<8|r[3]),rw=(int16_t)(r[4]<<8|r[5]),rh=(int16_t)(r[6]<<8|r[7]);
    int16_t dy=(int16_t)((p?e->players.p2y:e->players.p1y)-W(0x40));
    if (B(0x15)&2u) dy=(int16_t)-dy;
    if (dy<ry || dy>(int16_t)(ry+rh)) return true;
    int16_t dx=(int16_t)((p?e->players.p2x:e->players.p1x)-W(0x3c));
    if (B(0x15)&1u) dx=(int16_t)-dx;
    return dx<rx || dx>(int16_t)(rx+rw);
}
/* 08CECC: the points of `table` for player p (D1 = 0). False on a stop. */
static bool award(neo_gactor_t *a,neo_genv_t *e,unsigned p,uint32_t table) {
    uint32_t value=rlong(e,table);
    if (e->unsupported) return false;
    neo_pickup_env_t *pk=e->pickup;
    if (!pk) {ga_fail(e,0x8cedc);return false;}
    neo_item_add_score(pk->score[p],value);                           /* 057948 */
    sound(e,0x1121);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8d330,true):NULL; /* 00411C */
    if (c) {ga_sl(c,0x6e,value);ga_sb(c,0x8a,(uint8_t)p);ga_place_at(c,a,e,0,0);}
    return true;
}
/* 0997BE: carry (true) when a player in play touched it (points given). */
static bool touched(neo_gactor_t *a,neo_genv_t *e,bool *stop) {
    *stop=false;
    for (int p=1;p>=0;p--) {
        SW(0x74,(uint16_t)p);
        if (!((e->players.active_mask>>p)&1u)) continue;                /* 01AEB0 */
        if (player_outside(a,e,(unsigned)p,0x99830)) {if (e->unsupported) {*stop=true;return false;} continue;}
        if (!award(a,e,(unsigned)p,W(0x6e)?0x99858u:0x99854u)) {*stop=true;return false;}
        return true;
    }
    return false;
}
static ga_result_t treasure(neo_gactor_t *a,neo_genv_t *e) {
    int r;bool stop;
    switch (L(0)) {
    case 0x995ec:
        ga_floor(a,e);
        if (!W(0x70)) SB(0x15,B(0x15)|1u);
        SL(0x50,0x99838);SW(0x4e,W(0x72));ga_palette(a,0x104);
        SW(0x66,0x1001);SW(0x1e,0x1001);START(0x29f57a);SL(0,0x9963e);
        /* fall through */
    case 0x9963e:
        ga_project(a,e);STEP(r);
        goto hits;
    case 0x99684:
        START(0x29f586);SL(0,0x9969a);
        /* fall through */
    case 0x9969a:
        ga_project(a,e);STEP(r);
        if (r) SL(0,0x996b2);
    hits:                                                               /* 09964A */
        if (ga_register(a,e,NULL,NULL)) SL(0,0x99684);
        SB(0x5a,0);
        if (B(0x5f)&0x80u) SL(0,0x996e4);
        goto cull;
    case 0x996b2:
        START(0x29f5d8);SL(0,0x996c8);
        /* fall through */
    case 0x996c8:
        ga_project(a,e);STEP(r);
        if (touched(a,e,&stop)) SL(0,0x996e4);
        if (stop) return GA_UNSUPPORTED;
        goto cull;
    case 0x996e4:
        START(0x29f602);SL(0,0x996fa);
        /* fall through */
    case 0x996fa:
        ga_project(a,e);STEP(r);
        if (!r) goto cull;
        ga_attack(a,e,0x99844);
        SB(0x1d,0);                                                     /* 009F88 */
        sound(e,0x1022);
        SL(0,0x1c7ae);
        return neo_effects_tick(a,e);                                   /* 01C7AE */
    case 0x9972e:
        return GA_DELETE;                                               /* cartridge check, 004188 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
cull:
    if (e->unsupported) return GA_UNSUPPORTED;
    if (ga_cull(a,e,0x99828)) SL(0,0x9972e);                            /* 09966E */
    return GA_MORE;
}
/* Program 239492's callback 239604: +7E = FF one time in four (0029C6 & 3 = 0). */
static bool wrapped_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL || routine!=0x239604u) return false;
    SB(0x7e,(ga_rand(e)&3u)?0:0xff);return true;
}
#define WSTEP(r) do { if (((r)=ga_anim_step(a,e,wrapped_call))<0) return GA_UNSUPPORTED; } while (0)
static ga_result_t wrapped(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint16_t v;
    switch (L(0)) {
    case 0x32de2:
        SB(0x78,B(0x69));SB(0x69,0xff);                                 /* 019DFC */
        SW(0x74,W(0x6e));SW(0x76,W(0x70));
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x7a,0);
        ga_palette(a,0x105);SW(0x66,0x8820);SW(0x1e,0x8820);START(0x239492);
        SL(0x50,0x32dd6);SL(0x62,0);SW(0x4e,0x64);ga_floor(a,e);SL(0,0x32e54);
        /* fall through */
    case 0x32e54:
        k8ccd4(a,e);
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5a)==0x10u) {SB(0x69,B(0x78));SL(0,0x32f2e);}
            SB(0x5a,0);SW(0x4e,0x64);SB(0x5f,B(0x5f)&0x7fu);
        }
        WSTEP(r);
        break;
    case 0x32f2e: {
        neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x21cb6,0x1c):e->spawn?e->spawn(e->ctx,a,0x21cb6,false):NULL;
        if (c) ga_place_at(c,a,e,0,0);                                  /* 01ACF2 */
        SB(0x80,2);SL(0,0x32f54);
    }   /* fall through */
    case 0x32f54:
        k8ccd4(a,e);WSTEP(r);
        if (B(0x80)==3u) {                                              /* 068E94 */
            SB(0x1d,0);ga_palette(a,0x3fc);ga_palette(a,0x3fd);ga_palette(a,0xf2);ga_flash_palette(a,0xac);
            SB(0x61,0);SB(0x60,0);SW(0x6e,0);SB(0x9c,0);
            SW(0x94,(uint16_t)(0x38u-ga_rand_range(e,0x18)));
            if (!rank(a,e,0x2d2780,&v)) return GA_UNSUPPORTED;
            SW(0x4e,v);
            if (!rank(a,e,0x2d27e2,&v)) return GA_UNSUPPORTED;
            SL(0x8a,(uint32_t)v<<8);SL(0,0x67ab4);
        }
        WSTEP(r);
        break;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
    if (e->unsupported) return GA_UNSUPPORTED;
    return ga_cull(a,e,0x32dce)?GA_DELETE:GA_MORE;                      /* cartridge check, 004188 */
}
/* 03A05A: the gas vent (record 2D8E58: +6E puff mask, +70 camera progress
 * limit). Layer E0EF, facing clear, +74/+76 = 0260C8(2D3A76) (period),
 * +7A = 0260C8(2D3AD8) (every +7A-th emission the middle puff), +7C = 0.
 * 03A09C: gone (0041AC) once the camera progress 10E99E reaches +70; every
 * +74 updates, inside the rectangle 03A900 (01ABC8, 019504): 03A0E4 and
 * sound 10EE. 03A0E4: the +7A-th time one puff 03A1AE at (0, 0) with +72 =
 * bit 1 of +6E; otherwise two, at (-20, -5) with bit 2 and (20, 5) with bit
 * 0 (0040BC reserve 0x1C, 01B220; with no node the bit lands in the
 * dummy).
 * 03A1AE: program 23CABA[+72], palette 0x4BB, +78 = FFFF; 03A1E4: 01ABC8,
 * 00427A: at its end 004188 (after the cartridge check), else while +78 >=
 * 0 (its program writes +78) the row 3A8A0[+72][+78] (0049E8: kind 0x10,
 * the mummies' breath). */
static neo_gactor_t *spawn_at(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);                                 /* 01B220 */
    return c;
}
static ga_result_t vent(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t v;
    switch (L(0)) {
    case 0x3a05a:
        SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x15,B(0x15)&0xfeu);
        if (!rank(a,e,0x2d3a76,&v)) return GA_UNSUPPORTED;
        SW(0x74,v);SW(0x76,v);
        if (!rank(a,e,0x2d3ad8,&v)) return GA_UNSUPPORTED;
        SW(0x7a,v);SW(0x7c,0);SL(0,0x3a09c);
        /* fall through */
    case 0x3a09c: {
        if (e->camera>=W(0x70)) return GA_DELETE_TREE;                  /* 0041AC */
        SW(0x76,W(0x76)-1u);
        if (!(W(0x76)&0x8000u)) return GA_MORE;
        SW(0x76,W(0x74));ga_project(a,e);
        if (ga_cull_rect(a,e,0x3a900)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        neo_gactor_t *c;
        if (W(0x7a) && (SW(0x7c,W(0x7c)+1u),W(0x7c)==W(0x7a))) {
            SW(0x7c,0);
            c=spawn_at(a,e,0x3a1ae,0x1c,0,0);
            if (c) ga_sw(c,0x72,(W(0x6e)>>1)&1u);
        } else {
            c=spawn_at(a,e,0x3a1ae,0x1c,-20,-5);
            if (c) ga_sw(c,0x72,(W(0x6e)>>2)&1u);
            c=spawn_at(a,e,0x3a1ae,0x1c,20,5);
            if (c) ga_sw(c,0x72,W(0x6e)&1u);
        }
        sound(e,0x10ee);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static ga_result_t puff(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    if (L(0)==0x3a1aeu) {
        START(rlong(e,0x23cabau+4u*W(0x72)));
        ga_palette(a,0x4bb);SW(0x78,0xffff);SL(0,0x3a1e4);
    }
    ga_project(a,e);STEP(r);
    if (r) return GA_DELETE;                                            /* cartridge check, 004188 */
    if (!(W(0x78)&0x8000u)) {
        uint32_t rows=rlong(e,0x3a8a0u+4u*W(0x72));
        if (e->unsupported) return GA_UNSUPPORTED;
        ga_attack(a,e,rlong(e,rows+4u*W(0x78)));
    }
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static bool treasure_routine(uint32_t r) {
    return r==0x995ecu || r==0x9963eu || r==0x99684u || r==0x9969au || r==0x996b2u || r==0x996c8u || r==0x996e4u || r==0x996fau || r==0x9972eu;
}
static bool wrapped_routine(uint32_t r) {return r==0x32de2u || r==0x32e54u || r==0x32f2eu || r==0x32f54u;}
bool neo_m2traps_routine(uint32_t r) {return treasure_routine(r) || wrapped_routine(r) || r==0x3a05au || r==0x3a09cu || r==0x3a1aeu || r==0x3a1e4u;}
ga_result_t neo_m2traps_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (treasure_routine(r)) return treasure(a,e);
    if (wrapped_routine(r)) return wrapped(a,e);
    if (r==0x3a05au || r==0x3a09cu) return vent(a,e);
    if (r==0x3a1aeu || r==0x3a1e4u) return puff(a,e);
    ga_fail(e,r);return GA_UNSUPPORTED;
}
