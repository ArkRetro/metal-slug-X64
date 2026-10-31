#include <libdragon.h>
#include <stdio.h>
#include "scene_renderer.h"
#include "fixed.h"
#include "sprites.h"
#include "actor.h"
#include "msx_audio.h"
#include "game_audio.h"
#include "msx_perf.h"
#include "neogeo_logo.h"
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
/* Parachute entry (docs/MISSION_ENTRY.md): the offline frames of the MAME
 * capture at their own clock with its audio; Start skips it (after half a
 * second). Ends on the handoff frame; false when it cannot play. */
static bool entry_cinematic(void) {
    if (!msx_cinematic_open("rom:/entry.msxc")) return false;
    if (msx_cinematic_stats().frames!=MSX_ENTRY_FRAMES || msx_cinematic_pixels_crc()!=msx_entry_crc[0]) {msx_cinematic_close();return false;}
    bool audio=msx_audio_open(MSX_AUDIO_INTRO,"rom:/entry.wav64");
    if (audio) msx_audio_play(MSX_AUDIO_INTRO,false,0.8f);
    unsigned rendered=0,unique=0,skipped=0,last_presented=~0u;bool previous=true,start_skip=false;uint32_t draw_ticks=0;
    uint32_t started=TICKS_READ();
    debugf("MSX INTRO start source=3830 frames=%u audio=%u\n",MSX_ENTRY_FRAMES,audio);
    for (;;) {
        uint32_t a0=TICKS_READ();joypad_poll();msx_audio_service();uint32_t audio_ticks=TICKS_READ()-a0;
        unsigned target=(unsigned)(((uint64_t)(uint32_t)(TICKS_READ()-started)*59185606u)/((uint64_t)TICKS_PER_SECOND*1000000u));
        bool start=joypad_get_buttons(JOYPAD_PORT_1).start;
        if (target>=30 && start && !previous) {start_skip=true;break;}
        previous=start;
        uint32_t w0=TICKS_READ();surface_t *surface=display_get();uint32_t waited=TICKS_READ()-w0;
        // The frame shown is the one due when this framebuffer frees (as the title does).
        target=(unsigned)(((uint64_t)(uint32_t)(TICKS_READ()-started)*59185606u)/((uint64_t)TICKS_PER_SECOND*1000000u));
        msx_cinematic_stats_t state=msx_cinematic_stats();unsigned work=0;
        while (state.index<target && state.index+1<state.frames && work<8) {
            if (!msx_cinematic_next()) {start_skip=true;break;}
            state=msx_cinematic_stats();work++;
        }
        uint32_t decoded=TICKS_READ()-w0-waited;
        if (last_presented!=state.index) {
            if (last_presented!=~0u && state.index>last_presented+1) {
                skipped+=state.index-last_presented-1;
                debugf("MSX INTRO miss from=%u to=%lu wait_us=%lu decode_us=%lu audio_us=%lu draw_us=%lu\n",last_presented,(unsigned long)state.index,(unsigned long)TICKS_TO_US(waited),(unsigned long)TICKS_TO_US(decoded),(unsigned long)TICKS_TO_US(audio_ticks),(unsigned long)TICKS_TO_US(draw_ticks));
            }
            unique++;last_presented=state.index;
        }
        uint32_t d0=TICKS_READ();msx_cinematic_draw(surface);draw_ticks=TICKS_READ()-d0;rendered++;
        if (target>=state.frames && state.index+1==state.frames) break;
    }
    msx_cinematic_stats_t final=msx_cinematic_stats();
    debugf("MSX INTRO cadence unique=%u skipped=%u\n",unique,skipped);
    debugf("MSX INTRO complete frames=%lu rendered=%u crc=%08lx skipped=%u decode_max_us=%lu t=%lu ms\n",(unsigned long)final.frames,rendered,
        (unsigned long)msx_cinematic_pixels_crc(),start_skip,(unsigned long)TICKS_TO_US(final.max_decode_ticks),(unsigned long)get_ticks_ms());
    rspq_wait();
    if (audio) msx_audio_stop(MSX_AUDIO_INTRO);
    msx_cinematic_close();
    return true;
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
        if (!msx_title_intro()) debugf("MSX TITLE could not play\n");
        scene_assets_mem("title done");
        handoff=entry_cinematic();
        scene_assets_mem("entry done");
        if (!game_audio_init()) debugf("MSX AUDIO incomplete: run tools/render_sound_commands.py and tools/pack_game_audio.py\n");
        scene_assets_mem("audio");
    }
#endif
    actor_init();
#if MSX_BOOT_CINEMATICS
    // Marco where the entry's last frame leaves him (measured MAME handoff).
    if (handoff && !actor_begin_mission(MSX_ENTRY_X,MSX_ENTRY_Y)) fail("Soporte al aterrizar");
#endif
    (void)handoff;
#else
    actor_init();
    console_close();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    joypad_init();
#endif
#if !MSX_GRAPHICS_LAB
    for (;;) {
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
