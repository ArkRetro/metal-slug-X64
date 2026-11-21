#ifndef END_ZOOM_H
#define END_ZOOM_H
#include <stdbool.h>
#include <stdint.h>
/* The ending's shrunk sprites drawn as the LSPC draws them (end_zoom_core.h), from romfs/endzoom.bin. */
typedef struct {
    unsigned entries,strips,hits,misses,no_slot,file_reads,missing_descriptors,missing_tiles,overflow,auto_animated;
    unsigned cache_bytes;
    uint32_t last_frame_ticks,worst_frame_ticks;
} end_zoom_stats_t;
/* One draw_end entry with a zoom (+12 / +13 != FF): false when its descriptor is not exported (the caller scales). */
bool end_zoom_draw(uint32_t descriptor,uint16_t x,uint16_t y,uint8_t zoom_x,uint8_t zoom_y,uint16_t attributes,unsigned palette);
void end_zoom_frame(void);   /* once per drawn frame: the frame's time into the worst */
const end_zoom_stats_t *end_zoom_stats(void);
#endif
