#ifndef MSX_MARCO_CAMPAIGN_SENSING_H
#define MSX_MARCO_CAMPAIGN_SENSING_H
#include "marco_stance.h"
#include "marco_ammo.h"

/* Native scene values read by 018F24, 018FFE and 0191FC. The scene owner
 * supplies these values; none is a CPU address or a recorded frame result. */
typedef struct {
    uint8_t field4301,field4304;
    uint16_t field42f6;
} marco_campaign_sensing_t;

/* Native values read/written by 0AA458. These are independent of animation
 * form and mode. An actor slot of zero selects P1; any other word selects P2. */
typedef struct {
    uint16_t active,field12;
    uint8_t field1c;
} marco_campaign_environment_slot_t;
typedef enum {
    MARCO_CAMPAIGN_WORLD_POLICY_UNSET,
    MARCO_CAMPAIGN_WORLD_POLICY_DEFAULT
} marco_campaign_world_policy_t;
typedef struct {
    marco_campaign_environment_slot_t players[2];
    marco_campaign_world_policy_t policy[2];
    bool pending[2];
} marco_campaign_environment_t;
typedef struct {
    uint8_t player;
    marco_campaign_world_policy_t policy;
    /* Source 016DBE, copied by 0128E8 into the collision request context.
     * These are numerical policy words, never a callable ROM pointer. */
    uint16_t words[4];
} marco_campaign_environment_effect_t;

/* 0AA458..0AA4C6, including its 018F24 test. value is the incoming D0.word;
 * its unchanged lanes survive byte writes. The query result and the default
 * world-policy assignment are independent: the owner consumes pending policy
 * writes before a subsequent movement service that uses that context. */
bool marco_campaign_sensing_environment_update(marco_campaign_environment_t*,
    uint16_t player_slot,uint16_t counter10000e,const marco_campaign_sensing_t*,
    uint16_t *value,bool *carry);
/* Acknowledges the selected slot's pending assignment. An already-consumed
 * assignment is not re-emitted until the original routine writes it again. */
bool marco_campaign_sensing_environment_take_effect(marco_campaign_environment_t*,
    uint16_t player_slot,marco_campaign_environment_effect_t*);

/* 018FFE changes D0.byte before LSR.W. incoming_word retains the other byte
 * explicitly; native input lanes with only eight button bits supply zero.
 * 014850 is a separate terrain kernel query and is deliberately unsupported. */
bool marco_campaign_sensing_idle_query(const marco_campaign_sensing_t*,
    marco_idle_query_t,uint16_t incoming_word,uint16_t *value,bool *carry);

/* C0EE2..C0F14: actor +95 and all four per-player weapon-service values.
 * Preserves the upper controller's mode, aim and animation clocks. */
bool marco_campaign_sensing_reset_weapon(marco_upper_t*,marco_ammo_t*);

/* 004DA6..004DC8, the return portion of 004C24 after object registration.
 * This does not register geometry or discover contacts. The owner must have
 * completed registration and native damage producers before calling it. */
uint16_t marco_campaign_sensing_registered_damage(uint8_t timer5a,
    int8_t impact_x,int8_t impact_y,int16_t *offset_x,int16_t *offset_y,bool *carry);
#endif
