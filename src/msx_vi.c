#include <libdragon.h>
#include "msx_vi.h"
/* src/msx_vi.h. VI registers (n64brew, libdragon vi.h): 0xA4400018 V_SYNC, 0xA440001C H_SYNC (bits 0..11 the line,
 * 16..20 the leap pattern), 0xA4400020 H_SYNC_LEAP (bits 0..11 leap A, 16..27 leap B). */
#define MSX_VI_REG(o) ((volatile uint32_t *)(0xA4400000u+(o)))
static volatile uint32_t vi_count;
static void on_vi(void) {vi_count++;}
void msx_vi_neogeo(void) {
    static bool hooked;
    if (!hooked) {register_VI_handler(on_vi);hooked=true;}
    if (get_tv_type()!=TV_NTSC) return;   /* PAL and MPAL keep libdragon's timing (the derivation is for the NTSC clock) */
    disable_interrupts();
    *MSX_VI_REG(0x18)=MSX_VI_V_SYNC;MEMORY_BARRIER();
    *MSX_VI_REG(0x1C)=MSX_VI_H_SYNC;MEMORY_BARRIER();   /* leap pattern 0, as libdragon's NTSC */
    *MSX_VI_REG(0x20)=MSX_VI_H_SYNC<<16|MSX_VI_H_SYNC;MEMORY_BARRIER();
    enable_interrupts();
}
uint32_t msx_vi_count(void) {return vi_count;}
