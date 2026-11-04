#ifndef MSX_RECOVERED_RAM_10FC00_H
#define MSX_RECOVERED_RAM_10FC00_H
#include <stdint.h>

/* Manual C recovery of P1:0010EE..001157 executed at RAM:10FC00.
 * Explicit caller ABI until the boot/platform binding is recovered.
 * Inputs/outputs are host integer values; never cast ROM bytes to this struct.
 * No instruction interpreter, cycle model, interrupts or outer RTS stack pop.
 * The platform must supply the addressed NV/header/control imports. */
typedef struct {
    uint32_t d[8], a[8];
    uint16_t sr;
} msx_boot_select_registers;

typedef struct {
    void *context;
    uint8_t (*read8)(void *, uint32_t);
    uint16_t (*read16)(void *, uint32_t);
    void (*write8)(void *, uint32_t, uint8_t);
} msx_boot_select_imports;

void msx_boot_select(msx_boot_select_registers *, const msx_boot_select_imports *);
#endif
