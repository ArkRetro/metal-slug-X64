#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_control.h"
#include "../src/neo_contact.h"
#include "../src/neo_animation.h"
#include "control_fixture.h"
#include "contact_fixture.h"
#include "animation_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define SAME(f) assert(a->f==b->f)
static void equal(const neo_control_t *a,const neo_control_t *b) {
    SAME(target84);SAME(velocity88);SAME(phase8c);SAME(timer70);SAME(counter74);SAME(delay92);
    SAME(target8e);SAME(target90);SAME(selector6e);SAME(flags15);SAME(flags5f);SAME(flags7c);SAME(selected80);SAME(variant68);
}
#undef SAME
static neo_rng_t initial(void) {
    neo_rng_t r;memcpy(r.words,control_initial_words,sizeof(r.words));r.cursor=control_initial_cursor;return r;
}
static void mutation(uint16_t *expected,unsigned i) {
    if (control_rng[i].changed_index>=0) expected[(unsigned)control_rng[i].changed_index]=control_rng[i].changed_value;
}
static unsigned pipeline_draws;
static void emit(void *context,const neo_anim_draw_t *draw) {
    (void)context;assert(pipeline_draws<COUNT(anim_expected_draws));
    const neo_anim_draw_t *e=&anim_expected_draws[pipeline_draws++];
    assert(draw->descriptor==e->descriptor && draw->x==e->x && draw->y==e->y);
    assert(draw->attributes==e->attributes && draw->z==e->z);
}
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    neo_rng_t rng=initial();uint16_t expected[55];memcpy(expected,rng.words,sizeof(expected));unsigned differences=0;
    for (unsigned i=0;i<COUNT(control_rng);i++) {
        neo_rng_result_t r=neo_rng_next_result(&rng);
        assert(r.value==control_rng[i].value && r.mixed_word==control_rng[i].mixed);
        assert(!!(r.mixed_word&32768u)==!!(control_rng[i].sr&8u));
        assert((r.mixed_word==0)==!!(control_rng[i].sr&4u));
        differences+=!!(r.value&32768u)!=!!(r.mixed_word&32768u);
        mutation(expected,i);assert(memcmp(expected,rng.words,sizeof(expected))==0);
        assert(rng.cursor==control_rng[i].cursor);
    }
    assert(differences==CONTROL_SIGN_DIFFERENCES);
    rng=initial();memcpy(expected,rng.words,sizeof(expected));unsigned index=0;
    neo_control_t state=control_cases[0].before;
    neo_contact_actor_t actor=contact_cases[0].before;
    neo_anim_state_t animation={0};unsigned anim_index=0,movements=0,speed_changes=0;
    animation.flash=anim_cases[0].before.flash;animation.field18=anim_cases[0].before.field18;
    animation.field50_tag=anim_cases[0].before.field50;
    for (unsigned i=0;i<COUNT(control_cases);i++) {
        neo_control_frame_t frame=control_cases[i].frame;
        if (i) {
            unsigned m=i-1;assert(m<COUNT(contact_cases));
            while (anim_index<COUNT(anim_cases) && anim_cases[anim_index].init) {
                neo_anim_start(&animation,&anim_programs[anim_cases[anim_index].program],anim_cases[anim_index].requested);anim_index++;
            }
            assert(anim_index<COUNT(anim_cases));
            actor.scalar62=contact_cases[m].before.scalar62; // main state machine remains external.
            actor.flags5f=state.flags5f; // computed by native RNG/control, NOT captured flags.
            assert(actor.flags5f==contact_cases[m].before.flags5f);
            assert(neo_contact_ground_step(&actor,&contact_cases[m].parent,contact_cases[m].epoch,NULL,NULL));
            const neo_contact_actor_t *e=&contact_cases[m].after;
            assert(actor.x==e->x && actor.y==e->y && actor.vx==e->vx && actor.vy==e->vy);
            assert(actor.projected_x==e->projected_x && actor.projected_y==e->projected_y);
            neo_anim_render_t render=anim_cases[anim_index].render;
            render.x=(uint16_t)((uint32_t)actor.projected_x>>16);render.y=(uint16_t)((uint32_t)actor.projected_y>>16);
            render.attributes=(uint16_t)((render.attributes&0xff00u)|actor.flags15);render.correction=actor.correction;render.z=actor.word1e;
            if (animation.speed!=anim_cases[anim_index].before.speed) {animation.speed=anim_cases[anim_index].before.speed;speed_changes++;}
            assert(neo_anim_step(&animation,&render,emit,NULL,1024)==(anim_cases[anim_index].complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE));
            assert(animation.cursor==anim_cases[anim_index].after.cursor && animation.timer==anim_cases[anim_index].after.timer);
            actor.flags15=(uint8_t)render.attributes;state.flags15=actor.flags15;state.flags5f=actor.flags5f;
            frame.x=(uint16_t)((uint32_t)actor.projected_x>>16); // before draw-time correction.
            frame.y=(uint16_t)((uint32_t)actor.projected_y>>16);
            frame.origin_x=(uint16_t)((uint32_t)contact_cases[m].parent.x>>16);frame.origin_y=(uint16_t)((uint32_t)contact_cases[m].parent.y>>16);
            anim_index++;movements++;
        }
        equal(&state,&control_cases[i].before);
        assert(frame.x==control_cases[i].frame.x && frame.y==control_cases[i].frame.y);
        neo_control_target(&state,&frame);equal(&state,&control_cases[i].target);
        assert(neo_control_probability(&state,&frame)==control_cases[i].threshold);
        // Preserve actual global RNG ordering while other actor logic is pending.
        while (index<control_cases[i].start) {assert(neo_rng_next(&rng)==control_rng[index].value);mutation(expected,index++);}
        assert(neo_control_prefix(&state,&frame,&rng));
        equal(&state,&control_cases[i].prefix);
        while (index<control_cases[i].end) mutation(expected,index++);
        assert(memcmp(expected,rng.words,sizeof(expected))==0 && rng.cursor==control_rng[index-1].cursor);
        neo_control_finish(&state,control_cases[i].probe1,control_cases[i].probe2);equal(&state,&control_cases[i].after);
    }
    assert(movements==COUNT(contact_cases) && pipeline_draws==COUNT(anim_expected_draws) && speed_changes==ANIM_EXTERNAL_SPEED_WRITES);
    printf("PASS: %u RNG flags, %u control cycles, %u movements and %u draws without injected flags5f\n",(unsigned)COUNT(control_rng),(unsigned)COUNT(control_cases),movements,pipeline_draws);
#ifdef N64
    debugf("MSX CONTROL REGRESSION PASS cycles=%u rng=%u moves=%u draws=%u\n",(unsigned)COUNT(control_cases),(unsigned)COUNT(control_rng),movements,pipeline_draws);
    for (;;) {wait_ms(100);}
#endif
}
