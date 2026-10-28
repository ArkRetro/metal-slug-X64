#include "marco_upper.h"
#include <stddef.h>
void marco_upper_reset(marco_upper_t *s) {
    s->desired90=255;s->active91=255;s->action98=0;s->mode=MUP_RESET;
}
void marco_upper_projectile_created(marco_upper_t *s,bool allocated) {
    if(allocated) s->combo95=(uint8_t)(s->combo95+1u);
}
bool marco_upper_consume_grenade(marco_upper_t *s) {
    if(s->unlimited_bombs) return true;
    if(!s->bombs) return false;
    --s->bombs;return true;
}
static uint32_t input(marco_upper_t *s) {return marco_input_filter(&s->input,s->packed,s->gate);}
static bool compat(const marco_upper_services_t *v,unsigned active,uint16_t *mask) {
    /* ADD.B wraps the doubled index before the 68000 word-indexed read. */
    unsigned index=((active*2u)&255u)/2u;
    if (!v->compatibility || index>=v->compatibility_count) return false;
    *mask=v->compatibility[index];return true;
}
static bool special(marco_upper_t *s,const marco_upper_services_t *v,bool *result) {
    *result=false;
    if (s->form93!=3) return true; // C487A
    uint16_t value;if (!v->random16 || !v->random16(v->context,&value)) return false;
    if (!(value&7u)) *result=s->unlimited_bombs || s->bombs!=0;
    return true;
}
bool marco_upper_update(marco_upper_t *s,marco_animation_t *a,const marco_upper_services_t *v) {
    if (!s || !a || !v || s->failed || a->failed) return false;
    uint32_t buttons=0;uint16_t mask=0,random=0;
    marco_upper_table_t table=MUT_3998;bool ready=false,done=false,alternate=false;
#define SELECT(key,direct) do { if (!v->select || !v->select(v->context,key,direct,s,a)) goto fail; } while(0)
#define AVAILABLE(key) do { if (!v->available || !v->available(v->context,key,s,&ready)) goto fail; } while(0)
#define ANIMATE() do { if (!v->animate || !v->animate(v->context,s,a,&done)) goto fail; } while(0)
#define LATCH() do { if (!v->latch_input || !v->latch_input(v->context,2)) goto fail; } while(0)
    switch(s->mode) {
    case MUP_RESET:goto reset;
    case MUP_SETTLE:goto settle;
    case MUP_POSE0:goto pose0;
    case MUP_ACTIVE:goto active;
    case MUP_MELEE_WAIT:goto melee_wait;
    case MUP_GRENADE:goto grenade;
    case MUP_FIRE:goto fire;
    case MUP_FIRE_WAIT:goto fire_wait;
    case MUP_REPEAT_A_WAIT:goto repeat_a_wait;
    case MUP_REPEAT_B_WAIT:goto repeat_b_wait;
    case MUP_TURN_WAIT:goto turn_wait;
    case MUP_TURN_DOWN:goto turn_down;
    case MUP_TURN_UP:goto turn_up;
    case MUP_TURN_FROM_UP:goto turn_from_up;
    case MUP_TURN_FROM_DOWN:goto turn_from_down;
    case MUP_AIM_WAIT:goto aim_wait;
    default:goto fail;
    }
reset: // C381C; fallback C382E is RTS, not the following C3830 entry.
    s->action98=0;s->active91=255;alternate=true;goto dispatch;
dispatch: { // C37CE. Reuse a compatible active pose instead of restarting it.
    unsigned desired=s->desired90;
    if (desired!=255 && s->active91!=255) {
        if (!compat(v,s->active91,&mask)) goto fail;
        if ((desired&31u)<16u && (mask&(1u<<(desired&31u)))) desired=255;
    }
    if (desired==2) {s->action98=0;s->active91=2;table=MUT_3850;goto select23;}
    if (desired==3) {s->action98=0;s->active91=3;table=MUT_383C;goto select23;}
    if (desired==1) goto pose1;
    if (desired==0) goto pose0;
    if (alternate) return true;
    ANIMATE();if(done) s->active91=255;return true; // C3B34
}
select23: // C3854
    if (s->aim92) {
        if (s->aim92&128u) table=MUT_3872;
        else {s->aim92=0;s->active91=5;table=MUT_386A;}
    }
    SELECT(table,false);goto selected;
pose1: // C387E
    s->action98=0;s->active91=1;
    if (s->aim92 && !(s->aim92&128u)) table=MUT_38A0;
    else {s->aim92=0;table=MUT_3898;}
    if (s->form93==0) goto combo;
    goto select_form;
settle: // C38B2
    s->action98=0;s->active91=6;
    table=!s->aim92?MUT_38C6:(s->aim92&128u)?MUT_38DA:MUT_38D2;
    goto select_form;
pose4: // C38E2
    s->input.flags9a&=(uint8_t)~32u;s->action98=0;s->active91=4;table=MUT_38F4;goto select_form;
pose0: // C397E
    s->input.flags9a|=32;s->action98=0;s->active91=0;
    table=!s->aim92?MUT_3998:(s->aim92&128u)?MUT_39AC:MUT_39A4;
combo: // C39B0. Threshold is a native read-only table, not a RAM service.
    if (!s->aim92) {
        unsigned index=s->weapon_kind*4u+s->character;
        if (!v->combo_limits || index>=v->combo_limit_count) goto fail;
        unsigned limit=v->combo_limits[index];
        if (limit!=255 && limit<=s->combo95 && (s->input.attributes&64u)) {
            AVAILABLE(MUT_39F4);
            if (ready) {
                s->combo95=0;s->input.flags9a&=(uint8_t)~32u;
                s->active91=s->form93?7:4;table=MUT_39F4;
            }
        }
    }
select_form: // C3A26
    if (s->form93==2) {
        bool pending=(s->input.flags9a&16u)!=0;s->input.flags9a&=(uint8_t)~16u;
        if(pending) {SELECT(MUT_3A3A,true);s->active91=4;goto selected;}
    }
    SELECT(table,false);
selected: // C3A50
    s->idle97=0;s->mode=MUP_ACTIVE;
active: // C3A5E
    buttons=input(s);
    if (buttons&64u) goto grenade;
    if (buttons&16u) goto fire;
    if (!s->aim92) {
        if (s->input.attributes&64u) {
            if (buttons&1u) {s->aim92=1;table=MUT_4106;goto change_aim;}
        } else if (buttons&2u) {s->aim92=255;table=MUT_413C;goto change_aim;}
    } else if (!(s->aim92&128u)) {
        if (buttons&2u) {s->aim92=255;table=MUT_412A;goto change_aim;}
        if (!(buttons&1u)) {s->aim92=0;table=MUT_4118;goto change_aim;}
    } else {
        if (buttons&1u) {s->aim92=1;table=MUT_4160;goto change_aim;}
        if (!(buttons&2u)) {s->aim92=0;table=MUT_414E;goto change_aim;}
    }
    if (!s->aim92) {
        if (s->idle97!=255) ++s->idle97; // ADDQ.B/SUBX.B: saturating increment.
        if(s->idle97>=4) {
            if (!v->random16 || !v->random16(v->context,&random)) goto fail;
            if(random<s->idle97) {
                s->idle97=0;AVAILABLE(MUT_3B00);
                if(ready && s->active91!=255) {
                    if(!compat(v,s->active91,&mask)) goto fail;
                    if(!(mask&16u)) goto pose4;
                }
            }
        }
    }
    alternate=false;goto dispatch;
melee: // C3B46
    s->input.flags9a&=(uint8_t)~32u;s->aim92=0;s->action98=1;
    table=(s->input.flags9a&2u)?MUT_3B5E:MUT_3B6C;s->input.flags9a^=2u;
    SELECT(table,false);a->fields.field76=0;s->mode=MUP_MELEE_WAIT;
melee_wait: // C3B82
    buttons=input(s);
    if(a->fields.field76) {
        s->action98=0;
        if(buttons&64u) goto grenade;
        if(buttons&16u) goto fire;
    } else LATCH();
    ANIMATE();if(done) s->mode=MUP_SETTLE;return true;
grenade: // C3BCC
    s->input.flags9a&=(uint8_t)~32u;s->action98=2;
    s->input.accumulator9b&=(uint8_t)~64u;s->input.mask9c&=(uint8_t)~64u;
    if(!special(s,v,&ready)) goto fail;
    if(ready) SELECT(MUT_3BEC,true);
    else {s->aim92=0;a->fields.field76=0;SELECT(MUT_3C04,false);}
    goto firing_selected;
fire: // C3CD0
    s->input.accumulator9b&=(uint8_t)~16u;s->input.mask9c&=(uint8_t)~16u;
    s->input.flags9a&=(uint8_t)~32u;s->action98=2;
    if(s->form93!=3 && (s->input.flags9a&1u)) goto melee;
    if(s->weapon_kind==5 || s->weapon_kind==10) goto repeat_b;
    if(s->weapon_kind==6 || s->weapon_kind==11) goto repeat_a;
    buttons=input(s);
    if(buttons&1u) {s->aim92=1;table=MUT_FIRE_UP;}
    else if(buttons&2u) {s->aim92=255;table=MUT_FIRE_DOWN;}
    else {s->aim92=0;table=MUT_FIRE_FLAT;}
    SELECT(table,false);
firing_selected: // C3D74
    s->input.accumulator9b&=(uint8_t)~80u;s->input.mask9c&=(uint8_t)~80u;
    a->fields.field76=0;s->mode=MUP_FIRE_WAIT;
fire_wait: // C3D9A
    (void)input(s);
    if(a->fields.field76) {
        s->action98=0;
        if(s->input.accumulator9b&16u) s->mode=MUP_FIRE;
        if(s->input.accumulator9b&64u) s->mode=MUP_GRENADE;
    } else if(s->input.attributes&64u) LATCH();
    ANIMATE();if(done) s->mode=MUP_SETTLE;return true;
repeat_a: // C3E02
    s->input.accumulator9b&=(uint8_t)~16u;s->input.mask9c&=(uint8_t)~16u;
    if(s->input.flags9a&1u) goto melee;
    s->action98=2;buttons=input(s);s->aim92=0;table=MUT_3E28;
    if(buttons&1u) {s->aim92=1;table=MUT_3E3A;}
    if(buttons&2u) {s->aim92=255;table=MUT_3E4C;}
    SELECT(table,false);a->fields.field76=0;s->mode=MUP_REPEAT_A_WAIT;goto repeat_a_draw;
repeat_a_wait: // C3E66
    buttons=input(s);
    if(buttons&64u) goto grenade;
    if(a->fields.field76) {
        bool pressed=(s->input.accumulator9b&16u)!=0;s->input.accumulator9b&=(uint8_t)~16u;
        if(pressed) goto fire;
    }
repeat_a_draw:
    ANIMATE();if(done) s->mode=MUP_SETTLE;return true;
repeat_b: // C3E9C
    s->input.accumulator9b&=(uint8_t)~16u;s->input.mask9c&=(uint8_t)~16u;
    if(s->input.flags9a&1u) goto melee;
    s->action98=2;buttons=input(s);
    if(buttons&1u) {
        if(!s->aim92) goto turn_up;
        if(s->aim92&128u) goto turn_from_down;
        table=MUT_3ED0;
    } else if(buttons&2u) {
        if(!s->aim92) goto turn_down;
        if(!(s->aim92&128u)) goto turn_from_up;
        table=MUT_3EEC;
    } else {
        if(s->aim92 && !(s->aim92&128u)) goto turn_from_up;
        if(s->aim92&128u) goto turn_from_down;
        table=MUT_3F00;
    }
    SELECT(table,false);a->fields.field76=0;s->mode=MUP_REPEAT_B_WAIT;goto repeat_b_draw;
repeat_b_wait: // C3F1A: aim changes have priority until field76 opens.
    buttons=input(s);
    if(!a->fields.field76) {
        if(buttons&1u) {
            if(!s->aim92) goto turn_up;
            if(s->aim92&128u) goto turn_from_down;
        } else if(buttons&2u) {
            if(!s->aim92) goto turn_down;
            if(!(s->aim92&128u)) goto turn_from_up;
        } else {
            if(s->aim92 && !(s->aim92&128u)) goto turn_from_up;
            if(s->aim92&128u) goto turn_from_down;
        }
    }
    if(buttons&64u) goto grenade;
    if(a->fields.field76) {
        bool pressed=(s->input.accumulator9b&16u)!=0;s->input.accumulator9b&=(uint8_t)~16u;
        if(pressed) goto fire;
    }
repeat_b_draw:
    ANIMATE();
    if(done) {
        bool pressed=(s->input.accumulator9b&16u)!=0;s->input.accumulator9b&=(uint8_t)~16u;
        s->mode=pressed?MUP_FIRE:MUP_SETTLE;
    }
    return true;
turn_down: // C3FA6
    s->aim92=255;table=MUT_3FAC;goto turning;
turn_up: // C3FB4
    s->aim92=1;table=MUT_3FBA;goto turning;
turn_from_up: // C3FC2
    s->aim92=0;table=MUT_3FC8;goto turning;
turn_from_down: // C3FD0
    s->aim92=0;table=MUT_3FD6;
turning:
    SELECT(table,false);s->input.accumulator9b&=(uint8_t)~16u;s->input.mask9c&=(uint8_t)~16u;
    s->mode=MUP_TURN_WAIT;goto turn_draw;
turn_wait: // C3FFA
    buttons=input(s);if(buttons&64u) goto grenade;
turn_draw:
    ANIMATE();if(!done) return true;
    ready=(s->input.accumulator9b&16u)!=0;s->input.accumulator9b&=(uint8_t)~16u;
    if(!ready) {s->mode=MUP_SETTLE;return true;}
    /* Preserve the original two CMP/BEQ gates at C4022..C404E. The special
     * C4052 path is not entered for a value that cannot be both 5 and 10. */
    if(s->weapon_kind!=5 || s->weapon_kind!=10) {s->mode=MUP_FIRE;return true;}
    goto fail; // Unreachable for uint8_t; do not invent alternate source flow.
change_aim: // C4100..C416E
    SELECT(table,false);s->action98=1;s->mode=MUP_AIM_WAIT;
aim_wait: // C4176
    buttons=input(s);
    if(buttons&64u) goto grenade;
    if(buttons&16u) goto fire;
    ANIMATE();if(done) s->mode=MUP_POSE0;return true;
fail:
    s->failed=true;a->failed=true;return false;
#undef SELECT
#undef AVAILABLE
#undef ANIMATE
#undef LATCH
}

bool marco_upper_animation_step(marco_upper_t *s,marco_animation_t *a,marco_animation_view_t *view,
    neo_anim_emit_t emit,void *draw_context,unsigned budget,neo_anim_event_t event,void *event_context,bool *complete) {
    if(!s || !a || !view || !complete) return false;
    bool had_program=a->weapon.program!=NULL;view->render.attributes=s->input.attributes;
    neo_anim_result_t result=marco_animation_step(a,MARCO_WEAPON,view,emit,draw_context,budget,event,event_context);
    s->input.attributes=view->render.attributes;
    *complete=had_program && result==NEO_ANIM_COMPLETE;
    return result==NEO_ANIM_MORE || result==NEO_ANIM_COMPLETE;
}
