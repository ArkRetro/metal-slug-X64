#include "gameplay.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "neo_motion.h"
#include "neo_collision.h"
#include "mission_camera.h"

static int clamp(int x,int lo,int hi) { return x<lo?lo:x>hi?hi:x; }
static int right_limit(const msx_game_t *g);
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
/* Reference-only traversal build (mslugx-reference.z64, never the release):
 * the player ignores hits so that a scripted run reaches the mission end. */
#ifndef MSX_REFERENCE_INVULNERABLE
#define MSX_REFERENCE_INVULNERABLE 0
#endif
const bool msx_reference_invulnerable=MSX_REFERENCE_INVULNERABLE!=0;
const uint32_t msx_game_layout=MSX_GAME_LAYOUT;
void msx_game_hurt(msx_game_t *g,int from) {
    if (g->invulnerable || g->complete || MSX_REFERENCE_INVULNERABLE) return;
    g->health--;g->invulnerable=60;
    g->x=clamp(g->x+(g->x/MSX_UNIT<from?-20:20)*MSX_UNIT,(g->camera+28)*MSX_UNIT,right_limit(g));
    if (g->grounded) {
        neo_surface_t support;
        if (neo_surface_at(g->x,56*MSX_UNIT-g->y,8*MSX_UNIT,8*MSX_UNIT,&support))
            g->y=56*MSX_UNIT-support.height;
        else {g->grounded=false;g->air_ticks=0;}
    }
    if (!g->health) {
        /* 0C2FA4 (the dead player gone) and 0C0E6E (the new life). */
        marco_weapons_death(&g->world.weapons,0);marco_weapons_life(&g->world.weapons,0,&g->world.weapon_player);
        g->deaths++;g->health=3;g->velocity_x=g->velocity_y=0;g->invulnerable=90;
        memset(g->bullets,0,sizeof(g->bullets));
        if (g->world_enabled && g->world.camera_native) {
            // The mission camera never scrolls back: respawn inside the
            // current view, dropping onto the nearest floor below.
            g->x=clamp(g->x,(g->camera+64)*MSX_UNIT,(g->camera+256)*MSX_UNIT);
            g->y=56*MSX_UNIT-(g->camera_height+200)*MSX_UNIT;g->grounded=false;g->air_ticks=0;
            return;
        }
        g->x=g->checkpoint?g->checkpoint*MSX_UNIT:MSX_START_X;
        g->camera=clamp(g->x/MSX_UNIT-144,0,MSX_WORLD_WIDTH-320);
        g->y=msx_ground_offset(g->x);g->grounded=true;
    }
}
/* Projected words of the player as the original registers them (+3C/+40):
 * world position plus the scene root (RAM 10E97C/10E980) of the native
 * camera, or the integer camera at the base height without it. */
static neo_world_player_t player_frame(const msx_game_t *g) {
    int32_t rx=-(int32_t)((uint32_t)g->camera<<16),ry=MSX_PROJECTED_BASE_Y*MSX_UNIT;
    if (g->world.camera_native) {rx=neo_camera_root_x(&g->world.camera);ry=neo_camera_root_y(&g->world.camera);}
    neo_world_player_t p={(uint16_t)((uint32_t)(g->x+rx)>>16),(uint16_t)((uint32_t)(56*MSX_UNIT-g->y+ry)>>16),
        (uint8_t)((g->facing<0?1u:0u)|(g->grounded?64u:0u)),true,false,0xff,0xff,false,false};
    return p;
}
void msx_game_projected(const msx_game_t *g,uint16_t *x,uint16_t *y) {
    neo_world_player_t p=player_frame(g);*x=p.px;*y=p.py;
}
/* Mission actor camera step (neo_world_camera) and its command-list events. */
static void camera_step(msx_game_t *g,const neo_world_player_t *p) {
    neo_world_camera(&g->world,p);
    const neo_camera_t *c=&g->world.camera;
    for (unsigned i=0;i<c->event_count;i++)
        if (c->events[i].kind==NEO_CAMERA_EV_PALETTE && c->events[i].address==0x0B56CA) g->backdrop_after=true; /* progress 2304 */
    g->camera=(int)(((int64_t)c->x+0xffff)>>16);
    g->camera_height=(int)(((int64_t)c->y+0xffff)>>16);
}
/* Right limit of the player: camera+292-1/65536 (the arcade value behind
 * MSX_END_X), so a script camera lock also stops the player at the screen. */
