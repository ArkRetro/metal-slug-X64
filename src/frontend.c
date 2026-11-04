#include <libdragon.h>
#include "frontend.h"
#include "cinematic.h"
#include "msx_audio.h"
#include "frontend_meta.h"
#include "hud.h"

typedef struct {
    const char *name, *movie, *audio;
    unsigned frames;
    uint32_t first_crc, last_crc;
    bool skippable, confirm;
} frontend_segment_t;

static bool confirm_down(void) {
    joypad_buttons_t buttons=joypad_get_buttons(JOYPAD_PORT_1);
    return buttons.a || buttons.start;
}

static void marco_footer(void) {
    // Replace the arcade credit strip with a native label inside VI safe area.
    // The outer eight-pixel margin is cropped by some display configurations.
    rdpq_set_scissor(0,0,320,240);rdpq_set_mode_fill(RGBA32(0,0,0,255));
    rdpq_fill_rectangle(8,220,312,232);
    msx_hud_text(16,222,"MARCO ONLY / B FIRE / A JUMP");
}
static void draw(const frontend_segment_t *segment,surface_t *surface) {
    msx_cinematic_draw_overlay(surface,(segment->confirm || segment->skippable)?marco_footer:NULL);
}
static bool play(const frontend_segment_t *segment) {
    if (!msx_cinematic_open(segment->movie)) return false;
    unsigned count=msx_cinematic_stats().frames;
    uint32_t crc=msx_cinematic_pixels_crc();
    if (count!=segment->frames || crc!=segment->first_crc) goto error;
    if (!msx_audio_open(MSX_AUDIO_MUSIC,segment->audio)) goto error;
    draw(segment,display_get());
    if (!msx_audio_play(MSX_AUDIO_MUSIC,false,.8f)) goto error;
    debugf("MSX FRONTEND %s start frames=%u crc=%08lx audio=%u\n",segment->name,count,
           (unsigned long)crc,msx_audio_playing(MSX_AUDIO_MUSIC));
    uint32_t started=TICKS_READ();
    unsigned unique=0,dropped=0,last=~0u,rendered=0;
    bool previous=true,skipped=false;
    for (;;) {
        msx_audio_service();joypad_poll();
        bool down=confirm_down();
        unsigned target=(unsigned)((uint64_t)(uint32_t)(TICKS_READ()-started)*59185606u/
                                  ((uint64_t)TICKS_PER_SECOND*1000000u));
        skipped=segment->skippable && target>=30 && down && !previous;
        previous=down;
        if (skipped) break;
        surface_t *surface=display_get();
        target=(unsigned)((uint64_t)(uint32_t)(TICKS_READ()-started)*59185606u/
                          ((uint64_t)TICKS_PER_SECOND*1000000u));
        msx_cinematic_stats_t state=msx_cinematic_stats();
        unsigned work=0;
        while (state.index<target && state.index+1<count && work++<8) {
            if (!msx_cinematic_next()) goto error;
            state=msx_cinematic_stats();
        }
        if (last!=state.index) {
            if (last==~0u) dropped+=state.index;
            else if (state.index>last+1) dropped+=state.index-last-1;
            unique++;last=state.index;
        }
        draw(segment,surface);rendered++;
        if (target>=count && state.index+1==count) break;
    }
    msx_cinematic_stats_t final=msx_cinematic_stats();
    debugf("MSX FRONTEND %s timing decode_max_us=%lu draw_max_us=%lu\n",segment->name,
           (unsigned long)TICKS_TO_US(final.max_decode_ticks),(unsigned long)TICKS_TO_US(final.max_draw_ticks));
    crc=msx_cinematic_pixels_crc();
    if (!skipped && crc!=segment->last_crc) goto error;
    debugf("MSX FRONTEND %s complete rendered=%u unique=%u dropped=%u skipped=%u crc=%08lx\n",
           segment->name,rendered,unique,dropped,skipped,(unsigned long)crc);
    if (segment->confirm) {
        /* The captured selected actor is Marco. Accept no direction input that
         * would promise a character whose gameplay has not been integrated. */
        debugf("MSX FRONTEND select waiting character=MARCO\n");
        previous=true;
        for (;;) {
            msx_audio_service();joypad_poll();
            bool down=confirm_down();
            if (down && !previous) break;
            previous=down;
            draw(segment,display_get());
        }
        debugf("MSX FRONTEND select confirmed character=MARCO\n");
    }
    msx_audio_stop(MSX_AUDIO_MUSIC);msx_cinematic_close();
    return true;
error:
    msx_audio_stop(MSX_AUDIO_MUSIC);msx_cinematic_close();
    return false;
}

bool msx_frontend_after_title(void) {
    static const frontend_segment_t segments[]={
        {"tutorial","rom:/frontend/tutorial.msxc","rom:/frontend/tutorial.wav64",
         MSX_FRONTEND_TUTORIAL_FRAMES,MSX_FRONTEND_TUTORIAL_FIRST_CRC,
         MSX_FRONTEND_TUTORIAL_LAST_CRC,true,false},
        {"select","rom:/frontend/select.msxc","rom:/frontend/select.wav64",
         MSX_FRONTEND_SELECT_FRAMES,MSX_FRONTEND_SELECT_FIRST_CRC,
         MSX_FRONTEND_SELECT_LAST_CRC,false,true},
        {"select-confirm","rom:/frontend/select-confirm.msxc","rom:/frontend/select-confirm.wav64",
         MSX_FRONTEND_SELECT_CONFIRM_FRAMES,MSX_FRONTEND_SELECT_CONFIRM_FIRST_CRC,
         MSX_FRONTEND_SELECT_CONFIRM_LAST_CRC,false,false}
    };
    bool result=true;
    for (unsigned i=0;i<sizeof(segments)/sizeof(segments[0]);i++)
        if (!play(&segments[i])) {result=false;break;}
    return result;
}
