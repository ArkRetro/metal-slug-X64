#include "gameplay.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "neo_motion.h"
#include "neo_collision.h"
#include "mission_camera.h"
#include "marco_mount.h"
#include "marco_descent.h"
/* Reference-only traversal build (mslugx-reference.z64, never the release):
 * the player ignores hits so that a scripted run reaches the mission end. */
#ifndef MSX_REFERENCE_INVULNERABLE
#define MSX_REFERENCE_INVULNERABLE 0
#endif
/* Reference build only: the stage end of a mission pack stage once the
 * camera holds at the end of its list and the player reaches the right
 * edge (the stage-1 exit actor 0B6F24 is not ported, docs/MISION2.md). */
#ifndef MSX_REFERENCE_STAGE_EXIT
#define MSX_REFERENCE_STAGE_EXIT MSX_REFERENCE_INVULNERABLE
#endif
#ifndef MSX_DEBUG_CAMEL_DROP
#define MSX_DEBUG_CAMEL_DROP 0
#endif
/* Screenshot cartridge only (never a release): the R (id 3) granted as an
 * item would (marco_weapons_grant, 08B5BC) once Marco has landed, and
 * again whenever it runs out. */
#ifndef MSX_DEBUG_GIVE_R
#define MSX_DEBUG_GIVE_R 0
#endif

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
    /* 01829E for Mission 1 (record 01714E): the port's entry runs the
     * stage start, so the flow only keeps its words (docs/MISION2.md). */
    neo_mission_start(&g->flow,0);
    for (unsigned k=0;k<40 && g->flow.state!=NEO_MISSION_PLAYING;k++) neo_mission_update(&g->flow);
    g->x=MSX_START_X;g->y=msx_ground_offset(g->x);
    g->facing=1;g->grounded=true;g->health=1;
    // The original region script starts at its first record with the RNG
    // ring the mission handoff left (data/soldier_ai_tables.json).
    neo_world_init(&g->world,-(int32_t)((uint32_t)g->camera<<16));g->world_enabled=true;
    neo_continue_start(&g->continue_sprites,neo_world_program);       /* 01868A */
    /* Reference traversal build only: the original's debug flag 10F000 bit 7
     * (a destroyed vehicle keeps its rider), as the player ignores hits. */
    g->world.dip10f000=MSX_REFERENCE_INVULNERABLE?0x80u:0u;
    /* Mission start: timer 003586 (DIP index 0 of 10FD8C: 60, as in the
     * captures), lives 10FD88 = 3 (0C9406), one taken by the first spawn
     * (0C95E6). */
    neo_hud_init(&g->hud,(uint16_t)(neo_hud_timer_start(MSX_HUD_TIMER_DIP)<<8|0xff));
    g->hud.panel[0].f84=3;neo_hud_life_lost(&g->hud,0);
}
/* One update of the original HUD with the port's state (neo_hud.c): single
 * player game on the home cartridge path (docs/FIN_PARTIDA.md, "HUD sin
 * monedas"): RAM 10FD82 = 0 (AES), so the idle P2 panel 0C9380 asks 01A8AA
 * for the home credits of RAM 100004 (0x0505 from 0024F4) and shows PRESS
 * START, and the continue takes the AES branch of 0C987A. The home credits
 * are never spent in the port (deviation: the cartridge's own count of 5 is
 * not modelled). The credit counter 01A7F4 needs the soft-DIP bit 1 of
 * 10FD91, which only the MVS BIOS sets: off (no CREDIT text). No vehicle
 * gauge (no native vehicle binds player 1's slot yet), timer running while
 * the mission is in play. */
static void hud_step(msx_game_t *g) {
    if (g->world.timer_resets!=g->timer_resets_seen) {g->timer_resets_seen=g->world.timer_resets;neo_hud_timer_reset(&g->hud,MSX_HUD_TIMER_DIP);}
    neo_hud_input_t in={0};
    in.hud_on=true;in.join_mask=3;in.mvs=false;in.credits_available=true;in.credit_text=false;in.credits=0;
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
    /* 01883C: the mission actor's 00427A while the countdown runs (012022:
     * the front-layer test reads 0 in the go2 capture, as at the mission end). */
    if (g->hud.continue_draw) neo_continue_draw(&g->continue_sprites,g->hud.continue_timer,false,neo_world_program);
    else neo_continue_clear(&g->continue_sprites);
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
    g->landing_pending=true;   /* the entry ends at the parachute landing */
    return true;
}
void msx_game_begin_descent(msx_game_t *g,int32_t world_x,int32_t world_y,int32_t vx,int32_t vy) {
    msx_game_init(g);g->x=world_x;g->y=56*MSX_UNIT-world_y;g->grounded=false;g->air_ticks=0;
    uint8_t *m=g->world.player_node[0];
    marco_descent_begin(m);
    m[0x30]=(uint8_t)((uint32_t)vx>>24);m[0x31]=(uint8_t)((uint32_t)vx>>16);m[0x32]=(uint8_t)((uint32_t)vx>>8);m[0x33]=(uint8_t)vx;
    m[0x34]=(uint8_t)((uint32_t)vy>>24);m[0x35]=(uint8_t)((uint32_t)vy>>16);m[0x36]=(uint8_t)((uint32_t)vy>>8);m[0x37]=(uint8_t)vy;
    g->velocity_x=vx;g->velocity_y=-vy;g->descent=MSX_DESCENT_HANG;
    /* The canopy 0C4A58 was created in the same update (0C1604): its
     * constructor runs with the first landing step. */
    neo_landing_descent(&g->landing);g->landing_on=true;
    /* MISSION 1 START 0607DE: created by the mission actor (018646) just
     * before; its first update is in the same frame as the canopy's
     * constructor (ms2 capture: 1838). Its voice is in the entry audio. */
    neo_end_reset(&g->end);neo_end_start(&g->end,0x607de);g->start_letters=true;
}
void msx_game_begin_stage(msx_game_t *g,const neo_world_stage_t *st,int start_x,int camera_max) {
    neo_hud_t hud=g->hud;neo_mission_t flow=g->flow;unsigned continues=g->continues;uint8_t best=g->best_mission;
    neo_rng_t rng=g->world.rng;marco_weapons_t weapons=g->world.weapons;neo_pickup_env_t pickup=g->world.pickup;neo_ga_flags_t flags=g->world.ga_flags;
    msx_game_init(g);
    g->hud=hud;g->flow=flow;g->continues=continues;g->best_mission=best;
    /* Only the rescue counts outlive the stage: the exit's handlers, walk
     * target, screen box (10C00B) and end requests belong to the old one. */
    neo_ga_flags_t kept;memset(&kept,0,sizeof(kept));
    memcpy(kept.rescued,flags.rescued,sizeof(kept.rescued));memcpy(kept.tally,flags.tally,sizeof(kept.tally));
    memcpy(kept.gifts,flags.gifts,sizeof(kept.gifts));memcpy(kept.special,flags.special,sizeof(kept.special));
    g->world.rng=rng;g->world.weapons=weapons;g->world.pickup=pickup;g->world.ga_flags=kept;
    neo_world_init_stage(&g->world,0,st);
    g->world.script_skip_unported=MSX_REFERENCE_INVULNERABLE!=0;   /* reference: skip unported script records */
    g->world.pickup.mission=g->flow.mission;                        /* RAM 10C2F6 (0C0BAE) */
    /* Each stage starts its timer again (DIP 60, the run4 capture). */
    neo_hud_timer_reset(&g->hud,MSX_HUD_TIMER_DIP);
    g->in_stage=true;g->stage_camera_max=camera_max;
    /* On the highest floor of the left edge (run4: y 64, 184 and 48 at the
     * three stage starts; the grid's top rows are empty), searched in the
     * map grid: the kernel's ring, already loaded by neo_world_init_stage,
     * repeats every 64 rows, and its copy of the start floor 64k rows
     * higher (stage 2 row 86 for 22, stage 3 row 261 for 5) put the player
     * outside the ring's window, on terrain read from another row: phantom
     * walls and floors (pinned at the right edge at the stops 616/752 of
     * stage 2 and 336 of stage 3). */
    neo_surface_t top;g->x=0;neo_collision_ring_bypass(true);
    g->y=neo_surface_at(0,(int32_t)neo_collision_rows()*8*MSX_UNIT,0,(int32_t)neo_collision_rows()*8*MSX_UNIT,&top)?56*MSX_UNIT-top.height:msx_ground_offset(0);
    neo_collision_ring_bypass(false);
    g->walk_in_x=start_x*MSX_UNIT;
}
void msx_game_stage_end(msx_game_t *g) {
    if (g->stage_step) return;
    /* 018F40 then 018722: the next record (the flow's exit kind). */
    g->flow.end=0;neo_mission_stage_done(&g->flow);
    g->stage_step=1;g->moving=false;g->shooting=false;memset(g->bullets,0,sizeof(g->bullets));
}
/* Stage end to the next stage's 0183D0, one flow update per game update;
 * the exit fade of kind 8 (16 steps, 0184F6) is the port's palette fade. */
