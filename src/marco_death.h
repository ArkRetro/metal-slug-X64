#ifndef MSX_MARCO_DEATH_H
#define MSX_MARCO_DEATH_H
/* The player's death and new life (docs/JEFE_MISION1.md, "Muerte y
 * reaparición"): the generic death 0C2956/0C2DDA..0C2F12 (one hit kills),
 * the new life of the panel 0C95AE (lives, timer 019124) and the player's
 * respawn 0C1126/0196E8 at the record of the respawn table 10C31E (Mission 1:
 * bank 1 2D5C22..) with the type-0 entrance 0C1438 (floor 0146EE,
 * program 0BF498). Pure C: the caller does the motion with its own air and
 * ground steps (0143EE/014594) and tells the result. */
#include <stdbool.h>
#include <stdint.h>
#include "neo_animation.h"

typedef enum { MARCO_ALIVE,MARCO_DYING,MARCO_LYING,MARCO_GONE,MARCO_ENTERING } marco_life_phase_t;
typedef struct {
    marco_life_phase_t phase;
    neo_anim_state_t anim;      /* death/entrance program (+20..+26) */
    uint16_t speed_target;      /* +80 word while dying: 0x20 in the air, 0x100 landed */
    uint8_t f78;                /* +78: FF until the body lands */
    uint8_t count80;            /* 0C2F06: lying count-down */
    uint32_t descriptor;        /* last pose of the program (drawing) */
    neo_anim_draw_t draw;       /* that pose relative to the actor's words, facing right */
    bool drawn;                 /* a pose was emitted since the start */
    bool shown;                 /* the program drew in the last update (lying: every other one) */
    bool burnt;                 /* 0BF358 (kinds 7/0x10): drawn with its burnt palette */
    unsigned updates;           /* updates in the phase */
} marco_death_t;

/* 0C2DDA: a hit of kind `kind` (+5A) on the player; attacker_right is +5B
 * bit 7 (the player's projected x left of the attacker's +44). Sets the
 * velocities (+30/+34, Q16 y up) and the facing (-1 left, 1 right): kind 2
 * 0C2BCE, kinds 7/0x10 0C2C7A/0C2C40, the others 0C2DDA. Kind 8 (0C2ACC)
 * and 0x12 (0C2A70) have their own deaths: not translated, they return
 * false (explicit stop) and take 0C2DDA. grounded: +15 bit 6; the caller
 * clears it unless kind 2 (0C2E7E). */
bool marco_death_start(marco_death_t *d,uint8_t kind,bool grounded,bool attacker_right,int32_t *vx,int32_t *vy,int *facing);
/* Before the motion of an update: the velocities of 0C2E94..0C2EAA (air)
 * or 0C2F20..0C2F28 (lying). ground: the body stands (+15 bit 6). */
void marco_death_velocity(marco_death_t *d,bool ground,int32_t *vx,int32_t *vy);
/* After the motion: landing 0C2EE4, speed 0C2EC6, program 00427A, the
 * lying count-down 0C2F12 and the off-screen end 0C2F72 (screen x/y of the
 * projected +3C/+40). Returns the phase for the next update. */
/* The death/entrance pose at the actor's projected words (x, y) with the
 * facing (-1 left): mirrored offsets and attribute bit 0. False before the
 * program's first pose. */
bool marco_death_draw(const marco_death_t *d,uint16_t x,uint16_t y,int facing,neo_anim_draw_t *out);
marco_life_phase_t marco_death_after(marco_death_t *d,bool ground,int32_t *vx,int16_t screen_x,int16_t projected_y);
/* The respawn record of 0196E8 for the camera progress (10E99E): world x/y
 * (Q16, y up) from the scene root (root_x/root_y, Q16) after the 0143E4 step
 * of +7; type +8 (0 in Mission 1: 0C1438). */
typedef struct { int32_t x,y; uint8_t type; } marco_respawn_t;
marco_respawn_t marco_respawn_point(uint16_t progress,int32_t root_x,int32_t root_y);
/* 0C1438..0C14DC: the entrance program after the floor snap; true while it
 * runs (no control, +84 = 0x78). */
void marco_entrance_start(marco_death_t *d);
bool marco_entrance_step(marco_death_t *d);
#endif
