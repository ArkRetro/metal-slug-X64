#!/usr/bin/env python3
"""Closed-loop fixture of the game's ending actors (src/neo_ending.c) from a
family trace of tools/tas_updates.lua with the VRAM write trace
(MSX_TAS_FAMILY_ROOTS=... MSX_TAS_FAMILY_VRAM=1, docs/MISION6.md "El final").

As tools/extract_tally_fixture.py, but the RAM is generic: per update of a
node whose routine lies in --range (the roots are the nodes whose first
routine is a --root; the others are their descendants, by the B lines of
the trace), the first value read of every byte outside the actor pool, the
stack 10F200..10F3FF and the --skip intervals (the original's engine state
the port keeps its own way: free node count, palette slots, sprite lists),
the random words of 0029C6 (reads at 0029D2; the ring 100016..100087 is
skipped), the last value written to every such byte, and every VRAM word
written (Z: address, data) in order. Updates are grouped by list pass."""
import argparse,gzip,json
from pathlib import Path
POOL0,POOL1=0x1010a4,0x1010a4+0xb0*100
STACK0,STACK1=0x10f200,0x10f400
RNG0,RNG1=0x100016,0x100088
def ival(s):lo,hi=s.split('-');return int(lo,16),int(hi,16)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--range',action='append',required=True,help='lo-hi routine interval (hex)')
    p.add_argument('--root',action='append',required=True,help='first routine of a root (hex)')
    p.add_argument('--skip',action='append',default=[],help='lo-hi RAM interval (hex, hi exclusive) left out')
    p.add_argument('--from-frame',type=int,default=0);p.add_argument('--to-frame',type=int,default=1<<30)
    a=p.parse_args()
    ranges=[ival(r) for r in a.range];roots={int(r,16) for r in a.root};skips=[ival(s) for s in a.skip]
    keep_r=lambda r:any(lo<=r<=hi for lo,hi in ranges)
    def keep_ad(ad):
        if POOL0<=ad<POOL1 or STACK0<=ad<STACK1 or RNG0<=ad<RNG1:return False
        return not any(lo<=ad<hi for lo,hi in skips)
    life={};nlives=0;births=[];updates=[];frame=0;cur=None;npass=0;seen=set()
    # A node freed by 004126 (its +00 written at 004136, by itself or by the delete-all 004192: an X line) is no
    # creator any more: a later node in its slot is not its descendant.
    dead=set()
    def close(post):
        nonlocal cur
        if cur is not None and cur['keep']:
            cur['post']=post;del cur['keep']
            cur['writes']=sorted([int(k,16),v] for k,v in cur['writes'].items())
            updates.append(cur)
        cur=None
    for line in open(a.trace):
        t=line[0];p_=line.split()
        if t=='F':frame=int(p_[1]);continue
        if t=='U':
            close(None)
            n=int(p_[1],16);r=int(p_[2],16)
            if n in dead:dead.discard(n);life.pop(n,None)
            if r in roots and n not in life and a.from_frame<=frame+1<=a.to_frame:life[n]=nlives;nlives+=1
            keep=n in life and r!=0x3f1e and keep_r(r) and a.from_frame<=frame+1<=a.to_frame
            if keep:
                if n in seen:npass+=1;seen=set()
                seen.add(n)
            cur=dict(frame=frame+1,pass_=npass,node=life.get(n,-1),routine=r,pre=p_[3],keep=keep,ram={},rng=[],writes={},vram=[],n=n)
        elif t=='V':close(p_[2])
        elif t=='B':
            n,c=int(p_[1],16),int(p_[2],16)
            if n in dead:dead.discard(n);life.pop(n,None)
            if c in life and c not in dead:life[n]=nlives;nlives+=1;births.append(dict(node=life[n],creator=life[c],frame=frame+1))
            elif n in life:del life[n]
        elif t in 'WX' and int(p_[4],16)==0x4136 and POOL0<=int(p_[1],16)<POOL1 and (int(p_[1],16)-POOL0)%0xb0 in (0,2):
            dead.add(int(p_[1],16)-(int(p_[1],16)-POOL0)%0xb0)
            if t=='W' and cur is not None and cur['keep']:pass
            else:continue
        if t in 'FUVB':continue
        if cur is None or not cur['keep']:continue
        if t in 'RW':
            ad,m,d,pc=(int(x,16) for x in p_[1:5])
            if t=='R' and pc==0x29d2:cur['rng'].append(d);continue
            for b,bm,sh in ((0,0xff00,8),(1,0xff,0)):
                if not m&bm or not keep_ad(ad+b):continue
                k='%x'%(ad+b);v=(d>>sh)&0xff
                if t=='R':
                    if k not in cur['ram'] and k not in cur['writes']:cur['ram'][k]=v
                else:cur['writes'][k]=v
        elif t=='Z':cur['vram'].append([int(p_[1],16),int(p_[2],16)])
    close(None)
    for u in updates:del u['n']
    out=dict(source=str(a.trace),ranges=a.range,roots=a.root,skip=a.skip,updates=updates,births=births)
    with gzip.open(a.output,'wt') as f:json.dump(out,f,separators=(',',':'))
    rs={}
    for u in updates:rs[u['routine']]=rs.get(u['routine'],0)+1
    print(len(updates),'updates',len(births),'births',nlives,'lives',sum(len(u['vram']) for u in updates),'VRAM words',
          sum(len(u['rng']) for u in updates),'random words',' '.join('%05x:%d'%kv for kv in sorted(rs.items())))
if __name__=='__main__':main()
