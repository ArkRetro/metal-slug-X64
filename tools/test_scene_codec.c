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
    assert(scene_crc32_bitwise((const uint8_t*)"123456789",9)==0xcbf43926u);
    /* Table CRC == bitwise CRC on random buffers of every length 0..4096 and
     * on a full 16 KiB chunk. */
    static uint8_t buf[16384];uint32_t seed=0x1234567u;
    for (size_t i=0;i<sizeof(buf);i++) {seed=seed*1103515245u+12345u;buf[i]=(uint8_t)(seed>>16);}
    for (size_t n=0;n<=4096;n++) assert(scene_crc32(buf+(n%7),n)==scene_crc32_bitwise(buf+(n%7),n));
    assert(scene_crc32(buf,sizeof(buf))==scene_crc32_bitwise(buf,sizeof(buf)));
    puts("PASS: codec exact size, malformed streams, output guards, standard CRC32, table CRC == bitwise CRC");
}
