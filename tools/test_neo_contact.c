#include <assert.h>
#include <stdio.h>
#include "../src/neo_contact.h"
#include "../src/neo_collision.h"
#include "../src/neo_animation.h"
#include "contact_fixture.h"
#include "animation_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define SAME(field) assert(a->field==b->field)
static void actor_equal(const neo_contact_actor_t *a,const neo_contact_actor_t *b) {
    SAME(x);SAME(y);SAME(vx);SAME(vy);SAME(scalar62);SAME(projected_x);SAME(projected_y);
    SAME(cached_x);SAME(cached_y);SAME(parent_tag);SAME(kind_angle);SAME(field66);SAME(word1e);
    SAME(flags15);SAME(flags5f);SAME(timer5a);SAME(mark);SAME(correction);
}
static void request_equal(const neo_contact_request_t *a,const neo_contact_request_t *b) {
    SAME(x);SAME(y);SAME(dx);SAME(dy);SAME(parent_tag);SAME(mode);SAME(mode8);SAME(mode9);SAME(modea);SAME(moded);
}
static void result_equal(const neo_contact_result_t *a,const neo_contact_result_t *b) {
    SAME(x);SAME(y);SAME(vx);SAME(vy);SAME(parent_tag);SAME(kind_angle_word);SAME(flags);
}
#undef SAME
static unsigned pipeline_draw;
static void emit_pipeline(void *context,const neo_anim_draw_t *draw) {
    (void)context;assert(pipeline_draw<COUNT(anim_expected_draws));
    const neo_anim_draw_t *b=&anim_expected_draws[pipeline_draw++];
    assert(draw->descriptor==b->descriptor && draw->x==b->x && draw->y==b->y);
    assert(draw->attributes==b->attributes && draw->z==b->z);
}
static void test_pipeline(void) {
    neo_contact_actor_t actor=contact_cases[0].before;
    neo_anim_state_t animation={0};unsigned motion=0,speed_changes=0;
    animation.flash=anim_cases[0].before.flash;animation.field18=anim_cases[0].before.field18;
    animation.field50_tag=anim_cases[0].before.field50;
    for (unsigned i=0;i<COUNT(anim_cases);i++) {
        if (anim_cases[i].init) {
            neo_anim_start(&animation,&anim_programs[anim_cases[i].program],anim_cases[i].requested);continue;
        }
        assert(motion<COUNT(contact_cases));
        // These are measured inputs from unrecovered controller/sensing code,
        // not position, velocity, contact results or presentation coordinates.
        actor.scalar62=contact_cases[motion].before.scalar62;
        actor.flags5f=contact_cases[motion].before.flags5f;
        actor_equal(&actor,&contact_cases[motion].before);
        assert(neo_contact_ground_step(&actor,&contact_cases[motion].parent,contact_cases[motion].epoch,NULL,NULL));
        actor_equal(&actor,&contact_cases[motion].after);
        if (animation.speed!=anim_cases[i].before.speed) {
            animation.speed=anim_cases[i].before.speed;speed_changes++;
        } // the two recovered initializer writes.
        neo_anim_render_t render=anim_cases[i].render;
        render.x=(uint16_t)((uint32_t)actor.projected_x>>16);
        render.y=(uint16_t)((uint32_t)actor.projected_y>>16);
        render.attributes=(uint16_t)((render.attributes&0xff00u)|actor.flags15);
        render.correction=actor.correction;render.z=actor.word1e;
        assert(neo_anim_step(&animation,&render,emit_pipeline,NULL,1024)==
            (anim_cases[i].complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE));
        actor.flags15=(uint8_t)render.attributes; // retain the real facing callback.
        assert(animation.cursor==anim_cases[i].after.cursor && animation.timer==anim_cases[i].after.timer);
        motion++;
    }
    assert(motion==COUNT(contact_cases) && pipeline_draw==COUNT(anim_expected_draws));
    assert(speed_changes==ANIM_EXTERNAL_SPEED_WRITES);
}
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    for (unsigned i=0;i<COUNT(contact_cases);i++) {
        int32_t vx,vy;neo_contact_vector(contact_cases[i].scalar,contact_cases[i].angle,&vx,&vy);
        assert(vx==contact_cases[i].vx && vy==contact_cases[i].vy);
        neo_contact_actor_t actor=contact_cases[i].before;
        neo_contact_request_t request=neo_contact_prepare_ground(&actor);
        request_equal(&request,&contact_cases[i].request);
        assert(actor.flags15==contact_cases[i].prepared_flags15);
        // Independently check commit/projection with the real kernel output.
        neo_contact_commit(&actor,&contact_cases[i].result,&contact_cases[i].parent,contact_cases[i].epoch);
        actor_equal(&actor,&contact_cases[i].after);
        // Then run the actual native terrain subset; no captured kernel result
        // is injected into this solver or the complete ground-step call.
        actor=contact_cases[i].before;request=neo_contact_prepare_ground(&actor);
        neo_contact_result_t result;
        if (!neo_contact_plane_ground(&actor,&request,&result)) {
            fprintf(stderr,"Unsupported native terrain case %u\n",i);assert(0);
        }
        result_equal(&result,&contact_cases[i].result);
        actor=contact_cases[i].before;uint32_t returned;
        assert(neo_contact_ground_step(&actor,&contact_cases[i].parent,contact_cases[i].epoch,&result,&returned));
        result_equal(&result,&contact_cases[i].result);
        actor_equal(&actor,&contact_cases[i].after);assert(returned==contact_cases[i].returned);
    }
    test_pipeline();
    // Unsupported stacked surfaces must not silently select a different floor
    // or partially mutate an actor before falling back to a future solver.
    const neo_contact_actor_t original=contact_cases[0].before;
    neo_contact_actor_t actor=original;uint32_t returned=0x12345678;
    assert(neo_collision_vehicle_wreckage(original.x,original.y,1)>0);
    assert(!neo_contact_ground_step(&actor,&contact_cases[0].parent,contact_cases[0].epoch,NULL,&returned));
    actor_equal(&actor,&original);assert(returned==0x12345678);neo_collision_reset();
    printf("PASS: %u original vectors/ground updates and %u coupled animation draws\n",(unsigned)COUNT(contact_cases),pipeline_draw);
#ifdef N64
    debugf("MSX CONTACT REGRESSION PASS steps=%u draws=%u\n",(unsigned)COUNT(contact_cases),pipeline_draw);
    for (;;) {wait_ms(100);}
#endif
}
