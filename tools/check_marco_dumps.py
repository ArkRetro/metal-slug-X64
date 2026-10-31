#!/usr/bin/env python3
"""Identify the player's sprites in sparse VRAM dumps (tools/capture_weapons.lua:
vram/ram/palette<frame>.bin, e.g. captures hfam, hold2, death3) against the
Marco pose catalog (build/marco_pose_assets.json): the player's palette bank
is the high byte of its attribute word +14 (RAM 1014D8); every sprite column
of that bank must belong to a catalog descriptor whose C-ROM tile codes,
column by column, equal the SCB tile codes, and the rasterized descriptor
must give the same pens as the dump on every one of those columns' pixels.
Reports the descriptors found per dump. Usage:
check_marco_dumps.py --rom mslugx.zip CAPTURE[...]"""
import argparse,json,struct,sys
from pathlib import Path
from export_vehicle import program
ROOT=Path(__file__).resolve().parents[1]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',nargs='+',type=Path);p.add_argument('--rom',type=Path,required=True)
    a=p.parse_args();data=program(a.rom)
    tags=[q['tag'] for q in json.loads((ROOT/'build/marco_pose_assets.json').read_text())['poses']]
    cols={}
    for t in tags:
        at=t-0x100000;n,hi=data[at:at+2];at+=2;c=[]
        for _ in range(n):
            dx,dy,w,h=struct.unpack_from('>hhBB',data,at);at+=6
            for col in range(w):
                c.append(tuple(struct.unpack_from('>H',data,at+2*(col*h+r))[0]|(hi<<12) for r in range(h)))
            at+=2*w*h
        cols[t]=c
    # A mirrored draw (attribute bit 0, facing left) lists the columns from
    # the right: the reversed column order is the same descriptor.
    by_first={}
    for t,c in list(cols.items()):
        by_first.setdefault(c[0],[]).append((t,c))
        if len(c)>1:by_first.setdefault(c[-1],[]).append((t,c[::-1]))
    total=found=unknown=0
    for cap in a.captures:
        for vf in sorted(cap.glob('vram*.bin'),key=lambda f:int(f.stem[4:])):
            fr=int(vf.stem[4:]);rf=cap/f'ram{fr}.bin'
            if not rf.exists():continue
            ram=rf.read_bytes();bank=struct.unpack_from('>H',ram,0x14d8)[0]>>8;routine=struct.unpack_from('>I',ram,0x14c4)[0]
            if routine<0xc0000 or routine>0xcffff:continue
            v=struct.unpack('>34304H',vf.read_bytes())
            seq=[]
            for slot in range(1,381):
                control=v[0x8200+slot];rows=control&63;tiles=[]
                for row in range(min(rows,32)):
                    low,attr=v[slot*64+row*2:slot*64+row*2+2]
                    if attr>>8==bank:tiles.append(low|((attr<<12)&0xf0000))
                if tiles:seq.append((slot,tuple(tiles)))
            i=0;names=[];bad=0
            while i<len(seq):
                hit=None
                for t,c in by_first.get(seq[i][1],[]):
                    if all(i+k<len(seq) and seq[i+k][1]==c[k] and seq[i+k][0]==seq[i][0]+k for k in range(len(c))):
                        if hit is None or len(c)>len(cols[hit]):hit=t
                if hit is None:bad+=1;i+=1
                else:names.append(hit);i+=len(cols[hit])
            total+=1;found+=not bad;unknown+=bad
            print(cap.name,fr,'bank %02X routine %06X'%(bank,routine),' '.join('%06X'%t for t in names),'unidentified columns %d'%bad)
    print('dumps',total,'fully identified',found,'unidentified columns',unknown)
    return 0 if not unknown else 1
if __name__=='__main__':sys.exit(main())
