#ifndef END_ZOOM_H
#define END_ZOOM_H
#include <stdbool.h>
#include <stdint.h>
/* The ending's shrunk sprites drawn exactly as the LSPC draws them, with RDP rectangles (end_zoom_rdp.h), from
 * romfs/endzoom.bin. */
typedef struct end_zoom_stats {
    unsigned entries,rects,loads,refits,unfit,file_reads,missing_descriptors,missing_tiles,overflow,auto_animated,no_block,releases;
    unsigned cache_bytes;
    uint32_t last_frame_ticks,worst_frame_ticks,total_ticks,emit_ticks;   /* emit: inside scene_render_zoom_page / _rect */
    uint32_t window_ticks,window_rects,window_loads;   /* the most in one frame since end_zoom_window_reset */
} end_zoom_stats_t;
/* The entries: msx_overlay_ops(MSX_OVL_ENDING) (src/msx_overlay.h), only while the ending's image is loaded. */
#endif
