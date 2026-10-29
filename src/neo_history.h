#ifndef MSX_NEO_HISTORY_H
#define MSX_NEO_HISTORY_H
#include <stdint.h>
/* Executed ROM 01AB50..01AB5A, RAM 10C33A. Advance once per original actor
 * update pass, not per VI render. Zero remains the fresh-actor mark sentinel. */
static inline uint8_t neo_history_token_next(uint8_t previous) {
    return (uint8_t)((previous&1u)+1u);
}
#endif
