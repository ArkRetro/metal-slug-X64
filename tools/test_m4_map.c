/* Mission 4 map reconnaissance (docs/MISION4.md): the arcade's kernel cell
 * reads of the walk (MAME 0.264, tools/capture_m4_map.sh: every byte 0C79FE
 * read from the terrain ring 10C72E +88 in frames 149829..157581,
 * data/m4_map_fixture.json.gz) against romfs/mission4.pack as the runtime
 * reads it (neo_collision_cell on the pack's grid with its row origin).
 * tools/extract_m4_map_fixture.py explains every read from the ROM: static
 * cells (checked here, all of them), cells of the 0C70FC patches and of the
 * 0C88B0 stamps (their native events are phase 2; counted). Also the stage
 * record: descriptor 016E1E, camera list 0E741A, the layer rules of the
 * camera list. Private input: romfs/mission4.pack. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_collision.h"
#include "m4_map_fixture.h"
static unsigned rd16(const uint8_t *p) {return (unsigned)p[0]<<8|p[1];}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"romfs/mission4.pack","rb");
    if (!f) {printf("SKIP: Mission 4 map (no pack)\n");return 0;}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *p=malloc((size_t)n);assert(p && fread(p,1,(size_t)n,f)==(size_t)n);fclose(f);
    assert(!memcmp(p,"MSXM",4) && rd16(p+6)==4 && rd16(p+8)==1);
    const uint8_t *st=p+(rd16(p+4)>=4?48:40);
    assert(rd32(st)==0x016E1Eu && rd32(st+44)==0x0E741Au);
    unsigned columns=rd16(st+20),rows=rd16(st+22);int row0=(int16_t)rd16(st+26);uint32_t grid=rd32(st+28);
    assert((int16_t)rd16(st+24)==0 && !rd32(st+32) && grid+columns*rows<=(uint32_t)n);
    /* Layer rules: the terrain 10C72E and 10D7B6 (pack layer 2) follow the
     * camera 1:1; the sky 10E83E is drawn at its object's own position. */
    static const uint8_t follow[8]={1,1,1,1,0x80,0x00,0x7f,0xff},never[8]={1,1,1,1,0,1,0,0};
    assert(!memcmp(st+80,never,8) && !memcmp(st+88,never,8) && !memcmp(st+96,follow,8) && !memcmp(st+104,follow,8));
    neo_collision_set_grid(p+grid,NULL,NULL,columns,rows);neo_collision_set_row_origin(row0);
    unsigned bad=0,total=sizeof(m4_static)/sizeof(m4_static[0]),nonempty=0;
    for (unsigned i=0;i<total;i++) {
        uint8_t v=neo_collision_cell(m4_static[i].x,m4_static[i].y);nonempty+=m4_static[i].v!=0;
        if (v!=m4_static[i].v && bad++<12) fprintf(stderr,"cell (%d, %d): pack %02X, MAME %02X\n",m4_static[i].x,m4_static[i].y,v,m4_static[i].v);
    }
    if (bad) {fprintf(stderr,"FAIL: %u of %u static cells differ\n",bad,total);return 1;}
    unsigned changed=0;for (unsigned i=0;i<M4_PATCHES;i++) changed+=m4_patches[i].cells_changed;
    printf("PASS: Mission 4 map: grid %u x %u (row origin %d), %u of %u distinct static cells read by the arcade's kernel equal (%u not empty); "
           "of %u reads %u static, %u in the cells of %u patches 0C70FC (%u cells changed), %u in 0C88B0 stamps, %u through the ring: 0 unexplained\n",
           columns,rows,row0,total,total,nonempty,(unsigned)M4_READS,(unsigned)M4_READS_STATIC,(unsigned)M4_READS_PATCH,(unsigned)M4_PATCHES,changed,(unsigned)M4_READS_STAMP,(unsigned)M4_READS_RING);
    return 0;
}
