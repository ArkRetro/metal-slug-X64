/* Parachute descent of the player, see marco_descent.h. */
#include "marco_descent.h"
static uint32_t rl(const uint8_t *m,unsigned o) {return (uint32_t)m[o]<<24|(uint32_t)m[o+1]<<16|(uint32_t)m[o+2]<<8|m[o+3];}
static void wl(uint8_t *m,unsigned o,uint32_t v) {m[o]=(uint8_t)(v>>24);m[o+1]=(uint8_t)(v>>16);m[o+2]=(uint8_t)(v>>8);m[o+3]=(uint8_t)v;}
static void ww(uint8_t *m,unsigned o,uint16_t v) {m[o]=(uint8_t)(v>>8);m[o+1]=(uint8_t)v;}
static uint32_t asr(uint32_t v,unsigned n) {return (uint32_t)((int32_t)v>>n);}
void marco_descent_begin(uint8_t *m) {
    m[0x7a]=0xff;m[0x7b]=0;wl(m,0x30,0);wl(m,0x34,0xfff80000u);
    ww(m,0x66,0xe0ec);ww(m,0x1e,0xe0ec);ww(m,0x84,0x78);
    m[0x61]=0;m[0x90]=2;m[0x9a]|=4u;wl(m,0,MARCO_DESCENT_HANG);
}
void marco_descent_fall(uint8_t *m) {
    wl(m,0x34,rl(m,0x34)+asr(0xffea0000u-rl(m,0x34),5));
}
marco_descent_result_t marco_descent_hang(uint8_t *m,uint16_t random,uint32_t input) {
    uint32_t d0=0xffff0000u,d1=rl(m,0x40);
    if ((int32_t)(d1-0x1c00000u)>=0) d0-=asr(d1-0x1c00000u,5);
    d0=asr(d0-rl(m,0x34),3);wl(m,0x34,rl(m,0x34)+d0);
    d0=(d0&0xffff0000u)|random;                                            /* 0029C6: MOVE.W */
    d0=asr(d0-rl(m,0x30)-0x8000u,4);wl(m,0x30,rl(m,0x30)+d0);
    bool cut=(input&0x20u) || ((uint16_t)(m[0x40]<<8|m[0x41])<0x1c0u && (input&0x10u));
    if (!cut) return MARCO_DESCENT_MORE;
    wl(m,0x34,0x20000u);m[0x61]=2;                                         /* 0C167E */
    wl(m,0x50,0x2d0aa8u);m[0x90]=2;m[0x78]=0;wl(m,0x30,0);                 /* 0C16D8 */
    wl(m,0,MARCO_DESCENT_FALL);marco_descent_fall(m);
    return MARCO_DESCENT_CUT;
}
bool marco_descent_hang_after(uint8_t *m) {
    if (m[0x15]&0x40u) {m[0x61]=1;return true;}                          /* then 0C1F4A */
    m[0x78]=(int16_t)(m[0x34]<<8|m[0x35])>=-1?0xff:0;
    return false;
}
bool marco_descent_fall_after(uint8_t *m) {
    if (!(m[0x15]&0x40u)) return false;
    return true;                                                           /* then 0C1EBE */
}
