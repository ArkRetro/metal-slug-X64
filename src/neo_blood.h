#ifndef MSX_NEO_BLOOD_H
#define MSX_NEO_BLOOD_H
#include <stdbool.h>
#include <stdint.h>
/* Arcade soft-DIP option 6 "BLOOD", RAM 10FD8F. Its readers are ROM 01B2BE
 * (tst.b 10FD8F; carry set when 0) and 032B94. MSX_BLOOD (build flag,
 * default 1) is the port's setting: 1 = ON, the default of the Japanese
 * header (0x3388); the US (0x2F78) and EU (0x316E) headers default to OFF. */
#ifndef MSX_BLOOD
#define MSX_BLOOD 1
#endif
/* The 01B2BE call sites that the native port translates (ROM addresses of
 * the jsr 01B2BE). */
enum {
    NEO_BLOOD_SOLDIER_DEATH=0x405f0,   /* 0405EA: soldier death palette */
    NEO_BLOOD_SPLASH=0x40f98,          /* 040F88: death splash palette */
    NEO_BLOOD_ARAB_DEATH=0xa9436,      /* 0A93CE: swordsman death, +14 slot */
    NEO_BLOOD_PIECE_A9590=0xa95f2,     /* swordsman death pieces */
    NEO_BLOOD_PIECE_A9638=0xa969a,
    NEO_BLOOD_PIECE_A96E0=0xa9742,
    NEO_BLOOD_PIECE_A9814=0xa9876,
};
/* Logical palette resource (009EA8 d0) chosen at a translated call site; for
 * 0A9436 the resource of the 039DA8 slot copied to +14. 0 for a call site
 * the port does not translate. */
uint16_t neo_blood_palette(uint32_t call_site,bool blood);
#endif
