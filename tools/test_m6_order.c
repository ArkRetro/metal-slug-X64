/* The order of the Mission 6 bridge's update against MAME 0.264
 * (captures-m6/fam/O, MSX_FULL_ORDER_TAP + MSX_FULL_CAMERA; data/
 * m6order_fixture.json.gz): in each of the 1650 updates of the driver 0BCE2C
 *  - the CPU makes the camera's store 10E9D8, 0C81C0's step of 10D7B6 (+0,
 *    then +14 by 01ABC8), the swing 0BD1B8, the driver's 10E94E and its
 *    read of +36 in this order (what neo_world_camera / neo_world_tick do);
 *  - the layer step neo_layers_step_own with the root step 10E970/10E974,
 *    the flags 10E956.. and the factors of the camera store, and the
 *    driver's 10E94E of the update before (w->bridge_dx), gives MAME's +0;
 *  - +36 from that x (the world's formula, 0C748E) is the one the driver read.
 */
#include <stdint.h>
#include <stdio.h>
#include "neo_train.h"
#include "m6order_fixture.h"
enum { FRAME,CAM,STEP,WX,W14,W36,SWING,DRIVE,R36,XB,XA,STEPV,DRIVEV,O36,RVX,RVY,FX,FY,FRX,FRY };
int main(void) {
    unsigned n=sizeof m6order_cases/sizeof m6order_cases[0],bad=0,own=0;
    int32_t previous=0;   /* 0BCDC2: the driver's first 10E94E is its own (10C706 = 0) */
    for (unsigned i=0;i<n;i++) {
        const int64_t *v=m6order_cases[i].v;
        bool ok=v[CAM]>=0 && v[CAM]<v[WX] && v[WX]<v[W14] && v[W14]<v[SWING] && v[SWING]<v[DRIVE] && v[DRIVE]<v[R36];
        if (v[STEP]>=0) {ok&=v[CAM]<v[STEP] && v[STEP]<v[WX] && (uint32_t)v[STEPV]==(uint32_t)previous;own++;}
        if (v[W36]>=0) ok&=v[W14]<v[W36] && v[W36]<v[SWING];
        neo_layer_t l[NEO_LAYERS]={0};
        for (unsigned k=0;k<NEO_LAYERS;k++) {l[k].fx=l[k].fy=0x100;}
        l[NEO_LAYER_MID].x=(int32_t)(uint32_t)v[XB];l[NEO_LAYER_MID].fx=(int16_t)v[FX];l[NEO_LAYER_MID].fy=(int16_t)v[FY];
        l[NEO_LAYER_MID].frac_x=(uint8_t)v[FRX];l[NEO_LAYER_MID].frac_y=(uint8_t)v[FRY];
        neo_layers_step_own(l,(int32_t)(uint32_t)v[RVX],(int32_t)(uint32_t)v[RVY],m6order_cases[i].flags,true,previous);
        ok&=(uint32_t)l[NEO_LAYER_MID].x==(uint32_t)v[XA];
        int16_t o36=(int16_t)((int16_t)(uint16_t)((0xffc00000u-(uint32_t)l[NEO_LAYER_MID].x)>>16)>>4);
        ok&=(uint16_t)o36==(uint16_t)v[O36];
        if (!ok && bad++<5) printf("FAIL update %u (frame %lld): x %08x (want %08llx) +36 %04x (want %04llx)\n",i,(long long)v[FRAME],(unsigned)l[NEO_LAYER_MID].x,(unsigned long long)v[XA],(uint16_t)o36,(unsigned long long)v[O36]);
        previous=(int32_t)(uint32_t)v[DRIVEV];
    }
    if (bad) {printf("FAIL: bridge update order %u of %u updates differ\n",bad,n);return 1;}
    printf("PASS: bridge update order: %u updates (camera, layer step, swing, driver; %u with the driver's own step) equal to MAME\n",n,own);
    return 0;
}
