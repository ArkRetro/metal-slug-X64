#include "neo_region.h"
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
#include "game_audio.h"
#include "boss_renderer.h"
#include "marco_visual.h"
#include "marco_renderer.h"
#include "hud_renderer.h"
#include <stdio.h>

static msx_game_t game;
static bool death_logged;
static bool restart_was_down;
static bool diagnostics,diagnostics_was_down;
static bool has_entry;
static int32_t entry_x,entry_y;
static unsigned animation_started;
static scene_clock_t simulation_clock;
static bool previous_a,previous_b,pending_a,pending_b,previous_start;
static int previous_animation=-1;
static const color_t ink={8,16,32,255};
static unsigned observed_projectiles;
static bool observed_wreck_contact;
/* Campaign Marco: original programs (marco_visual.c) drawn from the audited
 * descriptor parts; the historical six-state banks remain only as fallback
 * when romfs/marco is absent. */
static marco_visual_t marco;static bool marco_native;static unsigned marco_shots,marco_failures;
static void reset_game(void) {
    marco_visual_reset(&marco);marco_shots=0;
    observed_wreck_contact=false;
    if (has_entry) {bool valid=msx_game_begin_at(&game,entry_x,entry_y);assert(valid);(void)valid;}
    else msx_game_init(&game);
#if MSX_VEHICLE_REFERENCE
    msx_game_vehicle_reference(&game);vehicle_renderer_load();
    debugf("MSX vehicle reference encounter\n");
#endif
    game_audio_sync(&game);
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
    scene_render_image(im,x,136+dy+game.camera_height+im->y,mirror,flash);
}
/* One native animation draw (original 004364 record): descriptor parts from
 * the scene pack, logical palette resource resolved through the pack's
 * palette table, mirrored draws one pixel left (observed, see
 * tools/export_soldier_sprites.py). Screen y = 248 - original y (+ the
 * camera height applied to every gameplay sprite). */
