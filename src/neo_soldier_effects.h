#ifndef MSX_NEO_SOLDIER_EFFECTS_H
#define MSX_NEO_SOLDIER_EFFECTS_H
#include "neo_animation.h"
#include "neo_throw.h"
/* Replace native palette resources; return the renderer's logical byte handle.
 * Spawn receives a temporary value and must copy it into a deferred actor queue. */
typedef bool (*neo_soldier_palette_t)(void *context,uint16_t asset,uint8_t *handle);
typedef bool (*neo_soldier_spawn_t)(void *context,const neo_contact_actor_t *actor,
    uint32_t constructor_tag,uint16_t field6e);
typedef struct {
    const neo_contact_actor_t *source;
    const neo_contact_parent_t *scene;
    uint8_t epoch;
    neo_soldier_palette_t replace_palette;
    neo_soldier_spawn_t enqueue;
    void *context;
} neo_soldier_effects_t;
/* Native effects of original 040648 and 03BB8E/01ACF2/01AC94. */
bool neo_soldier_animation_event(void *context,neo_anim_kind_t kind,uint32_t asset,
    neo_anim_render_t *render);
#endif
