#include "neo_region.h"
/* Destructible building of Mission 1 (barrel shop, camera ~2240), manual
 * translation; ROM labels are provenance only.
 *
 * 0B6566 (spawn table) is the building: health from the rank table 2D3A5C,
 * three damage stages (+79, programs 2A89B8/2A89C4/2A89D0, debris 0B69B6,
 * six 01D488 puffs) as its health halves, and the destruction 0B66F2:
 * kill-all (004BFE), six flying pieces 08732C (0B68B4), sixteen puffs, the
 * map patch 0B6882 (sub-map 0B6AD2 on layers 10E8C6/10D7B6, column 0x9F row
 * 0x0B) and the ruin program 2A89DC. Two children follow it: the overlay
 * 0B6770 (programs 2A89E8/2A89F4/2A8A00 by stage) and the roof platform
 * 0B680E: collision stamps 0B69F8/0B6A2C (0C88B0) and the dynamic object
 * 0B6A60 (0140C0) one pixel lower than the node, cleared (0B6A12/0B6A42,
 * 013F1E) with the building. 0B6202 is the delete through the cartridge
 * check (004188 on genuine hardware).
 *
 * 0B6298 (village decoration, camera 336, script record 2D63B8, group 9 not
 * counted): palette 0x442, program 2A89AC, depth 01AD8C, cull 0B6948 ->
 * 0B6202. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
#define START(t) do { if (!ga_anim_start(a,e,(t),0x100)) return GA_UNSUPPORTED; } while (0)
#define STEP() do { if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED; } while (0)
static neo_gactor_t *parent(neo_gactor_t *a,neo_genv_t *e) {return e->creator?e->creator(e->ctx,a):NULL;}
/* 01AD8C: +1E follows +66 - y when +66 is 8xxx. */
static void depth(neo_gactor_t *a) {
    uint16_t d0=W(0x66);
    if ((d0&0xf000u)==0x8000u) SW(0x1e,d0-W(0x40));
}
/* 01DA40: debris emitter 01D764 with the list `list`, +66/+1E = 8800. */
static void debris(neo_gactor_t *a,neo_genv_t *e,uint32_t list) {
    neo_gactor_t *c=ga_spawn_from(a,e,0x1d764,0,0);
    if (!c) return;
    if (B(0x15)&4u) ga_sb(c,0x15,(uint8_t)(ga_b(c,0x15)|4u));
    ga_sl(c,0x86,list);ga_sw(c,0x66,0x8800);ga_sw(c,0x1e,0x8800);
}
/* 01DC22: `n` smoke puffs 01D488 at (0,64). */
static void puffs(neo_gactor_t *a,neo_genv_t *e,unsigned n) {for (unsigned i=0;i<n;i++) ga_spawn_from(a,e,0x1d488,0,0x40);}
static uint16_t rank(neo_gactor_t *a,neo_genv_t *e,uint32_t table) {
    const uint8_t *t=ga_rom(table);
    if (!t) {ga_fail(e,table);return 0;}
    return ga_rank_word(a,e,t);
}
/* 0B68B4: six pieces 08732C inserted after the player node (00411C) with a
 * random offset and velocity; the RNG is drawn whether or not the node was
 * allocated. The pieces are an unported family (external). */
