#ifndef MSX_NEO_PALMIX_H
#define MSX_NEO_PALMIX_H
#include <stdint.h>
/* Palette slot target / weight of 009CCE (vblank 008402): +A = +C or weight
 * 0 show +A (005978), weight 31 or more +C, otherwise the mix 005EA0. The
 * mixes are palettes of the mission export with the logical id
 * NEO_PALMIX_BASE | pair << 5 | weight, computed from the ROM by
 * tools/palette_mix.py --mix A:C (in this order) and checked against MAME's
 * palette RAM. Pairs: the Mission 6 mid-boss 05310E's death fade 05390C
 * (its palette 38E toward 3C2). */
enum { NEO_PALMIX_BASE=0x4000 };
static const uint16_t neo_palmix_pairs[][2]={{0x38e,0x3c2}};
/* The logical resource the slot shows; -1: a mix not exported. */
static inline int neo_palmix_resource(uint16_t a,uint16_t c,uint8_t weight) {
    if (a==c || !weight) return a;
    if (weight>=0x1f) return c;
    for (unsigned p=0;p<sizeof(neo_palmix_pairs)/sizeof(neo_palmix_pairs[0]);p++)
        if (neo_palmix_pairs[p][0]==a && neo_palmix_pairs[p][1]==c) return NEO_PALMIX_BASE|p<<5|weight;
    return -1;
}
#endif
