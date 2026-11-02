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
#include "msx_diag.h"
#include "msx_stick.h"
#include "game_audio.h"
#include "msx_audio.h"
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
static int32_t entry_x,entry_y,entry_vx,entry_vy;static bool entry_descent;
/* reset_game has just begun this entry's descent (actor_prepare_descent
 * before actor_init): actor_begin_descent need not run it again (~60 ms of
 * the boot's mission handoff). */
static bool descent_fresh;
static unsigned animation_started;
static scene_clock_t simulation_clock;
enum { MSX_FRAME_STEPS=2 };   /* simulation steps a frame may run (the rest is deferred) */
static bool previous_a,previous_b,pending_a,pending_b,previous_start;
static int previous_animation=-1;
static const color_t ink={8,16,32,255};
static unsigned observed_projectiles;
static bool observed_wreck_contact;
/* Campaign Marco: original programs (marco_visual.c) drawn from the audited
 * descriptor parts; the historical six-state banks remain only as fallback
 * when romfs/marco is absent. */
static marco_visual_t marco;static bool marco_native;static unsigned marco_shots,marco_failures,marco_grenades,marco_cfires,marco_knives;
/* Reference cartridge of Mission 2 only (mslugx-m2-reference.z64, never a
 * release): the game jumps to this mission's first record at the first
 * frame instead of playing Mission 1. */
#ifndef MSX_REFERENCE_START_MISSION
#define MSX_REFERENCE_START_MISSION 0
#endif
/* Transition cartridge (mslugx-m1end.z64, never a release): the release game
 * whose Mission 1 ends (018F40, mission step 2) this many updates after the
 * landing, to check the mission end, the tally, the fade and Mission 2's
 * first stage (load, fade-in, MISSION 2 START, walk-in, music) quickly. */
#ifndef MSX_TEST_QUICK_END
#define MSX_TEST_QUICK_END 0
#endif
/* Mission packs (docs/MISION2.md "Fase 2"): the stage the flow selected,
 * loaded at its 0183D0. The pack holds the mission's stage records in play
 * order: index = 42FA (Mission 2: 0..2 for stages 1..3; Mission 3: its one
 * stage 4, docs/MISION3.md). The pack's descriptor must be the flow's. */
/* Frames keep presenting while a stage loads (the original loads behind
 * its wipe/fade, 3AED8/3AEF0, with the screen black): between chunk loads
 * the frame actor_draw holds is shown cleared to black every ~33 ms and the
 * next one is taken when the display frees it (display_try_get: the load
 * never waits for a blank; at the end, display_get). */
