#include "neo_progstore.h"
#include <stdlib.h>
#include <string.h>
typedef struct heap_block { struct heap_block *next; } heap_block_t;
static neo_progstore_read_t reader;static uint8_t *arena;static uint32_t arena_bytes,arena_used;
static heap_block_t *heap_blocks;static neo_progstore_stats_t stats;
void neo_progstore_setup(neo_progstore_read_t read,uint8_t *mem,uint32_t bytes) {
    neo_progstore_reset();reader=read;arena=mem;arena_bytes=bytes;
}
void neo_progstore_reset(void) {
    while (heap_blocks) {heap_block_t *n=heap_blocks->next;free(heap_blocks);heap_blocks=n;}
    arena_used=0;unsigned peak=stats.peak_bytes;memset(&stats,0,sizeof(stats));stats.peak_bytes=peak;
}
/* `head` bytes (a multiple of 16) before the commands, for the caller. */
static uint8_t *load(uint32_t offset,unsigned count,uint32_t head) {
    uint32_t bytes=10u*count,room=head+((bytes+15u)&~15u);uint8_t *d;
    if (!reader || !count) {stats.failed++;return NULL;}
    if (arena && arena_used+room<=arena_bytes) {d=arena+arena_used;arena_used+=room;}
    else {
        /* 16-byte aligned block after its link (the reader's alignment). */
        heap_block_t *b=aligned_alloc(16,16u+room);
        if (!b) {stats.failed++;return NULL;}
        b->next=heap_blocks;heap_blocks=b;d=(uint8_t*)b+16;stats.heap_blocks++;stats.heap_bytes+=16u+room;
    }
    uint8_t *h=d;d+=head;
    if (!reader(d,offset,bytes)) {stats.failed++;return NULL;}
    neo_anim_command_t *c=(neo_anim_command_t*)d;
    for (unsigned k=0;k<count;k++) {   /* in place: the record and the struct are 10 bytes */
        const uint8_t *q=d+10u*k;
        int32_t asset=(int32_t)((uint32_t)q[1]<<16|(uint32_t)q[2]<<8|q[3]);if (asset&0x800000) asset-=0x1000000;
        neo_anim_command_t v={(neo_anim_kind_t)q[0],asset,(uint16_t)(q[4]<<8|q[5]),q[6],q[7],(int8_t)q[8],(int8_t)q[9]};
        c[k]=v;
    }
    stats.programs++;stats.bytes+=room;
    if (stats.bytes>stats.peak_bytes) stats.peak_bytes=stats.bytes;
    return h;
}
const neo_anim_command_t *neo_progstore_load(uint32_t offset,unsigned count) {
    return (const neo_anim_command_t*)load(offset,count,0);
}
const neo_anim_program_t *neo_progstore_load_program(uint32_t offset,unsigned count) {
    _Static_assert(sizeof(neo_anim_program_t)<=16,"program header");
    uint8_t *h=load(offset,count,16);
    if (!h) return NULL;
    neo_anim_program_t *p=(neo_anim_program_t*)h;p->commands=(const neo_anim_command_t*)(h+16);p->count=(uint16_t)count;
    return p;
}
neo_progstore_stats_t neo_progstore_stats(void) {return stats;}