static void stage_end_step(msx_game_t *g) {
    if (g->stage_load) return;
    if (g->flow.fade_in && g->flow.fade_steps>1 && g->stage_step==1) {neo_fade_start(&g->fade,(int16_t)-0x1f,(int16_t)-0x1f,(int16_t)-0x1f,g->flow.fade_steps);g->stage_step=2;}
    neo_fade_step(&g->fade);
    if (g->flow.state==NEO_MISSION_STAGE_EXIT || g->flow.state==NEO_MISSION_STAGE_ENTRY) neo_mission_update(&g->flow);
    if (g->flow.load) g->stage_load=true;
}
void msx_game_vehicle_reference(msx_game_t *g) {
    // Observed stationary vehicle variant, at the captured position. This is
    // selected only by the internal reference build, not by provisional kills.
    g->x=1020*MSX_UNIT;g->camera=872;g->y=msx_ground_offset(g->x);
    g->facing=-1;g->world_enabled=false;
    neo_vehicle_init(&g->vehicle,937*MSX_UNIT+MSX_UNIT/2,64*MSX_UNIT,936*MSX_UNIT,0,1,300,9);
}
const bool msx_reference_invulnerable=MSX_REFERENCE_INVULNERABLE!=0;
const uint32_t msx_game_layout=MSX_GAME_LAYOUT;
/* Root of the projected words (the scene root 10E968 of player_frame). */
static void root_xy(const msx_game_t *g,int32_t *rx,int32_t *ry) {
    *rx=-(int32_t)((uint32_t)g->camera<<16);*ry=MSX_PROJECTED_BASE_Y*MSX_UNIT;
    if (g->world.camera_native) {*rx=neo_camera_root_x(&g->world.camera);*ry=neo_camera_root_y(&g->world.camera);}
}
/* The landing actors after the player's update (0C162A created the marker
 * and the canopy saw the ground bit in the landing update itself). The
 * player's +1E is 0x86D6 at the landing (land2: the canopy takes 0x86D5). */
