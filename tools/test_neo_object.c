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
typedef struct { unsigned calls,fail,acquired,active,released,release_calls; } fault_owner_t;
static bool fault_palette(void *context,uint16_t asset,bool replace,uint8_t *handle) {
    fault_owner_t *owner=context;
    assert(asset==0x35d || asset==0x364 || asset==0x365 || asset==0x36c);
    if (++owner->calls==owner->fail) return false;
    if (replace) {owner->released+=owner->active;owner->active=0;}
    owner->acquired++;owner->active++;*handle=(uint8_t)owner->calls;return true;
}
static void fault_release(void *context) {
    fault_owner_t *owner=context;owner->release_calls++;owner->released+=owner->active;owner->active=0;
}
static void fault_setup(neo_throw_actor_t *actor,fault_owner_t *owner,neo_throw_services_t *service,neo_throw_frame_t *frame) {
    *owner=(fault_owner_t){0};neo_contact_actor_t seed;
    neo_throw_spawn(&seed,&throw_source,&throw_scene,throw_spawn_epoch);
    neo_anim_render_t graphics=object_cases[0].render;graphics.attributes=seed.flags15;memset(graphics.palettes,0,sizeof(graphics.palettes));
    neo_throw_actor_start(actor,&seed,throw_field6e,object_initial_flash,&graphics);
    *service=(neo_throw_services_t){&object_programs[0],&object_programs[1],object_flight_rectangle,object_burst_rectangle,fault_palette,fault_release,owner};
    *frame=(neo_throw_frame_t){&throw_scene,&throw_players,&object_bounds,object_cases[0].video_phase,0,object_cases[0].epoch,false};
}
static void fault_failed(neo_throw_actor_t *actor,fault_owner_t *owner,neo_throw_services_t *service,neo_throw_frame_t *frame,unsigned events) {
    assert(!actor->active && !actor->owns_resources && actor->phase==NEO_THROW_FAILED);
    assert(!events && !owner->active && owner->released==owner->acquired);
    unsigned calls=owner->calls,releases=owner->release_calls;
    owner->fail=0;events=~0u;
    assert(neo_throw_actor_step(actor,frame,service,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
    neo_throw_actor_cancel(actor);neo_throw_actor_cancel(actor);
    assert(!events && owner->calls==calls && owner->release_calls==releases);
}
static unsigned test_failure_paths(void) {
    neo_throw_actor_t actor;fault_owner_t owner;neo_throw_services_t service;neo_throw_frame_t frame;unsigned events,cases=0;
    for (unsigned fail=1;fail<=3;fail++) {
        fault_setup(&actor,&owner,&service,&frame);owner.fail=fail;events=~0u;
        assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
        assert(owner.acquired==fail-1 && owner.release_calls==(fail>1));
        fault_failed(&actor,&owner,&service,&frame,events);cases++;
    }
    fault_setup(&actor,&owner,&service,&frame);frame.region_code=5;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_MORE && actor.phase==NEO_THROW_BURST);
    neo_anim_state_t previous=actor.animation;uint16_t layer=actor.body.field66;
    owner.fail=4;events=~0u;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
    assert(actor.animation.program==previous.program && actor.animation.cursor==previous.cursor && actor.animation.timer==previous.timer && actor.body.field66==layer);
    fault_failed(&actor,&owner,&service,&frame,events);cases++;
    fault_setup(&actor,&owner,&service,&frame);frame.region_code=5;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_MORE);
    const neo_anim_command_t bad_command={.kind=NEO_ANIM_UNRECOVERED};const neo_anim_program_t bad={&bad_command,1};service.burst=&bad;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
    assert(owner.acquired==4 && owner.released==4);fault_failed(&actor,&owner,&service,&frame,events);cases++;
    fault_setup(&actor,&owner,&service,&frame);neo_contact_parent_t moving=throw_scene;moving.previous_x++;frame.scene=&moving;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
    assert(owner.acquired==3);fault_failed(&actor,&owner,&service,&frame,events);cases++;
    fault_setup(&actor,&owner,&service,&frame);
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_MORE);
    assert(neo_throw_actor_step(&actor,NULL,NULL,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
    fault_failed(&actor,&owner,&service,&frame,events);cases++;
    fault_setup(&actor,&owner,&service,&frame);
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_MORE);
    fault_owner_t other={0};service.context=&other;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
    assert(!other.calls && !other.release_calls);fault_failed(&actor,&owner,&service,&frame,events);cases++;
    fault_setup(&actor,&owner,&service,&frame);
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_MORE);
    neo_throw_actor_cancel(&actor);neo_throw_actor_cancel(&actor);
    assert(!owner.active && owner.released==3 && owner.release_calls==1);
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_DONE && !events);cases++;
    fault_setup(&actor,&owner,&service,&frame);neo_cull_bounds_t outside=object_bounds;outside.left=2000;outside.right=2100;frame.bounds=&outside;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_DONE);
    neo_throw_actor_cancel(&actor);assert(owner.released==3 && owner.release_calls==1);cases++;
    fault_setup(&actor,&owner,&service,&frame);service.release=NULL;
    assert(neo_throw_actor_step(&actor,&frame,&service,NULL,NULL,&events)==NEO_THROW_UNSUPPORTED);
    assert(!owner.calls && !owner.release_calls);fault_failed(&actor,&owner,&service,&frame,events);cases++;
    assert(cases==11);return cases;
}

int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    unsigned failures=test_failure_paths();
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
    printf("PASS: %u terminal failure/cancel cases; no palette leaks or duplicate sound requests\n",failures);
    printf("PASS: native throw lifecycle: 41 draws, palette cycling, deferred burst, sound request and resource release\n");
#ifdef N64
    debugf("MSX OBJECT REGRESSION PASS steps=%u draws=%u palettes=%u sounds=%u failures=%u\n",(unsigned)COUNT(object_cases),draws,palette_calls,sounds,failures);
    for (;;) {wait_ms(100);}
#endif
}
