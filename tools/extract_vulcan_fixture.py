#!/usr/bin/env python3
"""Vulcan turret fixture (03B4E0/03B57C, docs/ARMAS_VEHICULOS.md §7) from the
weapons family traces: every update of a turret node (routine 03B57C) with
its node bytes before and after and the inputs it read: the creator's state
(0040AC through 00409A, +5F), the anchor word at its +6E, the rider (the
slot words 0AAB84 read and player 1's 0040AC), the input words 10E9EC/
10E9EE (0CA2CE) and the repeat byte 10F084 (0CA614). Outputs besides the
node: the slot's turret angle (0AABDC), a bullet created (00411C) with its
+6E (03B79A) and +5E (03B768), and the spark request (03B7CE, after the
sweep 014A02 answered D6 != 0)."""
import argparse,gzip,json
from pathlib import Path
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('trace',nargs='+',help='LABEL=boss-trace.txt');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();out=[]
    for spec in a.trace:
        label,path=spec.split('=',1);cur=None;frame=0
        for line in open(path):
            t=line.split()
            if not t:continue
            if t[0]=='F':frame=int(t[1]);continue
            if t[0]=='U':
                cur=dict(source=label,frame=frame,node=t[1],pre=t[3],post=None,inputs={},bullet=None,spark=False) if int(t[2],16)==0x3b57c else None
                continue
            if cur is None:continue
            if t[0]=='V':cur['post']=t[2];out.append(cur);cur=None;continue
            if t[0] not in 'RW':continue
            ad,mask,data,pc=int(t[1],16),int(t[2],16),int(t[3],16),int(t[4],16)
            key={0x409a:'creator',0x3b586:'creator5f',0x3b594:'anchor',0xaab96:'slot0',0xaab9e:'slot1c',0x3b5fa:'new',0x3b622:'held',0xca61e:'repeat'}.get(pc)
            if t[0]=='R' and key and key not in cur['inputs']:cur['inputs'][key]=[ad,mask,data]
            elif t[0]=='R' and key=='creator':cur['inputs']['creator2']=[ad,mask,data]
            # 0040AC: the first long read of a node (+00) per address.
            if t[0]=='R' and pc==0x40ac:cur['inputs'].setdefault('exists',{}).setdefault('%x'%ad,data)
            if t[0]=='W' and pc==0x3b79a:cur['bullet']=data&0xff if mask==0xff else data>>8
            if t[0]=='W' and pc==0x3b7ce:cur['spark']=True
        print(label,len([u for u in out if u['source']==label]),'turret updates')
    with gzip.open(a.output,'wt') as f:json.dump(dict(source='MAME 0.264 mslugx euro weapons family traces '+' '.join(a.trace),updates=out),f,separators=(',',':'))
if __name__=='__main__':main()
