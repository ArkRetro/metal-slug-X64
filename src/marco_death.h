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
    bool burnt;                 /* 0BF358 (kinds 7/0x10, form 3): drawn with its burnt palette */
    bool fat;                   /* form 2 (0C2A36..0C2A6C): no landing, no lying, gone at the program end */
    bool final_draw;            /* the update that ends the actor drew its pose (form 2 at its end, off the screen, water): it still shows */
    uint32_t palette_table;     /* ROM table of the palette 009EA8 puts in +19 (0BEDE8 burnt,
                                 * 0BEE08 form 3); 0: the palettes stay */
    unsigned updates;           /* updates in the phase */
    uint8_t own;                /* a death of its own: 8 (0C2ACC..0C2BCA), 0x12 (0C2A70..0C2AC8); 0: 0C2DDA's motion */
    bool keep_ground;           /* the start leaves +15 bit 6 (kinds 2, 8, 0x12 on the ground: no 0C2E7E) */
    bool child_5a0b4;           /* kind 8: the program's callback 05A09E ran (the caller makes 05A0B4; it clears this) */
    bool c4c28;                 /* the start reached 0C2E44 (0C2E74: 0C4C28, Fio's actor 0C4C64 for +6E = 3) */
    uint32_t palette_slot1a;    /* ROM table of the palette 009EA8 puts in +1A (kind 8: 0BEDE0); 0: none */
    bool water;                 /* the fall into water 0C2CCC..0C2D66 */
    uint16_t water80;           /* its +80: the updates left that look for the surface (0x1E) */
} marco_death_t;

/* The death and entrance programs of the chosen character (msx_character.h):
 * five programs in the order ground 0BF298, kind 2 on the ground 0BF3D8,
 * kinds 7/0x10 0BF358, in the air 0BF318, entrance 0BF498, each the
 * character's column (+6E) of the same table row. NULL: Marco. */
void marco_death_use_programs(const neo_anim_program_t *five);
/* The chosen character's +6E (0 Marco): the deaths of kinds 8 / 0x12 of the
 * others (programs 250DB6.. / 2509AA.., not in their packs) stop explicitly. */
void marco_death_use_character(unsigned column);
/* 0C2DDA: a hit of kind `kind` (+5A) on the player; attacker_right is +5B
 * bit 7 (the player's projected x left of the attacker's +44). Sets the
 * velocities (+30/+34, Q16 y up) and the facing (-1 left, 1 right): kind 2
 * 0C2BCE, kinds 7/0x10 0C2C7A/0C2C40, kind 8 0C2ACC (always the air step
 * 0143EE), kind 0x12 on the ground 0C2A70 (its splash 040F74 is the
 * caller's, for every kind 0x12: 040F38), the others 0C2DDA. Kinds 8 and
 * 0x12 of Tarma, Eri and Fio (programs not in their packs) return false
 * (explicit stop) and take 0C2DDA. grounded: +15 bit 6; the caller clears
 * it unless keep_ground (0C2E7E). */
bool marco_death_start(marco_death_t *d,uint8_t kind,bool grounded,bool attacker_right,int32_t *vx,int32_t *vy,int *facing);
/* 0C2956 by the form +93 (and 0C2D6A, the fall, when `fall`): form 3 (the
 * mummy) 0C2C40 for every kind (0BF358 with the palettes 0BEE08 / 0BEDE8,
 * then 0C2E44 as kind 7); form 2 (fat) 0C2A36: the body program of the
 * character's table 0C0518 (marco_death_use_fat_program; none: explicit
 * stop, false) with the velocities kept (the fall 0C2A06 first sets +34 = 0xA9000
 * and +30 = -/+2 by +5B, and the facing); the others marco_death_start.
 * The caller clears the ground flag unless kind 2 or form 2 (0C2E7E). */
bool marco_death_start_form(marco_death_t *d,uint8_t kind,uint8_t form93,bool fall,bool grounded,bool attacker_right,
    int32_t *vx,int32_t *vy,int *facing);
/* 0C2D6A's water branch (0C2DA6..0C2DC2): with water (class 5) under the
 * feet (03C55C, the caller's) the fall takes 0C2CCC when 0191FC gives 2 or
 * 3: RAM 10C2F6 (the stage word 42F6) 9 or 0xB (2), 0 (3). */
