#ifndef MSX_MARCO_MOUNT_H
#define MSX_MARCO_MOUNT_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_vslot.h"
/* The mounted player (docs/ARMAS_VEHICULOS.md §12, paso 9): manual
 * translation of ROM 0C3078..0C3260 and the slot hand-off of the player's
 * common processing 0C3348..0C3376, on the player's node record (original
 * layout +00..+BF, big-endian, as neo_gactor_t.m). ROM labels are
 * provenance only.
 *
 *   0C3352  0AA458 carry: +9A bit 7 set; newly set -> +00 = 0C3078
 *           no carry: +9A bit 7 cleared; it was set -> 0C30F0 (exit)
 *   0C3078  +7A/+7B = FF, +15 bit 0 clear, +50 = 0, 0C0F16 (+98 = 0, +90/+91
 *           = FF, +7C = 0C381C), +00 = 0C309C and on:
 *   0C309C  0AA550 (the player follows the slot), 019424/018F24: neither ->
 *           +9A bit 7 and the common processing; else 0C30C0 (0AAA88:
 *           kind 4 -> 0C315A, in the air -> 0C3232, else +9A bit 7)
 *   0C30F0  0AA5AC (its 0143EE is the caller's), kind 0 -> death (+5A =
 *           cause, +5F bit 7, 0C2956) unless RAM 10C300 (then 0C3232);
 *           1 and the unknown kinds -> 0C3232; 3 and 5 (+15 bit 0 set)
 *           -> 0C1F4A (stand; the fat form 2 with +86 >= 20 -> 0C1E94
 *           untranslated); 2 and 4 untranslated
 *   0C3232  program 0BF618 (0C0F32), 0C0F16, +30 = 0, +34 = 0xB7000,
 *           +00 = 0C3250 and on:
 *   0C3250  0AA4C8, +84 |= 0x1E, 0C0FEA, +34 += (-0x160000 - +34) >> 5,
 *           unless RAM 10C300: 0C0FEA again and the stick steers +30 to
 *           -+0x40000 (>> 2) or 0 (>> 5); 0C109C (turn with A/C new and
 *           +9A bit 3, then 0143EE keeping a rising +34 unless +5F bit 1),
 *           0427A, on the ground -> 0C1EBE.
 * The animation programs (0C0F32 with the table 0BF618, 0427A), the
 * sweeps 0143EE and the rest of the common processing are the caller's
 * (the port's player). */
enum { MARCO_MOUNT_C3078=0xc3078u, MARCO_MOUNT_C309C=0xc309cu, MARCO_MOUNT_C3250=0xc3250u, MARCO_MOUNT_C1EBE=0xc1ebeu };
typedef enum {
    MARCO_MOUNT_COMMON,     /* go on with the common processing */
    MARCO_MOUNT_ENTER,      /* +00 = 0C3078: mounted from the next update */
    MARCO_MOUNT_EXIT,       /* 0C30F0 now: call marco_mount_exit */
    MARCO_MOUNT_DIE,        /* 0C2956 with +5A (kind 0) */
    MARCO_MOUNT_EJECT,      /* 0C3232: the eject jump (marco_mount_eject each update) */
    MARCO_MOUNT_STAND,      /* 0C1F4A (kinds 3 and 5, the sewer crank): standing, +00 = 0C1FA6 (its program 0BF6D8 by 0C0F32 is the caller's) */
    MARCO_MOUNT_STOP        /* untranslated path (`stop` = its ROM label) */
} marco_mount_result_t;
typedef struct {
    unsigned p;                 /* player (+70) */
    uint16_t video;             /* RAM 10000E */
    bool halted,system;         /* RAM 10C301 (018F24), 10C300 (019424) */
    uint16_t fresh,held;        /* 0CA2CE: RAM 10E9EC+6p +0 / +2 */
    uint8_t shots,shot_limit,shot_count;   /* RAM 10C718+p, 10C71A+p, 10C71C+p (0C0FEA) */
} marco_mount_in_t;
/* 0C3348..0C3376: the slot hand-off of the common processing (with +5F
 * bit 7 set the original skips it: COMMON). */
marco_mount_result_t marco_mount_handoff(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in);
/* 0C3078 (when +00 is 0C3078) and 0C309C. `followed` (may be NULL) tells
 * the caller 0AA550 wrote the position (it then projects, 01ABC8). */
marco_mount_result_t marco_mount_ride(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in,bool *followed,uint32_t *stop);
/* 0C30F0: *cause = +1E of the slot. The player is at the vehicle's
 * position with the seat as velocity (+30/+34) for the caller's 0143EE,
 * after which the caller clears +30/+34 (0AA5F2). Kind 1 goes on with
 * marco_mount_eject_start. */
marco_mount_result_t marco_mount_exit(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in,uint8_t *cause,uint32_t *stop);
/* 0C3232 (without its program start 0C0F32/0BF618). */
void marco_mount_eject_start(uint8_t *m);
/* 0C3250 up to the 0143EE of 0C109C: returns the +34 0C109C saved; the
 * caller runs 0143EE and then marco_mount_eject_end. */
uint32_t marco_mount_eject(neo_vslots_t *v,uint8_t *m,const marco_mount_in_t *in);
/* 0C10C0..0C10D6 and 0C31FA..0C3210 (after 0427A): +34 restored while it
 * was rising (0 with +5F bit 1); on the ground +00 = 0C1EBE (true). */
bool marco_mount_eject_end(uint8_t *m,uint32_t saved34);
/* 0C0FEA: the player's input word (new presses, stick held) with C
 * removed at two projectiles (10C718+p >= 2) and A removed when +9A bit 0
 * is clear and 10C71C+p <= 10C71A+p; +5F bits 5/4 from up/down, +9A bit 3
 * for a stick against the facing, +9B |= d0 & +9C. */
uint16_t marco_mount_input(uint8_t *m,const marco_mount_in_t *in);
#endif
