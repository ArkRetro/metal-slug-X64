#include <libdragon.h>
#include <stdio.h>
#include "actor.h"
#include "cinematic.h"
#include "msx_audio.h"
#include "entry_meta.h"
#include "title_intro.h"
#ifdef MSX_FRONTEND
#include "frontend.h"
#endif
static void fail(const char *message) {
    console_init();printf("MSX ERROR: %s\n",message);debugf("MSX ERROR: %s\n",message);
    for (;;) wait_ms(100);
}
int main(void) {
    debug_init_emulog();console_init();
    if (dfs_init(DFS_DEFAULT_LOCATION)!=DFS_ESUCCESS) fail("DragonFS");
    if (get_memory_size()<8*1024*1024) fail("Se requiere Expansion Pak (8 MiB)");
    rdpq_init();msx_audio_init();
    console_close();display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);joypad_init();
    if (!msx_title_intro()) fail("Presentacion inicial");
#ifdef MSX_FRONTEND
    if (!msx_frontend_after_title()) fail("Tutorial y seleccion");
#endif
    if (!msx_cinematic_open("rom:/entry.msxc")) fail("Entrada cinematica");
    if (msx_cinematic_stats().frames!=MSX_ENTRY_FRAMES || msx_cinematic_pixels_crc()!=msx_entry_crc[0]) fail("CRC primer frame");
    if (!msx_audio_open(MSX_AUDIO_INTRO,"rom:/entry.wav64")) fail("Audio de entrada");
    msx_audio_play(MSX_AUDIO_INTRO,false,0.8f);
    unsigned rendered=0,unique=0,skipped=0,last_presented=~0u;uint32_t started=TICKS_READ();
    debugf("MSX INTRO start source=3830 frames=%u\n",MSX_ENTRY_FRAMES);
    for (;;) {
        joypad_poll();msx_audio_service();surface_t *surface=display_get();
        unsigned target=(unsigned)(((uint64_t)(uint32_t)(TICKS_READ()-started)*59185606u)/((uint64_t)TICKS_PER_SECOND*1000000u));
        unsigned work=0;msx_cinematic_stats_t state=msx_cinematic_stats();
        while (state.index<target && state.index+1<state.frames && work<8) {
            if (!msx_cinematic_next()) fail("Paquete cinematica");
            state=msx_cinematic_stats();work++;
        }
        if (last_presented!=state.index) {
            if (last_presented!=~0u && state.index>last_presented+1) skipped+=state.index-last_presented-1;
            unique++;last_presented=state.index;
        }
        msx_cinematic_draw(surface);rendered++;
        if (target>=state.frames && state.index+1==state.frames) break;
    }
    debugf("MSX INTRO cadence unique=%u skipped=%u\n",unique,skipped);
    msx_cinematic_stats_t final=msx_cinematic_stats();uint32_t crc=msx_cinematic_pixels_crc();
    if (crc!=msx_entry_crc[MSX_ENTRY_FRAMES-1]) fail("CRC ultimo frame");
    debugf("MSX INTRO complete frames=%lu rendered=%u crc=%08lx reads=%lu audio=%u\n",(unsigned long)final.frames,rendered,(unsigned long)crc,(unsigned long)final.read_bytes,msx_audio_playing(MSX_AUDIO_INTRO));
    debugf("MSX INTRO timing decode_max_us=%lu decode_mean_us=%lu draw_max_us=%lu\n",(unsigned long)TICKS_TO_US(final.max_decode_ticks),(unsigned long)TICKS_TO_US(final.total_decode_ticks/final.frames),(unsigned long)TICKS_TO_US(final.max_draw_ticks));
#ifdef MSX_INTRO_PREVIEW
    debugf("MSX INTRO preview complete\n");
    for (;;) {msx_audio_service();joypad_poll();msx_cinematic_draw(display_get());}
#endif
    msx_cinematic_close();
    actor_init();
    if (!actor_begin_mission(MSX_ENTRY_X,MSX_ENTRY_Y)) fail("Soporte al aterrizar");
    for (;;) {
        msx_audio_service();joypad_poll();
        joypad_inputs_t input=joypad_get_inputs(JOYPAD_PORT_1);input.btn=joypad_get_buttons(JOYPAD_PORT_1);
        actor_draw(display_get(),input);
    }
}
