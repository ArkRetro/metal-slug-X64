#ifndef MSX_NEO_HIT_H
#define MSX_NEO_HIT_H
#include <stdbool.h>
#include <stdint.h>
/* Word coordinates/dimensions in the original projected, Y-up space. */
typedef struct { uint16_t x,y,width,height; } neo_hit_rectangle_t;
/* ROM 04798..0484A: narrow-phase rectangle and local impact point only.
 * Registry filtering, damage callbacks and actor state writes are separate. */
bool neo_hit_rectangle(const neo_hit_rectangle_t *query,uint16_t target_x,uint16_t target_y,
    uint8_t target_flags,const neo_hit_rectangle_t *local,uint16_t *impact_x,uint16_t *impact_y);
#endif
