#ifndef MSX_FADE_H
#define MSX_FADE_H
#include <stdbool.h>
#include <stdint.h>
/* The sound driver's master level F804 (M1, tools/check_music_fade.py, docs/AUDIO_JUEGO.md). 0A xx (4FAF):
 * F806 = 16 * table[xx - 0x20] (M1 0x0700), FFFF for xx = 0x20; F808 = 1. One timer A tick (4D67..4D97): */
static inline uint16_t msx_fade_tick(uint16_t hl,uint16_t bc,bool out) {
    if (out) return hl==0?0:(hl<bc?0:(uint16_t)(hl-bc));
    if ((hl>>8)==0xffu) return hl;
    uint32_t s=(uint32_t)hl+bc;
    if (s>0xffffu || (s>>8)<0xffu) return 0xffffu;   /* 4D7B carry, 4D7D..4D80 cp FF / jr c */
    return (uint16_t)s;
}
static inline uint16_t msx_fade_step(const uint8_t *table,uint8_t xx) {
    uint8_t l=(uint8_t)(xx-0x20u);
    return l?(uint16_t)(table[l]<<4):0xffffu;
}
/* Timer A: (1024 - 691) * 144 = 47952 cycles of 8 MHz per tick; a Neo Geo frame: 384 * 264 pixels of 6 MHz =
 * 135168 cycles of 8 MHz. */
enum { MSX_FADE_TICK_CYCLES=47952,MSX_FADE_FRAME_CYCLES=135168 };
#endif
