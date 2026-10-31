#!/usr/bin/env python3
"""Closed-loop fixture of the mission-end actors (src/neo_end.c) from a family
trace of tools/capture_mission_rest.lua (MSX_REST_FAMILY_ROOTS=60974,6090a,
MSX_REST_BOSS_FROM before the mission end; docs/MISION1_COMPLETA.md).

Per update of a node in the routine range: frame, node, routine, the node
bytes before (U) and after (V, none when it deleted itself) and the RAM the
original read outside the family: epoch 10C33A (01AC0E), the scene root
10E97C..10E98C (01ACF2), the layer words 10B954/10B95C (012022) and the
mission 10C2F4. Births (B) give the creation order and creators."""
import argparse,gzip,json
from pathlib import Path
from extract_boss_fight import parse
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--range',required=True,help='lo-hi routine interval (hex)')
    a=p.parse_args();lo,hi=(int(x,16) for x in a.range.split('-'))
    family=set();updates=[];births=[]
    for u in parse(a.trace):
        n=u['node']
        if lo<=u['routine']<=hi:family.add(n)
        elif n in family and u['routine']!=0x3f1e:family.discard(n)
        if n not in family or u['routine']==0x3f1e:continue   # a deleted node kept in the list (00408C): no-op
        ram={}
        for k,ad,m,d,pc in u['recs']:
            if k=='B':
                births.append((ad,m,u['frame']));continue
            if k!='R':continue
            if ad in (0x10c33a,0x10b954,0x10b95c,0x10c2f4) or 0x10e97c<=ad<0x10e98e:
                ram['%x'%ad]=(m,d)
        updates.append(dict(frame=u['frame'],node=n,routine=u['routine'],pre=u['pre'].hex(),post=u['post'].hex() if u['post'] else None,ram=ram))
    out=dict(source=str(a.trace),range=a.range,updates=updates,births=[dict(node=b[0],creator=b[1],frame=b[2]) for b in births])
    with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
    print(len(updates),'updates',len(births),'births',len({u['node'] for u in updates}),'nodes')
if __name__=='__main__':main()
