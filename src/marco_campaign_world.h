#ifndef MSX_MARCO_CAMPAIGN_WORLD_H
#define MSX_MARCO_CAMPAIGN_WORLD_H
#include "marco_common.h"
#include "neo_contact.h"

/* Native adapter for the existing Mission-1 terrain solver. It retains the
 * recovered request/commit, scalar velocity and C109C wrapper semantics. The
 * map solver's measured floor/roof subset remains separate from the complete
 * 013968 kernel: moving supports, solid ceilings and actor push candidates
 * require their own owner. Coordinates are world Q16.16, Y upwards. */
typedef struct {
    neo_contact_actor_t contact;
    neo_contact_parent_t terrain;
    uint16_t policy_words[4];
    int32_t camera_x,scroll_y,projected_y_offset;
    uint8_t epoch;
    uint8_t boundary_hits; /* 0138A0 scratch right/left/up/down, bits 0..3. */
    bool initialized,policy_enabled;
} marco_campaign_world_t;

/* Entry requires a surface at the supplied feet, never a guessed floor. */
bool marco_campaign_world_init(marco_campaign_world_t*,marco_crouch_t*,int32_t x,int32_t y);
/* Import a real authored-world move, such as its damage/respawn response.
 * Grounded moves require an exact native support match. Refresh its material
 * and tangent plus presentation history atomically; preserve velocity, policy,
 * epoch and controller state. This is scene adaptation, not an invented
 * original teleport routine. Failure leaves both objects unchanged. */
bool marco_campaign_world_relocate(marco_campaign_world_t*,marco_crouch_t*,
    int32_t x,int32_t y,bool grounded);
/* 0128E8 selects four policy words; 0138A0 clips the requested displacement
 * in scene projection coordinates. All offsets use Q16.16. Policy assignment
 * is separate from camera/scroll ownership. This preserves the source's
 * lower-Y correction and never invents solid-wall/ceiling contact bits. */
bool marco_campaign_world_set_policy(marco_campaign_world_t*,const uint16_t words[4],
    int32_t camera_x,int32_t scroll_y,int32_t projected_y_offset);
bool marco_campaign_world_clip_request(marco_campaign_world_t*,neo_contact_request_t*);
bool marco_campaign_world_ground(marco_campaign_world_t*,marco_crouch_t*);
bool marco_campaign_world_air(marco_campaign_world_t*,marco_crouch_t*,bool player_wrapper);
bool marco_campaign_world_motion_move(void*,marco_motion_service_t,marco_motion_t*,marco_animation_t*);
bool marco_campaign_world_crouch_move(void*,marco_crouch_t*,marco_animation_t*);
/* Projection values produced by 01ABC8/01AD8C. The scene still owns palette,
 * flash phase, and the remaining render fields. */
bool marco_campaign_world_view(const marco_campaign_world_t*,const marco_crouch_t*,marco_animation_view_t*);
#endif
