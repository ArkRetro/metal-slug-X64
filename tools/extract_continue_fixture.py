#!/usr/bin/env python3
"""Continue / game-over fixture for tools/test_neo_continue.c.

Input: capture directories of tools/mame/capture_gameover.lua (state.csv, one
row per frame from 1900, and fix.bin: the fix map 7000..74FF at every frame it
changed). From the update in which P1's panel actor 101204 leaves its play
routine 0C9644 (the game over 0C9848), every game update (RAM 10000E changes)
until the HUD goes off or the panel takes the continue: the inputs of the
update (HUD flag 10E9E1, credits D00034, BIOS player mode 10FDB6 = 1 taken by
a start, input byte 10435D) and the result (P1 panel rows: 16 columns x 3 rows
from 7043, timer 1013E2, panel +88, score 10C46A, lives +84). Only RAM/VRAM
of the capture.
"""
import argparse,csv,gzip,json,struct
from pathlib import Path

def fix_frames(path):
    data=path.read_bytes();size=4+0x500*2;out={}
    for o in range(0,len(data),size):
        f=struct.unpack('>I',data[o:o+4])[0]
        out[f]=struct.unpack('>1280H',data[o+4:o+size])
    return out

def region(fix):
    return [fix[(0x7043-0x7000)+c*32+r] for c in range(16) for r in range(3)]

def extract(cap,taps):
    """taps: (first,last,period): the capture script pressed B for 4 frames
    every period frames in [first,last); 10435D is the edge byte the game
    clears in the same update, so the press is given by the schedule: an
    edge at frame E (pressed at the end of E) reaches the update ending at
    the first frame F with E+1 <= F."""
    rows=list(csv.DictReader(open(cap/'state.csv')))
    by={int(r['frame']):r for r in rows}
    fixes=fix_frames(cap/'fix.bin');keys=sorted(fixes)
    def fix_at(f):
        k=max(x for x in keys if x<=f);return fixes[k]
    start=min(f for f,r in by.items() if r['panel_routine']!='0c9644' and int(r['panel_routine'],16)>=0xc9000)
    updates=[]
    for f in range(start,max(by)+1):
        r=by[f]
        if by[f-1]['video']==r['video']:continue
        prev=max(x for x in range(start-4,f) if by[x]['video']!=by[x-1]['video'])
        edges=[e for e in range(taps[0],taps[1],taps[2])] if taps else []
        b=any(prev<e+1<=f for e in edges)
        u=dict(frame=f,b=b,hud_on=int(r['hud_on'],16)!=0,credits=int(r['credits_d00034'],16),
               start=int(r['mod1'],16)==1 and int(by[f-1]['mod1'],16)!=1,input=int(r.get('in435d') or '0',16),
               region=region(fix_at(f)),timer=int(r['ctimer'],16),mode=int(r['f88'],16),score=r['score'],lives=int(r['f84'],16),
               routine=r['panel_routine'])
        updates.append(u)
        if not u['hud_on'] or u['start']:break
    first=by[start-1]
    return dict(name=cap.name,start=start,initial=region(fix_at(start-1)),dip8a=int(first['dip8a'],16),mvs=int(first['mvs'],16)!=0,
                join=int(first['join'],16),score=first['score'],updates=updates)

def main():
    p=argparse.ArgumentParser();p.add_argument('captures',nargs='+',type=Path)
    p.add_argument('--b-taps',action='append',default=[],help='NAME:FIRST:LAST:PERIOD (MSX_GO_TAPB=1 capture)');p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    taps={t.split(':')[0]:tuple(map(int,t.split(':')[1:])) for t in a.b_taps}
    data=[extract(c,taps.get(c.name)) for c in a.captures]
    with gzip.open(a.output,'wt') as f:json.dump(data,f,separators=(',',':'))
    for d in data:print(d['name'],d['start'],len(d['updates']),d['updates'][-1]['routine'])
main()
