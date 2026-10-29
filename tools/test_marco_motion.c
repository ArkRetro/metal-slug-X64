#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_motion.h"
#include "marco_motion_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static unsigned case_index,next_op,end_op,total_ops,total_draws,total_events;
static uint32_t body_clock,weapon_clock;
static const neo_anim_command_t stub_commands[]={{.kind=NEO_ANIM_FINISH}};
static const neo_anim_program_t stub_program={stub_commands,1};
#ifdef MARCO_MOTION_NATIVE_ANIMATION
#include "marco_motion_native.h"
static const motion_op_t *native_operation;
static unsigned draw_index,event_index;
static marco_motion_t *native_owner;
static unsigned native_cursor(uint32_t tag) {
    unsigned lo=0,hi=sizeof(motion_source_commands)/sizeof(*motion_source_commands);
    while(lo<hi) {unsigned mid=lo+(hi-lo)/2;if(motion_source_commands[mid]<tag) lo=mid+1;else hi=mid;}
    assert(lo<sizeof(motion_source_commands)/sizeof(*motion_source_commands) && motion_source_commands[lo]==tag);return lo;
}
static int native_table(uint32_t tag) {
    for(unsigned i=0;i<sizeof(motion_native_tables)/sizeof(*motion_native_tables);i++) if(motion_native_tables[i].tag==tag) return (int)i;
    return -1;
}
static void native_draw(void *ctx,const neo_anim_draw_t *d) {
    (void)ctx;assert(draw_index<native_operation->draw_count);
    const neo_anim_draw_t *e=&motion_draws[native_operation->draw_first+draw_index++];
    assert(d->descriptor==e->descriptor && d->x==e->x && d->y==e->y && d->attributes==e->attributes && d->z==e->z);
}
static bool native_event(void *ctx,neo_anim_kind_t kind,uint32_t asset,neo_anim_render_t *r) {
    (void)ctx;(void)r;assert(kind==NEO_ANIM_EVENT && event_index<native_operation->event_count);
    unsigned index=native_operation->event_first+event_index++;
    assert(asset<sizeof(motion_event_targets)/sizeof(*motion_event_targets));
    assert(motion_event_targets[asset]==motion_events[index].target && (!motion_events[index].site || motion_event_sites[asset]==motion_events[index].site));
    for(unsigned n=0;n<motion_events[index].spawn_count;n++) marco_upper_projectile_created(&native_owner->player.upper,(motion_events[index].spawn_mask&(1u<<n))!=0);
    return true; // Observed world request, not an implemented child actor.
}
#endif
static void snapshot(const marco_motion_t *m,const marco_animation_t *a,uint32_t out[C_COUNT]) {
    const marco_crouch_t *s=&m->player;
    const marco_upper_t *u=&s->upper;
    uint32_t v[C_COUNT]={
        [C_F7A]=m->field7a,[C_F86]=m->field86,[C_ATTR]=u->input.attributes,[C_F5F]=u->input.flags5f,[C_F6C]=s->field6c,[C_CHARACTER]=u->character,
        [C_F80]=s->field80,[C_F81]=s->field81,[C_F90]=u->desired90,[C_F91]=u->active91,[C_F92]=u->aim92,[C_F93]=u->form93,
        [C_F95]=u->combo95,[C_F97]=u->idle97,[C_F98]=u->action98,[C_F9A]=u->input.flags9a,[C_F9B]=u->input.accumulator9b,[C_F9C]=u->input.mask9c,[C_F9D]=s->field9d,
        [C_MOTION]=(uint32_t)s->motion62,[C_X]=(uint32_t)s->x,[C_Y]=(uint32_t)s->y,[C_VX]=(uint32_t)s->vx,[C_VY]=(uint32_t)s->vy,
        [C_BODY_CURSOR]=body_clock,[C_BODY_TIMER]=a->body.timer,[C_BODY_SPEED]=a->body.speed,[C_UPPER_MODE]=u->mode,
        [C_UPPER_CURSOR]=weapon_clock,[C_UPPER_TIMER]=a->weapon.timer,[C_UPPER_SPEED]=a->weapon.speed,
        [C_F17]=a->flash,[C_F18]=a->field18,[C_FIELD50]=a->field50_tag,[C_F72]=((uint32_t)a->fields.weapon_dx<<16)|a->fields.weapon_dy,
        [C_F76]=a->fields.field76,[C_F77]=a->fields.frame77,[C_F78]=a->fields.field78,[C_F79]=a->fields.variant79,
        [C_KIND]=u->weapon_kind,[C_PACKED]=u->packed,[C_GATE_MODE]=u->gate.mode,[C_GATE_CURRENT]=u->gate.current,[C_GATE_LIMIT]=u->gate.limit};
    memcpy(out,v,sizeof(v));
}
static void check(const uint32_t *got,const uint32_t *expected,unsigned n) {
    for(unsigned i=0;i<n;i++) if(got[i]!=expected[i]) {
        printf("motion case %u field %u got=%u expected=%u\n",case_index,i,(unsigned)got[i],(unsigned)expected[i]);assert(false);
    }
}
static void seed(marco_motion_t *m,marco_animation_t *a,const uint32_t *b) {
    memset(m,0,sizeof(*m));marco_crouch_t *s=&m->player;m->field7a=b[C_F7A];m->field86=b[C_F86];marco_animation_reset(a);
    s->field6c=b[C_F6C];s->field80=b[C_F80];s->field81=b[C_F81];s->field9d=b[C_F9D];
    s->motion62=(int32_t)b[C_MOTION];s->x=(int32_t)b[C_X];s->y=(int32_t)b[C_Y];s->vx=(int32_t)b[C_VX];s->vy=(int32_t)b[C_VY];
    marco_upper_t *u=&s->upper;u->mode=b[C_UPPER_MODE];u->character=b[C_CHARACTER];u->desired90=b[C_F90];u->active91=b[C_F91];
    u->aim92=b[C_F92];u->form93=b[C_F93];u->combo95=b[C_F95];u->idle97=b[C_F97];u->action98=b[C_F98];u->weapon_kind=b[C_KIND];u->packed=b[C_PACKED];
    u->input=(marco_input_state_t){b[C_ATTR],b[C_F5F],b[C_F9A],b[C_F9B],b[C_F9C]};u->gate=(marco_input_gate_t){b[C_GATE_MODE],b[C_GATE_CURRENT],b[C_GATE_LIMIT]};
    body_clock=b[C_BODY_CURSOR];weapon_clock=b[C_UPPER_CURSOR];
    #ifdef MARCO_MOTION_NATIVE_ANIMATION
    a->body.program=body_clock?&marco_script:NULL;a->body.cursor=body_clock?(uint16_t)native_cursor(body_clock):0;
#else
    a->body.program=body_clock?&stub_program:NULL;
#endif
    a->body.timer=b[C_BODY_TIMER];a->body.speed=b[C_BODY_SPEED];
    a->weapon.program=weapon_clock?&stub_program:NULL;a->weapon.timer=b[C_UPPER_TIMER];a->weapon.speed=b[C_UPPER_SPEED];
    a->flash=b[C_F17];a->field18=b[C_F18];a->field50_tag=b[C_FIELD50];
    a->fields=(marco_pose_fields_t){b[C_F72]>>16,b[C_F72]&65535,b[C_F76],b[C_F77],b[C_F78],b[C_F79]};
}
static const motion_op_t *take(unsigned kind) {
    assert(next_op<end_op && motion_operations[next_op].kind==kind);total_ops++;return &motion_operations[next_op++];
}
static bool select_table(void *ctx,marco_motion_table_t key,marco_motion_t *m,marco_animation_t *a) {
    marco_crouch_t *s=&m->player;
    (void)ctx;const motion_op_t *op=take(0);
    assert(key<MMT_COUNT && motion_table_tags[key]==op->table);
    (void)s;
    #ifdef MARCO_MOTION_NATIVE_ANIMATION
    int i=native_table(op->table);assert(i>=0);
    assert(marco_program_select(a,MARCO_BODY,&motion_native_tables[i].table,s->upper.form93,s->upper.character));
    body_clock=a->body.program?motion_source_commands[a->body.cursor]:0;assert(body_clock==op->program);return true;
#else
    body_clock=op->program;return marco_animation_start(a,MARCO_BODY,body_clock?&stub_program:NULL,0,256);
#endif
}
static bool move_actor(void *ctx,marco_motion_service_t kind,marco_motion_t *m,marco_animation_t *a) {
    marco_crouch_t *s=&m->player;
    (void)ctx;const motion_op_t *op=take(1);assert((unsigned)kind==op->service);uint32_t before[C_COUNT];snapshot(m,a,before);check(before,op->before,C_COUNT);
    /* Only outputs of the independently observed ground/world service. */
    s->x=(int32_t)op->after[C_X];s->y=(int32_t)op->after[C_Y];s->vx=(int32_t)op->after[C_VX];s->vy=(int32_t)op->after[C_VY];
    s->field6c=op->after[C_F6C];s->upper.input.attributes=op->after[C_ATTR];a->field50_tag=op->after[C_FIELD50];return true;
}
static void anim_snapshot(const marco_animation_t *a,uint32_t out[A_COUNT]) {
    uint32_t v[]={body_clock,a->body.timer,a->body.speed,a->flash,a->field18,a->field50_tag,((uint32_t)a->fields.weapon_dx<<16)|a->fields.weapon_dy,a->fields.field76,a->fields.frame77,a->fields.field78,a->fields.variant79};
    memcpy(out,v,sizeof(v));
}
static bool animate(void *ctx,marco_motion_t *m,marco_animation_t *a,bool *complete) {
    marco_crouch_t *s=&m->player;
    (void)ctx;const motion_op_t *op=take(2);uint32_t before[A_COUNT];anim_snapshot(a,before);check(before,op->anim_before,A_COUNT);
#ifdef MARCO_MOTION_NATIVE_ANIMATION
    marco_animation_view_t view=op->view;
    assert(view.render.attributes==s->upper.input.attributes);
    native_operation=op;native_owner=m;draw_index=event_index=0;
    assert(marco_crouch_animation_step(s,a,&view,native_draw,NULL,256,native_event,NULL,complete));
    assert(*complete==op->complete && draw_index==op->draw_count && event_index==op->event_count);
    body_clock=a->body.program?motion_source_commands[a->body.cursor]:0;
    uint32_t after[A_COUNT];anim_snapshot(a,after);check(after,op->anim_after,A_COUNT);
#else
    const uint32_t *v=op->anim_after;
    body_clock=v[A_CURSOR];a->body.timer=v[A_TIMER];a->body.speed=v[A_SPEED];a->flash=v[A_FLASH];a->field18=v[A_F18];a->field50_tag=v[A_FIELD50];
    a->fields=(marco_pose_fields_t){v[A_F72]>>16,v[A_F72]&65535,v[A_F76],v[A_F77],v[A_F78],v[A_F79]};
    for(unsigned i=0;i<op->event_count;i++) {
        unsigned index=op->event_first+i;
        for(unsigned n=0;n<motion_events[index].spawn_count;n++) marco_upper_projectile_created(&s->upper,(motion_events[index].spawn_mask&(1u<<n))!=0);
    }
    *complete=op->complete;
#endif
    total_draws+=op->draw_count;total_events+=op->event_count;return true;
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    const marco_motion_services_t services={.select=select_table,.move=move_actor,.animate=animate};
    unsigned count=sizeof(motion_cases)/sizeof(*motion_cases);
    for(case_index=0;case_index<count;case_index++) {
        marco_motion_t s;marco_animation_t a;seed(&s,&a,motion_cases[case_index].before);s.mode=motion_cases[case_index].entry;
        next_op=motion_cases[case_index].first;end_op=next_op+motion_cases[case_index].count;
        assert(marco_motion_update(&s,&a,&services) && !s.player.failed && !s.player.upper.failed && !a.failed);
        assert(s.tail_call==motion_cases[case_index].tail_call);
        assert(next_op==end_op && s.mode==motion_cases[case_index].target);
        uint32_t after[C_COUNT];snapshot(&s,&a,after);check(after,motion_cases[case_index].after,C_COUNT);
    }
#ifdef MARCO_MOTION_NATIVE_ANIMATION
    puts("MSX MOTION NATIVE ANIMATION PASS");
#ifdef __mips__
    debugf("MSX MOTION NATIVE ANIMATION PASS\n");
#endif
#endif
    printf("PASS: motion calls=%u services=%u draws=%u events=%u\n",count,total_ops,total_draws,total_events);
#ifdef __mips__
    debugf("MSX MOTION REGRESSION PASS calls=%u services=%u draws=%u events=%u\n",count,total_ops,total_draws,total_events);
    console_render();while(1) {}
#endif
    return 0;
}
