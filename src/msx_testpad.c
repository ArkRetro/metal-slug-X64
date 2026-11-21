/* Scripted pad of the test cartridge (mslugx-testpad.z64 only, B78; docs/FALLOS.md "Mando por guion").
 *
 * Linked with --wrap=joypad_poll,joypad_get_buttons,joypad_get_inputs: the game's own code is the release's, and
 * the user cartridge (mslugx-explorer.z64) never links this file. Every joypad_poll() is one pad read, numbered
 * from the power-on (n = 1, 2, ...); the buttons the game sees at read n come from a script appended to the ROM
 * (tools/padscript.py: magic "MSXPAD01" at a 1 MiB boundary past the cartridge, a count, then {u32 n, u16 buttons,
 * u16 0} entries, big-endian, n increasing: the buttons hold from n until the next entry). Without a script the pad
 * is the real one (a recording run: xdotool through ares). In both modes the analogue stick reads 0 and every
 * change of the buttons is logged as `MSX PAD n=N btn=XXXX`: a recording's log is its script, and a replay writes
 * the same lines at the same reads (the same debug output, the same emulated time). With the same ROM, the same
 * script and the same EEPROM, two replays in ares are the same run, update for update. */
#include <libdragon.h>
#include <string.h>

joypad_buttons_t __real_joypad_get_buttons(joypad_port_t port);
void __real_joypad_poll(void);

#define PAD_MAGIC "MSXPAD01"
#define PAD_CHUNK 64u
static bool pad_init,pad_script;static uint32_t pad_base,pad_count,pad_next;   /* pad_next: index of the next entry */
static uint32_t pad_reads;static uint16_t pad_raw,pad_logged=0xffffu;
static uint8_t pad_buf[PAD_CHUNK*8u] __attribute__((aligned(16)));static uint32_t pad_buf_first=0xffffffffu;

static uint32_t be32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static const uint8_t *pad_entry(uint32_t i) {
    uint32_t first=i/PAD_CHUNK*PAD_CHUNK;
    if (first!=pad_buf_first) {
        uint32_t n=pad_count-first<PAD_CHUNK?pad_count-first:PAD_CHUNK;
        data_cache_hit_writeback_invalidate(pad_buf,sizeof pad_buf);
        dma_read(pad_buf,0x10000000u+pad_base+16u+first*8u,(n*8u+1u)&~1u);
        pad_buf_first=first;
    }
    return pad_buf+(i-first)*8u;
}
static void pad_find(void) {
    pad_init=true;
    static uint8_t head[16] __attribute__((aligned(16)));
    for (uint32_t at=0x100000u;at<0x8000000u;at+=0x100000u) {
        data_cache_hit_writeback_invalidate(head,sizeof head);
        dma_read(head,0x10000000u+at,sizeof head);
        if (!memcmp(head,PAD_MAGIC,8)) {pad_script=true;pad_base=at;pad_count=be32(head+8);break;}
    }
    debugf("MSX PAD %s count=%lu at=%07lX\n",pad_script?"script":"record",(unsigned long)pad_count,(unsigned long)pad_base);
}
void __wrap_joypad_poll(void) {
    if (!pad_init) pad_find();
    __real_joypad_poll();
    pad_reads++;
    if (pad_script) {
        while (pad_next<pad_count) {
            const uint8_t *e=pad_entry(pad_next);
            if (be32(e)>pad_reads) break;
            pad_raw=(uint16_t)(e[4]<<8|e[5]);pad_next++;
        }
    } else pad_raw=__real_joypad_get_buttons(JOYPAD_PORT_1).raw;
    if (pad_raw!=pad_logged) {pad_logged=pad_raw;debugf("MSX PAD n=%lu btn=%04X\n",(unsigned long)pad_reads,pad_raw);}
}
joypad_buttons_t __wrap_joypad_get_buttons(joypad_port_t port) {
    joypad_buttons_t b={0};if (port==JOYPAD_PORT_1) b.raw=pad_raw;
    return b;
}
joypad_inputs_t __wrap_joypad_get_inputs(joypad_port_t port) {
    joypad_inputs_t in;memset(&in,0,sizeof in);in.btn=__wrap_joypad_get_buttons(port);
    return in;
}
