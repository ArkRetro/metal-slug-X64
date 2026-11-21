#include "msx_ramfs.h"
#include <libdragon.h>
#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
/* 64 held only 63 effects with fixed.bin: the last six of msx_sfx_defs
 * (1142, 1158, 115A, 1168, 116A, 116E) never loaded (B34). */
enum { RAMFS_FILES=96, RAMFS_HANDLES=96 };   /* each resident effect keeps one open (wav64_open) */
typedef struct { char name[48]; uint8_t *data; size_t size; } ram_file_t;
typedef struct { const ram_file_t *file; size_t pos; bool used; } ram_handle_t;
static ram_file_t files[RAMFS_FILES];static unsigned file_count;
static ram_handle_t handles[RAMFS_HANDLES];
static size_t total;
static const ram_file_t *find(const char *name) {
    while (*name=='/') name++;
    for (unsigned i=0;i<file_count;i++) if (!strcmp(files[i].name,name)) return &files[i];
    return NULL;
}
static void *ram_open(char *name,int flags) {
    (void)flags;const ram_file_t *f=find(name);
    if (!f) {errno=ENOENT;return NULL;}
    for (unsigned i=0;i<RAMFS_HANDLES;i++) if (!handles[i].used) {handles[i]=(ram_handle_t){f,0,true};return &handles[i];}
    errno=EMFILE;return NULL;
}
static int ram_fstat(void *file,struct stat *st) {
    ram_handle_t *h=file;memset(st,0,sizeof(*st));st->st_size=(off_t)h->file->size;st->st_mode=S_IFREG;return 0;
}
static int ram_lseek(void *file,int ptr,int dir) {
    ram_handle_t *h=file;long p=dir==SEEK_SET?ptr:dir==SEEK_CUR?(long)h->pos+ptr:(long)h->file->size+ptr;
    if (p<0) p=0;
    if ((size_t)p>h->file->size) p=(long)h->file->size;
    h->pos=(size_t)p;return (int)p;
}
static int ram_read(void *file,uint8_t *ptr,int len) {
    ram_handle_t *h=file;size_t n=h->file->size-h->pos;
    if (len<0) return -1;
    if ((size_t)len<n) n=(size_t)len;
    memcpy(ptr,h->file->data+h->pos,n);h->pos+=n;
    /* The caller may hand the buffer to the RSP (wav64 VADPCM decoding DMAs
     * it from RDRAM): a CPU copy stays in the data cache, so write it back,
     * as a cartridge read (PI DMA) would have landed it in RDRAM. Without
     * this the RSP decoded stale memory: distorted effects (ROM 10). */
    if (n) data_cache_hit_writeback(ptr,n);
    return (int)n;
}
static int ram_close(void *file) {((ram_handle_t *)file)->used=false;return 0;}
static filesystem_t ramfs={.open=ram_open,.fstat=ram_fstat,.lseek=ram_lseek,.read=ram_read,.close=ram_close};
bool msx_ramfs_init(void) {
    static bool attached;
    if (!attached) attached=attach_filesystem("ram:/",&ramfs)==0;
    return attached;
}
/* Background reads: a FIFO of files, each read by asynchronous DMA chunks
 * (the PI is shared with the cinematic loader and the audio streams: a
 * chunk is issued only while it is idle). */
enum { PRELOAD_MAX=RAMFS_FILES,PRELOAD_CHUNK=8192 };
typedef struct { char name[48];uint32_t rom,size,pos;uint8_t *data;bool done; } preload_t;
/* In the heap from the first file queued to msx_ramfs_preload_end: 7 KB the
 * game does not keep (they were bss; docs/COMPRESION.md, section 5). */
