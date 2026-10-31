#include "neo_region.h"
/* Scene props of the region, manual translation (ROM labels as provenance):
 * stair/ledge markers 0403A0/040396 registering into the object list that
 * the soldiers follow (040110, queried by 040200/0402E6), and the market
 * stall's awning 0400AA (camera 192, script record 2D6364, group 9 not
 * counted): floor, palette 0x13B, program 2402CA, collision stamp 0E0932
 * (0C88B0; the port's static grid already holds its cells: the stamp writes
 * the same values), depth 01AD8C, cull 0E08B8 (03FADA -> 0041AC).
 * Explicit stop (stays external): the stall 03FC5A, whose fruit boxes
 * 03FD6E ask 01AF1A for the player's ground flag (+15 bit 6), a player
 * field the actors' environment does not carry. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define L(o) ga_l(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SL(o,v) ga_sl(a,(o),(uint32_t)(v))
/* 04051C -> 040110: entry at the actor's scene-root-relative position. */
static void register_marker(neo_gactor_t *a,neo_genv_t *e) {
    neo_contact_parent_t p,root;
    uint32_t x=L(0x28),y=L(0x2c);
    if (L(0x38) && ga_parent(e,L(0x38),&p)) {x+=(uint32_t)p.x;y+=(uint32_t)p.y;}
    if (!ga_parent(e,0x10e968u,&root)) return;
    if (e->marker) e->marker(e->ctx,(int32_t)(x-(uint32_t)root.x),(int32_t)(y-(uint32_t)root.y),(uint8_t)W(0x6e),B(0x70));
}
static ga_result_t marker(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x403a0 || L(0)==0x40396) {
        SB(0x71,L(0)==0x40396?0xff:0);
        SB(0x69,0xff);                               /* 019DFC */
        if (e->marker_seq) {(*e->marker_seq)++;SB(0x70,*e->marker_seq);}  /* RAM 10C446 */
        SL(0,0x403c0);
    }
    ga_project(a,e);
    if (B(0x71)) {
        if (!ga_creator_alive(a,e)) return GA_DELETE;
        neo_gactor_t *p=e->creator?e->creator(e->ctx,a):NULL;
        if (p && (ga_b(p,0x5f)&0x80u)) return GA_DELETE;
    } else if ((uint16_t)(W(0x3c)+0x30u)>=0x1a0u) return GA_DELETE;
    register_marker(a,e);
    return GA_MORE;
}
static ga_result_t awning(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x400aa) {
        ga_floor(a,e);SB(0x1d,0);                                       /* 009F88 */
        ga_palette(a,0x13b);ga_sw(a,0x66,0);ga_sw(a,0x1e,0);
        if (!ga_anim_start(a,e,0x2402ca,0x100)) return GA_UNSUPPORTED;
        if (e->stamp) e->stamp(e->ctx,a,0xe0932);
        SL(0,0x400f2);
    }
    ga_project(a,e);
    if ((ga_w(a,0x66)&0xf000u)==0x8000u) ga_sw(a,0x1e,ga_w(a,0x66)-ga_w(a,0x40)); /* 01AD8C */
    if (ga_anim_step(a,e,NULL)<0) return GA_UNSUPPORTED;
    if (ga_cull(a,e,0xe08b8)) return GA_DELETE_TREE;                   /* 03FADA */
    return e->unsupported?GA_UNSUPPORTED:GA_MORE;
}
bool neo_props_routine(uint32_t r) {return r==0x403a0 || r==0x40396 || r==0x403c0 || r==0x400aa || r==0x400f2;}
ga_result_t neo_props_tick(neo_gactor_t *a,neo_genv_t *e) {
    if (L(0)==0x400aa || L(0)==0x400f2) return awning(a,e);
    return marker(a,e);
}
