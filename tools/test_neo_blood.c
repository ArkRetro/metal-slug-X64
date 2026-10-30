/* BLOOD (RAM 10FD8F) call sites translated natively: the palette resources
 * of src/neo_blood.c against data/blood_fixture.json (ON: MAME capture with
 * 10FD8F = 1, tools/capture_blood.lua; OFF: the euro recordings). Also
 * checks that the native world starts with the MSX_BLOOD setting. */
#include <stdio.h>
#include "../src/neo_blood.h"
#include "../src/neo_world.h"
#include "blood_fixture.h"
int main(void) {
    unsigned n=0;
    for (unsigned i=0;i<sizeof(blood_sites)/sizeof(blood_sites[0]);i++) {
        const blood_site_t *s=&blood_sites[i];
        uint16_t on=neo_blood_palette(s->site,true),off=neo_blood_palette(s->site,false);
        if (on!=s->on || off!=s->off) {
            printf("FAIL blood site %05X: on %03X/%03X off %03X/%03X\n",(unsigned)s->site,on,s->on,off,s->off);return 1;
        }
        n+=s->requests;
    }
    if (neo_blood_palette(0x9f598,true)) {printf("FAIL untranslated site answered\n");return 1;}
    static neo_world_t w;neo_world_init(&w,0);
    if (w.blood!=(MSX_BLOOD!=0)) {printf("FAIL world blood %d, MSX_BLOOD %d\n",w.blood,MSX_BLOOD);return 1;}
    printf("Blood: %u call sites, %u recorded ON requests, default %s\n",(unsigned)(sizeof(blood_sites)/sizeof(blood_sites[0])),n,MSX_BLOOD?"ON":"OFF");
    return 0;
}
