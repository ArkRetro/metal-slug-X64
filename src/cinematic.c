#include "cinematic.h"
#include "cinematic_codec.h"
#include "cinematic_stream.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
/* Streaming player (docs/RENDIMIENTO.md, "Cinemáticas sin parones"). Each
 * movie slot runs two background stages in the spare time before each
 * vertical blank, every step bounded by the time left:
 *  - loader: packets read from the ROM by asynchronous PI DMA into a ring of
 *    v1 packets, LZ4-decompressed in place (MSXC v2, tools/msxc_compress.py),
 *    CRC-checked and validated, all in pieces;
 *  - producer: whole frames decoded ahead into a pool of canvases: the RDP
 *    copies the previous frame (or clears, for the first) into a free canvas
 *    and the CPU applies the frame's packet to it, in pieces.
 * Showing a frame only picks its canvas: no ROM read, LZ4, CRC or packet
 * apply runs in the frame's own time unless the producer fell behind (then
 * the frame is produced at once, as the old player always did).
 * Two slots: the movie playing and the next one, prepared during it (the
 * boot runs logo, title, HOW TO PLAY, SOLDIER SELECT, the choice and the
 * entry back to back). The pixels are exactly msx_cine_delta's. */
enum { QN=64, NBUF_MAX=8, NBUF_MIN=3, DMA_CHUNK=8192, MIN_PIECE=256 };
#define INPLACE_EXTRA(maximum) (((maximum)>>8)+80u)   // LZ4 in-place margin (libdragon lz4_dec_internal.h) + alignment
#define RING_PLAY_CAP (2u*1024*1024)   // ring beside another movie or grown at open (HOW TO PLAY's 650 KB fade at 2638 loads ahead)
#define RING_ALONE_CAP (4u*1024*1024)  // with no other movie (the title under the logo: all of it)
#define HEAP_KEEP (384*1024)           // left to the movie playing, the mixer and the logo
#define GROW_LEAVE (1536*1024)         // a playing slot grows only leaving this for the next movie
enum { L_TABLE,L_ROOM,L_DMA,L_LZ4,L_CRC,L_VALID,L_DONE };
enum { P_START,P_WAIT,P_APPLY,P_CRC };
enum { STEP_IDLE=0,STEP_WORK=1,STEP_WAIT=2,STEP_ERROR=-1 };
typedef struct {
    bool active,failed,opened;
    char path[64];
    uint32_t rom,file_size,raw_size,maximum,extra,ring_full;
    unsigned count,w,h;bool v2;
    uint32_t *tables,*off,*st;   // raw (v1) offsets, then stored offsets (v2): big-endian words, the CPU's order
    uint8_t **pkt;               // ring address of each loaded packet still needed: [prod,load)
    uint8_t *ring,*ring_old;uint32_t ring_size,ring_old_size;unsigned old_until;   // packets < old_until are in ring_old
    // loader
    int ls;unsigned load;uint8_t *room,*dma_dst;uint32_t len,have,at,dma_pos,dma_len,crc,crc_pos;
    msx_lz4r_t z;msx_cine_walk_t walk;
    // producer
    int ps;unsigned prod;int pbuf;volatile bool copy_done;msx_cine_apply_t run;size_t lo,hi;
    uint16_t *buf[NBUF_MAX];unsigned nbuf,nbuf_best;uint8_t q[QN];   // q[f%QN]: canvas of produced frame f
    unsigned chk,heavy;uint32_t chk_full,chk_prev;   // tables checked in pieces: next entry, packets > 16 KB, ring bytes, last stored offset
    unsigned index;   // frame shown (playing slot)
    /* CRC of the first and last frames, taken in the spare time after they
     * are produced (the entry/title checks and the logs read them at a cut,
     * where a 136 KB CRC was a 20 ms pause). */
    uint32_t crc_first,crc_last,crc_run;bool crc_first_ok,crc_last_ok;int crc_buf;unsigned crc_frame;uint32_t crc_at;
    surface_t canvas;msx_cinematic_stats_t stats;
} slot_t;
static slot_t slots[2];
static slot_t *playing;
static msx_cine_crc_tables_t crc_tables;static bool crc_ready;
/* Cost of each kind of piece, in ticks per KB: a running estimate that
 * rises at once and falls slowly. Work runs in small pieces until the
 * deadline (the next blank minus a margin) would be passed by the next one:
 * an estimate off by any factor overruns by one piece at most. */
