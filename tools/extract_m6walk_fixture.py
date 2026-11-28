#!/usr/bin/env python3
"""Mission 6 stage 1: the player walking on the hanging bridge 10D7B6 ->
data/m6walk_fixture.json.gz (tools/test_m6_walk.c).

captures-m6/fam/P (fam/runP.sh: run4 replayed from sec09_292683, MAME 0.264,
watchpoint on player 1's +38 = RAM 1014FC): every 014492 commit prints the
frame, the new +38 and +28/+2C. On the bridge (camera 1888, x 1916..2128)
+38 is always the root 10E968. Two stretches where the player state is the
walk 0C2888 (actors.csv) without a jump: 295951..295999 facing right and
296001..296039 facing left (+62 = 0x30000 / -0x30000, 0C2918). Frame
labels: the tap during frame N is labelled N; the bridge's header in
m6ring_fixture.json.gz is the snapshot at the end of the frame, labelled
N + 1 (tools/extract_m6ring_fixture.py)."""
import argparse,gzip,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
STRETCHES=((295951,295999,0x30000),(296001,296039,-0x30000))
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--capture',type=Path,default=Path('/home/user/captures-m6/fam/P'))
    ap.add_argument('--output',type=Path,default=ROOT/'data/m6walk_fixture.json.gz')
    a=ap.parse_args();pat=re.compile(r'PAR f=(\d+) v=([0-9A-F]+) pc=014492 cam=(\d+) px=([0-9A-F]+) py=([0-9A-F]+)')
    commits={}
    for l in open(a.capture/'debug.log'):
        m=pat.search(l)
        if not m or m.group(2)=='00000010':continue   # the high word of the long write
        f=int(m.group(1));assert m.group(2)=='0000E968' or not any(s<=f<=e for s,e,_ in STRETCHES),l
        commits[f]=(int(m.group(4),16),int(m.group(5),16))
    out=[]
    for s,e,speed in STRETCHES:
        out.append(dict(speed=speed,samples=[[f,*commits[f]] for f in sorted(commits) if s<=f<=e]))
    doc=dict(source='MAME 0.264 mslugx euro, run4 replay from sec09_292683: captures-m6/fam/P (player 1 +38 watchpoint at 014492, +28/+2C root-relative)',
        root=[-(1888<<16),0x1000000-(80<<16)],group_map=[0,1,2,3,9,5,6,7],stretches=out)
    with gzip.open(a.output,'wt') as f:json.dump(doc,f,separators=(',',':'))
    print('%s: %d stretches, %d commits'%(a.output,len(out),sum(len(s['samples']) for s in out)))
if __name__=='__main__':main()
