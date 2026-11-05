#ifndef MSX_MARCO_CAMPAIGN_QUERIES_H
#define MSX_MARCO_CAMPAIGN_QUERIES_H
#include "neo_hit.h"

/* Converted ROM metadata. Tags identify data only; they are never callable or
 * interpreted as an address. Unknown nonzero descriptors are unsupported. */
typedef struct {
    uint8_t actor_class,groups;
    neo_hit_rectangle_t rectangle;
} marco_campaign_hit_descriptor_t;
bool marco_campaign_hit_descriptor(uint32_t tag,marco_campaign_hit_descriptor_t*);

/* The scene imports actual native actors into this projected, Y-up view.
 * identity is a native scene identity, used to exclude the querying actor.
 * projected coordinates correspond to original +3C/+40 word values. */
typedef struct {
    uint32_t identity,field50_tag;
    uint16_t projected_x,projected_y,hitpoints;
    uint8_t flags15,flags5f,timer5a;
    int8_t impact_x,impact_y;
    uint8_t registry_index;
} marco_campaign_hit_actor_t;
typedef struct {
    marco_campaign_hit_actor_t actor;
    uint16_t x_cells;
    uint8_t y_cells,groups;
} marco_campaign_hit_registration_t;
#define MARCO_CAMPAIGN_HIT_CAPACITY 64
typedef struct {
    marco_campaign_hit_registration_t entries[MARCO_CAMPAIGN_HIT_CAPACITY];
    uint8_t count;
} marco_campaign_hit_registry_t;

/* Reset the bank selected for new registrations. The scene explicitly selects
 * the other bank for proximity queries, matching 004C38/0049FC. */
void marco_campaign_hit_registry_reset(marco_campaign_hit_registry_t*);
/* Original registry entries retain actor identity while narrow-phase reads
 * live actor state. Refresh a previously registered native identity without
 * rebuilding its older broad-phase masks. Missing identities are harmless. */
bool marco_campaign_hit_registry_refresh(marco_campaign_hit_registry_t*,
    const marco_campaign_hit_actor_t*);
/* 004BAC..004DC8 for recovered one-rectangle descriptors. kill_nonplayer is
 * the scene's E7E2 or F001.bit0/player-death gate, not a guessed contact flag.
 * Registration itself does not create damage. Damage producers own timer5a
 * and impact bytes before this function returns their native result. */
bool marco_campaign_hit_register(marco_campaign_hit_registry_t*,
    marco_campaign_hit_actor_t*,bool kill_nonplayer,uint16_t *value,bool *carry);
/* Read-only common-tail 0049E8 with the actual ROM 2D0AE4 query descriptor:
 * type zero, group one, zero damage. Returns max overlapping actor class.
 * The caller supplies the opposite registration bank and live native actor
 * views; unsupported descriptors fail rather than imply no collision. */
bool marco_campaign_hit_proximity(const marco_campaign_hit_registry_t*,
    const marco_campaign_hit_actor_t*,uint16_t *value,bool *carry);

/* C419E's two actual ROM 0049E8 attack descriptors. The overlap producer
 * emits typed damage requests to the native scene owner. It does not claim
 * the unrecovered generic 004892 damage/compatibility/response policy. */
typedef struct {
    neo_hit_rectangle_t rectangle;
    uint16_t type,groups,damage;
} marco_campaign_melee_descriptor_t;
bool marco_campaign_melee_descriptor(uint32_t tag,marco_campaign_melee_descriptor_t*);
typedef struct {
    uint32_t attacker,target;
    uint16_t type,damage,impact_x,impact_y;
    uint8_t target_class;
} marco_campaign_melee_request_t;
typedef bool (*marco_campaign_melee_request_handler_t)(void*,const marco_campaign_melee_request_t*);
bool marco_campaign_hit_melee_requests(const marco_campaign_hit_registry_t*,
    const marco_campaign_hit_actor_t*,uint32_t descriptor_tag,
    marco_campaign_melee_request_handler_t,void*,uint16_t *value,bool *carry);
#endif
