#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_control.h"
#include "../src/neo_contact.h"
#include "../src/neo_animation.h"
#include "control_fixture.h"
#include "contact_fixture.h"
#include "animation_fixture.h"
#ifdef MSX_STATE_REGRESSION
#include "../src/neo_soldier_state.h"
#include "state_fixture.h"
#include "action_fixture.h"
#include "../src/neo_cull.h"
#include "cull_fixture.h"
#endif
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
#ifdef MSX_STATE_REGRESSION
static unsigned resolved_programs;
static const neo_anim_program_t *resolve_program(void *context,uint32_t tag) {
    (void)context;
    for (unsigned i=0;i<COUNT(state_program_tags);i++) {
        if (tag==state_program_tags[i]) {resolved_programs++;return &anim_programs[i];}
    }
    return NULL;
}
static void machine_equal(const neo_soldier_state_t *a,const neo_soldier_state_t *b) {
    assert(a->routine==b->routine && a->field78==b->field78 && a->field7e==b->field7e && a->field81==b->field81);
}
#endif
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
#ifdef MSX_STATE_REGRESSION
    // Unsupported dispatches/air paths must not partially initialize state or RNG.
    neo_soldier_state_t bad={.routine=0x123456},saved_bad=bad;
    neo_contact_actor_t probe_actor=contact_cases[0].before,saved_actor=probe_actor;
    neo_control_t probe_control=control_cases[0].before;
    neo_anim_state_t probe_animation={0};neo_rng_t probe_rng=initial(),saved_rng=probe_rng;
    assert(!neo_soldier_state_begin(&bad,&probe_actor,&probe_control,&probe_animation,&probe_rng,resolve_program,NULL));
    machine_equal(&bad,&saved_bad);assert(memcmp(&probe_actor,&saved_actor,sizeof(saved_actor))==0);
    assert(memcmp(probe_rng.words,saved_rng.words,sizeof(saved_rng.words))==0 && probe_rng.cursor==saved_rng.cursor && probe_animation.program==NULL);
    bad=state_cases[0].before;saved_bad=bad;probe_actor.flags15&=191u;saved_actor=probe_actor;
    assert(!neo_soldier_state_begin(&bad,&probe_actor,&probe_control,&probe_animation,&probe_rng,resolve_program,NULL));
    machine_equal(&bad,&saved_bad);assert(memcmp(&probe_actor,&saved_actor,sizeof(saved_actor))==0);
    assert(memcmp(probe_rng.words,saved_rng.words,sizeof(saved_rng.words))==0 && probe_rng.cursor==saved_rng.cursor);
    unsigned removed=0;
    for (unsigned i=0;i<COUNT(cull_cases);i++) {
        bool outside=neo_cull_outside(&cull_cases[i].actor,&cull_cases[i].bounds,cull_cases[i].rectangle);
        assert(outside==cull_cases[i].outside);removed+=outside;
    }
    assert(removed==1);
    unsigned nonzero_actions=0;
    for (unsigned i=0;i<COUNT(action_cases);i++) {
        neo_control_t c=action_cases[i].before;
        neo_rng_t r;memcpy(r.words,action_cases[i].words_before,sizeof(r.words));r.cursor=action_cases[i].cursor_before;
        uint8_t result=neo_soldier_action_choose(&c,&action_cases[i].actor,&action_cases[i].scene,action_cases[i].mask,&r);
        assert(result==action_cases[i].result);nonzero_actions+=result!=0;
        equal(&c,&action_cases[i].after);
        assert(r.cursor==action_cases[i].cursor_after && memcmp(r.words,action_cases[i].words_after,sizeof(r.words))==0);
    }
    assert(nonzero_actions>0);
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
#ifdef MSX_STATE_REGRESSION
    neo_soldier_state_t machine=state_cases[0].before;
    actor.scalar62=state_cases[0].scalar_before;
    animation.speed=state_cases[0].speed_before;
    unsigned original_speed_writes=0;
