#include "neo_region.h"
#include "neo_soldier_ai.h"
/* The player's straight bullets (ROM 01B6DC..01BD00), manual translation;
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
bool neo_bullet_routine(uint32_t r) {
    return r==0x1b6dc || r==0x1b716 || r==0x1b750 || r==0x1b782 || r==0x1b882 || r==0x1bbe2 || r==0x1bc2e || r==0x1bc82;
}
ga_result_t neo_bullet_tick(neo_gactor_t *a,neo_genv_t *e) {return bullet_tick(a,e);}
