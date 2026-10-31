#include "neo_fade.h"
/* Manual translation of ROM 009C0A..009CBC and 00698C..0069E8 (labels are
 * provenance only). */
static int16_t clamp31(int16_t v) {return v>0x1f?0x1f:v<-0x1f?-0x1f:v;}
void neo_fade_start(neo_fade_t *f,int16_t d0,int16_t d1,int16_t d2,uint16_t d3) {
    int16_t t[3]={clamp31(d0),clamp31(d1),clamp31(d2)};
    for (unsigned i=0;i<3;i++) f->target[i]=(int16_t)(uint16_t)((uint16_t)t[i]<<8);
    uint8_t n=(uint8_t)d3;if (n<2) n=1;
    f->count=n;
    int16_t diff[3];uint16_t any=0;
    for (unsigned i=0;i<3;i++) {diff[i]=(int16_t)(uint16_t)((uint16_t)f->target[i]-(uint16_t)f->current[i]);any|=(uint16_t)diff[i];}
    if (!any) {f->count=0;return;}                        /* 009C94: d3 = 0 */
    /* divs.w: 32/16 signed, quotient toward zero (|diff| < 0x3F00, no overflow). */
    for (unsigned i=0;i<3;i++) f->step[i]=(int16_t)((int32_t)diff[i]/(int32_t)n);
}
bool neo_fade_step(neo_fade_t *f) {
    if (!f->count) return false;
    int8_t before[3];for (unsigned i=0;i<3;i++) before[i]=neo_fade_offset(f,i);
    if (!--f->count) for (unsigned i=0;i<3;i++) f->current[i]=f->target[i];
    else for (unsigned i=0;i<3;i++) f->current[i]=(int16_t)(uint16_t)((uint16_t)f->current[i]+(uint16_t)f->step[i]);
    bool changed=false;
    for (unsigned i=0;i<3;i++) changed|=before[i]!=neo_fade_offset(f,i);
    return changed;
}
