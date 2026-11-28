#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 2 stage 3: the boss 026254 (script record 2D9CA2), manual
 * translation of 026254..027E8E and of the delayed music 059F4E/059F76/
 * 059FA8 (ROM labels as provenance; docs/JEFE_MISION2.md).
 * - Body 026254: layer 8838, music 0x27 after the delay (059F4E), +78 =
 *   progress (027BC6), floor, 01B1DC, 0143E4, y kept at 0x60, hurtbox
 *   027BFC, health 2C0C80 (+7E = half: phase 2, +8F bit 7), palettes 0x57/
 *   0x5B/0xB2 and flash 0x59, +6E = 027E8E, parts 027E34 (027A32), program
 *   23502C, sound 1020; it rises (026354), opens (0263B6/026400/02644A) and
 *   then cycles through the attacks of 027936 (table 0264AE: 0264BE wait,
 *   0269F2 shots 02715A/0271EC, 026550 shake, 0267FA flash and the column
 *   02701E). Tail 026B78/026B82: row 027C14, 00427A, phase, RAM 10E9BA
 *   (0x1A0 / 0x110 with +8F bit 4), 019388 (mission actor speeds 0xA0000),
 *   10E99A, 004C24, progress > 0x7E00 kills it, +5F bit 7 -> 026C30.
 * - Death 026C30..026E7A: sounds 1033, stop 1099/109C (012192), 018F92,
 *   parts 027E7A, white flashes (009BB0), debris 027F14/027F02 (01DA40) and
 *   01DBAA, it sinks, 018F40 at y < 0x60 and 026E7A deletes it (after the
 *   cartridge check, not reproduced).
 * - Parts 026F6C/026FCE (rows 027C74 by +87): program 027D74, palettes
 *   027E14[027DF4], placed on the body (01AD6E), follow it (027698) until
 *   the body's +88 changes (0041AC); 02701E/027112: the column of
 *   explosions; 02715A/0271EC: the shots (row 027C44, cull 027C64,
 *   027616 -> 01C706).
 * Untraced branches are explicit stops: 0273AA / 027440 (a player in a
 * vehicle, 0AAA88), 02621E.. are not reached. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define S8(v) ((int8_t)(uint8_t)(v))
