#ifndef MSX_RECOVERED_CONDITION_SERVICES_H
#define MSX_RECOVERED_CONDITION_SERVICES_H
#include "rom_01a5ca.h"
typedef enum { MSX_NATIVE_CONDITION_OK, MSX_NATIVE_CONDITION_UNSUPPORTED,
               MSX_NATIVE_CONDITION_TRAP15, MSX_NATIVE_CONDITION_FAILURE } msx_native_condition_status;
/* Static C dispatcher with real native AAA88/01903A services. Input memory is
 * the addressed data/stack contract until campaign data producers are ported.
 * No synthetic service outputs and no M68000 instruction interpreter.
 * read/write callbacks must be provided; memory->helper is not consumed. */
msx_native_condition_status msx_condition_evaluate_native(msx_condition_registers *,const msx_condition_bus *memory);
#endif
