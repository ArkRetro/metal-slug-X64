#include "neo_player_positions.h"
#include <assert.h>
static void low_word(uint32_t *d,uint16_t value) {
    *d=(*d&UINT32_C(0xffff0000))|value;
}
static void flags(neo_player_positions_registers_t *r,uint16_t value) {
    r->sr=(uint16_t)((r->sr&0xfff0u)|(value==0?4u:0u)|(value&0x8000u?8u:0u));
}
void neo_player_positions(const neo_player_positions_frame_t *f,neo_player_positions_registers_t *r) {
    assert(f && r);
    for(unsigned p=0;p<2;p++) {
        low_word(&r->d[p*2],f->fallback_x);
        low_word(&r->d[p*2+1],f->fallback_y);
    }
    if(!f->enabled) {
        r->d[7]=0;flags(r,0);return; /* 019050: MOVEQ, not MOVE.W. */
    }
    uint16_t signature=0;
    for(unsigned p=0;p<2;p++) {
        const neo_position_player_t *actor=&f->players[p];
        if(actor->object_header==UINT32_C(0x3f1e))continue;
        signature|=p==0?0xff00u:0x00ffu;
        if(actor->use_x)low_word(&r->d[p*2],actor->x);
        if(actor->use_y)low_word(&r->d[p*2+1],actor->y);
    }
    low_word(&r->d[7],signature);
    if(!(signature&0xffu)) {
        low_word(&r->d[2],(uint16_t)r->d[0]);low_word(&r->d[3],(uint16_t)r->d[1]);
    }
    flags(r,signature); /* TST.W D7; BMI consumes its sign. */
    if(!(signature&0x8000u)) {
        low_word(&r->d[0],(uint16_t)r->d[2]);low_word(&r->d[1],(uint16_t)r->d[3]);
        flags(r,(uint16_t)r->d[3]);
    }
}
