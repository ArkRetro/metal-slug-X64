#!/usr/bin/env python3
"""LZ4 blocks for the scene chunks (codec 2, src/scene_codec.h): the optimal
parser tools/lz4opt.c, built on first use into build/host/lz4opt, with every
block decoded back here (a plain Python decoder) before it is accepted."""
import os,struct,subprocess,sys
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]

def unblock(src,size):
    """LZ4 block decoder with the checks of scene_decode (scene_codec.h)."""
    out=bytearray();i=0;n=len(src)
    while True:
        assert i<n;t=src[i];i+=1;l=t>>4
        if l==15:
            while True:
                b=src[i];i+=1;l+=b
                if b!=255:break
        assert i+l<=n and len(out)+l<=size;out+=src[i:i+l];i+=l
        if i==n:break
        off=src[i]|src[i+1]<<8;i+=2;m=t&15
        if m==15:
            while True:
                b=src[i];i+=1;m+=b
                if b!=255:break
        m+=4;assert 0<off<=len(out) and len(out)+m<=size
        if off>=m:out+=out[-off:len(out)-off+m]
        else:
            for _ in range(m):out.append(out[-off])
    assert len(out)==size
    return bytes(out)

def tool():
    src=ROOT/'tools/lz4opt.c';exe=ROOT/'build/host/lz4opt'
    if not exe.exists() or exe.stat().st_mtime<src.stat().st_mtime:
        exe.parent.mkdir(parents=True,exist_ok=True)
        subprocess.run([os.environ.get('HOST_CC','cc'),'-O2','-std=c11','-Wall','-Wextra','-Werror',str(src),'-o',str(exe)],check=True)
    return exe

def compress(blocks):
    """One LZ4 block per input block (all the same size), each checked."""
    blocks=list(blocks)
    if not blocks:return []
    size=len(blocks[0]);assert all(len(b)==size for b in blocks) and 0<size<=65536
    r=subprocess.run([str(tool())],input=struct.pack('<I',size)+b''.join(blocks),stdout=subprocess.PIPE,check=True).stdout
    out=[];o=0
    for b in blocks:
        n,=struct.unpack_from('<I',r,o);o+=4;z=r[o:o+n];o+=n
        assert unblock(z,size)==b,'LZ4 block does not decode to its source'
        out.append(z)
    assert o==len(r)
    return out

def compress_any(blocks):
    """One LZ4 block per input block of any length up to 1 MiB (the movies'
    packets, tools/msxc_compress.py), each checked."""
    blocks=list(blocks)
    if not blocks:return []
    assert all(0<len(b)<=1<<20 for b in blocks)
    r=subprocess.run([str(tool())],input=struct.pack('<I',0)+b''.join(struct.pack('<I',len(b))+b for b in blocks),stdout=subprocess.PIPE,check=True).stdout
    out=[];o=0
    for b in blocks:
        n,=struct.unpack_from('<I',r,o);o+=4;z=r[o:o+n];o+=n
        assert unblock(z,len(b))==b,'LZ4 block does not decode to its source'
        out.append(z)
    assert o==len(r)
    return out

if __name__=='__main__':
    data=Path(sys.argv[1]).read_bytes();size=int(sys.argv[2]) if len(sys.argv)>2 else 16384
    z=compress([data[i:i+size] for i in range(0,len(data)-len(data)%size,size)])
    print(len(z),'blocks',sum(min(len(x),size) for x in z),'bytes stored of',len(z)*size)
