/* All of this module goes after the rest of the program's code (the linker
 * script of libdragon places keep.text.* last): adding it does not move the
 * code of the game or of libdragon, whose cache alignment the frame times
 * depend on (docs/COMPRESION.md). */
#define MSXL_CODE __attribute__((section("keep.text.msx_pcm")))
#include "msx_pcm.h"
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
MSXL_CODE static bool buffers(msx_pcm_t *p) {
    if (p->raw) return true;
    unsigned block=1u<<p->info.block_log2,bytes=block*p->info.channels*sizeof(int16_t);
    /* One allocation, 16-byte aligned: the cartridge read goes straight to raw (DFS). */
    unsigned stored=(msxl_block_bound(p->info.channels,block)+2u+15u)&~15u;
    p->raw=memalign(16,stored+MSX_PCM_SLOTS*bytes);
    if (!p->raw) return false;
    for (unsigned i=0;i<MSX_PCM_SLOTS;i++) p->pcm[i]=(int16_t *)(void *)(p->raw+stored+i*bytes);
    return true;
}
/* The buffer holding `block`, or -1. */
MSXL_CODE static int find(const msx_pcm_t *p,uint32_t block) {
    for (int i=0;i<MSX_PCM_SLOTS;i++) if (p->have[i]==(int32_t)block) return i;
    return -1;
}
/* Block `block` into a buffer that holds none of block-or-the-two-after
 * (silence if the file is damaged). Returns the buffer. */
