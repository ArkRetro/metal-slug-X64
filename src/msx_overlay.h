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
typedef enum { MSX_OVL_M3BOSS, MSX_OVL_M4BOSS, MSX_OVL_M5BOSS, MSX_OVL_M6BOSS, MSX_OVL_ENDING,
    /* Region 2 (msx_overlay_code_region): each mission's own code but its boss, one image per mission; an image also
     * carries the modules of other missions that its mission's lists make (tools/link_overlays.py, OVERLAYS). */
    MSX_OVL_M2CODE, MSX_OVL_M3CODE, MSX_OVL_M4CODE, MSX_OVL_M5CODE, MSX_OVL_M6CODE, MSX_OVL_COUNT } msx_overlay_id_t;
/* The modules of region 2, in the order of tools/link_overlays.py (MODULES): an image's tick table is indexed by them. */
typedef enum { MSX_MOD_M2PROPS, MSX_MOD_M2TRAPS, MSX_MOD_M2CREATURE, MSX_MOD_M3MID, MSX_MOD_M3VEH,
    MSX_MOD_M3PROPS, MSX_MOD_M4PROPS, MSX_MOD_M4VEH, MSX_MOD_M5ENEMY, MSX_MOD_M5MECH, MSX_MOD_M5PROPS, MSX_MOD_M5SEWER,
    MSX_MOD_M5TRAIN, MSX_MOD_M6ENEMY, MSX_MOD_M6PROPS, MSX_MOD_COUNT } msx_overlay_module_t;
/* The region: exactly the largest image, neo_m6boss's (62 240 bytes, -Os, with the death 04180A, escape 04677A and air fight 037404 of work-chars 0171ec9, 20-XI; 43 824 before).
 * tools/link_overlays.py fails the build when an image does not fit and prints each image's size: a boss
 * that grows raises this to its new size. */
#define MSX_OVERLAY_BYTES 62480u   /* image head of 76 bytes (15 tick slots, src/msx_overlay.c) since 26-XI; 62432 since the slot 0x38 of 0421E4 reaches the world (its targets 10C..10E are no longer dead code, 20-XI) */
/* Region 2: exactly the largest mission image (tools/link_overlays.py prints each one's size). */
#define MSX_OVERLAY_CODE_BYTES 76064u   /* m5code (neo_m5enemy, neo_m5mech, neo_m5props, neo_m5sewer, neo_m5train), 26-XI */
#if MSX_OVERLAY
/* The tick of module `mod` of region 2: from the image there if it carries the module, else the module's own
 * mission's image is copied first; a copy while region 2 code runs is an assertion. */
ga_result_t msx_overlay_code_tick(msx_overlay_module_t mod,neo_gactor_t *a,neo_genv_t *e);
/* The overlay's tick, the overlay copied first if another one (or none) is in
 * the region. A copy while overlay code is running (a boss's tick reaching
 * another overlay through neo_region_tick) is an assertion. */
ga_result_t msx_overlay_tick(msx_overlay_id_t id,neo_gactor_t *a,neo_genv_t *e);
/* Mission `mission` (1..6) starts: its boss and its own code copied now, behind the load's black frames. */
void msx_overlay_mission(unsigned mission);
#endif
#endif
