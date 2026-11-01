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
 * the bullet: its splash 03C962 is not translated (explicit stop; Mission 1
 * has no class-5 cell). A wall (+15 bit 7) turns the bullet into its wall
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
    if (!B(0x79)) {
        /* 03C5C6 -> 03C55C: water (class 5) at y-1, y+7, y+15 (013CF8). */
        bool water=false;
        for (unsigned i=0;i<3 && !water;i++)
            water=neo_kernel_point_class(e->kernel,(int32_t)L(0x3c),(int32_t)(L(0x40)-0x10000u+0x80000u*i),5,0,NULL,NULL,NULL);
        if (water) {ga_fail(e,0x3c962);return STOP;}                 /* splash */
        SB(0x79,0);
    }
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
    case 0x1b6dc: throttled(e,0x10f4);program=0x22c1b6;goto build;
    case 0x1b716: throttled(e,0x10f4);program=0x22c3be;goto build;
    case 0x1b750: program=0x22c12e;goto build;
    case 0x1b782: sound(e,0x10f7);program=0x22c12e;
    build:
        SL(0x7a,0x22e504);SL(0x72,0x100000);
        START(program);ga_palette(a,0x202);
        goto common;
    case 0x1be24:                                                    /* the Camel Slug's vulcan */
        ga_palette(a,0x884);sound(e,0x108c);
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
    switch (L(0)) {
    case 0x1bf10: vy=0x6448f;goto build;
    case 0x1bf46: vy=0x85b6a;
    build: {
        ga_palette(a,0x27c);START(0x22c950);
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
static bool grenade_routine(uint32_t r) {return r==0x1bf10 || r==0x1bf46 || r==0x1bfd0 || r==0x1c152 || r==0x1c19a;}
bool neo_bullet_routine(uint32_t r) {
    return r==0x1b6a0 || r==0x1be24 || r==0x1b6dc || r==0x1b716 || r==0x1b750 || r==0x1b782 || r==0x1b882 || r==0x1bbe2 || r==0x1bc2e || r==0x1bc82 || grenade_routine(r);
}
ga_result_t neo_bullet_tick(neo_gactor_t *a,neo_genv_t *e) {return grenade_routine(ga_l(a,0))?grenade_tick(a,e):bullet_tick(a,e);}
