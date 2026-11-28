#!/usr/bin/env python3
"""Fixture of the Mission 3 train layer (docs/MISION3.md, src/neo_train.c).

From a full-game capture (run4: layers.csv, trace.csv) and the ROM camera
list of stage 4 (0B8968): one entry per update of the layers (frames where
10C72E, 10D7B6 or its +32 changed) with the root step (10C72E +14 delta),
the camera-list calls of the train due at that progress (op 0x2C after the
op-0 target the progress reached; op 0x3C sets 10E957), and the expected
10D7B6 +14 and +32, 10D7B6 +36 and the sky 10E83E +14 (op 0x3C also sets
10E958, op 0x1C on 10E83E its factor +26). Ring checks: at every VRAM dump
(vram<f>.bin) the map column of 0B8566 each sprite of 10D7B6 (+2C + 1 + k)
shows, when its tiles fit one map column only (docs/MISSION3_DEBUG.md G24).
Usage:
  python3 tools/extract_train_fixture.py CAPTURE --rom mslugx.zip --frames 117966:149171 --output data/train_fixture.json.gz"""
import argparse,csv,gzip,json,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from decode_level_script import Program
from extract_map_patches import Rom
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
        elif op==0x3c:
            out.append((target,'own',(int.from_bytes(prog.read(a+4,2),'big')&0xff)&1))
            out.append((target,'sky_own',(int.from_bytes(prog.read(a+6,2),'big')>>8)&1))
        elif op==0x1c and int.from_bytes(prog.read(a+4,4),'big')==0x10e83e:
            out.append((target,'sky_factor',int.from_bytes(prog.read(a+8,2),'big')))
        a+=n
    return out
def rings(capture,prog,uframes):
    """[[update index, [[sprite k, map column], ...]], ...] at the VRAM dumps."""
    import bisect,struct
    desc=0x0b8566;t=prog.u32(desc);w,h=prog.u16(desc+8),prog.u16(desc+10)
    cols=[]
    for i in range(w):
        cs=set()
        for j in range(h):
            b=prog.read(t+(i*h+j)*4,4);code=int.from_bytes(b[:2],'big')|((b[3]>>4)&15)<<16
            if code&0xffff:cs.add(code)
        cols.append(cs)
    out=[]
    for p in sorted(capture.glob('vram*.bin'),key=lambda p:int(p.stem[4:])):
        f=int(p.stem[4:]);i=bisect.bisect_right(uframes,f)-1
        if i<0 or f>uframes[-1]:continue
        v=p.read_bytes();ram=(capture/f'ram{f}.bin').read_bytes()
        slot=struct.unpack_from('>h',ram,0xd7b6+0x2c)[0];seen=[]
        for k in range(32):
            s=slot+1+k
            key={struct.unpack_from('>H',v,2*(s*64+2*j))[0]|((struct.unpack_from('>H',v,2*(s*64+2*j+1))[0]>>4)&15)<<16 for j in range(32)}
            key={c for c in key if c&0xffff}
            cand=[m for m in range(w) if key and key<=cols[m]]
            if len(cand)==1:seen.append([k,cand[0]])
        if seen:out.append([i,seen])
    return out
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--rom',type=Path,required=True)
    p.add_argument('--frames',required=True);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();lo,hi=(int(v) for v in a.frames.split(':'))
    fg={};tr={};prog={};sky={};o36={}
    for line in (a.capture/'layers.csv').open():
        f,o,h=line.strip().split(',')
        if not f.isdigit() or not lo<=int(f)<=hi:continue
        b=bytes.fromhex(h);f=int(f)
        if o=='10c72e':fg[f]=int.from_bytes(b[0x14:0x18],'big',signed=True)
        elif o=='10d7b6':
            tr[f]=(int.from_bytes(b[0x14:0x18],'big',signed=True),int.from_bytes(b[0x32:0x34],'big',signed=True))
            o36[f]=int.from_bytes(b[0x36:0x38],'big',signed=True)
        elif o=='10e83e':sky[f]=int.from_bytes(b[0x14:0x18],'big',signed=True)
    for d in csv.DictReader((a.capture/'trace.csv').open()):
        f=int(d['frame'])
        if lo<=f<=hi:prog[f]=int(d['camera_word'])
    sched=schedule(Program(a.rom));due=0
    frames=sorted(f for f in tr if f in fg and f-1 in tr and f-1 in fg and f-1 in prog)
    first=frames[0]-1;updates=[];uframes=[]
    for f in frames:
        if fg[f]==fg[f-1] and tr[f]==tr[f-1]:continue
        calls=[]
        while due<len(sched) and prog[f-1]>=sched[due][0]:
            calls.append([sched[due][1],sched[due][2]]);due+=1
        updates.append([fg[f]-fg[f-1],calls,tr[f][0],tr[f][1],sky[f],o36[f]]);uframes.append(f)
    fx=dict(source=str(a.capture),frames=[lo,hi],start_x=tr[first][0],start_origin=tr[first][1],start_sky=sky[first],updates=updates,
            rings=rings(a.capture,Rom(a.rom),uframes),
            rule='update = frame where 10C72E +14, 10D7B6 +14 or +32 changed; calls due when the progress of the previous frame reached their op-0 target')
    with gzip.open(a.output,'wt') as o:json.dump(fx,o,separators=(',',':'))
    print('%s: %d updates, %d calls, %d ring checks'%(a.output,len(updates),sum(len(u[1]) for u in updates),sum(len(r[1]) for r in fx['rings'])))
if __name__=='__main__':main()
