/* Mission 3 train layer (src/neo_train.c) against run4 (data/train_fixture.json.gz). */
#include <stdio.h>
#include "../src/neo_train.h"
#include "train_fixture.h"
int main(void) {
    neo_train_t t;neo_train_reset(&t,train_start_x);t.origin=train_start_origin;
    bool own=false;unsigned skies=0,bad=0,n=sizeof train_updates/sizeof train_updates[0];
    for (unsigned i=0;i<n;i++) {
        for (unsigned k=0;k<train_updates[i].count;k++) {
            unsigned c=train_updates[i].first+k;
            if (!train_calls[c].own) {if (!neo_train_call(&t,train_calls[c].value)) {printf("FAIL: call %06X\n",(unsigned)train_calls[c].value);return 1;}}
            else own=train_calls[c].value!=0;
        }
        unsigned sky=neo_train_sky(&t);
        int16_t o32=t.origin;int o36=(int)((((uint32_t)-(int64_t)t.x>>20)-4u)&0xfffu);
        neo_train_update(&t,train_updates[i].root_dx,own);
        if (neo_train_sky(&t)!=sky) {
            /* 0B8EDC's picture calls in MAME run4 (bpset 0B8F3C, sec04_117966):
             * 118892 / 121768 / 124930 / 144579 with +32 / +36 of 10D7B6. */
            static const uint16_t want[4][2]={{0x000,0x031},{0x100,0x191},{0x4e0,0x5d1},{0x0e0,0x221}};
            if (sky>=4 || (uint16_t)(o32&0xfff)!=want[sky][0] || o36!=want[sky][1]) {
                printf("FAIL: sky picture %u at update %u: +32 %03X +36 %03X\n",sky+1,i,(unsigned)(o32&0xfff),(unsigned)o36);return 1;
            }
            skies++;
        }
        if (t.x!=train_updates[i].x || t.origin!=train_updates[i].origin) {
            if (bad++<5) printf("update %u: x %d/%d origin %d/%d\n",i,t.x,train_updates[i].x,t.origin,train_updates[i].origin);
            t.x=train_updates[i].x;t.origin=train_updates[i].origin;
        }
    }
    if (bad) {printf("FAIL: train %u of %u updates differ\n",bad,n);return 1;}
    if (skies!=4) {printf("FAIL: %u sky pictures of 4\n",skies);return 1;}
    printf("PASS: train layer 10D7B6, %u updates (x +14 and column origin +32 exact), sky pictures 0B8EDC 4/4 at MAME's +32/+36\n",n);
    return 0;
}
