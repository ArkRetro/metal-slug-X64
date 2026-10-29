#!/usr/bin/env python3
"""Decode vehicle descriptors/animation tables and verify against captured SCBs."""
import argparse,json,struct,zipfile
from pathlib import Path
from export_marco import ROOT,palette_rgb,reconstruct,top_sprite_pixel
from mission_tiles import decode,rgba16
from PIL import Image
POSES=(0x16b442,0x16b484,0x16b4c6,0x16b508,0x16b54a,0x16b58e,0x16b5d2,0x16b616,0x16b65a)
SCRIPTS={'idle':0x2367e4,'idle_damaged':0x2367fe,'drive':0x236818,'drive_damaged':0x236846,'destroy':0x2368d0}

def program(zip_path):
    with zipfile.ZipFile(zip_path) as archive:raw=archive.read('250-p2.ep1')
    out=bytearray(raw);out[::2]=raw[1::2];out[1::2]=raw[::2];return out

def descriptor(data,address,crom):
    at=address-0x100000;groups,attr=data[at:at+2];at+=2
    assert attr==0x10
    canvas=bytearray(128*64)
    for group in range(groups):
        x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6
        for col in range(cols):
            for row in range(rows):
                low=struct.unpack_from('>H',data,at)[0];at+=2
                pens=decode(crom,low|0x10000,0)
                for py in range(16):
                    for px in range(16):
                        dx=64+x+col*16+px;dy=64-y+row*16+py
                        assert 0<=dx<128 and 0<=dy<64
                        pen=pens[py*16+px]
                        if pen:canvas[dy*128+dx]=pen
    return canvas

def animation(data,address):
    at=address-0x100000;frames=[];events=[];loop=False
    for step in range(32):
        opcode=data[at]
        if opcode==4:
            assert data[at+1:at+4]==b'\x40\x00\x00'
            pointer=struct.unpack_from('>I',data,at+4)[0];at+=8
            assert data[at]==0
            duration=data[at+1];at+=2
            frames.append(dict(pose=POSES.index(pointer),ticks=duration))
        elif opcode==8:
            assert struct.unpack_from('>I',data,at+2)[0]==address
            loop=True;break
        elif opcode==0x20:at+=6
        elif opcode==0x14:
            assert data[at+1]==4
            events.append(dict(before_frame=len(frames),routine=hex(struct.unpack_from('>I',data,at+2)[0])));at+=6
        elif opcode==0x28:break
        else:raise AssertionError(('Unsupported vehicle animation opcode',hex(at+0x100000),opcode))
    else:raise AssertionError('Vehicle animation did not terminate')
    return dict(frames=frames,loop=loop,events=events)

def export(zip_path,root):
    data=program(zip_path);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    poses=[descriptor(data,a,crom) for a in POSES]
    animations={name:animation(data,a) for name,a in SCRIPTS.items()}
    palette=struct.unpack('>4096H',(root/'palette3540.bin').read_bytes())
    pal_normal=palette[0x890:0x8a0];pal_flash=palette[0x8a0:0x8b0]
    observations=[];matched=occluded=0
    for frame in (3540,3600,3660,3720,3780,3791,3792,3793,3794,3795,3796,3797,3798,3840,3900,3960):
        ram=(root/f'ram{frame}.bin').read_bytes();v=struct.unpack('>34304H',(root/f'vram{frame}.bin').read_bytes())
        p=struct.unpack('>4096H',(root/f'palette{frame}.bin').read_bytes());base=0x4174
        sx=struct.unpack_from('>h',ram,base+0x3c)[0];foot=(512-struct.unpack_from('>h',ram,base+0x40)[0])&511
        mirror=bool(ram[base+0x15]&1)
        captured=[]
        for pal in (0x89,0x8a):
            im,recs=reconstruct(v,p,crom,pal)
            if recs:captured.append((pal,im))
        assert len(captured)==1
        pal,im=captured[0];actual=im.info['neo_indices'];found=[]
        for pose,pens in enumerate(poses):
            source=bytes(pens[y*128+(127-x if mirror else x)] for y in range(64) for x in range(128))
            for shift in (-1,0,1):
                left=sx-64+shift;top=foot-64
                if all(source[y*128+x]==actual[(top+y)*320+left+x] for y in range(64) for x in range(128) if 0<=left+x<320):found.append((pose,shift,source,left,top))
        assert len(found)==1,(frame,[(a,b) for a,b,*rest in found])
        pose,shift,pens,left,top=found[0];ref=Image.open(root/f'game{frame}.png').convert('RGB');visible=covered=0
        for y in range(64):
            for x in range(128):
                pen=pens[y*128+x]
                if not pen:continue
                xx,yy=left+x,top+y
                assert 8<=xx<312 and 16<=yy<240
                color=palette_rgb(p[pal*16+pen]);pixel=ref.getpixel((xx,yy-16))
                if max(abs(a-b) for a,b in zip(color,pixel))<=1:visible+=1
                else:
                    covering=top_sprite_pixel(v,p,crom,xx,yy)
                    assert covering and covering[0]!=pal and max(abs(a-b) for a,b in zip(covering[1],pixel))<=1,(frame,xx,yy)
                    covered+=1
        matched+=visible;occluded+=covered
        observations.append(dict(frame=frame,pose=pose,mirror=mirror,origin_shift_x=shift,matched_pixels=visible,occluded_pixels=covered))
    assert {o['origin_shift_x'] for o in observations}=={-1}
    frames=[dict(descriptor=hex(a),indices_hex=bytes((p[i]<<4)|p[i+1] for i in range(0,len(p),2)).hex()) for a,p in zip(POSES,poses)]
    result=dict(source='Original vehicle C-ROM descriptors and animation tables, verified MAME 0.276 euro',width=128,height=64,pivot=[64,64],mirrored_origin_shift=-1,
                palette_normal=rgba16(pal_normal),palette_flash=rgba16(pal_flash),frames=frames,animations=animations,
                matched_pixels=matched,occluded_pixels=occluded,observations=observations)
    (ROOT/'filesystem/vehicle.json').write_text(json.dumps(result,separators=(',',':'))+'\n')
    print('Vehicle:',len(frames),'poses;',matched,'MAME pixels matched;',occluded,'verified occlusions')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path);p.add_argument('captures',type=Path);a=p.parse_args();export(a.zip,a.captures)
