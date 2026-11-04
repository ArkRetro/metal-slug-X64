#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ram_descriptors_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
_Static_assert(offsetof(msx_descriptor_registers,sr)==64,"register ABI");
static uint8_t ram[65536],expected[65536];
static uint32_t writes,write_hash;
static void byte(uint8_t v) { write_hash=(write_hash^v)*UINT32_C(16777619); }
static void event(uint8_t op,uint32_t a,uint16_t v) {
    byte(op);byte((uint8_t)(a>>24));byte((uint8_t)(a>>16));byte((uint8_t)(a>>8));
    byte((uint8_t)a);byte((uint8_t)(v>>8));byte((uint8_t)v);writes++;
}
static unsigned offset(uint32_t a,unsigned size) {
    assert(a>=0x10114a && a<=0x110000-size);return a-0x100000;
}
static uint8_t rb(void *ctx,uint32_t a) { (void)ctx;return ram[offset(a,1)]; }
static uint16_t rw(void *ctx,uint32_t a) {
    (void)ctx;assert(!(a&1));unsigned i=offset(a,2);
    return (uint16_t)(((uint16_t)ram[i]<<8)|ram[i+1]);
}
static void wb(void *ctx,uint32_t a,uint8_t v) {
    (void)ctx;event(0,a,v);ram[offset(a,1)]=v;
}
static void ww(void *ctx,uint32_t a,uint16_t v) {
    (void)ctx;assert(!(a&1));event(1,a,v);unsigned i=offset(a,2);
    ram[i]=(uint8_t)(v>>8);ram[i+1]=(uint8_t)v;
}
static int same(const msx_descriptor_registers *a,const msx_descriptor_registers *b) {
    return !memcmp(a->d,b->d,sizeof(a->d)) && !memcmp(a->a,b->a,sizeof(a->a)) && a->sr==b->sr;
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    const msx_descriptor_memory memory={NULL,rb,rw,wb,ww};
    msx_descriptor_registers r={0};
    for(unsigned n=0;n<DESCRIPTOR_CASES;n++) {
        const descriptor_case_t *c=&descriptor_cases[n];
        if(c->chain_from<0) {
            for(unsigned i=0;i<sizeof(ram);i++)ram[i]=(uint8_t)(i*37u+(n+1u)*19u+(i>>5)*7u);
            r=c->input;
        } else {
            assert((unsigned)c->chain_from+1u==n && c->input_count==0);
            assert(same(&r,&c->input)); /* Keep actual native output, no reinjection. */
        }
        for(unsigned i=0;i<c->input_count;i++) {
            const descriptor_patch_t *p=&descriptor_patches[c->input_first+i];ram[p->offset]=p->value;
        }
        memcpy(expected,ram,sizeof(ram));
        for(unsigned i=0;i<c->output_count;i++) {
            const descriptor_patch_t *p=&descriptor_patches[c->output_first+i];expected[p->offset]=p->value;
        }
        writes=0;write_hash=UINT32_C(2166136261);
        msx_descriptor_result result={MSX_DESCRIPTOR_DONE,0,0};
        switch(c->mode) {
        case 0:msx_descriptor_allocate(&r,&memory);break;
        case 1:msx_descriptor_remove(&r,&memory);break;
        case 2:result=msx_descriptor_dispatch(&r,&memory);break;
        default:assert(!"Invalid descriptor fixture");
        }
        assert(same(&r,&c->expected) && !memcmp(ram,expected,sizeof(ram)));
        assert(writes==c->writes && write_hash==c->write_hash);
        assert(result.status==c->result.status && result.boundary==c->result.boundary && result.target==c->result.target);
    }
    printf("PASS: %u original RAM descriptor transitions\n",DESCRIPTOR_CASES);
#ifdef __mips__
    debugf("MSX DESCRIPTORS REGRESSION PASS cases=%u\n",DESCRIPTOR_CASES);
    console_render();while(1) {}
#endif
    return 0;
}
