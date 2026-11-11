#ifndef MSX_PCM_CODEC_H
#define MSX_PCM_CODEC_H
/* MSXL: lossless 16-bit PCM (docs/COMPRESION.md, "Audio PCM sin pérdida").
 * The decoder of the console and of the host tools and tests is this one
 * source. A raw WAV64 (libdragon format 0, 16 bits, 1 or 2 channels) is cut
 * in blocks of 1<<block_log2 samples; every block is decoded on its own
 * (no state crosses a block) and gives back the samples it was made from,
 * bit by bit: tools/msxl.c refuses to write a file whose decoding differs.
 *
 * File (big endian):
 *   0  'MSXL'
 *   4  u8 version (1), u8 channels, u8 block_log2, u8 0
 *   8  the 24-byte WAV64 header of the source, as it was
 *   32 u32 blocks
 *   36 u32 index offset = end of the last block
 *   40 u32 0
 *   44 u32 size of block 0
 *   48 u32 CRC-32 of the source file
 *   52 u32 0
 *   56 blocks: the block's bits padded to an even size, then u16 = size of
 *      the next block (0 after the last)
 *   index: u32 file offset of every block, then the index offset again
 *
 * Block bits (MSB first): 1 bit stereo mode, stream a, stream b (stereo).
 *   mode 0: a = left, b = right
 *   mode 1: a = (left + right) >> 1, b = left - right
 * Stream: 3 bits type.
 *   7: n samples of 16 bits (mode 0 only).
 *   0..4: fixed predictor of that order (the differences of Shorten and
 *      FLAC: 0, x1, 2x1-x2, 3x1-3x2+x3, 4x1-6x2+4x3-x4; no multiplier).
 *   then 2 bits p (1<<p partitions of n>>p samples) and per partition 5
 *   bits k and its values in Rice code: q zero bits, a one, k bits; value =
 *   zigzag((q << k) | bits). The values are the residuals of the predictor;
 *   before them, in the first partition, go the first `order` samples: the
 *   first one in 18 bits, each of the others as its difference from the one
 *   before, in Rice code. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
/* Where the console puts this code (src/msx_pcm.c); nothing on the host. */
#ifndef MSXL_CODE
#define MSXL_CODE
#endif
enum { MSXL_VERSION=1,MSXL_HEADER=56,MSXL_WAV_HEADER=24,MSXL_BLOCK_LOG2_MAX=10,MSXL_BLOCK_MAX=1<<MSXL_BLOCK_LOG2_MAX,
    MSXL_K_MAX=24,MSXL_Q_MAX=1<<16,
    /* A block never takes more than its samples stored raw plus this. */
    MSXL_BLOCK_SLACK=8 };
