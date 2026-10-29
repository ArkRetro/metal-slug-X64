#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/cinematic_codec.h"
int main(void) {
    uint16_t canvas[8]={1,2,3,4,5,6,7,8},original[8];memcpy(original,canvas,sizeof(canvas));
    const uint8_t valid[]={0,2,0,2,0x12,0x35,0xab,0xcd,0,2,0,1,0xff,0xff,0,0,0,0};
    for (size_t i=0;i<sizeof(valid);i++) {
        assert(!msx_cine_delta(canvas,8,valid,i));assert(!memcmp(canvas,original,sizeof(canvas)));
    }
    const uint8_t past_end[]={0xff,0xff,0,1,0,0,0,0,0,0};
    assert(!msx_cine_delta(canvas,8,past_end,sizeof(past_end)));assert(!memcmp(canvas,original,sizeof(canvas)));
    const uint8_t skip_then_overflow[]={0,8,0,1,0,1,0,0,0,0};
    assert(!msx_cine_delta(canvas,8,skip_then_overflow,sizeof(skip_then_overflow)));assert(!memcmp(canvas,original,sizeof(canvas)));
    assert(msx_cine_delta(canvas,8,valid,sizeof(valid)));
    assert(canvas[0]==1 && canvas[1]==2 && canvas[2]==0x1235 && canvas[3]==0xabcd && canvas[4]==5 && canvas[5]==6 && canvas[6]==0xffff && canvas[7]==8);
    const uint8_t repeated[]={0,1,0x80,3,0x12,0x35,0,0,0,0};
    assert(msx_cine_delta(canvas,8,repeated,sizeof(repeated)));
    assert(canvas[0]==1 && canvas[1]==0x1235 && canvas[2]==0x1235 && canvas[3]==0x1235 && canvas[4]==5);
    const uint8_t bad_repeat[]={0,0,0x80,0,0,0,0,0};
    assert(!msx_cine_delta(canvas,8,bad_repeat,sizeof(bad_repeat)));
    assert(msx_cine_crc32((const uint8_t *)"123456789",9)==0xcbf43926u);
    puts("PASS: cinematic delta edits, truncation, bounds, atomic rejection and standard CRC32");
}
