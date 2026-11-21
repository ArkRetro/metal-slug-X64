/* Scripted pad of the test cartridge (mslugx-testpad.z64 only, B78; docs/FALLOS.md "Mando por guion").
 *
 * Linked with --wrap=joypad_poll,joypad_get_buttons,joypad_get_inputs and with actor.c built -DMSX_TESTPAD; the user
 * cartridge (mslugx-explorer.z64) links neither. Every joypad_poll() is one pad read, numbered from the power-on
 * (n = 1, 2, ...). Two things the game takes from the host decide a run: the buttons, and how many simulation steps a
 * frame runs (actor.c: scene_clock_steps over the CPU count, which in an emulator follows the host's pace). Both are
 * streams of the script: kind 0 the buttons, kind 1 the frame's steps, kind 2 the steps of a paused frame. Each value
 * holds from its read n until the next entry of its kind.
 *
 * Without a script (a recording run: keys from xdotool, the real clock) every change is logged, `MSX PAD n=N btn=XXXX`
 * and `MSX PAD n=N sK=V`, and the log is the script (tools/padscript.py from-log). With a script (tools/padscript.py
 * apply: magic "MSXPAD01" at a 1 MiB boundary past the cartridge, a count, then {u32 n, u16 value, u16 kind} entries,
 * big-endian, n not decreasing) the game gets the script's values and the same lines are logged at the same reads, so
 * with the same ROM, script and EEPROM two replays in ares are the same run, update for update. */
#include <libdragon.h>
#include <string.h>

joypad_buttons_t __real_joypad_get_buttons(joypad_port_t port);
void __real_joypad_poll(void);

#define PAD_MAGIC "MSXPAD01"
#define PAD_CHUNK 64u
#define PAD_KINDS 3u
static bool pad_init,pad_script,pad_then_record;static uint32_t pad_last;static uint32_t pad_base,pad_count,pad_next;
static uint32_t pad_reads;static uint16_t pad_val[PAD_KINDS],pad_logged[PAD_KINDS]={0xffffu,0xffffu,0xffffu};
static uint8_t pad_buf[PAD_CHUNK*8u] __attribute__((aligned(16)));static uint32_t pad_buf_first=0xffffffffu;

static uint32_t be32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static const uint8_t *pad_entry(uint32_t i) {
    uint32_t first=i/PAD_CHUNK*PAD_CHUNK;
    if (first!=pad_buf_first) {
        uint32_t n=pad_count-first<PAD_CHUNK?pad_count-first:PAD_CHUNK;
        data_cache_hit_writeback_invalidate(pad_buf,sizeof pad_buf);
        dma_read(pad_buf,0x10000000u+pad_base+16u+first*8u,n*8u);
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
        if (!memcmp(head,PAD_MAGIC,8)) {pad_script=true;pad_base=at;pad_count=be32(head+8);pad_then_record=(head[15]&1u)!=0;break;}
    }
    debugf("MSX PAD %s count=%lu at=%07lX\n",pad_script?"script":"record",(unsigned long)pad_count,(unsigned long)pad_base);
}
static void pad_log(unsigned kind) {
    if (pad_val[kind]==pad_logged[kind]) return;
    pad_logged[kind]=pad_val[kind];
    if (!kind) debugf("MSX PAD n=%lu btn=%04X\n",(unsigned long)pad_reads,pad_val[0]);
    else debugf("MSX PAD n=%lu s%u=%u\n",(unsigned long)pad_reads,kind,pad_val[kind]);
}
void __wrap_joypad_poll(void) {
    if (!pad_init) pad_find();
    __real_joypad_poll();
    pad_reads++;
    if (pad_script && pad_then_record && pad_next>=pad_count && pad_reads>pad_last) {
        /* A continued recording (tools/padscript.py apply --then-record): past the script's last read the pad and the
         * clock are the real ones again, and the log goes on being the script. */
        pad_script=false;debugf("MSX PAD script end n=%lu: recording\n",(unsigned long)pad_reads);
    }
    if (pad_script) {
        while (pad_next<pad_count) {
            const uint8_t *e=pad_entry(pad_next);
            if (be32(e)>pad_reads) break;
            unsigned kind=e[7];if (kind<PAD_KINDS) pad_val[kind]=(uint16_t)(e[4]<<8|e[5]);
            pad_last=be32(e);pad_next++;
        }
    } else pad_val[0]=__real_joypad_get_buttons(JOYPAD_PORT_1).raw;
    pad_log(0);
}
/* actor.c (MSX_TESTPAD): the steps the host's clock gave this frame -> the steps the frame runs. */
unsigned msx_testpad_steps(unsigned steps,unsigned kind) {
    if (pad_script) steps=pad_val[kind];
    else pad_val[kind]=(uint16_t)steps;
    pad_log(kind);
    return steps;
}
joypad_buttons_t __wrap_joypad_get_buttons(joypad_port_t port) {
    joypad_buttons_t b={0};if (port==JOYPAD_PORT_1) b.raw=pad_val[0];
    return b;
}
joypad_inputs_t __wrap_joypad_get_inputs(joypad_port_t port) {
    joypad_inputs_t in;memset(&in,0,sizeof in);in.btn=__wrap_joypad_get_buttons(port);
    return in;
}