typedef struct {
    uint8_t channels,block_log2;
    uint8_t wav[MSXL_WAV_HEADER];
    uint32_t frequency,len,loop_len;   /* from the WAV64 header */
    uint32_t blocks,index_offset,first_size,crc,source_size;
} msxl_info_t;
MSXL_CODE static inline uint32_t msxl_be32(const uint8_t *p) {return (uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];}
MSXL_CODE static inline uint32_t msxl_be16(const uint8_t *p) {return (uint32_t)p[0]<<8|p[1];}
/* Largest stored size of a block of n samples. */
MSXL_CODE static inline uint32_t msxl_block_bound(unsigned channels,unsigned n) {return n*channels*2u+MSXL_BLOCK_SLACK;}
/* Samples of block `block`. */
MSXL_CODE static inline unsigned msxl_block_samples(const msxl_info_t *info,uint32_t block) {
    uint32_t size=1u<<info->block_log2,start=block<<info->block_log2;
    return info->len-start<size?info->len-start:size;
}
/* The 56-byte header. False unless it is a well-formed MSXL header. */
MSXL_CODE static inline bool msxl_parse(const uint8_t *h,msxl_info_t *info) {
    if (memcmp(h,"MSXL",4) || h[4]!=MSXL_VERSION || (h[5]!=1 && h[5]!=2) || h[6]<3 || h[6]>MSXL_BLOCK_LOG2_MAX || h[7]) return false;
    info->channels=h[5];info->block_log2=h[6];memcpy(info->wav,h+8,MSXL_WAV_HEADER);
    const uint8_t *w=info->wav;
    /* WAV64 version 2, raw, the same channels, 16 bits, samples at byte 24. */
    if (memcmp(w,"WV64",4) || w[4]!=2 || w[5]!=0 || w[6]!=info->channels || w[7]!=16 || msxl_be32(w+20)!=MSXL_WAV_HEADER) return false;
    info->frequency=msxl_be32(w+8);info->len=msxl_be32(w+12);info->loop_len=msxl_be32(w+16);
    info->blocks=msxl_be32(h+32);info->index_offset=msxl_be32(h+36);
    info->first_size=msxl_be32(h+44);info->crc=msxl_be32(h+48);
    if (!info->len || info->len>0x1fffffffu || info->loop_len>info->len || msxl_be32(h+40) || msxl_be32(h+52)) return false;
    if (info->blocks!=(info->len+(1u<<info->block_log2)-1)>>info->block_log2) return false;
    info->source_size=MSXL_WAV_HEADER+info->len*info->channels*2u;
    if (info->index_offset<MSXL_HEADER+info->blocks*4u || (info->index_offset&1u) || (info->first_size&1u)) return false;
    if (info->first_size<2 || info->first_size>msxl_block_bound(info->channels,1u<<info->block_log2)) return false;
    return true;
}
typedef struct { const uint8_t *p,*end;uint64_t acc;int n;unsigned pad; } msxl_bits_t;
/* 32 bits at any address (one unaligned load on the console; a memcpy
 * would be a call under -Os). */
typedef struct __attribute__((packed)) { uint32_t v; } msxl_word_t;
MSXL_CODE static inline uint32_t msxl_load32(const uint8_t *p) {
    uint32_t v=((const msxl_word_t *)(const void *)p)->v;
#if __BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__
    v=__builtin_bswap32(v);
#endif
    return v;
}
/* More than 32 bits in acc afterwards (zeros past the end, counted in pad). */
#define MSXL_REFILL(p,end,acc,n,pad) do { \
    if ((end)-(p)>=4) {(acc)|=(uint64_t)msxl_load32(p)<<(32-(n));(p)+=4;(n)+=32;} \
    else while ((n)<=56) {uint64_t byte_=0;if ((p)<(end)) byte_=*(p)++;else (pad)++;(acc)|=byte_<<(56-(n));(n)+=8;} \
} while (0)
/* 1..32 bits. Not inlined: the headers of the streams and the rare long
 * codes; the Rice loop below keeps its own state. */
MSXL_CODE static __attribute__((noinline)) uint32_t msxl_get(msxl_bits_t *b,unsigned bits) {
    if (b->n<=32) MSXL_REFILL(b->p,b->end,b->acc,b->n,b->pad);
    uint32_t v=(uint32_t)(b->acc>>(64-bits));b->acc<<=bits;b->n-=(int)bits;return v;
}
MSXL_CODE static inline int32_t msxl_signed(msxl_bits_t *b,unsigned bits) {
    uint32_t v=msxl_get(b,bits),sign=1u<<(bits-1);
    return (int32_t)((v^sign)-sign);
}
/* `count` Rice codes of parameter k into v. False on a code longer than any
 * the packer writes. */
