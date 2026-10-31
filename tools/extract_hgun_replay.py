#!/usr/bin/env python3
"""H replay fixture (docs/ARMAS_VEHICULOS.md, "H en diagonal: repetición") from
a capture of tools/capture_weapons.lua (plan air): one row per original
update (RAM 10000E changes) of the window: the input words 10E9EC (+0, +2),
+15 (bit 8: +9A bit 0, the knife's enemy-in-reach flag of 0C3744), +92, +95
and the angles (+6E) of the H bullets 01B716 created in that update
(actors.csv births and reused nodes). Capture RAM only.
Usage: extract_hgun_replay.py CAPTURE FIRST LAST OUT.json"""
import csv,json,sys
from pathlib import Path
d=Path(sys.argv[1]);a,b=int(sys.argv[2]),int(sys.argv[3])
births={}
for r in csv.DictReader(open(d/'actors.csv')):
    if r['routine']=='1b716' and r['event']!='death' and r['routine']!=r['previous']:births.setdefault(int(r['frame']),[]).append(int(r['kind6e'],16)>>8)
rows=[];last=None
for r in csv.DictReader(open(d/'weapons.csv')):
    f=int(r['frame'])
    if not a<=f<=b:continue
    if r['video']!=last:
        last=r['video'];w=int(r['input_word'],16)
        rows.append([f,w>>16,w&0xffff,int(r['flags15'],16)|(int(r['f9a'],16)&1)<<8,int(r['f92'],16),int(r['f95'],16),[]])
    if rows:rows[-1][6]+=births.get(f,[])
json.dump(dict(source=f'{d.name} {a}..{b}',updates=rows),open(sys.argv[4],'w'),separators=(',',':'))
print(len(rows),'updates ->',sys.argv[4])
