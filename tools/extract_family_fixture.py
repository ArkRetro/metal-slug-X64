#!/usr/bin/env python3
"""Closed-loop fixture of a rest-of-mission actor family from a family trace
(MSX_REST_FAMILY_ROOTS=... tools/run_capture_rest.sh, boss-trace.txt).

Every update (U..V) of a family node whose routine lies in one of the
--range intervals is kept: frame, node, routine before, node bytes before
and after, the first value of every watched RAM word the update read (scene
root, layer object 10D7B6 (the static terrain past camera 1023), dynamic
collision objects 10C06E..10C291 (eight 64-byte blocks, bins), camera progress, script flags 444E/444F, player 1 +3C/+40 and node,
group counts, epoch, RNG ring and cursor, layer flags 3954/395C) and the
update's writes to watched globals (camera 10E95A..10E963, shake 10E9C8,
kill-all 1067E3, flash 109944, player lock 10E9E3/10E9F8..10E9FF, 10C606).
Node addresses are kept: the native test maps a child it creates to the
recorded node with the same creator (+0C) and first routine."""
import argparse,gzip,json
from pathlib import Path
WATCH=set(range(0x10e97c,0x10e990,2))|set(range(0x10d7ca,0x10d7dc,2))|set(range(0x10c06e,0x10c292,2))|{0x10e99e,0x10c444,0x10c44e,0x101500,0x101504,0x1014c4,0x1014c6,0x10c33a,0x10c33c,0x10b954,0x10b95c,0x100084,0x100086}|set(range(0x10c326,0x10c330,2))|set(range(0x100016,0x100084,2))
GLOBALS=[(0x10e95a,0x10e964),(0x10e9c8,0x10e9d0),(0x1067e2,0x1067e4),(0x109944,0x109946),(0x10e9e2,0x10e9e4),(0x10e9f8,0x10ea00),(0x10c606,0x10c608)]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--range',action='append',required=True,help='lo-hi routine interval (hex)')
    a=p.parse_args();ranges=[tuple(int(x,16) for x in r.split('-')) for r in a.range]
    keep=lambda r:any(lo<=r<=hi for lo,hi in ranges)
    updates=[];cur=None;frame=0
    for line in open(a.trace):
        t=line.split()
        if not t:continue
        if t[0]=='F':frame=int(t[1]);continue
        if t[0]=='U':
            if cur is not None:cur['post']=None;updates.append(cur)
            r=int(t[2],16)
            cur=dict(frame=frame,node=t[1],routine=r,pre=t[3],reads={},writes=[]) if keep(r) else None
            continue
        if cur is None:continue
        if t[0]=='V':cur['post']=t[2];updates.append(cur);cur=None;continue
        if t[0] in 'RW':
            ad,mask,data,pc=int(t[1],16),int(t[2],16),int(t[3],16),int(t[4],16)
            if t[0]=='R' and ad in WATCH and hex(ad) not in cur['reads']:cur['reads'][hex(ad)]=[mask,data,pc]
            if t[0]=='W' and any(lo<=ad<hi for lo,hi in GLOBALS):cur['writes'].append([ad,mask,data,pc])
    if cur is not None:cur['post']=None;updates.append(cur)
    with gzip.open(a.output,'wt') as f:json.dump(dict(source=f'MAME 0.264 mslugx euro family trace {a.trace}',ranges=a.range,updates=updates),f,separators=(',',':'))
    rs={}
    for u in updates:rs[u['routine']]=rs.get(u['routine'],0)+1
    print(len(updates),'updates',' '.join('%05x:%d'%(k,v) for k,v in sorted(rs.items())))
if __name__=='__main__':main()
