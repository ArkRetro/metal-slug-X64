#include <assert.h>
#include <stdio.h>
#include "../src/gameplay.h"
#include "../src/msx_sfx.h"
#include "../src/mission_camera.h"
#include "../src/neo_collision.h"
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
    // Whole mission without the native world: the reference camera follows
    // the player up to the original command list's hold (3520); the player's
    // right limit keeps camera+292-1/65536.
    int highest=0;
    for (unsigned i=0;i<3200;i++) {
        msx_game_update(&g,1,false,false);
        assert(g.camera>=highest && g.camera<=MSX_CAMERA_MAX && g.x<=MSX_END_X);highest=g.camera;
        if (g.x>1200*MSX_UNIT && g.x<2100*MSX_UNIT) assert(g.grounded && g.y==-8*MSX_UNIT); // Walkable Y=64 floor.
    }
    assert(g.camera==MSX_CAMERA_MAX && g.x==(MSX_CAMERA_MAX+292)*MSX_UNIT-1);
    int camera=g.camera;
    for (unsigned i=0;i<100;i++) msx_game_update(&g,-1,false,false);
    assert(g.x<(MSX_CAMERA_MAX+292)*MSX_UNIT-1 && g.camera==camera); // Camera does not scroll backwards.
    for (unsigned i=0;i<1000;i++) msx_game_update(&g,-1,false,false);
    assert(g.x==(g.camera+28)*MSX_UNIT); // Player remains inside the current camera.
    // Whole Mission 1 with the native world and camera (reference run: walk
    // right firing, up-shots every other half second, jump every 4 s; the
    // test keeps the player alive). The script runs to its op 3 end
    // (2D7132) with the region's soldiers, truck, bomber, the robed swordsman
    // 0A8C46 and the boss native; the other rest-of-mission constructors are
    // counted as external. The robed swordsman's gesture toggles RAM 10C606,
    // the boss starts, dies and signals the mission end (018F92, 018F40).
    msx_game_init(&g);
    unsigned steps=0;
    unsigned voices=0,mapped=0,requests=0,pending=0;
    bool region_end_seen=false;int max_height=0;unsigned max_progress=0;
    for (;steps<40000 && !g.complete;steps++) {
        int dir=1;
        if (g.world.boss_active) dir=(steps/90)%3==0?-1:(steps/90)%3==1?1:0;
        g.aim_up=(steps%64)>=40;
        msx_game_update(&g,dir,(steps%240)<4,true);
        g.invulnerable=2;
        region_end_seen|=g.region_end;
        if (g.camera_height>max_height) max_height=g.camera_height;
        if (neo_camera_progress(&g.world.camera)>max_progress) max_progress=neo_camera_progress(&g.world.camera);
        // Sound requests of the native world: every one has a packed render.
        for (unsigned i=0;i<g.world.sound_count;i++) {
            int id=msx_sfx_lookup(g.world.sounds[i]);requests++;
            if (id>=0) {mapped++;voices+=msx_sfx_defs[id].channel==MSX_SFX_CH_VOICE;}
            // Not rendered yet (docs/AUDIO_JUEGO.md): vehicle engine loop 10A5,
            // 10E8 of the generic actors and the boss's own requests.
            else pending++;
        }
        g.world.sound_count=0;
    }
    printf("mission: steps=%u spawned=%u kills=%u ext=%u unsupported=%u boss_unsupported=%u (%05lx) boss_hits=%u boss_effects_ext=%u hits=%u mapped=%u/%u progress=%u height=%d\n",
        steps,g.world.spawned,g.kills,g.world.external_spawns,g.world.unsupported,g.world.boss_unsupported,(unsigned long)g.world.boss_unsupported_label,
        g.world.boss_hits,g.world.boss_external_effects,g.world.player_hits,mapped,requests,max_progress,max_height);
    assert(requests && voices && !g.world.sound_dropped);
    assert(g.shots>=g.hits);
    assert(g.complete && g.mission_complete && g.world.mission_step==2 && steps<40000);
    assert(region_end_seen && g.world.script.finished && !g.world.script.halted);
    assert(max_progress==3376 && g.camera==MSX_CAMERA_MAX);
    msx_game_init(&g);g.x=640*MSX_UNIT;g.y=msx_ground_offset(g.x);g.checkpoint=640;
    g.health=1;g.invulnerable=0;g.world_enabled=false;
    g.bullets[0]=(msx_bullet_t){g.x/MSX_UNIT,168+g.y/MSX_UNIT,1,true,false};
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
    // Extended terrain: floor beyond the old end, and a measured upper roof.
    assert(msx_ground_offset(1192*MSX_UNIT)== -8*MSX_UNIT && msx_ground_offset(2200*MSX_UNIT)== -8*MSX_UNIT);
    msx_game_init(&g);g.world_enabled=false;g.camera=1200;g.x=1350*MSX_UNIT;g.y=56*MSX_UNIT-170*MSX_UNIT;g.grounded=false;
    for (unsigned i=0;i<120 && !g.grounded;i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==56*MSX_UNIT-144*MSX_UNIT); // Lands on the Y=144 roof, not the floor below.
    msx_game_init(&g);g.world_enabled=false;g.camera=1800-144;g.x=1800*MSX_UNIT;g.y=msx_ground_offset(g.x);
    msx_game_update(&g,0,true,false);
    for (unsigned i=0;i<200 && (g.ticks<4 || !g.grounded);i++) msx_game_update(&g,0,false,false);
    assert(g.grounded && g.y==-8*MSX_UNIT && g.camera==1800-144 && g.air_ticks>0);
    // Vertical camera: scroll-driven rise 944..1008; middle bank switches at 1023.
    assert(msx_camera_height(872)==0 && msx_camera_height(944)==0 && msx_camera_height(947)==1);
    assert(msx_camera_height(965)==6 && msx_camera_height(1008)==16 && msx_camera_height(MSX_CAMERA_MAX)==16);
    assert(msx_layer_visible(1,1022) && !msx_layer_visible(2,1022) && !msx_layer_visible(1,1023) && msx_layer_visible(2,1023));
    // The observed vehicle wreck at X=1174 is complete now: 13 cells, as in the arcade trace.
    neo_collision_reset();
    assert(neo_collision_vehicle_wreckage(1174*MSX_UNIT,64*MSX_UNIT,1)==13);
    assert(neo_collision_cell(1196,80)==10 && neo_collision_cell(1208,64)==10); // Cells beyond the old 1192 limit.
    neo_collision_reset();
    assert(!neo_collision_cell(1196,80) && !neo_collision_cell(1208,64)); // Event-only: never static terrain.
    // A soldier's knife (ROM 0AEF14) reaches the player standing still at
    // the first spawn: the native world reports the hit and the player loses
    // health. Re-measured with the ported soldiers.
    msx_game_init(&g);
    unsigned hit_at=0;
    for (unsigned i=0;i<1200 && !hit_at;i++) {msx_game_update(&g,i<170?1:0,false,false);if (g.world.player_hits) hit_at=i;}
    assert(hit_at && g.health==2 && g.world.player_hits==1 && g.invulnerable);
    puts("PASS: half-rate input, air landing, dune/village support, reference camera to 3520, whole Mission 1 with native camera, script, boss and mission end, extended terrain, knife hit and respawn");
}
