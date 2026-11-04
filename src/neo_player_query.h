#ifndef MSX_NEO_PLAYER_QUERY_H
#define MSX_NEO_PLAYER_QUERY_H
#include <stdint.h>
/* Native input contract for ROM 0AAA88..0AAADE and object guard 0040AC.
 * Fields are values supplied by the scene, not a packed Neo Geo RAM overlay. */
typedef struct {
    uint32_t object_header;
    uint16_t active, field18, field14;
    uint8_t state21, field1f, field20;
} neo_player_query_slot_t;
typedef struct {
    uint32_t d[5]; /* Live partial-register results until caller ABI is removed. */
    uint16_t sr;
    uint8_t selected_slot;
} neo_player_query_registers_t;
typedef enum { NEO_PLAYER_QUERY_OK, NEO_PLAYER_QUERY_TRAP15 } neo_player_query_status_t;
/* Low byte of input d[0] selects slot 0 if zero, otherwise slot 1.
 * TRAP15 identifies the original invariant failure before its exception handler;
 * it must not be treated as an ordinary inactive-player result. */
neo_player_query_status_t neo_player_query(const neo_player_query_slot_t slots[2],
                                          neo_player_query_registers_t *registers);
#endif
