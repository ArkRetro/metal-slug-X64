#ifndef MSX_CINEMATIC_STREAM_H
#define MSX_CINEMATIC_STREAM_H
/* Resumable pieces of the cinematic player (src/cinematic.c): each one does
 * a bounded amount of work per call and keeps its position, so preparing a
 * movie never takes more than the frame time it is given (docs/RENDIMIENTO.md,
 * "Cinemáticas sin parones"). Host-tested by tools/test_cinematic_stream.c
 * against the one-shot forms of cinematic_codec.h. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "cinematic_codec.h"

/* CRC-32 (the msx_cine_crc32 one), four bytes per step (slicing by 4) on
 * aligned words of the big-endian CPU; same running form as
 * msx_cine_crc32_update (start from ~0u, complement at the end). */
typedef struct { uint32_t t[4][256]; } msx_cine_crc_tables_t;
static inline void msx_cine_crc_tables_init(msx_cine_crc_tables_t *k) {
    for (uint32_t i=0;i<256;i++) {
        uint32_t c=i;
        for (int b=0;b<8;b++) c=(c&1u)?(c>>1)^0xEDB88320u:c>>1;
        k->t[0][i]=c;
    }
    for (uint32_t i=0;i<256;i++) {
        k->t[1][i]=(k->t[0][i]>>8)^k->t[0][k->t[0][i]&255u];
        k->t[2][i]=(k->t[1][i]>>8)^k->t[0][k->t[1][i]&255u];
        k->t[3][i]=(k->t[2][i]>>8)^k->t[0][k->t[2][i]&255u];
    }
}
static inline uint32_t msx_cine_crc32_fast(const msx_cine_crc_tables_t *k,uint32_t crc,const uint8_t *p,size_t n) {
    const uint32_t *t0=k->t[0],*t1=k->t[1],*t2=k->t[2],*t3=k->t[3];
    while (n && ((uintptr_t)p&3u)) {crc=(crc>>8)^t0[(crc^*p++)&255u];n--;}
    for (;n>=4;p+=4,n-=4) {
        uint32_t v=(uint32_t)p[0]<<24|(uint32_t)p[1]<<16|(uint32_t)p[2]<<8|p[3];
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__==__ORDER_BIG_ENDIAN__
        v=*(const uint32_t *)(const void *)p;   // one aligned load (same value)
#endif
        crc=t3[(crc^(v>>24))&255u]^t2[((crc>>8)^(v>>16))&255u]^t1[((crc>>16)^(v>>8))&255u]^t0[(crc>>24)^(v&255u)];
    }
    while (n--) crc=(crc>>8)^t0[(crc^*p++)&255u];
    return crc;
}

/* LZ4 block decoder that stops after about `budget` output bytes and resumes
 * where it stopped. In place (the input at the end of the output's room, the
 * libdragon margin) works as with the one-shot decoder: the output never
 * passes the input still unread. Returns 1 when the block decoded to exactly
 * the output size, 0 when it stopped on the budget, -1 on a bad block. */
typedef struct {
    const uint8_t *in,*in_end;uint8_t *out,*base,*end;
    uint32_t lit,mlen,moff;uint8_t state,token;
} msx_lz4r_t;
static inline void msx_lz4r_init(msx_lz4r_t *z,const uint8_t *in,size_t in_size,uint8_t *out,size_t out_size) {
    *z=(msx_lz4r_t){.in=in,.in_end=in+in_size,.out=out,.base=out,.end=out+out_size};
}
static inline int msx_lz4r_run(msx_lz4r_t *z,size_t budget) {
    uint8_t *stop=(size_t)(z->end-z->out)>budget?z->out+budget:z->end;
    for (;;) switch (z->state) {
    case 0: {
        if (z->out>=stop && stop<z->end) return 0;
        if (z->in>=z->in_end) return -1;
        uint8_t t=*z->in++;uint32_t l=t>>4;z->token=t;
        if (l==15) {uint8_t b;do {if (z->in>=z->in_end) return -1;b=*z->in++;l+=b;} while (b==255);}
        z->lit=l;z->state=1;
    } /* fall through */
    case 1:
        if (z->lit) {
            uint32_t n=z->lit;
            if (n>(uint32_t)(stop-z->out)) {if (z->out>=stop) return z->out>=z->end?-1:0;n=(uint32_t)(stop-z->out);}
            if (n>(uint32_t)(z->in_end-z->in)) return -1;
            memmove(z->out,z->in,n);z->out+=n;z->in+=n;z->lit-=n;
            if (z->lit) return 0;
        }
        if (z->in==z->in_end) {z->state=4;return z->out==z->end?1:-1;}
        z->state=2;
        /* fall through */
    case 2: {
        if (z->in_end-z->in<2) return -1;
        uint32_t o=(uint32_t)z->in[0]|(uint32_t)z->in[1]<<8,m=z->token&15u;z->in+=2;
        if (m==15) {uint8_t b;do {if (z->in>=z->in_end) return -1;b=*z->in++;m+=b;} while (b==255);}
        if (!o || o>(uint32_t)(z->out-z->base)) return -1;
        z->moff=o;z->mlen=m+4;z->state=3;
    } /* fall through */
    case 3: {
        uint32_t n=z->mlen;
        if (n>(uint32_t)(stop-z->out)) {if (z->out>=stop) return z->out>=z->end?-1:0;n=(uint32_t)(stop-z->out);}
        uint8_t *d=z->out;const uint8_t *s=d-z->moff;
        if (z->moff>=n) memcpy(d,s,n);
        else for (uint32_t i=0;i<n;i++) d[i]=s[i];
        z->out+=n;z->mlen-=n;
        if (z->mlen) return 0;
        z->state=0;break;
    }
    default: return z->out==z->end?1:-1;
    }
}

