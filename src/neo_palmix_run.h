#ifndef MSX_NEO_PALMIX_RUN_H
#define MSX_NEO_PALMIX_RUN_H
#include <stdint.h>
/* 009CCE at run time: the colours of a palette slot whose resource +A is
 * blended towards +C with the weight +E, as the vertical blank builds them
 * (008402 -> 005EA0; tools/palette_mix.py Mixer.mix is the same routine in
 * Python, checked against MAME's palette RAM). The exported mixes of
 * neo_palmix.h cover the few pairs of the missions; the credits blend 288
 * page palettes and the pictures' towards palette 1 by 31 weights, so the
 * console mixes them itself from the palettes' ROM words (the 16 words at
 * 200000 + 32 n of bank 1; the pack carries them as the resource 8000 | n).
 *
 * 005EA0, per colour word (pen 0 is not written): for the channels at bits
 * 0..4 and 5..9, base = +A's channel, difference = +C's - +A's as a 16-bit
 * word; the low byte of the difference is replaced by the signed byte at
 * ROM 0051D8 + 64 * weight + difference and the base is added. The third
 * channel does the same with the words shifted right by 10 (bit 15 stays in
 * it). The three results are packed (shift left by 5 and OR of the low
 * byte, twice) and the word goes through the table 224000 (index = word * 2
 * as a signed word).
 *
 * The table 224000 holds, for every 15-bit word, the Neo Geo colour whose
 * 5-bit channels (4 bits and the shared low bit) are the word's own three
 * channels, red from bits 10..14, green from 5..9, blue from 0..4, and never
 * the dark bit: tools/test_palmix_run.py checks all 32768 entries against
 * the ROM. The console's colour of a 5-bit Neo Geo channel is neo_palmix_dac
 * (the resistor ladder of tools/export_marco.py palette_rgb, as every export
 * of the packs; the same test checks the 32 values).
 *
 * table: ROM 0051D8 (ga_rom(0x51d8): the bytes 0051D8 + 64 * weight +
 * difference of the weights 1..30 lie in 0051D9..005997). raw_a / raw_c: the
 * ROM words of +A and +C. weight 1..30 (0 shows +A and 31 or more +C: plain
 * palettes, the caller's). out: RGBA 5551 words, pen 0 transparent. */
static const uint8_t neo_palmix_dac[32]={0,1,1,2,3,4,5,6,8,9,9,10,11,12,13,14,17,18,19,20,21,22,22,23,25,26,27,28,29,30,30,31};
static inline uint16_t neo_palmix_channel(uint16_t x,uint16_t y,unsigned weight,const uint8_t *table) {
    uint16_t d=(uint16_t)(y-x);
    return (uint16_t)(((d&0xff00u)|table[64*(int)weight+(int16_t)d])+x);
}
static inline void neo_palmix_mix(const uint16_t raw_a[16],const uint16_t raw_c[16],unsigned weight,const uint8_t *table,uint16_t out[16]) {
    out[0]=0;
    for (unsigned k=1;k<16;k++) {
        uint16_t d0=raw_a[k],d1=raw_c[k];
        uint16_t lo=neo_palmix_channel(d0&0x1fu,d1&0x1fu,weight,table);
        d0>>=5;d1>>=5;
        uint16_t mid=neo_palmix_channel(d0&0x1fu,d1&0x1fu,weight,table);
        d0>>=5;d1>>=5;
        uint16_t hi=neo_palmix_channel(d0,d1,weight,table);
        uint16_t r=(uint16_t)(hi<<5);r=(uint16_t)((r&0xff00u)|((r|mid)&0xffu));
        r=(uint16_t)(r<<5);r=(uint16_t)((r&0xff00u)|((r|lo)&0xffu));
        out[k]=(uint16_t)(neo_palmix_dac[(r>>10)&31u]<<11|neo_palmix_dac[(r>>5)&31u]<<6|neo_palmix_dac[r&31u]<<1|1u);
    }
}
#endif
