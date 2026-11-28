#ifndef MSX_VI_H
#define MSX_VI_H
/* The VI at the Neo Geo's refresh (B92, docs/RENDIMIENTO.md "VI a la frecuencia de la Neo Geo").
 *
 * Neo Geo (MAME 0.264 neogeo.h/neogeo.cpp: set_raw(NEOGEO_PIXEL_CLOCK = 24 MHz / 4, NEOGEO_HTOTAL 384,
 * NEOGEO_VTOTAL 264)): 6 000 000 / (384 * 264) = 59.185606 Hz.
 * N64 NTSC VI: clock 48 681 812 Hz; a line lasts VI_H_SYNC + 1 clocks (libdragon's NTSC 0xC15 -> 3094 clocks,
 * 15 734.26 Hz, the NTSC line) and a progressive field (VI_V_SYNC + 1) / 2 lines (0x20D -> 263 lines, 59.826 Hz).
 * One field must last 48 681 812 / 59.185606 = 822 527.9 VI clocks. Of all lines x clocks near NTSC the closest is
 * 265 lines (V_SYNC 529, two more lines of vertical blanking) of 3104 clocks (H_SYNC 3103, ten more clocks of
 * horizontal blanking): 822 560 clocks, 59.183296 Hz, 39 ppm slower. The visible window (H_VIDEO, V_VIDEO, burst,
 * scales) is libdragon's, unchanged. The leap lengths are set to the same line so no line of the field differs. */
#define MSX_VI_CLOCK      48681812u
#define MSX_VI_H_SYNC     3103u          /* line = 3104 VI clocks (15 683.57 Hz) */
#define MSX_VI_V_SYNC     529u           /* 265 lines per progressive field */
#define MSX_VI_LINE       (MSX_VI_H_SYNC+1u)
#define MSX_VI_LINES      ((MSX_VI_V_SYNC+1u)/2u)
/* Neo Geo vblanks per VI field: (6e6 / 101376) / (48681812 / 822560) = 401640625 / 401624949 (1.000039). */
#define MSX_VI_STEP_NUM   401640625u
#define MSX_VI_STEP_DEN   401624949u
#ifdef N64
#include <stdint.h>
/* Rewrites VI_V_SYNC, VI_H_SYNC and VI_H_SYNC_LEAP after display_init (which writes libdragon's NTSC preset). */
void msx_vi_neogeo(void);
/* VI interrupts since msx_vi_neogeo() first ran. */
uint32_t msx_vi_count(void);
#endif
#endif
