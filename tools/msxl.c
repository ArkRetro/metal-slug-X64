/* MSXL packer (docs/COMPRESION.md): lossless storage of the raw 16-bit
 * WAV64 files of the cartridge. Format and decoder: src/msx_pcm_codec.h.
 *   msxl pack in.wav64 out      writes out when the file is a raw 16-bit
 *                               WAV64 and its MSXL file is smaller; else
 *                               writes nothing (exit 0) and says so
 *   msxl unpack in out.wav64    the source WAV64, byte by byte
 *   msxl size files...          sizes only (measurements) */
#include "msxl_encode.h"
static uint8_t *load(const char *path,size_t *size) {
    FILE *f=fopen(path,"rb");if (!f) {perror(path);exit(1);}
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *d=malloc(n>0?(size_t)n:1);
    if (n<0 || fread(d,1,(size_t)n,f)!=(size_t)n) {perror(path);exit(1);}
    fclose(f);*size=(size_t)n;return d;
}
static void save(const char *path,const uint8_t *d,size_t size) {
    FILE *f=fopen(path,"wb");
    if (!f || fwrite(d,1,size,f)!=size || fclose(f)) {perror(path);exit(1);}
}
int main(int argc,char **argv) {
    unsigned log2=MSXL_BLOCK_LOG2_MAX;
    if (argc>2 && !strcmp(argv[1],"--block-log2")) {log2=(unsigned)atoi(argv[2]);argv+=2;argc-=2;}
    if (argc>=3 && !strcmp(argv[1],"size")) {
        size_t all=0,packed=0;
        for (int i=2;i<argc;i++) {
            size_t size,out;uint8_t *d=load(argv[i],&size),*m=msxl_encode(d,size,log2,&out);
            if (m) {printf("%10zu %10zu %s\n",size,out,argv[i]);all+=size;packed+=out<size?out:size;}
            free(m);free(d);
        }
        printf("%10zu %10zu total\n",all,packed);return 0;
    }
    if (argc!=4) {fprintf(stderr,"usage: msxl pack in.wav64 out | unpack in out.wav64 | size files...\n");return 2;}
    size_t size,out=0;uint8_t *d=load(argv[2],&size);
    if (!strcmp(argv[1],"pack")) {
        uint8_t *m=msxl_encode(d,size,log2,&out);
        if (!m) {printf("%s: not a raw 16-bit WAV64, kept\n",argv[2]);return 0;}
        size_t back;uint8_t *w=msxl_decode(m,out,&back);
        if (!w || back!=size || memcmp(w,d,size)) {fprintf(stderr,"%s: the MSXL file does not decode to the source\n",argv[2]);return 1;}
        if (out>=size) {printf("%s: %zu -> %zu, kept raw\n",argv[2],size,out);return 0;}
        save(argv[3],m,out);printf("%s: %zu -> %zu\n",argv[2],size,out);return 0;
    }
    if (!strcmp(argv[1],"unpack")) {
        uint8_t *w=msxl_decode(d,size,&out);
        if (!w) {fprintf(stderr,"%s: not a well-formed MSXL file\n",argv[2]);return 1;}
        save(argv[3],w,out);return 0;
    }
    return 2;
}
