#!/usr/bin/env python3
"""B84 fixture: the run's turn and stop, from MAME 0.264 captures of
tools/mame/plans/marco-run-turn.json (run_marco_catalog.py --plan; pistol,
--give-hgun, --give-cgun). Per update (odd frames 2101..): the visual
layer's inputs (stick: moving and facing, the port's gameplay.c; up held;
the A press read in this update, pressed at the frame before), the shot of
the update (a weapon program chosen from a fire table: pistol 0BFD18/58/98,
H 0BFDD8.., C 0C02D8/0C0318/0C0358), the H program started (marco_hgun.c
order), and every MARCO_DRAW (descriptor and facing bit). `lost`: a press
followed by no shot until the next press (the turn 0C2746 sets the weapon
state 0C381C, which reads no input)."""
import argparse,json,re
from pathlib import Path
PISTOL={0xbfd18,0xbfd58,0xbfd98};C={0xc02d8,0xc0318,0xc0358}
HGUN=[0xbfdd8,0xbfe18,0xbfe58,0xbfe98,0xbfed8,0xbff18,0xbff58]
def segment(cap,form,cgun):
    plan=json.loads((cap/'plan.json').read_text());choices={};draws={};runs=set()
    for line in (cap/'debug.log').read_text().splitlines():
        m=re.match(r'MARCO_PROGRAM_CHOICE frame=(\d+) part=1 table=([0-9A-F]+)',line)
        if m:choices.setdefault(int(m[1]),[]).append(int(m[2],16))
        m=re.match(r'MARCO_PROGRAM_CHOICE frame=(\d+) part=0 table=0*BF718 ',line)
        if m:runs.add(int(m[1]))
        m=re.match(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+) xy=\S+ attr=([0-9A-F]+)',line)
        if m:draws.setdefault(int(m[1]),[]).append([int(m[2],16),int(m[3],16)&1])
    presses=set()
    # Captures made before plan.json kept the fire_at key: the plan file's.
    source={c['name']:c for c in json.loads((Path(__file__).resolve().parent/'mame/plans/marco-run-turn.json').read_text())}
    for c in plan['cases']:
        c=dict(c,**{k:v for k,v in source[c['name']].items() if k=='fire_at'})
        if 'fire' in c['inputs']:
            offs=c.get('fire_at')
            # run_marco_catalog.py schedule: A at the case frames, every 8 by default
            presses|=set(range(c['first'],c['last'],8)) if offs is None else {c['first']+o for o in offs}
    dump=lambda f:(cap/('actor%d.bin'%f)).read_bytes()
    # From the first update with the segment's form (+93: a weapon given at
    # the first plan frame takes it from the next update) to the last one
    # whose draws are in the capture.
    # The form taken mid-run (a weapon given at the first plan frame): from
    # the run started again (0BF718) after that, as the visual's form
    # change starts its parts again.
    first=plan['first']|1
    if dump(first)[0x93]!=form:
        while dump(first)[0x93]!=form:first+=2
        while first not in runs:first+=2
    facing=-1 if dump(first)[0x15]&1 else 1;rows=[]
    for f in range(first,plan['last']-1,2):
        case=next(c for c in plan['cases'] if c['first']<=f<=c['last']);held=case['inputs']
        stick=-1 if 'Left' in held else 1 if 'Right' in held else 0
        if stick:facing=stick
        ch=choices.get(f,[])
        fired=any(t in (PISTOL if form==0 else C if cgun else ()) for t in ch)
        hg=next((HGUN.index(t) for t in ch if form==1 and not cgun and t in HGUN),-1)
        rows.append(dict(frame=f,moving=stick!=0,facing=facing,aim_up='Up' in held,pressed=(f-1) in presses,
                         fired=fired,hgun=hg,draws=draws.get(f,[])))
    for i,r in enumerate(rows):
        if r['pressed']:
            j=i
            while j<len(rows) and (j==i or not rows[j]['pressed']) and not (rows[j]['fired'] or rows[j]['hgun']>=0):j+=1
            r['lost']=not (j<len(rows) and (rows[j]['fired'] or rows[j]['hgun']>=0) and (j==i or not rows[j]['pressed']))
    return dict(capture=cap.name,form=form,cgun=cgun,rows=rows)
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('pistol',type=Path);p.add_argument('hgun',type=Path);p.add_argument('cgun',type=Path)
    p.add_argument('--output',type=Path,default=Path(__file__).resolve().parents[1]/'data/run_turn_fixture.json');a=p.parse_args()
    segs=[segment(a.pistol,0,False),segment(a.hgun,1,False),segment(a.cgun,1,True)]
    a.output.write_text(json.dumps(dict(source='MAME 0.264, tools/mame/plans/marco-run-turn.json (pistol, --give-hgun, --give-cgun)',segments=segs))+'\n')
    print(a.output,sum(len(s['rows']) for s in segs),'updates',sum(r.get('lost',False) for s in segs for r in s['rows']),'presses lost')
if __name__=='__main__':main()
