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
/* ---- 082BCE / 082BD8: the soldier hanging from a rope (records 2D8D6A,
 * 2D8E12, 2D8F8A/2D8F9C, 2D9408/2D941A: words +6E/+70 trigger distances,
 * +72/+74 kept in +7C/+7E; +84 = 0 for 082BCE, 1 for 082BD8).
 * 08326A: +76/+78/+7C/+7E = +6E/+70/+72/+74 (then 0), +4E = 0260C8(2D5354),
 * +7A = 0260C8(2D5418). With +28 and +2C set and +76/+78 nonzero, 082C10
 * waits (01ABC8) for the nearest player (01AF9A) in the air (01AF34) within
 * +76 px in X (0833F8) and +78 in Y (08342A); +28 = 0 places it at the
 * player's X less +76 (01AF9A), +2C = 0 at the floor (0146E8) + +78 - 0x18.
 * 082C9A: layer 8820, palette 0x3FD, flash 0xAC, facing the nearest player
 * (01DEA2 with none), 01B1DC, +74 = 0, program 2818E4 (the descent: hurtbox
 * and solid body 004E76 by 0833A8) -> 082D06 until its end, then 082D2E
 * (2818E4 hanging, 08345C: dynamic collision E4C2E): a player behind
 * (01B08A) turns it (082DB0, 281A5C); on screen (0..0x140), +74 run out and
 * +84 = 0 or a player within +7A (0832BE) -> 082DE4 (281AC6: the knife
 * 083350 by its callback; at its end +74 = 0260C8(2D53B6)). 082E28: +74 and
 * +85 count down; 004C24: a hit makes the splash 0682F0 (0833B4, every 0x18
 * updates) and +5A = 0; killed (+5F bit 7): 013F1E, 004E76(0) and 082EAA
 * (sound 1143, 019DFC, +17 = FF, +61 = 0, palettes released, 0xF2 for a
 * flame (+5A = 7) else 0x3FD, program 281C20 whose callback 083380 makes the
 * rope end 083212 and drops it at -3.0: 082F1A falls (0143DC) to the floor,
 * 082F48: 01AD52, palette 0x3FD, 281D66 to its end). +5A = 0x0A/0x0B (083018,
 * the burnt body 068D32) is an explicit stop. Culled by E4B3C (01945C).
 * 083350: sound 1144, the knife 0830FE (0040BC reserve 0x0E) at (-6, 0):
 * palette 0x3FB, layer E0EF, program 268F24, +60 = FF; 08313A: the row
 * E1A76[+60] while +60 != FF, gone at the program's end or when the
 * soldier dies (+5F bit 7 of +0C).
 * 0682F0 (the splash, after the script actor, 0x2A): palettes released,
 * 0x16A, program 22DB44, +30 = +-(0029C6 & 0x7F) << 8 (sign by bit 0), +34
 * -= that & 0x7FFF; 068340: 01AC38, 00427A, gone at its end or out of
 * E1A5E (0688D6). */
