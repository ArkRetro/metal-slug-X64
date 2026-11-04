#!/usr/bin/env python3
"""Mission 3 mid-boss 0A4A94: the 16 x 16 blocks of its layer object 10E8C6
as native actor frames of the private Mission 3 export (docs/JEFE_MISION3.md,
"Jefe intermedio: estado del port").

The aircraft is not a sprite program: 0A6C18 loads the map 2A6142 (19 x 10
blocks) into the layer object 10E8C6, 0A6D7C patches it from the sub-maps of
the list 2A6210 and 0A6F30 swaps in the wreck 2A619A (0C714E writes the
blocks' tile words into the layer's sprite columns). Every block of those
maps (tile u16, attribute u16; code = tile | bank << 16, flip = attribute &
3) becomes one frame with the descriptor tag F0000000 | code << 2 | flip (no
ROM descriptor has bit 31), pens decoded from the user's C ROM
(filesystem/sprites.bin), and the layer's palette number (attribute >> 8,
0x5E) becomes the resource F000 | number with the colours of section 4 of
filesystem/mission3/s1/layers_rom.json (palette RAM of run4). The runtime
(actor.c) draws the layer's block grid with them. Frames and palette are
added to (or replaced in) the export given by --export."""
import argparse,json,sys,zipfile
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from mission_tiles import decode,rgba16
ROOT=Path(__file__).resolve().parents[1]
MAPS=(0x2a6142,0x2a6158,0x2a616e,0x2a6184,0x2a619a)
def swap(b):return bytes(b[i^1] for i in range(len(b)))
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    ap.add_argument('--crom',type=Path,default=ROOT/'filesystem/sprites.bin')
    ap.add_argument('--layers',type=Path,default=ROOT/'filesystem/mission3/s1/layers_rom.json')
    ap.add_argument('--export',type=Path,default=ROOT/'filesystem/mission3/rest-native.json')
    a=ap.parse_args()
    p2=swap(zipfile.ZipFile(a.rom).read('250-p2.ep1'))
    rd=lambda ad,n:p2[ad-0x100000:ad-0x100000+n] if ad>=0x300000 else p2[0x100000+ad-0x200000:0x100000+ad-0x200000+n]
    u16=lambda ad:int.from_bytes(rd(ad,2),'big');u32=lambda ad:int.from_bytes(rd(ad,4),'big')
    crom=a.crom.read_bytes()
    tiles=set();numbers=set()
    for m in MAPS:
        t,w,h=u32(m),u16(m+8),u16(m+10)
        for k in range(w*h):
            code,attr=u16(t+4*k),u16(t+4*k+2)
            if not code:continue
            assert not attr&0xc,('auto-animated block',hex(m),k)
            tiles.add((code|((attr>>4)&15)<<16,attr&3));numbers.add(attr>>8)
    lay=json.loads(a.layers.read_text())
    sec=next(s for s in lay['section_palettes'] if str(s['section'])=='4')
    exp=json.loads(a.export.read_text())
    frames={f['descriptor']:f for f in exp['frames']}
    for code,flip in sorted(tiles):
        key='0x%x'%(0xf0000000|code<<2|flip)
        pens=decode(crom,code,flip)
        frames[key]=dict(descriptor=key,x=0,y=0,width=16,height=16,columns=[0],pens_hex=''.join('%02x'%p for p in pens))
    for n in sorted(numbers):
        exp['palettes']['0x%x'%(0xf000|n)]=rgba16(sec['palettes']['%02X'%n])
    exp['frames']=[frames[k] for k in sorted(frames,key=lambda s:int(s,16))]
    a.export.write_text(json.dumps(exp,separators=(',',':'))+'\n')
    print(json.dumps(dict(blocks=len(tiles),palettes=['0x%x'%(0xf000|n) for n in sorted(numbers)],frames=len(exp['frames']))))
if __name__=='__main__':main()
