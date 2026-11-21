#!/usr/bin/env python3
"""The ending's shrunk sprites for the console (src/end_zoom.c, docs/MISION6.md "Sprites con zoom en N64").

Exports into one romfs file, from the user's ROM and BIOS only:
- the P2 descriptors the sprite list carries with a zoom word != FFFF (the results' dial 0577E2 and the title's pieces
  0A81A4): every entry of the two lists (A5 + 1948 / 294E, check_zoom_scb.pending) of every RAM dump of the captures
  given (MAME 0.264: captures-m6/fam/T1, build/captures/ZC, ZT); their words exactly as neo_scb_entry reads them;
- the C-ROM tiles those descriptors name (tile = SCB1 word | (descriptor word 0 bits 4..7) << 12), 128 bytes each in
  the layout of filesystem/sprites.bin (check_lspc_shrink.pen_of);
- the L0 ROM of the BIOS (000-lo.lo), all 256 rows of 256 bytes: the zoom y byte of a flying piece takes values the
  captures do not all show, and a row missing on the console would have to be invented.

File (big-endian): 'EZM1', ndesc, ntiles, nwords, then ndesc x (P2 address, first word, word count), the words (u16, padded to 4 bytes), the
sorted tile numbers (u32), the tiles (128 B each), the L0 (65 536 B).

  export_ending_zoom.py --rom mslugx.zip --bios neogeo.zip --crom filesystem/sprites.bin --out romfs/endzoom.bin CAPTURE..."""
import argparse,struct,sys,zipfile
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from check_zoom_scb import A5,load_p2,ram_l,ram_w

def descriptor_words(p2,ptr):
    a=(((ptr>>20)&0xf)-1)*0x100000+(ptr&0xfffff)
    w=lambda i:(p2[a+2*i]<<8)|p2[a+2*i+1]
    w0=w(0);i=1;tiles=[]
    for _ in range(w0>>8):
        cr=w(i+2);i+=3
        n=((cr&0xff)-1)&0x1f
        for _c in range(cr>>8):
            tiles+=[w(i+k)|((w0&0xf0)<<12) for k in range(n+1)];i+=n+1
    return [w(k) for k in range(i)],tiles

def main():
    ap=argparse.ArgumentParser();ap.add_argument('captures',nargs='+',type=Path)
    ap.add_argument('--rom',type=Path,required=True);ap.add_argument('--bios',type=Path,required=True)
    ap.add_argument('--crom',type=Path,required=True);ap.add_argument('--out',type=Path,required=True)
    a=ap.parse_args()
    p2=load_p2(a.rom);l0=zipfile.ZipFile(a.bios).read('000-lo.lo')[:65536]   # (zoom y << 8) | line: the rows check_lspc_shrink reads
    crom=a.crom.read_bytes()
    descs=set();dumps=0
    for c in a.captures:
        for f in sorted(c.glob('ram*.bin')):
            r=f.read_bytes();dumps+=1
            for fwd in (False,True):
                # both lists of the dump (the one 11D04 wrote and the one 11E60 shows)
                base=A5+(0x1948 if fwd else 0x294e);end=ram_l(r,base+0x400)
                if not base<=end<=base+0x400:continue
                for p in range(base,end,4):
                    e=base+ram_w(r,p)
                    if ram_w(r,e+4)!=0xffff:descs.add(ram_l(r,e+8))
    table=[];words=[];tiles=set()
    for d in sorted(descs):
        w,t=descriptor_words(p2,d);table.append((d,len(words),len(w)));words+=w;tiles|=set(t)
    tiles=sorted(tiles)
    out=bytearray(b'EZM1'+struct.pack('>III',len(table),len(tiles),len(words)))
    for d,o,n in table:out+=struct.pack('>III',d,o,n)
    out+=struct.pack('>%dH'%len(words),*words)
    if len(words)&1:out+=b'\0\0'   # the words padded to 4 bytes
    out+=struct.pack('>%dI'%len(tiles),*tiles)
    for t in tiles:out+=crom[t*128:t*128+128]
    out+=l0
    a.out.write_bytes(out)
    print('export_ending_zoom: %d dumps, %d descriptors, %d words, %d tiles, %d bytes -> %s'%(dumps,len(table),len(words),len(tiles),len(out),a.out))
if __name__=='__main__':main()
