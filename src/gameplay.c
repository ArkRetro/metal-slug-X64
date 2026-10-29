#include "gameplay.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "neo_motion.h"
#include "neo_collision.h"

static int clamp(int x,int lo,int hi) { return x<lo?lo:x>hi?hi:x; }
int msx_ground_offset(int x) {
    neo_surface_t s;
    return neo_surface_at(x,64*MSX_UNIT,64*MSX_UNIT,64*MSX_UNIT,&s)?56*MSX_UNIT-s.height:0;
}
int msx_game_animation(const msx_game_t *g) {
    if (!g->grounded) return g->moving?MSX_RUN_JUMP:MSX_JUMP;
    if (g->shooting) return g->moving?MSX_RUN_FIRE:MSX_FIRE;
    return g->moving?MSX_RUN:MSX_IDLE;
}
void msx_game_init(msx_game_t *g) {
    neo_collision_reset();
    memset(g,0,sizeof(*g));
    g->x=MSX_START_X;g->y=msx_ground_offset(g->x);
    g->facing=1;g->grounded=true;g->health=3;
    // The original region script starts at its first record with the RNG
    // ring the mission handoff left (data/soldier_ai_tables.json).
    neo_world_init(&g->world,-(int32_t)((uint32_t)g->camera<<16));g->world_enabled=true;
}
bool msx_game_begin_at(msx_game_t *g,int32_t world_x,int32_t world_y) {
    neo_surface_t support;
    if (!neo_surface_unique(world_x,&support) || support.height!=world_y) return false;
    msx_game_init(g);g->x=world_x;g->y=56*MSX_UNIT-world_y;
    return true;
}
void msx_game_vehicle_reference(msx_game_t *g) {
    // Observed stationary vehicle variant, at the captured position. This is
    // selected only by the internal reference build, not by provisional kills.
    g->x=1020*MSX_UNIT;g->camera=872;g->y=msx_ground_offset(g->x);
    g->facing=-1;g->world_enabled=false;
    neo_vehicle_init(&g->vehicle,937*MSX_UNIT+MSX_UNIT/2,64*MSX_UNIT,936*MSX_UNIT,0,1,300,9);
}
void msx_game_hurt(msx_game_t *g,int from) {
    if (g->invulnerable || g->complete) return;
    g->health--;g->invulnerable=60;
    g->x=clamp(g->x+(g->x/MSX_UNIT<from?-20:20)*MSX_UNIT,(g->camera+28)*MSX_UNIT,MSX_END_X);
    if (g->grounded) {
        neo_surface_t support;
        if (neo_surface_at(g->x,56*MSX_UNIT-g->y,8*MSX_UNIT,8*MSX_UNIT,&support))
            g->y=56*MSX_UNIT-support.height;
        else {g->grounded=false;g->air_ticks=0;}
    }
    if (!g->health) {
        g->deaths++;g->health=3;g->x=g->checkpoint?g->checkpoint*MSX_UNIT:MSX_START_X;
        g->camera=clamp(g->x/MSX_UNIT-144,0,MSX_WORLD_WIDTH-320);
        g->y=msx_ground_offset(g->x);g->velocity_x=g->velocity_y=0;g->grounded=true;g->invulnerable=90;
        memset(g->bullets,0,sizeof(g->bullets));
    }
}
/* Projected words of the player as the original registers them (+3C/+40). */
static neo_world_player_t player_frame(const msx_game_t *g) {
    neo_world_player_t p={(uint16_t)(g->x/MSX_UNIT-g->camera),(uint16_t)(MSX_PROJECTED_BASE_Y+56-g->y/MSX_UNIT),
        (uint8_t)((g->facing<0?1u:0u)|(g->grounded?64u:0u)),true,false};
    return p;
}
void msx_game_update(msx_game_t *g,int direction,bool jump,bool fire) {
    if (g->complete) return;
    g->ticks++;g->moving=direction!=0;
    if (g->invulnerable) g->invulnerable--;
    if (g->vehicle_flash) g->vehicle_flash--;
    if (direction) g->facing=direction<0?-1:1;
    g->jump_pending|=jump && !g->jump_was_down;
    g->jump_was_down=jump;
    // The recovered arcade player routine advances on alternate video ticks.
    if (!(g->ticks&1)) {
        unsigned events=neo_vehicle_step(&g->vehicle);g->vehicle_events|=events;
        if (events&NEO_VEHICLE_STAMP)
            g->wreckage_cells=neo_collision_vehicle_wreckage(g->vehicle.x,g->vehicle.y,g->vehicle.flags);
        if (g->jump_pending && g->grounded) {
            g->grounded=false;g->velocity_y=-NEO_JUMP_VY;g->air_ticks=0;
        }
        g->jump_pending=false;
        neo_air_t body={g->x,56*MSX_UNIT-g->y,g->velocity_x,-g->velocity_y};
        if (g->grounded) {
            g->grounded=neo_walk_step(&body,direction);
            if (!g->grounded) g->air_ticks=0;
        } else {
            int previous=body.y;
            neo_air_step(&body,direction);
            g->air_ticks+=2;
            g->grounded=neo_land_step(&body,previous);
        }
        g->x=clamp(body.x,(g->camera+28)*MSX_UNIT,MSX_END_X);
        if (g->x!=body.x) body.vx=0;
        g->y=56*MSX_UNIT-body.y;g->velocity_x=body.vx;g->velocity_y=-body.vy;
    }
    int player=g->x/MSX_UNIT;
    if (player>=640 && g->kills>=1) g->checkpoint=640;
    if (g->cooldown) g->cooldown--;
    if (g->shooting && ++g->shot_age>=8) g->shooting=false;
    if (fire && !g->cooldown) for (unsigned i=0;i<MSX_BULLETS;i++) if (!g->bullets[i].active) {
        g->bullets[i]=(msx_bullet_t){player+g->facing*24,168+g->y/MSX_UNIT,g->facing,true};
        g->cooldown=8;g->shot_age=0;g->shooting=true;break;
    }
    if (g->world_enabled && !(g->ticks&1)) {
        neo_world_player_t p=player_frame(g);
        g->world.video_phase=(uint16_t)g->ticks;
        if (neo_world_tick(&g->world,-(int32_t)((uint32_t)g->camera<<16),&p))
            msx_game_hurt(g,g->camera+(int)(int16_t)g->world.hit_x);
        g->kills=g->world.kills;
    }
    for (unsigned i=0;i<MSX_BULLETS;i++) {
        msx_bullet_t *b=&g->bullets[i];if (!b->active) continue;
        int previous=b->x;b->x+=b->direction*6;
        if (b->y>=192+msx_ground_offset(b->x*MSX_UNIT)/MSX_UNIT) b->active=false;
        else if (g->world_enabled && neo_world_shot(&g->world,previous*MSX_UNIT,b->x*MSX_UNIT,
            (MSX_SCREEN_BASE_Y-b->y)*MSX_UNIT,b->direction<0)) {
            b->active=false;g->hits++;
        } else if (neo_vehicle_hit_segment(&g->vehicle,previous*MSX_UNIT,b->x*MSX_UNIT,
            (248-b->y)*MSX_UNIT,4*MSX_UNIT,2*MSX_UNIT)) {
            b->active=false;neo_vehicle_damage(&g->vehicle,10);g->vehicle_flash=6;
        } else if (b->x<g->camera-32 || b->x>g->camera+352) b->active=false;
    }
    if (neo_vehicle_hit_segment(&g->vehicle,g->x,g->x,73*MSX_UNIT-g->y,9*MSX_UNIT,17*MSX_UNIT))
        msx_game_hurt(g,g->vehicle.x/MSX_UNIT);
    int camera=clamp(g->x/MSX_UNIT-144,0,MSX_WORLD_WIDTH-320);
    if (camera>g->camera) g->camera=camera;
    // Region end: the script has passed its "all groups cleared" waits
    // (ROM 2D650E/2D6514) and the player stands at the native world limit.
    if (g->world_enabled && neo_world_region_clear(&g->world) && g->x>=MSX_END_X && g->grounded) {
        g->complete=true;g->moving=false;g->shooting=false;
        memset(g->bullets,0,sizeof(g->bullets));
    }
}
