#ifndef MSX_NEO_COLLISION_H
#define MSX_NEO_COLLISION_H
#include <stdbool.h>
#include <stdint.h>
#include "neo_motion.h"
typedef struct { int32_t height; uint8_t shape,flags; } neo_surface_t;
/* Q16.16 world coordinates; bounded queries never wrap outside the recovered map. */
uint8_t neo_collision_cell(int x,int y);
unsigned neo_collision_columns(void);   /* 8-px columns of the recovered map */
void neo_collision_reset(void);
/* Stage grid of a mission pack (NULL: Mission 1's compiled grid). `dynamic`
 * holds columns*rows bytes for the stamps; `unknown_mask` (or NULL) is a bit
 * per cell, set for cells no capture observed (read as empty, counted). */
void neo_collision_set_grid(const uint8_t *cells,uint8_t *dynamic,const uint8_t *unknown_mask,unsigned columns,unsigned rows);
unsigned neo_collision_rows(void);
unsigned neo_collision_unknown_reads(void);
unsigned neo_collision_stamp(int32_t x,int32_t y,const uint8_t *template_bytes); /* 0C88B0 */
unsigned neo_collision_vehicle_wreckage(int32_t x,int32_t y,uint8_t flags);
bool neo_surface_at(int32_t x,int32_t reference,int32_t rise,int32_t drop,neo_surface_t *out);
/* Conservative static-terrain query: false for no floor or stacked surfaces. */
bool neo_surface_unique(int32_t x,neo_surface_t *out);
void neo_ground_velocity(unsigned shape,int direction,int32_t *vx,int32_t *vy);
bool neo_walk_step(neo_air_t *p,int direction);
bool neo_land_step(neo_air_t *p,int32_t previous_y);
#endif
