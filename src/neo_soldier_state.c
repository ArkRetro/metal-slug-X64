#include "neo_soldier_state.h"
#include "neo_soldier.h"
#include "neo_motion.h"
#include <stddef.h>
#define NEO_STATE_ROUTINE_TABLE
#include "state_tables.h"

static uint16_t high(int32_t value) { return (uint16_t)((uint32_t)value>>16); }
static uint16_t abs_word(uint16_t value) { return value&0x8000u?(uint16_t)(0u-value):value; }

bool neo_soldier_state_begin(neo_soldier_state_t *s,neo_contact_actor_t *a,
    const neo_control_t *c,neo_anim_state_t *animation,neo_rng_t *rng,
    neo_soldier_program_t resolve,void *context) {
    uint32_t program=0,next=s->routine;
    if (!(a->flags15&64u)) return false; // Air entry routes require separate recovery.
    switch (s->routine) {
    case 0xaf65a:program=0x23d510;next=0xaf6c0;break;
    case 0xaf890:program=0x23d594;next=0xaf8a6;break;
    case 0xaf380:program=0x23d5e2;next=0xaf39e;break;
    case 0xaf8d4:program=0x23ddb4;next=0xaf8ea;break;
    case 0xb0336:program=0x23ef98;next=0xb0354;break;
    case 0xaf6c0:case 0xaf8a6:case 0xaf39e:case 0xaf8ea:case 0xb0354:break;
    default:return false;
    }
    const neo_anim_program_t *asset=NULL;
    if (program) {
        if (!resolve || !(asset=resolve(context,program))) return false;
        neo_anim_start(animation,asset,0x100);
    }
    if (s->routine==0xaf65a) {
        neo_soldier_motion_seed_t seed=neo_soldier_seed_from_rng(rng,a->flags15,(int16_t)c->delay92);
        a->scalar62=seed.value_62;animation->speed=seed.value_26;
    } else if (s->routine==0xaf380 || s->routine==0xb0336) a->scalar62=0;
    s->routine=next;
    if (next==0xaf8a6) a->scalar62=neo_soldier_damp_motion(a->scalar62);
    if (next==0xaf8ea) a->scalar62-=neo_asr(a->scalar62,2);
    return true;
}
/* Original AF02A..AF070 carry result. Field6D is the angle's low byte. */
static bool contact_decision(const neo_contact_actor_t *a,const neo_control_t *c) {
    if ((a->flags5f&32u) && (c->flags7c&16u) && (uint8_t)a->kind_angle==64u) return true;
    if (c->flags7c&(32u|64u)) return false;
    return (c->flags7c&4u)!=0;
}
/* Original 00510C indexing, shared scene grid supplied in native word order. */
static uint16_t lookup(const uint16_t *grid,uint16_t x,uint16_t y) {
    x=(uint16_t)(x-32u);y=(uint16_t)(y-256u);
    if (x>=512u || y>=256u) return 0;
    unsigned offset=((x>>1)&240u)+((y>>4)&14u);
    uint16_t value=grid[offset/2];return value>=256u?255u:value;
}
static void environment_decisions(neo_soldier_state_t *s,const neo_contact_actor_t *a,
    const neo_control_t *c,const neo_soldier_senses_t *f) {
    if (contact_decision(a,c)) s->routine=0xafbc0;
    if ((c->flags7c&2u) && (uint8_t)a->kind_angle==64u &&
        lookup(f->delay_grid,high(a->projected_x),(uint16_t)(high(a->projected_y)+32u))>=64u)
        s->routine=0xafd94;
}
static void object_decision(neo_soldier_state_t *s,const neo_contact_actor_t *a,
    const neo_soldier_senses_t *f) {
    if (f->object_code&0x8000u) return;
    s->field7e=(uint8_t)f->object_code;
    if ((a->flags5f&32u) && (f->object_flags&1u)) {s->routine=0xb00c0;return;}
    if ((a->flags5f&16u) && (f->object_flags&2u)) s->routine=0xb01ce;
}
bool neo_soldier_state_end(neo_soldier_state_t *s,const neo_contact_actor_t *a,
    neo_control_t *c,uint16_t ground_flags,bool complete,const neo_soldier_senses_t *f,neo_rng_t *rng) {
    uint32_t mode=s->routine;
    if (mode==0xaf8a6 || mode==0xaf8ea || mode==0xb0354) {
        if (complete) s->routine=mode==0xaf8ea?0xaf65a:0xaf380;
        if (mode==0xb0354 && !(a->flags15&64u)) s->routine=0xafce2;
        return true;
    }
    if (mode!=0xaf6c0 && mode!=0xaf39e) return false;
    if (!f || !f->delay_grid || !f->scene || !rng) return false;
    if (mode==0xaf6c0 && (a->flags5f&1u)) return false; // AF072 branch not recovered.
    neo_soldier_state_t candidate=*s;
    if (mode==0xaf6c0) {
        neo_soldier_ground_inputs_t input={ground_flags,s->field78,high(c->target84),high(a->projected_x),
            high(a->scalar62),c->flags7c,(uint8_t)(a->kind_angle>>8)};
        candidate.routine=neo_soldier_ground_select_prefix(candidate.routine,&input);
        environment_decisions(&candidate,a,c,f);
    } else if ((uint8_t)(a->kind_angle>>8)!=3u) {
        uint16_t delta=(uint16_t)(high(c->target84)-high(a->projected_x));
        uint16_t oriented=(uint16_t)(delta^((a->flags15&1u)?0u:0xffffu));
        uint16_t distance=abs_word(delta);
        if (distance>=16u) candidate.routine=0xaf4f6;
        if (!(oriented&0x8000u)) {if (distance>=48u) candidate.routine=0xaf65a;}
        else {if (distance>=48u) candidate.routine=0xafc68;if (distance>=72u) candidate.routine=0xaf8d4;}
        environment_decisions(&candidate,a,c,f);
    }
    if ((c->flags7c&8u) && f->region_code==5u) candidate.routine=0xb0336;
    if (mode==0xaf39e || (c->flags7c&1u)) {
        uint8_t action=neo_soldier_action_choose(c,a,f->scene,candidate.field81,rng);
        if (state_action_routines[action]) candidate.routine=state_action_routines[action];
    }
    if (mode==0xaf39e && !(c->flags7c&1u)) candidate.routine=0xaf4b4;
    object_decision(&candidate,a,f);
    if (mode==0xaf39e && (uint16_t)(high(a->projected_x)-24u)<272u &&
        (c->selected80&0x80u)) candidate.routine=0xb0396;
    if (!c->counter74) candidate.routine=0xb063e;
    *s=candidate;return true;
}
