/* Mission 3 train layer (src/neo_train.c) against run4 (data/train_fixture.json.gz). */
#include <stdio.h>
#include "../src/neo_train.h"
#include "train_fixture.h"
int main(void) {
    neo_train_t t;neo_train_reset(&t,train_start_x);t.origin=train_start_origin;
    bool own=false;unsigned bad=0,n=sizeof train_updates/sizeof train_updates[0];
    for (unsigned i=0;i<n;i++) {
        for (unsigned k=0;k<train_updates[i].count;k++) {
            unsigned c=train_updates[i].first+k;
            if (!train_calls[c].own) {if (!neo_train_call(&t,train_calls[c].value)) {printf("FAIL: call %06X\n",(unsigned)train_calls[c].value);return 1;}}
            else own=train_calls[c].value!=0;
        }
        neo_train_update(&t,train_updates[i].root_dx,own);
        if (t.x!=train_updates[i].x || t.origin!=train_updates[i].origin) {
            if (bad++<5) printf("update %u: x %d/%d origin %d/%d\n",i,t.x,train_updates[i].x,t.origin,train_updates[i].origin);
            t.x=train_updates[i].x;t.origin=train_updates[i].origin;
        }
    }
    if (bad) {printf("FAIL: train %u of %u updates differ\n",bad,n);return 1;}
    printf("PASS: train layer 10D7B6, %u updates (x +14 and column origin +32 exact)\n",n);
    return 0;
}
