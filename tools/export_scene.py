#!/usr/bin/env python3
"""Rebuild the mission 1 backdrop from C tiles + captured sprite tables.

Static visual reference, not a recovered level map or collision map.
"""
import argparse, struct
from pathlib import Path
from PIL import Image
from export_marco import ROOT, palette_rgb

def export(folder):
    v=struct.unpack('>34304H',(folder/'vram2100.bin').read_bytes())
    p=struct.unpack('>4096H',(folder/'palette2100.bin').read_bytes())
    crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    ref=Image.open(folder/'game2100.png').convert('RGB')
    results=[]
    for phase in range(8):
        im=Image.new('RGB',(320,224),palette_rgb(p[4095])); x=y=rows=zx=zy=0
        for slot in range(1,381):
            c,z=v[0x8200+slot],v[0x8000+slot]
            if c&64: x=(x+zx+1)&511; zx=z>>8&15
            else: x=v[0x8400+slot]>>7; y=(512-(c>>7))&511; rows=c&63; zx=z>>8&15; zy=z&255
            if not rows or x>=320: continue
            assert zx==15 and zy==255, (slot,z)
            for row in range(min(rows,32)):
                low,a=v[slot*64+row*2:slot*64+row*2+2]
                if a>>8 in (0x63,0x66,0x70): continue # Actor, GO and player marker.
                code=low|((a<<12)&0xf0000)
                if a&8: code=(code&~7)|phase
                elif a&4: code=(code&~3)|(phase&3)
                assert code<len(crom)//128
                for py in range(16):
                    yy=(y+row*16+py)&511
                    if not 16<=yy<240: continue
                    sy=15-py if a&2 else py
                    for px in range(16):
                        if x+px>=320: continue
                        sx=15-px if a&1 else px
                        o=code*128+(64 if sx<8 else 0)+sy*4
                        pen=sum(((crom[o+b]>>(sx%8))&1)<<bit for bit,b in enumerate((0,2,1,3)))
                        if pen: im.putpixel((x+px,yy-16),palette_rgb(p[(a>>8)*16+pen]))
        # Compare sky, sign and landscape, excluding HUD and player markers.
        errors=total=0
        for yy in range(40,224):
            for xx in range(8,312):
                if 96<=xx<=160 and 65<=yy<=165: continue
                if yy>=216 and 216<=xx<=287: continue # CREDIT text, FIX overlay.
                total+=1
                errors+=max(abs(a-b) for a,b in zip(im.getpixel((xx,yy)),ref.getpixel((xx,yy))))>1
        results.append((errors,phase,im,total))
    errors,phase,im,total=min(results,key=lambda x:x[0])
    assert errors==0,(errors,total,'Backdrop differs from original reference')
    im.save(ROOT/'filesystem/desert.png')
    raw=bytearray()
    for y in range(224):
        for x in range(320):
            r,g,b=im.getpixel((x,y)); raw.extend(struct.pack('>H',((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1))
    (ROOT/'filesystem/desert.bin').write_bytes(raw)
    print(f'PASS: {total} background pixels, phase={phase}, {len(raw)} bytes')

if __name__=='__main__':
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('captures',type=Path)
    export(ap.parse_args().captures)
