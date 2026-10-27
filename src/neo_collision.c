#include "neo_collision.h"
#include "mission_collision.h"
#include <limits.h>

uint8_t neo_collision_cell(int x,int y) {
    if (x<0 || x>=NEO_GRID_COLUMNS*8 || y<0 || y>=256) return 0;
    return neo_grid[(x/8)*32+y/8];
}
static bool surface(int32_t x,int row,neo_surface_t *out) {
    if (x<0 || x>=NEO_GRID_COLUMNS*8*NEO_UNIT) return false;
    uint8_t code=neo_collision_cell(x/NEO_UNIT,row*8);
    unsigned shape=code&15;int32_t base=row*8*NEO_UNIT,u=(uint32_t)x&(8*NEO_UNIT-1),height;
    if (shape>=1 && shape<=4) height=base+(u/4)+(shape-1)*2*NEO_UNIT;
    else if (shape>=6 && shape<=9) height=base+8*NEO_UNIT-u/4-(shape-6)*2*NEO_UNIT;
    else if (shape==5) height=base+u;
    else if (shape==10) height=base+8*NEO_UNIT-u-1;
    else if (shape>=11 && shape<=13) height=base+8*NEO_UNIT;
    else return false;
    // Original slope contact is one fractional unit above the plane.
    if (shape<=9) height++;
    *out=(neo_surface_t){height,shape,code&0xf0};return true;
}
bool neo_surface_at(int32_t x,int32_t reference,int32_t rise,int32_t drop,neo_surface_t *out) {
    if (x<0 || x>=NEO_GRID_COLUMNS*8*NEO_UNIT) return false;
    bool found=false;int64_t closest=INT64_MAX;
    for (int row=0;row<32;row++) {
        neo_surface_t s;if (!surface(x,row,&s)) continue;
        int64_t distance=(int64_t)s.height-reference;
        if (distance>rise || distance< -(int64_t)drop) continue;
        int64_t magnitude=distance<0?-distance:distance;
        if (!found || magnitude<closest) { *out=s;closest=magnitude;found=true; }
    }
    return found;
}
void neo_ground_velocity(unsigned shape,int direction,int32_t *vx,int32_t *vy) {
    // Original angle coefficients: (sin(angle), -cos(angle)), scaled by 256.
    int horizontal=256,vertical=0;
    if (shape>=1 && shape<=4) {horizontal=248;vertical=62;}
    else if (shape>=6 && shape<=9) {horizontal=248;vertical=-62;}
    else if (shape==5) {horizontal=181;vertical=181;}
    else if (shape==10) {horizontal=181;vertical=-181;}
    *vx=direction*3*256*horizontal;*vy=direction*3*256*vertical;
}
bool neo_walk_step(neo_air_t *p,int direction) {
    neo_surface_t current,next;
    if (!neo_surface_at(p->x,p->y,1,NEO_UNIT,&current)) return false;
    neo_ground_velocity(current.shape,direction,&p->vx,&p->vy);
    p->x+=p->vx;
    int32_t predicted=p->y+p->vy;
    if (!neo_surface_at(p->x,predicted,4*NEO_UNIT,4*NEO_UNIT,&next)) {
        p->y=predicted;return false;
    }
    int32_t previous=p->y;p->y=next.height;p->vy=p->y-previous;return true;
}
bool neo_land_step(neo_air_t *p,int32_t previous_y) {
    if (p->vy>0 || p->y>previous_y) return false;
    bool hit=false;neo_surface_t highest={0};
    for (int row=0;row<32;row++) {
        neo_surface_t s;if (!surface(p->x,row,&s)) continue;
        if (s.height>previous_y || s.height<p->y) continue;
        if (!hit || s.height>highest.height) { highest=s;hit=true; }
    }
    if (hit) {p->y=highest.height;p->vy=p->y-previous_y;}
    return hit;
}
