#include "msx_ramfs.h"
#include <libdragon.h>
#include <system.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
enum { RAMFS_FILES=64, RAMFS_HANDLES=64 };
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
size_t msx_ramfs_load(const char *path) {
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
