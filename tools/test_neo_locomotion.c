#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_soldier_state.h"
#include "locomotion_fixture.h"
#ifdef N64
#include <libdragon.h>
#endif
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
#define SAME(f) assert(a->f==b->f)
static void actor_equal(const neo_contact_actor_t *a,const neo_contact_actor_t *b) {
    SAME(x);SAME(y);SAME(vx);SAME(vy);SAME(scalar62);SAME(projected_x);SAME(projected_y);SAME(cached_x);SAME(cached_y);
    SAME(parent_tag);SAME(kind_angle);SAME(field66);SAME(word1e);SAME(flags15);SAME(flags5f);SAME(timer5a);SAME(mark);SAME(correction);
}
static void control_equal(const neo_control_t *a,const neo_control_t *b) {
    SAME(target84);SAME(velocity88);SAME(phase8c);SAME(timer70);SAME(counter74);SAME(delay92);SAME(target8e);SAME(target90);
    SAME(selector6e);SAME(flags15);SAME(flags5f);SAME(flags7c);SAME(selected80);SAME(variant68);
}
static void state_equal(const neo_soldier_state_t *a,const neo_soldier_state_t *b) {
    SAME(routine);SAME(field78);SAME(field7e);SAME(field81);SAME(field61);
}
#undef SAME
static unsigned index_case,starts,draws;
static const neo_anim_program_t *resolve(void *context,uint32_t tag) {
    (void)context;for (unsigned i=0;i<COUNT(loco_tags);i++) if (tag==loco_tags[i]) {starts++;return &loco_programs[i];}return NULL;
}
static void emit(void *context,const neo_anim_draw_t *draw) {
    (void)context;const neo_anim_draw_t *e=&loco_cases[index_case].draw;
    assert(draws==index_case && draw->descriptor==e->descriptor && draw->x==e->x && draw->y==e->y && draw->attributes==e->attributes && draw->z==e->z);draws++;
}
int main(void) {
#ifdef N64
    debug_init_emulog();console_init();
#endif
    neo_contact_actor_t actor={0};neo_soldier_state_t state={0};neo_anim_state_t animation={0};neo_anim_render_t render={0};unsigned calls=0;
    for (index_case=0;index_case<COUNT(loco_cases);index_case++) {
        const unsigned i=index_case;
        neo_control_t control=loco_cases[i].control_before;
        neo_rng_t rng;memcpy(rng.words,loco_cases[i].rng_before,sizeof(rng.words));rng.cursor=loco_cases[i].cursor_before;
        if (loco_cases[i].init) {
            actor=loco_cases[i].actor_before;state=loco_cases[i].before;
            animation=(neo_anim_state_t){0};animation.flash=loco_cases[i].flash;animation.field50_tag=loco_cases[i].field50;
            animation.speed=loco_cases[i].speed_before;render=loco_cases[i].render;
        } else {
            // The unrecovered common/global boundary still supplies flags5f and control/RNG inputs.
            actor.flags5f=loco_cases[i].actor_before.flags5f;
            state_equal(&state,&loco_cases[i].before);actor_equal(&actor,&loco_cases[i].actor_before);
        }
        assert(neo_soldier_state_begin(&state,&actor,&control,&animation,&rng,resolve,NULL));
        state_equal(&state,&loco_cases[i].begun);actor_equal(&actor,&loco_cases[i].actor_body);
        assert(animation.program==&loco_programs[loco_cases[i].program] && animation.speed==loco_cases[i].speed_body);
        neo_contact_result_t ground;
        bool supported=neo_contact_ground_step(&actor,&loco_cases[i].parent,loco_cases[i].epoch,&ground,NULL);
        assert(supported);actor_equal(&actor,&loco_cases[i].actor_after);
        const neo_contact_result_t *k=&loco_cases[i].kernel;
        assert(ground.x==k->x && ground.y==k->y && ground.vx==k->vx && ground.vy==k->vy && ground.parent_tag==k->parent_tag && ground.kind_angle_word==k->kind_angle_word && ground.flags==k->flags);
        neo_soldier_state_motion_ready(&state,&actor,&ground);assert(state.field61==loco_cases[i].ready61);
        render.x=(uint16_t)((uint32_t)actor.projected_x>>16);render.y=(uint16_t)((uint32_t)actor.projected_y>>16);
        render.attributes=(uint16_t)((render.attributes&0xff00u)|actor.flags15);render.correction=actor.correction;render.z=actor.word1e;render.variant61=state.field61;
        assert(render.x==loco_cases[i].render.x && render.y==loco_cases[i].render.y && render.attributes==loco_cases[i].render.attributes && render.variant61==loco_cases[i].render.variant61);
        assert(animation.cursor==loco_cases[i].anim_before_cursor && animation.timer==loco_cases[i].anim_before_timer);
        neo_anim_result_t result=neo_anim_step(&animation,&render,emit,NULL,1024);
        assert(result==(loco_cases[i].complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE));
        assert(animation.cursor==loco_cases[i].anim_after_cursor && animation.timer==loco_cases[i].anim_after_timer && animation.field50_tag==loco_cases[i].after_field50);
        control.flags15=actor.flags15;control.flags5f=actor.flags5f;
        neo_control_frame_t scene=loco_cases[i].scene;scene.x=render.x;scene.y=render.y;
        neo_soldier_senses_t senses=loco_cases[i].senses;senses.scene=&scene;
        assert(neo_soldier_state_end(&state,&actor,&control,ground.flags,result==NEO_ANIM_COMPLETE,&senses,&rng));
        state_equal(&state,&loco_cases[i].after);control_equal(&control,&loco_cases[i].control_after);
        assert(animation.speed==loco_cases[i].speed_after && rng.cursor==loco_cases[i].cursor_after && memcmp(rng.words,loco_cases[i].rng_after,sizeof(rng.words))==0);
        calls+=loco_cases[i].rng_calls;
    }
    assert(starts==5 && draws==34 && calls==6);
    printf("PASS: 34 native reposition updates/draws, five starts, six RNG calls and inline pose selection\n");
#ifdef N64
    debugf("MSX LOCOMOTION REGRESSION PASS steps=%u starts=%u draws=%u rng=%u\n",(unsigned)COUNT(loco_cases),starts,draws,calls);
    for (;;) {wait_ms(100);}
#endif
}
