#ifndef MSX_NEO_FADE_H
#define MSX_NEO_FADE_H
#include <stdint.h>
#include <stdbool.h>
/* Palette fade of the original (RAM 10991A..10992C): three channel offsets
 * in 8.8 (only the high byte, -31..31, is added to each 5-bit channel with
 * clamping by the palette conversion 005A0A), their targets, per-update
 * steps and the step count. */
typedef struct {
    int16_t current[3];   /* 10991A/10991C/10991E */
    int16_t target[3];    /* 109920/109922/109924 */
    int16_t step[3];      /* 109926/109928/10992A */
    uint8_t count;        /* 10992C */
} neo_fade_t;
/* ROM 009C0A: fade to (d0,d1,d2) (each clamped to -31..31) in d3 updates
 * (low byte; 0 or 1 = at once on the next step). */
void neo_fade_start(neo_fade_t *f,int16_t d0,int16_t d1,int16_t d2,uint16_t d3);
/* ROM 00698C (list head actor 003918, once per update): one step. Returns
 * true when an offset's high byte changed (the original marks every
 * palette without flag bit 1 for conversion). */
bool neo_fade_step(neo_fade_t *f);
/* The offset the palette conversion adds to channel i. */
static inline int8_t neo_fade_offset(const neo_fade_t *f,unsigned i) {return (int8_t)(uint8_t)((uint16_t)f->current[i]>>8);}
/* 005A0A: one 5-bit channel plus the offset, clamped to 0..31. */
static inline unsigned neo_fade_channel(unsigned c,int8_t offset) {
    int v=(int)c+offset;return v<0?0u:v>31?31u:(unsigned)v;
}
#endif
