#ifndef MSX_MARCO_CONTROLLER_H
#define MSX_MARCO_CONTROLLER_H
#include "marco_animation.h"
/* Native fields consumed/written by original C0FEA. Names remain neutral until
 * all gameplay meanings are recovered. No original RAM layout is reproduced. */
typedef struct {
    uint16_t attributes;
    uint8_t flags5f,flags9a,accumulator9b,mask9c;
} marco_input_state_t;
typedef struct {uint8_t mode,current,limit;} marco_input_gate_t;
/* packed comes from the selected input-buffer service (CA2CE), still a caller
 * boundary. Returns the exact post-C1094 value, including its low-byte mask. */
uint32_t marco_input_filter(marco_input_state_t *state,uint32_t packed,marco_input_gate_t gate);
typedef enum {MARCO_PROGRAM_UNKNOWN,MARCO_PROGRAM_ABSENT,MARCO_PROGRAM_READY} marco_slot_kind_t;
typedef struct {
    const neo_anim_program_t *program;
    uint16_t entry;
    marco_slot_kind_t kind;
} marco_program_slot_t;
typedef struct {const marco_program_slot_t *slots;unsigned count;} marco_program_table_t;
/* C0F32/C0F62: form is the neutral numeric selector at actor+93 (its complete semantics
 * are not assigned here); character is word actor+6E, in its original
 * numeric representation. Arithmetic wraps at 16 bits before signed indexing.
 * NULL source slots trigger the first-row fallback; unknown native resources
 * are errors and must never be mistaken for a NULL source slot. */
bool marco_program_select(marco_animation_t *actor,marco_part_t part,
    const marco_program_table_t *table,uint8_t form,uint16_t character);
/* C0F96: direct availability query. Unlike select, it does NOT fall back. */
bool marco_program_available(const marco_program_table_t *table,uint8_t form,
    uint16_t character,bool *available);
#endif
