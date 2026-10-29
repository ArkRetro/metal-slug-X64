#ifndef MSX_MARCO_ANIMATION_H
#define MSX_MARCO_ANIMATION_H
#include "neo_animation.h"
typedef enum { MARCO_BODY, MARCO_WEAPON } marco_part_t;
typedef struct {
    /* Original +72/+74 are latched as signed offsets by ROM 0C3778. */
    uint16_t weapon_dx,weapon_dy;
    uint8_t field76,frame77,field78,variant79;
} marco_pose_fields_t;
typedef struct {
    neo_anim_state_t body,weapon;
    marco_pose_fields_t fields;
    /* These are shared actor fields, not one copy per animation thread. */
    uint32_t field50_tag;
    uint8_t flash,field18;
    bool failed;
} marco_animation_t;
typedef struct {
    neo_anim_render_t render;
    uint16_t cached_x,cached_y;
} marco_animation_view_t;
void marco_animation_reset(marco_animation_t *actor);
/* Main start (004256) clears shared field18; secondary start (004260) preserves
 * it. entry is an offline-converted command index, never a 68000 address. */
bool marco_animation_start(marco_animation_t *actor,marco_part_t part,
    const neo_anim_program_t *program,uint16_t entry,uint16_t speed);
/* Body: 00427A/00427E. Weapon: 0C3778 (temporary offsets, then 00427E).
 * This component does not choose programs from player input. A complete player
 * controller must supply the original call order, starts and external fields.
 * An unhandled event stops the actor; retry never duplicates prior requests. */
neo_anim_result_t marco_animation_step(marco_animation_t *actor,marco_part_t part,
    marco_animation_view_t *view,neo_anim_emit_t emit,void *draw_context,
    unsigned budget,neo_anim_event_t event,void *event_context);
#endif
