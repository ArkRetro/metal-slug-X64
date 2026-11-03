#include <libdragon.h>
#include <stdio.h>
#include "scene_renderer.h"
#include "fixed.h"
#include "sprites.h"
#include "actor.h"
#include "gameplay.h"
#include "msx_audio.h"
#include "game_audio.h"
#include "msx_perf.h"
#include "msx_diag.h"
#include "neogeo_logo.h"
#include "msx_ramfs.h"
#include "marco_renderer.h"
#if MSX_BOOT_CINEMATICS
#include "title_intro.h"
#include "cinematic.h"
#include "entry_meta.h"
#endif

/* The FIX/C tile lab is a diagnostic: it needs the raw 48 MiB C-ROM copy, so
 * only the separate lab cartridge (make mslugx-lab.z64) includes it. */
#ifndef MSX_GRAPHICS_LAB
#define MSX_GRAPHICS_LAB 0
#endif

static void fail(const char *message) {
    console_init();
    printf("MSX ERROR: %s\n", message);
    debugf("MSX ERROR: %s\n", message);
    for (;;) {}
}
#ifndef MSX_BOOT_CINEMATICS
#define MSX_BOOT_CINEMATICS 0
#endif
#if MSX_BOOT_CINEMATICS && !MSX_GRAPHICS_LAB
static bool rom_file(const char *path) { return dfs_rom_addr(path)!=0; }
/* One offline cinematic of the MAME capture at its own clock with its audio
 * (docs/MISSION_ENTRY.md): HOW TO PLAY, SOLDIER SELECT, the selection and the
 * parachute entry. Start skips it after half a second. hold >= 0 (-2: its last
 * frame): the movie stops on that frame (SOLDIER SELECT with P1 on Marco) until A or Start is
 * pressed, as the arcade waits for the player's choice; only Marco can be
 * chosen for now. Ends on its last frame; false when it cannot play. */
/* The game's own loading in the cinematics' spare time, after theirs
 * (docs/RENDIMIENTO.md, "Cinemáticas sin parones"): from SOLDIER SELECT on
 * (HOW TO PLAY's 2 MB ring and the next movie leave too little heap before)
 * the resident effects and Marco's parts, each only while the heap keeps
 * room for the movies still to be prepared. */
static bool game_preloading;
/* actor_init's allocations made already (its reserve in scene_assets.c):
 * Marco's parts and the HUD's S-ROM copy. */
static unsigned game_preloaded_bytes(void) {
    size_t fixed=0;msx_ramfs_data("fixed.bin",&fixed);
    return marco_renderer_preloaded_bytes()+(unsigned)fixed;
}
static int scene_early;   // 1: the scene cache may begin (the choice and the entry), 2: begun
static bool game_preload_idle(void) {
    if (!game_preloading) return false;
    uint32_t deadline=msx_cinematic_idle_deadline();
    if (!deadline) return false;
    int keep=msx_cinematic_heap_reserve();
    if (msx_ramfs_preload_idle(deadline,keep)) return true;
    if (marco_renderer_preload(deadline,keep)) return true;
    /* The scene cache, once no movie still has to take its memory: as many
     * slots as the heap has now, warmed (scene_assets_idle, its own blank
     * timing); the rest is added after the entry. */
    if (scene_early==1 && keep<=512*1024 && (int32_t)(deadline-TICKS_READ())>(int32_t)TICKS_FROM_US(9000)) {
        scene_assets_game_preloaded(game_preloaded_bytes());
        scene_early=scene_assets_begin_early(msx_cinematic_heap_bytes(),keep)?2:1;
        return false;
    }
    return scene_early==2 && scene_assets_idle();
}
static void spare_time(void) {
    while (msx_cinematic_prepare_idle()) {}   // this movie's frames ahead, then the next one's
    {uint32_t deadline=msx_cinematic_idle_deadline();if (deadline) scene_assets_plan_step(deadline);}
    while (game_preload_idle()) {}
}
/* SOLDIER SELECT: Z held while Marco is confirmed turns on the optional
 * life bar (not arcade; docs/PORT_STATUS.md, B42). */
static void lifebar_choice(bool z) {
    msx_lifebar_request=z;if (z) debugf("MSX LIFEBAR on\n");
}
/* loop: an optional loop body played after `sound` (msx_audio_open_loop),
 * sample-exact, until the next movie opens its own audio. */
