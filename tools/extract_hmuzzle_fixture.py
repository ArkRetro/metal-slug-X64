#!/usr/bin/env python3
"""H muzzle fixture (docs/ARMAS_VEHICULOS.md §10) from captures of
tools/capture_weapons.lua: every H bullet 01B716 born in actors.csv with the
player of that frame (weapons.csv): routine, facing (+15 bit 0), ground
(+15 bit 6), +92, the bullet's angle (+6E high byte) and its birth position
relative to the player's +28/+2C (pixels, y up). Births farther than 64
pixels are slot reuses logged late and are left out. Capture RAM only.
Usage: extract_hmuzzle_fixture.py OUT.json CAPTURE..."""
import csv,json,sys
from pathlib import Path
rows=[]
for cap in sys.argv[2:]:
    d=Path(cap);W={int(r['frame']):r for r in csv.DictReader(open(d/'weapons.csv'))}
    for r in csv.DictReader(open(d/'actors.csv')):
        if r['routine']!='1b716' or r['event']!='birth':continue
        f=int(r['frame']);w=W.get(f)
        if not w:continue
        dx=int(r['x'])-(int(w['x16'])>>16);dy=int(r['y'])-(int(w['y16'])>>16)
        if abs(dx)>64 or abs(dy)>64:continue
        flags=int(w['flags15'],16)
        rows.append([d.name,f,w['routine'],flags&1,(flags>>6)&1,int(w['f92'],16),int(r['kind6e'],16)>>8,dx,dy])
json.dump(dict(source=' '.join(Path(c).name for c in sys.argv[2:]),births=rows),open(sys.argv[1],'w'),separators=(',',':'))
print(len(rows),'H births ->',sys.argv[1])
