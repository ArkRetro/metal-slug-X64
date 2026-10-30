#include "rom_01a5ca.h"
#include <assert.h>
/* Structured static translation of 01A5CA..01A7F2. No instruction dispatch.
 * The observed table has 22 entries / 14 distinct handlers. Bytes >=22
 * dispatch beyond that table in the original; this recovery rejects them
 * explicitly rather than inventing destinations. Logical names stay neutral. */
enum { C=1, V=2, Z=4, N=8, X=16 };
static uint32_t addr(uint32_t a) { return a & UINT32_C(0xffffff); }
static uint8_t rb(const msx_condition_bus *b, uint32_t a) {
    return b->read8(b->context, addr(a));
}
static uint16_t rw(const msx_condition_bus *b, uint32_t a) {
    return b->read16(b->context, addr(a));
}
static uint32_t rl(const msx_condition_bus *b, uint32_t a) {
    uint32_t high=rw(b,a); return (high<<16)|rw(b,a+2u);
}
static void wb(const msx_condition_bus *b, uint32_t a, uint8_t v) {
    b->write8(b->context,addr(a),v);
}
static void wl(const msx_condition_bus *b, uint32_t a, uint32_t v) {
    b->write16(b->context,addr(a),(uint16_t)(v>>16));
    b->write16(b->context,addr(a+2u),(uint16_t)v);
}
static void push(msx_condition_registers *s,const msx_condition_bus *b,uint32_t v) {
    s->a[7]-=4u;wl(b,s->a[7],v);
}
static uint32_t pop(msx_condition_registers *s,const msx_condition_bus *b) {
    uint32_t v=rl(b,s->a[7]);s->a[7]+=4u;return v;
}
static void nz(msx_condition_registers *s,uint32_t value,uint32_t sign) {
    s->sr=(s->sr&0xfff0u)|(value==0?Z:0)|(value&sign?N:0);
}
static void byte(msx_condition_registers *s,unsigned i,uint8_t v) {
    s->d[i]=(s->d[i]&UINT32_C(0xffffff00))|v;nz(s,v,0x80u);
}
static void word(msx_condition_registers *s,unsigned i,uint16_t v) {
    s->d[i]=(s->d[i]&UINT32_C(0xffff0000))|v;nz(s,v,0x8000u);
}
static void quick(msx_condition_registers *s,unsigned i,uint32_t v) {
    s->d[i]=v;nz(s,v,UINT32_C(0x80000000));
}
static void addw(msx_condition_registers *s,unsigned i,uint16_t b) {
    uint16_t a=(uint16_t)s->d[i],r=(uint16_t)(a+b);
    unsigned carry=(uint32_t)a+b>0xffffu;
    word(s,i,r);s->sr=(s->sr&0xffe0u)|(r==0?Z:0)|(r&0x8000u?N:0)|
        (carry?(C|X):0)|((~(a^b)&(a^r)&0x8000u)?V:0);
}
static void cmpw(msx_condition_registers *s,uint16_t a,uint16_t b) {
    uint16_t r=(uint16_t)(a-b);
    s->sr=(s->sr&0xfff0u)|(r==0?Z:0)|(r&0x8000u?N:0)|
        (a<b?C:0)|(((a^b)&(a^r)&0x8000u)?V:0);
}
static void roxrw(msx_condition_registers *s,unsigned i) {
    uint16_t a=(uint16_t)s->d[i];unsigned oldx=(s->sr&X)!=0;
    uint16_t r=(uint16_t)((a>>1)|(oldx<<15));word(s,i,r);
    s->sr=(s->sr&0xffe0u)|(r==0?Z:0)|(r&0x8000u?N:0)|((a&1u)?(C|X):0);
}
static bool call(msx_condition_registers *s,const msx_condition_bus *b,
                 msx_condition_helper id,uint32_t continuation) {
    if(!b->helper)return false;
    push(s,b,continuation);
    if(!b->helper(b->context,id,s))return false;
    (void)pop(s,b);return true;
}
msx_condition_status msx_rom_01a5ca(msx_condition_registers *s,const msx_condition_bus *b) {
    assert(s && b && b->read8 && b->read16 && b->write8 && b->write16);
    quick(s,0,0);byte(s,0,rb(b,s->a[1]));
    unsigned op=(uint8_t)s->d[0];
    if(op>=22)return MSX_CONDITION_UNSUPPORTED_INDEX;
    word(s,1,(uint16_t)s->d[0]);addw(s,0,(uint16_t)s->d[0]);addw(s,0,(uint16_t)s->d[0]);
    push(s,b,s->a[0]);nz(s,s->a[0],UINT32_C(0x80000000)); /* MOVE.L at 01A5D4 */
    push(s,b,0x1a5dau);                               /* JSR at 01A5D6 */
    if(op<=9) {
        if(op<9) {
            s->a[2]=s->a[5]+0x4326u;quick(s,0,0);
            byte(s,0,rb(b,s->a[2]+(uint16_t)s->d[1]));
        } else {
            quick(s,0,0);quick(s,1,0);s->a[2]=s->a[5]+0x4326u;
            for(unsigned i=0;i<8;i++) {byte(s,1,rb(b,s->a[2]));s->a[2]++;addw(s,0,(uint16_t)s->d[1]);}
        }
        push(s,b,0);nz(s,0,UINT32_C(0x80000000));
        cmpw(s,(uint16_t)s->d[0],rw(b,s->a[1]+2u));
        wb(b,s->a[7],s->sr&Z?255:0);
        wb(b,s->a[7]+1u,!(s->sr&(C|Z))?255:0);
        wb(b,s->a[7]+2u,s->sr&C?255:0);
        wb(b,s->a[7]+3u,!(s->sr&Z)?255:0);
        byte(s,0,rb(b,s->a[1]+1u));
        /* MOVE.B retains the high byte of D0.word. Preserve the original
         * signed indexed stack read even for a sum >=256 or a relation >3. */
        uint32_t offset=(uint16_t)s->d[0];
        if(offset&0x8000u)offset|=UINT32_C(0xffff0000);
        nz(s,rb(b,s->a[7]+offset),0x80u);
        if(!(s->sr&Z))s->sr|=C;
        s->a[7]+=4u;
    } else if(op==10) {
        s->sr|=C;                                    /* 01A698 */
    } else if(op<=19) {
        static const uint32_t continuations[9]={0x1a6a4,0x1a6c4,0x1a6e4,
            0x1a708,0x1a728,0x1a748,0x1a76c,0x1a77e,0x1a796};
        if(!call(s,b,MSX_QUERY_1903A,continuations[op-11]))return MSX_CONDITION_MISSING_HELPER;
        bool result=false;
        if(op==19) {
            addw(s,7,1);result=(s->sr&Z)!=0;
        } else {
            nz(s,(uint16_t)s->d[7],0x8000u);
            if(op==17)result=(s->sr&Z)!=0;
            else if(!(s->sr&Z)) {
                if(op==18) {addw(s,7,1);result=!(s->sr&Z);}
                else {
                    unsigned axis=op>=14?1:0;
                    if(op==11 || op==12 || op==14 || op==15) {
                        addw(s,axis,(uint16_t)s->d[axis+2]);roxrw(s,axis);
                    }
                    word(s,5,rw(b,s->a[5]+(axis?0x4332u:0x4330u)));
                    cmpw(s,(uint16_t)s->d[axis],(uint16_t)s->d[5]);
                    if(op==13 || op==16) {
                        s->d[6]=(s->d[6]&UINT32_C(0xffffff00))|(!(s->sr&N)?255u:0u);
                        cmpw(s,(uint16_t)s->d[axis+2],(uint16_t)s->d[5]);
                        s->d[7]=(s->d[7]&UINT32_C(0xffffff00))|(!(s->sr&N)?255u:0u);
                        byte(s,7,(uint8_t)(s->d[7]^s->d[6]));
                        result=op==13?(s->sr&N)!=0:!(s->sr&Z);
                    } else result=(op==11 || op==15)?(s->sr&N)!=0:!(s->sr&N);
                }
            }
        }
        s->sr=(s->sr&~C)|(result?C:0);                /* shared 01A7EE / ORI */
    } else {
        push(s,b,s->a[0]);nz(s,s->a[0],UINT32_C(0x80000000));
        quick(s,0,0);
        if(!call(s,b,MSX_QUERY_AAA88,op==20?0x1a7ac:0x1a7d2))return MSX_CONDITION_MISSING_HELPER;
        nz(s,(uint8_t)s->d[0],0x80u);
        if(s->sr&Z) {
            quick(s,0,1);
            if(!call(s,b,MSX_QUERY_AAA88,op==20?0x1a7ba:0x1a7e0))return MSX_CONDITION_MISSING_HELPER;
        }
        s->a[0]=pop(s,b);nz(s,(uint8_t)s->d[0],0x80u);
        bool result=op==20?!(s->sr&Z):(s->sr&Z)!=0;
        s->sr=(s->sr&~C)|(result?C:0);
    }
    (void)pop(s,b);                                  /* handler RTS */
    s->a[0]=pop(s,b);                                /* 01A5DA */
    return MSX_CONDITION_OK;                         /* before 01A5DC RTS */
}