static int right_limit(const msx_game_t *g) {
    int64_t r=(int64_t)(g->camera+292)*MSX_UNIT-1;
    return r<MSX_END_X?(int)r:MSX_END_X;
}
/* Floor search 013DB2 of the native world's collision kernel from the
 * player's feet (terrain-relative, y up) down `depth` (negative, Q16.16):
 * true when the first floor found is a dynamic object (0140C0 platform);
 * *y gets the world height of its surface. The static terrain stays with
 * neo_collision (the same grid). */
static bool platform_floor(msx_game_t *g,int32_t x,int32_t y,int32_t depth,uint8_t *object,int32_t *top) {
    if (!g->world_enabled) return false;
    neo_kscene_t *k=&g->world.kernel;bool any=false;
    for (unsigned i=0;i<NEO_KDYN_COUNT;i++) any|=k->dyn[i].map!=NULL;
    if (!any) return false;
    k->self_dyn=0;k->self_block=0;
    neo_kfloor_t f;
    if (!neo_kernel_floor(k,x,y,NEO_KOBJ_TERRAIN,depth,0xff,0,0,&f) || !f.found || f.parent<NEO_KOBJ_DYN0) return false;
    const neo_kdyn_t *d=&k->dyn[f.parent-NEO_KOBJ_DYN0];
    if (!d->map) return false;
    *object=f.parent;*top=(int32_t)((uint32_t)f.y+(uint32_t)d->pos.y-(uint32_t)k->terrain.y);
    return true;
}
static void ride_on(msx_game_t *g,uint8_t object,int32_t x,int32_t y) {
    const neo_kscene_t *k=&g->world.kernel;const neo_kdyn_t *d=&k->dyn[object-NEO_KOBJ_DYN0];
    if (!g->ride) g->rides++;
    g->ride=(uint8_t)(object-NEO_KOBJ_DYN0+1);
    g->ride_x=(int32_t)((uint32_t)x+(uint32_t)k->terrain.x-(uint32_t)d->pos.x);
    g->ride_y=(int32_t)((uint32_t)y+(uint32_t)k->terrain.y-(uint32_t)d->pos.y);
}
/* Push-out 004ED2 from the solid bodies of the table 106756 (registered by
 * 004E76: armoured vehicle 087E36, truck 02916E), called by the player's
 * ground step 014594 (with +30 = ground speed +62) and air step 0143EE
 * before their sweeps. (vx,vy) is the velocity of this update (+30/+34).
 * Per body (descriptor words: kind, half width, x, y, width, height; x
 * mirrored by the owner's +15 bit 0), the player's position predicted
 * relative to the body (its relative velocity added) must lie inside the
 * box; the side is then the half holding the current position: left sets
 * the player's +15 bit 3 and the body's bit 4 (right: player bit 4, body
 * bit 3), and the player moves 1/16 of the way to that edge from where it
 * stands (each hit restarts from there: the last one wins). Kind 1 with
 * the player's current feet at or above the top returns D7 = -1 (top).
 * The nudge is applied directly: the original sweeps it through 013968,
 * which this player model does not use (neo_collision supports it). The
 * truck's descriptor 029BC2 has height 0, so it never pushes. */
