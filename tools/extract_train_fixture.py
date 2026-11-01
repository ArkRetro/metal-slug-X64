#!/usr/bin/env python3
"""Fixture of the Mission 3 train layer (docs/MISION3.md, src/neo_train.c).

From a full-game capture (run4: layers.csv, trace.csv) and the ROM camera
list of stage 4 (0B8968): one entry per update of the layers (frames where
10C72E, 10D7B6 or its +32 changed) with the root step (10C72E +14 delta),
the camera-list calls of the train due at that progress (op 0x2C after the
op-0 target the progress reached; op 0x3C sets 10E957), and the expected
10D7B6 +14 and +32. Usage:
  python3 tools/extract_train_fixture.py CAPTURE --rom mslugx.zip --frames 117966:149171 --output data/train_fixture.json.gz"""
import argparse,csv,gzip,json,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from decode_level_script import Program
TRAIN=(0xb8d34,0xb8cf2,0xb8cfc,0xb8d06,0xb8eac,0xb8e50,0xb8e7e,0xb8e84,0xb8e8c,0xb8e94,0xb8e9c,0xb8ea4)
def schedule(prog,start=0xb8968):
    """[(progress, kind, value)]: kind 'call' (routine) or 'own' (10E957 bit 0)."""
    out=[];target=0;a=start
    for _ in range(400):
        op=int.from_bytes(prog.read(a,2),'big');n=int.from_bytes(prog.read(a+2,2),'big')
        if op==0:target=int.from_bytes(prog.read(a+4,2),'big')
        elif op==8 or n==0:break
        elif op in (0x2c,0x54,0x58):
            r=int.from_bytes(prog.read(a+4,4),'big')
            if r in TRAIN:
                assert op==0x2c,('conditional train call',hex(a))
                out.append((target,'call',r))
        elif op==0x3c:out.append((target,'own',(int.from_bytes(prog.read(a+4,2),'big')&0xff)&1))
        a+=n
    return out
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--rom',type=Path,required=True)
    p.add_argument('--frames',required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();lo,hi=(int(v) for v in a.frames.split(':'))
    fg={};tr={};prog={}
    for line in (a.capture/'layers.csv').open():
        f,o,h=line.strip().split(',')
        if not f.isdigit() or not lo<=int(f)<=hi:continue
        b=bytes.fromhex(h);f=int(f)
        if o=='10c72e':fg[f]=int.from_bytes(b[0x14:0x18],'big',signed=True)
        elif o=='10d7b6':tr[f]=(int.from_bytes(b[0x14:0x18],'big',signed=True),int.from_bytes(b[0x32:0x34],'big',signed=True))
    for d in csv.DictReader((a.capture/'trace.csv').open()):
        f=int(d['frame'])
        if lo<=f<=hi:prog[f]=int(d['camera_word'])
    sched=schedule(Program(a.rom));due=0
    frames=sorted(f for f in tr if f in fg and f-1 in tr and f-1 in fg and f-1 in prog)
    first=frames[0]-1;updates=[]
    for f in frames:
        if fg[f]==fg[f-1] and tr[f]==tr[f-1]:continue
        calls=[]
        while due<len(sched) and prog[f-1]>=sched[due][0]:
            calls.append([sched[due][1],sched[due][2]]);due+=1
        updates.append([fg[f]-fg[f-1],calls,tr[f][0],tr[f][1]])
    fx=dict(source=str(a.capture),frames=[lo,hi],start_x=tr[first][0],start_origin=tr[first][1],updates=updates,
            rule='update = frame where 10C72E +14, 10D7B6 +14 or +32 changed; calls due when the progress of the previous frame reached their op-0 target')
    with gzip.open(a.output,'wt') as o:json.dump(fx,o,separators=(',',':'))
    print('%s: %d updates, %d calls'%(a.output,len(updates),sum(len(u[1]) for u in updates)))
if __name__=='__main__':main()
