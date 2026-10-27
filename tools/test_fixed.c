#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "../src/fixed.h"

int main(void) {
    uint8_t rom[131072] = {0};
    const unsigned offsets[8] = {16, 16, 24, 24, 0, 0, 8, 8};
    /* Each storage nibble must map to precisely one pixel, with no aliases. */
    unsigned checks = 0;
    for (unsigned t = 0; t < 4096; t++) {
        for (unsigned byte = 0; byte < 32; byte++)
            rom[t * 32 + byte] = (uint8_t)(t + byte * 7);
        for (unsigned y = 0; y < 8; y++)
            for (unsigned x = 0; x < 8; x++) {
                unsigned b = rom[t * 32 + offsets[x] + y];
                unsigned expected = x % 2 ? b / 16 : b % 16;
                assert(fixed_pixel(rom, t, x, y) == expected);
                checks++;
            }
    }
    /* Known row: low nibble is the left pixel in each pair. */
    rom[16] = 0x21; rom[24] = 0x43; rom[0] = 0x65; rom[8] = 0x87;
    for (unsigned x = 0; x < 8; x++) assert(fixed_pixel(rom, 0, x, 0) == x + 1);
    printf("PASS: %u pixel checks and known row\n", checks);
}
