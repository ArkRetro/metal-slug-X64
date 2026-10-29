#include <assert.h>
#include <stdio.h>
#include "../src/gameplay.h"
int main(void) {
    msx_game_t g;msx_game_init(&g);
    int start=g.x;
    msx_game_update(&g,1,true,false);
    assert(g.x==start); // Short input edges survive the half-rate player update.
    msx_game_update(&g,1,false,false);
    assert(!g.grounded && g.x>start && g.velocity_y<0);
    assert(msx_game_animation(&g)==MSX_RUN_JUMP);
    for (unsigned i=0;i<180;i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==msx_ground_offset(g.x));
    msx_game_init(&g);msx_game_update(&g,1,false,true);
    assert(msx_game_animation(&g)==MSX_RUN_FIRE);
    assert(msx_ground_offset(700*MSX_UNIT)== -8*MSX_UNIT); // Original village floor Y=64.
    assert(msx_ground_offset(g.x)<-37*MSX_UNIT); // Initial dune is above the village.
    msx_game_t roof;msx_game_init(&roof);roof.world_enabled=false;
    roof.x=600*MSX_UNIT;roof.y=msx_ground_offset(roof.x);
    for (unsigned i=0;i<100;i++) msx_game_update(&roof,0,i<2,false);
    assert(roof.grounded && roof.y== -40*MSX_UNIT); // First roof: original world Y=96.
    bool fell=false;
    for (unsigned i=0;i<160;i++) {msx_game_update(&roof,1,false,false);fell|=!roof.grounded;}
    assert(fell && roof.grounded && roof.y== -8*MSX_UNIT); // Walk off to lower ground.
    g.world_enabled=false;
    for (unsigned i=0;i<1600;i++) msx_game_update(&g,1,false,false);
    assert(g.x==MSX_END_X && g.camera==872);
    int camera=g.camera;
    for (unsigned i=0;i<100;i++) msx_game_update(&g,-1,false,false);
    assert(g.x<MSX_END_X && g.camera==camera); // Camera does not scroll backwards.
    for (unsigned i=0;i<1000;i++) msx_game_update(&g,-1,false,false);
    assert(g.x==(g.camera+28)*MSX_UNIT); // Player remains inside the current camera.
    // Native region: walking right while firing. Re-measured with the ported
    // script/soldiers (replacing the three provisional guards): the script
    // spawns 8 soldiers (0AF17E) for this walk, 7 are killed by one pistol hit
    // each (health 0, ROM 01B4C2 damage 10), one leaves the scene; the three
    // 099DCC/099E06/0400AA entities of the script are counted as external.
    msx_game_init(&g);
    unsigned steps=0;
    for (;steps<6000 && !g.complete;steps++) msx_game_update(&g,1,false,true);
    assert(g.complete && steps<6000);
    assert(g.world.spawned==8 && g.kills==7 && g.hits==7 && !neo_world_alive(&g.world));
    assert(g.world.external_spawns==7 && !g.world.unsupported && !g.world.overflow);
    assert(g.health==3 && g.world.player_hits==0 && g.deaths==0);
    assert(neo_world_region_clear(&g.world) && g.checkpoint==640 && g.camera==872);
    int final_x=g.x;msx_game_update(&g,-1,true,true);assert(g.x==final_x);
    msx_game_init(&g);g.x=640*MSX_UNIT;g.y=msx_ground_offset(g.x);g.checkpoint=640;
    g.health=1;g.invulnerable=0;g.world_enabled=false;
    g.bullets[0]=(msx_bullet_t){g.x/MSX_UNIT,168+g.y/MSX_UNIT,1,true};
    msx_game_hurt(&g,g.x/MSX_UNIT+12);
    assert(g.deaths==1 && g.health==3 && g.x==640*MSX_UNIT && g.invulnerable);
    assert(g.camera==496);
    for (unsigned i=0;i<MSX_BULLETS;i++) assert(!g.bullets[i].active);
    msx_game_init(&g);g.x=200*MSX_UNIT;g.y=msx_ground_offset(g.x);g.camera=56;g.world_enabled=false;
    msx_game_hurt(&g,212);
    assert(g.health==2 && g.x==180*MSX_UNIT && g.y==msx_ground_offset(g.x));
    for (unsigned i=0;i<100;i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==msx_ground_offset(g.x)); // Knockback cannot bury feet in a slope.
    msx_game_init(&g);
    assert(msx_game_begin_at(&g,7852402,6157405));
    assert(g.x==7852402 && g.y==56*MSX_UNIT-6157405 && g.grounded && !g.velocity_x && !g.velocity_y);
    msx_game_t unchanged=g;
    assert(!msx_game_begin_at(&g,7852402,6157406));
    assert(g.x==unchanged.x && g.y==unchanged.y);
    // A soldier's knife (ROM 0AEF14) reaches the player standing still at
    // the first spawn: the native world reports the hit and the player loses
    // health. Re-measured with the ported soldiers.
    msx_game_init(&g);
    unsigned hit_at=0;
    for (unsigned i=0;i<1200 && !hit_at;i++) {msx_game_update(&g,i<170?1:0,false,false);if (g.world.player_hits) hit_at=i;}
    assert(hit_at && g.health==2 && g.world.player_hits==1 && g.invulnerable);
    puts("PASS: half-rate input, air landing, dune/village support, camera bounds, native region encounters, knife hit and respawn");
}
