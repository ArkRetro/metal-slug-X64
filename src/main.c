#include <libdragon.h>
#include <stdio.h>
#include "scene_renderer.h"
#include "fixed.h"
#include "sprites.h"
#include "actor.h"
#include "msx_audio.h"
#include "game_audio.h"
#include "msx_perf.h"

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
    // Gameplay audio: optional ROM-derived assets (docs/AUDIO_JUEGO.md).
    msx_audio_init();
    if (!game_audio_init()) debugf("MSX AUDIO incomplete: run tools/render_sound_commands.py and tools/pack_game_audio.py\n");
#endif
    actor_init();
    console_close();
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_RESAMPLE);
    joypad_init();
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
