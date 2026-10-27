#include "neo_target.h"
static int s16(uint16_t v) {return (int)(v&32767u)-(v&32768u?32768:0);}
static uint16_t abs16(uint16_t v) {return v&32768u?(uint16_t)(0u-v):v;}
static bool inside(const neo_contact_actor_t *a,uint16_t x,uint16_t y,const int16_t r[4]) {
    uint16_t dx=(uint16_t)(x-((uint32_t)a->projected_x>>16)),dy=(uint16_t)(y-((uint32_t)a->projected_y>>16));
    if (a->flags15&1u) dx=(uint16_t)(0u-dx);
    if (a->flags15&2u) dy=(uint16_t)(0u-dy);
    return s16(dx)>=r[0] && s16(dx)<=s16((uint16_t)(r[0]+r[2])) &&
        s16(dy)>=r[1] && s16(dy)<=s16((uint16_t)(r[1]+r[3]));
}
neo_target_t neo_target_pick(const neo_contact_actor_t *a,const neo_control_frame_t *f,const int16_t r[4]) {
    unsigned mask=f->active_mask&3u;
    if ((mask&1u) && !inside(a,f->p1x,f->p1y,r)) mask^=1u;
    if ((mask&2u) && !inside(a,f->p2x,f->p2y,r)) mask^=2u;
    unsigned player=mask==2u;
    if (mask==3u) {
        uint16_t x=(uint16_t)((uint32_t)a->projected_x>>16);
        player=s16(abs16((uint16_t)(f->p2x-x)))<s16(abs16((uint16_t)(f->p1x-x)));
    }
    return (neo_target_t){mask!=0,(uint8_t)player,player?f->p2x:f->p1x,player?f->p2y:f->p1y};
}
