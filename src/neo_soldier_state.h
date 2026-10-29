#ifndef MSX_NEO_SOLDIER_STATE_H
#define MSX_NEO_SOLDIER_STATE_H
#include "neo_animation.h"
#include "neo_contact.h"
#include "neo_control.h"
#include "neo_soldier_action.h"
/* ROM labels are provenance tags, never executable pointers or CPU opcodes. */
typedef struct { uint32_t routine; uint16_t field78; uint8_t field7e,field81,field61; uint16_t field72; } neo_soldier_state_t;
typedef const neo_anim_program_t *(*neo_soldier_program_t)(void *context,uint32_t tag);
typedef struct {
    /* Original 0049E8 / 040200 query engines remain external scene services. */
    uint16_t region_code,object_code,object_flags;
    const uint16_t *delay_grid;
    const neo_control_frame_t *scene;
} neo_soldier_senses_t;
/* Grounded observed subset. Unsupported routine/asset/context returns false.
 * On success initializes animation and scalar/speed before native motion. */
bool neo_soldier_state_begin(neo_soldier_state_t *state,neo_contact_actor_t *actor,
    neo_control_t *control,neo_anim_state_t *animation,neo_rng_t *rng,
    neo_soldier_program_t resolve,void *context);
/* ROM AF582..AF5B8, after the ground result and before animation. */
void neo_soldier_state_motion_ready(neo_soldier_state_t *state,
    const neo_contact_actor_t *actor,const neo_contact_result_t *ground);
/* After native motion/animation, preserving instruction order of state writes.
 * Scene query engines remain external; action selection itself is native. */
bool neo_soldier_state_end(neo_soldier_state_t *state,const neo_contact_actor_t *actor,
    neo_control_t *control,uint16_t ground_flags,bool animation_complete,
    const neo_soldier_senses_t *senses,neo_rng_t *rng);
#endif
