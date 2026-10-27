#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_throw_actor.h"
#include "object_fixture.h"
#include "throw_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define SAME(f) assert(a->f==b->f)
static void equal(const neo_contact_actor_t *a,const neo_contact_actor_t *b) {
    SAME(x);SAME(y);SAME(vx);SAME(vy);SAME(projected_x);SAME(projected_y);SAME(cached_x);SAME(cached_y);
    SAME(parent_tag);SAME(kind_angle);SAME(field66);SAME(word1e);SAME(flags15);SAME(flags5f);SAME(mark);SAME(correction);
}
#undef SAME
static unsigned step,draws,palette_calls,active_palettes,released,sounds;
static bool palette(void *context,uint16_t asset,bool replace,uint8_t *handle) {
    (void)context;
    static const uint16_t assets[]={0x35d,0x364,0x365,0x36c};
    assert(palette_calls<COUNT(assets) && asset==assets[palette_calls]);
    assert(replace==(palette_calls==0 || palette_calls==3));
    if (replace) {released+=active_palettes;active_palettes=0;}
    *handle=palette_calls==3?(uint8_t)(object_cases[33].render.attributes>>8):object_cases[0].render.palettes[palette_calls];
    palette_calls++;active_palettes++;return true;
}
static void release(void *context) {(void)context;released+=active_palettes;active_palettes=0;}
static void emit(void *context,const neo_anim_draw_t *draw) {
    (void)context;assert(draws==step);const neo_anim_draw_t *expected=&object_cases[step].draw;
    assert(draw->descriptor==expected->descriptor && draw->x==expected->x && draw->y==expected->y && draw->attributes==expected->attributes && draw->z==expected->z);draws++;
}
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    neo_contact_actor_t child;neo_throw_spawn(&child,&throw_source,&throw_scene,throw_spawn_epoch);
    neo_anim_render_t graphics=object_cases[0].render;graphics.attributes=child.flags15;memset(graphics.palettes,0,sizeof(graphics.palettes));
    neo_throw_actor_t object;neo_throw_actor_start(&object,&child,throw_field6e,object_initial_flash,&graphics);
    neo_throw_services_t services={&object_programs[0],&object_programs[1],object_flight_rectangle,object_burst_rectangle,palette,release,NULL};
    for (step=0;step<COUNT(object_cases);step++) {
        neo_throw_frame_t frame={&throw_scene,&throw_players,&object_bounds,object_cases[step].video_phase,object_cases[step].region_code,
            object_cases[step].epoch,object_cases[step].other_contact};
        unsigned events;neo_throw_result_t result=neo_throw_actor_step(&object,&frame,&services,emit,NULL,&events);
        assert(result==(object_cases[step].deleted?NEO_THROW_DONE:NEO_THROW_MORE));sounds+=!!(events&NEO_THROW_AUDIO_0);
        assert(object.phase==object_cases[step].phase && object.active!=object_cases[step].deleted);
        equal(&object.body,&object_cases[step].body);
        assert(object.animation.cursor==object_cases[step].cursor && object.animation.timer==object_cases[step].timer && object.animation.speed==object_cases[step].speed);
        assert(object.animation.flash==object_cases[step].flash && object.animation.field18==object_cases[step].field18 && object.animation.field50_tag==object_cases[step].field50);
        assert(object.render.attributes==object_cases[step].attributes);
    }
    assert(draws==41 && palette_calls==4 && released==4 && active_palettes==0 && sounds==1);
    printf("PASS: native throw lifecycle: 41 draws, palette cycling, deferred burst, sound request and resource release\n");
#ifdef N64
    debugf("MSX OBJECT REGRESSION PASS steps=%u draws=%u palettes=%u sounds=%u\n",(unsigned)COUNT(object_cases),draws,palette_calls,sounds);
    for (;;) {wait_ms(100);}
#endif
}
