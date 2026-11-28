#include "neo_wipe.h"
#include <string.h>
#include "neo_gactor.h"   /* ga_rom: columns E0818/E0856 */

static uint16_t rw(uint32_t a) {const uint8_t *p=ga_rom(a);return p?(uint16_t)(p[0]<<8|p[1]):0xffffu;}
void neo_wipe_create(neo_wipe_t *w,bool exit) {
    memset(w,0,sizeof *w);w->routine=exit?0x3aef0u:0x3aed8u;
}
bool neo_wipe_update(neo_wipe_t *w,neo_wipe_write_t write,void *ctx) {
    switch (w->routine) {
    case 0x3aed8: case 0x3aef0:                               /* constructor */
        w->cursor=0xffe2u;w->routine=w->routine==0x3aed8u?0x3aee6u:0x3aefeu;return true;
    case 0x3aee6: case 0x3aefe: break;
    default: return false;
    }
    uint32_t a=w->routine==0x3aee6u?0xe0818u:0xe0856u;
    int16_t col=(int16_t)(w->cursor+3u);
    if (col>=0x46) {w->signal=true;w->routine=0;return false;}   /* 03AF16, 004188 */
    w->cursor=(uint16_t)col;                                   /* 03AFAE */
    if (col<=0) {a+=(uint32_t)(1-col)*2u;col=1;}
    for (;;a+=2,col++) {                                       /* 03AFCE */
        uint16_t tile=rw(a);
        if (col>=0x27 || tile==0xffffu) break;
        for (unsigned row=1;row<0x1f;row++) {
            uint16_t vram=(uint16_t)(0x7000u+(unsigned)col*32u+row);
            w->fix[col*NEO_WIPE_ROWS+row]=tile;
            if (write) write(ctx,vram,tile);
        }
    }
    return true;
}
