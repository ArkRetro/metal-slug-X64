#include "actor.h"
#include "gameplay.h"
#include "scene_assets.h"
#include "scene_renderer.h"
#include "scene_clock.h"
#include "visual_config.h"
#include "neo_collision.h"
#include "vehicle_renderer.h"
#include "retro_events.h"
#include "mission_camera.h"
#include "hud.h"
#ifdef MSX_NATIVE_MARCO
#include "player_runtime.h"
#include "marco_campaign_renderer.h"
#include "campaign_audio.h"
#include "campaign_footprint_renderer.h"
static player_runtime_t native_player;
static bool native_failure_logged;
static uint32_t native_state_signature;
static bool applied_footprints[72*5];
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static msx_game_t game;
static bool restart_was_down;
static bool diagnostics,diagnostics_was_down;
static bool has_entry;
static int32_t entry_x,entry_y;
static unsigned animation_started;
static scene_clock_t simulation_clock;
static bool previous_a,previous_b,pending_a,pending_b;
static int previous_animation=-1;
static const color_t ink={8,16,32,255};
static unsigned observed_projectiles;
static bool observed_wreck_contact;
static void reset_game(void) {
    observed_wreck_contact=false;
    if (has_entry) {bool valid=msx_game_begin_at(&game,entry_x,entry_y);assert(valid);(void)valid;}
    else msx_game_init(&game);
#if MSX_VEHICLE_REFERENCE
    msx_game_vehicle_reference(&game);vehicle_renderer_load();
    debugf("MSX vehicle reference encounter\n");
#endif
#ifdef MSX_NATIVE_MARCO
    scene_clear_replacements();memset(applied_footprints,0,sizeof(applied_footprints));
    native_failure_logged=false;
    native_state_signature=UINT32_MAX;
    if (!player_runtime_start(&native_player,&game)) game.player_failed=true;
#endif
}
#if MSX_RETRO25D
static retro_events_t depth_events;
static float depth_support_y(void) {
    neo_surface_t support;
    if (neo_surface_at(game.x,56*MSX_UNIT-game.y,0,256*MSX_UNIT,&support))
        return 248.0f-(float)support.height/MSX_UNIT;
    return 192.0f+(float)msx_ground_offset(game.x)/MSX_UNIT;
}
#endif

