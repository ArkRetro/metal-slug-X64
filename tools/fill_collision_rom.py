#!/usr/bin/env python3
"""Fill the unobserved cells of a stage collision grid (collision.json of
tools/extract_region_collision.py --descriptor-layers --layer-camera) from
the ROM grid the original streams into the layer (docs/MISION2.md).

ROM 0C72EC copies the collision cells of the layer's map descriptor (+2E,
fixed ROM: +4 collision pointer, linear P2 address, bank (A>>20)-1 through
2FFFF0; +8 column pairs; +A blocks per column pair) into the RAM grid
(+88): 4 bytes per 2x2 block, (x,r) (x+1,r) (x,r+1) (x+1,r+1), column pairs
one after another, r counting upwards (world row y = Y0 - r).

The mapping is checked against every observed cell first: in the filled
rows every known, non-mutable cell must equal the ROM cell, except the
cells listed by --allow-conflicts (an unidentified write the observed value
keeps). Known cells are never changed; only unknown cells are filled."""
import argparse,json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from decode_level_script import Program
def rom_grid(prog,descriptor):
    m,c,w,h=struct.unpack_from('>IIHH',prog.fixed,descriptor)
    o=c-0x100000;g=bytearray(2*w*2*h);R=2*h
    for pair in range(w):
        for b in range(h):
            v=prog.p2[o+(pair*h+b)*4:o+(pair*h+b)*4+4]
            for k,(dx,dr) in enumerate(((0,0),(1,0),(0,1),(1,1))):g[(2*pair+dx)*R+2*b+dr]=v[k]
    return g,2*w,R
def main():
    ap=argparse.ArgumentParser(description=__doc__);ap.add_argument('collision',type=Path);ap.add_argument('--rom',type=Path,required=True)
    ap.add_argument('--descriptor',type=lambda v:int(v,16),required=True);ap.add_argument('--top',type=int,required=True,help='Y0: world row of ROM row 0')
    ap.add_argument('--rows',default=None,help='LO:HI world rows to fill (inclusive; default all ROM rows)')
    ap.add_argument('--allow-conflicts',type=int,default=0,help='known cells that may differ from the ROM (kept)')
    ap.add_argument('--output',type=Path)
    a=ap.parse_args();d=json.loads(a.collision.read_text());C,R=d['columns'],d['rows']
    cells=bytearray.fromhex(d['cells_hex']);unk=bytearray.fromhex(d['unknown_mask_hex']);mut=bytes.fromhex(d['mutable_mask_hex'])
    g,gc,gr=rom_grid(Program(a.rom),a.descriptor)
    lo,hi=(int(x) for x in a.rows.split(':')) if a.rows else (a.top-gr+1,a.top)
    ok=0;conflicts=[];filled=0
    for x in range(min(C,gc)):
        for y in range(max(0,lo),min(R-1,hi)+1):
            r=a.top-y
            if not 0<=r<gr:continue
            i=x*R+y
            if unk[i]:continue
            if mut[i]:continue
            if cells[i]==g[x*gr+r]:ok+=1
            else:conflicts.append([x,y,cells[i],g[x*gr+r]])
    assert len(conflicts)<=a.allow_conflicts,('ROM grid disagrees with observed cells',len(conflicts),conflicts[:8])
    for x in range(min(C,gc)):
        for y in range(max(0,lo),min(R-1,hi)+1):
            r=a.top-y;i=x*R+y
            if 0<=r<gr and unk[i]:cells[i]=g[x*gr+r];unk[i]=0;filled+=1
    d['cells_hex']=cells.hex();d['unknown_mask_hex']=unk.hex();d['unknown_cells']=sum(1 for v in unk if v)
    d['rom_fill']=dict(descriptor='0x%06X'%a.descriptor,top=a.top,rows=[lo,hi],rom_columns=gc,rom_rows=gr,checked=ok,filled=filled,conflicts=conflicts,
        rule='ROM 0C72EC: collision pointer +4 of the layer map descriptor, 2x2 blocks per column pair; unknown cells filled, known cells kept')
    (a.output or a.collision).write_text(json.dumps(d,separators=(',',':'))+'\n')
    print('%s: %d known cells equal, %d conflicts kept, %d filled, %d unknown left'%(a.collision,ok,len(conflicts),filled,d['unknown_cells']))
if __name__=='__main__':main()
