#!/usr/bin/env python3
"""Mission 3 map reconnaissance: the arcade's collision queries against the
port's data (docs/MISION3.md, "Reconocimiento del mapa").

Input: a capture of tools/capture_fullgame.lua from states4/sec04_117966
(the replay equals run4 frame for frame) with
  MSX_FULL_CELL_TAP=117966-149300  cells.txt: every cell 0C79FE read (PC
                                   0C7A18) from the rings 10C72E/10D7B6 +88
  MSX_FULL_DEBUG=1 MSX_FULL_NO_PROBES=1 MSX_FULL_DEBUG_CMDS=...
                                   debug.log: player 1's sweeps (01448C in,
                                   014492 out, A6 = 1014C4) with the scene,
                                   the dynamic objects and every 0C70FC call
(the command lines are in the doc), plus trace.csv/layers.csv/platforms.csv.

Output:
  data/m3_map_fixture.json.gz  (tools/compile_m3_map_fixture.py ->
    tools/test_m3_map.c, make check): the sections, the deduplicated cell
    reads of the terrain ring (column already resolved through the ring
    window) and of the train ring, the 0C70FC patches with their cells.
  --sweeps-output (private, make check-m3sweeps): the deduplicated player
    sweeps with the dynamic maps they met (ROM bytes).
  --report: per-section counts (filesystem/mission3/s1/recon.json).

Ring models checked here (every read must be explained, else the tool
stops):
  terrain 10C72E: slot (x>>3)&63 holds column first + ((c - first) & 63),
    first = (-64 - X) >> 3, X = the layer's integer +14 of that frame
    (trace.csv fore_x); a column past the map (860..) keeps column c - 64;
    row slot 63 - (r & 63) holds kernel row r = y >> 3 (grid row r - origin).
  train 10D7B6: slot q = ((~y) & 0x1F8) >> 3 holds map row (q - 24) & 63
    (+34 = 19 all mission), column ((x - 16 * (+32)) >> 3) mod 784; at the
    screen's left edge the ring already holds the column 64 ahead."""
import argparse,gzip,json,re,struct,sys
from collections import Counter,defaultdict
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from decode_level_script import Program
from fill_collision_rom import rom_grid
ROOT=Path(__file__).resolve().parents[1]
# Sections: camera-list command pointers (descriptor 016E0E, list 0B8968)
# whose passing starts the next section.
SECTIONS=[('1','salida (progreso 0..0x140)',0xB8A50,'0B853A'),
          ('2','vagones (0x140..0xD40)',0xB8BB8,'0B853A'),
          ('3','hueco 0B8600 (0xD40..0xE80)',0xB8C1C,'0B8600'),
          ('4','tramo 0B8550 (0xE80..0x18A0)',0xB8CAC,'0B8550'),
          ('5','jefe (0x18A0..fin)',None,'0B8550')]
