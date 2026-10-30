#!/usr/bin/env python3
"""Death/respawn fixture (docs/JEFE_MISION1.md) from a capture of
tools/capture_weapons.lua with MSX_WEAPONS_MORTAL_CAMERA (deaths.csv): per
death, from the update of the hit (routine 0C2956 appears), per update the
player's routine, +15, +30/+34, the projected +3C/+40, +28/+2C, +38 and +26 of the update's
last frame (10000E counts the updates), until the entrance ends (0C229C).
Capture RAM only. Usage: extract_death_fixture.py CAPTURE OUT.json"""
import csv,json,sys
from pathlib import Path
d=Path(sys.argv[1]);rows=list(csv.DictReader(open(d/'deaths.csv')))
byv={}
for r in rows:byv.setdefault(r['video'],[]).append(r)
deaths=[];cur=None
for i,r in enumerate(rows):
    p=rows[i-1] if i else r
    if r['routine']=='c2956' and p['routine']!='c2956':
        cur=dict(frame=int(r['frame']),progress=int(r['progress'],16),kind=int(r['f5a'],16),f5b=int(r['f5b'],16),updates=[]);deaths.append(cur);v0=int(r['video'],16);last=None
    if cur is None:continue
    v=int(r['video'],16)
    if v!=last:
        last=v
        cur['updates'].append(None)
    cur['updates'][-1]=[r['routine'],int(r['f15'],16),int(r['v30'],16),int(r['v34'],16),int(r['x3c'],16),int(r['y40'],16),int(r['x28'],16),int(r['y2c'],16),r['p38'],int(r['f26'],16)]
    if r['routine']=='c229c':cur=None
# Only complete deaths of the first mission (the table of 2D5C22).
deaths=[x for x in deaths if x['updates'][-1][0]=='c229c' and x['frame']<17000]
json.dump(dict(source=f'{d.name} deaths.csv',deaths=deaths),open(sys.argv[2],'w'),separators=(',',':'))
print(len(deaths),'deaths ->',sys.argv[2])
