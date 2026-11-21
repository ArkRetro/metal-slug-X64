#!/usr/bin/env python3
"""The console's shrunk sprites of the ending (src/end_zoom_core.c, built as a shared library) against MAME 0.264.

For every pair of dumps f, g of check_zoom_scb (f just after an 11D04, g the next after the other list's), f's list
(A5 + 1948 / 294E), the one VRAM(g) shows, is drawn entry by entry, zoomed entries only, through end_zoom_entry and
end_zoom_compose from romfs/endzoom.bin (tools/export_ending_zoom.py: the descriptors, tiles and L0 ROM exactly as the
console reads them), in list order (later entries on top, as their sprites are), strips 1:1. Then per pixel of the
raster lines 16..239, against the LSPC drawing of VRAM(g) (check_lspc_shrink.render): where the top sprite is
a zoomed one, the strip pixel must be there with its pen; where the strips draw and the VRAM has no sprite pixel, it is
a difference; pixels under an unzoomed sprite or a fix tile are not compared. The LSPC drawing of a VRAM dump is
itself equal to MAME's screenshot of that frame (check_lspc_shrink.py: T1); captures with a screenshot of g also
compare the strip's colour (the entry's palette) with it.

  check_end_zoom.py --lib build/host/end_zoom.so --file romfs/endzoom.bin --zip neogeo.zip CAPTURE.. [--quick]"""
import argparse,ctypes as C,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from check_zoom_scb import A5,ram_l,ram_w
from check_lspc_shrink import sprites,render,fix_mask
from export_marco import ROOT,palette_rgb
from PIL import Image

class Strip(C.Structure):_fields_=[('tile',C.c_uint32),('flips',C.c_uint8),('zx',C.c_uint8),('n',C.c_uint8),('line',C.c_uint8*16)]
L0=C.CFUNCTYPE(C.c_void_p,C.c_void_p,C.c_uint)
ST=C.CFUNCTYPE(None,C.c_void_p,C.POINTER(Strip),C.c_int,C.c_int)
class IO(C.Structure):_fields_=[('l0_row',L0),('strip',ST),('ctx',C.c_void_p),('auto_animated',C.c_uint)]
class File(C.Structure):_fields_=[('descriptors',C.c_uint),('tiles',C.c_uint),('table',C.c_void_p),('words',C.c_void_p),('tile_keys',C.c_void_p),('tiles_offset',C.c_uint32),('l0_offset',C.c_uint32)]

def listed(r):
    """The list 11D04 has written at this dump (check_zoom_scb.pending): the screen of the next frame shows it."""
    fwd=bool(r[0x9946]&0x80)
    base=A5+(0x1948 if fwd else 0x294e);end=ram_l(r,base+0x400)
    if not base<=end<=base+0x400:return []
    out=[]
    for p in range(base,end,4):
        a=base+ram_w(r,p);out.append((ram_w(r,a),ram_w(r,a+2),ram_w(r,a+4),ram_w(r,a+6),ram_l(r,a+8)))
    return out