#define S16(v) ((int16_t)(uint16_t)(v))
#define STOP(at) do { ga_fail(e,(at)); return GA_UNSUPPORTED; } while (0)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
/* 0260C8. Type 2 tables (the boss's choices) are random: the byte of the
 * rank row picks a group of 8 words after the rows, 0029C6 & 0xE the word. */
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
    const uint8_t *w=ga_rom(table+o);
    if (!w) {ga_fail(e,table+o);return 0;}
    return (uint16_t)(w[0]<<8|w[1]);
}
static const uint8_t *rom(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) ga_fail(e,at);return p;}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
/* 0040BC (reserve d0) + 01B220 (dx, dy). */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
static void flash(neo_genv_t *e,uint8_t v) {if (e->flags) e->flags->flash=v;}   /* 009BB0: RAM 109944 */
/* 01ABC8 twice around +2C: y kept at `y` (the pattern of 026280/02652A). */
static void keep_y(neo_gactor_t *a,neo_genv_t *e,uint16_t y) {
    ga_project(a,e);SW(0x2c,W(0x2c)+(uint16_t)(y-W(0x40)));ga_project(a,e);
}
/* The pattern of 026332: the y before the first projection kept. */
static void hold_y(neo_gactor_t *a,neo_genv_t *e) {uint16_t y=W(0x40);keep_y(a,e,y);}
/* 027848 / 02785A / 027890 / 0278C4: the pose counter +84. */
static void k27848(neo_gactor_t *a) {SB(0x80,1);SB(0x81,1);SB(0x82,0);}
static bool k2785a(neo_gactor_t *a) {
    SB(0x80,B(0x80)-1u);
    if (S8(B(0x80))>0) return false;
    SB(0x80,B(0x81));SB(0x84,B(0x84)+1u);
    if (S8(B(0x84))<4) return false;
    SB(0x84,3);return true;
}
static bool k27890(neo_gactor_t *a) {
    SB(0x80,B(0x80)-1u);
    if (S8(B(0x80))>0) return false;
    SB(0x80,B(0x81));SB(0x84,B(0x84)-1u);
    if (S8(B(0x84))>0) return false;
    SB(0x84,0);return true;
}
static neo_gactor_t *debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {   /* 01DA40 */
    neo_gactor_t *c=child(a,e,0x1d764,0x1c,0,0);
    if (!c) return NULL;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
    return c;
}
static bool k278c4(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x82)) SB(0x82,1);
    SB(0x80,B(0x80)-1u);
    if (S8(B(0x80))>0) return false;
    SB(0x80,B(0x81));
    const uint8_t *t=rom(e,0x27926);
    if (!t) return false;
    for (unsigned guard=0;guard<64;guard++) {
        uint8_t d1=B(0x82);SB(0x82,d1+1u);
        uint8_t d0=t[d1&0x1fu];
        if (!(d0&0x80u)) {SB(0x84,d0);return false;}
        if (d0==0xffu) {SB(0x82,0);return true;}
        debris(a,e,0x27e9e);                                            /* 027BAE */
    }
    ga_fail(e,0x278e8);return false;
}
/* 027A4C: part `index` (row 027C74 + 8 * index). */
static void part(neo_gactor_t *a,neo_genv_t *e,uint8_t index) {
    const uint8_t *r=rom(e,0x27c74u+8u*index);
    if (!r) return;
    neo_gactor_t *c=child(a,e,0x26f6c,r[7]?0xe:0x1c,0,0);
    if (!c) return;
    ga_sb(c,0x87,index);ga_sb(c,0x88,B(0x88));ga_sb(c,0x8b,r[6]);
    ga_sw(c,0x7a,(uint16_t)(r[0]<<8|r[1]));ga_sw(c,0x7c,(uint16_t)(r[2]<<8|r[3]));
    ga_sb(c,0x89,r[4]);ga_sb(c,0x8a,r[5]);
}
static void parts(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {      /* 027A32 */
    SB(0x88,B(0x88)+1u);
    for (unsigned i=0;i<64;i++) {
        const uint8_t *p=rom(e,list+i);
        if (!p || *p==0xffu) return;
        part(a,e,*p);
    }
}
static void k27640(neo_gactor_t *a,neo_genv_t *e,uint16_t bit) {      /* 027640 (bit 2) / 02766C (bit 1) */
    if (!(e->video&bit)) {SB(0x8c,1);SB(0x14,B(0x1a));} else {SB(0x8c,2);SB(0x14,B(0x1b));}
}
static uint16_t tower(neo_genv_t *e) {return e->m2boss?(uint16_t)e->m2boss->tower_mode:0;}   /* 02624C: RAM 10C354 */
/* 027936: the next attack (0..3). */
static uint16_t k27936(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0;
    if (B(0x8f)&0x40u) {
        if (S16(tower(e))<=6) goto select;
        SB(0x8f,B(0x8f)&~0x40u);
        if (e->m2boss) e->m2boss->tower_mode=0;                        /* 026244 */
        return 3;
    }
    if (S8(B(0x8d))>=0x3c) {SB(0x8d,0x3c);goto select;}
    d0=rank(a,e,(B(0x8f)&0x80u)?0x2c12cc:0x2c1290);SB(0x8f,B(0x8f)|0x20u);
    if ((d0&3u)==3u) {
        if (e->m2boss && !e->m2boss->tower_mode) e->m2boss->tower_mode=-1;   /* 026230 */
        SB(0x8f,B(0x8f)|0x40u);return 0;
    }
    return d0;
select:
    d0=rank(a,e,(B(0x8f)&0x80u)?0x2c12ea:0x2c12ae);
    if (d0==2u) SB(0x8f,B(0x8f)&~0x20u);
    return d0;
}
/* 0279F2: carry when a live player rides a vehicle (0AAA88 d0 != 0). The
 * port's players ride nothing unless the vehicle slots say so. */
static bool k279f2(neo_genv_t *e) {
    for (unsigned p=0;p<2;p++) {
        if (!((e->players.active_mask>>p)&1u)) continue;              /* 01AEB0 */
        if (e->vehicle && e->vehicle->slots && e->vehicle->exists[p] && neo_vslot_node(&e->vehicle->slots->slot[p])) return true;
    }
    return false;
}
/* 01B04A: player 1, 2 or (both live) one at random; the node even when none. */
static unsigned k1b04a(neo_genv_t *e) {
    unsigned d0=e->players.active_mask&0xffu;
    if (d0>=3u) d0=(ga_rand(e)&1u)+1u;
    return d0==2u?1u:0u;
}
/* 01B14A/01B162 (+8F bit 7) or 01B1C0 (0, 0x10): the shot's angle to the player. */
static bool aim(neo_gactor_t *a,neo_genv_t *e,uint8_t *angle) {
    unsigned p=k1b04a(e);
    const uint8_t *n=e->vehicle?e->vehicle->player[p]:NULL;
    if (!n) {ga_fail(e,0x1b04a);return false;}
    int16_t x=(int16_t)(n[0x3c]<<8|n[0x3d]),y=(int16_t)(n[0x40]<<8|n[0x41]);
    if (B(0x8f)&0x80u) {
        uint32_t d=(uint32_t)n[0x50]<<24|(uint32_t)n[0x51]<<16|(uint32_t)n[0x52]<<8|n[0x53];
        if (d) {
            const uint8_t *r=rom(e,d+2u);
            if (!r) return false;
            int16_t r0=(int16_t)(r[0]<<8|r[1]),r1=(int16_t)(r[2]<<8|r[3]),r2=(int16_t)(r[4]<<8|r[5]),r3=(int16_t)(r[6]<<8|r[7]);
            int16_t dx=(int16_t)((r2>>1)+r0),dy=(int16_t)((r3>>1)+r1);
            if (B(0x15)&1u) dx=(int16_t)-dx;
            if (B(0x15)&2u) dy=(int16_t)-dy;
            x=(int16_t)(x+dx);y=(int16_t)(y+dy);
        }
    } else y=(int16_t)(y+0x10);
    *angle=neo_angle((int16_t)(x-S16(W(0x3c))),(int16_t)(y-S16(W(0x40))));
    return true;
}
static void k27820(neo_gactor_t *a,uint16_t speed,uint8_t angle) {
    SL(0x30,(uint32_t)((int32_t)ga_cosine(angle)*(int32_t)(int16_t)speed));
    SL(0x34,(uint32_t)((int32_t)ga_sine(angle)*(int32_t)(int16_t)speed));
}
/* 027BD4: sound 103C every 0x30 of progress. */
static void k27bd4(neo_gactor_t *a,neo_genv_t *e) {
    if (S16((uint16_t)(e->camera-W(0x78)))>0x30) {sound(e,0x103c);SW(0x78,W(0x78)+0x30u);}
}
/* 018F92: mission complete (kill-all and flash requests, 0041D0 of the
 * script actor, 0C09D8, RAM 10C300 = FF, 10C304 = +5E, handlers 0C0538). */
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
/* 059F4E: the music 0x27 after the delay of 059F76. */
static void music(neo_gactor_t *a,neo_genv_t *e,uint16_t id) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x59f76,0):e->spawn?e->spawn(e->ctx,a,0x59f76,false):NULL;
    if (c) ga_sw(c,0x72,id);
}
static bool part_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter);
/* ---- body ------------------------------------------------------------------- */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e,bool row) {
    if (row) ga_attack(a,e,0x27c14);                                   /* 026B78 */
    if (ga_anim_step(a,e,part_call)<0) return GA_UNSUPPORTED;          /* 026B82 */
    if (S16(W(0x4e))<S16(W(0x7e))) SB(0x8f,B(0x8f)|0x80u);
    if (e->m2boss) {
        e->m2boss->point69ba=(B(0x8f)&0x10u)?0x110:0x1a0;              /* RAM 10E9BA */
        e->m2boss->speed[0]=e->m2boss->speed[1]=0xa0000;               /* 019388 */
        if (!e->m2boss->camera_dy) {SB(0x8f,B(0x8f)&~8u);SB(0x8d,0);} else {SB(0x8f,B(0x8f)|8u);SB(0x8d,B(0x8d)+1u);}
    } else {ga_fail(e,0x26bbc);return GA_UNSUPPORTED;}
    ga_register(a,e,NULL,NULL);SB(0x5a,0);
    if (S16(e->camera)>0x7e00) SB(0x5f,B(0x5f)|0x80u);
    if (B(0x5f)&0x80u) SL(0,0x26c30);
    k27bd4(a,e);
    SB(0x8e,B(0x5e));SB(0x5e,0);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t dying(neo_gactor_t *a,neo_genv_t *e) {                /* 026E6E */
    if (ga_anim_step(a,e,part_call)<0) return GA_UNSUPPORTED;
    k27bd4(a,e);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
static ga_result_t body(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0;
    switch (L(0)) {
    case 0x26254:
        SW(0x66,0x8838);SW(0x1e,0x8838);
        music(a,e,0x27);
        SW(0x78,e->camera);                                             /* 027BC6 */
        ga_floor(a,e);k1b1dc(a);ga_air(a,e);
        SW(0x2c,W(0x2c)+(uint16_t)(0x60u-W(0x40)));ga_project(a,e);
        SB(0x83,0);SB(0x84,3);SL(0x50,0x27bfc);
        d0=rank(a,e,0x2c0c80);SW(0x4e,d0);SW(0x7e,d0>>1);
        ga_palette(a,0x57);ga_palette(a,0x5b);ga_palette(a,0xb2);SB(0x14,B(0x19));
        ga_flash_palette(a,0x59);
        {const uint8_t *p=rom(e,0x27e8e);if (!p) return GA_UNSUPPORTED;SL(0x6e,(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3]);}
        SW(0x74,0x38);SW(0x76,0x140);SL(0x34,0x80000);
        parts(a,e,0x27e34);
        START(0x23502c);sound(e,0x1020);hold_y(a,e);
        SL(0,0x26354);
        /* fall through */
    case 0x26354:
        ga_move(a,e);
        if (S16(W(0x40))<0x60) keep_y(a,e,0x60);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x263b6);
        if (S16(W(0x40))>=S16(W(0x76))) SL(0,0x263b6);
        if (S16(W(0x40))>=0x160) SL(0,0x263b6);
        return tail(a,e,false);
    case 0x263b6:
        SL(0x34,0);k1b1dc(a);k27848(a);SW(0x74,6);START(0x234f10);SL(0,0x263e0);
        /* fall through */
    case 0x263e0:
        SW(0x74,W(0x74)-1u);
        if (k27890(a) && S16(W(0x74))<=0) SL(0,0x26400);
        return tail(a,e,false);
    case 0x26400:
        k1b1dc(a);k27848(a);SB(0x8f,B(0x8f)|0x10u);START(0x234f10);hold_y(a,e);
        SL(0x34,0xfffc8000u);SL(0,0x2644a);
        /* fall through */
    case 0x2644a:
        ga_move(a,e);
        if (S16(W(0x40))<=0xf0) {SB(0x8f,B(0x8f)&~0x10u);keep_y(a,e,0xf0);}
        if (k2785a(a) && S16(W(0x40))<=0xf0) SL(0,0x264be);
        return tail(a,e,false);
    case 0x264be: wait:
        parts(a,e,0x27e34);
        SW(0x74,rank(a,e,(B(0x8f)&0x80u)?0x2c1322:0x2c1308));
        SB(0x8c,0);SB(0x14,B(0x19));k27848(a);START(0x234f10);SL(0,0x26514);
        /* fall through */
    case 0x26514:
        SW(0x74,W(0x74)-1u);
        if (k278c4(a,e) && S16(W(0x74))<=0) {
            switch (k27936(a,e)&3u) {                                  /* 026498: table 0264AE */
            case 0: goto wait;
            case 1: goto shots;
            case 2: goto shake;
            default: goto flash;
            }
        }
        keep_y(a,e,0xf0);ga_attack(a,e,0x27c24);
        return tail(a,e,false);
    case 0x26550: shake:
        parts(a,e,0x27e34);k27848(a);
        SW(0x74,(B(0x8f)&0x80u)?0xf:0x1e);keep_y(a,e,0xf0);SL(0,0x26594);
        /* fall through */
    case 0x26594:
        if (!(ga_rand(e)&1u)) {
            int16_t s=S16(W(0x40))<0xd0?2:-2;
            SW(0x2c,W(0x2c)+(uint16_t)s);SW(0x40,W(0x40)+(uint16_t)s);
        }
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x265e2);
        ga_attack(a,e,0x27c24);
        return tail(a,e,false);
    case 0x265e2:
        if (B(0x8f)&0x20u) {SW(0x74,rank(a,e,0x2c0e24));SW(0x76,rank(a,e,0x2c0d78));d0=rank(a,e,0x2c0dda);}
        else {SW(0x74,rank(a,e,0x2c0f04));SW(0x76,rank(a,e,0x2c0e58));d0=rank(a,e,0x2c0eba);}
        SL(0x34,(uint32_t)(int32_t)(int16_t)d0<<8);
        parts(a,e,0x27e34);
        START((B(0x8f)&0x80u)?0x23502c:0x234fcc);
        sound(e,0x1020);hold_y(a,e);SL(0,0x266b2);
        /* fall through */
    case 0x266b2:
        ga_move(a,e);
        if (S16(W(0x40))<0xf0) keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x26714);
        if (S16(W(0x40))>=S16(W(0x76))) SL(0,0x26714);
        if (S16(W(0x40))>=0x160) SL(0,0x26714);
        return tail(a,e,true);
    case 0x26714:
        SL(0x34,0);k1b1dc(a);k27848(a);SW(0x74,6);START(0x234f10);SL(0,0x2673e);
        /* fall through */
    case 0x2673e:
        SW(0x74,W(0x74)-1u);
        if (k27890(a) && S16(W(0x74))<=0) SL(0,0x2675e);
        return tail(a,e,true);
    case 0x2675e:
        k1b1dc(a);k27848(a);SB(0x8f,B(0x8f)|0x10u);START(0x234f10);hold_y(a,e);
        SL(0x34,0xfffc8000u);SL(0,0x267a8);
        /* fall through */
    case 0x267a8:
        ga_move(a,e);
        if (S16(W(0x40))<=0xf0) {SL(0x34,0);SB(0x8f,B(0x8f)&~0x10u);keep_y(a,e,0xf0);}
        if (k2785a(a) && S16(W(0x40))<=0xf0) SL(0,0x264be);
        return tail(a,e,true);
    case 0x267fa: flash:
        parts(a,e,0x27e3c);
        SW(0x74,rank(a,e,(B(0x8f)&0x80u)?0x2c0d2c:0x2c0ce2));
        START(0x234f3c);sound(e,0x1099);SL(0,0x2684c);
        /* fall through */
    case 0x2684c:
        keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x2688c);
        k27640(a,e,2);SB(0x5f,B(0x5f)&0x7fu);SB(0x17,0);
        return tail(a,e,true);
    case 0x2688c:
        flash(e,1);parts(a,e,0x27e4e);SW(0x74,2);SL(0,0x268ae);
        goto k268be;
    case 0x268ae:
        flash(e,0);SL(0,0x268be);
        /* fall through */
    case 0x268be: k268be:
        keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x26908);
        k27640(a,e,1);ga_attack(a,e,0x27c34);SB(0x5f,B(0x5f)&0x7fu);SB(0x17,0);
        return tail(a,e,false);
    case 0x26908:
        sound(e,0x109c);child(a,e,0x2701e,0x1c,0,(int16_t)-0x20);
        if (e->shake) e->shake(e->ctx);                                /* 0C801A */
        SW(0x74,0x1e);SL(0,0x2694a);
        /* fall through */
    case 0x2694a:
        keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x2698a);
        k27640(a,e,1);SB(0x5f,B(0x5f)&0x7fu);SB(0x17,0);
        return tail(a,e,false);
    case 0x2698a:
        parts(a,e,0x27e66);SW(0x74,(B(0x8f)&0x80u)?0xf:0x1e);SL(0,0x269b2);
        /* fall through */
    case 0x269b2:
        keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x264be);
        k27640(a,e,2);SB(0x5f,B(0x5f)&0x7fu);SB(0x17,0);
        return tail(a,e,true);
    case 0x269f2: shots:
        k27848(a);parts(a,e,0x27e34);SW(0x74,0xf);SL(0,0x26a0a);
        /* fall through */
    case 0x26a0a:
        keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x26a3e);
        k278c4(a,e);
        return tail(a,e,true);
    case 0x26a3e:
        if (ga_rand(e)&1u) {SB(0x83,rank(a,e,0x2c11cc));goto k26ad2;}
        SB(0x83,rank(a,e,0x2c0fe2));
        /* fall through */
    case 0x26a5c:
        SW(0x74,rank(a,e,0x2c1044));SB(0x83,B(0x83)-1u);
        {neo_gactor_t *c=child(a,e,0x2715a,0x1c,0,0x10);if (c) ga_sb(c,0x8f,B(0x8f));}   /* 027AE6 */
        SL(0,0x26a7a);
        /* fall through */
    case 0x26a7a:
        keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,S8(B(0x83))>0?0x26a5c:0x26b48);
        k278c4(a,e);
        return tail(a,e,true);
    case 0x26ad2: k26ad2:
        SW(0x74,rank(a,e,0x2c122e));SB(0x83,B(0x83)-2u);
        if (!(B(0x83)&0x80u)) {                                        /* 027B14 */
            neo_gactor_t *c=child(a,e,0x271ec,0x1c,(int16_t)-0x40,0x10);
            if (c) {ga_sb(c,0x8f,B(0x8f));ga_sb(c,0x85,0x60);}
            c=child(a,e,0x271ec,0x1c,0x28,0x10);
            if (c) {ga_sb(c,0x8f,B(0x8f));ga_sb(c,0x85,0x20);}
        } else {                                                       /* 027B7A */
            neo_gactor_t *c=child(a,e,0x271ec,0x1c,0x30,0x10);
            if (c) {ga_sb(c,0x8f,B(0x8f));ga_sb(c,0x85,0x20);}
        }
        SL(0,0x26b00);
        /* fall through */
    case 0x26b00:
        keep_y(a,e,0xf0);
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,S8(B(0x83))>0?0x26ad2:0x26b48);
        k278c4(a,e);
        return tail(a,e,true);
    case 0x26b48:
        SL(0,0x26b4e);
        /* fall through */
    case 0x26b4e:
        keep_y(a,e,0xf0);
        if (k278c4(a,e)) SL(0,0x264be);
        return tail(a,e,true);
    /* ---- death ---- */
    case 0x26c30:
        sound(e,0x1033);sound(e,0x1099|0x8000u);sound(e,0x109c|0x8000u);   /* 012192: stop requests */
        SB(0x8f,B(0x8f)&~0x10u);
        if (e->m2boss) e->m2boss->point69ba=0x1a0;
        SB(0x5e,B(0x8e));
        k18f92(a,e);
        SL(0x50,0);parts(a,e,0x27e7a);SB(0x8c,0);SB(0x14,B(0x19));hold_y(a,e);
        flash(e,1);SW(0x74,2);SL(0,0x26cb0);
        /* fall through */
    case 0x26cb0:
        SW(0x74,W(0x74)-1u);
        if (!W(0x74)) SL(0,0x26cc2);
        return dying(a,e);
    case 0x26cc2:
        hold_y(a,e);flash(e,0);k27848(a);SL(0,0x26cf2);
        /* fall through */
    case 0x26cf2:
        if (k2785a(a)) SL(0,0x26d04);
        return dying(a,e);
    case 0x26d04:
        flash(e,0);START(0x23508c);SL(0x34,0xffff0000u);hold_y(a,e);
        debris(a,e,0x27f14);debris(a,e,0x27f02);
        for (unsigned i=0;i<16;i++) child(a,e,0x1d31c,0x2a,0,0x18);   /* 01DBAA */
        SL(0,0x26d62);
        /* fall through */
    case 0x26d62:
        ga_move(a,e);
        if (S16(W(0x40))<0xa0) SL(0,0x26d7c);
        return dying(a,e);
    case 0x26d7c:
        sound(e,0x1020);SL(0x34,0x80000);hold_y(a,e);SL(0,0x26db0);
        /* fall through */
    case 0x26db0:
        ga_move(a,e);
        if (S16(W(0x40))>0x160) SL(0,0x26dca);
        return dying(a,e);
    case 0x26dca:
        sound(e,0x1032);flash(e,1);SL(0x34,0);hold_y(a,e);SL(0,0x26e08);
        goto k26e28;
    case 0x26e08:
        flash(e,0);debris(a,e,0x27f02);
        for (unsigned i=0;i<16;i++) child(a,e,0x1d31c,0x2a,0,0x18);   /* 01DBAA */
        SL(0,0x26e28);
        /* fall through */
    case 0x26e28: k26e28:
        SL(0x34,L(0x34)+0xfffff000u);
        if ((int32_t)L(0x34)<(int32_t)0xfffe0000) {SL(0x34,0xfffe0000u);SL(0,0x26e4a);}
        /* fall through */
    case 0x26e4a:
        ga_move(a,e);
        if (S16(W(0x40))<0x60) {
            if (e->flags) e->flags->stage_done=1;                      /* 018F40 */
            SB(0x88,B(0x88)+1u);SL(0,0x26e7a);
        }
        return dying(a,e);
    case 0x26e7a:
        START(0x235a8a);ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return GA_DELETE;                                              /* 004188 (after the cartridge check) */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- parts ------------------------------------------------------------------ */
