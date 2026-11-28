/* B14: Mission 2 stage 2, the wall of the slot-7 dynamic object (06851A's
 * map E1B6A, 0140C0) on the slope x 2048..2335 (docs/MISION2.md, "Caída en
 * x 2286"). MAME 0.264 capture m2wall (capture_fullgame.lua from
 * sec02_19932, MSX_FULL_DEBUG_CMDS breakpoints at 01448C and 014492 with
 * A6 = 1014C4, frames 24193..24221; the command is in the doc): player 1
 * walks right up the quarter slopes, the ground sweep 014594 -> 013968 stops
 * at x 2263.99998 (frame 24207, D6 bit 2, and every update after it) with the y advanced by the whole
 * step (0xBA00), then stands there (y +0x3255 once more, then still) until
 * the object is broken (24218: map 0) and walks on.
 *  1. Every captured sweep is reproduced bit for bit by the native kernel
 *     (the port's grid cells of the window, the captured terrain and slot-7
 *     positions, the group map 0B7000).
 *  2. The port's ground step (gameplay.c) took only the swept x: the feet
 *     stayed 0.2 px under the slope at the new x, its one-unit contact
 *     test found no floor and the player fell through the slope (ares
 *     trail: x 2263 -> 2293, the fall rescues of B14). With the sweep's
 *     x/y (014492 stores d0/d1) on a floor (D6 bit 0) the player stays on
 *     the ground at the arcade's position. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_collision.h"
#include "../src/neo_kernel.h"
#include "../src/neo_gactor.h"
/* Grid of the stage (341 x 99, rows from the bottom), nonzero cells of
 * columns 240..329 (the ring window at terrain x -2048 is 248..311). */
enum { COLUMNS=341, ROWS=99 };
static const uint16_t cells[][3]={
    {240,59,1},{241,59,2},{242,59,3},{243,59,4},{244,60,1},{245,60,2},{246,60,3},{247,60,4},{248,61,1},{249,61,2},{250,61,3},{251,61,4},
    {252,62,1},{253,62,2},{254,50,30},{254,62,3},{255,62,4},{256,63,1},{257,63,2},{258,63,3},{259,63,4},{260,64,1},{261,64,2},{262,64,3},
    {263,64,4},{264,65,1},{265,65,2},{266,65,3},{267,65,4},{268,66,1},{269,66,2},{270,66,3},{271,66,4},{272,67,1},{273,67,2},{274,67,3},
    {275,67,4},{276,68,1},{277,68,2},{278,68,3},{279,68,4},{280,69,1},{281,69,2},{282,69,3},{283,69,4},{284,70,1},{285,70,2},{286,70,3},
    {287,70,4},{288,71,1},{289,71,2},{290,71,3},{291,71,4},{292,71,11},{293,71,11},{294,71,11},{295,71,11},{296,71,11},{297,71,11},
    {298,71,11},{299,71,11},{300,71,11},{301,71,11},{302,71,11},{303,71,11},{304,71,11},{305,71,11},{306,71,11},{307,71,11},{308,71,11},
    {309,72,197},{310,73,197},{311,74,197},{312,75,197},{313,76,197},{314,77,197},{315,78,197},{316,79,197},{317,79,11},{318,79,11},
    {319,79,11},{320,79,11},{321,79,11},{322,79,11},{323,79,11},{324,79,11},{325,79,11},{326,79,11},{327,79,11},{328,79,11},{329,79,11}};
/* in: d0..d3, 10C008..10C00B, 10C018, 10C068, terrain +14..+20, slot 7
 * +14..+20, slot 7 map (+26); out: d0..d3, d6, d7, a0 (all terrain). */
