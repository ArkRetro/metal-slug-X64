#include "ram_descriptors.h"
#include <assert.h>

static uint32_t physical(uint32_t a) { return a & UINT32_C(0xffffff); }
static uint8_t rb(const msx_descriptor_memory *m,uint32_t a) {
    return m->read8(m->context,physical(a));
}
static uint16_t rw(const msx_descriptor_memory *m,uint32_t a) {
    return m->read16(m->context,physical(a));
}
static uint32_t rl(const msx_descriptor_memory *m,uint32_t a) {
    uint32_t high=rw(m,a);
    return (high<<16)|rw(m,a+2u);
}
static void wb(const msx_descriptor_memory *m,uint32_t a,uint8_t v) {
    m->write8(m->context,physical(a),v);
}
static void ww(const msx_descriptor_memory *m,uint32_t a,uint16_t v) {
    m->write16(m->context,physical(a),v);
}
static void wl(const msx_descriptor_memory *m,uint32_t a,uint32_t v) {
    ww(m,a,(uint16_t)(v>>16));ww(m,a+2u,(uint16_t)v);
}
static void low8(uint32_t *r,uint8_t v) { *r=(*r&UINT32_C(0xffffff00))|v; }
static void low16(uint32_t *r,uint16_t v) { *r=(*r&UINT32_C(0xffff0000))|v; }
static void flags8(msx_descriptor_registers *r,uint8_t v,uint16_t x) {
    r->sr=(uint16_t)((r->sr&0xffe0u)|x|(v==0?4u:0u)|(v&0x80u?8u:0u));
}
static void push(const msx_descriptor_memory *m,msx_descriptor_registers *r,uint32_t v) {
    r->a[7]-=4u;
    /* MOVEM predecrement in the reference writes low word then high word. */
    ww(m,r->a[7]+2u,(uint16_t)v);ww(m,r->a[7],(uint16_t)(v>>16));
}
static uint32_t pop(const msx_descriptor_memory *m,msx_descriptor_registers *r) {
    uint32_t v=rl(m,r->a[7]);r->a[7]+=4u;return v;
}
static void contract(const msx_descriptor_memory *m,const msx_descriptor_registers *r) {
    assert(m && r && m->read8 && m->read16 && m->write8 && m->write16);
}

void msx_descriptor_allocate(msx_descriptor_registers *r,const msx_descriptor_memory *m) {
    contract(m,r);
    uint8_t available=rb(m,r->a[5]+0x7ce9u);           /* 10082E TST.B */
    if(!available) { flags8(r,0,r->sr&0x10u);return; }
    /* MOVEM.L D1-D3/A2,-(SP): RAM saves, not host-local register snapshots. */
    push(m,r,r->a[2]);push(m,r,r->d[3]);push(m,r,r->d[2]);push(m,r,r->d[1]);
    r->d[1]=UINT32_C(0x10fc00);
    r->a[2]=r->a[4];r->d[1]=r->a[4];
    if((uint8_t)r->d[0]<rb(m,r->a[2]+6u)) {
        do {
            low8(&r->d[1],rb(m,r->a[2]+4u));r->a[2]=r->d[1];
        } while((uint8_t)r->d[0]<rb(m,r->a[2]+6u));
        r->a[1]=r->a[5]+0x7ce0u;r->d[2]=0;
        low8(&r->d[2],rb(m,r->a[5]+0x7ce7u));
        low8(&r->d[1],rb(m,r->a[1]+(uint16_t)r->d[2]));r->a[1]=r->d[1];
        low16(&r->d[3],(uint16_t)r->a[2]);wb(m,r->a[1]+4u,(uint8_t)r->d[3]);
        low8(&r->d[1],rb(m,r->a[2]+5u));wb(m,r->a[1]+5u,(uint8_t)r->d[1]);
        low16(&r->d[3],(uint16_t)r->a[1]);wb(m,r->a[2]+5u,(uint8_t)r->d[3]);
        r->a[2]=r->d[1];wb(m,r->a[2]+4u,(uint8_t)r->d[3]);
    } else {
        do {
            low8(&r->d[1],rb(m,r->a[2]+5u));r->a[2]=r->d[1];
        } while((uint8_t)r->d[0]>=rb(m,r->a[2]+6u));
        r->a[1]=r->a[5]+0x7ce0u;r->d[2]=0;
        low8(&r->d[2],rb(m,r->a[5]+0x7ce7u));
        low8(&r->d[1],rb(m,r->a[1]+(uint16_t)r->d[2]));r->a[1]=r->d[1];
        low16(&r->d[3],(uint16_t)r->a[2]);wb(m,r->a[1]+5u,(uint8_t)r->d[3]);
        low8(&r->d[1],rb(m,r->a[2]+4u));wb(m,r->a[1]+4u,(uint8_t)r->d[1]);
        low16(&r->d[3],(uint16_t)r->a[1]);wb(m,r->a[2]+4u,(uint8_t)r->d[3]);
        r->a[2]=r->d[1];wb(m,r->a[2]+5u,(uint8_t)r->d[3]);
    }
    wb(m,r->a[1]+6u,(uint8_t)r->d[0]);wl(m,r->a[1],r->a[0]);
    low8(&r->d[2],(uint8_t)((uint8_t)r->d[2]+1u));
    if((uint16_t)r->d[2]>=7u)low8(&r->d[2],0);
    wb(m,r->a[5]+0x7ce7u,(uint8_t)r->d[2]);
    uint8_t old=rb(m,r->a[5]+0x7ce9u),value=(uint8_t)(old-1u);
    wb(m,r->a[5]+0x7ce9u,value);
    flags8(r,value,old==0?0x10u:0u);
    r->sr=(uint16_t)(r->sr|(old==0?1u:0u)|(old==0x80u?2u:0u));
    r->d[1]=pop(m,r);r->d[2]=pop(m,r);r->d[3]=pop(m,r);r->a[2]=pop(m,r);
}

