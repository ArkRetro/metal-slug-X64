#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_soldier_effects.h"
#include "../src/neo_throw_actor.h"
#include "attack_fixture.h"
#include "throw_fixture.h"
#include "object_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define SAME(f) assert(a->f==b->f)
static void equal(const neo_contact_actor_t *a,const neo_contact_actor_t *b) {
    SAME(x);SAME(y);SAME(vx);SAME(vy);SAME(scalar62);SAME(projected_x);SAME(projected_y);SAME(cached_x);SAME(cached_y);
    SAME(parent_tag);SAME(kind_angle);SAME(field66);SAME(word1e);SAME(flags15);SAME(flags5f);SAME(timer5a);SAME(mark);SAME(correction);
}
#undef SAME
static unsigned draws,events,palettes,spawns,tick,birth;
static neo_contact_actor_t child;
static bool palette(void *context,uint16_t asset,uint8_t *handle) {
    (void)context;assert(asset==0x35c && palettes==0);palettes++;events++;*handle=attack_palette_handle;return true;
}
static bool enqueue(void *context,const neo_contact_actor_t *actor,uint32_t tag,uint16_t field6e) {
    (void)context;assert(tag==0x3bc28 && field6e==throw_field6e && spawns==0);spawns++;events++;birth=tick;
    child=*actor;
    assert(child.x==throw_spawned.x && child.y==throw_spawned.y && child.vx==throw_spawned.vx && child.vy==throw_spawned.vy);
    assert(child.parent_tag==throw_spawned.parent_tag && child.flags15==throw_spawned.flags15);
    return true;
}
static void emit(void *context,const neo_anim_draw_t *draw) {
    (void)context;assert(draws<COUNT(attack_draws));const neo_anim_draw_t *e=&attack_draws[draws++];
    assert(draw->descriptor==e->descriptor && draw->x==e->x && draw->y==e->y && draw->attributes==e->attributes && draw->z==e->z);
}
static unsigned child_step,child_draws,child_palette_calls,child_active_palettes,child_released,child_sounds;
static bool child_palette(void *context,uint16_t asset,bool replace,uint8_t *handle) {
    (void)context;static const uint16_t assets[]={0x35d,0x364,0x365,0x36c};
    assert(child_palette_calls<COUNT(assets) && asset==assets[child_palette_calls]);
    assert(replace==(child_palette_calls==0 || child_palette_calls==3));
    if (replace) {child_released+=child_active_palettes;child_active_palettes=0;}
    *handle=child_palette_calls==3?(uint8_t)(object_cases[33].render.attributes>>8):object_cases[0].render.palettes[child_palette_calls];
    child_palette_calls++;child_active_palettes++;return true;
}
static void child_release(void *context) {(void)context;child_released+=child_active_palettes;child_active_palettes=0;}
static void child_emit(void *context,const neo_anim_draw_t *draw) {
    (void)context;assert(child_draws==child_step);const neo_anim_draw_t *e=&object_cases[child_step].draw;
    assert(draw->descriptor==e->descriptor && draw->x==e->x && draw->y==e->y && draw->attributes==e->attributes && draw->z==e->z);child_draws++;
}
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    neo_anim_state_t animation={0};animation.field50_tag=attack_initial.field50;animation.flash=attack_initial.flash;
    neo_anim_start(&animation,&attack_program,0x100);
    neo_anim_render_t render=attack_cases[0].render;
    assert(neo_anim_step(&animation,&render,NULL,NULL,1024)==NEO_ANIM_UNSUPPORTED);
    neo_anim_start(&animation,&attack_program,0x100);
    neo_soldier_effects_t effects={&throw_source,&throw_scene,throw_spawn_epoch,palette,enqueue,NULL};
    unsigned attack=0;bool initialized=false;neo_throw_actor_t object;
    neo_throw_services_t services={&object_programs[0],&object_programs[1],object_flight_rectangle,object_burst_rectangle,child_palette,child_release,NULL};
    for (tick=attack_first_frame;attack<COUNT(attack_cases) || child_step<COUNT(object_cases);tick+=2) {
        if (attack<COUNT(attack_cases)) {
            const neo_anim_render_t *input=&attack_cases[attack].render;
            assert(render.x==input->x && render.y==input->y && render.attributes==input->attributes && render.z==input->z);
            assert(render.correction==input->correction && render.flash_palette==input->flash_palette && render.suppress==input->suppress);
            assert(memcmp(render.palettes,input->palettes,sizeof(render.palettes))==0);
            unsigned previous_events=events;
            neo_anim_result_t result=neo_anim_step_events(&animation,&render,emit,NULL,1024,neo_soldier_animation_event,&effects);
            assert(result==(attack_cases[attack].complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE));
            assert(animation.cursor==attack_cases[attack].cursor && animation.timer==attack_cases[attack].timer);
            assert(animation.speed==attack_cases[attack].speed && animation.flash==attack_cases[attack].flash && animation.field18==attack_cases[attack].field18);
            assert(animation.field50_tag==attack_cases[attack].field50 && events-previous_events==attack_cases[attack].events);
            attack++;
        }
        if (spawns && tick>birth && child_step<COUNT(object_cases)) {
            assert(tick==object_cases[child_step].frame);
            if (!initialized) {
                assert(birth==throw_spawn_frame);
                neo_anim_render_t graphics=object_cases[0].render;graphics.attributes=child.flags15;memset(graphics.palettes,0,sizeof(graphics.palettes));
                neo_throw_actor_start(&object,&child,throw_field6e,object_initial_flash,&graphics);initialized=true;
            }
            neo_throw_frame_t frame={&throw_scene,&throw_players,&object_bounds,object_cases[child_step].video_phase,object_cases[child_step].region_code,
                object_cases[child_step].epoch,object_cases[child_step].other_contact};
            unsigned requests;neo_throw_result_t result=neo_throw_actor_step(&object,&frame,&services,child_emit,NULL,&requests);
            assert(result==(object_cases[child_step].deleted?NEO_THROW_DONE:NEO_THROW_MORE));
            child_sounds+=!!(requests&NEO_THROW_AUDIO_0);
            if (child_step<COUNT(throw_cases)) equal(&object.body,&throw_cases[child_step].after);
            else {
                const neo_contact_actor_t *e=&object_cases[child_step].body;
                assert(object.body.x==e->x && object.body.y==e->y && object.body.vx==e->vx && object.body.vy==e->vy);
                assert(object.body.projected_x==e->projected_x && object.body.projected_y==e->projected_y && object.body.cached_x==e->cached_x && object.body.cached_y==e->cached_y);
                assert(object.body.field66==e->field66 && object.body.word1e==e->word1e && object.body.flags15==e->flags15 && object.body.mark==e->mark && object.body.correction==e->correction);
            }
            assert(object.phase==object_cases[child_step].phase && object.render.attributes==object_cases[child_step].attributes);
            assert(object.animation.cursor==object_cases[child_step].cursor && object.animation.timer==object_cases[child_step].timer);
            child_step++;
        }
        assert(tick<attack_first_frame+200); // Fixture guard, not a gameplay timeout.
    }
    assert(draws==20 && palettes==1 && spawns==1 && events==2 && child_step==41 && child_draws==41);
    assert(child_palette_calls==4 && child_active_palettes==0 && child_released==4 && child_sounds==1 && !object.active);
    printf("PASS: 20 native attack draws, deferred spawn, 41 child draws, burst and cleanup\n");
#ifdef N64
    debugf("MSX ATTACK REGRESSION PASS steps=%u draws=%u events=%u flight=%u child=%u child_draws=%u\n",attack,draws,events,(unsigned)COUNT(throw_cases),child_step,child_draws);
    for (;;) {wait_ms(100);}
#endif
}
