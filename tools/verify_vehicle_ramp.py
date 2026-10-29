#!/usr/bin/env python3
"""Verify destruction-triggered geometry against original CPU writes and RAM."""
import argparse
import csv
import json
from pathlib import Path
import struct
from vehicle_wreckage import stamp_destroyed_vehicle


def verify(root):
    events=list(csv.DictReader((root/'ramp-writes.csv').open()))
    candidates=[e for e in events if e['a4']=='29bf4' and e.get('object_health')=='0'
                and e.get('object_routine')=='294e4']
    assert candidates,'No observed zero-health ground vehicle destruction stamp'
    first=candidates[0];frame=int(first['frame']);object_address=int(first['a6'],16)
    event=[e for e in candidates if int(e['frame'])==frame and int(e['a6'],16)==object_address]
    snapshots=sorted((int(p.stem[3:]),p) for p in root.glob('ram*.bin'))
    before_frame,before_path=max((f,p) for f,p in snapshots if f<frame)
    after_frame,after_path=min((f,p) for f,p in snapshots if f>frame)
    before=before_path.read_bytes();after=after_path.read_bytes()
    assert before[0xc742:0xc74a]==after[0xc742:0xc74a], 'Need closer captures: camera moved across verification window'
    x16,y16=int(first['object_x16']),int(first['object_y16'])
    flags=int(first['object_flags'])
    predicted,writes=stamp_destroyed_vehicle(before,x16,y16,0,flags)
    expected=[(int(e['address'],16),int(e['old']),int(e['new'])) for e in event]
    actual=[(e['address'],e['old'],e['new']) for e in writes]
    assert actual==expected,('Collision writes differ',actual,expected)
    mismatches=sum(a!=b for a,b in zip(predicted[0xc7b6:0xd7b6],after[0xc7b6:0xd7b6]))
    assert mismatches==0,('Additional collision changes in capture window',mismatches)
    return dict(source='MAME 0.276 / mslugx / euro; read-only collision write trace',
        activation_trace_frame=frame,object_address=hex(object_address),
        object_routine=first['object_routine'],vehicle_health=0,object_flags=flags,
        object_position=[x16/65536,y16/65536],stamp_source=first['a4'],
        observed_pc=first['pc'],before_frame=before_frame,after_frame=after_frame,
        traced_writes=len(writes),predicted_writes_match=True,
        collision_grid_bytes_verified=4096,collision_grid_mismatches=mismatches,writes=writes,
        limits='Observed ground vehicle variant only. Captures use survival/ammunition overrides, never enemy-health or collision writes. Soldier-wave prerequisite not yet established. Native vehicle controller not integrated.')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('captures',type=Path);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();report=verify(args.captures)
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(json.dumps(report,indent=2)+'\n')
    print('PASS:',report['traced_writes'],'exact destruction writes, 4096 collision bytes agree; position',report['object_position'])
