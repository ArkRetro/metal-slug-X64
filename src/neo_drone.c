#include "neo_region.h"
#include "neo_soldier_ai.h"   /* neo_angle (ROM 002EB4) */
/* Mission 2 stage 3: the drone 03670E (+96 = 4) and its bomb 0369AC,
 * manual translation (ROM labels as provenance).
 * - 03670E: +76/+74/+77/+78 = 0, palettes 0x142/0x143, program 23BA74,
 *   layer E0ED, health 2D0C7C (0260C8), hurtbox 035A84, 01B1DC, 01ABC8,
 *   00427A; speed +70 = 2D0CAE << 8, +30 = +70 (negated right of x 0xA0).
 * - 0367B6: +78 = FF once x - 0x28 is below 0xF0 (unsigned); +74 counts
 *   down; 035AF0 (004C24: +9F = +5A, +5A = 0; +5F bit 7 -> 0368DA); with
 *   the nearest player (01AF9A) it turns 0x30 px past it, and above it
 *   (|dx| <= 4, +74 = 0) drops the bomb (0368C2 path, when +78 and +77)
 *   or arms +74 = 2D0CF8 / 2 and +77 = FF. 03685A: +34 = 0x18000 towards
 *   y 0x100 + +6E (none within 4 px), 01AC38, 00427A, cull 035A54
 *   (01945C, 0041AC).
 * - 03689C: +14 = +19, program 23BB52, 019DFC, 0366DA (the bomb 0369AC
 *   20 px up once, +76), 0368C2: 035AF0 (-> 0368DA), 01AC38, 00427A, cull.
 * - 0368DA: +14 = +19, 0366DA, program 23BBD0, 019DFC, sound 10F0;
 *   03690A: 01ABC8, 00427A; at its end 004188 (after the cartridge check,
 *   not reproduced), else the cull.
 * - 0369AC (bomb): 01B1DC, program 23BC72, palette 0x143, layer 8820;
 *   0369E2: 0143DC; on the floor (+15 bit 6) or when row 035AC8 hits
 *   (0049E8) -> 036A0C: palettes released, palette 0x4BB, program 23BCF2,
 *   +61 = FF; 036A3A: row 035AB8 while +61, 01ABC8, 00427A, 004188 at its
 *   end. Otherwise 00427A and the cull. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define S16(v) ((int16_t)(uint16_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static bool rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table,uint16_t *out) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return false;}
    *out=ga_rank_word(a,e,t);return true;
}
static void k1b1dc(neo_gactor_t *a) {SL(0x30,0);SL(0x34,0);SL(0x62,0);}
/* 035AF0: carry = +5F bit 7. */
static bool k35af0(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_register(a,e,NULL,NULL)) {SB(0x9f,B(0x5a));SB(0x5a,0);}
    return (B(0x5f)&0x80u)!=0;
}
/* 01AF9A: the nearest live player's X; carry (true) when none. */
static bool nearest_x(neo_gactor_t *a,neo_genv_t *e,uint16_t *x) {
    unsigned m=e->players.active_mask&0xffu;
    if (!m) return true;
    if (m==1u) {*x=e->players.p1x;return false;}
    if (m==2u) {*x=e->players.p2x;return false;}
    uint16_t d1=(uint16_t)(e->players.p1x-W(0x3c)),d2=(uint16_t)(e->players.p2x-W(0x3c));
    if (d1&0x8000u) d1=(uint16_t)(0u-d1);
    if (d2&0x8000u) d2=(uint16_t)(0u-d2);
    *x=S16(d2)<S16(d1)?e->players.p2x:e->players.p1x;
    return false;
}
/* 0366DA */
static void bomb(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x76)) ga_spawn_from(a,e,0x369ac,0,(int16_t)-0x14);
    SB(0x76,0xff);
}
/* 03623E */
static ga_result_t cull(neo_gactor_t *a,neo_genv_t *e) {
    if (e->unsupported) return GA_UNSUPPORTED;
    return ga_cull(a,e,0x35a54)?GA_DELETE_TREE:GA_MORE;
}
static ga_result_t drone_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;uint16_t v;
    switch (L(0)) {
    case 0x3670e:
        SB(0x96,4);SB(0x76,0);SW(0x74,0);SB(0x77,0);SB(0x78,0);
        ga_palette(a,0x142);ga_palette(a,0x143);START(0x23ba74);
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        if (!rank(a,e,0x2d0c7c,&v)) return GA_UNSUPPORTED;
        SW(0x4e,v);SL(0x50,0x35a84);k1b1dc(a);ga_project(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (!rank(a,e,0x2d0cae,&v)) return GA_UNSUPPORTED;
        SL(0x70,(uint32_t)(int32_t)S16(v)<<8);
        SL(0x30,S16(W(0x3c))>0xa0?0u-L(0x70):L(0x70));SL(0,0x367b6);
        return e->unsupported?GA_UNSUPPORTED:GA_MORE;
    case 0x367b6: {
        if ((uint16_t)(W(0x3c)-0x28u)<0xf0u) SB(0x78,0xff);
        if (W(0x74)) SW(0x74,W(0x74)-1u);
        if (k35af0(a,e)) goto dying;
        uint16_t px;
        if (!nearest_x(a,e,&px)) {
            uint16_t d0=(uint16_t)(W(0x3c)-px);
            if (!(L(0x30)&0x80000000u)) {if (S16(d0)>=0x30) SL(0x30,0u-L(0x70));}
            else if (S16(d0)<=-0x30) SL(0x30,L(0x70));
            if (!W(0x74)) {
                uint16_t m=(d0&0x8000u)?(uint16_t)(0u-d0):d0;
                if (m<=4u) {
                    if (B(0x78) && B(0x77)) goto drop;
                    if (!rank(a,e,0x2d0cf8,&v)) return GA_UNSUPPORTED;
                    SW(0x74,v>>1);SB(0x77,0xff);
                }
            }
        }
        SL(0x34,0);
        uint16_t d0=(uint16_t)(W(0x40)-0x100u-W(0x6e)),d1=(d0&0x8000u)?(uint16_t)(0u-d0):d0;
        if (d1>4u) SL(0x34,(d0&0x8000u)?0x18000u:0xfffe8000u);
        ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return cull(a,e);
    }
    drop:
        SB(0x14,B(0x19));START(0x23bb52);SB(0x69,0xff);bomb(a,e);SL(0,0x368c2);
        /* fall through */
    case 0x368c2:
        if (k35af0(a,e)) goto dying;
        ga_move(a,e);
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        return cull(a,e);
    dying:
        SB(0x14,B(0x19));bomb(a,e);START(0x23bbd0);SB(0x69,0xff);
        if (e->sound) e->sound(e->ctx,0x10f0);
        SL(0,0x3690a);
        /* fall through */
    case 0x3690a:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        return cull(a,e);
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
static ga_result_t bomb_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    switch (L(0)) {
    case 0x369ac:
        k1b1dc(a);START(0x23bc72);ga_palette(a,0x143);SW(0x66,0x8820);SW(0x1e,0x8820);SL(0,0x369e2);
        /* fall through */
    case 0x369e2:
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                      /* 0143DC */
        if (!(B(0x15)&0x40u) && !ga_attack(a,e,0x35ac8)) {
            if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
            return cull(a,e);
        }
        if (e->unsupported) return GA_UNSUPPORTED;
        SB(0x1d,0);ga_palette(a,0x4bb);START(0x23bcf2);SB(0x61,0xff);SL(0,0x36a3a);
        /* fall through */
    case 0x36a3a:
        if (B(0x61)) ga_attack(a,e,0x35ab8);
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (e->unsupported) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    ga_fail(e,L(0));return GA_UNSUPPORTED;
}
/* ---- Bats 035B66 / 035C68 (the same module, 035A40..0366F0) ------------
 * Common constructor 0362EE: palette 0xB7 and +97 = 1 when +96 != 3 and
 * +6E != 0 (the bat worth points, 0364D2: 08C0C8 pickup / 08CE42; not in
 * Mission 2's records: explicit stop), else 0x94; 036326 (+96 = 2: +90 =
 * +70, the first waypoint (+6E, +70) = its place); depth E0ED; hurtbox +50 =
 * 035A78 (+96 = 0), 035A90 (3) or 035A6C; +4E = 0260C8(2D0C7C); +9D/+98/+9F
 * = 0, +92 = 7FFF.
 * - 035B66 (+96 = 0, the swarm): 03635E: +70 bats 035BF8 after it
 *   (0040BC, 01B220 (0, 0), words +6E..+78), each a different +86 of 0..4
 *   (0029F2(5)); then it leaves (004188 after the cartridge check).
 *   035BF8: +7E = 035ADC[+86], a wing program 035B1A, +30 = (0029F2(0x1E1)
 *   + +72 - 0xF0) << 8 (facing right when positive); 035C42: +34 = +7E,
 *   +7E += 0x1400, 035AF0 (hit -> 036068), 01AC38, 03623E, 0364D2.
 * - 035C68 (+96 = 2): 035F0E perches (23B838, +8C = 0029F2(0xDC) + 0x50;
 *   at 0 a stretch 23B88E/23B912 by 0029C6 & 1), until +92 runs out or the
 *   camera progress reaches +90: 035FBC (wing program, +60 = 0, 036486: a
 *   new waypoint +98 of 0..2 by 0029F2(3), facing it, +92 = 0029F2(0x3D) +
 *   0x3C) and 035FD0: within 3 px of the waypoint it perches again, else
 *   flies to it at 1.0 px (002EB4 angle +- 0x18 by its quadrant, 002E7C),
 *   01AC38, 0364D2, 03623E, 035AF0; +60 set: 036254 (a random angle & 0x7F
 *   at 0x38000 in +7A/+7E) and 03603C for 7 updates, then 035FBC.
 * - 036068 (hit, +5F bit 7): sound 10F0, 23B76C (+9F = 7: burnt) or
 *   23B68C; at its end 23B7AA / 23B6DE, +30/+34 = 0, 03610E falls (0143DC)
 *   until the floor, then +9B = FF (or 8 and +9C = 0 one time in three by
 *   0029F2(3)) and 23B7EC / 23B720 at speed 0x100 (+9B < 0) or 0x200;
 *   03618A crawls (03663E random steps of +9B/+9C) or falls, +9D counts to
 *   0x50 (004188 tree), at 0x28 +5F bit 3.
 * The +96 = 1 and 3 paths (nest 035D12, 035EEC, 035C76/035D04, 035E0A,
 * 0361EC; stage 3) are explicit stops. */
static uint32_t rlong(neo_genv_t *e,uint32_t at) {
    const uint8_t *p=ga_rom(at);
    if (!p) {ga_fail(e,at);return 0;}
    return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];
}
static void bat_init(neo_gactor_t *a,neo_genv_t *e) {                  /* 0362EE */
    if (B(0x96)!=3u && W(0x6e)) {ga_palette(a,0xb7);SB(0x97,1);} else {ga_palette(a,0x94);SB(0x97,0);}
    if (B(0x96)==1u) {SW(0x88,W(0x72));SW(0x90,W(0x74));}
    else if (B(0x96)==2u) {SW(0x90,W(0x70));SW(0x6e,W(0x28));SW(0x70,W(0x2c));}
    SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
    SL(0x50,B(0x96)==3u?0x35a90u:B(0x96)==0u?0x35a78u:0x35a6cu);
    uint16_t v;if (rank(a,e,0x2d0c7c,&v)) SW(0x4e,v);
    SB(0x9d,0);SB(0x98,0);SB(0x9f,0);SW(0x92,0x7fff);
}
static bool bat_wings(neo_gactor_t *a,neo_genv_t *e) {                 /* 035B1A */
    uint16_t r=ga_rand(e)&3u;
    return ga_anim_start(a,e,r==0?0x23b672:r==1?0x23b654:0x23b636,0x100);
}
static int bat_anim(neo_gactor_t *a,neo_genv_t *e) {                   /* 0364D2 */
    if (B(0x97)) {ga_fail(e,0x364d2);return -1;}                        /* points bat: not in Mission 2 */
    return ga_anim_step(a,e,NULL);
}
static void bat_waypoint(neo_gactor_t *a,int16_t *x,int16_t *y) {       /* 0365A4 */
    unsigned i=(B(0x98)&3u)*4u;*x=S16(W(0x6eu+i));*y=S16(W(0x70u+i));
}
static void bat_pick(neo_gactor_t *a,neo_genv_t *e) {                  /* 036486 */
    if (B(0x96)!=2u) return;
    uint16_t d0;
    do d0=ga_rand_range(e,3); while ((uint8_t)d0==B(0x98));
    SB(0x98,d0);SB(0x15,B(0x15)&0xfeu);
    int16_t x,y;bat_waypoint(a,&x,&y);
    if ((int16_t)(x-S16(W(0x28)))>=0) SB(0x15,B(0x15)|1u);
    SW(0x92,ga_rand_range(e,0x3d)+0x3cu);
}
static ga_result_t bat_tick(neo_gactor_t *a,neo_genv_t *e) {
    int r;
    for (;;) switch (L(0)) {
    case 0x35b66: {                                                     /* 03635E */
        int n=(int)W(0x70)-1;
        if (n<0 || n>5) {ga_fail(e,0x3636c);return GA_UNSUPPORTED;}
        uint32_t mask=0;
        for (;n>=0;n--) {
            neo_gactor_t *c=ga_spawn_from(a,e,0x35bf8,0,0);
            if (c) for (unsigned o=0x6e;o<=0x78;o+=2) ga_sw(c,o,W(o));   /* 0363D4 */
            uint16_t d2;
            do d2=ga_rand_range(e,5)&0x1fu; while ((mask>>d2)&1u);
            mask|=1u<<d2;
            if (c) ga_sw(c,0x86,d2);
        }
        return GA_DELETE;                                               /* 004188 */
    }
    case 0x35bf8: {
        SB(0x96,0);bat_init(a,e);
        SL(0x7e,rlong(e,0x35adcu+4u*W(0x86)));
        if (!bat_wings(a,e)) return GA_UNSUPPORTED;
        int16_t d0=(int16_t)(ga_rand_range(e,0x1e1)+W(0x72)-0xf0u);
        uint32_t vx=(uint32_t)(int32_t)d0<<8;SL(0x30,vx);
        if (!(vx&0x80000000u)) SB(0x15,B(0x15)|1u);
        SL(0,0x35c42);
    }   /* fall through */
    case 0x35c42:
        SL(0x34,L(0x7e));SL(0x7e,L(0x7e)+0x1400u);
        if (k35af0(a,e)) {SL(0,0x36068);break;}
        ga_move(a,e);
        if (ga_cull(a,e,0x35a54)) return GA_DELETE_TREE;                /* 03623E -> 0041AC */
        if (bat_anim(a,e)<0) return GA_UNSUPPORTED;
        return GA_MORE;
    case 0x35c68:
        SB(0x96,2);bat_init(a,e);SL(0,0x35f0e);
        break;
    case 0x35f0e:
        SB(0x15,B(0x15)&0xfeu);SL(0x62,0);START(0x23b838);
        SW(0x8c,ga_rand_range(e,0xdc)+0x50u);                            /* 03622A */
        SL(0,0x35f32);
        /* fall through */
    case 0x35f32:
        ga_stationary(a,e);
        if ((r=bat_anim(a,e))<0) return GA_UNSUPPORTED;
        if (r) START(0x23b838);
        SW(0x8c,W(0x8c)-1u);
        if (S16(W(0x8c))<0) {
            SW(0x8c,ga_rand_range(e,0xdc)+0x50u);
            START((ga_rand(e)&1u)?0x23b912:0x23b88e);
        }
        if (ga_cull(a,e,0x35a54)) return GA_DELETE_TREE;
        if (k35af0(a,e)) {SL(0,0x36068);break;}
        SW(0x92,W(0x92)-1u);
        if (S16(W(0x92))>=0) {
            if (e->camera<W(0x90)) return GA_MORE;
            SW(0x90,0xffff);
        }
        SL(0,0x35fbc);
        break;
    case 0x35fbc:
        if (!bat_wings(a,e)) return GA_UNSUPPORTED;
        SB(0x60,0);bat_pick(a,e);SL(0,0x35fd0);
        return GA_MORE;
    case 0x35fd0: {
        if (B(0x96)!=2u) {ga_fail(e,0x35fd0);return GA_UNSUPPORTED;}   /* +96 = 1/3: 036502.. not ported */
        int16_t x,y;bat_waypoint(a,&x,&y);
        int16_t dx=(int16_t)(x-S16(W(0x28))),dy=(int16_t)(y-S16(W(0x2c)));
        uint16_t ax=(uint16_t)(dx<0?-dx:dx),ay=(uint16_t)(dy<0?-dy:dy);
        if (ax<=3u && ay<=3u) {SL(0,0x35f0e);break;}                    /* 0365BE */
        uint8_t ang=neo_angle(dx,dy),d1=ang;                             /* 03654C */
        int8_t d2=0x18;
        if (!((uint8_t)(ang+0x40u)&0x80u)) d2=(int8_t)-d2;
        d1=(uint8_t)(d1+(uint8_t)d2);
        uint32_t vx,vy;ga_polar(0x10000,d1,&vx,&vy);SL(0x30,vx);SL(0x34,vy);
        ga_move(a,e);
        if (bat_anim(a,e)<0) return GA_UNSUPPORTED;
        if (ga_cull(a,e,0x35a54)) return GA_DELETE_TREE;
        if (k35af0(a,e)) {SL(0,0x36068);break;}
        if (!B(0x60)) return GA_MORE;
        uint32_t wx,wy;ga_polar(0x38000,(uint8_t)(ga_rand(e)&0x7fu),&wx,&wy);   /* 036254 */
        SL(0x7a,wx);SL(0x7e,wy);SB(0x15,B(0x15)&0xfeu);
        if (!(wx&0x80000000u)) SB(0x15,B(0x15)|1u);
        SW(0x8e,7);SL(0,0x3603c);
        return GA_MORE;
    }
    case 0x3603c:
        SL(0x30,L(0x7a));SL(0x34,L(0x7e));ga_move(a,e);
        if (bat_anim(a,e)<0) return GA_UNSUPPORTED;
        if (k35af0(a,e)) {SL(0,0x36068);break;}
        if (ga_cull(a,e,0x35a54)) return GA_DELETE_TREE;
        SW(0x8e,W(0x8e)-1u);
        if (S16(W(0x8e))>=0) return GA_MORE;
        SL(0,0x35fbc);
        break;
    case 0x36068:
        if (e->sound) e->sound(e->ctx,0x10f0);
        START(B(0x9f)==7u?0x23b76c:0x23b68c);SL(0,0x360a8);
        return GA_MORE;
    case 0x360a8:
        ga_project(a,e);
        if ((r=ga_anim_step(a,e,NULL))<0) return GA_UNSUPPORTED;
        if (!r) return ga_cull(a,e,0x35a54)?GA_DELETE_TREE:GA_MORE;
        START(B(0x9f)==7u?0x23b7aa:0x23b6de);
        if (B(0x96)==3u) {ga_fail(e,0x360f6);return GA_UNSUPPORTED;}
        SL(0x30,0);SL(0x34,0);SL(0,0x3610e);
        return GA_MORE;
    case 0x3610e:
        if (!(B(0x15)&0x40u)) {
            SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);                   /* 0143DC */
            if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
            return ga_cull(a,e,0x35a54)?GA_DELETE_TREE:GA_MORE;
        }
        SB(0x9b,0xff);
        if (!(uint8_t)ga_rand_range(e,3)) {SB(0x9b,8);SB(0x9c,0);}
        if (!ga_anim_start(a,e,B(0x9f)==7u?0x23b7ec:0x23b720,(B(0x9b)&0x80u)?0x100:0x200)) return GA_UNSUPPORTED;
        SL(0,0x3618a);
        return GA_MORE;
    case 0x3618a:
        if (B(0x15)&0x40u) {
            SL(0x30,0);SL(0x34,0);                                      /* 03663E */
            if (!(B(0x9b)&0x80u)) {
                int16_t d0=(int16_t)(ga_rand_range(e,7)-3u);
                int8_t d1=(int8_t)(B(0x9b)+(uint8_t)d0);
                if (d1<0) d0=0;
                if ((uint8_t)d1>0x10u) d0=0;
                SB(0x9b,B(0x9b)+(uint8_t)d0);SL(0x30,(uint32_t)(int32_t)d0<<16);
                d0=(int16_t)(ga_rand_range(e,3)-1u);
                d1=(int8_t)(B(0x9c)+(uint8_t)d0);
                if (d1<0) d0=1;
                if ((uint8_t)d1>1u) d0=0;
                SB(0x9c,B(0x9c)+(uint8_t)d0);SL(0x34,(uint32_t)(int32_t)d0<<16);
            }
            ga_air(a,e);                                                /* 0143E4 */
        } else {
            if ((int32_t)L(0x34)>0) SL(0x34,0);
            SL(0x30,0);SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e);        /* 0143DC */
        }
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        SB(0x9d,B(0x9d)+1u);
        if (B(0x9d)>=0x50u) return GA_DELETE_TREE;                      /* 0041AC */
        if (B(0x9d)==0x28u) SB(0x5f,B(0x5f)|8u);
        return ga_cull(a,e,0x35a54)?GA_DELETE_TREE:GA_MORE;
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool bat_routine(uint32_t r) {
    return r==0x35b66u || r==0x35bf8u || r==0x35c42u || r==0x35c68u || r==0x35f0eu || r==0x35f32u || r==0x35fbcu ||
        r==0x35fd0u || r==0x3603cu || r==0x36068u || r==0x360a8u || r==0x3610eu || r==0x3618au;
}
bool neo_drone_routine(uint32_t r) {
    return bat_routine(r) || r==0x3670eu || r==0x367b6u || r==0x368c2u || r==0x3690au || r==0x369acu || r==0x369e2u || r==0x36a3au;
}
ga_result_t neo_drone_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (bat_routine(r)) return bat_tick(a,e);
    if (r>=0x369acu) return bomb_tick(a,e);
    return drone_tick(a,e);
}
