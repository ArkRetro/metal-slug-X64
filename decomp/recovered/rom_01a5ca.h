#ifndef MSX_RECOVERED_01A5CA_H
#define MSX_RECOVERED_01A5CA_H
#include <stdbool.h>
#include <stdint.h>
/* Addressed C recovery. Not an instruction interpreter or campaign backend.
 * Stack writes and live flags are explicit to audit the original calling ABI.
 * The outer caller's RTS is excluded; execution stops before 01A5DC. */
typedef struct { uint32_t d[8], a[8]; uint16_t sr; } msx_condition_registers;
typedef enum { MSX_QUERY_1903A, MSX_QUERY_AAA88 } msx_condition_helper;
typedef struct {
    void *context;
    uint8_t (*read8)(void *, uint32_t);
    uint16_t (*read16)(void *, uint32_t);
    void (*write8)(void *, uint32_t, uint8_t);
    void (*write16)(void *, uint32_t, uint16_t);
    /* Required when reached. Returns at the helper RTS boundary, with live
     * outputs in registers/CCR and the return address still at A7.
     * This function owns that RTS pop. false means unavailable, never false C. */
    bool (*helper)(void *, msx_condition_helper, msx_condition_registers *);
} msx_condition_bus;
typedef enum { MSX_CONDITION_OK, MSX_CONDITION_UNSUPPORTED_INDEX,
               MSX_CONDITION_MISSING_HELPER } msx_condition_status;
msx_condition_status msx_rom_01a5ca(msx_condition_registers *, const msx_condition_bus *);
#endif
