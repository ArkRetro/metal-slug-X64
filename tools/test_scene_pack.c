/* Host check of the ROM scene pack against the runtime decoder: every chunk
 * of romfs/scene.pack decodes to exactly SCENE_CHUNK_BYTES with
 * scene_decode, and the table CRC used at runtime equals both the bitwise
 * reference CRC and the CRC recorded by tools/pack_scene.py (zlib.crc32). */
#include <stdio.h>
#include <stdlib.h>
#include "../src/scene_assets.h"
#include "../src/scene_codec.h"
#include "scene_meta.h"
int main(int argc,char **argv) {
    const char *path=argc>1?argv[1]:"romfs/scene.pack";
    FILE *f=fopen(path,"rb");
    if (!f) {fprintf(stderr,"scene pack: cannot open %s\n",path);return 1;}
    static uint8_t pack[SCENE_PACK_BYTES+64],out[SCENE_CHUNK_BYTES];
    size_t got=fread(pack,1,sizeof(pack),f);fclose(f);
    if (got!=SCENE_PACK_BYTES) {fprintf(stderr,"scene pack: %zu bytes, meta says %u\n",got,(unsigned)SCENE_PACK_BYTES);return 1;}
    unsigned bad=0;
    for (unsigned i=0;i<SCENE_CHUNK_COUNT;i++) {
        const scene_chunk_t *c=&scene_chunks[i];
        bool ok=c->offset+c->length<=SCENE_PACK_BYTES && scene_decode(out,sizeof(out),pack+c->offset,c->length,c->codec);
        uint32_t t=ok?scene_crc32(out,sizeof(out)):0,b=ok?scene_crc32_bitwise(out,sizeof(out)):1;
        if (!ok || t!=b || t!=c->crc) {if (bad++<5) fprintf(stderr,"chunk %u: decoded=%d table=%08x bitwise=%08x meta=%08x\n",i,ok,(unsigned)t,(unsigned)b,(unsigned)c->crc);}
    }
    if (bad) {fprintf(stderr,"FAIL: %u/%u chunks\n",bad,(unsigned)SCENE_CHUNK_COUNT);return 1;}
    printf("PASS: %u pack chunks decode; table CRC == bitwise CRC == pack CRC\n",(unsigned)SCENE_CHUNK_COUNT);
    return 0;
}
