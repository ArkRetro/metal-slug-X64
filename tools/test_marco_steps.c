/* Whole measured helper calls. External controller changes remain test inputs. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_animation.h"
#include "marco_script.h"
#include "marco_steps.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static const pair_case_t *current;
static unsigned draw_count,event_count;
static void draw(void *context,const neo_anim_draw_t *d) {
    (void)context; assert(draw_count<current->draw_count);
    const neo_anim_draw_t *e=&pair_draws[current->draw_first+draw_count++];
    assert(d->descriptor==e->descriptor && d->x==e->x && d->y==e->y && d->attributes==e->attributes && d->z==e->z);
}
static bool event(void *context,neo_anim_kind_t kind,uint32_t id,neo_anim_render_t *r) {
    (void)context; assert(kind==NEO_ANIM_EVENT && event_count<current->event_count);
    const pair_event_t *e=&pair_events[current->event_first+event_count++];
    assert(id==e->id && r->x==e->x && r->y==e->y && r->attributes==e->attributes);
    /* Forwarding verified here; actual world effects are a different service. */
    return true;
}
static void snapshot(marco_animation_t *a,neo_anim_state_t *s,marco_animation_view_t *v,uint32_t out[P_COUNT]) {
    uint32_t values[]={s->cursor,s->timer,s->speed,a->flash,a->field18,a->field50_tag,
        ((uint32_t)a->fields.weapon_dx<<16)|a->fields.weapon_dy,
        a->fields.field76,a->fields.frame77,a->fields.field78,a->fields.variant79,
        v->render.x,v->render.y,v->cached_x,v->cached_y,v->render.attributes};
    memcpy(out,values,sizeof(values));
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();
#endif
    marco_animation_t actor;marco_animation_reset(&actor);
    unsigned cases=sizeof(pair_cases)/sizeof(*pair_cases),reused=0,total_draws=0,total_events=0;
    for (unsigned i=0;i<cases;i++) {
        current=&pair_cases[i]; const uint32_t *b=current->before;
        neo_anim_state_t *s=current->part?&actor.weapon:&actor.body;
        marco_animation_view_t view={.render=current->render,.cached_x=b[P_CACHED_X],.cached_y=b[P_CACHED_Y]};
        uint32_t prior[P_COUNT];snapshot(&actor,s,&view,prior);
        /* Reuse persistent state when it matches. Reconcile only measured
         * external changes at controller boundaries; never seed expected after. */
        bool same=s->program && !memcmp(prior,b,P_X*sizeof(*b));
        if (same) reused++;
        else {
        s->program=&marco_script;s->cursor=b[P_CURSOR];s->timer=b[P_TIMER];s->speed=b[P_SPEED];
        actor.flash=b[P_F17];actor.field18=b[P_F18];actor.field50_tag=b[P_FIELD50];
        actor.fields=(marco_pose_fields_t){b[P_F72]>>16,b[P_F72]&65535,b[P_F76],b[P_F77],b[P_F78],b[P_F79]};
        }
        draw_count=0;event_count=0;
        neo_anim_result_t result=marco_animation_step(&actor,(marco_part_t)current->part,&view,draw,NULL,256,event,NULL);
        uint32_t after[P_COUNT];snapshot(&actor,s,&view,after);
        if (result!=(current->complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE) || memcmp(after,current->after,sizeof(after))) {
            printf("Case %u result=%u expected_complete=%u\n",i,result,current->complete);
            for(unsigned k=0;k<P_COUNT;k++) if(after[k]!=current->after[k]) printf(" field %u got=%u expected=%u\n",k,(unsigned)after[k],(unsigned)current->after[k]);
            assert(false);
        }
        assert(draw_count==current->draw_count && event_count==current->event_count);
        total_draws+=draw_count;total_events+=event_count;
    }
    printf("PASS: Marco paired steps=%u draws=%u events=%u reused=%u\n",cases,total_draws,total_events,reused);
#ifdef __mips__
    debugf("MSX PAIR REGRESSION PASS steps=%u draws=%u events=%u\n",cases,total_draws,total_events);
    for (;;) wait_ms(100);
#endif
}
