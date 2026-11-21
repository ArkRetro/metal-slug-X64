#!/usr/bin/env python3
"""Mission 6 stage 1: the collision ring of the bridge 10D7B6 (map 0BC6E6,
collision 45FC2A) -> data/m6ring_fixture.json.gz (tools/test_m6_ring.c).

Two replays of run4 from sec09_292683 (MAME 0.264, the same inputs):
* captures-m6/fam/L (fam/runL.sh): layers.csv, the header 10D7B6 +0..+87 at
  every frame's end (+14, +18, +2E, +32, +34, +82);
* captures-m6/s9 (tools/capture_m6.sh): events.csv, every write to the ring
  10D83E.. (0C7038 clear, 0C72EC loads at PC 0C734E/52/56/5A), and cells.txt,
  every cell the kernel read from it with 0C79FE (aggregated: x, y, value,
  first and last frame).
Frame labels: capture_fullgame.lua counts frames in its frame_done callback,
so a tap during a frame is labelled N and the snapshot taken at that frame's
end N + 1; the fixture gives both on the snapshot's label (writes and reads
+ 1). Only the frames where the header's inputs change are kept."""
import argparse,collections,gzip,json,struct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];RING=0x10d83e
LOAD={'c734e','c7352','c7356','c735a'}
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--layers',type=Path,default=Path('/home/user/captures-m6/fam/L'))
    ap.add_argument('--cells',type=Path,default=Path('/home/user/captures-m6/s9'))
    ap.add_argument('--output',type=Path,default=ROOT/'data/m6ring_fixture.json.gz')
    a=ap.parse_args();frames=[];last=None
    for l in open(a.layers/'layers.csv'):
        p=l.rstrip().split(',')
        if len(p)<3 or p[1]!='10d7b6':continue
        h=bytes.fromhex(p[2]);x,y=struct.unpack('>ii',h[0x14:0x1c]);o32,o34=struct.unpack('>hh',h[0x32:0x36])
        m=struct.unpack('>I',h[0x2e:0x32])[0];v=[x,y,o32,o34,h[0x82]&0x7f,m]
        if v!=last:frames.append([int(p[0])]+v);last=v
    writes=collections.defaultdict(list);clear=None;loads=0
    for l in open(a.cells/'events.csv').read().splitlines()[1:]:
        p=l.split(',')
        if p[1]!='grid' or not RING<=int(p[2],16)<RING+0x1000:continue
        f=int(p[0])+1
        if p[5]=='c7038':clear=f;continue
        assert p[5] in LOAD,('unknown ring writer',p[5])
        writes[f].append([int(p[2],16)-RING,int(p[4])]);loads+=1
    reads=collections.Counter()
    for l in open(a.cells/'cells.txt'):
        p=l.split()
        if p[1]!='10d7b6':continue
        x,y,v,f0,f1=(int(t) for t in p[2:7])
        reads[(((x&0x1f8)<<3)|(((~y)&0x1f8)>>3),v,f0+1,f1+1)]+=int(p[7])
    doc=dict(source='MAME 0.264 mslugx euro, run4 replays from sec09_292683: captures-m6/fam/L (layers.csv) and captures-m6/s9 (events.csv, cells.txt); tools/extract_m6ring_fixture.py',
             clear=clear,frames=frames,writes=sorted([f,w] for f,w in writes.items()),reads=[[k[0],k[1],k[2],k[3],n] for k,n in sorted(reads.items(),key=lambda e:e[0][2])])
    with gzip.open(a.output,'wt') as f:json.dump(doc,f,separators=(',',':'))
    print('%d header frames, ring clear at %s, %d load frames (%d bytes), %d reads (%d cells x value x frames) -> %s'%(len(frames),clear,len(writes),loads,sum(reads.values()),len(reads),a.output))
if __name__=='__main__':main()
