/* The 68K's sprite-list column writers (neo_scb.h), translated from 00AB7A / 00C3EE / 00C782 / 00CB18 / 00F536 and
 * the unshrunk 00A106 / 00B8F6 / 00BBAC / 00BE64 / 00EA4E (the 32 writers differ only in the parameters below). */
#include "neo_scb.h"
static int16_t s16(uint32_t v) {return (int16_t)(uint16_t)v;}
void neo_scb_entry(bool vblank,bool forward,const neo_scb_entry_t *e,uint16_t win[4],neo_scb_read_t rd,neo_scb_write_t wr,void *ctx) {
    const unsigned zx=e->zoom>>8,zy=e->zoom&0xffu;
    const bool zoomed=e->zoom!=0xffffu,hflip=(e->attr&1u)!=0,vflip=(e->attr&2u)!=0;
    uint32_t a3=0;
    uint16_t idx=forward?win[2]:win[3];
    uint16_t w0=rd(ctx,e->descriptor,a3++);
    uint16_t d1=(uint16_t)((e->attr&0xff00u)|((e->attr^w0)&0xffu));
    const uint16_t lim=forward?(vblank?win[1]:win[3]):(vblank?win[0]:win[2]);
    for (unsigned g=0;g<(unsigned)(w0>>8);g++) {
        uint16_t xo=rd(ctx,e->descriptor,a3),yo=rd(ctx,e->descriptor,a3+1),cr=rd(ctx,e->descriptor,a3+2);a3+=3;
        const unsigned cols=cr>>8,rows=cr&0xffu,n=(rows-1)&0x1fu;
        if (hflip) xo=(uint16_t)~xo;
        if (vflip) yo=(uint16_t)(~yo+(rows<<4));
        int32_t d5,d4=0,d3=0;uint16_t scb3;
        if (!zoomed) {
            d5=(int32_t)s16(xo+e->x)*128;
            uint16_t v=(uint16_t)((yo+e->y)<<7);scb3=(uint16_t)((v&0xff00u)|((v|rows)&0xffu));
        } else {
            int32_t m=(int32_t)zy+1;
            uint16_t v=(uint16_t)((((int32_t)s16(yo)*m)&~0xff)/2+(uint16_t)(e->y<<7));
            scb3=(uint16_t)((v&0xff00u)|((v|((rows*(unsigned)m+0xffu)>>8))&0xffu));
            m=(int32_t)zx+1;
            int32_t p=(int32_t)s16(xo)*m;d4=p&0xff;d5=(p&~0xff)/2+(int32_t)s16(e->x)*128;
            d3=s16((uint32_t)((m<<4)-0x100));
        }
        for (unsigned c=0;c<cols;c++) {
            if (c && !hflip) d5+=zoomed?(((d4+0x100)&0xff00)>>1):0x800;   /* c49e / b970 */
            if (forward?s16(idx-lim)>=0:s16(idx-lim)<=0) {if (forward) win[2]=idx; else win[3]=idx;return;}
            if (zoomed) {
                d4=(d4&0xff)+d3;
                if (d4<0) {a3+=n+1;continue;}   /* a column of no width */
                if (hflip) d5-=((d4+0x100)&0xff00)>>1;
            } else if (hflip) d5-=0x800;
            if ((uint16_t)((d5>>1)+0x200)>=0x5000u) {a3+=n+1;continue;}   /* x outside -8..311 */
            if (!forward) idx--;
            bool show=true;
            if (vblank) {
                wr(ctx,(uint16_t)(0x8201u+idx),scb3);
                show=forward?s16(idx-win[3])>=0:s16(idx-win[2])<0;
            }
            if (show) {
                wr(ctx,(uint16_t)(0x8401u+idx),(uint16_t)d5);
                wr(ctx,(uint16_t)(0x8001u+idx),zoomed?(uint16_t)((d4&0xff00)|zy):0xffffu);
                uint16_t a=(uint16_t)((idx+1u)<<6);
                for (unsigned k=0;k<=n;k++) {
                    uint16_t o=(uint16_t)(vflip?2*(n-k):2*k);
                    wr(ctx,(uint16_t)(a+o),rd(ctx,e->descriptor,a3++));wr(ctx,(uint16_t)(a+o+1),d1);
                }
                if (zoomed) for (unsigned k=n+1;k<32;k++) wr(ctx,(uint16_t)(a+2*k),0);
            } else a3+=n+1;
            if (forward) idx++;
        }
    }
    if (forward) win[2]=idx; else win[3]=idx;
}
void neo_scb_hide_rest(bool forward,const uint16_t win[4],neo_scb_write_t wr,void *ctx) {
    if (forward) {
        uint16_t d0=s16(win[2])>s16(win[3])?win[2]:win[3];
        for (unsigned k=0;k<((unsigned)(win[1]-d0)&0x1ffu);k++) wr(ctx,(uint16_t)(0x8201u+d0+k),0x8000u);
    } else {
        uint16_t d0=s16(win[2])<s16(win[3])?win[2]:win[3];
        for (unsigned k=0;k<((unsigned)(d0-win[0])&0x1ffu);k++) wr(ctx,(uint16_t)(0x8201u+d0-1-k),0x8000u);
    }
}
