/* Host check of src/neo_palmix_run.h (tools/test_palmix_run.py writes the vectors: the ROM table 0051D8.., then per
 * case the ROM words of +A and +C, the weight and the colours tools/palette_mix.py gives, as RGBA 5551). */
#include <stdio.h>
#include <string.h>
#include "../src/neo_palmix_run.h"
static uint16_t be(FILE *f) {int a=fgetc(f),b=fgetc(f);return (uint16_t)(a<<8|b);}
int main(int argc,char **argv) {
    if (argc!=2) return 2;
    FILE *f=fopen(argv[1],"rb");if (!f) return 2;
    static uint8_t table[0x800];
    if (fread(table,1,sizeof(table),f)!=sizeof(table)) return 2;
    unsigned n=be(f)<<16;n|=be(f);unsigned bad=0;
    for (unsigned i=0;i<n;i++) {
        uint16_t a[16],c[16],want[16],got[16];
        for (unsigned k=0;k<16;k++) a[k]=be(f);
        for (unsigned k=0;k<16;k++) c[k]=be(f);
        unsigned w=be(f);
        for (unsigned k=0;k<16;k++) want[k]=be(f);
        neo_palmix_mix(a,c,w,table,got);
        if (memcmp(want,got,sizeof(got))) {if (!bad++) fprintf(stderr,"case %u (weight %u) differs\n",i,w);}
    }
    fclose(f);
    printf("%u cases, %u different\n",n,bad);
    return bad!=0;
}
