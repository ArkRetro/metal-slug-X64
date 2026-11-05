#include <assert.h>
#include <stdio.h>
#include "../src/marco_campaign_sensing.h"
#include "marco_campaign_environment_cases.h"

static void original_environment_results(void) {
    for(unsigned i=0;i<sizeof(original_environment_cases)/sizeof(*original_environment_cases);++i) {
        marco_campaign_environment_t environment={0};
        unsigned player=original_environment_cases[i].slot!=0;
        environment.players[player]=(marco_campaign_environment_slot_t){
            .active=original_environment_cases[i].active,.field12=original_environment_cases[i].field12,
            .field1c=original_environment_cases[i].marker};
        marco_campaign_sensing_t scene={.field4301=original_environment_cases[i].scene};
        uint16_t value=original_environment_cases[i].value;bool carry=false;
        assert(marco_campaign_sensing_environment_update(&environment,original_environment_cases[i].slot,
            original_environment_cases[i].counter,&scene,&value,&carry));
        assert(environment.players[player].active==original_environment_cases[i].result_active);
        assert(environment.players[player].field1c==original_environment_cases[i].result_marker);
        assert(environment.pending[player]==original_environment_cases[i].result_write);
        assert(value==original_environment_cases[i].result_value && carry==original_environment_cases[i].result_carry);
    }
}

static void environment_service(void) {
    marco_campaign_environment_t environment={0};
    marco_campaign_sensing_t scene={0};
    uint16_t value=0xa5ee;bool carry=true;
    assert(marco_campaign_sensing_environment_update(&environment,0,1,&scene,&value,&carry));
    assert(!carry && value==0xa500 && !environment.players[0].active);
    assert(environment.policy[0]==MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT && environment.pending[0]);
    assert(!environment.pending[1]);
    marco_campaign_environment_effect_t effect;
    assert(marco_campaign_sensing_environment_take_effect(&environment,0,&effect));
    assert(effect.player==0 && effect.policy==MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT);
    assert(effect.words[0]==28 && effect.words[1]==0 && effect.words[2]==264 && effect.words[3]==480);
    assert(!marco_campaign_sensing_environment_take_effect(&environment,0,&effect));
    environment.players[0].field1c=0x81;value=0x12fe;
    assert(marco_campaign_sensing_environment_update(&environment,0,1,&scene,&value,&carry));
    assert(value==0x1240 && environment.players[0].field1c==0x40 && !carry);
    assert(marco_campaign_sensing_environment_take_effect(&environment,0,&effect));
    /* Active + zero marker skips the epoch read and preserves all of D0. */
    environment.players[0]=(marco_campaign_environment_slot_t){.active=7,.field12=0x4321};
    value=0x98ab;
    assert(marco_campaign_sensing_environment_update(&environment,0,0x4321,&scene,&value,&carry));
    assert(!carry && value==0x98ab && !environment.players[0].active && environment.players[0].field1c==1);
    assert(marco_campaign_sensing_environment_take_effect(&environment,0,&effect));
    /* Matching epoch with the scene query clear is the only carry path. */
    environment.players[1]=(marco_campaign_environment_slot_t){.active=0xffff,.field12=0xffff,.field1c=0x80};
    value=0x1234;
    assert(marco_campaign_sensing_environment_update(&environment,0xffff,0xffff,&scene,&value,&carry));
    assert(carry && value==0xffff && environment.players[1].active==0xffff && !environment.players[1].field1c);
    assert(environment.policy[1]==MARCO_CAMPAIGN_WORLD_POLICY_UNSET && !environment.pending[1]);
    /* Epoch mismatch and a nonzero scene byte both schedule the default. */
    environment.players[1]=(marco_campaign_environment_slot_t){.active=1,.field12=65535,.field1c=1};
    assert(marco_campaign_sensing_environment_update(&environment,2,0,&scene,&value,&carry));
    assert(!carry && value==65535 && !environment.players[1].active && environment.players[1].field1c==1);
    assert(marco_campaign_sensing_environment_take_effect(&environment,2,&effect) && effect.player==1);
    environment.players[1]=(marco_campaign_environment_slot_t){.active=1,.field12=3,.field1c=1};
    scene.field4301=0x80;
    assert(marco_campaign_sensing_environment_update(&environment,1,3,&scene,&value,&carry));
    assert(!carry && value==3 && !environment.players[1].active && environment.players[1].field1c==1);
    assert(environment.pending[1]);
    /* A carry return does not erase an earlier, unconsumed policy write. */
    scene.field4301=0;environment.players[1].active=1;
    assert(marco_campaign_sensing_environment_update(&environment,1,3,&scene,&value,&carry));
    assert(carry && environment.pending[1]);
    assert(marco_campaign_sensing_environment_take_effect(&environment,1,&effect));
    assert(!marco_campaign_sensing_environment_update(NULL,0,0,&scene,&value,&carry));
    assert(!marco_campaign_sensing_environment_update(&environment,0,0,NULL,&value,&carry));
}

int main(void) {
    original_environment_results();
    environment_service();
    marco_campaign_sensing_t s={.field4301=1,.field4304=0xff};
    uint16_t value=0x1234;bool carry=false;
    assert(marco_campaign_sensing_idle_query(&s,MSQ_18F24,0,&value,&carry));
    assert(carry && value==0x1234);
    s.field4301=0;
    assert(marco_campaign_sensing_idle_query(&s,MSQ_18F24,0,&value,&carry) && !carry);
    assert(marco_campaign_sensing_idle_query(&s,MSQ_18FFE,0x8100,&value,&carry));
    assert(value==0x207f && carry);
    s.field4304=4;
    assert(marco_campaign_sensing_idle_query(&s,MSQ_18FFE,0,&value,&carry));
    assert(value==1 && !carry);
    static const uint8_t wanted[13]={3,1,0,0,0,0,0,0,0,2,1,2,0};
    for(unsigned i=0;i<sizeof(wanted);++i) {
        s.field42f6=i;carry=true;
        assert(marco_campaign_sensing_idle_query(&s,MSQ_191FC,0,&value,&carry));
        assert(value==wanted[i] && !carry);
    }
    assert(!marco_campaign_sensing_idle_query(&s,MSQ_14850,0,&value,&carry));
    marco_upper_t upper={.mode=MUP_FIRE_WAIT,.combo95=7,.weapon_kind=5,.aim92=2};
    marco_ammo_t ammo={.ammunition=4,.active=2,.cap=3,.kind=5};
    assert(marco_campaign_sensing_reset_weapon(&upper,&ammo));
    assert(!upper.combo95 && !upper.weapon_kind && upper.mode==MUP_FIRE_WAIT && upper.aim92==2);
    assert(ammo.ammunition==65535 && !ammo.active && ammo.cap==255 && !ammo.kind);
    assert(marco_ammo_reserve(&ammo,0,false,false) && ammo.ammunition==65535 && !ammo.active);
    int16_t x,y;
    assert(marco_campaign_sensing_registered_damage(0,-128,127,&x,&y,&carry)==0);
    assert(!x && !y && !carry);
    assert(marco_campaign_sensing_registered_damage(16,-128,127,&x,&y,&carry)==16);
    assert(x==-256 && y==254 && carry);
    puts("PASS: native environment transitions/policy writes, scene query widths, pistol reset and damage return");
}
