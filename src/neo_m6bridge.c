/* The hanging bridge's driver (neo_m6bridge.h). */
#include "neo_m6bridge.h"

/* 0BCE96: entries (column, count, advance, mask), FFFF ends. */
static const uint16_t table[]={0x000c,0xffff,0x000c,0x0000, 0x001c,0xffff,0x0010,0xff00, 0x002c,0xffff,0x0010,0xffff, 0xffff};
enum { ENTRIES=3 };
static uint16_t tw(int byte) {return table[byte/2];}
/* 0BCF50: entry d0 (bytes) into 10C702 and its count into 10C704. */
static void select(neo_m6bridge_t *b,int d0) {
    if (d0<0 || d0/8>=ENTRIES) {b->fault=true;return;}   /* FFFF: trap #F */
    b->index=(int16_t)d0;b->count=tw(d0+2);
}
bool neo_m6bridge_call(neo_m6bridge_t *b,uint32_t routine) {
    switch (routine) {
    case 0x0BCDC2:   /* 10C70C = 0, 0BD2CE, 0BD2BA, 0BCE18, 10C706 = 0, 0BCE2C, 0BD18E */
        b->lock=0;b->mode=0;b->speed=0;b->on=true;return true;
    case 0x0BCE18: b->mode=0;return true;
    case 0x0BCE22: b->mode=1;return true;
    case 0x0BCF36: b->index=-8;b->count=0;return true;
    case 0x0BCF46: select(b,b->index+8);return true;
    case 0x0BCF86: select(b,0);return true;
    case 0x0BCF8C: select(b,8);return true;
    case 0x0BCF94: select(b,16);return true;
    }
    return false;
}
int32_t neo_m6bridge_drive(neo_m6bridge_t *b,int32_t root_vx) {
    int32_t d0=root_vx,d1=b->speed,d3;
    if (b->mode==1) {d1=(int32_t)((uint32_t)d1-0x4000u);d3=-0x20000;}
    else if (b->mode==2) {int32_t d2=d0>>2;d0=d2*3;d1=(int32_t)((uint32_t)d1-0x2000u);d3=-0x80000;}
    else {d1=0;d3=0;}
    if (d1>d3) b->speed=d1; else d1=b->speed;   /* 0BCE7A: cmp.l d3,d1; ble */
    return (int32_t)((uint32_t)d0+(uint32_t)d1);
}
static int16_t s12(int v) {v&=0xfff;return (int16_t)((v&0x800)?v-4096:v);}
bool neo_m6bridge_loop(neo_m6bridge_t *b,int16_t *o32,int16_t *o34,int16_t o36) {
    if (!b->count || b->index<0) return false;
    uint16_t d4=(uint16_t)(tw(b->index)+(uint16_t)*o32);                       /* 0C6F6A */
    uint16_t d3=(uint16_t)(((uint16_t)o36<<4)+0x1f0u-(uint16_t)(d4<<4));
    if (d3&0x8000u) return false;
    int16_t d0=(int16_t)(uint16_t)((uint16_t)(tw(b->index+4)<<4)+d3);d0=(int16_t)(d0>>4);
    *o32=s12(*o32+d0);*o34=s12(*o34);                                           /* 0C6F82 */
    if (b->count!=0xffffu) b->count--;
    b->lock&=tw(b->index+6);
    return true;
}

/* ---- The bridge's collision ring (neo_m6bridge.h) ---- */
static int16_t s16w(uint32_t v) {return (int16_t)(uint16_t)v;}
int16_t neo_m6ring_o36(int32_t x) {return (int16_t)(s16w((0xffc00000u-(uint32_t)x)>>16)>>4);}   /* 0C748E: +36 */
int16_t neo_m6ring_o38(int32_t y) {return (int16_t)(s16w((0x00800000u-(uint32_t)y)>>16)>>4);}   /* +38 */
void neo_m6ring_clear(neo_m6ring_t *r) {
    for (unsigned s=0;s<32;s++) for (unsigned k=0;k<NEO_M6RING_ROWS;k++) {r->col[s][k]=-1;r->ring_row[s][k]=0;}
}
/* 0C714E with the collision copy 0C72EC: d0 map block column, d1 map block
 * row, d2 columns, d3 rows, d4 local block column, d5 ring row (before ~). */
