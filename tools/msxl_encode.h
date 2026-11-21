#ifndef MSXL_ENCODE_H
#define MSXL_ENCODE_H
/* Encoder of MSXL (src/msx_pcm_codec.h has the format and the decoder),
 * shared by tools/msxl.c and tools/test_msxl.c. Deterministic; every block
 * is decoded with the console's decoder before it is accepted. */
#include "../src/msx_pcm_codec.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct { uint8_t *data;size_t size,cap;uint64_t acc;int n; } msxl_writer_t;
static void msxl_byte(msxl_writer_t *w,uint8_t v) {
    if (w->size==w->cap) {w->cap=w->cap?w->cap*2:4096;w->data=realloc(w->data,w->cap);if (!w->data) abort();}
    w->data[w->size++]=v;
}
static void msxl_put(msxl_writer_t *w,unsigned bits,uint32_t value) {   /* 0..32 bits, MSB first */
    for (int i=(int)bits-1;i>=0;i--) {
        w->acc=w->acc<<1|((value>>i)&1u);
        if (++w->n==8) {msxl_byte(w,(uint8_t)w->acc);w->acc=0;w->n=0;}
    }
}
static void msxl_flush(msxl_writer_t *w) {while (w->n) msxl_put(w,1,0);}
static void msxl_put32(msxl_writer_t *w,uint32_t v) {for (int i=24;i>=0;i-=8) msxl_byte(w,(uint8_t)(v>>i));}
static uint32_t msxl_crc32(const uint8_t *p,size_t n) {
    uint32_t c=0xffffffffu;
    for (size_t i=0;i<n;i++) {c^=p[i];for (int k=0;k<8;k++) c=c>>1^(0xedb88320u&-(c&1u));}
    return ~c;
}
typedef struct { uint64_t bits;int type,order,p;uint8_t k[8]; } msxl_plan_t;
static inline uint32_t msxl_zigzag(int32_t r) {return (uint32_t)r<<1^(uint32_t)(r>>31);}
/* The residuals r[order..n) of the predictor of `plan`, with the decoder's
 * arithmetic, and r[1..order): the differences of the first samples. */
static void msxl_residual(const int32_t *x,int n,const msxl_plan_t *plan,int32_t *r) {
    for (int i=1;i<plan->order;i++) r[i]=x[i]-x[i-1];
    for (int i=plan->order;i<n;i++) {
        int32_t pred;
        switch (plan->type) {
        case 0:pred=0;break;
        case 1:pred=x[i-1];break;
        case 2:pred=2*x[i-1]-x[i-2];break;
        case 3:pred=3*x[i-1]-3*x[i-2]+x[i-3];break;
        default:pred=4*x[i-1]-6*x[i-2]+4*x[i-3]-x[i-4];break;
        }
        r[i]=x[i]-pred;
    }
}
/* Best partition order and Rice parameters for the residuals; fills
 * plan->p, plan->k and returns the bits (2 for p, 5 per partition, the codes,
 * the first sample). */
static uint64_t msxl_rice_plan(const int32_t *r,int n,msxl_plan_t *plan) {
    int order=plan->order,pmax=0;
    while (pmax<3 && !(n&((2<<pmax)-1)) && (n>>(pmax+1))>=order && (n>>(pmax+1))>0) pmax++;
    int parts=1<<pmax,size=n>>pmax;
    static uint64_t sum[8][MSXL_K_MAX+1];static uint32_t most[8];int count[8];
    for (int q=0;q<parts;q++) {
        int from=q?q*size:order?1:0,to=(q+1)*size;
        count[q]=to-from;most[q]=0;
        for (int k=0;k<=MSXL_K_MAX;k++) sum[q][k]=0;
        for (int i=from;i<to;i++) {
            uint32_t u=msxl_zigzag(r[i]);
            if (u>most[q]) most[q]=u;
            for (int k=0;k<=MSXL_K_MAX;k++) sum[q][k]+=u>>k;
        }
    }
    uint64_t best=UINT64_MAX;
    for (int p=0;p<=pmax;p++) {
        int group=1<<(pmax-p);uint64_t total=order?20:2;uint8_t ks[8];
        for (int q=0;q<1<<p;q++) {
            uint64_t least=UINT64_MAX;int kbest=-1;
            for (int k=0;k<=MSXL_K_MAX;k++) {
                uint64_t bits=0;int samples=0;bool fits=true;
                for (int g=q*group;g<(q+1)*group;g++) {bits+=sum[g][k];samples+=count[g];if ((most[g]>>k)>=(uint32_t)MSXL_Q_MAX) fits=false;}
                bits+=(uint64_t)samples*(unsigned)(k+1);
                if (fits && bits<least) {least=bits;kbest=k;}
            }
            if (kbest<0) {total=UINT64_MAX;break;}
            total+=5+least;ks[q]=(uint8_t)kbest;
        }
        if (total<best) {best=total;plan->p=p;memcpy(plan->k,ks,sizeof(ks));}
    }
    return best;
}
/* The cheapest way to store x[0..n): the raw stream (when allowed) or one of
 * the five fixed predictors, each with its best partitions and parameters. */
