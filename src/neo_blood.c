#include "neo_blood.h"
/* ROM 01B2BE call sites the port runs natively and the palettes of both
 * branches (009EA8 d0). OFF: carry set, the branch of every euro recording.
 * ON: carry clear, recorded by tools/capture_blood.lua (10FD8F forced to 1;
 * data/blood_fixture.json, checked by tools/test_neo_blood.c). */
static const struct { uint32_t site; uint16_t off,on; } sites[]={
    {NEO_BLOOD_SOLDIER_DEATH,0x22f,0x20e},
    {NEO_BLOOD_SPLASH,0x3ba,0x37a},
    {NEO_BLOOD_ARAB_DEATH,0x3ba,0x20b},   /* +14 = +1B (OFF) or +1A (ON) */
    {NEO_BLOOD_PIECE_A9590,0x19,0x18},
    {NEO_BLOOD_PIECE_A9638,0x1a,0x17},
    {NEO_BLOOD_PIECE_A96E0,0x1a,0x17},
    {NEO_BLOOD_PIECE_A9814,0x19,0x18},
};
uint16_t neo_blood_palette(uint32_t call_site,bool blood) {
    for (unsigned i=0;i<sizeof(sites)/sizeof(sites[0]);i++)
        if (sites[i].site==call_site) return blood?sites[i].on:sites[i].off;
    return 0;
}
