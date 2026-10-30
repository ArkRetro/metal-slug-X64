#!/usr/bin/env python3
"""H aim fixture (docs/ARMAS_VEHICULOS.md §10, "H en diagonal") from the hfam
capture of tools/capture_weapons.lua: per frame of the window the inputs, the
P1 fields +15/+92/+95 and the angles (+6E) of the H bullets 01B716 created in
that frame (actors.csv). Capture RAM only, no ROM data.
Usage: extract_hgun_fixture.py CAPTURE FIRST LAST OUT.json"""
import csv,json,sys
from pathlib import Path
d=Path(sys.argv[1]);a,b=int(sys.argv[2]),int(sys.argv[3])
births={}
for r in csv.DictReader(open(d/'actors.csv')):
    if r['routine']=='1b716':births.setdefault(int(r['frame']),[]).append(int(r['kind6e'],16)>>8)
rows=[]
for r in csv.DictReader(open(d/'weapons.csv')):
    f=int(r['frame'])
    if a<=f<=b:rows.append([f,r['inputs'],int(r['flags15'],16),int(r['f92'],16),int(r['f95'],16),births.get(f,[])])
json.dump(dict(source=f'{d.name} {a}..{b}',frames=rows),open(sys.argv[4],'w'),separators=(',',':'))
print(len(rows),'frames ->',sys.argv[4])
