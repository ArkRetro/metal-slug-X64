#include "marco_stance.h"
static uint32_t idle_input(marco_upper_t *u) {
    return marco_input_filter(&u->input,u->packed,u->gate);
}
marco_idle_branch_t marco_stance_idle_branch(marco_motion_t *m) {
    marco_crouch_t *p=&m->player;marco_upper_t *u=&p->upper;
    if(!(u->input.attributes&64u)) return MSI_FALL;
    if(p->field6c==12) return MSI_CROUCH;
    uint32_t buttons=idle_input(u);
    if(buttons&12u) return MSI_RUN;
    if(buttons&32u) return MSI_JUMP;
    if(buttons&2u) return MSI_CROUCH;
    if(p->field9d && (u->input.flags9a&1u) && (idle_input(u)&16u)) return MSI_MELEE;
    if(!(u->input.flags9a&32u)) return (idle_input(u)&81u)?MSI_FAST:MSI_DRAW;
    return (idle_input(u)&81u)?MSI_DRAW:MSI_WORLD;
}
bool marco_stance_enter(marco_stance_t *s,marco_motion_t *m,marco_animation_t *a,const marco_stance_services_t *v) {
    if(!s || !m || !a || !v || m->player.failed || m->player.upper.failed || a->failed) return false;
    marco_crouch_t *p=&m->player;marco_upper_t *u=&p->upper;
    marco_stance_table_t table;
#define SELECT() do {if(!v->select || !v->select(v->context,table,s,m,a)) goto fail;} while(0)
    switch(s->mode) {
    case MST_STAND: // C1E34: preserve field50 during the stand-up program.
        p->motion62=0;table=MST_1E3C;goto reset_select;
    case MST_BRAKE:goto brake;
    case MST_LAND: // C1EBE
        s->field66=s->field1e=0x8834;m->field7a=s->field7b=255;
        if(u->form93==2 && (!v->land_effect || !v->land_effect(v->context,s,m,a))) goto fail;
        goto idle_start;
    case MST_IDLE_START:goto idle_start;
    case MST_IDLE_FAST:goto idle_fast;
    default:goto fail;
    }
idle_start: // C1EFA: TST.B/BGT uses signed byte semantics.
    s->field7b=255;a->field50_tag=0x2d0aa8;
    if(u->aim92 && u->aim92<128) goto idle_fast;
    p->motion62=0;if(u->action98) goto idle_fast;
    table=MST_1F20;goto reset_select;
idle_fast: // C1F4A
    if(u->form93==2 && m->field86>=32) goto brake;
    u->input.flags9a|=32;s->field7b=m->field7a=255;
    s->field66=s->field1e=0x8834;a->field50_tag=0x2d0aa8;
    u->desired90=0;table=MST_1F8A;SELECT();
    p->motion62=0;s->field96=0;s->mode=MST_IDLE;return true;
brake: // C1E94
    m->field86=0;a->field50_tag=0x2d0aa8;table=MST_1EA2;
reset_select: // C1F24: reset upper controller only on this path.
    u->aim92=0;SELECT();marco_upper_reset(u);
    u->input.flags9a&=(uint8_t)~32u;p->motion62=0;s->mode=MST_IDLE;return true;
fail:
    p->failed=true;u->failed=true;a->failed=true;return false;
#undef SELECT
}
