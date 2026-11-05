#include "neo_region.h"
/* Mission 3 wagon boxes and their map stamper, manual translation (ROM
 * labels as provenance; MAME run4 tap, docs/MISION3.md "Fase 3").
 * - 0B918C / 0B91AE (calls of the camera list 0B8AC8 / 0B8B0C): 039382 /
 *   03938A (+6E = 0 / 1) after the mission actor (0040BC, D0 = 1C) with +28
 *   = 0770 / 0870 and +2C = 0040 (neo_world).
 * - 039390: 01AB98 on 10C72E at (+28, +2C), layer 8834, palette 04BF
 *   (009EA8), +15 bit 0 clear, program 23C762 (004256, 0x100), +26 = 0,
 *   +4E = 1, hurtbox 039542; 0393DC: 01ABC8, 00427A and, with +3C < 0x148,
 *   004C24: a hit (carry) goes to 039406: sound 1069, +26 = 0x100; 03941C:
 *   01ABC8, 00427A; at its end 039436: the stamper of table 039572[+6E]
 *   (0040BC, D0 = 1C) and 039458 (01ABC8, 00427A). Every state ends in
 *   039468: 004188 once +3C <= -0x40 (after the cartridge check 039472,
 *   not reproduced).
 * - 0B911E / 0B9140: +78 = 0x74 / 0x84, +7A = 0x0B, +7C = +7E = 4, program
 *   0B9090, sub-map 0B85EA, +80 = 1, +76 = 0, then 0B8FA8: --+80; at 0 the
 *   next 8-byte record of the program (+1 = length): FF -> 004188; 1 -> +80
 *   = its timer and 0C70FC on 10C72E with D0/D1 = its source block, D2/D3 =
 *   +7C/+7E, D4/D5 = +78/+7A. The last record of 0B9090 leaves the 0B
 *   ledge at the rows the player climbs onto the wagon (x 1864..1903 and
 *   2120..2159): the 145 mutable cells of the pack are these six stamps.
 * - 0B9148 / 0B916A (camera-list calls): the wagon 038D1E / 038D26 (+6E =
 *   0 / 1) at (09D0 / 0AB0, 0048) of 10C72E: 039502, layer FFF1, palette
 *   04AB, program 23C624; two doors 038B06 (01B220 at (-58, 18) / (40, 18),
 *   +6E = 2*+6E / 2*+6E+1) and seven targets 038F38 (reserve 1C, (-40..20,
 *   60)); +74 = 0. 038EC6: +74 != 0 -> 0BC548 (10C700 = 7FFF); 038EDA: +74
 *   bit 1 -> program 23C62C; 038EFA: 01ABC8, 00427A, 0041AC at +3C <= -60;
 *   038D26 with +74 = 0 holds 10C700 = +3C - 58 (0BC542).
 * - 038B06 (door): +70 = +6E & 1, +1E + 1, palette 04AC, program
 *   23C574[+70], +4E = 0260C8(2D39DA), hurtbox 039516[+70], flash 04C0
 *   (004520). 038B78: 01ABC8, 00427A, 004C24 while +3C < 148; a hit
 *   (+5F bit 7) -> 038BB0: sound 1029, the creator's +74 bit +70, the
 *   stamper 039562[+6E] (0040BC, D0 = 1C), program 23C5D8, +15 bit 5, +30
 *   = -1 / +1, +34 = 3, +72 = 3; 038C20: 01AC38 three times then 0143E4,
 *   a bounce on the floor halves +30 and turns +34 into -3/4 of it, gravity
 *   -0.3125, 00427A. 038C74: 004188 at +3C <= -30 (after the cartridge
 *   check, not reproduced) or +40 <= D0.
 * - 038F38 (target): +4E = 1, hurtbox 039536; 038F4C: 01ABC8, 004C24; a hit
 *   -> 038F74: palette 04AB, program 23C638[rand & 1], two pieces 0390A6
 *   (D0 = 2A, 01B220 at (0, 0)); 038FB6: 01ABC8, 00427A. 038FC6: 004188 at
 *   +3C <= -30.
 * - 0390A6 (piece): 01ABC8, program 03957A[rand & 3], palette 020A, 01DEA2,
 *   +15 bit 5, layer FFF1, 03B332(rand(60) - 30, 0, -0.3125, rand(1E) +
 *   1E); 039130: 0143DC, 00427A, 004188 on the floor or outside 03955A
 *   (019504).
 * - 0B90BC / 0B90FA / 0B9106 / 0B9112: the stampers of the doors (+78 =
 *   95 / 9E / A3 / AC, +7A = 8, programs 0B9040 / 0B9064), as 0B911E.
 * - 0B925C (camera-list call): the platform 0B9286 at (0FA0, 0030) of the
 *   root 10E968. 0B9286: 0B9748 unless 10E9D0 > 10E99E, 0B93B4 at (1060,
 *   0030) of the root, layer 0, palette 04B7, program 2A8E50, +30 = -1,
 *   10C6BD = 0. 0B930E: 01AC38; +30 = +1 from 10E99E >= F10 or below x
 *   F60; at x >= FA0 with 10E99E >= F10 it stops there (0B939A, 10C6BD =
 *   1), else +30 = -1; 0140C0 with the map 0E6896, 00427A, 004188 at
 *   10E99E >= 10F0. 0B939A: 01ABC8, 0140C0, then as 0B930E's tail.
 * - 0B93B4: layer 0, palette 04B7, program 2A8EA6; 0B93F0: 01AD6E (C0, 0),
 *   the tail of 0B9286.
 * - 0B9400 (script record 2DADF4): 004188 once 10C6BD != 0. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
static neo_gactor_t *spawn_d0(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
}
static neo_gactor_t *spawn(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return spawn_d0(a,e,routine,0x1c);}
static uint32_t rl(uint32_t at) {const uint8_t *p=ga_rom(at);return p?((uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]):0;}
/* 039502: 01AB98 on 10C72E at (+28, +2C). */
static void place(neo_gactor_t *a,neo_genv_t *e) {ga_place(a,e,(int16_t)W(0x28),(int16_t)W(0x2c),0x10c72eu);}
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine) {return e->spawn?e->spawn(e->ctx,a,routine,false):NULL;}
/* 03B332 (as neo_rebel2.c): speeds to cover D0/D1 in D3 updates under D2. */
static void launch(int16_t d0,int16_t d1,uint32_t d2,uint16_t d3,uint32_t *vx,uint32_t *vy) {
    uint32_t g=(d2&0x80000000u)?0u-d2:d2;
    if ((int16_t)d3<2) d3=2;
    uint16_t q=(uint16_t)(0x10000u/d3);
    uint32_t x=(uint32_t)((int32_t)d0*(int16_t)q),y=(uint32_t)((int32_t)d1*(int16_t)q);
    uint32_t t=(g&0xffffu)*d3+(((g>>16)*d3)<<16);
    t=(uint32_t)((int32_t)t>>1);
    if (d2&0x80000000u) t=0u-t;
    *vx=x;*vy=y-t;
}
/* 039468 */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    if (e->unsupported) return GA_UNSUPPORTED;
    return (int16_t)W(0x3c)<=-0x40?GA_DELETE:GA_MORE;
}
static ga_result_t step(neo_gactor_t *a,neo_genv_t *e,int *r) {
    ga_project(a,e);
    *r=ga_anim_step(a,e,NULL);
    return *r<0?GA_UNSUPPORTED:GA_MORE;
}
/* 0B8FA8 */
static ga_result_t stamper(neo_gactor_t *a,neo_genv_t *e) {
    int16_t t=(int16_t)(W(0x80)-1u);
    if (t>0) {SW(0x80,t);return GA_MORE;}
    const uint8_t *p=ga_rom(L(0x6e));
    if (!p) {ga_fail(e,L(0x6e));return GA_UNSUPPORTED;}
    uint16_t d3=W(0x76);d3=(uint16_t)(d3+p[d3+1u]);
    if (p[d3]==0xffu) return GA_DELETE;
    if (p[d3]!=1u) {ga_fail(e,0xb902a);return GA_UNSUPPORTED;}                  /* TRAP #15 */
    SW(0x76,d3);SW(0x80,p[d3+2u]<<8|p[d3+3u]);
    const int16_t d[6]={(int16_t)(p[d3+4u]<<8|p[d3+5u]),(int16_t)(p[d3+6u]<<8|p[d3+7u]),(int16_t)W(0x7c),(int16_t)W(0x7e),(int16_t)W(0x78),(int16_t)W(0x7a)};
    if (e->map_patch) e->map_patch(e->ctx,0x10c72eu,L(0x72),d);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t m3props_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0xb9748:case 0xb9780:return neo_m3reward_tick(a,e);
    case 0x39382: SW(0x6e,0);goto box;
    case 0x3938a: SW(0x6e,1);
    box:
        ga_place(a,e,(int16_t)W(0x28),(int16_t)W(0x2c),0x10c72eu);
        SW(0x66,0x8834);SW(0x1e,0x8834);ga_palette(a,0x4bf);SB(0x15,B(0x15)&0xfeu);
        if (!ga_anim_start(a,e,0x23c762,0x100)) return GA_UNSUPPORTED;
        SW(0x26,0);SW(0x4e,1);SL(0x50,0x39542);SL(0,0x393dc);
        /* fall through */
    case 0x393dc:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        if ((int16_t)W(0x3c)<0x148 && ga_register(a,e,NULL,NULL)) SL(0,0x39406);
        return tail(a,e);
    case 0x39406:
        if (e->sound) e->sound(e->ctx,0x1069);
        SW(0x26,0x100);SL(0,0x3941c);
        /* fall through */
    case 0x3941c:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        if (r) SL(0,0x39436);
        return tail(a,e);
    case 0x39436:
        spawn(a,e,W(0x6e)?0xb9140u:0xb911eu);SL(0,0x39458);
        /* fall through */
    case 0x39458:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        return tail(a,e);
    case 0xb90bc: SW(0x78,0x95);SL(0x6e,0xb9040);goto door;
    case 0xb90fa: SW(0x78,0x9e);SL(0x6e,0xb9064);goto door;
    case 0xb9106: SW(0x78,0xa3);SL(0x6e,0xb9040);goto door;
    case 0xb9112: SW(0x78,0xac);SL(0x6e,0xb9064);
    door:
        SW(0x7a,8);goto ledge;
    case 0xb9140: SW(0x78,0x84);goto box_ledge;
    case 0xb911e: SW(0x78,0x74);
    box_ledge:
        SW(0x7a,0xb);SL(0x6e,0xb9090);
    ledge:
        SW(0x7c,4);SW(0x7e,4);SL(0x72,0xb85ea);SW(0x80,1);SW(0x76,0);SL(0,0xb8fa8);
        /* fall through */
    case 0xb8fa8:
        return stamper(a,e);
    /* ---- the wagons 038D1E / 038D26 ---- */
    case 0x38d1e: SW(0x6e,0);goto wagon;
    case 0x38d26: SW(0x6e,1);
    wagon: {
        place(a,e);
        SW(0x66,0xfff1);SW(0x1e,0xfff1);ga_palette(a,0x4ab);SB(0x15,B(0x15)&0xfeu);
        if (!ga_anim_start(a,e,0x23c624,0x100)) return GA_UNSUPPORTED;
        neo_gactor_t *c=child(a,e,0x38b06);
        if (c) {ga_place_from(c,a,e,-0x58,0x18);ga_sw(c,0x6e,(uint16_t)(2u*W(0x6e)));}
        c=child(a,e,0x38b06);
        if (c) {ga_place_from(c,a,e,0x40,0x18);ga_sw(c,0x6e,(uint16_t)(2u*W(0x6e)+1u));}
        for (int i=0;i<7;i++) {c=spawn(a,e,0x38f38);if (c) ga_place_from(c,a,e,(int16_t)(-0x40+0x10*i),0x60);}
        SB(0x74,0);SL(0,0x38ec6);
    }   /* fall through */
    case 0x38ec6:
        if (!B(0x74)) goto wagon_step;
        if (e->flags) e->flags->limit700=0x7fff;                       /* 0BC548 */
        SL(0,0x38eda);
        /* fall through */
    case 0x38eda:
        if (!(B(0x74)&2u)) goto wagon_step;
        if (!ga_anim_start(a,e,0x23c62c,0x100)) return GA_UNSUPPORTED;
        SL(0,0x38efa);
        /* fall through */
    case 0x38efa:
    wagon_step:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        if ((int16_t)W(0x3c)<=-0x60) return e->unsupported?GA_UNSUPPORTED:GA_DELETE_TREE;   /* 0041AC */
        if (W(0x6e)==1u && !B(0x74) && e->flags) e->flags->limit700=(uint16_t)(W(0x3c)-0x58u);   /* 0BC542 */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- 038B06: a door ---- */
    case 0x38b06: {
        SW(0x70,W(0x6e)&1u);SW(0x1e,W(0x1e)+1u);SB(0x15,B(0x15)&0xfeu);ga_palette(a,0x4ac);
        if (!ga_rom(0x23c574u+4u*W(0x70))) {ga_fail(e,0x23c574);return GA_UNSUPPORTED;}
        if (!ga_anim_start(a,e,rl(0x23c574u+4u*W(0x70)),0x100)) return GA_UNSUPPORTED;
        const uint8_t *t=ga_rom(0x2d39da);
        if (!t) {ga_fail(e,0x2d39da);return GA_UNSUPPORTED;}
        SW(0x4e,ga_rank_word(a,e,t));
        if (!ga_rom(0x39516u+4u*W(0x70))) {ga_fail(e,0x39516);return GA_UNSUPPORTED;}
        SL(0x50,rl(0x39516u+4u*W(0x70)));
        ga_flash_palette(a,0x4c0);SL(0,0x38b78);
    }   /* fall through */
    case 0x38b78:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
        if ((int16_t)W(0x3c)<0x148 && ga_register(a,e,NULL,NULL)) {
            if (B(0x5f)&0x80u) SL(0,0x38bb0);
            SB(0x5a,0);
        }
        goto door_tail;
    case 0x38bb0: {
        if (e->sound) e->sound(e->ctx,0x1029);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x38bba);return GA_UNSUPPORTED;}
        ga_sb(p,0x74,(uint8_t)(ga_b(p,0x74)|(1u<<(W(0x70)&7u))));
        if (!ga_rom(0x39562u+4u*W(0x6e))) {ga_fail(e,0x39562);return GA_UNSUPPORTED;}
        spawn(a,e,rl(0x39562u+4u*W(0x6e)));
        if (!ga_anim_start(a,e,0x23c5d8,0x100)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)|0x20u);
        SL(0x30,W(0x70)?0x00010000u:0xffff0000u);SL(0x34,0x30000);SW(0x72,3);SL(0,0x38c20);
    }   /* fall through */
    case 0x38c20: {
        uint32_t d1=L(0x34);
        if ((int16_t)W(0x72)>=0) {SW(0x72,W(0x72)-1u);ga_move(a,e);}
        else ga_air(a,e);
        if (B(0x15)&0x40u) {
            uint32_t d0=(uint32_t)((int32_t)L(0x30)>>1);d1=(uint32_t)((int32_t)d1>>1);
            uint32_t d2=(uint32_t)((int32_t)d1>>1);d1=0u-(d1+d2);
            SL(0x30,d0);SL(0x34,d1);
        }
        SL(0x34,L(0x34)+0xffffb000u);
        r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
    }
    door_tail:
        if (e->unsupported) return GA_UNSUPPORTED;
        if ((int16_t)W(0x3c)<=-0x30) return GA_DELETE;
        return (int16_t)W(0x40)<=0xd0?GA_DELETE:GA_MORE;
    /* ---- 038F38: a target ---- */
    case 0x38f38:
        SW(0x4e,1);SL(0x50,0x39536);SL(0,0x38f4c);
        /* fall through */
    case 0x38f4c:
        ga_project(a,e);
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5f)&0x80u) SL(0,0x38f74);
            SB(0x5a,0);
        }
        goto target_tail;
    case 0x38f74: {
        ga_palette(a,0x4ab);SB(0x15,B(0x15)&0xfeu);
        uint16_t k=ga_rand(e)&1u;
        if (!ga_rom(0x23c638u+4u*k)) {ga_fail(e,0x23c638);return GA_UNSUPPORTED;}
        if (!ga_anim_start(a,e,rl(0x23c638u+4u*k),0x100)) return GA_UNSUPPORTED;
        for (int i=0;i<2;i++) {neo_gactor_t *c=spawn_d0(a,e,0x390a6,0x2a);if (c) ga_place_from(c,a,e,0,0);}
        SL(0,0x38fb6);
    }   /* fall through */
    case 0x38fb6:
        if (step(a,e,&r)!=GA_MORE) return GA_UNSUPPORTED;
    target_tail:
        if (e->unsupported) return GA_UNSUPPORTED;
        return (int16_t)W(0x3c)<=-0x30?GA_DELETE:GA_MORE;
    /* ---- 0390A6: a piece ---- */
    case 0x390a6: {
        ga_project(a,e);
        uint16_t k=ga_rand(e)&3u;
        if (!ga_rom(0x3957au+4u*k)) {ga_fail(e,0x3957a);return GA_UNSUPPORTED;}
        if (!ga_anim_start(a,e,rl(0x3957au+4u*k),0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x20a);
        if (!(ga_rand(e)&2u)) SB(0x15,B(0x15)^1u);                     /* 01DEA2 */
        SB(0x15,B(0x15)|0x20u);SW(0x66,0xfff1);SW(0x1e,0xfff1);
        int16_t dx=(int16_t)(ga_rand_range(e,0x60)-0x30u);
        uint16_t n=(uint16_t)(ga_rand_range(e,0x1e)+0x1eu);
        uint32_t vx,vy;launch(dx,0,0xffffb000u,n,&vx,&vy);SL(0x30,vx);SL(0x34,vy);
        SL(0,0x39130);
    }   /* fall through */
    case 0x39130:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                      /* 0143DC */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        if (B(0x15)&0x40u) return GA_DELETE;
        return ga_cull_rect(a,e,0x3955a)?GA_DELETE:GA_MORE;
    /* ---- the platform 0B9286 ---- */
    case 0xb9286: {
        uint16_t restart=e->flags?e->flags->camera_restart:0;
        if (!(restart>e->camera)) spawn(a,e,0xb9748);
        neo_gactor_t *c=spawn(a,e,0xb93b4);
        if (c) {ga_sl(c,0x28,0x10600000u);ga_sl(c,0x2c,0x300000u);ga_sl(c,0x38,0x10e968u);}
        SW(0x66,0);SW(0x1e,0);ga_palette(a,0x4b7);SB(0x15,B(0x15)&0xfeu);
        if (!ga_anim_start(a,e,0x2a8e50,0x100)) return GA_UNSUPPORTED;
        SL(0x30,0xffff0000u);if (e->flags) e->flags->platform6bd=0;
        SL(0,0xb930e);
    }   /* fall through */
    case 0xb930e: {
        ga_move(a,e);
        if (e->camera>=0xf10u) SL(0x30,0x10000);
        int32_t x=(int32_t)L(0x28);
        if (x<0xf600000) SL(0x30,0x10000);
        if (x>=0xfa00000) {
            if (e->camera>=0xf10u) {SL(0x28,0xfa00000u);SL(0,0xb939a);if (e->flags) e->flags->platform6bd=1;}
            else SL(0x30,0xffff0000u);
        }
        goto platform_tail;
    }
    case 0xb939a:
        ga_project(a,e);
    platform_tail:
        if (e->dyn_set) e->dyn_set(e->ctx,a,0xe6896,0,0);             /* 0140C0 */
        /* fall through */
    case 0xb9380:
    platform_anim:
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        return e->camera>=0x10f0u?GA_DELETE:GA_MORE;
    case 0xb93b4:
        SW(0x66,0);SW(0x1e,0);ga_palette(a,0x4b7);SB(0x15,B(0x15)&0xfeu);
        if (!ga_anim_start(a,e,0x2a8ea6,0x100)) return GA_UNSUPPORTED;
        SL(0,0xb93f0);
        /* fall through */
    case 0xb93f0: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        ga_place(a,e,0xc0,0,p?NEO_GA_TAG(p->index):0);                 /* 01AD6E */
        goto platform_anim;
    }
    case 0xb9400:
        return e->flags && e->flags->platform6bd?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_m3props_routine(uint32_t r) {
    static const uint32_t k[]={0x39382,0x3938a,0x393dc,0x39406,0x3941c,0x39436,0x39458,0xb911e,0xb9140,0xb8fa8,
        0xb90bc,0xb90fa,0xb9106,0xb9112,0x38d1e,0x38d26,0x38ec6,0x38eda,0x38efa,0x38b06,0x38b78,0x38bb0,0x38c20,
        0x38f38,0x38f4c,0x38f74,0x38fb6,0x390a6,0x39130,0xb9286,0xb930e,0xb939a,0xb9380,0xb93b4,0xb93f0,0xb9400,0xb9748,0xb9780};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
ga_result_t neo_m3props_tick(neo_gactor_t *a,neo_genv_t *e) {return m3props_tick(a,e);}
