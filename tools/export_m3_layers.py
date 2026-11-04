#!/usr/bin/env python3
"""Mission 3: the complete ROM maps of every layer object (docs/MISION3.md,
"Reconocimiento del mapa"), private output filesystem/mission3/s1/layers_rom.json.

The camera list 0B8968 (op 0x18: layer object, block column, top block row,
map descriptor) and the layer objects' RAM (run4 layers.csv) name the maps;
each map descriptor (+0 tiles, +4 collision, +8 block columns, +A block rows;
P2 linear addresses) is read whole from the ROM, blocks placed at
x = 16 * (column + i), y = y0 + 16 * j (model coordinates of
tools/extract_mission_map.py: the foreground y0 = 240 - 16 * (top + 1), the
layers with the vertical offset 8 one more block row up). The placement of
every map is checked against the cells MAME showed (map.json cells, swapped
cells and variants): the tool stops when a placed block disagrees with an
observed one at the same position (the train, whose columns loop, and the
overlay, moved by an actor, are checked by code identity instead).

Colours: the palette RAM of run4 (palette<f>.bin) at the middle snapshot of
each section (tools/extract_m3_recon.py sections) for every palette number
the maps use; checked against the observed cells' colours.

Collision: the 2 x 2 cells of each block (ROM 0C72EC), kernel rows (row 0 =
the kernel's y 0, as the pack's grid after tools/rebase_collision.py)."""
import argparse,gzip,json,struct,sys
from collections import Counter
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from extract_map_patches import Rom
ROOT=Path(__file__).resolve().parents[1]
# (model layer, layer object, descriptor, block column, top block row, model
# x0 (px), model y0 (px), camera progress from which the map is loaded, note)
MAPS=[(3,0x10c72e,0x0b853a,0x000,0x12,0,-64,0,'camera list 0B898E'),
      (3,0x10c72e,0x0b8600,0x0f0,0x12,3840,-64,0xd40,'camera list 0B8BB8: no tiles, no collision (FFFFFFFF): the columns it loads stay empty'),
      (3,0x10c72e,0x0b8550,0x104,0x0c,4160,32,0xe80,'camera list 0B8C1C'),
      (1,0x10d7b6,0x0b8566,0x000,0x13,0,-88,0,'camera list 0B89A2; the train: columns loop with the origin +32 (neo_train.c)'),
      (0,0x10e83e,0x0b857c,0x000,0x13,0,-88,0,'camera list 0B89B6; from progress 0x140 (0B8A6C) the map is 0 and another writer draws the sky'),
      (2,0x10e8c6,0x2a6142,None,None,None,None,None,'set by the actor 0A6C8C at camera 3944 (run4 132390..), moved vertically by it'),
      # The same actor patches the layer from four more sub-maps (0C70FC
      # lists at P2 2A620E..2A62FA: sub-map, D0..D5 bytes; 0A6F3C: 2A619A).
      (2,0x10e8c6,0x2a6158,None,None,'patch',None,None,'sub-map of the 0A6C8C patch list 2A620E'),
      (2,0x10e8c6,0x2a616e,None,None,'patch',None,None,'sub-map of the 0A6C8C patch list 2A6274'),
      (2,0x10e8c6,0x2a6184,None,None,'patch',None,None,'sub-map of the 0A6C8C patch list 2A62EC'),
      (2,0x10e8c6,0x2a619a,None,None,'patch',None,None,'sub-map of 0A6F3C')]
