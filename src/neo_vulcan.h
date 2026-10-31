#ifndef MSX_NEO_VULCAN_H
#define MSX_NEO_VULCAN_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_vslot.h"
/* Vulcan turret of the player's vehicles (Camel Slug 03458E, SV-001 092A32):
 * manual translation of the control part of ROM 03B57C..03B808 (aim, burst,
 * shot angle, +88 pose index). `m` is the turret's node in the original
 * layout (+00..+BF, as neo_gactor_t.m). Not translated here (the caller's
 * part, explicit in the result): the placement on the vehicle's anchor
 * 03B80C..03B86C (01ABC8, 0044F6 flash, 00427A), the bullet constructors
 * 01B6A0/01BE24, the sweep 014A02 and the spark 03BA76, and the turret's end
 * 03B86E (the vehicle gone). */
typedef enum { NEO_VULCAN_RUN, NEO_VULCAN_HOLD, NEO_VULCAN_GONE } neo_vulcan_state_t;
typedef struct {
    bool creator_alive;          /* 00409A carry */
    uint8_t creator5f;           /* the vehicle's +5F */
    uint16_t anchor;             /* word at the turret's +6E (the vehicle's +78 / +86) */
    int rider;                   /* 0AAB84: player riding the vehicle this update, -1 none */
    uint16_t fresh,held;         /* 0CA2CE: input words +0 / +2 of the rider */
    bool repeat;                 /* 0CA614: RAM 10F084+p nonzero (held A repeats) */
    const uint8_t *aim;          /* 03B4B0[+72]: 16 longs, angle per stick direction (negative: none) */
} neo_vulcan_in_t;
typedef struct {
    neo_vulcan_state_t state;
    bool fire;                   /* 00411C: bullet 01B6A0 (01BE24 when +72 = 5) */
    uint32_t bullet;             /* its routine */
    uint8_t bullet6e,bullet5e;   /* its +6E (angle with the +-2 spread) and +5E */
    uint8_t heading;             /* D1 for 002E7C (speed 0x1C0000): the base angle */
    bool angle_set;uint16_t angle;  /* 0AABDC: slot +1A of the rider */
} neo_vulcan_out_t;
/* One update up to 03B7F6 except the spark, then +88 (03B7FA). `sweep_hit`
 * is the D6 of 014A02 after a shot (the spark 03BA76 needs it and +84 = 0;
 * then +84 = FF and *spark is set). */
void neo_vulcan_step(uint8_t *m,const neo_vulcan_in_t *in,bool sweep_hit,neo_vulcan_out_t *out,bool *spark);
#endif
