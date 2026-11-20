#!/usr/bin/env python3
"""The LSPC's sprite shrink against MAME 0.264 screenshots of the same frame.

Every sprite of a VRAM dump (SCB1 tiles, SCB2 shrink, SCB3 y / size / chain, SCB4 x) is drawn as the LSPC draws it:
- vertical: line L of a sprite (L = raster line - y, modulo 512) takes the byte zoomy[(zoom_y << 8) | (L & 0xFF)] of
  the L0 ROM (000-lo.lo of the BIOS: high nibble = tile of the column, low nibble = line of the tile); with L bit 8
  set the line is mirrored (L ^ 0xFF in, tile ^ 0x1F and line ^ 0xF out). A size of 0x21 or more repeats the shrunk
  height (the lines fold back on themselves, mirrored every other pass). A line whose tile is beyond the size
  (tile >= rows) is not drawn;
- horizontal: the 16 columns of a tile drawn for a shrink nibble zx are the ones of row zx of COLUMNS (zx = 15: all
  16; zx = 0: one); a chained sprite stands at the previous one's x + its zx + 1.
COLUMNS is checked here, not assumed: each row is the only one of the candidate rows that gives the pixels of the
screenshot (--derive prints, per nibble value seen, which column subsets match).

The screenshot (game<frame>.png, 320 x 224) is the raster lines 16..239. Pixels the fix layer covers and pixels of
auto-animated sprites (attribute bits 2 / 3: the tile depends on the LSPC counter, not in the dump) are not compared.

  check_lspc_shrink.py --capture captures-m6/fam/T1 --zip ROMS/neogeo.zip [--frames 61500,...] [--report OUT.json]"""
import argparse,json,struct,zipfile
from pathlib import Path
from PIL import Image
from export_marco import ROOT,palette_rgb
# Row zx: the columns (0..15) of a 16-pixel sprite drawn for the shrink nibble zx.
COLUMNS=[
 (8,),(4,8),(4,8,12),(2,4,8,12),(2,4,8,12,14),(2,4,6,8,12,14),(2,4,6,8,10,12,14),(0,2,4,6,8,10,12,14),
 (0,2,4,6,8,9,10,12,14),(0,2,3,4,6,8,9,10,12,14),(0,2,3,4,6,8,9,10,12,14,15),(0,2,3,4,6,7,8,9,10,12,14,15),
 (0,2,3,4,6,7,8,9,10,12,13,14,15),(0,1,2,3,4,6,7,8,9,10,12,13,14,15),(0,1,2,3,4,6,7,8,9,10,11,12,13,14,15),
 tuple(range(16))]
def pen_of(crom,tile,xx,yy):
    off=tile*128+(64 if xx<8 else 0)+yy*4
    return sum(((crom[off+b]>>(xx%8))&1)<<bit for bit,b in enumerate((0,2,1,3)))
def sprites(v):
    """(slot, x, y, rows, zx, zy) per SCB entry, chains resolved as the LSPC does."""
    out=[];x=y=rows=zx=zy=0
    for slot in range(1,381):
        control,zoom=v[0x8200+slot],v[0x8000+slot]
        if control&64:x=(x+zx+1)&511;zx=(zoom>>8)&15
        else:x=v[0x8400+slot]>>7;y=(512-(control>>7))&511;rows,zy,zx=control&63,zoom&255,(zoom>>8)&15
        out.append((slot,x,y,rows,zx,zy))
    return out
def line_tile(zoomy,zy,rows,line):
    """Tile row (0..31) and tile line (0..15) of the sprite line `line` (0..511), None when not drawn."""
    zl=line&0xff;inv=bool(line&0x100)
    if inv:zl^=0xff
    if rows>0x20:
        span=(zy+1)<<1;zl%=span
        if zl>zy:zl=span-1-zl;inv=not inv
    b=zoomy[(zy<<8)|zl];tile,yy=b>>4,b&15
    if inv:tile^=0x1f;yy^=0xf
    if rows<=0x20 and tile>=rows:return None
    return tile,yy
