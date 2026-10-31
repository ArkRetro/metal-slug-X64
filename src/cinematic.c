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
static enum { PREP_NONE,PREP_LOAD,PREP_CHECK,PREP_APPLY,PREP_DONE } prep;
static char prep_path[64];
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
    canvas=(surface_t){0};stats=(msx_cinematic_stats_t){0};prep=PREP_NONE;prep_path[0]=0;
}
/* Preparation of a preloaded movie (Expansion Pak): read it from the ROM,
 * validate every packet, then apply them all once to keep the heaviest
 * frames ready. It ran inside msx_cinematic_open, about 2 s of black between
 * the boot logo and the title in ares and longer on hardware (MiSTer report
 * "parones en la intro"). Now msx_cinematic_prepare starts it and
 * msx_cinematic_prepare_idle advances it in the logo's spare frame time;
 * msx_cinematic_open finishes whatever is left. */
static uint32_t prep_pos,prep_size;
static unsigned prep_index;
static volatile uint32_t vblank_at,vblank_period;
static bool vblank_hooked;
static void on_vblank(void) {
    uint32_t now=TICKS_READ(),d=now-vblank_at;
    if (vblank_at && d>TICKS_FROM_US(15000) && d<TICKS_FROM_US(18000)) vblank_period=d;
    vblank_at=now;
}
enum { PREP_DMA_BYTES=16*1024, PREP_CRC_BYTES=8*1024, PREP_MARGIN_US=1500 };
static uint32_t prep_crc,prep_crc_pos;
static uint32_t idle_left(void) {
    uint32_t v=vblank_at,period=vblank_period?vblank_period:TICKS_FROM_US(16683);
    if (!v) return 0;
    uint32_t since=TICKS_READ()-v,margin=TICKS_FROM_US(PREP_MARGIN_US);
    return since+margin>=period?0:period-since-margin;
}
static uint32_t apply_ticks_per_kb,check_ticks_per_kb;   // running mean per KB (0: first step unbounded)
static bool pi_busy(void) { return (*PI_STATUS & 3u)!=0; }
static bool begin(const char *path) {
    if (movie || !path || strncmp(path,"rom:/",5) || strlen(path)>=sizeof(prep_path)) return false;
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
    stats=(msx_cinematic_stats_t){.frames=count,.max_packet=maximum,.width=w,.height=h};
    work_canvas=memalign(16,w*h*2);if (!work_canvas) goto error;
    memset(work_canvas,0,w*h*2);canvas=surface_make(work_canvas,FMT_RGBA16,w,h,w*2);
    strcpy(prep_path,path);prep_size=(uint32_t)size;prep_pos=0;prep_index=0;prep_crc_pos=0;
    if (get_memory_size()>=8*1024*1024 && size<=4*1024*1024) {
        unsigned allocation=((unsigned)size+63u)&~63u;movie_cache=memalign(64,allocation);
        if (!movie_cache) goto error;
        data_cache_hit_invalidate(movie_cache,allocation);
        prep=PREP_LOAD;
    } else {
        packet=memalign(64,(maximum+63u)&~63u);if (!packet) goto error;
        prep=PREP_DONE;
    }
    if (!vblank_hooked) {register_VI_handler(on_vblank);vblank_hooked=true;}
    return true;
error:
    msx_cinematic_close();return false;
}
/* The heaviest frames get a prepared copy, within the same 6 MiB cinematic
 * budget as before (a 3 MiB cliff cut preparation to seven frames; a 2 MiB
 * cap left the entry's 8-16 KB packets, up to 13 ms to decode, unprepared and
 * dropped 14 of its frames), and never more than the heap has left minus
 * 256 KB: the game's own allocations share the Expansion Pak with it. */
static bool select_heavy(void) {
    unsigned w=stats.width,h=stats.height,count=stats.frames;
    heavy_slot=malloc(count);if (!heavy_slot) return false;
    memset(heavy_slot,-1,count);
    unsigned preparation_budget=6*1024*1024-prep_size;
    {heap_stats_t hs;sys_get_heap_stats(&hs);int room=hs.total-hs.used-256*1024;
     if ((int)preparation_budget>room) preparation_budget=room>0?(unsigned)room:0;}
    unsigned limit=preparation_budget/(w*h*2);if (limit>32) limit=32;
    for (unsigned slot=0;slot<limit;slot++) {
        unsigned selected=count,biggest=8192;
        for (unsigned i=0;i<count;i++) if (heavy_slot[i]<0 && offsets[i+1]-offsets[i]>biggest) {selected=i;biggest=offsets[i+1]-offsets[i];}
        if (selected==count) break;
        heavy_slot[selected]=(int8_t)slot;heavy_count++;
    }
    if (heavy_count) {heavy_pixels=memalign(16,(size_t)heavy_count*w*h*2);if (!heavy_pixels) return false;}
    return true;
}
/* One bounded step. budget: ticks it may take (0 = no limit). Returns 1 when
 * it did work, 0 when the next step does not fit the budget or nothing is
 * left, -1 on a bad movie. */
