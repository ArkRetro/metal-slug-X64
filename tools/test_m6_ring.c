/* The collision ring of the Mission 6 bridge 10D7B6 (map 0BC6E6, collision
 * 45FC2A; src/neo_m6bridge.c) against MAME 0.264 (data/m6ring_fixture.json.gz,
 * tools/extract_m6ring_fixture.py: captures-m6/fam/L and captures-m6/s9, run4
 * replays of stage 1). From the header 10D7B6 +14/+18/+2E/+32/+34/+82 at each
 * frame's end, the ring loaders 0C7038 / 0C741A / 0C74C4 (and again after
 * the loop's origin move 0C6F82) must write the bytes MAME wrote, in order,
 * on the same frames, and every cell the kernel read with 0C79FE (first and
 * last frame of each aggregated read) must be the model's cell. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "neo_m6bridge.h"
#include "region_rom.h"
#include "m6ring_fixture.h"
#define COUNT(a) (sizeof(a)/sizeof((a)[0]))
static const uint8_t *blob(uint32_t a,unsigned n) {
    for (unsigned i=0;i<COUNT(region_blobs);i++) if (region_blobs[i].address==a && region_blobs[i].length>=n) return region_blobs[i].data;
    return NULL;
}
typedef struct { uint32_t got[64]; unsigned n; bool over; } wlog_t;
static void wr(void *ctx,unsigned offset,uint8_t value) {
    wlog_t *l=ctx;if (l->n<COUNT(l->got)) l->got[l->n++]=offset<<8|value;else l->over=true;
}
int main(void) {
    const uint8_t *d=blob(0x0BC6E6u,14),*c=blob(0x45FC2Au,1200);
    if (!d || !c) {printf("FAIL: region_rom.h lacks 0BC6E6 / 45FC2A\n");return 1;}
    neo_m6map_t m={c,(uint16_t)(d[8]<<8|d[9]),(uint16_t)(d[10]<<8|d[11])};
    if (m.rows!=NEO_M6RING_ROWS || (uint32_t)(d[4]<<24|d[5]<<16|d[6]<<8|d[7])!=0x45FC2Au) {printf("FAIL: 0BC6E6 header\n");return 1;}
    neo_m6ring_t r;neo_m6ring_clear(&r);
    unsigned fi=0,li=0,r0=0,r1=0,bad=0,loads_ok=0,bytes=0,checked=0;bool on=false;int16_t p32=0,p34=0;
    uint32_t first=m6ring_frames[0].frame,last=m6ring_frames[COUNT(m6ring_frames)-1].frame;
    /* reads sorted by first frame; their last frames are checked on a second walk below */
    static uint32_t lastv[COUNT(m6ring_reads)];
    for (uint32_t f=first;f<=last;f++) {
        wlog_t log={{0},0,false};
        if (f==m6ring_clear) neo_m6ring_clear(&r);
        while (fi+1<COUNT(m6ring_frames) && m6ring_frames[fi+1].frame<=f) fi++;
        const m6ring_frame_t *h=&m6ring_frames[fi];
        if (h->map==0x0BC6E6u) {
            bool rows=(h->b82&4u)!=0;
            if (!on) {neo_m6ring_init(&r,&m,h->x,h->y,h->o32,h->o34,wr,&log);on=true;}
            else {
                neo_m6ring_pass(&r,&m,h->x,h->y,p32,p34,rows,wr,&log);
                if (h->o32!=p32 || h->o34!=p34) neo_m6ring_pass(&r,&m,h->x,h->y,h->o32,h->o34,rows,wr,&log);   /* 0C6F82 -> 0C748E */
                neo_m6ring_mark(&r,h->x,h->y,rows);
            }
            p32=h->o32;p34=h->o34;
        } else on=false;
        bool want=li<COUNT(m6ring_loads) && m6ring_loads[li].frame==f;
        unsigned wn=want?m6ring_loads[li].count:0,wf=want?m6ring_loads[li].first:0;bool same=!log.over && log.n==wn;
        for (unsigned k=0;same && k<wn;k++) same=log.got[k]==m6ring_bytes[wf+k];
        if (!same) {if (bad++<5) printf("FAIL frame %u: %u ring writes (MAME %u)\n",(unsigned)f,log.n,wn);}
        else if (want) {loads_ok++;bytes+=wn;}
        if (want) li++;
        for (;r0<COUNT(m6ring_reads) && m6ring_reads[r0].first<=f;r0++) {
            const m6ring_read_t *q=&m6ring_reads[r0];
            uint16_t x=(uint16_t)((q->offset>>6)<<3),y=(uint16_t)~(uint16_t)((q->offset&63u)<<3);
            uint8_t v=neo_m6ring_cell(&r,&m,x,y);checked++;
            if (v!=q->value && bad++<5) printf("FAIL read at %u: cell %03x = %02x (MAME %02x)\n",(unsigned)f,q->offset,v,q->value);
        }
        for (;r1<COUNT(m6ring_by_last) && m6ring_reads[m6ring_by_last[r1]].last<=f;r1++) {unsigned k=m6ring_by_last[r1];lastv[k]=1u+neo_m6ring_cell(&r,&m,(uint16_t)((m6ring_reads[k].offset>>6)<<3),(uint16_t)~(uint16_t)((m6ring_reads[k].offset&63u)<<3));}
    }
    for (unsigned k=0;k<COUNT(m6ring_reads);k++) {
        checked++;
        if (lastv[k]!=1u+m6ring_reads[k].value && bad++<5) printf("FAIL read (last frame %u): cell %03x\n",(unsigned)m6ring_reads[k].last,m6ring_reads[k].offset);
    }
    if (bad || li!=COUNT(m6ring_loads)) {printf("FAIL: bridge ring %u differences (%u of %u load frames)\n",bad,loads_ok,(unsigned)COUNT(m6ring_loads));return 1;}
    printf("PASS: bridge 10D7B6 collision ring (map 0BC6E6, 45FC2A): %u load frames, %u ring writes equal to MAME; %u of %u kernel reads (%u read events) equal\n",
           loads_ok,bytes,checked,checked,(unsigned)m6ring_read_total);
    return 0;
}
