/* Mission 5 map reconnaissance (docs/MISION5.md), its three stages (016E2E the
 * street, 016E3E the underground, 016E4E the sewer): the arcade's kernel cell
 * reads of two plays (MAME 0.264: tools/capture_m5.sh, the replay of run4,
 * data/m5_map_fixture.json.gz; and tools/capture_m5_tas.sh, the TAS by game
 * update with no memory write, data/m5tas_map_fixture.json.gz; every byte
 * 0C79FE read from the terrain ring 10C72E +88) against
 * romfs/mission5.pack as the runtime reads it (neo_collision_cell on the
 * pack's grid with its row origin). tools/extract_m5_map_fixture.py explains
 * every read by the ring's own write history: static cells and cells no map
 * holds (checked here, all of them), cells of another column still in the
 * ring slot (the ring, counted) and cells of the template stampers 0C88B0 /
 * 0C8A3C (their native events are phase 2; counted). Also the stage records:
 * descriptors, camera lists and the layer rules of each list. Private
 * input: romfs/mission5.pack. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_collision.h"
#include "m5_map_fixture.h"
#ifndef M5_SOURCE
#define M5_SOURCE "run4 replay"   /* -DM5_SOURCE="\"TAS\"" with data/m5tas_map_fixture.json.gz: the TAS, no memory writes */
#endif
static unsigned rd16(const uint8_t *p) {return (unsigned)p[0]<<8|p[1];}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"romfs/mission5.pack","rb");
    if (!f) {printf("SKIP: Mission 5 map (no pack)\n");return 0;}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *p=malloc((size_t)n);assert(p && fread(p,1,(size_t)n,f)==(size_t)n);fclose(f);
    assert(!memcmp(p,"MSXM",4) && rd16(p+4)>=4 && rd16(p+6)==5 && rd16(p+8)==3);
    static const uint32_t lists[3]={0x0BB8E6u,0x0BB9BAu,0x0BBA24u};
    /* Layer rules: the terrain 10C72E follows the camera 1:1; a background
     * (stage 1: 10D7B6, pack layer 2, factors 0xC0 / 0) is drawn at its
     * object's own position; a layer without a map is never drawn. */
    static const uint8_t follow[8]={1,1,1,1,0x80,0x00,0x7f,0xff},never[8]={1,1,1,1,0,1,0,0};
    for (unsigned k=0;k<3;k++) {
        const uint8_t *st=p+48+128*k;
        assert(rd32(st)==m5_stages[k].descriptor && rd32(st+44)==lists[k]);
        unsigned columns=rd16(st+20),rows=rd16(st+22);int row0=(int16_t)rd16(st+26);uint32_t grid=rd32(st+28);
        assert((int16_t)rd16(st+24)==0 && !rd32(st+32) && grid+columns*rows<=(uint32_t)n);
        assert(!memcmp(st+80,never,8) && !memcmp(st+88,never,8) && !memcmp(st+96,never,8) && !memcmp(st+104,follow,8));
        neo_collision_set_grid(p+grid,NULL,NULL,columns,rows);neo_collision_set_row_origin(row0);
        unsigned bad=0,total=m5_stages[k].count,nonempty=0;
        for (unsigned i=0;i<total;i++) {
            const m5_cell_t *c=&m5_stages[k].cells[i];
            uint8_t v=neo_collision_cell(c->x,c->y);nonempty+=c->v!=0;
            if (v!=c->v && bad++<12) fprintf(stderr,"stage %u cell (%d, %d): pack %02X, MAME %02X\n",k+1,c->x,c->y,v,c->v);
        }
        if (bad) {fprintf(stderr,"FAIL: Mission 5 stage %u: %u of %u static cells differ\n",k+1,bad,total);return 1;}
        printf("PASS: Mission 5 stage %u map %06lX (" M5_SOURCE "): grid %u x %u (row origin %d), %u of %u distinct cells read by the arcade's kernel equal (%u not empty); "
               "of %u reads %u static, %u where no map has a cell, %u through the ring, %u before the column's load, %u in cells of the stampers 0C88B0 / 0C8A3C (%u stamps, %u cells written), %u of the previous stage's ring before the load: 0 unexplained\n",
               k+1,(unsigned long)m5_stages[k].descriptor,columns,rows,row0,total,total,nonempty,m5_stages[k].reads,m5_stages[k].reads_static,m5_stages[k].reads_empty,
               m5_stages[k].reads_ring,m5_stages[k].reads_early,m5_stages[k].reads_stamp,m5_stages[k].stamps,m5_stages[k].stamp_cells,m5_stages[k].reads_previous);
    }
    return 0;
}