static bool play_cinematic(const char *label,const char *movie,const char *sound,const char *loop,int hold,bool entry,const char *next) {
    uint32_t s0=TICKS_READ();
    if (!msx_cinematic_open(movie)) return false;
    uint32_t s1=TICKS_READ();
    // The next movie is read and prepared in this one's spare frame time.
    if (next && !msx_cinematic_prepare(next)) next=NULL;
    uint32_t s2=TICKS_READ();
    if (entry && (msx_cinematic_stats().frames!=MSX_ENTRY_FRAMES || msx_cinematic_pixels_crc()!=msx_entry_crc[0])) {msx_cinematic_close();return false;}
    if (hold==-2) hold=(int)msx_cinematic_stats().frames-1;   // its last frame (capture 3619)
    uint32_t s3=TICKS_READ();
    bool audio=msx_audio_open(MSX_AUDIO_INTRO,sound);
    bool looped=audio && loop && msx_audio_open_loop(MSX_AUDIO_INTRO,loop);
    if (audio) msx_audio_play(MSX_AUDIO_INTRO,looped,0.8f);
    if (loop) debugf("MSX CINE %s audio loop %s\n",label,looped?"on":"missing");
    debugf("MSX CINE %s setup open_us=%lu prepare_us=%lu check_us=%lu audio_us=%lu\n",label,(unsigned long)TICKS_TO_US(s1-s0),(unsigned long)TICKS_TO_US(s2-s1),(unsigned long)TICKS_TO_US(s3-s2),(unsigned long)TICKS_TO_US(TICKS_READ()-s3));
    unsigned rendered=0,unique=0,skipped=0,last_presented=~0u;bool previous=true,previous_a=true,start_skip=false,held=false;uint32_t draw_ticks=0,idle_ticks=0;
    uint32_t started=TICKS_READ();
    msx_frames_stage(label);
    debugf("MSX CINE %s start frames=%lu audio=%u hold=%d\n",label,(unsigned long)msx_cinematic_stats().frames,audio,hold);
    if (entry) debugf("MSX INTRO start source=3830 frames=%u audio=%u\n",MSX_ENTRY_FRAMES,audio);
    /* The frame due is taken right after the blank (display_get), then the
     * audio is mixed: mixing first (up to ~3 ms) pushed the blank's frame
     * choice late when the spare time before it was used, skipping frames. */
    for (;;) {
        joypad_poll();
        unsigned target=(unsigned)(((uint64_t)(uint32_t)(TICKS_READ()-started)*59185606u)/((uint64_t)TICKS_PER_SECOND*1000000u));
        joypad_buttons_t buttons=joypad_get_buttons(JOYPAD_PORT_1);
        bool start=buttons.start,press=(start && !previous) || (buttons.a && !previous_a);
        previous=start;previous_a=buttons.a;
        if (held) {
            /* Waiting on the choice: the clock stands on the hold frame. */
            if (press) {held=false;hold=-1;debugf("MSX CINE %s chosen audio=%u\n",label,(unsigned)msx_audio_playing(MSX_AUDIO_INTRO));lifebar_choice(buttons.z);}
            else {surface_t *surface=display_get();msx_audio_service();msx_cinematic_draw(surface);msx_frames_present();rendered++;
                spare_time();
                started=TICKS_READ()-(uint32_t)((uint64_t)msx_cinematic_stats().index*TICKS_PER_SECOND*1000000u/59185606u);continue;}
        } else if (target>=30 && press) {
            if (hold>=0) {hold=-1;debugf("MSX CINE %s chosen early\n",label);lifebar_choice(buttons.z);}   // the choice made before the cursor settles
            else {start_skip=true;break;}
        }
        uint32_t w0=TICKS_READ();surface_t *surface=display_get();uint32_t waited=TICKS_READ()-w0;
        // The frame shown is the one due when this framebuffer frees (as the title does).
        target=(unsigned)(((uint64_t)(uint32_t)(TICKS_READ()-started)*59185606u)/((uint64_t)TICKS_PER_SECOND*1000000u));
        if (hold>=0 && target>(unsigned)hold) target=(unsigned)hold;
        uint32_t a0=TICKS_READ();msx_audio_service();uint32_t audio_ticks=TICKS_READ()-a0;
        msx_cinematic_stats_t state=msx_cinematic_stats();unsigned work=0;
        while (state.index<target && state.index+1<state.frames && work<8) {
            if (!msx_cinematic_next()) {start_skip=true;break;}
            state=msx_cinematic_stats();work++;
        }
        uint32_t decoded=TICKS_READ()-a0-audio_ticks;
        if (last_presented!=state.index) {
            if (last_presented!=~0u && state.index>last_presented+1) {
                skipped+=state.index-last_presented-1;
                debugf("MSX CINE %s miss from=%u to=%lu wait_us=%lu decode_us=%lu audio_us=%lu draw_us=%lu idle_us=%lu\n",label,last_presented,(unsigned long)state.index,(unsigned long)TICKS_TO_US(waited),(unsigned long)TICKS_TO_US(decoded),(unsigned long)TICKS_TO_US(audio_ticks),(unsigned long)TICKS_TO_US(draw_ticks),(unsigned long)TICKS_TO_US(idle_ticks));
            }
            unique++;last_presented=state.index;
        }
        uint32_t d0=TICKS_READ();msx_cinematic_draw(surface);msx_frames_present();draw_ticks=TICKS_READ()-d0;rendered++;
        if (hold>=0 && state.index==(unsigned)hold) {held=true;debugf("MSX CINE %s waiting for A/Start at frame %d\n",label,hold);}
        // The last frame shown once, as the others: the next movie's first
        // frame follows on the next blank (the arcade's cut), so no spare-time
        // work after it.
        if (hold<0 && target+1>=state.frames && state.index+1==state.frames) break;
        uint32_t i0=TICKS_READ();
        spare_time();
        idle_ticks=TICKS_READ()-i0;
    }
    msx_cinematic_stats_t final=msx_cinematic_stats();
    debugf("MSX CINE %s cadence unique=%u skipped=%u\n",label,unique,skipped);
    if (entry) debugf("MSX INTRO cadence unique=%u skipped=%u\n",unique,skipped);
    debugf("MSX CINE %s complete frames=%lu rendered=%u crc=%08lx skipped=%u decode_max_us=%lu t=%lu ms\n",label,(unsigned long)final.frames,rendered,
        (unsigned long)msx_cinematic_pixels_crc(),start_skip,(unsigned long)TICKS_TO_US(final.max_decode_ticks),(unsigned long)get_ticks_ms());
    if (entry) debugf("MSX INTRO complete frames=%lu rendered=%u crc=%08lx skipped=%u decode_max_us=%lu t=%lu ms\n",(unsigned long)final.frames,rendered,
        (unsigned long)msx_cinematic_pixels_crc(),start_skip,(unsigned long)TICKS_TO_US(final.max_decode_ticks),(unsigned long)get_ticks_ms());
    uint32_t c0=TICKS_READ();
    rspq_wait();
    if (audio) msx_audio_stop(MSX_AUDIO_INTRO);
    msx_cinematic_close();
    debugf("MSX CINE %s close_us=%lu\n",label,(unsigned long)TICKS_TO_US(TICKS_READ()-c0));
    return true;
}
/* Arcade order after the title (MAME capture, coin and Start at the title):
 * HOW TO PLAY 770..3459, SOLDIER SELECT 3460..3619 (P1 on Marco from 3600),
 * the choice 3620..3829, then the parachute entry from 3830. The first three
 * are optional private files; the entry is required for the handoff. */
