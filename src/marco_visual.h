#ifndef MSX_MARCO_VISUAL_H
#define MSX_MARCO_VISUAL_H
#include <stdbool.h>
#include <stdint.h>
#include "marco_animation.h"
/* Campaign Marco animation (docs/MARCO_CAMPAIGN.md): the original body and
 * weapon programs (converted graph of an audited catalog capture) stepped
 * by the native animation engine once per logic update, as the original
 * player does every second video tick. The program for each situation is
 * one the original selection code (0C0F32/0C0F62) chose under the same input
 * condition in the capture (tools/compile_marco_campaign.py checks it); the
 * situation itself comes from the port's gameplay state, not from the
 * original controller (0C1xxx..0C3xxx), whose world services are not
 * connected yet. Drawing is marco_renderer_draw_native. */
enum { MARCO_VISUAL_DRAWS=8 };
typedef struct {
    bool grounded,moving,crouch,aim_up,fired;
    bool running_jump;     /* takeoff while moving */
    int facing;            /* -1 left, 1 right */
    uint16_t x,y;          /* projected words of the player (+3C/+40) */
    uint8_t form;          /* +93: 1 with the H (the form-1 rows, +0x10, of the tables) */
    int hgun_program;      /* H upper body (marco_hgun.c): its program (marco_hgun_program_t), -1 none */
    bool hgun_start;       /* that program started (or restarted) in this update */
} marco_visual_input_t;
typedef struct {
    marco_animation_t anim;
    marco_animation_view_t view;
    int body_role,weapon_role;     /* MARCO_CAMPAIGN_* entry, -1 none */
    bool body_done,weapon_done;    /* program finished: its last draws stay */
    bool grounded,crouch,moving,running_jump;
    uint8_t form;                  /* +93 of the previous update */
    uint16_t base_x,base_y;
    neo_anim_draw_t body_draws[MARCO_VISUAL_DRAWS],weapon_draws[MARCO_VISUAL_DRAWS];
    unsigned body_count,weapon_count,collect;   /* collect: 0 body, 1 weapon */
    unsigned failures,starts;
} marco_visual_t;
void marco_visual_reset(marco_visual_t *v);
/* One logic update. */
void marco_visual_step(marco_visual_t *v,const marco_visual_input_t *in);
/* Draws of the current update in emission order (body, then weapon). */
unsigned marco_visual_draws(const marco_visual_t *v,neo_anim_draw_t *out,unsigned max);
#endif
