#!/usr/bin/env python3
"""Mission 6 map reconnaissance fixture (docs/MISION6.md), one stage per argument.

As tools/extract_m5_map_fixture.py (every kernel cell read of 0C79FE explained
by the ring's own write history, or the tool stops), with what Mission 6 adds:
* the base cells of 10C72E are the stage's ROM grid as tools/export_stage_rom.py
  wrote it (filesystem/mission6/s<k>/collision.json: the maps of the camera
  list and, in the stage 016E6E, the columns the looper 0B0B72 repeats);
* 0C70FC patches (debug.log PATCH lines: sub-map, D0..D5): a read whose value
  is the patch's collision cell at its place, written by the column loader,
  is a patch read (counted; the native patch events are phase 2);
* the second terrain object 10D7B6 (stage 016E5E: map 0BC6E6, collision
  only, ring 10D83E): its reads are explained the same way against the ROM
  cells of the maps the list loads into it (static / empty / ring / clear);
  cells of that map the column loader wrote at another column origin (the
  object's owner moves it) are counted apart as `moved origin`. The pack has
  no second grid yet, so they are checked here and counted (phase 2).

Output: a stage entry of data/m6_map_fixture.json.gz for tools/test_m6_map.c."""
import argparse,bisect,gzip,json,sys
from collections import Counter
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from extract_map_patches import Rom
from export_stage_rom import camera_maps,map_blocks
ROOT=Path(__file__).resolve().parents[1]
LOAD={0xc734e,0xc7352,0xc7356,0xc735a};CLEAR={0xc7038};STAMP={0xc8982:'0C88B0',0xc8aa2:'0C8A3C'}
RING={0x10c72e:0x10c7b6,0x10d7b6:0x10d83e}
def s16(v):return v-65536 if v>=32768 else v
def explain(capture,obj,base,patches,last,first,moved=False):
    values=set(base.values())
    ring=RING[obj];hist={};writers=Counter();stamp_frames={}
    for line in (capture/'events.csv').read_text().splitlines()[1:]:
        p=line.split(',');a=int(p[2],16)
        if p[1]!='grid' or not ring<=a<ring+0x1000:continue
        pc=int(p[5],16);assert pc in LOAD or pc in CLEAR or pc in STAMP,('unknown ring writer',hex(pc))
        f=int(p[0]);hist.setdefault(a-ring,[]).append((f,int(p[4]),pc));writers[pc]+=1
        if pc in STAMP:stamp_frames.setdefault((f,pc),0);stamp_frames[(f,pc)]+=1
    clears=[e[0] for h in hist.values() for e in h if e[2] in CLEAR and e[0]<=last]
    cleared=max(clears) if clears else first   # no clear in the capture: the ring was cleared before the section state
    kinds=Counter();static={};unexplained=[];reads=0
    for line in (capture/'cells.txt').read_text().splitlines():
        p=line.split()
        if p[0]!='C' or int(p[1],16)!=obj:continue
        x,y,v,f0,f1=s16(int(p[2])),s16(int(p[3])),int(p[4]),int(p[5]),int(p[6])
        if f0>last:continue
        reads+=1;c,r=x>>3,y>>3;h=hist.get(((c&63)<<6)|(63-(r&63)),[])
        if f1<cleared:kinds['previous stage ring']+=1;continue
        i=bisect.bisect_left(h,(f0,-1,-1));cand=[e for e in h[max(0,i-1):] if e[0]<=f1 and e[1]==v]
        kind=None
        for f,_,pc in cand:
            if pc in LOAD and base.get((c,r),0)==v:kind='static';break
        if kind is None:
            for f,_,pc in cand:
                if pc in STAMP:kind='stamp';break
                if pc in LOAD and any(q['frame']<=f1 and q['cells'].get((c,r))==v for q in patches):kind='patch';break
                if pc in LOAD and any(base.get((c+64*kc,r+64*kr),0)==v for kc in range(-10,11) for kr in (-1,0,1) if (kc,kr)!=(0,0)):kind='ring';break
                if pc in LOAD and any(q['frame']<=f1 and q['cells'].get((c+64*kc,r+64*kr))==v for q in patches for kc in range(-10,11) for kr in (-1,0,1)):kind='ring patch';break
                if pc in CLEAR and v==0:kind='empty' if base.get((c,r),0)==0 else 'clear before the load';break
        if kind is None and v==0 and (not h or h[0][0]>=f0) and not any(e[0]<=f1 and e[1] for e in h):
            # A slot nothing had written yet in the capture, read as 0: the ring was cleared before the section state.
            kind='empty' if base.get((c,r),0)==0 else 'clear before the load'
        if kind is None and moved and v and v in values and any(pc in LOAD for f,_,pc in cand):
            # The second terrain object: a cell of its map that the column loader wrote at another column origin than the
            # list's (its owner moves +32 / +34; not in this capture): counted apart, not explained here (phase 2).
            kind='moved origin'
        if kind is None:unexplained.append((x,y,v,f0,f1,[(f,hex(pc)) for f,_,pc in cand][:4]));continue
        kinds[kind]+=1
        if kind in ('static','empty'):static[(x&~7,y&~7)]=v
    assert not unexplained,('reads not explained','%06X'%obj,len(unexplained),unexplained[:10])
    events=[dict(frame=f,routine=STAMP[pc],cells=n) for (f,pc),n in sorted(stamp_frames.items())]
    return reads,kinds,static,events,writers