typedef struct { unsigned frame; uint32_t v[23]; } wall_call_t;
static const wall_call_t calls[]={
    {24193,{0x08C2DEAB,0x0228B7AB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF80920DB,0xFF2C4837,0xF80C20DB,0xFF2D0837,0x010120DB,0x01624838,0x010420DB,0x01630838,0x000E1B6A,0x08C5C6AB,0x022971AB,0x0002E800,0x0000BA00,0xFFFF0043,0x00004A00,0x0010C72E}},
    {24195,{0x08C5C6AB,0x022971AB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF80620DB,0xFF2B8837,0xF80920DB,0xFF2C4837,0x00FE20DB,0x01618838,0x010120DB,0x01624838,0x000E1B6A,0x08C8AEAB,0x022A2BAB,0x0002E800,0x0000BA00,0xFFFF0043,0x00004A00,0x0010C72E}},
    {24197,{0x08C8AEAB,0x022A2BAB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF80320DB,0xFF2AC837,0xF80620DB,0xFF2B8837,0x00FB20DB,0x0160C838,0x00FE20DB,0x01618838,0x000E1B6A,0x08CB96AB,0x022AE5AB,0x0002E800,0x0000BA00,0xFFFF0043,0x00004A00,0x0010C72E}},
    {24199,{0x08CB96AB,0x022AE5AB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF80020DB,0xFF2A0837,0xF80320DB,0xFF2AC837,0x00F820DB,0x01600838,0x00FB20DB,0x0160C838,0x000E1B6A,0x08CE7EAB,0x022B9FAB,0x0002E800,0x0000BA00,0xFFFF0043,0x00004A00,0x0010C72E}},
    {24202,{0x08CE7EAB,0x022B9FAB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF8000000,0xFF2A0000,0xF80020DB,0xFF2A0837,0x00F80000,0x01600001,0x00F820DB,0x01600838,0x000E1B6A,0x08D166AB,0x022C59AB,0x0002E800,0x0000BA00,0xFFFF0043,0x00004A00,0x0010C72E}},
    {24203,{0x08D166AB,0x022C59AB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x000E1B6A,0x08D44EAB,0x022D13AB,0x0002E800,0x0000BA00,0xFFFF0043,0x00004A00,0x0010C72E}},
    {24205,{0x08D44EAB,0x022D13AB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x000E1B6A,0x08D736AB,0x022DCDAB,0x0002E800,0x0000BA00,0xFFFF0043,0x00004A00,0x0010C72E}},
    {24207,{0x08D736AB,0x022DCDAB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4A00,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x000E1B6A,0x08D7FFFF,0x022E87AB,0x0000C954,0x0000BA00,0xFFFF004F,0x00004A00,0x0010C72E}},
    {24209,{0x08D7FFFF,0x022E87AB,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x8000,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x000E1B6A,0x08D7FFFF,0x022EBA00,0x00000000,0x00003255,0xFFFF010F,0x00004A00,0x0010C72E}},
    {24211,{0x08D7FFFF,0x022EBA00,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x4000,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x000E1B6A,0x08D7FFFF,0x022EBA00,0x00000000,0x00000000,0xFFFF000F,0x00004A00,0x0010C72E}},
    {24213,{0x08D7FFFF,0x022EBA00,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x8000,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x000E1B6A,0x08D7FFFF,0x022EBA00,0x00000000,0x00000000,0xFFFF000F,0x00004A00,0x0010C72E}},
    {24215,{0x08D7FFFF,0x022EBA00,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x8000,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x000E1B6A,0x08D7FFFF,0x022EBA00,0x00000000,0x00000000,0xFFFF000F,0x00004A00,0x0010C72E}},
    {24218,{0x08D7FFFF,0x022EBA00,0x0002E800,0x0000BA00,0xFFFF0000,0x0004,0x8000,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x00000000,0x08DAE7FF,0x022EBA00,0x0002E800,0x00000000,0xFFFF0001,0x00004A00,0x0010C72E}},
    {24221,{0x08DAE7FF,0x022EBA00,0x00000000,0x00000000,0xFFFF0000,0x0010,0x8000,0xF8000000,0xFF2A0000,0xF8000000,0xFF2A0000,0x00F80000,0x01600001,0x00F80000,0x01600001,0x00000000,0x08DAE7FF,0x022EBA00,0x00000000,0x00000000,0xFFFF0001,0x00004A00,0x0010C72E}},
};
static uint8_t grid[COLUMNS*ROWS],grid_dynamic[COLUMNS*ROWS],ring[4096];
static neo_kscene_t scene_of(const wall_call_t *c) {
    neo_kscene_t k;memset(&k,0,sizeof(k));
    k.terrain=(neo_kobj_t){(int32_t)c->v[7],(int32_t)c->v[8],(int32_t)c->v[9],(int32_t)c->v[10]};
    k.root=k.terrain;
    k.group_map=ga_rom(0xb7000);assert(k.group_map);
    k.ring=neo_collision_ring(k.terrain.x,k.terrain.y);
    k.w4018=(uint16_t)c->v[5];k.w4068=(uint16_t)c->v[6];
    neo_kernel_dyn_begin(&k);
    if (c->v[15]) {
        k.dyn[7].map=ga_rom(c->v[15]);assert(k.dyn[7].map);
        k.dyn[7].pos=(neo_kobj_t){(int32_t)c->v[11],(int32_t)c->v[12],(int32_t)c->v[13],(int32_t)c->v[14]};
        neo_kernel_dyn_update(&k,7);
    }
    return k;
}
static neo_kreq_t request_of(const wall_call_t *c,int32_t x,int32_t y) {
    return (neo_kreq_t){x,y,(int32_t)c->v[2],(int32_t)c->v[3],NEO_KOBJ_TERRAIN,0,(uint8_t)(c->v[4]>>24),(uint8_t)(c->v[4]>>16),(uint8_t)(c->v[4]>>8),(uint8_t)c->v[4],0xff};
}
int main(void) {
    for (unsigned i=0;i<sizeof(cells)/sizeof(cells[0]);i++) grid[cells[i][0]*ROWS+cells[i][1]]=(uint8_t)cells[i][2];
    neo_collision_set_grid(grid,grid_dynamic,NULL,COLUMNS,ROWS);neo_collision_set_ring(ring);
    unsigned n=sizeof(calls)/sizeof(calls[0]),walls=0;
    /* 1. The sweeps. */
    for (unsigned i=0;i<n;i++) {
        const wall_call_t *c=&calls[i];neo_kscene_t k=scene_of(c);
        neo_kreq_t q=request_of(c,(int32_t)c->v[0],(int32_t)c->v[1]);neo_kres_t r;
        assert(neo_kernel_player_sweep(&k,&q,&r));
        if ((uint32_t)r.x!=c->v[16] || (uint32_t)r.y!=c->v[17] || (uint32_t)r.vx!=c->v[18] || (uint32_t)r.vy!=c->v[19] ||
            r.flags!=(uint16_t)c->v[20] || r.kind!=c->v[21] || r.parent!=NEO_KOBJ_TERRAIN) {
            printf("FAIL: sweep frame %u: %08X %08X %08X %08X flags %04X kind %08X parent %u, MAME %08X %08X %08X %08X flags %04X kind %08X\n",c->frame,
                (unsigned)r.x,(unsigned)r.y,(unsigned)r.vx,(unsigned)r.vy,r.flags,(unsigned)r.kind,r.parent,
                c->v[16],c->v[17],c->v[18],c->v[19],c->v[20]&0xffffu,c->v[21]);
            return 1;
        }
        if (r.flags&4u) {walls++;assert(c->frame>=24207 && c->frame<=24215);}   /* against the wall until it breaks */
    }
    assert(walls==5);
    /* 2. gameplay.c's ground step at the stop (frame 24207): the walk to
     * the right meets the wall (wall_stop); before the fix only x was
     * taken and the ground step (direction 0) ran from the old y. */
    const wall_call_t *c=&calls[7];assert(c->frame==24207);
    neo_kscene_t k=scene_of(c);
    neo_kreq_t q=request_of(c,(int32_t)c->v[0],(int32_t)c->v[1]);neo_kres_t r;
    assert(neo_kernel_player_sweep(&k,&q,&r) && (r.flags&4u) && (r.flags&1u));
    neo_air_t old={r.x,(int32_t)c->v[1],0,0};
    bool old_grounded=neo_walk_step_shift(&old,0,0);
    neo_surface_t s;assert(neo_surface_at(r.x,(int32_t)c->v[1],4*NEO_UNIT,4*NEO_UNIT,&s));
    printf("x only: feet %.3f px under the slope (%.3f) at x %.5f: %s\n",(s.height-(int32_t)c->v[1])/65536.0,s.height/65536.0,r.x/65536.0,
        old_grounded?"on the ground":"no floor, falls through the slope");
    assert(!old_grounded);
    /* The fix: x and y of the sweep, still on the ground; the following
     * updates (still walking right into the wall) stay there as in MAME. */
    neo_air_t body={r.x,r.y,0,0};bool grounded=(r.flags&1u)!=0;
    assert(grounded && (uint32_t)body.x==calls[8].v[0] && (uint32_t)body.y==calls[8].v[1]);
    for (unsigned i=8;i<=11;i++) {
        k=scene_of(&calls[i]);q=request_of(&calls[i],body.x,body.y);
        assert(neo_kernel_player_sweep(&k,&q,&r) && (r.flags&1u));
        if (r.flags&4u) {body.x=r.x;body.y=r.y;}
        else grounded=neo_walk_step_shift(&body,1,0);
        assert(grounded && (uint32_t)body.x==calls[i].v[16] && (uint32_t)body.y==calls[i].v[17]);
    }
    printf("PASS: %u player ground sweeps 014594/013968 at the slot-7 wall bit-exact (5 wall stops from x %.5f); x only falls through the slope, "
        "the sweep's x/y stays on the ground at MAME's position for 5 updates\n",n,calls[7].v[16]/65536.0);
    return 0;
}