static unsigned native_missing_descriptor,native_missing_palette;
static int native_screen_y(uint16_t projected_y) {
    /* The projected word already includes the scene root (camera height). */
    return MSX_SCREEN_BASE_Y-((int)(int16_t)projected_y-MSX_PROJECTED_BASE_Y);
}
static void draw_native(const neo_world_draw_t *d,bool request_only) {
    if (d->kind==NEO_WORLD_BOSS) {
        /* Boss entries: column cut and palette resource (boss_renderer.c). */
        neo_anim_draw_t a={d->descriptor,d->x,d->y,d->attributes,0};
        boss_render_draw(&a,d->palette,0,d->flash && (game.ticks&1),request_only);
        return;
    }
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
        if (d->zoom_x!=0xff || d->zoom_y!=0xff) {
            /* Shrunk sprite (+12/+13): offsets and size scaled about the
             * actor's point (render approximation of the hardware shrink). */
            float sx=(d->zoom_x+1)/256.0f,sy=(d->zoom_y+1)/256.0f;
            float w=im->width*sx,h=im->height*sy;
            float x=mirror?(int16_t)d->x-(im->x+im->width)*sx-1:(int16_t)d->x+im->x*sx;
            float y=origin_y+im->y*sy;
            if (x+w<=0 || x>=320 || y+h<=0 || y>=240) continue;
            scene_render_image_scaled(&part,x,y,w,h,mirror,d->flash && (game.ticks&1));
            continue;
        }
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
    int y=bullet->y+game.camera_height-bank->height/2+im->y;
    scene_render_image(im,x,y,mirror,false);
    unsigned observed=1u<<(owner*2+mirror);
    if (!(observed_projectiles&observed)) {
        observed_projectiles|=observed;
        debugf("MSX projectile sprite owner=%u direction=%d size=%ux%u\n",owner,bullet->direction,im->width,im->height);
    }
}
void actor_init(void) {
    assertf(msx_game_layout==MSX_GAME_LAYOUT,"stale gameplay object: msx_game_t layout %08lx, actor.c %08lx (rebuild)",
        (unsigned long)msx_game_layout,(unsigned long)MSX_GAME_LAYOUT);
    scene_assets_init();scene_renderer_init();reset_game();
    marco_native=marco_renderer_load();
    hud_renderer_init();
    debugf("MSX marco native animation %s (%u poses)\n",marco_native?"loaded":"missing: six-state fallback",marco_native?marco_renderer_count():0u);
#if MSX_RETRO25D
    scene_render_set_depth_mode(true);debugf("MSX DEPTH mode enabled (depth25d default)\n");
#endif
    debugf("MSX v10 scene loaded: original collision grid + slope motion\n");
    if (msx_reference_invulnerable) debugf("MSX reference build: player invulnerable (not a release)\n");
}
bool actor_begin_mission(int32_t world_x,int32_t world_y) {
    if (!msx_game_begin_at(&game,world_x,world_y)) return false;
    has_entry=true;entry_x=world_x;entry_y=world_y;
    previous_animation=-1;pending_a=pending_b=previous_a=previous_b=false;
    scene_clock_reset(&simulation_clock);scene_render_forget_texture();
    game_audio_sync(&game);
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
    /* GAME OVER shown until the original hands the machine back to the BIOS:
     * 190 updates after player mode 3 (MAME go1: frames 4578..4957). The
     * ranking/attract that follow are not ported: the mission restarts. */
    if (game.mission_end==MSX_END_DONE) {
        /* 01891E: Mission 2 is not ported; Mission 1 starts again. */
        debugf("MSX mission end 018846 done (explicit stops=%u): Mission 2 not ported, Mission 1 restarts\n",game.end_stops);
        reset_game();previous_animation=-1;scene_clock_reset(&simulation_clock);
    }
    if (game.game_over_frames>=MSX_GAME_OVER_UPDATES) {
        debugf("MSX game over: mission restart (continues=%u)\n",game.continues);
        reset_game();previous_animation=-1;scene_clock_reset(&simulation_clock);
    }
    if (input.btn.r && !restart_was_down) { reset_game();previous_animation=-1;scene_clock_reset(&simulation_clock);debugf("MSX reset\n"); }
    restart_was_down=input.btn.r;
    uint8_t old_weapon=marco_weapons_id(&game.world.weapons,0),old_bombs=marco_weapons_bomb_count(&game.world.weapons,0);
    unsigned old_weapon_stops=game.weapon_stops,old_grenades=game.grenades,old_grenade_stops=game.grenade_stops,old_served=game.world.pickup.taken;
    unsigned old_hits=game.hits,old_kills=game.kills,old_deaths=game.deaths,old_spawned=game.world.spawned,old_unsupported=game.world.unsupported,old_pickups=game.world.item_pickups;
    bool old_halted=game.world.script.halted;
    int old_checkpoint=game.checkpoint,old_camera=game.camera,old_x=game.x,old_height=game.camera_height;
    unsigned old_progress=neo_camera_progress(&game.world.camera);bool old_region_end=game.region_end,old_backdrop=game.backdrop_after;
    neo_ga_flags_t old_flags=game.world.ga_flags;
    bool old_boss=game.world.boss_active,old_mission=game.mission_complete;uint8_t old_flag606=game.world.flag606,old_phase=game.world.boss_active?neo_boss_phase(&game.world.boss):0;
    unsigned old_boss_unsupported=game.world.boss_unsupported,old_hp=game.world.boss_active?neo_boss_health(&game.world.boss):0;
    int32_t old_lock=game.world.camera_lock;uint8_t old_auto=game.world.camera.auto_mode;
    bool was_complete=game.complete,was_grounded=game.grounded;
    unsigned vehicle_health=game.vehicle.health,wreckage=game.wreckage_cells,vehicle_state=game.vehicle.state;
    pending_a|=input.btn.a && !previous_a;pending_b|=input.btn.b && !previous_b;
    game.start_edge|=input.btn.start && !previous_start;game.jump_edge|=input.btn.a && !previous_a;previous_start=input.btn.start;
    previous_a=input.btn.a;previous_b=input.btn.b;
    unsigned steps=scene_clock_steps(&simulation_clock,TICKS_READ(),TICKS_PER_SECOND);
    for (unsigned step=0;step<steps;step++) {
        uint32_t step_start=TICKS_READ();
        game.aim_up=input.btn.d_up || input.stick_y>25;
        game.crouch=input.btn.d_down || input.stick_y<-25;
        game.grenade=input.btn.z;   /* bomb (the arcade's C button) */
        msx_game_update(&game,direction,input.btn.a || pending_a,input.btn.b || pending_b);
        /* Dying, lying and the entrance (marco_death.c): the death and
         * entrance programs (0BF298/0BF318/0BF358/0BF3D8, 0BF498) draw;
         * the body/weapon programs restart with the new life. */
        if (game.life.phase==MARCO_DYING && !death_logged) {death_logged=true;debugf("MSX player death kind=%u\n",game.world.hit_kind);}
        if (game.life.phase==MARCO_ALIVE) death_logged=false;
        if (game.life.phase!=MARCO_ALIVE) {marco_visual_reset(&marco);marco_shots=game.shots;}
        if (!(game.ticks&1) && !game.complete && game.life.phase==MARCO_ALIVE) {
            /* One original player update: step the body/weapon programs. */
            marco_visual_input_t mi={game.grounded,game.moving,game.crouching,game.aim_up,game.shots!=marco_shots,
                game.jump_moving,game.facing,0,0,game.world.weapon_player.form93==1?1u:0u,-1,false};
            if (game.hgun.state==MARCO_HGUN_BURST || game.hgun.state==MARCO_HGUN_SWEEP) {
                mi.hgun_program=game.hgun.program;mi.hgun_start=game.hgun.t==1;
            }
            msx_game_projected(&game,&mi.x,&mi.y);marco_shots=game.shots;
            marco_visual_step(&marco,&mi);
            if (marco.failures!=marco_failures) {debugf("MSX marco program stopped (unrecovered) count=%u\n",marco.failures);marco_failures=marco.failures;}
        }
        pending_a=pending_b=false;
        msx_perf_add(MSX_PERF_STEP,TICKS_READ()-step_start);
    }
    msx_perf_steps(steps);msx_perf_mark(MSX_PERF_SIM);
    game_audio_update(&game);
    {static uint8_t panel_state;static unsigned hud_stops;
     if (game.hud.panel[0].state!=panel_state) {
        static const char *const names[]={"play","continue","game over","stopped"};
        panel_state=game.hud.panel[0].state;
        debugf("MSX panel %s (0C9848) timer=%04x continues=%u score=%02x%02x%02x%02x\n",names[panel_state&3],game.hud.continue_timer,game.continues,
            game.world.pickup.score[0][0],game.world.pickup.score[0][1],game.world.pickup.score[0][2],game.world.pickup.score[0][3]);
     }
     if (game.hud.unsupported!=hud_stops) {hud_stops=game.hud.unsupported;debugf("MSX HUD explicit stops=%u\n",hud_stops);}}
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
    if (game.world.unsupported!=old_unsupported && game.world.boss_unsupported==old_boss_unsupported) debugf("MSX world unsupported routine=%05lx\n",(unsigned long)game.world.unsupported_routine);
    if (game.world.item_pickups!=old_pickups) debugf("MSX item pickup stopped explicitly at %05lx (untranslated)\n",(unsigned long)game.world.item_pickup_routine);
    if (game.world.pickup.taken!=old_served) debugf("MSX item served=%u score=%02x%02x%02x%02x\n",game.world.pickup.taken,
        game.world.pickup.score[0][0],game.world.pickup.score[0][1],game.world.pickup.score[0][2],game.world.pickup.score[0][3]);
    if (marco_weapons_id(&game.world.weapons,0)!=old_weapon) debugf("MSX weapon id=%x rounds=%u\n",marco_weapons_id(&game.world.weapons,0),marco_weapons_rounds(&game.world.weapons,0));
    if (marco_weapons_bomb_count(&game.world.weapons,0)!=old_bombs) debugf("MSX bombs=%u\n",marco_weapons_bomb_count(&game.world.weapons,0));
    if (game.weapon_stops!=old_weapon_stops && !old_weapon_stops) debugf("MSX weapon shot stopped explicitly: constructor %05lx not ported (pistol bullet fired)\n",(unsigned long)game.weapon_stop_constructor);
    if (game.grenades!=old_grenades) debugf("MSX grenade thrown count=%u\n",game.grenades);
    if (game.grenade_stops!=old_grenade_stops) debugf("MSX bomb type %u stopped explicitly (0C4756 entry not ported)\n",game.world.weapons.b[10]);
    if (native_missing_descriptor || native_missing_palette) {
        debugf("MSX native draw missing descriptor=%u palette=%u\n",native_missing_descriptor,native_missing_palette);
        native_missing_descriptor=native_missing_palette=0;
    }
    if (game.deaths!=old_deaths) debugf("MSX respawn checkpoint=%d\n",game.checkpoint);
    if (game.checkpoint!=old_checkpoint) debugf("MSX checkpoint=%d\n",game.checkpoint);
    if (!old_camera && game.camera) debugf("MSX camera moving\n");
    if (game.camera_height!=old_height && (game.camera_height==MSX_CAMERA_RISE_MAX || game.camera_height==32))
        debugf("MSX camera raised=%d height=%d\n",game.camera,game.camera_height);
    if (game.world.camera_native && neo_camera_progress(&game.world.camera)!=old_progress && !(neo_camera_progress(&game.world.camera)%64))
        debugf("MSX camera progress=%u x=%d height=%d\n",neo_camera_progress(&game.world.camera),game.camera,game.camera_height);
    if (game.region_end && !old_region_end) debugf("MSX region end passed camera=%d\n",game.camera);
    if (game.world.camera_lock!=old_lock) debugf("MSX camera stop=%ld progress=%u\n",(long)game.world.camera_lock,neo_camera_progress(&game.world.camera));
    if (game.world.camera.auto_mode!=old_auto) debugf("MSX camera autoscroll=%u progress=%u\n",game.world.camera.auto_mode,neo_camera_progress(&game.world.camera));
    if (game.world.boss_active && !old_boss) debugf("MSX boss spawned camera=%d\n",game.camera);
    if (game.world.flag606!=old_flag606) debugf("MSX boss start signal 10C606=%02x camera=%d\n",game.world.flag606,game.camera);
    if (game.world.boss_active) {
        uint8_t phase=neo_boss_phase(&game.world.boss);unsigned hp=neo_boss_health(&game.world.boss);
        if (phase!=old_phase || hp/1000!=old_hp/1000) debugf("MSX boss phase=%u hp=%u hits=%u\n",phase,hp,game.world.boss_hits);
    }
    if (game.world.boss_unsupported!=old_boss_unsupported) debugf("MSX boss stopped node routine=%05lx label=%05lx\n",(unsigned long)game.world.boss_unsupported_routine,(unsigned long)game.world.boss_unsupported_label);
    if (game.mission_complete && !old_mission) debugf("MSX mission complete signal (018F92) camera=%d\n",game.camera);
    if (game.world.ga_flags.dome_signal && !old_flags.dome_signal) debugf("MSX domes signal 05486A group=%u camera=%d\n",game.world.ga_flags.dome_group,game.camera);
    if (game.world.ga_flags.player_handlers!=old_flags.player_handlers) debugf("MSX player handlers=%s camera=%d\n",game.world.ga_flags.player_handlers?"054B18":"restored",game.camera);
    if (game.world.ga_flags.barrel_patched && !old_flags.barrel_patched) debugf("MSX shop destroyed (0B6882 patch) camera=%d\n",game.camera);
    {   /* Armoured vehicle 087E36 of the 1304 stop: standing (+5F bit 7 clear) or not. */
        static bool armored_standing;bool standing=false;
        for (unsigned i=0;i<NEO_WORLD_GACTORS;i++) {
            const neo_gactor_t *a=&game.world.gactors[i];uint32_t r=ga_l(a,0);
            if (a->active && neo_armored_vehicle(r) && !(ga_b(a,0x5f)&0x80u)) standing=true;
        }
        if (standing && !armored_standing) debugf("MSX armoured vehicle 087E36 camera=%d\n",game.camera);
        if (!standing && armored_standing) debugf("MSX armoured vehicle destroyed camera=%d\n",game.camera);
        armored_standing=standing;
    }
    if (game.world.ga_flags.collapsed && !old_flags.collapsed) debugf("MSX domed building collapsed (0B58CC) camera=%d height=%d\n",game.camera,game.camera_height);
    if (game.backdrop_after && !old_backdrop) debugf("MSX backdrop palette 0B56CA camera=%d\n",game.camera);
    if (msx_layer_visible(2,game.camera) && !msx_layer_visible(2,old_camera)) debugf("MSX middle bank full speed=%d\n",game.camera);
    if (old_x<596*MSX_UNIT && game.x>=596*MSX_UNIT) debugf("MSX first roof x=%d\n",game.x/MSX_UNIT);
    if (!was_complete && game.complete) debugf("MSX mission complete camera=%d x=%d\n",game.camera,game.x);
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
    retro_frame_t effects={.camera={game.camera,-game.camera_height,0,320},.tick=game.ticks};
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
    scene_render_set_collapse(game.world.ga_flags.collapsed!=0,game.world.ga_flags.flash!=0);
    scene_render_set_patch(game.world.ga_flags.barrel_patched!=0);
    scene_render_back(game.camera,game.camera_height,game.backdrop_after,game.ticks);
    /* Sprites with attribute bit 2 (+15 bit 2) lie behind layers 3/4. Every
     * sprite is clipped to the arcade's 304x224 window, like the layers. */
    rdpq_set_scissor(8,8,312,232);
    for (unsigned i=0;i<game.world.draw_count;i++) if (game.world.draws[i].attributes&4u) draw_native(&game.world.draws[i],false);
    scene_render_front(game.camera,game.camera_height,game.backdrop_after,game.ticks);
    rdpq_set_scissor(8,8,312,232);
    vehicle_renderer_draw(&game.vehicle,game.camera,game.camera_height,game.vehicle_flash && (game.ticks&2));
    for (unsigned i=0;i<game.world.draw_count;i++) if (!(game.world.draws[i].attributes&4u)) draw_native(&game.world.draws[i],false);
    if (game.life.phase!=MARCO_GONE && !(game.invulnerable && game.life.phase==MARCO_ALIVE && ((game.ticks/3)&1)))
    {
        static neo_anim_draw_t parts[2*MARCO_VISUAL_DRAWS];static unsigned shown,missing;
        neo_anim_draw_t next[2*MARCO_VISUAL_DRAWS];unsigned n=marco_native?marco_visual_draws(&marco,next,2*MARCO_VISUAL_DRAWS):0;
        if (marco_native && game.life.phase!=MARCO_ALIVE) {
            uint16_t bx,by;msx_game_projected(&game,&bx,&by);
            n=marco_death_draw(&game.life,bx,by,game.facing,&next[0])?1u:0u;
        }
        /* A descriptor outside the audited catalog (one run-start pose,
         * 188930, never reached the screen in the captures) keeps the
         * previous pose for that update instead of a blank frame. */
        bool known=n>0;unsigned pose;
        for (unsigned i=0;i<n && known;i++) known=marco_renderer_find(next[i].descriptor,&pose);
        if (known) {for (unsigned i=0;i<n;i++) parts[i]=next[i];shown=n;}
        else if (n && !missing++) debugf("MSX marco descriptor outside the catalog: %06lx\n",(unsigned long)next[0].descriptor);
        n=shown;
        if (n) {
            /* Neo Geo raster rows 16..239 are the screen rows 8..231. */
            /* Dying in a kind 7/0x10 death: the burnt colours (marco-d2). */
            unsigned palette=game.life.phase!=MARCO_ALIVE && game.life.burnt?marco_renderer_burnt_palette():0;
            for (unsigned i=0;i<n;i++) marco_renderer_draw_native(&parts[i],0,-8,palette);
            scene_render_forget_texture();
        } else draw_frame(bank_id,frame,game.x/MSX_UNIT-game.camera,game.y/MSX_UNIT,game.facing<0,false);
    }
    for (unsigned owner=0;owner<2;owner++) {
        const msx_bullet_t *list=owner?game.enemy_bullets:game.bullets;
        unsigned n=owner?MSX_ENEMY_BULLETS:MSX_BULLETS;
        for (unsigned i=0;i<n;i++) if (list[i].active) {
            int x=list[i].x-game.camera;
            if (x>=-16 && x<336) draw_projectile(&list[i],owner);
        }
    }
    scene_render_foreground_fog();
    rdpq_set_scissor(0,0,320,240);
    msx_perf_mark(MSX_PERF_ACTORS);
    /* The original fix layer above every layer, on the 304x224 window. */
    rdpq_set_scissor(8,8,312,232);hud_renderer_draw(&game.hud);rdpq_set_scissor(0,0,320,240);
    scene_render_forget_texture();
    if (diagnostics) {
        scene_render_box(28,20,264,40,ink);scene_render_box(28,202,264,24,ink);
    }
    /* Stand-in for the letters actor 06090A while 0188F4 waits. */
    if (game.mission_end==MSX_END_LETTERS) scene_render_box(64,92,192,48,ink);
    // Fonts are last: their own uploads may replace TMEM texture/palette data.
    if (diagnostics) {
        scene_render_text(32,24,"MSX V10 - MISSION 1");
        char status[40];
        if (game.vehicle.active) snprintf(status,sizeof(status),"HP:%u VEHICLE:%u",game.health,game.vehicle.health);
        else snprintf(status,sizeof(status),"HP:%u REBELS:%u/%u CP:%s",game.health,game.kills,game.world.spawned,game.checkpoint?"YES":"NO");
        scene_render_text(32,38,status);
        scene_render_text(32,204,"MOVE / A:JUMP B:FIRE");scene_render_text(32,216,"START:LAB R:RESTART");
    }
    if (game.mission_end==MSX_END_LETTERS) scene_render_text(80,110,"MISSION COMPLETE");
    msx_perf_mark(MSX_PERF_HUD);
    scene_render_end();msx_perf_mark(MSX_PERF_END);
    msx_perf_counts(scene_render_draw_count(),neo_world_alive(&game.world));
    msx_perf_frame_end();
}