static uint32_t cost_lz4,cost_crc,cost_valid,cost_apply,cost_check;   // set with the CRC tables
enum { PIECE_LZ4=2048,PIECE_CRC=4096,PIECE_VALID=4096,PIECE_APPLY=2048,PIECE_CHECK=256 };   // PIECE_CHECK: table entries (costed as 8 bytes each)
enum { ALLOCATE_US=700 };   // ring + canvases + heap statistics once the tables are checked
static void cost_learn(uint32_t *cost,uint32_t ticks,size_t bytes) {
    if (bytes<512) return;
    uint32_t per=(uint32_t)((uint64_t)ticks*1024u/bytes);
    *cost=per>*cost?(*cost+per)/2u:(*cost*7u+per)/8u;
}
/* Whether a piece of `bytes` fits before `deadline` (0: no deadline). */
static bool fits(uint32_t deadline,uint32_t cost,size_t bytes) {
    if (!deadline) return true;
    int32_t left=(int32_t)(deadline-TICKS_READ());
    return left>0 && (uint64_t)left>(uint64_t)cost*bytes/1024u+TICKS_FROM_US(30);
}
static volatile uint32_t vblank_at,vblank_period;
static bool vblank_hooked;
static void on_vblank(void) {
    uint32_t now=TICKS_READ(),d=now-vblank_at;
    if (vblank_at && d>TICKS_FROM_US(15000) && d<TICKS_FROM_US(18000)) vblank_period=d;
    vblank_at=now;
}
enum { PREP_MARGIN_US=1500 };
static uint32_t idle_left(void) {
    uint32_t v=vblank_at,period=vblank_period?vblank_period:TICKS_FROM_US(16683);
    if (!v) return 0;
    uint32_t since=TICKS_READ()-v,margin=TICKS_FROM_US(PREP_MARGIN_US);
    return since+margin>=period?0:period-since-margin;
}
static bool pi_busy(void) { return (*PI_STATUS & 3u)!=0; }
static int heap_free(void) { heap_stats_t h;sys_get_heap_stats(&h);return h.total-h.used; }
static uint32_t frame_bytes(const slot_t *s) { return s->w*s->h*2u; }
static uint32_t a16(uint32_t n) { return (n+15u)&~15u; }

static void close_slot(slot_t *s) {
    if (!s->active) return;
    if (s->ps==P_WAIT) {rspq_flush();while (!s->copy_done) {}}
    if (s->ls==L_DMA || s->ls==L_TABLE) dma_wait();
    for (unsigned i=0;i<s->nbuf;i++) free(s->buf[i]);
    free(s->ring);free(s->ring_old);free(s->pkt);free(s->tables);
    if (playing==s) playing=NULL;
    *s=(slot_t){.ps=P_START,.pbuf=-1,.crc_buf=-1};
}
static void dma_begin(slot_t *s,uint8_t *dst,uint32_t at,uint32_t len) {
    s->dma_dst=dst;s->at=at;s->dma_len=len;s->dma_pos=0;
    data_cache_hit_invalidate((void *)((uintptr_t)dst&~(uintptr_t)15),a16((uint32_t)((uintptr_t)dst&15u)+len));
}
/* One DMA chunk: STEP_WORK when one was issued, STEP_WAIT while the PI is
 * busy (ours or the audio's), STEP_IDLE when the transfer is complete. */
