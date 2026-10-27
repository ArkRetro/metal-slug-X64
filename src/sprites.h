#ifndef MSX_SPRITES_H
#define MSX_SPRITES_H
#include <stdint.h>
/* Neo Geo C-ROM interleaved tile: 16x16, four bitplanes, 128 bytes.
 * Layout checked against MAME neogeo_spr.cpp optimize_helper. */
static inline unsigned sprite_pixel(const uint8_t *tile, unsigned x, unsigned y) {
    const uint8_t *row = tile + (x < 8 ? 64 : 0) + y * 4;
    unsigned bit = x & 7;
    return ((row[0] >> bit) & 1) | (((row[2] >> bit) & 1) << 1)
         | (((row[1] >> bit) & 1) << 2) | (((row[3] >> bit) & 1) << 3);
}
#define SPRITE_TILE_COUNT 393216u
#define SPRITES_PER_PAGE 64u
#define SPRITE_PAGE_COUNT (SPRITE_TILE_COUNT / SPRITES_PER_PAGE)
#endif
