#include "neo_region.h"
#include "neo_target.h"
#include "neo_soldier_ai.h"
/* Mission 6 stage 1 (the coast) props, manual translation (ROM labels as
 * provenance; MAME run4 replay from sec09_292683, captures-m6/fam/R,
 * docs/MISION6.md "Fase 2").
 * - 0BD71E (call of the camera list 0BCA00, head): the rafts 0BD822 /
 *   0BD836 / 0BD84A at (010A / 0270 / 02F0, 0020) of the root 10E968 with
 *   +74 = 0 / 4 / 8, then three 0BDA4E at the same points (0040BC, D0 = 1C).
 * - 0BD822 / 0BD836 / 0BD84A (the raft): its float 0BD8F4 / 0BD906 / 0BD918
 *   (0040BC, D0 = 1C; +38 = the root; 0BD84A also copies its +74), +66 =
 *   +1E = 0, +30 = 0, +34 = 1, +15 bit 0 clear, +6E = 0340, +72 = 0.
 *   0BD898: 0BD9D4 counts the players in play inside 0BDA46 (01B332: +72 =
 *   2 per player), +34 = 0 / +1 / -1 to bring +2C to 0BDA0C[+72] (0020,
 *   001C, 0018: the raft sinks under its riders), 01AC38, 004188 once
 *   10E99E >= +6E.
 * - 0BD8F4 / 0BD906 / 0BD918 (the float): palette 4CB / 4CC / 4CD, +6E =
 *   the creator's +6E, program 2A953E, +66 = +1E = 0, +15 bit 0 clear, +76 =
 *   0. 0BD95E: the bobbing table 0BDA12 (updates, dy) by +74 (+76 counts
 *   the updates; at the entry's count +74 steps 4, & 1C), 01AD6E at (0, dy)
 *   of the raft, 01ABC8, the cell map 0BDA32 (0140C0: nine cells 0B, the
 *   floor the player stands on), 00427A, 004188 once 10E99E >= +6E.
 * - 0BDA4E (the raft's picture): palette 509, program 2A954E, +66 = +1E =
 *   0, +15 bit 0 clear, +6E = 0340, +15 bit 2 set; then 0BD508.
 * - 0BD416 / 0BD444 / 0BD472 / 0BD4A6 / 0BD4D4 (decorations of the list
 *   calls 0BD2FA, 0BD386, 0BD3B6, 0BD3E6; +28/+2C/+38/+6E from the call):
 *   programs 2A946C / 2A947C / 2A948C / 2A949C / 2A94B4, palettes 4D4 /
 *   4D5 / 50B / 4DF / 11D, layers FFF1 (0BD472: 1111 and 012022; 0BD4D4: 0
 *   with +15 bit 0 clear). 0BD508 -> 0BD50E: 01ABC8, 00427A, 004188 once
 *   10E99E >= +6E.
 * - 0BDA8E (list call, progress 32): 0BDAB8 at (01B0, 0060) of the root:
 *   program 2A95A4, +4E = rank 2D3FEE (0260C8); 0BDADE: 01ABC8, 00427A.
 * The hanging bridge 10D7B6 (made by 0BCDC2 with its driver 0BCE2C,
 * neo_m6bridge.c; the layer and RAM 10C70C..10C714 through e->m6bridge):
 * - 0BD18E (the swing): 10C712..10C714 = 0, 10C70E = 10D7B6 +0; then
 *   0BD1B8: d = (+0 word) - (10C70E word), 10C712 = d & 3, 10C713 =
 *   (10C713 + 2) & 7, 10C714 = ((10C713 & 6) << 1) + (d & 3) & F, 10C70E =
 *   10D7B6 +0.
 * - 0BD2BA / 0BD2CE (the span makers): +6E = 0 / 2, +72 = 10C70C /
 *   10C70D (the lock byte), 0BD202: +70 = 0; 0BD20E: d4 = 0BD2A8[+6E]
 *   (6 / 14; FFFF: 004188) + 10D7B6 +32 (0C6F6A), d3 = (+36 + 1F - d4) << 4;
 *   with d3 >= 0, the lock byte 0 and d3 != +70 it calls 0BD2AE[2 +6E]
 *   (0BD09A / 0BD114) and sets the lock byte to 1; +70 = d3.
 * - 0BD09A / 0BD114: the spans 0BCFA4 + 0BCFC4 / 0BCFE4 + 0BD004 (0040BC,
 *   D0 = 1C) at ((+36 + 1F) << 4, 0058) of 10D7B6.
 * - 0BCFA4 / 0BCFC4 / 0BCFE4 / 0BD004 (the spans): +66 = +1E = FFF0 / 0 /
 *   FFF0 / 0, program 2A923C / 2A92C8 / 2A9354 / 2A93E0, palette 4E1, +15
 *   bit 0 clear; 0BD044: the byte +6E = 10C714, 01ABC8, 01945C (0BCF9C) ->
 *   004188, 00427A, 004188 once 10E99E >= 08B0 or +28 < 04F00000. */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,(uint32_t)(v))
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
static uint16_t rw(uint32_t at) {const uint8_t *p=ga_rom(at);return p?(uint16_t)(p[0]<<8|p[1]):0;}
static bool rom_ok(neo_genv_t *e,uint32_t at) {if (ga_rom(at)) return true;ga_fail(e,at);return false;}
/* 01B332 (01AEB0, 01AF3C): player p in play and inside the rectangle `rect`
 * (x, y, w, h; both edges in) of the actor's projected point. */
static bool player_in(neo_gactor_t *a,neo_genv_t *e,unsigned p,uint32_t rect) {
    if (!((e->players.active_mask>>p)&1u)) return false;
    int16_t d1=(int16_t)(uint16_t)((p?e->players.p2y:e->players.p1y)-W(0x40)),d2=(int16_t)rw(rect+2),d3=(int16_t)(uint16_t)((uint16_t)d2+rw(rect+6));
    if (B(0x15)&2u) d1=(int16_t)-d1;
    if (d1<d2 || d1>d3) return false;
    d1=(int16_t)(uint16_t)((p?e->players.p2x:e->players.p1x)-W(0x3c));d2=(int16_t)rw(rect);d3=(int16_t)(uint16_t)((uint16_t)d2+rw(rect+4));
    if (B(0x15)&1u) d1=(int16_t)-d1;
    return d1>=d2 && d1<=d3;
}
static ga_result_t until(neo_gactor_t *a,neo_genv_t *e) {   /* 10E99E >= +6E: 004188 */
    if (e->unsupported) return GA_UNSUPPORTED;
    return e->camera>=W(0x6e)?GA_DELETE:GA_MORE;
}
static ga_result_t step(neo_gactor_t *a,neo_genv_t *e) {     /* 00427A */
    return ga_anim_step(a,e,NULL)<0?GA_UNSUPPORTED:GA_MORE;
}
static bool start(neo_gactor_t *a,neo_genv_t *e,uint32_t program,uint16_t palette,uint16_t layers,bool palette_first) {
    if (palette_first) ga_palette(a,palette);
    if (!ga_anim_start(a,e,program,0x100)) return false;
    if (!palette_first) ga_palette(a,palette);
    SW(0x66,layers);SW(0x1e,layers);
    return true;
}
/* ---- The mid-boss 05310E of Mission 6 stage 1 (script, camera 2352) ----
 * Manual translation (captures-m6/fam/B: run4 replay from sec09_292683,
 * 296800..303000; docs/MISION6.md "Jefe intermedio"). Node fields:
 * +4E life (rank 2CFF8A), +76 / +78 the next flash threshold and its step
 * (+4E / 5), +54 = 2D01BA, +6E the target (a player node, FFFFFFFF none),
 * +8B a player inside 052B3E, +89 the target ahead, +88 the aim angle, +8A
 * the side (FF: right), +84 airborne, +85 its hold, +72 the gravity, +62 the
 * walk speed, +7A a timer, +82 the voice delay, +83 the hit spark delay, +86
 * the hit tint, +87 no cull, +8C the jump bias, +8D the melee delay, +8E..
 * +91 the hits of the four parts 0539C4..0539EE, +92 frames without 004C24.
 * The cartridge check (10F00A/10F00C, 2FFFE0) before the deletions is not
 * reproduced: on genuine hardware it ends in 004188.
 * 009CCE (the hit tint 3BF..3C2 and the death fade toward 3C2 of the slot
 * +14) goes to e->palette_mix (neo_palmix.h).
 * The thrown objects 053B38 / 053CB2 (00411C after the script actor) are
 * outside the family of the capture (their creator is 101414). */
