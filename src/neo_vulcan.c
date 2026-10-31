#include "neo_vulcan.h"
/* Manual translation of ROM 03B57C..03B808 (vulcan turret control); ROM
 * labels are provenance. */
static uint16_t rw(const uint8_t *m,unsigned o) {return (uint16_t)(m[o]<<8|m[o+1]);}
static void ww(uint8_t *m,unsigned o,uint16_t v) {m[o]=(uint8_t)(v>>8);m[o+1]=(uint8_t)v;}
static void pose(uint8_t *m) {
    /* 03B7FA: +88 = (+7C >> 3 rounded by the last bit out) & 1F. */
    uint8_t a=m[0x7c];
    m[0x88]=(uint8_t)((((int8_t)a>>3)+((a>>2)&1u))&0x1fu);
}
void neo_vulcan_step(uint8_t *m,const neo_vulcan_in_t *in,bool sweep_hit,neo_vulcan_out_t *out,bool *spark) {
    *out=(neo_vulcan_out_t){NEO_VULCAN_RUN,false,0,0,0,0,false,0};*spark=false;
    if (!in->creator_alive || (in->creator5f&0x80u) || in->anchor==0x8000u) {out->state=NEO_VULCAN_GONE;return;}
    if (in->anchor==0x8080u) {out->state=NEO_VULCAN_HOLD;return;}         /* 03B5A6 */
    ww(m,0x84,(uint16_t)(rw(m,0x84)>>1));                                 /* 03B5A8 lsr.w */
    if (in->rider<0) {                                                   /* 03B5B8 */
        m[0x78]=0;m[0x76]=0;m[0x77]=0;m[0x89]=0;pose(m);return;
    }
    unsigned p=(unsigned)in->rider&1u;
    ww(m,0x86,(uint16_t)p);m[0x5e]=(uint8_t)(1u<<p);
    out->angle_set=true;out->angle=m[0x7c];                               /* 0AABDC (byte +7C) */
    uint16_t d0=in->fresh;bool chase=false;
    if (d0&0x10u) {                                                      /* 03B5FC: A pressed */
        if (!m[0x77]) m[0x76]=m[0x74];
        m[0x77]=m[0x75];m[0x89]=8;
    } else {
        d0=in->held;
        if ((d0&0x10u) && in->repeat) {                                  /* 03B622 / 0CA614 */
            if (m[0x89]) m[0x89]--;                                       /* subq.b; bcc */
            else {m[0x89]=0;m[0x77]=m[0x75];chase=true;}
        }
    }
    uint16_t d1;
    if (!chase) {
        unsigned dir=(d0&0x0fu)*4u;                                       /* 03B65C */
        const uint8_t *t=in->aim+dir;
        uint32_t v=(uint32_t)t[0]<<24|(uint32_t)t[1]<<16|(uint32_t)t[2]<<8|t[3];
        if (v&0x80000000u) chase=true; else d1=(uint16_t)v;
    }
    if (chase) {d1=rw(m,0x7c);ww(m,0x82,d1);}                             /* 03B682 */
    uint16_t d2=rw(m,0x82);ww(m,0x82,d1);                                 /* 03B68A */
    if (d1!=d2) {ww(m,0x7e,0x800);ww(m,0x80,0);}
    uint16_t d7=(uint16_t)(rw(m,0x7e)+rw(m,0x80));                        /* 03B6A4 */
    d2=rw(m,0x7c);d1=(uint16_t)(d1-d2);
    if (d1) {
        if (d1==0x8000u) m[0x79]=(uint8_t)~m[0x79];
        else m[0x79]=(d1&0x8000u)?0xff:0;          /* spl on cmpi.w #8000: N of d1-8000 */
        if (m[0x79]) d7=(uint16_t)-d7;
        d2=(uint16_t)(d2+d7);d1=(uint16_t)(d1-d7);
        if ((d1^d7)&0x8000u) d2=(uint16_t)(d2+d1);                        /* overshoot: the target */
        ww(m,0x7c,d2);
        int16_t e=(int16_t)(uint16_t)(0xfa00u-rw(m,0x7e));ww(m,0x7e,(uint16_t)(rw(m,0x7e)+(uint16_t)(e>>2)));
        e=(int16_t)(uint16_t)(0x3000u-rw(m,0x80));ww(m,0x80,(uint16_t)(rw(m,0x80)+(uint16_t)(e>>4)));
    }
    uint8_t burst=m[0x77];                                               /* 03B700 */
    if (!burst) {m[0x78]=0;pose(m);return;}
    if (m[0x76]--) {pose(m);return;}                                     /* subq.b; bcc */
    m[0x77]=(uint8_t)(burst-1);m[0x78]=0xff;m[0x76]=(uint8_t)(m[0x76]+m[0x73]);
    out->fire=true;out->bullet=m[0x72]==5?0x1be24u:0x1b6a0u;out->bullet5e=m[0x5e];
    uint8_t spread=0,a=m[0x7c];
    if (a==m[0x7b]) {                                                    /* 03B774: same angle, spread */
        uint8_t k=(uint8_t)(m[0x7a]+1u);if (k>=3) k=0;
        m[0x7a]=k;spread=(uint8_t)((uint8_t)(k-1u)*2u);
    }
    m[0x7b]=a;out->bullet6e=(uint8_t)(a+spread);out->heading=a;
    if (sweep_hit && !rw(m,0x84)) {ww(m,0x84,0xff);*spark=true;}          /* 03B7C0 */
    pose(m);
}
