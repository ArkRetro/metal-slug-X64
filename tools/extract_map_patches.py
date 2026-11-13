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
# Mission 4 (neo_m4props.c): every patch the props 0B9F4A can make before the
# boss's road (callbacks 0BA288..0BA7AA, record +E of 0E68D0): the street's
# fronts 0..5 with both variants of each shared edge, one sub-map for the
# props 6..1A, 1C and 1E at their record's column/row (+6/+8), two for 1B
# and 1D (the second hit). The copies of the stage map 0E6DA8 (prop 1F and
# the callbacks +1C, 0x20 columns further each time) at the props' own
# columns: past the map's end (block 344) the runtime records each copy at
# the column 32 blocks back whose ring slot it writes (neo_world.c
# g_map_patch, neo_world_terrain_end), with the columns the window clip let
# through: 1C 0BA240 (70, 1E, 7, 5), 1D 0BA264 (78, 1E, 4, 5) and 0BA218
# (78, 1E, 10, 5) (one entry: same key and target, the record keeps the 4
# columns of the first), 1E 0BA270 (88, 1A, 4, 9), 1F 0BA27C (8C, 1F, 4, 4)
# and 1F's own patch 0BA7C2 (5A, 1F, 4, 4).
MISSION4_ROAD=[(0xe6da8,(0x70,0x1e,7,5),0x1c),(0xe6da8,(0x78,0x1e,0x10,5),0x1d),(0xe6da8,(0x88,0x1a,4,9),0x1e),
               (0xe6da8,(0x8c,0x1f,4,4),0x1f),(0xe6da8,(0x5a,0x1f,4,4),0x1f)]
