#!/usr/bin/env python3
"""Thin fire fixture (docs/APROXIMACIONES.md A3) from a MAME 0.264 capture of
tools/capture_fat_weapons.lua with MSX_FAT_THIN=1 (fat.txt with W lines,
debug.log with the ALLOC lines).

One row per player update (an update toggles the player's +4C; a snapshot
can fall inside an update, so the update owns the frames from its toggle to
the next). Inputs: the weapon id 10C71E, the input word 10E9EC (fire press
bit 4, up/down bits 0/1, left/right bits 2/3), +15 (ground bit 6, facing bit
0), the body routine +00 (crouched 0C2214..0C2641, the landing 0C1EBE run in
this update, a jump started 0C2692), +9A bit 0, 10C71A live shots (less the
shot of this update) and 10C71C cap. Expected: a projectile constructor of
the thin programs allocated in the update (ALLOC lines, frame + 1). The beam
(ids 6/0B) is not in this machine: its rows are marked and skipped."""
import argparse,re
from pathlib import Path
SHOTS={0x63f32,0x644c4,0xa71f8,0x6381c,0x646b0,0xa716e,0x641ec,0x8606a,0xde4da}
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();P={};W={}
    for l in open(a.capture/'fat.txt'):
        t=l.split()
        if t[0]=='P':P[int(t[1])]=bytes.fromhex(t[3])
        elif t[0]=='W':W[int(t[1])]=(bytes.fromhex(t[2]),bytes.fromhex(t[3]))
    allocs=[]
    for l in open(a.capture/'debug.log'):
        m=re.match(r'ALLOC f=(\d+) n=\w+ r=(\w+) c=00101414',l)
        if m and int(m.group(2),16) in SHOTS:allocs.append(int(m.group(1))+1)
    frames=sorted(P);ups=[f for f,g in zip(frames[1:],frames) if P[f][0x4c]!=P[g][0x4c]]
    rows=[]
    for i,f in enumerate(ups[1:],1):
        b,pb=P[f],P[ups[i-1]];w=W[f];nx=ups[i+1] if i+1<len(ups) else 1<<30
        fire=sum(f<=x<nx for x in allocs)
        body,prev=int.from_bytes(b[0:4],'big'),int.from_bytes(pb[0:4],'big')
        word=w[0][1];g=(b[0x15]>>6)&1;crouch=int(0xc2214<=body<0xc2642)
        jumped=int(body==0xc2692 and prev!=0xc2692)
        walking=int(g and not crouch and not jumped and (word&0xc)!=0)
        turned=int(g and not crouch and (b[0x15]^pb[0x15])&1)
        rows.append((f,w[1][6],(word>>4)&1,word&1,(word>>1)&1,g,crouch,int(prev==0xc1ebe),turned,walking,jumped,b[0x9a]&1,w[1][2]-fire,w[1][4],fire))
    with open(a.output,'w') as o:
        o.write('# frame id fresh up down grounded crouch landed turned walking jumped knife live cap fire\n')
        for r in rows:o.write(' '.join(str(v) for v in r)+'\n')
    print(len(rows),'updates,',sum(r[-1] for r in rows),'shots')
if __name__=='__main__':main()
