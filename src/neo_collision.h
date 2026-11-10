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
/* Kernel row of the grid's row 0 (0 after neo_collision_set_grid; the pack's
 * grid_origin_y, Mission 4). */
void neo_collision_set_row_origin(int row);
int neo_collision_row_origin(void);
unsigned neo_collision_rows(void);
/* Readers see the map grid instead of the ring while `on` (the ring holds
 * only its 64-row window: a row outside it reads another row's cells). */
void neo_collision_ring_bypass(bool on);
/* The kernel's cell ring (0C79FE) of a stage taller than 32 rows, loaded
 * like 0C73CE/0C74C4/0C72EC for the terrain layer position (+14/+18,
 * Q16); NULL for Mission 1 (32 rows: neo_kernel's column model). */
const uint8_t *neo_collision_ring(int32_t terrain_x,int32_t terrain_y);
/* The 4096 bytes of that ring (stage memory; cleared by set_grid). */
void neo_collision_set_ring(uint8_t *buffer);
/* ROM 0C70FC (collision part): the map's blocks (d0, d1) (2x2 cells, rows
 * from the top), d2 x d3 of them, into the ring at the window blocks
 * (d4, d5) (+36/+38 units, d5 the top block), clipped to the window. */
void neo_collision_ring_copy(int d0,int d1,int d2,int d3,int d4,int d5);
unsigned neo_collision_unknown_reads(void);
unsigned neo_collision_stamp(int32_t x,int32_t y,const uint8_t *template_bytes); /* 0C88B0 */
/* 0C70FC collision part over the whole stage grid: sub-map cells (pairs x
 * blocks) at cell column `column`, world row `top` for r = 0. */
unsigned neo_collision_patch(const uint8_t *cells,unsigned pairs,unsigned blocks,unsigned stride,int column,int top);
/* 0C70FC window clip of D0..D5 on the ring (stages taller than 32 rows). */
bool neo_collision_window_clip(int d[6]);
unsigned neo_collision_vehicle_wreckage(int32_t x,int32_t y,uint8_t flags);
bool neo_surface_at(int32_t x,int32_t reference,int32_t rise,int32_t drop,neo_surface_t *out);
/* The player's floor of one cell row (the sweeps' group-1 clearing): the
 * reference build's stage-3 autopilot enumerates them. */
bool neo_player_surface(int32_t x,int row,neo_surface_t *out);
/* Conservative static-terrain query: false for no floor or stacked surfaces. */
bool neo_surface_unique(int32_t x,neo_surface_t *out);
void neo_ground_velocity(unsigned shape,int direction,int32_t *vx,int32_t *vy);
bool neo_walk_step(neo_air_t *p,int direction);
bool neo_walk_step_shift(neo_air_t *p,int direction,unsigned shift);
bool neo_land_step(neo_air_t *p,int32_t previous_y);
#endif