static bool nearest_player(neo_genv_t *e,neo_gactor_t *a,unsigned *p) {   /* 01AF9A: carry (true) = none */
    unsigned m=e->players.active_mask&3u;
    if (!m) return true;
    if (m==1u) {*p=0;return false;}
    if (m==2u) {*p=1;return false;}
    int16_t d1=(int16_t)(e->players.p1x-W(0x3c)),d2=(int16_t)(e->players.p2x-W(0x3c));
    if (d1<0) d1=(int16_t)-d1;
    if (d2<0) d2=(int16_t)-d2;
    *p=d2<d1?1u:0u;return false;
}
static int16_t pxw(neo_genv_t *e,unsigned p) {return (int16_t)(p?e->players.p2x:e->players.p1x);}
static int16_t pyw(neo_genv_t *e,unsigned p) {return (int16_t)(p?e->players.p2y:e->players.p1y);}
/* 01AF34: carry when the player is in play and off the floor (+15 bit 6). */
static bool airborne(neo_genv_t *e,unsigned p,bool *stop) {
    *stop=false;
    if (!((e->players.active_mask>>p)&1u)) return false;
    if (!e->vehicle || !e->vehicle->player[p]) {ga_fail(e,0x1af1e);*stop=true;return false;}
    return !(e->vehicle->player[p][0x15]&0x40u);
}
/* 0833F8 / 08342A: carry when the player is within the signed reach. */
static bool within(int16_t reach,int16_t delta) {
    if (!reach) return true;
    int16_t m=reach<0?-1:0;
    uint16_t r=(uint16_t)((reach^m)-m),d=(uint16_t)((delta^m)-m);
    if ((int16_t)d<0) return false;
    return d<=r;
}
/* 01B08A: carry when the player is behind the node. */
static bool behind(neo_gactor_t *a,neo_genv_t *e,unsigned p) {
    int16_t d0=(int16_t)(W(0x3c)-pxw(e,p));
    return (B(0x15)&1u)?d0>0:d0<0;
}
/* 0832BE: a player in play in front of it within +7A px. */
static bool k832be(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned p=0;p<2;p++) {
        if (!((e->players.active_mask>>p)&1u)) continue;            /* 01AEB0 */
        if (behind(a,e,p)) continue;
        int16_t d=(int16_t)(pxw(e,p)-W(0x3c));
        if (d<0) d=(int16_t)-d;
        if (d<=S16(W(0x7a))) return true;
    }
    return false;
}
static void solid_by_6e(neo_gactor_t *a,neo_genv_t *e) {if (e->solid) e->solid(e->ctx,a,L(0x6e));}   /* 0833A8 */
static void k8345c(neo_gactor_t *a,neo_genv_t *e) {if (e->dyn_set) e->dyn_set(e->ctx,a,0xe4c2e,0,0);}  /* 0140C0 */
static void k833b4(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x85)) return;
    SB(0x85,0x18);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x682f0,true):NULL;   /* 00411C, 0x2A */
    uint16_t r=ga_rand(e);
    int16_t dx=(int16_t)((r&7u)-3u),dy=(int16_t)(((r>>8)&7u)+0x10u);
    if (c) ga_place_at(c,a,e,dx,dy);                                   /* 01ACF2 */
}
static bool rope_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x833a8: solid_by_6e(a,e);return true;
    case 0x83350: {
        sound(e,0x1144);
        neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x830fe,0xe):e->spawn?e->spawn(e->ctx,a,0x830fe,false):NULL;
        if (c) ga_place_from(c,a,e,-6,0);                               /* 01B220 */
        return true;
    }
    case 0x83380:
        if (e->spawn_reserve) e->spawn_reserve(e->ctx,a,0x83212,0xe); else if (e->spawn) e->spawn(e->ctx,a,0x83212,false);
        SL(0x30,0);SL(0x34,0xfffd0000u);SL(0,0x82f1a);
        return true;
    }
    return false;
}
#define RSTEP(r) do { if (((r)=ga_anim_step(a,e,rope_call))<0) return GA_UNSUPPORTED; } while (0)
static ga_result_t rope(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint16_t v;unsigned p;bool stop;
    switch (L(0)) {
    case 0x82bce: case 0x82bd8: {
        SB(0x84,L(0)==0x82bd8u?1:0);
        uint16_t d0=W(0x6e),d1=W(0x70),d2=W(0x72),d3=W(0x74);          /* 08326A */
        SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);
        SW(0x76,d0);SW(0x78,d1);SW(0x7c,d2);SW(0x7e,d3);
        if (!rank(a,e,0x2d5354,&v)) return GA_UNSUPPORTED;
        SW(0x4e,v);
        if (!rank(a,e,0x2d5418,&v)) return GA_UNSUPPORTED;
        SW(0x7a,v);
        if (!W(0x28) || !W(0x2c)) {                                     /* 082C3C */
            if (!W(0x28)) {
                if (nearest_player(e,a,&p)) {ga_project(a,e);return GA_MORE;}
                /* 082C56: +28 += the player's X long - the node's - (+76 << 16) (the low words: +3E) */
                uint32_t px=(uint32_t)(uint16_t)pxw(e,p)<<16|(p?e->player_low[1]:e->player_low[0]);
                uint32_t d=px-L(0x3c)-((uint32_t)W(0x76)<<16);
                SL(0x28,L(0x28)+d);ga_project(a,e);
            }
            if (!W(0x2c)) {
                SW(0x2c,0xff);ga_floor(a,e);
                SW(0x2c,W(0x2c)+W(0x78));SW(0x2c,W(0x2c)-0x18u);ga_project(a,e);
            }
            goto appear;
        }
        SW(0x80,W(0x76));SW(0x82,W(0x78));
        if (!(W(0x80)|W(0x82))) goto appear;
        SL(0,0x82c10);
    }   /* fall through */
    case 0x82c10:
        ga_project(a,e);
        if (nearest_player(e,a,&p)) return GA_MORE;
        if (airborne(e,p,&stop) || stop) return stop?GA_UNSUPPORTED:GA_MORE;
        if (!within(S16(W(0x80)),(int16_t)(pxw(e,p)-W(0x3c)))) return GA_MORE;
        if (!within(S16(W(0x82)),(int16_t)(pyw(e,p)-W(0x40)))) return GA_MORE;
    appear:                                                             /* 082C9A */
        SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0x3fd);ga_flash_palette(a,0xac);
        if (!nearest_player(e,a,&p)) {if (pxw(e,p)>=S16(W(0x3c))) SB(0x15,B(0x15)|1u);}
        else if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);                 /* 01DEA2 */
        SL(0x30,0);SL(0x34,0);SL(0x62,0);SW(0x74,0);                    /* 01B1DC */
        if (!ga_anim_start(a,e,0x2818e4,0x100)) return GA_UNSUPPORTED;
        SL(0,0x82d06);
        /* fall through */
    case 0x82d06:
        ga_project(a,e);RSTEP(r);
        if (r) SL(0,0x82d2e);
        if (e->unsupported) return GA_UNSUPPORTED;
        return ga_cull(a,e,0xe4b3c)?GA_DELETE:GA_MORE;                  /* 083070: cartridge check, 004188 */
    case 0x82d2e:
        if (!ga_anim_start(a,e,0x281a08,0x100)) return GA_UNSUPPORTED;
        SL(0,0x82d44);
        /* fall through */
    case 0x82d44:
        ga_project(a,e);k8345c(a,e);RSTEP(r);
        if (!nearest_player(e,a,&p) && behind(a,e,p)) SL(0,0x82db0);
        if (S16(W(0x3c))>=0 && S16(W(0x3c))<=0x140 && S16(W(0x74))<=0) {
            if (!B(0x84) || k832be(a,e)) SL(0,0x82de4);
        }
        goto tail;
    case 0x82db0:
        if (!ga_anim_start(a,e,0x281a5c,0x100)) return GA_UNSUPPORTED;
        SL(0,0x82dc6);
        /* fall through */
    case 0x82dc6:
        ga_project(a,e);k8345c(a,e);RSTEP(r);
        if (r) SL(0,0x82d2e);
        goto tail;
    case 0x82de4:
        if (!ga_anim_start(a,e,0x281ac6,0x100)) return GA_UNSUPPORTED;
        SL(0,0x82dfa);
        /* fall through */
    case 0x82dfa:
        ga_project(a,e);k8345c(a,e);RSTEP(r);
        if (r) {
            if (!rank(a,e,0x2d53b6,&v)) return GA_UNSUPPORTED;
            SW(0x74,v);SL(0,0x82d2e);
        }
        goto tail;
    case 0x82f0a:
        ga_project(a,e);RSTEP(r);
        goto cull;
    case 0x82f1a:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                      /* 0143DC */
        RSTEP(r);
        if (B(0x15)&0x40u) SL(0,0x82f48);
        goto cull;
    case 0x82f48: {
        neo_contact_parent_t op,np;uint32_t tag=L(0x38);                /* 01AD52 */
        if (tag!=0x10e968u) {
            bool ho=tag && ga_parent(e,tag,&op),hn=ga_parent(e,0x10e968u,&np);
            uint32_t d0=L(0x28),d1=L(0x2c),d2=L(0x30)+d0,d3=L(0x34)+d1;
            if (ho) {d0+=(uint32_t)op.previous_x;d1+=(uint32_t)op.previous_y;d2+=(uint32_t)op.x;d3+=(uint32_t)op.y;}
            if (hn) {d0-=(uint32_t)np.previous_x;d1-=(uint32_t)np.previous_y;d2-=(uint32_t)np.x;d3-=(uint32_t)np.y;}
            SL(0x28,d0);SL(0x2c,d1);SL(0x30,d2-d0);SL(0x34,d3-d1);SL(0x38,0x10e968u);
        }
        ga_project(a,e);
        SB(0x1d,0);ga_palette(a,0x3fd);
        if (!ga_anim_start(a,e,0x281d66,0x100)) return GA_UNSUPPORTED;
        SL(0,0x82f78);
    }   /* fall through */
    case 0x82f78:
        ga_project(a,e);RSTEP(r);
        return r?GA_DELETE:GA_MORE;                                     /* cartridge check, 004188 */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
