#ifndef MSX_VEHICLE_RENDERER_H
#define MSX_VEHICLE_RENDERER_H
#include "neo_vehicle.h"
void vehicle_renderer_load(void);
void vehicle_renderer_draw(const neo_vehicle_t *vehicle,int camera,bool flash);
#endif
