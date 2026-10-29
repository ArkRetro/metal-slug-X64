#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/marco_stance.h"
#include "marco_stance_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static unsigned case_index,next_op,end_op,total_ops;
static uint32_t body_clock,weapon_clock;
static const neo_anim_command_t stub_commands[]={{.kind=NEO_ANIM_FINISH}};
static const neo_anim_program_t stub_program={stub_commands,1};
static void snapshot(const marco_stance_t *st,const marco_motion_t *m,const marco_animation_t *a,uint32_t out[C_COUNT]) {
    const marco_crouch_t *s=&m->player;
    const marco_upper_t *u=&s->upper;
    uint32_t v[C_COUNT]={
        [C_F7B]=st->field7b,[C_F96]=st->field96,[C_F1E]=st->field1e,[C_F66]=st->field66,[C_F7A]=m->field7a,[C_F86]=m->field86,[C_ATTR]=u->input.attributes,[C_F5F]=u->input.flags5f,[C_F6C]=s->field6c,[C_CHARACTER]=u->character,
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
        printf("stance case %u field %u got=%u expected=%u\n",case_index,i,(unsigned)got[i],(unsigned)expected[i]);assert(false);
    }
}
static void seed(marco_stance_t *st,marco_motion_t *m,marco_animation_t *a,const uint32_t *b) {
    memset(st,0,sizeof(*st));st->field7b=b[C_F7B];st->field96=b[C_F96];st->field1e=b[C_F1E];st->field66=b[C_F66];
    memset(m,0,sizeof(*m));marco_crouch_t *s=&m->player;m->field7a=b[C_F7A];m->field86=b[C_F86];marco_animation_reset(a);
    s->field6c=b[C_F6C];s->field80=b[C_F80];s->field81=b[C_F81];s->field9d=b[C_F9D];
    s->motion62=(int32_t)b[C_MOTION];s->x=(int32_t)b[C_X];s->y=(int32_t)b[C_Y];s->vx=(int32_t)b[C_VX];s->vy=(int32_t)b[C_VY];
    marco_upper_t *u=&s->upper;u->mode=b[C_UPPER_MODE];u->character=b[C_CHARACTER];u->desired90=b[C_F90];u->active91=b[C_F91];
    u->aim92=b[C_F92];u->form93=b[C_F93];u->combo95=b[C_F95];u->idle97=b[C_F97];u->action98=b[C_F98];u->weapon_kind=b[C_KIND];u->packed=b[C_PACKED];
    u->input=(marco_input_state_t){b[C_ATTR],b[C_F5F],b[C_F9A],b[C_F9B],b[C_F9C]};u->gate=(marco_input_gate_t){b[C_GATE_MODE],b[C_GATE_CURRENT],b[C_GATE_LIMIT]};
    body_clock=b[C_BODY_CURSOR];weapon_clock=b[C_UPPER_CURSOR];
    a->body.program=body_clock?&stub_program:NULL;
    a->body.timer=b[C_BODY_TIMER];a->body.speed=b[C_BODY_SPEED];
    a->weapon.program=weapon_clock?&stub_program:NULL;a->weapon.timer=b[C_UPPER_TIMER];a->weapon.speed=b[C_UPPER_SPEED];
    a->flash=b[C_F17];a->field18=b[C_F18];a->field50_tag=b[C_FIELD50];
    a->fields=(marco_pose_fields_t){b[C_F72]>>16,b[C_F72]&65535,b[C_F76],b[C_F77],b[C_F78],b[C_F79]};
}
static const stance_op_t *take(unsigned kind) {
    assert(next_op<end_op && stance_operations[next_op].kind==kind);total_ops++;return &stance_operations[next_op++];
}
static bool select_table(void *ctx,marco_stance_table_t key,marco_stance_t *st,marco_motion_t *m,marco_animation_t *a) {
    (void)ctx;(void)st;(void)m;const stance_op_t *op=take(0);
    assert(key<MST_TABLE_COUNT && stance_table_tags[key]==op->table);
    body_clock=op->program;return marco_animation_start(a,MARCO_BODY,body_clock?&stub_program:NULL,0,256);
}
/* Static-evidence edge cases, separate from the original-game observations. */
static unsigned effect_calls;
static marco_stance_table_t selected;
static bool limit_select(void *ctx,marco_stance_table_t key,marco_stance_t *st,marco_motion_t *m,marco_animation_t *a) {
    (void)ctx;(void)st;(void)m;selected=key;
    return marco_animation_start(a,MARCO_BODY,&stub_program,0,256);
}
static bool limit_effect(void *ctx,marco_stance_t *s,marco_motion_t *m,marco_animation_t *a) {
    (void)ctx;(void)a;
    assert(s->field1e==0x8834 && s->field66==0x8834 && m->field7a==255 && s->field7b==255);
    ++effect_calls;return true;
}
static void limits(void) {
    marco_stance_services_t v={.select=limit_select,.land_effect=limit_effect};
    marco_stance_t s={.mode=MST_IDLE_START};marco_motion_t m={0};marco_animation_t a;
    marco_animation_reset(&a);m.player.upper.aim92=255;m.player.upper.mode=MUP_ACTIVE;
    assert(marco_stance_enter(&s,&m,&a,&v) && selected==MST_1F20);
    assert(m.player.upper.aim92==0 && m.player.upper.mode==MUP_RESET);
    s.mode=MST_IDLE_START;m.player.upper.aim92=1;m.player.upper.mode=MUP_ACTIVE;
    assert(marco_stance_enter(&s,&m,&a,&v) && selected==MST_1F8A);
    assert(m.player.upper.aim92==1 && m.player.upper.mode==MUP_ACTIVE);
    m.player.upper.form93=2;m.field86=31;s.mode=MST_IDLE_FAST;
    assert(marco_stance_enter(&s,&m,&a,&v) && selected==MST_1F8A && m.field86==31);
    m.field86=32;s.mode=MST_IDLE_FAST;
    assert(marco_stance_enter(&s,&m,&a,&v) && selected==MST_1EA2 && m.field86==0);
    s.mode=MST_LAND;
    assert(marco_stance_enter(&s,&m,&a,&v) && effect_calls==1);
    s.mode=MST_LAND;v.land_effect=NULL;
    assert(!marco_stance_enter(&s,&m,&a,&v) && m.player.failed && a.failed);
    assert(effect_calls==1);
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    limits();
    const marco_stance_services_t services={.select=select_table};
    unsigned count=sizeof(stance_cases)/sizeof(*stance_cases);
    for(case_index=0;case_index<count;case_index++) {
        marco_stance_t st;marco_motion_t m;marco_animation_t a;
        seed(&st,&m,&a,stance_cases[case_index].before);st.mode=stance_cases[case_index].entry;
        next_op=stance_cases[case_index].first;end_op=next_op+stance_cases[case_index].count;
        assert(marco_stance_enter(&st,&m,&a,&services) && st.mode==MST_IDLE);
        assert(next_op==end_op && !m.player.failed && !a.failed);
        uint32_t after[C_COUNT];snapshot(&st,&m,&a,after);check(after,stance_cases[case_index].after,C_COUNT);
    }
    printf("PASS: stance calls=%u services=%u\n",count,total_ops);
#ifdef __mips__
    debugf("MSX STANCE REGRESSION PASS calls=%u services=%u\n",count,total_ops);
    console_render();while(1) {}
#endif
    return 0;
}
