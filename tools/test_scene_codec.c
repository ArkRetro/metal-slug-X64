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
    assert(!scene_decode(dst+1,16,runs,4,3));
    /* Codec 2, one LZ4 block (docs/COMPRESION.md): literals, a match, an
     * overlapping match (offset 1 and 2), the last literals; then every
     * malformed block is refused without writing outside the output. */
    const uint8_t lz[]={0x21,'a','b',2,0, 0x10,'c',1,0, 0x40,'w','x','y','z'};   /* ab + ababa (offset 2), c + cccc (offset 1), wxyz */
    memset(dst,0x5a,sizeof(dst));
    assert(scene_decode(dst+1,16,lz,sizeof(lz),2));
    assert(!memcmp(dst+1,"abababa" "ccccc" "wxyz",16));
    assert(dst[0]==0x5a && dst[17]==0x5a);
    assert(!scene_decode(dst+1,15,lz,sizeof(lz),2) && !scene_decode(dst+1,17,lz,sizeof(lz),2));   /* not the exact size */
    assert(!scene_decode(dst+1,16,lz,sizeof(lz)-1,2) && !scene_decode(dst+1,16,lz,5,2) && !scene_decode(dst+1,16,lz,0,2));   /* cut short */
    const uint8_t before[]={0x20,'a','b',3,0, 0x40,'w','x','y','z'},zero_off[]={0x20,'a','b',0,0, 0x40,'w','x','y','z'};
    assert(!scene_decode(dst+1,12,before,sizeof(before),2));   /* match from before the output */
    assert(!scene_decode(dst+1,12,zero_off,sizeof(zero_off),2));
    const uint8_t long_match[]={0x2f,'a','b',2,0,200, 0x40,'w','x','y','z'},long_lit[]={0xf0,255,255,1,'a'};
    assert(!scene_decode(dst+1,16,long_match,sizeof(long_match),2));   /* match past the output */
    assert(!scene_decode(dst+1,16,long_lit,sizeof(long_lit),2));       /* literals past the input */
    assert(dst[0]==0x5a && dst[17]==0x5a);
    assert(scene_crc32((const uint8_t*)"123456789",9)==0xcbf43926u);
    assert(scene_crc32_bitwise((const uint8_t*)"123456789",9)==0xcbf43926u);
    /* Table CRC == bitwise CRC on random buffers of every length 0..4096 and
     * on a full 16 KiB chunk. */
    static uint8_t buf[16384];uint32_t seed=0x1234567u;
    for (size_t i=0;i<sizeof(buf);i++) {seed=seed*1103515245u+12345u;buf[i]=(uint8_t)(seed>>16);}
    for (size_t n=0;n<=4096;n++) assert(scene_crc32(buf+(n%7),n)==scene_crc32_bitwise(buf+(n%7),n));
    assert(scene_crc32(buf,sizeof(buf))==scene_crc32_bitwise(buf,sizeof(buf)));
    puts("PASS: codecs (run pairs, LZ4 block) exact size, malformed streams, output guards, standard CRC32, table CRC == bitwise CRC");
}
