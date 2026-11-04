#include "condition_consumers.h"
#include <assert.h>
static uint8_t rb(const msx_condition_bus *b,uint32_t a) {return b->read8(b->context,a&UINT32_C(0xffffff));}
static uint16_t rw(const msx_condition_bus *b,uint32_t a) {return b->read16(b->context,a&UINT32_C(0xffffff));}
static void ww(const msx_condition_bus *b,uint32_t a,uint16_t v) {b->write16(b->context,a&UINT32_C(0xffffff),v);}
static void nz(msx_condition_registers *r,uint32_t value,uint32_t sign) {
    r->sr=(uint16_t)((r->sr&0xfff0u)|(value==0?4u:0u)|(value&sign?8u:0u));
}
static void advance(msx_condition_registers *r,const msx_condition_bus *b) {
    r->d[0]=rb(b,r->a[0]+1u); /* MOVEQ #0 then MOVE.B, no retained high byte. */
    nz(r,r->d[0],0x80u);r->a[0]+=r->d[0]; /* ADDA.W with 0..255 does not affect CCR. */
}
static msx_native_condition_status condition(msx_condition_registers *r,const msx_condition_bus *b,uint32_t continuation) {
    r->a[7]-=4u;ww(b,r->a[7],(uint16_t)(continuation>>16));ww(b,r->a[7]+2u,(uint16_t)continuation);
    msx_native_condition_status status=msx_condition_evaluate_native(r,b);
    if(status==MSX_NATIVE_CONDITION_OK)r->a[7]+=4u; /* Original outer 01A5DC RTS. */
    return status;
}
msx_consumer_result msx_condition_consume(msx_condition_consumer entry,msx_condition_registers *r,const msx_condition_bus *b) {
    assert(r && b && b->read8 && b->read16 && b->write8 && b->write16);
    assert(entry>=MSX_CONSUMER_1A01E && entry<=MSX_CONSUMER_1A4F2);
    r->a[1]=r->a[0]+(entry==MSX_CONSUMER_1A4F2?10u:2u);
    if(entry==MSX_CONSUMER_1A2D8)advance(r,b);
    msx_native_condition_status status=condition(r,b,entry==MSX_CONSUMER_1A01E?0x1a026u:entry==MSX_CONSUMER_1A2D8?0x1a2e8u:0x1a4fau);
    if(status!=MSX_NATIVE_CONDITION_OK)return (msx_consumer_result){status,status==MSX_NATIVE_CONDITION_TRAP15?0x0aaaccu:0};
    bool carry=(r->sr&1u)!=0;
    if(entry==MSX_CONSUMER_1A01E) {
        if(!carry)return (msx_consumer_result){status,0x1a02au};
        advance(r,b);return (msx_consumer_result){status,0x1a056u};
    }
    if(entry==MSX_CONSUMER_1A2D8)return (msx_consumer_result){status,carry?0x1a0a6u:0x1a372u};
    if(!carry)return (msx_consumer_result){status,0x4188u};
    uint16_t timer=rw(b,r->a[6]+0x72u);nz(r,timer,0x8000u);
    if(timer) {
        uint16_t value=(uint16_t)(timer-1u);ww(b,r->a[6]+0x72u,value);
        r->sr=(uint16_t)((r->sr&0xffe0u)|(value==0?4u:0u)|(value&0x8000u?8u:0u)|(((timer^1u)&(timer^value)&0x8000u)?2u:0u));
        return (msx_consumer_result){status,0x1a510u};
    }
    ww(b,r->a[6]+0x72u,rw(b,r->a[0]+4u));
    advance(r,b);b->write8(b->context,(r->a[6]+0x74u)&UINT32_C(0xffffff),0);nz(r,0,0x80u);
    return (msx_consumer_result){status,0x1a526u};
}
