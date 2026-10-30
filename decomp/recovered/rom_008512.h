#ifndef MSX_RECOVERED_008512_H
#define MSX_RECOVERED_008512_H
#include <stdint.h>
/* Addressed C recovery, not an interpreter. A5 and the other live registers
 * are explicit until the caller ABI is recovered. The bus is a required backend.
 * Callbacks use CPU addresses and host integer values, never packed host structs.
 * Bus-cycle timing and the RTS stack pop are outside this function's contract. */
typedef struct {
    uint32_t d[8], a[8];
    uint16_t sr;
} msx_008512_registers;
typedef struct {
    void *context;
    uint8_t (*read8)(void *, uint32_t);
    uint16_t (*read16)(void *, uint32_t);
    void (*write8)(void *, uint32_t, uint8_t);
    void (*write16)(void *, uint32_t, uint16_t);
} msx_008512_bus;
void msx_rom_008512(msx_008512_registers *, const msx_008512_bus *);
#endif
