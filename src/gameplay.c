#include "gameplay.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "neo_motion.h"
#include "neo_collision.h"
#include "mission_camera.h"

static int clamp(int x,int lo,int hi) { return x<lo?lo:x>hi?hi:x; }
static int right_limit(const msx_game_t *g);
static void ride_on(msx_game_t *g,uint8_t object,int32_t x,int32_t y);
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
    g->facing=1;g->grounded=true;g->health=1;
    // The original region script starts at its first record with the RNG
    // ring the mission handoff left (data/soldier_ai_tables.json).
    neo_world_init(&g->world,-(int32_t)((uint32_t)g->camera<<16));g->world_enabled=true;
    /* Mission start: timer 003586 (DIP index 0 of 10FD8C: 60, as in the
     * captures), lives 10FD88 = 3 (0C9406), one taken by the first spawn
     * (0C95E6). */
    neo_hud_init(&g->hud,(uint16_t)(neo_hud_timer_start(MSX_HUD_TIMER_DIP)<<8|0xff));
    g->hud.panel[0].f84=3;neo_hud_life_lost(&g->hud,0);
}
/* One update of the original HUD with the port's state (neo_hud.c): single
 * player MVS game with no credit (P2 panel "INSERT COIN", "CREDIT 0"), no
 * vehicle gauge (no native vehicle binds player 1's slot yet), timer running while the
 * mission is in play. */
