#!/usr/bin/env python3
"""Rewrite an MSXC v1 movie as v2: every packet compressed as one LZ4 block.

v2 keeps the 32-byte header (version 2) and the v1 offset table unchanged:
those raw offsets still describe the decompressed layout, so the player's
movie cache, CRC checks, delta packets and heavy-frame choice are the v1
ones and the frames are bit-identical. After the raw table comes a second
table (count+1 words) of file offsets of the stored packets (bit 0: one pad
byte follows the packet); a packet that LZ4 does not shrink is stored raw. The device decompresses with
libdragon's decompress_lz4_full_fast (docs/RENDIMIENTO.md, ROM budget).

v3 (written when it is smaller): a packet equal byte for byte to an earlier
one is stored once (docs/COMPRESION.md, Duplicados). After the two tables
comes a third, count u16 (padded to a word): the packet whose stored bytes
each packet reads, itself or the first equal one. A packet that reads another
stores nothing: its stored word is the next stored packet's offset, with no
pad bit.

usage: msxc_compress.py [--greedy] in.msxc out.msxc
"""
import struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
import lz4_blocks

def lz4_block(src):
    """Greedy LZ4 block (format rules: last 5 bytes literal, no match starting
    in the last 12 bytes, offsets < 65536)."""
    n=len(src);out=bytearray();anchor=0;i=0;table={}
    limit=n-12
    def emit(lit_end,match_len,offset):
        lit=lit_end-anchor
        tok_l=min(lit,15);tok_m=0 if match_len is None else min(match_len-4,15)
        out.append(tok_l<<4|tok_m)
        if lit>=15:
            r=lit-15
            while r>=255:out.append(255);r-=255
            out.append(r)
        out.extend(src[anchor:lit_end])
        if match_len is None:return
        out.extend(struct.pack('<H',offset))
        if match_len-4>=15:
            r=match_len-4-15
            while r>=255:out.append(255);r-=255
            out.append(r)
    while i<limit:
        key=src[i:i+4];j=table.get(key);table[key]=i
        if j is not None and i-j<65536:
            m=4;end=n-5
            while i+m<end and src[j+m]==src[i+m]:m+=1
            emit(i,m,i-j)
            for k in range(i+1,min(i+m,limit)):table[src[k:k+4]]=k
            i+=m;anchor=i
        else:i+=1
    emit(n,None,0)
    return bytes(out)

def lz4_unblock(src,size):
    out=bytearray();i=0
    while i<len(src):
        t=src[i];i+=1;l=t>>4
        if l==15:
            while True:
                b=src[i];i+=1;l+=b
                if b!=255:break
        out+=src[i:i+l];i+=l
        if i>=len(src):break
        off=src[i]|src[i+1]<<8;i+=2;m=(t&15)+4
        if m==19:
            while True:
                b=src[i];i+=1;m+=b
                if b!=255:break
        for _ in range(m):out.append(out[-off])
    assert len(out)==size
    return bytes(out)

def unpack(o):
    """The v1 movie a v2/v3 file holds (the reader of the player's tables)."""
    version,count=struct.unpack('>I',o[4:8])[0],struct.unpack('>I',o[16:20])[0]
    assert o[:4]==b'MSXC' and version in (2,3)
    raw=[struct.unpack('>I',o[32+4*i:36+4*i])[0] for i in range(count+1)]
    st=[struct.unpack('>I',o[32+4*(count+1)+4*i:36+4*(count+1)+4*i])[0] for i in range(count+1)]
    ref=list(struct.unpack('>%dH'%count,o[32+8*(count+1):32+8*(count+1)+2*count])) if version==3 else list(range(count))
    out=bytearray(o[:4]+struct.pack('>I',1)+o[8:32+4*(count+1)])
    for k in range(count):
        j=ref[k];assert j<=k and ref[j]==j;at=st[j]&~1;have=(st[j+1]&~1)-at-(st[j]&1);n=raw[k+1]-raw[k]
        assert n==raw[j+1]-raw[j]
        if j!=k:assert st[k]&1==0 and st[k]&~1==st[k+1]&~1
        c=o[at:at+have];out+=c if have==n else lz4_unblock(c,n)
    assert st[count]==len(o) and len(out)==raw[count]
    return bytes(out)

def main(a,b,greedy=False):
    d=open(a,'rb').read()
    assert d[:4]==b'MSXC' and struct.unpack('>I',d[4:8])[0]==1,a
    count=struct.unpack('>I',d[16:20])[0]
    raw=[struct.unpack('>I',d[32+4*i:36+4*i])[0] for i in range(count+1)]
    assert raw[0]==32+4*(count+1) and raw[count]==len(d)
    # The optimal parser of the scene chunks (tools/lz4opt.c): the same LZ4
    # blocks to the player, 7-13 % smaller than the greedy ones (lz4_block,
    # kept for --greedy and the measurements of docs/COMPRESION.md).
    packets=[d[raw[i]:raw[i+1]] for i in range(count)]
    packed=[lz4_block(p) for p in packets] if greedy else lz4_blocks.compress_any(packets)
    blobs=[]
    for p,c in zip(packets,packed):
        assert lz4_unblock(c,len(p))==p
        blobs.append(c if len(c)<len(p) else p)
    first={};ref=[first.setdefault(p,i) for i,p in enumerate(packets)]
    def write(version):
        r=ref if version==3 else list(range(count))
        at=32+8*(count+1)+(4*((count+1)//2) if version==3 else 0);stored=[None]*(count+1);body=bytearray()
        # Packets start even (PI DMA); bit 0 marks a packet followed by one pad byte.
        for k,c in enumerate(blobs):
            if r[k]!=k:continue
            stored[k]=at|(len(c)&1);body+=c+(b'\0' if len(c)&1 else b'');at+=(len(c)+1)&~1
        stored[count]=at
        for k in range(count-1,-1,-1):
            if stored[k] is None:stored[k]=stored[k+1]&~1
        o=bytearray(d[:4]+struct.pack('>I',version)+d[8:32+4*(count+1)])
        o+=b''.join(struct.pack('>I',v) for v in stored)
        if version==3:
            o+=struct.pack('>%dH'%count,*r)
            while len(o)%4:o.append(0)
        o+=body;assert len(o)==stored[-1]
        return bytes(o)
    o=min((write(v) for v in (2,3)),key=len)
    assert unpack(o)==d,'the stored movie does not give the source back'
    open(b,'wb').write(o)
    print('%s: %d -> %d bytes (%d packets)'%(b,len(d),len(o),count))

if __name__=='__main__':
    args=[x for x in sys.argv[1:] if x!='--greedy']
    main(args[0],args[1],'--greedy' in sys.argv[1:])