static int dma_step(slot_t *s) {
    if (pi_busy()) return STEP_WAIT;
    if (s->dma_pos>=s->dma_len) return STEP_IDLE;
    uint32_t n=s->dma_len-s->dma_pos;if (n>DMA_CHUNK) n=DMA_CHUNK;
    dma_read_async(s->dma_dst+s->dma_pos,s->rom+s->at+s->dma_pos,n);s->dma_pos+=n;
    return STEP_WORK;
}
static bool begin(slot_t *s,const char *path) {
    if (!path || strncmp(path,"rom:/",5) || strlen(path)>=sizeof(s->path)) return false;
    uint32_t addr=dfs_rom_addr(path+4);if (!addr) return false;
    *s=(slot_t){.active=true,.ps=P_START,.pbuf=-1,.ls=L_DONE,.crc_buf=-1};strcpy(s->path,path);
    s->rom=PhysicalAddr((void *)(uintptr_t)addr);s->file_size=dfs_rom_size(path+4);
    static uint8_t header[32] __attribute__((aligned(16)));
    data_cache_hit_invalidate(header,sizeof(header));dma_read(header,s->rom,sizeof(header));
    uint32_t version=msx_cine_be32(header+4);
    if (s->file_size<32 || memcmp(header,"MSXC",4) || (version!=1 && version!=2)) goto error;
    s->v2=version==2;
    s->w=msx_cine_be32(header+8);s->h=msx_cine_be32(header+12);s->count=msx_cine_be32(header+16);s->maximum=msx_cine_be32(header+20);
    if (!s->w || !s->h || s->w>320 || s->h>240 || !s->count || s->count>6000 || s->maximum<8 || s->maximum>512*1024 ||
        msx_cine_be32(header+24)!=59185606u || msx_cine_be32(header+28)!=1000000u || s->file_size>64u*1024*1024) goto error;
    uint32_t words=(s->count+1)*(s->v2?2u:1u);
    if (32+4*words>s->file_size) goto error;
    s->tables=memalign(16,a16(4*words));if (!s->tables) goto error;
    s->off=s->tables;s->st=s->v2?s->tables+s->count+1:NULL;
    s->extra=s->v2?INPLACE_EXTRA(s->maximum):0;
    s->stats=(msx_cinematic_stats_t){.frames=s->count,.max_packet=s->maximum,.width=(uint16_t)s->w,.height=(uint16_t)s->h};
    dma_begin(s,(uint8_t *)s->tables,32,4*words);s->ls=L_TABLE;
    if (!crc_ready) {
        msx_cine_crc_tables_init(&crc_tables);crc_ready=true;
        cost_lz4=TICKS_FROM_US(300);cost_crc=TICKS_FROM_US(120);cost_valid=TICKS_FROM_US(60);cost_apply=TICKS_FROM_US(200);cost_check=TICKS_FROM_US(400);
    }
    if (!vblank_hooked) {register_VI_handler(on_vblank);vblank_hooked=true;}
    return true;
error:
    close_slot(s);return false;
}
/* The tables are in: check them as the v1/v2 reader always did, entries
 * [s->chk, s->chk+n) at a time (HOW TO PLAY's 2691 entries and its ring took
 * 3.4 ms in one step, past a title frame's blank in test ROM 21): -1 bad,
 * 1 all checked, 0 more to do. allocate() then takes the ring and the
 * canvases this slot can have now. */
static int check_part(slot_t *s,unsigned n) {
    uint32_t count=s->count;unsigned end=s->chk+n>count+1?count+1:s->chk+n;
    for (unsigned i=s->chk;i<end;i++) {
        uint32_t o=s->off[i];
        if ((o&1u) || (i && (o<=s->off[i-1] || o-s->off[i-1]>s->maximum || o-s->off[i-1]<8))) return -1;
        if (i) {s->chk_full+=a16(o-s->off[i-1]);s->heavy+=o-s->off[i-1]>16384;}
        if (s->v2) {
            uint32_t value=s->st[i],at=value&~1u;
            if (!i && at!=32+8*(count+1)) return -1;
            if (i==count && value!=s->file_size) return -1;
            if (at>s->file_size || (i && (at<=s->chk_prev || at-s->chk_prev>o-s->off[i-1]+1))) return -1;
            s->chk_prev=at;
        }
    }
    s->chk=end;
    if (end<=count) return 0;
    if (!s->v2 && s->off[count]!=s->file_size) return -1;
    if (s->off[0]!=32+4*(count+1)) return -1;
    return 1;
}
static bool allocate(slot_t *s,bool alone) {
    uint32_t count=s->count;
    s->raw_size=s->off[count]-s->off[0];s->ring_full=s->chk_full+s->extra+64u;
    s->pkt=malloc(count*sizeof(*s->pkt));if (!s->pkt) return false;
    int avail=heap_free()-HEAP_KEEP;
    uint32_t frame=frame_bytes(s),minimum=a16(s->maximum)+s->extra+64u,cap=alone?RING_ALONE_CAP:RING_PLAY_CAP;
    uint32_t want=s->ring_full<cap?s->ring_full:cap;if (want<minimum) want=minimum;
    /* Canvases: the frames decoded ahead absorb the heavy ones (> 16 KB
     * packets); a light movie (the choice: one) needs few, and the heap it
     * leaves takes the game's own loading meanwhile. */
    unsigned heavy=s->heavy,best=heavy+3<4?4:heavy+3>NBUF_MAX?NBUF_MAX:heavy+3;
    const unsigned tries[]={best,best<4?best:4,NBUF_MIN};s->nbuf_best=best;
    for (unsigned t=0;t<3 && !s->ring;t++) {
        int room=avail-(int)(tries[t]*frame);
        uint32_t ring=(room<(int)want?(room>0?(uint32_t)room:0u):want)&~63u;
        while (ring>=minimum && !(s->ring=memalign(64,ring))) ring=ring/2>minimum?(ring/2)&~63u:(ring>minimum?minimum:0);
        if (s->ring) {s->ring_size=ring;s->nbuf=tries[t];}
    }
    if (!s->ring) return false;
    unsigned want_buffers=s->nbuf;
    for (s->nbuf=0;s->nbuf<want_buffers;s->nbuf++) if (!(s->buf[s->nbuf]=memalign(64,frame))) break;
    return s->nbuf>=NBUF_MIN;
}
/* A playing slot prepared beside another movie gets the canvases and ring
 * it could not have then (the old ring drains as its packets are used). */
