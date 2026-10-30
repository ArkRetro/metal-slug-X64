#!/usr/bin/env python3
"""Decode the sprite descriptors drawn by the native soldier/death/throw
programs (data/soldier_programs.json) from the original P-ROM and C-ROM, with
the logical palette resources they use, and verify them against MAME captures.

Descriptor format (ROM, verified by tools/export_vehicle.py and here):
[groups][attr] then per group x,y (s16, origin-relative: x right, y up),
cols,rows and cols*rows tile words; tile = word | (attr & 0xF0) << 12; attr
bits 0/1 are the tile flips. Verification: for every soldier/death/splash
actor in the capture frames (RAM actor list), the sprite image rebuilt from
VRAM for its palette slot must equal one descriptor rendered at the actor's
projected position: x = +3C word (+ flip shift), VRAM y = 512 - +40 word.
Palette resources are resolved through the original allocator table (RAM
1068EA, ROM 009EA8): entry index = palette slot, +A = resource id."""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
from export_marco import ROOT,reconstruct
from mission_tiles import decode,rgba16
SOLDIER=lambda rt:0xAE000<=rt<0xB1000 or rt in (0x40DB4,0x40DF0,0x40F88,0x40FD4)
def program(zip_path):
    with zipfile.ZipFile(zip_path) as archive:raw=archive.read('250-p2.ep1')
    out=bytearray(raw);out[::2]=raw[1::2];out[1::2]=raw[::2];return bytes(out)
def descriptor(data,address,crom):
    at=address-0x100000;groups,attr=data[at:at+2];at+=2
    assert attr&0xC==0,'auto-animation descriptor'
    high=(attr&0xF0)<<12;cells={}
    for group in range(groups):
        x,y,cols,rows=struct.unpack_from('>hhBB',data,at);at+=6
        for col in range(cols):
            for row in range(rows):
                low=struct.unpack_from('>H',data,at)[0];at+=2
                pens=decode(crom,low|high,attr&3)
                for py in range(16):
                    for px in range(16):
                        pen=pens[py*16+px]
                        if pen:cells[(x+col*16+px,-y+row*16+py)]=pen  # y down
    assert cells,hex(address)
    xs=[c[0] for c in cells];ys=[c[1] for c in cells]
    x0,y0,x1,y1=min(xs),min(ys),max(xs)+1,max(ys)+1
    pens=bytearray((x1-x0)*(y1-y0))
    for (x,y),pen in cells.items():pens[(y-y0)*(x1-x0)+x-x0]=pen
    return dict(descriptor=hex(address),x=x0,y=y0,width=x1-x0,height=y1-y0,pens=bytes(pens))
def resources(ram):
    out={}
    for i in range(256):
        e=ram[0x68EA+16*i:0x68EA+16*i+16];rid=struct.unpack('>H',e[10:12])[0]
        if rid not in (0,0xFFFF):out[i]=rid
    return out
