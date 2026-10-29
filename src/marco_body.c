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
            case MMR_MELEE_REQUEST:return MARCO_BODY_EXTERNAL;
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
                case MSI_MELEE:m->mode=MMR_MELEE_REQUEST;b->domain=MARCO_BODY_MOTION;return MARCO_BODY_EXTERNAL;
                case MSI_DRAW:case MSI_WORLD:return MARCO_BODY_EXTERNAL;
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
