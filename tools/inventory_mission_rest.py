#!/usr/bin/env python3
"""Inventory of everything the Mission 1 script spawns from a camera point to
the mission end, from a debugger capture of tools/capture_mission_rest.lua
(MSX_REST_DEBUG=1: SPAWN probe at ROM 01A132, DRAW probe at ROM 004406).

Every spawn is the executed op 7 record (exact, from the probe), its
constructor (spawn table ROM 0197DC), the actors it created afterwards
(creator chain +0C over the list walk), the sprite descriptors those actors
drew and the palette slots/resources (allocator table RAM 1068EA, entry
index = palette slot, +A = resource id). Nothing is inferred from names or
appearance; labels in the documentation are visual identifications."""
import argparse,bisect,collections,csv,json,re,struct,zipfile
from pathlib import Path
from decode_level_script import Program,decode,walk

DEAD=0x3F1E
LINE=re.compile(r'(\w+)=([0-9A-Fa-f]+)')

def rom(zip_path):
    """Physical program space: P1 at 0, P2 at 0x100000 (68000 byte order)."""
    with zipfile.ZipFile(zip_path) as z:p1,p2=z.read('250-p1.p1'),z.read('250-p2.ep1')
    def swap(b):
        o=bytearray(len(b));o[0::2]=b[1::2];o[1::2]=b[0::2];return bytes(o)
    return swap(p1)+swap(p2)

def descriptor(data,address):
    """[groups][attr], then per group x,y (s16), cols,rows and tile words
    (format of tools/export_soldier_sprites.py)."""
    at=address;groups,attr=data[at],data[at+1];at+=2;tiles=set();x0=y0=10**9;x1=y1=-10**9
    if groups==0 or groups>64:return None
    for _ in range(groups):
        x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6
        if cols*rows>512:return None
        for i in range(cols*rows):tiles.add(struct.unpack_from('>H',data,at)[0]|(attr&0xf0)<<12);at+=2
        x0=min(x0,x);x1=max(x1,x+cols*16);y0=min(y0,-y);y1=max(y1,-y+rows*16)
    return dict(groups=groups,attr=attr,tiles=len(tiles),tile_codes=sorted(tiles),box=[x0,y0,x1-x0,y1-y0],auto_animation=bool(attr&0xc))

