/* The few libdragon calls src/scene_assets.c makes, for the host build of
 * the pack loader (tools/test_pack_loader.c, make check; docs/COMPRESION.md).
 * The cartridge is a set of files at made-up ROM addresses; PI DMA is a copy
 * that ends at once. Nothing here is used by the console build. */
#ifndef HOST_N64_LIBDRAGON_H
#define HOST_N64_LIBDRAGON_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
enum { HOST_ROM_FILES=16 };
typedef struct { char path[40];uint8_t *data;uint32_t size,address; } host_rom_file_t;
static host_rom_file_t host_rom_files[HOST_ROM_FILES];
static unsigned host_rom_count;
static unsigned long host_dma_bytes;
/* Registers `file` as the cartridge's `path` ("/scene.pack"); false if missing. */
static inline bool host_rom_add(const char *path,const char *file) {
    FILE *f=fopen(file,"rb");
    if (!f || host_rom_count==HOST_ROM_FILES) {if (f) fclose(f);return false;}
    host_rom_file_t *r=&host_rom_files[host_rom_count];
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    r->data=malloc((size_t)n+64);r->size=(uint32_t)n;r->address=0x10000000u+0x02000000u*host_rom_count;
    if (fread(r->data,1,(size_t)n,f)!=(size_t)n) {fclose(f);return false;}
    fclose(f);snprintf(r->path,sizeof(r->path),"%s",path);host_rom_count++;
    return true;
}
static inline uint32_t dfs_rom_addr(const char *path) {
    for (unsigned i=0;i<host_rom_count;i++) if (!strcmp(host_rom_files[i].path,path)) return host_rom_files[i].address;
    return 0;
}
static inline int dfs_rom_size(const char *path) {
    for (unsigned i=0;i<host_rom_count;i++) if (!strcmp(host_rom_files[i].path,path)) return (int)host_rom_files[i].size;
    return -1;
}
static inline void dma_read(void *dst,unsigned long address,unsigned long length) {
    for (unsigned i=0;i<host_rom_count;i++) {
        host_rom_file_t *r=&host_rom_files[i];
        /* A chunk's read is rounded up to 64 bytes: the pad after the last file byte reads as zero. */
        if (address>=r->address && address-r->address+length<=((r->size+63u)&~63u)) {
            unsigned long at=address-r->address,n=at+length<=r->size?length:r->size-at;
            memcpy(dst,r->data+at,n);memset((uint8_t*)dst+n,0,length-n);host_dma_bytes+=length;return;
        }
    }
    fprintf(stderr,"host DMA outside the cartridge files: %08lx+%lu\n",address,length);abort();
}
#define dma_read_async dma_read
static inline void dma_wait(void) {}
static inline void rspq_wait(void) {}
static volatile uint32_t host_pi_status;
#define PI_STATUS (&host_pi_status)
static uint32_t host_ticks;
#define TICKS_READ() (host_ticks+=16u)
#define TICKS_FROM_US(us) ((uint32_t)(us))
#define TICKS_TO_US(t) ((uint32_t)(t))
static inline unsigned long get_ticks_ms(void) {return host_ticks/1000u;}
#define PhysicalAddr(p) ((uint32_t)(uintptr_t)(p))
static inline void data_cache_hit_invalidate(void *p,unsigned long n) {(void)p;(void)n;}
static inline void data_cache_hit_writeback(const void *p,unsigned long n) {(void)p;(void)n;}
static inline void data_cache_hit_writeback_invalidate(void *p,unsigned long n) {(void)p;(void)n;}
static inline int get_memory_size(void) {return 8*1024*1024;}
typedef struct { int total,used; } heap_stats_t;
/* A heap large enough for every chunk of scene.pack to have a slot. */
static inline void sys_get_heap_stats(heap_stats_t *h) {h->total=64*1024*1024;h->used=0;}
static inline void register_VI_handler(void (*handler)(void)) {(void)handler;}
static inline void host_debugf(const char *format,...) {(void)format;}
#define debugf host_debugf
#define assertf(cond,...) do {if (!(cond)) {fprintf(stderr,"assertf: " __VA_ARGS__);fputc('\n',stderr);abort();}} while (0)
#endif
