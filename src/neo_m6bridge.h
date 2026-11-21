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
/* The bridge 10D7B6 as the second object of the collision kernel's static
 * list 013810 (map 0BC6E6: no tiles, +4 collision 45FC2A, P2 linear: +8 = 60
 * block columns, +A = 5 block rows, 4 bytes a block; +C = 0, so 01384E
 * yields it without the vertical test). Its RAM ring (+88, 64 x 64 cells,
 * read by 0C79FE) as the loaders write it: 0C7038 clears it, 0C741A loads
 * the window at the map's op 0x18, 0C74C4 (0C748E with a map) loads the
 * columns that came in at the left / right (+3A - +36) and, with bit 2 of
 * +82, the rows that came in (+3C - +38), through 0C714E (clipped to the
 * map) and 0C72EC. 0C6F82 (the loop's origin move) runs 0C74C4 again in the
 * same update: +3A / +3C still hold the previous +36 / +38 (0C7480 copies
 * them at the next camera pass), so the columns and rows that came in are
 * loaded again with the new origin. Per ring slot (local block column & 31)
 * and map block row: the map column written there (-1: none since the
 * clear) and its block row in the ring ((~d5 + k) & 31). */
#define NEO_M6RING_ROWS 5
typedef struct neo_m6ring {
    int16_t o3a,o3c;                       /* +3A / +3C: +36 / +38 at the last camera pass */
    int8_t col[32][NEO_M6RING_ROWS];
    uint8_t ring_row[32][NEO_M6RING_ROWS];
} neo_m6ring_t;
/* The map: collision blocks (45FC2A), block columns and rows (0BC6E6 +8/+A). */
typedef struct neo_m6map { const uint8_t *blocks; uint16_t columns,rows; } neo_m6map_t;
/* +36 / +38 of a layer at +14 / +18 (Q16.16; 0C748E). */
int16_t neo_m6ring_o36(int32_t x);
int16_t neo_m6ring_o38(int32_t y);
void neo_m6ring_clear(neo_m6ring_t *r);                                                  /* 0C7038 */
/* Writes: called with each ring byte offset (0..0xFFF) and value 0C72EC stores (the tests). */
typedef void (*neo_m6ring_write_fn)(void *ctx,unsigned offset,uint8_t value);
void neo_m6ring_init(neo_m6ring_t *r,const neo_m6map_t *m,int32_t x,int32_t y,int16_t o32,int16_t o34,neo_m6ring_write_fn w,void *ctx);   /* 0C741A */
void neo_m6ring_pass(neo_m6ring_t *r,const neo_m6map_t *m,int32_t x,int32_t y,int16_t o32,int16_t o34,bool rows,neo_m6ring_write_fn w,void *ctx);   /* 0C74C4 */
void neo_m6ring_mark(neo_m6ring_t *r,int32_t x,int32_t y,bool rows);                     /* 0C7480 */
/* 0C79FE on the ring: local pixel (x, y). */
static inline uint8_t neo_m6ring_cell(const neo_m6ring_t *r,const neo_m6map_t *m,uint16_t x,uint16_t y) {
    unsigned c=(x>>3)&63u,q=(((uint16_t)~y)>>3)&63u,s=c>>1;   /* 0C79FE: ((x & 1F8) << 3) | ((~y & 1F8) >> 3) */
    for (unsigned k=0;k<NEO_M6RING_ROWS && k<m->rows;k++)
        if (r->col[s][k]>=0 && r->ring_row[s][k]==q>>1)
            return m->blocks[((unsigned)r->col[s][k]*m->rows+k)*4u+(c&1u)+2u*(q&1u)];
    return 0;
}
#endif
