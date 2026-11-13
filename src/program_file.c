#include "program_file.h"
#include <libdragon.h>
#include <string.h>
/* The arena is part of the program image (bss), so the scene cache is sized
 * with it already taken (scene_assets.c). Its size is the measured need of a
 * stage with room (docs/MEMORY_OPTIMIZATION.md "Programs on demand"); a
 * program beyond it takes a heap block and the line below says so. */
enum { ARENA_BYTES=PROGRAM_ARENA_KB*1024 };
static uint8_t arena[ARENA_BYTES] __attribute__((aligned(16)));
static uint32_t rom,rom_bytes,read_us,read_max_us;static bool opened,reported;static unsigned logged_kb;
static bool file_read(void *dst,uint32_t offset,uint32_t bytes) {
    bytes=(bytes+1u)&~1u;
    if (!rom || offset+bytes>rom_bytes) return false;
    data_cache_hit_writeback_invalidate(dst,(bytes+15u)&~15u);dma_read(dst,rom+offset,bytes);
    return true;
}
static void open_file(void) {
    static uint8_t head[16] __attribute__((aligned(16)));
    opened=true;
    uint32_t at=(uint32_t)dfs_rom_addr("/programs.bin");
    if (!at) {debugf("MSX PROGRAMS /programs.bin missing (explicit stop: no animation programs)\n");return;}
    rom=PhysicalAddr((void*)(uintptr_t)at);
    data_cache_hit_writeback_invalidate(head,sizeof(head));dma_read(head,rom,sizeof(head));
    uint32_t bytes=(uint32_t)head[12]<<24|(uint32_t)head[13]<<16|(uint32_t)head[14]<<8|head[15];
    if (memcmp(head,"MSXP",4) || (head[4]<<8|head[5])!=1u) {debugf("MSX PROGRAMS /programs.bin not read (explicit stop)\n");rom=0;return;}
    rom_bytes=bytes;neo_progstore_setup(file_read,arena,ARENA_BYTES);
}
const neo_anim_program_t *program_file_load(uint32_t offset,unsigned count) {
    if (!opened) open_file();
    uint32_t t0=TICKS_READ();
    const neo_anim_program_t *c=neo_progstore_load_program(offset,count);
    uint32_t us=TICKS_TO_US(TICKS_SINCE(t0));read_us+=us;if (us>read_max_us) read_max_us=us;
    neo_progstore_stats_t s=neo_progstore_stats();
    if (!c && !reported) {reported=true;debugf("MSX PROGRAMS program at %lu not read (explicit stop)\n",(unsigned long)offset);}
    if (s.bytes/8192u>logged_kb) {   /* one line per 8 KB more */
        logged_kb=s.bytes/8192u;
        debugf("MSX PROGRAMS %u read, %u KB of %u (peak %u KB), heap blocks %u (%u bytes), read time %lu us in all, longest %lu us\n",s.programs,s.bytes/1024u,(unsigned)PROGRAM_ARENA_KB,s.peak_bytes/1024u,s.heap_blocks,s.heap_bytes,(unsigned long)read_us,(unsigned long)read_max_us);
    }
    return c;
}
void program_file_reset(void) {
    neo_progstore_stats_t s=neo_progstore_stats();
    if (s.programs) debugf("MSX PROGRAMS reset: %u read, %u bytes of %u KB, heap blocks %u (%u bytes), failed %u, read time %lu us in all, longest %lu us\n",s.programs,s.bytes,(unsigned)PROGRAM_ARENA_KB,s.heap_blocks,s.heap_bytes,s.failed,(unsigned long)read_us,(unsigned long)read_max_us);
    neo_progstore_reset();logged_kb=0;read_us=read_max_us=0;
}
const uint8_t *program_file_rom(uint32_t offset) {
    static uint8_t window[8][48] __attribute__((aligned(16)));static unsigned next;
    static uint32_t brom,bbytes;static bool bopened;
    if (!bopened) {
        static uint8_t head[16] __attribute__((aligned(16)));
        bopened=true;
        uint32_t at=(uint32_t)dfs_rom_addr("/romblobs.bin");
        if (!at) {debugf("MSX ROMBLOBS /romblobs.bin missing (explicit stop: no ROM tables of the cartridge)\n");return NULL;}
        brom=PhysicalAddr((void*)(uintptr_t)at);
        data_cache_hit_writeback_invalidate(head,sizeof(head));dma_read(head,brom,sizeof(head));
        if (memcmp(head,"MSXB",4) || (head[4]<<8|head[5])!=1u) {debugf("MSX ROMBLOBS /romblobs.bin not read (explicit stop)\n");brom=0;return NULL;}
        bbytes=(uint32_t)head[8]<<24|(uint32_t)head[9]<<16|(uint32_t)head[10]<<8|head[11];brom+=16u;
    }
    if (!brom || offset>=bbytes) return NULL;
    uint32_t start=offset&~1u,n=32u;
    if (start+n>bbytes) n=(bbytes-start+1u)&~1u;
    uint8_t *w=window[next++&7u];
    data_cache_hit_writeback_invalidate(w,sizeof(window[0]));dma_read(w,brom+start,n);
    return w+(offset-start);
}
