#!/usr/bin/env python3
"""Last step of a stage grid (collision.json of tools/extract_region_collision.py
and tools/fill_collision_rom.py) for the runtime, docs/MISION3.md
"Reconocimiento del mapa".

* The runtime (src/neo_collision.c, src/neo_kernel.c cell()) reads grid row
  y >> 3 of the kernel's local y and ignores the pack's origin: a grid whose
  origin_cells row is negative (Mission 3: -8, the observed window started
  64 px below the kernel's row 0) is shifted so that grid row = kernel row.
  The dropped rows must be empty, known and never written (else stop).
* --ring-tail N: N columns past the map's last one. The arcade's ring keeps
  in the slot of a column past the map the column 64 before it (0C714E
  clips the load), so the kernel reads column c - 64 there (MAME: Mission 3
  columns 860..862 in the boss arena, cells of 796..798). The appended
  column c holds column c - 64."""
import argparse,json
from pathlib import Path
ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('collision',type=Path)
ap.add_argument('--ring-tail',type=int,default=0);ap.add_argument('--output',type=Path)
a=ap.parse_args();d=json.loads(a.collision.read_text())
C,R=d['columns'],d['rows'];ox,oy=d['origin_cells']
cells=bytearray.fromhex(d['cells_hex']);unk=bytearray.fromhex(d['unknown_mask_hex']);mut=bytearray.fromhex(d['mutable_mask_hex'])
# fill_collision_rom.py --columns keeps the observed mask's wrapped columns (column major): the stage's are its first C.
assert len(mut)>=C*R;mut=mut[:C*R]
assert ox==0,'column origin'
if oy<0:
    drop=-oy
    for x in range(C):
        for y in range(drop):
            i=x*R+y;assert not cells[i] and not unk[i] and not mut[i],('row below the kernel row 0 is not empty',x,y)
    R2=R-drop
    cells=bytearray(b for x in range(C) for b in cells[x*R+drop:(x+1)*R]);unk=bytearray(b for x in range(C) for b in unk[x*R+drop:(x+1)*R])
    mut=bytearray(b for x in range(C) for b in mut[x*R+drop:(x+1)*R])
    d['rebased_rows']=drop;d['rows']=R=R2;d['origin_cells']=[0,0]
if a.ring_tail:
    n=a.ring_tail;assert n<=64 and C>=64
    for x in range(C,C+n):
        cells+=cells[(x-64)*R:(x-63)*R];unk+=unk[(x-64)*R:(x-63)*R];mut+=bytes(R)
    d['ring_tail']=dict(columns=n,rule='column c past the map reads column c - 64 (ring slot not reloaded, 0C714E)')
    d['columns']=C=C+n
d['cells_hex']=cells.hex();d['unknown_mask_hex']=unk.hex();d['mutable_mask_hex']=mut.hex();d['unknown_cells']=sum(1 for v in unk if v)
(a.output or a.collision).write_text(json.dumps(d,separators=(',',':'))+'\n')
print('%s: %d x %d, origin %s%s'%(a.collision,C,R,d['origin_cells'],', ring tail %d'%a.ring_tail if a.ring_tail else ''))
