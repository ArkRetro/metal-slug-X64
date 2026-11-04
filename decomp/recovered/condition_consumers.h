#ifndef MSX_RECOVERED_CONDITION_CONSUMERS_H
#define MSX_RECOVERED_CONDITION_CONSUMERS_H
#include "condition_services.h"
typedef enum { MSX_CONSUMER_1A01E, MSX_CONSUMER_1A2D8, MSX_CONSUMER_1A4F2 } msx_condition_consumer;
typedef struct { msx_native_condition_status status; uint32_t target; } msx_consumer_result;
/* Returns an explicit native scheduling boundary, never executes the target
 * as a CPU address. 01A526/01A056/01A0A6/01A372/004188 require their owners.
 * A RTS boundary is returned before popping the outer caller's return address. */
msx_consumer_result msx_condition_consume(msx_condition_consumer,msx_condition_registers *,const msx_condition_bus *);
#endif
