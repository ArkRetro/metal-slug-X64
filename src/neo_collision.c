#include "neo_collision.h"
#include "mission_collision.h"
#include <limits.h>
#include <string.h>
#include "vehicle_wreckage.h"
static uint8_t dynamic_grid[sizeof(neo_grid)];
static bool dynamic_active;
void neo_collision_reset(void) {dynamic_active=false;}
static int cell_floor(int32_t value) {
    int64_t v=value;return v>=0?(int)(v/(8*NEO_UNIT)):-(int)((-v+8*NEO_UNIT-1)/(8*NEO_UNIT));
}
unsigned neo_collision_vehicle_wreckage(int32_t x,int32_t y,uint8_t flags) {
    if (!(flags&1) || flags&32) return 0;
    if (!dynamic_active) {memcpy(dynamic_grid,neo_grid,sizeof(neo_grid));dynamic_active=true;}
    int base_x=cell_floor(x)-7,base_y=cell_floor(y);unsigned changed=0;
    for (unsigned col=0;col<13;col++) for (unsigned row=0;row<3;row++) {
        int cx=base_x+(int)col,cy=base_y+2-(int)row;
        if (cx<0 || cx>=NEO_GRID_COLUMNS || cy<0 || cy>=32) continue;
        uint8_t *cell=&dynamic_grid[cx*32+cy];
        if (*cell&15) continue;
        uint8_t value=(*cell&0xe0)|(flags&2?16:0)|vehicle_wreckage[row][col];
        if (value!=*cell) { *cell=value;changed++; }
    }
    return changed;
}

unsigned neo_collision_stamp(int32_t x,int32_t y,const uint8_t *t) {
    /* ROM 0C88B0 (tools/collision_stamp.py): template dx,dy,w,h,material,
     * -,mask.l, w*h shape nibbles (top row first) at the actor's integer
     * cell; an existing shape s survives unless mask bit s is set; bits
     * 5..7 of the old cell are kept. The ring window of the layer always
     * covers the stamps of Mission 1 (actors on screen). */
    if (!dynamic_active) {memcpy(dynamic_grid,neo_grid,sizeof(neo_grid));dynamic_active=true;}
    int dx=(int8_t)t[0],dy=(int8_t)t[1],w=t[2],h=t[3];
    uint32_t mask=(uint32_t)t[6]<<24|(uint32_t)t[7]<<16|(uint32_t)t[8]<<8|t[9];
    int base_x=cell_floor(x)+dx,base_y=cell_floor(y)+dy+h-1;unsigned changed=0;
    for (int col=0;col<w;col++) for (int row=0;row<h;row++) {
        int cx=base_x+col,cy=base_y-row;
        if (cx<0 || cx>=NEO_GRID_COLUMNS || cy<0 || cy>=32) continue;
        uint8_t *cell=&dynamic_grid[cx*32+cy];unsigned shape=*cell&15u;
        if (shape && !((mask>>shape)&1u)) continue;
        uint8_t value=(uint8_t)((t[10+row*w+col]|(t[4]?16:0))+(*cell&0xe0));
        if (value!=*cell) {*cell=value;changed++;}
    }
    return changed;
}
unsigned neo_collision_columns(void) {return NEO_GRID_COLUMNS;}
uint8_t neo_collision_cell(int x,int y) {
    if (x<0 || x>=NEO_GRID_COLUMNS*8 || y<0 || y>=256) return 0;
    return (dynamic_active?dynamic_grid:neo_grid)[(x/8)*32+y/8];
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
    // Rising planes keep the one-unit contact bias. Declining quarter-slopes
    // already include it: ROM 012DEE uses 0x7FFFF before the contact +1.
    if (shape<=5) height++;
    *out=(neo_surface_t){height,shape,code&0xf0};return true;
}
/* The player's sweeps (0143EE air, 014594 ground) run 0128E8, which sets
 * RAM 10C00D: the kernel then clears cells of group 1 (0129A2..0129BC; the
 * Mission 1 group map keeps 1 as 1), i.e. the codes 31..3B of the upper
 * roofs. MAME roof1 capture (docs/MISION1_COMPLETA.md, "Tejados"): the
 * player dropped at y 202 over x 1731..1736 falls through the 3B cells at
 * y 192 and lands on the FB roof at y 144. Soldiers and other actors keep
 * them (neo_surface_unique). */
static bool player_surface(int32_t x,int row,neo_surface_t *out) {
    if (x<0 || x>=NEO_GRID_COLUMNS*8*NEO_UNIT) return false;
    if ((neo_collision_cell(x/NEO_UNIT,row*8)>>5)==1u) return false;
    return surface(x,row,out);
}
bool neo_surface_unique(int32_t x,neo_surface_t *out) {
    bool found=false;
    for (int row=0;row<32;row++) {
        neo_surface_t candidate;if (!surface(x,row,&candidate)) continue;
        if (found) return false;
        *out=candidate;found=true;
    }
    return found;
}
bool neo_surface_at(int32_t x,int32_t reference,int32_t rise,int32_t drop,neo_surface_t *out) {
    if (x<0 || x>=NEO_GRID_COLUMNS*8*NEO_UNIT) return false;
    bool found=false;int64_t closest=INT64_MAX;
    for (int row=0;row<32;row++) {
        neo_surface_t s;if (!player_surface(x,row,&s)) continue;
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
    bool found=false;
    // A new ramp can overlap an older flat surface. Select the upper reachable
    // contact within the original step tolerance, so the old floor cannot pull
    // feet through the wreck. Distant roofs remain outside that tolerance.
    for (int row=0;row<32;row++) {
        neo_surface_t candidate;
        if (!player_surface(p->x,row,&candidate)) continue;
        int64_t delta=(int64_t)candidate.height-predicted;
        if (delta>4*NEO_UNIT || delta< -4*NEO_UNIT) continue;
        if (!found || candidate.height>next.height) {next=candidate;found=true;}
    }
    if (!found) {
        p->y=predicted;return false;
    }
    int32_t previous=p->y;p->y=next.height;p->vy=p->y-previous;return true;
}
bool neo_land_step(neo_air_t *p,int32_t previous_y) {
    if (p->vy>0 || p->y>previous_y) return false;
    bool hit=false;neo_surface_t highest={0};
    for (int row=0;row<32;row++) {
        neo_surface_t s;if (!player_surface(p->x,row,&s)) continue;
        if (s.height>previous_y || s.height<p->y) continue;
        if (!hit || s.height>highest.height) { highest=s;hit=true; }
    }
    if (hit) {p->y=highest.height;p->vy=p->y-previous_y;}
    return hit;
}
