#include <assert.h>
#include <stdio.h>
#include "../src/marco_controller.h"
#ifdef MARCO_PRIVATE_CONTROLLER_FIXTURE
#include "marco_controller_private_fixture.h"
#else
#include "marco_controller_fixture.h"
#endif
#ifdef __mips__
#include <libdragon.h>
#endif
static void synthetic(void) {
    /* Missing-table behavior and variants not observed in the capture. */
    neo_anim_command_t command={.kind=NEO_ANIM_FINISH};neo_anim_program_t program={&command,1};
    marco_program_slot_t slots[8]={{0}};marco_program_table_t table={slots,8};marco_animation_t actor;
    slots[0]=(marco_program_slot_t){&program,0,MARCO_PROGRAM_READY};
    slots[4].kind=MARCO_PROGRAM_ABSENT;marco_animation_reset(&actor);actor.field18=23;
    bool available=true;
    assert(marco_program_available(&table,1,0,&available) && !available);
    assert(marco_program_select(&actor,MARCO_WEAPON,&table,1,0) && actor.weapon.program==&program && actor.field18==23);
    assert(marco_program_select(&actor,MARCO_BODY,&table,1,0) && actor.body.program==&program && actor.field18==0);
    slots[0].kind=MARCO_PROGRAM_ABSENT;
    assert(marco_program_select(&actor,MARCO_BODY,&table,1,0) && !actor.body.program);
    slots[4].kind=MARCO_PROGRAM_UNKNOWN;
    assert(!marco_program_select(&actor,MARCO_BODY,&table,1,0) && actor.failed);
    marco_animation_reset(&actor);assert(!marco_program_select(&actor,MARCO_BODY,&table,0,0x2000) && actor.failed);
    marco_animation_reset(&actor);slots[0]=(marco_program_slot_t){&program,0,MARCO_PROGRAM_READY};
    assert(marco_program_select(&actor,MARCO_BODY,&table,0,0x4000)); // 16-bit wrap.
    assert(!marco_program_available(&table,0,0,NULL));
    /* Mode gate and signed-looking bytes use unsigned comparisons. Low byte
     * mask happens AFTER flag writes, and preserves upper 24 bits of result. */
    marco_input_state_t input={.attributes=0,.flags5f=0xcf,.flags9a=0xf6,.accumulator9b=0x80,.mask9c=0};
    uint32_t result=marco_input_filter(&input,0xffffffffu,(marco_input_gate_t){2,255,128});
    assert(result==0xffafff00u && input.flags5f==0xff && input.flags9a==0xfe && input.accumulator9b==0x80);
    input=(marco_input_state_t){.attributes=1,.flags9a=1,.mask9c=255};
    assert(marco_input_filter(&input,0x00100010u,(marco_input_gate_t){0,255,0})==0x00100010u);
}
int main(void) {
#ifdef __mips__
    debug_init_emulog();console_init();
#endif
    unsigned inputs=sizeof(controller_inputs)/sizeof(*controller_inputs),choices=sizeof(controller_choices)/sizeof(*controller_choices);
    for(unsigned i=0;i<inputs;i++) {
        marco_input_state_t state=controller_inputs[i].state;
        uint32_t result=marco_input_filter(&state,controller_inputs[i].packed,controller_inputs[i].gate);
        assert(result==controller_inputs[i].result && state.flags5f==controller_inputs[i].flags5f && state.flags9a==controller_inputs[i].flags9a && state.accumulator9b==controller_inputs[i].accumulator9b);
    }
    for(unsigned i=0;i<choices;i++) {
        marco_animation_t actor;marco_animation_reset(&actor);
        actor.flash=controller_choices[i].flash;actor.field18=controller_choices[i].field18;actor.field50_tag=controller_choices[i].field50;
        const marco_program_slot_t *slot=&controller_roots[controller_choices[i].slot];marco_program_table_t table={slot,1};
        assert(marco_program_select(&actor,(marco_part_t)controller_choices[i].part,&table,controller_choices[i].form,controller_choices[i].character));
        const neo_anim_state_t *s=controller_choices[i].part?&actor.weapon:&actor.body;
        assert(s->program==slot->program && s->cursor==slot->entry && !s->timer && s->speed==256);
        assert(actor.flash==controller_choices[i].flash && actor.field18==controller_choices[i].after18 && actor.field50_tag==controller_choices[i].field50);
    }
    synthetic();printf("PASS: Marco controller inputs=%u choices=%u\n",inputs,choices);
#ifdef __mips__
    debugf("MSX CONTROLLER REGRESSION PASS inputs=%u choices=%u\n",inputs,choices);for (;;) wait_ms(100);
#endif
}
