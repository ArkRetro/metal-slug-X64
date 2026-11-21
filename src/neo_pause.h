#ifndef MSX_NEO_PAUSE_H
#define MSX_NEO_PAUSE_H
/* The original's own pause (docs/PAUSA.md): manual translation of ROM
 * 015520..01574E (the pause of the home system: RAM 10FD82 = 0, the path
 * the port runs) and of the main loop's turn 003A88..003B20 around it
 * (labels are provenance only). On the Neo Geo the SELECT button of the
 * pad pauses and resumes; the N64 pad's START is that button while the
 * game is played (actor.c).
 *
 * Not translated (states the port never has): the developer mode 10C2BE
 * (BIOS 10FE80: frame advance), the menu of the two-player modes 10C2E7 != 0
 * (10C2BC, 0155B0/015750) and the forced request 10C2C0 (0154F8, called by
 * 018AB0 of those modes only). RAM 10F000 bit 1 (01553E: pause allowed
 * while 10C2BD is set) is never set by the ROM: clear.
 * Host-compilable; checked against the MAME 0.264 capture
 * data/pause_fixture.json (make check-pause). */
#include <stdbool.h>
#include <stdint.h>
#include "neo_fade.h"
#include "neo_hud.h"
#include "neo_lspc.h"
typedef struct {
    uint8_t owner;      /* 10C2BF: bit 0 player 1 paused, bit 1 player 2 */
    uint8_t blink;      /* 10C2C2 */
    uint8_t held[2];    /* 10C2CA / 10C2CB: 10C35C / 10C362 at the pause */
    uint8_t last;       /* 10008D: 015520's result on the previous turn (0038FC: 1) */
    uint8_t resumed;    /* 10008E: FF on the turn that resumes (0C8AE4) */
    uint8_t faded;      /* main actor +70: the palette fade was saved */
    uint16_t lspc;      /* main actor +72: 002836 at the pause */
    neo_fade_t fade;    /* 10992E..: 009BBC's copy of 10991A.. */
} neo_pause_t;
typedef struct {
    uint8_t select;     /* 10C367: new presses of 380000 (bit 1 SELECT of player 1, bit 3 of player 2) */
    uint8_t off;        /* 10C2BD: 015418 sets FF, 015422 clears */
    uint8_t mode[2];    /* 10FDB6 / 10FDB7: BIOS player modes */
    uint8_t held[2];    /* 10C35C / 10C362: the buttons held on this turn */
} neo_pause_input_t;
/* 0153F0 (game start 00390C) and 0038FC. */
void neo_pause_init(neo_pause_t *p);
/* 015520: D0 (0 paused, 1 the game runs). pressed[2] = 10C35D / 10C363 (the
 * new presses of this turn, in and out: 0156FA). */
uint8_t neo_pause_step(neo_pause_t *p,const neo_pause_input_t *in,uint8_t pressed[2],neo_hud_t *hud);
/* One turn of the main loop 003A88..003B20: true when the game update of
 * this turn runs; false: paused (the turn then waits two vblanks). */
bool neo_pause_turn(neo_pause_t *p,const neo_pause_input_t *in,uint8_t pressed[2],neo_hud_t *hud,neo_fade_t *fade,neo_lspc_t *lspc);
#endif