void msx_descriptor_remove(msx_descriptor_registers *r,const msx_descriptor_memory *m) {
    contract(m,r);
    push(m,r,r->a[1]);push(m,r,r->a[0]);push(m,r,r->d[0]);
    r->d[0]=UINT32_C(0x10fc00);
    low8(&r->d[0],rb(m,r->a[4]+4u));r->a[0]=r->d[0];
    low8(&r->d[0],rb(m,r->a[4]+5u));r->a[1]=r->d[0];
    wb(m,r->a[0]+5u,rb(m,r->a[4]+5u));
    wb(m,r->a[1]+4u,rb(m,r->a[4]+4u));
    wb(m,r->a[5]+0x7ce9u,(uint8_t)(rb(m,r->a[5]+0x7ce9u)+1u));
    uint8_t cursor=rb(m,r->a[5]+0x7ce8u);
    low8(&r->d[0],(uint8_t)(cursor+1u));
    if((uint8_t)r->d[0]==7u)r->d[0]=0;
    wb(m,r->a[5]+0x7ce8u,(uint8_t)r->d[0]);
    flags8(r,(uint8_t)r->d[0],cursor==0xffu?0x10u:0u);
    r->d[0]=pop(m,r);r->a[0]=pop(m,r);r->a[1]=pop(m,r);
}

msx_descriptor_result msx_descriptor_dispatch(msx_descriptor_registers *r,
                                              const msx_descriptor_memory *m) {
    contract(m,r);
    r->a[4]=r->a[5]+0x7c00u;r->a[5]=UINT32_C(0x108000); /* Preserve LEA order. */
    for(;;) {
        r->a[0]=rl(m,r->a[4]);
        uint32_t callback=physical(r->a[0]);
        if(callback!=UINT32_C(0x10008c) && callback!=UINT32_C(0x100158))
            return (msx_descriptor_result){MSX_DESCRIPTOR_PENDING_CALLBACK,0x10081au,r->a[0]};
        /* The reference JSR uses normal longword order, unlike MOVEM above. */
        r->a[7]-=4u;wl(m,r->a[7],UINT32_C(0x10081c));
        if(callback==UINT32_C(0x10008c)) {
            ww(m,r->a[4]+0x1cu,0);wb(m,r->a[4]+0x16u,0);
            ww(m,r->a[4]+0x1eu,0);wb(m,r->a[5]+0x7cecu,0);
            wl(m,r->a[4],UINT32_C(0x1000a4));
            r->sr=(uint16_t)((r->sr&0xfff0u));         /* MOVE.L #1000A4,(A4) */
        } /* 100158 is RTS only, with no register/CCR changes. */
        uint32_t target=rl(m,r->a[7]);
        if(target!=UINT32_C(0x10081c))
            return (msx_descriptor_result){MSX_DESCRIPTOR_PENDING_RETURN,
                callback==UINT32_C(0x10008c)?0x1000a2u:0x100158u,target};
        r->a[7]+=4u;                                  /* Return of native binding. */
        low8(&r->d[0],rb(m,r->a[4]+5u));
        flags8(r,(uint8_t)r->d[0],r->sr&0x10u);
        if(!(uint8_t)r->d[0])
            return (msx_descriptor_result){MSX_DESCRIPTOR_DONE,0x10082cu,0};
        r->d[1]=r->a[4];
        low8(&r->d[1],(uint8_t)r->d[0]);r->a[4]=r->d[1];
    }
}