static surface_t **load_target;static bool load_shown_pending;static uint32_t load_shown_at,load_presents,load_gap;
static void load_idle(void) {
    msx_audio_idle();
    if (!load_target) return;
    if (load_shown_pending) {
        surface_t *s=display_try_get();
        if (s) {*load_target=s;load_shown_pending=false;}
        return;
    }
    if (TICKS_SINCE(load_shown_at)<TICKS_FROM_MS(33)) return;
    uint32_t gap=TICKS_SINCE(load_shown_at);if (gap>load_gap) load_gap=gap;
    rdpq_attach_clear(*load_target,NULL);rdpq_detach_show();
    load_shown_pending=true;load_shown_at=TICKS_READ();load_presents++;
}
static void load_idle_end(void) {
    if (load_target && load_shown_pending) {*load_target=display_get();load_shown_pending=false;}
}
static bool load_stage(void) {
    unsigned index=game.flow.record_index;
    scene_assets_set_load_idle(load_idle);load_shown_at=TICKS_READ();load_presents=load_gap=0;
    bool loaded=scene_stage_load(index);
    {uint32_t gap=TICKS_SINCE(load_shown_at);if (gap>load_gap) load_gap=gap;}
    load_idle_end();
    debugf("MSX stage load frames: %lu black frames shown, longest gap %lu us\n",(unsigned long)load_presents,(unsigned long)TICKS_TO_US(load_gap));
    if (!loaded) return false;
    const scene_stage_t *st=scene_stage();
    if (st->descriptor!=game.flow.descriptor) {debugf("MSX stage %u: pack descriptor %06lX is not the flow's %06lX (explicit stop)\n",index+1u,(unsigned long)st->descriptor,(unsigned long)game.flow.descriptor);return false;}
    neo_collision_set_grid(st->grid,st->grid_dynamic,st->unknown,st->columns,st->rows);neo_collision_set_ring(st->ring);
    /* 1805C on the mission's first record: the sound driver reset 03
     * (0120F8 via 01205E) and the mission actor's +80 = FFFF (01829E), so
     * the descriptor's music is requested again (018412..01842E). */
    if (game.flow.record_index==0) game_audio_mission_start();
    game_audio_stage_music(neo_mission_music(&game.flow));   /* 018412..01842E */
    scene_renderer_stage_changed();
    neo_world_stage_t ws={st->script,st->script_len,st->script_rom,st->camera_list,st->camera_len,st->camera_rom};
    /* 019604 record +2: the x the player is placed at (0x68 from the left). */
    int start_x=st->players_len>=4?(int)(int16_t)(st->players[2]<<8|st->players[3]):0x68;
    msx_game_begin_stage(&game,&ws,start_x,st->camera_max);
    /* 019604 +4: the stage's respawn entries (019658/0196E8, B33). */
    game.respawn_table=st->players;game.respawn_length=st->players_len;
    /* The exit kind (fade-in, wipe) and the stage loop run in the game's
     * updates (gameplay.c, stage_entry_step). */
    if (game.mission_start) debugf("MSX mission start 1805C: weapon=%u bombs=%u rescued=%u\n",(unsigned)marco_weapons_id(&game.world.weapons,0),
        (unsigned)marco_weapons_bomb_count(&game.world.weapons,0),(unsigned)game.world.ga_flags.rescued[0]);
    debugf("MSX mission %u stage %u start: record %06lX descriptor %06lX camera list %06lX script %06lX (%u bytes) walk-in to x=%d\n",
        (unsigned)game.flow.mission+1u,(unsigned)game.flow.stage,(unsigned long)game.flow.record,(unsigned long)game.flow.descriptor,
        (unsigned long)st->camera_rom,(unsigned long)st->script_rom,st->script_len,start_x);
    return true;
}
/* The flow's mission (42F4) from its first record: pack and stage 1. */
static bool enter_mission(void) {
    for (unsigned k=0;k<40 && !game.flow.load && game.flow.state!=NEO_MISSION_PLAYING;k++) neo_mission_update(&game.flow);
    if (!scene_mission_load(game.flow.index+1u)) return false;
    return load_stage();
}
static void leave_mission(void) {
    if (!scene_stage()) return;
    scene_mission_unload();neo_collision_set_grid(NULL,NULL,NULL,0,0);scene_renderer_stage_changed();
}
static void reset_game(void) {
    msx_diag_event(MSX_EV_RESET,game.flow.mission+1u,game.deaths,(uint32_t)game.camera);
    marco_visual_reset(&marco);marco_shots=0;marco_grenades=marco_cfires=marco_knives=0;
    observed_wreck_contact=false;
    leave_mission();
    descent_fresh=false;
    if (has_entry && entry_descent) {msx_game_begin_descent(&game,entry_x,entry_y,entry_vx,entry_vy);
#if !MSX_VEHICLE_REFERENCE
        descent_fresh=true;   // the vehicle reference below changes the game: begun again then
#endif
    }
    else if (has_entry) {bool valid=msx_game_begin_at(&game,entry_x,entry_y);assert(valid);(void)valid;}
    else msx_game_init(&game);
#if MSX_VEHICLE_REFERENCE
    msx_game_vehicle_reference(&game);vehicle_renderer_load();
    debugf("MSX vehicle reference encounter\n");
#endif
    game_audio_stage_music(0x0021);   /* Mission 1's descriptor +C */
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
static unsigned native_missing_descriptor,native_missing_palette;static uint32_t native_missing_tag;
static int native_screen_y(uint16_t projected_y) {
    /* The projected word already includes the scene root (camera height). */
    return MSX_SCREEN_BASE_Y-((int)(int16_t)projected_y-MSX_PROJECTED_BASE_Y);
}
static void draw_native_parts(const neo_world_draw_t *d,bool request_only);
/* Actor sprite parts may be deferred a frame when their chunk is not
 * resident and the frame's synchronous decode budget is spent. */
static void draw_native(const neo_world_draw_t *d,bool request_only) {
    if (!request_only) scene_render_set_deferrable(true);
    draw_native_parts(d,request_only);
    if (!request_only) scene_render_set_deferrable(false);
}
static void draw_native_parts(const neo_world_draw_t *d,bool request_only) {
    if (d->kind==NEO_WORLD_BOSS) {
        /* Boss entries: column cut and palette resource (boss_renderer.c). */
        neo_anim_draw_t a={d->descriptor,d->x,d->y,d->attributes,0};
        boss_render_draw(&a,d->palette,0,d->flash && (game.ticks&1),request_only);
        return;
    }
    const scene_native_descriptor_t *desc=scene_native_descriptor(d->descriptor);
    if (!desc) {native_missing_descriptor++;native_missing_tag=d->descriptor;return;}
    int palette=scene_native_palette(d->palette);
    if (palette<0) {native_missing_palette++;palette=scene_native_palette(0x20e);}
    /* Attribute bit 0 mirrors, bit 1 flips vertically (the H bullets'
     * 32 directions 22C3BE are 9 shapes with both flips; ignoring bit 1
     * drew the bullets going up-right as "\", MiSTer report). Parts are
     * placed as marco_renderer.c does: -(y+height)-1 when flipped. */
    bool mirror=(d->attributes&1u)!=0,flip_y=(d->attributes&2u)!=0;
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
            float y=flip_y?origin_y-(im->y+im->height)*sy-1:origin_y+im->y*sy;
            if (x+w<=0 || x>=320 || y+h<=0 || y>=240) continue;
            scene_render_sprite_scaled(&part,x,y,w,h,mirror,flip_y,d->flash && (game.ticks&1));
            continue;
        }
        int x=mirror?(int)(int16_t)d->x-(im->x+im->width)-1:(int)(int16_t)d->x+im->x;
        int y=flip_y?origin_y-(im->y+im->height)-1:origin_y+im->y;
        if (x+im->width<=0 || x>=320 || y+im->height<=0 || y>=240) continue;
        scene_render_sprite(&part,x,y,mirror,flip_y,d->flash && (game.ticks&1));
    }
}
static void draw_end(const neo_end_draw_t *e,bool request_only) {
    neo_world_draw_t d={e->descriptor,e->x,e->y,e->attributes,e->palette,false,NEO_WORLD_GACTOR,e->zoom_x,e->zoom_y};
    draw_native(&d,request_only);
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
static int heap_used_kb(void) { heap_stats_t h;sys_get_heap_stats(&h);return h.used/1024; }
void actor_init(void) {
    assertf(msx_game_layout==MSX_GAME_LAYOUT,"stale gameplay object: msx_game_t layout %08lx, actor.c %08lx (rebuild)",
        (unsigned long)msx_game_layout,(unsigned long)MSX_GAME_LAYOUT);
    uint32_t began=TICKS_READ();int k[6];k[0]=heap_used_kb();
    scene_assets_init();uint32_t t1=TICKS_READ();k[1]=heap_used_kb();scene_renderer_init();uint32_t t2=TICKS_READ();k[2]=heap_used_kb();reset_game();uint32_t t3=TICKS_READ();k[3]=heap_used_kb();
    marco_native=marco_renderer_load();uint32_t t4=TICKS_READ();k[4]=heap_used_kb();
    hud_renderer_init();uint32_t t5=TICKS_READ();k[5]=heap_used_kb();
    scene_assets_check_budget();
    debugf("MSX actor_init %lu us (scene %lu renderer %lu reset %lu marco %lu hud %lu) heap +%d KB (scene %d renderer %d reset %d marco %d hud %d)\n",(unsigned long)TICKS_TO_US(TICKS_READ()-began),
        (unsigned long)TICKS_TO_US(t1-began),(unsigned long)TICKS_TO_US(t2-t1),(unsigned long)TICKS_TO_US(t3-t2),(unsigned long)TICKS_TO_US(t4-t3),(unsigned long)TICKS_TO_US(t5-t4),
        k[5]-k[0],k[1]-k[0],k[2]-k[1],k[3]-k[2],k[4]-k[3],k[5]-k[4]);
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
/* The handoff at the start of the parachute descent (MAME 3860). */
void actor_prepare_descent(int32_t world_x,int32_t world_y,int32_t vx,int32_t vy) {
    has_entry=true;entry_descent=true;entry_x=world_x;entry_y=world_y;entry_vx=vx;entry_vy=vy;
}
void actor_begin_descent(int32_t world_x,int32_t world_y,int32_t vx,int32_t vy) {
    if (!(descent_fresh && has_entry && entry_descent && entry_x==world_x && entry_y==world_y && entry_vx==vx && entry_vy==vy))
        msx_game_begin_descent(&game,world_x,world_y,vx,vy);
    descent_fresh=false;
    has_entry=true;entry_descent=true;entry_x=world_x;entry_y=world_y;entry_vx=vx;entry_vy=vy;
    previous_animation=-1;pending_a=pending_b=previous_a=previous_b=false;
    scene_clock_reset(&simulation_clock);scene_render_forget_texture();
    game_audio_sync(&game);
    debugf("MSX mission handoff (descent) x=%ld y=%ld vx=%ld vy=%ld\n",(long)world_x,(long)world_y,(long)vx,(long)vy);
}
void actor_set_active(bool active) {
    (void)active;scene_clock_reset(&simulation_clock);pending_a=pending_b=false;
}
/* Window A (B26): the vehicle renderer, then the sprites without +15 bit
 * 2 in +1E order with player 1 at its +1E (msx_game_depth; it is near the
 * list head, so it draws under an actor of the same word): the parachute
 * canopy (+1E = the player's - 1) right before it and the landing marker
 * (+1E = FFFE) at its word. */
static void draw_marco(unsigned bank_id,unsigned frame);
static void draw_window_a(const uint8_t *order,unsigned bank_id,unsigned frame) {
    vehicle_renderer_draw(&game.vehicle,game.camera,game.camera_height,game.vehicle_flash && (game.ticks&2));
    uint16_t z=msx_game_depth(&game);bool player=false,marker=false;
    for (unsigned k=0;k<=game.world.draw_count;k++) {
        const neo_world_draw_t *d=k<game.world.draw_count?&game.world.draws[order[k]]:NULL;
        if (d && (d->attributes&4u)) continue;
        if (!player && (!d || d->z>=z)) {
            player=true;
            for (unsigned i=game.landing.marker_draws;i<game.landing.draw_count;i++) draw_end(&game.landing.draws[i],false);
            draw_marco(bank_id,frame);
        }
        if (player && !marker && (!d || d->z>=0xfffeu)) {
            marker=true;
            for (unsigned i=0;i<game.landing.marker_draws;i++) draw_end(&game.landing.draws[i],false);
        }
        if (d) draw_native(d,false);
    }
}
static void draw_marco(unsigned bank_id,unsigned frame) {
    /* Riding (0C309C): the vehicle's rider graphic (034F1A) draws the player. */
    if (game.life.phase!=MARCO_GONE && game.mount!=MSX_MOUNT_RIDE && !(game.invulnerable && game.life.phase==MARCO_ALIVE && ((game.ticks/3)&1)))
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
            /* The mummy (form 3): slots 63/64 hold its two palettes
             * (marco-mz2 palette dumps). The transformation's body
             * (1887E0/1887CC) keeps the player's own palettes: marco-mz2's
             * audit uses them in its first 37 frames (2101..2137, the
             * transformation), the mummy's from the end of it on. */
            bool mummy=game.life.phase==MARCO_ALIVE && game.world.weapon_player.form93==3 && !game.mummy_transform;
            for (unsigned i=0;i<n;i++) marco_renderer_draw_native(&parts[i],0,-8,mummy?marco_renderer_mummy_palette((parts[i].attributes>>8)==0x64):palette);
            scene_render_forget_texture();
        } else draw_frame(bank_id,frame,game.x/MSX_UNIT-game.camera,game.y/MSX_UNIT,game.facing<0,false);
    }
}
void actor_draw(surface_t *surface,joypad_inputs_t input) {
    msx_perf_frame_start();
    load_target=&surface;
    /* The diagnostic overlay (L) only on the internal reference cartridge:
     * the release game has no debug HUD (user request). */
    if (msx_reference_invulnerable && input.btn.l && !diagnostics_was_down) {
        diagnostics=!diagnostics;debugf("MSX diagnostics %s\n",diagnostics?"on":"off");
    }
    diagnostics_was_down=input.btn.l;
    int stick_h,stick_v;msx_stick_lever(input.stick_x,input.stick_y,&stick_h,&stick_v);
    int direction=(input.btn.d_right || stick_h>0)-(input.btn.d_left || stick_h<0);
    /* GAME OVER shown until the original hands the machine back to the BIOS:
     * 190 updates after player mode 3 (MAME go1: frames 4578..4957). The
     * ranking/attract that follow are not ported: the mission restarts. */
#if MSX_REFERENCE_START_MISSION>1
    {static bool jumped;
     if (!jumped) {
        jumped=true;game.flow.mission=MSX_REFERENCE_START_MISSION-1;neo_mission_start(&game.flow,MSX_REFERENCE_START_MISSION-1);
        debugf("MSX reference build: start at mission %u (not a release)\n",MSX_REFERENCE_START_MISSION);
        if (!enter_mission()) debugf("MSX mission %u pack not loaded: Mission 1 goes on\n",MSX_REFERENCE_START_MISSION);
        previous_animation=-1;scene_clock_reset(&simulation_clock);
     }}
#endif
    if (game.mission_end==MSX_END_DONE) {
        /* 01891E (neo_mission.c) selected the next mission's first stage:
         * its pack is loaded now (docs/MISION2.md "Fase 2"). Without the
         * pack (romfs/missionN.pack missing): explicit stop, Mission 1. */
        debugf("MSX mission end 018846 done (explicit stops=%u): 01891E -> mission %u record %06lX stage %u descriptor %06lX map %06lX script %04X\n",
               game.end_stops,(unsigned)game.flow.mission+1u,(unsigned long)game.flow.record,(unsigned)game.flow.stage,(unsigned long)game.flow.descriptor,
               (unsigned long)neo_mission_map_list(&game.flow),(unsigned)neo_mission_script_entry(&game.flow));
        if (!enter_mission()) {
            debugf("MSX mission %u not loaded (explicit stop): Mission 1 restarts\n",(unsigned)game.flow.mission+1u);
            reset_game();
        }
        previous_animation=-1;scene_clock_reset(&simulation_clock);
    }
    if (game.stage_load) {
        debugf("MSX stage end (018722) mission %u: next stage %u (exit hooks=%u, script lock releases=%u)\n",(unsigned)game.flow.mission+1u,(unsigned)game.flow.stage,game.stage_exit_hooks,game.stage_lock_releases);
        if (!load_stage()) {debugf("MSX stage %u not loaded (explicit stop): Mission 1 restarts\n",(unsigned)game.flow.stage);reset_game();}
        previous_animation=-1;scene_clock_reset(&simulation_clock);
    }
    if (game.game_over_frames>=MSX_GAME_OVER_UPDATES) {
        debugf("MSX game over: mission restart (continues=%u)\n",game.continues);
        reset_game();previous_animation=-1;scene_clock_reset(&simulation_clock);
    }
    if (input.btn.r && !restart_was_down) { reset_game();previous_animation=-1;scene_clock_reset(&simulation_clock);debugf("MSX reset\n"); }
    restart_was_down=input.btn.r;
    uint8_t old_weapon=marco_weapons_id(&game.world.weapons,0),old_bombs=marco_weapons_bomb_count(&game.world.weapons,0);
    unsigned old_mounts=game.mounts,old_dismounts=game.dismounts,old_mount_stops=game.mount_stops;
    unsigned old_vehicle_stops=game.world.vehicle_stops,old_weapon_stops=game.weapon_stops,old_grenades=game.grenades,old_grenade_stops=game.grenade_stops,old_served=game.world.pickup.taken;
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
    /* Reference build: Start in Mission 2 stage 3 toggles the test's route
     * planner (gameplay.h, msx_autopilot.h) instead of reaching the HUD. */
    bool start_press=input.btn.start && !previous_start;
    if (msx_reference_invulnerable && start_press && game.in_stage && game.flow.mission==1u && game.flow.stage==3u) {
        game.pilot.on=!game.pilot.on;start_press=false;debugf("MSX autopilot %s\n",game.pilot.on?"on":"off");
    }
    game.start_edge|=start_press;game.jump_edge|=input.btn.a && !previous_a;previous_start=input.btn.start;
    previous_a=input.btn.a;previous_b=input.btn.b;
    unsigned steps=scene_clock_steps(&simulation_clock,TICKS_READ(),TICKS_PER_SECOND);
    /* At most two steps per frame: the world updates on every second step
     * (gameplay.c), so a frame never runs two world updates (10-13 ms each
     * around camera 2336). The rest of the catch-up stays as debt for the
     * next frames (scene_clock_defer): no simulation step is lost. */
    if (steps>MSX_FRAME_STEPS) {scene_clock_defer(&simulation_clock,steps-MSX_FRAME_STEPS,TICKS_PER_SECOND);steps=MSX_FRAME_STEPS;}
    for (unsigned step=0;step<steps;step++) {
        uint32_t step_start=TICKS_READ();
        game.aim_up=input.btn.d_up || stick_v>0;
        game.crouch=input.btn.d_down || stick_v<0;
        game.grenade=input.btn.z;   /* bomb (the arcade's C button) */
        int pilot_direction=direction;bool pilot_jump=input.btn.a || pending_a;
        msx_game_autopilot(&game,&pilot_direction,&pilot_jump);
#if MSX_TEST_QUICK_END
        {static unsigned updates;
         if (game.flow.mission==0u && game.world_enabled && !game.complete && game.grounded && ++updates==MSX_TEST_QUICK_END) {
             game.world.mission_step=2;debugf("MSX test cartridge: Mission 1 ended now (not a release)\n");
         }}
#endif
        msx_game_update(&game,pilot_direction,pilot_jump,input.btn.b || pending_b);
        /* Dying, lying and the entrance (marco_death.c): the death and
         * entrance programs (0BF298/0BF318/0BF358/0BF3D8, 0BF498) draw;
         * the body/weapon programs restart with the new life. */
        if (game.life.phase==MARCO_DYING && !death_logged) {death_logged=true;debugf("MSX player death kind=%u\n",game.world.hit_kind);}
        if (game.life.phase==MARCO_ALIVE) death_logged=false;
        if (game.life.phase!=MARCO_ALIVE) {marco_visual_reset(&marco);marco_shots=game.shots;marco_grenades=game.grenades;marco_cfires=game.c_fires;marco_knives=game.knife_cuts;}
        if (!(game.ticks&1) && !game.complete && game.life.phase==MARCO_ALIVE && game.mount!=MSX_MOUNT_RIDE) {
            /* One original player update: step the body/weapon programs. */
            marco_visual_input_t mi={game.grounded,game.moving,game.crouching,game.aim_up,game.shots!=marco_shots,game.aim_down,
                game.jump_moving,game.facing,0,0,game.world.weapon_player.form93==1 || game.world.weapon_player.form93==3?game.world.weapon_player.form93:0u,-1,false};
            if (game.hgun.state==MARCO_HGUN_BURST || game.hgun.state==MARCO_HGUN_SWEEP) {
                mi.hgun_program=game.hgun.program;mi.hgun_start=game.hgun.t==1;
            }
            /* The throw (0C47BA: the grenade of this update) and the C's fire
             * state start the upper-body programs of marco_visual.c. */
            mi.thrown=game.grenades!=marco_grenades;marco_grenades=game.grenades;
            /* The R (id 3) runs the C's fire programs too (MAME, --give-rgun:
             * fire state 0C3D9A, weapon program 253A56). */
            mi.cgun=msx_game_c_programs(&game);mi.cgroup=msx_game_fire_group(&game);
            mi.cgun_fire=game.c_fires!=marco_cfires;marco_cfires=game.c_fires;
            mi.knife=game.knife_cuts!=marco_knives;marco_knives=game.knife_cuts;
            mi.hanging=game.descent==MSX_DESCENT_HANG;
            mi.transforming=game.mummy_transform!=0;mi.takeoff=game.mummy_takeoff!=0;
            msx_game_projected(&game,&mi.x,&mi.y);marco_shots=game.shots;
            marco_visual_step(&marco,&mi);
            if (marco.failures!=marco_failures) {debugf("MSX marco program stopped (unrecovered) count=%u\n",marco.failures);marco_failures=marco.failures;msx_diag_stop(MSX_STOP_MARCO,marco.failures,0);}
        }
        pending_a=pending_b=false;
        msx_perf_add(MSX_PERF_STEP,TICKS_READ()-step_start);
    }
    msx_perf_steps(steps);msx_perf_mark(MSX_PERF_SIM);
    /* MSX_DIAG: pools, stops and events of this frame's updates (read only). */
    msx_diag_game(&game,(uint16_t)(input.btn.c_left|input.btn.c_right<<1|input.btn.c_up<<2|input.btn.c_down<<3));
    game_audio_update(&game);
    /* A second AI service per frame, between the simulation and the draw:
     * a slow frame (two updates plus a full redraw) no longer drains both
     * buffers before the main loop's service. */
    {uint32_t a0=TICKS_READ();msx_audio_idle();msx_perf_add(MSX_PERF_AUDIO,TICKS_READ()-a0);}
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
    /* Every level-script record the world ran (tools/compare_m2_events.py). */
    for (unsigned i=0;i<game.world.spawn_log_count;i++)
        debugf("MSX script spawn record %06lx constructor %06lx native %u camera %u\n",(unsigned long)game.world.spawn_log[i].record,
            (unsigned long)game.world.spawn_log[i].constructor,(unsigned)game.world.spawn_log[i].created,(unsigned)neo_camera_progress(&game.world.camera));
    game.world.spawn_log_count=0;
    {static unsigned skipped;
     if (game.world.script.skipped!=skipped) {skipped=game.world.script.skipped;if (skipped) debugf("MSX reference: script record of an unported constructor skipped (count=%u) record %06lx constructor %06lx\n",skipped,(unsigned long)game.world.script.skipped_address,(unsigned long)(game.world.script.skipped_entry&0x7fffffffu));}}
    {static unsigned tower_logged;
     if (game.world.tower.drawn<tower_logged) tower_logged=0;
     for (;tower_logged<game.world.tower.drawn;tower_logged++)   /* stage 3 tower: collision and drawing share types[] */
        debugf("MSX tower piece %u type %u camera y=%d\n",tower_logged,game.world.tower.types[tower_logged],game.camera_height);}
    if (game.world.spawned!=old_spawned) debugf("MSX rebel spawned=%u alive=%u camera=%d\n",game.world.spawned,neo_world_alive(&game.world),game.camera);
    if (game.world.script.halted && !old_halted) {
        const neo_script_t *sc=&game.world.script;unsigned pc=sc->halt_address-sc->base;
        debugf("MSX script stopped at %06lx (records beyond are not ported): %02x %02x %02x %02x %02x %02x\n",(unsigned long)sc->halt_address,
            pc<sc->size?sc->code[pc]:0xffu,pc+1u<sc->size?sc->code[pc+1]:0xffu,pc+2u<sc->size?sc->code[pc+2]:0xffu,
            pc+3u<sc->size?sc->code[pc+3]:0xffu,pc+4u<sc->size?sc->code[pc+4]:0xffu,pc+5u<sc->size?sc->code[pc+5]:0xffu);
    }
    if (game.world.unsupported!=old_unsupported && game.world.boss_unsupported==old_boss_unsupported) debugf("MSX world unsupported routine=%05lx\n",(unsigned long)game.world.unsupported_routine);
    if (game.world.item_pickups!=old_pickups) debugf("MSX item pickup stopped explicitly at %05lx (untranslated)\n",(unsigned long)game.world.item_pickup_routine);
    if (game.world.pickup.taken!=old_served) debugf("MSX item served=%u score=%02x%02x%02x%02x\n",game.world.pickup.taken,
        game.world.pickup.score[0][0],game.world.pickup.score[0][1],game.world.pickup.score[0][2],game.world.pickup.score[0][3]);
    {
        /* Items by kind (neo_item.c counters): a line when one changes. */
        static uint16_t seen[3][NEO_ITEM_KINDS];
        const neo_pickup_env_t *pk=&game.world.pickup;
        for (unsigned k=0;k<NEO_ITEM_KINDS;k++)
            if (pk->item_created[k]!=seen[0][k] || pk->item_external[k]!=seen[1][k] || pk->item_taken[k]!=seen[2][k]) {
                seen[0][k]=pk->item_created[k];seen[1][k]=pk->item_external[k];seen[2][k]=pk->item_taken[k];
                debugf("MSX item kind %02x created=%u external=%u taken=%u\n",k,seen[0][k],seen[1][k],seen[2][k]);
            }
    }
    if (marco_weapons_id(&game.world.weapons,0)!=old_weapon) debugf("MSX weapon id=%x rounds=%u\n",marco_weapons_id(&game.world.weapons,0),marco_weapons_rounds(&game.world.weapons,0));
    if (marco_weapons_bomb_count(&game.world.weapons,0)!=old_bombs) debugf("MSX bombs=%u\n",marco_weapons_bomb_count(&game.world.weapons,0));
    if (game.weapon_stops!=old_weapon_stops && !old_weapon_stops) debugf("MSX weapon shot stopped explicitly: constructor %05lx not ported (pistol bullet fired)\n",(unsigned long)game.weapon_stop_constructor);
    if (game.world.vehicle_stops!=old_vehicle_stops) debugf("MSX vehicle stopped explicitly: script spawn %05lx not ported (not created)\n",(unsigned long)game.world.vehicle_stop_constructor);
    if (game.grenades!=old_grenades) debugf("MSX grenade thrown count=%u\n",game.grenades);
    if (game.mounts!=old_mounts) debugf("MSX player mounted vehicle kind=%u (0C3078) count=%u\n",game.world.vslots.slot[0].b[0x21],game.mounts);
    if (game.dismounts!=old_dismounts) debugf("MSX player left vehicle kind=%u (0C30F0) count=%u\n",game.world.vslots.slot[0].b[0x1d],game.dismounts);
    if (game.mount_stops!=old_mount_stops) debugf("MSX mounted player stopped explicitly at %06lx (untranslated)\n",(unsigned long)game.mount_stop);
    if (game.grenade_stops!=old_grenade_stops) debugf("MSX bomb type %u stopped explicitly (0C4756 entry not ported)\n",game.world.weapons.b[10]);
    if (native_missing_descriptor || native_missing_palette) {
        debugf("MSX native draw missing descriptor=%u palette=%u last=%06lX\n",native_missing_descriptor,native_missing_palette,(unsigned long)native_missing_tag);
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
    if (game.in_stage) {   /* mission pack stage: camera, player and coverage */
        static unsigned last;static int seen_camera=-1,seen_height=-99999;
        if (game.ticks-last>=120 || (game.camera/64!=seen_camera/64) || (game.camera_height/32!=seen_height/32)) {
            last=game.ticks;seen_camera=game.camera;seen_height=game.camera_height;
            debugf("MSX stage %u camera x=%d y=%d progress=%u mode=%u player x=%d y=%d grounded=%u unknown cells read=%u camera events unsupported=%u pit deaths=%u rescues=%u walls=%u pushes=%u kernel landings=%u\n",
                (unsigned)game.flow.stage,game.camera,game.camera_height,neo_camera_progress(&game.world.camera),game.world.camera.mode,
                game.x/MSX_UNIT,game.y/MSX_UNIT,game.grounded,neo_collision_unknown_reads(),game.world.camera.unsupported,game.pit_deaths,game.pit_rescues,game.wall_stops,game.solid_pushes,game.kernel_landings);
        }
    }
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
    {   /* Prisoner tally 0308C0 (neo_tally.c): start and end. */
        static uint8_t seen_end;static uint16_t rescued_at_start;
        if (game.mission_end!=seen_end) {
            if (game.mission_end==MSX_END_TALLY) {
                rescued_at_start=game.end_ram.rescued[0];
                debugf("MSX prisoner tally started (0308C0 prisoners=%u special=%u/%u)\n",rescued_at_start,game.end_ram.special[0][0],game.end_ram.special[0][1]);
            } else if (seen_end==MSX_END_TALLY) {
                const uint8_t *sc=game.world.pickup.score[0];
                debugf("MSX prisoner tally done (counted=%u left=%u score=%02x%02x%02x%02x nodes=%u stops=%u text stops=%u)\n",
                    rescued_at_start,game.end_ram.rescued[0],sc[0],sc[1],sc[2],sc[3],game.end.spawned,game.end.unsupported,game.end_ram.unsupported_text);
            }
            seen_end=game.mission_end;
        }
    }
    {   /* Stage entry (gameplay.c, stage_entry_step): the exit kind's fade-in
         * done when the stage loop 01860A starts; its letters. */
        static bool entering;
        bool now=game.in_stage && (game.flow.state==NEO_MISSION_STAGE_ENTRY || game.flow.state==NEO_MISSION_STAGE_EXIT);
        if (entering && !now && game.in_stage)
            debugf("MSX stage fade-in done: stage loop 01860A after %u updates (fade started at update %u, offset %d)%s\n",game.entry_updates,game.entry_fade_updates,
                (int)neo_fade_offset(&game.fade,0),game.entry_letters?"; MSX mission start letters 0607DE":"");
        entering=now;
    }
    {   /* MISSION COMPLETE letters: all arrived (root +61 = 0x11, 060974). */
        static bool logged;
        if (game.mission_end!=MSX_END_LETTERS) logged=false;
        else if (!logged && game.end.count && ga_l(&game.end.nodes[game.end.order[0]],0)==0x609a4) {
            logged=true;debugf("MSX mission end letters arrived (06090A nodes=%u draws=%u stops=%u)\n",game.end.spawned,game.end.draw_count,game.end.unsupported);
        }
    }
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
    for (unsigned i=0;i<game.end.draw_count;i++) draw_end(&game.end.draws[i],true);
    for (unsigned i=0;i<game.continue_sprites.draw_count;i++) draw_end(&game.continue_sprites.draws[i],true);
    for (unsigned i=0;i<game.landing.draw_count;i++) draw_end(&game.landing.draws[i],true);
    if (game.world.boss_active) scene_prefetch_boss();
    scene_prefetch_ahead(game.camera);
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
    load_target=NULL;
    scene_render_begin(surface);msx_perf_mark(MSX_PERF_BEGIN);
    scene_render_set_fade(neo_fade_offset(&game.fade,0),neo_fade_offset(&game.fade,1),neo_fade_offset(&game.fade,2));
    scene_render_set_collapse(game.world.ga_flags.collapsed!=0,game.world.ga_flags.flash!=0);
    scene_render_set_patch(game.world.ga_flags.barrel_patched!=0);
    scene_render_set_tower(game.world.tower.types,game.world.tower.drawn);
    scene_render_set_layer_offset(1,game.world.train.on,(int)neo_train_scroll(&game.world.train));
    /* B26: the frame's sprite windows and layers in their SCB slot order
     * (msx_draw_order, gameplay.c): window A holds the sprites in +1E
     * order, the player among them at its +1E (msx_game_depth), window B
     * the +15 bit 2 ones (e.g. Mission 2's exit marker 0B6F6A, +1E FFF1,
     * and the gate's frame 03A82E: over Marco when he walks through). */
    msx_draw_order_t layout=game.in_stage?msx_draw_order(game.flow.mission,game.flow.stage):msx_draw_order(0,0);
    scene_render_layers(game.camera,game.camera_height,game.backdrop_after,game.ticks,layout.back,true);
    /* Sprites with attribute bit 2 (+15 bit 2) lie in window B. Every
     * sprite is clipped to the arcade's 304x224 window, like the layers. */
    rdpq_set_scissor(8,8,312,232);
    /* 0113EA: the original sorts the frame's sprite-list entries by the
     * priority word +1E (two stable counting passes, low then high byte)
     * and lists them in that order, so a higher +1E draws over a lower one
     * (an item, +1E 0x8838, over the wagon's canvas, 0x8700); equal words
     * keep the actor-list order. */
    static uint8_t order[NEO_WORLD_DRAWS];
    {
        unsigned n=game.world.draw_count;
        for (unsigned i=0;i<n;i++) {
            unsigned j=i;
            while (j && game.world.draws[order[j-1]].z>game.world.draws[i].z) {order[j]=order[j-1];j--;}
            order[j]=(uint8_t)i;
        }
    }
    if (layout.normal_mid) draw_window_a(order,bank_id,frame);
    else for (unsigned k=0;k<game.world.draw_count;k++) {const neo_world_draw_t *d=&game.world.draws[order[k]];if (d->attributes&4u) draw_native(d,false);}
    scene_render_layers(game.camera,game.camera_height,game.backdrop_after,game.ticks,layout.front,false);
    rdpq_set_scissor(8,8,312,232);
    if (layout.normal_mid) {for (unsigned k=0;k<game.world.draw_count;k++) {const neo_world_draw_t *d=&game.world.draws[order[k]];if (d->attributes&4u) draw_native(d,false);}}
    else draw_window_a(order,bank_id,frame);
    for (unsigned owner=0;owner<2;owner++) {
        const msx_bullet_t *list=owner?game.enemy_bullets:game.bullets;
        unsigned n=owner?MSX_ENEMY_BULLETS:MSX_BULLETS;
        for (unsigned i=0;i<n;i++) if (list[i].active) {
            int x=list[i].x-game.camera;
            if (x>=-16 && x<336) draw_projectile(&list[i],owner);
        }
    }
    scene_render_foreground_fog();
    /* Mission-end actors (MISSION COMPLETE letters 06090A), created after
     * the mission actor: above the world's sprites, below the fix layer. */
    for (unsigned i=0;i<game.end.draw_count;i++) draw_end(&game.end.draws[i],false);
    /* The mission actor's CONTINUE? and digit (program 018120). */
    for (unsigned i=0;i<game.continue_sprites.draw_count;i++) draw_end(&game.continue_sprites.draws[i],false);
    rdpq_set_scissor(0,0,320,240);
    msx_perf_mark(MSX_PERF_ACTORS);
    /* The original fix layer above every layer, on the 304x224 window. */
    /* The fix layer's palettes are outside the fade (MAME runK: slots 0..9 keep their colours). */
    scene_render_set_fade(0,0,0);
    rdpq_set_scissor(8,8,312,232);hud_renderer_draw(&game.hud);rdpq_set_scissor(0,0,320,240);
    scene_render_forget_texture();
    if (diagnostics) {
        scene_render_box(28,20,264,40,ink);scene_render_box(28,202,264,24,ink);
    }
    // Fonts are last: their own uploads may replace TMEM texture/palette data.
    if (diagnostics) {
        scene_render_text(32,24,"MSX V10 - MISSION 1");
        char status[40];
        if (game.vehicle.active) snprintf(status,sizeof(status),"HP:%u VEHICLE:%u",game.health,game.vehicle.health);
        else snprintf(status,sizeof(status),"HP:%u REBELS:%u/%u CP:%s",game.health,game.kills,game.world.spawned,game.checkpoint?"YES":"NO");
        scene_render_text(32,38,status);
        scene_render_text(32,204,"MOVE / A:JUMP B:FIRE");scene_render_text(32,216,"START:LAB R:RESTART");
    }
    msx_perf_mark(MSX_PERF_HUD);
    scene_render_end();msx_perf_mark(MSX_PERF_END);
    msx_perf_counts(scene_render_draw_count(),neo_world_alive(&game.world));
    msx_perf_frame_end();
}
