#ifndef MSX_NEO_REGION_H
#define MSX_NEO_REGION_H
#include <stddef.h>
#include "neo_gactor.h"
/* Families of generic actors of the Mission 1 region (camera 0..1920). Each
 * family file exports a tick (one update of the actor, dispatched by its
 * routine label +00) and a predicate naming the routine labels it owns. */
ga_result_t neo_truck_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_truck_routine(uint32_t routine);
ga_result_t neo_effects_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_effects_routine(uint32_t routine);
ga_result_t neo_flyer_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_flyer_routine(uint32_t routine);
ga_result_t neo_props_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_props_routine(uint32_t routine);
ga_result_t neo_rest_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_rest_routine(uint32_t routine);
ga_result_t neo_dome_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_dome_routine(uint32_t routine);
ga_result_t neo_barrel_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_barrel_routine(uint32_t routine);
ga_result_t neo_wagon_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_wagon_routine(uint32_t routine);
#endif
