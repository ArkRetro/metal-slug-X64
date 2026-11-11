#include "neo_region.h"
#include "neo_blood.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 5 metro: the machine 0D3B36 (script record of camera 992) and its
 * children, manual translation of ROM 0D3B36..0D596E (ROM labels as provenance).
 *
 * - 0D3B36 / 0D3C5A (spawn table 0197DC; 0D3C5A with +15 bit 0, +7C = 1, the
 *   fifth arm 0D444E, rank 2C9E46): palette 3E5, flash 30F, the pilot 0D447C
 *   and the arms 0D4396 / 0D43C4 / 0D43F2 / 0D4420 (0040BC reserve E), the
 *   commands +7F..+82 = 1 and +7E = 0, +4E = rank 2C9D84, the exhaust 01DC8E
 *   (0), layers 112D (11F5 with +6F), the AI timers +76 / +78 = random & 3F,
 *   solid E8E22 (+9E, once), hurtbox E8CC6, attack E8DF2 (+9A). The entries
 *   0D3B4C / 0D3C76 belong to the actor of 0D39xx (not in this mission):
 *   explicit stop.
 * - It drives back and forth (0D3E48: +30 = -C000 by +A2, programs 2AB858 /
 *   2AB888, sound 1071) on the cell map 0D559A / 0D55FE (0140C4) until +30 is
 *   0 or 0D5060 (x outside 50..F0 in its direction), then turns (0D3F0A:
 *   2AB8B8, 2AB8F2 for +74 = 3C updates, 2AB914). Every update 0D41A0: 0D50A8,
 *   the AI 0D50EC (commands to its children through +7E..+82: 0D512E,
 *   0D5298, 0D5396 with the ranks 2C9F08, 2C9FCA, 2CA08C, 2CA2D2, 2CA394,
 *   2CA456), 004C24, +5F bit 7 -> 0D40A6 (sound 1033, 2ABA16 with the debris
 *   01DA40 E8EF0, the commands 5; then three blasts 01C71E, the debris E8F02,
 *   004188), 01945C E8E02 -> the commands 5 and 004188 (0D41DE / 0D4204).
 * - The arms 0D42A4 (+8E = their command byte, +88 = offsets by the body's
 *   frame +7D, +90 = program row): commands 0E8FBC (1: 0E8FF0, 2: 0E9004,
 *   3: 0E9018, 4: 0E902C, 6: 0E9040, 5: deleted).
 * - The pilot 0D447C (+7E): palette 238, idle 2AC67A, commands 0E8FD8 and
 *   0D545C (+76, the players in E8E12) to aim (2AC68C / 2AC72A / 2AC7C8);
 *   hit -> 23643A (sound 1041, 0D4F1C when it was aiming), then hidden for
 *   rank 2CA69C; command 5 -> 0D470E: E0EF, 0405EA and the soldier death 040846.
 * - Program calls: 0D5722 (debris E8EF0), 0D5740 / 0D57D0 (the muzzle 0D498C
 *   and the shells 0D4762 / 0D4776: speed rows 2CA75E / 2CA7BE by +6E, 01B0F6,
 *   E8CE6, 2AC462 aimed by 002EB4, attack, 01C71E), 0D5662 (0D4ABA, 0D4A1E),
 *   0D56D2 (0D4C26, 0D4A82), 0D585C (05DF3E, the grenade 0D4D84: 2CA518 /
 *   2CA5DA), 0D58D4 / 0D5908 / 0D593C (the sparks 0D5018), 029034.
 * The dead code 0D3F00, 0D4034, 0D404C, 0D4EF0 and 0D4FAA is not reached
 * (no reference in the ROM). 08EF46-style cartridge checks (0D4204) end in
 * 004188 as in neo_m5train.c. */
