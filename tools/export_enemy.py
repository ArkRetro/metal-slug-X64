#!/usr/bin/env python3
"""Extract the first green soldier from verified mission 1 running captures."""
import argparse,struct,json
from pathlib import Path
from PIL import Image
from export_marco import ROOT,reconstruct,palette_rgb

def export(folder):
    crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    frames=list(range(2392,2420,4))
    raw=bytearray(struct.pack('>4s6H',b'MSXA',48,64,len(frames),1,4,0))
    indexed=bytearray(struct.pack('>4s6H',b'MSXI',48,64,len(frames),1,4,0))
    report=[];matched=0;last=None;last_indexed=None
    for frame in frames:
        v=struct.unpack('>34304H',(folder/f'vram{frame}.bin').read_bytes())
        p=struct.unpack('>4096H',(folder/f'palette{frame}.bin').read_bytes())
        image,records=reconstruct(v,p,crom,0x71,(160,256))
        reference=Image.open(folder/f'game{frame}.png').convert('RGB')
        bbox=image.getbbox();assert bbox,frame
        x0,y0,x1,y1=bbox;assert x1-x0<=48 and y1-y0<=56,(frame,bbox)
        for y in range(y0,y1):
            for x in range(x0,x1):
                pixel=image.getpixel((x,y))
                if pixel[3]:
                    ref=reference.getpixel((x,y-16))
                    assert max(abs(a-b) for a,b in zip(pixel[:3],ref))<=1,(frame,x,y)
                    matched+=1
        pose=Image.new('RGBA',(48,64));pose.paste(image.crop(bbox),((48-x1+x0)//2,56-y1+y0))
        block=bytearray()
        for y in range(64):
            for x in range(48):
                r,g,b,a=pose.getpixel((x,y));block.extend(struct.pack('>H',((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|bool(a)))
        frame_indices=bytearray(48*64);original=image.info['neo_indices']
        dx=(48-x1+x0)//2;dy=56-y1+y0
        for y in range(y0,y1):
            for x in range(x0,x1):frame_indices[(dy+y-y0)*48+dx+x-x0]=original[y*320+x]
        encoded=bytearray()
        for pen in range(16):
            r,g,b=palette_rgb(p[0x710+pen]);color=((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1 if pen else 0
            encoded.extend(struct.pack('>H',color))
        encoded.extend((frame_indices[i]<<4)|frame_indices[i+1] for i in range(0,len(frame_indices),2))
        indexed.extend(encoded);last_indexed=encoded
        raw.extend(block);last=block
        report.append(dict(frame=frame,bounds=bbox,sprites=records))
    indexed.extend(last_indexed)
    (ROOT/'filesystem/soldier.idx4').write_bytes(indexed)
    raw.extend(last) # Reserved second animation, not used as a death animation.
    (ROOT/'filesystem/soldier.bin').write_bytes(raw)
    (ROOT/'filesystem/soldier.json').write_text(json.dumps(dict(palette=0x71,matched_pixels=matched,frames=report),indent=2)+'\n')
    print(f'Soldier exported: {len(frames)} samples, {matched} source pixels matched')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path);export(p.parse_args().captures)