/* msx_cine_delta_validate in pieces: about `budget` packet bytes per call.
 * Returns 1 when the packet is valid (and ends exactly at its length), 0
 * when it stopped on the budget, -1 when it is invalid. */
typedef struct { size_t offset,cursor; } msx_cine_walk_t;
static inline int msx_cine_validate_part(msx_cine_walk_t *w,size_t pixels,const uint8_t *data,size_t length,size_t budget) {
    if (!pixels || !data || length<4) return -1;
    size_t offset=w->offset,cursor=w->cursor,stop=offset+budget;
    for (;;) {
        if (offset>=stop) {w->offset=offset;w->cursor=cursor;return 0;}
        if (length-offset<4) return -1;
        unsigned skip=msx_cine_be16(data+offset),word=msx_cine_be16(data+offset+2),count=word&32767u;offset+=4;
        if (!skip && !word) return offset==length?1:-1;
        if (word==32768u) return -1;
        if (skip>pixels-cursor) return -1;
        cursor+=skip;
        size_t bytes=(word&32768u)?2u:count*2u;
        if (count>pixels-cursor || bytes>length-offset) return -1;
        cursor+=count;offset+=bytes;
    }
}

/* msx_cine_delta_apply_trusted in pieces (a packet already validated): about
 * `budget` bytes written per call, runs split anywhere. Pixel runs are
 * copied whole (the packet's pixels are big-endian RGBA5551, the canvas'
 * order on the N64), repeated pixels filled a word at a time (headers count
 * as 4 bytes). lo/hi widen to
 * the pixel span written (for a cache writeback of only that span). Returns
 * true when the packet is done. */
typedef struct { size_t offset,cursor,done;uint32_t left;uint16_t pixel;bool fill; } msx_cine_apply_t;   // done: bytes written so far
static inline void msx_cine_fill16(uint16_t *d,uint16_t pixel,uint32_t n) {
    if (n && ((uintptr_t)d&2u)) {*d++=pixel;n--;}
    uint32_t pair=(uint32_t)pixel<<16|pixel,*w=(uint32_t *)(void *)d;
    for (;n>=8;n-=8,w+=4) {w[0]=pair;w[1]=pair;w[2]=pair;w[3]=pair;}
    for (;n>=2;n-=2) *w++=pair;
    if (n) *(uint16_t *)(void *)w=pixel;
}
static inline bool msx_cine_apply_part(msx_cine_apply_t *a,uint16_t *canvas,const uint8_t *data,size_t budget,size_t *lo,size_t *hi) {
    size_t written=0;
    for (;;) {
        if (written>=budget) return false;
        if (!a->left) {
            unsigned skip=msx_cine_be16(data+a->offset),word=msx_cine_be16(data+a->offset+2);a->offset+=4;
            if (!skip && !word) return true;
            a->cursor+=skip;a->left=word&32767u;a->fill=(word&32768u)!=0;
            if (a->fill) {a->pixel=msx_cine_be16(data+a->offset);a->offset+=2;}
            if (a->cursor<*lo) *lo=a->cursor;
            written+=4;a->done+=4;
        }
        uint32_t n=a->left;
        if (written+(size_t)n*2u>budget) {n=(uint32_t)((budget>written?budget-written:0)/2u);if (!n) n=1;}
        uint16_t *d=canvas+a->cursor;
        if (a->fill) msx_cine_fill16(d,a->pixel,n);
        else {
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__==__ORDER_BIG_ENDIAN__
            memcpy(d,data+a->offset,(size_t)n*2u);
#else
            for (uint32_t i=0;i<n;i++) d[i]=msx_cine_be16(data+a->offset+2u*i);
#endif
            a->offset+=(size_t)n*2u;
        }
        a->cursor+=n;a->left-=n;written+=(size_t)n*2u;a->done+=(size_t)n*2u;
        if (a->cursor>*hi) *hi=a->cursor;
    }
}
#endif
