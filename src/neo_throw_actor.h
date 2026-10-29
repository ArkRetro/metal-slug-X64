#ifndef MSX_NEO_THROW_ACTOR_H
#define MSX_NEO_THROW_ACTOR_H
#include "neo_animation.h"
#include "neo_throw.h"
#include "neo_cull.h"
enum { NEO_THROW_NEW,NEO_THROW_FLYING,NEO_THROW_BURST,NEO_THROW_EFFECT,NEO_THROW_FAILED };
enum { NEO_THROW_AUDIO_0=1 };
typedef bool (*neo_throw_palette_t)(void *context,uint16_t asset,bool replace,uint8_t *handle);
typedef void (*neo_throw_release_t)(void *context);
typedef struct {
    neo_contact_actor_t body;
    neo_anim_state_t animation;
    neo_anim_render_t render;
    uint16_t field6e;
    uint8_t phase;
    bool active,owns_resources;
    neo_throw_palette_t resource_palette;
    neo_throw_release_t resource_release;
    void *resource_context;
} neo_throw_actor_t;
typedef struct {
    const neo_anim_program_t *flight,*burst;
    const uint16_t *flight_rectangle,*burst_rectangle;
    neo_throw_palette_t palette;
    neo_throw_release_t release;
    void *context;
} neo_throw_services_t;
typedef struct {
    const neo_contact_parent_t *scene;
    const neo_control_frame_t *players;
    const neo_cull_bounds_t *bounds;
    uint16_t video_phase,region_code;
    uint8_t epoch;
    bool other_contact;
} neo_throw_frame_t;
typedef enum { NEO_THROW_MORE,NEO_THROW_DONE,NEO_THROW_UNSUPPORTED } neo_throw_result_t;
/* Seed is a copied deferred-spawn value. Graphics seed/flash belong to the
 * native allocator/resource layer, not gameplay ROM registers. */
void neo_throw_actor_start(neo_throw_actor_t *state,const neo_contact_actor_t *seed,
    uint16_t field6e,uint8_t flash,const neo_anim_render_t *graphics_seed);
/* Idempotent cleanup using the resource owner bound by the first acquisition.
 * A failed palette request must leave that owner's allocations unchanged. */
void neo_throw_actor_cancel(neo_throw_actor_t *state);
/* UNSUPPORTED is terminal: cancel resources exactly once and emit no events.
 * Repeated steps remain UNSUPPORTED; the actor is not a retryable partial state.
 * The two scene collision services remain inputs. This module computes phase
 * changes, physics, animation, color cycling and resource lifetime itself. */
neo_throw_result_t neo_throw_actor_step(neo_throw_actor_t *state,const neo_throw_frame_t *frame,
    const neo_throw_services_t *services,neo_anim_emit_t emit,void *context,unsigned *events);
#endif
