#ifndef MSX_MARCO_LEGS_H
#define MSX_MARCO_LEGS_H
/* The player's +72/+74 (offset of the upper body, added to the muzzles of
 * 0C4244/0C453A) as the body programs write them (WRITE_WORD/WRITE_LONG to
 * +72/+74, bank 1): idle 24C5EA (0,17); walk 24DD18 (-1, 17..21 per pose,
 * loop from its fifth pose); jump 24E4E0 (1, 24..18, holding at the top while
 * airborne: BRANCH on +78); running jump 24E858 (-3/-4, 15..11, holding while
 * airborne) then the run loop 24DF6C (-1,15). The program chosen per
 * situation and its restarts are those of marco_visual.c. Crouched no body
 * program writes them (24ECD6/24ECF6, 256BE6/256C06): the values measured
 * in hold2 stand in (H (4,12): bullets (28,16); pistol (0,6)), declared
 * approximation (writer not located). Pure C, timelines only (y up). */
#include <stdbool.h>
#include <stdint.h>
typedef enum { MARCO_LEGS_IDLE,MARCO_LEGS_RUN,MARCO_LEGS_JUMP,MARCO_LEGS_RUN_JUMP,MARCO_LEGS_RUN_LOOP,MARCO_LEGS_CROUCH } marco_legs_program_t;
typedef struct { marco_legs_program_t program; uint8_t index,wait; bool grounded,started; int8_t x72,y74; } marco_legs_t;
void marco_legs_reset(marco_legs_t *l);
/* One player update with the situation of marco_visual_input_t. */
void marco_legs_step(marco_legs_t *l,bool grounded,bool moving,bool crouch,bool running_jump,bool hgun);
/* Muzzle of a fire callback relative to the player's +28/+2C (pixels, y up),
 * with +90 != FF (every H and pistol birth of hold2/hfam/hudp2/hudv2):
 * 0C4244, table 0C4204 by +92 (1 up, 0 level, -1 down) and form +93, plus
 * +72/+74, x negated facing left; 0C453A (sweep step at the stored angle,
 * already mirrored facing left): x = int(12 cos) + +72, y = int(12 sin)
 * (+ half of it for an angle < 0x80) + +74 + 2, from the sine table of
 * 002E7C (sine/cosine are its words, 1.0 = 0x100). */
/* The +72/+74 of entry i of a program's timeline (false past its end). */
bool marco_legs_entry(marco_legs_program_t program,unsigned i,int8_t *x72,int8_t *y74);
typedef struct { int16_t x,y; } marco_muzzle_t;
marco_muzzle_t marco_muzzle_aim(const marco_legs_t *l,int aim92,uint8_t form,bool facing_left);
marco_muzzle_t marco_muzzle_sweep(const marco_legs_t *l,uint8_t angle,int16_t sine,int16_t cosine);
#endif