TRAIN_SLOT_ROW=24
def s16(v):return v-65536 if v&0x8000 else v
def s32(v):return v-(1<<32) if v&0x80000000 else v
def load_pack_grid(path):
    pack=path.read_bytes();assert pack[:4]==b'MSXM'
    st=struct.unpack_from('>IHHIIIHHhhIIII',pack,48 if struct.unpack_from('>H',pack,4)[0]>=4 else 40)   # pack version 4: 48-byte header
    C,R,ox,oy,go=st[6],st[7],st[8],st[9],st[10]
    return pack[go:go+C*R],C,R,ox,oy
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('capture',type=Path)
    ap.add_argument('--collision',type=Path,default=ROOT/'filesystem/mission3/s1/collision.json',help='stage grid (origin_cells applied)')
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    ap.add_argument('--output',type=Path,default=ROOT/'data/m3_map_fixture.json.gz')
    ap.add_argument('--sweeps-output',type=Path,default=ROOT/'filesystem/mission3/m3_sweeps.json.gz',help='the player sweeps (private: make check-m3sweeps)')
    ap.add_argument('--report',type=Path)
    ap.add_argument('--dynamic-output',type=Path,help='filesystem/mission3/s1/dynamic.json (private)')
    a=ap.parse_args();cap=a.capture
    prog=Program(a.rom)
    col=json.loads(a.collision.read_text())
    grid=bytes.fromhex(col['cells_hex']);C,R=col['columns'],col['rows'];ox,oy=col['origin_cells']
    assert ox==0
    # Frames: camera-list pointer and layer x (trace.csv), train origin +32.
    cmd={};X={}
    for row in open(cap/'trace.csv'):
        t=row.split(',')
        if not t[0].isdigit():continue
        f=int(t[0]);cmd[f]=int(t[21],16);X[f]=int(t[3])
    first_frame,last_frame=min(cmd),max(cmd)
    bounds=[];k=0;start=first_frame
    for f in range(first_frame,last_frame+1):
        if k<len(SECTIONS)-1 and cmd.get(f,0)>SECTIONS[k][2]:
            bounds.append((start,f-1));start=f;k+=1
    bounds.append((start,last_frame));assert len(bounds)==len(SECTIONS)
    def section(f):
        for i,(lo,hi) in enumerate(bounds):
            if lo<=f<=hi:return i
        return 0 if f<first_frame else len(bounds)-1
    cam={i:(min(-X[f] for f in range(lo,hi+1) if f in X),max(-X[f] for f in range(lo,hi+1) if f in X)) for i,(lo,hi) in enumerate(bounds)}
    origin={};train_y={}
    for row in open(cap/'layers.csv'):
        t=row.rstrip().split(',')
        if t[1]=='10d7b6':
            b=bytes.fromhex(t[2]);origin[int(t[0])]=s16(int.from_bytes(b[0x32:0x34],'big'));train_y[int(t[0])]=int.from_bytes(b[0x34:0x36],'big')
    assert {v for f,v in train_y.items() if f>=first_frame+4}=={19},'train ring window +34 changed'
    # 0C70FC patches (the wagon ledges of 0B8FA8, sub-map 0B85EA).
    patches=[]
    for line in open(cap/'debug.log'):
        m=re.match(r'PATCH f=(\d+) a0=(\w+) a1=(\w+) d0=(\w+) d1=(\w+) d2=(\w+) d3=(\w+) d4=(\w+) d5=(\w+) a6=(\w+)',line)
        if not m:continue
        f,obj,sub=int(m.group(1)),int(m.group(2),16),int(m.group(3),16);d=[int(m.group(i),16) for i in range(4,10)]
        assert obj==0x10c72e,hex(obj)
        cp,w,h=struct.unpack('>IHH',prog.read(sub+4,8))
        cells={}
        for i in range(d[2]):
            for j in range(d[3]):
                if d[0]+i>=w or d[1]+j>=h:continue
                o=cp-0x100000+((d[0]+i)*h+d[1]+j)*4;blk=prog.p2[o:o+4]
                for q in range(4):cells[(2*(d[4]+i)+(q&1),2*d[5]+1-2*j-(q>>1))]=blk[q]
        patches.append(dict(frame=f,submap='%06X'%sub,d=d,actor='%06X'%int(m.group(10),16),cells=sorted([c,r,v] for (c,r),v in cells.items())))
    pcells=[(p['frame'],{(c,r):v for c,r,v in p['cells']}) for p in patches]
    def static(c,r):
        if C<=c<C+64:c-=64       # past the map end: the slot keeps column c - 64
        g=r-oy
        return grid[c*R+g] if 0<=c<C and 0<=g<R else 0
    def expected(c,r,f,same):
        v=static(c,r)
        for pf,cs in pcells:
            if (pf<f or (same and pf==f)) and (c,r) in cs:v=cs[(c,r)]
        return v
    # Terrain reads.
    terrain={};train={};stats=defaultdict(Counter)
    tg,tgc,tgr=rom_grid(prog,0xb8566);assert (tgc,tgr)==(784,48)
    for line in open(cap/'cells.txt'):
        _,obj,x,y,v,f0,f1,n,pl=line.split();x,y,v,f0,f1,pl=s16(int(x)),s16(int(y)),int(v),int(f0),int(f1),int(pl)
        s=section(f0)
        if obj=='10c72e':
            c,r=x>>3,y>>3
            for f in (f0,f1):
                first=(-64-X[f])>>3;lc=first+((c-first)&63)
                assert any(expected(lc,r,f,same)==v for same in (False,True)),('terrain read not explained',x,y,v,f)
                k=(lc,r,v);e=terrain.get(k)
                terrain[k]=[min(e[0],f),max(e[1],f),e[2] or pl>0] if e else [f,f,pl>0]
            stats[s]['terrain_reads']+=1;stats[s]['terrain_player_reads']+=pl>0;stats[s]['terrain_window_alias']+=lc!=c
        else:
            assert obj=='10d7b6',obj
            q=((~y)&0x1f8)>>3;row=(q-TRAIN_SLOT_ROW)&63
            ok=False
            for f in (f0,f0+1,f0-1):
                o=origin.get(f)
                if o is None:continue
                for ahead in (0,64):
                    cc=(((x-16*o)>>3)+ahead)%784
                    if (tg[cc*48+row] if row<48 else 0)==v:ok=True;key=(cc,q,v);aliased=ahead;break
                if ok:break
            assert ok,('train read not explained',x,y,v,f0)
            e=train.get(key);train[key]=[min(e[0],f0),max(e[1],f1)] if e else [f0,f1]
            stats[s]['train_reads']+=1;stats[s]['train_window_ahead']+=aliased>0
            # The kernel before this reconnaissance read map row q - 20.
            oq=q-20;oc=((x-16*origin.get(f0,0))>>3)%784
            stats[s]['train_q20_ok']+=(tg[oc*48+oq] if 0<=oq<48 else 0)==v
    # Sweeps.
    IN=re.compile(r'IN f=(\d+) d0=(\w+) d1=(\w+) d2=(\w+) d3=(\w+) a0=(\w+) m4004=(\w+) b4008=(\w+) b4009=(\w+) b400a=(\w+) b400b=(\w+) b400c=(\w+) b400d=(\w+) w4068=(\w+) w4018=(\w+)')
    OUT=re.compile(r'OUT f=(\d+) d0=(\w+) d1=(\w+) d2=(\w+) d3=(\w+) a0=(\w+) d4=(\w+) d6=(\w+) d7=(\w+)')
    HEX=re.compile(r'[0-9A-F]{4,8}')
    sweeps={};pend=None;maps=set()
    lines=open(cap/'debug.log').read().split('\n')
    i=0
    while i<len(lines):
        m=IN.match(lines[i])
        if m:
            extra=' '.join(lines[i+1:i+6]);i+=6
            vals=[int(v,16) for v in m.groups()[1:]]
            box=[int(v,16) for v in re.search(r'box=(\w+),(\w+),(\w+),(\w+)',extra).groups()]
            shake=int(re.search(r'shake=(\w+)',extra).group(1),16)
            scene=[]
            for tag in ('t','r','m'):scene+= [int(v,16) for v in re.search(r' %s=(\w+),(\w+),(\w+),(\w+)'%tag,extra).groups()]
            o=int(re.search(r' o=(\w+)',extra).group(1),16);g=int(re.search(r' g=(\w+)',extra).group(1),16);mm=int(re.search(r' mm=(\w+)',extra).group(1),16)
            dyn=[[int(v,16) for v in re.search(r's%d=(\w+),(\w+),(\w+),(\w+),(\w+)'%k,extra).groups()] for k in range(8)]
            assert g==0,g   # no cell group table in Mission 3 (10C00E = 0)
            pend=(int(m.group(1)),vals,shake,scene,o,mm,dyn,box);continue
        m=OUT.match(lines[i]);i+=1
        if not m or not pend:continue
        f=pend[0];assert int(m.group(1)) in (f,f+1)   # the frame may end between the call and its return
        out=[int(v,16) for v in m.groups()[1:]]
        dyn=pend[6]
        for d in dyn:
            if d[0]:maps.add(d[0])
        key=json.dumps([pend[1],pend[2],pend[3],pend[4],pend[5],dyn,out,pend[7]])
        if key in sweeps:sweeps[key][1]+=1
        else:sweeps[key]=[f,1]
        pend=None
    sw=[]
    for key,(f,n) in sorted(sweeps.items(),key=lambda t:t[1][0]):
        vals,shake,scene,o,mm,dyn,out,box=json.loads(key)
        sw.append(dict(frame=f,count=n,section=section(f),**{'in':vals},box=box,shake=shake,scene=scene,origin=o,train_map=mm,dyn=dyn,out=out))
        stats[section(f)]['sweeps']+=n;stats[section(f)]['sweeps_unique']+=1
        stats[section(f)]['sweeps_on_dynamic']+=n if out[4] not in (0x10c72e,0x10e968,0x10d7b6) else 0
    dmaps={}
    for p in sorted(maps):
        w,h=prog.u8(p+2),prog.u8(p+3);dmaps['%06X'%p]=prog.read(p,10+w*h).hex()
    fixture=dict(source='MAME 0.264 capture_fullgame.lua from sec04_117966 (= run4), docs/MISION3.md "Reconocimiento del mapa"',
        sections=[dict(id=SECTIONS[k][0],name=SECTIONS[k][1],map=SECTIONS[k][3],frames=list(bounds[k]),camera=list(cam[k])) for k in range(len(SECTIONS))],
        train_slot_row=TRAIN_SLOT_ROW,
        terrain=[[c,r,v,f0,f1,int(p)] for (c,r,v),(f0,f1,p) in sorted(terrain.items())],
        train=[[c,q,v,f0,f1] for (c,q,v),(f0,f1) in sorted(train.items())],
        patches=patches,dynamic_maps=dmaps,sweeps=sw)
    a.output.parent.mkdir(parents=True,exist_ok=True)
    sweeps_only=dict(source=fixture['source'],sections=fixture['sections'],dynamic_maps=fixture.pop('dynamic_maps'),sweeps=fixture.pop('sweeps'))
    def write_gz(path,obj):   # reproducible bytes (no time stamp)
        with open(path,'wb') as raw,gzip.GzipFile(fileobj=raw,mode='wb',mtime=0) as o:o.write(json.dumps(obj,separators=(',',':')).encode())
    write_gz(a.output,fixture)
    a.sweeps_output.parent.mkdir(parents=True,exist_ok=True)
    write_gz(a.sweeps_output,sweeps_only)
    report=dict(sections=[dict(id=SECTIONS[k][0],name=SECTIONS[k][1],map=SECTIONS[k][3],frames=list(bounds[k]),camera=list(cam[k]),**stats[k]) for k in range(len(SECTIONS))],
        terrain_checks=len(terrain),train_checks=len(train),patches=len(patches),dynamic_maps=sorted(dmaps),sweeps=sum(s['count'] for s in sw),sweeps_unique=len(sw))
    if a.report:a.report.write_text(json.dumps(report,indent=1)+'\n')
    if a.dynamic_output:
        # Dynamic collision of the run: the train layer's motion (x, origin
        # +32 per frame), the dynamic objects (RAM 10C06E + 64 i: map +26,
        # owner +10, position +14/+18 on every change, platforms.csv) with
        # their ROM maps, and the 0C70FC patches.
        tx=[];last=None
        for row in open(cap/'layers.csv'):
            t=row.rstrip().split(',')
            if t[1]!='10d7b6':continue
            b=bytes.fromhex(t[2]);v=(s32(int.from_bytes(b[0x14:0x18],'big')),s16(int.from_bytes(b[0x32:0x34],'big')))
            if v!=last:tx.append([int(t[0]),*v]);last=v
        objs={};dmaps={}
        for row in open(cap/'platforms.csv'):
            t=row.rstrip().split(',')
            if not t[0].isdigit():continue
            b=bytes.fromhex(t[3]);m=int.from_bytes(b[0x26:0x2a],'big')
            if not m:continue
            k=(int(t[2]),m);o=objs.setdefault(k,dict(slot=int(t[2]),map='%06X'%m,owner='%06X'%int.from_bytes(b[0x10:0x14],'big'),track=[]))
            o['track'].append([int(t[0]),s32(int.from_bytes(b[0x14:0x18],'big')),s32(int.from_bytes(b[0x18:0x1c],'big'))])
            if m not in dmaps:w,h=prog.u8(m+2),prog.u8(m+3);dmaps['%06X'%m]=prog.read(m,10+w*h).hex()
        dyn=dict(source=fixture['source'],
            train=dict(layer_object='10D7B6',map='0B8566',ring_slot_row=TRAIN_SLOT_ROW,columns=784,rows=48,
                       motion='[frame, +14 (Q16), +32 column origin] on change; native in src/neo_train.c (test_neo_train)',track=tx),
            dynamic_objects=sorted(objs.values(),key=lambda o:o['track'][0][0]),dynamic_maps=dmaps,
            patches=patches)
        a.dynamic_output.write_text(json.dumps(dyn,separators=(',',':'))+'\n')
    print(json.dumps(report,indent=1))
if __name__=='__main__':main()
