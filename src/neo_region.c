#include "neo_region.h"
#include "region_rom.h"
#include "neo_boss.h"
/* Dispatch of the generic actors by routine label (see neo_region.h) and the
 * ROM data tables they read (data/region_rom.json). */
const uint8_t *ga_rom(uint32_t address) {
    for (unsigned i=0;i<sizeof(region_blobs)/sizeof(region_blobs[0]);i++) {
        const region_blob_t *b=&region_blobs[i];
        if (address>=b->address && address<b->address+b->length) return b->data+(address-b->address);
    }
    /* The boss's ROM regions (debris lists 0E932A.. read by the generic
     * actors it creates). */
    return neo_boss_rom(address,1);
}
bool neo_region_routine(uint32_t r) {return neo_truck_routine(r) || neo_effects_routine(r) || neo_flyer_routine(r) || neo_props_routine(r) || neo_rest_routine(r) || neo_dome_routine(r) || neo_barrel_routine(r) || neo_wagon_routine(r) || neo_arab_routine(r);}
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
    ga_fail(e,r);return GA_UNSUPPORTED;
}
