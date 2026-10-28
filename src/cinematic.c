#include "cinematic.h"
#include "cinematic_codec.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
static FILE *movie;
static uint32_t *offsets;
static uint8_t *packet,*movie_cache;
static uint32_t rom;
static uint16_t *work_canvas,*heavy_pixels;
static int8_t *heavy_slot;
static unsigned heavy_count;
static surface_t canvas;
static msx_cinematic_stats_t stats;
static bool decode(unsigned index) {
    uint32_t began=TICKS_READ(),length=offsets[index+1]-offsets[index];
    const uint8_t *source;
    if (movie_cache) source=movie_cache+offsets[index];
    else {
        dma_wait();data_cache_hit_invalidate(packet,(length+15u)&~15u);dma_read(packet,rom+offsets[index],length);
        stats.read_bytes+=length;source=packet;
    }
    bool prepared=heavy_slot && heavy_slot[index]>=0;
    if (prepared) canvas.buffer=heavy_pixels+(size_t)heavy_slot[index]*stats.width*stats.height;
    else if (movie_cache) {
        if (canvas.buffer!=work_canvas) {memcpy(work_canvas,canvas.buffer,(size_t)stats.width*stats.height*2);canvas.buffer=work_canvas;}
        msx_cine_delta_apply_trusted(canvas.buffer,source+4);
    }
    else {
        if (msx_cine_crc32(source+4,length-4)!=msx_cine_be32(source)) return false;
        if (!msx_cine_delta(canvas.buffer,(size_t)stats.width*stats.height,source+4,length-4)) return false;
    }
    if (!prepared) data_cache_hit_writeback(canvas.buffer,(size_t)stats.width*stats.height*2);
    uint32_t elapsed=TICKS_READ()-began;stats.total_decode_ticks+=elapsed;
    if (elapsed>stats.max_decode_ticks) stats.max_decode_ticks=elapsed;
    stats.index=index;return true;
}
void msx_cinematic_close(void) {
    if (movie) fclose(movie);
    free(work_canvas);free(heavy_pixels);free(heavy_slot);work_canvas=heavy_pixels=NULL;heavy_slot=NULL;heavy_count=0;
    free(packet);free(movie_cache);free(offsets);movie=NULL;packet=movie_cache=NULL;offsets=NULL;rom=0;
    canvas=(surface_t){0};stats=(msx_cinematic_stats_t){0};
}
bool msx_cinematic_open(const char *path) {
    if (movie || !path || strncmp(path,"rom:/",5)) return false;
    movie=fopen(path,"rb");if (!movie) return false;
    uint8_t header[32];
    if (fread(header,1,sizeof(header),movie)!=sizeof(header) || memcmp(header,"MSXC",4) || msx_cine_be32(header+4)!=1) goto error;
    uint32_t w=msx_cine_be32(header+8),h=msx_cine_be32(header+12),count=msx_cine_be32(header+16),maximum=msx_cine_be32(header+20);
    if (!w || !h || w>320 || h>240 || !count || count>6000 || maximum<8 || maximum>512*1024 ||
        msx_cine_be32(header+24)!=59185606u || msx_cine_be32(header+28)!=1000000u) goto error;
    if (fseek(movie,0,SEEK_END)) goto error;
    long size=ftell(movie);if (size<0 || size>64*1024*1024) goto error;
    offsets=malloc((count+1)*sizeof(*offsets));
    if (!offsets || fseek(movie,32,SEEK_SET)) goto error;
    for (unsigned i=0;i<=count;i++) {
        uint8_t word[4];if (fread(word,1,4,movie)!=4) goto error;
        offsets[i]=msx_cine_be32(word);
        if ((offsets[i]&1u) || offsets[i]>(uint32_t)size || (i && (offsets[i]<=offsets[i-1] || offsets[i]-offsets[i-1]>maximum || offsets[i]-offsets[i-1]<8))) goto error;
    }
    if (offsets[0]!=32+4*(count+1) || offsets[count]!=(uint32_t)size) goto error;
    rom=PhysicalAddr((void *)(uintptr_t)dfs_rom_addr(path+4));
    if (!rom || dfs_rom_size(path+4)!=(uint32_t)size) goto error;
    bool preload=get_memory_size()>=8*1024*1024 && size<=2*1024*1024;
    if (preload) {
        unsigned allocation=((unsigned)size+63u)&~63u;movie_cache=memalign(64,allocation);
        if (!movie_cache) goto error;
        dma_wait();data_cache_hit_invalidate(movie_cache,allocation);dma_read(movie_cache,rom,(unsigned)size);
        for (unsigned i=0;i<count;i++) {
            const uint8_t *source=movie_cache+offsets[i];unsigned length=offsets[i+1]-offsets[i];
            if (msx_cine_crc32(source+4,length-4)!=msx_cine_be32(source) || !msx_cine_delta_validate((size_t)w*h,source+4,length-4)) goto error;
        }
    } else {packet=memalign(64,(maximum+63u)&~63u);if (!packet) goto error;}
    work_canvas=memalign(16,w*h*2);if (!work_canvas) goto error;
    memset(work_canvas,0,w*h*2);canvas=surface_make(work_canvas,FMT_RGBA16,w,h,w*2);
    if (preload) {
        heavy_slot=malloc(count);if (!heavy_slot) goto error;memset(heavy_slot,-1,count);
        unsigned limit=(2*1024*1024)/(w*h*2);if (limit>16) limit=16;
        for (unsigned slot=0;slot<limit;slot++) {
            unsigned selected=count,biggest=16384;
            for (unsigned i=0;i<count;i++) if (heavy_slot[i]<0 && offsets[i+1]-offsets[i]>biggest) {selected=i;biggest=offsets[i+1]-offsets[i];}
            if (selected==count) break;
            heavy_slot[selected]=(int8_t)slot;heavy_count++;
        }
        if (heavy_count) {
            heavy_pixels=memalign(16,(size_t)heavy_count*w*h*2);if (!heavy_pixels) goto error;
            for (unsigned i=0;i<count;i++) {
                msx_cine_delta_apply_trusted(work_canvas,movie_cache+offsets[i]+4);
                if (heavy_slot[i]>=0) memcpy(heavy_pixels+(size_t)heavy_slot[i]*w*h,work_canvas,(size_t)w*h*2);
            }
            data_cache_hit_writeback(heavy_pixels,(size_t)heavy_count*w*h*2);
            memset(work_canvas,0,w*h*2);
        }
    }
    stats=(msx_cinematic_stats_t){.frames=count,.max_packet=maximum,.width=w,.height=h,.read_bytes=preload?(uint32_t)size:0};
    if (!decode(0)) goto error;
    debugf("MSX CINEMATIC open frames=%lu size=%lux%lu packet=%lu\n",(unsigned long)count,(unsigned long)w,(unsigned long)h,(unsigned long)maximum);
    debugf("MSX CINEMATIC cache=%ld streaming=%u prepared=%u prepared_bytes=%lu\n",preload?size:0L,!preload,heavy_count,(unsigned long)heavy_count*w*h*2);
    return true;
error:
    msx_cinematic_close();return false;
}
bool msx_cinematic_next(void) {
    return movie && stats.index+1<stats.frames && decode(stats.index+1);
}
void msx_cinematic_draw(surface_t *target) {
    assert(movie && target);uint32_t began=TICKS_READ();
    rdpq_attach_clear(target,NULL);rdpq_set_mode_standard();rdpq_mode_filter(FILTER_POINT);rdpq_mode_dithering(DITHER_NONE_NONE);
    rdpq_tex_blit(&canvas,(320-stats.width)/2,(240-stats.height)/2,NULL);
    rdpq_detach_wait();display_show(target);
    uint32_t elapsed=TICKS_READ()-began;if (elapsed>stats.max_draw_ticks) stats.max_draw_ticks=elapsed;
}
msx_cinematic_stats_t msx_cinematic_stats(void) {return stats;}

uint32_t msx_cinematic_pixels_crc(void) {
    return movie?msx_cine_crc32(canvas.buffer,(size_t)stats.width*stats.height*2):0;
}