static preload_t *queue;static unsigned queued,queue_next;
static bool pi_idle(void) {return !(*PI_STATUS & 3u);}
static preload_t *queued_file(const char *path) {
    for (unsigned i=0;i<queued;i++) if (!queue[i].done && !strcmp(queue[i].name,path)) return &queue[i];
    return NULL;
}
static void publish(preload_t *p) {
    ram_file_t *r=&files[file_count++];strcpy(r->name,p->name);r->data=p->data;r->size=p->size;
    total+=p->size;p->done=true;
}
bool msx_ramfs_preload(const char *path) {
    if (queued==PRELOAD_MAX || file_count+queued>=RAMFS_FILES || strlen(path)>=sizeof(queue[0].name) || find(path) || queued_file(path)) return false;
    if (!queue && !(queue=malloc(PRELOAD_MAX*sizeof(*queue)))) return false;
    preload_t *p=&queue[queued++];*p=(preload_t){0};strcpy(p->name,path);   // looked up in the ROM when its turn comes
    return true;
}
/* The ROM address and size of a queued file (a DFS directory walk: in the
 * background, not at the cut where the files are queued). */
static bool locate(preload_t *p) {
    if (p->rom) return true;
    char rom[64];snprintf(rom,sizeof(rom),"/%s",p->name);
    uint32_t addr=dfs_rom_addr(rom);if (!addr) return false;
    uint32_t pi=PhysicalAddr((void *)(uintptr_t)addr),size=dfs_rom_size(rom);
    if ((pi&1u) || !size) return false;   // a DMA needs the RAM and ROM addresses of the same parity
    p->rom=pi;p->size=size;return true;
}
bool msx_ramfs_preload_idle(unsigned long deadline,int keep) {
    while (queue_next<queued && queue[queue_next].done) queue_next++;
    if (queue_next>=queued || file_count==RAMFS_FILES) return false;
    if ((int32_t)((uint32_t)deadline-TICKS_READ())<(int32_t)TICKS_FROM_US(200)) return false;
    preload_t *p=&queue[queue_next];
    if (!locate(p)) {p->done=true;queue_next++;return true;}   // msx_ramfs_load reads it (or reports it missing)
    if (!p->data) {
        heap_stats_t h;sys_get_heap_stats(&h);
        if (h.total-h.used-(int)p->size<keep) return false;
        p->data=memalign(16,(p->size+15u)&~15u);if (!p->data) return false;
        data_cache_hit_invalidate(p->data,(p->size+15u)&~15u);
    }
    if (!pi_idle()) return true;   // ours or another reader's DMA: poll again
    if (p->pos<p->size) {
        uint32_t n=p->size-p->pos;if (n>PRELOAD_CHUNK) n=PRELOAD_CHUNK;
        dma_read_async(p->data+p->pos,p->rom+p->pos,n);p->pos+=n;
        return true;
    }
    publish(p);queue_next++;
    return true;
}
void msx_ramfs_preload_end(void) {
    /* A file still in the queue stays as it was left (msx_ramfs_load has
     * finished the ones the game uses). */
    free(queue);queue=NULL;queued=queue_next=0;
}
size_t msx_ramfs_load(const char *path) {
    {const ram_file_t *f=find(path);if (f) return f->size;}   // preloaded
    preload_t *p=queued_file(path);
    if (p && p->data) {   // started in the background: finish it now
        dma_wait();
        if (p->pos<p->size) dma_read(p->data+p->pos,p->rom+p->pos,p->size-p->pos);
        p->pos=p->size;publish(p);return p->size;
    }
    if (p) p->done=true;   // not started: read it below
    if (file_count==RAMFS_FILES || strlen(path)>=sizeof(files[0].name)) return 0;
    char rom[64];snprintf(rom,sizeof(rom),"rom:/%s",path);
    FILE *f=fopen(rom,"rb");
    if (!f) return 0;
    fseek(f,0,SEEK_END);long size=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *data=size>0?malloc((size_t)size):NULL;
    if (!data || fread(data,1,(size_t)size,f)!=(size_t)size) {free(data);fclose(f);return 0;}
    fclose(f);
    ram_file_t *r=&files[file_count++];strcpy(r->name,path);r->data=data;r->size=(size_t)size;
    total+=(size_t)size;return (size_t)size;
}
size_t msx_ramfs_bytes(void) {return total;}
const void *msx_ramfs_data(const char *path,size_t *size) {
    const ram_file_t *f=find(path);
    if (!f) return NULL;
    if (size) *size=f->size;
    return f->data;
}
