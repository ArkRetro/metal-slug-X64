#!/usr/bin/env python3
"""Tile part of the map patches 0C70FC of a mission stage (B37 = B15).

0C70FC copies a sub-map (header: +0 tile data, +4 collision cells, +8
width, +A height, in 16-pixel blocks) into the layer object's window: the
tile words (code, attribute) go straight to the layer's SCB1 entries, so the
patched blocks are drawn until the layer reloads those columns. The ROM
gives the codes and attributes; this tool turns them into map cells of the
stage model (filesystem/missionN/sK/map.json coordinates) for
tools/pack_mission.py:

* x = 16 * (D4 + column), y = 240 - 16 * (D5 + 1) + 16 * row - the layer's
  vertical offset (map.json observations; D5 counts block rows from the
  bottom: the collision part writes from cell row 2 * D5 + 1). Checked by
  the observed blocks: the MAME capture saw the patched screen, so most
  blocks equal, at their position, an observed cell or coordinate variant of
  the model with the same code (at least half of each patch, or the tool
  fails).
* colours: those of the observed cell/variant with the same code and
  position; a block never observed takes the colours its palette number
  (attribute >> 8) has in the observed blocks of the same patch.
* attribute bits 0-1 flip, bit 2/3 auto-animation over 4/8 codes.

Output: <stage>/patches.json (private, ROM-derived): per patch the layer
object, the sub-map, D0..D5, the model layer and its cells."""
import argparse,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
# Layer objects -> model layers (tools/extract_mission_map.py maps_slots).
LAYERS={0x10e83e:0,0x10c72e:3}
# The patches each stage makes (ROM provenance), D0..D5 as 0C70FC takes them.
PATCHES={
    (2,'s1'):[(0x10c72e,0x0b6bec,(0,0,7,7,0x21,0xa)),     # 0B6E4A (gate 0B6E06)
              (0x10c72e,0x0b6c02,(0,0,7,4,0x21,3)),       # 0B6E74
              (0x10e83e,0x0b6c18,(0,0,7,4,0x21,3))],
    (2,'s2'):[(0x10c72e,0x0b706c,(0,0,8,0xa,0xa2,0x2f))], # 0B7350 (door 03A6EA)
    # 0B7A2C (boulder 03A2AA): source offsets D0/D1 in the 8x9 sub-map
    # 0B7436; its fifth patch (2, 9, 4, 1, 0x1B, 0x1D) starts below the
    # sub-map's last row and copies nothing (0C714E).
    (2,'s3'):[(0x10c72e,0x0b7436,(1,1,6,3,0x1a,0x25)),
              (0x10c72e,0x0b7436,(0,4,7,1,0x19,0x22)),
              (0x10c72e,0x0b7436,(1,5,6,2,0x1a,0x21)),
              (0x10c72e,0x0b7436,(2,7,5,2,0x1b,0x1f))],
    # The ledges of the wagons (stampers 0B8FA8 on the sub-map 0B85EA,
    # neo_m3props.c): each program copies a few 4x4 frames to one target
    # (the platform coming out); only the last frame stays, so the pack
    # keeps that one. Boxes 0B911E / 0B9140 (program 0B9090, last record
    # (C, 8)); the lever doors' 0B90BC / 0B9106 (0B9040, (8, 0)) and
    # 0B90FA / 0B9112 (0B9064, (14, 0)).
    (3,'s1'):[(0x10c72e,0x0b85ea,(0xc,8,4,4,0x74,0xb)),
              (0x10c72e,0x0b85ea,(0xc,8,4,4,0x84,0xb)),
              (0x10c72e,0x0b85ea,(8,0,4,4,0x95,8)),
              (0x10c72e,0x0b85ea,(0x14,0,4,4,0x9e,8)),
              (0x10c72e,0x0b85ea,(8,0,4,4,0xa3,8)),
              (0x10c72e,0x0b85ea,(0x14,0,4,4,0xac,8))],
}
def swap(d):
    b=bytearray(d);b[0::2],b[1::2]=d[1::2],d[0::2];return bytes(b)
class Rom:
    def __init__(self,path):
        z=zipfile.ZipFile(path);self.p1=swap(z.read('250-p1.p1'));self.p2=swap(z.read('250-p2.ep1'))
    def read(self,a,n):
        # P1 below 0x100000; above, the linear P2 (0C71CC..0C71EC: the bank is
        # the address's second hex digit - 1, mapped at 0x200000).
        if a<0x100000:return self.p1[a:a+n]
        o=a-0x100000;assert o+n<=len(self.p2),hex(a);return self.p2[o:o+n]
    def u16(self,a):return int.from_bytes(self.read(a,2),'big')
    def u32(self,a):return int.from_bytes(self.read(a,4),'big')
