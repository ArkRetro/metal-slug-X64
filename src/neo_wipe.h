#ifndef MSX_NEO_WIPE_H
#define MSX_NEO_WIPE_H
#include <stdbool.h>
#include <stdint.h>
/*
 * Stage transition actors of the mission actor (docs/MISION2.md): 3AED8
 * (entry kind 4, columns E0818) and 3AEF0 (exit kind 4, columns E0856).
 * Each update moves the column cursor +72 by 3 (from -30) and fills the
 * fix-map columns from the cursor on (rows 1..30) with the table's tiles
 * (03AF08); at 0x46 it sets the creator's +60 to 1 and is deleted (004188).
 * The copy-protection probe of 03AF20 (10F00A/10F00C, 2FFFE0) is not
 * translated: it only changes its own counters when the check passes.
 * The fix map is the port's 40x32 overlay (VRAM 0x7000 + 32*column + row).
 */
enum { NEO_WIPE_COLUMNS=40, NEO_WIPE_ROWS=32 };
typedef struct {
    uint32_t routine;          /* +00: 3AED8/3AEF0, 3AEE6/3AEFE; 0 deleted */
    uint16_t cursor;           /* +72 */
    bool signal;               /* the creator's +60 = 1 (03AF1A) */
    uint16_t fix[NEO_WIPE_COLUMNS*NEO_WIPE_ROWS];   /* tiles written (VRAM 7000..) */
} neo_wipe_t;
typedef void (*neo_wipe_write_t)(void *ctx,uint16_t vram,uint16_t tile);
void neo_wipe_create(neo_wipe_t *w,bool exit);
/* One update; `write` (optional) gets every fix-map write in order.
 * Returns false once deleted. */
bool neo_wipe_update(neo_wipe_t *w,neo_wipe_write_t write,void *ctx);
#endif