MSXL_CODE static unsigned load(msx_pcm_t *p,uint32_t block,uint32_t keep) {
    unsigned slot=0;
    while (slot<MSX_PCM_SLOTS-1 && p->have[slot]>=(int32_t)keep && p->have[slot]<=(int32_t)keep+2) slot++;
    const msxl_info_t *info=&p->info;
    unsigned n=msxl_block_samples(info,block);
    uint32_t offset,size;bool ok=true;
#if MSX_PCM_STATS
    uint32_t t0=TICKS_READ();
#endif
    if (block==p->next) {offset=p->next_offset;size=p->next_size;}
    else {
        if (!block) {offset=MSXL_HEADER;size=info->first_size;}
        else {
            uint8_t entry[16] __attribute__((aligned(16)));
            lseek(p->fd,(off_t)(info->index_offset+4u*block),SEEK_SET);
            ok=read(p->fd,entry,8)==8;
            offset=msxl_be32(entry);size=msxl_be32(entry+4)-offset-2u;
        }
        if (ok) lseek(p->fd,(off_t)offset,SEEK_SET);
    }
    ok=ok && !(offset&1u) && !(size&1u) && size>=2 && size<=msxl_block_bound(info->channels,n) && offset>=MSXL_HEADER && offset+size+2u<=info->index_offset;
    ok=ok && read(p->fd,p->raw,size+2u)==(int)(size+2u);
    if (ok) {
        int32_t scratch[MSXL_BLOCK_MAX];
#if MSX_PCM_STATS
        uint32_t d0=TICKS_READ();
#endif
        ok=msxl_decode_block(p->raw,size,info->channels,(int)n,p->pcm[slot],scratch);
#if MSX_PCM_STATS
        p->decode_ticks+=TICKS_READ()-d0;
#endif
    }
    if (ok) {p->next=block+1;p->next_offset=offset+size+2u;p->next_size=msxl_be16(p->raw+size);}
    else {
        if (!p->errors++) debugf("MSX ERROR pcm %s block %lu\n",p->name,(unsigned long)block);
        memset(p->pcm[slot],0,n*info->channels*sizeof(int16_t));p->next=UINT32_MAX;
    }
    p->have[slot]=(int32_t)block;
#if MSX_PCM_STATS
    uint32_t t=TICKS_READ()-t0;
    p->load_ticks+=t;p->loads++;
    if (t>p->load_max) p->load_max=t;
#endif
    return slot;
}
MSXL_CODE static void pcm_read(void *ctx,samplebuffer_t *sbuf,int wpos,int wlen,bool seeking) {
    (void)seeking;
    msx_pcm_t *p=ctx;const msxl_info_t *info=&p->info;
    unsigned frame=info->channels*sizeof(int16_t);
    uint8_t *start=CachedAddr(samplebuffer_append(sbuf,wlen)),*dst=start;
    bool ready=buffers(p);
    while (wlen>0) {
        int count=wlen;
        if (!ready || wpos<0 || (uint32_t)wpos>=info->len) memset(dst,0,(size_t)count*frame);
        else {
            uint32_t block=(uint32_t)wpos>>info->block_log2,at=(uint32_t)wpos&((1u<<info->block_log2)-1);
            if (p->have[p->current]!=(int32_t)block) {
                int slot=find(p,block);
                p->current=(uint8_t)(slot>=0?(unsigned)slot:load(p,block,block));
            }
            unsigned n=msxl_block_samples(info,block);
            if ((unsigned)count>n-at) count=(int)(n-at);
            memcpy(dst,p->pcm[p->current]+at*info->channels,(size_t)count*frame);
        }
        dst+=(size_t)count*frame;wpos+=count;wlen-=count;
    }
    /* The RSP mixes from RDRAM: land the samples there, as the cartridge
     * read of a raw WAV64 does (see ram_read in msx_ramfs.c). */
    if (dst>start) data_cache_hit_writeback(start,(unsigned long)(dst-start));
}
MSXL_CODE bool msx_pcm_prefetch(msx_pcm_t *p,uint32_t deadline) {
    if (!p->raw || p->have[p->current]<0) return false;   /* not playing yet */
    uint32_t playing=(uint32_t)p->have[p->current],want=playing+1u;
    if (want<p->info.blocks && find(p,want)>=0) want++;
    if (want>=p->info.blocks || find(p,want)>=0) return false;
    if ((int32_t)(deadline-TICKS_READ())<(int32_t)TICKS_FROM_US(MSX_PCM_PREFETCH_US)) return false;
    load(p,want,playing);
#if MSX_PCM_STATS
    p->ahead++;
#endif
    return true;
}
MSXL_CODE bool msx_pcm_open(msx_pcm_t *pcm,const char *path) {
    memset(pcm,0,sizeof(*pcm));pcm->fd=-1;
    int fd=open(path,O_RDONLY);
    if (fd<0) return false;
    uint8_t header[MSXL_HEADER+8] __attribute__((aligned(16)));
    if (read(fd,header,MSXL_HEADER)!=MSXL_HEADER || !msxl_parse(header,&pcm->info)) {close(fd);return false;}
    pcm->fd=fd;pcm->next=UINT32_MAX;
    for (unsigned i=0;i<MSX_PCM_SLOTS;i++) pcm->have[i]=-1;
    strncpy(pcm->name,path,sizeof(pcm->name)-1);
    pcm->wave=(waveform_t){.name=pcm->name,.bits=16,.channels=pcm->info.channels,.frequency=(float)pcm->info.frequency,
        .len=(int)pcm->info.len,.loop_len=(int)pcm->info.loop_len,.read=pcm_read,.ctx=pcm};
    return true;
}
MSXL_CODE void msx_pcm_release(msx_pcm_t *pcm) {
#if MSX_PCM_STATS
    if (pcm->loads) {
        debugf("MSX PCM %s blocks=%lu ahead=%lu load avg=%lu max=%lu us (decode avg=%lu us) errors=%lu\n",pcm->name,(unsigned long)pcm->loads,(unsigned long)pcm->ahead,
            (unsigned long)(TICKS_TO_US((uint64_t)pcm->load_ticks)/pcm->loads),(unsigned long)TICKS_TO_US((uint64_t)pcm->load_max),
            (unsigned long)(TICKS_TO_US((uint64_t)pcm->decode_ticks)/pcm->loads),(unsigned long)pcm->errors);
        pcm->loads=pcm->ahead=pcm->load_ticks=pcm->load_max=pcm->decode_ticks=0;
    }
#endif
    free(pcm->raw);pcm->raw=NULL;pcm->next=UINT32_MAX;
    for (unsigned i=0;i<MSX_PCM_SLOTS;i++) {pcm->pcm[i]=NULL;pcm->have[i]=-1;}
}
MSXL_CODE void msx_pcm_close(msx_pcm_t *pcm) {
    msx_pcm_release(pcm);
    if (pcm->fd>=0) close(pcm->fd);
    pcm->fd=-1;
}
