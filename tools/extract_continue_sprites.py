#!/usr/bin/env python3
"""Fixture of the mission actor's CONTINUE? sprites (src/neo_continue.c,
program 018120) from tools/mame/capture_gameover.lua with MSX_GO_TRACE=1
(docs/FIN_PARTIDA.md).

Per update whose 00427A at 01883C drew (S records: the sprite-list entries
written at PC 00A078 with A6 = 101364): the frame, the counter +7E and the
front-layer bit (+15 bit 2) of the mission actor after the update (M
record), and its entries (x, y, zoom word, attributes, descriptor). The item
of 019798 (first I record) is kept with its bytes."""
import argparse,gzip,json
from pathlib import Path
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    entries={};mission={};items=[]
    for line in open(a.trace):
        q=line.split()
        if q[0]=='S':entries.setdefault(int(q[1]),[]).append([int(x,16) for x in q[2:8]])
        elif q[0]=='M':mission[int(q[1])]=q[2]
        elif q[0]=='I':items.append(dict(frame=int(q[1]),node=int(q[2],16),bytes=q[3]))
    ups=[]
    for f in sorted(entries):
        m=bytes.fromhex(mission[f])
        ups.append(dict(frame=f,counter=m[0x7e]<<8|m[0x7f],front=(m[0x15]>>2)&1,entries=[[e[0],e[1],e[2],e[3],e[4]<<16|e[5]] for e in entries[f]]))
    with gzip.open(a.output,'wt') as f:json.dump(dict(source=str(a.trace),updates=ups,items=items),f,separators=(',',':'))
    print(len(ups),'updates',sum(len(u['entries']) for u in ups),'entries',len(items),'items')
if __name__=='__main__':main()