static bool palettes(neo_gactor_t *a,neo_genv_t *e,uint32_t f) {        /* 027E14 entries */
    switch (f) {
    case 0x27750: ga_palette(a,0x58);ga_palette(a,0x5c);ga_palette(a,0xb3);SB(0x14,B(0x19));ga_flash_palette(a,0x5a);return true;
    case 0x2778c: ga_palette(a,0xb3);ga_palette(a,0x5c);ga_palette(a,0xb3);SB(0x14,B(0x19));ga_flash_palette(a,0x5a);return true;
    case 0x277c8: ga_palette(a,0x5f);return true;
    case 0x277d8: ga_palette(a,0x5d);ga_palette(a,0x5e);SB(0x14,B(0x19));return true;
    case 0x277fc: ga_palette(a,0x61);ga_palette(a,0x60);SB(0x14,B(0x19));return true;
    }
    ga_fail(e,f);return false;
}
static bool part_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x27734: {                                                    /* +26 = rand & 1E0, at least 0x40 */
        uint16_t d0=(uint16_t)(ga_rand(e)&0x1e0u);
        SW(0x26,d0<0x40u?0x40u:d0);return true;
    }
    case 0x1adb2: SB(0x15,B(0x15)^2u);return true;
    case 0x27bba: debris(a,e,0x27eb0);return true;
    }
    return false;
}
static ga_result_t partf(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
    switch (L(0)) {
    case 0x26f6c: {
        const uint8_t *t=rom(e,0x27d74u+4u*B(0x87));
        if (!t) return GA_UNSUPPORTED;
        START((uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3]);
        const uint8_t *s=rom(e,0x27df4u+(B(0x87)&0x1fu));
        if (!s) return GA_UNSUPPORTED;
        const uint8_t *f=rom(e,0x27e14u+4u*(s[0]&7u));
        if (!f || !palettes(a,e,(uint32_t)f[0]<<24|(uint32_t)f[1]<<16|(uint32_t)f[2]<<8|f[3])) return GA_UNSUPPORTED;
        if (S8(B(0x87))<=1) {SB(0x87,0);SB(0x15,B(0x15)|4u);}
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);                    /* 01AD6E */
        SL(0,0x26fce);
    }   /* fall through */
    case 0x26fce: {
        if (!p) STOP(0x26fce);
        if (ga_b(p,0x88)!=B(0x88)) return GA_DELETE_TREE;              /* 0041AC */
        /* 027698 */
        SW(0x1e,(uint16_t)(S8(B(0x8b))+ga_w(p,0x1e)));
        int16_t d4=(int16_t)(ga_b(p,0x84)&3u);
        SW(0x28,(uint16_t)(S8(B(0x89))*d4+W(0x7a)));SW(0x2c,(uint16_t)(S8(B(0x8a))*d4+W(0x7c)));
        if (B(0x8c)&1u) SB(0x14,B(0x19+(ga_b(p,0x8c)&3u)));
        if (B(0x8c)&2u) SB(0x14,(e->video&2u)?B(0x1a):B(0x19));
        if (!B(0x87)) {
            uint8_t f=ga_b(p,0x17);                                    /* 0044F6 */
            if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}
            SW(0x66,0);SW(0x1e,0);
        }
        if (B(0x12)!=0xffu) SB(0x12,B(0x12)+8u);
        if (B(0x13)!=0xffu) SB(0x13,B(0x13)+8u);
        ga_project(a,e);
        int r=ga_anim_step(a,e,part_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x27016);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    }
    case 0x27016: return GA_DELETE_TREE;                               /* 0041AC */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- the column of explosions 02701E / 027112 --------------------------------- */
