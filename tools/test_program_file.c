/* The animation programs the console reads on demand (src/neo_progstore.c,
 * build/programs.bin and soldier_program_refs of
 * tools/compile_soldier_ai_tables.py; docs/MEMORY_OPTIMIZATION.md "Programs on
 * demand") against the tables they replace in the console image: every
 * program of no group, command by command, read through the store with an
 * arena that only holds part of them (the rest take heap blocks), twice with
 * a reset in between; references in tag order, one per program. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MSX_SOLDIER_PROGRAMS 1
#include "../src/neo_progstore.h"
#include "../src/neo_soldier_ai.h"
#include "soldier_ai_tables.h"
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
static FILE *file;static unsigned long file_bytes;
static bool file_read(void *dst,uint32_t offset,uint32_t bytes) {
    bytes=(bytes+1u)&~1u;
    assert(((uintptr_t)dst&15u)==0);
    if (offset+bytes>file_bytes || fseek(file,(long)offset,SEEK_SET)) return false;
    return fread(dst,1,bytes,file)==bytes;
}
static const soldier_program_t *table(uint32_t tag) {
    for (unsigned i=0;i<COUNT(soldier_programs);i++) if (soldier_programs[i].tag==tag) return &soldier_programs[i];
    return NULL;
}
int main(int argc,char **argv) {
    assert(argc==2);file=fopen(argv[1],"rb");if (!file) {fprintf(stderr,"%s not found\n",argv[1]);return 1;}
    uint8_t head[16];assert(fread(head,1,16,file)==16 && !memcmp(head,"MSXP",4) && (head[4]<<8|head[5])==1);
    unsigned n=(unsigned)head[8]<<24|(unsigned)head[9]<<16|(unsigned)head[10]<<8|head[11];
    file_bytes=(unsigned long)head[12]<<24|(unsigned long)head[13]<<16|(unsigned long)head[14]<<8|head[15];
    fseek(file,0,SEEK_END);assert((unsigned long)ftell(file)==file_bytes && file_bytes==SOLDIER_PROGRAM_FILE_BYTES);
    assert(n==COUNT(soldier_program_refs));
    enum {ARENA=65536};
    uint8_t *arena=aligned_alloc(16,ARENA);assert(arena);
    neo_progstore_setup(file_read,arena,ARENA);
    unsigned long commands=0;unsigned heap=0;
    for (unsigned pass=0;pass<2;pass++) {
        commands=0;
        for (unsigned i=0;i<n;i++) {
            const soldier_program_ref_t *r=&soldier_program_refs[i];
            if (i && r[-1].tag>=r->tag) {fprintf(stderr,"reference %u: tags not ascending\n",i);return 1;}
            const soldier_program_t *t=table(r->tag);
            if (!t || t->program.count!=SOLDIER_PROGRAM_REF_COUNT(r)) {fprintf(stderr,"program %06x: not in the table with %u commands\n",(unsigned)r->tag,(unsigned)SOLDIER_PROGRAM_REF_COUNT(r));return 1;}
            /* the second pass reads each program with its header (the console's neo_world.c) */
            const neo_anim_program_t *h=pass?neo_progstore_load_program(r->offset,SOLDIER_PROGRAM_REF_COUNT(r)):NULL;
            const neo_anim_command_t *c=pass?(h && h->count==SOLDIER_PROGRAM_REF_COUNT(r)?h->commands:NULL):neo_progstore_load(r->offset,SOLDIER_PROGRAM_REF_COUNT(r));
            if (!c) {fprintf(stderr,"program %06x: not read\n",(unsigned)r->tag);return 1;}
            for (unsigned k=0;k<SOLDIER_PROGRAM_REF_COUNT(r);k++) {
                const neo_anim_command_t *a=&c[k],*b=&t->program.commands[k];
                if (a->kind!=b->kind || a->asset!=b->asset || a->target!=b->target || a->operand!=b->operand || a->flags!=b->flags || a->dx!=b->dx || a->dy!=b->dy) {
                    fprintf(stderr,"program %06x command %u differs\n",(unsigned)r->tag,k);return 1;
                }
            }
            commands+=SOLDIER_PROGRAM_REF_COUNT(r);
        }
        neo_progstore_stats_t s=neo_progstore_stats();
        if (s.programs!=n || s.failed || !s.heap_blocks || s.bytes-s.heap_bytes+16u*s.heap_blocks>ARENA) {fprintf(stderr,"store counts: %u programs, %u failed, %u heap blocks\n",s.programs,s.failed,s.heap_blocks);return 1;}
        heap=s.heap_blocks;
        neo_progstore_reset();
        s=neo_progstore_stats();assert(!s.programs && !s.bytes && !s.heap_blocks);
    }
    /* Every program of no group has its reference (the console has no other copy). */
    unsigned image=0;
    for (unsigned i=0;i<COUNT(soldier_programs);i++) {
        unsigned lo=0,hi=n;uint32_t tag=soldier_programs[i].tag;
        while (lo<hi) {unsigned m=(lo+hi)/2;if (soldier_program_refs[m].tag<tag) lo=m+1; else hi=m;}
        if (lo<n && soldier_program_refs[lo].tag==tag) image++;
    }
    assert(image==n);
    /* A read past the file is an explicit failure, not garbage. */
    assert(!neo_progstore_load((uint32_t)file_bytes-8u,1) && neo_progstore_stats().failed==1);
    neo_progstore_reset();free(arena);fclose(file);
    printf("PASS: %u programs (%lu commands) read on demand equal to the tables, %u beyond a %u KB arena in heap blocks, twice with a reset (the second with the 16-byte program header)\n",n,commands,heap,ARENA/1024);
    return 0;
}
