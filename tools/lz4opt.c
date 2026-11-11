/* LZ4 block compressor with an optimal parse, for the 16 KiB scene chunks
 * and the packets of the movies (tools/lz4_blocks.py, docs/COMPRESION.md). Lossless and deterministic: the
 * same input always gives the same block. Not a copy of the reference
 * encoder: a hash-chain match finder (longest match per position) and a
 * backward dynamic program over (literal run, match) sequences with the exact
 * byte cost of the format; the search is bounded (DEPTH candidates per
 * position, NEAR lengths and starts around the best), so the parse is near
 * optimal, never wrong: tools/lz4_blocks.py decodes every block back.
 *
 * stdin:  u32le block size, then blocks of that size (the last may be short);
 *         or u32le 0, then per block u32le length and the block
 * stdout: per block u32le length, then the LZ4 block
 *
 * Format rules kept (LZ4 block format): the last 5 bytes are literals, no
 * match starts in the last 12 bytes, offsets 1..65535. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum { MAXN=1<<20,HASH=1<<16,DEPTH=512,NEAR=64,INF=0x3fffffff };
static uint8_t in[MAXN],out[MAXN+MAXN/255+64];
static int head[HASH],prev_[MAXN],mlen[MAXN],moff[MAXN];
static int f[MAXN+1],fl[MAXN+1],g[MAXN+1],gm[MAXN+1];
static unsigned hash4(const uint8_t *p) {uint32_t v;memcpy(&v,p,4);return (v*2654435761u)>>16&(HASH-1);}
static int ext(int v) {return v<15?0:1+(v-15)/255;}   /* extra length bytes of a 4-bit field holding v */
static size_t put_len(uint8_t *o,size_t n,int v) {
    if (v<15) return n;
    v-=15;while (v>=255) {o[n++]=255;v-=255;}
    o[n++]=(uint8_t)v;return n;
}
static size_t compress(int n) {
    int last_match=n-12,match_end=n-5;   /* a match starts at <= last_match and ends at <= match_end */
    for (int i=0;i<HASH;i++) head[i]=-1;
    for (int i=0;i<n;i++) {
        mlen[i]=0;moff[i]=0;
        if (i+4>n) continue;
        unsigned h=hash4(in+i);
        if (i<=last_match) {
            int limit=match_end-i,best=0,bo=0,depth=DEPTH;
            /* The previous position's match, one byte shorter, holds here:
             * the chain is then walked only for longer ones (a candidate
             * must agree at the byte after the best so far). */
            if (i && mlen[i-1]>1) {best=mlen[i-1]-1;bo=moff[i-1];if (best>limit) best=limit;}
            for (int j=head[h];j>=0 && depth-- && best<limit;j=prev_[j]) {
                if (i-j>65535) break;
                if (best && in[j+best]!=in[i+best]) continue;
                int m=0;while (m<limit && in[j+m]==in[i+m]) m++;
                if (m>best) {best=m;bo=i-j;}
            }
            if (best>=4) {mlen[i]=best;moff[i]=bo;}
        }
        prev_[i]=head[h];head[h]=i;
    }
    /* f[i]: least bytes for in[i..n) starting a sequence at i; fl[i] its
     * literal run (the match, if any, starts at i+fl[i], length gm[]).
     * g[j]: least bytes for a match at j and everything after it. */
    f[n]=INF;g[n]=INF;
    for (int i=n-1;i>=0;i--) {
        g[i]=INF;gm[i]=0;
        for (int m=4;m<=mlen[i];m++) {
            if (m==NEAR+4 && mlen[i]-NEAR>m) m=mlen[i]-NEAR;   /* short matches and the longest ones */
            int c=2+ext(m-4)+f[i+m];
            if (c<g[i]) {g[i]=c;gm[i]=m;}
        }
        int best=1+ext(n-i)+(n-i),bl=n-i;   /* the block's last sequence: literals only */
        for (int l=0,tries=0;i+l<=last_match && l<best && tries<NEAR;l++) {
            if (g[i+l]>=INF) continue;
            int c=1+ext(l)+l+g[i+l];
            if (c<best) {best=c;bl=l;tries=0;} else tries++;   /* stops NEAR matches after the best start */
        }
        f[i]=best;fl[i]=bl;
    }
    size_t o=0;int i=0;
    if (!n) {out[o++]=0;return o;}
    while (i<n) {
        int l=fl[i],at=i+l;
        if (at>=n) {   /* last literals */
            out[o++]=(uint8_t)((l<15?l:15)<<4);o=put_len(out,o,l);memcpy(out+o,in+i,(size_t)l);o+=(size_t)l;break;
        }
        int m=gm[at];
        out[o++]=(uint8_t)((l<15?l:15)<<4|(m-4<15?m-4:15));o=put_len(out,o,l);memcpy(out+o,in+i,(size_t)l);o+=(size_t)l;
        out[o++]=(uint8_t)(moff[at]&255);out[o++]=(uint8_t)(moff[at]>>8);o=put_len(out,o,m-4);
        i=at+m;
    }
    return o;
}
int main(void) {
    uint8_t w[4];
    if (fread(w,1,4,stdin)!=4) return 1;
    uint32_t size=(uint32_t)w[0]|(uint32_t)w[1]<<8|(uint32_t)w[2]<<16|(uint32_t)w[3]<<24;
    if (size>MAXN) return 1;
    size_t n;
    for (;;) {
        if (size) n=fread(in,1,size,stdin);
        else {   /* blocks of their own lengths */
            if (fread(w,1,4,stdin)!=4) break;
            n=(uint32_t)w[0]|(uint32_t)w[1]<<8|(uint32_t)w[2]<<16|(uint32_t)w[3]<<24;
            if (!n || n>MAXN || fread(in,1,n,stdin)!=n) return 1;
        }
        if (!n) break;
        size_t o=compress((int)n);
        w[0]=(uint8_t)o;w[1]=(uint8_t)(o>>8);w[2]=(uint8_t)(o>>16);w[3]=(uint8_t)(o>>24);
        if (fwrite(w,1,4,stdout)!=4 || fwrite(out,1,o,stdout)!=o) return 1;
    }
    return 0;
}