def mission4_patches(rom):
    T=0x10c72e
    out=[(T,0xe6e16,(0,0,3,8,0x2a,0xb)),(T,0xe6e42,(0,0,3,8,0x2a,0xb)),(T,0xe6e00,(0,0,6,8,0x24,0xb)),(T,0xe6e2c,(0,0,3,8,0x2a,0xb)),
         (T,0xe6e6e,(0,0,2,8,0x31,0xb)),(T,0xe6e9a,(0,0,2,8,0x31,0xb)),(T,0xe6e58,(0,0,4,8,0x2d,0xb)),(T,0xe6e84,(0,0,2,8,0x31,0xb)),
         (T,0xe6ec6,(0,0,3,8,0x35,0xb)),(T,0xe6ef2,(0,0,3,8,0x35,0xb)),(T,0xe6eb0,(0,0,2,8,0x33,0xb)),(T,0xe6edc,(0,0,3,8,0x35,0xb)),
         (T,0xe6f1e,(0,0,3,0xc,0x3f,0xf)),(T,0xe6f4a,(0,0,3,0xc,0x3f,0xf)),(T,0xe6f08,(0,0,7,8,0x38,0xb)),(T,0xe6f34,(0,0,3,0xc,0x3f,0xf)),
         (T,0xe6f76,(0,0,3,0xc,0x4a,0xf)),(T,0xe6fa2,(0,0,3,0xc,0x4a,0xf)),(T,0xe6f60,(0,0,8,0xc,0x42,0xf)),(T,0xe6f8c,(0,0,3,0xc,0x4a,0xf)),
         (T,0xe6fb8,(0,0,7,0xc,0x4d,0xf))]
    one={6:0xe6fce,7:0xe6fe4,8:0xe6ffa,9:0xe7010,0xa:0xe7026,0xb:0xe70aa,0xc:0xe70c0,0xd:0xe70d6,0xe:0xe6ffa,0xf:0xe7010,0x10:0xe70ec,
         0x11:0xe70c0,0x12:0xe7102,0x13:0xe703c,0x14:0xe6ffa,0x15:0xe7010,0x16:0xe7118,0x17:0xe712e,0x18:0xe7144,0x19:0xe715a,0x1a:0xe7052,
         0x1b:(0xe7068,0xe7170),0x1c:0xe7186,0x1d:(0xe707e,0xe719c),0x1e:0xe7094}
    s16=lambda v:v-0x10000 if v&0x8000 else v
    for n,subs in sorted(one.items()):
        rec=0xe68d0+0x24*n;col,row=rom.u16(rec+6),s16(rom.u16(rec+8))
        for sub in (subs if isinstance(subs,tuple) else (subs,)):
            out.append((T,sub,(0,0,rom.u16(sub+8),rom.u16(sub+10),col,row)))
    for sub,src,n in MISSION4_ROAD:
        rec=0xe68d0+0x24*n;out.append((T,sub,src+(rom.u16(rec+6),s16(rom.u16(rec+8)))))
    return out
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
    p.add_argument('--palette-capture',type=Path,help='ROM model: capture directory with palette<frame>.bin, for the palette numbers no block of the map uses (as tools/export_stage_rom.py)')
    p.add_argument('--palette-frames',default='',help='MAP:FRAME,... (hex map)')
    a=p.parse_args()
    rom=Rom(a.rom)
    model=json.loads((a.stage/'map.json').read_text())
    vram=pal=None
    if a.capture_frame:
        import struct
        vram=struct.unpack('>34304H',a.capture_frame.read_bytes())
        pal=struct.unpack('>4096H',a.capture_frame.with_name(a.capture_frame.name.replace('vram','palette')).read_bytes())
    todo=PATCHES.get((a.mission,a.stage.name),[])
    if 'palette_numbers' in model:
        # A ROM model (tools/export_stage_rom.py, Mission 4): the blocks at
        # the placement of that model (no vertical offset) with the colour
        # set of their palette number in the map they are copied into; the
        # observed model only counts how many it saw at that place.
        todo=mission4_patches(rom) if a.mission==4 else todo
        pal_frames={k.upper():int(v) for k,v in (x.split(':') for x in a.palette_frames.split(',') if x)}
        seen=set()
        if (a.stage/'map_observed.json').exists():
            ob=json.loads((a.stage/'map_observed.json').read_text())
            for c in ob['cells']:seen.add((c['layer'],c['x'],c['y'],c['code'],c['flip']))
            for c in ob['conflicts']:
                for ch in c.get('choices',()):seen.add((c['layer'],c['x'],c['y'],ch['code'],ch['flip']))
        out=[];report=[]
        for obj,submap,d in todo:
            layer=LAYERS[obj];mapname='0E6D7C' if d[4]<0xc8 else '0E6DA8';numbers=model['palette_numbers'][mapname];cells=[];from_ram=set()
            for col,row,code,attr in blocks(rom,submap,d):
                mask=7 if attr&8 else 3 if attr&4 else 0
                if not code&0xffff and not attr:continue
                n=attr>>8
                if str(n) in numbers:colors=model['palettes'][numbers[str(n)]]
                else:
                    # Not used by the map's own blocks: the palette RAM of the capture (export_stage_rom.py checked
                    # that every number the map uses has there the colours MAME drew).
                    assert mapname in pal_frames and a.palette_capture,('palette number without colours',hex(submap),n)
                    raw=(a.palette_capture/('palette%d.bin'%pal_frames[mapname])).read_bytes()
                    colors=[0]+[int.from_bytes(raw[32*n+2*k:32*n+2*k+2],'big') for k in range(1,16)];from_ram.add(n)
                cells.append(dict(layer=layer,x=16*(d[4]+col),y=240-16*(d[5]+1)+16*row,code=code&~mask,flip=attr&3,animation_mask=mask,
                                  palette_number=n,colors=colors))
            assert cells,('empty patch',hex(submap))
            observed=sum(1 for c in cells if (c['layer'],c['x'],c['y'],c['code'],c['flip']) in seen)
            out.append(dict(layer_object='%06X'%obj,submap='%06X'%submap,d=list(d),layer=layer,cells=cells))
            report.append(dict(submap='%06X'%submap,d=list(d),blocks=len(cells),observed=observed,palette_numbers_from_palette_ram=sorted(from_ram)))
        (a.stage/'patches.json').write_text(json.dumps(dict(source='tools/extract_map_patches.py: ROM sub-maps (0C70FC tile part), colours of the palette numbers of the ROM model',patches=out),indent=0)+'\n')
        print(json.dumps(report));return
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
