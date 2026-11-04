#ifndef MSX_RECOVERED_RAM_DESCRIPTORS_H
#define MSX_RECOVERED_RAM_DESCRIPTORS_H
#include <stdint.h>

/* Initial RAM_COPY_100000 image only. Pure work-RAM imports, host integers.
 * Addressed ABI is temporary; no CPU opcode interpreter or cycle model.
 * Outer RTS is excluded. Allocation/list walks require terminating links.
 * No callback may observe intermediate CPU flags through these imports. */
typedef struct {
    uint32_t d[8], a[8];
    uint16_t sr;
} msx_descriptor_registers;
typedef struct {
    void *context;
    uint8_t (*read8)(void *, uint32_t);
    uint16_t (*read16)(void *, uint32_t);
    void (*write8)(void *, uint32_t, uint8_t);
    void (*write16)(void *, uint32_t, uint16_t);
} msx_descriptor_memory;

void msx_descriptor_allocate(msx_descriptor_registers *, const msx_descriptor_memory *);
void msx_descriptor_remove(msx_descriptor_registers *, const msx_descriptor_memory *);

typedef enum {
    MSX_DESCRIPTOR_DONE,
    MSX_DESCRIPTOR_PENDING_CALLBACK,
    MSX_DESCRIPTOR_PENDING_RETURN
} msx_descriptor_status;
typedef struct {
    msx_descriptor_status status;
    uint32_t boundary, target;
} msx_descriptor_result;
/* Native bindings for 10008C and 100158 only. Unknown callbacks stop before
 * JSR at 10081A. Changed return links stop before the callback RTS. */
msx_descriptor_result msx_descriptor_dispatch(msx_descriptor_registers *,
                                              const msx_descriptor_memory *);
#endif
