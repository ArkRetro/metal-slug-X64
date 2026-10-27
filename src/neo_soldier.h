#ifndef MSX_NEO_SOLDIER_H
#define MSX_NEO_SOLDIER_H
#include <stdint.h>
/* Only the measured AF65A arithmetic fragment, not the complete soldier AI.
 * Inputs r0/r1 are outputs of the ORIGINAL RNG call, not C rand(). Field names
 * retain offsets until their complete actor semantics have been established. */
typedef struct { int32_t value_62; uint16_t value_26; } neo_soldier_motion_seed_t;
neo_soldier_motion_seed_t neo_soldier_seed_motion(int16_t r0,int16_t r1,
    uint8_t field_15,int16_t field_92);
#endif
