/* Mission 3 camera height on the bridge and over the gap: the camera list's
 * calls 0B8EBC(n) (0C8764: A0 = the command + 8; 0B8EBC: MOVE.L (A0),6960)
 * set the y autoscroll that op 0x40 0200 (695A = 2) turns on at 2888. MAME
 * run4 (tools/capture_m3_actors.sh trace.csv, fore_y with 256 = level):
 * from 122146 the camera goes down 8 px per update (0B8EBC(-8): 2 = 0xFFF80000)
 * to 64 px below (fore_y 320) for the bridge and the river, 0B8EBC(8) at
 * 123924 brings it 32 px above (224) for 3296..3463, 0B8EBC(-4) at 126262
 * back to the level (256) at 3716; op 0x40 0000 (141260) ends it. Before,
 * the port ignored 0B8EBC: the camera stayed level and the bridge, its
 * trestles and the water were never on screen. The player stands at the
 * camera's point (no vertical tracking) and walks right; no script locks.
 * Private input: romfs/mission3.pack. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/neo_world.h"
static unsigned rd16(const uint8_t *p) {return (unsigned)p[0]<<8|p[1];}
static uint32_t rd32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
static neo_world_t w;
int main(int argc,char **argv) {
    FILE *f=fopen(argc>1?argv[1]:"romfs/mission3.pack","rb");
    if (!f) {printf("SKIP: Mission 3 camera bridge (no pack)\n");return 0;}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *p=malloc((size_t)n);assert(p && fread(p,1,(size_t)n,f)==(size_t)n);fclose(f);
    assert(!memcmp(p,"MSXM",4) && rd16(p+6)==3 && rd16(p+4)==4);
    const uint8_t *st=p+48;assert(rd32(st)==0x016E0Eu);
    uint32_t rom=rd32(st+44),off=rd32(st+48),len=rd32(st+52);assert(rom==0x0B8968u && off+len<=(uint32_t)n);
    w.stage_camera=p+off;w.stage_camera_size=(uint16_t)len;w.stage_camera_base=rom;
    /* Plateaus of MAME's camera height (fore_y - 256 as height: up > 0). */
    static const struct {unsigned lo,hi;int height;} want[]={{2900,3100,-64},{3300,3460,32},{3730,3900,0}};
    int seen[3]={99999,99999,99999};int lo[3]={99999,99999,99999},hi[3]={-99999,-99999,-99999};
    unsigned prev=0,updates=0;int vmax=0,y0=0;
    for (;updates<20000;updates++) {
        neo_world_player_t pl;memset(&pl,0,sizeof(pl));pl.alive=true;pl.follow_x=pl.follow_y=0xff;pl.flags15=0x40;   /* on the ground: op 4 (0C8524) tests +15 bit 6 */
        pl.px=0x140;pl.py=w.camera_native?w.script.w69ba:0x100;
        neo_world_camera(&w,&pl);
        unsigned pr=neo_camera_progress(&w.camera);int y=(int)(((int64_t)w.camera.y+0xffff)>>16);
        if (updates && abs(y-y0)>vmax) vmax=abs(y-y0);
        y0=y;
        for (unsigned k=0;k<3;k++) if (pr>=want[k].lo && pr<=want[k].hi) {seen[k]=y;if (y<lo[k]) lo[k]=y;if (y>hi[k]) hi[k]=y;}
        if (pr>=3900) break;
        assert(pr>=prev);prev=pr;
    }
    for (unsigned k=0;k<3;k++) {
        printf("progress %u..%u: height %d..%d (MAME %d)\n",want[k].lo,want[k].hi,lo[k],hi[k],want[k].height);
        assert(lo[k]==want[k].height && hi[k]==want[k].height);
    }
    assert(vmax<=8);
    printf("PASS: Mission 3 camera bridge: 0B8EBC heights -64 / +32 / 0 as MAME, at most 8 px per update (%u updates)\n",updates);
    (void)seen;return 0;
}