static void landing_step(msx_game_t *g) {
    int32_t rx,ry;root_xy(g,&rx,&ry);
    neo_landing_input_t in={0};
    const uint8_t *m=g->world.player_node[0];
    in.state61=m[0x61];in.flags5f=g->life.phase==MARCO_ALIVE?0:0x80u;
    in.root_x=rx;in.root_y=ry;
    if (!g->landing_root_x && !g->landing_root_y) {g->landing_root_x=rx;g->landing_root_y=ry;}
    in.root_previous_x=g->landing_root_x;in.root_previous_y=g->landing_root_y;
    in.odd_phase=(g->ticks>>1)&1u;
    in.x=(int32_t)((uint32_t)g->x+(uint32_t)rx);in.y=(int32_t)((uint32_t)(56*MSX_UNIT-g->y)+(uint32_t)ry);
    if (g->landing_pending) {g->landing_x=in.x;g->landing_y=in.y;}
    in.previous_x=g->landing_x;in.previous_y=g->landing_y;
    in.flags15=(uint8_t)((g->facing<0?1u:0u)|(g->grounded?0x40u:0u));
    in.z=g->descent?0xe0ec:0x86d6;in.player=0;in.epoch=g->world.epoch;   /* 0C15EE / the stance */
    if (g->landing_pending) {neo_landing_start(&g->landing,&in,neo_world_program);g->landing_pending=false;g->landing_on=true;}
    bool first=g->landing.canopy.active && ga_l(&g->landing.canopy,0)==0xc4a58u;
    if (first) {g->landing_x=in.x;g->landing_y=in.y;}
    neo_landing_update(&g->landing,&in,neo_world_program);
    g->landing_x=in.x;g->landing_y=in.y;g->landing_root_x=rx;g->landing_root_y=ry;
    if (!g->landing.canopy.active && !g->landing.marker.active) {g->landing_on=false;g->landing.draw_count=0;}
}
static void start_death(msx_game_t *g,uint8_t kind,bool right) {
    int32_t vx=g->velocity_x,vy=-g->velocity_y;int facing=g->facing;
    if (!marco_death_start(&g->life,kind,g->grounded,right,&vx,&vy,&facing)) g->death_stops++;
    if (kind!=2) {g->grounded=false;g->ride=0;g->air_ticks=0;}      /* 0C2E7E */
    g->velocity_x=vx;g->velocity_y=-vy;g->facing=facing;
    g->deaths++;g->moving=g->shooting=false;g->jump_pending=false;
    marco_hgun_reset(&g->hgun);
    g->mount=MSX_MOUNT_NONE;
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
    g->mount=MSX_MOUNT_NONE;
    int32_t rx,ry;root_xy(g,&rx,&ry);
    int32_t x,y;
    if (g->world_enabled && g->world.camera_native) {
        marco_respawn_t r=marco_respawn_point(neo_camera_progress(&g->world.camera),rx,ry);
        x=r.x;y=r.y;
        /* 019658's table is Mission 1's: on another mission's screen the
         * point can lie below it (Mission 2 stage 3, the tower: y 65 under
         * a camera at y 2200, three fall deaths in a row). Then the new
         * life drops from the screen's upper part. */
        int px=(int)((x+rx)>>16),py=(int)((y+ry)>>16);
        if (g->flow.mission!=0u && (py<0x120 || py>0x1f0 || px<32 || px>288)) {x=(160<<16)-rx;y=(0x1c0<<16)-ry;}
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
    p.unregistered=g->mount==MSX_MOUNT_RIDE;   /* 0C308A: +50 = 0 */
    return p;
}

/* ---- The mounted player (marco_mount.c) on player 1's node record ------- */
static uint32_t nrl(const uint8_t *m,unsigned o) {return (uint32_t)m[o]<<24|(uint32_t)m[o+1]<<16|(uint32_t)m[o+2]<<8|m[o+3];}
static void nwl(uint8_t *m,unsigned o,uint32_t v) {m[o]=(uint8_t)(v>>24);m[o+1]=(uint8_t)(v>>16);m[o+2]=(uint8_t)(v>>8);m[o+3]=(uint8_t)v;}
/* The fields of player 1's node 1014C4 the vehicles read (0C0556/0AA6B6
 * selection, 0AA760 binding, 0C0808 bombs), from the port's player, once
 * per update before the world: +3C/+40 projected (+44/+48 the previous
 * ones), +15 (facing, ground), +28/+2C/+30/+34 at the scene root (+38 =
 * 10E968, as in the traces), +5E = 1 and +6E/+70 = 0 (player 1 in the
 * traces), +5F bit 7 dying, +93 form, +9C = FF. While mounted the record
 * is the original's (marco_mount.c) and only the projection is refreshed. */
static void mount_record(msx_game_t *g,const neo_world_player_t *p) {
    uint8_t *m=g->world.player_node[0];
    memcpy(m+0x44,m+0x3c,4);memcpy(m+0x48,m+0x40,4);
    nwl(m,0x3c,(uint32_t)p->px<<16);nwl(m,0x40,(uint32_t)p->py<<16);
    if (g->mount!=MSX_MOUNT_NONE) return;
    m[0x15]=(uint8_t)((m[0x15]&~0x41u)|(g->facing<0?1u:0u)|(g->grounded?0x40u:0u));
    nwl(m,0x28,(uint32_t)g->x);nwl(m,0x2c,(uint32_t)(56*MSX_UNIT-g->y));
    nwl(m,0x30,(uint32_t)g->velocity_x);nwl(m,0x34,(uint32_t)(g->grounded?0:-g->velocity_y));nwl(m,0x38,0x10e968u);
    m[0x5e]=1;m[0x5f]=(uint8_t)((m[0x5f]&0x7fu)|(p->dead?0x80u:0u));
    m[0x6e]=m[0x6f]=m[0x70]=m[0x71]=0;m[0x93]=g->world.weapon_player.form93;m[0x9c]=0xff;
    nwl(m,0,0);
}
/* 0CA1B8 from the port's buttons: stick up/down/left/right (bits 0..3), A
 * (fire, bit 4), B (jump, bit 5), C (bomb, bit 6); new = pressed since the
 * previous update; A+B together is bit 7 (mask table 0CA1B0). */
static void mount_pad(msx_game_t *g,int stick,bool jump,bool fire) {
    uint8_t held=(uint8_t)((g->aim_up?1u:0u)|(g->crouch?2u:0u)|(stick<0?4u:0u)|(stick>0?8u:0u)|(fire?0x10u:0u)|(jump?0x20u:0u)|(g->grenade?0x40u:0u));
    uint8_t pressed=(uint8_t)(held&~g->pad_held),fresh;g->pad_held=held;
    uint8_t h=neo_vslot_route(held,pressed,neo_vslot_masks,&fresh);
    g->world.pad[0]=fresh;g->world.pad[1]=h;
}
static marco_mount_in_t mount_in(const msx_game_t *g) {
    marco_mount_in_t in;memset(&in,0,sizeof(in));
    in.video=(uint16_t)(g->world.video_phase+1u);in.fresh=g->world.pad[0];in.held=g->world.pad[1];
    in.shots=g->world.weapons.b[0];in.shot_limit=g->world.weapons.b[2];in.shot_count=g->world.weapons.b[4];
    return in;
}
/* The port's player at the record's +28/+2C in its parent +38 (0AA550). */
static void mount_place(msx_game_t *g) {
    const uint8_t *m=g->world.player_node[0];neo_contact_parent_t p;
    if (!neo_world_parent(&g->world,nrl(m,0x38),&p)) {g->mount_stops++;g->mount_stop=nrl(m,0x38);return;}
    int32_t rx,ry;root_xy(g,&rx,&ry);
    g->x=(int32_t)((uint32_t)p.x+nrl(m,0x28)-(uint32_t)rx);
    g->y=56*MSX_UNIT-(int32_t)((uint32_t)p.y+nrl(m,0x2c)-(uint32_t)ry);
    g->grounded=(m[0x15]&0x40u)!=0;g->facing=(m[0x15]&1u)?-1:1;
    g->velocity_x=g->velocity_y=0;g->ride=0;g->air_ticks=0;
}
/* The screen edges (camera+28 .. right_limit) on the feet: a grounded
 * player stopped by an edge stands on the floor at the stopped x (MAME,
 * left held after the landing: x 28.0 at camera 0, y on the floor there).
 * Clamping x alone left the feet at the height of the unclamped x, inside
 * the slope, and the next walk step found no floor (ROM 12, MiSTer). */
static void clamp_feet(msx_game_t *g,neo_air_t *body,bool grounded) {
    /* 10C00B = FF (stage exit 0B6ED2): the sweeps 0143AC/014574 skip the
     * screen box, so the exit's walk 0CA3EC leaves the screen. */
    if (g->world_enabled && g->world.ga_flags.scroll_lock) {g->x=body->x<MSX_END_X?body->x:MSX_END_X;return;}
    g->x=clamp(body->x,(g->camera+28)*MSX_UNIT,right_limit(g));
    neo_surface_t s;
    if (g->x!=body->x && grounded && neo_surface_at(g->x,body->y,4*NEO_UNIT,4*NEO_UNIT,&s)) body->y=s.height;
}
/* The parachute descent (marco_descent.c): 0C162A hanging or 0C170C after
 * the cut, with the port's air step for the sweep (0143EE, 0C109C). The
 * input of 0C0FEA: B (jump press) bit 5, A (fire press) bit 4. */
static void descent_step(msx_game_t *g) {
    uint8_t *m=g->world.player_node[0];
    int32_t rx,ry;root_xy(g,&rx,&ry);
    nwl(m,0x40,(uint32_t)(56*MSX_UNIT-g->y)+(uint32_t)ry);                /* +40: projected */
    m[0x15]=(uint8_t)((m[0x15]&~0x41u)|(g->facing<0?1u:0u));
    if (g->descent==MSX_DESCENT_HANG) {
        uint32_t input=(g->jump_pending?0x20u:0u)|(g->descent_fire?0x10u:0u);
        if (marco_descent_hang(m,neo_rng_next(&g->world.rng),input)==MARCO_DESCENT_CUT) {
            g->descent=MSX_DESCENT_FALL;g->descent_cuts++;neo_landing_marker(&g->landing);   /* 0C16D8 */
        }
    } else marco_descent_fall(m);
    g->jump_pending=false;g->descent_fire=false;
    neo_air_t body={g->x,56*MSX_UNIT-g->y,(int32_t)nrl(m,0x30),(int32_t)nrl(m,0x34)};
    int32_t previous=body.y;body.x+=body.vx;body.y+=body.vy;
    bool ground=neo_land_step(&body,previous);
    clamp_feet(g,&body,ground);g->y=56*MSX_UNIT-body.y;
    m[0x15]=(uint8_t)((m[0x15]&~0x40u)|(ground?0x40u:0u));
    if (g->descent==MSX_DESCENT_HANG) {
        if (marco_descent_hang_after(m)) {g->descent=MSX_DESCENT_NONE;neo_landing_marker(&g->landing);}   /* 0C16B0 */
    } else if (marco_descent_fall_after(m)) g->descent=MSX_DESCENT_NONE;                               /* 0C1EBE */
    g->grounded=ground;g->air_ticks=0;g->ride=0;
    g->velocity_x=ground?0:(int32_t)nrl(m,0x30);g->velocity_y=ground?0:-(int32_t)nrl(m,0x34);
}
/* 0C3250 with the port's air step for its 0143EE (as the player's own). */
static void mount_eject_step(msx_game_t *g) {
    uint8_t *m=g->world.player_node[0];marco_mount_in_t in=mount_in(g);
    uint32_t saved=marco_mount_eject(&g->world.vslots,m,&in);
    neo_air_t body={g->x,56*MSX_UNIT-g->y,(int32_t)nrl(m,0x30),(int32_t)nrl(m,0x34)};
    int32_t previous=body.y;body.x+=body.vx;body.y+=body.vy;
    bool ground=neo_land_step(&body,previous);
    clamp_feet(g,&body,ground);g->y=56*MSX_UNIT-body.y;
    m[0x15]=(uint8_t)((m[0x15]&~0x40u)|(ground?0x40u:0u));
    if (marco_mount_eject_end(m,saved)) g->mount=MSX_MOUNT_NONE;      /* 0C1EBE */
    g->grounded=ground;g->facing=(m[0x15]&1u)?-1:1;g->air_ticks=0;
    g->velocity_x=(int32_t)nrl(m,0x30);g->velocity_y=ground?0:-(int32_t)nrl(m,0x34);
}
/* Player 1's update after the world's actors (the original 1014C4 runs
 * after the Camel): the ride 0C309C, the slot hand-off of the common
 * processing 0C3352 and the exits 0C30F0. The dying player frees the slot
 * (0C2F6C), the entering one too (0C149E). 0AA5AC's sweep 0143EE is the
 * straight move by the seat (no collision: declared deviation). */
static void mount_after_world(msx_game_t *g) {
    neo_world_t *w=&g->world;uint8_t *m=w->player_node[0];
    if (g->life.phase==MARCO_DYING || g->life.phase==MARCO_LYING || g->life.phase==MARCO_ENTERING) {neo_vslot_release(&w->vslots,0);return;}
    if (g->life.phase!=MARCO_ALIVE || g->complete) return;
    marco_mount_in_t in=mount_in(g);uint32_t stop=0;marco_mount_result_t r;
    if (g->mount==MSX_MOUNT_RIDE) {
        bool followed=false;r=marco_mount_ride(&w->vslots,m,&in,&followed,&stop);
        if (followed) mount_place(g);
        if (r==MARCO_MOUNT_COMMON) r=marco_mount_handoff(&w->vslots,m,&in);
    } else r=marco_mount_handoff(&w->vslots,m,&in);
    if (r==MARCO_MOUNT_ENTER) {
        g->mount=MSX_MOUNT_RIDE;g->mounts++;marco_hgun_reset(&g->hgun);g->shooting=false;g->fire_request=false;
        return;
    }
    if (r==MARCO_MOUNT_EXIT) {
        uint8_t cause=0;r=marco_mount_exit(&w->vslots,m,&in,&cause,&stop);
        /* 0AA5AC: from the vehicle's position by the seat (slot +02/+04). */
        const uint8_t *b=w->vslots.slot[0].b;
        nwl(m,0x28,nrl(m,0x28)+((uint32_t)(b[2]<<8|b[3])<<16));nwl(m,0x2c,nrl(m,0x2c)+((uint32_t)(b[4]<<8|b[5])<<16));
        if (r!=MARCO_MOUNT_EJECT) {nwl(m,0x30,0);nwl(m,0x34,0);}   /* 0AA5F2 */
        mount_place(g);
        if (r==MARCO_MOUNT_DIE) {
            g->dismounts++;start_death(g,cause,false);neo_vslot_release(&w->vslots,0);
            return;
        }
    }
    if (r==MARCO_MOUNT_EJECT) {
        g->dismounts++;g->mount=MSX_MOUNT_EJECT;g->grounded=false;
        mount_eject_step(g);(void)marco_mount_handoff(&w->vslots,m,&in);
        return;
    }
    if (r==MARCO_MOUNT_STOP) {g->mount_stops++;g->mount_stop=stop;g->mount=MSX_MOUNT_NONE;}
}
void msx_game_projected(const msx_game_t *g,uint16_t *x,uint16_t *y) {
    neo_world_player_t p=player_frame(g);*x=p.px;*y=p.py;
}
/* The stage 3 tower: actor 0B7C38 (MAME run4/t2: the only writer of the
 * ring besides the loader during the whole climb). Each update, while
 * (camera y 10E992 >> 4) + 0x13 - 0x66 - 10C6B2 >= 0, a piece type from
 * 0261A4 goes to 0B7B1C: type >= 0xD is 0xB; the 24 x 3 blocks of the map
 * 0B740A at (0x10, 3 * type) are copied (0C70FC) to the window blocks
 * (0x10, 0x66 + 10C6B2), and 10C6B2 += 3. 0261A4 reads the sequence at
 * 10C350 (P2 bank 1: 2C1374 first, then one of four picked by 0029C6 & 3
 * from table 027E8E while 10C354 = 0, else 027E7E; 10C354 > 0 counts the
 * bytes read up to 0x100, negative becomes 1). The writes of 10C354 by
 * other actors (-1 and 2 seen in run4) are not translated: the mode stays
 * as 026190 and 0261A4 leave it. */
static const uint8_t tower_sequences[9][12]={
    {1,0,1,0,1,0xff},{0xb,0xc,0xb,0xc,0xb,0xff},{0xb,8,0xc,7,8,0xff},{0xb,0xc,9,3,1,0xff},          /* 2C133C.. (027E7E) */
    {0xb,0,0xa,0xa,0xb,7,0xff},{0xb,0xa,4,3,9,0,0xff},{0xb,8,4,7,7,8,0xff},{0xb,7,9,1,0xa,8,0xff},  /* 2C1354.. (027E8E) */
    {0xb,0xc,0xa,8,7,9,7,8,0xc,0xb,0xff}};                                                       /* 2C1374 */
static uint8_t tower_next(msx_game_t *g) {
    for (;;) {
        uint8_t v=tower_sequences[g->world.tower.table][g->world.tower.at];
        if (v!=0xff) {
            g->world.tower.at++;
            if (g->world.tower.mode>=1 && g->world.tower.mode<0x100) g->world.tower.mode++;
            return v;
        }
        unsigned pick=neo_rng_next(&g->world.rng)&3u;                       /* 0261F2 */
        if (g->world.tower.mode<0) g->world.tower.mode=1;
        g->world.tower.table=(uint8_t)(g->world.tower.mode?pick:4u+pick);g->world.tower.at=0;
    }
}
static void tower_step(msx_game_t *g) {
    if (!g->world.tower.on) return;
    for (unsigned guard=0;guard<64;guard++) {
        int d0=(int)((uint16_t)(g->world.camera.y>>16)>>4)+0x13-0x66-(int)g->world.tower.counter;
        if (d0<0) return;
        unsigned type=tower_next(g);if (type>=0xd) type=0xb;
        neo_collision_ring_copy(0x10,(int)(3*type),0x18,3,0x10,0x66+(int)g->world.tower.counter);
        /* 0C714E copies the same blocks of 0B740A to the sprite map: the
         * renderer draws piece k (10C6B2 = 3k) from the type's template. */
        unsigned k=g->world.tower.counter/3u;
        if (k<NEO_TOWER_PIECES) {g->world.tower.types[k]=(uint8_t)type;if (k>=g->world.tower.drawn) g->world.tower.drawn=k+1u;}
        g->world.tower.counter=(uint16_t)(g->world.tower.counter+3u);g->world.tower.pieces++;
    }
}
/* Mission actor camera step (neo_world_camera) and its command-list events. */
static void camera_step(msx_game_t *g,const neo_world_player_t *p) {
    neo_world_camera(&g->world,p);
    const neo_camera_t *c=&g->world.camera;
    for (unsigned i=0;i<c->event_count;i++) {
        if (c->events[i].kind==NEO_CAMERA_EV_PALETTE && c->events[i].address==0x0B56CA) g->backdrop_after=true; /* progress 2304 */
        if (c->events[i].kind==NEO_CAMERA_EV_CALL && c->events[i].address==0x0B7B00) g->world.tower.counter=0;   /* 10C6B2 */
        if (c->events[i].kind==NEO_CAMERA_EV_CALL && c->events[i].address==0x0B7C20) {
            /* 0B7C20: actor 0B7C38 (040BC) and 026190 (10C350 = 2C1374, 10C354 = 0). */
            g->world.tower.on=true;g->world.tower.table=8;g->world.tower.at=0;g->world.tower.mode=0;
        }
    }
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
/* Side contact of the player's sweeps 0143EE (air) and 014594 (ground):
 * their 013968 call (A0 = terrain, 10C008 = FF with +15 bit 5 clear,
 * 10C009 = FF on the ground, 10C00D = FF from 0128E8) stops the motion at
 * a wall cell, the surface response 0131D6/0131EC (D6 bit 2) clearing d2,
 * which 014492 stores back as +30. The port keeps its own floor model
 * (neo_walk_step/neo_land_step) and takes only that side stop from the
 * native kernel: true with *x the swept x when the sweep hit a wall. */
static bool wall_stop(msx_game_t *g,const neo_air_t *body,int32_t vx,int32_t vy,bool ground,int32_t *x) {
    if (!g->world_enabled || !vx) return false;
    neo_kscene_t *k=&g->world.kernel;k->self_dyn=0;k->self_block=0;
    neo_kreq_t q={body->x,body->y,vx,vy,NEO_KOBJ_TERRAIN,0,0xff,(uint8_t)(ground?0xff:0),0,0,0xff};
    neo_kres_t r;
    if (!neo_kernel_sweep(k,&q,&r) || !(r.flags&4u) || r.parent!=NEO_KOBJ_TERRAIN) return false;
    g->wall_stops++;*x=r.x;
    return true;
}
static solid_push_t solid_push(msx_game_t *g,neo_air_t *body,int32_t vx,int32_t vy) {
    solid_push_t r={false,false,false};
    neo_world_t *w=&g->world;
    if (!g->world_enabled) return r;
    if (w->ga_flags.player_lock) return r;                    /* 0C8CFC: 10E9E3 set, D7 = 0 */
    if (g->pilot.on) return r;   /* reference autopilot: soldiers' bodies (0E1B4E) do not hold it at a ledge */
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
 *   018884  cae6c(mission): RAM 10EA1B keeps the highest mission reached
 *           (signed byte); +60 = 0 and the prisoner tally actor 0308C0
 *           (neo_tally.c, created after the mission actor: it runs in the
 *           same update); 0188B0 waits for its +60 (the tally's root sets
 *           it when it ends; it adds the rescue bonus to 10C46A).
 *   0188B8  music 15422; sounds 0x20 and 0x1137, +7C = 0xA0, +60 = 0, the
 *           MISSION COMPLETE letters actor 06090A (neo_end.c, created after
 *           the mission actor: it runs in the same update); 0188F4 waits
 *           (subq/bcc: 161 updates).
 *   0188FC  fade 9C0A (E1,E1,E1, 0x20: to -31 in 32 updates, neo_fade.c,
 *           stepped by the list head actor 003918 before the mission actor),
 *           +7C = 0x20; 018916 waits 33 updates.
 *   01891E  mission 42F4 + 1 < 6: the next mission (1829E), neo_mission.c:
 *           Mission 2's first stage record 017212 (descriptor 016DDE). Its
 *           scene is not loaded yet: mission_end = done, actor.c logs the
 *           stage and restarts Mission 1 (docs/MISION2.md).
 * MAME runJ: 018870 at 13710, 0188B0 13770, 0188F4 <=14040, 018916 14370,
 * Mission 2 at 14460 (dumps every 30 frames). */
static uint16_t end_rand(void *ctx) {return neo_rng_next(&((msx_game_t *)ctx)->world.rng);}
/* The RAM of the tally from the world (once, as 0308C0 starts). The
 * port's single player: 10C33C = 1 (none while awaiting the continue),
 * the euro region of every capture (RAM 100000 = 1), medals 10C306/7 = 0
 * (no ported actor writes them; 0 in every capture). */
static void end_ram_start(msx_game_t *g) {
    neo_end_ram_t *m=&g->end_ram;
    memset(m,0,sizeof(*m));
    m->hud=&g->hud;m->fade=&g->fade;m->rand=end_rand;m->rand_ctx=g;
    for (unsigned p=0;p<2;p++) {
        m->rescued[p]=g->world.ga_flags.rescued[p];
        m->special[p][0]=g->world.ga_flags.special[p];m->special[p][1]=g->world.ga_flags.gifts[p];
        memcpy(m->score[p],g->world.pickup.score[p],4);
    }
    m->players=g->awaiting_continue?0u:1u;m->region=1;
}
bool msx_game_c_programs(const msx_game_t *g) {
    uint8_t id=marco_weapons_id(&g->world.weapons,0);return id==0x0c || id==3;
}
static void end_sound(msx_game_t *g,uint16_t word) {
    if (g->world.sound_count<NEO_WORLD_SOUNDS) g->world.sounds[g->world.sound_count++]=word;else g->world.sound_dropped++;
}
static void mission_end_step(msx_game_t *g) {
    switch (g->mission_end) {
    case MSX_END_NONE:
        /* 018F40 cleared 4303; the stage loop takes record +2 (018722,
         * 0182C8): type 4 with +4 = 0, the mission end 018846. */
        g->flow.end=0;neo_mission_stage_done(&g->flow);
        g->mission_end=MSX_END_WAIT;g->end_timer=0x1e;
        __attribute__((fallthrough)); /* 018864 -> 018870 in the same update */
    case MSX_END_WAIT:
        if (--g->end_timer) return;
        if ((int8_t)0>(int8_t)g->best_mission) g->best_mission=0;   /* cae6c(42F4 = 0) */
        neo_end_reset(&g->end);end_ram_start(g);
        neo_end_start(&g->end,0x308c0);                                /* 01889A */
        g->mission_end=MSX_END_TALLY;
        __attribute__((fallthrough)); /* 0188AA -> 0188B0 */
    case MSX_END_TALLY:
        /* A tally node stopped explicitly (neo_tally.c) would never set
         * +60: the port then goes on (counted in end_stops). */
        if (!g->end.mission60 && !g->end.unsupported) return;
        g->end_stops+=g->end.unsupported;
        /* 0188B8: 15422 (clears 10C2BD), then 0120F8 with 0x0020 (the
         * mission complete jingle, bank 00) and 0x1137 (the voice), in this
         * order and before the letters actor's own requests. */
        end_sound(g,0x0020);end_sound(g,0x1137);
        g->end_timer=0xa0;g->end.mission60=0;
        neo_end_start(&g->end,0x6090a);                                /* 0188DE */
        g->mission_end=MSX_END_LETTERS;
        __attribute__((fallthrough)); /* 0188EE -> 0188F4 */
    case MSX_END_LETTERS:
        if (g->end_timer--) return;
        neo_fade_start(&g->fade,(int16_t)-0x1f,(int16_t)-0x1f,(int16_t)-0x1f,0x20);   /* 9C0A */
        g->end_timer=0x20;g->mission_end=MSX_END_FADE;
        __attribute__((fallthrough)); /* 018910 -> 018916 */
    case MSX_END_FADE:
        if (g->end_timer--) return;
        neo_mission_next(&g->flow);                                     /* 01891E */
        g->mission_end=MSX_END_DONE;
        return;
    default: return;
    }
}
/* One update of the mission-end list after the mission actor: inputs of
 * the tally (new presses of A/B, video counter 10000E as the HUD's), its
 * RAM back into the world (rescued counts, score) and its sounds into the
 * world's request list. */
static void end_actors(msx_game_t *g,bool jump,bool fire) {
    neo_end_ram_t *m=&g->end_ram;
    m->pressed[0]=(uint8_t)((fire && !g->end_fire_was?0x10u:0u)|(jump && !g->end_jump_was?0x20u:0u));
    m->video=(uint16_t)(g->ticks>>1);
    neo_end_inputs_t in={g->world.root,{false,0,0,0,0,0,256,320,512},g->world.epoch,false,0,neo_world_program,m};
    neo_end_update(&g->end,&in);
    for (unsigned p=0;p<2;p++) {g->world.ga_flags.rescued[p]=m->rescued[p];memcpy(g->world.pickup.score[p],m->score[p],4);}
    for (unsigned i=0;i<m->sound_count;i++) {
        if (g->world.sound_count<NEO_WORLD_SOUNDS) g->world.sounds[g->world.sound_count++]=m->sounds[i];
        else g->world.sound_dropped++;
    }
    m->sound_count=0;
}
void msx_game_update(msx_game_t *g,int direction,bool jump,bool fire) {
    if (g->complete) {
        g->ticks++;
        if (!(g->ticks&1) && g->end_started) {
            neo_fade_step(&g->fade);                /* 003918 -> 00698C, list head */
            mission_end_step(g);
            /* The end actors after the mission actor in the list. */
            if (g->end.count) end_actors(g,jump,fire);
            g->end_fire_was=fire;g->end_jump_was=jump;
        }
        g->end_started=true;
        return;
    }
    if (g->stage_step) {g->ticks++;if (!(g->ticks&1)) stage_end_step(g);return;}
    /* Stage start of a mission pack: the player walks in (no control). */
    if (g->walk_in_x) {
        if (g->x<g->walk_in_x && g->life.phase==MARCO_ALIVE) {direction=1;jump=false;fire=false;}
        else g->walk_in_x=0;
    }
#if MSX_REFERENCE_STAGE_EXIT
    /* Reference build only, stages without their native exit yet: the
     * stage ends at the end of the camera list. Mission 2 stage 1 ends by
     * the native gate 03889C (0B6E06 -> 0B6ED2 -> 018F40); stage 3 by its
     * boss (the tower stairs reach the screen's right edge at camera x
     * 624, MAME run4 player x 915: the climb goes on). */
    if (g->in_stage && g->camera>=g->stage_camera_max && g->x>=(g->camera+280)*MSX_UNIT &&
        !(g->flow.mission==1u && (g->flow.stage==1u || g->flow.stage==3u) && g->world_enabled)) {g->stage_exit_hooks++;msx_game_stage_end(g);return;}
    /* The level script stopped at a record of an unported actor (explicit
     * stop): its camera lock would hold the stage there for good. The
     * reference build releases it (0C7C12) to walk the engine further;
     * the actors that the script would create stay missing. */
    if (g->in_stage && g->world_enabled && g->world.script.halted && g->world.camera_lock>=0) {
        g->world.camera_lock=-1;if (g->world.camera_native) neo_camera_release(&g->world.camera);
        g->stage_lock_releases++;
    }
    /* A camera stop the script holds for a group the pad does not clear
     * (Mission 2 stage 2, 2D7864: the soldiers 067E98/068020 of 06835C at
     * the right edge, under the player): after 2400 steps (40 s) at the
     * stop the reference build releases it too (lock_timeouts). */
    int lock_at=g->world.camera_native?(int)neo_camera_progress(&g->world.camera):g->camera;   /* 0C7C12 compares the list progress */
    /* The count starts at the stop and survives the camera's jitter around
     * it (Mission 2 stage 2: x 2366..2368 at the stop 2368 in ares). */
    if (g->world.camera_lock!=g->lock_value) {g->lock_value=g->world.camera_lock;g->lock_held=0;}
    if (g->in_stage && g->world_enabled && g->world.camera_lock>=0 && (g->lock_held || lock_at>=g->world.camera_lock)) {
        if (++g->lock_held>=2400u) {
            g->world.camera_lock=-1;if (g->world.camera_native) neo_camera_release(&g->world.camera);
            g->stage_lock_releases++;g->lock_timeouts++;g->lock_held=0;
        }
    } else g->lock_held=0;
#endif
    /* Stage exit 0B6ED2 (neo_goarrow.c): 018F40 ends the stage. */
    if (g->world_enabled && g->world.ga_flags.stage_done) {g->world.ga_flags.stage_done=0;msx_game_stage_end(g);return;}
    /* Collapse of the domed building: the players' input handlers are the
     * no-op 054B18 (0CA2E0) until 0CA2F2 restores them. */
    if (g->world_enabled && g->world.ga_flags.player_handlers==3u) {
        /* 0CA3EC: on the ground, walk towards screen X walk_to (10EA00);
         * no buttons. */
        int d2=g->x/MSX_UNIT-g->camera-(int)g->world.ga_flags.walk_to[0];
        direction=!g->grounded||!d2?0:d2<0?1:-1;jump=false;fire=false;g->grenade=false;
    } else if (g->world_enabled && g->world.ga_flags.player_handlers) {direction=0;jump=false;fire=false;}
    /* Player 1's input words for the vehicles (0CA1B8), once per update. */
    if ((g->ticks&1) && g->world_enabled) mount_pad(g,direction,jump,fire);
    /* Mounted (0C309C) or ejected (0C3250): the vehicle and its turret take
     * the input; the player's own controls rest. */
    if (g->mount!=MSX_MOUNT_NONE) {direction=0;jump=false;fire=false;g->grenade=false;g->jump_pending=false;}
    g->crouching=g->crouch && g->grounded && !g->ride && g->mount==MSX_MOUNT_NONE;
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
#if MSX_DEBUG_CAMEL_DROP
    /* Screenshot cartridge only (mslugx-camel-shot.z64, never a release):
     * once, the player is dropped 40 px above the Camel Slug waiting on the
     * screen (0345E0); the original's 0AA610 then takes the falling player. */
    if (!(g->ticks&1) && g->world_enabled && g->mount==MSX_MOUNT_NONE && g->life.phase==MARCO_ALIVE && !g->debug_dropped)
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *c=&g->world.gactors[i];
            if (!c->active || ga_l(c,0)!=0x345e0u) continue;
            int16_t sx=(int16_t)ga_w(c,0x3c);
            if (sx<48 || sx>272) continue;
            int32_t rx,ry;root_xy(g,&rx,&ry);
            g->x=(int32_t)(ga_l(c,0x3c)-(uint32_t)rx);
            g->y=56*MSX_UNIT+ry-(int32_t)(ga_l(c,0x40)+(40u<<16));
            g->grounded=false;g->ride=0;g->velocity_x=0;g->velocity_y=MSX_UNIT;g->air_ticks=0;
            g->debug_dropped=true;break;
        }
#endif
#if MSX_DEBUG_GIVE_R
    if (g->world_enabled && g->grounded && !g->descent && g->life.phase==MARCO_ALIVE && marco_weapons_id(&g->world.weapons,0)!=3) {
        uint16_t sound=0;marco_weapons_grant(&g->world.weapons,0,3,&g->world.weapon_player,&sound);
    }
#endif
    g->descent_fire|=fire && !g->fire_was && g->descent;
    if (!(g->ticks&1) && g->start_letters) {
        /* The MISSION 1 START letters after the mission actor (neo_end.c). */
        neo_end_inputs_t in={g->world.root,{false,0,0,0,0,0,256,320,512},g->world.epoch,false,0,neo_world_program,NULL};
        neo_end_update(&g->end,&in);
        g->start_stops+=g->end.unsupported;
        if (!g->end.count || g->end.unsupported) {g->start_letters=false;neo_end_reset(&g->end);}
    }
    if (!(g->ticks&1) && g->life.phase!=MARCO_GONE && g->descent) descent_step(g);
    else if (!(g->ticks&1) && g->life.phase!=MARCO_GONE && g->mount==MSX_MOUNT_EJECT) mount_eject_step(g);
    else if (!(g->ticks&1) && g->life.phase!=MARCO_GONE && g->mount==MSX_MOUNT_NONE) {
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
            int32_t pushed_from=body.x;
            solid_push_t s=solid_push(g,&body,gvx,body.vy);
            if ((s.left && gvx>=0) || (s.right && gvx<0)) direction=0;
            /* A push-out along a slope keeps the feet on it (Mission 2
             * stage 3, x 536: a solid pushes the player up the 1234 steps
             * and the zero-speed walk then found no floor one unit away). */
            if (body.x!=pushed_from && neo_surface_at(body.x,body.y,4*MSX_UNIT,4*MSX_UNIT,&feet)) body.y=feet.height;
            int32_t wx;
            if (direction && wall_stop(g,&body,gvx,gvy,true,&wx)) {body.x=wx;direction=0;}
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
            int32_t wx;
            if (wall_stop(g,&body,body.vx,body.vy,false,&wx)) {body.x=wx;body.vx=0;}
            body.x+=body.vx;body.y+=body.vy;
            g->air_ticks+=2;
            uint8_t object;int32_t top;
            if (body.y<previous && platform_floor(g,body.x,previous,body.y-previous,&object,&top) && top<=previous && top>=body.y) {
                body.vy=top-previous;body.y=top;g->grounded=true;ride_on(g,object,body.x,body.y);
            } else g->grounded=neo_land_step(&body,previous);
        }
        clamp_feet(g,&body,g->grounded && !g->ride);
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
        else if (g->life.phase==MARCO_ALIVE && !g->complete && g->world_enabled) {
            /* 0C3712: the projected feet (+40) below RAM 10C72C (0x100 from
             * the new life 0C0E42: the bottom of the screen) start the fall
             * death 0C2D6A (+5B = projected x >= 0xA0, then 0C2DDA with +5A
             * as it is, 0 for a player not hit). Independent of the
             * reference cartridge's invulnerability (only hits). */
            neo_world_player_t p=player_frame(g);
            int32_t safe_py=(int32_t)(int16_t)p.py+(g->y-g->safe_y)/MSX_UNIT;
            if (MSX_REFERENCE_INVULNERABLE && (int16_t)p.py<0x100 && g->safe && safe_py>=0x100) {
                /* Reference build: back to the last grounded position,
                 * unless the camera has since risen past it (Mission 2
                 * stage 3, modes 2/4): then the fall death and a new life. */
                g->x=g->safe_x;g->y=g->safe_y;g->velocity_x=g->velocity_y=0;g->ride=0;g->pit_rescues++;
            } else if ((int16_t)p.py<0x100) {g->pit_deaths++;start_death(g,0,(int16_t)p.px>=0xa0);}
            else if (MSX_REFERENCE_INVULNERABLE && g->grounded && !g->ride && (int16_t)p.py>=0x120) {g->safe_x=g->x;g->safe_y=g->y;g->safe=true;}
        }
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
    /* Down held in the air: 0C0FEA bit 1, the fire state 0C3CD0 sets +92 =
     * FF and takes the down tables (0C3C90; the H's sweeps 0C3FA6..); on
     * the ground down is the crouch. MAME capture `air` (tools/
     * capture_weapons.lua plan air): pistol, H and C shots at C0 (the H's
     * fan BE/C0/C2 and its sweeps C8..F8, mirrored B8..88), muzzle the
     * 0C4204 down row plus the jump programs' +72/+74 (tools/
     * test_marco_muzzle.c). Up wins over down (0C3D30 tests bit 0 first). */
    g->aim_down=g->crouch && !g->grounded && !g->aim_up && g->mount==MSX_MOUNT_NONE && !g->ride;
    bool fire_edge=fire && !g->fire_was;g->fire_was=fire;
    /* The knife. 0C3744 (each player update): the query row 2D0AE4 (kind
     * 0, no damage) at the player; a hurtbox of type 4 or 5 in reach sets
     * +9A bit 0. 0C3CD0 (any weapon but +93 = 3): a fire press with it
     * runs 0C3B46 instead of firing: the cut program (tables 0BF918 /
     * 0BF958, alternating by +9A bit 1; knife1 capture: 0BF958 at rest,
     * 21 frames until the next fire choice) whose first callback 0C419E
     * (one update later) hits with the row 2D0AF4 (kind 2, class 1, damage
     * 0x1E): type 5 -> sound 10FB, type 4 -> 1069. Presses during the cut
     * wait for its end (+76). */
    enum { KNIFE_TICKS=21 };
    bool knife_ok=g->world_enabled && g->mount==MSX_MOUNT_NONE && g->life.phase==MARCO_ALIVE && !g->complete && g->world.weapon_player.form93!=3;
    if (!(g->ticks&1)) {
        g->knife_reach=false;
        const uint8_t *q=ga_rom(0x2d0ae4);
        if (knife_ok && q) {
            int16_t row[7];for (unsigned i=0;i<7;i++) row[i]=(int16_t)(q[2*i]<<8|q[2*i+1]);
            uint8_t t=neo_world_player_attack(&g->world,g->x,56*MSX_UNIT-g->y,g->facing<0,row);
            g->knife_reach=t==4 || t==5;
        }
    }
    if (g->knife_t) {
        g->knife_t--;
        if (g->knife_t==KNIFE_TICKS-2u && knife_ok) {
            const uint8_t *q=ga_rom(0x2d0af4);
            if (q) {
                int16_t row[7];for (unsigned i=0;i<7;i++) row[i]=(int16_t)(q[2*i]<<8|q[2*i+1]);
                uint8_t t=neo_world_player_attack(&g->world,g->x,56*MSX_UNIT-g->y,g->facing<0,row);
                uint16_t snd=t==5?(row[4]==2?0x10fbu:0x10f9u):t==4?0x1069u:0;
                if (snd && g->world.sound_count<NEO_WORLD_SOUNDS) g->world.sounds[g->world.sound_count++]=snd;
                if (t) g->knife_hits++;
            }
        }
        if (fire_edge && !g->complete) g->fire_request=true;
        fire_edge=false;
    } else if (fire_edge && g->knife_reach && knife_ok) {
        g->knife_t=KNIFE_TICKS;g->knife_cuts++;g->fire_request=false;fire_edge=false;
        g->shot_age=0;g->shooting=true;
    }
    if (fire_edge && !g->complete) g->fire_request=true;   /* +9B bit 4 */
    if (g->knife_t) g->pistol_wait=g->pistol_wait>g->knife_t?g->pistol_wait:g->knife_t;
    bool hgun=false,chaser=false;
    if (g->world_enabled && g->world.weapon_player.form93!=2) {
        uint8_t kind=marco_weapons_ammo(&g->world.weapons,0).kind;hgun=kind==5 || kind==10;chaser=kind==0x0c || kind==3;
    }
    if (!hgun) marco_hgun_reset(&g->hgun);
    /* The body program's +72/+74 of this update, before the upper body
     * (00427A then 0C3778) reads them for the muzzle. */
    if (!(g->ticks&1)) marco_legs_step(&g->legs,g->grounded,g->moving,g->grounded && g->crouching,g->jump_moving,hgun || chaser,!g->grounded && g->velocity_y<0);
    if (hgun) {
        if (fire_edge && !g->complete) marco_hgun_press(&g->hgun);
        g->fire_request=false;g->pistol_wait=0;
        if (!(g->ticks&1)) {
            marco_weapon_player_t *m=&g->world.weapon_player;
            g->hgun.combo95=m->combo95;
            marco_hgun_input_t in={g->aim_up,g->aim_down,g->facing<0};
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
    if (chaser) {
        /* The C (id 0C, +93 = 1): the general fire state 0C3CD0 with the
         * programs of the tables 0C3C10/0C3C50/0C3C90 for id 0C (form 1:
         * level 253A56, CALL 0C4372 at 0 and +76 at 5; up 253AC6 and down
         * 255FDE, +76 at 2), so a new press fires again 6 (3) updates
         * later. 0C4372 -> 0C0D64, then after the list head (00411C) the
         * flash 0C58D2 (table 0C432A) at the 0C4244 point minus +72/+74
         * (0C43CC: facing left the x is -(t+x72)-x72) with the player's
         * motion (01ACF2), and the shell 063F32 (0C42AA) at the 0C4244
         * point (0C482E: 014A02 is the direct placement of the H bullets,
         * declared approximation) with +95 + 1, +6E = 0/40 mirrored facing
         * left, +70 = 0, +5E = the player's. The shells give their
         * projectile back themselves (0C0DFC, neo_chaser.c). */
        g->pistol_pending=0;
        if (g->pistol_wait) g->pistol_wait--;
        if (g->fire_request && !g->pistol_wait && !g->complete && !(g->ticks&1)) {
            g->fire_request=false;g->c_fires++;
            marco_ammo_t ammo=marco_weapons_ammo(&g->world.weapons,0);
            /* The R (id 3) shares the C's fire programs (tables 0C3C10/
             * 0C3C50/0C3C90: 0C02D8/0C0318/0C0358 for both) and flash;
             * its projectile is the rocket 06381C (neo_chaser.c). */
            uint8_t id=ammo.kind==3?3:0x0c;
            bool allowed=marco_ammo_reserve(&ammo,id,false,false);
            marco_weapons_set_ammo(&g->world.weapons,0,&ammo);
            bool vertical=g->aim_up || g->crouching || g->aim_down;
            g->pistol_wait=vertical?6u:12u;
            if (allowed) {
                bool left=g->facing<0;
                int aim92=g->aim_up?1:g->aim_down?-1:0;
                marco_muzzle_t m=marco_muzzle_aim(&g->legs,aim92,1,left);
                uint8_t angle=aim92>0?0x40:aim92<0?0xc0:0;
                if (left) angle=(uint8_t)(0x80-angle);
                int fx=m.x-g->legs.x72,fy=m.y-g->legs.y74;
                int32_t vx=g->velocity_x,vy=g->grounded?0:-g->velocity_y;
                neo_world_player_actor(&g->world,0xc58d2u,g->x+fx*MSX_UNIT,(56+fy)*MSX_UNIT-g->y,vx,vy,angle,left?1u:0u,0,0);
                if (neo_world_player_actor(&g->world,id==3?0x6381cu:0x63f32u,g->x+m.x*MSX_UNIT,(56+m.y)*MSX_UNIT-g->y,vx,vy,angle,0,g->world.player_node[0][0x5e],0))
                    g->world.weapon_player.combo95++;
                g->shot_age=0;g->shooting=true;g->shots++;g->weapon_shots++;g->c_shots++;
            }
        }
    } else if (!hgun) {
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
            bool vertical=g->aim_up || g->crouching || g->aim_down;
            g->pistol_wait=vertical?6u:4u;   /* ticks: 3 or 2 updates */
            if (allowed) {
                if (ammo.kind) {
                    static const uint32_t constructor[MARCO_WEAPON_IDS]={0x1b782,0xc5508,0x644c4,0x6381c,0xa71f8,0x1b6dc,0xc46d6,0x646b0,
                        0x641ec,0xa716e,0x1b716,0xc46d6,0x63f32,0x8606a,0xde4da,0xdea20};
                    /* 0C44C2/0C44E0 (+93 = 2: 01B668/01BDD2) are not ported either. */
                    g->weapon_stops++;g->weapon_stop_constructor=g->world.weapon_player.form93==2 && (ammo.kind==5 || ammo.kind==10)?
                        (ammo.kind==5?0x1b668u:0x1bdd2u):g->world.weapon_player.form93==2 && ammo.kind==0x0c?0x63f10u:constructor[ammo.kind&15u];
                }
                /* 0C4244 muzzle: table 0C4204 row by aim, form 0, plus the
                 * body program's +72/+74 (hold2: standing level (16,10)+(0,17)
                 * = (16,27), up (0,40)+(0,17) = (0,57), crouching (16,16)). */
                if (g->aim_down && !ammo.kind && g->world_enabled) {
                    /* Down in the air (+92 = FF, program 24EABC): the
                     * native bullet 01B782 at angle C0 (both facings,
                     * 0x80 - C0 = C0) from the 0C4204 down row (0,-16)
                     * plus +72/+74, after the list head like the H's. */
                    marco_muzzle_t m=marco_muzzle_aim(&g->legs,-1,0,g->facing<0);
                    neo_world_player_shot(&g->world,0x1b782u,g->x+m.x*MSX_UNIT,(56+m.y)*MSX_UNIT-g->y,0xc0,0);
                    g->shot_age=0;g->shooting=true;g->shots++;g->down_shots++;
                } else {
                marco_muzzle_t m=marco_muzzle_aim(&g->legs,g->aim_up?1:0,0,g->facing<0);
                int mx=g->facing*m.x,my=m.y;
                g->pistol_next=(msx_bullet_t){player+g->facing*mx,192-my+g->y/MSX_UNIT,g->facing,true,g->aim_up,ammo.kind};
                g->pistol_pending=2;
                g->shot_age=0;g->shooting=true;g->shots++;g->weapon_shots+=ammo.kind!=0;
                }
            }
        }
    }
    if (g->world_enabled && !(g->ticks&1)) {
        neo_world_player_t p=player_frame(g);
        camera_step(g,&p);
        /* RAM 10000E counts the original's updates (+1 per update, every
         * second video frame: MAME marco-g1, MARCO_STEP_IN phase 95, 96,
         * 97.. at frames 2101, 2103, 2105..), not video frames: its bit 0
         * flips the double buffers such as the object-list markers
         * (0401E8/04051C write one, 040200 reads the other). */
        g->world.video_phase=(uint16_t)(g->ticks>>1);
        g->world.scroll_y=neo_camera_root_y(&g->world.camera);
        mount_record(g,&p);
        if (g->landing_pending || g->landing_on) landing_step(g);
        if (neo_world_tick(&g->world,neo_camera_root_x(&g->world.camera),&p))
            msx_game_hurt(g,g->camera+(int)(int16_t)g->world.hit_x);
        tower_step(g);
        mount_after_world(g);
        /* Time over (0186FA -> 0C0A2C): +9E = FF, +5A = 6, a death however
         * invulnerable (not in the reference cartridge). */
        if (!g->hud.time_over) g->time_over_seen=false;
        else if (!g->time_over_seen && g->life.phase==MARCO_ALIVE && !g->complete && !MSX_REFERENCE_INVULNERABLE) {
            g->time_over_seen=true;g->time_overs++;start_death(g,6,g->world.hit_right);
        }
        g->kills=g->world.kills;
        if (!g->region_end && neo_world_region_clear(&g->world)) g->region_end=true;
        if (g->world.mission_step>=1) g->mission_complete=true;
        /* 018F92 of Mission 2's boss (026C30 of 026254, stage 3; the stage
         * gates 0B6ED2 of stages 1/2 run it too). Mission 3 is one stage (record 0173F6): its 018F92 ends it. */
        if (((g->flow.mission==1u && g->flow.stage==3u) || g->flow.mission==2u) && g->world.ga_flags.mission_end) g->mission_complete=true;
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
#include "neo_region.h"
#include "msx_autopilot.h"
