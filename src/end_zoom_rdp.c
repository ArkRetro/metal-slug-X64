/* The ending's shrunk sprites as exact RDP texture rectangles (end_zoom_rdp.h). */
#include "end_zoom_rdp.h"
#include <string.h>
#include "neo_scb.h"
static inline uint32_t be32(const uint8_t *p) {
    uint32_t v;memcpy(&v,p,4);
#if __BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__
    v=__builtin_bswap32(v);
#endif
    return v;
}
static inline uint16_t be16(const uint8_t *p) {
    uint16_t v;memcpy(&v,p,2);
#if __BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__
    v=__builtin_bswap16(v);
#endif
    return v;
}
/* end_zoom_find_tile, first trying the tile after the previous one (a column's tiles are usually consecutive). */
static int find_tile(const end_zoom_file_t *f,uint32_t first,uint32_t count,uint32_t tile,unsigned *hint) {
    if (*hint+1<count && be32(f->tile_keys+4*(first+*hint+1))==tile) return (int)++*hint;
    int lo=(int)first,hi=(int)(first+count)-1;
    while (lo<=hi) {int m=(lo+hi)/2;uint32_t k=be32(f->tile_keys+4*m);if (k==tile) {*hint=(unsigned)m-first;return (int)*hint;}if (k<tile) lo=m+1; else hi=m-1;}
    return -1;
}
/* A rectangle continuing `a` below it with the same texels' arithmetic: one rectangle (the same pixels). */
static bool merge(end_zoom_rect_t *a,const end_zoom_rect_t *b) {
    if (a->page!=b->page || a->x0!=b->x0 || a->x1!=b->x1 || a->s0!=b->s0 || a->dsdx!=b->dsdx || a->dtdy!=b->dtdy || a->y1!=b->y0) return false;
    if ((int32_t)a->t0*32+(int32_t)a->dtdy*(a->y1-a->y0)!=(int32_t)b->t0*32) return false;
    a->y1=b->y1;return true;
}
static int16_t bes16(const uint8_t *p) {return (int16_t)be16(p);}
bool end_zoom_rdp_parse(end_zoom_rdp_t *t,const uint8_t *p,uint32_t size) {
    if (size<8) return false;
    t->nseq=be32(p);t->npieces=be32(p+4);
    t->horizontal=p+8;t->runs=t->horizontal+32*20;t->seqs=t->runs+256*16*10;t->pieces=t->seqs+4*t->nseq;
    return (uint32_t)(t->pieces+6*t->npieces-p)<=size;
}
typedef struct { uint16_t scb2,scb3,scb4,scb1[64]; } column_t;
typedef struct { const end_zoom_file_t *f;int index;column_t *c;unsigned count; } scb_ctx_t;
static uint16_t rd(void *ctx,uint32_t descriptor,uint32_t i) {
    scb_ctx_t *s=ctx;(void)descriptor;
    const uint8_t *e=s->f->table+20*s->index;
    return i<be32(e+8)?be16(s->f->words+2*(be32(e+4)+i)):0;
}
static void wr(void *ctx,uint16_t a,uint16_t d) {
    scb_ctx_t *s=ctx;unsigned idx;
    if (a>=0x8401u) {idx=a-0x8401u;if (idx<END_ZOOM_COLUMNS) s->c[idx].scb4=d;}
    else if (a>=0x8201u) {idx=a-0x8201u;if (idx<END_ZOOM_COLUMNS) s->c[idx].scb3=d;}
    else if (a>=0x8001u) {idx=a-0x8001u;if (idx<END_ZOOM_COLUMNS) s->c[idx].scb2=d;}
    else {idx=(a>>6)-1u;if (idx<END_ZOOM_COLUMNS) s->c[idx].scb1[a&63u]=d; else return;}
    if (idx<END_ZOOM_COLUMNS && idx+1>s->count) s->count=idx+1;
}
/* (S, d) with floor((32 S + d i) / 1024) = v[i] for every i (export_ending_zoom.fit, exhaustive over d). */
static bool fit(const int *v,int n,int *S,int *D) {
    if (n==1) {*S=v[0]*32;*D=0;return true;}
    int k=n-1,a=v[n-1]-v[0],lo_d=((a-1)*1024)/k-3,hi_d=((a+1)*1024)/k+3;
    if (lo_d<-16384) lo_d=-16384;
    if (hi_d>16383) hi_d=16383;
    /* nearest to the mean step first, as export_ending_zoom.fit */
    int c0=a>=0?(a*1024+k/2)/k:-((-a*1024+k/2)/k);
    for (int j=0;j<=2*(hi_d-lo_d+1);j++) {
        int d=c0+((j&1)?-((j+1)/2):j/2);
        if (d<lo_d || d>hi_d) continue;
        int lo=-0x7fffffff,hi=0x7fffffff,i;
        for (i=0;i<n;i++) {
            int l=v[i]*1024-d*i,h=v[i]*1024+1023-d*i;
            if (l>lo) lo=l;
            if (h<hi) hi=h;
            if (lo>hi) break;
        }
        if (i<n) continue;
        int s=lo>=0?(lo+31)/32:-((-lo)/32);
        if (32*s<=hi) {*S=s;*D=d;return true;}
    }
    return false;
}
/* The piece (S, d) of n texels from texel k on: shifted when 32 S + d k is a multiple of 32, else fitted again. */
static bool skip(int *S,int *D,int k,int n,end_zoom_rdp_io_t *io) {
    int u=32**S+*D*k;
    if (!(u&31)) {*S=u/32;return true;}
    int v[512];
    for (int i=0;i<n;i++) {int w=32**S+*D*(k+i);v[i]=w>=0?w/1024:-((-w+1023)/1024);}
    io->refits++;
    if (fit(v,n,S,D)) return true;
    io->unfit++;return false;
}
bool end_zoom_rdp_entry(const end_zoom_file_t *f,const end_zoom_rdp_t *t,int index,uint16_t x,uint16_t y,uint16_t zoom,uint16_t attr,end_zoom_rdp_io_t *io) {
    column_t *cols=io->work;scb_ctx_t sc={f,index,cols,0};
    neo_scb_entry_t e={x,y,zoom,(uint16_t)(attr&3u),be32(f->table+20*index)};
    uint16_t win[4]={0,END_ZOOM_COLUMNS,0,0};
    neo_scb_entry(true,true,&e,win,rd,wr,&sc);
    uint32_t first,count;end_zoom_descriptor_tiles(f,index,&first,&count);
    unsigned n_out=0,hint=0;
    for (unsigned c=0;c<sc.count;c++) {
        const column_t *k=&cols[c];
        unsigned rows=k->scb3&63u,zx=(k->scb2>>8)&15u,zy=k->scb2&0xffu;
        if (!rows) continue;
        int sx=(k->scb4>>7)&511,sy=(512-(k->scb3>>7))&511;
        if (sx>=320) sx-=512;
        if (sx+(int)zx<0 || sx>=320) continue;
        end_zoom_rect_t open_r[3];unsigned open_n[3]={0,0,0};
        for (unsigned half=0;half<2;half++) for (unsigned tr0=0;tr0<16;tr0++) {
            const uint8_t *run=t->runs+10*(zy*16+tr0);
            unsigned len=run[1],tr=half?tr0^0x1fu:tr0;
            if (!len || tr>=rows) continue;
            uint16_t lo=k->scb1[2*tr],at=k->scb1[2*tr+1];
            if (at&12u) {io->auto_animated++;continue;}
            int ti=find_tile(f,first,count,lo|(((uint32_t)at<<12)&0xf0000u),&hint);
            if (ti<0) {io->missing_tiles++;continue;}
            unsigned local=(unsigned)ti;
            bool hf=(at&1u)!=0,vf=(at&2u)!=0;
            unsigned variant=half?(vf?1u:3u):(vf?2u:0u);
            int lstart=half?256+(255-(int)(run[0]+len-1)):run[0];
            const uint8_t *sq=t->seqs+4*be16(run+2+2*variant);
            const uint8_t *hp=t->horizontal+20*(zx*2+(hf?1u:0u));
            for (unsigned p=0;p<sq[2];p++) {
                const uint8_t *pc=t->pieces+6*(be16(sq)+p);
                int voff=pc[0],vlen=pc[1];
                /* the piece's lines may wrap at 512 (the LSPC's line counter): two parts */
                int r0=(sy+lstart+voff)&511;
                for (int part=0,k0=0;part<2 && k0<vlen;part++) {
                    int m=r0+vlen-k0>512?512-r0:vlen-k0,ry=part?0:r0;
                    int Ty=bes16(pc+2),Dy=bes16(pc+4),yy=ry-8,kk=k0,mm=m;
                    k0+=m;r0=0;
                    if (yy>=240 || yy+mm<=0) continue;
                    if (yy<0) {kk+=-yy;mm-=-yy;yy=0;}
                    if (kk && !skip(&Ty,&Dy,kk,mm,io)) continue;
                    int y1=yy+mm>240?240:yy+mm;
                    for (unsigned h=0;h<hp[0];h++) {
                        const uint8_t *hc=hp+1+6*h;
                        int Sx=bes16(hc+2),Dx=bes16(hc+4),xx=sx+hc[0],w=hc[1],hk=0;
                        if (xx>=320 || xx+w<=0) continue;
                        if (xx<0) {hk=-xx;w-=hk;xx=0;if (!skip(&Sx,&Dx,hk,w,io)) continue;}
                        end_zoom_rect_t r={(int16_t)xx,(int16_t)yy,(int16_t)(xx+w>320?320:xx+w),(int16_t)y1,
                            (uint16_t)Sx,(uint16_t)(Ty+512*(int)(local&15u)),(int16_t)Dx,(int16_t)Dy,(uint16_t)(local>>4)};
                        if (open_n[h] && merge(&open_r[h],&r)) continue;
                        if (open_n[h]) {if (n_out<END_ZOOM_RECTS) io->sorted[n_out++]=open_r[h]; else io->unfit++;}
                        open_r[h]=r;open_n[h]=1;
                    }
                }
            }
        }
        for (unsigned h=0;h<3;h++) if (open_n[h]) {if (n_out<END_ZOOM_RECTS) io->sorted[n_out++]=open_r[h]; else io->unfit++;}
    }
    /* page by page, in order within a page */
    uint16_t start[129];unsigned pages=(count+15)/16;
    if (pages>128) pages=128;
    memset(start,0,sizeof(start));
    for (unsigned i=0;i<n_out;i++) start[io->sorted[i].page+1]++;
    for (unsigned pg=0;pg<pages;pg++) start[pg+1]+=start[pg];
    end_zoom_rect_t *out=io->sorted+END_ZOOM_RECTS;   /* the second half of the buffer */
    for (unsigned i=0;i<n_out;i++) out[start[io->sorted[i].page]++]=io->sorted[i];
    for (unsigned i=0;i<n_out;i++) {io->rect(io->ctx,&out[i]);io->rects++;}
    return win[2]<END_ZOOM_COLUMNS;
}
