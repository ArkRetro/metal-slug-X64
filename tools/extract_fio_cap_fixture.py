#!/usr/bin/env python3
"""Fio's actor 0C4C64 of a run_marco_catalog.py capture (--character 3 --hit
AFTER:KIND --child-routines c4c64,c4cba): the player's +30/+34 after the
update of 0C2956 (the creator the constructor reads) and the actor's node
at every update (children.log: one line per frame, an update every other
frame) until it is gone: +15, +26, +30, +34, +38, +5F, +80. Capture RAM
only. Usage: extract_fio_cap_fixture.py CAPTURE --output OUT.json"""
import argparse,json,re
from pathlib import Path
def main():
    p=argparse.ArgumentParser();p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    d=a.capture;L=lambda b,o:int.from_bytes(b[o:o+4],'big');W=lambda b,o:int.from_bytes(b[o:o+2],'big')
    frames=sorted(int(m.group(1)) for m in (re.match(r'actor(\d+)\.bin$',x.name) for x in d.iterdir()) if m)
    node={f:(d/f'actor{f}.bin').read_bytes() for f in frames}
    hit=next(f for f in frames if L(node[f],0)==0xc2956)
    after=node[hit+2]
    rows=[];last=None;gone=None
    lines=[l.split() for l in (d/'children.log').read_text().splitlines()]
    by={int(f):bytes.fromhex(h) for f,_,h in lines}
    first=min(by)
    for f in range(first,max(by)+3,2):
        b=by.get(f)
        if b is None:gone=f;break
        rows.append([b[0x15],W(b,0x26),L(b,0x30),L(b,0x34),L(b,0x38),b[0x5f],W(b,0x80)])
    out=dict(source=d.name+' (run_marco_catalog.py --character 3 --hit --child-routines c4c64,c4cba)',hit=hit,first=first,gone=gone,
        creator_v30=L(after,0x30),creator_v34=L(after,0x34),updates=rows)
    json.dump(out,open(a.output,'w'),separators=(',',':'))
    print(d.name,'hit',hit,'first',first,'updates',len(rows),'gone',gone)
if __name__=='__main__':main()
