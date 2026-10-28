#include "marco_controller.h"
#include <stddef.h>
uint32_t marco_input_filter(marco_input_state_t *s,uint32_t packed,marco_input_gate_t gate) {
    uint16_t mask=0xffff;
    if (gate.mode>=2) mask&=0xffbf;
    if (!(s->flags9a&1u) && gate.limit<=gate.current) mask&=0xffef;
    uint32_t result=packed&(((uint32_t)mask<<16)|mask);
    result=(result<<16)|(result>>16);
    s->flags5f=(uint8_t)((s->flags5f&~0x30u)|((result&1u)?0x20u:0)|((result&2u)?0x10u:0));
    s->flags9a=(uint8_t)(s->flags9a&~8u);
    if (result&((s->attributes&1u)?8u:4u)) s->flags9a|=8u;
    /* AND.B changes only D0's low byte. Upper bits remain visible to callers. */
    result=(result&0xffffff00u)|((result&255u)&s->mask9c);
    s->accumulator9b|=(uint8_t)result;
    return result;
}
static const marco_program_slot_t *slot_at(const marco_program_table_t *table,unsigned index) {
    if (!table || !table->slots || index>=table->count) return NULL;
    const marco_program_slot_t *slot=&table->slots[index];
    if (slot->kind==MARCO_PROGRAM_ABSENT) return slot;
    if (slot->kind!=MARCO_PROGRAM_READY || !slot->program || slot->entry>=slot->program->count) return NULL;
    return slot;
}
static bool index_for(uint8_t form,uint16_t character,unsigned *index) {
    uint16_t offset=(uint16_t)(((unsigned)form*4u+character)*4u);
    if (offset&0x8000u) return false; // signed 68000 displacement is outside this table.
    *index=offset/4u;return true;
}
bool marco_program_select(marco_animation_t *actor,marco_part_t part,
    const marco_program_table_t *table,uint8_t form,uint16_t character) {
    unsigned index;
    const marco_program_slot_t *slot=NULL;
    if (index_for(form,character,&index)) {
        slot=slot_at(table,index);
        if (slot && slot->kind==MARCO_PROGRAM_ABSENT) slot=slot_at(table,index&3u);
    }
    if (!slot || (part!=MARCO_BODY && part!=MARCO_WEAPON)) {actor->failed=true;return false;}
    if (slot->kind==MARCO_PROGRAM_ABSENT) return marco_animation_start(actor,part,NULL,0,256);
    return marco_animation_start(actor,part,slot->program,slot->entry,256);
}
bool marco_program_available(const marco_program_table_t *table,uint8_t form,
    uint16_t character,bool *available) {
    unsigned index;if (!available || !index_for(form,character,&index)) return false;
    const marco_program_slot_t *slot=slot_at(table,index);if (!slot) return false;
    *available=slot->kind==MARCO_PROGRAM_READY;return true;
}
