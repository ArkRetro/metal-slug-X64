#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "../src/scene_codec.h"
int main(void) {
    uint8_t dst[18];memset(dst,0x5a,sizeof(dst));
    const uint8_t runs[]={8,0x12,8,0xab};
    assert(scene_decode(dst+1,16,runs,sizeof(runs),1));
    assert(dst[0]==0x5a && dst[17]==0x5a && dst[1]==0x12 && dst[16]==0xab);
    const uint8_t overflow[]={17,1},zero[]={0,1},shorter[]={15,1};
    assert(!scene_decode(dst+1,16,overflow,2,1));
    assert(!scene_decode(dst+1,16,zero,2,1));
    assert(!scene_decode(dst+1,16,shorter,2,1));
    assert(!scene_decode(dst+1,16,runs,3,1));
    assert(!scene_decode(dst+1,16,runs,4,0));
    assert(!scene_decode(dst+1,16,runs,4,2));
    assert(scene_crc32((const uint8_t*)"123456789",9)==0xcbf43926u);
    puts("PASS: codec exact size, malformed streams, output guards and standard CRC32");
}
