/* The ending's shrunk sprites as the LSPC draws them (end_zoom_core.h). */
#include "end_zoom_core.h"
#include <string.h>
#include "neo_scb.h"
static uint32_t be32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static uint16_t be16(const uint8_t *p) {return (uint16_t)(p[0]<<8|p[1]);}
/* Row zx: the columns (0..15) of a 16-pixel tile drawn for the shrink nibble zx; derived from MAME 0.264 screenshots
 * (T1) by tools/check_lspc_shrink.py --derive, which fails unless they give this table (its COLUMNS). */
static const uint8_t columns[16][16]={
 {8},{4,8},{4,8,12},{2,4,8,12},{2,4,8,12,14},{2,4,6,8,12,14},{2,4,6,8,10,12,14},{0,2,4,6,8,10,12,14},
 {0,2,4,6,8,9,10,12,14},{0,2,3,4,6,8,9,10,12,14},{0,2,3,4,6,8,9,10,12,14,15},{0,2,3,4,6,7,8,9,10,12,14,15},
 {0,2,3,4,6,7,8,9,10,12,13,14,15},{0,1,2,3,4,6,7,8,9,10,12,13,14,15},{0,1,2,3,4,6,7,8,9,10,11,12,13,14,15},
 {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}};
uint32_t end_zoom_rest_size(const uint8_t *h) {
    return 20*be32(h+4)+((2*be32(h+12)+3)&~3u)+4*be32(h+8);
}
bool end_zoom_parse(end_zoom_file_t *f,const uint8_t *h,const uint8_t *rest,uint32_t size) {
    if (memcmp(h,"EZM3",4) && memcmp(h,"EZM2",4)) return false;   /* EZM3: EZM2 + the RDP tables after the L0 */
    f->descriptors=be32(h+4);f->tiles=be32(h+8);f->table=rest;
    uint32_t at=20*f->descriptors;f->words=rest+at;at+=(2*be32(h+12)+3)&~3u;   /* the words, padded to 4 bytes */
    f->tile_keys=rest+at;at+=4*f->tiles;
    if (at>size) return false;
    f->tiles_offset=16+at;f->l0_offset=f->tiles_offset+128*f->tiles;
    return true;
}
int end_zoom_find_descriptor(const end_zoom_file_t *f,uint32_t d) {
    int lo=0,hi=(int)f->descriptors-1;
    while (lo<=hi) {int m=(lo+hi)/2;uint32_t k=be32(f->table+20*m);if (k==d) return m;if (k<d) lo=m+1; else hi=m-1;}
    return -1;
}
void end_zoom_descriptor_tiles(const end_zoom_file_t *f,int index,uint32_t *first,uint32_t *count) {
    *first=be32(f->table+20*index+12);*count=be32(f->table+20*index+16);
}
int end_zoom_find_tile(const end_zoom_file_t *f,int index,uint32_t t) {
    uint32_t first,count;end_zoom_descriptor_tiles(f,index,&first,&count);
    int lo=(int)first,hi=(int)(first+count)-1;
    while (lo<=hi) {int m=(lo+hi)/2;uint32_t k=be32(f->tile_keys+4*m);if (k==t) return m;if (k<t) lo=m+1; else hi=m-1;}
    return -1;
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
/* check_lspc_shrink.line_tile: tile row and tile line of sprite line L, or -1 when not drawn. */
static int line_tile(const uint8_t *l0,unsigned zy,unsigned rows,unsigned line,unsigned *yy) {
    unsigned zl=line&0xffu;bool inv=(line&0x100u)!=0;
    if (inv) zl^=0xffu;
    if (rows>0x20) {
        unsigned span=(zy+1)<<1;zl%=span;
        if (zl>zy) {zl=span-1-zl;inv=!inv;}
    }
    unsigned b=l0[zl],tile=b>>4;*yy=b&15u;
    if (inv) {tile^=0x1fu;*yy^=0xfu;}
    if (rows<=0x20 && tile>=rows) return -1;
    return (int)tile;
}
_Static_assert(sizeof(column_t)*END_ZOOM_COLUMNS<=END_ZOOM_WORK_BYTES,"work");
bool end_zoom_entry(const end_zoom_file_t *f,int index,uint16_t x,uint16_t y,uint16_t zoom,uint16_t attr,int first,int last,end_zoom_io_t *io) {
    column_t *scb_columns=io->work;scb_ctx_t s={f,index,scb_columns,0};
    neo_scb_entry_t e={x,y,zoom,(uint16_t)(attr&3u),be32(f->table+20*index)};
    /* 011E60's forward writers over a window of END_ZOOM_COLUMNS sprites from 0 (every column shown). */
    uint16_t win[4]={0,END_ZOOM_COLUMNS,0,0};
    neo_scb_entry(true,true,&e,win,rd,wr,&s);
    bool whole=win[2]<END_ZOOM_COLUMNS;
    for (unsigned c=0;c<s.count;c++) {
        const column_t *k=&scb_columns[c];
        unsigned rows=k->scb3&63u,zx=(k->scb2>>8)&15u,zy=k->scb2&0xffu;
        if (!rows) continue;
        int sx=(k->scb4>>7)&511,sy=(512-(k->scb3>>7))&511;
        if (sx>=320) sx-=512;   /* x .. x + zx modulo 512 */
        if (sx+(int)zx<0 || sx>=320) continue;
        const uint8_t *l0=io->l0_row(io->ctx,zy);
        end_zoom_strip_t st;int row=-1,start=0;st.n=0;
        for (int r=first;r<=last+1;r++) {
            unsigned yy=0;int t=r<=last?line_tile(l0,zy,rows,(unsigned)(r-sy)&511u,&yy):-1;
            if (st.n && (t!=row || st.n==16)) {io->strip(io->ctx,&st,sx,start);st.n=0;}
            if (t<0) {row=-1;continue;}
            if (!st.n) {
                uint16_t lo=k->scb1[2*t],at=k->scb1[2*t+1];
                if (at&12u) {io->auto_animated++;row=-1;continue;}
                st.tile=lo|(((uint32_t)at<<12)&0xf0000u);st.flips=(uint8_t)(at&3u);st.zx=(uint8_t)zx;start=r;row=t;
            }
            st.line[st.n++]=(uint8_t)yy;
        }
        if (io->column_end) io->column_end(io->ctx);
    }
    return whole;
}
void end_zoom_compose(const end_zoom_strip_t *s,const uint8_t *t,uint8_t *out) {
    /* t: the tile as 16 x 16 CI4 (export_ending_zoom.linear) */
    uint8_t xs[16];const unsigned w=s->zx+1u;
    for (unsigned j=0;j<w;j++) {unsigned c=s->zx==15?j:columns[s->zx][j];xs[j]=(uint8_t)((s->flips&1u)?15u-c:c);}
    for (unsigned i=0;i<s->n;i++) {
        const uint8_t *row=t+8*((s->flips&2u)?15u-s->line[i]:s->line[i]);uint8_t *o=out+8*i;
        if (w==16 && !(s->flips&1u)) {memcpy(o,row,8);continue;}
        memset(o,0,8);
        for (unsigned j=0;j<w;j++) {
            unsigned xx=xs[j],pen=(xx&1u)?row[xx>>1]&15u:row[xx>>1]>>4;
            o[j>>1]|=(uint8_t)((j&1u)?pen:pen<<4);
        }
    }
}
