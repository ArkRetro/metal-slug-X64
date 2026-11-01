#!/usr/bin/env python3
"""Closed-loop fixture of a rest-of-mission actor family from a family trace
(MSX_REST_FAMILY_ROOTS=... tools/run_capture_rest.sh, boss-trace.txt).

Every update (U..V) of a family node whose routine lies in one of the
--range intervals is kept: frame, node, routine before, node bytes before
and after, the first value of every watched RAM word the update read (scene
root, layer objects 10D7B6 (the static terrain past camera 1023) and 10C72E (the village's), dynamic
collision objects 10C06E..10C291 (eight 64-byte blocks, bins), camera progress, script flags 444E/444F, player 1 +3C/+40 and node,
kill-all 1067E2 (004BAC), score-label counts 10C5B2/10C5B4 (08D330), group counts, item pickup boxes of the players 10C5EC/10C5F4, the kernel's
last surface word 10C068 (D4 at the end of 013968), object lists 10C3A2/10C3F4 (markers, 040200/0402E6), visible rectangle 10E9D2..10E9D8 (0C8BEC), the solid-body table 106756..1067D5 (004E76), epoch, RNG ring and cursor, layer flags 3954/395C) and the
update's writes to watched globals (camera 10E95A..10E963, shake 10E9C8,
kill-all 1067E3, flash 109944, player lock 10E9E3/10E9F8..10E9FF, 10C606).
Node addresses are kept: the native test maps a child it creates to the
recorded node with the same creator (+0C) and first routine."""
import argparse,gzip,json
from pathlib import Path
WATCH={0x10c068,0x10c5b2,0x10c5b4}|set(range(0x10e97c,0x10e990,2))|set(range(0x10d7ca,0x10d7dc,2))|set(range(0x10c742,0x10c754,2))|set(range(0x10c06e,0x10c292,2))|{0x10e99e,0x10c444,0x10c44e,0x101500,0x101502,0x101504,0x1014c4,0x1014c6,0x10c33a,0x10c33c,0x10b954,0x10b95c,0x100084,0x100086,0x1067e2}|set(range(0x10c326,0x10c330,2))|set(range(0x10c5ec,0x10c5fc,2))|set(range(0x100016,0x100084,2))|set(range(0x10c3a2,0x10c446,2))|set(range(0x10e9d2,0x10e9da,2))|set(range(0x106756,0x1067d6,2))
GLOBALS=[(0x10e95a,0x10e964),(0x10e9c8,0x10e9d0),(0x1067e2,0x1067e4),(0x109944,0x109946),(0x10e9e2,0x10e9e4),(0x10e9f8,0x10ea00),(0x10c606,0x10c608)]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('trace',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--range',action='append',required=True,help='lo-hi routine interval (hex)')
    p.add_argument('--attacks',action='store_true',help='record every 0049E8 call of an update (its D0 at 004B96 and A0/A1 at 004B9A/004B9E, 0 on the 004BA4 path) and what 01B162 read of a target (+3C/+40 words, +50): the C shell\'s homing')
    p.add_argument('--ram',action='append',default=[],help='lo-hi RAM interval (hex): every first read of a byte there (input) and every write (output), e.g. the vehicle slots, input words, weapon block and player nodes a vehicle reads')
    a=p.parse_args();ranges=[tuple(int(x,16) for x in r.split('-')) for r in a.range]
    rams=[tuple(int(x,16) for x in r.split('-')) for r in a.ram]
    inram=lambda ad:any(lo<=ad<hi for lo,hi in rams)
    keep=lambda r:any(lo<=r<=hi for lo,hi in ranges)
    updates=[];cur=None;frame=0
    for line in open(a.trace):
        t=line.split()
        if not t:continue
        if t[0]=='F':frame=int(t[1]);continue
        if t[0]=='U':
            if cur is not None:cur['post']=None;updates.append(cur)
            r=int(t[2],16)
            cur=dict(frame=frame,node=t[1],routine=r,pre=t[3],reads={},writes=[],ram_in={},ram_out=[]) if keep(r) else None
            if cur is not None and a.attacks:cur['attacks']=[];cur['targets']={}
            continue
        if cur is None:continue
        if t[0]=='V':cur['post']=t[2];updates.append(cur);cur=None;continue
        if t[0] in 'RW':
            ad,mask,data,pc=int(t[1],16),int(t[2],16),int(t[3],16),int(t[4],16)
            if t[0]=='R' and ad in WATCH:
                if hex(ad) not in cur['reads']:cur['reads'][hex(ad)]=[mask,data,pc]
                elif 0x10c3a2<=ad<0x10c446 and mask&~cur['reads'][hex(ad)][0]:
                    # Object-list bytes: the id (+9) is read apart from the height (+8).
                    o=cur['reads'][hex(ad)];new=mask&~o[0];o[1]=(o[1]&o[0])|(data&new);o[0]|=new
            if t[0]=='W' and any(lo<=ad<hi for lo,hi in GLOBALS):cur['writes'].append([ad,mask,data,pc])
            if a.attacks and t[0]=='R':
                if pc==0x49e8 and ad==0x10f000:cur['attacks'].append([0,0,0])
                elif pc==0x4b96 and ad==0x106754:cur['attacks'][-1][0]=data
                elif pc==0x4b9a:cur['attacks'][-1][1]=(cur['attacks'][-1][1]<<16|data)&0xffffffff
                elif pc==0x4b9e:cur['attacks'][-1][2]=(cur['attacks'][-1][2]<<16|data)&0xffffffff
                elif pc in (0x638de,0x6391c):
                    # The rocket 06381C reads the target's +42 after 01B162
                    # (level) or its +3C long (vertical: the low word +3E).
                    if pc==0x638de:base,slot=ad-0x42,3
                    elif ad&2:base,slot=ad-0x3e,4
                    else:base,slot=ad-0x3c,0
                    tg=cur['targets'].setdefault('%x'%base,[None,None,0,0,0])
                    while len(tg)<5:tg.append(0)
                    tg[slot]=data
                elif pc in (0x1b176,0x1b17a,0x1b17e):
                    # +50 is read as two words (the second at +52).
                    base=ad-{0x1b176:0x3c,0x1b17a:0x40,0x1b17e:0x50}[pc]
                    if pc==0x1b17e and '%x'%(ad-0x52) in cur['targets']:base=ad-0x52
                    tg=cur['targets'].setdefault('%x'%base,[None,None,0,0,0])
                    if pc==0x1b176:tg[0]=data
                    elif pc==0x1b17a:tg[1]=data
                    else:tg[2]=(tg[2]<<16|data)&0xffffffff
            if inram(ad):
                for b,m in ((ad,0xff00),(ad+1,0xff)):
                    if not mask&m:continue
                    v=(data>>8)&255 if m==0xff00 else data&255
                    if t[0]=='R':
                        if '%x'%b not in cur['ram_in'] and not any(w[0]==b for w in cur['ram_out']):cur['ram_in']['%x'%b]=v
                    else:cur['ram_out'].append([b,v])
    if cur is not None:cur['post']=None;updates.append(cur)
    with gzip.open(a.output,'wt') as f:json.dump(dict(source=f'MAME 0.264 mslugx euro family trace {a.trace}',ranges=a.range,ram=a.ram,updates=updates),f,separators=(',',':'))
    rs={}
    for u in updates:rs[u['routine']]=rs.get(u['routine'],0)+1
    print(len(updates),'updates',' '.join('%05x:%d'%(k,v) for k,v in sorted(rs.items())))
if __name__=='__main__':main()
