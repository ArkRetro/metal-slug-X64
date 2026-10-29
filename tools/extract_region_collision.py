#!/usr/bin/env python3
"""Export static collision for a longer region, separating event-made cells.

A cell may change only when the complete 4096-byte RAM grid change between two
snapshots of one run is reproduced by a recovered event model, the destroyed
ground vehicle stamp (tools/vehicle_wreckage.py), or when a run carries a
per-write trace (events.csv of tools/capture_mission_rest.lua) in which every
write of the original stamp routine (ROM 0C88B0, write PC 0C897E reported as
0C8982) is reproduced exactly, in order, by one ROM template group of
TEMPLATE_EVENTS at a unique anchor (tools/collision_stamp.py).
Cells that an event writes keep their pre-event value in the static grid; the
runtime recreates them from the event, not from this table. Any other change,
an ambiguous anchor, or a change outside the traced writes still fails like
extract_collision.py.
"""
import argparse,csv,json,struct,zipfile
from collections import defaultdict
from pathlib import Path
from extract_collision import FOREGROUND,MIDDLE,collision_layer,observed_cells
from vehicle_wreckage import stamp_destroyed_vehicle
from collision_stamp import stamp,template

STAMP_PC=0xc8982  # Address after MOVE.B D7,(A3) at 0C897E, as MAME reports it.
# Template groups called back to back by one actor (ROM addresses of the
# routine and of the templates, fixed program block).
TEMPLATE_EVENTS={
    'building_raise':dict(routine=0x0b680e,templates=(0x0b69f8,0x0b6a2c)),
    'building_clear':dict(routine=0x0b685a,templates=(0x0b6a12,0x0b6a42)),
}

def program_rom(zip_path):
    """Fixed program block (P1, byte-swapped to 68000 order) of the user's set."""
    with zipfile.ZipFile(zip_path) as z:raw=z.read('250-p1.p1')
    out=bytearray(raw);out[0::2]=raw[1::2];out[1::2]=raw[0::2];return bytes(out)

def traced_stamps(root,program,frames=None):
    """Explain each frame's stamp writes in root/events.csv by a template group.

    The layer origin comes from root/layers.csv at that frame; the cells' old
    values come from the trace itself. Returns [(frame, event)] in order."""
    path=root/'events.csv'
    if not path.exists():return []
    assert program is not None,'events.csv stamp writes need --rom to read the templates'
    groups=defaultdict(list)
    for row in csv.DictReader(path.read_text().splitlines()):
        if row['kind']=='grid' and int(row['pc'],16)==STAMP_PC:
            f=int(row['frame'])
            if frames and not frames[0]<=f<=frames[1]:continue
            groups[f].append((int(row['address'],16),int(row['old']),int(row['new'])))
    headers={}
    if groups:
        for row in csv.DictReader((root/'layers.csv').read_text().splitlines()):
            f=int(row['frame'])
            if f in groups:headers[f,int(row['object'],16)]=bytes.fromhex(row['header'])
    events=[]
    for frame,writes in sorted(groups.items()):
        addresses=[a for a,_,_ in writes]
        layer=next(l for l in (FOREGROUND+0x100000,MIDDLE+0x100000) if all(l+0x88<=a<l+0x88+4096 for a in addresses))
        ram=bytearray(65536);base=layer-0x100000
        ram[base:base+0x88]=headers[frame,layer]
        first={}
        for a,old,new in writes:first.setdefault(a,old)
        for a,old in first.items():ram[a-0x100000]=old
        matches=[]
        for name,spec in TEMPLATE_EVENTS.items():
            temps=[template(program,t) for t in spec['templates']]
            for cell_x in range(512):
                for cell_y in range(-32,64):
                    out=bytes(ram);produced=[]
                    for t in temps:
                        out,w=stamp(out,layer,t,cell_x*8,cell_y*8);produced+=w
                    if [(w['address'],w['old'],w['new']) for w in produced]==writes:
                        matches.append(dict(kind=name,anchor_cell=[cell_x,cell_y],layer=f'0x{layer:06X}',
                            templates=[f'0x{t:06X}' for t in spec['templates']],routine=f'0x{spec["routine"]:06X}',writes=produced))
        assert len(matches)==1,('Unexplained or ambiguous traced stamp',str(root),frame,len(matches))
        events.append((frame,matches[0]))
    return events

