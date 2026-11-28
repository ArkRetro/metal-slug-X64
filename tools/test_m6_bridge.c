/* The Mission 6 bridge driver 0BCE2C / 0BCEB0 (src/neo_m6bridge.c) against
 * MAME 0.264 (captures-m6/fam/R, data/m6bridge_fixture.json.gz): each of the
 * 1650 updates from the bytes it read; its writes must be the capture's
 * (10C706, 10E94E, +32/+34 of 10D7B6, 10C704, 10C70C). */
#include <stdint.h>
#include <stdio.h>
#include "neo_m6bridge.h"
#include "m6bridge_fixture.h"
enum { I_E970,I_C706,I_C70A,I_C704,I_C702,I_O32,I_O34,I_O36,I_C70C };
enum { O_C706,O_E94E,O_O32,O_O34,O_C704,O_C70C };
int main(void) {
    unsigned n=sizeof m6bridge_cases/sizeof m6bridge_cases[0],bad=0,moves=0;
    for (unsigned i=0;i<n;i++) {
        const int64_t *in=m6bridge_cases[i].in,*out=m6bridge_cases[i].out;
        neo_m6bridge_t b={0};
        b.on=true;b.mode=(uint8_t)in[I_C70A];b.speed=(int32_t)(uint32_t)in[I_C706];
        b.index=(int16_t)(uint16_t)in[I_C702];b.count=(uint16_t)in[I_C704];b.lock=(uint16_t)(in[I_C70C]<0?0:in[I_C70C]);
        int32_t before=b.speed;
        int32_t e94e=neo_m6bridge_drive(&b,(int32_t)(uint32_t)in[I_E970]);
        int16_t o32=(int16_t)in[I_O32],o34=(int16_t)in[I_O34];uint16_t count=b.count,lock=b.lock;
        bool moved=b.count && neo_m6bridge_loop(&b,&o32,&o34,(int16_t)(uint16_t)in[I_O36]);
        moves+=moved;
        bool ok=(uint32_t)e94e==(uint32_t)out[O_E94E];
        ok&=out[O_C706]<0?b.speed==before:(uint32_t)b.speed==(uint32_t)out[O_C706];
        ok&=out[O_O32]<0?!moved:(moved && (uint16_t)o32==(uint16_t)out[O_O32] && (uint16_t)o34==(uint16_t)out[O_O34]);
        ok&=out[O_C704]<0?b.count==count:b.count==(uint16_t)out[O_C704];
        ok&=out[O_C70C]<0?b.lock==lock:b.lock==(uint16_t)out[O_C70C];
        if (!ok && bad++<5) printf("FAIL update %u: 10E94E %08lx (want %08llx) +32 %04x moved %d\n",i,(unsigned long)(uint32_t)e94e,(unsigned long long)out[O_E94E],(uint16_t)o32,moved);
    }
    if (bad) {printf("FAIL: bridge driver %u of %u updates differ\n",bad,n);return 1;}
    printf("PASS: bridge driver 0BCE2C/0BCEB0: %u updates equal to MAME (%u origin moves of 10D7B6)\n",n,moves);
    return 0;
}
