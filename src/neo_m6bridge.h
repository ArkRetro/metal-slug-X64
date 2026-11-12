/* Mission 6 stage 1: the driver of the hanging bridge 10D7B6 (docs/MISION6.md,
 * "Fase 2"). Manual translation of 0BCDC2 (camera-list call: the bridge's
 * actors), 0BCE18 / 0BCE22 (mode), 0BCE2C (its own step 10E94E, as the
 * train's 0B8D64), 0BCEB0 (the column loop: the origin +32 moves when the
 * column of the table 0BCE96 comes in at the right) and 0BCF36 / 0BCF46 /
 * 0BCF86 / 0BCF8C / 0BCF94 (the loop's entry). Labels are provenance only. */
#ifndef NEO_M6BRIDGE_H
#define NEO_M6BRIDGE_H
#include <stdbool.h>
#include <stdint.h>
typedef struct {
    bool on;            /* 0BCDC2 created 0BCE2C */
    uint8_t mode;       /* 10C70A: 0 keep 10C706, 1 (-0x4000 per update down to -2 px), 2 (root x 3/4, -0x2000 down to -8 px) */
    int32_t speed;      /* 10C706 (Q16.16) */
    int16_t index;      /* 10C702: byte offset in 0BCE96 (-8: none) */
    uint16_t count;     /* 10C704 (0xFFFF: no end) */
    uint16_t lock;      /* 10C70C/10C70D: and-ed with the entry's mask */
    bool fault;         /* trap #F of 0BCF46 (past the table's end) */
} neo_m6bridge_t;
/* A camera-list call; false: not one of the bridge's. */
bool neo_m6bridge_call(neo_m6bridge_t *b,uint32_t routine);
/* 0BCE2C: 10E94E for the root's step root_vx (RAM 10E970). */
int32_t neo_m6bridge_drive(neo_m6bridge_t *b,int32_t root_vx);
/* 0BCEB0 on 10D7B6 (+32 origin, +34 row, +36 local column): true when the
 * origin moved (then +36 is to be recomputed, 0C748E). */
bool neo_m6bridge_loop(neo_m6bridge_t *b,int16_t *o32,int16_t *o34,int16_t o36);
#endif
