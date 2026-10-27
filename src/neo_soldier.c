#include "neo_soldier.h"
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
