#include "neo_region.h"
#include "neo_soldier_ai.h"
/* The player's straight bullets (ROM 01B6DC..01BD00) and grenades
 * (01BF10..01C306), manual translation;
 * ROM labels are provenance only. Constructors: 01B6DC / 01B716 (H, ids 5
 * / A: sound 10F4 at most once per 12 frames, 01B60E), 01B750 (silent) and
 * 01B782 (pistol: sound 10F7), each with impact program +7A = 22E504, speed
 * +72 = 0x100000 and its own program, palette 202; then 01B7BA: three
 * ricochets (+78), velocity from the angle +6E (002E7C), direction indices
 * +6F/+70/+71.
 *
 * 01B882 each update: 0143E4 (flight), attack row 01B842[+6F] through
 * 0049E8 (the world's hurtboxes: soldiers, domes, shop, armoured vehicle);
 * a hit of type 3 ricochets (sound 108E, random turn, impact 01BC2E, one
 * update with row 01B5C2 at 01BBE2), type 2 sounds 108D, every hit leaves an
 * impact 01BC2E and the bullet goes. 013CF8 looks for water (class 5) under
 * the bullet: found, the splash 03C962 (neo_gate.c) and +79 = FF (the
 * bullet goes at its next wall). A wall (+15 bit 7) turns the bullet into its wall
 * impact (programs 22C884 / 22C81E, routine 01BC82). It goes when its x
 * word leaves 0..13F or its y word 100..1FF. The paths that would run the
 * cartridge check (2FFFE0) end in 004188 on genuine hardware: delete. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
static uint32_t rl(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint32_t)(p[0]<<24|p[1]<<16|p[2]<<8|p[3]):0;}
static void sound(neo_genv_t *e,uint16_t id) {if (e->sound) e->sound(e->ctx,id);}
/* 01B60E: the H sound at most once per 12 frames (RAM 10C33E). */
static void throttled(neo_genv_t *e,uint16_t id) {
    if (!e->pickup) {sound(e,id);return;}
    uint16_t d7=(uint16_t)(e->pickup->shot_sound-e->video);
    if ((uint32_t)d7+0xcu>0xffffu) return;
    e->pickup->shot_sound=e->video;sound(e,id);
}
/* 01B7EE..01B834 / 01BB80..01BBAE: direction indices from the angle. */
static void index6f(neo_gactor_t *a) {uint8_t d=B(0x6e);if (d&0x20u) d=(uint8_t)(d+0xfu);SB(0x6f,(d>>4)&15u);}
static void index70(neo_gactor_t *a) {uint8_t d=B(0x6e);if (d&0x20u) d=(uint8_t)(d+7u);SB(0x70,(d>>3)&0x1fu);}
static void index71(neo_gactor_t *a) {uint8_t d=B(0x6e);if (d&0x20u) d=(uint8_t)(d+3u);SB(0x71,(d>>2)&0x3fu);}
static void velocity(neo_gactor_t *a) {uint32_t vx,vy;ga_polar(L(0x72),B(0x6e),&vx,&vy);SL(0x30,vx);SL(0x34,vy);}
static int16_t jitter(neo_genv_t *e) {return (int16_t)((ga_rand(e)&15u)-7);}
/* 01BC02 / 01BBB2: the impact 01BC2E (list 101414) at the bullet. */
static void impact(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=e->spawn(e->ctx,a,0x1bc2e,true);
    if (!c) return;
    ga_sl(c,0x7a,L(0x7a));ga_place_at(c,a,e,0,0);
}
enum { RUN, DONE, STOP };
/* 01B8A6..01BA44 (and the wall impact 01BA46). */
static int after_attack(neo_gactor_t *a,neo_genv_t *e) {
    if (!B(0x79)) SB(0x79,neo_gate_splash(a,e,0)?0xff:0);         /* 03C5C6 (the splash 03C962), SCS */
    if (B(0x15)&0x80u) {
        if (B(0x15)&0x40u) {if (e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));}  /* 00505A */
        if (B(0x79)) return DONE;
        /* 01BA46: wall impact. */
        if (!((uint8_t)(B(0x6e)-0x40u)&0x80u)) SB(0x15,B(0x15)|1u);
        SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        uint32_t program;
        if (B(0x5f)&1u) {SW(0x2c,W(0x2c)+(uint16_t)jitter(e));program=0x22c884;}
        else if (B(0x5f)&2u) {SW(0x28,W(0x28)+(uint16_t)jitter(e));SB(0x15,B(0x15)|2u);program=0x22c81e;}
        else {
            SW(0x28,W(0x28)+(uint16_t)jitter(e));
            if (B(0x6d)!=0x40u) SW(0x2c,W(0x2c)+(uint16_t)jitter(e));
            program=0x22c81e;
        }
        if (!ga_anim_start(a,e,program,0x100)) return STOP;
        ga_palette(a,0x202);SL(0,0x1bc82);
        return RUN;
    }
    if (ga_anim_step(a,e,NULL)<0) return STOP;
    if (W(0x3c)>=0x140u) return DONE;
    if ((uint16_t)(W(0x40)-0x100u)>=0x100u) return DONE;
    SB(0x7f,(B(0x7f)+3u)&15u);
    SB(0x80,B(0x80)-1u);
    if ((int8_t)B(0x80)>0) return RUN;
    if (B(0x7e)<5u) SB(0x7e,B(0x7e)+1u);
    SB(0x80,1);
    return RUN;
}
static ga_result_t result(int r) {return r==RUN?GA_MORE:r==DONE?GA_DELETE:GA_UNSUPPORTED;}
static ga_result_t bullet_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t program;
    switch (L(0)) {
    /* Form 2 (fat Marco, table 0C42EA / 0C4454 / 0C447C): the pistol
     * 01B62C (program 22C0A6) and the H 01B668 (+7A 22C040, program
     * 22C000), as 01B782/01B6DC with their own programs. */
    case 0x1b62c: sound(e,0x10f7);program=0x22c0a6;goto build;
    case 0x1b668: throttled(e,0x10f4);SL(0x7a,0x22c040);program=0x22c000;goto build7a;
    case 0x1b6dc: throttled(e,0x10f4);program=0x22c1b6;goto build;
    case 0x1b716: throttled(e,0x10f4);program=0x22c3be;goto build;
    case 0x1b750: program=0x22c12e;goto build;
    case 0x1b782: sound(e,0x10f7);program=0x22c12e;
    build:
        SL(0x7a,0x22e504);
    build7a:
        SL(0x72,0x100000);
        START(program);ga_palette(a,0x202);
        goto common;
    case 0x1bdd2:                                                    /* the H's other bullet with +93 = 2 (0C447C) */
        ga_palette(a,0x874);throttled(e,0x10f4);goto vulcan;
    case 0x1be24:                                                    /* the Camel Slug's vulcan */
        ga_palette(a,0x884);sound(e,0x108c);
    vulcan:
        SL(0x7a,0x22c040);SL(0x72,0x100000);START(0x22c4c6);         /* 01BDE8 */
        SB(0x7e,0);SB(0x7f,(uint8_t)ga_rand(e)>>4);SB(0x80,1);
        goto common;
    case 0x1b6a0:                                                    /* the SV-001's vulcan */
        sound(e,0x108c);SL(0x7a,0x22e504);SL(0x72,0x100000);START(0x22c1b8);ga_palette(a,0x3d6);
    common:
        /* 01B7BA */
        SB(0x78,3);SB(0x15,B(0x15)|0x20u);SW(0x66,0xe000);SW(0x1e,0xe000);SB(0x15,B(0x15)&0xfeu);
        velocity(a);index6f(a);index70(a);index71(a);
        SL(0,0x1b882);
        /* fall through */
    case 0x1b882: {
        ga_air(a,e);
        uint8_t type=ga_attack(a,e,rl(0x1b842u+4u*(B(0x6f)&15u)));
        if (e->unsupported) return GA_UNSUPPORTED;
        if (type) {
            /* 01BB02 */
            if (e->pickup) e->pickup->bullet_hits++;
            if (type==2) sound(e,0x108d);
            if (type==3) {
                sound(e,0x108e);
                SB(0x78,B(0x78)-1u);
                if ((int8_t)B(0x78)>=0) {
                    /* Ricochet: a random turn of the angle. */
                    uint16_t d0=(uint16_t)((ga_rand(e)&0x3fffu)-0x1f80u);
                    d0=(uint16_t)(d0+(uint16_t)((B(0x6e)<<8)+0x8000u));
                    SB(0x6e,d0>>8);
                    velocity(a);index6f(a);index71(a);
                    impact(a,e);
                    SL(0,0x1bbe2);
                    return result(after_attack(a,e));
                }
            }
            impact(a,e);return GA_DELETE;                            /* 01BC02 */
        }
        return result(after_attack(a,e));
    }
    case 0x1bbe2:
        ga_air(a,e);
        if (!ga_attack(a,e,0x1b5c2)) SL(0,0x1b882);
        if (e->unsupported) return GA_UNSUPPORTED;
        return result(after_attack(a,e));
    case 0x1bc2e:                                                    /* impact */
        ga_palette(a,0x202);SW(0x66,0xe0ed);SW(0x1e,0xe0ed);
        if (e->front_layer) {if (e->front_layer(e->ctx)) SB(0x15,B(0x15)|4u); else SB(0x15,B(0x15)&0xfbu);}  /* 012022 */
        START(L(0x7a));
        SW(0x28,W(0x28)+(uint16_t)jitter(e));SW(0x2c,W(0x2c)+(uint16_t)jitter(e));
        SL(0,0x1bc82);
        /* fall through */
    case 0x1bc82: {
        ga_project(a,e);                                             /* 01ABC8 */
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        /* 01BD20: outside the x words 0..13F or the y words 100..1FF. */
        if (W(0x3c)>=0x140u || (uint16_t)(W(0x40)-0x100u)>=0x100u) return GA_DELETE;
        return GA_MORE;
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
/* ---- grenades 01BF10 / 01BF46 (0C4786, bomb type 1) ---------------------- */
static int32_t asr(int32_t v,unsigned n) {return v>=0?v>>n:~(~v>>n);}
static int32_t muls(uint32_t v,int16_t w) {return (int32_t)(int16_t)(uint16_t)v*(int32_t)w;}
/* 01C6AC: (d0,d1) reflected on the surface of angle d4; (d2,d3) the
 * normal component. */
static void k1c6ac(int32_t *d0,int32_t *d1,uint8_t d4,int32_t *d2o,int32_t *d3o) {
    int16_t sn=ga_sine(d4),cs=ga_cosine(d4);
    int32_t d2=muls((uint32_t)asr(*d0,8),sn),d3=muls((uint32_t)asr(*d1,8),cs);
    d2=asr((int32_t)((uint32_t)d2-(uint32_t)d3),8);d3=d2;
    d2=muls((uint32_t)d2,sn);d3=muls((uint32_t)d3,cs);d3=(int32_t)(0u-(uint32_t)d3);
    uint32_t x=(uint32_t)*d0-(uint32_t)d2,y=(uint32_t)*d1-(uint32_t)d3;
    x=0u-x;y=0u-y;x+=(uint32_t)d2;y+=(uint32_t)d3;
    *d0=(int32_t)x;*d1=(int32_t)y;*d2o=d2;*d3o=d3;
}
static void landed(neo_gactor_t *a,neo_genv_t *e) {if (e->pickup && e->pickup->weapons) marco_weapons_landed(e->pickup->weapons,W(0x6e));}  /* 0C0D48 */
static ga_result_t grenade_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t vy;
    uint16_t palette=0x27c;uint32_t program=0x22c950;
    switch (L(0)) {
    case 0x1bea4: vy=0x6448f;palette=0x204;program=0x22ca58;goto build;   /* +93 = 2 (table 0C476E), +90 = FF */
    case 0x1beda: vy=0x85b6a;palette=0x204;program=0x22ca58;goto build;   /* +93 = 2 (table 0C476E) */
    case 0x1bf10: vy=0x6448f;goto build;
    case 0x1bf46: vy=0x85b6a;
    build: {
        ga_palette(a,palette);START(program);
        SL(0x84,0xfff053a0);
        uint32_t vx=0x90000;uint16_t spin=0x320;
        if (B(0x15)&1u) {vx=0u-vx;spin=(uint16_t)(0u-spin);}
        SL(0x30,L(0x30)+vx);SL(0x34,vy);SW(0x74,spin);SW(0x72,0);
        SW(0x66,0xe000);SW(0x1e,0xe000);SB(0x15,(B(0x15)|0x20u)&0xfeu);SW(0x70,1);
        /* 03C526: the water ripple 03C62A when 019240 has a palette for
         * the mission (none in Mission 1, RAM 10C2F6 = 0). */
        if (!neo_gate_ripple(a,e,1)) return GA_UNSUPPORTED;
        if (e->pickup && e->pickup->weapons) marco_weapons_armed(e->pickup->weapons,W(0x6e));  /* 0C0D34 */
        SL(0,0x1bfd0);
    }   /* fall through */
    case 0x1bfd0: {
        if (ga_attack(a,e,0x1b5d2)) goto explode;
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0x78,L(0x30));SL(0x7c,L(0x34));
        uint16_t d4=0;ga_air_d4(a,e,&d4);
        if (e->unsupported) return GA_UNSUPPORTED;
        SW(0x76,d4);
        bool bounce=(B(0x15)&0x40u) || ((B(0x15)&0x80u) && !(B(0x5f)&3u));
        if (bounce) {
            if (e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));      /* 00505A */
            uint8_t angle=B(0x76);
            int32_t d0=(int32_t)L(0x78),d1=(int32_t)L(0x7c),d2,d3;
            k1c6ac(&d0,&d1,angle,&d2,&d3);
            SL(0x30,(uint32_t)d0);SL(0x34,(uint32_t)d1);
            int16_t sn=ga_sine(angle),cs=ga_cosine(angle);
            int32_t x=muls((uint32_t)asr(d2,8),sn),y=muls((uint32_t)asr(d3,8),cs);
            x=asr(asr((int32_t)((uint32_t)x-(uint32_t)y),8),3);
            uint16_t s74=W(0x74);
            uint16_t w=(uint16_t)((uint16_t)x-s74);w=(uint16_t)((int16_t)w>>1);
            SW(0x74,(uint16_t)(s74+w));
            uint16_t s3=(uint16_t)(s74<<3);
            int32_t px=muls(s3,sn),py=(int32_t)(0u-(uint32_t)muls(s3,cs));
            px=asr((int32_t)((uint32_t)px-(uint32_t)d2),1);py=asr((int32_t)((uint32_t)py-(uint32_t)d3),1);
            SL(0x30,L(0x30)+(uint32_t)px);SL(0x34,L(0x34)+(uint32_t)py);
            uint16_t n=W(0x70);SW(0x70,(uint16_t)(n-1u));
            if (!n) goto explode;                                   /* BCS */
        }
        /* 01C0A6: gravity towards +84, drag, spin. */
        SL(0x34,L(0x34)+(uint32_t)asr((int32_t)(L(0x84)-L(0x34)),4));
        SL(0x30,L(0x30)-(uint32_t)asr((int32_t)L(0x30),4));
        {uint16_t d0=W(0x74),d1=(uint16_t)((int16_t)d0>>5);
         SW(0x74,(uint16_t)(d0-d1));SW(0x72,(uint16_t)((d0+W(0x72))&0x1fffu));}
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (W(0x3c)>=0x140u || (uint16_t)(W(0x40)-0x100u)>=0x140u) {landed(a,e);return GA_DELETE;}
        return GA_MORE;
    explode:                                                         /* 01C10C */
        landed(a,e);SL(0x30,0);SL(0x34,0);SL(0,0x1c152);
        return GA_MORE;
    }
    case 0x1c152:                                                    /* explosion */
        sound(e,0x1027);SB(0x1d,0);                                  /* 009F88 */
        ga_palette(a,0x378);SW(0x66,0x1001);SW(0x1e,0x1001);
        START(0x22cb60);SL(0x80,0x1b602);SL(0,0x1c19a);
        /* fall through */
    case 0x1c19a: {
        /* Its attack rows 01B5E2, 01B5F2 (list 01B602), one per update. */
        uint32_t row=rl(L(0x80));
        if (row) {SL(0x80,L(0x80)+4u);if (ga_attack(a,e,row) && e->pickup) e->pickup->blast_hits++;if (e->unsupported) return GA_UNSUPPORTED;}
        ga_project(a,e);                                             /* 01ABC8 */
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) return GA_DELETE;
        if ((uint16_t)(W(0x3c)+0x20u)>=0x180u || (uint16_t)(W(0x40)-0x100u)>=0x140u) return GA_DELETE;
        return GA_MORE;
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool grenade_routine(uint32_t r) {return r==0x1bea4 || r==0x1beda || r==0x1bf10 || r==0x1bf46 || r==0x1bfd0 || r==0x1c152 || r==0x1c19a;}
/* ---- bomb type 3 01C362 / 01C334 (+93 = 2, table 0C476E; family m3bomb) ---
 * 01C362 (+90 = FF: 01C334): +82 = FF, program 22CD04, palette 20A, +30 +=
 * 90000 (facing), +34 += 85B6A (6448F), gravity +7E = FFF053A0, three
 * bounces (+70), the ripple 03C526. 01C448 each update: unless stopped
 * (+5F bit 3) the attack row 01C308; a hit sounds 10F9 (type 5) or 116D,
 * stops (01C318), reverses +30, makes +34 positive and leaves the piece
 * 01C654. 0143E4 (+72 = D4); on a bounce 00505A and the reflection 01C6AC,
 * a piece when the rebound's +34 word > 2, the third stops. 01C318: +5F
 * bit 3, 0x1E updates (+74) to go, 0C0D48. Out of the x words 0..13F or
 * the y words 100..23F: 0C0D48 unless stopped, the cartridge check (on
 * genuine hardware 004188: delete). The thin 01C3BE / 01C390 (+82 = 0,
 * 22CC34) share the code; no capture. */
static void bomb_stop(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x5f)&8u) return;
    SB(0x5f,B(0x5f)|8u);SW(0x74,0x1e);landed(a,e);
}
static void bomb_piece(neo_gactor_t *a,neo_genv_t *e) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x1c654,true):NULL;   /* 00411C (+0C = 101414), reserve 0x2A */
    if (!c) return;
    ga_sb(c,0x82,B(0x82));ga_place_at(c,a,e,0,0);                     /* 01ACF2 */
}
static ga_result_t bomb_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x1c334: case 0x1c362: case 0x1c390: case 0x1c3be: {
        const uint32_t r=L(0);const bool fat=r==0x1c334u || r==0x1c362u;
        SB(0x82,fat?0xff:0);START(fat?0x22cd04:0x22cc34);
        /* 01C3EC: the palette call 009EA8 replaces the low words of the
         * speeds D0/D1: D0.w = its hardware slot, D1.w = 20A. */
        const unsigned n=B(0x1d);
        uint32_t vx=0x90000u|(e->palette_slot?e->palette_slot(e->ctx,a,n):0u),vy=((r==0x1c334u || r==0x1c390u)?0x60000u:0x80000u)|0x20au;
        SL(0x7e,0xfff053a0u);
        ga_palette(a,0x20a);
        if (B(0x15)&1u) vx=0u-vx;
        SL(0x30,L(0x30)+vx);SL(0x34,L(0x34)+vy);
        SW(0x66,0xe000);SW(0x1e,0xe000);SB(0x15,B(0x15)&0xfeu);SW(0x70,3);
        if (!neo_gate_ripple(a,e,1)) return GA_UNSUPPORTED;            /* 03C526 */
        SW(0x74,0);
        if (e->pickup && e->pickup->weapons) marco_weapons_armed(e->pickup->weapons,W(0x6e));  /* 0C0D34 */
        SB(0x15,B(0x15)|0x20u);SL(0,0x1c448);
    }   /* fall through */
    case 0x1c448: {
        if (!(B(0x5f)&8u)) {
            uint8_t t=ga_attack(a,e,0x1c308);
            if (e->unsupported) return GA_UNSUPPORTED;
            if (t) {
                sound(e,t==5u?0x10f9:0x116d);
                bomb_stop(a,e);
                SL(0x30,0u-L(0x30));
                if ((int32_t)L(0x34)<0) SL(0x34,0u-L(0x34));
                bomb_piece(a,e);
            }
        }
        SL(0x76,L(0x30));SL(0x7a,L(0x34));
        uint16_t d4=0;ga_air_d4(a,e,&d4);
        if (e->unsupported) return GA_UNSUPPORTED;
        SW(0x72,d4);
        if ((B(0x15)&0x40u) || ((B(0x15)&0x80u) && !(B(0x5f)&3u))) {
            if (e->grid) neo_grid_splat(e->grid,W(0x3c),W(0x40));      /* 00505A */
            int32_t d0=(int32_t)L(0x76),d1=(int32_t)L(0x7a),d2,d3;
            k1c6ac(&d0,&d1,B(0x72),&d2,&d3);
            SL(0x30,(uint32_t)d0);SL(0x34,(uint32_t)d1);
            if ((int16_t)W(0x34)>2) bomb_piece(a,e);
            SW(0x70,W(0x70)-1u);
            if (!W(0x70)) bomb_stop(a,e);
        }
        if (W(0x74)) {
            SW(0x74,W(0x74)-1u);
            if (!W(0x74)) return GA_DELETE;                              /* 004188 */
            SB(0x5f,B(0x5f)|8u);
        }
        SL(0x34,L(0x34)+(uint32_t)asr((int32_t)(L(0x7e)-L(0x34)),4));
        SL(0x30,L(0x30)-(uint32_t)asr((int32_t)L(0x30),4));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (W(0x3c)<0x140u && (uint16_t)(W(0x40)-0x100u)<0x140u) return GA_MORE;
        if (!(B(0x5f)&8u)) landed(a,e);                                 /* 0C0D48 */
        return GA_DELETE;                                               /* cartridge check, 004188 */
    }
    /* 01C654: the piece (+82 = 0: 22CC8A, else 22CD5A), palette 204,
     * 01ABC8 until its program ends. */
    case 0x1c654:
        START(B(0x82)?0x22cd5a:0x22cc8a);ga_palette(a,0x204);SL(0,0x1c694);
        /* fall through */
    case 0x1c694: {
        ga_project(a,e);
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        return r?GA_DELETE:GA_MORE;
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool bomb_routine(uint32_t r) {return r==0x1c334 || r==0x1c362 || r==0x1c390 || r==0x1c3be || r==0x1c448 || r==0x1c654 || r==0x1c694;}
/* ---- 0C4E86: the fat player's landing dust (0C1ED6, 0040BC reserve 0x2A,
 * 01ACF2 (0, 0)): program 0C4E48, palette 2D; 0C4EAA each update: +1E =
 * the creator's +1E - 1 (the player's: 8834 from 0C1EC4), 01ABC8, 00427A;
 * at its end the cartridge check (004188 on genuine hardware: delete). */
static const uint8_t *player_node(const neo_gactor_t *a,const neo_genv_t *e) {
    if (!e->vehicle) return NULL;
    return e->vehicle->player[L(0x0c)==0x101574u?1:0];
}
static ga_result_t dust_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xc4e86u) {START(0xc4e48);ga_palette(a,0x2d);SL(0,0xc4eaa);}
    const uint8_t *p=player_node(a,e);
    uint16_t z=p?(uint16_t)(p[0x1e]<<8|p[0x1f]):0x8834u;
    SW(0x1e,(uint16_t)(z-1u));
    ga_project(a,e);
    int r=ga_anim_step(a,e,NULL);
    if (r<0) return GA_UNSUPPORTED;
    return r?GA_DELETE:GA_MORE;
}
static bool dust_routine(uint32_t r) {return r==0xc4e86 || r==0xc4eaa;}
/* ---- molotov 01E648 / 01E664 (0C4786, bomb type 2; +88 = 0) ------------- */
/* 0040BC with D0 = reserve, then 01B220 (0, 0). */
static neo_gactor_t *child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,uint16_t reserve) {
    neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,routine,reserve):e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (c) ga_place_from(c,a,e,0,0);
    return c;
}
/* 01E99A: 0C0D48, or with +6E < 0 (the Slugnoid's shell, 01EC0E) 097B24:
 * the walker that fired it gets its shell back (its +8A - 1); 01E4F2. */
