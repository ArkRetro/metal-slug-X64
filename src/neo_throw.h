#ifndef MSX_NEO_THROW_H
#define MSX_NEO_THROW_H
#include "neo_target.h"
/* Native subset of the actor spawned by original callback 03BB8E.
 * Palette allocation, animation and damage/region queries are separate. */
void neo_throw_spawn(neo_contact_actor_t *child,const neo_contact_actor_t *source,
    const neo_contact_parent_t *scene,uint8_t epoch);
/* ROM 03BC52..03BD34: orientation, layer and initial ballistics. The native
 * scene supplies player states; no ROM/68000 memory accesses occur here. */
void neo_throw_initialize(neo_contact_actor_t *actor,const neo_control_frame_t *scene,
    uint16_t field6e);
/* ROM 03BD3A..03BD50: update velocity before the air/contact wrapper. */
void neo_throw_accelerate(neo_contact_actor_t *actor);
/* Atomic airborne no-terrain-contact subset. Requires caller-established static
 * scene context with no relevant dynamic kernel candidates, a stationary scene
 * parent, unique floors, <=4px horizontal motion and clearance above terrain.
 * Collision/impact cases return false without modifying the actor. */
bool neo_throw_free_step(neo_contact_actor_t *actor,const neo_contact_parent_t *scene,
    uint8_t epoch,neo_contact_result_t *kernel_output);
#endif
