#!/usr/bin/env python3
"""Fixture of player 1's port block 10C35A (ROM 03CC10..03CC54, the main loop's port read 03CB3A) from a family trace
of the results with inputs during the name entry 05812A (captures-m6/fam/NE1, runNE1.sh: docs/MISION6.md "Entrada de
nombre").

Per game update of the capture's updates.txt in which the node 058198 ran: the port byte the game read (the update's
input mask: A 0x10, B 0x20, C 0x40, D 0x80, up 1, down 2, left 4, right 8, as REG_P1CNT inverted), the byte 10C35D the
node read at 0581AC and, when it read them (058586), the bytes 10C35E / 10C35F. The block's state before the first
row is the record's: the held byte of the update before and the count 10C35F of the first row plus one (the first
row has no new press)."""
import argparse,gzip,json
from pathlib import Path
def held(mask):
    return ((mask>>8)&1)<<4|((mask>>9)&1)<<5|((mask>>10)&1)<<6|((mask>>11)&1)<<7|((mask>>12)&1)<<1|((mask>>13)&1)<<2|((mask>>14)&1)<<3|((mask>>15)&1)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    ups=[]
    for line in (a.capture/'updates.txt').read_text().split('\n'):
        f=line.split()
        if len(f)>=3:ups.append((int(f[1]),int(f[0]),int(f[2],16)))
    ups.sort()
    trace=a.capture/'boss-trace.txt'
    fh=gzip.open(str(trace)+'.gz','rt') if not trace.exists() else open(trace)
    rows={};frame=0;node=False;k=0
    def update_of(fr):
        nonlocal k
        # An update is counted (003B36: its line, with the frame of that write) after its port read and its list pass:
        # a node update of frame fr belongs to the first update counted at fr or later.
        while k+1<len(ups) and ups[k][0]<fr:k+=1
        return ups[k]
    cur=None
    for line in fh:
        if line[0]=='F':frame=int(line.split()[1])
        elif line[0]=='U':
            f=line.split();node=f[2]=='58198'
            if node:
                _,u,mask=update_of(frame);assert u not in rows,('two updates of the node in one game update',u)
                cur=rows[u]=dict(update=u,frame=frame,held=held(mask))
        elif node and line.startswith('R 10c35'):
            f=line.split()
            if f[1]=='10c35c' and f[2]=='ff' and f[4]=='581ac':cur['new']=int(f[3],16)&255
            if f[1]=='10c35e' and f[2]=='ff00' and f[4]=='58586':cur['repeat']=int(f[3],16)>>8;cur['count']=int(f[3],16)&255
    out=[rows[u] for u in sorted(rows)]
    assert out and all('new' in r for r in out) and [r['update'] for r in out]==list(range(out[0]['update'],out[0]['update']+len(out)))
    first=out[0];assert first['new']==0 and 'count' in first
    before=next(m for f,u,m in ups if u==first['update']-1)
    doc=dict(source=str(a.capture),held_before=held(before),count_before=(first['count']+1)&255,delay=0x20,repeat=4,rows=out)
    with gzip.open(a.output,'wt') as f:json.dump(doc,f)
    print('%d updates (%d..%d), %d with the repeat bytes read, %d with a held byte, %d new presses, %d repeats'%(len(out),out[0]['update'],out[-1]['update'],
        sum(1 for r in out if 'repeat' in r),sum(1 for r in out if r['held']),sum(1 for r in out if r['new']),sum(1 for r in out if r.get('repeat') and not r['new'])))
if __name__=='__main__':main()
