#include "marco_crouch.h"
static uint32_t input(marco_crouch_t *s) {
    return marco_input_filter(&s->upper.input,s->upper.packed,s->upper.gate);
}
bool marco_crouch_update(marco_crouch_t *s,marco_animation_t *a,const marco_crouch_services_t *v) {
    if(!s || !a || !v || s->failed || s->upper.failed || a->failed) return false;
    s->tail_call=false;
    uint32_t buttons=0;bool done=false;marco_crouch_table_t table=MCT_22A4;
#define SELECT(key) do {if(!v->select || !v->select(v->context,key,s,a)) goto fail;} while(0)
#define MOVE() do {if(!v->move || !v->move(v->context,s,a)) goto fail;} while(0)
#define ANIMATE() do {if(!v->animate || !v->animate(v->context,s,a,&done)) goto fail;} while(0)
#define GROUNDED() ((s->upper.input.attributes&64u)!=0)
#define REQUEST(next) do {s->mode=next;s->tail_call=true;return true;} while(0)
    switch(s->mode) {
    case MCR_ENTER:goto enter;
    case MCR_ENTER_WAIT:goto enter_wait;
    case MCR_AFTER_GRENADE:table=MCT_2294;goto idle_select;
    case MCR_AFTER_FIRE:table=MCT_229C;goto idle_select;
    case MCR_IDLE_START:goto idle_start;
    case MCR_IDLE:goto idle;
    case MCR_TURN_START:goto turn_start;
    case MCR_TURN:goto turn;
    case MCR_MOVE_START:goto move_start;
    case MCR_MOVE:goto move;
    case MCR_GRENADE:goto grenade;
    case MCR_FIRE:goto fire;
    case MCR_ATTACK:goto attack;
    default:goto fail;
    }
enter: // C2214
    s->motion62=0;a->field50_tag=0x2d0ab4;
    SELECT(MCT_2224);marco_upper_reset(&s->upper);s->mode=MCR_ENTER_WAIT;
enter_wait: // C2236: firing takes precedence over grenade on this entry.
    if(!GROUNDED()) REQUEST(MCR_FALL_REQUEST);
    buttons=input(s);
    if(s->field6c!=12 && (buttons&32u)) REQUEST(MCR_JUMP_REQUEST);
    if(buttons&16u) goto fire;
    if(buttons&64u) goto grenade;
    MOVE();ANIMATE();if(done) s->mode=MCR_IDLE_START;return true;
idle_start: // C22A4
    table=MCT_22A4;
idle_select: // C2294 / C229C / C22A8
    SELECT(table);marco_upper_reset(&s->upper);s->mode=MCR_IDLE;
idle: // C22B6
    if(!GROUNDED()) REQUEST(MCR_FALL_ACTION_REQUEST);
    buttons=input(s);
    if(s->field6c!=12) {
        if(buttons&32u) REQUEST(MCR_JUMP_REQUEST);
        if(!(buttons&2u)) REQUEST(MCR_STAND_REQUEST);
    }
    if(buttons&4u) {
        if(s->upper.input.attributes&1u) goto move_start;
        goto turn_start;
    }
    if(buttons&8u) {
        if(!(s->upper.input.attributes&1u)) goto move_start;
        goto turn_start;
    }
    if(buttons&64u) goto grenade;
    if(buttons&16u) goto fire;
    s->motion62=0;MOVE();ANIMATE();if(done) s->mode=MCR_IDLE_START;return true;
turn_start: // C2338
    s->upper.input.attributes^=1;
    SELECT(MCT_233E);marco_upper_reset(&s->upper);
    s->upper.input.accumulator9b&=(uint8_t)~80u;s->upper.input.mask9c&=(uint8_t)~80u;
    s->mode=MCR_TURN;
turn: // C2368
    if(!GROUNDED()) REQUEST(MCR_FALL_ACTION_REQUEST);
    if(s->field6c!=12) {
        buttons=input(s);
        if(!(buttons&2u)) REQUEST(MCR_STAND_REQUEST);
        if(buttons&32u) REQUEST(MCR_JUMP_REQUEST);
    }
    MOVE();ANIMATE();
    if(done) {
        s->mode=MCR_IDLE_START;
        if(s->upper.input.accumulator9b&16u) s->mode=MCR_FIRE;
        if(s->upper.input.accumulator9b&64u) s->mode=MCR_GRENADE;
    }
    return true;
move_start: // C23CA
    SELECT(MCT_23CA);marco_upper_reset(&s->upper);
    s->motion62=(s->upper.input.attributes&1u)?-0x12000:0x12000;s->mode=MCR_MOVE;
move: // C23F2
    if(!GROUNDED()) REQUEST(MCR_FALL_ACTION_REQUEST);
    buttons=input(s);
    if(s->upper.input.flags9a&8u) goto turn_start;
    if(!(buttons&12u)) goto idle_start;
    if(s->field6c!=12) {
        if(buttons&32u) REQUEST(MCR_JUMP_REQUEST);
        if(!(buttons&2u)) REQUEST(MCR_STAND_REQUEST);
    }
    if(buttons&64u) goto grenade;
    if(buttons&16u) goto fire;
    MOVE();ANIMATE();if(done) s->mode=MCR_IDLE_START;return true;
grenade: // C2458
    s->field80=0;SELECT(MCT_245E);goto attack_start;
fire: // C246A
    s->field80=1;
    if(s->upper.input.flags9a&1u) {
        bool alternate=(s->upper.input.flags9a&2u)!=0;s->upper.input.flags9a^=2;
        table=s->field9d?(alternate?MCT_24AE:MCT_24A6):(alternate?MCT_2494:MCT_248C);
    } else {
        if(s->upper.weapon_kind>=16) goto fail; // C24C8..C24D0
        table=MCT_FIRE;
    }
    SELECT(table);s->field81=255;
    if(s->upper.weapon_kind==5 || s->upper.weapon_kind==6 || s->upper.weapon_kind==10 || s->upper.weapon_kind==11) s->field81=4;
attack_start: // C2560
    s->upper.input.accumulator9b&=(uint8_t)~80u;s->upper.input.mask9c&=(uint8_t)~80u;
    marco_upper_reset(&s->upper);a->fields.field76=0;s->mode=MCR_ATTACK;
attack: // C2588
    if(!GROUNDED()) REQUEST(MCR_FALL_ACTION_REQUEST);
    buttons=input(s);
    if(s->field6c!=12) {
        if(!(buttons&2u)) REQUEST(MCR_STAND_REQUEST);
        if(buttons&32u) REQUEST(MCR_JUMP_REQUEST);
    }
    s->motion62=0;MOVE();ANIMATE();
    if(done) s->mode=s->field80?MCR_AFTER_FIRE:MCR_AFTER_GRENADE;
    if(a->fields.field76) {
        if(s->upper.input.accumulator9b&16u) s->mode=MCR_FIRE;
        if(s->upper.input.accumulator9b&64u) s->mode=MCR_GRENADE;
        if(s->upper.input.flags9a&8u) s->mode=MCR_TURN_START;
    }
    if(!(s->field81&128u)) {
        if(s->field81) --s->field81;
        else if(s->upper.input.flags9a&8u) s->mode=MCR_TURN_START;
    }
    return true;
fail:
    s->failed=true;s->upper.failed=true;a->failed=true;return false;
#undef SELECT
#undef MOVE
#undef ANIMATE
#undef GROUNDED
#undef REQUEST
}

bool marco_crouch_animation_step(marco_crouch_t *s,marco_animation_t *a,marco_animation_view_t *view,
    neo_anim_emit_t emit,void *draw_context,unsigned budget,neo_anim_event_t event,void *event_context,bool *complete) {
    if(!s || !a || !view || !complete) return false;
    view->render.attributes=s->upper.input.attributes;
    neo_anim_result_t result=marco_animation_step(a,MARCO_BODY,view,emit,draw_context,budget,event,event_context);
    s->upper.input.attributes=view->render.attributes;*complete=result==NEO_ANIM_COMPLETE;
    return result==NEO_ANIM_MORE || result==NEO_ANIM_COMPLETE;
}
