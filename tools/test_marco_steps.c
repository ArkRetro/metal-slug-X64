/* Whole measured helper calls. External controller changes remain test inputs. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_animation.h"
#ifdef MARCO_COUPLED_CONTROLLER
#include "../src/marco_controller.h"
#include "marco_controller_private_fixture.h"
#include "marco_controller_timeline.h"
#else
#include "marco_script.h"
#endif
#include "marco_steps.h"
#ifdef __mips__
#include <libdragon.h>
#endif
#ifdef MARCO_RENDER_COUPLED
#include "../src/marco_renderer.h"
#include "../src/cinematic_codec.h"
#include "marco_pixel_sequence.h"
static neo_anim_draw_t render_queue[32];
static unsigned queued,pixel_index,pixel_draws;
static void render_batch(unsigned frame) {
    if (pixel_index>=sizeof(pixel_frames)/sizeof(*pixel_frames) || pixel_frames[pixel_index].frame!=frame) {
        assert(!queued || frame>pixel_frames[sizeof(pixel_frames)/sizeof(*pixel_frames)-1].frame);
        queued=0;return;
    }
    surface_t *surface=display_get();rdpq_attach_clear(surface,NULL);
    rdpq_set_scissor(0,0,320,240);rdpq_set_mode_fill(RGBA32(0,0,0,255));rdpq_fill_rectangle(0,0,320,240);
    rdpq_set_scissor(8,8,312,232);
    for (unsigned i=0;i<queued;i++) {
        unsigned profile=~0u;
        for (unsigned j=0;j<pixel_frames[pixel_index].count;j++)
            if (pixel_frames[pixel_index].banks[j]==(render_queue[i].attributes>>8)) profile=pixel_frames[pixel_index].profiles[j];
        assert(profile!=~0u && marco_renderer_draw_native(&render_queue[i],0,-8,profile));
    }
    rdpq_detach_wait();uint32_t crc=msx_cine_crc32(surface->buffer,surface->stride*surface->height);
    if (crc!=pixel_frames[pixel_index].crc) debugf("MSX COUPLED PIXELS mismatch frame=%u got=%08lx expected=%08lx\n",frame,(unsigned long)crc,(unsigned long)pixel_frames[pixel_index].crc);
    assert(crc==pixel_frames[pixel_index].crc);display_show(surface);
    pixel_draws+=queued;queued=0;pixel_index++;
}
#endif
static const pair_case_t *current;
static unsigned draw_count,event_count;
static void draw(void *context,const neo_anim_draw_t *d) {
    (void)context; assert(draw_count<current->draw_count);
#ifdef MARCO_RENDER_COUPLED
    assert(queued<sizeof(render_queue)/sizeof(*render_queue));render_queue[queued++]=*d;
#endif
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
static void run_step(marco_animation_t *actor,unsigned i,unsigned *reused,unsigned *total_draws,unsigned *total_events) {
    current=&pair_cases[i]; const uint32_t *b=current->before;
    neo_anim_state_t *s=current->part?&actor->weapon:&actor->body;
    marco_animation_view_t view={.render=current->render,.cached_x=b[P_CACHED_X],.cached_y=b[P_CACHED_Y]};
    uint32_t prior[P_COUNT];snapshot(actor,s,&view,prior);
    /* Reuse persistent state when it matches. Reconcile only measured
     * external changes at controller boundaries; never seed expected after. */
    bool same=s->program && !memcmp(prior,b,P_X*sizeof(*b));
    if (same) (*reused)++;
    else {
        s->program=&marco_script;s->cursor=b[P_CURSOR];s->timer=b[P_TIMER];s->speed=b[P_SPEED];
        actor->flash=b[P_F17];actor->field18=b[P_F18];actor->field50_tag=b[P_FIELD50];
        actor->fields=(marco_pose_fields_t){b[P_F72]>>16,b[P_F72]&65535,b[P_F76],b[P_F77],b[P_F78],b[P_F79]};
    }
    draw_count=0;event_count=0;
    neo_anim_result_t result=marco_animation_step(actor,(marco_part_t)current->part,&view,draw,NULL,256,event,NULL);
    uint32_t after[P_COUNT];snapshot(actor,s,&view,after);
    if (result!=(current->complete?NEO_ANIM_COMPLETE:NEO_ANIM_MORE) || memcmp(after,current->after,sizeof(after))) {
        printf("Case %u result=%u expected_complete=%u\n",i,result,current->complete);
        for(unsigned k=0;k<P_COUNT;k++) if(after[k]!=current->after[k]) printf(" field %u got=%u expected=%u\n",k,(unsigned)after[k],(unsigned)current->after[k]);
        assert(false);
    }
    assert(draw_count==current->draw_count && event_count==current->event_count);
    *total_draws+=draw_count;*total_events+=event_count;
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();
#endif
    marco_animation_t actor;marco_animation_reset(&actor);
    unsigned cases=sizeof(pair_cases)/sizeof(*pair_cases),reused=0,total_draws=0,total_events=0;