def stage(capture,rom,descriptor,last,collision):
    g=json.loads(collision.read_text());cols,rows,row0=g['columns'],g['rows'],g['origin_cells'][1];cells=bytes.fromhex(g['cells_hex'])
    base={(x,r+row0):cells[x*rows+r] for x in range(cols) for r in range(rows) if cells[x*rows+r]}
    loads=camera_maps(rom,descriptor)
    base2={}
    for l in loads:
        if l['object']==0x10d7b6 and l['map']:
            t,c,w,h,tiles,coll=map_blocks(rom,l['map'])
            for (x,r),v in coll.items():base2[(2*l['column']+x,2*l['top']+1-r)]=v
    patches=[];stamps=[];stamps2=[];first=None
    for line in ((capture/'debug.log').read_text(errors='replace').splitlines() if (capture/'debug.log').exists() else ()):
        if line.startswith('PATCH '):
            f=dict(t.split('=') for t in line.split()[1:]);d=[s16(int(f['d%d'%i],16)) for i in range(6)]
            sub=int(f['a1'],16);t,c,w,h,tiles,coll=map_blocks(rom,sub);pc={}
            if coll and int(f['a0'],16)==0x10c72e:
                for i in range(d[2]):
                    for j in range(d[3]):
                        si,sj=d[0]+i,d[1]+j
                        if not (0<=si<w and 0<=sj<h):continue
                        for k in range(4):pc[(2*(d[4]+i)+(k&1),2*(d[5]-j)+1-(k>>1))]=coll[(2*si+(k&1),2*sj+(k>>1))]
            patches.append(dict(frame=int(f['f']),object=f['a0'][-6:],submap='%06X'%sub,d=d,owner=f['a6'][-6:],blocks=[w,h],collision=bool(coll),cells=pc))
        elif line.startswith('STAMP '):
            f=dict(t.split('=') for t in line.split()[1:]);stamps.append(dict(frame=int(f['f']),owner='%06X'%int(f['a6'],16),template='%06X'%int(f['a1'],16),object='%06X'%int(f['a0'],16)))
        elif line.startswith('STAMP2 '):
            f=dict(t.split('=') for t in line.split()[1:]);stamps2.append(dict(frame=int(f['f']),owner='%06X'%int(f['a6'],16),routine=f['r'][-6:],template='%06X'%int(f['a1'],16),object='%06X'%int(f['a0'],16)))
    with open(capture/'events.csv') as f:
        f.readline();first=int(f.readline().split(',')[0])
    reads,kinds,static,events,writers=explain(capture,0x10c72e,base,patches,last,first)
    print('%06X 10C72E: %d reads %s; ring writes %s'%(descriptor,reads,dict(kinds),{'%06X'%k:v for k,v in sorted(writers.items())}))
    out=dict(descriptor='%06X'%descriptor,capture=capture.name,last=last,maps=['%06X'%l['map'] for l in loads if l['object']==0x10c72e and l['map']],reads=reads,kinds=dict(kinds),
             static=[[x,y,v] for (x,y),v in sorted(static.items())],stamps=stamps,stamps_0C8A3C=stamps2,stamp_writes=events,
             patches=[dict((k,v) for k,v in q.items() if k!='cells')|dict(cells_changed=sum(1 for k,v in q['cells'].items() if base.get(k,0)!=v)) for q in patches])
    if base2:
        reads,kinds,static,events,writers=explain(capture,0x10d7b6,base2,[],last,first,moved=True)
        assert not events
        print('%06X 10D7B6: %d reads %s; ring writes %s'%(descriptor,reads,dict(kinds),{'%06X'%k:v for k,v in sorted(writers.items())}))
        out['second']=dict(object='10D7B6',maps=['%06X'%l['map'] for l in loads if l['object']==0x10d7b6 and l['map']],reads=reads,kinds=dict(kinds),
                           distinct=len(static),not_empty=sum(1 for v in static.values() if v))
    return out
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('stages',nargs='+',help='CAPTURE:DESCRIPTOR:LAST:COLLISION per stage (LAST = last frame of the reads kept)')
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'));ap.add_argument('--output',type=Path,default=ROOT/'data/m6_map_fixture.json.gz')
    ap.add_argument('--source');a=ap.parse_args();rom=Rom(a.rom);out=[]
    for s in a.stages:
        c,d,l,g=s.split(':');out.append(stage(Path(c),rom,int(d,16),int(l),Path(g)))
    doc=dict(source=a.source or 'MAME 0.264 mslugx euro, tools/capture_m6.sh (states4 sec09_292683 / sec10_304271 / sec11_322339, replays of run4); tools/extract_m6_map_fixture.py',stages=out)
    with gzip.open(a.output,'wt') as f:json.dump(doc,f,separators=(',',':'))
    print('%s static reads (deduplicated) -> %s'%(' + '.join(str(len(s['static'])) for s in out),a.output))
if __name__=='__main__':main()
