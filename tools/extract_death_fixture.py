#!/usr/bin/env python3
"""Death/respawn fixture (docs/JEFE_MISION1.md) from a capture of
tools/capture_weapons.lua with MSX_WEAPONS_MORTAL_CAMERA (deaths.csv): per
death, from the update of the hit (routine 0C2956 appears), per update the
player's routine, +15, +30/+34, the projected +3C/+40, +28/+2C, +38 and +26 of the update's
last frame (10000E counts the updates), until the entrance ends (0C229C),
and, when the capture logged the player's draws (MSX_WEAPONS_MARCO_DUMP:
MARCO_DRAW of 004406), the descriptor drawn in the update (0: none).
Capture RAM only. Usage: extract_death_fixture.py CAPTURE OUT.json"""
import csv,json,re,sys
from pathlib import Path
d=Path(sys.argv[1]);rows=list(csv.DictReader(open(d/'deaths.csv')))
drawn={}
for f,desc in re.findall(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+)',(d/'debug.log').read_text(errors='replace')):drawn.setdefault(int(f),int(desc,16))
frames_of={}
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
        cur['updates'].append(None);desc=0
    f=int(r['frame']);desc=drawn.get(f,desc) if cur['updates'][-1] is None or True else desc
    prev=cur['updates'][-1][10] if cur['updates'][-1] else 0
    cur['updates'][-1]=[r['routine'],int(r['f15'],16),int(r['v30'],16),int(r['v34'],16),int(r['x3c'],16),int(r['y40'],16),int(r['x28'],16),int(r['y2c'],16),r['p38'],int(r['f26'],16),drawn.get(f,prev)]
    if r['routine']=='c229c':cur=None
# The death program's poses (marco-d2): the MARCO_DRAW of every frame from
# the hit while the routine is dying (0C2956/0C2E8A), in order (one draw per
# update), and the number of draws while lying (0C2F06/0C2F12, until 003F1E).
routine_at={int(r['frame']):r['routine'] for r in rows}
for x in deaths:
    if not drawn:break
    f=x['frame'];dying=[];lying=0
    while routine_at.get(f) in ('c2956','c2e8a','c2692','c2888'):
        if f in drawn and routine_at.get(f) in ('c2956','c2e8a'):dying.append(drawn[f])
        f+=1
    while routine_at.get(f) in ('c2f06','c2f12'):
        lying+=f in drawn;f+=1
    x['dying_poses']=dying;x['lying_draws']=lying
# Only complete deaths of the first mission (the table of 2D5C22).
deaths=[x for x in deaths if x['updates'][-1][0]=='c229c' and x['frame']<17000]
json.dump(dict(source=f'{d.name} deaths.csv',deaths=deaths),open(sys.argv[2],'w'),separators=(',',':'))
print(len(deaths),'deaths ->',sys.argv[2])
