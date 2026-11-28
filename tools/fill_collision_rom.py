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
keeps). Known cells are never changed; only unknown cells are filled,
except a conflict explained by --patch: a sub-map the game copies later
with 0C70FC (e.g. 0B7350, Mission 2 stage 2 gate: 0B706C at block (0xA2,
0x2F)); the observed cell equals the patch's, so it was seen after the
patch and the grid takes the ROM value (the patch is made at run time)."""
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
    ap.add_argument('--rom-rows',action='store_true',help='after the check, the grid becomes the ROM grid itself (rows 0..TOP, no unknown cell): the rows above it were the ring of a looping stage (stage 3 tower, writer 0B7C38)')
    ap.add_argument('--column',type=int,default=0,help='world column of ROM column 0 (Mission 3: the swapped map 0B8550 starts at column 520)')
    ap.add_argument('--columns',type=int,help='keep only the first N columns (Mission 3: the observed grid has wrapped columns past the stage)')
    ap.add_argument('--outside-empty',action='store_true',help='last fill (Mission 3): the unknown cells left are outside every ROM grid of the stage (rows above/below the map, columns of 0B8600 without collision pointer) and become empty')
    ap.add_argument('--patch',action='append',default=[],help='SUBMAP:D4:D5 (hex): 0C70FC sub-map written at run time to block column D4, block row D5 (top world row 2*D5+1); conflicts it explains take the ROM value')
    ap.add_argument('--output',type=Path)
    a=ap.parse_args();d=json.loads(a.collision.read_text());C,R=d['columns'],d['rows']
    cells=bytearray.fromhex(d['cells_hex']);unk=bytearray.fromhex(d['unknown_mask_hex']);mut=bytes.fromhex(d['mutable_mask_hex'])
    if a.columns and a.columns<C:
        cells,unk,mut=cells[:a.columns*R],unk[:a.columns*R],mut[:a.columns*R];C=d['columns']=a.columns
        d['mutable_cells']=[m for m in d.get('mutable_cells',[]) if m['x']//8<C] if isinstance(d.get('mutable_cells'),list) else d.get('mutable_cells')
    g0,gc,gr=rom_grid(Program(a.rom),a.descriptor)
    # Columns shifted by --column: g[x*gr+r] is world column x (empty before it).
    g=bytearray(a.column*gr)+g0;gc+=a.column
    lo,hi=(int(x) for x in a.rows.split(':')) if a.rows else (a.top-gr+1,a.top)
    ok=0;conflicts=[];filled=0
    for x in range(a.column,min(C,gc)):
        for y in range(max(0,lo),min(R-1,hi)+1):
            r=a.top-y
            if not 0<=r<gr:continue
            i=x*R+y
            if unk[i]:continue
            if mut[i]:continue
            if cells[i]==g[x*gr+r]:ok+=1
            else:conflicts.append([x,y,cells[i],g[x*gr+r]])
    prog=Program(a.rom);patched=[]
    for spec in a.patch:
        sub,d4,d5=(int(v,16) for v in spec.split(':'))
        _,pc,pw,ph=struct.unpack_from('>IIHH',prog.fixed,sub);po=pc-0x100000
        column,top=2*d4,2*d5+1
        for cf in list(conflicts):
            x,y=cf[0],cf[1];p,b=(x-column)//2,(top-y)//2
            if not (0<=p<pw and 0<=b<ph):continue
            k=((x-column)&1)|(((top-y)&1)<<1)
            if prog.p2[po+(p*ph+b)*4+k]!=cf[2]:continue
            cells[x*R+y]=cf[3];conflicts.remove(cf);patched.append(cf+['0x%06X'%sub])
    assert len(conflicts)<=a.allow_conflicts,('ROM grid disagrees with observed cells',len(conflicts),conflicts[:8])
    for x in range(a.column,min(C,gc)):
        for y in range(max(0,lo),min(R-1,hi)+1):
            r=a.top-y;i=x*R+y
            if 0<=r<gr and unk[i]:cells[i]=g[x*gr+r];unk[i]=0;filled+=1
    if a.rom_rows:
        # Column x, world row y = TOP - r for every ROM row r; observed rows
        # above TOP are dropped (with their mutable cells).
        R2=a.top+1;cols=min(C,gc);cells=bytearray(cols*R2);mm=bytearray(cols*R2)
        for x in range(cols):
            for y in range(R2):
                r=a.top-y
                if 0<=r<gr:cells[x*R2+y]=g[x*gr+r]
                if y<R:mm[x*R2+y]=mut[x*R+y] if isinstance(mut,(bytes,bytearray)) and len(mut)==C*R else 0
        unk=bytearray(cols*R2);d['columns'],d['rows']=cols,R2;d['mutable_mask_hex']=mm.hex()
        if isinstance(d.get('mutable_cells'),list):d['mutable_cells']=[m for m in d['mutable_cells'] if m['y']//8<R2]
    if a.outside_empty:
        d['outside_empty']=sum(1 for v in unk if v);unk=bytearray(len(unk))
    d['cells_hex']=cells.hex();d['unknown_mask_hex']=unk.hex();d['unknown_cells']=sum(1 for v in unk if v)
    d.setdefault('rom_fills',[]).append(dict(descriptor='0x%06X'%a.descriptor,column=a.column,top=a.top,rows=[lo,hi]));d['rom_fill']=dict(descriptor='0x%06X'%a.descriptor,top=a.top,rows=[lo,hi],rom_columns=gc,rom_rows=gr,checked=ok,filled=filled,conflicts=conflicts,patched=patched,
        rule='ROM 0C72EC: collision pointer +4 of the layer map descriptor, 2x2 blocks per column pair; unknown cells filled, known cells kept')
    (a.output or a.collision).write_text(json.dumps(d,separators=(',',':'))+'\n')
    print('%s: %d known cells equal, %d conflicts kept, %d explained by patches (ROM value), %d filled, %d unknown left'%(a.collision,ok,len(conflicts),len(patched),filled,d['unknown_cells']))
if __name__=='__main__':main()
