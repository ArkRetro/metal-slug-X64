#include "neo_region.h"
/* Hidden item boxes 09A67E / 09A688 / 09A692 (spawn-table entry 0x2E8;
 * Mission 1 camera 1920: one counted box in group 1 at 2072,136 in the
 * domed building's ruin, then uncounted ones while group 1 is empty),
 * manual translation; ROM labels are provenance only.
 *
 * Words: +6E hurtbox size (09ADF8: 09ADA0.. and the scatter ranges +80/+82
 * of 09AE08), +70 items (0: 1..3 at random), +72 health (+4E; 0 = opened at
 * once), +74 item kind, +76 its +70, +78 0 or the item's +72 = 1, +7A the
 * release motion (09A8E0). 09A67E/09A688 give items (08CD58 -> 08CDB2: item
 * 08C374 after the script actor), 09A692 a prisoner 099E1C after itself
 * (0040BC reserve 0x1C, 01B220, +28/+2C moved by +84/+86; +74/+76 its gift,
 * +6E = our +78: neo_prisoner.c). 09A688
 * counts hits (+72) instead of damage. Opened (+5F bit 7: shots, or the
 * kill-all of 004BAC while 1067E2 is set), it drops one item per update, scattered by +80/+82 (the last at
 * (0,4)); it leaves outside the view rectangle 09ADD8 (0C8BEC). Paths to the
 * cartridge check (2FFFE0) end in 004188 on genuine hardware: translated so. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define BIT(o,b) ((B(o)>>(b))&1u)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static uint16_t rw(uint32_t address) {const uint8_t *p=ga_rom(address);return p?(uint16_t)(p[0]<<8|p[1]):0;}
/* 09A696/09A722 common part: hurtbox and scatter ranges by +6E, item count. */
static void setup(neo_gactor_t *a,neo_genv_t *e) {
    unsigned d0=(W(0x6e)&3u)*4u;
    SL(0x50,(uint32_t)rw(0x9adf8u+d0)<<16|rw(0x9adfau+d0));
    SW(0x80,rw(0x9ae08u+d0));SW(0x82,rw(0x9ae0au+d0));
    (void)e;
}
static void count(neo_gactor_t *a,neo_genv_t *e) {if (!W(0x70)) SW(0x70,ga_rand_range(e,3)+1u);}
/* 09A708: leave outside the view rectangle 09ADD8. */
static ga_result_t view(neo_gactor_t *a,neo_genv_t *e) {
    if (ga_outside_view(a,e,0x9add8)) SL(0,0x9a5ee);
    return GA_MORE;
}
/* 09A7C2: one item per update. */
static ga_result_t release(neo_gactor_t *a,neo_genv_t *e) {
    ga_project(a,e);
    if (S16(W(0x70))<=0) {SL(0,0x9a5ee);return view(a,e);}
    switch (W(0x7a)&3u) { /* 09A8E0 */
    case 0: break;
    default: {
        uint16_t d1=(uint16_t)((ga_rand(e)&0xfu)+0x38u);
        uint32_t speed=(uint32_t)((int32_t)(int16_t)(uint16_t)((ga_rand(e)&0x1ffu)+((W(0x7a)&3u)==1u?0x600u:0x800u))<<8);
        uint32_t vx,vy;ga_polar(speed,(uint8_t)d1,&vx,&vy);
        SL(0x3c,L(0x3c)+vx);SL(0x40,L(0x40)+vy);
    }
    }
    if (S16(W(0x7c))>1) {
        uint16_t d0=W(0x80);if (d0) d0=ga_rand_range(e,d0);
        SW(0x84,(uint16_t)(d0-(uint16_t)(S16(W(0x80))>>1)));
        d0=W(0x82);if (d0) d0=ga_rand_range(e,d0);
        SW(0x86,(uint16_t)(d0-(uint16_t)(S16(W(0x82))>>1)));
    } else {SW(0x84,0);SW(0x86,4);}
    SW(0x70,W(0x70)-1u);
    uint16_t kind=W(0x74);
    if (!W(0x7e)) {                                                 /* 09A842: prisoner 099E1C */
        neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x99e1c,0x1c):e->spawn?e->spawn(e->ctx,a,0x99e1c,false):NULL;
        if (c) {
            ga_place_from(c,a,e,0,0);                               /* 01B220 */
            ga_sw(c,0x28,(uint16_t)(ga_w(c,0x28)+W(0x84)));ga_sw(c,0x2c,(uint16_t)(ga_w(c,0x2c)+W(0x86)));
            ga_sw(c,0x74,W(0x74));ga_sw(c,0x76,W(0x76));ga_sw(c,0x6e,W(0x78));
        }
    } else if (kind<0x5au) {                                               /* 08CDB2 */
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8c374,true):NULL;
        if (c) {
            ga_sb(c,0x8b,0);ga_sw(c,0x70,W(0x76));ga_sw(c,0x6e,kind);ga_sw(c,0x72,0);ga_sb(c,0x7f,0);
            ga_place_at(c,a,e,S16(W(0x84)),S16(W(0x86)));
            ga_sw(c,0x72,W(0x78)?1u:0u);                            /* 08CD58: d5 */
        }
    }
    ga_project(a,e);
    return view(a,e);
}
static ga_result_t secret_tick(neo_gactor_t *a,neo_genv_t *e) {
    switch (L(0)) {
    case 0x9a67e: case 0x9a688: case 0x9a692:
        SW(0x7e,L(0)==0x9a692?0:1);
        setup(a,e);
        if (L(0)==0x9a688) {SW(0x4e,0);count(a,e);SL(0,0x9a76a);goto hits;}
        SW(0x4e,W(0x72));if (!W(0x72)) SB(0x5f,B(0x5f)|0x80u);
        count(a,e);SL(0,0x9a6e8);
        /* fall through */
    case 0x9a6e8:
        ga_project(a,e);ga_register(a,e,NULL,NULL);SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0x9a7b2);
        return view(a,e);
    case 0x9a71e:   /* the hidden prisoner box of Mission 4 (09A71E): +7E = 0, 09A688's hits */
        SW(0x7e,0);setup(a,e);SW(0x4e,0);count(a,e);SL(0,0x9a76a);
        /* fall through */
    case 0x9a76a:
    hits:
        ga_project(a,e);
        if (ga_register(a,e,NULL,NULL)) {
            SB(0x5f,B(0x5f)&0x7fu);SW(0x4e,0);SW(0x72,W(0x72)-1u);
            if (S16(W(0x72))<=0) SB(0x5f,B(0x5f)|0x80u);
        }
        SB(0x5a,0);
        if (BIT(0x5f,7)) SL(0,0x9a7b2);
        return view(a,e);
    case 0x9a7b2: { /* opened */
        SW(0x7c,W(0x70));
        /* 09A956 -> 03C5D2 (table 09A96C, every +6E): 03C55C looks for a
         * group-5 cell at +3C/+40 - 1, +7, +15 (013CF8); found, 03C962 would
         * be created after the script actor (not translated: explicit stop).
         * A dynamic object with a group-5 map would depend on RAM 10C008
         * left by the previous query: explicit stop too. */
        bool unknown=false;
        uint32_t x=L(0x3c),y=L(0x40)-0x10000u;
        for (unsigned i=0;i<3;i++,y+=0x80000u)
            if (neo_kernel_point_group(e->kernel,(int32_t)x,(int32_t)y,5,&unknown) || unknown) {ga_fail(e,0x3c5e8);return GA_UNSUPPORTED;}
        SL(0,0x9a7c2);
        return release(a,e);
    }
    case 0x9a7c2: return release(a,e);
    case 0x9a5ee: return GA_DELETE;
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
bool neo_secret_routine(uint32_t r) {
    return r==0x9a67e || r==0x9a688 || r==0x9a692 || r==0x9a71e || r==0x9a6e8 || r==0x9a76a || r==0x9a7b2 || r==0x9a7c2 || r==0x9a5ee;
}
ga_result_t neo_secret_tick(neo_gactor_t *a,neo_genv_t *e) {return secret_tick(a,e);}
