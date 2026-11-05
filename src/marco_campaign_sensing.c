#include "marco_campaign_sensing.h"

bool marco_campaign_sensing_environment_update(marco_campaign_environment_t *environment,
    uint16_t slot,uint16_t counter,const marco_campaign_sensing_t *scene,
    uint16_t *value,bool *carry) {
    if(!environment || !scene || !value || !carry) return false;
    unsigned player=slot!=0;
    marco_campaign_environment_slot_t *state=&environment->players[player];
    if(!state->active) {
        /* MOVE.B, LSR.B, MOVE.B preserve the upper byte of D0.word. */
        state->field1c>>=1;
        *value=(uint16_t)((*value&0xff00u)|state->field1c);
    } else {
        if(state->field1c) {
            *value=state->field12;
            if(state->field12==counter && !scene->field4301) {
                state->field1c=0;*carry=true;return true;
            }
        }
        state->active=0;state->field1c=1;
    }
    /* AA4BC assigns the default collision policy even when active was zero.
     * Keep it as live native state and a pending assignment for its owner. */
    environment->policy[player]=MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT;
    environment->pending[player]=true;*carry=false;return true;
}

bool marco_campaign_sensing_environment_take_effect(marco_campaign_environment_t *state,
    uint16_t slot,marco_campaign_environment_effect_t *effect) {
    if(!state || !effect) return false;
    unsigned player=slot!=0;
    if(!state->pending[player]) return false;
    if(state->policy[player]!=MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT) return false;
    *effect=(marco_campaign_environment_effect_t){.player=(uint8_t)player,
        .policy=MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT,.words={0x001c,0x0000,0x0108,0x01e0}};
    state->pending[player]=false;return true;
}

bool marco_campaign_sensing_idle_query(const marco_campaign_sensing_t *s,
    marco_idle_query_t kind,uint16_t incoming,uint16_t *value,bool *carry) {
    if(!s || !value || !carry) return false;
    switch(kind) {
    case MSQ_18F24:
        *carry=s->field4301!=0;return true;
    case MSQ_18FFE: {
        uint16_t word=(uint16_t)((incoming&0xff00u)|s->field4304);
        *value=word>>2;*carry=(word&2u)!=0;return true;
    }
    case MSQ_191FC:
        if(s->field42f6==1 || s->field42f6==10) *value=1;
        else if(s->field42f6==9 || s->field42f6==11) *value=2;
        else *value=s->field42f6==0?3:0;
        /* Final MOVEQ clears carry on every path. */
        *carry=false;return true;
    case MSQ_14850:return false;
    }
    return false;
}

bool marco_campaign_sensing_reset_weapon(marco_upper_t *u,marco_ammo_t *ammo) {
    if(!u || !ammo) return false;
    u->combo95=0;u->weapon_kind=0;
    /* The two source tables are C08A8[0]=FF and C08B8[0]=FFFF. */
    *ammo=(marco_ammo_t){.ammunition=UINT16_MAX,.active=0,.cap=UINT8_MAX,.kind=0};
    return true;
}

uint16_t marco_campaign_sensing_registered_damage(uint8_t timer5a,
    int8_t impact_x,int8_t impact_y,int16_t *x,int16_t *y,bool *carry) {
    if(x) *x=timer5a?(int16_t)(impact_x*2):0;
    if(y) *y=timer5a?(int16_t)(impact_y*2):0;
    if(carry) *carry=timer5a!=0;
    return timer5a;
}