typedef struct { bool left,right,top; } solid_push_t;
/* Motion of a +38 parent since the previous update (none for tag 0). */
static bool parent_motion(const neo_world_t *w,uint32_t tag,uint32_t *dx,uint32_t *dy) {
    neo_contact_parent_t p;*dx=*dy=0;
    if (!tag) return true;
    if (!neo_world_parent(w,tag,&p)) return false;
    *dx=(uint32_t)p.x-(uint32_t)p.previous_x;*dy=(uint32_t)p.y-(uint32_t)p.previous_y;
    return true;
}
static solid_push_t solid_push(msx_game_t *g,neo_air_t *body,int32_t vx,int32_t vy) {
    solid_push_t r={false,false,false};
    neo_world_t *w=&g->world;
    if (!g->world_enabled) return r;
    if (w->ga_flags.player_lock) return r;                    /* 0C8CFC: 10E9E3 set, D7 = 0 */
    /* 01ABC8: projected position (+3C/+40), as player_frame. */
    int32_t rx=-(int32_t)((uint32_t)g->camera<<16),ry=MSX_PROJECTED_BASE_Y*MSX_UNIT;
    if (w->camera_native) {rx=neo_camera_root_x(&w->camera);ry=neo_camera_root_y(&w->camera);}
    uint32_t px=(uint32_t)body->x+(uint32_t)rx,py=(uint32_t)body->y+(uint32_t)ry;
    /* Motion of the player's parent (+38): the platform it rides, else the
     * scene root of player_frame. */
    uint32_t pdx,pdy;
    parent_motion(w,g->ride?NEO_DYN_TAG(g->ride-1):0x10e968u,&pdx,&pdy);
    int32_t start=body->x;bool hit=false;
    for (unsigned i=0;i<NEO_WORLD_SOLIDS;i++) {                /* 004EF2 */
        int o=w->solids[i].actor;
        if (o<0) continue;
        neo_gactor_t *a=&w->gactors[o];
        if (ga_b(a,0x6b)!=i) continue;
        const uint8_t *t=ga_rom(w->solids[i].desc);
        uint32_t bdx,bdy;
        if (!t || !parent_motion(w,ga_l(a,0x38),&bdx,&bdy)) {w->solid_unsupported++;continue;}  /* explicit stop: unknown descriptor/parent */
        #define DW(k) ((uint16_t)(t[2*(k)]<<8|t[2*(k)+1]))
        uint32_t d0=px-ga_l(a,0x3c),d1=py-ga_l(a,0x40);
        uint32_t d2=(uint32_t)vx+pdx-ga_l(a,0x30)-bdx+d0,d3=(uint32_t)vy+pdy-ga_l(a,0x34)-bdy+d1;
        uint16_t x0=(uint16_t)(d0>>16),y0=(uint16_t)(d1>>16),x1=(uint16_t)(d2>>16),y1=(uint16_t)(d3>>16);
        uint16_t oy=DW(3),h=DW(5);
        if ((uint16_t)(y1-oy)>=h) continue;                    /* 004F7A */
        int16_t above=(int16_t)(uint16_t)(y0-oy-h);
        uint16_t ox=DW(2),wd=DW(4);
        if (ga_b(a,0x15)&1u) ox=(uint16_t)-(uint16_t)(ox+wd);  /* 004F8A */
        if ((uint16_t)(x1-ox)>=wd) continue;                   /* 004F9C */
        int16_t side=(int16_t)(uint16_t)(x0-ox-DW(1)),push;
        if (side>=0) {                                          /* 004FAC: right half */
            r.right=true;r.left=false;ga_sb(a,0x15,(uint8_t)(ga_b(a,0x15)|8u));
            push=(int16_t)-(int16_t)(uint16_t)(side-DW(1));if (push<0) push=0;
        } else {                                                /* 004FC8: left half */
            r.left=true;r.right=false;ga_sb(a,0x15,(uint8_t)(ga_b(a,0x15)|16u));
            push=(int16_t)-(int16_t)(uint16_t)(side+DW(1));if (push>0) push=0;
        }
        if (above>=0 && DW(0)==1u) r.top=true;                 /* 004FE0 */
        body->x=start+(int32_t)push*(MSX_UNIT/16);             /* 004FF4..00503E */
        hit=true;
        #undef DW
    }
    if (hit) g->solid_pushes++;
    return r;
}
/* One port bullet (pistol model); false once it is gone. */
static bool active_bullet_step(msx_game_t *g,msx_bullet_t *b) {
    if (b->up) {
        int previous_y=b->y;b->y-=6;
        if (b->y<-32) b->active=false;
        else if (g->world_enabled && neo_world_shot_row(&g->world,b->x*MSX_UNIT,(MSX_SCREEN_BASE_Y-previous_y)*MSX_UNIT,
            b->x*MSX_UNIT,(MSX_SCREEN_BASE_Y-b->y)*MSX_UNIT,4,false)) {b->active=false;g->hits++;}
        return b->active;
    }
    int previous=b->x;b->x+=b->direction*6;
    if (b->y>=192+msx_ground_offset(b->x*MSX_UNIT)/MSX_UNIT) b->active=false;
    else if (g->world_enabled && neo_world_shot(&g->world,previous*MSX_UNIT,b->x*MSX_UNIT,
        (MSX_SCREEN_BASE_Y-b->y)*MSX_UNIT,b->direction<0)) {
        b->active=false;g->hits++;
    } else if (neo_vehicle_hit_segment(&g->vehicle,previous*MSX_UNIT,b->x*MSX_UNIT,
        (248-b->y)*MSX_UNIT,4*MSX_UNIT,2*MSX_UNIT)) {
        b->active=false;neo_vehicle_damage(&g->vehicle,10);g->vehicle_flash=6;
    } else if (b->x<g->camera-32 || b->x>g->camera+352) b->active=false;
    return b->active;
}
void msx_game_update(msx_game_t *g,int direction,bool jump,bool fire) {
    if (g->complete) return;
    /* Collapse of the domed building: the players' input handlers are the
     * no-op 054B18 (0CA2E0) until 0CA2F2 restores them. */
    if (g->world_enabled && g->world.ga_flags.player_handlers) {direction=0;jump=false;fire=false;}
    g->crouching=g->crouch && g->grounded && !g->ride;
    if (g->crouching && direction) {g->facing=direction<0?-1:1;direction=0;}  /* turns in place */
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
            g->grounded=false;g->velocity_y=-NEO_JUMP_VY;g->air_ticks=0;g->ride=0;g->jump_moving=direction!=0;
        }
        g->jump_pending=false;
        neo_air_t body={g->x,56*MSX_UNIT-g->y,g->velocity_x,-g->velocity_y};
        if (g->ride) {
            /* Carried: the feet follow the platform object (projected
             * position + relative feet - terrain). */
            const neo_kscene_t *k=&g->world.kernel;const neo_kdyn_t *d=&k->dyn[g->ride-1];
            if (!d->map) {g->ride=0;g->grounded=false;g->air_ticks=0;}
            else {
                body.x=(int32_t)((uint32_t)d->pos.x+(uint32_t)g->ride_x-(uint32_t)k->terrain.x);
                body.y=(int32_t)((uint32_t)d->pos.y+(uint32_t)g->ride_y-(uint32_t)k->terrain.y);
            }
        }
        if (g->ride) {
            /* Walking on the platform (flat top): still supported within
             * the original step tolerance, else the player falls off. */
            neo_ground_velocity(0,direction,&body.vx,&body.vy);body.vy=0;
            solid_push_t s=solid_push(g,&body,body.vx,body.vy);
            if ((s.left && body.vx>=0) || (s.right && body.vx<0)) body.vx=0;   /* 014676..01469C */
            body.x+=body.vx;
            uint8_t object;int32_t top;
            if (platform_floor(g,body.x,body.y+8*MSX_UNIT,-16*MSX_UNIT,&object,&top)) {body.y=top;ride_on(g,object,body.x,body.y);}
            else {g->ride=0;g->grounded=false;g->air_ticks=0;}
        } else if (g->grounded) {
            /* 014594: the push-out sees the ground speed (+62) as +30; the
             * sweep's velocity into the body is cancelled (014676..01469C). */
            neo_surface_t feet;int32_t gvx,gvy;
            neo_ground_velocity(neo_surface_at(body.x,body.y,1,NEO_UNIT,&feet)?feet.shape:0,direction,&gvx,&gvy);
            solid_push_t s=solid_push(g,&body,gvx,body.vy);
            if ((s.left && gvx>=0) || (s.right && gvx<0)) direction=0;
            g->grounded=neo_walk_step(&body,direction);
            if (!g->grounded) g->air_ticks=0;
        } else {
            int previous=body.y;
            neo_air_accel(&body,direction);
            solid_push_t s=solid_push(g,&body,body.vx,body.vy);
            if (s.top) {
                /* 014408: on top of a kind-1 body (player +5F bit 7, dying,
                 * has no equivalent here): thrown up, and away when it was
                 * in the left half; neither side then blocks. */
                body.vy=0x60000;
                if (s.left) {int32_t v=0u-(uint32_t)body.vx;body.vx=(v>=0?0x40000:-0x40000)+(v>>1);}
            } else if ((s.left && body.vx>=0) || (s.right && body.vx<0)) body.vx=0;   /* 014468..01448A */
            body.x+=body.vx;body.y+=body.vy;
            g->air_ticks+=2;
            uint8_t object;int32_t top;
            if (body.y<previous && platform_floor(g,body.x,previous,body.y-previous,&object,&top) && top<=previous && top>=body.y) {
                body.vy=top-previous;body.y=top;g->grounded=true;ride_on(g,object,body.x,body.y);
            } else g->grounded=neo_land_step(&body,previous);
        }
        g->x=clamp(body.x,(g->camera+28)*MSX_UNIT,right_limit(g));
        if (g->x!=body.x) {body.vx=0;if (g->ride) ride_on(g,(uint8_t)(NEO_KOBJ_DYN0+g->ride-1),g->x,body.y);}
        g->y=56*MSX_UNIT-body.y;g->velocity_x=body.vx;g->velocity_y=-body.vy;
    }
    int player=g->x/MSX_UNIT;
    if (player>=640 && g->kills>=1) g->checkpoint=640;
    if (g->cooldown) g->cooldown--;
    if (g->shooting && ++g->shot_age>=8) g->shooting=false;
    if (!(g->ticks&1)) {
        /* The player's weapon fields: the controller takes the requested
         * form +94 into +93 (observed in weapons.csv), then the expiry
         * check 0C337A of the common processing 0C32F4. */
        marco_weapon_player_t *m=&g->world.weapon_player;
        if (m->form94!=0xffu) {m->form93=m->form94;m->form94=0xff;}
        if (marco_weapons_expire(&g->world.weapons,0,m)) g->expiries++;
        /* Bomb button: 0C478E consumes one; bomb type 1 (0C47BA) throws the
         * grenade 01BF46, or 01BF10 with the legs' +90 = FF (observed:
         * standing; 01 running, 03 in the air). Its position (0C4806:
         * the player's +72/+74 words, -1/0x11..0x15 in cfam) uses the
         * standing value -1/0x15: the port's player has no +72/+74. Other
         * bomb types are explicit stops. */
        if (g->grenade && !g->grenade_was_down && !g->complete && marco_weapons_throw(&g->world.weapons,0,false)) {
            uint8_t type=g->world.weapons.b[10];
            if (type==1 && g->world_enabled) {
                bool still=g->grounded && !g->moving;
                neo_world_player_grenade(&g->world,still?0x1bf10u:0x1bf46u,g->x-MSX_UNIT,(56+4+0x15)*MSX_UNIT-g->y,
                    g->velocity_x,g->grounded?0:-g->velocity_y,g->facing<0,0);
                g->grenades++;
            } else g->grenade_stops++;
        }
        g->grenade_was_down=g->grenade;
    }
    if (fire && !g->cooldown) for (unsigned i=0;i<MSX_BULLETS;i++) if (!g->bullets[i].active) {
        /* 0C0D64 for the current weapon (it may consume a round and still
         * refuse at the active-projectile cap). */
        marco_ammo_t ammo=marco_weapons_ammo(&g->world.weapons,0);
        bool allowed=marco_ammo_reserve(&ammo,ammo.kind,false,false);
        marco_weapons_set_ammo(&g->world.weapons,0,&ammo);
        g->cooldown=8;
        if (!allowed) break;
        if ((ammo.kind==5 || ammo.kind==10) && g->world_enabled && g->world.weapon_player.form93!=2) {
            /* 0C4454/0C447C: the H bullet 01B6DC/01B716 (neo_bullet.c);
             * 0C4430 and 0C44A6 count +95 twice, the angle fans by
             * 2*((+95 mod 3)-1). The muzzle (0C4244 table, 014A02) is the
             * port's pistol muzzle: the player model has no +72/+74. */
            marco_weapon_player_t *m=&g->world.weapon_player;
            m->combo95=(uint8_t)(m->combo95+1u);m->combo95=(uint8_t)(m->combo95+1u);
            uint8_t angle=g->aim_up?0x40:(g->facing<0?0x80:0);
            angle=(uint8_t)(angle+2*((int)(m->combo95%3u)-1));
            int32_t x=g->aim_up?g->x+g->facing*3*MSX_UNIT:g->x+g->facing*24*MSX_UNIT;
            int32_t y=(g->aim_up?108:80)*MSX_UNIT-g->y;
            neo_world_player_shot(&g->world,ammo.kind==5?0x1b6dcu:0x1b716u,x,y,angle,0);
            g->shot_age=0;g->shooting=true;g->shots++;g->weapon_shots++;break;
        }
        if (ammo.kind) {
            static const uint32_t constructor[MARCO_WEAPON_IDS]={0x1b782,0xc5508,0x644c4,0x6381c,0xa71f8,0x1b6dc,0xc46d6,0x646b0,
                0x641ec,0xa716e,0x1b716,0xc46d6,0x63f32,0x8606a,0xde4da,0xdea20};
            /* 0C44C2/0C44E0 (+93 = 2: 01B668/01BDD2) are not ported either. */
            g->weapon_stops++;g->weapon_stop_constructor=g->world.weapon_player.form93==2 && (ammo.kind==5 || ammo.kind==10)?
                (ammo.kind==5?0x1b668u:0x1bdd2u):constructor[ammo.kind&15u];
        }
        g->bullets[i]=g->aim_up?(msx_bullet_t){player+g->facing*3,140+g->y/MSX_UNIT,g->facing,true,true,ammo.kind}:
            (msx_bullet_t){player+g->facing*24,168+g->y/MSX_UNIT,g->facing,true,false,ammo.kind};
        g->shot_age=0;g->shooting=true;g->shots++;g->weapon_shots+=ammo.kind!=0;break;
    }
    if (g->world_enabled && !(g->ticks&1)) {
        neo_world_player_t p=player_frame(g);
        camera_step(g,&p);
        g->world.video_phase=(uint16_t)g->ticks;
        g->world.scroll_y=neo_camera_root_y(&g->world.camera);
        if (neo_world_tick(&g->world,neo_camera_root_x(&g->world.camera),&p))
            msx_game_hurt(g,g->camera+(int)(int16_t)g->world.hit_x);
        g->kills=g->world.kills;
        if (!g->region_end && neo_world_region_clear(&g->world)) g->region_end=true;
        if (g->world.mission_step>=1) g->mission_complete=true;
        if (g->world.mission_step>=2) {g->complete=true;g->moving=false;g->shooting=false;memset(g->bullets,0,sizeof(g->bullets));}
    }
    for (unsigned i=0;i<MSX_BULLETS;i++) {
        msx_bullet_t *b=&g->bullets[i];if (!b->active) continue;
        if (!active_bullet_step(g,b)) {
            /* 0C0DFC: a projectile of the current weapon is gone. */
            marco_ammo_t ammo=marco_weapons_ammo(&g->world.weapons,0);
            marco_ammo_release(&ammo,b->kind);marco_weapons_set_ammo(&g->world.weapons,0,&ammo);
        }
    }
    if (neo_vehicle_hit_segment(&g->vehicle,g->x,g->x,73*MSX_UNIT-g->y,9*MSX_UNIT,17*MSX_UNIT))
        msx_game_hurt(g,g->vehicle.x/MSX_UNIT);
    if (!g->world_enabled) {
        // Reference/test runs without the native world: follow the player.
        int camera=clamp(g->x/MSX_UNIT-144,0,MSX_CAMERA_MAX);
        if (camera>g->camera) g->camera=camera;
    }
}