static void grow(slot_t *s) {
    if (s->failed || s->ls==L_TABLE || !s->ring) return;
    uint32_t frame=frame_bytes(s);
    for (;s->nbuf<s->nbuf_best && heap_free()-(int)frame>HEAP_KEEP+GROW_LEAVE;s->nbuf++) if (!(s->buf[s->nbuf]=memalign(64,frame))) break;
    uint32_t want=s->ring_full<RING_PLAY_CAP?s->ring_full:RING_PLAY_CAP;
    if (s->ring_old || s->ls==L_DONE || want<=s->ring_size+256u*1024) return;
    int room=heap_free()-HEAP_KEEP-GROW_LEAVE;
    if (room<(int)(s->ring_size+256u*1024)) return;
    if ((uint32_t)room<want) want=(uint32_t)room&~63u;
    uint8_t *ring=memalign(64,want);if (!ring) return;
    unsigned until=s->load+(s->ls>L_ROOM?1u:0u);   // a packet being loaded stays in the old ring
    if (until>s->prod) {s->ring_old=s->ring;s->ring_old_size=s->ring_size;s->old_until=until;}
    else free(s->ring);
    s->ring=ring;s->ring_size=want;
    debugf("MSX CINEMATIC %s ring grown to %lu KB, %u canvases\n",s->path,(unsigned long)want/1024,s->nbuf);
}
/* Room for the next packet (and its in-place margin) in the current ring. */
static uint8_t *ring_room(slot_t *s,uint32_t need) {
    uint8_t *r=s->ring;uint32_t size=s->ring_size;
    unsigned first=s->prod>s->old_until?s->prod:s->old_until;
    if (first>=s->load) return need<=size?r:NULL;
    unsigned last=s->load-1;
    uint32_t tail=(uint32_t)(s->pkt[first]-r),head=(uint32_t)(s->pkt[last]-r)+a16(s->off[last+1]-s->off[last]);
    if (s->pkt[last]>=s->pkt[first]) {
        if (head+need<=size) return r+head;
        return need<=tail?r:NULL;
    }
    return head+need<=tail?r+head:NULL;
}
static int load_step(slot_t *s,uint32_t deadline) {
    switch (s->ls) {
    case L_TABLE: {
        int r=dma_step(s);if (r!=STEP_IDLE) return r;
        /* The tables, then the heap, in pieces that fit before the deadline
         * (ensure() passes none and does it all at once). */
        bool did=false;
        while (s->chk<=s->count) {
            if (!fits(deadline,cost_check,PIECE_CHECK*8u)) return did?STEP_WORK:STEP_IDLE;
            uint32_t t0=TICKS_READ();unsigned at=s->chk;
            int c=check_part(s,PIECE_CHECK);did=true;
            if (c<0) {debugf("MSX CINEMATIC %s: bad tables\n",s->path);return STEP_ERROR;}
            cost_learn(&cost_check,TICKS_READ()-t0,(size_t)(s->chk-at)*8u);
        }
        if (deadline && (int32_t)(deadline-TICKS_READ())<(int32_t)TICKS_FROM_US(ALLOCATE_US)) return did?STEP_WORK:STEP_IDLE;
        bool alone=true;
        for (unsigned i=0;i<2;i++) if (&slots[i]!=s && slots[i].active) alone=false;
        if (!allocate(s,alone)) {
            debugf("MSX CINEMATIC cannot prepare %s (heap free %d)\n",s->path,heap_free());
            return STEP_ERROR;
        }
        s->ls=L_ROOM;return STEP_WORK;
    }
    case L_ROOM: {
        if (s->load>=s->count) {s->ls=L_DONE;return STEP_IDLE;}
        unsigned k=s->load;
        s->len=s->off[k+1]-s->off[k];s->have=s->len;s->at=s->off[k];
        if (s->v2) {
            uint32_t first=s->st[k],next=s->st[k+1];s->at=first&~1u;
            if ((next&~1u)<=s->at) return STEP_ERROR;
            s->have=(next&~1u)-s->at-(first&1u);
            if (s->have>s->len || s->at+s->have>s->file_size) return STEP_ERROR;
        }
        uint8_t *room=ring_room(s,a16(s->len)+s->extra);
        if (!room) return STEP_IDLE;
        s->room=room;
        /* LZ4 in place: the stored bytes go at the end of the packet's room,
         * at least the in-place margin past the packet's end. */
        uint8_t *dst=room;
        if (s->have<s->len) dst=(uint8_t *)(((uintptr_t)room+s->len+(s->have>>8)+48u-s->have)&~(uintptr_t)15);
        dma_begin(s,dst,s->at,s->have);s->ls=L_DMA;
        return STEP_WORK;
    }
    case L_DMA: {
        int r=dma_step(s);if (r!=STEP_IDLE) return r;
        s->crc=~0u;s->crc_pos=4;
        if (s->have<s->len) {msx_lz4r_init(&s->z,s->dma_dst,s->have,s->room,s->len);s->ls=L_LZ4;}
        else s->ls=L_CRC;
        return STEP_WORK;
    }
    case L_LZ4: {   // decompressed and CRC-checked a piece at a time (the bytes still in the cache)
        bool did=false;
        for (;;) {
            if (!fits(deadline,cost_lz4,PIECE_LZ4)) return did?STEP_WORK:STEP_IDLE;
            uint32_t t0=TICKS_READ();uint8_t *before=s->z.out;
            int r=msx_lz4r_run(&s->z,PIECE_LZ4);did=true;
            if (r<0) {debugf("MSX CINEMATIC %s packet %u: bad LZ4 block\n",s->path,s->load);return STEP_ERROR;}
            uint32_t done=(uint32_t)(s->z.out-s->room);
            if (done>s->crc_pos) {s->crc=msx_cine_crc32_fast(&crc_tables,s->crc,s->room+s->crc_pos,done-s->crc_pos);s->crc_pos=done;}
            cost_learn(&cost_lz4,TICKS_READ()-t0,(size_t)(s->z.out-before));
            if (r==1) {s->walk=(msx_cine_walk_t){0};s->ls=L_VALID;return STEP_WORK;}
        }
    }
    case L_CRC: {
        bool did=false;
        while (s->crc_pos<s->len) {
            uint32_t n=s->len-s->crc_pos;if (n>PIECE_CRC) n=PIECE_CRC;
            if (!fits(deadline,cost_crc,n)) return did?STEP_WORK:STEP_IDLE;
            uint32_t t0=TICKS_READ();
            s->crc=msx_cine_crc32_fast(&crc_tables,s->crc,s->room+s->crc_pos,n);s->crc_pos+=n;did=true;
            cost_learn(&cost_crc,TICKS_READ()-t0,n);
        }
        s->walk=(msx_cine_walk_t){0};s->ls=L_VALID;
        return STEP_WORK;
    }
    case L_VALID: {
        int r=0;bool did=false;
        while (!r) {
            if (!fits(deadline,cost_valid,PIECE_VALID)) return did?STEP_WORK:STEP_IDLE;
            uint32_t t0=TICKS_READ();size_t at=s->walk.offset;
            r=msx_cine_validate_part(&s->walk,(size_t)s->w*s->h,s->room+4,s->len-4,PIECE_VALID);did=true;
            cost_learn(&cost_valid,TICKS_READ()-t0,s->walk.offset-at);
        }
        if (r<0) {debugf("MSX CINEMATIC %s packet %u: bad structure\n",s->path,s->load);return STEP_ERROR;}
        if (~s->crc!=msx_cine_be32(s->room)) {debugf("MSX CINEMATIC %s packet %u: CRC mismatch\n",s->path,s->load);return STEP_ERROR;}
        s->pkt[s->load++]=s->room;s->stats.read_bytes+=s->len;
        s->ls=s->load<s->count?L_ROOM:L_DONE;
        return STEP_WORK;
    }
    default: return STEP_IDLE;
    }
}
static void copy_finished(void *arg) { *(volatile bool *)arg=true; }
static surface_t surface_of(const slot_t *s,unsigned b) { return surface_make(s->buf[b],FMT_RGBA16,s->w,s->h,s->w*2); }
static int free_canvas(const slot_t *s) {
    for (unsigned b=0;b<s->nbuf;b++) {
        bool used=(int)b==s->pbuf || (int)b==s->crc_buf;
        for (unsigned f=s->index;f<s->prod && !used;f++) used=s->q[f%QN]==b;
        if (!used) return (int)b;
    }
    return -1;
}
static int produce_step(slot_t *s,uint32_t deadline) {
    switch (s->ps) {
    case P_START: {
        unsigned k=s->prod;
        if (s->ls==L_TABLE || k>=s->count || k>=s->load || k-s->index>=QN-1) return STEP_IDLE;
        uint32_t len=s->off[k+1]-s->off[k];
        if (k && len==8) {s->q[k%QN]=s->q[(k-1)%QN];s->prod++;return STEP_WORK;}   // no change (CRC + end mark): same canvas
        int b=free_canvas(s);if (b<0) return STEP_IDLE;
        if (!fits(deadline,TICKS_FROM_US(600),1024)) return STEP_IDLE;   // cache invalidate + RDP commands
        surface_t target=surface_of(s,(unsigned)b);
        data_cache_hit_invalidate(s->buf[b],frame_bytes(s));   // the RDP writes it: no stale line may be written back over it
        s->copy_done=false;s->pbuf=b;
        rdpq_attach(&target,NULL);
        if (!k) {rdpq_set_mode_fill(RGBA32(0,0,0,0));rdpq_fill_rectangle(0,0,s->w,s->h);}
        else {surface_t source=surface_of(s,s->q[(k-1)%QN]);rdpq_set_mode_copy(false);rdpq_tex_blit(&source,0,0,NULL);}
        rdpq_detach_cb(copy_finished,(void *)&s->copy_done);rspq_flush();
        s->ps=P_WAIT;return STEP_WORK;
    }
    case P_WAIT:
        if (!s->copy_done) return STEP_WAIT;
        s->run=(msx_cine_apply_t){0};s->lo=(size_t)-1;s->hi=0;s->ps=P_APPLY;
        /* fall through */
    case P_APPLY: {
        unsigned k=s->prod;bool done=false,did=false;
        while (!done) {
            if (!fits(deadline,cost_apply,PIECE_APPLY)) return did?STEP_WORK:STEP_IDLE;
            uint32_t t0=TICKS_READ();size_t at=s->run.done;
            done=msx_cine_apply_part(&s->run,s->buf[s->pbuf],s->pkt[k]+4,PIECE_APPLY,&s->lo,&s->hi);did=true;
            cost_learn(&cost_apply,TICKS_READ()-t0,s->run.done-at);
        }
        if (s->hi>s->lo) {
            uintptr_t a=((uintptr_t)(s->buf[s->pbuf]+s->lo))&~(uintptr_t)15,e=((uintptr_t)(s->buf[s->pbuf]+s->hi)+15u)&~(uintptr_t)15;
            data_cache_hit_writeback((void *)a,(unsigned long)(e-a));
        }
        s->q[k%QN]=(uint8_t)s->pbuf;s->prod++;s->ps=P_START;
        if (!k || k+1==s->count) {s->crc_buf=s->pbuf;s->crc_frame=k;s->crc_at=0;s->crc_run=~0u;s->ps=P_CRC;}
        s->pbuf=-1;
        if (s->ring_old && s->prod>=s->old_until) {free(s->ring_old);s->ring_old=NULL;}
        return STEP_WORK;
    }
    case P_CRC: {
        uint32_t total=frame_bytes(s);const uint8_t *pixels=(const uint8_t *)s->buf[s->crc_buf];bool did=false;
        while (s->crc_at<total) {
            uint32_t n=total-s->crc_at;if (n>PIECE_CRC) n=PIECE_CRC;
            if (!fits(deadline,cost_crc,n)) return did?STEP_WORK:STEP_IDLE;
            uint32_t t0=TICKS_READ();
            s->crc_run=msx_cine_crc32_fast(&crc_tables,s->crc_run,pixels+s->crc_at,n);s->crc_at+=n;did=true;
            cost_learn(&cost_crc,TICKS_READ()-t0,n);
        }
        if (!s->crc_frame) {s->crc_first=~s->crc_run;s->crc_first_ok=true;}
        if (s->crc_frame+1==s->count) {s->crc_last=~s->crc_run;s->crc_last_ok=true;}
        s->crc_buf=-1;s->ps=P_START;
        return STEP_WORK;
    }
    }
    return STEP_IDLE;
}
/* One step of a slot: the producer first while it can, then the loader. */
static int slot_step(slot_t *s,uint32_t deadline) {
    if (!s->active || s->failed) return STEP_IDLE;
    int r=produce_step(s,deadline);
    if (r==STEP_IDLE) r=load_step(s,deadline);
    else if (r==STEP_WAIT) {int l=load_step(s,deadline);if (l!=STEP_IDLE) r=l;}
    if (r==STEP_ERROR) {debugf("MSX CINEMATIC bad movie %s (loaded %u produced %u)\n",s->path,s->load,s->prod);s->failed=true;return STEP_IDLE;}
    return r;
}
/* Frame f of a slot produced now (no time limit: waits on the DMA/RDP). */
static bool ensure(slot_t *s,unsigned f) {
    while (s->prod<=f) {
        if (s->failed || !s->active) return false;
        int r=slot_step(s,0);
        if (r==STEP_IDLE) {
            if (!s->failed) debugf("MSX CINEMATIC %s stalled at frame %u (loaded %u)\n",s->path,s->prod,s->load);
            return false;
        }
    }
    return true;
}
static slot_t *find(const char *path) {
    for (unsigned i=0;i<2;i++) if (slots[i].active && !strcmp(slots[i].path,path)) return &slots[i];
    return NULL;
}
static slot_t *spare(void) {
    for (unsigned i=0;i<2;i++) if (!slots[i].active) return &slots[i];
    return NULL;
}
void msx_cinematic_close(void) {if (playing) close_slot(playing);playing=NULL;}
/* Drops a movie prepared beside the playing one (SOLDIER SELECT: the choice
 * of another character changes the movie that follows). */