static msxl_plan_t msxl_best_plan(const int32_t *x,int n,bool raw_allowed,int32_t *r) {
    msxl_plan_t best={.bits=UINT64_MAX};
    if (raw_allowed) {best.type=7;best.bits=3+16u*(unsigned)n;}
    for (int order=0;order<=4 && order<=n;order++) {
        msxl_plan_t plan={.type=order,.order=order};
        msxl_residual(x,n,&plan,r);
        uint64_t rice=msxl_rice_plan(r,n,&plan);
        if (rice==UINT64_MAX) continue;
        plan.bits=3+rice;
        if (plan.bits<best.bits) best=plan;
    }
    return best;
}
static void msxl_emit(msxl_writer_t *w,const int32_t *x,int n,const msxl_plan_t *plan,int32_t *r) {
    msxl_put(w,3,(uint32_t)plan->type);
    if (plan->type==7) {for (int i=0;i<n;i++) msxl_put(w,16,(uint32_t)x[i]&0xffffu);return;}
    msxl_put(w,2,(uint32_t)plan->p);
    msxl_residual(x,n,plan,r);
    int size=n>>plan->p,i=plan->order?1:0;
    for (int q=0;q<1<<plan->p;q++) {
        unsigned k=plan->k[q];
        msxl_put(w,5,k);
        if (!q && plan->order) msxl_put(w,18,(uint32_t)x[0]&0x3ffffu);
        for (;i<(q+1)*size;i++) {
            uint32_t u=msxl_zigzag(r[i]);
            for (uint32_t z=u>>k;z;z--) msxl_put(w,1,0);
            msxl_put(w,1,1);
            if (k) msxl_put(w,k,u&((1u<<k)-1));
        }
    }
}
/* One block of n samples per channel (interleaved, native int16) appended
 * to w, padded to an even size. Returns the size. */
static size_t msxl_encode_block(msxl_writer_t *w,const int16_t *pcm,unsigned channels,int n) {
    static int32_t s[4][MSXL_BLOCK_MAX],r[MSXL_BLOCK_MAX];
    size_t start=w->size;
    if (channels==1) {
        for (int i=0;i<n;i++) s[0][i]=pcm[i];
        msxl_plan_t plan=msxl_best_plan(s[0],n,true,r);
        msxl_put(w,1,0);msxl_emit(w,s[0],n,&plan,r);
    } else {
        /* 0 left, 1 right, 2 middle, 3 side */
        for (int i=0;i<n;i++) {int32_t l=pcm[2*i],rr=pcm[2*i+1];s[0][i]=l;s[1][i]=rr;s[2][i]=(l+rr)>>1;s[3][i]=l-rr;}
        msxl_plan_t plan[4];
        for (int c=0;c<4;c++) plan[c]=msxl_best_plan(s[c],n,c<2,r);
        int mode=plan[2].bits+plan[3].bits<plan[0].bits+plan[1].bits;
        msxl_put(w,1,(uint32_t)mode);
        msxl_emit(w,s[2*mode],n,&plan[2*mode],r);
        msxl_emit(w,s[2*mode+1],n,&plan[2*mode+1],r);
    }
    msxl_flush(w);
    if ((w->size-start)&1u) msxl_byte(w,0);
    return w->size-start;
}
/* Whether `file` is a WAV64 this format stores: version 2, raw, 16 bits,
 * 1 or 2 channels, the samples from byte 24 to the end of the file. */
static bool msxl_eligible(const uint8_t *file,size_t size) {
    if (size<=MSXL_WAV_HEADER || memcmp(file,"WV64",4) || file[4]!=2 || file[5]!=0 || (file[6]!=1 && file[6]!=2) || file[7]!=16) return false;
    uint32_t len=msxl_be32(file+12),loop=msxl_be32(file+16);
    return msxl_be32(file+20)==MSXL_WAV_HEADER && len && len<=0x1fffffffu && loop<=len && size==MSXL_WAV_HEADER+(size_t)len*file[6]*2u;
}
/* The MSXL file of a WAV64 (malloc'ed; NULL when it is not eligible).
 * Aborts if any block does not decode to its samples. */
