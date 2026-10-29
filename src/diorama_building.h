#ifndef MSX_DIORAMA_BUILDING_H
#define MSX_DIORAMA_BUILDING_H
#include "retro25d.h"
/* Authored decorative cutout. Front stays at z=0, preserving roof collision. */
typedef struct {float left,top,right,bottom,depth;} diorama_building_t;
bool diorama_building_cell(const diorama_building_t*,unsigned layer,int x,int y);
/* Four side-wall triangles, emitted behind the original front tiles. */
unsigned diorama_building_sides(retro_camera_t,const diorama_building_t*,retro_emit_fn,void*);
#endif
