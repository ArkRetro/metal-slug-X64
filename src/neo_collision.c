#include "neo_collision.h"
#include "mission_collision.h"
#include <limits.h>
#include <string.h>
#include "vehicle_wreckage.h"
static uint8_t dynamic_grid_m1[sizeof(neo_grid)];
static bool dynamic_active;
/* Active grid (column major, `rows` cells per column, row 0 at the bottom):
 * Mission 1's compiled neo_grid, or a stage of a mission pack
 * (neo_collision_set_grid, docs/MISION2.md "Fase 2"). */
static const uint8_t *grid=neo_grid;static uint8_t *dynamic_grid=dynamic_grid_m1;static const uint8_t *unknown;
static unsigned columns=NEO_GRID_COLUMNS,rows=32,unknown_reads;
/* Kernel row of the grid's row 0 (neo_collision_set_row_origin): 0, or the
 * negative row of a map that goes below the kernel's y 0 (Mission 4: the
 * map 0E6DA8 with +34 = 7 is 36 blocks tall, rows 15 down to -56). */
static int row0;
/* The terrain layer's 64x64 cell ring (object 10C72E +88) of the vertical
 * stages: what the kernel's 0C79FE reads (neo_kscene_t.ring). */
static uint8_t *ring;static bool ring_on,ring_bypass;static int ring_c0,ring_r0;
static void ring_put(int c,int r) {
    /* 0C72EC: cell (c, r) of the map (row r from the bottom) goes to the
     * ring byte (c & 63) * 64 + ((~(8r)) >> 3 & 63), the index 0C79FE reads
     * for the local pixel (8c, 8r). Rows and columns beyond the map are
     * clipped (0C714E) and keep what the slot held. */
    if (c<0 || c>=(int)columns || r<row0 || r>=row0+(int)rows) return;
    ring[((unsigned)(c&63)<<6)|(63u-((unsigned)r&63u))]=(dynamic_active?dynamic_grid:grid)[(unsigned)c*rows+(unsigned)(r-row0)];
}
static void ring_touch(int c,int r) {
    if (ring_on && c>=ring_c0 && c<ring_c0+64 && r>=ring_r0 && r<ring_r0+64) ring_put(c,r);
}
const uint8_t *neo_collision_ring(int32_t terrain_x,int32_t terrain_y) {
    if (rows<=32u || !ring) {ring_on=false;return NULL;}
    /* 0C73CE (stage start) / 0C74C4 (each update): +36 = (-64 - layer x)
     * >> 4 and +38 = (0x80 - layer y) >> 4 in 16-px blocks of 2x2 cells;
     * 32 blocks from each are loaded (columns 2*+36.., rows 2*+38.. from
     * the bottom: the top row 2*+34+1 sits at the map's last row), and a
     * change of either loads the blocks entering the window. */
    int c0=2*((int16_t)(uint16_t)((0xffc00000u-(uint32_t)terrain_x)>>16)>>4);
    int r0=2*((int16_t)(uint16_t)((0x00800000u-(uint32_t)terrain_y)>>16)>>4);
    if (!ring_on) {memset(ring,0,4096);ring_on=true;ring_c0=c0-128;ring_r0=r0-128;}
    if (c0!=ring_c0 || r0!=ring_r0) {
        for (int c=c0;c<c0+64;c++) for (int r=r0;r<r0+64;r++)
            if (c<ring_c0 || c>=ring_c0+64 || r<ring_r0 || r>=ring_r0+64) ring_put(c,r);
        ring_c0=c0;ring_r0=r0;
    }
    return ring;
}
void neo_collision_ring_copy(int d0,int d1,int d2,int d3,int d4,int d5) {
    if (!ring_on) return;
    /* 0C70FC: clipped to the loaded window (+36/+38, 32 blocks each). */
    int b36=ring_c0/2,b38=ring_r0/2,d6=b36-d4;
    if (d6>=0) {d4+=d6;d0+=d6;d2-=d6;if (d2<=0) return;d6=0;}
    d6+=d2-32;if (d6>=0) {d2-=d6;if (d2<=0) return;}
    d6=b38-d5;
    if (d6>=0) {d5+=d6;d1-=d6;d3-=d6;if (d3<=0) return;d6=0;}
    d6+=d3-32;if (d6>=0) {d3-=d6;if (d3<=0) return;}
    /* 0C714E: 12-bit block numbers clipped to the map (+8 column pairs, +A
     * blocks), then 0C72EC: block (d0+i, d1+j) of the map (rows from the
     * top) to the ring's column pair (d4+i) & 31 and row pair
     * (~d5 & 31) + j, i.e. the window block d5 - j. */
    d0=(int16_t)(uint16_t)(d0<<4)>>4;d1=(int16_t)(uint16_t)(d1<<4)>>4;
    int pairs=(int)columns/2,blocks=(int)rows/2;
    if (d0>=pairs || d1>=blocks) return;
    if (d0<0) {d2+=d0;if (d2<=0) return;d4-=d0;d0=0;}
    if (d1<0) {d3+=d1;if (d3<=0) return;d5+=d1;d1=0;}
    if (d0+d2>pairs) {d2=pairs-d0;if (d2<=0) return;}
    if (d1+d3>blocks) {d3=blocks-d1;if (d3<=0) return;}
    unsigned top=rows-1u,slot0=((unsigned)~d5&31u)*2u;
    for (int i=0;i<d2;i++) for (int j=0;j<d3;j++) for (unsigned k=0;k<4;k++) {
        unsigned dx=k&1u,dr=k>>1,c=2u*(unsigned)(d0+i)+dx,t=2u*(unsigned)(d1+j)+dr;
        unsigned rc=2u*((unsigned)(d4+i)&31u)+dx,rs=(slot0+2u*(unsigned)j+dr)&63u;
        ring[rc<<6|rs]=grid[c*rows+(top-t)];
    }
}
void neo_collision_reset(void) {dynamic_active=false;ring_on=false;}
void neo_collision_set_grid(const uint8_t *cells,uint8_t *dynamic,const uint8_t *unknown_mask,unsigned column_count,unsigned row_count) {
    if (!cells) {grid=neo_grid;dynamic_grid=dynamic_grid_m1;unknown=NULL;columns=NEO_GRID_COLUMNS;rows=32;}
    else {grid=cells;dynamic_grid=dynamic;unknown=unknown_mask;columns=column_count;rows=row_count;}
    dynamic_active=false;unknown_reads=0;ring_on=false;ring=NULL;row0=0;
}
void neo_collision_set_row_origin(int row) {row0=row;}
int neo_collision_row_origin(void) {return row0;}
void neo_collision_set_ring(uint8_t *buffer) {ring=buffer;ring_on=false;}
void neo_collision_ring_bypass(bool on) {ring_bypass=on;}
unsigned neo_collision_rows(void) {return rows;}
unsigned neo_collision_unknown_reads(void) {return unknown_reads;}
static void make_dynamic(void) {
    if (!dynamic_active) {memcpy(dynamic_grid,grid,columns*rows);dynamic_active=true;}
}
static int cell_floor(int32_t value) {
    int64_t v=value;return v>=0?(int)(v/(8*NEO_UNIT)):-(int)((-v+8*NEO_UNIT-1)/(8*NEO_UNIT));
}
unsigned neo_collision_vehicle_wreckage(int32_t x,int32_t y,uint8_t flags) {
    if (!(flags&1) || flags&32) return 0;
    make_dynamic();
    int base_x=cell_floor(x)-7,base_y=cell_floor(y);unsigned changed=0;
    for (unsigned col=0;col<13;col++) for (unsigned row=0;row<3;row++) {
        int cx=base_x+(int)col,cy=base_y+2-(int)row;
        if (cx<0 || cx>=(int)columns || cy<row0 || cy>=row0+(int)rows) continue;
        uint8_t *cell=&dynamic_grid[cx*rows+(cy-row0)];
        if (*cell&15) continue;
        uint8_t value=(*cell&0xe0)|(flags&2?16:0)|vehicle_wreckage[row][col];
        if (value!=*cell) { *cell=value;changed++;ring_touch(cx,cy); }
    }
    return changed;
}

