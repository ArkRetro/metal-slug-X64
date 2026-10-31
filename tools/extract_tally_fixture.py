#!/usr/bin/env python3
"""Closed-loop fixture of the prisoner tally 0308C0 (src/neo_tally.c) from a
family trace of tools/capture_mission_rest.lua with the VRAM write trace
(MSX_REST_FAMILY_ROOTS=308c0 MSX_REST_VRAM_TRACE=1, docs/FIN_PARTIDA.md).

Per update of a family node (routines 0308C0..0311D8; deleted nodes kept in
the list, routine 003F1E, are skipped): frame, node life, routine, the node
bytes before (U) and after (V; none when it deleted itself), the RAM words
the original read outside the family (region 100000, video counter 10000E,
players 10C33C, new presses 10C35C/10C362, rescued 10C5FC..10C603, medals
10C306, epoch 10C33A, the scene root 10E97C..10E98D, the scores 10C46A..), the random words of
0029C6 (reads at 0029D2) in order, the RAM it wrote that the native state
keeps (score 10C46A..10C475, rescued 10C5FC/10C600, fade 109920..10992C),
and every VRAM word it wrote (Z: address, data) in order. Births (B) give
the creation order and creators by life (a node address is reused by the
parade icons). Updates are grouped by list pass (`pass_`): a pass of the
original can span a frame end when an update lags."""
import argparse,gzip,json
from pathlib import Path
STACK0,STACK1=0x10f200,0x10f300
READS=(0x100000,0x10000e,0x10c33c,0x10c35c,0x10c362,0x10c5fc,0x10c5fe,0x10c600,0x10c602,0x10c306,0x10c33a)
def keep_read(a):return a in READS or a-1 in READS or 0x10e97c<=a<0x10e98e or 0x10c46a<=a<0x10c476
def keep_write(a):return 0x10c46a<=a<0x10c476 or a in (0x10c5fc,0x10c600) or 0x109920<=a<0x10992e
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--range',default='308c0-311d8',help='lo-hi routine interval (hex)')
    a=p.parse_args();lo,hi=(int(x,16) for x in a.range.split('-'))
    life={};nlives=0;births=[];updates=[];frame=0;cur=None
    # A list pass can span a frame end (a long update: the original lags);
    # a pass ends when a node updates a second time.
    npass=0;seen=set()
    def close(post):
        nonlocal cur
        if cur is not None and cur['keep']:
            cur['post']=post;del cur['keep'];updates.append(cur)
        cur=None
    for line in open(a.trace):
        t=line[0];p_=line.split()
        if t=='F':frame=int(p_[1]);continue
        if t=='U':
            close(None)
            n=int(p_[1],16);r=int(p_[2],16)
            if r==0x308c0 and n not in life:life[n]=nlives;nlives+=1
            keep=n in life and r!=0x3f1e and lo<=r<=hi
            if keep:
                if n in seen:npass+=1;seen=set()
                seen.add(n)
            cur=dict(frame=frame+1,pass_=npass,node=life.get(n,-1),routine=r,pre=p_[3],keep=keep,ram={},rng=[],writes=[],vram=[])
        elif t=='V':close(p_[2])
        elif t=='B':
            n,c=int(p_[1],16),int(p_[2],16)
            if c in life:
                life[n]=nlives;nlives+=1;births.append(dict(node=life[n],creator=life[c],frame=frame+1))
        elif cur is None or not cur['keep']:continue
        elif t=='R':
            ad,m,d,pc=(int(x,16) for x in p_[1:5])
            if STACK0<=ad<STACK1:continue
            if pc==0x29d2:cur['rng'].append(d)
            else:
                # per byte: the first value read of each byte address
                for b,bm,sh in ((0,0xff00,8),(1,0xff,0)):
                    if m&bm and keep_read(ad+b) and '%x'%(ad+b) not in cur['ram']:cur['ram']['%x'%(ad+b)]=(d>>sh)&0xff
        elif t=='W':
            ad,m,d,pc=(int(x,16) for x in p_[1:5])
            if keep_write(ad):cur['writes'].append([ad,m,d])
        elif t=='Z':cur['vram'].append([int(p_[1],16),int(p_[2],16)])
    close(None)
    out=dict(source=str(a.trace),range=a.range,updates=updates,births=births)
    with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
    print(len(updates),'updates',len(births),'births',nlives,'lives',sum(len(u['vram']) for u in updates),'VRAM words',
          sum(len(u['rng']) for u in updates),'random words')
if __name__=='__main__':main()
