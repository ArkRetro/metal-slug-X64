#include "rom_008512.h"
#include <assert.h>
/* Manual structured recovery of P1 0x008512..0x009B82, including both paths.
 * 008540..009738 is 256 dirty-palette copies; 009772..009B70 repeats the
 * same eight longwords 256 times. The original bytes are checked separately.
 * This hardware-bus contract is not linked to the N64 campaign. */
static uint32_t address24(uint32_t a) { return a & UINT32_C(0xffffff); }
static uint8_t r8(const msx_008512_bus *b, uint32_t a) {
    return b->read8(b->context, address24(a));
}
static uint16_t r16(const msx_008512_bus *b, uint32_t a) {
    return b->read16(b->context, address24(a));
}
static uint32_t r32(const msx_008512_bus *b, uint32_t a) {
    uint32_t hi = r16(b, a);
    return (hi << 16) | r16(b, a + 2u);
}
static void w8(const msx_008512_bus *b, uint32_t a, uint8_t v) {
    b->write8(b->context, address24(a), v);
}
static void w16(const msx_008512_bus *b, uint32_t a, uint16_t v) {
    b->write16(b->context, address24(a), v);
}
static void w32(const msx_008512_bus *b, uint32_t a, uint32_t v) {
    w16(b, a, (uint16_t)(v >> 16));
    w16(b, a + 2u, (uint16_t)v);
}
void msx_rom_008512(msx_008512_registers *s, const msx_008512_bus *b) {
    assert(s && b && b->read8 && b->read16 && b->write8 && b->write16);
    const uint32_t a5 = s->a[5];
    uint16_t x = s->sr & 0x10u;
    w8(b, 0x300001u, (uint8_t)s->d[0]);                 /* 008512 */
    uint16_t count = r16(b, a5 + 0x1942u);
    if (count != 0) {                                 /* 00975A */
        w8(b, 0x3a001fu, (uint8_t)s->d[0]);
        uint32_t signed_count = count < 0x8000u ? count :
            UINT32_C(0xffff0000) | count;              /* ADDA.W sign extension */
        s->a[0] = 0x400000u + signed_count;
        s->a[1] = a5 + 0x18fau;
        for (unsigned i = 0; i < 8; ++i)
            s->d[i] = r32(b, s->a[1] + 4u * i);       /* 00976E */
        for (unsigned block = 0; block < 256; ++block) {
            for (unsigned i = 8; i-- > 0;) {
                s->a[0] -= 4u;
                w32(b, s->a[0], s->d[i]);
            }
        }
        uint16_t remaining = (uint16_t)(count - 0x2000u);
        w16(b, a5 + 0x1942u, remaining);               /* 009B72 */
        x = count < 0x2000u ? 0x10u : 0u;
        if (remaining == 0) w16(b, s->a[0], 0);        /* 009B7C */
    } else {
        uint8_t flags = r8(b, a5 + 0x1945u);           /* 008520 BCLR */
        w8(b, a5 + 0x1945u, flags & 0x7fu);
        if (flags & 0x80u) {
            w8(b, 0x3a000fu, (uint8_t)s->d[0]);
            s->a[0] = a5 - 0x706u;
            w16(b, s->a[0], r16(b, s->a[0]) & 0x8000u);
            s->a[1] = 0x400000u;
            s->d[7] = 7;
            for (unsigned slot = 0; slot < 256; ++slot) {
                uint32_t offset = slot * 32u;
                uint8_t tag = r8(b, s->a[0] + offset);
                w8(b, s->a[0] + offset, tag & 0x7fu);
                if (!(tag & 0x80u)) continue;
                /* MOVEM loads ALL registers before the first store. */
                for (unsigned i = 0; i < 6; ++i)
                    s->d[i] = r32(b, s->a[0] + offset + i * 4u);
                s->a[2] = r32(b, s->a[0] + offset + 24u);
                s->a[3] = r32(b, s->a[0] + offset + 28u);
                for (unsigned i = 0; i < 6; ++i)
                    w32(b, s->a[1] + offset + i * 4u, s->d[i]);
                w32(b, s->a[1] + offset + 24u, s->a[2]);
                w32(b, s->a[1] + offset + 28u, s->a[3]);
            }
        }
        w8(b, 0x300001u, (uint8_t)s->d[0]);            /* 00973A */
    }
    w8(b, r8(b, a5 + 0x1944u) ? 0x3a001fu : 0x3a000fu,
       (uint8_t)s->d[0]);                             /* 009740..009758 */
    uint8_t result = (uint8_t)s->d[0];
    s->sr = (s->sr & 0xffe0u) | x |
        (result == 0 ? 4u : (result & 0x80u ? 8u : 0u));
}
