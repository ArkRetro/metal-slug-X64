#ifndef MSX_NEO_SOLDIER_ACTION_H
#define MSX_NEO_SOLDIER_ACTION_H
#include "neo_contact.h"
#include "neo_control.h"
/* ROM AEE0C..AEF12. Returns an action-table index, not an executable pointer.
 * The original player rectangle helper is pure apart from its CPU stack;
 * this translation can short-circuit its result when timer70 is nonzero. */
uint8_t neo_soldier_action_choose(neo_control_t *state,const neo_contact_actor_t *actor,
    const neo_control_frame_t *scene,uint8_t action_mask81,neo_rng_t *rng);
#endif