def blocks(rom,submap,d):
    d0,d1,d2,d3,d4,d5=d
    tiles=rom.u32(submap);width=rom.u16(submap+8);height=rom.u16(submap+10)
    assert tiles not in (0,0xffffffff),('no tile part',hex(submap))
    assert d0+d2<=width and d1+d3<=height,('outside the sub-map',hex(submap))
    out=[]
    for c in range(d2):
        for r in range(d3):
            w=rom.read(tiles+((d0+c)*height+d1+r)*4,4)
            code,attr=int.from_bytes(w[:2],'big'),int.from_bytes(w[2:],'big')
            out.append((c,r,code|((attr>>4)&15)<<16,attr))
    return out
def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('mission',type=int);p.add_argument('stage',type=Path)
    p.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    p.add_argument('--capture-frame',type=Path,help='vram<f>.bin of a MAME capture after the patches (capture_fullgame.lua, '
        'palette<f>.bin beside it): a patch whose blocks the stage model never observed (the stage-3 boulder 0B7A2C) is '
        'checked by its columns in the SCB1 words (code and attribute, in order) and takes the colours of the palette RAM')
    a=p.parse_args()
    rom=Rom(a.rom)
    model=json.loads((a.stage/'map.json').read_text())
    vram=pal=None
    if a.capture_frame:
        import struct
        vram=struct.unpack('>34304H',a.capture_frame.read_bytes())
        pal=struct.unpack('>4096H',a.capture_frame.with_name(a.capture_frame.name.replace('vram','palette')).read_bytes())
    todo=PATCHES.get((a.mission,a.stage.name),[])
    if not todo:print('[]');return
    # Each layer's vertical offset relative to the terrain layer 3 (the
    # observations' offsets also carry the camera's rise).
    voff={}
    for L in {LAYERS[obj] for obj,_,_ in todo}:
        rel={o['vertical_offsets'][L]-o['vertical_offsets'][3] for o in model['observations'] if 'vertical_offsets' in o}
        assert len(rel)<=1,('layer offset changes',L,rel)
        voff[L]=rel.pop() if rel else 0
    seen={}
    for c in model['cells']:
        seen.setdefault((c['layer'],c['x'],c['y'],c['code'],c['flip']),[]).append(model['palettes'][c['palette']])
    for c in model['conflicts']:
        for ch in c.get('choices',()):seen.setdefault((c['layer'],c['x'],c['y'],ch['code'],ch['flip']),[]).append(ch['palette'])
    out=[];report=[]
    for obj,submap,d in todo:
        layer=LAYERS[obj];cells=[];bypal={};missing=[]
        for col,row,code,attr in blocks(rom,submap,d):
            x=16*(d[4]+col);y=240-16*(d[5]+1)+16*row-voff[layer];flip=attr&3
            mask=7 if attr&8 else 3 if attr&4 else 0
            obs=seen.get((layer,x,y,code,flip))
            cell=dict(layer=layer,x=x,y=y,code=code,flip=flip,animation_mask=mask,palette_number=attr>>8)
            if obs:
                cell['colors']=obs[-1];bypal.setdefault(attr>>8,set()).add(tuple(obs[-1]))
            else:missing.append(cell)
            cells.append(cell)
        for n,cs in bypal.items():
            if pal:assert cs=={tuple(pal[16*n:16*n+16])},('palette RAM and observed colours differ',hex(submap),hex(n))
        seen_cols=0;columns=0
        if vram:
            # Each column of the patch as consecutive SCB1 words of one
            # sprite slot (the original copies the sub-map's words there).
            cols={}
            for col,row,code,attr in blocks(rom,submap,d):cols.setdefault(col,[]).append((code&0xffff,attr))
            for seq in cols.values():
                columns+=1
                seen_cols+=any(all(vram[sl*64+2*(i+k)]==seq[k][0] and vram[sl*64+2*(i+k)+1]==seq[k][1] for k in range(len(seq)))
                               for sl in range(1,381) for i in range(32-len(seq)+1))
            for cell in missing:cell['colors']=list(pal[16*cell['palette_number']:16*cell['palette_number']+16])
            missing=[c for c in missing if 'colors' not in c]
        for cell in missing:
            got=bypal.get(cell['palette_number'],set())
            assert len(got)==1,('block never observed and its palette ambiguous',hex(submap),cell['x'],cell['y'],len(got))
            cell['colors']=list(next(iter(got)))
        observed=sum(1 for c in cells if (layer,c['x'],c['y'],c['code'],c['flip']) in seen)
        assert 2*observed>=len(cells) or (vram and 2*seen_cols>=columns),('patch not observed at its position',hex(submap))
        for n,cs in bypal.items():assert len(cs)==1,('palette %X has several colour sets'%n,hex(submap))
        out.append(dict(layer_object='%06X'%obj,submap='%06X'%submap,d=list(d),layer=layer,cells=cells))
        report.append(dict(submap='%06X'%submap,d=list(d),layer=layer,blocks=len(cells),observed=observed,**({'vram_columns':[seen_cols,columns]} if vram else {}),palettes=sorted('%X'%n for n in {c['palette_number'] for c in cells})))
    (a.stage/'patches.json').write_text(json.dumps(dict(source='tools/extract_map_patches.py: ROM sub-maps (0C70FC tile part), colours of the observed cells',patches=out),indent=0)+'\n')
    print(json.dumps(report))
if __name__=='__main__':main()