static const char *first_after_title(void) {
    if (rom_file("/howto.msxc")) return "rom:/howto.msxc";
    if (rom_file("/select.msxc")) return "rom:/select.msxc";
    if (rom_file("/confirm.msxc")) return "rom:/confirm.msxc";
    return "rom:/entry.msxc";
}
static bool entry_cinematic(void) {
    bool howto=rom_file("/howto.msxc"),select=rom_file("/select.msxc"),confirm=rom_file("/confirm.msxc");
    const char *after_select=confirm?"rom:/confirm.msxc":"rom:/entry.msxc";
    if (howto) play_cinematic("howto","rom:/howto.msxc","rom:/howto.wav64",NULL,-1,false,select?"rom:/select.msxc":after_select);
    msx_ramfs_preload("fixed.bin");game_audio_preload();game_preloading=true;
    /* B44: the arcade keeps playing the select music 0030 until the choice;
     * the movie's audio holds only its first 2.70 s. With the measured
     * continuation (tools/pack_select_music.py: the movie's audio + the
     * render of 0030 up to its loop splice, then the 50 s loop body) the
     * hold keeps the music; without it the movie's own audio, as before. */
    bool select_music=rom_file("/select-music-head.wav64") && rom_file("/select-music-loop.wav64");
    if (select) play_cinematic("select","rom:/select.msxc",select_music?"rom:/select-music-head.wav64":"rom:/select.wav64",
        select_music?"rom:/select-music-loop.wav64":NULL,-2,false,after_select);
    scene_early=1;
    if (confirm) play_cinematic("confirm","rom:/confirm.msxc","rom:/confirm.wav64",NULL,-1,false,"rom:/entry.msxc");
    if (scene_early==2) scene_assets_grow_early(msx_cinematic_heap_reserve());   // the choice's movie is gone: more slots to warm
    return play_cinematic("entry","rom:/entry.msxc","rom:/entry.wav64",NULL,-1,true,NULL);
}
#endif
#if MSX_GRAPHICS_LAB
static uint8_t fixed_rom[131072];
static uint8_t sprite_cache[SPRITES_PER_PAGE * 128];
static void load_page(FILE *file, unsigned page) {
    if (fseek(file, (long)page * sizeof(sprite_cache), SEEK_SET) != 0 ||
        fread(sprite_cache, 1, sizeof(sprite_cache), file) != sizeof(sprite_cache))
        fail("Lectura de pagina C");
    debugf("MSX C page %u loaded\n", page);
}
#endif
int main(void) {
    debug_init_emulog();
    msx_diag_init();   /* MSX_DIAG builds: ring dump on assertion/exception */
    console_init();
    if (dfs_init(DFS_DEFAULT_LOCATION) != DFS_ESUCCESS) fail("DragonFS");
#if MSX_GRAPHICS_LAB
    FILE *file = fopen("rom:/fixed.bin", "rb");
    if (!file || fread(fixed_rom, 1, sizeof(fixed_rom), file) != sizeof(fixed_rom))
        fail("fixed.bin ausente o incompleto");
    fclose(file);
    FILE *sprites = fopen("rom:/sprites.bin", "rb");
    if (!sprites || fseek(sprites, 0, SEEK_END) != 0 ||
        ftell(sprites) != (long)SPRITE_TILE_COUNT * 128)
        fail("sprites.bin ausente o incompleto");
    load_page(sprites, 0);
#endif
    rdpq_init();
#if !MSX_GRAPHICS_LAB
    msx_audio_init();
    console_close();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    joypad_init();
    scene_assets_mem("display");
    /* Boot order: NEOGEO logo (docs/LOGO_NEOGEO.md), X64 title and parachute
     * entry (docs/MISSION_ENTRY.md) when the cartridge carries them, then
     * Mission 1. Each private part is optional. */
    bool cinematics=false;
#if MSX_BOOT_CINEMATICS
    cinematics=rom_file("/title.msxc") && rom_file("/title-motion.wav64") && rom_file("/entry.msxc") && rom_file("/entry.wav64");
#endif
    if (cinematics) {
        /* The title preloads its 3.4 MB movie plus prepared frames (most of
         * the heap): the logo runs alone, the audio, the scene cache and its
         * warm-up come after the entry, sized from what is free then. */
        // The title's movie is read, validated and its heavy frames prepared
        // in the logo's spare frame time (before: ~2 s of black after it).
        if (msx_cinematic_prepare("rom:/title.msxc")) msx_neogeo_logo_set_idle(msx_cinematic_prepare_idle);
        msx_frames_stage("logo");
        msx_neogeo_logo();
        msx_neogeo_logo_set_idle(NULL);
    } else {
        // Gameplay audio first (what it keeps in RAM is counted), then the
        // scene cache, warmed in the logo's spare frame time.
        if (!game_audio_init()) debugf("MSX AUDIO incomplete: run tools/render_sound_commands.py and tools/pack_game_audio.py\n");
        scene_assets_mem("audio");
        scene_assets_begin();
        msx_neogeo_logo_set_idle(scene_assets_idle);
        msx_neogeo_logo();
        msx_neogeo_logo_set_idle(NULL);
        scene_assets_mem("logo done");
    }
    bool handoff=false;
#if MSX_BOOT_CINEMATICS
    if (cinematics) {
        if (!msx_title_intro(first_after_title())) debugf("MSX TITLE could not play\n");
        scene_assets_mem("title done");
        handoff=entry_cinematic();
        scene_assets_mem("entry done");
        scene_assets_game_preloaded(game_preloaded_bytes());
        if (!game_audio_init()) debugf("MSX AUDIO incomplete: run tools/render_sound_commands.py and tools/pack_game_audio.py\n");
        scene_assets_mem("audio");
    }
#endif
#if MSX_BOOT_CINEMATICS && MSX_ENTRY_DESCENT
    if (handoff) actor_prepare_descent(MSX_ENTRY_X,MSX_ENTRY_Y,MSX_ENTRY_VX,MSX_ENTRY_VY);
#endif
    actor_init();
#if MSX_BOOT_CINEMATICS
    // Marco where the entry's last frame leaves him (measured MAME handoff).
#if MSX_ENTRY_DESCENT
    if (handoff) actor_begin_descent(MSX_ENTRY_X,MSX_ENTRY_Y,MSX_ENTRY_VX,MSX_ENTRY_VY);
#else
    if (handoff && !actor_begin_mission(MSX_ENTRY_X,MSX_ENTRY_Y)) fail("Soporte al aterrizar");
#endif
#endif
    (void)handoff;
#else
    actor_init();
    console_close();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    joypad_init();
#endif
#if !MSX_GRAPHICS_LAB
    // The handoff and the first two seconds of play (the descent), then the
    // next eight (landing and the first soldiers), logged apart.
    msx_frames_stage("game-first2s");unsigned game_logged=0;
    for (;;) {
        if (game_logged<2 && msx_frames_count()>=(game_logged?480u:120u)) msx_frames_stage(game_logged++?"game-later":"game-next8s");
        uint32_t audio_started=TICKS_READ();msx_audio_service();msx_perf_add(MSX_PERF_AUDIO,TICKS_READ()-audio_started);
        joypad_poll();
        surface_t *surface=msx_display_get();
        joypad_inputs_t input=joypad_get_inputs(JOYPAD_PORT_1);
        input.btn=joypad_get_buttons(JOYPAD_PORT_1);
        actor_draw(surface,input);
    }
#else
    unsigned pages[2] = {0, 0};
    bool sprite_mode = true, joined = false, inverted = false;
    bool actor_mode = true;
    uint32_t palette[16];
    for (;;) {
        joypad_poll();
        joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);
        if (pressed.start) { actor_mode = !actor_mode;actor_set_active(actor_mode); }
        if (actor_mode) {
            surface_t *surface=msx_display_get();
            joypad_inputs_t input=joypad_get_inputs(JOYPAD_PORT_1);
            input.btn=joypad_get_buttons(JOYPAD_PORT_1);
            actor_draw(surface,input);
            continue;
        }
        if (pressed.a) sprite_mode = !sprite_mode;
        if (pressed.z) joined = !joined;
        if (pressed.b) inverted = !inverted;
        unsigned mode = sprite_mode ? 1 : 0;
        unsigned count = sprite_mode ? SPRITE_PAGE_COUNT : 16;
        int delta = 0;
        if (pressed.d_right) delta++;
        if (pressed.d_left) delta--;
        if (pressed.d_up) delta += 16;
        if (pressed.d_down) delta -= 16;
        if (pressed.r) delta += 256;
        if (pressed.l) delta -= 256;
        unsigned old_page = pages[mode];
        pages[mode] = (unsigned)((int)pages[mode] + (int)count + delta % (int)count) % count;
        unsigned page = pages[mode];
        if (sprite_mode && old_page != page) load_page(sprites, page);
        for (unsigned i = 0; i < 16; i++) {
            unsigned shade = (inverted ? 15 - i : i) * 17;
            palette[i] = graphics_make_color(shade, shade, shade, 255);
        }
        surface_t *surface = msx_display_get();
        uint32_t black = graphics_make_color(0, 0, 0, 255);
        uint32_t white = graphics_make_color(255, 255, 255, 255);
        graphics_fill_screen(surface, black);
        graphics_set_color(white, black);
        graphics_draw_text(surface, 32, 24, "MSX v10 - Graphics lab");
        char status[48];
        snprintf(status, sizeof(status), "%s page %u/%u", sprite_mode ? "C16" : "FIX", page + 1, count);
        graphics_draw_text(surface, 32, 36, status);
        unsigned tiles = sprite_mode ? 64 : 256;
        unsigned size = sprite_mode ? 16 : 8;
        unsigned cols = sprite_mode ? 8 : 16;
        unsigned step_x = joined ? size : (sprite_mode ? 32 : 16);
        unsigned step_y = joined ? size : (sprite_mode ? 18 : 9);
        unsigned origin_x = joined ? 96 : 32;
        for (unsigned t = 0; t < tiles; t++)
            for (unsigned y = 0; y < size; y++)
                for (unsigned x = 0; x < size; x++) {
                    unsigned pen = sprite_mode ? sprite_pixel(sprite_cache + t * 128, x, y)
                        : fixed_pixel(fixed_rom, page * 256 + t, x, y);
                    graphics_draw_pixel(surface, origin_x + (t % cols) * step_x + x,
                                        48 + (t / cols) * step_y + y, palette[pen]);
                }
        snprintf(status, sizeof(status), "Tiles %u-%u", page * tiles, page * tiles + tiles - 1);
        graphics_draw_text(surface, 32, 214, status);
        graphics_draw_text(surface, 32, 202, "A:FIX/C Z:join B:invert");
        display_show(surface);
    }
#endif
}