static void pieces(neo_gactor_t *a,neo_genv_t *e) {
    if (e->sound) e->sound(e->ctx,0x1142);
    for (unsigned i=0;i<6;i++) {
        neo_gactor_t *c=e->spawn?e->spawn(e->ctx,NULL,0x8732c,true):NULL;
        uint16_t r=ga_rand(e);
        int16_t dx=(int16_t)((r&0x7fu)-0x40u),dy=(int16_t)(((r>>8)&0x3fu)+0x30u);
        if (c) ga_place_at(c,a,e,dx,dy);
        uint8_t angle=(uint8_t)((ga_rand(e)&15u)+0x68u);
        uint32_t speed=(uint32_t)((int32_t)(int16_t)((ga_rand(e)&0x3ffu)+0xa00u)<<8),vx,vy;
        ga_polar(speed,angle,&vx,&vy);
        if (c) {ga_sl(c,0x30,vx);ga_sl(c,0x34,vy);}
    }
}
static ga_result_t building_tick(neo_gactor_t *a,neo_genv_t *e) {
    for (;;) switch (L(0)) {
    case 0xb6566: {
        uint16_t h=rank(a,e,0x2d3a5c);SW(0x4e,h);SW(0x76,h>>1);
        SL(0x50,0xb6986);SB(0x79,0);ga_palette(a,0x46e);
        SW(0x66,0x87e8);SW(0x1e,0x87e8);depth(a);START(0x2a89b8);
        ga_spawn_from(a,e,0xb6770,0,0);ga_spawn_from(a,e,0xb680e,0,0x80);
        SL(0,0xb6606);
    }   /* fall through */
    case 0xb6606:
        ga_project(a,e);depth(a);STEP();
        if (ga_register(a,e,NULL,NULL)) {
            if (B(0x5a)==0x0cu) debris(a,e,0xb69da);
            SB(0x5a,0);
            uint16_t d0=W(0x76);
            if ((int16_t)d0>(int16_t)W(0x4e)) {
                SW(0x76,d0>>1);
                uint8_t s=(uint8_t)(B(0x79)+1u);if (s>=3u) s=2;
                if (s!=B(0x79)) {
                    SB(0x79,s);START(s==1u?0x2a89c4:0x2a89d0);
                    debris(a,e,0xb69b6);puffs(a,e,6);
                    if (e->sound) e->sound(e->ctx,0x1035);
                }
            }
        }
        if (B(0x5f)&0x80u) SL(0,0xb66f2);
        if (ga_cull(a,e,0xb6970)) SL(0,0xb6736);
        return GA_MORE;
    case 0xb66f2:
        if (e->sound) e->sound(e->ctx,0x1035);
        if (e->flags) e->flags->kill_request=0xff;                  /* 004BFE */
        pieces(a,e);puffs(a,e,16);debris(a,e,0xb69c8);
        if (e->flags) e->flags->barrel_patched=1;                   /* 0B6882 */
        START(0x2a89dc);
        /* fall through */
    case 0xb6736:
        SB(0x5f,B(0x5f)|0x80u);SB(0x69,0xff);SL(0,0xb6748);           /* 019DFC */
        /* fall through */
    case 0xb6748:
        ga_project(a,e);depth(a);STEP();
        if (ga_cull(a,e,0xb6970)) SL(0,0xb6202);
        return GA_MORE;
    default: ga_fail(e,L(0));return GA_UNSUPPORTED;
    }
}
static ga_result_t overlay_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xb6770) {
        ga_palette(a,0x46e);SW(0x66,0x87e8);SW(0x1e,0x87e8);depth(a);START(0x2a89e8);SL(0,0xb67a6);
    }
    ga_project(a,e);depth(a);STEP();
    neo_gactor_t *p=parent(a,e);
    if (!p) {ga_fail(e,0xb67b8);return GA_UNSUPPORTED;}
    if (ga_b(p,0x79)!=B(0x79)) {SB(0x79,ga_b(p,0x79));START(B(0x79)==1u?0x2a89f4:0x2a8a00);}
    if (ga_b(p,0x5f)&0x80u) SL(0,0xb6202);
    return GA_MORE;
}
static ga_result_t roof_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xb680e) {
        ga_floor(a,e);ga_stationary(a,e);
        if (e->stamp) {e->stamp(e->ctx,a,0xb69f8);e->stamp(e->ctx,a,0xb6a2c);}
        SL(0,0xb6834);
    }
    SW(0x2c,W(0x2c)+1u);ga_project(a,e);
    if (e->dyn_set) e->dyn_set(e->ctx,a,0xb6a60,0,0);
    SW(0x2c,W(0x2c)-1u);
    neo_gactor_t *p=parent(a,e);
    if (!p) {ga_fail(e,0xb684c);return GA_UNSUPPORTED;}
    if (ga_b(p,0x5f)&0x80u) {
        ga_project(a,e);
        if (e->stamp) {e->stamp(e->ctx,a,0xb6a12);e->stamp(e->ctx,a,0xb6a42);}
        if (e->dyn_release) e->dyn_release(e->ctx,a);
        SL(0,0xb6202);
    }
    return GA_MORE;
}
static ga_result_t decor_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0xb6298) {
        ga_palette(a,0x442);SW(0x66,0);SW(0x1e,0);START(0x2a89ac);SL(0,0xb62c8);
    }
    depth(a);ga_project(a,e);STEP();
    if (ga_cull(a,e,0xb6948)) SL(0,0xb6202);
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
bool neo_barrel_routine(uint32_t r) {
    return r==0xb6298 || r==0xb62c8 || r==0xb6566 || r==0xb6606 || r==0xb66f2 || r==0xb6736 || r==0xb6748 || r==0xb6202 ||
        r==0xb6770 || r==0xb67a6 || r==0xb680e || r==0xb6834;
}
ga_result_t neo_barrel_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=L(0);
    if (r==0xb6770 || r==0xb67a6) return overlay_tick(a,e);
    if (r==0xb680e || r==0xb6834) return roof_tick(a,e);
    if (r==0xb6202) {SB(0x5f,B(0x5f)|0x80u);return GA_DELETE;}       /* 0B6202 -> 004188 */
    if (r==0xb6298 || r==0xb62c8) return decor_tick(a,e);
    return building_tick(a,e);
}