def render(v,crom,zoomy,columns=COLUMNS):
    """Raster lines 16..239: per pixel (slot, palette index), or (slot, -1) for an auto-animated tile."""
    top=[[None]*320 for _ in range(224)]
    for slot,x,y,rows,zx,zy in sprites(v):
        if not rows:continue
        cols=columns[zx]
        for r in range(224):
            t=line_tile(zoomy,zy,rows,(r+16-y)&511)
            if t is None:continue
            tile_row,yy=t
            low,attr=v[slot*64+tile_row*2],v[slot*64+tile_row*2+1]
            tile=low|((attr<<12)&0xf0000)
            for i,c in enumerate(cols):
                sx=(x+i)&511
                if sx>=320:continue
                if attr&12:top[r][sx]=(slot,-1);continue
                xx=15-c if attr&1 else c;ty=15-yy if attr&2 else yy
                pen=pen_of(crom,tile,xx,ty)
                if pen:top[r][sx]=(slot,(attr>>8)*16+pen)
    return top
def fix_mask(v):
    """Raster lines 16..239 covered by a fix tile other than the blank 0x0FF (not decoded: excluded). Tile 0 is not
    blank: the columns 0 and 39 are all tile 0 and the screenshots are black there over the sprites."""
    m=[[False]*320 for _ in range(224)]
    for col in range(40):
        for row in range(2,30):
            w=v[0x7000+col*32+row]
            if (w&0xfff)!=0x0ff:
                for r in range(row*8-16,row*8-8):
                    for c in range(col*8,col*8+8):m[r][c]=True
    return m
def main():
    p=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('--capture',type=Path,required=True);p.add_argument('--zip',type=Path,required=True)
    p.add_argument('--frames');p.add_argument('--report',type=Path)
    a=p.parse_args()
    zoomy=zipfile.ZipFile(a.zip).read('000-lo.lo');crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    frames=[int(f) for f in a.frames.split(',')] if a.frames else sorted(int(f.stem[4:]) for f in a.capture.glob('vram*.bin'))
    nibbles={};total=dict(zoomed_y_equal=0,zoomed_y_different=0,frames=0,zoomed_equal=0,zoomed_different=0,full_equal=0,full_different=0,skipped_fix=0,skipped_auto=0);per=[]
    for fr in frames:
        png=a.capture/f'game{fr}.png'
        if not png.exists() or not (a.capture/f'palette{fr}.bin').exists():continue
        v=struct.unpack('>34304H',(a.capture/f'vram{fr}.bin').read_bytes());pw=struct.unpack('>4096H',(a.capture/f'palette{fr}.bin').read_bytes())
        info={s:(zx,zy) for s,x,y,rows,zx,zy in sprites(v) if rows};zoomed={s for s,(zx,zy) in info.items() if zx!=15 or zy!=255}
        if not zoomed:continue
        im=Image.open(png).convert('RGB');px=im.load();top=render(v,crom,zoomy);fm=fix_mask(v)
        st=dict(frame=fr,zoomed_equal=0,zoomed_different=0,full_equal=0,full_different=0)
        for r in range(224):
            for c in range(320):
                t=top[r][c]
                if t is None:continue
                if fm[r][c]:total['skipped_fix']+=1;continue
                if t[1]<0:total['skipped_auto']+=1;continue
                k='zoomed' if t[0] in zoomed else 'full';same=palette_rgb(pw[t[1]])==px[c,r]
                st[k+('_equal' if same else '_different')]+=1
                if k=='zoomed':
                    zx,zy=info[t[0]];row=nibbles.setdefault(zx,[0,0]);row[0 if same else 1]+=1
                    if zy!=255:total['zoomed_y_'+('equal' if same else 'different')]+=1
        for k in st:
            if k!='frame':total[k]+=st[k]
        total['frames']+=1;per.append(st)
    total['by_shrink_nibble']={'%d'%k:dict(equal=e,different=d) for k,(e,d) in sorted(nibbles.items())}
    print(json.dumps(total))
    if a.report:a.report.write_text(json.dumps(dict(total=total,frames=per),indent=1)+'\n')
if __name__=='__main__':main()