def verify(descs,poses,folder,frames,crom):
    matched=0;observations=[];palettes={}
    by_size={}
    for d in descs.values():by_size.setdefault((d['width'],d['height']),[]).append(d)
    for frame in frames:
        ram=(folder/f'ram{frame}.bin').read_bytes();v=struct.unpack('>34304H',(folder/f'vram{frame}.bin').read_bytes())
        p=struct.unpack('>4096H',(folder/f'palette{frame}.bin').read_bytes());slots=resources(ram)
        node=0x1010A4;seen=set()
        while node and node not in seen and 0x100000<=node<0x110000:
            seen.add(node);rt=struct.unpack_from('>I',ram,node-0x100000)[0];base=node-0x100000
            if SOLDIER(rt):
                # The VRAM of a dumped frame holds the sprite list built by the
                # previous logic update: its draw used the position that the
                # dump keeps as the cached words +44/+48.
                px=struct.unpack_from('>h',ram,base+0x44)[0];py=struct.unpack_from('>H',ram,base+0x48)[0];flags=ram[base+0x15];pal=ram[base+0x19]
                im,recs=reconstruct(v,p,crom,pal,(px-64,px+64))
                if recs:
                    actual=im.info['neo_indices'];bbox=im.getbbox()
                    found=[]
                    for key,(dx,dy,toggle) in poses.items():
                        d=descs[key[0]];w,h=d['width'],d['height']
                        # 004392..0043C6: command offsets, mirrored per attribute
                        # bit 0, minus the position correction (+4D bit 0).
                        mirror=bool((flags^toggle)&1)
                        for shift in (0,-1):
                            ox=-dx if flags&1 else dx
                            left=px+ox+(-(d['x']+w) if mirror else d['x'])+shift;top=((512-(py+dy))&511)+d['y']
                            ok=True;visible=0
                            for y in range(h):
                                yy=top+y
                                if not 0<=yy<256:continue
                                for x in range(w):
                                    xx=left+x
                                    if not 0<=xx<320:continue
                                    pen=d['pens'][y*w+(w-1-x if mirror else x)]
                                    if pen!=actual[yy*320+xx]:ok=False;break
                                    if pen:visible+=1
                                if not ok:break
                            # Pixels of the sprite outside the descriptor box belong
                            # to another actor with the same palette; only pixels
                            # inside the box are compared. At least half of the
                            # descriptor must be on screen to count.
                            if ok and visible*2>=sum(1 for q in d['pens'] if q):found.append((d['descriptor'],shift,visible,dx,dy))
                    if len(found)==1:
                        matched+=1;observations.append(dict(frame=frame,node=hex(node),routine=hex(rt),descriptor=found[0][0],shift=found[0][1],pixels=found[0][2],offset=[found[0][3],found[0][4]],mirror=bool(flags&1),palette_slot=pal,resource=slots.get(pal)))
                        if pal in slots:palettes.setdefault(slots[pal],rgba16(p[pal*16:pal*16+16]))
                    else:observations.append(dict(frame=frame,node=hex(node),routine=hex(rt),descriptor=None,candidates=found,bbox=bbox))
            node=struct.unpack_from('>I',ram,base+8)[0]
    return matched,observations,palettes
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path);p.add_argument('captures',type=Path)
    p.add_argument('--frames',default='2280:4260:60')
    p.add_argument('--palettes',type=Path,nargs='*',default=[],help='capture folders whose RAM/palette dumps supply palette resources')
    a=p.parse_args()
    data=program(a.zip);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    programs=json.loads((ROOT/'data/soldier_programs.json').read_text())
    poses={}
    for prog in programs['programs']:
        for c in prog['commands']:
            if c['kind']!='POSE':continue
            dx=c['dx']*2+((c['flags']>>1)&1);dy=c['dy']*2+(c['flags']&1);toggle=(c['flags']>>2)&3
            poses[(hex(c['asset']),dx,dy,toggle)]=(dx,dy,toggle)
    descs={k:descriptor(data,int(k,16),crom) for k in sorted({k[0] for k in poses})}
    first,last,step=map(int,a.frames.split(':'))
    matched,observations,palettes=verify(descs,poses,a.captures,range(first,last,step),crom)
    unmatched=[o for o in observations if o['descriptor'] is None]
    # Palette resources requested by the programs (009EA8 ids) that the
    # verification frames did not show: read them wherever a capture holds
    # them allocated (RAM 1068EA table + palette dump of the same frame).
    wanted={0x20E,0x22F,0x385,0x214,0x35C,0x221,0x21A,0x3BA,0x37A,0x35D,0x364,0x365,0x36C}
    # Generic actors of the region (src/neo_truck.c, neo_effects.c, neo_flyer.c):
    # truck and its flash, effects, bomber sets/propeller/flash, bombs, splash.
    wanted|={0x22C,0x306,0x204,0x20D,0x387,0x388,0x898,0x899,0x389,0x38A,0x38B,0x378,0x13A,0x13B,0x20B,0x16B}
    for folder in a.palettes:
        for pal_file in sorted(folder.glob('palette*.bin')):
            frame=int(pal_file.stem[7:]);ram_file=folder/f'ram{frame}.bin'
            if not ram_file.exists():continue
            ram=ram_file.read_bytes();p=struct.unpack('>4096H',pal_file.read_bytes())
            for slot,rid in resources(ram).items():
                if rid in wanted and rid not in palettes:palettes[rid]=rgba16(p[slot*16:slot*16+16])
    missing=sorted(hex(r) for r in wanted-set(palettes))
    frames=[dict(descriptor=d['descriptor'],x=d['x'],y=d['y'],width=d['width'],height=d['height'],pens_hex=d['pens'].hex()) for d in descs.values()]
    out=dict(source='Original P-ROM descriptors and C-ROM tiles; verified against MAME 0.264 mslugx euro VRAM/palette dumps',
        p2_sha256=hashlib.sha256(data).hexdigest(),frames=frames,palettes={hex(k):v for k,v in sorted(palettes.items())},
        matched_actors=matched,unmatched_actors=len(unmatched),missing_palettes=missing,observations=observations)
    (ROOT/'filesystem/soldier-native.json').write_text(json.dumps(out,separators=(',',':'))+'\n')
    print('Descriptors:',len(frames),'; actors matched:',matched,'; unmatched:',len(unmatched),'; palettes:',sorted(hex(k) for k in palettes),'; missing:',missing)
    for o in unmatched[:10]:print('  unmatched',o)
if __name__=='__main__':main()
