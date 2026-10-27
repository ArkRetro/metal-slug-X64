#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/sprites.h"
int main(void) {
    uint8_t tile[128];
    unsigned planes[] = {0, 2, 1, 3};
    unsigned checks = 0;
    /* Turn on each of the 1024 stored bits separately. Verify that exactly
       its intended pixel/plane lights, including both halves and all rows. */
    for (unsigned y = 0; y < 16; y++)
        for (unsigned x = 0; x < 16; x++)
            for (unsigned p = 0; p < 4; p++) {
                memset(tile, 0, sizeof(tile));
                unsigned base = x < 8 ? 64 : 0;
                tile[base + y * 4 + planes[p]] = 1u << (x % 8);
                for (unsigned yy = 0; yy < 16; yy++)
                    for (unsigned xx = 0; xx < 16; xx++) {
                        assert(sprite_pixel(tile, xx, yy) ==
                               ((xx == x && yy == y) ? 1u << p : 0));
                        checks++;
                    }
            }
    printf("PASS: %u sprite bitplane checks\n", checks);
}
