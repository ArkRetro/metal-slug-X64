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
    msx_game_t roof;msx_game_init(&roof);roof.enemy.alive=false;
    roof.x=600*MSX_UNIT;roof.y=msx_ground_offset(roof.x);
    for (unsigned i=0;i<100;i++) msx_game_update(&roof,0,i<2,false);
    assert(roof.grounded && roof.y== -40*MSX_UNIT); // First roof: original world Y=96.
    bool fell=false;
    for (unsigned i=0;i<160;i++) {msx_game_update(&roof,1,false,false);fell|=!roof.grounded;}
    assert(fell && roof.grounded && roof.y== -8*MSX_UNIT); // Walk off to lower ground.
    g.enemy.alive=false;
    for (unsigned i=0;i<1600;i++) msx_game_update(&g,1,false,false);
    assert(g.x==MSX_END_X && g.camera==872);
    int camera=g.camera;
    for (unsigned i=0;i<100;i++) msx_game_update(&g,-1,false,false);
    assert(g.x<MSX_END_X && g.camera==camera); // Camera does not scroll backwards.
    for (unsigned i=0;i<1000;i++) msx_game_update(&g,-1,false,false);
    assert(g.x==(g.camera+28)*MSX_UNIT); // Player remains inside the current camera.
    msx_game_init(&g);
    bool last_guard_seen=false;
    for (unsigned i=0;i<2500 && !g.complete;i++) {
        unsigned before=g.kills;msx_game_update(&g,1,false,true);
        if (before==2 && g.kills==3) {
            assert(!g.enemy.alive && g.enemy.respawn==90 && !g.complete);
            last_guard_seen=true;msx_game_t after=g;
            for (unsigned j=0;j<90;j++) msx_game_update(&after,0,false,false);
            assert(after.kills==3 && !after.enemy.alive && !after.enemy.respawn);
        }
    }
    assert(last_guard_seen);
    assert(g.complete && g.kills==3 && g.hits==6 && g.health>0);
    assert(g.checkpoint==640 && g.camera==872 && !g.enemy.alive && !g.enemy.respawn);
    int final_x=g.x;msx_game_update(&g,-1,true,true);assert(g.x==final_x);
    msx_game_init(&g);g.x=640*MSX_UNIT;g.y=msx_ground_offset(g.x);g.checkpoint=640;
    g.health=1;g.invulnerable=0;g.enemy.alive=false;
    g.enemy_bullets[0]=(msx_bullet_t){g.x/MSX_UNIT+12,168+g.y/MSX_UNIT,-1,true};
    msx_game_update(&g,0,false,false);
    assert(g.deaths==1 && g.health==3 && g.x==640*MSX_UNIT && g.invulnerable);
    assert(g.camera==496);
    for (unsigned i=0;i<MSX_ENEMY_BULLETS;i++) assert(!g.enemy_bullets[i].active);
    msx_game_init(&g);g.x=200*MSX_UNIT;g.y=msx_ground_offset(g.x);g.camera=56;g.enemy.alive=false;
    g.enemy_bullets[0]=(msx_bullet_t){212,168+g.y/MSX_UNIT,-1,true};
    msx_game_update(&g,0,false,false);
    assert(g.health==2 && g.x==180*MSX_UNIT && g.y==msx_ground_offset(g.x));
    for (unsigned i=0;i<100;i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==msx_ground_offset(g.x)); // Knockback cannot bury feet in a slope.
    msx_game_init(&g);
    assert(msx_game_begin_at(&g,7852402,6157405));
    assert(g.x==7852402 && g.y==56*MSX_UNIT-6157405 && g.grounded && !g.velocity_x && !g.velocity_y);
    msx_game_t unchanged=g;
    assert(!msx_game_begin_at(&g,7852402,6157406));
    assert(g.x==unchanged.x && g.y==unchanged.y);
    puts("PASS: half-rate input, air landing, dune/village support, camera bounds, provisional encounters and respawn");
}
