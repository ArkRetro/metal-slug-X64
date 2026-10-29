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
#include "msx_perf.h"
#include <stdio.h>

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
/* One native animation draw (original 004364 record): descriptor parts from
 * the scene pack, logical palette resource resolved through the pack's
 * palette table, mirrored draws one pixel left (observed, see
 * tools/export_soldier_sprites.py). Screen y = 248 - original y (+ the
 * camera height applied to every gameplay sprite). */
static unsigned native_missing_descriptor,native_missing_palette;
static int native_screen_y(uint16_t projected_y) {
    return MSX_SCREEN_BASE_Y-((int)(int16_t)projected_y-MSX_PROJECTED_BASE_Y)+msx_camera_height(game.camera);
}
static void draw_native(const neo_world_draw_t *d,bool request_only) {
    const scene_native_descriptor_t *desc=scene_native_descriptor(d->descriptor);
    if (!desc) {native_missing_descriptor++;return;}
    int palette=scene_native_palette(d->palette);
    if (palette<0) {native_missing_palette++;palette=scene_native_palette(0x20e);}
    bool mirror=(d->attributes&1u)!=0;
    int origin_y=native_screen_y(d->y);
    for (unsigned i=0;i<desc->count;i++) {
        const scene_image_t *im=scene_frame(SCENE_NATIVE,desc->first+i);
        if (request_only) {scene_request_image(im,2);continue;}
        scene_image_t part=*im;part.palette=(uint16_t)palette;
        int x=mirror?(int)(int16_t)d->x-(im->x+im->width)-1:(int)(int16_t)d->x+im->x;
        int y=origin_y+im->y;
        if (x+im->width<=0 || x>=320 || y+im->height<=0 || y>=240) continue;
        scene_render_image(&part,x,y,mirror,d->flash && (game.ticks&1));
    }
}
static void draw_projectile(const msx_bullet_t *bullet,unsigned owner) {
    const scene_bank_t *bank=scene_bank(SCENE_PROJECTILES);
    const scene_image_t *im=scene_frame(SCENE_PROJECTILES,0);
    bool mirror=bullet->direction<0;
    int x=bullet->x-game.camera-bank->width/2+(mirror?bank->width-im->x-im->width:im->x);
    int y=bullet->y+msx_camera_height(game.camera)-bank->height/2+im->y;
    scene_render_image(im,x,y,mirror,false);
    unsigned observed=1u<<(owner*2+mirror);
    if (!(observed_projectiles&observed)) {
        observed_projectiles|=observed;
        debugf("MSX projectile sprite owner=%u direction=%d size=%ux%u\n",owner,bullet->direction,im->width,im->height);
    }
}
void actor_init(void) {
    scene_assets_init();scene_renderer_init();reset_game();
#if MSX_RETRO25D
    scene_render_set_depth_mode(true);debugf("MSX DEPTH mode enabled (depth25d default)\n");
#endif
    debugf("MSX v10 scene loaded: original collision grid + slope motion\n");
}
bool actor_begin_mission(int32_t world_x,int32_t world_y) {
    if (!msx_game_begin_at(&game,world_x,world_y)) return false;
    has_entry=true;entry_x=world_x;entry_y=world_y;
    previous_animation=-1;pending_a=pending_b=previous_a=previous_b=false;
    scene_clock_reset(&simulation_clock);scene_render_forget_texture();
    debugf("MSX mission handoff x=%ld y=%ld\n",(long)world_x,(long)world_y);
    return true;
}
void actor_set_active(bool active) {
    (void)active;scene_clock_reset(&simulation_clock);pending_a=pending_b=false;
}
void actor_draw(surface_t *surface,joypad_inputs_t input) {
    msx_perf_frame_start();
    if (input.btn.l && !diagnostics_was_down) {
        diagnostics=!diagnostics;debugf("MSX diagnostics %s\n",diagnostics?"on":"off");
    }
    diagnostics_was_down=input.btn.l;
    int direction=(input.btn.d_right || input.stick_x>25)-(input.btn.d_left || input.stick_x < -25);
    if (input.btn.r && !restart_was_down) { reset_game();previous_animation=-1;scene_clock_reset(&simulation_clock);debugf("MSX reset\n"); }
    restart_was_down=input.btn.r;
    unsigned old_hits=game.hits,old_kills=game.kills,old_deaths=game.deaths,old_spawned=game.world.spawned,old_unsupported=game.world.unsupported;
    bool old_halted=game.world.script.halted;
    int old_checkpoint=game.checkpoint,old_camera=game.camera,old_x=game.x;
    bool was_complete=game.complete,was_grounded=game.grounded;
    unsigned vehicle_health=game.vehicle.health,wreckage=game.wreckage_cells,vehicle_state=game.vehicle.state;
    pending_a|=input.btn.a && !previous_a;pending_b|=input.btn.b && !previous_b;
    previous_a=input.btn.a;previous_b=input.btn.b;
    unsigned steps=scene_clock_steps(&simulation_clock,TICKS_READ(),TICKS_PER_SECOND);
    for (unsigned step=0;step<steps;step++) {
        uint32_t step_start=TICKS_READ();
        msx_game_update(&game,direction,input.btn.a || pending_a,input.btn.b || pending_b);
        pending_a=pending_b=false;
        msx_perf_add(MSX_PERF_STEP,TICKS_READ()-step_start);
    }
    msx_perf_steps(steps);msx_perf_mark(MSX_PERF_SIM);
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
    if (game.world.spawned!=old_spawned) debugf("MSX rebel spawned=%u alive=%u camera=%d\n",game.world.spawned,neo_world_alive(&game.world),game.camera);
    if (game.world.script.halted && !old_halted) debugf("MSX script stopped at %06lx (records beyond are not ported)\n",(unsigned long)game.world.script.halt_address);
    if (game.world.unsupported!=old_unsupported) debugf("MSX world unsupported routine=%05lx\n",(unsigned long)game.world.unsupported_routine);
    if (native_missing_descriptor || native_missing_palette) {
        debugf("MSX native draw missing descriptor=%u palette=%u\n",native_missing_descriptor,native_missing_palette);
        native_missing_descriptor=native_missing_palette=0;
    }
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
    scene_request_image(scene_frame(SCENE_PROJECTILES,0),3);
    for (unsigned i=0;i<game.world.draw_count;i++) draw_native(&game.world.draws[i],true);
    scene_service();
#if MSX_RETRO25D
    retro_frame_t effects={.camera={game.camera,-msx_camera_height(game.camera),0,320},.tick=game.ticks};
    effects.shadows[effects.shadow_count++]=(retro_shadow_t){(float)game.x/MSX_UNIT,192.0f+(float)game.y/MSX_UNIT,depth_support_y(),12};
    // Contact shadow per drawn rebel soldier: feet at its draw origin
    // (projected words, camera height excluded: shadows use world space).
    for (unsigned i=0;i<game.world.draw_count;i++) if (game.world.draws[i].kind==NEO_WORLD_SOLDIER) {
        const neo_world_draw_t *d=&game.world.draws[i];
        float feet=(float)(MSX_SCREEN_BASE_Y-((int)(int16_t)d->y-MSX_PROJECTED_BASE_Y));
        retro_frame_add_shadow(&effects,retro_actor_shadow((float)(game.camera+(int16_t)d->x),feet,feet,10));
    }
    if (game.shooting && game.shot_age<3) effects.lights[effects.light_count++]=(retro_light_t){
        {(float)game.x/MSX_UNIT+game.facing*24,168.0f+(float)game.y/MSX_UNIT,-18},{255,174,72},72,1.0f-(float)game.shot_age/3};
    // Vehicle shadow, impact/explosion light pulses and fog focus (render-only).
    retro_events_frame(&depth_events,&game,&effects);
    scene_render_depth_frame(&effects);
#endif
    msx_perf_mark(MSX_PERF_PREP);
    scene_render_begin(surface);msx_perf_mark(MSX_PERF_BEGIN);
    scene_render_background(game.camera,game.ticks);
    vehicle_renderer_draw(&game.vehicle,game.camera,game.vehicle_flash && (game.ticks&2));
    for (unsigned i=0;i<game.world.draw_count;i++) draw_native(&game.world.draws[i],false);
    if (!(game.invulnerable && ((game.ticks/3)&1)))
        draw_frame(bank_id,frame,game.x/MSX_UNIT-game.camera,game.y/MSX_UNIT,game.facing<0,false);
    for (unsigned owner=0;owner<2;owner++) {
        const msx_bullet_t *list=owner?game.enemy_bullets:game.bullets;
        unsigned n=owner?MSX_ENEMY_BULLETS:MSX_BULLETS;
        for (unsigned i=0;i<n;i++) if (list[i].active) {
            int x=list[i].x-game.camera;
            if (x>=-16 && x<336) draw_projectile(&list[i],owner);
        }
    }
    scene_render_foreground_fog();
    msx_perf_mark(MSX_PERF_ACTORS);
    if (diagnostics) {
        scene_render_box(28,20,264,40,ink);scene_render_box(28,202,264,24,ink);
    }
    if (game.complete) scene_render_box(64,92,192,48,ink);
    // Fonts are last: their own uploads may replace TMEM texture/palette data.
    if (diagnostics) {
        scene_render_text(32,24,"MSX V10 - MISSION 1");
        char status[40];
        if (game.vehicle.active) snprintf(status,sizeof(status),"HP:%u VEHICLE:%u",game.health,game.vehicle.health);
        else snprintf(status,sizeof(status),"HP:%u REBELS:%u/%u CP:%s",game.health,game.kills,game.world.spawned,game.checkpoint?"YES":"NO");
        scene_render_text(32,38,status);
        scene_render_text(32,204,"MOVE / A:JUMP B:FIRE");scene_render_text(32,216,"START:LAB R:RESTART");
    }
    if (game.complete) {scene_render_text(104,102,"REGION END");scene_render_text(88,120,"R: PLAY AGAIN");}
    msx_perf_mark(MSX_PERF_HUD);
    scene_render_end();msx_perf_mark(MSX_PERF_END);
    msx_perf_counts(scene_render_draw_count(),neo_world_alive(&game.world));
    msx_perf_frame_end();
}
