#include "neo_hit.h"
static int signed16(uint16_t v) {return (int)(v&32767u)-(v&32768u?32768:0);}
static uint16_t half_signed(uint16_t v) {return (uint16_t)((v>>1)|(v&0x8000u));}
static uint16_t impact(uint16_t sum,uint16_t distance,uint16_t dimension,uint16_t origin,bool flip) {
    uint16_t value=(uint16_t)(half_signed(sum)-distance),half=(uint16_t)(dimension>>1);
    value=(uint16_t)(value+half);
    if (value&0x8000u) value=0;
    value=(uint16_t)(value-half);
    if (signed16(value)>signed16(half)) value=half;
    return (uint16_t)(value+(flip?(uint16_t)(0u-origin):origin));
}
bool neo_hit_rectangle(const neo_hit_rectangle_t *q,uint16_t x,uint16_t y,
    uint8_t flags,const neo_hit_rectangle_t *r,uint16_t *ix,uint16_t *iy) {
    uint16_t right=r->x,top=r->y;
    if (flags&1u) right=(uint16_t)(0u-(uint16_t)(right+r->width));
    right=(uint16_t)(right+r->width+x-q->x);
    uint16_t width=(uint16_t)(r->width+q->width);
    if (right>=width) return false;
    if (flags&2u) top=(uint16_t)(0u-(uint16_t)(top+r->height));
    top=(uint16_t)(top+r->height+y-q->y);
    uint16_t height=(uint16_t)(r->height+q->height);
    if (top>=height) return false;
    uint16_t local_y=impact(height,top,r->height,r->y,(flags&2u)!=0);
    uint16_t local_x=impact(width,right,r->width,r->x,(flags&1u)!=0);
    if (ix) *ix=local_x;
    if (iy) *iy=local_y;
    return true;
}