void msx_cinematic_cancel(const char *path) {
    slot_t *s=path?find(path):NULL;
    if (s && s!=playing) close_slot(s);
}
bool msx_cinematic_prepare(const char *path) {
    if (!path) return false;
    slot_t *s=find(path);if (s) return !s->failed;
    s=spare();if (!s) return false;
    return begin(s,path);
}
int msx_cinematic_heap_bytes(void) {
    int bytes=0;
    for (unsigned i=0;i<2;i++) {
        const slot_t *s=&slots[i];if (!s->active) continue;
        bytes+=(int)(s->ring_size+(s->ring_old?s->ring_old_size:0)+s->nbuf*frame_bytes(s)+(s->count+1)*4u*(s->v2?2u:1u)+(s->pkt?s->count*4u:0u));
    }
    return bytes;
}
int msx_cinematic_heap_reserve(void) {
    int need=HEAP_KEEP;
    for (unsigned i=0;i<2;i++) if (slots[i].active && !slots[i].failed && slots[i].ls==L_TABLE) need+=2*1024*1024;
    return need;
}
uint32_t msx_cinematic_idle_deadline(void) {
    if (!vblank_hooked) {register_VI_handler(on_vblank);vblank_hooked=true;}
    uint32_t left=idle_left();
    if (left<TICKS_FROM_US(300)) return 0;
    uint32_t deadline=TICKS_READ()+left;return deadline?deadline:1;
}
bool msx_cinematic_prepare_idle(void) {
    uint32_t left=idle_left();
    if (left<TICKS_FROM_US(100)) return false;
    uint32_t deadline=TICKS_READ()+left;if (!deadline) deadline=1;
    slot_t *order[2]={playing,NULL};unsigned n=playing?1:0;
    for (unsigned i=0;i<2;i++) if (slots[i].active && &slots[i]!=playing) order[n++]=&slots[i];
    bool waiting=false;
    for (unsigned i=0;i<n;i++) {
        int ls0=order[i]->ls,ps0=order[i]->ps;
        int r=slot_step(order[i],deadline);
        int32_t over=(int32_t)(TICKS_READ()-deadline);
        if (over>(int32_t)TICKS_FROM_US(500)) debugf("MSX CINEMATIC %s step past the blank margin by %ld us (loader %d producer %d)\n",order[i]->path,(long)TICKS_TO_US(over),ls0,ps0);
        if (r==STEP_WORK) return true;
        if (r==STEP_WAIT) waiting=true;
    }
    return waiting;   // DMA or RDP busy: poll again while the frame has time
}
bool msx_cinematic_open(const char *path) {
    if (playing) return false;
    uint32_t began=TICKS_READ();
    slot_t *s=find(path);
    if (s && s->failed) {close_slot(s);s=NULL;}
    unsigned loaded=s?s->load:0,produced=s?s->prod:0;
    if (!s) {s=spare();if (!s || !begin(s,path)) return false;}
    playing=s;grow(s);
    if (!ensure(s,0)) {close_slot(s);return false;}
    grow(s);   // when the tables were read only now
    s->opened=true;s->index=0;
    s->canvas=surface_of(s,s->q[0]);
    debugf("MSX CINEMATIC open %s frames=%lu size=%lux%lu packet=%lu finish_ms=%lu loaded_before=%u produced_before=%u\n",path,(unsigned long)s->count,(unsigned long)s->w,(unsigned long)s->h,(unsigned long)s->maximum,
        (unsigned long)TICKS_TO_MS(TICKS_READ()-began),loaded,produced);
    debugf("MSX CINEMATIC ring=%lu KB of %lu KB raw, canvases=%u\n",(unsigned long)s->ring_size/1024,(unsigned long)s->raw_size/1024,s->nbuf);
    return true;
}
bool msx_cinematic_next(void) {
    slot_t *s=playing;
    if (!s || !s->opened || s->index+1>=s->count) return false;
    uint32_t began=TICKS_READ();
    if (!ensure(s,s->index+1)) return false;
    s->index++;s->canvas.buffer=s->buf[s->q[s->index%QN]];s->stats.index=s->index;
    uint32_t elapsed=TICKS_READ()-began;s->stats.total_decode_ticks+=elapsed;
    if (elapsed>s->stats.max_decode_ticks) s->stats.max_decode_ticks=elapsed;
    return true;
}
void msx_cinematic_draw(surface_t *target) {
    slot_t *s=playing;
    assert(s && s->opened && target);uint32_t began=TICKS_READ();
    rdpq_attach_clear(target,NULL);rdpq_set_mode_standard();rdpq_mode_filter(FILTER_POINT);rdpq_mode_dithering(DITHER_NONE_NONE);
    rdpq_tex_blit(&s->canvas,(320-s->w)/2,(240-s->h)/2,NULL);
    rdpq_detach_wait();display_show(target);
    uint32_t elapsed=TICKS_READ()-began;if (elapsed>s->stats.max_draw_ticks) s->stats.max_draw_ticks=elapsed;
}
msx_cinematic_stats_t msx_cinematic_stats(void) {return playing?playing->stats:(msx_cinematic_stats_t){0};}
uint32_t msx_cinematic_pixels_crc(void) {
    slot_t *s=playing;
    if (!s || !s->opened) return 0;
    if (!s->index && s->crc_first_ok) return s->crc_first;
    if (s->index+1==s->count && s->crc_last_ok) return s->crc_last;
    return ~msx_cine_crc32_fast(&crc_tables,~0u,s->canvas.buffer,(size_t)s->w*s->h*2);
}
