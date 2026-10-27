#ifndef MSLUGX_FIXED_H
#define MSLUGX_FIXED_H
#include <stdint.h>
/* Neo Geo S-ROM: 4096 tiles, 8x8 pixels, 4 bits per pixel.
 * Returns a palette index; the original palette is runtime state, not in S-ROM.
 * Caller supplies valid tile (0..4095) and coordinates (0..7).
 */
static inline unsigned fixed_pixel(const uint8_t *rom, unsigned tile,
                                   unsigned x, unsigned y) {
    static const unsigned columns[4] = {16, 24, 0, 8};
    unsigned byte = rom[tile * 32 + columns[x / 2] + y];
    return (byte >> ((x & 1) * 4)) & 15;
}
#endif
