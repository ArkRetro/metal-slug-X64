#ifndef MSX_SCENE_CODEC_H
#define MSX_SCENE_CODEC_H
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
static inline bool scene_decode(uint8_t *out,size_t size,const uint8_t *in,size_t length,unsigned codec) {
    if (codec==0) {
        if (length!=size) return false;
        for (size_t i=0;i<size;i++) out[i]=in[i];
        return true;
    }
    if (codec!=1 || (length&1)) return false;
    size_t n=0;
    for (size_t i=0;i<length;i+=2) {
        unsigned run=in[i];
        if (!run || run>size-n) return false;
        for (unsigned j=0;j<run;j++) out[n++]=in[i+1];
    }
    return n==size;
}
static inline uint32_t scene_crc32(const uint8_t *p,size_t size) {
    uint32_t crc=~0u;
    for (size_t i=0;i<size;i++) {
        crc^=p[i];
        for (unsigned b=0;b<8;b++) crc=(crc>>1)^(0xedb88320u & (0u-(crc&1)));
    }
    return ~crc;
}
#endif
