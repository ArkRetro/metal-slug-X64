#include "neo_region.h"
#include "region_rom.h"
#include "neo_boss.h"
/* Dispatch of the generic actors by routine label (see neo_region.h) and the
 * ROM data tables they read (data/region_rom.json). */
/* The blobs sorted by address, with the running maximum of their ends: a
 * lookup is a binary search plus a short walk back over the blobs that can
 * still contain the address (the linear scan over every blob was the
 * hottest function of the world update, docs/RENDIMIENTO.md). Same answer as
 * the scan: the most bytes left after the address, the first blob on ties. */
enum { REGION_BLOBS=sizeof(region_blobs)/sizeof(region_blobs[0]) };
static uint8_t blob_order[REGION_BLOBS];
static uint32_t blob_start[REGION_BLOBS],blob_max_end[REGION_BLOBS];
static bool blob_sorted;
static void sort_blobs(void) {
    _Static_assert(REGION_BLOBS<=256,"blob index is a byte");
    for (unsigned i=0;i<REGION_BLOBS;i++) {
        unsigned j=i;
        while (j && region_blobs[blob_order[j-1]].address>region_blobs[i].address) {blob_order[j]=blob_order[j-1];j--;}
        blob_order[j]=(uint8_t)i;
    }
    uint32_t end=0;
    for (unsigned k=0;k<REGION_BLOBS;k++) {
        const region_blob_t *b=&region_blobs[blob_order[k]];
        blob_start[k]=b->address;
        if (b->address+b->length>end) end=b->address+b->length;
        blob_max_end[k]=end;
    }
    blob_sorted=true;
}
static const uint8_t *(*rom_extra)(uint32_t);
const uint8_t *ga_rom(uint32_t address) {
    /* Blobs may overlap (the truck solid box 029BC2 runs into the rectangle
     * 029BCA): take the one with the most bytes left after the address so a
     * reader never runs off the end of a shorter blob. */
    if (!blob_sorted) sort_blobs();
    unsigned lo=0,hi=REGION_BLOBS;                  /* first start > address */
    while (lo<hi) {unsigned m=(lo+hi)/2;if (blob_start[m]<=address) lo=m+1; else hi=m;}
    const uint8_t *best=NULL;uint32_t left=0;unsigned best_index=REGION_BLOBS;
    for (unsigned k=lo;k-- && blob_max_end[k]>address;) {
        unsigned i=blob_order[k];const region_blob_t *b=&region_blobs[i];
        if (address<b->address+b->length) {
            uint32_t l=b->address+b->length-address;
            if (l>left || (l==left && i<best_index)) {left=l;best_index=i;best=b->data+(address-b->address);}
        }
    }
    if (best) return best;
    /* The boss's ROM regions (debris lists 0E932A.. read by the generic
     * actors it creates). */
    const uint8_t *boss=neo_boss_rom(address,1);
    if (boss) return boss;
    /* The stage's own ROM data held by its mission pack (the dynamic maps
     * of the 'MSXR' block, Mission 3: scene_stage_rom). */
    return rom_extra?rom_extra(address):NULL;
}
void ga_rom_set_extra(const uint8_t *(*lookup)(uint32_t address)) {rom_extra=lookup;}
bool neo_region_routine(uint32_t r) {return neo_truck_routine(r) || neo_effects_routine(r) || neo_flyer_routine(r) || neo_props_routine(r) || neo_rest_routine(r) || neo_dome_routine(r) || neo_barrel_routine(r) || neo_wagon_routine(r) || neo_arab_routine(r) || neo_prisoner_routine(r) || neo_item_routine(r) || neo_armored_routine(r) || neo_secret_routine(r) || neo_swords_routine(r) || neo_rocket_routine(r) || neo_heli_routine(r) || neo_camel_routine(r) || neo_porter_routine(r) || neo_civil_routine(r) || neo_slug_routine(r) || neo_tank_routine(r) || neo_slugnoid_routine(r) || neo_bullet_routine(r) || neo_chaser_routine(r) || neo_pweapon_routine(r) || neo_goarrow_routine(r) || neo_rebel2_routine(r) || neo_rebel3_routine(r) || neo_gate_routine(r) || neo_crate_routine(r) || neo_m2props_routine(r) || neo_m3props_routine(r) || neo_drone_routine(r) || neo_m2traps_routine(r) || neo_hostage_routine(r) || neo_m2boss_routine(r);}
ga_result_t neo_region_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (neo_truck_routine(r)) return neo_truck_tick(a,e);
    if (neo_effects_routine(r)) return neo_effects_tick(a,e);
    if (neo_flyer_routine(r)) return neo_flyer_tick(a,e);
    if (neo_props_routine(r)) return neo_props_tick(a,e);
    if (neo_rest_routine(r)) return neo_rest_tick(a,e);
    if (neo_dome_routine(r)) return neo_dome_tick(a,e);
    if (neo_barrel_routine(r)) return neo_barrel_tick(a,e);
    if (neo_wagon_routine(r)) return neo_wagon_tick(a,e);
    if (neo_arab_routine(r)) return neo_arab_tick(a,e);
    if (neo_prisoner_routine(r)) return neo_prisoner_tick(a,e);
    if (neo_item_routine(r)) return neo_item_tick(a,e);
    if (neo_armored_routine(r)) return neo_armored_tick(a,e);
    if (neo_secret_routine(r)) return neo_secret_tick(a,e);
    if (neo_swords_routine(r)) return neo_swords_tick(a,e);
    if (neo_rocket_routine(r)) return neo_rocket_tick(a,e);
    if (neo_heli_routine(r)) return neo_heli_tick(a,e);
    if (neo_camel_routine(r)) return neo_camel_tick(a,e);
    if (neo_porter_routine(r)) return neo_porter_tick(a,e);
    if (neo_civil_routine(r)) return neo_civil_tick(a,e);
    if (neo_slug_routine(r)) return neo_slug_tick(a,e);
    if (neo_tank_routine(r)) return neo_tank_tick(a,e);
    if (neo_slugnoid_routine(r)) return neo_slugnoid_tick(a,e);
    if (neo_bullet_routine(r)) return neo_bullet_tick(a,e);
    if (neo_chaser_routine(r)) return neo_chaser_tick(a,e);
    if (neo_pweapon_routine(r)) return neo_pweapon_tick(a,e);
    if (neo_goarrow_routine(r)) return neo_goarrow_tick(a,e);
    if (neo_rebel2_routine(r)) return neo_rebel2_tick(a,e);
    if (neo_rebel3_routine(r)) return neo_rebel3_tick(a,e);
    if (neo_gate_routine(r)) return neo_gate_tick(a,e);
    if (neo_crate_routine(r)) return neo_crate_tick(a,e);
    if (neo_m2props_routine(r)) return neo_m2props_tick(a,e);
    if (neo_m3props_routine(r)) return neo_m3props_tick(a,e);
    if (neo_drone_routine(r)) return neo_drone_tick(a,e);
    if (neo_m2traps_routine(r)) return neo_m2traps_tick(a,e);
    if (neo_hostage_routine(r)) return neo_hostage_tick(a,e);
    if (neo_m2boss_routine(r)) return neo_m2boss_tick(a,e);
    ga_fail(e,r);return GA_UNSUPPORTED;
}