#endif
    for (unsigned i=0;i<COUNT(control_cases);i++) {
        neo_control_frame_t frame=control_cases[i].frame;
        if (i) {
            unsigned m=i-1;assert(m<COUNT(contact_cases));
#ifdef MSX_STATE_REGRESSION
            machine_equal(&machine,&state_cases[m].before);
            assert(actor.scalar62==state_cases[m].scalar_before);
            assert(animation.speed==state_cases[m].speed_before);
            unsigned start=control_cases[i].start-state_cases[m].rng_calls;
            while (index<start) {assert(neo_rng_next(&rng)==control_rng[index].value);mutation(expected,index++);}
            unsigned resolved_before=resolved_programs;
            assert(neo_soldier_state_begin(&machine,&actor,&state,&animation,&rng,resolve_program,NULL));
            assert(resolved_programs-resolved_before==state_cases[m].init_count);
            for (unsigned n=0;n<state_cases[m].rng_calls;n++) mutation(expected,index++);
            assert(memcmp(expected,rng.words,sizeof(expected))==0 && rng.cursor==control_rng[index-1].cursor);
            assert(actor.scalar62==state_cases[m].scalar_motion && animation.speed==state_cases[m].speed_motion);
            original_speed_writes+=state_cases[m].rng_calls!=0;
            while (anim_index<COUNT(anim_cases) && anim_cases[anim_index].init) {
                assert(animation.program==&anim_programs[anim_cases[anim_index].program]);
                anim_index++;
            }
#else
            while (anim_index<COUNT(anim_cases) && anim_cases[anim_index].init) {
                neo_anim_start(&animation,&anim_programs[anim_cases[anim_index].program],anim_cases[anim_index].requested);anim_index++;
            }
#endif
            assert(anim_index<COUNT(anim_cases));
#ifdef MSX_STATE_REGRESSION
            assert(actor.scalar62==contact_cases[m].before.scalar62);
#else
            actor.scalar62=contact_cases[m].before.scalar62; // main state machine remains external.
#endif
            actor.flags5f=state.flags5f; // computed by native RNG/control, NOT captured flags.
            assert(actor.flags5f==contact_cases[m].before.flags5f);
            neo_contact_result_t native_kernel;
            assert(neo_contact_ground_step(&actor,&contact_cases[m].parent,contact_cases[m].epoch,&native_kernel,NULL));
            const neo_contact_actor_t *e=&contact_cases[m].after;
            assert(actor.x==e->x && actor.y==e->y && actor.vx==e->vx && actor.vy==e->vy);
            assert(actor.projected_x==e->projected_x && actor.projected_y==e->projected_y);
            neo_anim_render_t render=anim_cases[anim_index].render;
            render.x=(uint16_t)((uint32_t)actor.projected_x>>16);render.y=(uint16_t)((uint32_t)actor.projected_y>>16);
            render.attributes=(uint16_t)((render.attributes&0xff00u)|actor.flags15);render.correction=actor.correction;render.z=actor.word1e;
#ifdef MSX_STATE_REGRESSION
            assert(animation.speed==anim_cases[anim_index].before.speed);
#else
            if (animation.speed!=anim_cases[anim_index].before.speed) {animation.speed=anim_cases[anim_index].before.speed;speed_changes++;}
#endif
            neo_anim_result_t animation_result=neo_anim_step(&animation,&render,emit,NULL,1024);
            assert(animation_result==(anim_cases[anim_index].complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE));
            assert(animation.cursor==anim_cases[anim_index].after.cursor && animation.timer==anim_cases[anim_index].after.timer);
            actor.flags15=(uint8_t)render.attributes;state.flags15=actor.flags15;state.flags5f=actor.flags5f;
#ifdef MSX_STATE_REGRESSION
            neo_soldier_senses_t senses=state_cases[m].senses;senses.scene=&frame;
            assert(neo_soldier_state_end(&machine,&actor,&state,native_kernel.flags,animation_result==NEO_ANIM_COMPLETE,&senses,&rng));
            machine_equal(&machine,&state_cases[m].after);
            assert(actor.scalar62==state_cases[m].scalar_after && animation.speed==state_cases[m].speed_after);
#endif
            frame.x=(uint16_t)((uint32_t)actor.projected_x>>16); // before draw-time correction.
            frame.y=(uint16_t)((uint32_t)actor.projected_y>>16);
            frame.origin_x=(uint16_t)((uint32_t)contact_cases[m].parent.x>>16);frame.origin_y=(uint16_t)((uint32_t)contact_cases[m].parent.y>>16);
            anim_index++;movements++;
        }
        equal(&state,&control_cases[i].before);
        assert(frame.x==control_cases[i].frame.x && frame.y==control_cases[i].frame.y);
        neo_control_target(&state,&frame);equal(&state,&control_cases[i].target);
#ifdef MSX_STATE_REGRESSION
        if (i) {
            const neo_contact_actor_t *original=&cull_cases[i-1].actor;
            assert(actor.projected_x==original->projected_x && actor.projected_y==original->projected_y);
            assert(actor.cached_x==original->cached_x && actor.cached_y==original->cached_y && actor.flags15==original->flags15);
            assert(!neo_cull_outside(&actor,&cull_cases[i-1].bounds,cull_cases[i-1].rectangle));
        }
#endif
        assert(neo_control_probability(&state,&frame)==control_cases[i].threshold);
        // Preserve actual global RNG ordering while other actor logic is pending.
        while (index<control_cases[i].start) {assert(neo_rng_next(&rng)==control_rng[index].value);mutation(expected,index++);}
        assert(neo_control_prefix(&state,&frame,&rng));
        equal(&state,&control_cases[i].prefix);
        while (index<control_cases[i].end) mutation(expected,index++);
        assert(memcmp(expected,rng.words,sizeof(expected))==0 && rng.cursor==control_rng[index-1].cursor);
        neo_control_finish(&state,control_cases[i].probe1,control_cases[i].probe2);equal(&state,&control_cases[i].after);
    }
