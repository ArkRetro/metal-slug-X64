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
typedef enum { MSX_OVL_M3BOSS, MSX_OVL_M4BOSS, MSX_OVL_M5BOSS, MSX_OVL_M6BOSS, MSX_OVL_COUNT } msx_overlay_id_t;
/* The region: exactly the largest image, neo_m6boss's (43 824 bytes, -Os, release and diag alike, 17-XI).
 * tools/link_overlays.py fails the build when an image does not fit and prints each image's size: a boss
 * that grows raises this to its new size. */
#define MSX_OVERLAY_BYTES 43824u
#if MSX_OVERLAY
/* The overlay's tick, the overlay copied first if another one (or none) is in
 * the region. A copy while overlay code is running (a boss's tick reaching
 * another overlay through neo_region_tick) is an assertion. */
ga_result_t msx_overlay_tick(msx_overlay_id_t id,neo_gactor_t *a,neo_genv_t *e);
/* Mission `mission` (1..6) starts: its boss copied now, behind the load's black frames. */
void msx_overlay_mission(unsigned mission);
#endif
#endif
