#include "neo_cull.h"
static bool negative(uint32_t v) {return (v&0x80000000u)!=0;}
static bool less_word(uint16_t a,uint16_t b) {return ((uint16_t)(a-b)&32768u)!=0;}
bool neo_cull_outside(const neo_contact_actor_t *a,const neo_cull_bounds_t *b,const uint16_t rect[4]) {
    uint32_t x=(uint32_t)a->projected_x,y=(uint32_t)a->projected_y;
    uint32_t oldx=(uint32_t)a->cached_x,oldy=(uint32_t)a->cached_y;
    if (b->has_parent) {x-=(uint32_t)b->x;y-=(uint32_t)b->y;oldx-=(uint32_t)b->previous_x;oldy-=(uint32_t)b->previous_y;}
    uint32_t dx=oldx-x,dy=oldy-y;
    uint16_t left=rect[0],top=rect[1],right=(uint16_t)(left+rect[2]),bottom=(uint16_t)(top+rect[3]);
    if (a->flags15&1u) {uint16_t t=left;left=(uint16_t)(0u-right);right=(uint16_t)(0u-t);}
    if (dx) {
        if (!negative(dx) && less_word((uint16_t)((x>>16)+right),b->left)) return true;
        if (negative(dx) && !less_word((uint16_t)((x>>16)+left),b->right)) return true;
    }
    if (a->flags15&2u) {uint16_t t=top;top=(uint16_t)(0u-bottom);bottom=(uint16_t)(0u-t);}
    if (dy) {
        if (!negative(dy) && less_word((uint16_t)((y>>16)+bottom),b->top)) return true;
        if (negative(dy) && !less_word((uint16_t)((y>>16)+top),b->bottom)) return true;
    }
    return false;
}
