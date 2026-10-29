#include "marco_animation.h"
#include <string.h>
void marco_animation_reset(marco_animation_t *actor) {memset(actor,0,sizeof(*actor));}
static neo_anim_state_t *thread(marco_animation_t *actor,marco_part_t part) {
    return part==MARCO_BODY?&actor->body:part==MARCO_WEAPON?&actor->weapon:NULL;
}
bool marco_animation_start(marco_animation_t *actor,marco_part_t part,
    const neo_anim_program_t *program,uint16_t entry,uint16_t speed) {
    neo_anim_state_t *state=thread(actor,part);
    if (!state || actor->failed || (program?entry>=program->count:entry!=0)) return false;
    if (part==MARCO_BODY) actor->field18=0;
    state->flash=actor->flash;state->field50_tag=actor->field50_tag;
    neo_anim_start(state,program,speed);state->cursor=entry;state->field18=actor->field18;
    return true;
}
static bool field(void *context,unsigned id,unsigned bytes,bool write,uint32_t *value) {
    marco_pose_fields_t *f=&((marco_animation_t*)context)->fields;
    if (id==0x72 && bytes==4) {
        if (write) {f->weapon_dx=(uint16_t)(*value>>16);f->weapon_dy=(uint16_t)*value;}
        else *value=((uint32_t)f->weapon_dx<<16)|f->weapon_dy;
        return true;
    }
    if ((id==0x72 || id==0x74) && bytes==2) {
        uint16_t *member=id==0x72?&f->weapon_dx:&f->weapon_dy;
        if (write) *member=(uint16_t)*value;else *value=*member;
        return true;
    }
    uint8_t *member=id==0x76?&f->field76:id==0x77?&f->frame77:
        id==0x78?&f->field78:id==0x79?&f->variant79:NULL;
    if (!member || bytes!=1) return false;
    if (write) *member=(uint8_t)*value;else *value=*member;
    return true;
}
neo_anim_result_t marco_animation_step(marco_animation_t *actor,marco_part_t part,
    marco_animation_view_t *view,neo_anim_emit_t emit,void *draw_context,
    unsigned budget,neo_anim_event_t event,void *event_context) {
    neo_anim_state_t *state=thread(actor,part);
    if (actor->failed) return NEO_ANIM_UNSUPPORTED;
    if (!state || !view) {actor->failed=true;return NEO_ANIM_INVALID;}
    uint16_t dx=0,dy=0;
    if (part==MARCO_WEAPON) {
        dx=actor->fields.weapon_dx;dy=actor->fields.weapon_dy;
        if (view->render.attributes&1u) dx=(uint16_t)(0u-dx);
        if (view->render.attributes&2u) dy=(uint16_t)(0u-dy);
        view->render.x=(uint16_t)(view->render.x+dx);view->render.y=(uint16_t)(view->render.y+dy);
        view->cached_x=(uint16_t)(view->cached_x+dx);view->cached_y=(uint16_t)(view->cached_y+dy);
    }
    state->flash=actor->flash;state->field18=actor->field18;state->field50_tag=actor->field50_tag;
    neo_anim_result_t result=neo_anim_step_fields(state,&view->render,emit,draw_context,
        budget,event,event_context,field,actor);
    actor->flash=state->flash;actor->field18=state->field18;actor->field50_tag=state->field50_tag;
    /* Subtract the saved offsets. Re-reading field72/74 here would be wrong if
     * the script changed them; assigning old coordinates would erase effects. */
    if (part==MARCO_WEAPON) {
        view->render.x=(uint16_t)(view->render.x-dx);view->render.y=(uint16_t)(view->render.y-dy);
        view->cached_x=(uint16_t)(view->cached_x-dx);view->cached_y=(uint16_t)(view->cached_y-dy);
    }
    if (result!=NEO_ANIM_MORE && result!=NEO_ANIM_COMPLETE) actor->failed=true;
    return result;
}
