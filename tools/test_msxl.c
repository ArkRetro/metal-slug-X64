/* make check: MSXL (lossless 16-bit PCM, src/msx_pcm_codec.h) gives back
 * every sample of its source (docs/COMPRESION.md).
 *   test_msxl work-file [sources.wav64 ...] [--packed file.msxl source.wav64 ...]
 * 1. Synthetic signals (always): silence, extremes, noise, tones, short and
 *    odd lengths, mono and stereo.
 * 2. Every source given (the private raw WAV64s of the cartridge): packed
 *    here and compared whole.
 * 3. --packed: the MSXL files the cartridge carries against their sources.
 * Each file is decoded twice: block by block through the index
 * (msxl_decode) and by the console's stream reader, src/msx_pcm.c, compiled
 * for the host, with the reads the mixer makes (in order with every length
 * and blocks decoded ahead, then seeks, then after a release). Damaged blocks must be refused or
 * decoded without touching memory outside the buffers. */
#include "msxl_encode.h"
#include "../src/msx_pcm.h"
#include <assert.h>
#include <math.h>
uint32_t host_ticks;
struct samplebuffer_s { uint8_t *data;int samples,frame; };
void *samplebuffer_append(samplebuffer_t *s,int wlen) {
    s->data=realloc(s->data,(size_t)wlen*(unsigned)s->frame+1);s->samples=wlen;return s->data;
}
static uint32_t seed=0x1234567u;
static uint32_t rnd(void) {seed=seed*1664525u+1013904223u;return seed>>8;}
static unsigned files,blocks_total;static size_t raw_total,packed_total;
static void fail(const char *name,const char *what) {fprintf(stderr,"FAIL: %s: %s\n",name,what);exit(1);}
/* What the stream reader hands to the mixer for [wpos,wpos+wlen) against the source. */
static void expect(const char *name,msx_pcm_t *p,samplebuffer_t *sb,const uint8_t *wav,int wpos,int wlen) {
    p->wave.read(p->wave.ctx,sb,wpos,wlen,true);
    unsigned ch=p->info.channels;
    const int16_t *got=(const int16_t *)(void *)sb->data;
    for (int i=0;i<wlen*(int)ch;i++) {
        const uint8_t *s=wav+MSXL_WAV_HEADER+((size_t)wpos*ch+(unsigned)i)*2u;
        if (got[i]!=(int16_t)(s[0]<<8|s[1])) fail(name,"the stream reader gives other samples");
    }
}
static void stream(const char *name,const char *work,const uint8_t *msxl,size_t msxl_size,const uint8_t *wav) {
    FILE *f=fopen(work,"wb");
    if (!f || fwrite(msxl,1,msxl_size,f)!=msxl_size || fclose(f)) fail(work,"cannot write the work file");
    msx_pcm_t p;
    if (!msx_pcm_open(&p,work)) fail(name,"msx_pcm_open refuses the file");
    samplebuffer_t sb={NULL,0,(int)(p.info.channels*2u)};
    int len=p.wave.len;
    if (len!=(int)msxl_be32(wav+12) || p.wave.loop_len!=(int)msxl_be32(wav+16) || p.wave.channels!=wav[6] || p.wave.bits!=16
        || p.wave.frequency!=(float)msxl_be32(wav+8)) fail(name,"the waveform is not the WAV64's");
    if (rnd()&1u) msx_pcm_prime(&p);   /* msx_audio_play: the first blocks before the stream plays */
    for (int pos=0;pos<len;) {   /* in order, lengths 1..3000 */
        int n=1+(int)(rnd()%3000u);
        if (n>len-pos) n=len-pos;
        expect(name,&p,&sb,wav,pos,n);pos+=n;
        if (rnd()&1u) msx_pcm_prefetch(&p,host_ticks+1000000u);   /* the next block ahead, some of the times */
    }
    if (len>16*MSXL_BLOCK_MAX && !p.ahead) fail(name,"no block was decoded ahead");
    for (int k=0;k<300;k++) {    /* seeks */
        int pos=(int)(rnd()%(unsigned)len),n=1+(int)(rnd()%2500u);
        if (n>len-pos) n=len-pos;
        expect(name,&p,&sb,wav,pos,n);
        if (k%50==49) msx_pcm_release(&p);
    }
    msx_pcm_release(&p);msx_pcm_prime(&p);expect(name,&p,&sb,wav,0,len<5000?len:5000);   /* a replay, primed */
    if (p.errors) fail(name,"the stream reader met a damaged block");
    msx_pcm_close(&p);free(sb.data);remove(work);
}
static void whole(const char *name,const char *work,const uint8_t *msxl,size_t msxl_size,const uint8_t *wav,size_t wav_size) {
    size_t back=0;uint8_t *w=msxl_decode(msxl,msxl_size,&back);
    if (!w || back!=wav_size || memcmp(w,wav,wav_size)) fail(name,"the MSXL file does not decode to its source");
    free(w);
    stream(name,work,msxl,msxl_size,wav);
    files++;raw_total+=wav_size;packed_total+=msxl_size;blocks_total+=msxl_be32(msxl+32);
}
static void roundtrip(const char *name,const char *work,const uint8_t *wav,size_t size,unsigned log2) {
    size_t out=0;uint8_t *m=msxl_encode(wav,size,log2,&out);
    if (!m) fail(name,"not a raw 16-bit WAV64");
    whole(name,work,m,out,wav,size);free(m);
}
static uint8_t *wav64(unsigned channels,uint32_t len,uint32_t loop,size_t *size) {
    *size=MSXL_WAV_HEADER+(size_t)len*channels*2u;
    uint8_t *w=calloc(*size,1);memcpy(w,"WV64",4);w[4]=2;w[5]=0;w[6]=(uint8_t)channels;w[7]=16;
    uint32_t f[4]={32000,len,loop,MSXL_WAV_HEADER};
    for (int i=0;i<4;i++) for (int b=0;b<4;b++) w[8+4*i+b]=(uint8_t)(f[i]>>(24-8*b));
    return w;
}
static void set(uint8_t *w,size_t i,int v) {
    if (v>32767) v=32767;
    if (v<-32768) v=-32768;
    w[MSXL_WAV_HEADER+2*i]=(uint8_t)((unsigned)v>>8);w[MSXL_WAV_HEADER+2*i+1]=(uint8_t)v;
}
static void synthetic(const char *work) {
    static const uint32_t lens[]={1,2,3,5,7,8,9,33,1023,1024,1025,2048,3000,5121};
    for (unsigned channels=1;channels<=2;channels++) for (unsigned kind=0;kind<9;kind++) for (unsigned l=0;l<sizeof(lens)/sizeof(lens[0]);l++) {
        uint32_t len=lens[l];size_t size;uint8_t *w=wav64(channels,len,kind==3?len/2:0,&size);
        for (uint32_t i=0;i<len;i++) for (unsigned c=0;c<channels;c++) {
            int v=0;double t=i/32000.0;
            switch (kind) {
            case 0:v=0;break;                                              /* silence */
            case 1:v=(i+c)&1u?32767:-32768;break;                          /* extremes, every sample */
            case 2:v=(int)(rnd()&0xffffu)-32768;break;                     /* noise: raw streams */
            case 3:v=(int)(12000*sin(6.2831853*440*t)+6000*sin(6.2831853*1870*t+c));break;
            case 4:v=(int)(20000*sin(6.2831853*97*t))*(c?-1:1);break;      /* right = -left */
            case 5:v=(int)(30000*sin(6.2831853*3000*t)+(int)(rnd()%64u)-32);break;
            case 6:v=(i/37u)&1u?(c?-32768:32767):(c?32767:-32768);break;   /* opposite extremes: the widest side */
            case 7:v=(int)(i*13u%1500u)-750+(c?(int)(rnd()%7u):0);break;   /* nearly the same channels */
            default:v=(int)(rnd()%3u)-1;break;                             /* a few values */
            }
            set(w,(size_t)i*channels+c,v);
        }
        char name[64];snprintf(name,sizeof(name),"synthetic %u channels, kind %u, %u samples",channels,kind,(unsigned)len);
        roundtrip(name,work,w,size,MSXL_BLOCK_LOG2_MAX);
        if (kind==3 || kind==5) roundtrip(name,work,w,size,7);
        free(w);
    }
}
/* Damaged blocks: refused, or decoded inside the buffers. */
static void damaged(void) {
    size_t size;uint8_t *w=wav64(2,4096,0,&size);
    for (uint32_t i=0;i<8192;i++) set(w,i,(int)(9000*sin(i*0.05)+(int)(rnd()%512u)));
    size_t out;uint8_t *m=msxl_encode(w,size,MSXL_BLOCK_LOG2_MAX,&out);assert(m);
    enum { GUARD=64 };
    static int16_t pcm[MSXL_BLOCK_MAX*2+2*GUARD];static int32_t scratch[MSXL_BLOCK_MAX+2*GUARD];
    unsigned refused=0,trials=0;
    for (uint32_t k=0;k<4;k++) {
        uint32_t at=msxl_be32(m+msxl_be32(m+36)+4*k),stored=msxl_be32(m+msxl_be32(m+36)+4*k+4)-at-2;
        for (unsigned t=0;t<1500;t++,trials++) {
            uint8_t *copy=malloc(stored);memcpy(copy,m+at,stored);size_t length=stored;
            if (t%3==0) length=rnd()%stored;
            else for (unsigned flips=1+rnd()%4u;flips;flips--) copy[rnd()%(t%3==1?8u:stored)]^=(uint8_t)(1u<<(rnd()%8u));
            for (int i=0;i<GUARD;i++) {pcm[i]=pcm[MSXL_BLOCK_MAX*2+GUARD+i]=0x5a5a;scratch[i]=scratch[MSXL_BLOCK_MAX+GUARD+i]=0x5a5a5a5a;}
            if (!msxl_decode_block(copy,length,2,1024,pcm+GUARD,scratch+GUARD)) refused++;
            for (int i=0;i<GUARD;i++) if (pcm[i]!=0x5a5a || pcm[MSXL_BLOCK_MAX*2+GUARD+i]!=0x5a5a || scratch[i]!=0x5a5a5a5a || scratch[MSXL_BLOCK_MAX+GUARD+i]!=0x5a5a5a5a)
                fail("damaged block","the decoder wrote outside its buffers");
            free(copy);
        }
    }
    if (!refused) fail("damaged block","no damaged block was refused");
    /* A damaged header or a block count that does not fit is no MSXL file. */
    msxl_info_t info;uint8_t h[MSXL_HEADER];
    memcpy(h,m,MSXL_HEADER);h[4]=2;if (msxl_parse(h,&info)) fail("header","version 2 accepted");
    memcpy(h,m,MSXL_HEADER);h[35]++;if (msxl_parse(h,&info)) fail("header","a wrong block count accepted");
    memcpy(h,m,MSXL_HEADER);h[8+5]=1;if (msxl_parse(h,&info)) fail("header","a VADPCM WAV64 accepted");
    free(m);free(w);
    printf("damaged blocks: %u of %u refused, none wrote outside its buffers\n",refused,trials);
}
static uint8_t *load(const char *path,size_t *size) {
    FILE *f=fopen(path,"rb");if (!f) fail(path,"cannot open");
    fseek(f,0,SEEK_END);long n=ftell(f);fseek(f,0,SEEK_SET);
    uint8_t *d=malloc(n>0?(size_t)n:1);
    if (n<0 || fread(d,1,(size_t)n,f)!=(size_t)n) fail(path,"cannot read");
    fclose(f);*size=(size_t)n;return d;
}
int main(int argc,char **argv) {
    if (argc<2) {fprintf(stderr,"usage: test_msxl work-file [sources.wav64 ...] [--packed file source ...]\n");return 2;}
    const char *work=argv[1];
    synthetic(work);
    printf("synthetic: %u files, %u blocks, %zu -> %zu bytes, every sample back\n",files,blocks_total,raw_total,packed_total);
    damaged();
    int i=2;unsigned sources=0,skipped=0;files=0;blocks_total=0;raw_total=packed_total=0;
    for (;i<argc && strcmp(argv[i],"--packed");i++) {
        size_t size;uint8_t *d=load(argv[i],&size);
        if (msxl_eligible(d,size)) {roundtrip(argv[i],work,d,size,MSXL_BLOCK_LOG2_MAX);sources++;} else skipped++;
        free(d);
    }
    if (sources || skipped) printf("sources: %u raw 16-bit WAV64 (%u other files left as they are), %u blocks, %zu -> %zu bytes, every sample back\n",sources,skipped,blocks_total,raw_total,packed_total);
    else printf("sources: none (no private raw WAV64 here)\n");
    files=0;raw_total=packed_total=0;
    for (i++;i+1<argc;i+=2) {
        size_t msize,size;uint8_t *m=load(argv[i],&msize),*d=load(argv[i+1],&size);
        if (msize<4 || memcmp(m,"MSXL",4)) {
            /* Not packed: it must then be the source itself. */
            if (msize!=size || memcmp(m,d,size)) fail(argv[i],"neither MSXL nor its source");
        } else whole(argv[i],work,m,msize,d,size);
        free(m);free(d);
    }
    if (files) printf("cartridge: %u MSXL files, %zu -> %zu bytes, each one decodes to its source\n",files,raw_total,packed_total);
    printf("PASS: MSXL\n");return 0;
}