def main():
    ap=argparse.ArgumentParser();ap.add_argument('captures',nargs='+',type=Path)
    ap.add_argument('--lib',type=Path,required=True);ap.add_argument('--file',type=Path,required=True);ap.add_argument('--zip',type=Path,required=True)
    ap.add_argument('--quick',action='store_true',help='make check: skip without the file or captures; PASS needs 0 different')
    a=ap.parse_args()
    if not a.file.is_file() or not a.zip.is_file() or not any(c.is_dir() for c in a.captures):
        print('check_end_zoom: skipped (no endzoom.bin, BIOS or capture)');return 0
    import zipfile
    zoomy=zipfile.ZipFile(a.zip).read('000-lo.lo');crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    lib=C.CDLL(str(a.lib));data=a.file.read_bytes();buf=C.create_string_buffer(data,len(data))
    f=File();rest=C.cast(C.addressof(buf)+16,C.c_void_p)
    assert lib.end_zoom_parse(C.byref(f),buf,rest,C.c_uint32(len(data)-16)),'bad endzoom.bin'
    lib.end_zoom_find_descriptor.restype=C.c_int;lib.end_zoom_find_tile.restype=C.c_int
    rows=[C.addressof(buf)+f.l0_offset+256*z for z in range(256)]
    state={}
    def l0(_,z):return rows[z]
    def strip(_,sp,x,line):
        s=sp.contents;state['strips']+=1
        t=lib.end_zoom_find_tile(C.byref(f),s.tile)
        if t<0:state['missing_tiles']+=1;return
        out=(C.c_uint8*128)()
        lib.end_zoom_compose(sp,C.cast(C.addressof(buf)+f.tiles_offset+128*t,C.c_void_p),out)
        for i in range(s.n):
            r=line+i-16
            if not 0<=r<224:continue
            for j in range(s.zx+1):
                sx=x+j
                if not 0<=sx<320:continue
                b=out[8*i+j//2];pen=(b&15) if j&1 else b>>4
                if pen:state['buf'][r][sx]=(state['pal']*16+pen)
    l0c=L0(l0);stc=ST(strip)
    tot=dict(frames=0,entries=0,missing_descriptors=0,strips=0,missing_tiles=0,auto_animated=0,equal=0,different=0,colour_equal=0,colour_different=0,overflow=0)
    for cap in a.captures:
        dumps=sorted(int(p.stem[3:]) for p in cap.glob('ram*.bin'));have=set(dumps)
        fl={d:(cap/f'ram{d}.bin').read_bytes()[0x9946:0x9948] for d in dumps}
        for f0 in dumps:
            # f0 just after 11D04 (1946 != 1947); its list is shown from g, the next dump after the other list's 11D04
            # (check_zoom_scb's pairs: VRAM(f0) + 11E60(f0's list) + 11D04(g's) = VRAM(g))
            if fl[f0][0]==fl[f0][1] or fl.get(f0-1,b'')[:1]==fl[f0][:1]:continue
            fr=f0+1
            while fr in have and not (fl[fr][0]!=fl[fr][1] and fl[fr][0]!=fl[f0][0]):fr+=1
            vf=cap/f'vram{fr}.bin'
            if fr not in have or not vf.exists():continue
            ents=[e for e in listed((cap/f'ram{f0}.bin').read_bytes()) if e[2]!=0xffff]
            if not ents:continue
            v=struct.unpack('>34304H',vf.read_bytes())
            state.update(buf=[[None]*320 for _ in range(224)],strips=0,missing_tiles=0)
            io=IO(l0c,stc,None,0)
            for x,y,zoom,attr,d in ents:
                k=lib.end_zoom_find_descriptor(C.byref(f),C.c_uint32(d))
                tot['entries']+=1
                if k<0:tot['missing_descriptors']+=1;continue
                state['pal']=attr>>8
                if not lib.end_zoom_entry(C.byref(f),k,C.c_uint16(x),C.c_uint16(y),C.c_uint16(zoom),C.c_uint16(attr),16,239,C.byref(io)):tot['overflow']+=1
            tot['strips']+=state['strips'];tot['missing_tiles']+=state['missing_tiles'];tot['auto_animated']+=io.auto_animated
            info={s:(zx,zy) for s,x,y,rw,zx,zy in sprites(v) if rw};zoomed={s for s,(zx,zy) in info.items() if zx!=15 or zy!=255}
            top=render(v,crom,zoomy);fm=fix_mask(v)
            png=cap/f'game{fr}.png';pal=cap/f'palette{fr}.bin'
            px=Image.open(png).convert('RGB').load() if png.exists() else None
            pw=struct.unpack('>4096H',pal.read_bytes()) if pal.exists() else None
            d0=tot['different']
            for r in range(224):
                for c in range(320):
                    if fm[r][c]:continue
                    t=top[r][c];m=state['buf'][r][c]
                    if t is not None and t[0] not in zoomed:continue   # under an unzoomed sprite
                    if t is None and m is None:continue
                    if t is not None and t[1]<0:continue
                    if t is not None and m==t[1]:tot['equal']+=1
                    else:tot['different']+=1
                    if px and m is not None and t is not None:
                        tot['colour_equal' if palette_rgb(pw[m])==px[c,r] else 'colour_different']+=1
            tot['frames']+=1
            if tot['different']>d0:print(cap.name,fr,'different',tot['different']-d0)
    print(tot)
    ok=tot['frames']>0 and tot['different']==0 and tot['missing_descriptors']==0 and tot['missing_tiles']==0 and tot['overflow']==0
    print(('PASS' if ok else 'FAIL')+': end_zoom strips: %d pixels equal to the LSPC drawing of the VRAM, %d different; %d equal to the screenshots, %d different'%(tot['equal'],tot['different'],tot['colour_equal'],tot['colour_different']))
    return 0 if ok else 1
if __name__=='__main__':sys.exit(main())