#define MID_P1 0x1014c4u
#define MID_P2 0x101574u
#define MID_NONE 0xffffffffu
static neo_gactor_t *mid_kid(neo_gactor_t *a,neo_genv_t *e,uint32_t r,uint16_t reserve) {   /* 0040BC */
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,r,reserve):e->spawn?e->spawn(e->ctx,a,r,false):NULL;
}
static neo_gactor_t *mid_after(neo_gactor_t *a,neo_genv_t *e,uint32_t r) {   /* 00411C (A1 = 101414) */
    return e->spawn?e->spawn(e->ctx,a,r,true):NULL;
}
/* 009CCE on the slot +14 of `owner` (neo_world.c g_palette_mix). */
static void mid_tint(neo_gactor_t *owner,neo_genv_t *e,uint16_t target,uint8_t weight) {if (e->palette_mix) e->palette_mix(e->ctx,owner,target,weight);}
static bool mid_live(const neo_genv_t *e,uint32_t n) {   /* 01AEDA: carry (true) while the player node is in play */
    if (n!=MID_P1 && n!=MID_P2) return false;
    return (e->players.active_mask>>(n==MID_P2?1u:0u))&1u;
}
static uint16_t mid_tw(const neo_genv_t *e,uint32_t n,unsigned o) {   /* a word of the target (a player node) */
    if (o==0x3c) return n==MID_P2?e->players.p2x:e->players.p1x;
    if (o==0x40) return n==MID_P2?e->players.p2y:e->players.p1y;
    const uint8_t *m=e->node?e->node(e->ctx,n):NULL;
    return m?(uint16_t)(m[o]<<8|m[o+1]):0;
}
/* 01AFFC with its A0: the nearest player inside `rect`; carry (true) when none. */
static bool mid_in(neo_gactor_t *a,neo_genv_t *e,uint32_t rect,uint32_t *n) {
    const uint8_t *r=ga_rom(rect);
    if (!r) {ga_fail(e,rect);return true;}
    int16_t q[4];for (unsigned i=0;i<4;i++) q[i]=(int16_t)(r[2*i]<<8|r[2*i+1]);
    neo_contact_actor_t c=ga_contact(a);
    neo_target_t t=neo_target_pick(&c,&e->players,q);
    if (t.valid) *n=t.player?MID_P2:MID_P1;
    return !t.valid;
}
/* 01AF9A: the nearest player in play (horizontally, P1 on ties); carry (true) when none. */
static bool mid_nearest(neo_gactor_t *a,neo_genv_t *e,uint32_t *n) {
    unsigned m=e->players.active_mask&3u;
    if (!m) return true;
    if (m==1u) {*n=MID_P1;return false;}
    if (m==2u) {*n=MID_P2;return false;}
    int16_t d1=(int16_t)(e->players.p1x-W(0x3c)),d2=(int16_t)(e->players.p2x-W(0x3c));
    if (d1<0) d1=(int16_t)-d1;
    if (d2<0) d2=(int16_t)-d2;
    *n=d2<d1?MID_P2:MID_P1;return false;
}
static bool mid_ground(neo_gactor_t *a,neo_genv_t *e) {ga_air(a,e);return (B(0x15)&0x40u)!=0;}   /* 052B5E: 0143E4, carry = +15 bit 6 */
static void mid_fall(neo_gactor_t *a,neo_genv_t *e) {   /* 052B7A */
    if (B(0x84)) {
        SL(0x34,L(0x34)+L(0x72));
        if ((int16_t)W(0x2c)<=0xd8) SB(0x85,0xff);
        if (B(0x85)&0x80u) SB(0x84,mid_ground(a,e)?0:0xff);
        else {SB(0x85,B(0x85)-1u);ga_move(a,e);}
        return;
    }
    ga_ground(a,e,NULL);                                                 /* 01456A */
    if (!(B(0x15)&0x40u)) {SL(0x34,L(0x34)+0xffffb000u);SB(0x84,0xff);}
}
static void mid_target(neo_gactor_t *a,neo_genv_t *e) {   /* 052BDA */
    if (B(0x8d)) SB(0x8d,B(0x8d)-1u);
    uint32_t n=0;bool none=true;
    if (!(L(0x6e)&0x80000000u)) {
        if (!mid_live(e,L(0x6e))) SL(0x6e,MID_NONE);
        else {
            bool c=mid_in(a,e,0x52b3e,&n);SB(0x8b,c?0:0xff);
            if (!c) SL(0x6e,n);
            none=false;
        }
    }
    if (none) {
        bool c=mid_in(a,e,0x52b3e,&n);SB(0x8b,c?0:0xff);
        if (!c) SL(0x6e,n);
        else if (!mid_nearest(a,e,&n)) SL(0x6e,n);
    }
    if (!(L(0x6e)&0x80000000u)) {
        int16_t d0=(int16_t)(mid_tw(e,L(0x6e),0x3c)-W(0x3c));
        SB(0x89,(uint8_t)((d0<0?0xffu:0u)^(B(0x15)&1u?0xffu:0u)));
    } else SB(0x89,0xff);
}
static bool mid_melee(neo_gactor_t *a,neo_genv_t *e) {   /* 052C78: carry */
    if ((uint16_t)(W(0x3c)-0x20u)>=0x100u || B(0x8d) || (L(0x6e)&0x80000000u)) return false;
    uint32_t n=L(0x6e);int16_t d0=(int16_t)(mid_tw(e,n,0x3c)-W(0x3c));
    if (!((d0<0?0xffu:0u)^(B(0x15)&1u?0xffu:0u))) return false;
    if (d0<0) d0=(int16_t)-d0;
    int16_t d1=(int16_t)(mid_tw(e,n,0x2c)-W(0x2c));
    if (d1<0) return false;
    d1=(int16_t)(d1-d0-0x10);
    return (uint16_t)d1<0x20u;
}
static bool mid_grab(neo_gactor_t *a) {   /* 052CDA: carry */
    return (uint16_t)(W(0x3c)-0x20u)<0x100u && !B(0x8d) && (B(0x89)&B(0x8b));
}
static bool mid_edge(neo_gactor_t *a) {   /* 052D0A: carry */
    return (L(0x30)&0x80000000u)?(int16_t)W(0x3c)<0x30:(int16_t)W(0x3c)>=0x110;
}
static bool mid_jump(neo_gactor_t *a,neo_genv_t *e,uint8_t part) {   /* 052D36 (part +90) / 052E88 (+91): carry */
    if (L(0x6e)&0x80000000u) return false;
    if (part==0x90u?(int16_t)W(0x2c)>=0x138:(int16_t)W(0x2c)<=0xd8) return false;
    if (B(part)) return false;
    if (B(0x8b) && (B(0x8e)|B(0x8f))) return true;
    return (uint8_t)(ga_rand(e)&0xfu)<B(0x8c);
}
static bool mid_shoot(neo_gactor_t *a,neo_genv_t *e) {   /* 052D82: carry */
    if (B(0x82)) SB(0x82,B(0x82)-1u);
    if ((L(0x6e)&0x80000000u) || !B(0x89)) return false;
    uint32_t n=L(0x6e);
    int16_t d0=(int16_t)(W(0x40)-mid_tw(e,n,0x40));
    if (d0<=0) return false;
    int16_t d1=(int16_t)(W(0x3c)-mid_tw(e,n,0x3c));if (d1<0) d1=(int16_t)-d1;
    if ((uint16_t)(d1+d1)<(uint16_t)d0) return false;
    int16_t dx=(int16_t)((int16_t)(ga_rand(e)&0x1fu)-0x10+(int16_t)mid_tw(e,n,0x3c)-(int16_t)W(0x3c));
    int16_t dy=(int16_t)(mid_tw(e,n,0x40)-W(0x40));
    SB(0x88,neo_angle(dx,dy));                                           /* 002EB4 */
    return true;
}
static bool mid_kick(neo_gactor_t *a,neo_genv_t *e) {   /* 052E06: carry */
    if (B(0x92)) return false;
    int16_t x=(int16_t)W(0x3c);
    if (x<0x20 || x>=0x300) return false;
    return ga_attack(a,e,0x52ad6)!=0;
}
static bool mid_parts(neo_gactor_t *a) {   /* 052ED4: carry */
    if (B(0x92)) return false;
    return B(0x8a)?B(0x8e)!=0:B(0x8f)!=0;
}
static void mid_speed(neo_gactor_t *a) {SL(0x62,B(0x8a)?0xfffa0000u:0x60000u);}   /* 05323E */
static bool mid_start(neo_gactor_t *a,neo_genv_t *e,uint32_t p) {return ga_anim_start(a,e,p,0x100);}
static void mid_flash(neo_gactor_t *a,neo_genv_t *e) {   /* 053DCC */
    if (B(0x86)<0x20u) SB(0x86,B(0x86)+7u);
    uint16_t d1;
    switch (B(0x86)) {case 7: d1=0x3bf;break; case 0xe: d1=0x3c0;break; case 0x15: d1=0x3c1;break; case 0x1c: d1=0x3c2;break;
    default: if (e->external) e->external(e->ctx,0x9cce);return;}   /* D1 of the caller 052F5E (not reached in captures-m6/fam/B) */
    mid_tint(a,e,d1,0x20);
}
static ga_result_t mid_end(neo_gactor_t *a,neo_genv_t *e) {   /* 052F8A */
    SB(0x69,0xff);                                                       /* 019DFC */
    if (e->flags) {e->flags->scroll_lock=0;e->flags->player_handlers=0;e->flags->player_lock=0;e->flags->m6_signal=0;}   /* 10C00B, 10C00C, 0CA2F2 x2, 0C8CE6, 10C44C */
    if (e->kernel) e->kernel->b400c=0;
    if (e->camera_mode) e->camera_mode(e->ctx,0);                        /* 0C7B48 */
    return GA_DELETE;                                                    /* (cartridge check) 004188 */
}
static ga_result_t mid_cull(neo_gactor_t *a,neo_genv_t *e);
/* 052F04: the common tail of the boss's states. */
static ga_result_t mid_tail(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x83)) SB(0x83,B(0x83)-1u);
    if (!B(0x92)) ga_register(a,e,NULL,NULL); else SB(0x92,B(0x92)-1u);   /* 004C24 */
    if (B(0x5a)) {
        SB(0x5a,0);
        if (!B(0x83)) {SB(0x15,B(0x15)^1u);ga_spawn_from(a,e,0x40f88,8,0);SB(0x15,B(0x15)^1u);SB(0x83,0x0c);}   /* 040ED0 */
        if (W(0x4e)<W(0x76)) {SW(0x76,W(0x76)-W(0x78));mid_flash(a,e);}
    }
    if (B(0x5f)&0x80u) SL(0,0x536f4);
    return mid_cull(a,e);
}
static ga_result_t mid_cull(neo_gactor_t *a,neo_genv_t *e) {   /* 052F72 */
    if (e->unsupported) return GA_UNSUPPORTED;
    if (B(0x87)) return GA_MORE;
    if (ga_cull_rect(a,e,0x52b46)) return mid_end(a,e);                  /* 019504 */
    return GA_MORE;
}
static int mid_step(neo_gactor_t *a,neo_genv_t *e);
/* The decisions of 0532F6 in their order (the last that holds wins). */
static void mid_decide(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0x6e)&0x80000000u) SL(0,0x534a8);
    if (!B(0x89)) SL(0,0x533a2);
    if (mid_melee(a,e)) SL(0,0x5345e);
    if (mid_grab(a)) SL(0,0x5344a);
    if (mid_kick(a,e)) SL(0,0x53698);
    if (mid_shoot(a,e)) SL(0,0x533ee);
    if (mid_edge(a)) SL(0,0x533e2);
    if (mid_jump(a,e,0x90)) SL(0,0x535ae);
    if (mid_jump(a,e,0x91)) SL(0,0x53658);
    if (mid_parts(a)) SL(0,0x533e2);
    if (B(0x84)) SL(0,0x53520);
}
/* The jumps 053520 (-> 05358A / 0535AE / the upward one), 0535AE, 053658:
 * program 249E38, +30/+34/+72, then 05362A. */
