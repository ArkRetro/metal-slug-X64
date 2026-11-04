#ifndef MSX_NEO_PLAYER_POSITIONS_H
#define MSX_NEO_PLAYER_POSITIONS_H
#include <stdint.h>
/* Native values consumed by ROM 01903A..0190BA, 01964A and 0040AC.
 * No packed RAM structures, original pointers or CPU instruction execution. */
typedef struct {
    uint32_t object_header;
    uint16_t x,y;
    uint8_t use_x,use_y;
} neo_position_player_t;
typedef struct {
    neo_position_player_t players[2];
    uint16_t fallback_x,fallback_y;
    uint8_t enabled;
} neo_player_positions_frame_t;
typedef struct { uint32_t d[8]; uint16_t sr; } neo_player_positions_registers_t;
/* Original D7.word: FF00=P1, 00FF=P2, FFFF=both, 0000=neither.
 * Absent players duplicate the available player's coordinates. Per-axis flags
 * independently select actor coordinates or the fallback values. */
void neo_player_positions(const neo_player_positions_frame_t *,neo_player_positions_registers_t *);
#endif
