#!/usr/bin/env python3
"""Mission 4 boss: the layer object 10D7B6 as the camera steps it (0C8240,
docs/MISION4.md "Jefe en juego"), from the boss trace T of the TAS
(tools/capture_fullgame.lua with the trace of the family 02267E..025EC0;
/home/user/captures-m4/fam/T/boss-trace.txt). One record per logic update
(an 'U' line of the body node 102EE4): the first values the update read of
the layer's +14/+18 (10D7CA/10D7CE) and of the root's +14/+18 (10E97C/
10E980), both after the mission actor's 0C81C0 of that update, and the
speeds 694E/6952 (10E94E/10E952) at the end of the update (the treads
write them after the camera: the next update's 0C8240 uses them); the
update that ran 0C8022 (the treads' 023432: a read of 10E9C8 by C8022).
Writes data/m4layer_fixture.json.gz; tools/test_m4layer.c compares."""
import argparse,gzip,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
WANT={0x10d7ca:'l14',0x10d7cc:'l14b',0x10d7ce:'l18',0x10d7d0:'l18b',0x10e97c:'r14',0x10e97e:'r14b',0x10e980:'r18',0x10e982:'r18b',
      0x10e94e:'s0',0x10e950:'s0b',0x10e952:'s1',0x10e954:'s1b'}
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('trace',type=Path,nargs='?',default=Path('/home/user/captures-m4/fam/T/boss-trace.txt'))
    ap.add_argument('--out',type=Path,default=ROOT/'data/m4layer_fixture.json.gz')
    a=ap.parse_args()
    segs=[];cur=None;frame=0
    for line in a.trace.open():
        k=line[0]
        if k=='F':frame=int(line.split()[1]);continue
        if k=='U':
            p=line.split()
            if p[1]=='102ee4':cur={'frame':frame};segs.append(cur)
            continue
        if k not in 'RW' or cur is None:continue
        p=line.split();ad=int(p[1],16)
        if ad==0x10e9c8 and p[4]=='c8022':cur['pulse']=1
        if ad in WANT:
            n=WANT[ad];v=int(p[3],16)
            if n[0]=='s':cur[n]=v
            else:cur.setdefault(n,v)
    recs=[]
    for c in segs:
        r={'frame':c['frame'],'pulse':c.get('pulse',0)}
        for n in ('l14','l18','r14','r18','s0','s1'):
            if n in c and n+'b' in c:r[n]=(c[n]<<16)|c[n+'b']
        recs.append(r)
    doc=dict(source='tools/extract_m4layer_fixture.py %s: %d updates of the boss body 102EE4'%(a.trace,len(recs)),records=recs)
    with gzip.open(a.out,'wt') as f:json.dump(doc,f,separators=(',',':'))
    print(len(recs),'updates,',sum(1 for r in recs if 'l14' in r and 'r14' in r),'with the layer and the root read ->',a.out)
if __name__=='__main__':main()
