/* The soldier's push 0AF2BA (0049E8 with the row 0AE9B0: kind 13, class 1)
 * against the wagon door 0391EA, through the runtime's path
 * (neo_world_soldier_attack: the class-1 bodies registered in the previous
 * update), against MAME 0.264: states4/sec04_117966, frame 121357, the
 * debugger at 0AF2BA (before 0049E8) and 0AF2C4 (after) for the two
 * soldiers +76 = 7 of the records 2DA880/2DA896 and the door 101E64 of
 * 2DA800 (docs/MISION3.md "Puertas de los vagones"). Words: the door's
 * +14, +3C, +40, +44, +48, +4C, +50, +54, +58, +5C and the soldier's +14,
 * +3C, +40, +44, +48, +5C. The first soldier (104BC4) sets the door's +5A..
 * +5E; the second (103934) finds +5A = 13 already (no change). */
#include <stdio.h>
#include <string.h>
#include "../src/neo_world.h"
static const uint32_t door_pre[10]={0x44003affu,0x011eb084u,0x01200000u,0x0121c868u,0x01200000u,0x01000000u,0x0003954eu,0,0x0303004bu,0xd0180480u};
static const uint32_t door_post[10]={0x44003affu,0x011eb084u,0x01200000u,0x0121c868u,0x01200000u,0x01000000u,0x0003954eu,0,0x030313c0u,0xecfc0480u};
static const uint32_t soldier[2][6]={{0x84003affu,0x0126b084u,0x01310000u,0x0126b084u,0x01310000u,0xf6070030u},
                                    {0x84003affu,0x010eb084u,0x01310000u,0x010eb084u,0x01310000u,0xf5000030u}};
static const unsigned at[10]={0x14,0x3c,0x40,0x44,0x48,0x4c,0x50,0x54,0x58,0x5c};
static neo_world_t w;
int main(void) {
    static const int16_t row[7]={-16,8,32,32,0x13,1,0};   /* ROM 0AE9B0 */
    neo_gactor_t *g=&w.gactors[0];g->active=true;g->index=0;
    for (unsigned i=0;i<10;i++) ga_sl(g,at[i],door_pre[i]);
    w.last_boxes[0].owner=0;w.last_boxes[0].xbins=0xffffu;w.last_boxes[0].ybins=0xffu;w.last_box_count=1;
    for (unsigned k=0;k<2;k++) {
        neo_contact_actor_t c;memset(&c,0,sizeof c);
        c.flags15=(uint8_t)(soldier[k][0]>>16);
        c.projected_x=(int32_t)soldier[k][1];c.projected_y=(int32_t)soldier[k][2];c.cached_x=(int32_t)soldier[k][3];c.cached_y=(int32_t)soldier[k][4];
        neo_world_soldier_attack(&w,-1,&c,(uint8_t)(soldier[k][5]>>8),row);
        for (unsigned i=0;i<10;i++) if (ga_l(g,at[i])!=door_post[i]) {
            fprintf(stderr,"FAIL: m3push soldier %u door +%02X %08X, MAME %08X\n",k,at[i],(unsigned)ga_l(g,at[i]),(unsigned)door_post[i]);return 1;
        }
    }
    printf("PASS: m3push soldier push 0AF2BA -> door 0391EA (+5A..+5E equal to MAME 121357, two soldiers)\n");
    return 0;
}
