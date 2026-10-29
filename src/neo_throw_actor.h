#ifndef MSX_NEO_THROW_ACTOR_H
#define MSX_NEO_THROW_ACTOR_H
#include "neo_animation.h"
#include "neo_throw.h"
#include "neo_cull.h"
enum { NEO_THROW_NEW,NEO_THROW_FLYING,NEO_THROW_BURST,NEO_THROW_EFFECT };
enum { NEO_THROW_AUDIO_0=1, NEO_THROW_AUDIO_1027=2 };
/* Entry chosen after a flight update: 03BE52 on terrain contact (+15 bit 7),
 * 03BE6C on an attack hit (0049E8 with ROM 0E0894) or the 004C0E flag; the
 * later test wins, as in the original routine assignment order. */
enum { NEO_THROW_BURST_NONE, NEO_THROW_BURST_CONTACT, NEO_THROW_BURST_HIT };
typedef struct {
    neo_contact_actor_t body;
    neo_anim_state_t animation;
    neo_anim_render_t render;
    uint16_t field6e;
    uint8_t phase,burst;
    bool active;
} neo_throw_actor_t;
typedef bool (*neo_throw_palette_t)(void *context,uint16_t asset,bool replace,uint8_t *handle);
typedef void (*neo_throw_release_t)(void *context);
struct neo_throw_frame;
typedef struct {
    const neo_anim_program_t *flight,*burst;
    const uint16_t *flight_rectangle,*burst_rectangle;
    neo_throw_palette_t palette;
    neo_throw_release_t release;
    void *context;
    /* Optional complete flight step (03BD3A acceleration + 0143E4 air step
     * through the native collision kernel). When absent, the bounded
     * no-contact neo_throw_free_step() subset is used. */
    bool (*air)(void *context,neo_contact_actor_t *body,const struct neo_throw_frame *frame);
    /* Optional 0049E8 query with the flight attack row ROM 0E0894. */
    bool (*hit)(void *context,const neo_contact_actor_t *body);
} neo_throw_services_t;
typedef struct neo_throw_frame {
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
/* The two scene collision services remain inputs. This module computes phase
 * changes, physics, animation, color cycling and resource lifetime itself. */
neo_throw_result_t neo_throw_actor_step(neo_throw_actor_t *state,const neo_throw_frame_t *frame,
    const neo_throw_services_t *services,neo_anim_emit_t emit,void *context,unsigned *events);
#endif
