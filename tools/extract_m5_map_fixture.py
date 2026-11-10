#!/usr/bin/env python3
"""Mission 5 map reconnaissance fixture (docs/MISION5.md), one stage.

Input: a capture of tools/capture_m5.sh (MAME 0.264 from the section states of
run4): cells.txt (every collision cell the kernel read with 0C79FE from the
terrain ring 10C72E +88: local pixel x, y, value, first and last frame),
events.csv (EVERY write to the ring with its PC, MSX_FULL_GRID_ALL=1) and
debug.log (0C70FC patches, 0C88B0 stamps).

Every read is explained by the ring's own history or the tool stops. The
history of each ring slot (column & 63, 63 - (row & 63)) is rebuilt from the
writes; a read of value v seen in frames f0..f1 must find v in its slot in
that interval, and the PC that wrote it says what it is:
* 0C734E..0C735A (0C72EC, a map column load) with the ROM cell of the stage
  grid (tools/export_stage_rom.py) at the read's own column and row: static;
* the same writer with the ROM cell of a column 64 k away (or a row 64 k
  away): ring, the slot still held another column of the map when an actor
  far from the view read it;
* 0C7038 (the ring's clear at the stage load) with value 0: empty when the
  ROM grid has no cell there either (rows above or below the map: checked as
  static), else a read before the column's load (counted);
* 0C8982 (0C88B0) or 0C8AA2 (0C8A3C), the two template stampers: stamp;
* a read that ended before the stage's ring clear: the previous stage's ring
  (counted).

Output: a stage entry of data/m5_map_fixture.json.gz for tools/test_m5_map.c
(make check): the static reads (deduplicated; x, y, value) that the pack's
grid must return through neo_collision_cell, the counts per kind and the
stamps (frame, owner, template, cells written) whose native events are phase 2."""
import argparse,bisect,gzip,json,sys
from collections import Counter
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from extract_map_patches import Rom
from export_stage_rom import camera_maps,map_blocks
ROOT=Path(__file__).resolve().parents[1]
LOAD={0xc734e,0xc7352,0xc7356,0xc735a};CLEAR={0xc7038};STAMP={0xc8982:'0C88B0',0xc8aa2:'0C8A3C'}
def s16(v):return v-65536 if v>=32768 else v
def stage(capture,rom,descriptor,last):
    base={};loads=[l for l in camera_maps(rom,descriptor) if l['object']==0x10c72e and l['map']]
    for l in loads:
        t,c,w,h,tiles,coll=map_blocks(rom,l['map'])
        for (x,r),v in coll.items():base[(2*l['column']+x,2*l['top']+1-r)]=v
    hist={};writers=Counter();stamp_frames={}
    for line in (capture/'events.csv').read_text().splitlines()[1:]:
        p=line.split(',');a=int(p[2],16)
        if p[1]!='grid' or not 0x10c7b6<=a<0x10d7b6:continue
        pc=int(p[5],16);assert pc in LOAD or pc in CLEAR or pc in STAMP,('unknown ring writer',hex(pc))
        f=int(p[0]);hist.setdefault(a-0x10c7b6,[]).append((f,int(p[4]),pc));writers[pc]+=1
        if pc in STAMP:stamp_frames.setdefault((f,pc),0);stamp_frames[(f,pc)]+=1
    cleared=max(e[0] for h in hist.values() for e in h if e[2] in CLEAR and e[0]<=last)   # the stage's own ring clear (the capture can start in the stage before)
    patches=[];stamps=[]
    # (A TAS capture, tools/capture_m5_tas.sh, has no debugger log: its stamps are the ring writes alone.)
    for line in ((capture/'debug.log').read_text(errors='replace').splitlines() if (capture/'debug.log').exists() else ()):
        if line.startswith('PATCH '):patches.append(line)
        elif line.startswith('STAMP '):
            f=dict(t.split('=') for t in line.split()[1:]);stamps.append(dict(frame=int(f['f']),owner='%06X'%int(f['a6'],16),template='%06X'%int(f['a1'],16),object='%06X'%int(f['a0'],16)))
    assert not patches,'0C70FC patches: use the Mission 4 extractor rules'
    kinds=Counter();static={};unexplained=[];reads=0
    for line in (capture/'cells.txt').read_text().splitlines():
        p=line.split()
        if p[0]!='C' or int(p[1],16)!=0x10c72e:continue
        x,y,v,f0,f1=s16(int(p[2])),s16(int(p[3])),int(p[4]),int(p[5]),int(p[6])
        if f0>last:continue
        reads+=1;c,r=x>>3,y>>3;h=hist.get(((c&63)<<6)|(63-(r&63)),[])
        # The section state is a few frames before the stage's ring clear: until it the ring is the previous stage's.
        if f1<cleared:kinds['previous stage ring']+=1;continue
        # The slot's values during f0..f1: the last write before f0 and every write up to f1 (the taps' frame is the
        # last completed one for both files; a write and a read of the same frame can come in either order).
        i=bisect.bisect_left(h,(f0,-1,-1));cand=[e for e in h[max(0,i-1):] if e[0]<=f1 and e[1]==v]
        kind=None
        for f,_,pc in cand:
            if pc in LOAD and base.get((c,r),0)==v:kind='static';break
        if kind is None:
            for f,_,pc in cand:
                if pc in STAMP:kind='stamp';break
                if pc in LOAD and any(base.get((c+64*kc,r+64*kr),0)==v for kc in range(-6,7) for kr in (-1,0,1) if (kc,kr)!=(0,0)):kind='ring';break
                if pc in CLEAR and v==0:kind='empty' if base.get((c,r),0)==0 else 'clear before the load';break
        if kind is None:unexplained.append((x,y,v,f0,f1,[(f,hex(pc)) for f,_,pc in cand][:4]));continue
        kinds[kind]+=1
        if kind in ('static','empty'):static[(x&~7,y&~7)]=v
    assert not unexplained,('reads not explained',len(unexplained),unexplained[:10])
    events=[dict(frame=f,routine=STAMP[pc],cells=n) for (f,pc),n in sorted(stamp_frames.items())]
    print('%06X: %d reads %s; ring writes %s'%(descriptor,reads,dict(kinds),{'%06X'%k:v for k,v in sorted(writers.items())}))
    return dict(descriptor='%06X'%descriptor,capture=capture.name,last=last,maps=['%06X'%l['map'] for l in loads],reads=reads,kinds=dict(kinds),
                static=[[x,y,v] for (x,y),v in sorted(static.items())],stamps=stamps,stamp_writes=events)
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('stages',nargs='+',help='CAPTURE:DESCRIPTOR:LAST per stage (LAST = last frame of the reads kept)')
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'));ap.add_argument('--output',type=Path,default=ROOT/'data/m5_map_fixture.json.gz')
    ap.add_argument('--source');a=ap.parse_args();rom=Rom(a.rom);out=[]
    for s in a.stages:
        c,d,l=s.split(':');out.append(stage(Path(c),rom,int(d,16),int(l)))
    doc=dict(source=a.source or 'MAME 0.264 mslugx euro, tools/capture_m5.sh (states4 sec06_247661 / sec07_252827 / sec08_274387, replays of run4); tools/extract_m5_map_fixture.py',stages=out)
    with gzip.open(a.output,'wt') as f:json.dump(doc,f,separators=(',',':'))
    print('%s static reads (deduplicated) -> %s'%(' + '.join(str(len(s['static'])) for s in out),a.output))
if __name__=='__main__':main()
