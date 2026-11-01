/* Host check of src/cinematic_stream.h against the one-shot codec: for every
 * packet of a v1 movie and its v2 (LZ4) form, decode in place in random
 * pieces, CRC four bytes at a time, validate and apply in random pieces; the
 * bytes, CRCs and every frame's pixels must match msx_cine_* exactly.
 * usage: test_cinematic_stream v1.msxc v2.msxc */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/cinematic_stream.h"
static uint8_t *slurp(const char *path,size_t *size) {
    FILE *f=fopen(path,"rb");if (!f) {perror(path);exit(1);}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *b=malloc((size_t)n);if (!b || fread(b,1,(size_t)n,f)!=(size_t)n) exit(1);
    fclose(f);*size=(size_t)n;return b;
}
static unsigned seed=12345;
static size_t rnd(size_t lo,size_t hi) {seed=seed*1103515245u+12345u;return lo+(seed>>8)%(hi-lo+1);}
int main(int argc,char **argv) {
    if (argc!=3) {fprintf(stderr,"usage: %s v1.msxc v2.msxc\n",argv[0]);return 2;}
    size_t n1,n2;uint8_t *v1=slurp(argv[1],&n1),*v2=slurp(argv[2],&n2);
    if (memcmp(v1,"MSXC",4) || msx_cine_be32(v1+4)!=1 || memcmp(v2,"MSXC",4) || msx_cine_be32(v2+4)!=2) {fprintf(stderr,"versions\n");return 1;}
    unsigned w=msx_cine_be32(v1+8),h=msx_cine_be32(v1+12),count=msx_cine_be32(v1+16),maximum=msx_cine_be32(v1+20);
    size_t pixels=(size_t)w*h;
    static msx_cine_crc_tables_t tables;msx_cine_crc_tables_init(&tables);
    uint16_t *a=calloc(pixels,2),*b=calloc(pixels,2);
    uint8_t *room=malloc(maximum+(maximum>>8)+256);
    size_t compressed=0;
    for (unsigned i=0;i<count;i++) {
        uint32_t o=msx_cine_be32(v1+32+4*i),e=msx_cine_be32(v1+32+4*(i+1)),len=e-o;
        const uint8_t *raw=v1+o;
        // v2 stored packet, decoded in place in random pieces.
        uint32_t first=msx_cine_be32(v2+32+4*(count+1)+4*i),next=msx_cine_be32(v2+32+4*(count+1)+4*(i+1));
        uint32_t at=first&~1u,have=(next&~1u)-at-(first&1u);
        if (have>len) {fprintf(stderr,"packet %u: stored %u > raw %u\n",i,have,len);return 1;}
        memset(room,0xA5,maximum+(maximum>>8)+256);
        if (have==len) memcpy(room,v2+at,len);
        else {
            uint8_t *src=(uint8_t *)(((uintptr_t)room+len+(have>>8)+48u-have)&~(uintptr_t)15);
            memcpy(src,v2+at,have);
            msx_lz4r_t z;msx_lz4r_init(&z,src,have,room,len);int r;
            while (!(r=msx_lz4r_run(&z,rnd(1,20000)))) {}
            if (r!=1) {fprintf(stderr,"packet %u: lz4 %d\n",i,r);return 1;}
            compressed++;
        }
        if (memcmp(room,raw,len)) {fprintf(stderr,"packet %u: bytes differ\n",i);return 1;}
        // CRC in random pieces.
        uint32_t crc=~0u;size_t pos=4;
        while (pos<len) {size_t k=rnd(1,9000);if (k>len-pos) k=len-pos;crc=msx_cine_crc32_fast(&tables,crc,room+pos,k);pos+=k;}
        if (~crc!=msx_cine_crc32(raw+4,len-4) || ~crc!=msx_cine_be32(raw)) {fprintf(stderr,"packet %u: crc\n",i);return 1;}
        msx_cine_walk_t walk={0};int v;
        while (!(v=msx_cine_validate_part(&walk,pixels,room+4,len-4,rnd(1,4000)))) {}
        if (v!=1 || !msx_cine_delta_validate(pixels,raw+4,len-4)) {fprintf(stderr,"packet %u: validate %d\n",i,v);return 1;}
        msx_cine_delta_apply_trusted(a,raw+4);
        msx_cine_apply_t ap={0};size_t lo=(size_t)-1,hi=0;
        while (!msx_cine_apply_part(&ap,b,room+4,rnd(1,30000),&lo,&hi)) {}
        if (memcmp(a,b,pixels*2)) {fprintf(stderr,"frame %u: pixels differ\n",i);return 1;}
        if (hi>pixels || (hi && lo>hi)) {fprintf(stderr,"frame %u: span %zu..%zu\n",i,lo,hi);return 1;}
    }
    printf("cinematic stream PASS %s: %u packets (%zu LZ4), %ux%u, canvas crc=%08x\n",argv[2],count,compressed,w,h,msx_cine_crc32((const uint8_t *)a,pixels*2));
    free(v1);free(v2);free(a);free(b);free(room);return 0;
}
