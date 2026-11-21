#!/usr/bin/env python3
"""Mission 4 map reconnaissance fixture (docs/MISION4.md).

Input: a capture of tools/capture_m4_map.sh (MAME 0.264 from
states4/sec05_149829, the replay of run4): cells.txt, every collision cell
the kernel read with 0C79FE from the terrain ring 10C72E +88 (local pixel
x, y, value, first and last frame), and debug.log, the map patches 0C70FC
(sub-map, D0..D5) and the stamps 0C88B0.

Every read is explained from the ROM or the tool stops:
* static: the ROM cell of the stage grid (tools/export_stage_rom.py: the
  maps the camera list 0E741A loads into 10C72E, kernel rows) at column
  x >> 3, row y >> 3;
* ring: a column outside the 64 loaded ones reads the loaded column that
  shares its slot (column +- 64 k); rows likewise (the ring is 64 x 64);
* patch: the collision part of a sub-map copied by 0C70FC before the read's
  last frame (0C72EC: block (D0 + i, D1 + j) to the block column D4 + i,
  block row D5 - j);
* stamp: the value a 0C88B0 stamp wrote (PC 0C8982, --grid-events: the grid
  write tap of a capture with MSX_FULL_GRID_ALL=1) into the cell's ring
  slot before the read's last frame.

Output: data/m4_map_fixture.json.gz for tools/test_m4_map.c (make check):
the static reads (deduplicated; x, y, value) that the pack's grid must
return through neo_collision_cell, and the counts and list of the patches
(sub-map, D0..D5, frame, cells changed) whose native events are phase 2."""
import argparse,gzip,json,re,sys
from collections import Counter
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from extract_map_patches import Rom
from export_stage_rom import camera_maps,map_blocks
ROOT=Path(__file__).resolve().parents[1]
def s16(v):return v-65536 if v>=32768 else v
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('capture',type=Path);ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    ap.add_argument('--descriptor',type=lambda v:int(v,16),default=0x16e1e)
    ap.add_argument('--last',type=int,default=157581,help='last frame of the reads kept (the boss call 0BB028 is at 157582)')
    ap.add_argument('--output',type=Path,default=ROOT/'data/m4_map_fixture.json.gz')
    ap.add_argument('--grid-events',type=Path,help='events.csv of a capture with MSX_FULL_GRID_ALL=1: the stamp writes (PC 0C8982) to the ring 10C7B6')
    a=ap.parse_args();rom=Rom(a.rom)
    base={};loads=[l for l in camera_maps(rom,a.descriptor) if l['object']==0x10c72e and l['map']]
    for l in loads:
        t,c,w,h,tiles,coll=map_blocks(rom,l['map'])
        for (x,r),v in coll.items():base[(2*l['column']+x,2*l['top']+1-r)]=v
    patches=[];stamps=[]
    for line in (a.capture/'debug.log').read_text(errors='replace').splitlines():
        if line.startswith('PATCH '):
            f=dict(t.split('=') for t in line.split()[1:]);d=[s16(int(f['d%d'%i],16)) for i in range(6)]
            if int(f['a0'],16)!=0x10c72e:continue
            sub=int(f['a1'],16);t,c,w,h,tiles,coll=map_blocks(rom,sub)
            cells={}
            if coll:
                for i in range(d[2]):
                    for j in range(d[3]):
                        si,sj=s16(d[0])+i,s16(d[1])+j
                        if not (0<=si<w and 0<=sj<h):continue
                        for k in range(4):
                            v=coll[(2*si+(k&1),2*sj+(k>>1))]
                            cells[(2*(d[4]+i)+(k&1),2*(d[5]-j)+1-(k>>1))]=v
            patches.append(dict(frame=int(f['f']),submap=sub,d=d,owner=int(f['a6'],16),cells=cells,blocks=[w,h],collision=bool(coll)))
        elif line.startswith('STAMP '):
            f=dict(t.split('=') for t in line.split()[1:]);stamps.append(dict(frame=int(f['f']),owner=int(f['a6'],16),a2=int(f['a2'],16)))
    stamp_writes={}
    if a.grid_events:
        for line in a.grid_events.read_text().splitlines()[1:]:
            p=line.split(',')
            if p[1]=='grid' and p[5]=='c8982' and 0x10c7b6<=int(p[2],16)<0x10d7b6:
                stamp_writes.setdefault(int(p[2],16)-0x10c7b6,[]).append((int(p[0]),int(p[4])))
    reads=[]
    for line in (a.capture/'cells.txt').read_text().splitlines():
        p=line.split()
        if p[0]!='C' or int(p[1],16)!=0x10c72e:continue
        x,y,v,f0,f1=s16(int(p[2])),s16(int(p[3])),int(p[4]),int(p[5]),int(p[6])
        if f0>a.last:continue
        reads.append((x,y,v,f0,f1,int(p[7])))
    kinds=Counter();static={};unexplained=[];dynamic=[];patch_hits=Counter()
    for x,y,v,f0,f1,n in reads:
        c,r=x>>3,y>>3
        if base.get((c,r),0)==v:kinds['static']+=1;static[(x&~7,y&~7)]=v;continue
        hit=None
        for i,p in enumerate(patches):
            if p['frame']<=f1 and p['cells'].get((c,r))==v:hit=i
        if hit is not None:kinds['patch']+=1;patch_hits[hit]+=1;continue
        # A patch can also restore the ROM value over an earlier patch; a cell a patch changed and read before it is static above.
        if any(base.get((c+64*kc,r+64*kr),0)==v for kc in (-2,-1,0,1,2) for kr in (-1,0,1)):kinds['ring']+=1;continue
        if any(p['frame']<=f1 and p['cells'].get((c+64*kc,r+64*kr))==v for p in patches for kc in (-2,-1,0,1,2) for kr in (-1,0,1)):kinds['ring patch']+=1;continue
        # 0C8982: a stamp wrote the value into the ring slot of the cell (0C79FE's index) before the read's last frame.
        if any(f<=f1 and nv==v for f,nv in stamp_writes.get(((c&63)<<6)|(63-(r&63)),())):kinds['stamp 0C88B0']+=1;dynamic.append((x,y,v,f0,f1));continue
        unexplained.append((x,y,v,f0,f1));kinds['unexplained']+=1
    print('reads',len(reads),dict(kinds))
    assert not unexplained,('reads not explained',unexplained[:10])
    out=dict(source='MAME 0.264 mslugx euro, tools/capture_m4_map.sh (states4/sec05_149829, replay of run4) frames 149829..%d; tools/extract_m4_map_fixture.py'%a.last,
        descriptor='%06X'%a.descriptor,maps=['%06X'%l['map'] for l in loads],reads=len(reads),kinds=dict(kinds),
        static=[[x,y,v] for (x,y),v in sorted(static.items())],
        stamp_reads=[list(u) for u in dynamic],
        patches=[dict(frame=p['frame'],submap='%06X'%p['submap'],d=p['d'],owner='%06X'%p['owner'],blocks=p['blocks'],collision=p['collision'],
                      cells_changed=sum(1 for k,v in p['cells'].items() if base.get(k,0)!=v),reads=patch_hits[i]) for i,p in enumerate(patches)],
        stamps=[dict(frame=s['frame'],owner='%06X'%s['owner']) for s in stamps])
    with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
    print('%d static reads (deduplicated), %d patches, %d stamps -> %s'%(len(static),len(patches),len(stamps),a.output))
if __name__=='__main__':main()