GRID=slice(0xc7b6,0xc7b6+4096)

def snapshots(root):
    paths=sorted(root.glob('ram*.bin'),key=lambda p:int(p.stem[3:]))
    assert paths,f'No RAM snapshots in {root}'
    return [(int(p.stem[3:]),p.read_bytes()) for p in paths]

def explain(before,after):
    """Return the unique vehicle-wreck stamp turning `before` into `after`."""
    # The recovered stamp model writes the foreground ring; later events are not modelled.
    assert collision_layer(before)==collision_layer(after)==FOREGROUND,'Event outside the foreground collision grid'
    assert before[0xc72e+0x14:0xc72e+0x1c]==after[0xc72e+0x14:0xc72e+0x1c],'Grid origin moved during the event'
    matches=[]
    for cell_x in range(512):
        for cell_y in range(32):
            for flags in (1,3):
                result,writes=stamp_destroyed_vehicle(before,cell_x<<19,cell_y<<19,0,flags)
                if writes and result[GRID]==after[GRID]:
                    matches.append(dict(anchor_cell=[cell_x,cell_y],flags=flags,writes=writes))
    assert len(matches)==1,('Unexplained or ambiguous dynamic collision',len(matches))
    return matches[0]

def extract(roots,program=None,frames=None,layer_camera=False):
    values=defaultdict(dict);events=[];count=0;preloaded=[]
    for root in roots:
        shots=snapshots(root)
        if frames:shots=[(f,r) for f,r in shots if frames[0]<=f<=frames[1]]
        count+=len(shots);traced=traced_stamps(root,program,frames)
        seen={}  # per run: cell -> (frame, code) of the latest observation
        stamped={}  # cells written by an explained event in this run
        for index,(frame,ram) in enumerate(shots):
            # Traced template stamps since the previous snapshot of this run.
            previous=shots[index-1][0] if index else -1
            for event_frame,event in [e for e in traced if previous<e[0]<=frame]:
                assert index,('Traced stamp before the first snapshot',str(root),event_frame)
                for w in event['writes']:
                    cell=(w['world_x']//8,w['world_y']//8)
                    if cell in stamped:
                        assert stamped[cell]['new']==w['old'],('Traced stamp over an unexplained value',cell)
                        stamped[cell]=dict(w,old=stamped[cell]['old'])
                    else:
                        if cell in seen:assert seen[cell][1]==w['old'],('Traced old value differs from the observation',cell)
                        else:preloaded.append((cell,shots[index-1][1]))  # Verified below like the vehicle cells.
                        stamped[cell]=dict(w)
                events.append(dict(kind=event['kind'],run=str(root),frame=event_frame,anchor_cell=event['anchor_cell'],
                    layer=event['layer'],templates=event['templates'],routine=event['routine'],
                    cells=[dict(x=w['world_x'],y=w['world_y'],old=w['old'],new=w['new']) for w in event['writes']]))
            cells=[(x,y,code) for camera,x,y,code,address in observed_cells(ram,layer_camera)]
            changed=[(x,y) for x,y,code in cells if (x,y) not in stamped and (x,y) in seen and seen[x,y][1]!=code]
            if changed:
                # The change must be visible between consecutive snapshots of one run.
                assert all(seen[c][0]==shots[index-1][0] for c in changed),('Change between unobserved frames',root,frame)
                event=explain(shots[index-1][1],ram)
                for w in event['writes']:
                    cell=(w['world_x']//8,w['world_y']//8)
                    assert cell not in stamped,('Repeated event',cell)
                    stamped[cell]=w
                    if cell not in seen:preloaded.append((cell,shots[index-1][1]))  # Foreground ring (asserted in explain).
                assert all(c in stamped for c in changed),('Change outside the explained event',root,frame)
                events.append(dict(kind='vehicle_wreckage',run=str(root),
                    before_frame=shots[index-1][0],after_frame=frame,
                    anchor_cell=event['anchor_cell'],flags=event['flags'],
                    cells=[dict(x=w['world_x'],y=w['world_y'],static=w['old'],event=w['new']) for w in event['writes']]))
            for x,y,code in cells:
                if (x,y) in stamped:
                    assert code==stamped[x,y]['new'],('Wreck cell changed again',root,frame,(x,y))
                    continue
                seen[x,y]=(frame,code)
                values[x,y].setdefault(code,[]).append((str(root),frame))
        for cell,w in stamped.items():
            # The pre-event byte comes from the verified RAM grid, observed or
            # preloaded ahead of the viewport. It must agree with every run.
            values[cell].setdefault(w['old'],[]).append((str(root),'pre-event'))
    width=max(x for x,y in values)+1
    for x in range(width):
        for y in range(32):
            assert (x,y) in values,('Unobserved collision cell',x*8,y*8)
            assert len(values[x,y])==1,('Dynamic collision cell',x*8,y*8,sorted(values[x,y]))
    data=bytes(next(iter(values[x,y])) for x in range(width) for y in range(32))
    for (x,y),before in preloaded:
        # A pre-event byte read ahead of the viewport is accepted only when the
        # rest of that preloaded ring column equals its later observed terrain.
        grid=collision_layer(before)+0x88
        ring=[before[grid+(x&63)*64+(~row&63)] for row in range(32)]
        assert all(ring[row]==data[x*32+row] for row in range(32) if (x,row) not in {c for c,_ in preloaded}),('Unverified preloaded column',x*8)
    return dict(source='MAME mslugx euro; active collision grid (0x10C72E or, at full-speed middle bank, 0x10D7B6) + 0x88, observed viewports; explained vehicle-wreck and traced template-stamp cells keep their pre-event value',
                columns=width,rows=32,frames=count,cells_hex=data.hex(),events=events,
                runtime_policy='Event cells are static pre-event terrain; neo_collision_vehicle_wreckage() recreates them when the vehicle is destroyed.')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path,nargs='+')
    p.add_argument('--output',type=Path,default=Path('filesystem/mission1/collision.json'))
    p.add_argument('--expect-prefix',type=Path,help='Earlier collision.json that must match the leading columns exactly')
    p.add_argument('--rom',type=Path,help='MAME mslugx.zip: templates for traced stamps (events.csv)')
    p.add_argument('--frames',type=lambda v:tuple(int(x) for x in v.split(':')),help='LO:HI inclusive snapshot frames')
    p.add_argument('--layer-camera',action='store_true',help='Viewport from the active layer position (camera stops with bounds)')
    a=p.parse_args();report=extract(a.captures,program_rom(a.rom) if a.rom else None,a.frames,a.layer_camera);data=bytes.fromhex(report['cells_hex'])
    if a.expect_prefix:
        old=bytes.fromhex(json.loads(a.expect_prefix.read_text())['cells_hex'])
        assert data[:len(old)]==old,'Region extension changed previously validated collision cells'
        report['prefix_columns_verified']=len(old)//32
    a.output.parent.mkdir(parents=True,exist_ok=True)
    a.output.write_text(json.dumps(report,separators=(',',':'))+'\n')
    print('Region collision grid:',report['columns'],'x32;',sum(bool(c) for c in data),'nonempty cells;',
          report['frames'],'snapshots;',sum(len(e['cells']) for e in report['events']),'event cells kept as pre-event terrain')
