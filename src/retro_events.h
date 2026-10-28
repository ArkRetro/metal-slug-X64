#ifndef MSX_RETRO_EVENTS_H
#define MSX_RETRO_EVENTS_H
#include "gameplay.h"
#include "retro25d.h"
/* Render-side observer for the depth25d mode. It only reads msx_game_t
 * snapshots (never writes gameplay, collision or sprite data) and turns
 * changes into cosmetic depth cues: the vehicle's contact shadow, short
 * point-light pulses for bullet impacts and the vehicle's destruction, and
 * the camera-motion fog focus. Pure C: host tests exercise it directly. */
typedef struct {
    msx_bullet_t bullets[MSX_BULLETS],enemy_bullets[MSX_ENEMY_BULLETS];
    unsigned ticks,deaths,vehicle_events,vehicle_health;
    int camera;
    bool primed;
    float focus;
    retro_pulses_t pulses;
    unsigned impacts,explosions; /* Diagnostics: pulses started so far. */
} retro_events_t;
/* Footprint of the observed ground vehicle (wheels span ~94 px). */
retro_shadow_t retro_vehicle_shadow(const neo_vehicle_t *vehicle);
/* Call once per rendered frame after the simulation steps and after the
 * frame's own shadows/lights were added; appends the cues to frame. */
void retro_events_frame(retro_events_t *events,const msx_game_t *game,retro_frame_t *frame);
#endif