def blocks(rom,desc):
    t,c,w,h=rom.u32(desc),rom.u32(desc+4),rom.u16(desc+8),rom.u16(desc+10)
    tiles={};coll={}
    for i in range(w):
        for j in range(h):
            if t not in (0,0xffffffff):
                b=rom.read(t+(i*h+j)*4,4);code,attr=int.from_bytes(b[:2],'big'),int.from_bytes(b[2:],'big')
                tiles[(i,j)]=(code|((attr>>4)&15)<<16,attr)
            if c not in (0,0xffffffff):
                b=rom.read(c+(i*h+j)*4,4)
                for k in range(4):coll[(2*i+(k&1),2*h-1-2*j-(k>>1))]=b[k]
    return t,c,w,h,tiles,coll
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    ap.add_argument('--model',type=Path,default=ROOT/'filesystem/mission3/s1/map.json')
    ap.add_argument('--captures',type=Path,default=Path('/home/user/captures-fullgame/run4'))
    ap.add_argument('--fixture',type=Path,default=ROOT/'data/m3_map_fixture.json.gz',help='sections (tools/extract_m3_recon.py)')
    ap.add_argument('--output',type=Path,default=ROOT/'filesystem/mission3/s1/layers_rom.json')
    a=ap.parse_args();rom=Rom(a.rom);model=json.loads(a.model.read_text())
    sections=json.load(gzip.open(a.fixture))['sections']
    seen={}
    for c in model['cells']+model['swapped_cells']:seen.setdefault((c['layer'],c['x'],c['y']),set()).add((c['code'],c['flip']))
    for c in model['conflicts']:
        for ch in c.get('choices',[]):seen.setdefault((c['layer'],c['x'],c['y']),set()).add((ch['code'],ch['flip']))
    colours={}
    for c in model['cells']+model['swapped_cells']:colours.setdefault((c['layer'],c['x'],c['y'],c['code']),set()).add(tuple(model['palettes'][c['palette']]))
    out=[];used=set();report=[]
    for layer,obj,desc,col,top,x0,y0,cam,note in MAPS:
        t,c,w,h,tiles,coll=blocks(rom,desc)
        rec=dict(layer=layer,object='%06X'%obj,descriptor='%06X'%desc,tiles='%06X'%t,collision='%06X'%c,block_columns=w,block_rows=h,note=note)
        if x0=='patch':
            # Placed by the actor's patches (D4/D5 of each list entry): kept
            # in sub-map coordinates (block i, j at 16 i, 16 j).
            x0=y0=0;rec['placement']='0C70FC sub-map (actor)'
        elif x0 is None:
            # Placement by an actor: find it from the observed cells (code identity).
            best=max(((sum(1 for (i,j),(code,attr) in tiles.items() if (code,attr&3) in seen.get((layer,dx+16*i,dy+16*j),())),dx,dy)
                      for dx in range(-320,320,16) for dy in range(-320,320,8)))
            n,x0,y0=best;rec['placement']='observed (actor)'
        else:
            rec.update(block_column=col,top_block_row=top,camera_progress=cam)
        rec.update(x0=x0,y0=y0)
        same=diff=0
        for (i,j),(code,attr) in tiles.items():
            if rec.get('placement','').startswith('0C70FC'):break
            o=seen.get((layer,x0+16*i,y0+16*j))
            if o is None:continue
            if (code,attr&3) in o:same+=1
            else:diff+=1
        if rec.get('placement','').startswith('0C70FC'):same=diff=None
        rec['observed_equal']=same;rec['observed_other']=diff
        if layer in (0,3):assert diff==0,(hex(desc),same,diff)
        rec['cells']=[[x0+16*i,y0+16*j,code,attr&3,7 if attr&8 else 3 if attr&4 else 0,attr>>8] for (i,j),(code,attr) in sorted(tiles.items())]
        used.update(attr>>8 for code,attr in tiles.values())
        if coll:
            rows=2*h;rec['collision_rows']=rows
            # Kernel rows: the map's bottom cell row is 2 * (top + 1 - h) (0C72EC
            # writes block row j of a map with top block row T at rows 2(T - j), 2(T - j) + 1).
            r0=2*(top+1-h) if top is not None else 0
            rec['collision_column0']=2*col if col is not None else 0;rec['collision_row0']=r0
            rec['collision_hex']=bytes(coll[(x,y)] for x in range(2*w) for y in range(rows)).hex()
        out.append(rec)
        report.append({k:rec[k] for k in ('layer','descriptor','block_columns','block_rows','x0','y0','observed_equal','observed_other')})
    # Palettes per section (palette RAM of the middle snapshot).
    frames=sorted(int(p.stem[7:]) for p in a.captures.glob('palette*.bin'))
    pals=[]
    for s in sections:
        inside=[f for f in frames if s['frames'][0]<=f<=s['frames'][1]]
        f=inside[len(inside)//2]
        ram=struct.unpack('>4096H',(a.captures/f'palette{f}.bin').read_bytes())
        pals.append(dict(section=s['id'],frame=f,palettes={'%02X'%n:list(ram[16*n:16*n+16]) for n in sorted(used)}))
    # Colour check: observed cells equal to a ROM block take one of the
    # section palettes of its number.
    ok=bad=0
    for rec in out:
        for x,y,code,flip,anim,n in rec['cells']:
            for cs in colours.get((rec['layer'],x,y,code),()):
                if any(tuple(p['palettes']['%02X'%n])==cs for p in pals):ok+=1
                else:bad+=1
    doc=dict(source='tools/export_m3_layers.py: ROM map descriptors of the camera list 0B8968 and the layer objects; palette RAM of run4',
             maps=out,section_palettes=pals,colour_check=dict(equal=ok,other=bad))
    a.output.write_text(json.dumps(doc,separators=(',',':'))+'\n')
    for r in report:print(r)
    print('colours: %d observed cells take a section palette, %d other (palette changes in the section)'%(ok,bad))
if __name__=='__main__':main()
