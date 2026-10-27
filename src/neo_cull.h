#ifndef MSX_NEO_CULL_H
#define MSX_NEO_CULL_H
#include "neo_contact.h"
typedef struct {
    bool has_parent;
    int32_t x,y,previous_x,previous_y;
    uint16_t left,top,right,bottom;
} neo_cull_bounds_t;
/* ROM 01945C..019502. This is directional history-based removal, not a
 * generic rectangle-visibility test. A stationary axis does not trigger it. */
bool neo_cull_outside(const neo_contact_actor_t *actor,const neo_cull_bounds_t *bounds,
    const uint16_t rectangle[4]);
#endif
