#!/usr/bin/env python3
"""Check recovered layers against original SCBs and uncovered MAME pixels.

Known coordinate variants are checked separately; unknown pixel mismatches fail.
The source is the observed region, not a claim of complete mission recovery.
"""
import argparse,json,struct
from functools import lru_cache
from pathlib import Path
from PIL import Image
from export_marco import palette_rgb
from mission_tiles import decode

def validate(captures,model_path,crom_path,out):
    model=json.loads(model_path.read_text());crom=crom_path.read_bytes()
    fixed=(crom_path.parent/'fixed.bin').read_bytes()
    @lru_cache(None)
    def tile(code,flip,palette):
        colors=[(0,0,0,0)]+[palette_rgb(w)+(255,) for w in palette[1:]]
        im=Image.new('RGBA',(16,16));im.putdata([colors[p] for p in decode(crom,code,flip)]);return im
    def paste(dst,im,x,y):dst.paste(im,(x,y),im)
    total=changed=checked=0
    out.mkdir(parents=True,exist_ok=True)
    for observation in model['observations']:
        frame=observation['frame'];camera=observation['camera'];phase=frame&7
        offsets=observation.get('offsets',((camera+1)//2,(camera*3+2)//4,camera,camera))
        vertical=observation.get('vertical_offsets',(0,0,0,0))
        active=observation.get('active',(True,True,True,True))
        folder=Path(observation.get('captures',str(captures)))
        fill=palette_rgb(observation.get('backdrop',model['backdrop']))+(255,)
        recovered=Image.new('RGBA',(320,224),fill)
        for c in model['cells']:
            if not active[c['layer']]:continue
            paste(recovered,tile(c['code']|(phase&c['animation_mask']),c['flip'],tuple(model['palettes'][c['palette']])),c['x']-offsets[c['layer']],c['y']+vertical[c['layer']])
        reference=Image.new('RGBA',(320,224),fill);covered=Image.new('L',(320,224))
        v=struct.unpack('>34304H',(folder/f'vram{frame}.bin').read_bytes())
        p=struct.unpack('>4096H',(folder/f'palette{frame}.bin').read_bytes())
        x=y=rows=zx=zy=0
        for slot in range(1,381):
            control,zoom=v[0x8200+slot],v[0x8000+slot]
            if control&64:x=(x+zx+1)&511;zx=(zoom>>8)&15
            else:x=v[0x8400+slot]>>7;y=(512-(control>>7))&511;rows=control&63;zx=(zoom>>8)&15;zy=zoom&255
            if not rows:continue
            xx=x-512 if x>=496 else x
            if xx>=320:continue
            is_background=slot<=64 or 105<=slot<=136
            for row in range(min(rows,32)):
                yy=(y+row*16)&511
                if yy>=496:yy-=512
                yy-=16
                if yy>=224 or yy+16<=0:continue
                low,attr=v[slot*64+row*2:slot*64+row*2+2]
                mask=7 if attr&8 else 3 if attr&4 else 0
                code=(low|((attr<<12)&0xf0000))&~mask;code|=phase&mask
                im=tile(code,attr&3,tuple(p[(attr>>8)*16:(attr>>8)*16+16]))
                if is_background and (attr&0xf0)==0x40:
                    assert zx==15 and zy==255,('Scaled background requires a recovered transform',frame,slot,zx,zy)
                    paste(reference,im,xx,yy)
                elif zx!=15 or zy!=255:covered.paste(255,(xx,yy,xx+16,yy+16))
                else:covered.paste(255,(xx,yy),im.getchannel('A'))
        known=set()
        for c in model['conflicts']:
            if not active[c['layer']]:continue
            xx=c['x']-offsets[c['layer']];yy=c['y']+vertical[c['layer']]
            known.update((x,y) for y in range(max(0,yy),min(224,yy+16)) for x in range(max(8,xx),min(312,xx+16)))
        screenshot=Image.open(folder/f'game{frame}.png').convert('RGB')
        for y in range(224):
            for x in range(8,312):
                total+=1
                if recovered.getpixel((x,y))!=reference.getpixel((x,y)):
                    assert (x,y) in known,('Unexplained map difference',frame,x,y)
                    changed+=1
                fix_code=v[0x7000+(x//8)*32+(y+16)//8]&4095
                fix_byte=fixed[fix_code*32+(16,24,0,8)[(x%8)//2]+(y%8)]
                fix_pen=(fix_byte>>((x&1)*4))&15
                if y>=48 and not fix_pen and not covered.getpixel((x,y)):
                    rgb=reference.getpixel((x,y))[:3];actual=screenshot.getpixel((x,y))
                    assert max(abs(a-b) for a,b in zip(rgb,actual))<=1,('MAME difference',frame,x,y,rgb,actual)
                    checked+=1
        if frame in (2100,2400,2700,2940,3480,3780,4320):recovered.save(out/f'map{frame}.png')
    result=dict(frames=len(model['observations']),map_pixels=total,known_variant_pixels=changed,mame_uncovered_pixels=checked)
    (out/'validation.json').write_text(json.dumps(result,indent=2)+'\n');print(result)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('captures',type=Path)
    p.add_argument('--model',type=Path,default=Path('filesystem/mission1/map.json'))
    p.add_argument('--crom',type=Path,default=Path('filesystem/sprites.bin'))
    p.add_argument('--output',type=Path,default=Path('build/mission-validation'))
    a=p.parse_args();validate(a.captures,a.model,a.crom,a.output)
