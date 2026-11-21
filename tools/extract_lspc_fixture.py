#!/usr/bin/env python3
"""LSPC auto-animation fixture (MISSION3_DEBUG.md G25): the REG_LSPCMODE
writes (lspc-writes.csv, as 002802's D0: bit 3 set -> 0, else speed + 1) and
the counter bits of 3C0006 at the dump frames (lspc.csv) of MAME 0.264
captures, for tools/test_neo_lspc.c.
Usage: extract_lspc_fixture.py OUT.json RUN:FIRST:LAST [RUN:FIRST:LAST ...]"""
import csv,json,sys
out=[]
for spec in sys.argv[2:]:
    run,lo,hi=spec.rsplit(':',2);lo,hi=int(lo),int(hi)
    w=[(int(f),int(v,16)) for f,v in list(csv.reader(open(run+'/lspc-writes.csv')))[1:]]
    s=[(int(r[0]),int(r[2])) for r in list(csv.reader(open(run+'/lspc.csv')))[1:] if lo<=int(r[0])<=hi]
    d0=lambda v:0 if v&8 else (v>>8)+1
    start=[d0(v) for f,v in w if f<=lo][-1]
    out.append(dict(source=run,first=lo,last=hi,start_d0=start,writes=[(f,d0(v)) for f,v in w if lo<f<=hi],samples=s))
json.dump(dict(source='MAME 0.264 mslugx lspc-writes.csv/lspc.csv',runs=out),open(sys.argv[1],'w'))
print([(r['source'],len(r['writes']),len(r['samples'])) for r in out])
