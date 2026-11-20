#ifndef NEO_SCB_H
#define NEO_SCB_H
#include <stdbool.h>
#include <stdint.h>
/* The sprite-list column writers of the 68K (00A086 + 4 * ((zoom != FFFF) << 2 | attribute & 3), +0x20 in the
 * vblank pass 011E60, +0x40 backward; docs/MISION6.md, tools/check_zoom_scb.py), translated: one list entry
 * (00A03E: x, y, zoom x << 8 | zoom y, palette << 8 | attribute & 7, descriptor) into SCB1..SCB4 words.
 * win: the window 3954 / 395C (+0 start, +2 end, +4 forward cursor, +6 backward cursor), updated as the ROM does. */
typedef struct { uint16_t x,y,zoom,attr;uint32_t descriptor; } neo_scb_entry_t;
/* Descriptor words of P2 (bank = bits 20..23 - 1, offset = bits 0..19), big-endian; NULL past the data. */
typedef uint16_t (*neo_scb_read_t)(void *ctx,uint32_t descriptor,uint32_t word_index);
typedef void (*neo_scb_write_t)(void *ctx,uint16_t address,uint16_t data);
/* vblank: 011E60's writers (SCB3 too, the overlap only for SCB1/2/4); else 011D04's. */
void neo_scb_entry(bool vblank,bool forward,const neo_scb_entry_t *e,uint16_t win[4],neo_scb_read_t rd,neo_scb_write_t wr,void *ctx);
/* 0103A6 / 010BC8 after 011E60: SCB3 = 8000 for the rest of the window. */
void neo_scb_hide_rest(bool forward,const uint16_t win[4],neo_scb_write_t wr,void *ctx);
#endif
