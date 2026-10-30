#include "neo_region.h"
/* Scene props of the region, manual translation (ROM labels as provenance):
 * stair/ledge markers 0403A0/040396 registering into the object list that
 * the soldiers follow (040110, queried by 040200/0402E6). */
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
bool neo_props_routine(uint32_t r) {return r==0x403a0 || r==0x40396 || r==0x403c0;}
ga_result_t neo_props_tick(neo_gactor_t *a,neo_genv_t *e) {return marker(a,e);}
