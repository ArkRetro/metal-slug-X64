#include "neo_soldier.h"
#include "neo_motion.h"
static int16_t signed_word(uint16_t value) {
    return (int16_t)((int32_t)(value&0x7fffu)-(value&0x8000u?32768:0));
}
neo_soldier_motion_seed_t neo_soldier_seed_from_rng(neo_rng_t *rng,
    uint8_t field_15,int16_t field_92) {
    int16_t first=signed_word(neo_rng_next(rng));
    int16_t second=signed_word(neo_rng_next(rng));
    return neo_soldier_seed_motion(first,second,field_15,field_92);
}
neo_soldier_motion_seed_t neo_soldier_seed_motion(int16_t r0,int16_t r1,
    uint8_t field_15,int16_t field_92) {
    // ROM AF670/AF67C: EXT.L of the low 16 bits returned by the two RNG calls.
    // AF67E/AF684: +0x28000 and +saved first result. No invented speed constant.
    int32_t value=(int32_t)r0+(int32_t)r1+0x28000;
    if (field_92!=0) value+=0x10000; // AF688..AF690, statically decoded branch.
    if (!(field_15&1)) value=-value; // AF696..AF6A0.
    uint32_t magnitude=value<0?(uint32_t)(-(int64_t)value):(uint32_t)value;
    // AF6AE..AF6B6: 32-bit LSL #6, SWAP, ADDI.W #0x80, store low word.
    uint16_t step=(uint16_t)(((magnitude<<6)>>16)+0x80u);
    return (neo_soldier_motion_seed_t){value,step};
}
uint32_t neo_soldier_ground_select_prefix(uint32_t routine,
    const neo_soldier_ground_inputs_t *in) {
    if ((in->after_motion_d6&0x200u) && (in->field_78&1u)) routine=0xaf7f6; // AF6D0..AF6E4
    // SUB.W wraps before EOR.W/BPL. Do not use a full-width signed distance.
    uint16_t delta=(uint16_t)(in->after_animation_84-in->after_animation_3c);
    if ((delta^in->after_animation_62)&0x8000u) { // AF6F0..AF6FE
        routine=0xaf890;
        if (in->after_animation_7c&2u) routine=0xaf8d4; // AF702..AF712
    }
    if (in->after_animation_6c==3u) routine=0xaf890; // AF718..AF724
    return routine;
}
int32_t neo_soldier_damp_motion(int32_t value_62) {
    return value_62-neo_asr(value_62,3);
}
