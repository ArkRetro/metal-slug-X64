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
static void spawn_enemy(msx_game_t *g) {
    // PROVISIONAL authored test encounters. These positions, health and firing
    // rules are NOT the verified Mission-1 soldier AI; see FIRST_SOLDIER.md.
    static const int positions[]={420,780,1140};
    if (g->kills>=3) { g->enemy.alive=false; return; }
    g->enemy=(msx_enemy_t){.x=positions[g->kills],.facing=-1,.health=2,.cooldown=60,.alive=true};
}
void msx_game_init(msx_game_t *g) {
    neo_collision_reset();
    memset(g,0,sizeof(*g));
    g->x=0x78bad9;g->y=msx_ground_offset(g->x);
    g->facing=1;g->grounded=true;g->health=3;spawn_enemy(g);
}
void msx_game_vehicle_reference(msx_game_t *g) {
    // Observed stationary vehicle variant, at the captured position. This is
    // selected only by the internal reference build, not by provisional kills.
    g->x=1020*MSX_UNIT;g->camera=872;g->y=msx_ground_offset(g->x);
    g->facing=-1;g->enemy.alive=false;
    neo_vehicle_init(&g->vehicle,937*MSX_UNIT+MSX_UNIT/2,64*MSX_UNIT,936*MSX_UNIT,0,1,300,9);
}
static void damage(msx_game_t *g,int from) {
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
        g->deaths++;g->health=3;g->x=g->checkpoint?g->checkpoint*MSX_UNIT:0x78bad9;
        g->camera=clamp(g->x/MSX_UNIT-144,0,MSX_WORLD_WIDTH-320);
        g->y=msx_ground_offset(g->x);g->velocity_x=g->velocity_y=0;g->grounded=true;g->invulnerable=90;
        memset(g->bullets,0,sizeof(g->bullets));memset(g->enemy_bullets,0,sizeof(g->enemy_bullets));
        spawn_enemy(g);
    }
}
static bool swept_hit(int previous,int current,int half,int left,int right) {
    int lo=previous<current?previous:current,hi=previous>current?previous:current;
    return hi+half>=left && lo-half<=right;
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
    msx_enemy_t *e=&g->enemy;
    if (e->flash) e->flash--;
    if (!e->alive && e->respawn && --e->respawn==0) spawn_enemy(g);
    if (e->alive && abs(e->x-player)<240) {
        e->facing=player<e->x?-1:1;
        if (abs(e->x-player)>64 && !(g->ticks&1)) e->x+=e->facing;
        if (e->cooldown) e->cooldown--;
        if (!e->cooldown) for (unsigned i=0;i<MSX_ENEMY_BULLETS;i++) if (!g->enemy_bullets[i].active) {
            g->enemy_bullets[i]=(msx_bullet_t){e->x+e->facing*16,168+msx_ground_offset(e->x*MSX_UNIT)/MSX_UNIT,e->facing,true};
            e->cooldown=90;break;
        }
    }
    for (unsigned i=0;i<MSX_BULLETS;i++) {
        msx_bullet_t *b=&g->bullets[i];if (!b->active) continue;
        int previous=b->x;b->x+=b->direction*6;
        int enemy_feet=192+msx_ground_offset(e->x*MSX_UNIT)/MSX_UNIT;
        if (b->y>=192+msx_ground_offset(b->x*MSX_UNIT)/MSX_UNIT) b->active=false;
        else if (e->alive && swept_hit(previous,b->x,4,e->x-10,e->x+10) && b->y+2>=enemy_feet-38 && b->y<=enemy_feet) {
            b->active=false;g->hits++;e->flash=10;
            if (--e->health==0) { e->alive=false;e->respawn=g->kills<2?90:0;g->kills++; }
        } else if (neo_vehicle_hit_segment(&g->vehicle,previous*MSX_UNIT,b->x*MSX_UNIT,
            (248-b->y)*MSX_UNIT,4*MSX_UNIT,2*MSX_UNIT)) {
            b->active=false;neo_vehicle_damage(&g->vehicle,10);g->vehicle_flash=6;
        } else if (b->x<g->camera-32 || b->x>g->camera+352) b->active=false;
    }
    for (unsigned i=0;i<MSX_ENEMY_BULLETS;i++) {
        msx_bullet_t *b=&g->enemy_bullets[i];if (!b->active) continue;
        int previous=b->x;b->x+=b->direction*4;
        int feet=192+g->y/MSX_UNIT;
        if (b->y>=192+msx_ground_offset(b->x*MSX_UNIT)/MSX_UNIT) b->active=false;
        else if (swept_hit(previous,b->x,3,g->x/MSX_UNIT-9,g->x/MSX_UNIT+9) && b->y+2>=feet-34 && b->y<=feet) {
            b->active=false;damage(g,b->x-b->direction*4);
        } else if (b->x<g->camera-32 || b->x>g->camera+352) b->active=false;
    }
    if (e->alive && abs(e->x-g->x/MSX_UNIT)<18 && g->y>msx_ground_offset(g->x)-28*MSX_UNIT) damage(g,e->x);
    if (neo_vehicle_hit_segment(&g->vehicle,g->x,g->x,73*MSX_UNIT-g->y,9*MSX_UNIT,17*MSX_UNIT))
        damage(g,g->vehicle.x/MSX_UNIT);
    int camera=clamp(g->x/MSX_UNIT-144,0,MSX_WORLD_WIDTH-320);
    if (camera>g->camera) g->camera=camera;
    if (g->kills==3 && g->x>=MSX_END_X && g->grounded) {
        g->complete=true;g->moving=false;g->shooting=false;
        memset(g->bullets,0,sizeof(g->bullets));memset(g->enemy_bullets,0,sizeof(g->enemy_bullets));
    }
}
