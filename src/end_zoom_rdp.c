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
static int16_t bes16(const uint8_t *p) {return (int16_t)be16(p);}
bool end_zoom_rdp_parse(end_zoom_rdp_t *t,const uint8_t *p,uint32_t size) {
    if (size<8) return false;
    t->nseq=be32(p);t->npieces=be32(p+4);
    t->horizontal=p+8;t->runs=t->horizontal+32*20;t->seqs=t->runs+256*16*10;t->pieces=t->seqs+4*t->nseq;
    return (uint32_t)(t->pieces+6*t->npieces-p)<=size;
}
typedef struct { uint16_t scb2,scb3,scb4,scb1[64]; } column_t;
typedef struct { int16_t y0,y1,t0,d;uint16_t page;uint8_t hf; } seg_t;
typedef struct { const end_zoom_file_t *f;int index;column_t *c;unsigned count;const uint8_t *w;uint32_t nw;uint32_t last;const uint16_t *map;uint16_t (*loc)[32]; } scb_ctx_t;
static uint16_t rd(void *ctx,uint32_t descriptor,uint32_t i) {
    scb_ctx_t *s=ctx;(void)descriptor;s->last=i;
    return i<s->nw?be16(s->w+2*i):0;
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
uint32_t end_zoom_descriptor_words(const end_zoom_file_t *f,int index) {return be32(f->table+20*index+8);}
/* Per word of descriptor `index`: the local index of its tile (descriptor words that are tiles), else 0xFFFF. */
void end_zoom_rdp_local_map(const end_zoom_file_t *f,int index,uint16_t *map) {
    const uint8_t *te=f->table+20*index;const uint8_t *w=f->words+2*be32(te+4);uint32_t nw=be32(te+8),first,count;
    end_zoom_descriptor_tiles(f,index,&first,&count);
    for (uint32_t i=0;i<nw;i++) map[i]=0xffffu;
    if (!nw) return;
    unsigned w0=be16(w),i=1,hint=0;
    for (unsigned g=0;g<(w0>>8) && i+3<=nw;g++) {
        unsigned cr=be16(w+2*(i+2)),n=(((cr&0xffu)-1u)&0x1fu)+1u;i+=3;
        for (unsigned c=0;c<(cr>>8);c++) for (unsigned k=0;k<n && i<nw;k++,i++) {
            int ti=find_tile(f,first,count,be16(w+2*i)|((uint32_t)(w0&0xf0u)<<12),&hint);
            map[i]=ti<0?0xffffu:(uint16_t)ti;
        }
    }
}
typedef struct { int16_t lstart,len,T,d; } vpiece_t;
bool end_zoom_rdp_entry(const end_zoom_file_t *f,const end_zoom_rdp_t *t,int index,uint16_t x,uint16_t y,uint16_t zoom,uint16_t attr,end_zoom_rdp_io_t *io) {
    const uint8_t *te=f->table+20*index;
    scb_ctx_t sc={f,index,NULL,0,f->words+2*be32(te+4),be32(te+8),0xffffffffu,NULL,NULL};
    neo_scb_entry_t e={x,y,zoom,(uint16_t)(attr&3u),be32(te)};
    uint16_t win[4]={0,END_ZOOM_COLUMNS,0,0};
    neo_scb_columns_t cl={io->work,0,END_ZOOM_COLUMNS,0};
    neo_scb_entry_columns(&e,win,rd,&sc,&cl);
    uint32_t first,count;end_zoom_descriptor_tiles(f,index,&first,&count);
    unsigned n_out=0,hint=0;
    if (!cl.count) return true;
    /* once per entry (every column has the entry's zoom y and attribute): per tile row, its pieces in sprite lines */
    const unsigned zy=zoom&0xffu,at=cl.c[0].attr;
    if (at&12u) {io->auto_animated++;return true;}
    const bool vf=(at&2u)!=0;const unsigned hf=at&1u;
    vpiece_t vp[32][4];uint8_t nvp[32];
    for (unsigned tr=0;tr<32;tr++) {
        unsigned half=tr>>4,tr0=half?tr^0x1fu:tr;
        const uint8_t *run=t->runs+10*(zy*16+tr0);
        unsigned len=run[1];nvp[tr]=0;
        if (!len) continue;
        unsigned variant=half?(vf?1u:3u):(vf?2u:0u);
        int lstart=half?256+(255-(int)(run[0]+len-1)):run[0];
        const uint8_t *sq=t->seqs+4*be16(run+2+2*variant);
        for (unsigned p=0;p<sq[2] && p<4;p++) {
            const uint8_t *pc=t->pieces+6*(be16(sq)+p);
            vp[tr][nvp[tr]++]=(vpiece_t){(int16_t)(lstart+pc[0]),pc[1],bes16(pc+2),bes16(pc+4)};
        }
    }
    for (unsigned c=0;c<cl.count;c++) {
        const neo_scb_column_t *k=&cl.c[c];
        unsigned rows=k->scb3&63u,zx=(k->scb2>>8)&15u;
        if (!rows) continue;
        int sx=(k->scb4>>7)&511,sy=(512-(k->scb3>>7))&511;
        if (sx>=320) sx-=512;
        if (sx+(int)zx<0 || sx>=320) continue;
        /* the column's vertical segments (tile row pieces on screen), merged where the texels continue */
        seg_t sg[64];unsigned ns=0;
        for (unsigned tr=0;tr<32;tr++) {
            if (tr>=rows) break;
            if (!nvp[tr]) continue;
            unsigned row=k->vflip?k->rows-1u-tr:tr;
            if (row>=k->rows) continue;
            uint32_t wi=k->first_word+row;
            int ti=io->local_map && wi<sc.nw?(int)io->local_map[wi]:-1;
            if (ti<0 || ti==0xffff) ti=find_tile(f,first,count,(wi<sc.nw?be16(sc.w+2*wi):0)|(((uint32_t)at<<12)&0xf0000u),&hint);
            if (ti<0) {io->missing_tiles++;continue;}
            unsigned local=(unsigned)ti;
            for (unsigned p=0;p<nvp[tr];p++) {
                const vpiece_t *pc=&vp[tr][p];
                int vlen=pc->len;
                int r0=(sy+pc->lstart)&511;
                for (int part=0,k0=0;part<2 && k0<vlen;part++) {
                    int m=r0+vlen-k0>512?512-r0:vlen-k0,ry=part?0:r0;
                    int Ty=pc->T,Dy=pc->d,yy=ry-8,kk=k0,mm=m;
                    k0+=m;r0=0;
                    if (yy>=240 || yy+mm<=0) continue;
                    if (yy<0) {kk+=-yy;mm-=-yy;yy=0;}
                    if (kk && !skip(&Ty,&Dy,kk,mm,io)) continue;
                    int y1=yy+mm>240?240:yy+mm,t0=Ty+512*(int)(local&15u);
                    seg_t *q=ns?&sg[ns-1]:NULL;
                    if (q && q->page==(local>>4) && q->d==Dy && q->y1==yy && q->t0*32+Dy*(q->y1-q->y0)==t0*32) {q->y1=(int16_t)y1;continue;}
                    if (ns<64) sg[ns++]=(seg_t){(int16_t)yy,(int16_t)y1,(int16_t)t0,(int16_t)Dy,(uint16_t)(local>>4),(uint8_t)hf};
                    else io->unfit++;
                }
            }
        }
        for (unsigned g=0;g<ns;g++) {
            const uint8_t *hp=t->horizontal+20*(zx*2+sg[g].hf);
            for (unsigned h=0;h<hp[0];h++) {
                const uint8_t *hc=hp+1+6*h;
                int Sx=bes16(hc+2),Dx=bes16(hc+4),xx=sx+hc[0],w=hc[1];
                if (xx>=320 || xx+w<=0) continue;
                if (xx<0) {int hk=-xx;w-=hk;xx=0;if (!skip(&Sx,&Dx,hk,w,io)) continue;}
                if (n_out<END_ZOOM_RECTS) io->sorted[n_out++]=(end_zoom_rect_t){(int16_t)xx,sg[g].y0,(int16_t)(xx+w>320?320:xx+w),sg[g].y1,
                    (uint16_t)Sx,(uint16_t)sg[g].t0,(int16_t)Dx,sg[g].d,sg[g].page};
                else io->unfit++;
            }
        }
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