MSXL_CODE static inline bool msxl_rice(msxl_bits_t *b,unsigned k,int32_t *v,int count) {
    const uint8_t *p=b->p,*end=b->end;uint64_t acc=b->acc;int n=b->n;unsigned pad=b->pad;
    const uint32_t one=1u<<k;
    while (count-->0) {
        if (n<=32) MSXL_REFILL(p,end,acc,n,pad);
        uint32_t top=(uint32_t)(acc>>32),u;
        if (top>>k) {   /* the whole code is in the top 32 bits */
            unsigned z=0;
            while (!(top&0x80000000u)) {top<<=1;z++;}
            u=(z<<k)+(uint32_t)(((uint64_t)top<<(k+1))>>32)-one;
            acc<<=z+1+k;n-=(int)(z+1+k);
        } else {        /* a long code, bit by bit */
            uint32_t q=0;
            b->p=p;b->acc=acc;b->n=n;b->pad=pad;
            while (!msxl_get(b,1)) if (++q>MSXL_Q_MAX) return false;
            u=k?(q<<k)|msxl_get(b,k):q;
            p=b->p;acc=b->acc;n=b->n;pad=b->pad;
        }
        *v++=(int32_t)(u>>1)^-(int32_t)(u&1u);
    }
    b->p=p;b->acc=acc;b->n=n;b->pad=pad;return true;
}
/* One stream of n values into v. */
MSXL_CODE static inline bool msxl_stream(msxl_bits_t *b,int n,int32_t *v,bool raw_allowed) {
    unsigned type=msxl_get(b,3);
    if (type==7) {
        if (!raw_allowed) return false;
        for (int i=0;i<n;i++) v[i]=msxl_signed(b,16);
        return true;
    }
    if (type>4) return false;
    int order=(int)type;
    unsigned p=msxl_get(b,2);int size=n>>p,i=0;
    if ((size<<p)!=n || size<order) return false;
    for (unsigned part=0;part<1u<<p;part++) {
        unsigned k=msxl_get(b,5);int end=(int)(part+1)*size;
        if (k>MSXL_K_MAX) return false;
        if (!part && order) {v[0]=msxl_signed(b,18);i=1;}
        if (!msxl_rice(b,k,v+i,end-i)) return false;
        i=end;
    }
    /* The first samples from their differences, then the predictor. */
    for (i=1;i<order;i++) v[i]+=v[i-1];
    switch (order) {
    case 1:{int32_t a=v[0];for (i=1;i<n;i++) {a+=v[i];v[i]=a;}break;}
    case 2:{int32_t a=v[1],c=v[0];for (i=2;i<n;i++) {int32_t x=v[i]+2*a-c;c=a;a=x;v[i]=x;}break;}
    case 3:{int32_t a=v[2],c=v[1],d=v[0];for (i=3;i<n;i++) {int32_t x=v[i]+3*(a-c)+d;d=c;c=a;a=x;v[i]=x;}break;}
    case 4:{int32_t a=v[3],c=v[2],d=v[1],e=v[0];for (i=4;i<n;i++) {int32_t x=v[i]+4*(a+d)-6*c-e;e=d;d=c;c=a;a=x;v[i]=x;}break;}
    default:break;
    }
    return true;
}
/* Decode one block of n samples per channel into out (interleaved, native
 * int16). False when the block is malformed (out is then undefined but
 * nothing is read or written out of bounds). scratch: n int32. */
MSXL_CODE static inline bool msxl_decode_block(const uint8_t *in,size_t in_size,unsigned channels,int n,int16_t *out,int32_t *scratch) {
    if (n<=0 || n>MSXL_BLOCK_MAX || (channels!=1 && channels!=2)) return false;
    msxl_bits_t b={in,in+in_size,0,0,0};
    unsigned mode=msxl_get(&b,1);int32_t wrong=0;
    if (channels==1) {
        if (mode || !msxl_stream(&b,n,scratch,true)) return false;
        for (int i=0;i<n;i++) {int32_t s=scratch[i];out[i]=(int16_t)s;wrong|=s^(int16_t)s;}
    } else {
        /* Stream a fits 16 bits: left or the middle. */
        if (!msxl_stream(&b,n,scratch,!mode)) return false;
        for (int i=0;i<n;i++) {int32_t s=scratch[i];out[2*i]=(int16_t)s;wrong|=s^(int16_t)s;}
        if (!msxl_stream(&b,n,scratch,!mode)) return false;
        for (int i=0;i<n;i++) {
            int32_t l=out[2*i],r=scratch[i];
            if (mode) {int32_t mid=(int32_t)((uint32_t)l<<1)|(r&1);l=(mid+r)>>1;r=(mid-r)>>1;}
            out[2*i]=(int16_t)l;out[2*i+1]=(int16_t)r;wrong|=(l^(int16_t)l)|(r^(int16_t)r);
        }
    }
    /* No bit past the end was used, and every sample fits 16 bits. */
    return b.pad*8u<=(unsigned)b.n && !wrong;
}
#endif
