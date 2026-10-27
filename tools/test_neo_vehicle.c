#include <assert.h>
#include <stdio.h>
#include "../src/neo_vehicle.h"
#include "../src/neo_collision.h"
#include "../src/gameplay.h"
#define U 65536
static neo_vehicle_t vehicle(int facing) {
    neo_vehicle_t v;neo_vehicle_init(&v,937*U+U/2,64*U,936*U,0,facing,300,9);return v;
}
int main(void) {
    neo_vehicle_t v;neo_vehicle_init(&v,855*U,64*U,936*U,5*U/2,1,300,13);
    for (unsigned i=0;i<30;i++) assert(!neo_vehicle_step(&v));
    assert(v.x==930*U && v.state==NEO_VEHICLE_MOVING); // Original frames 3420 -> 3480.
    for (unsigned i=0;i<3;i++) neo_vehicle_step(&v);
    assert(v.x==937*U+U/2 && !v.vx && v.state==NEO_VEHICLE_STOPPED);
    v=vehicle(-1);
    assert(neo_vehicle_hit_segment(&v,v.x-20*U,v.x-20*U,104*U,0,0));
    v.facing=1;
    assert(!neo_vehicle_hit_segment(&v,v.x-20*U,v.x-20*U,104*U,0,0));
    assert(neo_vehicle_hit_segment(&v,v.x+20*U,v.x+20*U,104*U,0,0));
    int32_t outside=(v.x/U+48)*U+U/4;
    assert(!neo_vehicle_hit_segment(&v,outside,outside,80*U,0,0)); // Integer box anchor, fractional motion retained.
    assert(neo_vehicle_hit_segment(&v,v.x-100*U,v.x+100*U,80*U,0,0));
    assert(!neo_vehicle_hit_segment(&v,v.x-100*U,v.x+100*U,130*U,0,0));
    neo_vehicle_damage(&v,150);assert(v.health==150 && v.pose<4);
    neo_vehicle_damage(&v,10);assert(v.health==140 && v.pose>=4);
    neo_collision_reset();assert(neo_collision_cell(880,64)==0);
    neo_vehicle_damage(&v,1000);assert(!v.health && v.state==NEO_VEHICLE_DYING);
    unsigned events=neo_vehicle_step(&v);assert(events==(NEO_VEHICLE_STAMP|NEO_VEHICLE_SMOKE));
    assert(neo_collision_vehicle_wreckage(v.x,v.y,v.flags)==13);
    assert(neo_collision_vehicle_wreckage(v.x,v.y,v.flags)==0);
    assert(neo_collision_cell(880,64)==5 && neo_collision_cell(896,80)==5 && neo_collision_cell(936,80)==11);
    unsigned explosions=0;
    for (unsigned tick=1;tick<18;tick++) {
        events=neo_vehicle_step(&v);assert(!(events&NEO_VEHICLE_STAMP));
        if (events&NEO_VEHICLE_EXPLOSION) {explosions++;assert(tick==5 && v.pose==8);}
    }
    assert(v.state==NEO_VEHICLE_WRECK && v.pose==8 && explosions==1);
    for (unsigned i=0;i<20;i++) assert(!neo_vehicle_step(&v));
    neo_collision_reset();assert(neo_collision_cell(880,64)==0);
    msx_game_t g;msx_game_init(&g);msx_game_vehicle_reference(&g);
    for (unsigned i=0;i<350;i++) msx_game_update(&g,0,false,true);
    assert(g.vehicle.health==0 && g.vehicle.state==NEO_VEHICLE_WRECK && g.wreckage_cells==13);
    for (unsigned i=0;i<100 && g.x>944*U;i++) msx_game_update(&g,-1,false,false);
    assert(g.grounded && g.y== -32*U); // Walk up the right ramp onto the wreck.
    msx_game_init(&g);assert(!g.vehicle.active && neo_collision_cell(880,64)==0);
    puts("PASS: original vehicle movement, mirrored hitboxes, strict damage threshold, one-shot death events, wreck geometry, 30 pistol hits and walkable ramp");
}
