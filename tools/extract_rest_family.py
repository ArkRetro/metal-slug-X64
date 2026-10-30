#!/usr/bin/env python3
"""Fixture of one rest-of-mission actor family from a family trace
(MSX_REST_FAMILY_ROOTS=<constructor> MSX_REST_BOSS_FROM=<frame>
tools/run_capture_rest.sh, file boss-trace.txt; format in
tools/capture_mission_rest.lua).

For every update of the family's root node: frame, routine before, node
bytes before/after (U/V lines) and the RAM words the update read that the
native test needs as inputs: scene root position (RAM 10E97C/10E980 via
01ABCC), camera progress (10E99E), player 1 +3C (101500), group counts
(10C326..10C32E), epoch (10C33A), player node words (101574.. 0040AC); plus
its writes to RAM 10C606."""
import argparse,gzip,json
from pathlib import Path
WATCH={0x10e97c,0x10e97e,0x10e980,0x10e982,0x10e99e,0x101500,0x10c33a,0x10c33c}|set(range(0x10c326,0x10c330))
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--root',type=lambda v:int(v,16),required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    updates=[];cur=None;frame=0;root_node=None
    for line in open(a.trace):
        t=line.split()
        if not t:continue
        if t[0]=='F':frame=int(t[1]);continue
        if t[0]=='U':
            node,routine=int(t[1],16),int(t[2],16)
            if routine==a.root and root_node is None:root_node=node
            cur=dict(frame=frame,node=node,routine=routine,pre=t[3],reads={},writes606=[]) if node==root_node else None
            continue
        if cur is None:continue
        if t[0]=='V':
            cur['post']=t[2];updates.append(cur);cur=None;continue
        if t[0] in('R','W'):
            addr,mask,data=int(t[1],16),int(t[2],16),int(t[3],16)
            if t[0]=='R' and addr in WATCH and hex(addr) not in cur['reads']:cur['reads'][hex(addr)]=[mask,data]
            if t[0]=='W' and addr&~1==0x10c606:cur['writes606'].append([addr,mask,data])
    out=dict(source=f'MAME 0.264 mslugx euro family trace {a.trace}',root=hex(a.root),node=hex(root_node),updates=updates)
    with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
    print(len(updates),'updates of',hex(a.root),'routines',sorted({hex(u['routine']) for u in updates}))
if __name__=='__main__':main()
