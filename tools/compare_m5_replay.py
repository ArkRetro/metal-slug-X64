#!/usr/bin/env python3
"""A replay of run4 from a section state must be run4 itself (docs/MISION5.md):
compares every column of trace.csv (camera, layers, player, script, groups,
inputs, palette hash) frame by frame between the reference capture and a
replay. The first frames after the load are skipped (--skip, default 2: the
plan's stillness counter starts again) and the `plan` column is reported
apart (it depends on that counter, not on the game).
usage: compare_m5_replay.py REFERENCE REPLAY [--skip N]"""
import argparse,csv,sys
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('reference',type=Path);p.add_argument('replay',type=Path);p.add_argument('--skip',type=int,default=2)
a=p.parse_args()
rep={}
with open(a.replay/'trace.csv') as f:
    r=csv.reader(f);head=next(r)
    for row in r:rep[int(row[0])]=row
lo,hi=min(rep),max(rep);plan=head.index('plan');bad=0;n=0;planbad=0;first=None
with open(a.reference/'trace.csv') as f:
    r=csv.reader(f);next(r)
    for row in r:
        fr=int(row[0])
        if fr<lo+a.skip or fr>hi:continue
        q=rep.get(fr)
        if q is None:continue
        n+=1
        d=[head[i] for i in range(len(head)) if row[i]!=q[i] and i!=plan]
        if row[plan]!=q[plan]:planbad+=1
        if d:
            bad+=1
            if first is None:first=(fr,d)
print('frames %d..%d: %d compared, %d differ (first %s), plan label differs in %d'%(lo+a.skip,hi,n,bad,first,planbad))
sys.exit(1 if bad else 0)