def instances(actors_csv,splits=()):
    """Actor instances (node, birth) from the list-walk log: a birth, a
    routine change together with a new creator, or a known allocation
    (`splits`: (node, frame) of the SPAWN probe) starts an instance."""
    out=[];current={};splits=set(splits)
    for r in csv.DictReader(open(actors_csv)):
        f=int(r['frame']);n=int(r['node'],16);rt=int(r['routine'],16);cr=int(r['creator'],16)
        inst=current.get(n)
        if r['event']=='death':
            if inst:inst['death']=f;del current[n]
            continue
        if inst is None or r['event']=='birth' or inst['creator']!=cr or ((n,f) in splits and inst['birth']!=f):
            if inst:inst['death']=f
            inst=dict(node=n,birth=f,death=None,creator=cr,routines=[],first_xy=[int(r['x']),int(r['y'])])
            current[n]=inst;out.append(inst)
        if rt not in inst['routines']:inst['routines'].append(rt)
    return out

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('zip',type=Path);p.add_argument('capture',type=Path,help='debugger capture directory')
    p.add_argument('--from-record',type=lambda v:int(v,0),default=0x2D6A1A,help='first script statement (default: camera stop 1920)')
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--all-sections',action='store_true',help='Spawn records of every script of the table ROM 0x000800 (full game)')
    p.add_argument('--frames',type=lambda v:tuple(int(x) for x in v.split(':')),help='LO:HI spawn frames (one mission of a full-game capture)')
    a=p.parse_args()
    prog=Program(a.zip);data=rom(a.zip)
    if a.all_sections:
        from fullgame_scripts import table
        script=[st for b in table(prog) if 0x200000<=b<0x300000 for st in decode(prog,b)]
    else:script=decode(prog)
    records={int(s['address'],16):s for s in walk(script) if s['op']==7}
    log=(a.capture/'debug.log').read_text().splitlines()
    assert any('MAME debugger' in l for l in log[:3]) and not any('Error in' in l for l in log),'debug.log missing or has command errors'
    spawns=[];draws=collections.defaultdict(list)
    for l in log:
        if l.startswith('SPAWN '):
            v={k:int(x,16) for k,x in LINE.findall(l)};v['frame']=int(re.search(r'frame=(\d+)',l).group(1));spawns.append(v)
        elif l.startswith('DRAW '):
            v={k:int(x,16) for k,x in LINE.findall(l)};f=int(re.search(r'frame=(\d+)',l).group(1))
            draws[v['a6']].append((f,v['routine'],v['desc'],v['attr']))
    trace={int(r['frame']):r for r in csv.DictReader(open(a.capture/'trace.csv'))}
    insts=instances(a.capture/'actors.csv',{(s['node'],s['frame']+1) for s in spawns})
    by_node=collections.defaultdict(list)
    for i in insts:by_node[i['node']].append(i)
    def alive(node,frame):
        for i in reversed(by_node.get(node,[])):
            if i['birth']<=frame and (i['death'] is None or frame<i['death']):return i
        return None
    # Family = the spawned instance and every instance created (transitively)
    # by a family member while that member was alive.
    family={}
    roots=[]
    for s in spawns:
        if s['record'] not in records:continue
        # temp9 holds the last completed frame: the spawn runs during frame+1
        # and the list walk at the end of that frame logs the birth.
        inst=alive(s['node'],s['frame']+1)
        if inst is not None and inst['birth']!=s['frame']+1:inst=None
        # None: created and removed inside the same update (never in the walk).
        if inst is not None:family[id(inst)]=len(roots)
        roots.append(dict(spawn=s,inst=inst))
    for i in sorted(insts,key=lambda i:i['birth']):
        if id(i) in family:continue
        parent=alive(i['creator'],i['birth'])
        if parent is not None and id(parent) in family:family[id(i)]=family[id(parent)]
    members=collections.defaultdict(list)
    for i in insts:
        if id(i) in family:members[family[id(i)]].append(i)
    # Palette resources per slot from the RAM dumps (nearest dump at or before).
    dumps=sorted(int(p.stem[3:]) for p in a.capture.glob('ram*.bin'))
    cache={}
    def resource(frame,slot):
        k=bisect.bisect_right(dumps,frame)-1
        if k<0:return None
        f=dumps[k]
        if f not in cache:
            ram=(a.capture/f'ram{f}.bin').read_bytes()
            cache[f]={i:struct.unpack_from('>H',ram,0x68EA+16*i+10)[0] for i in range(256)}
        r=cache[f].get(slot);return None if r in (0,0xFFFF) else r
    kinds=collections.OrderedDict();desc_cache={}
    start=next(s for s in spawns if s['record']>=a.from_record and (not a.frames or a.frames[0]<=s['frame']<=a.frames[1]))['frame']
    for k,root in enumerate(roots):
        s=root['spawn']
        if s['record']<a.from_record:continue
        if a.frames and not a.frames[0]<=s['frame']<=a.frames[1]:continue
        rec=records[s['record']];entry=s['entry']&0xFFFFFF
        kind=kinds.setdefault(entry,dict(constructor=f'0x{entry:06X}',table_offset=rec['table_offset'],group_counted=not s['entry']>>31,
            spawns=[],transient=0,routines=set(),descriptors=collections.Counter(),palettes=collections.Counter(),members=0,undrawn_members=0))
        row=trace.get(s['frame'],{})
        kind['spawns'].append(dict(frame=s['frame'],camera=int(row.get('camera_word',row.get('camera',-1))),record=f'0x{s["record"]:06X}',
            node=f'0x{s["node"]:06X}',creator=f'0x{s["creator"]:06X}',x=rec['x'],y=rec['y'],screen_relative=[rec['x_screen_relative'],rec['y_screen_relative']],
            group=rec['group'],words=rec['words'],world_xy=root['inst']['first_xy'] if root['inst'] else None))
        if root['inst'] is None:kind['transient']+=1
        for m in members[k]:
            kind['members']+=1;kind['routines'].update(m['routines'])
            ds=[d for d in draws.get(m['node'],[]) if d[0]>=m['birth'] and (m['death'] is None or d[0]<m['death'])]
            if not ds:kind['undrawn_members']+=1
            for f,rt,desc,attr in ds:
                kind['descriptors'][desc]+=1
                kind['palettes'][(attr>>8,resource(f,attr>>8))]+=1
    out=[]
    for entry,kd in kinds.items():
        descs=[]
        for d,count in kd['descriptors'].most_common():
            if d not in desc_cache:desc_cache[d]=descriptor(data,d) if d<len(data) else None
            info=desc_cache[d]
            descs.append(dict(address=f'0x{d:06X}',draws=count,**({k:v for k,v in info.items() if k!='tile_codes'} if info else dict(decoded=False))))
        tiles=set()
        for d in kd['descriptors']:
            if desc_cache.get(d):tiles.update(desc_cache[d]['tile_codes'])
        out.append(dict(constructor=kd['constructor'],table_offset=kd['table_offset'],group_counted=kd['group_counted'],
            spawn_count=len(kd['spawns']),removed_in_same_update=kd['transient'],first=kd['spawns'][0],spawns=kd['spawns'],
            routines=[f'0x{r:06X}' for r in sorted(kd['routines'])],actor_instances=kd['members'],instances_without_004406_draws=kd['undrawn_members'],
            descriptors=descs,unique_tiles=len(tiles),
            palettes=[dict(slot=s,resource=(f'0x{r:04X}' if r is not None else None),draws=c) for (s,r),c in kd['palettes'].most_common()]))
    report=dict(source='MAME 0.264 mslugx euro, debugger playthrough of tools/capture_mission_rest.lua; SPAWN probe ROM 01A132, DRAW probe ROM 004406',
        capture=str(a.capture),from_record=f'0x{a.from_record:06X}',first_frame=start,
        spawn_records_executed=sum(k['spawn_count'] for k in out),kinds=out)
    a.output.write_text(json.dumps(report,indent=1)+'\n')
    for k in out:print(k['constructor'],k['spawn_count'],'first',k['first']['frame'],k['first']['camera'],'members',k['actor_instances'],'descs',len(k['descriptors']),'tiles',k['unique_tiles'],'pal',[(p['slot'],p['resource']) for p in k['palettes'][:3]])

if __name__=='__main__':main()
