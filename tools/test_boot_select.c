#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "boot_select_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif

_Static_assert(offsetof(msx_boot_select_registers, sr) == 64, "register ABI");
static boot_header_t slots[256];
static uint32_t product_reads[256],io_hash,io_count;
static uint8_t selected,nv[65536];
static const boot_case_t *current;
static void byte(uint8_t value) { io_hash=(io_hash^value)*UINT32_C(16777619); }
static void event(uint8_t op,uint32_t address,uint16_t value) {
    byte(op);byte((uint8_t)(address>>24));byte((uint8_t)(address>>16));
    byte((uint8_t)(address>>8));byte((uint8_t)address);
    byte((uint8_t)(value>>8));byte((uint8_t)value);io_count++;
}
static uint8_t rb(void *ctx,uint32_t address) {
    (void)ctx;uint8_t value=0;
    switch(address) {
    case 0xd00108:value=current->previous_slot;break;
    case 0xd00047:value=current->scan_count;break;
    case 0x114:value=slots[selected].flag;break;
    default:assert(!"Unexpected boot import read8");
    }
    event(0,address,value);return value;
}
static uint16_t rw(void *ctx,uint32_t address) {
    (void)ctx;const boot_header_t *h=&slots[selected];uint16_t value=0;
    switch(address) {
    case 0x100:value=(uint16_t)(h->magic>>16);break;
    case 0x102:value=(uint16_t)h->magic;break;
    case 0x108:value=product_reads[selected]++?h->product_later:h->product;break;
    default:assert(!"Unexpected boot import read16");
    }
    event(1,address,value);return value;
}
static void wb(void *ctx,uint32_t address,uint8_t value) {
    (void)ctx;event(2,address,value);
    switch(address) {
    case 0x380021:selected=value;break;
    case 0xd00058:nv[0x58]=value;break;
    case 0x3a0003:case 0x3a001d:case 0x3a000d:case 0x3a0013:break;
    default:assert(!"Unexpected boot import write8");
    }
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    const msx_boot_select_imports imports={NULL,rb,rw,wb};
    for(unsigned n=0;n<BOOT_SELECT_CASES;n++) {
        current=&boot_cases[n];selected=0;io_count=0;io_hash=UINT32_C(2166136261);
        memset(product_reads,0,sizeof(product_reads));
        for(unsigned i=0;i<256;i++)slots[i]=current->header;
        for(unsigned i=0;i<current->count;i++) {
            const boot_override_t *override=&boot_overrides[current->first+i];
            slots[override->slot]=override->header;
        }
        for(unsigned i=0;i<sizeof(nv);i++)nv[i]=(uint8_t)(i*13u+n*29u+(i>>7));
        msx_boot_select_registers r=current->input;
        msx_boot_select(&r,&imports);
        assert(!memcmp(r.d,current->expected.d,sizeof(r.d)));
        assert(!memcmp(r.a,current->expected.a,sizeof(r.a)) && r.sr==current->expected.sr);
        assert(io_count==current->io_count && io_hash==current->io_hash);
        for(unsigned i=0;i<sizeof(nv);i++) {
            uint8_t expected=i==0x58?current->nv_result:(uint8_t)(i*13u+n*29u+(i>>7));
            assert(nv[i]==expected);
        }
    }
    printf("PASS: %u original boot-selector fixtures (synthetic platform imports)\n",BOOT_SELECT_CASES);
#ifdef __mips__
    debugf("MSX BOOTSELECT REGRESSION PASS cases=%u\n",BOOT_SELECT_CASES);
    console_render();while(1) {}
#endif
    return 0;
}
