#ifndef MSX_NEO_SOLDIER_H
#define MSX_NEO_SOLDIER_H
#include <stdint.h>
#include "neo_rng.h"
/* Measured initialization/decision/motion fragments, not the complete soldier AI.
 * Inputs r0/r1 are outputs of the ORIGINAL RNG call, not C rand(). Field names
 * retain offsets until their complete actor semantics have been established. */
typedef struct { int32_t value_62; uint16_t value_26; } neo_soldier_motion_seed_t;
neo_soldier_motion_seed_t neo_soldier_seed_motion(int16_t r0,int16_t r1,
    uint8_t field_15,int16_t field_92);
neo_soldier_motion_seed_t neo_soldier_seed_from_rng(neo_rng_t *rng,
    uint8_t field_15,int16_t field_92);
/* Labels are ROM provenance, not executable pointers. This is only the ordered
 * decision prefix AF6D0..AF724, after the bit-6 path has called 01456A.
 * Later helper/impact decisions can override it and are NOT implemented here. */
typedef struct {
    uint16_t after_motion_d6,field_78;
    uint16_t after_animation_84,after_animation_3c,after_animation_62;
    uint8_t after_animation_7c,after_animation_6c;
} neo_soldier_ground_inputs_t;
uint32_t neo_soldier_ground_select_prefix(uint32_t current_routine,
    const neo_soldier_ground_inputs_t *input);
/* AF8A6..AF8AC: decay +0x62 with signed ASR.L; collision remains external. */
int32_t neo_soldier_damp_motion(int32_t value_62);
#endif