static ga_result_t column(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x2701e:
        SW(0x2c,W(0x2c)+0x28u);ga_project(a,e);
        SB(0x15,B(0x15)^1u);child(a,e,0x27112,0x1c,0,0);
        if (!(e->video&1u)) {
            child(a,e,0x27112,0x1c,(int16_t)-0x28,0);
            SB(0x15,B(0x15)^1u);child(a,e,0x27112,0x1c,(int16_t)-0x28,0);
        } else {
            child(a,e,0x27112,0x1c,(int16_t)-0x20,0);
            SB(0x15,B(0x15)^1u);child(a,e,0x27112,0x1c,(int16_t)-0x20,0);
        }
        if (S16(W(0x40))>0x220) return GA_DELETE;
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x27112:
        ga_palette(a,0xb1);SW(0x66,0x1001);SW(0x1e,0x1001);START(0x22cf2c);SL(0,0x27142);
        /* fall through */
    case 0x27142: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x27016);
        return GA_MORE;
    }
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- shots 02715A / 0271EC ---------------------------------------------------- */
static ga_result_t shot(neo_gactor_t *a,neo_genv_t *e) {
    uint8_t ang;
    switch (L(0)) {
    case 0x2715a:
        if (k279f2(e)) STOP(0x273aa);
        ga_project(a,e);
        if (!aim(a,e,&ang)) return GA_UNSUPPORTED;
        SB(0x85,ang);
        ga_palette(a,0x223);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x15,B(0x15)|0x20u);
        START(0x235872);
        SW(0x62,rank(a,e,0x2c116a));SW(0x4e,rank(a,e,0x2c1108));SB(0x5e,B(0x5e)|0x80u);   /* 004C1C */
        goto k2731e;
    case 0x271ec:
        if (k279f2(e)) STOP(0x27440);
        SW(0x74,rank(a,e,0x2c10a6));SW(0x62,rank(a,e,0x2c0f80));SW(0x4e,rank(a,e,0x2c0f1e));
        ga_palette(a,0x223);SW(0x66,0xe0ef);SW(0x1e,0xe0ef);SB(0x15,B(0x15)|0x20u);
        k27820(a,W(0x62),B(0x85));SB(0x5e,B(0x5e)|0x80u);
        START(0x235872);SL(0,0x2726c);
        /* fall through */
    case 0x2726c:
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x27284);
        goto k27330;
    case 0x27284: {
        if (!aim(a,e,&ang)) return GA_UNSUPPORTED;
        uint8_t d0=(uint8_t)(ang&0xf8u),d1=(uint8_t)(B(0x85)&0xf8u);
        SB(0x86,d0);SB(0x85,d1);
        if (!(W(0x30)&0x8000u)) SB(0x86,B(0x86)|1u);
        SW(0x74,8);SL(0,0x272d8);
    }   /* fall through */
    case 0x272d8: {
        uint8_t d1=0xf8,d0=B(0x86);
        if (d0&1u) {d1=8;d0&=0xfeu;}
        if (d0!=B(0x85)) SB(0x85,B(0x85)+d1);
        k27820(a,W(0x62),B(0x85));
        SW(0x74,W(0x74)-1u);
        if (S16(W(0x74))<=0) SL(0,0x2731e);
        goto k27330;
    }
    case 0x2731e: k2731e:
        k27820(a,W(0x62),B(0x85));SL(0,0x27330);
        /* fall through */
    case 0x27330: k27330:
        ga_project(a,e);ga_move(a,e);
        SB(0x84,(uint8_t)((uint8_t)(B(0x85)+4u)>>3));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if (ga_attack(a,e,0x27c44)) SL(0,0x27616);
        if (B(0x5f)&0x80u) SL(0,0x27616);
        if (e->flags && e->flags->mission_end) SL(0,0x27616);         /* 019424: RAM 10C300 */
        if (ga_cull(a,e,0x27c64)) SL(0,0x27638);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x27616:
        sound(e,0x1022);SW(0x66,0x1001);SW(0x1e,0x1001);
        SB(0x1d,0);                                                     /* 009F88 */
        SL(0,0x1c706);
        return neo_effects_tick(a,e);                                   /* jmp 01C706 */
    case 0x27638: return GA_DELETE_TREE;                               /* 0041AC */
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- 059F76 / 059FA8: music after a delay ------------------------------------- */
static ga_result_t delayed(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->m2boss) STOP(L(0));
    if (L(0)==0x59f76) {
        uint16_t m=e->pickup?e->pickup->mission:0,d0;                  /* 05A054: RAM 10C2F6 */
        d0=m==3u?1u:m==4u?2u:m==5u?3u:m==8u?4u:0u;
        SW(0x6e,d0);
        const uint8_t *t=rom(e,0xe11aau+4u*d0);
        if (!t) return GA_UNSUPPORTED;
        SW(0x70,(uint16_t)(t[2]<<8|t[3]));
        sound(e,(uint16_t)(0x0a00u|(uint8_t)(t[1]+0x20u)));           /* 01228C: command 0x0A, then d0 + 0x20 */
        SL(0,0x59fa8);
    }
    if (L(0)!=0x59fa8) STOP(L(0));
    SW(0x70,W(0x70)-(uint16_t)e->m2boss->frame_step);                  /* 003B4C */
    if (!(W(0x70)&0x8000u)) return GA_MORE;
    sound(e,W(0x72));
    return GA_DELETE;                                                  /* 004188 (after the cartridge check) */
}
static bool body_routine(uint32_t r) {
    static const uint32_t S[]={0x26254,0x26354,0x263b6,0x263e0,0x26400,0x2644a,0x264be,0x26514,0x26550,0x26594,0x265e2,0x266b2,
        0x26714,0x2673e,0x2675e,0x267a8,0x267fa,0x2684c,0x2688c,0x268ae,0x268be,0x26908,0x2694a,0x2698a,0x269b2,0x269f2,0x26a0a,
        0x26a3e,0x26a5c,0x26a7a,0x26ad2,0x26b00,0x26b48,0x26b4e,0x26c30,0x26cb0,0x26cc2,0x26cf2,0x26d04,0x26d62,0x26d7c,0x26db0,
        0x26dca,0x26e08,0x26e28,0x26e4a,0x26e7a};
    for (unsigned i=0;i<sizeof(S)/sizeof(S[0]);i++) if (S[i]==r) return true;
    return false;
}
bool neo_m2boss_routine(uint32_t r) {
    return body_routine(r) || r==0x26f6c || r==0x26fce || r==0x27016 || r==0x2701e || r==0x27112 || r==0x27142 ||
        r==0x2715a || r==0x271ec || r==0x2726c || r==0x27284 || r==0x272d8 || r==0x2731e || r==0x27330 || r==0x27616 || r==0x27638 ||
        r==0x59f76 || r==0x59fa8;
}
ga_result_t neo_m2boss_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (body_routine(r)) return body(a,e);
    if (r==0x26f6c || r==0x26fce) return partf(a,e);
    if (r==0x27016) return GA_DELETE_TREE;                             /* 0041AC */
    if (r==0x2701e || r==0x27112 || r==0x27142) return column(a,e);
    if (r==0x59f76 || r==0x59fa8) return delayed(a,e);
    return shot(a,e);
}
