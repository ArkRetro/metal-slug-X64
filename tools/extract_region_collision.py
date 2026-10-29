#!/usr/bin/env python3
"""Export static collision for a longer region, separating event-made cells.

A cell may change only when the complete 4096-byte RAM grid change between two
snapshots of one run is reproduced by a recovered event model. The only model
accepted here is the destroyed ground vehicle stamp (tools/vehicle_wreckage.py).
Cells that model writes keep their pre-event value in the static grid; the
runtime recreates the wreck from the vehicle event, not from this table. Any
other change, or an ambiguous anchor, still fails like extract_collision.py.
"""
import argparse,json,struct
from collections import defaultdict
from pathlib import Path
from extract_collision import FOREGROUND,collision_layer,observed_cells
from vehicle_wreckage import stamp_destroyed_vehicle

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

def extract(roots):
    values=defaultdict(dict);events=[];frames=0;preloaded=[]
    for root in roots:
        shots=snapshots(root);frames+=len(shots)
        seen={}  # per run: cell -> (frame, code) of the latest observation
        stamped={}  # cells written by an explained event in this run
        for index,(frame,ram) in enumerate(shots):
            cells=[(x,y,code) for camera,x,y,code,address in observed_cells(ram)]
            changed=[(x,y) for x,y,code in cells if (x,y) not in stamped and (x,y) in seen and seen[x,y][1]!=code]
            if changed:
                # The change must be visible between consecutive snapshots of one run.
                assert all(seen[c][0]==shots[index-1][0] for c in changed),('Change between unobserved frames',root,frame)
                event=explain(shots[index-1][1],ram)
                for w in event['writes']:
                    cell=(w['world_x']//8,w['world_y']//8)
                    assert cell not in stamped,('Repeated event',cell)
                    stamped[cell]=w
                    if cell not in seen:preloaded.append((cell,shots[index-1][1]))  # Foreground ring (asserted).
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
        ring=[before[0xc7b6+(x&63)*64+(~row&63)] for row in range(32)]
        assert all(ring[row]==data[x*32+row] for row in range(32) if (x,row) not in {c for c,_ in preloaded}),('Unverified preloaded column',x*8)
    return dict(source='MAME mslugx euro; active collision grid (0x10C72E or, at full-speed middle bank, 0x10D7B6) + 0x88, observed viewports; explained vehicle-wreck cells keep their pre-event value',
                columns=width,rows=32,frames=frames,cells_hex=data.hex(),events=events,
                runtime_policy='Event cells are static pre-event terrain; neo_collision_vehicle_wreckage() recreates them when the vehicle is destroyed.')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path,nargs='+')
    p.add_argument('--output',type=Path,default=Path('filesystem/mission1/collision.json'))
    p.add_argument('--expect-prefix',type=Path,help='Earlier collision.json that must match the leading columns exactly')
    a=p.parse_args();report=extract(a.captures);data=bytes.fromhex(report['cells_hex'])
    if a.expect_prefix:
        old=bytes.fromhex(json.loads(a.expect_prefix.read_text())['cells_hex'])
        assert data[:len(old)]==old,'Region extension changed previously validated collision cells'
        report['prefix_columns_verified']=len(old)//32
    a.output.parent.mkdir(parents=True,exist_ok=True)
    a.output.write_text(json.dumps(report,separators=(',',':'))+'\n')
    print('Region collision grid:',report['columns'],'x32;',sum(bool(c) for c in data),'nonempty cells;',
          report['frames'],'snapshots;',sum(len(e['cells']) for e in report['events']),'event cells kept as pre-event terrain')
