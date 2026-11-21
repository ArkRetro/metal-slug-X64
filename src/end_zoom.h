#ifndef END_ZOOM_H
#define END_ZOOM_H
#include <stdbool.h>
#include <stdint.h>
/* The ending's shrunk sprites drawn exactly as the LSPC draws them, with RDP rectangles (end_zoom_rdp.h), from
 * romfs/endzoom.bin. */
typedef struct {
    unsigned entries,rects,loads,refits,unfit,file_reads,missing_descriptors,missing_tiles,overflow,auto_animated,no_block;
    unsigned cache_bytes;
    uint32_t last_frame_ticks,worst_frame_ticks,total_ticks,emit_ticks;   /* emit: inside scene_render_zoom_page / _rect */
    uint32_t window_ticks,window_rects,window_loads;   /* the most in one frame since end_zoom_window_reset */
} end_zoom_stats_t;
/* One draw_end entry with a zoom (+12 / +13 != FF): false when its descriptor is not exported (the caller scales). */
bool end_zoom_draw(uint32_t descriptor,uint16_t x,uint16_t y,uint8_t zoom_x,uint8_t zoom_y,uint16_t attributes,unsigned palette);
void end_zoom_frame(void);   /* once per drawn frame */
void end_zoom_window_reset(void);
const end_zoom_stats_t *end_zoom_stats(void);
#endif