#ifdef MARCO_COUPLED_CONTROLLER
    unsigned inputs=0,choices=0;
#ifdef MARCO_RENDER_COUPLED
    assert(dfs_init(DFS_DEFAULT_LOCATION)==DFS_ESUCCESS);rdpq_init();
    display_init(RESOLUTION_320x240,DEPTH_16_BPP,2,GAMMA_NONE,FILTERS_RESAMPLE);
    assert(marco_renderer_load());
#ifndef MARCO_PUBLICATION_QUEUE
    unsigned source_frame=controller_timeline[0].frame;
#endif
#endif
    for(unsigned t=0;t<sizeof(controller_timeline)/sizeof(*controller_timeline);t++) {
        unsigned i=controller_timeline[t].index;
#if defined(MARCO_RENDER_COUPLED) && !defined(MARCO_PUBLICATION_QUEUE)
        if (source_frame!=controller_timeline[t].frame) {render_batch(source_frame);source_frame=controller_timeline[t].frame;}
#endif
        switch(controller_timeline[t].kind) {
        case 0: {
            marco_input_state_t state=controller_inputs[i].state;
            uint32_t result=marco_input_filter(&state,controller_inputs[i].packed,controller_inputs[i].gate);
            assert(result==controller_inputs[i].result && state.flags5f==controller_inputs[i].flags5f && state.flags9a==controller_inputs[i].flags9a && state.accumulator9b==controller_inputs[i].accumulator9b);
            inputs++;break;
        }
        case 1: {
            actor.flash=controller_choices[i].flash;actor.field18=controller_choices[i].field18;actor.field50_tag=controller_choices[i].field50;
            const marco_program_slot_t *slot=&controller_roots[controller_choices[i].slot];marco_program_table_t table={slot,1};
            assert(marco_program_select(&actor,(marco_part_t)controller_choices[i].part,&table,controller_choices[i].form,controller_choices[i].character));
            const neo_anim_state_t *state=controller_choices[i].part?&actor.weapon:&actor.body;
            assert(state->program==slot->program && state->cursor==slot->entry && !state->timer && state->speed==256 && actor.field18==controller_choices[i].after18);
            choices++;break;
        }
        case 2:run_step(&actor,i,&reused,&total_draws,&total_events);break;
#ifdef MARCO_PUBLICATION_QUEUE
        case 3:render_batch(i);break;
        case 4:queued=0;break;
#endif
        default:assert(false);
        }
    }
#ifdef MARCO_RENDER_COUPLED
#ifndef MARCO_PUBLICATION_QUEUE
    render_batch(source_frame);
#endif
    assert(pixel_index==sizeof(pixel_frames)/sizeof(*pixel_frames));
    for(unsigned i=0;i<sizeof(native_limit_cases)/sizeof(*native_limit_cases);i++) {
        surface_t *surface=display_get();rdpq_attach_clear(surface,NULL);
        rdpq_set_scissor(0,0,320,240);rdpq_set_mode_fill(RGBA32(0,0,0,255));rdpq_fill_rectangle(0,0,320,240);rdpq_set_scissor(8,8,312,232);
        assert(marco_renderer_draw_native(&native_limit_cases[i].draw,0,-8,native_limit_cases[i].profile));rdpq_detach_wait();
        uint32_t crc=msx_cine_crc32(surface->buffer,surface->stride*surface->height);
        if (crc!=native_limit_cases[i].crc) debugf("MSX NATIVE LIMIT mismatch case=%u\n",i);
        assert(crc==native_limit_cases[i].crc);display_show(surface);
    }
    debugf("MSX MARCO NATIVE LIMITS PASS cases=%u\n",(unsigned)(sizeof(native_limit_cases)/sizeof(*native_limit_cases)));
    unsigned tmem=marco_renderer_max_upload();assert(tmem>0 && tmem<=2048);marco_renderer_close();
    debugf("MSX COUPLED PIXELS PASS frames=%u draws=%u tmem_max=%u\n",pixel_index,pixel_draws,tmem);
#endif
    printf("PASS: Marco coupled inputs=%u choices=%u steps=%u draws=%u events=%u reused=%u\n",inputs,choices,cases,total_draws,total_events,reused);
#ifdef __mips__
    debugf("MSX COUPLED REGRESSION PASS inputs=%u choices=%u steps=%u draws=%u events=%u\n",inputs,choices,cases,total_draws,total_events);
#endif
#else
    for(unsigned i=0;i<cases;i++) run_step(&actor,i,&reused,&total_draws,&total_events);
    printf("PASS: Marco paired steps=%u draws=%u events=%u reused=%u\n",cases,total_draws,total_events,reused);
#ifdef __mips__
    debugf("MSX PAIR REGRESSION PASS steps=%u draws=%u events=%u\n",cases,total_draws,total_events);
#endif
#endif
#ifdef __mips__
    for (;;) wait_ms(100);
#endif
}
