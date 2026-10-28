#ifndef MSX_CINEMATIC_H
#define MSX_CINEMATIC_H
#include <libdragon.h>
#include <stdbool.h>
#include <stdint.h>
typedef struct { uint32_t frames,index,read_bytes,max_packet,max_decode_ticks,total_decode_ticks,max_draw_ticks;uint16_t width,height; } msx_cinematic_stats_t;
/* Native noninteractive cutscene data. Pixels are generated offline from ROM
 * graphics; no CPU, VDP or actor emulation runs in this player. */
bool msx_cinematic_open(const char *path);
bool msx_cinematic_next(void);
void msx_cinematic_draw(surface_t *target);
msx_cinematic_stats_t msx_cinematic_stats(void);
/* Linear scan for internal verification, not per-frame rendering. */
uint32_t msx_cinematic_pixels_crc(void);
/* Must be called after the last draw has completed. draw() waits for the RDP. */
void msx_cinematic_close(void);
#endif
