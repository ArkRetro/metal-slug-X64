#include "marco_body.h"
marco_body_result_t marco_body_update(marco_body_t *b,marco_animation_t *a,
    const marco_motion_services_t *motion,const marco_crouch_services_t *crouch,
    const marco_stance_services_t *stance) {
    if(!b || !a) return MARCO_BODY_FAILED;
    marco_motion_t *m=&b->motion;marco_crouch_t *p=&m->player;
    if(p->failed || p->upper.failed || a->failed) return MARCO_BODY_FAILED;
    /* Safety budget for malformed services/state, not an animation tick cap. */
    for(unsigned branches=0;branches<16;branches++) {
        switch(b->domain) {
        case MARCO_BODY_MOTION:
            switch(m->mode) {
            case MMR_CROUCH_REQUEST:
                p->mode=MCR_ENTER;b->domain=MARCO_BODY_CROUCH;continue;
            case MMR_LAND_REQUEST:
                b->stance.mode=MST_LAND;b->domain=MARCO_BODY_STANCE;continue;
            case MMR_IDLE_FAST_REQUEST:
                b->stance.mode=MST_IDLE_FAST;b->domain=MARCO_BODY_STANCE;continue;
            case MMR_IDLE_REQUEST:
                b->stance.mode=MST_IDLE_START;b->domain=MARCO_BODY_STANCE;continue;
            case MMR_MELEE_REQUEST: { // C215E..C2192, then C2198 in this tick.
                bool alternate=(p->upper.input.flags9a&2u)!=0;
                p->motion62=0;a->field50_tag=0x2d0aa8;p->upper.input.flags9a^=2;
                if(!stance || !stance->select || !stance->select(stance->context,
                   alternate?MST_2180:MST_2178,&b->stance,m,a)) goto fail;
                marco_upper_reset(&p->upper);a->fields.field76=0;m->mode=MMR_MELEE;
                /* fall through to the recovered wait body below. */
            }
                /* fall through */
            case MMR_MELEE: { // C2198..C2210.
                uint32_t buttons=marco_input_filter(&p->upper.input,p->upper.packed,p->upper.gate);
                if(buttons&32u) {m->mode=MMR_IDLE_FAST_REQUEST;continue;}
                if(a->fields.field76) {
                    if(buttons&2u) {m->mode=MMR_CROUCH_REQUEST;continue;}
                    if(buttons&80u) {m->mode=MMR_IDLE_FAST_REQUEST;continue;}
                    if(buttons&12u) {m->mode=MMR_RUN_START;continue;}
                } else {
                    if(!stance || !stance->latch_input || !stance->latch_input(stance->context,1)) goto fail;
                    if(p->upper.input.flags9a&8u) {m->mode=MMR_RUN_START;continue;}
                }
                p->motion62=0;bool complete=false;
                if(!motion || !motion->move || !motion->animate ||
                   !motion->move(motion->context,MMM_GROUND,m,a) ||
                   !motion->animate(motion->context,m,a,&complete)) goto fail;
                if(complete) m->mode=MMR_IDLE_FAST_REQUEST;
                return MARCO_BODY_COMMON;
            }
            default:break;
            }
            if(!marco_motion_update(m,a,motion)) return MARCO_BODY_FAILED;
            if(!m->tail_call) return MARCO_BODY_COMMON;
            break;
        case MARCO_BODY_CROUCH:
            switch(p->mode) {
            case MCR_JUMP_REQUEST:m->mode=MMR_JUMP;break;
            case MCR_FALL_ACTION_REQUEST:m->mode=MMR_FALL_ACTION;break;
            case MCR_FALL_REQUEST:m->mode=MMR_FALL;break;
            case MCR_STAND_REQUEST:
                b->stance.mode=MST_STAND;b->domain=MARCO_BODY_STANCE;continue;
            default:
                if(!marco_crouch_update(p,a,crouch)) return MARCO_BODY_FAILED;
                if(!p->tail_call) return MARCO_BODY_COMMON;
                continue;
            }
            b->domain=MARCO_BODY_MOTION;break;
        case MARCO_BODY_STANCE:
            if(b->stance.mode==MST_IDLE) {
                switch(marco_stance_idle_branch(m)) {
                case MSI_RUN:m->mode=MMR_RUN_START;b->domain=MARCO_BODY_MOTION;continue;
                case MSI_JUMP:m->mode=MMR_JUMP;b->domain=MARCO_BODY_MOTION;continue;
                case MSI_FALL:m->mode=MMR_FALL;b->domain=MARCO_BODY_MOTION;continue;
                case MSI_CROUCH:p->mode=MCR_ENTER;b->domain=MARCO_BODY_CROUCH;continue;
                case MSI_FAST:b->stance.mode=MST_IDLE_FAST;continue;
                case MSI_MELEE:m->mode=MMR_MELEE_REQUEST;b->domain=MARCO_BODY_MOTION;continue;
                case MSI_DRAW: {
idle_draw:; // C2144..C215A: support first, then body clock.
                    bool complete=false;
                    if(!motion || !motion->move || !motion->animate ||
                       !motion->move(motion->context,MMM_GROUND,m,a) ||
                       !motion->animate(motion->context,m,a,&complete)) goto fail;
                    if(complete) b->stance.mode=MST_IDLE_FAST;
                    return MARCO_BODY_COMMON;
                }
                case MSI_WORLD:
                    if(!stance || !stance->idle_query) return MARCO_BODY_EXTERNAL;
                    if(!marco_stance_idle_world(&b->stance,m,a,stance)) goto fail;
                    goto idle_draw;
                }
                goto fail;
            }
            if(!stance || !marco_stance_enter(&b->stance,m,a,stance)) goto fail;
            break;
        default:goto fail;
        }
    }
fail:
    p->failed=true;p->upper.failed=true;a->failed=true;return MARCO_BODY_FAILED;
}
