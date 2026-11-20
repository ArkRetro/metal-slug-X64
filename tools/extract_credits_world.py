#!/usr/bin/env python3
"""Fixture of the credits' stage 0x2C run by the native world (tools/test_neo_credits.c) from a family trace of
tools/tas_updates.lua that holds the mission actor 101364 and the script actor 101414 (captures-m6/fam/runT1.sh).

A list pass runs from one update of the mission actor to the next. From the pass in which the script actor starts
(routine 019E72) to the one in which the mission actor leaves 018AC6, per pass: the updates of the script's
descendants in list order (life, routine before the update, a 32-bit FNV-1a of the bytes +10..+AF before and after it
without the native-only fields, see HASHED), and the byte RAM 10C35C the music actor 0A4408 read (the buttons held).
Per life: its creator's life (-1: the script actor) and its bytes before its first update (the bytes 004126 does
not clear are the previous occupant's). --ram: the work RAM dump of
the update before the first pass (the random ring 100016.. and its pointer 100084)."""
import argparse,gzip,json
from pathlib import Path
M,S=0x101364,0x101414
SKIP=set([0x14,0x16]+list(range(0x19,0x1d))+list(range(0x20,0x24))+list(range(0x44,0x4c)))
def fnv(hexs):
    b=bytes.fromhex(hexs);h=0x811c9dc5
    for o in range(0x10,0xb0):
        if o in SKIP:continue
        h=((h^b[o])*0x01000193)&0xffffffff
    return h
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--ram',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    life={};creator=[];first=[];passes=[];cur=None;on=False;done=False;u=None;frame=0;dead=set()
    for line in (gzip.open(a.trace,'rt') if a.trace.suffix=='.gz' else open(a.trace)):
        t=line[0]
        if t=='F':frame=int(line.split()[1]);continue
        if t=='U':
            q=line.split();n=int(q[1],16);r=int(q[2],16);u=None
            if n==M:
                if on and r not in (0x184b4,0x18ac6,0x189f2):done=True
                if done:break
                if on:cur=dict(frame=frame,held=0,updates=[]);passes.append(cur)
                continue
            if n==S and r==0x19e72 and not on and frame>62000:
                on=True;cur=dict(frame=frame,held=0,updates=[]);passes.append(cur);continue
            if not on or n not in life or n in dead:continue
            if r==0x3f1e:continue
            u=dict(life=life[n],routine=r,pre=fnv(q[3]),post=None,n=n);cur['updates'].append(u)
            if first[life[n]] is None:first[life[n]]=q[3][:0x160]
        elif t=='V':
            if u is not None:
                v=line.split()[2]
                if int(v[:8],16)!=0x3f1e:u['post']=fnv(v)   # deleted by its own update (004188): no bytes to compare
            u=None
        elif t=='B':
            q=line.split();n=int(q[1],16);c=int(q[2],16)
            if not on:continue
            if c==S or (c in life and c not in dead):
                dead.discard(n);life[n]=len(creator);creator.append(-1 if c==S else life[c]);first.append(None)
            elif n in life:del life[n]
        elif t in 'WX':
            q=line.split();ad=int(q[1],16);pc=int(q[4],16)
            if pc==0x4136 and 0x1010a4<=ad<0x1010a4+0xb0*100 and (ad-0x1010a4)%0xb0 in (0,2):dead.add(ad-(ad-0x1010a4)%0xb0)
        elif t=='R' and u is not None:
            q=line.split();ad=int(q[1],16);m=int(q[2],16);d=int(q[3],16)
            if ad==0x10c35c and m&0xff00:cur['held']=d>>8
    for ps in passes:
        for x in ps['updates']:del x['n']
    ram=a.ram.read_bytes();ring=[ram[0x16+2*i]<<8|ram[0x17+2*i] for i in range(55)]
    ptr=int.from_bytes(ram[0x84:0x88],'big')
    out=dict(source=str(a.trace),ring=ring,cursor=(ptr-0x100016)//2,creators=creator,first=first,passes=passes)
    with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
    print(len(passes),'passes',sum(len(x['updates']) for x in passes),'updates',len(creator),'lives','frames',passes[0]['frame'],passes[-1]['frame'])
if __name__=='__main__':main()
