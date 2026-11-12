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
