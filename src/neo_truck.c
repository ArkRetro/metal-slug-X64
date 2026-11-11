#include "neo_region.h"
/* Rebel truck of the Mission 1 region (spawn-table entry ROM 02916E) and its
 * body child (ROM 029802), manual translation. ROM labels are provenance.
 * +84 bits: 0/1 from +6E (drop soldiers / variant), 2 moving, 3 target X is
 * relative to the scene, 4 floor search on death, 5 "background" truck (no
 * hurtbox, no sounds), 6 damaged animations, 7 drives left. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define SET(o,b) SB(o,B(o)|(1u<<(b)))
#define CLR(o,b) SB(o,B(o)&~(1u<<(b)))
#define BIT(o,b) ((B(o)>>(b))&1u)
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if (ga_anim_step(a,e,truck_call)<0) return GA_UNSUPPORTED; } while (0)
static int16_t S16(uint16_t v) {return (int16_t)v;}
static ga_result_t truck_tick(neo_gactor_t *a,neo_genv_t *e);
/* 029904: decode the spawn words. */
static void k29904(neo_gactor_t *a) {
    SB(0x84,0);
    uint16_t d0=W(0x6e);
    if (d0) {SET(0x84,0);if ((uint8_t)d0!=1u) SET(0x84,1);}
    if (!W(0x74)) SB(0x15,B(0x15)|1u);
    if (W(0x72)) SET(0x84,3);
    SW(0x7a,W(0x70));
    SW(0x6e,0);SW(0x70,0);SW(0x72,0);SW(0x74,0);SW(0x78,0);
    if (!W(0x76)) SW(0x76,0xc0);
}
/* 029A3A: target X (relative to the scene root when +84 bit 3) minus +3C. */
static int16_t k29a3a(neo_gactor_t *a,neo_genv_t *e) {
    uint16_t d0=W(0x7a);
    if (BIT(0x84,3)) {neo_contact_parent_t root;if (ga_parent(e,0x10e968u,&root)) d0=(uint16_t)(d0+(uint16_t)((uint32_t)root.x>>16));}
    return S16((uint16_t)(d0-W(0x3c)));
}
/* 029A56: carry when the truck reached its target X. */
static bool k29a56(neo_gactor_t *a,neo_genv_t *e) {
    int16_t d=k29a3a(a,e);
    return d>0?BIT(0x84,7)!=0:BIT(0x84,7)==0;
}
/* 0299E6: attach the cell map matching the state and facing. */
static void k299e6(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t map;
    if (BIT(0x84,5)) map=0x29d60;
    else if (BIT(0x84,2)) map=(B(0x15)&1u)?0x29c9a:0x29c58;
    else map=(B(0x15)&1u)?0x29d1e:0x29cdc;
    if (e->dyn_set) e->dyn_set(e->ctx,a,map,0,0);
}
/* 02997A / 0299A8: motion. */
static void k2997a(neo_gactor_t *a,neo_genv_t *e) {
    if (B(0x15)&64u) { /* 0146A8: ground step, restore on a non-flat contact */
        uint32_t x=L(0x28),y=L(0x2c),p=L(0x38),kind=0;
        ga_ground(a,e,&kind);
        if ((B(0x15)&64u) && (uint16_t)kind!=0x4000u) {SL(0x38,p);SL(0x2c,y);SL(0x28,x);SET(0x5f,0);ga_project(a,e);}
    } else {
        SL(0x34,L(0x34)+0xffffb000u);ga_air(a,e); /* 0143DC */
        if (B(0x15)&64u) {SL(0x30,0);SL(0x34,0);}
    }
}
static void k299a8(neo_gactor_t *a,neo_genv_t *e) {
    ga_air(a,e);
    if (B(0x15)&64u) SL(0x34,0);
    else {SL(0x34,L(0x34)+0xffffb000u);if (B(0x15)&64u) SL(0x34,0);}
    SL(0x30,0);
}
static void spawn_child(neo_gactor_t *a,neo_genv_t *e,uint32_t routine,int16_t dx,int16_t dy,uint16_t layer) {
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,routine,false):NULL;
    if (!c) return;
    ga_place_from(c,a,e,dx,dy);
    if (layer) {ga_sw(c,0x66,layer);ga_sw(c,0x1e,layer);}
}
/* 029AE4: smoke over the truck (01DA40 with the list at ROM 029BE2). */
static void k29ae4(neo_gactor_t *a,neo_genv_t *e) {
    if (e->sound) e->sound(e->ctx,0x1023);
    neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x1d764,false):NULL;
    if (!c) return;
    ga_place_from(c,a,e,0,0);
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,0x29be2);
    ga_sw(c,0x66,0x1001);ga_sw(c,0x1e,0x1001);
}
static bool truck_call(neo_gactor_t *a,neo_genv_t *e,neo_anim_kind_t kind,uint32_t routine,uint16_t parameter) {
    (void)parameter;
    if (kind!=NEO_ANIM_CALL) return false;
    switch (routine) {
    case 0x29ae4: k29ae4(a,e);return true;
    case 0x29a98: /* wreck: grid stamp, landing attack, solid body removed */
        SB(0x60,0xff);
        if (BIT(0x84,5) || !BIT(0x84,0)) return true;                  /* 029A9E / 029AA8: nothing else (the Mission 4 truck, +6E = 0) */
        if (e->stamp) e->stamp(e->ctx,a,BIT(0x84,1)?0x29c26:0x29bf4);
        ga_attack(a,e,0x29ba2);
        if (e->solid) e->solid(e->ctx,a,0);
        return true;
    case 0x29b14: /* explosion */
        if (e->sound) e->sound(e->ctx,0x1033);
        spawn_child(a,e,0x1c71e,0,0,0x1001);
        if (!BIT(0x84,5)) ga_attack(a,e,0x29bb2);
        return true;
    default:return false;
    }
}
/* 02A448: a piece: palettes 02A9BC[+83 & 3] and 201, program 02A878[rand & F], then 01D5AA. */
static ga_result_t piece_tick(neo_gactor_t *a,neo_genv_t *e) {
    const uint8_t *t=ga_rom(0x2a9bcu+2u*(B(0x83)&3u));
    if (!t) {ga_fail(e,0x2a9bc);return GA_UNSUPPORTED;}
    ga_palette(a,(uint16_t)(t[0]<<8|t[1]));ga_palette(a,0x201);SB(0x14,B(0x19));
    t=ga_rom(0x2a878u+4u*(ga_rand(e)&0xfu));
    if (!t) {ga_fail(e,0x2a878);return GA_UNSUPPORTED;}
    START((uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3]);
    return neo_effects_frag_1d5aa(a,e);
}
/* 02951A: damage registration, death check and visibility. */
static ga_result_t tail(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0x50)) {
        if (ga_register(a,e,NULL,NULL)) {if (B(0x5a)==5u) SL(0,0x294a2);SB(0x5a,0);}
        if (B(0x5f)&0x80u) SL(0,0x294a2);
    }
    if (ga_outside_view(a,e,0x29bca)) SL(0,0x295bc);
    return GA_MORE;
}
static void damaged(neo_gactor_t *a,neo_genv_t *e,uint32_t program) {
    if (!BIT(0x84,6) && S16(W(0x4e))<S16(W(0x7e))) {
        if (ga_anim_start(a,e,program,0x100)) {SET(0x84,6);k29ae4(a,e);}
    }
}
static ga_result_t truck_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (unsigned guard=0;guard<8;guard++) switch (L(0)) {
    case 0x2916e: /* constructor */
        k29904(a);SET(0x84,2);
        ga_floor(a,e);
        SW(0x66,0x8820);SW(0x1e,0x8820);
        SL(0x30,0);SL(0x34,0);SL(0x62,0);  /* 01B1DC */
        ga_air(a,e);
        ga_palette(a,0x22c);ga_flash_palette(a,0x306);
        SW(0x4e,ga_rank_word(a,e,ga_rom(0x2ce974)));SW(0x7e,W(0x4e)>>1);
        if (k29a3a(a,e)<=0) SET(0x84,7);
        { neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x29802,false):NULL;
          if (c) {ga_place_from(c,a,e,0,0);ga_sb(c,0x84,B(0x84));ga_sb(c,0x1e,B(0x1e));} }
        if (!BIT(0x84,5)) {
            if (e->sound) e->sound(e->ctx,0x10a5);
            if (e->solid) e->solid(e->ctx,a,0x29bc2);
            SL(0x50,0x29b62);
        } else SL(0x50,0);
        { neo_gactor_t *c=e->spawn?e->spawn(e->ctx,a,0x1d86c,false):NULL; /* 01DC8E */
          if (c) {ga_place_from(c,a,e,0,0);ga_sb(c,0x94,0);ga_sw(c,0x6e,0);ga_sw(c,0x70,0);ga_sw(c,0x72,0x3f);ga_sw(c,0x74,0x1f);} }
        { uint32_t v=(uint32_t)(int32_t)S16(W(0x76))<<8;if (BIT(0x84,7)) v=0u-v;SL(0x62,v); }
        if (!BIT(0x84,6)) START(0x236818); else {START(0x236846);SET(0x84,6);}
        SL(0,0x29294);continue;
    case 0x29294: /* drive */
        k2997a(a,e);
        if (B(0x5f)&1u) SL(0,0x29326);
        SW(0x1e,W(0x1e)-1u);
        STEP();
        if (!BIT(0x84,5)) ga_attack(a,e,0x29b92);
        damaged(a,e,0x236846);
        if (!ga_players_in(a,e,0x29bda)) CLR(0x84,2); /* 029A82: a player inside the rectangle */
        if (k29a56(a,e)) SL(0,0x29326);
        if (BIT(0x84,5) && e->flag448) SL(0,0x293dc);
        return tail(a,e);
    case 0x29326: /* stop */
        CLR(0x84,2);SL(0x30,0);SL(0x34,0);SL(0x62,0);
        if (!BIT(0x84,6)) START(0x2367e4); else {START(0x2367fe);SET(0x84,6);}
        if (e->sound) e->sound(e->ctx,0x10a5|0x8000u); /* 012192: stop request */
        SL(0,0x2936c);continue;
    case 0x2936c: /* idle */
        k299a8(a,e);SW(0x1e,W(0x1e)-1u);STEP();
        damaged(a,e,0x2367fe);
        if (BIT(0x84,5)) {
            if (e->flag448) SL(0,0x293dc);
            SW(0x82,W(0x82)-1u);
            if (S16(W(0x82))<=0) SL(0,0x293dc);
        }
        return tail(a,e);
    case 0x293dc: { /* leave */
        uint32_t v=(uint32_t)(int32_t)S16(W(0x76))<<8;if (!BIT(0x84,7)) v=0u-v;SL(0x62,v);
        if (!BIT(0x84,6)) START(0x236874); else {START(0x2368a2);SET(0x84,6);}
        if (!BIT(0x84,5) && e->sound) e->sound(e->ctx,0x10a5);
        SL(0,0x29438);continue;
    }
    case 0x29438:
        k2997a(a,e);
        if (B(0x5f)&1u) SL(0,0x294a2);
        SW(0x1e,W(0x1e)-1u);STEP();
        if (!BIT(0x84,5)) ga_attack(a,e,0x29b92);
        damaged(a,e,0x236846);
        if (!ga_players_in(a,e,0x29bda)) CLR(0x84,2); /* 029A82: a player inside the rectangle */
        return tail(a,e);
    case 0x294a2: /* destroyed */
        if (e->sound) e->sound(e->ctx,0x10a5|0x8000u);
        SB(0x69,0xff);
        if (BIT(0x84,4)) ga_floor(a,e);
        SL(0x30,0);SL(0x34,0);SL(0x62,0);
        SB(0x5f,B(0x5f)|0x80u);
        START(0x2368d0);SL(0,0x294e4); /* fall through */
        continue;
    case 0x294e4: {
        if (!BIT(0x84,4)) k299a8(a,e); else ga_project(a,e);
        int r=ga_anim_step(a,e,truck_call);
        if (r<0) return GA_UNSUPPORTED;
        if (r && !BIT(0x84,0)) SL(0,0x29566);
        if (ga_outside_view(a,e,0x29bca)) SL(0,0x295bc);
        return GA_MORE;
    }
    case 0x29566: /* debris */
        if (e->sound) e->sound(e->ctx,0x102f);
        spawn_child(a,e,0x1c71e,-32,0,0);spawn_child(a,e,0x1c71e,32,0,0);
        for (unsigned i=0;i<16;i++) { /* 02A71E */
            neo_gactor_t *c=e->spawn_reserve?e->spawn_reserve(e->ctx,a,0x2a448,0x2a):e->spawn?e->spawn(e->ctx,a,0x2a448,false):NULL;   /* 0040BC, D0 = 2A */
            if (c) {ga_place_from(c,a,e,0,0);ga_sb(c,0x83,B(0x83));}
        }
        for (unsigned i=0;i<16;i++) spawn_child(a,e,0x1d31c,0,24,0); /* 01DBAA */
        return GA_DELETE;
    case 0x295bc:return GA_DELETE;
    default:return GA_UNSUPPORTED;
    }
    return GA_UNSUPPORTED;
}
/* Body child 029802: carries the cell map and the damage hurtbox. */
static ga_result_t body_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x29802) {
        SL(0x50,BIT(0x84,5)?0:0x29b6e);
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (p) SW(0x4e,ga_w(p,0x4e));
        ga_place(a,e,0,0,p?NEO_GA_TAG(p->index):0);  /* 01AD6E */
        ga_palette(a,0x22c);ga_flash_palette(a,0x306);
        SL(0,0x29852);
    }
    if (L(0)==0x298f4) {if (e->dyn_release) e->dyn_release(e->ctx,a);return GA_DELETE_TREE;} /* 0298F4 -> 0041AC */
    if (L(0)!=0x29852) return GA_UNSUPPORTED;
    if (!e->creator_alive || !e->creator_alive(e->ctx,a)) {if (e->dyn_release) e->dyn_release(e->ctx,a);return GA_DELETE_TREE;}
    ga_project(a,e);
    neo_gactor_t *p=e->creator(e->ctx,a);
    SB(0x84,ga_b(p,0x84));
    if (BIT(0x84,5) && e->flag448) {if (e->dyn_release) e->dyn_release(e->ctx,a);}
    else k299e6(a,e);
    if (L(0x50)) {
        if (ga_register(a,e,NULL,NULL)) {
            SB(0x5a,0);ga_sb(p,0x17,B(0x17));
            if (S16(ga_w(p,0x4e))>S16(W(0x4e))) ga_sw(p,0x4e,W(0x4e)); else SW(0x4e,ga_w(p,0x4e));
            if (B(0x5f)&0x80u) ga_sb(p,0x5f,(uint8_t)(ga_b(p,0x5f)|0x80u));
        }
        if (B(0x5f)&0x80u) SL(0,0x298f4);
    }
    return GA_MORE;
}
ga_result_t neo_truck_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x2a448) return piece_tick(a,e);
    uint32_t r=L(0);
    if (r==0x29802 || r==0x29852 || r==0x298f4) return body_tick(a,e);
    return truck_tick(a,e);
}
bool neo_truck_routine(uint32_t r) {return r==0x2916e || r==0x2a448 || (r>=0x29294 && r<=0x295bc) || r==0x29802 || r==0x29852 || r==0x298f4;}