unsigned neo_collision_patch(const uint8_t *cells,unsigned pairs,unsigned blocks,unsigned stride,int column,int top) {
    /* 0C72EC layout of a sub-map's collision cells (4 bytes per 2x2 block,
     * (x,r) (x+1,r) (x,r+1) (x+1,r+1), r counting up from world row `top`;
     * a column of the sub-map is `stride` blocks: 0C7360 skips the rows
     * after the copied ones). */
    make_dynamic();unsigned changed=0;
    for (unsigned p=0;p<pairs;p++) for (unsigned b=0;b<blocks;b++) for (unsigned k=0;k<4;k++) {
        int cx=column+2*(int)p+(int)(k&1u),cy=top-2*(int)b-(int)(k>>1);
        if (cx<0 || cx>=(int)columns || cy<row0 || cy>=row0+(int)rows) continue;
        uint8_t v=cells[(p*stride+b)*4u+k],*cell=&dynamic_grid[cx*rows+(cy-row0)];
        if (*cell!=v) {*cell=v;changed++;ring_touch(cx,cy);}
    }
    return changed;
}
bool neo_collision_window_clip(int d[6]) {
    /* 0C70FC on the ring of a stage taller than 32 rows: D0..D5 clipped to
     * the loaded window (+36/+38, 32 blocks each) before 0C714E; false when
     * nothing is left (carry clear, nothing copied). Mission 3's wagon
     * ledges (0B8FA8, docs/MISION3.md). */
    if (!ring_on) return false;
    int b36=ring_c0/2,b38=ring_r0/2,d6=b36-d[4];
    if (d6>=0) {d[4]+=d6;d[0]+=d6;d[2]-=d6;if (d[2]<=0) return false;d6=0;}
    d6+=d[2]-32;if (d6>=0) {d[2]-=d6;if (d[2]<=0) return false;}
    d6=b38-d[5];
    if (d6>=0) {d[5]+=d6;d[1]-=d6;d[3]-=d6;if (d[3]<=0) return false;d6=0;}
    d6+=d[3]-32;if (d6>=0) {d[3]-=d6;if (d[3]<=0) return false;}
    return true;
}
unsigned neo_collision_stamp(int32_t x,int32_t y,const uint8_t *t) {
    /* ROM 0C88B0 (tools/collision_stamp.py): template dx,dy,w,h,material,
     * -,mask.l, w*h shape nibbles (top row first) at the actor's integer
     * cell; an existing shape s survives unless mask bit s is set; bits
     * 5..7 of the old cell are kept. The ring window of the layer always
     * covers the stamps of Mission 1 (actors on screen). */
    make_dynamic();
    int dx=(int8_t)t[0],dy=(int8_t)t[1],w=t[2],h=t[3];
    uint32_t mask=(uint32_t)t[6]<<24|(uint32_t)t[7]<<16|(uint32_t)t[8]<<8|t[9];
    int base_x=cell_floor(x)+dx,base_y=cell_floor(y)+dy+h-1;unsigned changed=0;
    for (int col=0;col<w;col++) for (int row=0;row<h;row++) {
        int cx=base_x+col,cy=base_y-row;
        if (cx<0 || cx>=(int)columns || cy<row0 || cy>=row0+(int)rows) continue;
        uint8_t *cell=&dynamic_grid[cx*rows+(cy-row0)];unsigned shape=*cell&15u;
        if (shape && !((mask>>shape)&1u)) continue;
        uint8_t value=(uint8_t)((t[10+row*w+col]|(t[4]?16:0))+(*cell&0xe0));
        if (value!=*cell) {*cell=value;changed++;ring_touch(cx,cy);}
    }
    return changed;
}
unsigned neo_collision_stamp_cells(int32_t x,int32_t y,const uint8_t *t) {
    /* ROM 0C8A3C: template dx,dy,w,h,flag +4,group +5, w*h shapes (rows
     * from the top, +2 apart) at the cell (x word >> 3 + dx, y word >> 3 +
     * dy); every cell is replaced by shape | flag (bit 4) | group << 5
     * (the old cell is not read; a shape >= 0x10 traps). No window: the
     * ring slot is written wherever the column is (the sewer gates are on
     * screen). */
    make_dynamic();
    int dx=(int8_t)t[0],dy=(int8_t)t[1],w=t[2],h=t[3];
    int base_x=(int)((uint32_t)x>>19)+dx,base_y=(int)((uint32_t)y>>19)+dy+h-1;unsigned changed=0;
    for (int col=0;col<w;col++) for (int row=0;row<h;row++) {
        int cx=base_x+col,cy=base_y-row;
        if (cx<0 || cx>=(int)columns || cy<row0 || cy>=row0+(int)rows) continue;
        uint8_t *cell=&dynamic_grid[cx*rows+(cy-row0)];
        uint8_t value=(uint8_t)(t[10+row*w+col]|(t[4]?16:0)|(t[5]<<5));
        if (value!=*cell) {*cell=value;changed++;ring_touch(cx,cy);}
    }
    return changed;
}
unsigned neo_collision_columns(void) {return columns;}
uint8_t neo_collision_cell(int x,int y) {
    /* Vertical stages: every reader sees the kernel's ring (0C79FE), which
     * the tower writer 0B7C38 fills beyond the map's rows. */
    if (x<0 || x>=(int)columns*8 || y<row0*8) return 0;
    if (ring_on && !ring_bypass) return ring[((unsigned)(x>>3)&63u)<<6|(63u-((unsigned)(y>>3)&63u))];
    if (y>=(row0+(int)rows)*8) return 0;
    unsigned at=(unsigned)(x/8)*rows+(unsigned)((y>>3)-row0);
    /* A never observed cell (mission packs only) reads as empty and is
     * counted: an explicit stop of the collision coverage. */
    if (unknown && (unknown[at>>3]>>(at&7)&1u)) {unknown_reads++;return 0;}
    return (dynamic_active?dynamic_grid:grid)[at];
}
/* Rows the floor searches visit: the map's, or with the ring the 64 rows
 * of its window. The ring repeats every 64 rows, so beyond the window a
 * row reads the window row sharing its slot: Mission 2 stage 3's tower
 * rises above the map's 272 rows (y 2176+), where the map loop found no
 * floor and the player fell through the pieces it stood on. */
