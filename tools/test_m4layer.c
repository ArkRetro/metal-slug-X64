/* Mission 4 boss: the layer object 10D7B6 as the camera's 0C8240 steps it
 * (src/neo_train.c neo_layerobj_step) with the flag byte 10E957 = 3
 * (0C7BD0 of the boss's 024BDA): +8/+C from the treads' speeds 694E/6952
 * through 0C6F9E (factors +26/+28 = 0x100), 01AC3C, 01ABCC against the
 * scene root 10E968. Closed loop from the first update after the body's
 * constructor: the native +0/+4 are never reset; every update that read
 * both the layer's +14/+18 and the root's must see the native values
 * (data/m4layer_fixture.json.gz, tools/extract_m4layer_fixture.py, trace T
 * of the TAS). The 69CC added to +18 is not in the trace: the updates
 * from the treads' 0C8022 (023432) on whose y differs are counted, not
 * compared (the camera's shake is tools/test_m4_camera.c's). */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../src/neo_train.h"
#include "m4layer_fixture.h"
#define N (sizeof(m4layer_records)/sizeof(m4layer_records[0]))
static uint32_t rl(const uint8_t *l,unsigned o) {return (uint32_t)l[o]<<24|(uint32_t)l[o+1]<<16|(uint32_t)l[o+2]<<8|l[o+3];}
static void sl(uint8_t *l,unsigned o,uint32_t v) {l[o]=(uint8_t)(v>>24);l[o+1]=(uint8_t)(v>>16);l[o+2]=(uint8_t)(v>>8);l[o+3]=(uint8_t)v;}
int main(void) {
    uint8_t l[0x88];memset(l,0,sizeof(l));
    l[0x26]=1;l[0x28]=1;sl(l,0x10,0x10e968u);                 /* factors 0x100 (op 0x1C), parent: the root (01AB9C) */
    int32_t own[2]={0,0};bool started=false,pulsed=false;
    unsigned x_equal=0,y_equal=0,shake_updates=0,failures=0,stepped=0;uint8_t epoch=1;
    for (unsigned k=1;k<N;k++) {
        const m4layer_record_t *p=&m4layer_records[k-1],*r=&m4layer_records[k];
        if (p->has_speed[0]) own[0]=(int32_t)p->s0;
        if (p->has_speed[1]) own[1]=(int32_t)p->s1;
        if (p->pulse) pulsed=true;
        if (!started) {
            if (!(r->has_layer && r->has_root)) continue;
            sl(l,0,r->l14-r->r14);sl(l,4,r->l18-r->r18);sl(l,0x14,r->l14);sl(l,0x18,r->l18);l[0x24]=epoch;started=true;continue;
        }
        epoch=epoch==1?2:1;stepped++;
        neo_layerobj_step(l,3,own,0,0,(int32_t)r->r14,(int32_t)r->r18,0,epoch,0);
        if (!(r->has_layer && r->has_root)) {
            /* No root read this update: +14/+18 hold the previous root; only +0/+4 carry on. */
            continue;
        }
        if (rl(l,0x14)==r->l14) x_equal++;
        else if (failures++<8) fprintf(stderr,"frame %u +14 native %08X recorded %08X\n",(unsigned)r->frame,(unsigned)rl(l,0x14),(unsigned)r->l14);
        if (rl(l,0x18)==r->l18) y_equal++;
        else if (pulsed) shake_updates++;
        else if (failures++<8) fprintf(stderr,"frame %u +18 native %08X recorded %08X\n",(unsigned)r->frame,(unsigned)rl(l,0x18),(unsigned)r->l18);
    }
    if (failures || !x_equal) {fprintf(stderr,"FAIL: Mission 4 boss layer 10D7B6: %u mismatches\n",failures);return 1;}
    printf("PASS: Mission 4 boss layer 10D7B6 (0C8240, owner 3): %u updates stepped, +14 equal in %u, +18 in %u; %u updates in the 0C8022 shake not compared (69CC not traced)\n",stepped,x_equal,y_equal,shake_updates);
    return 0;
}
