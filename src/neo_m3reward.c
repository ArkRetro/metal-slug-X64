#include "neo_region.h"

/* Platform jump award, original P1 0B9748..0B99CA. 0B9286 creates this
 * invisible child before the moving platform. ROM labels are provenance;
 * only native scene/player services are called. 0B97D8..0B985E is the same
 * cartridge protection tail omitted by other families: genuine hardware
 * falls through to 004188; native deletion is explicit here. */
#define B(o) ga_b(a,(o))
#define W(o) ga_w(a,(o))
#define SB(o,v) ga_sb(a,(o),(uint8_t)(v))
#define SW(o,v) ga_sw(a,(o),(uint16_t)(v))

/* 08CECC, restricted to its callers here (player0/1, D1=1): add the BCD
 * value (057948), sound1121, a score label after the script (00411C), then
 * 01ACBE places it in screen coordinates with no parent, integer precision.
 * Even the ordinary zero-value request creates a label and plays the sound.
 * Allocation failure follows the existing native score-label owner policy. */
static bool award(neo_gactor_t *a,neo_genv_t *e,unsigned player,uint32_t value) {
    if (!e->pickup || !e->spawn || !e->sound) {ga_fail(e,0x8cecc);return false;}
    neo_item_add_score(e->pickup->score[player],value);
    e->sound(e->ctx,0x1121);
    neo_gactor_t *label=e->spawn(e->ctx,NULL,0x8d330,true);
    if (label) {
        ga_sl(label,0x6e,value);ga_sb(label,0x8a,(uint8_t)player);
        ga_place(label,e,(int16_t)W(0x3c),(int16_t)W(0x40),0);
    }
    e->pickup->popups++;
    return e->unsupported==0;
}

/* 01AF3C and inline B99D8: signed word subtraction, then mirrored inclusive
 * rectangle [-28,4] x [-4,4]. The upper edge is BGT, not BGE. */
static bool in_rectangle(neo_gactor_t *a,uint16_t px,uint16_t py) {
    uint16_t dx=(uint16_t)(px-W(0x3c)),dy=(uint16_t)(py-W(0x40));
    if (B(0x15)&1u) dx=(uint16_t)(0u-dx);
    if (B(0x15)&2u) dy=(uint16_t)(0u-dy);
    return (int16_t)dx>=-28 && (int16_t)dx<=4 && (int16_t)dy>=-4 && (int16_t)dy<=4;
}

ga_result_t neo_m3reward_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t routine=ga_l(a,0);
    if (routine!=0xb9748 && routine!=0xb9780) {ga_fail(e,routine);return GA_UNSUPPORTED;}
    if (!e->live_players_valid) {ga_fail(e,0x1aeb0);return GA_UNSUPPORTED;}
    if (routine==0xb9748) {
        neo_gactor_t *owner=e->creator?e->creator(e->ctx,a):NULL;
        if (!owner) {ga_fail(e,0x1ad6e);return GA_UNSUPPORTED;}
        SB(0x15,B(0x15)&~5u);SW(0x66,0xffff);SW(0x1e,0xffff);
        SB(0x78,0);SB(0x79,0);SB(0x7a,0);
        ga_place(a,e,-80,40,NEO_GA_TAG(owner->index));
        ga_sl(a,0,0xb9780);
    }
    ga_project(a,e);                                             /* 01ABC8 */
    neo_contact_parent_t root;
    if (!ga_parent(e,0x10e968,&root)) return GA_UNSUPPORTED;
    uint16_t limit=(uint16_t)(0xf40u+((uint32_t)root.x>>16));       /* 01AD7C */
    SB(0x7b,(int16_t)limit<=(int16_t)W(0x3c)?1:0);
    /* B9868: arm both players before checking either landing. Inactive or
     * consumed players clear the latch; air/outside-band leaves it intact. */
    for (unsigned p=0;p<2;p++) {
        SW(0x7c,p);
        if (B(0x78+p) || !(e->players.active_mask&(1u<<p))) SB(0x76+p,0);
        else if (e->live_player[p].flags15&64u) {
            int16_t dx=(int16_t)(uint16_t)(e->live_player[p].x-W(0x3c));
            int16_t dy=(int16_t)(uint16_t)(e->live_player[p].y-W(0x40));
            if (dx< -36 && dy> -8) SB(0x76+p,1);
        }
    }
    for (unsigned p=0;p<2;p++) {                                  /* B98F8 */
        if (B(0x7a)) continue;
        SW(0x7c,p);
        if (B(0x78+p) || !(e->players.active_mask&(1u<<p)) ||
            !(e->live_player[p].flags15&64u)) continue;
        int16_t dx=(int16_t)(uint16_t)(e->live_player[p].x-W(0x3c));
        bool special=false;
        if (dx<=4) {
            if (!in_rectangle(a,e->live_player[p].x,e->live_player[p].y)) continue;
            special=B(0x76+p) && B(0x7b);
        }
        if (special) SB(0x7a,1);
        SB(0x78+p,1);
        /* B99AC: only the high words move, preserving fractional position. */
        SW(0x28,W(0x28)-16u);SW(0x3c,W(0x3c)-16u);
        bool ok=award(a,e,p,special?0x00050000u:0);               /* B99E0/E4 */
        SW(0x28,W(0x28)+16u);SW(0x3c,W(0x3c)+16u);
        if (!ok) return GA_UNSUPPORTED;
    }
    if (e->unsupported) return GA_UNSUPPORTED;
    if ((int16_t)W(0x3c)<0 || B(0x7a)) return GA_DELETE;
    if (!e->creator_alive) {ga_fail(e,0x409a);return GA_UNSUPPORTED;}
    return ga_creator_alive(a,e)?GA_MORE:GA_DELETE;
}
