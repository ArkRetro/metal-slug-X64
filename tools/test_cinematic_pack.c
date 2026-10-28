#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/cinematic_codec.h"
#include "entry_meta.h"
int main(int argc,char **argv) {
    assert(argc==2);FILE *f=fopen(argv[1],"rb");assert(f);uint8_t header[32];assert(fread(header,1,32,f)==32);
    unsigned frames=msx_cine_be32(header+16),maximum=msx_cine_be32(header+20),pixels=msx_cine_be32(header+8)*msx_cine_be32(header+12);
    assert(frames==MSX_ENTRY_FRAMES && maximum && maximum<=512*1024 && pixels==304*224);
    uint32_t *offsets=malloc((frames+1)*sizeof(*offsets));uint16_t *canvas=calloc(pixels,2);uint8_t *packet=malloc(maximum),*be=malloc(pixels*2);assert(offsets && canvas && packet && be);
    for (unsigned i=0;i<=frames;i++) {uint8_t value[4];assert(fread(value,1,4,f)==4);offsets[i]=msx_cine_be32(value);}
    for (unsigned i=0;i<frames;i++) {
        unsigned length=offsets[i+1]-offsets[i];assert(length>=8 && length<=maximum);
        assert(fseek(f,offsets[i],SEEK_SET)==0 && fread(packet,1,length,f)==length);
        assert(msx_cine_crc32(packet+4,length-4)==msx_cine_be32(packet));
        assert(msx_cine_delta(canvas,pixels,packet+4,length-4));
        for (unsigned p=0;p<pixels;p++) {be[p*2]=canvas[p]>>8;be[p*2+1]=canvas[p]&255;}
        assert(msx_cine_crc32(be,pixels*2)==msx_entry_crc[i]);
    }
    free(offsets);free(canvas);free(packet);free(be);fclose(f);
    printf("PASS: all %u native C decoded frames match independently generated pixel CRCs\n",frames);
}