#define L(o) ga_l(a,o)
#define W(o) ga_w(a,o)
#define B(o) ga_b(a,o)
#define SL(o,v) ga_sl(a,o,v)
#define SW(o,v) ga_sw(a,o,(uint16_t)(v))
#define SB(o,v) ga_sb(a,o,(uint8_t)(v))
#define BIT(o,n) ((B(o)>>(n))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if ((r=ga_anim_step(a,e,mech_call))<0) return GA_UNSUPPORTED; } while (0)
static int16_t S16v(uint16_t v) {return (int16_t)v;}
static int32_t S32v(uint32_t v) {return (int32_t)v;}
static uint16_t rw(neo_genv_t *e,uint32_t at) {const uint8_t *p=ga_rom(at);if (!p) {ga_fail(e,at);return 0;} return (uint16_t)(p[0]<<8|p[1]);}
static uint32_t rl(neo_genv_t *e,uint32_t at) {return (uint32_t)rw(e,at)<<16|rw(e,at+2u);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {   /* 0260C8 */
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
static neo_gactor_t *creator(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* 0040BC with its reserve D0, then 01B220 (dx, dy). */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve,int16_t dx,int16_t dy) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,dx,dy);
    return c;
}
/* 0D5090: D0 negated when facing left (+15 bit 0). */
static int16_t facing(neo_gactor_t *a,int16_t d0) {return BIT(0x15,0)?(int16_t)-d0:d0;}
/* 01AD6E: the position block on the creator's at (dx, dy). */
static void on_creator(neo_gactor_t *a,neo_genv_t *e,int16_t dx,int16_t dy) {
    neo_gactor_t *p=creator(a,e);ga_place(a,e,dx,dy,p?NEO_GA_TAG(p->index):0);
}
static uint32_t speed8(uint16_t w) {return (uint32_t)((int32_t)((uint32_t)w<<16)>>8);}   /* swap, clr.w, asr.l #8 */
static void layers(neo_gactor_t *a,uint16_t v) {SW(0x66,v);SW(0x1e,v);}
static void commands(neo_gactor_t *a,uint8_t c7f,uint8_t c80,uint8_t c81,uint8_t c82,uint8_t c7e) {
    SB(0x7f,c7f);SB(0x80,c80);SB(0x81,c81);SB(0x82,c82);SB(0x7e,c7e);
}
/* 01DA40: the debris emitter 01D764 with the list `list` (+86). */
static void debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=child(a,e,0x1d764,0x1c,0,0);
    if (!c) return;
    if (BIT(0x15,2)) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);
}
/* 00409A: carry clear (false) when the creator was deleted. */
static bool alive(neo_gactor_t *a,neo_genv_t *e) {return ga_creator_alive(a,e);}
/* 0049E8 on +9A (D0 = 0 without a row). */
static uint8_t attack9a(neo_gactor_t *a,neo_genv_t *e) {return L(0x9a)?ga_attack(a,e,L(0x9a)):0;}
/* 004E76 on +9E once. */
static void solid9e(neo_gactor_t *a,neo_genv_t *e) {
    if (!L(0x9e)) return;
    if (e->solid) e->solid(e->ctx,a,L(0x9e));
    SL(0x9e,0);
}
/* 0D5566: the cell map by the facing (0140C4). */
static void cells(neo_gactor_t *a,neo_genv_t *e) {
    if (!e->dyn_set) return;
    if (!BIT(0x15,0)) e->dyn_set(e->ctx,a,0xd559a,-0x48,0); else e->dyn_set(e->ctx,a,0xd55fe,-0x30,0);
}
/* 0D5060: carry when x is outside 50..F0 in the direction of +30. */
static bool k5060(neo_gactor_t *a) {return S32v(L(0x30))>0?S16v(W(0x3c))>0xf0:S16v(W(0x3c))<0x50;}
/* 0D50A8 / 0D50CC: with +7B bit 0 the creator's +61 (and +7A) count up and +60 is copied. */
static void k50a8(neo_gactor_t *a,neo_genv_t *e,bool b7a) {
    if (!BIT(0x7b,0)) return;
    neo_gactor_t *p=creator(a,e);
    if (!p) return;
    ga_sb(p,0x61,(uint8_t)(ga_b(p,0x61)+1u));
    if (b7a) ga_sb(p,0x7a,(uint8_t)(ga_b(p,0x7a)+1u));
    SB(0x60,ga_b(p,0x60));
}
/* 0D512E: the commands of the arms 7F / 80 / 81 and the pilot 7E. */
static void k512e(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x83)==0) {
        if (B(0x60)==1u) {
            SB(0x84,0x7f);SW(0x76,rank(a,e,0x2c9fca)+0x10u);SB(0x83,1);commands(a,2,2,0,0,2);
        } else {
            SB(0x84,rank(a,e,0x2ca08c));SW(0x76,rank(a,e,0x2c9fca)+0x1eu);SB(0x83,1);commands(a,2,2,2,0,2);
        }
        return;
    }
    if (B(0x83)==1u) {
        commands(a,3,3,0,0,0);
        if (BIT(0x84,0)) commands(a,0,0,3,0,0);
        uint16_t d0=(uint16_t)(rank(a,e,0x2c9fca)+0x1eu);
        if (B(0x60)!=1u) d0=(uint16_t)(d0+0xfff2u);
        SW(0x76,d0);
        SB(0x84,B(0x84)-1u);
        if ((int8_t)B(0x84)<0) SB(0x83,2);
        return;
    }
    SB(0x83,0);SW(0x76,rank(a,e,0x2c9f08));SW(0x76,W(0x76)+(ga_rand(e)&0x3fu));commands(a,4,4,4,0,0);
}
/* 0D5298 (+60 != 1) / 0D5396 (+60 = 1): the command of the arm 82 and the pilot. */
static void k5298(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x85)==0) {
        SB(0x86,rank(a,e,0x2ca456));SW(0x78,rank(a,e,0x2ca394)+0x1eu);SB(0x85,1);commands(a,0,0,0,2,3);
        return;
    }
    if (B(0x85)==1u) {commands(a,0,0,0,3,0);SW(0x78,rank(a,e,0x2ca394)+0x1eu);SB(0x85,2);return;}
    SW(0x78,rank(a,e,0x2ca394)+0x1eu);
    SB(0x86,B(0x86)-1u);
    if ((int8_t)B(0x86)<0) {SW(0x78,rank(a,e,0x2ca2d2));SW(0x78,W(0x78)+(ga_rand(e)&0x3fu));}
    SB(0x85,0);commands(a,0,0,0,4,0);
}
static void k5396(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x85)==0) {SW(0x78,(S16v(rank(a,e,0x2ca394))>>1)+0x14);SB(0x85,1);commands(a,0,0,0,2,0);return;}
    if (B(0x85)==1u) {commands(a,0,0,0,3,0);SW(0x78,(S16v(rank(a,e,0x2ca394))>>1)+4);SB(0x85,2);return;}
    SW(0x78,(S16v(rank(a,e,0x2ca394))>>1)+0xa);SB(0x85,1);commands(a,0,0,0,6,0);
}
/* 0D50EC: the two AI timers. */
static void k50ec(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x76,W(0x76)-1u);
    if (S16v(W(0x76))<=0) k512e(a,e);
    SW(0x78,W(0x78)-1u);
    if (S16v(W(0x78))<=0) {if (B(0x60)==1u) k5396(a,e); else k5298(a,e);}
}
/* 0D5498: the arm's palette by +7B and the creator's +7C, flash 30F. */
static void k5498(neo_gactor_t *a,neo_genv_t *e) {
    if (!BIT(0x7b,0)) ga_palette(a,0x3e5);
    else {neo_gactor_t *p=creator(a,e);ga_palette(a,p && ga_b(p,0x7c)?0x25c:0x25b);}
    ga_flash_palette(a,0x30f);
}
/* 0D54EE: 01ABC8 and the layer = the creator's + +8C. */
static void k54ee(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    neo_gactor_t *p=creator(a,e);
    if (p) SW(0x1e,ga_w(p,0x1e)+W(0x8c));
}
/* 0D5506: the offset (+28, +2C) from the table +88 by the creator's frame +7D. */
static void k5506(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    if (!p) {ga_fail(e,0xd5506);return;}
    uint32_t t=L(0x88)+(((ga_b(p,0x7d)-1u)&0xfu)<<2);
    SW(0x2c,rw(e,t+2u));SW(0x28,facing(a,S16v(rw(e,t))));
}
/* 0D5530: the creator's command byte +8E (cleared) as the routine of the table; 0 for none. */
static uint32_t k5530(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    neo_gactor_t *p=creator(a,e);
    if (!p) {ga_fail(e,0xd5530);return 0;}
    unsigned o=W(0x8e);uint8_t d0=ga_b(p,o);ga_sb(p,o,0);
    d0&=0xfu;
    return d0?rl(e,table+4u*d0):0;
}
/* The creator's command byte +8E read and cleared: true on 5. */
static bool take5(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    if (!p) {ga_fail(e,0xd4582);return false;}
    unsigned o=W(0x8e);uint8_t d0=ga_b(p,o);ga_sb(p,o,0);
    return d0==5u;
}
/* 0044F6: the creator's flash +17. */
static void flash_share(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p=creator(a,e);
    if (!p) {ga_fail(e,0x44f6);return;}
    uint8_t f=ga_b(p,0x17);
    if (f) {if (!(f&0x80u)) f++;SB(0x17,f);}
}
/* 0D545C: carry when +76 ran out and a player is in E8E12 (+76 = 1E + random & 3F). */
static bool k545c(neo_gactor_t *a,neo_genv_t *e) {
    SW(0x76,W(0x76)-1u);
    if (S16v(W(0x76))>0) return false;
    if (ga_players_in(a,e,0xe8e12)) return false;
    SW(0x76,(ga_rand(e)&0x3fu)+0x1eu);
    return true;
}
/* 01B0F6: carry clear when an active player (1 then 2) is in front (01AEB0, 01B08A). */
static bool k1b0f6(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned p=0;p<2;p++) {
        if (!((e->players.active_mask>>p)&1u)) continue;
        int16_t d0=(int16_t)(W(0x3c)-(p?e->players.p2x:e->players.p1x));
        if (BIT(0x15,0)?d0>0:d0<0) return false;
    }
    return true;
}
static bool mech_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    neo_gactor_t *c;
    switch (routine) {
    case 0xd573e:return true;
    case 0xd5722:debris(a,e,0xe8ef0);return true;
    case 0x29034:SB(0x5f,B(0x5f)|8u);return true;
    case 0xd5662:
        if ((c=child(a,e,0xd4aba,0x1c,0,0x40))) ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+facing(a,-0x2a)));
        if ((c=child(a,e,0xd4a1e,0x1c,0,0x16))) ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+facing(a,-0x10)));
        sound(e,0x1064);return true;
    case 0xd56d2:
        child(a,e,0xd4c26,0x1c,0,0);child(a,e,0xd4a82,0x1c,0,0);
        sound(e,0x1066);return true;
    case 0xd5740: case 0xd57d0: {
        bool second=routine==0xd57d0;
        if ((c=child(a,e,0xd498c,0x1c,0,0))) {ga_sw(c,0x70,(uint16_t)facing(a,second?0x2c:0x29));ga_sw(c,0x72,0x1e);}   /* addi.w #1E: +6E..+AF of a free node are 0 (003F24) */
        uint16_t d0=ga_rand(e);
        c=child(a,e,second?0xd4776:0xd4762,0x1c,0,0);
        if (second) SB(0x6e,(d0&7u)+8u);                                /* 0D583A: the arm's own +6E */
        else if (c) {ga_sb(c,0x6e,(uint8_t)(d0&7u));ga_sb(c,0x60,B(0x60));}
        if (c) {ga_sw(c,0x70,(uint16_t)facing(a,second?0x2c:0x25));ga_sw(c,0x72,0x44);}
        sound(e,0x10c6);return true;}
    case 0xd585c:
        if ((c=child(a,e,0x5df3e,0x1c,0,0x10))) {ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)^2u));ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+facing(a,0x18)));}
        if ((c=child(a,e,0xd4d84,0x1c,0,0))) {ga_sw(c,0x96,(uint16_t)facing(a,0x20));ga_sw(c,0x98,0x10);}   /* addi.w #10 on a fresh node (003F24) */
        sound(e,0x10a4);return true;
    case 0xd58d4:
        if ((c=child(a,e,0xd5018,0x1c,0,0xe))) ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+facing(a,-0x10)));
        return true;
    case 0xd5908: case 0xd593c:
        if ((c=child(a,e,0xd5018,0x1c,0,-2))) ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+facing(a,routine==0xd5908?-0xc:4)));
        return true;
    }
    return false;
}
static ga_result_t into(neo_gactor_t *a,neo_genv_t *e,uint32_t effect) {SL(0,effect);return neo_effects_tick(a,e);}   /* JMP 01C706 / 01C71E */
static ga_result_t done(neo_genv_t *e) {return e->unsupported?GA_UNSUPPORTED:GA_MORE;}
/* 0D41A0: the body's common tail. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    k50a8(a,e,true);k50ec(a,e);
    if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
    if (BIT(0x5f,7)) SL(0,0xd40a6);
    if (ga_cull(a,e,0xe8e02)) SL(0,0xd41de);
    return done(e);
}
static ga_result_t mech_tick(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *p,*c;int r;uint32_t h;
    for (;;) switch (L(0)) {
    /* the body */
    case 0xd3b36: case 0xd3c5a: {
        bool b=L(0)==0xd3c5au;
        if (b) SB(0x15,B(0x15)|1u);
        SB(0x7b,0);ga_palette(a,0x3e5);
        SB(0x7c,b?1:0);ga_flash_palette(a,0x30f);
        static const uint32_t kids[2][5]={{0xd447c,0xd4396,0xd43c4,0xd43f2,0xd4420},{0xd447c,0xd4396,0xd43c4,0xd43f2,0xd444e}};
        for (unsigned i=0;i<5;i++) child(a,e,kids[b][i],0xe,0,0);
        commands(a,1,1,1,1,0);
        SW(0x4e,rank(a,e,b?0x2c9e46:0x2c9d84));
        if ((c=child(a,e,0x1d86c,0x1c,0,0))) {                              /* 01DC8E (0) */
            ga_sb(c,0x94,0);ga_sw(c,0x6e,0);ga_sw(c,0x70,0);ga_sw(c,0x72,0x3f);ga_sw(c,0x74,0x1f);
        }
        layers(a,B(0x6f)?0x11f5:0x112d);
        SW(0x76,ga_rand(e)&0x3fu);SW(0x78,ga_rand(e)&0x3fu);
        SB(0x83,0);SB(0x85,0);
        SL(0x9e,0xe8e22);SL(0x50,0xe8cc6);SL(0x9a,0xe8df2);
        SW(0x74,1);
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0xd3e02);}
        /* fall through */
    case 0xd3e02:
        ga_project(a,e);k50a8(a,e,true);
        SW(0x74,W(0x74)-1u);
        if (S16v(W(0x74))<=0) SL(0,0xd3e22);
        return done(e);
    case 0xd3e22:
        if (!BIT(0x15,6)) ga_floor(a,e);                                 /* 0146E8 */
        SW(0xa2,BIT(0x15,0));
        /* fall through */
    case 0xd3e48: {
        uint32_t d0=0xffff4000u;
        if (W(0xa2)) d0=0u-d0;
        SL(0x30,d0);
        if (BIT(0x15,0)) d0=0u-d0;
        START(S32v(d0)>0?0x2ab888:0x2ab858);
        sound(e,0x1071);
        SL(0,0xd3ea6);}
        /* fall through */
    case 0xd3ea6:
        solid9e(a,e);cells(a,e);ga_move(a,e);STEP();
        if (!L(0x30)) SL(0,0xd3f0a);
        if (k5060(a)) SL(0,0xd3f0a);
        attack9a(a,e);
        return tail(a,e);
    case 0xd3f0a:
        SW(0x74,0x3c);SW(0xa2,W(0xa2)^1u);
        sound(e,0x1071|0x8000u);                                         /* 012192 */
        START(0x2ab8b8);
        SL(0,0xd3f36);
        /* fall through */
    case 0xd3f36:
        solid9e(a,e);cells(a,e);ga_project(a,e);STEP();
        if (r) SL(0,0xd3f82);
        attack9a(a,e);
        return tail(a,e);
    case 0xd3f82:
        START(0x2ab8f2);
        SL(0,0xd3f98);
        /* fall through */
    case 0xd3f98:
        solid9e(a,e);cells(a,e);ga_project(a,e);STEP();
        SW(0x74,W(0x74)-1u);
        if (!W(0x74)) SL(0,0xd3fd8);
        return tail(a,e);
    case 0xd3fd8:
        START(0x2ab914);
        SL(0,0xd3fee);
        /* fall through */
    case 0xd3fee:
        solid9e(a,e);cells(a,e);ga_project(a,e);STEP();
        if (r) SL(0,0xd3e48);
        attack9a(a,e);
        return tail(a,e);
    case 0xd40a6:
        sound(e,0x1033);
        START(0x2aba16);
        commands(a,5,5,5,5,5);
        SL(0,0xd40e4);
        /* fall through */
    case 0xd40e4:
        ga_project(a,e);STEP();
        if (r) SL(0,0xd4116);
        k50a8(a,e,false);
        if (ga_cull(a,e,0xe8e02)) SL(0,0xd41de);
        return done(e);
    case 0xd4116:
        sound(e,0x102f);
        child(a,e,0x1c71e,0x1c,-0x38,0);child(a,e,0x1c71e,0x1c,0,0);child(a,e,0x1c71e,0x1c,0x38,0);
        debris(a,e,0xe8f02);                                             /* 0D5730 */
        STEP();
        return e->unsupported?GA_UNSUPPORTED:GA_DELETE;                  /* 004188 */
    case 0xd41de:
        commands(a,5,5,5,5,5);
        SL(0,0xd4204);
        return GA_MORE;
    case 0xd4204:
        return GA_DELETE;                                                /* the cartridge check, 004188 */
    case 0xd4294:
        return GA_DELETE;                                                /* 004188 */
    case 0xd429c:
        return GA_DELETE_TREE;                                           /* 0041AC */
    /* the arms */
    case 0xd4396: case 0xd43c4: case 0xd43f2: case 0xd4420: case 0xd444e: {
        static const struct {uint32_t r;uint16_t z,cmd;uint32_t offsets;uint16_t row;uint32_t prog;} t[]={
            {0xd4396,0,0x7f,0xe8e30,0,0x2aba2e},{0xd43c4,0xffff,0x80,0xe8e30,1,0x2abb42},{0xd43f2,0xffff,0x81,0xe8e60,2,0x2abc3a},
            {0xd4420,0xffff,0x82,0xe8e90,3,0x2abd44},{0xd444e,0xffff,0x82,0xe8e90,4,0x2abe26}};
        unsigned i=0;while (t[i].r!=L(0)) i++;
        SW(0x8c,t[i].z);SW(0x8e,t[i].cmd);SL(0x88,t[i].offsets);SW(0x90,t[i].row);
        START(t[i].prog);
        on_creator(a,e,0,0);
        p=creator(a,e);
        if (p) SB(0x7b,ga_b(p,0x7b));
        k5498(a,e);}
        h=0xe8ff0;
        goto restart;
    restart:
        START(rl(e,h+4u*W(0x90)));
        SL(0,0xd42e2);
        /* fall through */
    case 0xd42e2:
        p=creator(a,e);
        if (p) SB(0x60,ga_b(p,0x60));
        k5506(a,e);k54ee(a,e);
        switch (k5530(a,e,0xe8fbc)) {
        case 0:break;
        case 0xd42c0:h=0xe8ff0;goto restart;
        case 0xd4312:h=0xe9004;goto restart;
        case 0xd4330:h=0xe9018;goto restart;
        case 0xd434e:h=0xe902c;goto restart;
        case 0xd436e:h=0xe9040;goto restart;
        case 0xd438e:SL(0,0xd4294);return done(e);
        default:ga_fail(e,0xd4302);return GA_UNSUPPORTED;
        }
        flash_share(a,e);STEP();
        return done(e);
    /* the pilot */
    case 0xd447c:
        on_creator(a,e,0,0);
        SB(0x5e,B(0x5e)|0x80u);                                          /* 004C1C */
        SW(0x8c,0);SW(0x8e,0x7e);SL(0x88,0xe8ec0);SW(0x90,5);
        SW(0x76,(ga_rand(e)&0x3fu)+0x1eu);
        ga_palette(a,0x238);
        /* fall through */
    case 0xd44ca:
        SB(0x92,1);SW(0x4e,1);SB(0x5a,0);SB(0x5f,B(0x5f)&~0x80u);
        START(0x2ac67a);
        STEP();
        SL(0,0xd44fc);
        /* fall through */
    case 0xd44fc:
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        SB(0x60,ga_b(p,0x60));
        if (k545c(a,e)) SL(0,0xd45d4);
        if (ga_b(p,W(0x8e))==5u) SL(0,0xd470e);
        switch (k5530(a,e,0xe8fd8)) {
        case 0:return done(e);
        case 0xd44ca:SL(0,0xd44ca);continue;
        case 0xd45a2:SB(0x92,2);START(0x2ac68c);break;
        case 0xd45ba:SB(0x92,3);START(0x2ac72a);break;
        case 0xd45d4:SB(0x92,4);START(0x2ac7c8);break;
        default:ga_fail(e,0xd453a);return GA_UNSUPPORTED;
        }
        goto aim;
    case 0xd45d4:
        SB(0x92,4);START(0x2ac7c8);
    aim:
        SW(0x4e,1);SB(0x5a,0);SB(0x5f,B(0x5f)&~0x88u);
        SL(0,0xd455a);
        /* fall through */
    case 0xd455a:
        k5506(a,e);k54ee(a,e);STEP();
        if (r) SL(0,0xd44ca);
        if (ga_register(a,e,NULL,NULL)) SL(0,0xd45ee);
        if (take5(a,e)) SL(0,0xd470e);
        return done(e);
    case 0xd45ee:
        if (B(0x92)==4u) child(a,e,0xd4f1c,0x1c,0,0);
        SB(0x92,5);sound(e,0x1041);
        START(0x23643a);
        SL(0,0xd4640);
        /* fall through */
    case 0xd4640:
        k5506(a,e);k54ee(a,e);STEP();
        if (r) SL(0,0xd4678);
        if (take5(a,e)) SL(0,0xd470e);
        return done(e);
    case 0xd4678:
        SB(0x74,0x3c);
        SL(0,0xd4684);
        /* fall through */
    case 0xd4684:
        k5506(a,e);k54ee(a,e);
        SB(0x74,B(0x74)-1u);
        if (!B(0x74)) SL(0,0xd46ca);
        if (!BIT(0x74,0)) STEP();
        if (take5(a,e)) SL(0,0xd470e);
        return done(e);
    case 0xd46ca:
        SW(0x74,rank(a,e,0x2ca69c));
        SL(0,0xd46e0);
        /* fall through */
    case 0xd46e0:
        SW(0x74,W(0x74)-1u);
        if (S16v(W(0x74))<=0) SL(0,0xd44ca);
        if (take5(a,e)) SL(0,0xd429c);
        return done(e);
    case 0xd470e:
        if (B(0x92)==4u) child(a,e,0xd4f1c,0xe,0,0);
        k5506(a,e);k54ee(a,e);STEP();
        layers(a,0xe0ef);
        SB(0x1d,0);ga_palette(a,neo_blood_palette(NEO_BLOOD_SOLDIER_DEATH,e->blood));   /* 0405EA: 009F88, 01B2BE */
        neo_m3enemy_death_40846(a,e);                                    /* 040846 */
        return done(e);
    /* the shells 0D4762 / 0D4776 */
    case 0xd4762: case 0xd4776:
        START(L(0)==0xd4762u?0x2ac1c2:0x2ac312);
        on_creator(a,e,S16v(W(0x70)),S16v(W(0x72)));
        p=creator(a,e);
        ga_palette(a,p && ga_b(p,0x7b)?0x25d:0x3e6);
        SB(0x15,B(0x15)|0x20u);SB(0x5e,B(0x5e)|0x80u);SW(0x8c,0);
        SL(0,0xd47d8);
        /* fall through */
    case 0xd47d8:
        k54ee(a,e);STEP();
        if (r) SL(0,0xd4814);
        if (!alive(a,e)) SL(0,0xd497e);
        if (ga_cull_rect(a,e,0xe8e0a)) SL(0,0xd429c);
        return done(e);
    case 0xd4814: {
        uint32_t t=B(0x60)==1u?0x2ca7be:0x2ca75e;
        SB(0x15,B(0x15)&~3u);
        t+=(B(0x6e)&0xfu)*6u;
        SW(0xa6,rw(e,t+2u));SL(0x34,speed8(rw(e,t+4u)));
        SL(0x30,speed8((uint16_t)facing(a,S16v(rw(e,t)))));
        if (k1b0f6(a,e)) SL(0x30,0u-L(0x30));
        SW(0x4e,0x32);SL(0x50,0xe8ce6);
        START(0x2ac462);
        SL(0,0xd489e);}
        /* fall through */
    case 0xd489e:
        if (S32v(L(0x34))<=0) {layers(a,0xe0ef);SL(0,0xd48bc);}
        /* fall through */
    case 0xd48bc: {
        SL(0x34,L(0x34)+speed8(W(0xa6)));
        ga_air(a,e);                                                     /* 0143E4 */
        if (BIT(0x15,7)) SL(0,0xd497e);
        if (S32v(L(0x34))>=0 && (p=creator(a,e))) SW(0x1e,ga_w(p,0x1e));
        int16_t dx=(int16_t)((int16_t)(uint16_t)(L(0x30)>>8)>>4),dy=(int16_t)((int16_t)(uint16_t)(L(0x34)>>8)>>4);
        SW(0xa2,(neo_angle(dx,dy)&0xffu)>>3);
        STEP();
        if (attack9a(a,e)) SL(0,0xd4ffe);
        if (ga_register(a,e,NULL,NULL)) SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0xd497e);
        if (!alive(a,e)) SL(0,0xd497e);
        if (ga_cull_rect(a,e,0xe8e0a)) SL(0,0xd429c);
        return done(e);}
    case 0xd497e:
        sound(e,0x1022);
        /* fall through */
    case 0xd4ffe:
        layers(a,0x1001);SL(0x50,0);
        return into(a,e,0x1c71e);
    /* the muzzle 0D498C, 0D4A82 */
    case 0xd498c:
        on_creator(a,e,S16v(W(0x70)),S16v(W(0x72)));
        p=creator(a,e);
        ga_palette(a,p && ga_b(p,0x7b)?0x25d:0x3e6);
        SW(0x8c,0);
        START(0x2ac0bc);
        SL(0,0xd49e2);
        /* fall through */
    case 0xd49e2:
        k54ee(a,e);STEP();
        if (r) SL(0,0xd429c);
        if (!alive(a,e)) SL(0,0xd429c);
        if (ga_cull_rect(a,e,0xe8e0a)) SL(0,0xd429c);
        return done(e);
    case 0xd4a82:
        ga_palette(a,0x25e);
        START(0x2abe96);
        on_creator(a,e,facing(a,-0x16),0x28);
        SW(0x8c,0);
        SL(0,0xd49e2);
        continue;
    /* 0D4A1E */
    case 0xd4a1e:
        ga_palette(a,0x208);
        START(0x236272);
        SB(0x15,B(0x15)^1u);layers(a,0x1001);
        SL(0,0xd4a54);
        /* fall through */
    case 0xd4a54:
        ga_project(a,e);STEP();
        if (r) SL(0,0xd429c);
        if (ga_cull_rect(a,e,0xe8e1a)) SL(0,0xd429c);
        return done(e);
    /* 0D4ABA / 0D4C26: on the arm, then thrown */
    case 0xd4aba: case 0xd4c26: {
        bool k=L(0)==0xd4c26u;
        on_creator(a,e,0,k?0x28:0x38);
        SW(0x8c,1);ga_palette(a,0x3cf);
        START(k?0x2abffa:0x2abf38);
        SL(0,k?0xd4c5e:0xd4af2);
        continue;}
    case 0xd4af2: case 0xd4c5e: {
        bool k=L(0)==0xd4c5eu;
        SW(0x28,facing(a,k?-0x3c:-0x50));
        k54ee(a,e);STEP();
        if (r) SL(0,k?0xd4cc2:0xd4b56);
        if (attack9a(a,e)) SL(0,0xd4fde);
        if (!alive(a,e)) SL(0,0xd4fde);
        if (ga_cull_rect(a,e,0xe8e02)) SL(0,0xd429c);
        return done(e);}
    case 0xd4b56: case 0xd4cc2: {
        bool k=L(0)==0xd4cc2u;
        SB(0x15,B(0x15)|0x20u);
        if (k) {layers(a,0xe0ef);START(0x2ac074);}
        ga_place(a,e,S16v(W(0x3c)),S16v(W(0x40)),0);                     /* 01AB98 (A1 = 0) */
        SL(0x30,0);SL(0x34,0);SL(0x62,0);                                /* 01B1DC */
        SL(0x30,speed8((uint16_t)facing(a,(int16_t)-S16v(rank(a,e,k?0x2ca210:0x2ca14e)))));
        SL(0x34,0);
        if (!k) {layers(a,0xe0ef);ga_palette(a,0x3cf);START(0x2abfb2);}
        SL(0,k?0xd4d24:0xd4bc6);}
        /* fall through */
    case 0xd4bc6: case 0xd4d24: {
        uint32_t end=L(0)==0xd4d24u?0xd4fca:0xd4fc2;
        ga_air(a,e);
        if (BIT(0x15,7)) SL(0,end);
        STEP();
        if (attack9a(a,e)) SL(0,end);
        if (!alive(a,e)) SL(0,end);
        if (ga_cull_rect(a,e,0xe8e02)) SL(0,0xd429c);
        return done(e);}
    case 0xd4fc2: case 0xd4fca:
        SW(0x28,W(0x28)+facing(a,L(0)==0xd4fc2u?-0x28:-0x30));
        /* fall through */
    case 0xd4fde:
        SB(0x1d,0);                                                      /* 009F88 */
        layers(a,0x1001);SL(0x50,0);
        return into(a,e,0x1c706);
    /* the grenade 0D4D84 */
    case 0xd4d84:
        on_creator(a,e,S16v(W(0x96)),S16v(W(0x98)));
        ga_palette(a,0x239);layers(a,0xe0ef);
        START(0x2ac8d0);
        SL(0,0xd4dc2);
        /* fall through */
    case 0xd4dc2:
        p=creator(a,e);
        if (!p) {ga_fail(e,L(0));return GA_UNSUPPORTED;}
        if (ga_b(p,0x92)==5u) {SL(0,0xd4fde);continue;}
        ga_project(a,e);STEP();
        if (r) SL(0,0xd4e2a);
        if (attack9a(a,e)) SL(0,0xd4fde);
        if (!alive(a,e)) SL(0,0xd4fde);
        if (ga_cull_rect(a,e,0xe8e1a)) SL(0,0xd429c);
        return done(e);
    case 0xd4e2a:
        SL(0x30,speed8((uint16_t)facing(a,S16v(rank(a,e,0x2ca518)))));
        SW(0xa4,facing(a,S16v(rank(a,e,0x2ca5da))));
        SB(0x15,B(0x15)|0x20u);
        START(0x2ac92e);
        SL(0,0xd4e74);
        /* fall through */
    case 0xd4e74:
        SL(0x30,L(0x30)+speed8(W(0xa4)));SL(0x34,L(0x34)+speed8(W(0xa6)));
        ga_air(a,e);
        if (BIT(0x15,7)) SL(0,0xd4fde);
        STEP();
        if (attack9a(a,e)) SL(0,0xd4fde);
        if (!alive(a,e)) SL(0,0xd4fde);
        if (ga_cull_rect(a,e,0xe8e1a)) SL(0,0xd429c);
        return done(e);
    /* the helmet 0D4F1C */
    case 0xd4f1c:
        SL(0x30,BIT(0x15,0)?0u-0x11100u:0x11100u);
        SL(0x34,0x87f00);SW(0xa6,0xff6f);SW(0xa4,0);
        ga_palette(a,0x238);
        START(0x263ad0);
        SL(0,0xd4f68);
        /* fall through */
    case 0xd4f68:
        SL(0x34,L(0x34)+speed8(W(0xa6)));
        ga_air(a,e);
        if (BIT(0x15,7)) SL(0,0xd429c);
        STEP();
        if (ga_cull_rect(a,e,0xe8e1a)) SL(0,0xd429c);
        return done(e);
    /* the sparks 0D5018 */
    case 0xd5018:
        layers(a,0xe0ed);ga_palette(a,0x348);
        START(0x2361de);
        SL(0,0xd5048);
        /* fall through */
    case 0xd5048:
        ga_project(a,e);STEP();
        if (r) SL(0,0xd429c);
        return done(e);
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
bool neo_m5mech_routine(uint32_t r) {
    switch (r) {
    case 0xd3b36: case 0xd3c5a: case 0xd3e02: case 0xd3e22: case 0xd3e48: case 0xd3ea6: case 0xd3f0a: case 0xd3f36: case 0xd3f82:
    case 0xd3f98: case 0xd3fd8: case 0xd3fee: case 0xd40a6: case 0xd40e4: case 0xd4116: case 0xd41de: case 0xd4204: case 0xd4294:
    case 0xd429c: case 0xd4396: case 0xd43c4: case 0xd43f2: case 0xd4420: case 0xd444e: case 0xd42e2: case 0xd447c: case 0xd44ca:
    case 0xd44fc: case 0xd45d4: case 0xd455a: case 0xd45ee: case 0xd4640: case 0xd4678: case 0xd4684: case 0xd46ca: case 0xd46e0:
    case 0xd470e: case 0xd4762: case 0xd4776: case 0xd47d8: case 0xd4814: case 0xd489e: case 0xd48bc: case 0xd497e: case 0xd4ffe:
    case 0xd498c: case 0xd49e2: case 0xd4a82: case 0xd4a1e: case 0xd4a54: case 0xd4aba: case 0xd4c26: case 0xd4af2: case 0xd4c5e:
    case 0xd4b56: case 0xd4cc2: case 0xd4bc6: case 0xd4d24: case 0xd4fc2: case 0xd4fca: case 0xd4fde: case 0xd4d84: case 0xd4dc2:
    case 0xd4e2a: case 0xd4e74: case 0xd4f1c: case 0xd4f68: case 0xd5018: case 0xd5048: return true;
    }
    return false;
}
ga_result_t neo_m5mech_tick(neo_gactor_t *a,neo_genv_t *e) {return mech_tick(a,e);}
