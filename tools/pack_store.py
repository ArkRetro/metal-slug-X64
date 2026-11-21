#!/usr/bin/env python3
"""How the 16 KiB chunks of the scene packs are stored and read back
(docs/COMPRESION.md): one module for the packers (pack_scene.py,
pack_mission.py) and for every tool that reads a pack.

A chunk record is (u32 offset, u32 length, u32 crc32 of the decoded 16 KiB,
u32 word): word bits 0..7 are the codec (0 raw, 1 (run, byte) pairs, 2 one
LZ4 block), bits 8..15 the mission whose pack holds the bytes (0: this pack;
mission packs from version 5 on). All codecs are lossless; `encode` keeps a
block only after decoding it back, and `decode` checks the CRC."""
import struct,zlib
from pathlib import Path
import lz4_blocks
CHUNK=16384;PAGE=2048
RAW,RLE,LZ4=0,1,2

def rle(data):
    out=bytearray();i=0
    while i<len(data):
        n=1
        while n<255 and i+n<len(data) and data[i+n]==data[i]: n+=1
        out.extend((n,data[i]));i+=n
    return bytes(out)
def unrle(data):
    assert len(data)%2==0
    return b''.join(bytes([data[i+1]])*data[i] for i in range(0,len(data),2))

def encode(chunks,legacy=False):
    """[(stored bytes, codec)] of decoded 16 KiB chunks. legacy: the codecs
    of pack versions <= 4 (run pairs, or raw when they do not shrink it)."""
    chunks=list(chunks);assert all(len(c)==CHUNK for c in chunks)
    if legacy:
        out=[]
        for c in chunks:
            p=rle(c)
            if len(p)<len(c):assert unrle(p)==c;out.append((p,RLE))
            else:out.append((c,RAW))
        return out
    return [(z,LZ4) if len(z)<len(c) else (c,RAW) for c,z in zip(chunks,lz4_blocks.compress(chunks))]

def decode(stored,crc,word):
    codec=word&0xff
    d=stored if codec==RAW else unrle(stored) if codec==RLE else lz4_blocks.unblock(stored,CHUNK) if codec==LZ4 else None
    assert d is not None and len(d)==CHUNK and zlib.crc32(d)==crc,'chunk does not decode to its CRC'
    return d

class MissionPack:
    """Header, chunk table and decoded chunks of romfs/missionN.pack
    (versions 3..5; the chunks another mission's pack holds are read from
    the pack beside it)."""
    def __init__(self,path):
        self.path=Path(path);self.raw=b=self.path.read_bytes();assert b[:4]==b'MSXM'
        self.version,self.mission,self.stages,self.palettes=struct.unpack_from('>HHHH',b,4)
        assert 3<=self.version<=5,('pack version',self.version)
        self.images,nchunk,self.cells,self.grid_bytes,self.actors,self.actor_images,self.resources=struct.unpack_from('>7I',b,12)
        self.header_bytes=48 if self.version>=4 else 40
        self.data_offset,self.data_length=struct.unpack_from('>II',b,40) if self.version>=4 else (0,0)
        o=self.header_bytes+128*self.stages
        self.cells_at=o;o+=22*self.cells;self.images_at=o;o+=8*self.images;self.palettes_at=o;o+=32*self.palettes
        self.chunks_at=o;o+=16*nchunk;self.actors_at=o;o+=8*self.actors;self.actor_images_at=o;o+=12*self.actor_images
        self.resources_at=o;o+=4*self.resources;self.grids_at=o
        self.chunks=[struct.unpack_from('>4I',b,self.chunks_at+16*i) for i in range(nchunk)]
        self._others={};self._cache={}
        self.refs=[]
        if self.version>=5 and b[-64:-60]==b'MSXX':
            n,=struct.unpack_from('>H',b,len(b)-60)
            self.refs=[struct.unpack_from('>HHII',b,len(b)-56+12*i) for i in range(n)]   # mission, 0, pack bytes, pack crc32
    def other(self,mission):
        if mission not in self._others:
            p=MissionPack(self.path.with_name(f'mission{mission}.pack'))
            ref=next((r for r in self.refs if r[0]==mission),None)
            assert ref and ref[2]==len(p.raw) and ref[3]==zlib.crc32(p.raw),f'mission{mission}.pack is not the one mission{self.mission}.pack was built against: rebuild mission{self.mission}.pack'
            self._others[mission]=p
        return self._others[mission]
    def stored(self,i):
        off,ln,crc,word=self.chunks[i];src=word>>8&0xff
        raw=self.other(src).raw if src else self.raw
        assert off+ln<=len(raw);return raw[off:off+ln]
    def chunk(self,i):
        if i not in self._cache:self._cache[i]=decode(self.stored(i),self.chunks[i][2],self.chunks[i][3])
        return self._cache[i]
    def page(self,p):return self.chunk(p//8)[(p%8)*PAGE:(p%8+1)*PAGE]
