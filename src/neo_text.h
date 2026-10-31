#ifndef MSX_NEO_TEXT_H
#define MSX_NEO_TEXT_H
/* The original's fix-layer text engine with its full glyph writer 03DD96
 * (docs/FIN_PARTIDA.md): the string printer 03D25A with the escapes of
 * 03D1B8, the BCD printer 03CFBA and every glyph page of 03DD96 (pages
 * below 0x10 and 0x30/default through 03CF9A, the two-row font 'a', the
 * three-row 0x31, the 2x2 blocks 'A' and 'X' (with its two-byte codes
 * 03D512), the tables 'H'/'K' (03D596/03D796), 'h'/'k' (03DB96/03D996)
 * with their '@' accents, page 0x32 with its accent pairs 03E194.. and the
 * DE/DF prefixes). It writes the fix map of neo_hud_t (VRAM 7000..74FF) as
 * the original writes VRAM; the font tables are ROM data (ga_rom). */
#include <stdbool.h>
#include <stdint.h>
#include "neo_hud.h"
typedef struct {
    neo_hud_t *hud;
    /* Test hook: every VRAM word written (address, data), in order. */
    void (*on_write)(void *ctx,uint16_t address,uint16_t word);void *ctx;
    unsigned unsupported;          /* TRAP #F (address outside 7000..74FF) or a missing table */
} neo_text_t;
/* One VRAM word (move.l d0,$3C0000). */
void neo_text_word(neo_text_t *t,uint16_t address,uint16_t word);
/* 03DD96: glyph d2 at d0 with d1 (palette<<8 | page); *a0 is the string
 * after the glyph (DE/DF prefixes and '@' accents read it). Returns d0. */
uint16_t neo_text_glyph(neo_text_t *t,uint16_t d0,uint16_t d1,uint8_t d2,const uint8_t **a0);
/* 03D25A with A1 = 03DD96: the string a0 at d0; d1 in/out (the escapes \c
 * and \p change the caller's D1, as in the original). Returns d0. */
uint16_t neo_text_string(neo_text_t *t,uint16_t d0,uint16_t *d1,const uint8_t *a0);
/* 03CFBA with A1 = 03DD96: d4 BCD digits from a0 (odd d4: low nibble
 * first), leading zeros blank while d3 is clear. Returns d0. */
uint16_t neo_text_bcd(neo_text_t *t,uint16_t d0,uint16_t d1,const uint8_t *a0,uint8_t d3,unsigned d4);
#endif