tail:                                                                   /* 082E28 */
    if (S16(W(0x74))>0) SW(0x74,W(0x74)-1u);
    if ((int8_t)B(0x85)>0) SB(0x85,B(0x85)-1u);
    if (ga_register(a,e,NULL,NULL)) {
        if (B(0x5f)&0x80u) {
            uint8_t k=B(0x5a);
            if (k==0x0au || k==0x0bu) {ga_fail(e,0x83018);return GA_UNSUPPORTED;}   /* the burnt body 068D32: no capture */
            SL(0,0x82eaa);
            if (e->dyn_release) e->dyn_release(e->ctx,a);               /* 013F1E */
            if (e->solid) e->solid(e->ctx,a,0);                         /* 004E76 */
        } else SB(0x5a,0);
        k833b4(a,e);
    }
cull:
    if (e->unsupported) return GA_UNSUPPORTED;
    return ga_cull(a,e,0xe4b3c)?GA_DELETE:GA_MORE;
}
static ga_result_t rope_dead(neo_gactor_t *a,neo_genv_t *e) {           /* 082EAA */
    int r;
    sound(e,0x1143);SB(0x69,0xff);SB(0x17,0xff);SB(0x61,0);SB(0x1d,0);
    ga_palette(a,B(0x5a)==7u?0xf2:0x3fd);
    if (!ga_anim_start(a,e,0x281c20,0x100)) return GA_UNSUPPORTED;
    SL(0,0x82f0a);
    ga_project(a,e);RSTEP(r);
    if (e->unsupported) return GA_UNSUPPORTED;
    return ga_cull(a,e,0xe4b3c)?GA_DELETE:GA_MORE;
}
static ga_result_t rope_end(neo_gactor_t *a,neo_genv_t *e) {            /* 083212 */
    int r;
    if (L(0)==0x83212u) {
        SW(0x66,0x8820);SW(0x1e,0x8820);ga_palette(a,0x3fd);START(0x281dea);
        neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;
        SB(0x15,c?(uint8_t)(ga_b(c,0x15)&3u):0);                        /* 01ACA2 */
        ga_place(a,e,0,0,c?NEO_GA_TAG(c->index):0);                     /* 01AD6E */
        SL(0,0x83252);
    }
    ga_project(a,e);STEP(r);
    return r?GA_DELETE:GA_MORE;
}
static ga_result_t knife2(neo_gactor_t *a,neo_genv_t *e) {               /* 0830FE */
    int r;
    if (L(0)==0x830feu) {
        SB(0x1d,0);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);ga_palette(a,0x3fb);START(0x268f24);
        SB(0x60,0xff);SL(0,0x8313a);
    }
    ga_project(a,e);STEP(r);
    if (r) return GA_DELETE;
    if (B(0x60)!=0xffu) ga_attack(a,e,rlong(e,0xe1a76u+4u*B(0x60)));
    if (e->unsupported) return GA_UNSUPPORTED;
    neo_gactor_t *c=e->creator?e->creator(e->ctx,a):NULL;
    if (c && (ga_b(c,0x5f)&0x80u)) return GA_DELETE;                    /* cartridge check, 004188 */
    return GA_MORE;
}
static ga_result_t splash(neo_gactor_t *a,neo_genv_t *e) {               /* 0682F0 */
    int r;
    if (L(0)==0x682f0u) {
        SB(0x1d,0);ga_palette(a,0x16a);START(0x22db44);
        int16_t d=(int16_t)(ga_rand(e)&0x7fu);
        if (d&1) d=(int16_t)-d;
        uint32_t v=(uint32_t)(int32_t)d<<8;
        SL(0x30,v);SL(0x34,L(0x34)-(v&0x7fffu));SL(0,0x68340);
    }
    ga_move(a,e);STEP(r);
    if (r) return GA_DELETE;
    if (ga_cull(a,e,0xe1a5e)) {SB(0x60,0xff);return GA_DELETE;}         /* 0688D6 */
    return GA_MORE;
}
static bool rope_routine(uint32_t r) {
    return r==0x82bceu || r==0x82bd8u || r==0x82c10u || r==0x82d06u || r==0x82d2eu || r==0x82d44u || r==0x82db0u || r==0x82dc6u ||
        r==0x82de4u || r==0x82dfau || r==0x82eaau || r==0x82f0au || r==0x82f1au || r==0x82f48u || r==0x82f78u;
}
static bool treasure_routine(uint32_t r) {
    return r==0x995ecu || r==0x9963eu || r==0x99684u || r==0x9969au || r==0x996b2u || r==0x996c8u || r==0x996e4u || r==0x996fau || r==0x9972eu;
}
static bool wrapped_routine(uint32_t r) {return r==0x32de2u || r==0x32e54u || r==0x32f2eu || r==0x32f54u;}
bool neo_m2traps_routine(uint32_t r) {return treasure_routine(r) || wrapped_routine(r) || r==0x3a05au || r==0x3a09cu || r==0x3a1aeu || r==0x3a1e4u ||
    rope_routine(r) || r==0x83212u || r==0x83252u || r==0x830feu || r==0x8313au || r==0x682f0u || r==0x68340u;}
ga_result_t neo_m2traps_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (treasure_routine(r)) return treasure(a,e);
    if (wrapped_routine(r)) return wrapped(a,e);
    if (r==0x3a05au || r==0x3a09cu) return vent(a,e);
    if (r==0x3a1aeu || r==0x3a1e4u) return puff(a,e);
    if (r==0x82eaau) return rope_dead(a,e);
    if (rope_routine(r)) return rope(a,e);
    if (r==0x83212u || r==0x83252u) return rope_end(a,e);
    if (r==0x830feu || r==0x8313au) return knife2(a,e);
    if (r==0x682f0u || r==0x68340u) return splash(a,e);
    ga_fail(e,r);return GA_UNSUPPORTED;
}
