#ifndef MSX_OVERLAY_H
#define MSX_OVERLAY_H
/* Code overlays (console only; docs/RENDIMIENTO.md, "Superposiciones de código"):
 * the tick code of the four mission bosses shares one region of the program
 * (tools/link_overlays.py links each one at its address; the cartridge carries
 * them as the rompak file `.ovl`). The routine tests stay in the main program.
 * The host builds link the modules as usual (MSX_OVERLAY 0). */
#include "neo_gactor.h"
#if defined(N64)
#define MSX_OVERLAY 1
#else
#define MSX_OVERLAY 0
#endif
/* The order of tools/link_overlays.py (OVERLAYS). */
typedef enum { MSX_OVL_M3BOSS, MSX_OVL_M4BOSS, MSX_OVL_M5BOSS, MSX_OVL_M6BOSS, MSX_OVL_ENDING, MSX_OVL_COUNT } msx_overlay_id_t;
/* The region: exactly the largest image, neo_m6boss's (62 240 bytes, -Os, with the death 04180A, escape 04677A and air fight 037404 of work-chars 0171ec9, 20-XI; 43 824 before).
 * tools/link_overlays.py fails the build when an image does not fit and prints each image's size: a boss
 * that grows raises this to its new size. */
#define MSX_OVERLAY_BYTES 62432u   /* 62432 since the slot 0x38 of 0421E4 reaches the world (its targets 10C..10E are no longer dead code, 20-XI) */
/* The draw entries an image carries (tools/link_overlays.py EXTRA): the ending's shrunk sprites (src/end_zoom.c).
 * Valid only while that image is in the region: msx_overlay_ops returns NULL otherwise. */
typedef struct {
    bool (*draw)(uint32_t descriptor,uint16_t x,uint16_t y,uint8_t zoom_x,uint8_t zoom_y,uint16_t attributes,unsigned palette);
    void (*frame)(void);
    void (*window_reset)(void);
    const struct end_zoom_stats *(*stats)(void);
    void (*release)(void);   /* before another image replaces this one */
} msx_overlay_ops_t;
#if MSX_OVERLAY
const msx_overlay_ops_t *msx_overlay_ops(msx_overlay_id_t id);
/* The overlay's tick, the overlay copied first if another one (or none) is in
 * the region. A copy while overlay code is running (a boss's tick reaching
 * another overlay through neo_region_tick) is an assertion. */
ga_result_t msx_overlay_tick(msx_overlay_id_t id,neo_gactor_t *a,neo_genv_t *e);
/* Mission `mission` (1..6) starts: its boss copied now, behind the load's black frames. */
void msx_overlay_mission(unsigned mission);
#endif
#endif
