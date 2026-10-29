#include "neo_control.h"
#include "neo_motion.h"
#include "control_tables.h"
#include <assert.h>
static int s16(uint16_t v) {return (int)(v&32767u)-(v&32768u?32768:0);}
static uint16_t asr16(uint16_t v,unsigned n) {return (uint16_t)neo_asr(s16(v),n);}
static uint16_t abs16(uint16_t v) {return v&32768u?(uint16_t)(0u-v):v;}
static int32_t s32(uint32_t v) {return v<=0x7fffffffu?(int32_t)v:(int32_t)((int64_t)v-0x100000000LL);}
static uint32_t abs32(uint32_t v) {return v&0x80000000u?0u-v:v;}
static uint16_t target_delta(uint16_t x,uint16_t y,const neo_control_t *s,
    const neo_control_frame_t *f,bool selected) {
    uint16_t dy=asr16(abs16((uint16_t)(y-f->y)),1);
    uint16_t radius=dy>96u?0:(uint16_t)(96u-dy);
    uint16_t dx=(uint16_t)(x-((uint32_t)s->target84>>16));
    dx=(uint16_t)(dx+(dx&32768u?radius:0u-radius));
    if (!selected) dx=(uint16_t)(dx-asr16(dx,2));
    return dx;
}
void neo_control_target(neo_control_t *s,const neo_control_frame_t *f) {
    uint32_t delta=0;
    if ((uint16_t)(f->x-32u)<256u) {
        if (f->targets_valid) {
            uint16_t p1x=f->p1x,p1y=f->p1y,p2x=f->p2x,p2y=f->p2y;
            if (!(f->active_mask&2u)) {p2x=p1x;p2y=p1y;}
            if (!(f->active_mask&1u)) {p1x=p2x;p1y=p2y;}
            uint16_t a=target_delta(p1x,p1y,s,f,s->selected80==0);
            uint16_t b=target_delta(p2x,p2y,s,f,s->selected80==1);
            uint16_t chosen=abs16(a)>=abs16(b)?a:b;
            if ((uint16_t)(chosen+20u)<40u) chosen=(uint16_t)(0u-chosen);
            delta=(uint32_t)chosen<<16;
        }
        if (s->delay92) {
            uint16_t dy=abs16((uint16_t)(f->y-s->target90-f->origin_y));
            uint16_t radius=dy>s->delay92?0:(uint16_t)(s->delay92-dy);
            uint16_t dx=(uint16_t)(s->target8e+f->origin_x-f->x);
            if (!(dx&32768u)) {dx=(uint16_t)(dx-radius);if (!(dx&32768u)) dx=0;}
            else {dx=(uint16_t)(dx+radius);if (dx&32768u) dx=0;}
            uint32_t candidate=(uint32_t)dx<<16;
            if (abs32(candidate)>=abs32(delta)) delta=candidate;
        }
    }
    uint32_t center=0xa00000u-(uint32_t)s->target84;
    if (center&0x80000000u) {center+=0x600000u;if (!(center&0x80000000u)) center=0;}
    else {center-=0x600000u;if (center&0x80000000u) center=0;}
    delta+=center;
    s->phase8c=(uint16_t)(s->phase8c+0x13fu);
    uint32_t impulse=(uint32_t)s->phase8c<<3;
    uint32_t velocity=(uint32_t)s->velocity88;
    if (!(velocity&0x80000000u)) impulse=0u-impulse;
    uint32_t correction=(uint32_t)neo_asr(s32(velocity+impulse),5);
    velocity+=(uint32_t)neo_asr(s32(delta-velocity),7)-correction;
    s->velocity88=s32(velocity);s->target84=s32((uint32_t)s->target84+velocity);
}
uint16_t neo_control_probability(const neo_control_t *s,const neo_control_frame_t *f) {
    if (s->variant68>=12 || f->active_mask>3) return 0xffffu;
    unsigned rank=f->mode88==2?f->rank:3u;
    unsigned mask=(1u<<((rank&7u)+1u))-1u;
    unsigned row=control_rank_lookup[control_probability_mask&mask];
    if (!row || row>8) return 0xffffu;
    unsigned coop=f->active_mask==3u;
    // 026128..026144 multiply the base index by 12 BYTES, then add
    // variant*2 bytes. Keep byte/word units distinct; no assumed row layout.
    return control_probability[((row-1)*2+coop)*6+s->variant68];
}
bool neo_control_prefix(neo_control_t *s,const neo_control_frame_t *f,neo_rng_t *rng) {
    uint16_t threshold=neo_control_probability(s,f);
    if (threshold==0xffffu || !f->delay_grid) return false;
    if (!f->active_mask) s->selected80=255;
    else {
        uint16_t x=f->p1x;
        if (f->active_mask==1) s->selected80=0;
        else if (f->active_mask==2) {s->selected80=1;x=f->p2x;}
        else {
            uint16_t a=abs16((uint16_t)(f->p1x-f->x)),b=abs16((uint16_t)(f->p2x-f->x));
            if (!s->selected80) {a>>=2;b=(uint16_t)(b<<4);}
            else {a=(uint16_t)(a<<4);b>>=2;}
            uint16_t sum=(uint16_t)(a+b);
            uint16_t choice=(uint16_t)(((uint32_t)neo_rng_next(rng)*sum)>>16);
            s->selected80=b>=choice?0:1;x=s->selected80?f->p2x:f->p1x;
        }
        bool positive=!((uint16_t)(x-f->x)&32768u),facing=s->flags15&1u;
        if (positive==facing) s->flags7c|=1u;else s->flags7c&=254u;
    }
    if (s->timer70) --s->timer70;
    s->flags7c&=253u;
    uint16_t random=neo_rng_next(rng);
    if ((random&255u)<threshold && (random>>8)<threshold) {
        s->flags7c|=2u;
        uint16_t x=(uint16_t)(f->x+((s->flags15&1u)?48u:0u-48u));
        uint16_t gx=(uint16_t)(x-32u),gy=(uint16_t)(f->y-256u),value=0;
        if (gx<512u && gy<256u) {
            unsigned index=((gx>>1)&240u)+((gy>>4)&14u);
            value=f->delay_grid[index/2];if (value>=256u) value=255;
        }
        if (value>s->delay92) {s->delay92=value;s->target8e=(uint16_t)(x-f->origin_x);s->target90=(uint16_t)(f->y-f->origin_y);}
    }
    if (s->selector6e==1) s->flags5f|=48u;
    else if (s->selector6e) {
        neo_rng_result_t random_flags=neo_rng_next_result(rng);
        s->flags5f=(uint8_t)((s->flags5f&207u)|(random_flags.mixed_word&32768u?16u:32u));
    }
    if (s->counter74) --s->counter74;
    if (neo_rng_next_result(rng).mixed_word&32768u) s->flags7c^=128u;
    if ((uint16_t)(f->x-32u)<256u) s->flags7c|=8u;else s->flags7c&=247u;
    if (s->delay92) {uint16_t step=asr16(s->delay92,6);if (!step) step=1;s->delay92=(uint16_t)(s->delay92-step);}
    return true;
}
void neo_control_finish(neo_control_t *s,uint32_t a,uint32_t b) {
    s->flags7c&=139u; // clear bits 2,4,5,6 before the two original probe results.
    if (a&16u) s->flags7c|=64u;
    else {if (a&256u) s->flags7c|=16u;if (a&4u) s->flags7c|=32u;}
    if (b&4u) s->flags7c|=4u;
}