static uint8_t *msxl_encode(const uint8_t *file,size_t size,unsigned block_log2,size_t *out_size) {
    if (!msxl_eligible(file,size) || block_log2<3 || block_log2>MSXL_BLOCK_LOG2_MAX) return NULL;
    unsigned channels=file[6];uint32_t len=msxl_be32(file+12),block=1u<<block_log2,blocks=(len+block-1)>>block_log2;
    msxl_writer_t w={0};
    for (int i=0;i<MSXL_HEADER;i++) msxl_byte(&w,0);
    uint32_t *index=malloc((blocks+1)*sizeof(*index));
    static int16_t pcm[MSXL_BLOCK_MAX*2],back[MSXL_BLOCK_MAX*2];static int32_t scratch[MSXL_BLOCK_MAX];
    uint32_t first=0;size_t size_at=0;
    for (uint32_t k=0;k<blocks;k++) {
        uint32_t n=len-k*block<block?len-k*block:block;
        const uint8_t *src=file+MSXL_WAV_HEADER+(size_t)k*block*channels*2u;
        for (uint32_t i=0;i<n*channels;i++) pcm[i]=(int16_t)msxl_be16(src+2*i);
        index[k]=(uint32_t)w.size;
        size_t stored=msxl_encode_block(&w,pcm,channels,(int)n);
        if (stored>msxl_block_bound(channels,n) || !msxl_decode_block(w.data+index[k],stored,channels,(int)n,back,scratch)
            || memcmp(pcm,back,n*channels*sizeof(int16_t))) {fprintf(stderr,"msxl: block %u does not decode to its samples\n",(unsigned)k);abort();}
        if (k) {w.data[size_at]=(uint8_t)(stored>>8);w.data[size_at+1]=(uint8_t)stored;} else first=(uint32_t)stored;
        size_at=w.size;msxl_byte(&w,0);msxl_byte(&w,0);
    }
    uint32_t index_offset=(uint32_t)w.size;index[blocks]=index_offset;
    for (uint32_t k=0;k<=blocks;k++) msxl_put32(&w,index[k]);
    free(index);
    uint8_t *h=w.data;
    memcpy(h,"MSXL",4);h[4]=MSXL_VERSION;h[5]=(uint8_t)channels;h[6]=(uint8_t)block_log2;h[7]=0;memcpy(h+8,file,MSXL_WAV_HEADER);
    uint32_t fields[6]={blocks,index_offset,0,first,msxl_crc32(file,size),0};
    for (int i=0;i<6;i++) for (int b=0;b<4;b++) h[32+4*i+b]=(uint8_t)(fields[i]>>(24-8*b));
    *out_size=w.size;return w.data;
}
/* The WAV64 of an MSXL file, block by block as the stream reader does
 * (malloc'ed; NULL when the file is malformed or its CRC is not the source's). */
static uint8_t *msxl_decode(const uint8_t *file,size_t size,size_t *out_size) {
    msxl_info_t info;
    if (size<MSXL_HEADER || !msxl_parse(file,&info) || (size_t)info.index_offset+(info.blocks+1)*4u!=size) return NULL;
    uint8_t *out=malloc(info.source_size);memcpy(out,info.wav,MSXL_WAV_HEADER);
    static int16_t pcm[MSXL_BLOCK_MAX*2];static int32_t scratch[MSXL_BLOCK_MAX];
    for (uint32_t k=0;k<info.blocks;k++) {
        uint32_t at=msxl_be32(file+info.index_offset+4*k),end=msxl_be32(file+info.index_offset+4*k+4),n=msxl_block_samples(&info,k);
        if (at<MSXL_HEADER || end>info.index_offset || end<at+4 || !msxl_decode_block(file+at,end-at-2,info.channels,(int)n,pcm,scratch)) {free(out);return NULL;}
        uint8_t *dst=out+MSXL_WAV_HEADER+((size_t)k<<info.block_log2)*info.channels*2u;
        for (uint32_t i=0;i<n*info.channels;i++) {dst[2*i]=(uint8_t)((uint16_t)pcm[i]>>8);dst[2*i+1]=(uint8_t)pcm[i];}
    }
    if (msxl_crc32(out,info.source_size)!=info.crc) {free(out);return NULL;}
    *out_size=info.source_size;return out;
}
#endif