static ga_result_t molotov_end(neo_gactor_t *a,neo_genv_t *e) {
    if (W(0x6e)&0x8000u) {
        neo_gactor_t *w=e->creator?e->creator(e->ctx,a):NULL;
        if (w) ga_sb(w,0x8a,(uint8_t)(ga_b(w,0x8a)-1u));
        return GA_DELETE;
    }
    landed(a,e);return GA_DELETE;
}
static bool flame_pair(neo_gactor_t *a,neo_genv_t *e) {
    for (int i=0;i<2;i++) {
        neo_gactor_t *c=child(a,e,0x1e9b6,0x1c);
        if (!c) continue;
        ga_sb(c,0x5e,B(0x5e));ga_sl(c,0x62,i?0u-L(0x62):L(0x62));ga_sb(c,0x86,B(0x86));ga_sb(c,0x88,B(0x88));
    }
    return true;
}
static ga_result_t molotov_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (;;) switch (L(0)) {
    case 0x1e648: case 0x1e664: case 0x1e680: case 0x1e69c: {
        /* 01E680/01E69C (+93 = 2, table 0C476E): +88 = 1, palette 872,
         * program 22F65E; the flames 01EA7A (family m3molotov). */
        const uint32_t r=L(0);const bool fat=r==0x1e680u || r==0x1e69cu;
        SB(0x88,fat?1:0);
        uint32_t vx=0x80000,vy=(r==0x1e648u || r==0x1e680u)?0x85b6au:0x6448fu;
        SB(0x15,B(0x15)|0x20u);
        if (B(0x15)&1u) vx=0u-vx;
        SL(0x30,L(0x30)+vx);SL(0x34,L(0x34)+vy);SL(0x72,0xfff053a0u);
        if (e->pickup && e->pickup->weapons) marco_weapons_armed(e->pickup->weapons,W(0x6e));  /* 0C0D34 */
        SW(0x66,0xe000);SW(0x1e,0xe000);
        if (fat) {ga_palette(a,0x872);START(0x22f65e);}
        else {ga_palette(a,0x203);START(0x22f5b8);}
        SL(0,0x1e738);
    }   /* fall through */
    case 0x1e738: {
        uint8_t r=ga_attack(a,e,0x1f126);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (r==2 || r==3) {                                          /* 01E7B0 */
            sound(e,0x1027);
            neo_gactor_t *c=child(a,e,0x1c7ea,0x1c);
            if (c) {ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);}
            ga_attack(a,e,0x1f136);
            if (e->unsupported) return GA_UNSUPPORTED;
            return molotov_end(a,e);
        }
        ga_air(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0x30,L(0x30)-(uint32_t)asr((int32_t)L(0x30),4));
        SL(0x34,L(0x34)+(uint32_t)asr((int32_t)(L(0x72)-L(0x34)),4));
        if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
        if (B(0x15)&0x40u) SL(0,0x1e80e);
        if (W(0x3c)>=0x140u || (uint16_t)(W(0x40)-0x100u)>=0x140u) SL(0,0x1e99a);
        return GA_MORE;
    }
    case 0x1e80e: {                                                  /* landed */
        SB(0x86,0);SL(0x62,0);SW(0x2c,W(0x2c)+4u);
        ga_project(a,e);
        neo_gactor_t *c=child(a,e,0x1e9b6,0xe);
        if (c) {ga_sb(c,0x5e,B(0x5e));ga_sb(c,0x88,B(0x88));}
        SL(0x62,0x100000);
        if (B(0x88)==1u) {SW(0x7c,4);SB(0x87,0x18);sound(e,0x116c);}   /* type 3 (01EC0E) */
        else {SW(0x7c,0);SB(0x87,0);sound(e,0x10f3);}
        SL(0,0x1e894);
    }   /* fall through */
    case 0x1e894:
        ga_project(a,e);
        SW(0x7c,W(0x7c)-1u);
        if ((int16_t)W(0x7c)<=0) {
            SW(0x7c,B(0x88)==1u?3:2);SB(0x86,B(0x86)+1u);
            flame_pair(a,e);
            if ((int8_t)B(0x86)>=4) SL(0,0x1e97a);
        }
        if (W(0x3c)>=0x140u || (uint16_t)(W(0x40)-0x100u)>=0x140u) SL(0,0x1e97a);
        if ((int8_t)B(0x87)>0) SB(0x87,B(0x87)-1u);                  /* 01E98C */
        return GA_MORE;
    case 0x1e97a:
        if ((int8_t)B(0x87)>0 && e->has_children && e->has_children(e->ctx,a)) {SB(0x87,B(0x87)-1u);return GA_MORE;}
        SL(0,0x1e99a);continue;
    case 0x1e99a:
        return molotov_end(a,e);
    /* ---- flame 01E9B6 ---- */
    case 0x1e9b6:
        ga_floor(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        SW(0x66,0x1001);SW(0x1e,0x1001);
        if (B(0x88)==1u) {
            /* 01EA7A: the type-3 flame (palette 0x873, program 22F704,
             * zoom +12/+13 = 0x7F). */
            ga_palette(a,0x873);START(0x22f704);SB(0x12,0x7f);SB(0x13,0x7f);
            SL(0,0x1eaac);return GA_MORE;
        }
        ga_palette(a,0xd5);START(0x242b52);
        SL(0,0x1e9f8);return GA_MORE;
    case 0x1eaac:
        while ((int8_t)B(0x86)>0) {
            ga_ground(a,e,NULL);
            if (e->unsupported) return GA_UNSUPPORTED;
            if (!(B(0x15)&0x40u)) return GA_DELETE;                   /* cartridge check, 004188 */
            SB(0x86,B(0x86)-1u);
        }
        SL(0x76,0x1f156);SL(0x30,0);SL(0x34,0);SL(0x62,0);            /* 01B1DC */
        SL(0,0x1eb72);
        /* fall through */
    case 0x1eb72: {
        /* 01EB72: 01436A, the attack row +76 until it touches, the
         * program, the zoom +12/+13 growing to full size. */
        ga_stationary(a,e);
        if (e->unsupported) return GA_UNSUPPORTED;
        if (L(0x76) && ga_attack(a,e,L(0x76))) SL(0x76,0);
        if (e->unsupported) return GA_UNSUPPORTED;
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x1e4f2);
        if (!(B(0x15)&0x40u)) return GA_DELETE;                       /* cartridge check, 004188 */
        if (W(0x3c)>=0x140u || (uint16_t)(W(0x40)-0x100u)>=0x140u) SL(0,0x1e4f2);
        uint8_t d1=B(0x12);
        if (d1!=0xffu) {
            d1=(uint8_t)(d1+((uint8_t)(0xffu-d1)>>2));
            if (d1>0xe0u) d1=0xff;
            SB(0x12,d1);SB(0x13,d1);
        }
        return GA_MORE;
    }
    case 0x1e9f8:
        while ((int8_t)B(0x86)>0) {
            ga_ground(a,e,NULL);
            if (e->unsupported) return GA_UNSUPPORTED;
            if (!(B(0x15)&0x40u)) return GA_DELETE;                   /* 004188 */
            SB(0x86,B(0x86)-1u);
        }
        ga_project(a,e);
        ga_attack(a,e,0x1f146);
        if (e->unsupported) return GA_UNSUPPORTED;
        SL(0,0x1ea3a);goto step;
    case 0x1ea3a:
        ga_project(a,e);
    step: {
        int r=ga_anim_step(a,e,NULL);
        if (r<0) return GA_UNSUPPORTED;
        if (r) SL(0,0x1e4f2);
        if (W(0x3c)>=0x140u || (uint16_t)(W(0x40)-0x100u)>=0x140u) SL(0,0x1e4f2);
        return GA_MORE;
    }
    default:
        ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static bool molotov_routine(uint32_t r) {
    return r==0x1e648 || r==0x1e664 || r==0x1e680 || r==0x1e69c || r==0x1e738 || r==0x1e80e || r==0x1e894 || r==0x1e97a || r==0x1e99a ||
        r==0x1e9b6 || r==0x1e9f8 || r==0x1ea3a || r==0x1eaac || r==0x1eb72;
}
bool neo_bullet_routine(uint32_t r) {
    return r==0x1b6a0 || r==0x1be24 || r==0x1bdd2 || r==0x1b62c || r==0x1b668 || r==0x1b6dc || r==0x1b716 || r==0x1b750 || r==0x1b782 || r==0x1b882 || r==0x1bbe2 || r==0x1bc2e || r==0x1bc82 || grenade_routine(r) || molotov_routine(r) || bomb_routine(r) || dust_routine(r);
}
ga_result_t neo_bullet_tick(neo_gactor_t *a,neo_genv_t *e) {return molotov_routine(ga_l(a,0))?molotov_tick(a,e):bomb_routine(ga_l(a,0))?bomb_tick(a,e):dust_routine(ga_l(a,0))?dust_tick(a,e):grenade_routine(ga_l(a,0))?grenade_tick(a,e):bullet_tick(a,e);}
