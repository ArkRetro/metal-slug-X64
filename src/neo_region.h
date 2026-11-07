#ifndef MSX_NEO_REGION_H
#define MSX_NEO_REGION_H
#include <stddef.h>
#include "neo_gactor.h"
/* Families of generic actors of the Mission 1 region (camera 0..1920). Each
 * family file exports a tick (one update of the actor, dispatched by its
 * routine label +00) and a predicate naming the routine labels it owns. */
ga_result_t neo_goarrow_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_goarrow_routine(uint32_t routine);
ga_result_t neo_rebel2_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_rebel2_routine(uint32_t routine);
ga_result_t neo_rebel3_tick(neo_gactor_t *a,neo_genv_t *e);
ga_result_t neo_gate_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_gate_routine(uint32_t routine);
bool neo_crate_routine(uint32_t routine);
ga_result_t neo_crate_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_m2props_routine(uint32_t routine);
bool neo_m2gate_routine(uint32_t routine);
ga_result_t neo_m2props_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_m3props_routine(uint32_t routine);
bool neo_m3enemy_routine(uint32_t routine);
ga_result_t neo_m3enemy_tick(neo_gactor_t *a,neo_genv_t *e);
ga_result_t neo_m3props_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_drone_routine(uint32_t routine);
ga_result_t neo_drone_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_m2traps_routine(uint32_t routine);
bool neo_m2creature_routine(uint32_t routine);
ga_result_t neo_m2creature_tick(neo_gactor_t *a,neo_genv_t *e);
ga_result_t neo_m2traps_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_hostage_routine(uint32_t routine);
ga_result_t neo_hostage_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_m2boss_routine(uint32_t routine);
bool neo_m3mid_routine(uint32_t routine);
ga_result_t neo_m3mid_tick(neo_gactor_t *a,neo_genv_t *e);
ga_result_t neo_m2boss_tick(neo_gactor_t *a,neo_genv_t *e);
/* 03C520..03C532: the water ripple 03C62A (+6E = kind) when 019240 has a
 * palette for the mission; false on an explicit stop. */
bool neo_gate_ripple(neo_gactor_t *a,neo_genv_t *e,uint16_t kind);
bool neo_rebel3_routine(uint32_t routine);
ga_result_t neo_truck_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_truck_routine(uint32_t routine);
ga_result_t neo_effects_tick(neo_gactor_t *a,neo_genv_t *e);
/* 01D51A / 01D54A / 01D578 entered by JMP with the program started (098C0C). */
ga_result_t neo_effects_frag_launch(neo_gactor_t *a,neo_genv_t *e,uint8_t tail);
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
ga_result_t neo_arab_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_arab_routine(uint32_t routine);
ga_result_t neo_prisoner_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_prisoner_routine(uint32_t routine);
ga_result_t neo_item_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_item_routine(uint32_t routine);
bool neo_item_pickup(uint32_t routine);
/* The pickup routine (record +10) of item `a` (kind +6E) served to player p
 * (host tests; the item's own update calls it on contact). */
bool neo_item_take(neo_gactor_t *a,neo_genv_t *e,unsigned p);
void neo_item_add_score(uint8_t score[4],uint32_t value);   /* 057948 */
ga_result_t neo_heli_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_heli_routine(uint32_t routine);
ga_result_t neo_camel_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_camel_routine(uint32_t routine);
ga_result_t neo_porter_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_porter_routine(uint32_t routine);
ga_result_t neo_civil_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_civil_routine(uint32_t routine);
ga_result_t neo_slug_tick(neo_gactor_t *a,neo_genv_t *e);
/* Mission 3 vehicle 0AC866 and its children (neo_m3veh.c). */
bool neo_m3veh_routine(uint32_t routine);
ga_result_t neo_m3veh_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_slug_routine(uint32_t routine);
ga_result_t neo_tank_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_tank_routine(uint32_t routine);
ga_result_t neo_slugnoid_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_slugnoid_routine(uint32_t routine);
uint32_t neo_slug_k14594(neo_gactor_t *a,neo_genv_t *e);   /* 014594 */
uint32_t neo_slug_k143ee(neo_gactor_t *a,neo_genv_t *e);   /* 0143EE */
/* neo_tread.c: the SV-001's tread body 014AE8..0153EE. */
neo_gactor_t *neo_tread_spawn(neo_gactor_t *a,neo_genv_t *e);
int32_t neo_tread_step(neo_gactor_t *a,neo_genv_t *e,unsigned entry);
void neo_tread_air_reset(neo_gactor_t *c,uint16_t d0);
ga_result_t neo_tread_tick(neo_gactor_t *a,neo_genv_t *e);
ga_result_t neo_bullet_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_bullet_routine(uint32_t routine);
ga_result_t neo_armored_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_armored_routine(uint32_t routine);
bool neo_armored_vehicle(uint32_t routine);
ga_result_t neo_swords_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_swords_routine(uint32_t routine);
ga_result_t neo_rocket_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_rocket_routine(uint32_t routine);
ga_result_t neo_chaser_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_chaser_routine(uint32_t routine);
bool neo_chaser_attacker(uint32_t routine);
ga_result_t neo_pweapon_tick(neo_gactor_t *a,neo_genv_t *e);   /* neo_pweapon.c: the other player weapons */
bool neo_pweapon_routine(uint32_t routine);
bool neo_pweapon_attacker(uint32_t routine);   /* the C's rows of the player's side (boss hurtboxes) */
ga_result_t neo_secret_tick(neo_gactor_t *a,neo_genv_t *e);
bool neo_secret_routine(uint32_t routine);   /* the vehicle 087E36 itself (not its children) */
#endif