#ifdef MSX_STATE_REGRESSION
    (void)speed_changes;
    assert(original_speed_writes==ANIM_EXTERNAL_SPEED_WRITES && resolved_programs==6);
    assert(movements==COUNT(state_cases) && pipeline_draws==COUNT(anim_expected_draws));
    printf("PASS: %u measured cull decisions (%u removed)\n",(unsigned)COUNT(cull_cases),removed);
    printf("PASS: %u measured action choices (%u nonzero), 54 native state cycles, six starts and two speed calculations without injected scalar62/speed/program/routine\n",(unsigned)COUNT(action_cases),nonzero_actions);
#else
    assert(movements==COUNT(contact_cases) && pipeline_draws==COUNT(anim_expected_draws) && speed_changes==ANIM_EXTERNAL_SPEED_WRITES);
#endif
    printf("PASS: %u RNG flags, %u control cycles, %u movements and %u draws without injected flags5f\n",(unsigned)COUNT(control_rng),(unsigned)COUNT(control_cases),movements,pipeline_draws);
#ifdef N64
#ifdef MSX_STATE_REGRESSION
    debugf("MSX STATE REGRESSION PASS culls=%u actions=%u states=%u cycles=%u rng=%u moves=%u draws=%u\n",(unsigned)COUNT(cull_cases),(unsigned)COUNT(action_cases),(unsigned)COUNT(state_cases),(unsigned)COUNT(control_cases),(unsigned)COUNT(control_rng),movements,pipeline_draws);
#else
    debugf("MSX CONTROL REGRESSION PASS cycles=%u rng=%u moves=%u draws=%u\n",(unsigned)COUNT(control_cases),(unsigned)COUNT(control_rng),movements,pipeline_draws);
#endif
    for (;;) {wait_ms(100);}
#endif
}