static ga_result_t mid_leap(neo_gactor_t *a,neo_genv_t *e,uint32_t at) {
    if (at==0x53520u) {
        if ((int16_t)W(0x2c)<0x138 && !(L(0x6e)&0x80000000u)) at=mid_tw(e,L(0x6e),0x40)<W(0x40)?0x535aeu:0x5358au;
        else if (B(0x8b)) at=0x5358a;
    }
    if (!mid_start(a,e,0x249e38)) return GA_UNSUPPORTED;
    if (at==0x53520u) {                                                  /* 05354C */
        SL(0x34,0x88885);SL(0x72,0xfffedcbbu);SL(0x30,B(0x8a)?0xfffaeeefu:0x51111u);SB(0x84,0xff);
    } else if (at==0x5358au) {
        SL(0x30,0);SL(0x34,0);SL(0x72,0xffffb000u);
    } else if (at==0x53658u) {
        SL(0x30,0);SL(0x34,0x92492);SL(0x72,0xfffd6344u);SB(0x84,0xff);SB(0x85,6);
    } else {                                                             /* 0535AE */
        uint32_t d1=(0x1111u*0xf0u)>>8;SL(0x34,(uint32_t)(int32_t)(int16_t)(uint16_t)d1<<8);
        d1=(0x1111u*0x180u)>>8;d1=0x1111u*(uint16_t)d1;
        SL(0x72,(uint32_t)(int32_t)(int16_t)(uint16_t)(0u-(uint16_t)(d1>>16))<<8);
        int16_t x=(int16_t)((int16_t)W(0x2c)>=0x108?0x60:0x30);
        if ((int16_t)W(0x3c)>=0xa0) x=(int16_t)(0x140-x);
        x=(int16_t)(x-(int16_t)W(0x3c));
        SL(0x30,(uint32_t)((int32_t)x*0x11)<<8);SB(0x84,0xff);
    }
    SL(0,0x5362a);
    return GA_MORE;
}
/* The boss's program callbacks. */
static bool mid_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    neo_gactor_t *c;
    switch (routine) {
    case 0x5304a: mid_kid(a,e,0x53a48,0x2a);return true;
    case 0x5305c: mid_kid(a,e,0x53a5c,0x2a);return true;
    case 0x5306e: case 0x530aa:                                          /* 00411C (D0 = E): 053CB2 */
        if ((c=mid_after(a,e,0x53cb2))) {
            ga_sb(c,0x80,routine==0x5306eu?(B(0x15)&1u?0:0x80):(B(0x15)&1u?0x20:0x60));
            if (routine==0x5306eu) ga_place_from(c,a,e,(int16_t)0xffc8,0x18); else ga_place_from(c,a,e,(int16_t)0xffd8,0x40);
        }
        if (e->sound) e->sound(e->ctx,0x10f4);
        return true;
    case 0x52e3e:                                                        /* 00411C (D0 = 1C): 053B38 */
        if ((c=mid_after(a,e,0x53b38))) {
            ga_place_from(c,a,e,0,0x10);
            ga_sb(c,0x80,!(L(0x6e)&0x80000000u)?B(0x88):(B(0x15)&1u?0:0x80));
        }
        return true;
    case 0x53dbc: ga_attack(a,e,0x52ae6);return true;
    case 0x53e20:                                                        /* 08CDB2 (0, 0, kind 49, 0, 0): +34 = 20000 */
        if ((c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL)) {
            ga_sb(c,0x8b,0);ga_sw(c,0x70,0);ga_sw(c,0x6e,0x49);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);ga_place_at(c,a,e,0,0);ga_sl(c,0x34,0x20000);
        }
        return true;
    }
    return false;
}
static int mid_step(neo_gactor_t *a,neo_genv_t *e) {return ga_anim_step(a,e,mid_call);}   /* 00427A */
static neo_gactor_t *mid_from(neo_gactor_t *a,neo_genv_t *e,uint32_t r,int16_t dx) {   /* 0040BC (D0 = 1C), 01B220 (dx, 0) */
    neo_gactor_t *c=mid_kid(a,e,r,0x1c);
    if (c) ga_place_from(c,a,e,dx,0);
    return c;
}
static void mid_front(neo_gactor_t *a,neo_genv_t *e,bool back) {   /* 012022 (back: 012040, the inverse) */
    if (!e->front_layer) {ga_fail(e,back?0x12040:0x12022);return;}
    if (e->front_layer(e->ctx)!=back) SB(0x15,B(0x15)|4u); else SB(0x15,B(0x15)&0xfbu);
}
static ga_result_t mid_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x5310e: {
        const uint8_t *t=ga_rom(0x2cff8a);
        if (!t) {ga_fail(e,0x2cff8a);return GA_UNSUPPORTED;}
        uint16_t d0=ga_rank_word(a,e,t);
        if (!d0) {ga_fail(e,0x53126);return GA_UNSUPPORTED;}              /* trap #F */
        SW(0x4e,d0);SW(0x78,(uint16_t)(d0/5u));SW(0x76,(uint16_t)(d0-d0/5u));
        SL(0x54,0x2d01ba);SW(0x66,0);SW(0x1e,0);
        ga_palette(a,0x38e);SB(0x86,0);
        mid_kid(a,e,0x539e0,0x1c);mid_kid(a,e,0x539ee,0x1c);mid_kid(a,e,0x539c4,0x1c);mid_kid(a,e,0x539d2,0x1c);
        SB(0x8d,0);SB(0x8c,0);SB(0x82,0);SB(0x83,0);SB(0x87,0xff);SB(0x8a,0xff);
        if (W(0x28)<0x8c0u) {SB(0x15,B(0x15)|1u);SB(0x8a,0);}
        mid_target(a,e);SL(0,0x531ec);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x531ec:
        SL(0x50,0);
        if (!mid_start(a,e,0x249d10)) return GA_UNSUPPORTED;
        mid_speed(a);SL(0x30,L(0x62));SL(0,0x53214);
        /* fall through */
    case 0x53214:
        ga_move(a,e);
        if (mid_step(a,e)<0) return GA_UNSUPPORTED;
        if ((uint16_t)(W(0x3c)-0x30u)<0xe0u) {SB(0x87,0);SL(0,0x53254);}
        return mid_tail(a,e);
    case 0x53254:
        SW(0x66,0x8820);SW(0x1e,0x8820);
        if (!mid_start(a,e,0x24a014)) return GA_UNSUPPORTED;
        SW(0x7a,0x5a);SL(0x62,0);SL(0,0x53280);
        /* fall through */
    case 0x53280:
        SB(0x92,0x1e);mid_target(a,e);ga_ground(a,e,NULL);
        if (mid_step(a,e)<0) return GA_UNSUPPORTED;
        SW(0x7a,W(0x7a)-1u);
        if (!W(0x7a)) {SL(0x50,0x52a8e);SB(0x92,1);SL(0,0x532b6);}
        return mid_tail(a,e);
    case 0x533e2:
        SB(0x8c,B(0x8c)+1u);SB(0x8a,(uint8_t)~B(0x8a));
        /* fall through */
    case 0x532b6:
        mid_speed(a);
        if (!mid_start(a,e,(B(0x8a)^(B(0x15)&1u?0xffu:0u))?0x249d10:0x249d84)) return GA_UNSUPPORTED;
        SL(0,0x532f6);
        /* fall through */
    case 0x532f6:
        mid_target(a,e);mid_fall(a,e);
        if (mid_step(a,e)<0) return GA_UNSUPPORTED;
        mid_decide(a,e);
        return mid_tail(a,e);
    case 0x533a2:
        SB(0x82,0);
        if (!mid_start(a,e,0x249dd0)) return GA_UNSUPPORTED;
        SL(0x62,0);SL(0,0x533c0);
        /* fall through */
    case 0x533c0: {
        mid_target(a,e);mid_fall(a,e);
        int r=mid_step(a,e);if (r<0) return GA_UNSUPPORTED;
        if (r>0) {SB(0x15,B(0x15)^1u);SL(0,0x532b6);}
        return mid_tail(a,e);
    }
    case 0x533ee:
        if (!B(0x82)) {
            SB(0x82,0x1e);
            const uint8_t *s=ga_rom(0x52b56u+(ga_rand(e)&6u));
            if (!s) {ga_fail(e,0x52b56);return GA_UNSUPPORTED;}
            if (e->sound) e->sound(e->ctx,(uint16_t)(s[0]<<8|s[1]));          /* 0120F8 */
        }
        if (!mid_start(a,e,0x249fb2)) return GA_UNSUPPORTED;
        SL(0x62,0);SL(0,0x5342e);
        /* fall through */
    case 0x5342e: {
        mid_target(a,e);mid_fall(a,e);
        int r=mid_step(a,e);if (r<0) return GA_UNSUPPORTED;
        if (r>0) SL(0,0x532b6);
        return mid_tail(a,e);
    }
    case 0x5344a: case 0x5345e:
        if (!mid_start(a,e,L(0)==0x5344au?0x249a40:0x249b42)) return GA_UNSUPPORTED;
        SB(0x82,0);SL(0x62,0);SL(0,0x5347c);
        /* fall through */
    case 0x5347c: {
        mid_target(a,e);mid_fall(a,e);
        int r=mid_step(a,e);if (r<0) return GA_UNSUPPORTED;
        if (r>0) {
            const uint8_t *t=ga_rom(0x2cffec);
            if (!t) {ga_fail(e,0x2cffec);return GA_UNSUPPORTED;}
            SB(0x8d,(uint8_t)ga_rank_word(a,e,t));SL(0,0x532b6);
        }
        return mid_tail(a,e);
    }
    case 0x534a8:
        SL(0x62,0);
        if (!mid_start(a,e,0x24a014)) return GA_UNSUPPORTED;
        SL(0,0x534c2);
        /* fall through */
    case 0x534c2:
        mid_target(a,e);mid_fall(a,e);
        if (mid_step(a,e)<0) return GA_UNSUPPORTED;
        if (!(L(0x6e)&0x80000000u)) SL(0,0x532b6);
        return mid_tail(a,e);
    case 0x534e2:
        SB(0x82,0);
        if (!mid_start(a,e,0x249e80)) return GA_UNSUPPORTED;
        SB(0x8c,0);SL(0x62,0);SL(0,0x53504);
        /* fall through */
    case 0x53504: {
        mid_target(a,e);mid_fall(a,e);
        int r=mid_step(a,e);if (r<0) return GA_UNSUPPORTED;
        if (r>0) SL(0,0x532b6);
        return mid_tail(a,e);
    }
    case 0x53520: case 0x535ae: case 0x53658: {
        ga_result_t r=mid_leap(a,e,L(0));
        if (r!=GA_MORE) return r;
    }   /* fall through */
    case 0x5362a:
        mid_target(a,e);mid_fall(a,e);
        if (mid_step(a,e)<0) return GA_UNSUPPORTED;
        if (!B(0x84)) SL(0,0x534e2);
        if (mid_kick(a,e)) SL(0,0x53698);
        return mid_tail(a,e);
    case 0x53698:
        SB(0x82,0);
        if (!mid_start(a,e,0x24a0be)) return GA_UNSUPPORTED;
        SL(0,0x536b2);
        /* fall through */
    case 0x536b2: {
        mid_target(a,e);mid_fall(a,e);
        int r=mid_step(a,e);if (r<0) return GA_UNSUPPORTED;
        if (r>0) SL(0,0x532b6);
        if (!B(0x84)) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}                   /* 01B1DC */
        else {SL(0x62,(uint32_t)((int32_t)L(0x62)>>1));SL(0x30,(uint32_t)((int32_t)L(0x30)>>1));}
        return mid_tail(a,e);
    }
    /* ---- the death ---- */
    case 0x536f4:
        SB(0x15,B(0x15)^1u);ga_spawn_from(a,e,0x40f60,8,0);SB(0x15,B(0x15)^1u);   /* 040F04 */
        mid_kid(a,e,0x5390c,0x1c);
        if (B(0x84)) {
            if (!mid_start(a,e,0x249f6a)) return GA_UNSUPPORTED;
            SL(0x30,0);SL(0x34,0x40000);SL(0x72,0xffff0000u);SL(0,0x537ae);
            goto rise;
        }
        if (!mid_start(a,e,0x249ec8)) return GA_UNSUPPORTED;
    dead:
        if (e->flags) e->flags->kill_request=0xff;                       /* 004BFE */
        SB(0x69,0xff);                                                   /* 019DFC */
        if (e->camera_mode) e->camera_mode(e->ctx,3);                    /* 0C7B48 */
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                                /* 01B1DC */
        SB(0x87,0xff);SL(0,0x53748);
        /* fall through */
    case 0x53748: {
        if (e->camera_rise) e->camera_rise(e->ctx,0);
        if (e->camera_push) e->camera_push(e->ctx,0);                    /* 10E95C / 10E960 */
        if (e->flags) e->flags->guard_min=0x0f;                          /* 0537E8: 0C09DA */
        SB(0x92,2);mid_fall(a,e);
        int r=mid_step(a,e);if (r<0) return GA_UNSUPPORTED;
        if (r>0) SL(0,0x537f4);
        return mid_cull(a,e);
    }
    case 0x537ae:
    rise:
        SB(0x92,2);mid_fall(a,e);
        if (mid_step(a,e)<0) return GA_UNSUPPORTED;
        if (!B(0x84)) SL(0,0x537d0);
        return mid_cull(a,e);
    case 0x537d0:
        if (!mid_start(a,e,0x249f8a)) return GA_UNSUPPORTED;
        goto dead;
    case 0x537f4:
        if (e->flags) {e->flags->player_lock=0xff;e->flags->flash_request=0xff;e->flags->scroll_lock=0;e->flags->player_handlers=5;}   /* 0C8CD6 (-> 004C06) */
        if (e->kernel) e->kernel->b400c=0xff;   /* 0C8CD6, 10C00B, 10C00C, 0CA2E0 (0537E4) x2 */
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                                /* 01B1DC */
        SW(0x7e,e->camera_y);SW(0x2c,W(0x2c)-0x18u);
        if (!mid_start(a,e,0x249f4c)) return GA_UNSUPPORTED;
        mid_kid(a,e,0x5439e,0);SB(0x60,0);SL(0,0x5385c);
        /* fall through */
    case 0x5385c:
        if (e->flags) e->flags->guard_min=0x0f;                          /* 0C09DA */
        if (e->camera_rise) e->camera_rise(e->ctx,(int32_t)0xfff20000);
        if (e->camera_push) e->camera_push(e->ctx,0);
        SL(0x34,L(0x34)+0xffffd000u);
        if (B(0x60)) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
        else {ga_move(a,e);if (mid_step(a,e)<0) return GA_UNSUPPORTED;}
        if (!(B(0x60)&0x80u)) return mid_cull(a,e);
        SL(0x30,0);SL(0x34,0);SL(0x62,0);SB(0x81,0x3c);SL(0,0x538ae);
        /* fall through */
    case 0x538ae:
        ga_project(a,e);SB(0x81,B(0x81)-1u);
        if (B(0x81)) return mid_cull(a,e);
        SW(0x7a,0x12c);SL(0,0x538c8);
        /* fall through */
    case 0x538c8: {
        if (e->flags) e->flags->guard_min=0x0f;                          /* 0C09DA */
        int16_t d0=(int16_t)(W(0x7e)-e->camera_y);
        if (d0>4) d0=4; else SL(0,0x52f8a);
        if (e->camera_rise) e->camera_rise(e->ctx,(int32_t)((uint32_t)(uint16_t)d0<<16));
        if (e->camera_push) e->camera_push(e->ctx,0);
        SW(0x7a,W(0x7a)-1u);
        if ((int16_t)W(0x7a)<0) SL(0,0x52f8a);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x52f8a: return mid_end(a,e);
    /* ---- the death fade 05390C ---- */
    case 0x5390c: SB(0x7c,0x20);SL(0,0x53918);
        /* fall through */
    case 0x53918:
        SB(0x7c,B(0x7c)-1u);
        if (B(0x7c)&0x80u) return GA_DELETE;                             /* (cartridge check) 004188 */
        {neo_gactor_t *boss=e->actor?e->actor(e->ctx,L(0x0c)):NULL;   /* 0539AE: +C, its +14 */
         if (boss) mid_tint(boss,e,0x3c2,B(0x7c));}
        return GA_MORE;
    /* ---- the four parts 0539C4 / 0539D2 / 0539E0 / 0539EE ---- */
    case 0x539c4: case 0x539d2: case 0x539e0: case 0x539ee: {
        static const uint32_t box[]={0x52aa6,0x52ab2,0x52abe,0x52aca};static const uint8_t flag[]={0x90,0x91,0x8e,0x8f};
        unsigned k=(L(0)-0x539c4u)/14u;
        SL(0x50,box[k]);SW(0x7c,flag[k]);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x53a00);return GA_UNSUPPORTED;}
        ga_place(a,e,0,0,NEO_GA_TAG(p->index));                          /* 01AD6E */
        SL(0,0x53a0c);
    }   /* fall through */
    case 0x53a0c: {
        if (!ga_creator_alive(a,e)) return GA_DELETE;                    /* 00409A */
        ga_project(a,e);ga_register(a,e,NULL,NULL);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x53a28);return GA_UNSUPPORTED;}
        ga_sb(p,(unsigned)W(0x7c)&0xffu,ga_b(p,0x83)?0:(B(0x5a)?0xff:0));SB(0x5a,0);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* ---- 053A48 / 053A5C: the dust of the landing and the kick ---- */
    case 0x53a48: case 0x53a5c: {
        if (!mid_start(a,e,L(0)==0x53a48u?0x249af0:0x249c1a)) return GA_UNSUPPORTED;
        ga_palette(a,0x38f);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x53a7a);return GA_UNSUPPORTED;}
        SB(0x15,(uint8_t)(ga_b(p,0x15)&3u));                             /* 01ACA2 */
        ga_place(a,e,0,0,NEO_GA_TAG(p->index));SL(0,0x53a90);            /* 01AD6E */
    }   /* fall through */
    case 0x53a90: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);if (r<0) return GA_UNSUPPORTED;
        return r>0?GA_DELETE:GA_MORE;                                    /* (cartridge check) 004188 */
    }
    /* ---- the fall into the water: 05439E and its children ---- */
    case 0x5439e: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x5439e);return GA_UNSUPPORTED;}
        ga_place(a,e,(int16_t)ga_w(p,0x28),0x1c,0x10c72e);                /* 01AB98 */
        SB(0x15,(uint8_t)(ga_b(p,0x15)&3u));SW(0x66,0);SW(0x1e,0);SL(0,0x543ce);   /* 01ACA2 */
    }   /* fall through */
    case 0x543ce: {
        ga_project(a,e);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x543d4);return GA_UNSUPPORTED;}
        if ((int16_t)ga_w(p,0x2c)>0xde) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        mid_from(a,e,0x53f0a,0);
        static const int16_t dx[]={0,0x14,-0x14,0x28,-0x28,0x3c,-0x3c};
        for (unsigned i=0;i<7;i++) mid_from(a,e,0x540ea,dx[i]);
        mid_from(a,e,0x542b0,0);
        SB(0x61,0);SW(0x2c,W(0x2c)-0xb0u);ga_palette(a,0x13c);
        if (!mid_start(a,e,0x24a20a)) return GA_UNSUPPORTED;
        SL(0x30,0);SL(0x34,0xb0000);SL(0,0x5456c);
    }   /* fall through */
    case 0x5456c: {
        SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);
        if ((int32_t)L(0x2c)>0x1c0000) SL(0x2c,0x1c0000);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x5458e);return GA_UNSUPPORTED;}
        if ((int16_t)(ga_w(p,0x7e)-e->camera_y)>=0x30) {if (mid_step(a,e)<0) return GA_UNSUPPORTED;SB(0x61,1);}
        if ((int16_t)(ga_w(p,0x2c)-W(0x2c))<=0x50) {ga_sb(p,0x60,1);SL(0,0x545d6);}
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x545d6:
        if (e->sound) e->sound(e->ctx,0x1161);
        SB(0x61,0xff);
        if (!mid_start(a,e,0x24a21e)) return GA_UNSUPPORTED;
        SL(0,0x545fa);
        /* fall through */
    case 0x545fa:
        SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);
        if ((int32_t)L(0x2c)>0x1c0000) SL(0x2c,0x1c0000);
        if (mid_step(a,e)<0) return GA_UNSUPPORTED;
        if ((int16_t)W(0x2c)>(int16_t)0xff6c) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        SL(0x2c,0x1c0000);SW(0x7a,0x28);SL(0,0x54642);
        /* fall through */
    case 0x54642:
        ga_project(a,e);SW(0x7a,W(0x7a)-1u);
        if (!(W(0x7a)&0x8000u)) return e->unsupported?GA_UNSUPPORTED:GA_MORE;
        SW(0x7a,0x3c);
        {   uint16_t pal=neo_gate_splash_palette(e);                     /* 019240 D1 */
            if (!pal) {ga_fail(e,0x19240);return GA_UNSUPPORTED;}
            ga_palette(a,pal);}
        if (!mid_start(a,e,0x22e064)) return GA_UNSUPPORTED;
        mid_front(a,e,false);SL(0,0x54684);
        /* fall through */
    case 0x54684: {
        ga_project(a,e);SW(0x7a,W(0x7a)-1u);
        if (W(0x7a)&0x8000u) {
            neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
            if (p) ga_sb(p,0x60,0xff);
            return GA_DELETE;
        }
        if (!(W(0x7a)&3u)) mid_from(a,e,0x541f2,0);
        int r=ga_anim_step(a,e,NULL);if (r<0) return GA_UNSUPPORTED;
        if (r>0 && !mid_start(a,e,0x22e064)) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x53f0a:
        if (e->sound) e->sound(e->ctx,0x1052);
        {neo_gactor_t *c=mid_from(a,e,0x53e40,0);if (c) {ga_sw(c,0x66,0xfff1);ga_sw(c,0x1e,0xfff1);}}
        SW(0x7a,2);SL(0,0x53f52);
        /* fall through */
    case 0x53f52: case 0x53fa0: case 0x54054: case 0x540a2:
    splash_wait: {
        ga_project(a,e);SW(0x7a,W(0x7a)-1u);
        if (!(W(0x7a)&0x8000u)) return GA_MORE;
        uint32_t r=L(0);
        neo_gactor_t *c=mid_from(a,e,0x53e40,r==0x53fa0u||r==0x540a2u?-0x30:0x30);
        if (c) {ga_sw(c,0x66,0xfff1);ga_sw(c,0x1e,0xfff1);}
        if (r==0x540a2u) return GA_DELETE;                               /* 004188 */
        if (r==0x53fa0u) {SL(0,0x53fe8);goto splash_dive;}
        SW(0x7a,2);SL(0,r==0x53f52u?0x53fa0u:0x540a2u);
        goto splash_wait;
    }
    case 0x53fe8:
    splash_dive: {
        ga_project(a,e);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x53fee);return GA_UNSUPPORTED;}
        if (!(ga_l(p,0x34)&0x80000000u) || (int16_t)ga_w(p,0x2c)>-4) return GA_MORE;
        if (e->sound) e->sound(e->ctx,0x1051);
        neo_gactor_t *c=mid_from(a,e,0x53e40,0);if (c) {ga_sw(c,0x66,0xfff1);ga_sw(c,0x1e,0xfff1);}
        SW(0x7a,2);SL(0,0x54054);goto splash_wait;
    }
    case 0x53e40:
        mid_front(a,e,false);ga_palette(a,0x166);
        if (!mid_start(a,e,0x22e1f0)) return GA_UNSUPPORTED;
        SL(0,0x53e6a);
        /* fall through */
    case 0x53e6a: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);if (r<0) return GA_UNSUPPORTED;
        return r>0?GA_DELETE:GA_MORE;                                    /* (cartridge check) 004188 */
    }
    case 0x540ea:
        mid_front(a,e,false);ga_palette(a,0x165);
        if (!mid_start(a,e,0x24a306)) return GA_UNSUPPORTED;
        SW(0x66,0xfff0);SW(0x1e,0xfff0);SW(0x7a,0x28);SL(0,0x54126);
        /* fall through */
    case 0x54126:
        ga_project(a,e);SW(0x7a,W(0x7a)-1u);
        if (!(W(0x7a)&0x8000u)) return GA_MORE;
        SW(0x2c,W(0x2c)+4u);SW(0x7a,0x28);SL(0,0x54148);
        /* fall through */
    case 0x54148:
        SW(0x7a,W(0x7a)-1u);
        if (W(0x7a)&0x8000u) return GA_DELETE;                           /* (cartridge check) 004188 */
        ga_project(a,e);
        return ga_anim_step(a,e,NULL)<0?GA_UNSUPPORTED:GA_MORE;
    case 0x541f2:
        mid_front(a,e,true);ga_palette(a,0x3fd);
        if (!mid_start(a,e,0x269340)) return GA_UNSUPPORTED;
        SW(0x66,0xfff1);SW(0x1e,0xfff1);SB(0x15,B(0x15)|0x20u);SB(0x7c,0);
        SL(0x30,(uint32_t)(int32_t)(int16_t)(uint16_t)(ga_rand_range(e,0x100)+0x30u)<<8);
        SL(0x34,(uint32_t)(int32_t)(int16_t)(uint16_t)(ga_rand_range(e,0x400)+0x400u)<<8);
        if (ga_rand(e)&0x80u) {SB(0x15,B(0x15)|1u);SL(0x30,0u-L(0x30));}
        SL(0,0x54274);
        /* fall through */
    case 0x54274:
        SL(0x34,L(0x34)+0xffffb000u);ga_move(a,e);
        if (!B(0x7c) && neo_gate_splash(a,e,B(0x7d)?1:0)) SB(0x7c,0xff);   /* 03C5C6 */
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if ((int16_t)W(0x40)<0x100) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x542b0: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x542b0);return GA_UNSUPPORTED;}
        ga_place(a,e,0,0,NEO_GA_TAG(p->index));ga_palette(a,0x13c);       /* 01AD6E */
        SW(0x66,0xffef);SW(0x1e,0xffef);
        if (!mid_start(a,e,0x24a214)) return GA_UNSUPPORTED;
        SL(0,0x542ea);
    }   /* fall through */
    case 0x542ea: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x542ea);return GA_UNSUPPORTED;}
        if (ga_b(p,0x61)&0x80u) return GA_DELETE;                        /* (cartridge check) 004188 */
        ga_project(a,e);
        if (ga_b(p,0x61) && ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    /* ---- 053B38: the grenade (00411C, +80 = angle) ---- */
    case 0x53b38: {
        SB(0x15,B(0x15)|0x20u);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);
        const uint8_t *r=ga_rom(0x2d00ae);
        if (!r) {ga_fail(e,0x2d00ae);return GA_UNSUPPORTED;}
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)(int16_t)ga_rank_word(a,e,r)<<8,B(0x80),&vx,&vy);   /* 002E7C */
        SL(0x30,vx);SL(0x34,vy);
        ga_palette(a,0x35d);ga_palette(a,0x364);ga_palette(a,0x365);
        if (!mid_start(a,e,0x24a260)) return GA_UNSUPPORTED;
        SL(0x50,0x52a9a);SW(0x7a,3);SL(0,0x53bbc);
    }   /* fall through */
    case 0x53bbc: {
        if (mid_ground(a,e)) SL(0,0x53c18);                              /* 052B5E */
        SW(0x7a,W(0x7a)-1u);
        if ((W(0x7a)&0x8000u) && ga_attack(a,e,0x52afa)) goto blast;
        const uint8_t *s=ga_rom(0x53b30u+((e->video&3u)<<1));
        if (!s) {ga_fail(e,0x53b30);return GA_UNSUPPORTED;}
        SB(0x14,B((unsigned)(s[0]<<8|s[1])&0xffu));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_cull_rect(a,e,0x52b36)) return GA_DELETE_TREE;            /* 019504 -> 0041AC */
        if ((B(0x15)&0x40u) && e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));   /* 00505A */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x53c18:
    blast:
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        if (e->sound) e->sound(e->ctx,0x1027);
        SL(0x50,0);SB(0x5a,0);SB(0x5f,B(0x5f)&0x7fu);SB(0x1d,0);         /* 009F88 */
        ga_palette(a,0x3e0);
        if (!mid_start(a,e,0x22cb60)) return GA_UNSUPPORTED;
        ga_attack(a,e,0x52b0e);ga_attack(a,e,0x52b22);SL(0,0x53c80);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x53c80: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);if (r<0) return GA_UNSUPPORTED;
        return r>0?GA_DELETE_TREE:GA_MORE;                               /* 0041AC */
    }
    /* ---- 053CB2: the knife (00411C, +80 = angle) ---- */
    case 0x53cb2: {
        ga_palette(a,0x22);ga_palette(a,0x178);ga_palette(a,0x179);
        if (!mid_start(a,e,0x244dbc)) return GA_UNSUPPORTED;
        const uint8_t *r=ga_rom(0x2d0110);
        if (!r) {ga_fail(e,0x2d0110);return GA_UNSUPPORTED;}
        uint32_t vx,vy;ga_polar((uint32_t)(int32_t)(int16_t)ga_rank_word(a,e,r)<<8,B(0x80),&vx,&vy);
        SL(0x30,vx);SL(0x34,vy);SL(0,0x53d16);
    }   /* fall through */
    case 0x53d16:
        ga_air(a,e);                                                     /* 0143E4 */
        if (!(B(0x15)&0x40u) && !(B(0x5f)&1u)) {
            if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
            if (!ga_attack(a,e,0x53c9e)) goto knife_bounds;
        }
        if (!mid_start(a,e,L(0x34)?0x24a1ae:0x24a15c)) return GA_UNSUPPORTED;
        SL(0x30,0);SL(0x34,0);SB(0x14,B(0x19));SL(0,0x53dae);
        /* fall through */
    case 0x53dae: {
        int r=ga_anim_step(a,e,NULL);if (r<0) return GA_UNSUPPORTED;
        if (r>0) return GA_DELETE;
    }
    knife_bounds:
        if ((uint16_t)(W(0x3c)+0x10u)>=0x160u || (uint16_t)(W(0x40)-0xf0u)>=0x120u) return GA_DELETE;
        if ((B(0x15)&0x40u) && e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));   /* 00505A */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static bool mid_routine(uint32_t r) {
    static const uint32_t k[]={0x52f8a,0x5310e,0x531ec,0x53214,0x53254,0x53280,0x532b6,0x532f6,0x533a2,0x533c0,0x533e2,0x533ee,0x5342e,0x5344a,0x5345e,0x5347c,0x534a8,0x534c2,0x534e2,0x53504,0x53520,0x535ae,0x5362a,0x53658,0x53698,0x536b2,0x536f4,0x53748,0x537ae,0x537d0,0x537f4,0x5385c,0x538ae,0x538c8,0x5390c,0x53918,0x539c4,0x539d2,0x539e0,0x539ee,0x53a0c,0x53a48,0x53a5c,0x53a90,0x53b38,0x53bbc,0x53c18,0x53c80,0x53cb2,0x53d16,0x53dae,0x53e40,0x53e6a,0x53f0a,0x53f52,0x53fa0,0x53fe8,0x54054,0x540a2,0x540ea,0x54126,0x54148,0x541f2,0x54274,0x542b0,0x542ea,0x5439e,0x543ce,0x5456c,0x545d6,0x545fa,0x54642,0x54684};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return false;
}
/* ---- 02011C / 020166: the runner of the script stop 2DFAD4 (its rider 020372) ----
 * 02011C / 020166: +15 bit 0 set, program 22FD1C / 22FE22 (004256), +28 +=
 * 0080 / 0040 (negated with +15 bit 0 clear), +82 = the attack row 0E003E /
 * 0E001E, +8C = 0 (020166: bit 0 set), +4E = rank 2D3578 / 2D35AA (0260C8);
 * 0201B2: 01B1DC, +30 = +6E << 8 (negated with +15 bit 0), 009F88, +66 =
 * +1E = 8801, palette 0074 (009EA8), 004C1C, the rider 020372 (0040BC, D0 =
 * 1C). 02020C: 01AC38, 00427A, the cell map (0140C0) 0DFF52 / 0DFF68 (+8C
 * bit 0, by +15 bit 0) or 0DFF7E / 0DFFA0, 0049E8 with +82 (a hit: 0202B2),
 * 004C24 (carry: +5F bit 7 -> 0202B2, else +5A = 0), 01945C (0DFF12) ->
 * 02010E. 02010E: 013F1E, 0041AC.
 * 0202B2: 013F1E, the creator's +82 cleared when it is this node, +66 = +1E
 * = 1001, 009F88, 004192, the debris 01DA40 0DFEA6 + 0DFE5E (+8C bit 0) or
 * 0DFEB8 + 0DFE70, the blast 01C71E (0040BC, D0 = 1C) with +28 moved by 0038
 * / 0078 (negated with +15 bit 0) and put back, sound 1021, 01C706.
 * 020372: 009F88, +66 = +1E = 8801, palette 0075, program 22FEB0, 01AD6E
 * (0, 0); 0203B6: +15 bit 0 set when the creator's is, 01ABC8, 01AD8C,
 * 00427A. */