static int prep_step(uint32_t budget) {
    switch (prep) {
    case PREP_LOAD: {
        if (pi_busy()) return budget?0:(dma_wait(),1);
        uint32_t n=prep_size-prep_pos;if (n>PREP_DMA_BYTES) n=PREP_DMA_BYTES;
        dma_read_async(movie_cache+prep_pos,rom+prep_pos,n);prep_pos+=n;
        if (prep_pos>=prep_size) {dma_wait();prep=PREP_CHECK;prep_index=0;}
        return 1;
    }
    case PREP_CHECK: {
        // CRC in pieces of PREP_CRC_BYTES (the key frames are ~110 KB), then
        // the packet's structure (one pass over its run words).
        const uint8_t *source=movie_cache+offsets[prep_index];unsigned length=offsets[prep_index+1]-offsets[prep_index];
        if (budget && check_ticks_per_kb*(PREP_CRC_BYTES>>10)>budget) return 0;
        uint32_t t0=TICKS_READ(),n=length-4-prep_crc_pos;if (n>PREP_CRC_BYTES) n=PREP_CRC_BYTES;
        if (!prep_crc_pos) prep_crc=~0u;
        prep_crc=msx_cine_crc32_update(prep_crc,source+4+prep_crc_pos,n);prep_crc_pos+=n;
        check_ticks_per_kb=(check_ticks_per_kb*3+(TICKS_READ()-t0)/((n>>10)+1))/4;
        if (prep_crc_pos<length-4) return 1;
        prep_crc_pos=0;
        if (~prep_crc!=msx_cine_be32(source) || !msx_cine_delta_validate((size_t)stats.width*stats.height,source+4,length-4)) return -1;
        if (++prep_index==stats.frames) {
            if (!select_heavy()) return -1;
            prep_index=0;prep=heavy_count?PREP_APPLY:PREP_DONE;
        }
        return 1;
    }
    case PREP_APPLY: {
        unsigned length=offsets[prep_index+1]-offsets[prep_index];
        if (budget && apply_ticks_per_kb*((length>>10)+1)>budget) return 0;
        uint32_t t0=TICKS_READ();
        msx_cine_delta_apply_trusted(work_canvas,movie_cache+offsets[prep_index]+4);
        unsigned plane=stats.width*stats.height;
        if (heavy_slot[prep_index]>=0) memcpy(heavy_pixels+(size_t)heavy_slot[prep_index]*plane,work_canvas,(size_t)plane*2);
        apply_ticks_per_kb=(apply_ticks_per_kb*3+(TICKS_READ()-t0)/((length>>10)+1))/4;
        if (++prep_index==stats.frames) {
            data_cache_hit_writeback(heavy_pixels,(size_t)heavy_count*plane*2);
            memset(work_canvas,0,(size_t)plane*2);prep=PREP_DONE;
        }
        return 1;
    }
    default: return 0;
    }
}
bool msx_cinematic_prepare(const char *path) {return begin(path);}
bool msx_cinematic_prepare_idle(void) {
    if (!movie || prep==PREP_DONE || prep==PREP_NONE) return false;
    uint32_t left=idle_left();
    if (!left) return false;
    int r=prep_step(left);
    if (r<0) {debugf("MSX CINEMATIC bad packet while preparing %s\n",prep_path);msx_cinematic_close();return false;}
    return r>0;
}
bool msx_cinematic_open(const char *path) {
    if (movie && strcmp(prep_path,path)) return false;
    uint32_t began=TICKS_READ();unsigned left_index=prep==PREP_APPLY?prep_index:0;int left_state=prep;
    if (!movie && !begin(path)) return false;
    for (int r;prep!=PREP_DONE;) if ((r=prep_step(0))<0) goto error;
    stats.read_bytes=movie_cache?prep_size:0;
    if (!decode(0)) goto error;
    debugf("MSX CINEMATIC open frames=%lu size=%lux%lu packet=%lu finish_ms=%lu left_state=%d left_index=%u\n",(unsigned long)stats.frames,(unsigned long)stats.width,(unsigned long)stats.height,(unsigned long)stats.max_packet,
        (unsigned long)TICKS_TO_MS(TICKS_READ()-began),left_state,left_index);
    debugf("MSX CINEMATIC cache=%ld streaming=%u prepared=%u prepared_bytes=%lu\n",movie_cache?(long)prep_size:0L,!movie_cache,heavy_count,(unsigned long)heavy_count*stats.width*stats.height*2);
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
