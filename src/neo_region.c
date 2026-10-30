#include "neo_region.h"
#include "region_rom.h"
/* Dispatch of the generic actors by routine label (see neo_region.h) and the
 * ROM data tables they read (data/region_rom.json). */
const uint8_t *ga_rom(uint32_t address) {
    for (unsigned i=0;i<sizeof(region_blobs)/sizeof(region_blobs[0]);i++) {
        const region_blob_t *b=&region_blobs[i];
        if (address>=b->address && address<b->address+b->length) return b->data+(address-b->address);
    }
    return NULL;
}
bool neo_region_routine(uint32_t r) {return neo_truck_routine(r);}
ga_result_t neo_region_tick(neo_gactor_t *a,neo_genv_t *e) {
    uint32_t r=ga_l(a,0);
    if (neo_truck_routine(r)) return neo_truck_tick(a,e);
    ga_fail(e,r);return GA_UNSUPPORTED;
}
