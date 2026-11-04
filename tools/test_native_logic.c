#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../decomp/recovered/rom_01a5ca.h"
#include "../src/neo_player_query.h"
#include "../src/neo_player_positions.h"
#include "native_logic_fixture.h"
#include "../decomp/recovered/condition_services.h"
#include "native_conditions_fixture.h"
#ifdef __mips__
#include <libdragon.h>
#endif
static uint8_t ram[65536],expected_ram[65536];
static const logic_case_t *current;
static unsigned call_at;
static uint8_t rb(void *ctx,uint32_t a) {(void)ctx;assert(a>=0x100000 && a<0x110000);return ram[a-0x100000];}
static uint16_t rw(void *ctx,uint32_t a) {return (uint16_t)(((uint16_t)rb(ctx,a)<<8)|rb(ctx,a+1u));}
static void wb(void *ctx,uint32_t a,uint8_t v) {(void)ctx;assert(a>=0x100000 && a<0x110000);ram[a-0x100000]=v;}
static void ww(void *ctx,uint32_t a,uint16_t v) {wb(ctx,a,(uint8_t)(v>>8));wb(ctx,a+1u,(uint8_t)v);}
static void input_word(unsigned a,uint16_t v) {ram[a]=(uint8_t)(v>>8);ram[a+1]=(uint8_t)v;}
static bool helper(void *ctx,msx_condition_helper kind,msx_condition_registers *s) {
    (void)ctx;assert(call_at<current->call_count);
    const logic_call_t *wanted=&logic_calls[current->call_first+call_at++];
    uint32_t ret=((uint32_t)rw(NULL,s->a[7])<<16)|rw(NULL,s->a[7]+2u);
    assert((unsigned)kind==wanted->kind && s->d[0]==wanted->d0 && s->a[0]==wanted->a0);
    assert(s->a[7]==wanted->sp && s->sr==wanted->sr && ret==wanted->ret);
    uint32_t argument=s->d[0];memcpy(s->d,current->helper_d,sizeof(s->d));
    if(kind==MSX_QUERY_AAA88)s->d[0]=current->query[argument!=0];
    s->a[0]=current->ha0;s->a[2]=current->ha2;s->sr=current->helper_sr;
    return true;
}
static void prepare_ram(const logic_case_t *c,unsigned n) {
        for(unsigned i=0;i<sizeof(ram);i++)ram[i]=(uint8_t)(i*37u+(n+1u)*19u+(i>>5)*7u);
        ram[0x1000]=c->opcode;ram[0x1001]=c->relation;input_word(0x1002,c->threshold);
        memcpy(ram+0xc326,c->counters,9);input_word(0xc330,c->x);input_word(0xc332,c->y);
        for(unsigned i=0;i<8;i++) {
            ww(NULL,0x10e000+i*4,(uint16_t)(c->helper_d[i]>>16));
            ww(NULL,0x10e002+i*4,(uint16_t)c->helper_d[i]);
        }
        input_word(0xff00,0x60fe);memcpy(expected_ram,ram,sizeof(ram));
}
static void conditions(void) {
    const msx_condition_bus bus={.read8=rb,.read16=rw,.write8=wb,.write16=ww,.helper=helper};
    for(unsigned n=0;n<sizeof(logic_cases)/sizeof(*logic_cases);n++) {
        current=&logic_cases[n];call_at=0;
        prepare_ram(current,n);
        for(unsigned i=0;i<current->patch_count;i++) {
            const logic_patch_t *p=&logic_patches[current->patch_first+i];expected_ram[p->offset]=p->value;
        }
        msx_condition_registers s=current->input;
        assert(msx_rom_01a5ca(&s,&bus)==MSX_CONDITION_OK);
        assert(!memcmp(s.d,current->expected.d,sizeof(s.d)) && !memcmp(s.a,current->expected.a,sizeof(s.a)));
        assert(s.sr==current->expected.sr && call_at==current->call_count && !memcmp(ram,expected_ram,sizeof(ram)));
    }
}
static void queries(void) {
    for(unsigned i=0;i<sizeof(query_cases)/sizeof(*query_cases);i++) {
        const query_case_t *c=&query_cases[i];neo_player_query_registers_t r=c->input;
        assert((unsigned)neo_player_query(c->slots,&r)==c->status);
        assert(!memcmp(r.d,c->expected.d,sizeof(r.d)) && r.sr==c->expected.sr && r.selected_slot==c->expected.selected_slot);
    }
}
static void positions(void) {
    for(unsigned i=0;i<sizeof(position_cases)/sizeof(*position_cases);i++) {
        const position_case_t *c=&position_cases[i];neo_player_positions_registers_t r=c->input;
        neo_player_positions(&c->frame,&r);
        assert(!memcmp(r.d,c->expected.d,sizeof(r.d)) && r.sr==c->expected.sr);
    }
}
static void real_services(void) {
    const msx_condition_bus bus={.read8=rb,.read16=rw,.write8=wb,.write16=ww};
    for(unsigned n=0;n<sizeof(native_service_cases)/sizeof(*native_service_cases);n++) {
        const native_service_case_t *c=&native_service_cases[n];prepare_ram(&c->base,n);
        for(unsigned i=0;i<c->input_count;i++) {
            const logic_patch_t *p=&native_input_patches[c->input_first+i];ram[p->offset]=p->value;
        }
        memcpy(expected_ram,ram,sizeof(ram));
        for(unsigned i=0;i<c->output_count;i++) {
            const logic_patch_t *p=&native_output_patches[c->output_first+i];expected_ram[p->offset]=p->value;
        }
        msx_condition_registers r=c->base.input;
        assert((unsigned)msx_condition_evaluate_native(&r,&bus)==c->status);
        assert(!memcmp(r.d,c->base.expected.d,sizeof(r.d)) && !memcmp(r.a,c->base.expected.a,sizeof(r.a)));
        assert(r.sr==c->base.expected.sr && !memcmp(ram,expected_ram,sizeof(ram)));
    }
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();console_set_render_mode(RENDER_MANUAL);
#endif
    conditions();queries();positions();real_services();
    puts("PASS: 447 conditions, 96 player queries and 96 position queries plus 451 real-service calls against original M68000 fixtures");
#ifdef __mips__
    debugf("MSX NATIVELOGIC REGRESSION PASS conditions=447 queries=96 positions=96 real_services=451\n");console_render();while(1) {}
#endif
    return 0;
}
