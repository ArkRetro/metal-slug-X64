#!/usr/bin/env python3
"""Mission 2 stage 2 stairs fixture (docs/MISION2.md, "Fase 2i").

Input: capture directories of tools/capture_m2_stairs.lua run with
MSX_STAIRS_DEBUG=1 (stairs.csv + debug.log), all from the state at_22370 of
the pilot's run (tools/capture_fullgame.lua, MSX_FULL_SAVE_AT=22370 from
sec02_19932 of run4). Output: data/m2_stairs_fixture.json.gz with
  grid    the stage's collision cells (filesystem/mission2/s2/collision.json)
          in the columns/rows window around the stairs, with the grid size;
  sweeps  every distinct call 0143EE -> 013968 of player 1 (entry registers
          and RAM 10C004..10C00D/10C018/10C068, exit D0..D3/A0/D4/D6/D7) with
          the terrain layer 10C72E and the scene root 10E968 positions
          (+14/+18, previous +1C/+20) of that frame;
  runs    per experiment: the scripted input plan and, per player update
          (the frame of each sweep), the input it read and the state after.
"""
import argparse,csv,gzip,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
ap=argparse.ArgumentParser()
ap.add_argument('captures',nargs='+',help='NAME=DIR (capture_m2_stairs.lua output)')
ap.add_argument('--collision',default=str(ROOT/'filesystem/mission2/s2/collision.json'))
ap.add_argument('--columns',default='100-170')
ap.add_argument('--rows',default='0-63')
ap.add_argument('--pilot',action='append',default=[],help='DIR:F0-F1 run of capture_fullgame.lua with the same breakpoints '
    '(MSX_FULL_DEBUG_CMDS, docs/MISION2.md) whose IN lines also print box=, shake=, t= (10C72E +14..+20) and r= (10E968 +14..+20)')
ap.add_argument('--output',default=str(ROOT/'data/m2_stairs_fixture.json.gz'))
a=ap.parse_args()
col=json.load(open(a.collision))
cells=bytes.fromhex(col['cells_hex']);C,R=col['columns'],col['rows']
c0,c1=map(int,a.columns.split('-'));r0,r1=map(int,a.rows.split('-'))
# Stamp events of the stage before the stairs (0C88B0 at frame 20458, x 768..808).
g=bytearray(cells)
for e in col['events']:
    assert e['frame']<22370,e
    for c in e['cells']:g[(c['x']//8)*R+c['y']//8]=c['new']
window=''.join('%02x'%g[c*R+r] for c in range(c0,c1+1) for r in range(r0,r1+1))
IN=re.compile(r'IN f=(\d+) d0=(\w+) d1=(\w+) d2=(\w+) d3=(\w+) a0=(\w+) m4004=(\w+) b4008=(\w+) b4009=(\w+) b400a=(\w+) b400b=(\w+) b400c=(\w+) b400d=(\w+) w4068=(\w+) w4018=(\w+)')
OUT=re.compile(r'OUT f=(\d+) d0=(\w+) d1=(\w+) d2=(\w+) d3=(\w+) a0=(\w+) d4=(\w+) d6=(\w+) d7=(\w+)')
sweeps={};runs=[]
for spec in a.captures:
    name,d=spec.split('=',1);d=Path(d)
    rows={int(r['frame']):r for r in csv.DictReader(open(d/'stairs.csv'))}
    pend=None;updates=[]
    for line in open(d/'debug.log'):
        m=IN.match(line)
        if m:pend=m.groups();continue
        m=OUT.match(line)
        if not m or not pend:continue
        f=int(m.group(1))
        if int(pend[0])!=f:pend=None;continue
        if f not in rows or f+1 not in rows or f-1 not in rows:pend=None;continue
        s=rows[f]
        scene=[int(s[k]) for k in ('terrain_x','terrain_y','prev_terrain_x','prev_terrain_y','root_x','root_y','prev_root_x','prev_root_y')]
        q=[int(v,16) for v in pend[1:]];o=[int(v,16) for v in m.groups()[1:]]
        key=(f,tuple(scene),tuple(q))
        if key in sweeps:assert sweeps[key][1]==o,(name,f)
        else:sweeps[key]=(f,o)
        after=rows[f+1]
        updates.append([f+1,rows[f-1]['inputs'],int(after['x']),int(after['y']),int(after['vx']),int(after['vy']),int(after['flags15'],16)]+scene)
        pend=None
    first=rows[min(rows)]
    runs.append({'name':name,'plan':(d/'plan.txt').read_text().strip() if (d/'plan.txt').exists() else '',
        'start':[int(first['frame']),int(first['x']),int(first['y']),int(first['vx']),int(first['vy']),int(first['flags15'],16)],
        'updates':updates})
FULL=re.compile(IN.pattern+r' box=(\w+),(\w+),(\w+),(\w+) shake=(\w+) t=(\w+),(\w+),(\w+),(\w+) r=(\w+),(\w+),(\w+),(\w+)')
def s32(v):return v-(1<<32) if v&0x80000000 else v
pilot=0
for spec in a.pilot:
    d,rng=spec.rsplit(':',1);f0,f1=map(int,rng.split('-'));pend=None
    for line in open(Path(d)/'debug.log'):
        m=FULL.match(line)
        if m:pend=m.groups();continue
        m=OUT.match(line)
        if not m or not pend:continue
        f=int(m.group(1));g_=pend;pend=None
        if int(g_[0])!=f or not f0<=f<=f1:continue
        assert [int(v,16) for v in g_[15:19]]==[0x1c,0,0x108,0x1e0] and int(g_[19],16)==0,g_   # box ROM 016DBE, no shake
        scene=[s32(int(v,16)) for v in g_[20:28]]
        sweeps[(f,tuple(scene),tuple(int(v,16) for v in g_[1:15]))]=(f,[int(v,16) for v in m.groups()[1:]]);pilot+=1
S=[]
for (f,scene,q),(_,o) in sorted(sweeps.items()):S.append([f]+list(scene)+list(q)+o)
out={'source':'tools/capture_m2_stairs.lua (MAME 0.264, mslugx euro BIOS) from at_22370, and the pilot run of tools/capture_fullgame.lua from sec02_19932 (%s); tools/extract_m2_stairs.py'%', '.join(a.pilot),
     'grid':{'columns':C,'rows':R,'window_columns':[c0,c1],'window_rows':[r0,r1],'cells_hex':window,
             'events':'stamp 0C88B0 frame 20458 applied'},
     'group_map':[0,1,2,3,4,5,10,7],
     'sweep_columns':['frame','terrain_x','terrain_y','terrain_prev_x','terrain_prev_y','root_x','root_y','root_prev_x','root_prev_y',
        'd0','d1','d2','d3','a0','m4004','b4008','b4009','b400a','b400b','b400c','b400d','w4068','w4018',
        'out_d0','out_d1','out_d2','out_d3','out_a0','out_d4','out_d6','out_d7'],
     'sweeps':S,
     'update_columns':['frame','input','x','y','vx','vy','flags15','terrain_x','terrain_y','terrain_prev_x','terrain_prev_y','root_x','root_y','root_prev_x','root_prev_y'],
     'runs':runs}
with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
print('%d sweeps of the pilot runs'%pilot)
print('%s: %d sweeps, %d runs (%s)'%(a.output,len(S),len(runs),', '.join('%s %d updates'%(r['name'],len(r['updates'])) for r in runs)))
