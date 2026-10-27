#!/usr/bin/env python3
"""Separate invariant collision cells from measured state changes.

Unknown and mutable cells have a zero *validity mask*, never an inferred empty
terrain value. Timelines are independent per capture run; frame numbers from
different playthroughs must not be combined into one game-event schedule.
"""
import argparse
from collections import defaultdict
import json
from pathlib import Path
from extract_collision import observed_cells


def audit(roots):
    observations=defaultdict(dict)
    snapshots=0
    for run,root in enumerate(roots):
        paths=sorted(root.glob('ram*.bin'),key=lambda p:int(p.stem[3:]))
        assert paths, f'No RAM snapshots in {root}'
        for path in paths:
            frame=int(path.stem[3:])
            for camera,x,y,code,address in observed_cells(path.read_bytes()):
                timeline=observations[x,y].setdefault(run,[])
                if not timeline or timeline[-1]['code']!=code:
                    timeline.append(dict(first_frame=frame,last_frame=frame,
                        first_camera=camera,last_camera=camera,code=code,samples=1,
                        ram_address=0x100000+address))
                else:
                    timeline[-1].update(last_frame=frame,last_camera=camera,
                        samples=timeline[-1]['samples']+1)
            snapshots+=1
    width=max(x for x,y in observations)+1
    data=bytearray(width*32);valid=bytearray(width*32);mutable=[]
    for (x,y),runs in sorted(observations.items()):
        values={state['code'] for timeline in runs.values() for state in timeline}
        if len(values)==1:
            data[x*32+y]=next(iter(values));valid[x*32+y]=1
        else:
            mutable.append(dict(x=x*8,y=y*8,values=sorted(values),
                runs=[dict(run=run,states=states) for run,states in sorted(runs.items())]))
    return dict(source='Observed Neo Geo collision RAM; invariant cells and per-run state timelines',
        columns=width,rows=32,snapshots=snapshots,runs=[str(root) for root in roots],
        observed_cells=len(observations),invariant_cells=sum(valid),mutable_cells=len(mutable),
        unknown_cells=width*32-len(observations),ready_for_static_export=all(valid),
        validity='One byte per column-major cell: 1=invariant observed value; 0=unknown or mutable. Zero data with mask 0 is NOT empty terrain.',
        cells_hex=data.hex(),valid_mask_hex=valid.hex(),mutable=mutable,
        runtime_policy='Do not compile this report directly. Recover original object/event triggers before activating mutable collision states; capture frame numbers are not gameplay triggers.')


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('captures',nargs='+',type=Path)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();report=audit(args.captures)
    args.output.parent.mkdir(parents=True,exist_ok=True)
    args.output.write_text(json.dumps(report,separators=(',',':'))+'\n')
    print(f"Collision audit: {report['snapshots']} snapshots, {report['invariant_cells']} invariant cells, "
          f"{report['mutable_cells']} mutable, {report['unknown_cells']} unknown; static-ready={report['ready_for_static_export']}")
