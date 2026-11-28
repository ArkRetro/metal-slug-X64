#!/usr/bin/env python3
"""B83 fixture: the player's draws (descriptor and facing bit of each
MARCO_DRAW) per update of a MAME 0.264 capture of
tools/mame/plans/marco-run-fire-turn.json (run_marco_catalog.py --plan),
with the visual layer's inputs of each update: moving and facing from the
plan's stick (the port's gameplay.c: the facing follows the stick at once),
a shot when the weapon state +7C becomes the fire state 0C3D9A in that
update (the actor dumps of the capture). Two segments, each from a fresh
body program: 2101-2449 (the walk from the landing, run and fire both ways:
three turns with +98 = 0, the turn program 24EF0E, and four with +98 = 2,
only the facing flipped) and 2451-2551 (fire standing, then the run started
with the flip). The turn takes no shot (0C0F16): a press during it is in
no row as a shot."""
import argparse,json,re
from pathlib import Path
SEGMENTS=((2101,2449),(2451,2551))
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path)
    p.add_argument('--output',type=Path,default=Path(__file__).resolve().parents[1]/'data/run_weapon_fixture.json');a=p.parse_args()
    plan=json.loads((a.capture/'plan.json').read_text());draws={}
    for line in (a.capture/'debug.log').read_text().splitlines():
        m=re.match(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+) xy=\S+ attr=([0-9A-F]+)',line)
        if m:draws.setdefault(int(m[1]),[]).append([int(m[2],16),int(m[3],16)&1])
    dump=lambda f:(a.capture/('actor%d.bin'%f)).read_bytes()
    state=lambda f:int.from_bytes(dump(f)[0x7c:0x80],'big')
    out=[]
    for first,last in SEGMENTS:
        facing=-1 if dump(first)[0x15]&1 else 1;rows=[]
        for f in range(first,last+1,2):
            case=next(c for c in plan['cases'] if c['first']<=f<=c['last']);held=case['inputs']
            stick=-1 if 'Left' in held else 1 if 'Right' in held else 0
            if stick:facing=stick
            rows.append(dict(frame=f,moving=stick!=0,facing=facing,fired=state(f+2)==0xc3d9a and state(f)!=0xc3d9a,draws=draws.get(f,[])))
        out.append(rows)
    a.output.write_text(json.dumps(dict(source='MAME 0.264, tools/mame/plans/marco-run-fire-turn.json',segments=out))+'\n')
    print(a.output,sum(map(len,out)),'updates')
if __name__=='__main__':main()
