/* Wagon box stamper 0B8FA8 (src/neo_m3props.c) against MAME: the five
 * 0C70FC copies of the sub-map 0B85EA made by 0B911E (data/m3ledge_fixture.json,
 * ring writes of states4/sec04_117966 frames 121043..121064), on a blank
 * Mission 3 grid (860 x 47, origin -8 rows) with the ring window over the
 * wagon. Every cell written by each MAME copy must hold its value in the
 * port's grid after the matching update. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_region.h"
#include "../src/neo_collision.h"
#include "m3ledge_fixture.h"
#define C 860
#define R 47
static uint8_t cells[C*R],dyn[C*R],ring[4096];
static unsigned patches;
static void map_patch(void *ctx,uint32_t layer,uint32_t submap,const int16_t d[6]) {
    (void)ctx;const uint8_t *s=ga_rom(submap);
    if (layer!=0x10c72eu || !s) {fprintf(stderr,"FAIL: patch %06x\n",(unsigned)submap);exit(1);}
    unsigned c=(unsigned)(s[4]<<24|s[5]<<16|s[6]<<8|s[7]),blocks=(unsigned)(s[10]<<8|s[11]),pairs=(unsigned)(s[8]<<8|s[9]);
    neo_collision_patch_window(ga_rom(c),blocks,pairs,blocks,d[0],d[1],d[2],d[3],d[4],d[5]);patches++;
}
int main(void) {
    neo_collision_set_grid(cells,dyn,NULL,C,R);neo_collision_set_ring(ring);neo_collision_set_origin(-8);
    neo_collision_ring(-(1700<<16),0);
    neo_gactor_t a;memset(&a,0,sizeof a);a.active=true;ga_sl(&a,0,0xb911e);
    neo_genv_t e;memset(&e,0,sizeof e);e.map_patch=map_patch;
    unsigned step=0,updates=0,bad=0,checked=0;
    for (;updates<64;updates++) {
        unsigned before=patches;
        ga_result_t r=neo_m3props_tick(&a,&e);
        if (r==GA_UNSUPPORTED || e.unsupported) {fprintf(stderr,"FAIL: unsupported\n");return 1;}
        if (patches!=before) {
            if (step>=M3LEDGE_STEPS) {fprintf(stderr,"FAIL: extra copy\n");return 1;}
            for (unsigned i=0;i<M3LEDGE_CELLS;i++) {
                const m3ledge_cell_t *m=&m3ledge_steps[step][i];checked++;
                if (dyn[m->column*R+m->row]!=m->value) {if (bad++<5) fprintf(stderr,"step %u cell %u,%u: %u, MAME %u\n",step,m->column,m->row,dyn[m->column*R+m->row],m->value);}
            }
            step++;
        }
        if (r==GA_DELETE) break;
    }
    if (bad || step!=M3LEDGE_STEPS) {fprintf(stderr,"FAIL: m3props %u bad, %u/%u copies\n",bad,step,M3LEDGE_STEPS);return 1;}
    printf("PASS: m3props stamper 0B911E (%u copies, %u cells equal to MAME, deleted after %u updates)\n",step,checked,updates+1);
    return 0;
}
