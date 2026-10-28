#include "neo_animation.h"

static int signed16(uint16_t value) {return (int)(value&0x7fffu)-(value&0x8000u?32768:0);}
void neo_anim_start(neo_anim_state_t *s,const neo_anim_program_t *program,uint16_t speed) {
    s->program=program;s->cursor=0;s->timer=0;s->speed=speed;s->field18=0;
}
static neo_anim_draw_t project(const neo_anim_state_t *s,const neo_anim_render_t *r,
    const neo_anim_command_t *c) {
    // 004392..0043C6: signed byte offsets, embedded low bits, mirror and borrow.
    int dx=(int)c->dx*2+((c->flags>>1)&1),dy=(int)c->dy*2+(c->flags&1);
    if (r->attributes&1u) dx=-dx;
    if (r->attributes&2u) dy=-dy;
    uint16_t x=(uint16_t)(r->x+dx-(r->correction&1));
    uint16_t y=(uint16_t)(r->y+dy-((r->correction>>1)&1));
    uint16_t attr=r->attributes^((c->flags>>2)&3u);
    // 0043D4..0043FC: signed flash byte and carry select palette override.
    if (!(s->flash&0x80u) && (s->flash&1u)) attr=(uint16_t)((attr&255u)|((unsigned)r->flash_palette<<8));
    else if ((c->flags>>4)<4u) attr=(uint16_t)((attr&255u)|((unsigned)r->palettes[c->flags>>4]<<8));
    return (neo_anim_draw_t){c->asset,x,y,attr,r->z};
}
neo_anim_result_t neo_anim_step_fields(neo_anim_state_t *s,neo_anim_render_t *r,
    neo_anim_emit_t emit,void *context,unsigned budget,neo_anim_event_t event,void *event_context,
    neo_anim_field_t field,void *field_context) {
    if (!s->program) return NEO_ANIM_COMPLETE; // 004284: carry set, no updates.
    if (s->flash && !(s->flash&0x80u)) --s->flash; // signed TST.B/BLE at 004290.
    unsigned pc=s->cursor;
    while (budget--) {
        if (pc>=s->program->count) return NEO_ANIM_INVALID;
        const neo_anim_command_t *c=&s->program->commands[pc++];
        switch (c->kind) {
        case NEO_ANIM_FIELD50: // 004470, observed command writes actor +0x50.
            s->field50_tag=c->asset;s->cursor=(uint16_t)pc;break;
        case NEO_ANIM_POSE: // 004364: descriptor consumed even when suppressed.
            if (!r->suppress && !(s->timer&0x8000u) && emit) {
                neo_anim_draw_t draw=project(s,r,c);emit(context,&draw);
            }
            break;
        case NEO_ANIM_SELECT_POSE:case NEO_ANIM_SELECT_FIELD_POSE: { // 004412: select/clamp, draw, skip table.
            if (!c->operand || (c->kind==NEO_ANIM_SELECT_POSE && c->asset!=0x61) || (unsigned)c->target+c->operand>s->program->count) return NEO_ANIM_INVALID;
            unsigned selected=r->variant61;
            if (c->kind==NEO_ANIM_SELECT_FIELD_POSE) {
                uint32_t value;
                if (!field || !field(field_context,c->asset,1,false,&value)) return NEO_ANIM_UNSUPPORTED;
                selected=value&255u;
            }
            if (selected>=c->operand) selected=c->operand-1u;
            const neo_anim_command_t *pose=&s->program->commands[c->target+selected];
            if (pose->kind!=NEO_ANIM_POSE) return NEO_ANIM_INVALID;
            if (!r->suppress && !(s->timer&0x8000u) && emit) {
                neo_anim_draw_t draw=project(s,r,pose);emit(context,&draw);
            }
            pc=(unsigned)c->target+c->operand;break;
        }
        case NEO_ANIM_WAIT: { // 0042F4..00435A, including byte-only timer reload.
            int old=signed16(s->timer);
            if (!c->operand) {
                if (old<0) {s->timer=0;pc=s->cursor;break;}
                return NEO_ANIM_MORE;
            }
            if (old<=0) s->timer=(uint16_t)((s->timer&255u)|((s->timer+((unsigned)c->operand<<8))&0xff00u));
            if (old>=0) s->timer=(uint16_t)(s->timer-s->speed);
            if (signed16(s->timer)<=0) {
                if (pc>=s->program->count) return NEO_ANIM_INVALID;
                if (s->program->commands[pc].kind==NEO_ANIM_FINISH) {
                    s->timer&=255u;return NEO_ANIM_COMPLETE;
                }
                s->cursor=(uint16_t)pc;
            }
            if (old<0) {pc=s->cursor;break;}
            return NEO_ANIM_MORE;
        }
        case NEO_ANIM_JUMP: // 00449E..0044A4: cursor changes before dispatch.
            if (c->target>=s->program->count) return NEO_ANIM_INVALID;
            pc=c->target;s->cursor=(uint16_t)pc;break;
        case NEO_ANIM_FINISH: // 004352; preserve low fractional timer byte.
            s->timer&=255u;return NEO_ANIM_COMPLETE;
        case NEO_ANIM_FLIP_FACING: // callback ROM 01ADAA, recovered BCHG #0,+15.
            r->attributes^=1u;s->cursor=(uint16_t)pc;break;
        case NEO_ANIM_REPLACE_PALETTE:case NEO_ANIM_SPAWN_THROW:
            if (!event || !event(event_context,c->kind,c->asset,r)) return NEO_ANIM_UNSUPPORTED;
            s->cursor=(uint16_t)pc;break;
        case NEO_ANIM_WRITE_BYTE:case NEO_ANIM_WRITE_WORD:case NEO_ANIM_WRITE_LONG: {
            unsigned bytes=c->kind==NEO_ANIM_WRITE_BYTE?1:c->kind==NEO_ANIM_WRITE_WORD?2:4;
            uint32_t value=c->asset;
            if (bytes==1) value&=255u;else if (bytes==2) value&=65535u;
            if (!field || !field(field_context,c->target,bytes,true,&value)) return NEO_ANIM_UNSUPPORTED;
            s->cursor=(uint16_t)pc;break;
        }
        case NEO_ANIM_DECREMENT_BYTE: {
            uint32_t value;
            if (!field || !field(field_context,c->target,1,false,&value)) return NEO_ANIM_UNSUPPORTED;
            value=(value-1u)&255u;
            if (!field(field_context,c->target,1,true,&value)) return NEO_ANIM_UNSUPPORTED;
            s->cursor=(uint16_t)pc;break;
        }
        case NEO_ANIM_BRANCH_FIELD: {
            uint32_t value;
            if (!field || !field(field_context,c->asset,1,false,&value)) return NEO_ANIM_UNSUPPORTED;
            if (!!(value&255u)==!!c->operand) {
                if (c->target>=s->program->count) return NEO_ANIM_INVALID;
                pc=c->target;
            }
            s->cursor=(uint16_t)pc;break;
        }
        case NEO_ANIM_UNRECOVERED:return NEO_ANIM_UNSUPPORTED;
        default:return NEO_ANIM_INVALID;
        }
    }
    return NEO_ANIM_BUDGET_EXHAUSTED;
}

neo_anim_result_t neo_anim_step_events(neo_anim_state_t *s,neo_anim_render_t *r,
    neo_anim_emit_t emit,void *context,unsigned budget,neo_anim_event_t event,void *event_context) {
    return neo_anim_step_fields(s,r,emit,context,budget,event,event_context,0,0);
}

neo_anim_result_t neo_anim_step(neo_anim_state_t *s,neo_anim_render_t *r,
    neo_anim_emit_t emit,void *context,unsigned budget) {
    return neo_anim_step_events(s,r,emit,context,budget,0,0);
}