static void draw_frame(unsigned bank_id,unsigned frame,int center,int dy,bool mirror,bool flash) {
    const scene_bank_t *bank=scene_bank(bank_id);
    const scene_image_t *im=scene_frame(bank_id,frame);
    int x=center-bank->width/2+(mirror?bank->width-im->x-im->width:im->x);
    scene_render_image(im,x,136+dy+msx_camera_height(game.camera)+im->y,mirror,flash);
}
static void draw_projectile(const msx_bullet_t *bullet,unsigned owner) {
    const scene_bank_t *bank=scene_bank(SCENE_PROJECTILES);
    if (bullet->native && game.ticks-bullet->born_tick<4) return;
    const scene_image_t *im=scene_frame(SCENE_PROJECTILES,bullet->native?1+bullet->angle/64:0);
    bool mirror=!bullet->native && bullet->direction<0;
    int x=bullet->x-game.camera-bank->width/2+(mirror?bank->width-im->x-im->width:im->x);
    int y=bullet->y+msx_camera_height(game.camera)-bank->height/2+im->y;
    scene_render_image(im,x,y,mirror,false);
    unsigned observed=1u<<(bullet->native?4+bullet->angle/64:owner*2+mirror);
    if (!(observed_projectiles&observed)) {
        observed_projectiles|=observed;
        debugf("MSX projectile sprite owner=%u direction=%d size=%ux%u angle=%u native=%u\n",owner,bullet->direction,im->width,im->height,bullet->angle,bullet->native);
    }
}
void actor_init(void) {
    scene_assets_init();scene_renderer_init();
#ifdef MSX_NATIVE_MARCO
    bool loaded=marco_campaign_renderer_load() && campaign_audio_init() && campaign_footprint_renderer_load();
    if (!loaded) {debugf("MSX ERROR: native player resources\n");abort();}
#endif
    reset_game();
#if MSX_RETRO25D
    scene_render_set_depth_mode(true);debugf("MSX DEPTH mode enabled (depth25d default)\n");
#endif
    debugf("MSX v10 scene loaded: original collision grid + slope motion\n");
}
bool actor_begin_mission(int32_t world_x,int32_t world_y) {
    if (!msx_game_begin_at(&game,world_x,world_y)) return false;
    has_entry=true;entry_x=world_x;entry_y=world_y;
#ifdef MSX_NATIVE_MARCO
    scene_clear_replacements();memset(applied_footprints,0,sizeof(applied_footprints));
    if (!player_runtime_start(&native_player,&game)) return false;
    native_failure_logged=false;
    native_state_signature=UINT32_MAX;
#endif
    previous_animation=-1;pending_a=pending_b=previous_a=previous_b=false;
    scene_clock_reset(&simulation_clock);scene_render_forget_texture();
    debugf("MSX mission handoff x=%ld y=%ld\n",(long)world_x,(long)world_y);
    return true;
}
void actor_set_active(bool active) {
    (void)active;scene_clock_reset(&simulation_clock);pending_a=pending_b=false;
}
void actor_draw(surface_t *surface,joypad_inputs_t input) {
    if (input.btn.l && !diagnostics_was_down) {
        diagnostics=!diagnostics;debugf("MSX diagnostics %s\n",diagnostics?"on":"off");
    }
    diagnostics_was_down=input.btn.l;
    int direction=(input.btn.d_right || input.stick_x>25)-(input.btn.d_left || input.stick_x < -25);
    if (input.btn.r && !restart_was_down) { reset_game();previous_animation=-1;scene_clock_reset(&simulation_clock);debugf("MSX reset\n"); }
    restart_was_down=input.btn.r;
    unsigned old_hits=game.hits,old_kills=game.kills,old_deaths=game.deaths;
    int old_checkpoint=game.checkpoint,old_camera=game.camera,old_x=game.x;
    bool was_complete=game.complete,was_grounded=game.grounded;
    unsigned vehicle_health=game.vehicle.health,wreckage=game.wreckage_cells,vehicle_state=game.vehicle.state;
    pending_a|=input.btn.a && !previous_a;pending_b|=input.btn.b && !previous_b;
    previous_a=input.btn.a;previous_b=input.btn.b;
    unsigned steps=scene_clock_steps(&simulation_clock,TICKS_READ(),TICKS_PER_SECOND);
    for (unsigned step=0;step<steps;step++) {
#ifdef MSX_NATIVE_MARCO
        msx_game_input_t control={.direction=direction,.jump=input.btn.a || pending_a,
            .fire=input.btn.b || pending_b,.up=input.btn.d_up || input.stick_y>25,
            .down=input.btn.d_down || input.stick_y < -25,
            .pressed=(uint16_t)((pending_a?32:0)|(pending_b?16:0))};
        msx_game_update_controlled(&game,&control,player_runtime_step,&native_player);
        player_runtime_after_world(&native_player,&game);
#else
        msx_game_update(&game,direction,input.btn.a || pending_a,input.btn.b || pending_b);
#endif
        pending_a=pending_b=false;
    }
#ifdef MSX_NATIVE_MARCO
    if (game.player_failed && !native_failure_logged) {
        native_failure_logged=true;
        debugf("MSX PLAYER STOP status=%s reason=%08lx\n",
            marco_campaign_status_name(native_player.campaign.status),
            (unsigned long)native_player.campaign.reason);
    }
    uint16_t sound_token;
    while (player_runtime_pop_sound(&native_player,&sound_token)) {
        if (!campaign_audio_play(sound_token)) {
            game.player_failed=true;debugf("MSX PLAYER SOUND unsupported=%04x\n",sound_token);break;
        }
        debugf("MSX PLAYER SOUND token=%04x\n",sound_token);
    }
    const marco_body_t *body=&native_player.campaign.body;
    const marco_crouch_t *player=&body->motion.player;
    uint32_t signature=((uint32_t)body->domain<<24)|((uint32_t)body->motion.mode<<16)|
        ((uint32_t)player->mode<<8)|(uint32_t)player->upper.mode;
    if (signature!=native_state_signature) {
        native_state_signature=signature;
        debugf("MSX PLAYER state body=%u motion=%u crouch=%u upper=%u aim=%u hitbox=%dx%d draws=%u x=%d y=%d\n",
            body->domain,body->motion.mode,player->mode,player->upper.mode,player->upper.aim92,
            game.player_hitbox.width,game.player_hitbox.height,native_player.campaign.draw_count,
            game.x/MSX_UNIT,game.y/MSX_UNIT);
    }
#endif
    if (game.hits!=old_hits) debugf("MSX enemy hit=%u\n",game.hits);
    if (vehicle_health!=game.vehicle.health) debugf("MSX vehicle health=%u\n",game.vehicle.health);
    if (wreckage!=game.wreckage_cells) debugf("MSX vehicle wreckage=%u\n",game.wreckage_cells);
    if (vehicle_state!=game.vehicle.state && game.vehicle.state==NEO_VEHICLE_WRECK) debugf("MSX vehicle wreck state\n");
    if (!observed_wreck_contact && game.vehicle.active && !game.vehicle.health && game.grounded &&
        game.y==32*MSX_UNIT-game.vehicle.y &&
        game.x>=game.vehicle.x-40*MSX_UNIT && game.x<=game.vehicle.x+40*MSX_UNIT) {
        observed_wreck_contact=true;debugf("MSX player on wreck\n");
    }
    if (game.kills!=old_kills) debugf("MSX defeated=%u\n",game.kills);
    if (game.deaths!=old_deaths) debugf("MSX respawn checkpoint=%d\n",game.checkpoint);
    if (game.checkpoint!=old_checkpoint) debugf("MSX checkpoint=%d\n",game.checkpoint);
    if (!old_camera && game.camera) debugf("MSX camera moving\n");
    if (msx_camera_height(game.camera)!=msx_camera_height(old_camera) && msx_camera_height(game.camera)==MSX_CAMERA_RISE_MAX)
        debugf("MSX camera raised=%d\n",game.camera);
    if (msx_layer_visible(2,game.camera) && !msx_layer_visible(2,old_camera)) debugf("MSX middle bank full speed=%d\n",game.camera);
    if (old_x<596*MSX_UNIT && game.x>=596*MSX_UNIT) debugf("MSX first roof x=%d\n",game.x/MSX_UNIT);
    if (!was_complete && game.complete) debugf("MSX observed region end camera=%d x=%d\n",game.camera,game.x);
    if (was_grounded && !game.grounded) debugf("MSX jump\n");
    if (!was_grounded && game.grounded) debugf("MSX landed height=%d\n",game.y/MSX_UNIT);
    int animation=msx_game_animation(&game);
    if (animation!=previous_animation) animation_started=game.ticks;
    if (animation!=previous_animation && animation==MSX_RUN_JUMP) debugf("MSX run-jump\n");
    if (animation!=previous_animation && animation==MSX_RUN_FIRE) debugf("MSX run-fire\n");
    previous_animation=animation;
#ifndef MSX_NATIVE_MARCO
    unsigned bank_id=animation>=MSX_RUN_JUMP?SCENE_MOTION:animation>=MSX_FIRE?SCENE_ACTIONS:SCENE_BASE;
    const scene_bank_t *bank=scene_bank(bank_id);
    bool second=animation==MSX_RUN || animation==MSX_JUMP || animation==MSX_RUN_FIRE;
    unsigned count=second?bank->second:bank->first,sample;
    if (animation==MSX_JUMP || animation==MSX_RUN_JUMP) {
        sample=game.air_ticks/bank->interval;if (sample>=count) sample=count-1;
    } else if (animation==MSX_FIRE) sample=(game.shot_age/bank->interval)%count;
    else sample=((game.ticks-animation_started)/bank->interval)%count;
    unsigned frame=sample+(second?bank->first:0);
    // Keep imminent frame transitions requested ahead of the draw deadline.
    for (unsigned next=0;next<3;next++)
        scene_request_image(scene_frame(bank_id,(sample+next)%count+(second?bank->first:0)),3);
#endif
    scene_request_image(scene_frame(SCENE_PROJECTILES,0),3);
    const scene_bank_t *soldier=scene_bank(SCENE_SOLDIER);
    unsigned sf=(game.ticks/soldier->interval)%soldier->first;
    if (game.enemy.x-game.x/MSX_UNIT<400) for (unsigned i=0;i<3;i++)
        scene_request_image(scene_frame(SCENE_SOLDIER,(sf+i)%soldier->first),2);
    scene_service();
#if MSX_RETRO25D
    retro_frame_t effects={.camera={game.camera,-msx_camera_height(game.camera),0,320},.tick=game.ticks};
    effects.shadows[effects.shadow_count++]=(retro_shadow_t){(float)game.x/MSX_UNIT,192.0f+(float)game.y/MSX_UNIT,depth_support_y(),12};
    if (game.enemy.alive) effects.shadows[effects.shadow_count++]=(retro_shadow_t){game.enemy.x,192.0f+(float)msx_ground_offset(game.enemy.x*MSX_UNIT)/MSX_UNIT,192.0f+(float)msx_ground_offset(game.enemy.x*MSX_UNIT)/MSX_UNIT,10};
    if (game.shooting && game.shot_age<3) effects.lights[effects.light_count++]=(retro_light_t){
        {(float)game.x/MSX_UNIT+game.facing*24,168.0f+(float)game.y/MSX_UNIT,-18},{255,174,72},72,1.0f-(float)game.shot_age/3};
    if (game.enemy.flash) effects.lights[effects.light_count++]=(retro_light_t){
        {game.enemy.x,168.0f+(float)msx_ground_offset(game.enemy.x*MSX_UNIT)/MSX_UNIT,-24},{255,128,48},100,(float)game.enemy.flash/12};
    // Vehicle shadow, impact/explosion light pulses and fog focus (render-only).
    retro_events_frame(&depth_events,&game,&effects);
    scene_render_depth_frame(&effects);
#endif
#ifdef MSX_NATIVE_MARCO
    for (int column=0;column<72;column++) {
        for (int row=0;row<5;row++) if (!applied_footprints[column*5+row] &&
                player_runtime_stamp(&native_player,column,row)) {
            uint16_t pixels[256];bool changed;
            bool applied=campaign_footprint_pixels(column,row,pixels,&changed);
            if (applied && changed) applied=scene_replace_foreground_tile(column*16,112+row*16,pixels);
            if (!applied) {game.player_failed=true;debugf("MSX PLAYER FOOTPRINT missing=%d,%d\n",column,row);}
            else applied_footprints[column*5+row]=true;
        }
    }
#endif
    scene_render_begin(surface);
    scene_render_background(game.camera,game.ticks);
    vehicle_renderer_draw(&game.vehicle,game.camera,game.vehicle_flash && (game.ticks&2));
    if (game.enemy.alive || game.enemy.respawn>75)
        draw_frame(SCENE_SOLDIER,sf,game.enemy.x-game.camera,msx_ground_offset(game.enemy.x*MSX_UNIT)/MSX_UNIT,game.enemy.facing>0,
            (game.enemy.flash || !game.enemy.alive) && (game.ticks&1));
    if (!(game.invulnerable && ((game.ticks/3)&1)))
#ifdef MSX_NATIVE_MARCO
    {
        rdpq_set_scissor(8,8,312,232);
        for (unsigned i=0;i<native_player.campaign.draw_count;i++) {
            bool drawn=marco_campaign_renderer_draw_native(&native_player.campaign.draws[i],
                native_player.draw_camera-game.camera+native_player.draw_offset_x,
                -8+msx_camera_height(game.camera)+native_player.draw_offset_y);
            if (!drawn) {game.player_failed=true;debugf("MSX PLAYER DRAW missing=%08lx\n",
                (unsigned long)native_player.campaign.draws[i].descriptor);break;}
        }
        scene_render_forget_texture();rdpq_set_scissor(0,0,320,240);
    }
#else
        draw_frame(bank_id,frame,game.x/MSX_UNIT-game.camera,game.y/MSX_UNIT,game.facing<0,false);
#endif
    for (unsigned owner=0;owner<2;owner++) {
        const msx_bullet_t *list=owner?game.enemy_bullets:game.bullets;
        unsigned n=owner?MSX_ENEMY_BULLETS:MSX_BULLETS;
        for (unsigned i=0;i<n;i++) if (list[i].active) {
            int x=list[i].x-game.camera;
            if (x>=-16 && x<336) draw_projectile(&list[i],owner);
        }
    }
    scene_render_foreground_fog();
    msx_hud_draw(&(msx_hud_state_t){.health=game.health,.kills=game.kills});
    scene_render_forget_texture();
    if (diagnostics) {
        scene_render_box(28,20,264,40,ink);scene_render_box(28,202,264,24,ink);
    }
    if (game.complete) scene_render_box(64,92,192,48,ink);
    // Fonts are last: their own uploads may replace TMEM texture/palette data.
    if (diagnostics) {
        scene_render_text(32,24,"MSX V10 - MISSION 1");
        char status[40];
        if (game.vehicle.active) snprintf(status,sizeof(status),"HP:%u VEHICLE:%u",game.health,game.vehicle.health);
        else snprintf(status,sizeof(status),"HP:%u GUARDS:%u/3 CP:%s",game.health,game.kills,game.checkpoint?"YES":"NO");
        scene_render_text(32,38,status);
        scene_render_text(32,204,"MOVE / A:JUMP B:FIRE");scene_render_text(32,216,"L:DEBUG R:RESTART");
    }
    if (game.complete) {scene_render_text(104,102,"REGION END");scene_render_text(88,120,"R: PLAY AGAIN");}
    if (game.player_failed) {
        scene_render_box(40,92,240,48,ink);
        scene_render_text(48,100,"PLAYER CONTROLLER STOPPED");
        scene_render_text(104,118,"R: RESTART");
    }
    scene_render_end();
}