bool marco_death_water_stage(uint16_t stage);
/* 0C2CCC: 009F88, +19 = 0BEDF0[+6E] (009EA8), the program 0BF2D8[+6E] by
 * 0C0F32 (no row of its own for the forms: +93's rows are 0, the fallback
 * row 0), 0C0F16, +34 = 0, +80 = 0x1E, then 0C2D06 in the same update
 * (marco_death_water_velocity, the step 0143E4, marco_death_after). The
 * other characters' programs (24F150, 24F5D4, 24FA58) are not in their
 * packs: explicit stop (false), with Marco's. */
bool marco_death_start_water(marco_death_t *d,int32_t *vy);
/* 0C2D06..0C2D4C, before 00427A and the air step 0143E4: +30 -= +30 >> 4;
 * while +80 counts down (from 0x1E) 03C55C looks for the surface: `surface`
 * (may be NULL: none) gives D1 + the object's +18 (the surface - 1, the
 * projected frame of +40); found, +34 = (that - 8 - +40) >> 3; else (or +80
 * at 0) +34 += (-0x10 - +34) >> 5. */
typedef bool (*marco_surface_fn)(void *ctx,int32_t *surface);
void marco_death_water_velocity(marco_death_t *d,int32_t *vx,int32_t *vy,int32_t y40,marco_surface_fn surface,void *ctx);
/* Form 2's death body: the program of table 0C0518 in the chosen
 * character's column (marco_visual.c, role B2_HIT_C2A06): the script and
 * its entry. NULL: none (explicit stop). */
void marco_death_use_fat_program(const neo_anim_program_t *script,uint16_t entry);
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
/* Fio's actor 0C4C64 (0C4C28 from 0C2E74 when +6E = 3: the deaths that
 * reach 0C2E44, marco_death_t.c4c28). Its constructor: +30 = the creator's
 * +30 + 1 away from 0 (+1 for 0), +34 = the creator's. 0C4CBA..0C4D04
 * before its 0143E4 step: +30 -= >>4, +34 += ((|2 +30| - 6) - +34) >> 4,
 * on the ground (+15 bit 6) +34 = 0 and +26 eases (>>3) to 0x400, else to
 * 0x80. 0C4D14..0C4D38 after 00427A: at its end (carry) +80 counts, from
 * 0xF on +5F bit 3 (blink), at 0x1E the actor ends (true). */
void marco_death_cap_start(int32_t creator_vx,int32_t creator_vy,int32_t *vx,int32_t *vy);
void marco_death_cap_velocity(int32_t *vx,int32_t *vy,uint16_t *speed,bool ground);
bool marco_death_cap_after(bool program_done,uint16_t *count80,uint8_t *f5f);
marco_life_phase_t marco_death_after(marco_death_t *d,bool ground,int32_t *vx,int16_t screen_x,int16_t projected_y);
/* The respawn record of 0196E8 for the camera progress (10E99E): world x/y
 * (Q16, y up) from the scene root (root_x/root_y, Q16) after the 0143E4 step
 * of +7; type +8 (0C1260: 0/4 -> 0C1438, 5 -> 0C143E, 4 after 014742).
 * marco_respawn_point is Mission 1's table (bank 1 2D5C22). */
typedef struct { int32_t x,y; uint8_t type; int8_t step; } marco_respawn_t;
marco_respawn_t marco_respawn_point(uint16_t progress,int32_t root_x,int32_t root_y);
/* The 12-byte entries of a stage's table (019604 +4, ROM bytes to the
 * 0xFFFF threshold): their count; 019658's pointer from entry `from` for
 * the progress (it only moves on); 0196E8's record of an entry. */
unsigned marco_respawn_entries(const uint8_t *table,unsigned length);
unsigned marco_respawn_select(const uint8_t *table,unsigned entries,unsigned from,uint16_t progress);
marco_respawn_t marco_respawn_entry(const uint8_t *table,unsigned index,int32_t root_x,int32_t root_y);
/* 014742 (record type 4): the floor search across the screen. `floor` is
 * 013DB2 with A0 = 0 at the absolute (projected) point, D3 = limit: true
 * when found (carry), with D6. See marco_death.c. */
typedef bool (*marco_floor_fn)(void *ctx,int32_t px,int32_t py,int32_t limit,int32_t *d6);
bool marco_respawn_search(int32_t *x,int32_t root_x,int32_t py,int32_t limit,marco_floor_fn floor,void *ctx);
/* 0C1438..0C14DC: the entrance program after the floor snap; true while it
 * runs (no control, +84 = 0x78). */
void marco_entrance_start(marco_death_t *d);
bool marco_entrance_step(marco_death_t *d);
#endif
