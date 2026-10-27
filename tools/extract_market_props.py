#!/usr/bin/env python3
"""Recover the original cart and market canopy from a verified reference frame."""
import argparse,json,struct
from pathlib import Path
from PIL import Image
from export_marco import palette_rgb,top_sprite_pixel
from mission_tiles import decode

def extract(captures,output):
    root=Path(__file__).resolve().parents[1];frame=2520
    v=struct.unpack('>34304H',(captures/f'vram{frame}.bin').read_bytes())
    p=struct.unpack('>4096H',(captures/f'palette{frame}.bin').read_bytes())
    ram=(captures/f'ram{frame}.bin').read_bytes();hint=struct.unpack_from('>H',ram,0x2036)[0]
    camera=(-(v[0x8400+105]>>7))%512;camera+=round((hint-camera)/512)*512
    crom=(root/'filesystem/sprites.bin').read_bytes();palettes=[];ids={};cells=[]
    composite=Image.new('RGBA',(320,256));x=y=rows=zx=zy=0
    for slot in range(1,381):
        c,z=v[0x8200+slot],v[0x8000+slot]
        if c&64:x=(x+zx+1)&511;zx=(z>>8)&15
        else:x=v[0x8400+slot]>>7;y=(512-(c>>7))&511;rows=c&63;zx=(z>>8)&15;zy=z&255
        if slot<=136 or not rows:continue
        for row in range(min(rows,32)):
            low,attr=v[slot*64+row*2:slot*64+row*2+2];pal=attr>>8
            if pal not in (0x76,0x78):continue
            assert zx==15 and zy==255 and not attr&12
            yy=(y+row*16)&511;assert x+16<=320 and 16<=yy<224
            code=low|((attr<<12)&0xf0000);pens=decode(crom,code,attr&3)
            if not any(pens):continue
            palette=tuple(p[pal*16:pal*16+16])
            if palette not in ids:ids[palette]=len(palettes);palettes.append(palette)
            cells.append(dict(layer=4,x=x+camera,y=yy-16,code=code,flip=attr&3,
                              animation_mask=0,palette=ids[palette],source_slot=slot))
            im=Image.new('RGBA',(16,16))
            im.putdata([palette_rgb(palette[pen])+(255,) if pen else (0,0,0,0) for pen in pens])
            composite.paste(im,(x,yy),im)
    screenshot=Image.open(captures/f'game{frame}.png').convert('RGB');matched=occluded=0
    for y in range(16,240):
        for x in range(320):
            expected=composite.getpixel((x,y))
            if not expected[3]:continue
            actual=screenshot.getpixel((x,y-16));top=top_sprite_pixel(v,p,crom,x,y)
            assert top and max(abs(a-b) for a,b in zip(top[1],actual))<=1,(x,y,top,actual)
            if top[0] in (0x76,0x78):
                assert max(abs(a-b) for a,b in zip(expected[:3],actual))<=1
                matched+=1
            else:occluded+=1
    output.parent.mkdir(parents=True,exist_ok=True)
    report=dict(source=f'MAME mslugx euro frame {frame}, palettes 0x76/0x78, original SCB order',
                palettes=palettes,cells=cells,matched_pixels=matched,verified_occluded_pixels=occluded)
    output.write_text(json.dumps(report,separators=(',',':'))+'\n')
    composite.save(output.with_suffix('.png'));print('Market props:',len(cells),'tiles;',matched,'matched pixels;',occluded,'verified occluded pixels')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path)
    p.add_argument('--output',type=Path,default=Path('filesystem/mission1/props.json'))
    a=p.parse_args();extract(a.captures,a.output)
