#include "ram_10fc00.h"
#include <assert.h>

static uint32_t address24(uint32_t address) {
    return address & UINT32_C(0xffffff);
}
static uint8_t read8(const msx_boot_select_imports *b, uint32_t address) {
    return b->read8(b->context, address24(address));
}
static uint16_t read16(const msx_boot_select_imports *b, uint32_t address) {
    return b->read16(b->context, address24(address));
}
static void write8(const msx_boot_select_imports *b, uint32_t address, uint8_t value) {
    b->write8(b->context, address24(address), value);
}
static void low8(uint32_t *reg, uint8_t value) {
    *reg = (*reg & UINT32_C(0xffffff00)) | value;
}

void msx_boot_select(msx_boot_select_registers *r, const msx_boot_select_imports *b) {
    assert(r && b && b->read8 && b->read16 && b->write8);
    const uint32_t a1 = r->a[1];
    write8(b, a1 + 3u, (uint8_t)r->d[0]);             /* 10FC00 */
    low8(&r->d[7], read8(b, 0xd00108u));
    const uint8_t count = read8(b, 0xd00047u);
    low8(&r->d[6], (uint8_t)(count - 1u));            /* SUBQ.B, not .W */
    uint16_t x = count == 0 ? 0x10u : 0u;
    r->d[0] = 0;                                    /* MOVEQ clears all 32 bits */
    low8(&r->d[1], 0);
    for (;;) {
        write8(b, a1 + 0x1du, (uint8_t)r->d[0]);
        write8(b, 0x380021u, (uint8_t)r->d[0]);
        /* Preserve both word reads and the later repeated product read. */
        uint32_t magic = (uint32_t)read16(b, 0x100u) << 16;
        magic |= read16(b, 0x102u);
        if (magic == UINT32_C(0x4e454f2d)) {
            if (read16(b, 0x108u) == 0x240u) break;   /* 10FC30 early exit */
            if ((uint8_t)r->d[1] == 0 && read8(b, 0x114u) == 1 &&
                read16(b, 0x108u) != 1) {
                low8(&r->d[1], 0xffu);
                low8(&r->d[2], (uint8_t)r->d[0]);
            }
        }
        uint8_t old = (uint8_t)r->d[0];
        low8(&r->d[0], (uint8_t)(old + 1u));          /* ADDQ.B updates X */
        x = old == 0xffu ? 0x10u : 0u;
        uint16_t remaining = (uint16_t)((uint16_t)r->d[6] - 1u);
        r->d[6] = (r->d[6] & UINT32_C(0xffff0000)) | remaining;
        if (remaining == 0xffffu) {                  /* DBRA preserves CCR */
            low8(&r->d[0], (uint8_t)r->d[2]);         /* live fallback, not zero */
            break;
        }
    }
    uint8_t result = (uint8_t)r->d[0];
    write8(b, 0xd00058u, result);
    write8(b, 0x380021u, (uint8_t)r->d[7]);
    write8(b, a1 + 0x0du, result);
    write8(b, a1 + 0x13u, result);
    /* Last MOVE.B supplies N/Z and clears V/C; X is from SUBQ/last ADDQ.
     * Intermediate CCR is not exposed to imports in this atomic ABI. */
    r->sr = (uint16_t)((r->sr & 0xffe0u) | x |
                       (result == 0 ? 4u : (result & 0x80u ? 8u : 0u)));
}