static void copy(neo_m6ring_t *r,const neo_m6map_t *m,int d0,int d1,int d2,int d3,int d4,int d5,neo_m6ring_write_fn w,void *ctx) {
    d0=s12(d0);d1=s12(d1);
    if ((int16_t)(d0-m->columns)>=0 || (int16_t)(d1-m->rows)>=0) return;
    if (d0<0) {d2+=d0;if (d2<=0) return;d4-=d0;d0=0;}
    if (d1<0) {d3+=d1;if (d3<=0) return;d5+=d1;d1=0;}
    if (d0+d2-m->columns>=0) {d2-=d0+d2-m->columns;if (d2<=0) return;}
    if (d1+d3-m->rows>=0) {d3-=d1+d3-m->rows;if (d3<=0) return;}
    d4&=31;d5=~d5&31;
    for (int i=0;i<d2;i++) {
        unsigned s=(unsigned)(d4+i)&31u;
        for (int k=0;k<d3;k++) {
            unsigned row=(unsigned)(d1+k),q=(unsigned)(2*d5+2*k)&0x3eu;
            if (row>=NEO_M6RING_ROWS) continue;   /* the map has NEO_M6RING_ROWS block rows (checked by the caller) */
            for (unsigned j=0;j<NEO_M6RING_ROWS;j++) if (j!=row && r->col[s][j]>=0 && r->ring_row[s][j]==q>>1) r->col[s][j]=-1;   /* the same bytes */
            r->col[s][row]=(int8_t)(d0+i);r->ring_row[s][row]=(uint8_t)(q>>1);
            if (w) {
                const uint8_t *b=m->blocks+((unsigned)(d0+i)*m->rows+row)*4u;unsigned base=(s<<7)&0xf80u;
                w(ctx,base+q,b[0]);w(ctx,base+q+0x40,b[1]);w(ctx,base+q+1,b[2]);w(ctx,base+q+0x41,b[3]);
            }
        }
    }
}
void neo_m6ring_init(neo_m6ring_t *r,const neo_m6map_t *m,int32_t x,int32_t y,int16_t o32,int16_t o34,neo_m6ring_write_fn w,void *ctx) {
    int d0=neo_m6ring_o36(x),d1=neo_m6ring_o38(y);
    r->o3a=(int16_t)d0;r->o3c=(int16_t)d1;d1+=0x1f;
    copy(r,m,d0-o32,-(d1-o34),0x20,0x20,d0,d1,w,ctx);
}
void neo_m6ring_pass(neo_m6ring_t *r,const neo_m6map_t *m,int32_t x,int32_t y,int16_t o32,int16_t o34,bool rows,neo_m6ring_write_fn w,void *ctx) {
    int d0=neo_m6ring_o36(x),d7=s12(r->o3a-d0);
    if (d7) {
        if (d7<0) {d7=-d7;d0=d0-d7+0x20;}
        int d1=neo_m6ring_o38(y)+0x1f;
        copy(r,m,d0-o32,-(d1-o34),d7,0x20,d0,d1,w,ctx);
    }
    if (rows) {   /* 0C752E: bit 2 of +82 */
        int d1=neo_m6ring_o38(y);d7=s12(r->o3c-d1);
        if (d7) {
            if (d7<0) {d7=-d7;d1+=0x1f;} else d1+=d7;
            int d4=neo_m6ring_o36(x);
            copy(r,m,d4-o32,-(d1-o34),0x20,d7,d4,d1,w,ctx);
        }
    }
}
void neo_m6ring_mark(neo_m6ring_t *r,int32_t x,int32_t y,bool rows) {
    r->o3a=neo_m6ring_o36(x);if (rows) r->o3c=neo_m6ring_o38(y);
}
