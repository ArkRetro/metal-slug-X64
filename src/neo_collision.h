#ifndef MSX_NEO_COLLISION_H
#define MSX_NEO_COLLISION_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_motion.h"
typedef struct { int32_t height; uint8_t shape,flags; } neo_surface_t;
/* Q16.16 world coordinates; bounded queries never wrap outside the recovered map. */
uint8_t neo_collision_cell(int x,int y);
bool neo_surface_at(int32_t x,int32_t reference,int32_t rise,int32_t drop,neo_surface_t *out);
void neo_ground_velocity(unsigned shape,int direction,int32_t *vx,int32_t *vy);
bool neo_walk_step(neo_air_t *p,int direction);
bool neo_land_step(neo_air_t *p,int32_t previous_y);
#endif
