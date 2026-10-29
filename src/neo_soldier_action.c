#include "neo_soldier_action.h"
#define NEO_STATE_RECT_TABLE
#include "state_tables.h"
static int s16(uint16_t value) {return (int)(value&32767u)-(value&32768u?32768:0);}
static bool player_inside(const neo_contact_actor_t *a,uint16_t px,uint16_t py) {
    uint16_t dx=(uint16_t)(px-((uint32_t)a->projected_x>>16));
    uint16_t dy=(uint16_t)(py-((uint32_t)a->projected_y>>16));
    if (a->flags15&1u) dx=(uint16_t)(0u-dx);
    if (a->flags15&2u) dy=(uint16_t)(0u-dy);
    return s16(dx)>=state_action_rect[0] && s16(dx)<=state_action_rect[0]+state_action_rect[2] &&
        s16(dy)>=state_action_rect[1] && s16(dy)<=state_action_rect[1]+state_action_rect[3];
}
uint8_t neo_soldier_action_choose(neo_control_t *c,const neo_contact_actor_t *a,
    const neo_control_frame_t *scene,uint8_t mask,neo_rng_t *rng) {
    int x=s16((uint16_t)((uint32_t)a->projected_x>>16));
    if (x<24 || x>296 || !(c->flags7c&8u) || (c->selected80&128u) || c->timer70) return 0;
    bool nearby=((scene->active_mask&1u) && player_inside(a,scene->p1x,scene->p1y)) ||
        ((scene->active_mask&2u) && player_inside(a,scene->p2x,scene->p2y));
    if (nearby) return 0; // Original 01AFFC returns carry clear for any survivor.
    uint8_t filtered=mask;
    if (!(a->flags15&64u)) filtered&=2u;
    if ((c->flags7c&64u) && (filtered&2u)) {
        filtered&=253u;
        if ((c->flags7c&2u) && c->delay92<255u) {
            c->delay92=255;
            c->target8e=(uint16_t)(((uint32_t)a->projected_x>>16)-scene->origin_x);
            c->target90=(uint16_t)(((uint32_t)a->projected_y>>16)-scene->origin_y);
        }
    }
    unsigned count=0;for (unsigned i=0;i<8;i++) count+=(filtered>>i)&1u;
    if (!count) return 0;
    unsigned selected=((uint32_t)neo_rng_next(rng)*count)>>16;
    // AEEB4 reloads ORIGINAL +81 after counting the filtered byte. Preserve
    // that ordering rather than silently changing which mask is enumerated.
    for (int bit=7;bit>=0;bit--) {
        if (mask&(1u<<bit)) {if (!selected) return (uint8_t)bit;selected--;}
    }
    return 0;
}
