#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/marco_campaign_world.h"
#include "../src/neo_collision.h"
#include "marco_campaign_clip_cases.h"

static int32_t signed32(uint32_t v) {
    return v<=INT32_MAX?(int32_t)v:(int32_t)((int64_t)v-INT64_C(4294967296));
}
static void original_clipping_results(void) {
    static const uint16_t words[4]={28,0,264,480};
    for(unsigned i=0;i<sizeof(original_clip_cases)/sizeof(*original_clip_cases);++i) {
        marco_campaign_world_t world={.initialized=true};
        assert(marco_campaign_world_set_policy(&world,words,signed32(original_clip_cases[i].camera),
            signed32(original_clip_cases[i].scroll),signed32(original_clip_cases[i].offset)));
        neo_contact_request_t r={.x=signed32(original_clip_cases[i].x),.y=signed32(original_clip_cases[i].y),
            .dx=signed32(original_clip_cases[i].dx),.dy=signed32(original_clip_cases[i].dy)};
        assert(marco_campaign_world_clip_request(&world,&r));
        assert((uint32_t)r.dx==original_clip_cases[i].result_dx && (uint32_t)r.dy==original_clip_cases[i].result_dy);
        assert(world.boundary_hits==original_clip_cases[i].result_hits);
    }
}
static void walk_trace(void) {
    FILE *f=fopen("data/original-walk-trace.csv","r");assert(f);
    char header[100];assert(fgets(header,sizeof(header),f));
    unsigned frame,checks=0;uint32_t x,y,vx,vy;
    assert(fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5);
    marco_campaign_world_t world;marco_crouch_t player={0};
    assert(marco_campaign_world_init(&world,&player,signed32(x),signed32(y)));
    player.motion62=0x30000;
    while(fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5) {
        ++world.epoch;assert(marco_campaign_world_ground(&world,&player));
        assert(abs(player.x-signed32(x))<=2 && abs(player.y-signed32(y))<=1);
        assert(abs(player.vx-signed32(vx))<=2 && abs(player.vy-signed32(vy))<=1);
        assert(player.upper.input.attributes&64u);++checks;
    }
    fclose(f);assert(checks==350);
}
static void roof_trace(void) {
    FILE *f=fopen("data/original-roof-trace.csv","r");assert(f);
    char header[100];assert(fgets(header,sizeof(header),f));
    unsigned frame,checks=0;uint32_t x,y,vx,vy;
    assert(fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5);
    marco_campaign_world_t world;marco_crouch_t player={0};
    assert(marco_campaign_world_init(&world,&player,signed32(x),signed32(y)));
    player.vy=NEO_JUMP_VY;player.upper.input.mask9c=255;bool landed=false;
    while(fscanf(f,"%u,%"SCNu32",%"SCNu32",%"SCNu32",%"SCNu32" ",&frame,&x,&y,&vx,&vy)==5) {
        ++world.epoch;
        if(landed) {player.motion62=0;assert(marco_campaign_world_ground(&world,&player));}
        else {
            /* Recovered C2692 has already updated velocity when C109C runs. */
            player.vy+=neo_asr(-22*(int64_t)NEO_UNIT-player.vy,5);
            assert(marco_campaign_world_air(&world,&player,true));
            landed=(player.upper.input.attributes&64u)!=0;
        }
        assert(player.x==signed32(x) && player.y==signed32(y));
        assert(player.vx==signed32(vx) && player.vy==signed32(vy));++checks;
    }
    fclose(f);assert(checks==24 && landed);
}
static void live_scalar_and_wrapper(void) {
    marco_campaign_world_t world;marco_crouch_t player={0};
    assert(!marco_campaign_world_init(&world,&player,-1,96*NEO_UNIT));
    assert(!marco_campaign_world_init(&world,&player,128*NEO_UNIT,97*NEO_UNIT));
    assert(marco_campaign_world_init(&world,&player,128*NEO_UNIT,96*NEO_UNIT));
    assert(player.field6c==8 && (player.upper.input.attributes&64u));
    player.motion62=0x12000;assert(marco_campaign_world_ground(&world,&player));
    assert(player.x==128*NEO_UNIT+0x12000 && player.vx==0x12000);
    player.motion62=-0x12000;assert(marco_campaign_world_ground(&world,&player));
    assert(player.x==128*NEO_UNIT && player.vx==-0x12000);
    player.motion62=0;assert(marco_campaign_world_ground(&world,&player));
    assert(player.vx==0 && player.vy==0);
    player.upper.input.mask9c=255;player.upper.gate.limit=255;
    player.upper.packed=20u<<16;player.upper.input.flags9a|=8;
    player.upper.input.accumulator9b=0;player.vy=0x10000;
    assert(marco_campaign_world_air(&world,&player,true));
    assert((player.upper.input.attributes&1u) && player.vy==0x10000);
    assert(!player.upper.input.accumulator9b); /* No third filter side effect. */
    marco_animation_view_t view={0};assert(marco_campaign_world_view(&world,&player,&view));
    assert(view.render.x==128 && view.render.y==97 && view.render.attributes==player.upper.input.attributes);
}
static void clipping_policy(void) {
    marco_campaign_world_t world;marco_crouch_t player={0};
    assert(marco_campaign_world_init(&world,&player,128*NEO_UNIT,96*NEO_UNIT));
    const uint16_t policy[4]={28,0,264,480};
    assert(marco_campaign_world_set_policy(&world,policy,100*NEO_UNIT,0,256*NEO_UNIT));
    neo_contact_request_t r={.x=128*NEO_UNIT,.y=96*NEO_UNIT,.dx=-3*NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && !r.dx && world.boundary_hits==2);
    r=(neo_contact_request_t){.x=390*NEO_UNIT,.y=96*NEO_UNIT,.dx=3*NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && r.dx==2*NEO_UNIT-1 && world.boundary_hits==1);
    r=(neo_contact_request_t){.x=400*NEO_UNIT,.y=96*NEO_UNIT,.dx=NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && !r.dx && world.boundary_hits==1);
    r=(neo_contact_request_t){.x=400*NEO_UNIT,.y=96*NEO_UNIT,.dx=-NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && r.dx==-NEO_UNIT && !world.boundary_hits);
    r=(neo_contact_request_t){.x=128*NEO_UNIT,.y=223*NEO_UNIT,.dy=2*NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && r.dy==NEO_UNIT-1 && world.boundary_hits==4);
    r=(neo_contact_request_t){.x=128*NEO_UNIT,.y=-255*NEO_UNIT,.dy=-2*NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && r.dy==-NEO_UNIT && world.boundary_hits==8);
    r=(neo_contact_request_t){.x=128*NEO_UNIT,.y=-257*NEO_UNIT,.dy=-NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && r.dy==NEO_UNIT && world.boundary_hits==8);
    /* Source lower-bound code may correct an already-below-bound actor's
     * downward request upwards; preserve this observed instruction behavior. */
    assert(marco_campaign_world_set_policy(&world,policy,100*NEO_UNIT,8*NEO_UNIT,256*NEO_UNIT));
    r=(neo_contact_request_t){.x=128*NEO_UNIT,.y=215*NEO_UNIT,.dy=2*NEO_UNIT};
    assert(marco_campaign_world_clip_request(&world,&r) && r.dy==NEO_UNIT-1 && world.boundary_hits==4);
    player.motion62=-0x30000;
    assert(marco_campaign_world_ground(&world,&player));
    assert(player.x==128*NEO_UNIT && !player.vx && (player.upper.input.attributes&64u));
    player.y=223*NEO_UNIT;player.vy=2*NEO_UNIT;
    assert(marco_campaign_world_set_policy(&world,policy,100*NEO_UNIT,0,256*NEO_UNIT));
    assert(marco_campaign_world_air(&world,&player,true));
    assert(player.y==224*NEO_UNIT-1 && player.vy==2*NEO_UNIT);
    assert(!(player.upper.input.flags5f&2u)); /* Screen bounds aren't a solid ceiling. */
}
static void scene_relocation(void) {
    marco_campaign_world_t world;marco_crouch_t player={0};
    assert(marco_campaign_world_init(&world,&player,128*NEO_UNIT,96*NEO_UNIT));
    const uint16_t policy[4]={28,0,264,480};
    assert(marco_campaign_world_set_policy(&world,policy,0,0,256*NEO_UNIT));
    world.epoch=27;player.upper.input.attributes|=0x6301;player.upper.input.flags5f=0x70;
    player.vx=0x12345;player.vy=-0x23456;player.motion62=0x30000;
    neo_surface_t slope;assert(neo_surface_at(96*NEO_UNIT,88*NEO_UNIT,8*NEO_UNIT,8*NEO_UNIT,&slope));
    assert(slope.shape==1);
    assert(marco_campaign_world_relocate(&world,&player,96*NEO_UNIT,slope.height,true));
    assert(player.x==96*NEO_UNIT && player.y==slope.height && player.field6c==8);
    assert((uint8_t)world.contact.kind_angle==0x4a && world.contact.parent_tag==world.terrain.tag);
    assert(player.vx==0x12345 && player.vy==-0x23456 && player.motion62==0x30000);
    assert(player.upper.input.attributes==0x6341 && player.upper.input.flags5f==0x70);
    assert(world.contact.cached_x==player.x && world.contact.cached_y==player.y && world.contact.mark==27);
    assert(world.contact.word1e==(uint16_t)(0x8834-((uint32_t)slope.height>>16)));
    assert(world.epoch==27 && world.policy_enabled && world.policy_words[2]==264 && world.projected_y_offset==256*NEO_UNIT);
    assert(marco_campaign_world_ground(&world,&player));
    assert(player.vx==190464); /* New slope tangent on the first body call. */
    marco_campaign_world_t before=world;marco_crouch_t old_player=player;
    assert(!marco_campaign_world_relocate(&world,&player,128*NEO_UNIT,97*NEO_UNIT,true));
    assert(!memcmp(&world,&before,sizeof(world)) && !memcmp(&player,&old_player,sizeof(player)));
    uint16_t z=world.contact.word1e;
    assert(marco_campaign_world_relocate(&world,&player,128*NEO_UNIT,120*NEO_UNIT,false));
    assert(!(player.upper.input.attributes&64u) && !world.contact.parent_tag && !player.field6c);
    assert(world.contact.projected_x==128*NEO_UNIT && world.contact.cached_y==120*NEO_UNIT);
    assert(world.contact.word1e==z); /* 01AD8C updates depth on grounded support. */
}
int main(void) {
    neo_collision_reset();original_clipping_results();walk_trace();roof_trace();live_scalar_and_wrapper();clipping_policy();scene_relocation();
    puts("PASS: original clipping requests, live Marco scalar/C109C, 350 walk and 24 roof states");
    return 0;
}
