#ifndef MSX_NEO_TARGET_H
#define MSX_NEO_TARGET_H
#include "neo_contact.h"
#include "neo_control.h"
typedef struct { bool valid; uint8_t player; uint16_t x,y; } neo_target_t;
/* Original 01AFFC/01AF3C/01AF9E: keep players inside the oriented relative
 * rectangle, then select the nearest horizontal target, preferring P1 on ties. */
neo_target_t neo_target_pick(const neo_contact_actor_t *actor,
    const neo_control_frame_t *scene,const int16_t rectangle[4]);
#endif