static void hud_step(msx_game_t *g) {
    if (g->world.timer_resets!=g->timer_resets_seen) {g->timer_resets_seen=g->world.timer_resets;neo_hud_timer_reset(&g->hud,MSX_HUD_TIMER_DIP);}
    neo_hud_input_t in={0};
    in.hud_on=true;in.join_mask=3;in.mvs=true;in.credit_text=true;in.credits=0;
    /* 0C071C/0C0764 read player 1's vehicle slot 10C608 (ARMS infinite and
     * the vehicle's shells while riding). */
    in.arms=neo_hud_arms(g->world.weapons.b,g->world.vslots.slot[0].b,0,false,0);
    neo_hud_bombs(g->world.weapons.b,g->world.vslots.slot[0].b,0,0,0,&in.bombs,&in.bomb_type);
    memcpy(in.score,g->world.pickup.score[0],4);in.rescued=g->world.ga_flags.rescued[0];
    in.video=(uint16_t)(g->ticks>>1);
    in.timer_running=!g->complete && !g->mission_complete && !g->awaiting_continue;
    /* Continue (docs/MISION1_COMPLETA.md, game over): DIP 10FD8A on, free play
     * (the home cartridge's AES path of 0C987A: a continue is always offered,
     * Start takes it), lives DIP 3, B of the arcade (the port's jump) speeds
     * the countdown (0187B0). */
    in.continue_dip=true;in.can_continue=true;in.credit_count=0;in.lives_dip=3;
    in.start=g->start_edge;in.speedup=g->jump_edge;in.no_life=g->no_life;g->no_life=false;
    neo_hud_frame(&g->hud,&in);
    if (g->hud.continued) {
        memcpy(g->world.pickup.score[0],g->hud.score,4);g->awaiting_continue=false;g->continues++;
    }
    g->start_edge=g->jump_edge=false;
    if (g->hud.panel[0].state==NEO_PANEL_GAME_OVER && g->game_over_frames<0xffff) g->game_over_frames++;
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
/* Root of the projected words (the scene root 10E968 of player_frame). */
static void root_xy(const msx_game_t *g,int32_t *rx,int32_t *ry) {
    *rx=-(int32_t)((uint32_t)g->camera<<16);*ry=MSX_PROJECTED_BASE_Y*MSX_UNIT;
    if (g->world.camera_native) {*rx=neo_camera_root_x(&g->world.camera);*ry=neo_camera_root_y(&g->world.camera);}
}
static void start_death(msx_game_t *g,uint8_t kind,bool right) {
    int32_t vx=g->velocity_x,vy=-g->velocity_y;int facing=g->facing;
    if (!marco_death_start(&g->life,kind,g->grounded,right,&vx,&vy,&facing)) g->death_stops++;
    if (kind!=2) {g->grounded=false;g->ride=0;g->air_ticks=0;}      /* 0C2E7E */
    g->velocity_x=vx;g->velocity_y=-vy;g->facing=facing;
    g->deaths++;g->moving=g->shooting=false;g->jump_pending=false;
    marco_hgun_reset(&g->hgun);
}
void msx_game_hurt(msx_game_t *g,int from) {
    if (g->invulnerable || g->complete || MSX_REFERENCE_INVULNERABLE || g->life.phase!=MARCO_ALIVE) return;
    /* 0C2956 with the update's +5A/+5B (neo_world.c player_hit). */
    if (g->world_enabled) start_death(g,g->world.hit_kind,g->world.hit_right);
    else start_death(g,0,g->x/MSX_UNIT<from);
}
/* Floor 0146EE: 013DB2 from the feet down 256 px, terrain or a platform. */
static bool respawn_floor(msx_game_t *g,int32_t x,int32_t *y) {
    if (g->world_enabled) {
        neo_kscene_t *k=&g->world.kernel;k->self_dyn=0;k->self_block=0;
        neo_kfloor_t f;
        if (neo_kernel_floor(k,x,*y,NEO_KOBJ_TERRAIN,-256*MSX_UNIT,0xff,0,0,&f) && f.found) {
            if (f.parent>=NEO_KOBJ_DYN0) {
                const neo_kdyn_t *d=&k->dyn[f.parent-NEO_KOBJ_DYN0];
                if (d->map) {*y=(int32_t)((uint32_t)f.y+(uint32_t)d->pos.y-(uint32_t)k->terrain.y);ride_on(g,f.parent,x,*y);return true;}
            } else {*y=f.y;return true;}
        }
    }
    neo_surface_t s;
    if (neo_surface_at(x,*y,256*MSX_UNIT,256*MSX_UNIT,&s) && s.height<=*y) {*y=s.height;return true;}
    return false;
}
/* The panel's new life (0C95AE): a life less (0C95E6; none left is the game
 * over 0C9848, not translated: explicit stop, the port goes on with 0), the
 * timer back to its start (019124), the weapons of a new life (0C2FA4 at
 * the end, 0C0E6E/0C0EAA), the respawn point 0196E8 and the type-0
 * entrance 0C1438 (floor 0146EE, program 0BF498, +84 = 0x78). */
static void new_life(msx_game_t *g) {
    /* No life left: the panel's game over 0C9848 (neo_hud.c) runs in the
     * next HUD update; the player stays gone until its continue. */
    if (!neo_hud_life_lost(&g->hud,0)) {g->hud_game_over++;g->no_life=true;g->awaiting_continue=true;return;}
    neo_hud_timer_reset(&g->hud,MSX_HUD_TIMER_DIP);
    marco_weapons_death(&g->world.weapons,0);marco_weapons_life(&g->world.weapons,0,&g->world.weapon_player);
    memset(g->bullets,0,sizeof(g->bullets));
    g->velocity_x=g->velocity_y=0;g->facing=1;g->ride=0;g->air_ticks=0;   /* 0C10FE: facing right */
    int32_t rx,ry;root_xy(g,&rx,&ry);
    int32_t x,y;
    if (g->world_enabled && g->world.camera_native) {
        marco_respawn_t r=marco_respawn_point(neo_camera_progress(&g->world.camera),rx,ry);
        x=r.x;y=r.y;
    } else {
        /* Without the native camera (lab builds): the port's checkpoint. */
        x=g->checkpoint?g->checkpoint*MSX_UNIT:MSX_START_X;
        g->camera=clamp(x/MSX_UNIT-144,0,MSX_WORLD_WIDTH-320);y=64*MSX_UNIT;
    }
    g->grounded=respawn_floor(g,x,&y);
    g->x=x;g->y=56*MSX_UNIT-y;
    marco_entrance_start(&g->life);g->invulnerable=2*0x78;
}
/* Projected words of the player as the original registers them (+3C/+40):
 * world position plus the scene root (RAM 10E97C/10E980) of the native
 * camera, or the integer camera at the base height without it. */
static neo_world_player_t player_frame(const msx_game_t *g) {
    int32_t rx=-(int32_t)((uint32_t)g->camera<<16),ry=MSX_PROJECTED_BASE_Y*MSX_UNIT;
    if (g->world.camera_native) {rx=neo_camera_root_x(&g->world.camera);ry=neo_camera_root_y(&g->world.camera);}
    neo_world_player_t p={(uint16_t)((uint32_t)(g->x+rx)>>16),(uint16_t)((uint32_t)(56*MSX_UNIT-g->y+ry)>>16),
        (uint8_t)((g->facing<0?1u:0u)|(g->grounded?64u:0u)),true,false,0xff,0xff,false,false};
    /* Dying: +5F bit 7. The dying player also clears its 10C33C bit and is
     * no actor for the update between its end (0C2F96) and the new life;
     * the soldiers' and items' paths without an active player (the
     * soldiers' 0AF6C0.. states loop) are not translated, so `inactive`
     * stays clear and `alive` set: explicit deviation (docs/JEFE_MISION1.md). */
    p.dead=g->life.phase==MARCO_DYING || g->life.phase==MARCO_LYING;
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
/* 0C0D64 for the H bullet of a fire callback (marco_hgun.c). */
static bool hgun_reserve(void *context) {
    msx_game_t *g=context;
    marco_ammo_t ammo=marco_weapons_ammo(&g->world.weapons,0);
    g->hgun_kind=ammo.kind;
    bool allowed=marco_ammo_reserve(&ammo,ammo.kind,false,false);
    marco_weapons_set_ammo(&g->world.weapons,0,&ammo);
    return allowed;
}
/* Mission end of the mission actor 101364 (0x018846..0x01893A), one step
 * per original update, from the update after 018F40 (step 2):
 *   018846  10E9E3 = FF (c8cd6), music 15418, +7C = 0x1E; 018870 waits
 *           (subq/bne: 30 updates).
 *   018884  cae6c(mission) and the prisoner tally actor 0308C0 (it adds the
 *           rescue bonus to 10C46A and sets the mission actor's +60):
 *           not ported, explicit stops; the port goes on at once.
 *   0188BE  sounds 0x20 and 0x1137, +7C = 0xA0, the MISSION COMPLETE letters
 *           actor 06090A (not ported: explicit stop; actor.c draws a text
 *           stand-in); 0188F4 waits (subq/bcc: 161 updates).
 *   0188FC  fade 9C0A (E1,E1,E1, 0x20; not ported: explicit stop), +7C =
 *           0x20; 018916 waits 33 updates.
 *   01891E  mission 42F4 + 1 < 6: the next mission (1829E). Missions 2..6
 *           are not ported: mission_end = done, actor.c restarts Mission 1.
 * MAME runJ: 018870 at 13710, 0188B0 13770, 0188F4 <=14040, 018916 14370,
 * Mission 2 at 14460 (dumps every 30 frames). */
static void mission_end_step(msx_game_t *g) {
    switch (g->mission_end) {
    case MSX_END_NONE:
        g->mission_end=MSX_END_WAIT;g->end_timer=0x1e;
        __attribute__((fallthrough)); /* 018864 -> 018870 in the same update */
    case MSX_END_WAIT:
        if (--g->end_timer) return;
        g->end_stops+=2;                       /* cae6c, tally actor 0308C0 */
        g->end_timer=0xa0;g->end_stops++;      /* letters actor 06090A */
        g->mission_end=MSX_END_LETTERS;
        __attribute__((fallthrough)); /* 0188EE -> 0188F4 */
    case MSX_END_LETTERS:
        if (g->end_timer--) return;
        g->end_stops++;                        /* fade 9C0A */
        g->end_timer=0x20;g->mission_end=MSX_END_FADE;
        __attribute__((fallthrough)); /* 018910 -> 018916 */
    case MSX_END_FADE:
        if (g->end_timer--) return;
        g->mission_end=MSX_END_DONE;
        return;
    default: return;
    }
}
void msx_game_update(msx_game_t *g,int direction,bool jump,bool fire) {
    if (g->complete) {
        g->ticks++;
        if (!(g->ticks&1) && g->end_started) mission_end_step(g);
        g->end_started=true;
        return;
    }
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
    /* Dying, gone or entering: no control (0C2E8A..0C2F12, 0C149E). */
    bool dying=g->life.phase==MARCO_DYING || g->life.phase==MARCO_LYING;
    if (g->life.phase!=MARCO_ALIVE) {direction=0;jump=false;fire=false;g->grenade=false;g->moving=false;g->jump_pending=false;}
    if (g->life.phase==MARCO_ENTERING) g->invulnerable=2*0x78;
    if (g->life.phase==MARCO_GONE && !(g->ticks&1) && !g->awaiting_continue) new_life(g);
    // The recovered arcade player routine advances on alternate video ticks.
    if (!(g->ticks&1) && g->life.phase!=MARCO_GONE) {
        unsigned events=neo_vehicle_step(&g->vehicle);g->vehicle_events|=events;
        if (events&NEO_VEHICLE_STAMP)
            g->wreckage_cells=neo_collision_vehicle_wreckage(g->vehicle.x,g->vehicle.y,g->vehicle.flags);
        if (g->jump_pending && g->grounded) {
            g->grounded=false;g->velocity_y=-NEO_JUMP_VY;g->air_ticks=0;g->ride=0;g->jump_moving=direction!=0;
        }
        g->jump_pending=false;
        neo_air_t body={g->x,56*MSX_UNIT-g->y,g->velocity_x,-g->velocity_y};
        if (dying) {
            /* 0C2E94..0C2EAA / 0C2F20: the death's own velocities, then the
             * air step 0143EE (no gravity of its own) or the ground step. */
            marco_death_velocity(&g->life,g->grounded,&body.vx,&body.vy);
            if (g->life.phase==MARCO_LYING && g->grounded) body.vy=0;
        }
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
            if (!dying) neo_air_accel(&body,direction);
            solid_push_t s=solid_push(g,&body,body.vx,body.vy);
            if (s.top && !dying) {
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
        if (dying) {
            /* 0C2EC6..0C2F8A: speed, program, landing, count-down, off-screen. */
            int32_t rx,ry;root_xy(g,&rx,&ry);
            int32_t vx=g->velocity_x;
            marco_death_after(&g->life,g->grounded,&vx,(int16_t)((uint32_t)(g->x+rx)>>16),(int16_t)((uint32_t)(56*MSX_UNIT-g->y+ry)>>16));
            g->velocity_x=vx;
            if (g->grounded) g->velocity_y=0;
        } else if (g->life.phase==MARCO_ENTERING) marco_entrance_step(&g->life);
    }
    int player=g->x/MSX_UNIT;
    if (player>=640 && g->kills>=1) g->checkpoint=640;
    if (g->cooldown) g->cooldown--;   /* unused by the pistol since the press semantics */
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
                g->grenades++;marco_hgun_throw(&g->hgun);
            } else g->grenade_stops++;
        }
        g->grenade_was_down=g->grenade;
    }
    /* The H (ammo kinds 5/10 with +93 != 2): the original upper-body fire
     * states (marco_hgun.c) on the player's updates; the fire press is +9B
     * bit 4. The bullets 01B6DC/01B716 take the callbacks' angles (0C4244
     * with the fan, or the 0C453A sweep steps). Muzzle: the port has no
     * +72/+74 (offset of the upper body), so the 0C4244/0C453A offsets are
     * applied to the port's pistol muzzles: forward (24,80), up (3,108),
     * linearly between them for a diagonal (declared approximation). */
    bool fire_edge=fire && !g->fire_was;g->fire_was=fire;
    if (fire_edge && !g->complete) g->fire_request=true;   /* +9B bit 4 */
    bool hgun=false;
    if (g->world_enabled && g->world.weapon_player.form93!=2) {
        uint8_t kind=marco_weapons_ammo(&g->world.weapons,0).kind;hgun=kind==5 || kind==10;
    }
    if (!hgun) marco_hgun_reset(&g->hgun);
    /* The body program's +72/+74 of this update, before the upper body
     * (00427A then 0C3778) reads them for the muzzle. */
    if (!(g->ticks&1)) marco_legs_step(&g->legs,g->grounded,g->moving,g->grounded && g->crouching,g->jump_moving,hgun,!g->grounded && g->velocity_y<0);
    if (hgun) {
        if (fire_edge && !g->complete) marco_hgun_press(&g->hgun);
        g->fire_request=false;g->pistol_wait=0;
        if (!(g->ticks&1)) {
            marco_weapon_player_t *m=&g->world.weapon_player;
            g->hgun.combo95=m->combo95;
            marco_hgun_input_t in={g->aim_up,false,g->facing<0};
            marco_hgun_shot_t shots[4];
            unsigned n=g->complete?0:marco_hgun_step(&g->hgun,&in,hgun_reserve,g,shots,4);
            m->combo95=g->hgun.combo95;
            for (unsigned k=0;k<n;k++) {
                /* One muzzle per aim (0C4244: table 0C4204 row by +92, form
                 * 1: level (24,4), up (0,48), down (-4,-20); 0C453A sweeps:
                 * 0C4234 form 1 (16,16)), facing-mirrored in x, plus +72/+74
                 * of the body program's pose (marco_legs.c; hold2/hfam: idle
                 * (24,21), walking (23,21..25), running jump (20..23,15..16)).
                 * The fan (angle +-2) and the sweep angles diverge from that
                 * single point. */
                uint8_t a=shots[k].angle;
                int base=g->facing<0?(0x80-(int)shots[k].base)&0xff:shots[k].base;
                marco_muzzle_t m=shots[k].sweep?marco_muzzle_sweep(&g->legs,a,ga_sine(a),ga_cosine(a)):
                    marco_muzzle_aim(&g->legs,base==0x40?1:base==0xc0?-1:0,1,g->facing<0);
                int32_t x=g->x+m.x*MSX_UNIT;
                int32_t y=(56+m.y)*MSX_UNIT-g->y;
                neo_world_player_shot(&g->world,g->hgun_kind==5?0x1b6dcu:0x1b716u,x,y,a,0);
                g->hgun_diagonal+=shots[k].sweep;
                g->shot_age=0;g->shooting=true;g->shots++;g->weapon_shots++;g->h_shots++;
            }
        }
    }
    if (!hgun) {
        /* The bullet of the callback two ticks (one update) ago: its
         * constructor (01B782) runs one update after 00411C allocated it. */
        if (g->pistol_pending && !--g->pistol_pending)
            for (unsigned i=0;i<MSX_BULLETS;i++) if (!g->bullets[i].active) {g->bullets[i]=g->pistol_next;break;}
        if (g->pistol_wait) g->pistol_wait--;
        /* 0C3CD0: a new press (+9B bit 4) once the previous program wrote
         * +76; the pistol never fires while the button is merely held (hold2:
         * one 01B782 per press, 2191/2491/2791/3091/3391). */
        if (g->fire_request && !g->pistol_wait && !g->complete) {
            g->fire_request=false;
            /* 0C0D64 for the current weapon (it may consume a round and still
             * refuse at the active-projectile cap). */
            marco_ammo_t ammo=marco_weapons_ammo(&g->world.weapons,0);
            bool allowed=marco_ammo_reserve(&ammo,ammo.kind,false,false);
            marco_weapons_set_ammo(&g->world.weapons,0,&ammo);
            bool vertical=g->aim_up || g->crouching;
            g->pistol_wait=vertical?6u:4u;   /* ticks: 3 or 2 updates */
            if (allowed) {
                if (ammo.kind) {
                    static const uint32_t constructor[MARCO_WEAPON_IDS]={0x1b782,0xc5508,0x644c4,0x6381c,0xa71f8,0x1b6dc,0xc46d6,0x646b0,
                        0x641ec,0xa716e,0x1b716,0xc46d6,0x63f32,0x8606a,0xde4da,0xdea20};
                    /* 0C44C2/0C44E0 (+93 = 2: 01B668/01BDD2) are not ported either. */
                    g->weapon_stops++;g->weapon_stop_constructor=g->world.weapon_player.form93==2 && (ammo.kind==5 || ammo.kind==10)?
                        (ammo.kind==5?0x1b668u:0x1bdd2u):constructor[ammo.kind&15u];
                }
                /* 0C4244 muzzle: table 0C4204 row by aim, form 0, plus the
                 * body program's +72/+74 (hold2: standing level (16,10)+(0,17)
                 * = (16,27), up (0,40)+(0,17) = (0,57), crouching (16,16)). */
                marco_muzzle_t m=marco_muzzle_aim(&g->legs,g->aim_up?1:0,0,g->facing<0);
                int mx=g->facing*m.x,my=m.y;
                g->pistol_next=(msx_bullet_t){player+g->facing*mx,192-my+g->y/MSX_UNIT,g->facing,true,g->aim_up,ammo.kind};
                g->pistol_pending=2;
                g->shot_age=0;g->shooting=true;g->shots++;g->weapon_shots+=ammo.kind!=0;
            }
        }
    }
    if (g->world_enabled && !(g->ticks&1)) {
        neo_world_player_t p=player_frame(g);
        camera_step(g,&p);
        g->world.video_phase=(uint16_t)g->ticks;
        g->world.scroll_y=neo_camera_root_y(&g->world.camera);
        if (neo_world_tick(&g->world,neo_camera_root_x(&g->world.camera),&p))
            msx_game_hurt(g,g->camera+(int)(int16_t)g->world.hit_x);
        /* Time over (0186FA -> 0C0A2C): +9E = FF, +5A = 6, a death however
         * invulnerable (not in the reference cartridge). */
        if (!g->hud.time_over) g->time_over_seen=false;
        else if (!g->time_over_seen && g->life.phase==MARCO_ALIVE && !g->complete && !MSX_REFERENCE_INVULNERABLE) {
            g->time_over_seen=true;g->time_overs++;start_death(g,6,g->world.hit_right);
        }
        g->kills=g->world.kills;
        if (!g->region_end && neo_world_region_clear(&g->world)) g->region_end=true;
        if (g->world.mission_step>=1) g->mission_complete=true;
        if (g->world.mission_step>=2) {g->complete=true;g->moving=false;g->shooting=false;memset(g->bullets,0,sizeof(g->bullets));}
        hud_step(g);
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