static neo_gactor_t *run_kid(neo_gactor_t *a,neo_genv_t *e,uint32_t r) {   /* 0040BC, D0 = 1C */
    return e->spawn_reserve?e->spawn_reserve(e->ctx,a,r,0x1c):e->spawn?e->spawn(e->ctx,a,r,false):NULL;
}
static void run_debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {   /* 01DA40 */
    neo_gactor_t *c=run_kid(a,e,0x1d764);
    if (!c) return;
    ga_place_from(c,a,e,0,0);                                               /* 01B220 */
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
static bool run_routine(uint32_t r) {return r==0x2010eu || r==0x2011cu || r==0x20166u || r==0x2020cu || r==0x202b2u || r==0x20372u || r==0x203b6u;}
static ga_result_t run_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x2011c: case 0x20166: {
        bool b=L(0)==0x20166u;
        SB(0x15,B(0x15)|1u);
        if (!ga_anim_start(a,e,b?0x22fe22:0x22fd1c,0x100)) return GA_UNSUPPORTED;
        uint16_t d0=b?0x40:0x80;
        if (!(B(0x15)&1u)) d0=(uint16_t)-d0;
        SW(0x28,W(0x28)+d0);
        SL(0x82,b?0xe001eu:0xe003eu);
        SB(0x8c,0);if (b) SB(0x8c,B(0x8c)|1u);
        const uint8_t *t=ga_rom(b?0x2d35aa:0x2d3578);
        if (!t) {ga_fail(e,b?0x2d35aa:0x2d3578);return GA_UNSUPPORTED;}
        SW(0x4e,ga_rank_word(a,e,t));                                        /* 0260C8 */
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                                    /* 01B1DC */
        uint32_t v=(uint32_t)(int32_t)(int16_t)W(0x6e)<<8;
        if (B(0x15)&1u) v=(uint32_t)-v;
        SL(0x30,v);
        SB(0x1d,0);                                                          /* 009F88 */
        SW(0x66,0x8801);SW(0x1e,0x8801);ga_palette(a,0x74);
        SB(0x5e,B(0x5e)|0x80u);                                              /* 004C1C */
        run_kid(a,e,0x20372);
        SL(0,0x2020c);
    }   /* fall through */
    case 0x2020c: {
        ga_move(a,e);                                                        /* 01AC38 */
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;                       /* 00427A */
        uint32_t map=B(0x8c)&1u?(B(0x15)&1u?0xdff68u:0xdff52u):(B(0x15)&1u?0xdffa0u:0xdff7eu);
        if (!e->dyn_set) {ga_fail(e,0x140c0);return GA_UNSUPPORTED;}
        e->dyn_set(e->ctx,a,map,0,0);                                        /* 0140C0 */
        if (!e->attack) {ga_fail(e,0x49e8);return GA_UNSUPPORTED;}
        if (ga_attack(a,e,L(0x82))) SL(0,0x202b2);                           /* 0049E8 */
        if (ga_register(a,e,NULL,NULL)) {                                    /* 004C24 */
            if (B(0x5f)&0x80u) SL(0,0x202b2); else SB(0x5a,0);
        }
        if (ga_cull(a,e,0xdff12)) SL(0,0x2010e);                             /* 01945C */
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x2010e:
        if (e->dyn_release) e->dyn_release(e->ctx,a);                        /* 013F1E */
        return GA_DELETE_TREE;                                               /* 0041AC */
    case 0x202b2: {
        if (e->dyn_release) e->dyn_release(e->ctx,a);                        /* 013F1E */
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (p && ga_l(p,0x82)==NEO_GA_TAG(a->index)) ga_sl(p,0x82,0);
        SW(0x66,0x1001);SW(0x1e,0x1001);SB(0x1d,0);                          /* 009F88 */
        if (!e->kill_children) {ga_fail(e,0x4192);return GA_UNSUPPORTED;}
        e->kill_children(e->ctx,a);                                          /* 004192 */
        bool b=(B(0x8c)&1u)!=0;
        run_debris(a,e,b?0xdfea6:0xdfeb8);run_debris(a,e,b?0xdfe5e:0xdfe70);
        uint32_t x=L(0x28);
        uint16_t d0=b?0x38:0x78;
        if (B(0x15)&1u) d0=(uint16_t)-d0;
        SW(0x28,W(0x28)+d0);
        run_kid(a,e,0x1c71e);
        SL(0x28,x);
        if (e->sound) e->sound(e->ctx,0x1021);                               /* 0120F8 */
        SL(0,0x1c706);
        return neo_effects_tick(a,e);                                        /* JMP 01C706 */
    }
    case 0x20372: {
        SB(0x1d,0);                                                          /* 009F88 */
        SW(0x66,0x8801);SW(0x1e,0x8801);ga_palette(a,0x75);
        if (!ga_anim_start(a,e,0x22feb0,0x100)) return GA_UNSUPPORTED;
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x1ad6e);return GA_UNSUPPORTED;}
        ga_place(a,e,0,0,NEO_GA_TAG(p->index));                              /* 01AD6E */
        SL(0,0x203b6);
    }   /* fall through */
    case 0x203b6: {
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0x203b6);return GA_UNSUPPORTED;}
        if (ga_b(p,0x15)&1u) SB(0x15,B(0x15)|1u);
        ga_project(a,e);                                                     /* 01ABC8 */
        if ((W(0x66)&0xf000u)==0x8000u) SW(0x1e,W(0x66)-W(0x40));            /* 01AD8C */
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static ga_result_t m6props_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (mid_routine(L(0))) return mid_tick(a,e);
    if (run_routine(L(0))) return run_tick(a,e);
    switch (L(0)) {
    /* ---- 0530F4: the hold of the mid-boss (script 2DFB88, group 0) ----
     * 0530F4: st.b 10C44C (444C(A5)), +0 = 0530FE; 0530FE: 10C44C set → rts,
     * clear (052F8A at the boss's end) → 004188. */
    case 0x530f4:
        if (!e->flags) {ga_fail(e,0x530f4);return GA_UNSUPPORTED;}
        e->flags->m6_signal=0xff;SL(0,0x530fe);
        /* fall through */
    case 0x530fe:
        if (!e->flags) {ga_fail(e,0x530fe);return GA_UNSUPPORTED;}
        return e->flags->m6_signal?GA_MORE:GA_DELETE;
    /* ---- 0BD822 / 0BD836 / 0BD84A: the raft ---- */
    case 0xbd822: case 0xbd836: case 0xbd84a: {
        uint32_t f=L(0)==0xbd822u?0xbd8f4u:L(0)==0xbd836u?0xbd906u:0xbd918u;
        neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,f,0x1c):e->spawn?e->spawn(e->ctx,a,f,false):NULL;
        if (c) {
            if (L(0)==0xbd84au) ga_sw(c,0x74,W(0x74));
            ga_sl(c,0x38,0x10e968u);
        }
        SW(0x66,0);SW(0x1e,0);SL(0x30,0);SL(0x34,0x10000);SB(0x15,B(0x15)&0xfeu);SW(0x6e,0x340);SW(0x72,0);SL(0,0xbd898);
    }   /* fall through */
    case 0xbd898: {
        SW(0x72,0);                                                          /* 0BD9D4 */
        for (unsigned p=0;p<2;p++) if (player_in(a,e,p,0xbda46)) SW(0x72,W(0x72)+2u);
        if (!rom_ok(e,0xbda0cu+W(0x72))) return GA_UNSUPPORTED;
        int16_t d1=(int16_t)rw(0xbda0cu+W(0x72)),d2=(int16_t)W(0x2c);
        SL(0x34,d1==d2?0u:d1>d2?0x10000u:0xffff0000u);
        ga_move(a,e);                                                        /* 01AC38 */
        return until(a,e);
    }
    /* ---- 0BD8F4 / 0BD906 / 0BD918: the float ---- */
    case 0xbd8f4: case 0xbd906: case 0xbd918: {
        ga_palette(a,L(0)==0xbd8f4u?0x4cb:L(0)==0xbd906u?0x4cc:0x4cd);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xbd926);return GA_UNSUPPORTED;}
        SW(0x6e,ga_w(p,0x6e));
        if (!ga_anim_start(a,e,0x2a953e,0x100)) return GA_UNSUPPORTED;
        SW(0x66,0);SW(0x1e,0);SB(0x15,B(0x15)&0xfeu);SW(0x76,0);SL(0,0xbd95e);
    }   /* fall through */
    case 0xbd95e: {
        uint16_t i=W(0x74);
        if (!rom_ok(e,0xbda12u+i)) return GA_UNSUPPORTED;
        int16_t n=(int16_t)rw(0xbda12u+i),d2=(int16_t)(W(0x76)+1u);
        if (n<=d2) {d2=0;SW(0x74,(W(0x74)+4u)&0x1cu);}
        SW(0x76,d2);
        int16_t dy=(int16_t)rw(0xbda14u+W(0x74));
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (!p) {ga_fail(e,0xbd9a2);return GA_UNSUPPORTED;}
        ga_place(a,e,0,dy,NEO_GA_TAG(p->index));                             /* 01AD6E */
        ga_project(a,e);                                                     /* 01ABC8 */
        if (!e->dyn_set) {ga_fail(e,0x140c0);return GA_UNSUPPORTED;}
        e->dyn_set(e->ctx,a,0xbda32,0,0);                                    /* 0140C0 */
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        return until(a,e);
    }
    /* ---- decorations ---- */
    case 0xbda4e:
        if (!start(a,e,0x2a954e,0x509,0,true)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)&0xfeu);SW(0x6e,0x340);SB(0x15,B(0x15)|4u);
        goto deco;
    case 0xbd416: if (!start(a,e,0x2a946c,0x4d4,0xfff1,false)) return GA_UNSUPPORTED;goto deco;
    case 0xbd444: if (!start(a,e,0x2a947c,0x4d5,0xfff1,false)) return GA_UNSUPPORTED;goto deco;
    case 0xbd472:
        if (!start(a,e,0x2a948c,0x50b,0x1111,false)) return GA_UNSUPPORTED;
        if (!e->front_layer) {ga_fail(e,0x12022);return GA_UNSUPPORTED;}
        if (e->front_layer(e->ctx)) SB(0x15,B(0x15)|4u); else SB(0x15,B(0x15)&0xfbu);   /* 012022 */
        goto deco;
    case 0xbd4a6: if (!start(a,e,0x2a949c,0x4df,0xfff1,false)) return GA_UNSUPPORTED;goto deco;
    case 0xbd4d4:
        if (!start(a,e,0x2a94b4,0x11d,0,false)) return GA_UNSUPPORTED;
        SB(0x15,B(0x15)&0xfeu);
    deco:
        SL(0,0xbd50e);
        /* fall through */
    case 0xbd50e:
        ga_project(a,e);
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        return until(a,e);
    /* ---- 0BDAB8 ---- */
    case 0xbdab8: {
        if (!ga_anim_start(a,e,0x2a95a4,0x100)) return GA_UNSUPPORTED;
        const uint8_t *t=ga_rom(0x2d3fee);
        if (!t) {ga_fail(e,0x2d3fee);return GA_UNSUPPORTED;}
        SW(0x4e,ga_rank_word(a,e,t));SL(0,0xbdade);
    }   /* fall through */
    case 0xbdade:
        ga_project(a,e);
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    /* ---- the hanging bridge ---- */
    case 0xbd18e: {
        neo_m6bridge_env_t *m=e->m6bridge;
        if (!m) {ga_fail(e,0xbd18e);return GA_UNSUPPORTED;}
        m->ram[6]=m->ram[7]=m->ram[8]=0;
        for (unsigned i=0;i<4;i++) m->ram[2+i]=(uint8_t)((uint32_t)m->x>>(24-8*i));
        SL(0,0xbd1b8);
    }   /* fall through */
    case 0xbd1b8: {
        neo_m6bridge_env_t *m=e->m6bridge;
        if (!m) {ga_fail(e,0xbd1b8);return GA_UNSUPPORTED;}
        uint8_t d3=(uint8_t)((uint16_t)((uint32_t)m->x>>16)-(uint16_t)(m->ram[2]<<8|m->ram[3]));
        uint8_t d0=d3&3u,d1=(uint8_t)((m->ram[7]+2u)&7u);
        m->ram[6]=d0;m->ram[7]=d1;m->ram[8]=(uint8_t)((d0+(uint8_t)((d1&6u)<<1))&0xfu);
        for (unsigned i=0;i<4;i++) m->ram[2+i]=(uint8_t)((uint32_t)m->x>>(24-8*i));
        return GA_MORE;
    }
    case 0xbd2ba: case 0xbd2ce:
        SW(0x6e,L(0)==0xbd2bau?0:2);SL(0x72,L(0)==0xbd2bau?0x10c70cu:0x10c70du);
        SW(0x70,0);SL(0,0xbd20e);                                            /* 0BD202 */
        /* fall through */
    case 0xbd20e: {
        neo_m6bridge_env_t *m=e->m6bridge;
        if (!m) {ga_fail(e,0xbd20e);return GA_UNSUPPORTED;}
        if (!rom_ok(e,0xbd2a8u+W(0x6e))) return GA_UNSUPPORTED;
        uint16_t d0=rw(0xbd2a8u+W(0x6e));
        if (d0==0xffffu) return GA_DELETE;                                   /* 004188 */
        uint16_t d4=(uint16_t)(d0+(uint16_t)m->origin);                      /* 0C6F6A */
        uint16_t d3=(uint16_t)((uint16_t)((uint16_t)m->o36+0x1fu-d4)<<4);
        uint8_t *lock=&m->ram[(L(0x72)-0x10c70cu)&1u];
        if (!(d3&0x8000u) && !*lock && d3!=W(0x70)) {
            uint32_t at=0xbd2aeu+2u*W(0x6e);
            if (!rom_ok(e,at)) return GA_UNSUPPORTED;
            uint32_t f=(uint32_t)rw(at)<<16|rw(at+2);
            if (f!=0xbd09au && f!=0xbd114u) {ga_fail(e,f);return GA_UNSUPPORTED;}   /* FFFFFFFF: trap #F */
            for (unsigned k=0;k<2;k++) {                                     /* 0BD09A / 0BD114 */
                uint32_t r=f==0xbd09au?(k?0xbcfc4u:0xbcfa4u):(k?0xbd004u:0xbcfe4u);
                neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,r,0x1c):e->spawn?e->spawn(e->ctx,a,r,false):NULL;
                if (c) {ga_sl(c,0x28,(uint32_t)(uint16_t)((uint16_t)((uint16_t)m->o36+0x1fu)<<4)<<16);ga_sl(c,0x2c,0x580000u);ga_sl(c,0x38,0x10d7b6u);}
            }
            *lock=1;
        }
        SW(0x70,d3);
        return GA_MORE;
    }
    case 0xbcfa4: case 0xbcfc4: case 0xbcfe4: case 0xbd004: {
        static const uint32_t prog[]={0x2a923c,0x2a92c8,0x2a9354,0x2a93e0};
        unsigned k=L(0)==0xbcfa4u?0:L(0)==0xbcfc4u?1:L(0)==0xbcfe4u?2:3;
        SW(0x66,k&1u?0:0xfff0);SW(0x1e,k&1u?0:0xfff0);
        if (!ga_anim_start(a,e,prog[k],0x100)) return GA_UNSUPPORTED;
        ga_palette(a,0x4e1);SB(0x15,B(0x15)&0xfeu);SL(0,0xbd044);            /* 0BD024 */
    }   /* fall through */
    case 0xbd044: {
        neo_m6bridge_env_t *m=e->m6bridge;
        if (!m) {ga_fail(e,0xbd044);return GA_UNSUPPORTED;}
        SB(0x6e,m->ram[8]);
        ga_project(a,e);
        if (ga_cull(a,e,0xbcf9c)) return e->unsupported?GA_UNSUPPORTED:GA_DELETE;   /* 01945C */
        if (step(a,e)!=GA_MORE) return GA_UNSUPPORTED;
        if (e->camera>=0x8b0u) return GA_DELETE;
        if (L(0x28)<0x4f00000u) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
bool neo_m6props_routine(uint32_t r) {
    static const uint32_t k[]={0x530f4,0x530fe,0xbd822,0xbd836,0xbd84a,0xbd898,0xbd8f4,0xbd906,0xbd918,0xbd95e,0xbda4e,0xbd416,0xbd444,0xbd472,0xbd4a6,0xbd4d4,0xbd50e,
        0xbdab8,0xbdade,0xbd18e,0xbd1b8,0xbd2ba,0xbd2ce,0xbd20e,0xbcfa4,0xbcfc4,0xbcfe4,0xbd004,0xbd044};
    for (unsigned i=0;i<sizeof k/sizeof k[0];i++) if (r==k[i]) return true;
    return mid_routine(r) || run_routine(r);
}
ga_result_t neo_m6props_tick(neo_gactor_t *a,neo_genv_t *e) {return m6props_tick(a,e);}