static int row_lo(void) {return ring_on && !ring_bypass?ring_r0:row0;}
static int row_hi(void) {return ring_on && !ring_bypass?ring_r0+64:row0+(int)rows;}
static bool surface(int32_t x,int row,neo_surface_t *out) {
    if (x<0 || x>=(int32_t)columns*8*NEO_UNIT) return false;
    uint8_t code=neo_collision_cell(x/NEO_UNIT,row*8);
    unsigned shape=code&15;int32_t base=row*8*NEO_UNIT,u=(uint32_t)x&(8*NEO_UNIT-1),height;
    if (shape>=1 && shape<=4) height=base+(u/4)+(shape-1)*2*NEO_UNIT;
    else if (shape>=6 && shape<=9) height=base+8*NEO_UNIT-u/4-(shape-6)*2*NEO_UNIT;
    else if (shape==5) height=base+u;
    else if (shape==10) height=base+8*NEO_UNIT-u-1;
    else if (shape>=11 && shape<=13) height=base+8*NEO_UNIT;
    /* The top of a wall column (shapes 14/15 under an empty cell) is a
     * floor for the sweep 013968 (Mission 2 stage 3, wall at x 616: the
     * kernel lands the player at its top, y 128). Mission 1's grid has no
     * such cell. */
    else if (shape>=14 && !neo_collision_cell(x/NEO_UNIT,row*8+8)) height=base+8*NEO_UNIT;
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
    if (x<0 || x>=(int32_t)columns*8*NEO_UNIT) return false;
    if ((neo_collision_cell(x/NEO_UNIT,row*8)>>5)==1u) return false;
    return surface(x,row,out);
}
bool neo_player_surface(int32_t x,int row,neo_surface_t *out) {return player_surface(x,row,out);}
bool neo_surface_unique(int32_t x,neo_surface_t *out) {
    bool found=false;
    for (int row=row_lo(),end=row_hi();row<end;row++) {
        neo_surface_t candidate;if (!surface(x,row,&candidate)) continue;
        if (found) return false;
        *out=candidate;found=true;
    }
    return found;
}
bool neo_surface_at(int32_t x,int32_t reference,int32_t rise,int32_t drop,neo_surface_t *out) {
    if (x<0 || x>=(int32_t)columns*8*NEO_UNIT) return false;
    bool found=false;int64_t closest=INT64_MAX;
    for (int row=row_lo(),end=row_hi();row<end;row++) {
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
bool neo_walk_step(neo_air_t *p,int direction) {return neo_walk_step_shift(p,direction,0);}
/* The ground step with the walk speed shifted right (1: half, the mummy's
 * 0x18000 of 0C18B4/0C1984 against the 0x30000 of the normal walk). */
bool neo_walk_step_shift(neo_air_t *p,int direction,unsigned shift) {
    neo_surface_t current,next;
    if (!neo_surface_at(p->x,p->y,1,NEO_UNIT,&current)) return false;
    neo_ground_velocity(current.shape,direction,&p->vx,&p->vy);
    p->vx>>=shift;p->vy>>=shift;
    p->x+=p->vx;
    int32_t predicted=p->y+p->vy;
    bool found=false;
    // A new ramp can overlap an older flat surface. Select the upper reachable
    // contact within the original step tolerance, so the old floor cannot pull
    // feet through the wreck. Distant roofs remain outside that tolerance.
    for (int row=row_lo(),end=row_hi();row<end;row++) {
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
    for (int row=row_lo(),end=row_hi();row<end;row++) {
        neo_surface_t s;if (!player_surface(p->x,row,&s)) continue;
        if (s.height>previous_y || s.height<p->y) continue;
        if (!hit || s.height>highest.height) { highest=s;hit=true; }
    }
    if (hit) {p->y=highest.height;p->vy=p->y-previous_y;}
    return hit;
}
