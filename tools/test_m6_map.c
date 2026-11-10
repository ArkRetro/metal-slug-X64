/* Mission 6 map reconnaissance (docs/MISION6.md), its three stages (016E5E the
 * coast and the bridge, 016E6E the factory, 016E7E the last arena): the arcade's kernel cell
 * reads of two plays (MAME 0.264: tools/capture_m6.sh, the replay of run4,
 * data/m6_map_fixture.json.gz; and tools/capture_m6_tas.sh, the TAS by game
 * update with no memory write, data/m6tas_map_fixture.json.gz; every byte
 * 0C79FE read from the terrain ring 10C72E +88) against
 * romfs/mission6.pack as the runtime reads it (neo_collision_cell on the
 * pack's grid with its row origin). tools/extract_m6_map_fixture.py explains
 * every read by the ring's own write history: static cells and cells no map
 * holds (checked here, all of them), cells of another column still in the
 * ring slot (the ring, counted) and cells of the template stampers 0C88B0 /
 * 0C8A3C and of the patches 0C70FC (their native events are phase 2;
 * counted). The stage grid of 016E6E repeats the 32 block columns before
 * 159 from there on (the looper 0B0B72 of its list). Also the stage records:
 * descriptors, camera lists and the layer rules of each list. Private
 * input: romfs/mission6.pack. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_collision.h"
#include "m6_map_fixture.h"
#ifndef M6_SOURCE
#define M6_SOURCE "run4 replay"   /* -DM6_SOURCE="\"TAS\"" with data/m6tas_map_fixture.json.gz: the TAS, no memory writes */
#endif
static unsigned rd16(const uint8_t *p) {return (unsigned)p[0]<<8|p[1];}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"romfs/mission6.pack","rb");
    if (!f) {printf("SKIP: Mission 6 map (no pack)\n");return 0;}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *p=malloc((size_t)n);assert(p && fread(p,1,(size_t)n,f)==(size_t)n);fclose(f);
    assert(!memcmp(p,"MSXM",4) && rd16(p+4)>=4 && rd16(p+6)==6 && rd16(p+8)==3);
    static const uint32_t lists[3]={0x0BCA00u,0x0BDCE2u,0x0BE99Eu};
    /* Layer rules: the terrain 10C72E follows the camera 1:1, and so do
     * the objects 10D7B6 / 10E8C6 of the stage 016E5E (factors 0x100 /
     * 0x100; 10D7B6 has no tiles: its map 0BC6E6 is collision only); the
     * background 10E83E (factors 0x80 / 0x80, 0x80 / 0x40, 0x80 / 0x100) is
     * drawn at its object's own position; a layer without a map is never
     * drawn. */
    static const uint8_t follow[8]={1,1,1,1,0x80,0x00,0x7f,0xff},never[8]={1,1,1,1,0,1,0,0};
    for (unsigned k=0;k<3;k++) {
        const uint8_t *st=p+48+128*k;
        assert(rd32(st)==m6_stages[k].descriptor && rd32(st+44)==lists[k]);
        unsigned columns=rd16(st+20),rows=rd16(st+22);int row0=(int16_t)rd16(st+26);uint32_t grid=rd32(st+28);
        assert((int16_t)rd16(st+24)==0 && !rd32(st+32) && grid+columns*rows<=(uint32_t)n);
        assert(!memcmp(st+80,never,8) && !memcmp(st+88,k?never:follow,8) && !memcmp(st+96,k?never:follow,8) && !memcmp(st+104,follow,8));
        neo_collision_set_grid(p+grid,NULL,NULL,columns,rows);neo_collision_set_row_origin(row0);
        unsigned bad=0,total=m6_stages[k].count,nonempty=0;
        for (unsigned i=0;i<total;i++) {
            const m6_cell_t *c=&m6_stages[k].cells[i];
            uint8_t v=neo_collision_cell(c->x,c->y);nonempty+=c->v!=0;
            if (v!=c->v && bad++<12) fprintf(stderr,"stage %u cell (%d, %d): pack %02X, MAME %02X\n",k+1,c->x,c->y,v,c->v);
        }
        if (bad) {fprintf(stderr,"FAIL: Mission 6 stage %u: %u of %u static cells differ\n",k+1,bad,total);return 1;}
        printf("PASS: Mission 6 stage %u map %06lX (" M6_SOURCE "): grid %u x %u (row origin %d), %u of %u distinct cells read by the arcade's kernel equal (%u not empty); "
               "of %u reads %u static, %u where no map has a cell, %u through the ring, %u before the column's load, %u in cells of the stampers 0C88B0 / 0C8A3C (%u stamps, %u cells written), "
               "%u in cells of the %u patches 0C70FC, %u of the previous stage's ring before the load: 0 unexplained\n",
               k+1,(unsigned long)m6_stages[k].descriptor,columns,rows,row0,total,total,nonempty,m6_stages[k].reads,m6_stages[k].reads_static,m6_stages[k].reads_empty,
               m6_stages[k].reads_ring,m6_stages[k].reads_early,m6_stages[k].reads_stamp,m6_stages[k].stamps,m6_stages[k].stamp_cells,m6_stages[k].reads_patch,m6_stages[k].patches,m6_stages[k].reads_previous);
        /* The second terrain object 10D7B6 (stage 016E5E, map 0BC6E6: collision only): the pack has no second grid yet
         * (phase 2); tools/extract_m6_map_fixture.py checked its reads against the ROM map and counted them. */
        if (m6_stages[k].second_reads)
            printf("NOTE: Mission 6 stage %u second terrain object 10D7B6 (" M6_SOURCE "): %u reads, %u equal to the ROM map 0BC6E6 at the list's place (%u distinct cells), %u where it has no cell, %u through the ring, "
                   "%u cells of the map at an origin its owner moved (not explained yet); not in the pack: phase 2\n",k+1,m6_stages[k].second_reads,m6_stages[k].second_static,m6_stages[k].second_distinct,
                   m6_stages[k].second_empty,m6_stages[k].second_ring,m6_stages[k].second_moved);
    }
    return 0;
}
