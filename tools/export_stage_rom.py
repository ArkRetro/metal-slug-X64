#!/usr/bin/env python3
"""Stage collision grid and map blocks straight from the ROM (docs/MISION4.md).

The stage descriptor's camera list (+0) names every map the list loads: op
0x18 (0C8654) = layer object, block column (+32), top block row (+34), map
descriptor (+0 tile words, +4 collision cells, +8 block columns, +A block
rows; P2 linear addresses). Nothing is taken from a screen:

* Collision (the terrain object 10C72E): ROM 0C72EC copies the 2 x 2 cells
  of each block into the object's ring; block row j from the top of a map
  with top row T holds the kernel rows 2T + 1 - 2j and 2T - 2j, block column
  i the cell columns 2 (C + i) and 2 (C + i) + 1. The grid is the union of
  the list's maps in kernel rows (row 0 = the kernel's local y 0..7); a map
  that goes below y 0 gives a negative row origin (collision.json
  origin_cells[1], the pack's grid_origin_y, neo_collision_set_row_origin).
  --tail N appends N columns after the last map: the arcade's ring keeps in
  the slot of a column past the map the column 64 before it (0C714E clips
  the load), as tools/rebase_collision.py --ring-tail.
* --observed COLLISION (tools/extract_region_collision.py --descriptor-layers
  --layer-camera of a MAME capture): every known cell that no event wrote
  must equal the ROM cell at its place, or the tool stops.
* --model MAP (tools/extract_mission_map.py --ram-layers): every observed
  block of a map with tiles must be the ROM block at its place (code with
  its bank bits, flips, auto-animation bits), x = 16 (C + i), y = 240 - 16
  (T + 1) + 16 j, the layers 0/1 of the model 8 px higher; the looping
  columns past a map's end are compared with the column 32 before (the
  ring). The report counts them per map; nothing is fitted.

Output: collision.json for tools/pack_mission.py and --report (private)."""
import argparse,json,sys
from collections import Counter
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from extract_map_patches import Rom
OBJECTS={0x10c72e:3,0x10d7b6:1,0x10e83e:0,0x10e8c6:2}   # pack layers (tools/pack_mission.py LAYER_OBJECTS)
def camera_maps(rom,descriptor):
    a=rom.u32(descriptor);target=0;out=[]
    for _ in range(4000):
        op,n=rom.u16(a),rom.u16(a+2)
        if op==8:break
        assert n and n<0x40,('camera list',hex(a),op,n)
        if op==0:target=rom.u16(a+4)
        if op==0x18:
            s16=lambda v:v-65536 if v>=32768 else v
            out.append(dict(at=a,progress=target,object=rom.u32(a+4),column=s16(rom.u16(a+8)),top=s16(rom.u16(a+10)),map=rom.u32(a+12),table=rom.u32(a+16),ring=False))
        if op==0x48:
            # 0C73CE (object, D0, D1, D2 columns, D3 rows): the map's first 32 columns copied once into the 32 sprite columns: the picture repeats every 512 px.
            for l in out[::-1]:
                if l['object']==rom.u32(a+4):l['ring']=(rom.u16(a+8),rom.u16(a+10),rom.u16(a+12))==(0,0,0x20);break
        a+=n
    return out
def map_blocks(rom,desc):
    t,c,w,h=rom.u32(desc),rom.u32(desc+4),rom.u16(desc+8),rom.u16(desc+10)
    tiles={};coll={}
    for i in range(w):
        for j in range(h):
            if t not in (0,0xffffffff):
                b=rom.read(t+(i*h+j)*4,4);code,attr=int.from_bytes(b[:2],'big'),int.from_bytes(b[2:],'big')
                tiles[(i,j)]=(code|((attr>>4)&15)<<16,attr)
            if c not in (0,0xffffffff):
                b=rom.read(c+(i*h+j)*4,4)
                # 0C72EC: 4 bytes per block, (x, r) (x+1, r) (x, r+1) (x+1, r+1), blocks and r from the map's top downwards.
                for k in range(4):coll[(2*i+(k&1),2*j+(k>>1))]=b[k]
    return t,c,w,h,tiles,coll
def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('descriptor',type=lambda v:int(v,16));ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    ap.add_argument('--output',type=Path,required=True,help='collision.json');ap.add_argument('--tail',type=int,default=0)
    ap.add_argument('--observed',type=Path);ap.add_argument('--model',type=Path);ap.add_argument('--report',type=Path);ap.add_argument('--keep-going',action='store_true')
    ap.add_argument('--palette-capture',type=Path,help='capture directory with palette<frame>.bin (8192 bytes: 256 palettes of 16 big-endian words): colours of the palette numbers no observed block used, from the snapshot --palette-frames MAP:FRAME,...; every observed palette number of the map must have the same colours there')
    ap.add_argument('--palette-frames',default='')
    ap.add_argument('--rom-model',type=Path,help='map.json for tools/pack_mission.py built from the ROM blocks');ap.add_argument('--camera-max',type=int);ap.add_argument('--outside-limit',type=int,default=1<<30,help='terrain blocks past the last map kept up to this x')
    a=ap.parse_args();rom=Rom(a.rom)
    loads=camera_maps(rom,a.descriptor);rep=dict(descriptor='%06X'%a.descriptor,camera_list='%06X'%rom.u32(a.descriptor),loads=[])
    cells={};last=0
    for l in loads:
        t,c,w,h,tiles,coll=map_blocks(rom,l['map']) if l['map'] else (0,0,0,0,{},{})
        l.update(tiles_at=t,collision_at=c,width=w,height=h,tiles=tiles)
        rep['loads'].append(dict(at='%06X'%l['at'],progress=l['progress'],object='%06X'%l['object'],column=l['column'],top=l['top'],map='%06X'%l['map'],
                                 column_table='%06X'%l['table'],tiles='%06X'%t,collision='%06X'%c,blocks=[w,h]))
        if l['object']==0x10c72e and coll:
            # The map's top cell row is the kernel row 2 T + 1.
            for (x,r),v in coll.items():
                key=(2*l['column']+x,2*l['top']+1-r)
                assert key not in cells,('two maps at one cell',key)
                cells[key]=v
            last=max(last,2*(l['column']+w))
    row0=min(r for _,r in cells);rows=max(r for _,r in cells)-row0+1;columns=last+a.tail
    grid=bytearray(columns*rows)
    for (x,r),v in cells.items():grid[x*rows+r-row0]=v
    for x in range(last,columns):grid[x*rows:(x+1)*rows]=grid[(x-64)*rows:(x-63)*rows]
    doc=dict(source='tools/export_stage_rom.py: ROM collision cells of the maps the camera list %06X loads into 10C72E (op 0x18), kernel rows'%rom.u32(a.descriptor),
             origin_cells=[0,row0],columns=columns,rows=rows,cells_hex=bytes(grid).hex(),unknown_cells=0,map_columns=last,ring_tail=a.tail)
    rep['grid']=dict(columns=columns,rows=rows,row_origin=row0,nonempty=sum(1 for v in grid if v),ring_tail=a.tail)
    if a.observed:
        o=json.loads(a.observed.read_text());oc,orows=o['columns'],o['rows'];ox,oy=o['origin_cells']
        g=bytes.fromhex(o['cells_hex']);unk=bytes.fromhex(o['unknown_mask_hex']);mut=bytes.fromhex(o['mutable_mask_hex']) if o.get('mutable_mask_hex') else bytes(len(g))
        same=diff=outside=mutable=0;bad=[]
        for x in range(oc):
            for r in range(orows):
                i=x*orows+r
                if unk[i]:continue
                if mut[i]:mutable+=1;continue
                gx,gr=x+ox,r+oy-row0
                if not (0<=gx<columns and 0<=gr<rows):
                    if g[i]:bad.append((gx,r+oy,g[i],None))
                    outside+=1;continue
                if grid[gx*rows+gr]==g[i]:same+=1
                else:diff+=1;bad.append((gx,r+oy,g[i],grid[gx*rows+gr]))
        rep['observed']=dict(source=str(a.observed),equal=same,different=diff,outside_empty=outside,event_cells=mutable,first_different=bad[:20])
        print('collision: %d observed cells equal, %d different, %d outside the ROM grid (empty), %d event cells left out'%(same,diff,outside,mutable))
        if bad and not a.keep_going:raise SystemExit(("observed cells differ from the ROM",bad[:8]))
    if a.model:
        m=json.loads(a.model.read_text());res=[]
        by_map={}
        for l in loads:
            if l['tiles']:by_map.setdefault((OBJECTS[l['object']],l['map']),l)
        first={}
        for l in loads:
            if l['tiles']:first.setdefault(OBJECTS[l['object']],l['map'])
        # Model layers (tools/neo_scene.py): 0 = 10E83E, 3 = 10C72E, 1 or 2 = 10D7B6.
        def check(cellset,layer,l,name):
            same=diff=loop=out=0;bad=[];y0=240-16*(l['top']+1)-(8 if layer<2 else 0)
            for c in cellset:
                i,j=c['x']//16-l['column'],(c['y']-y0)//16
                assert c['x']%16==0 and (c['y']-y0)%16==0,('block grid',c['x'],c['y'],y0)
                ring=False
                while i>=l['width'] and l['width']>=32:i-=32;ring=True
                b=l['tiles'].get((i,j))
                if b is None:out+=1;continue
                code,attr=b
                if code==c['code'] and (attr&3)==c['flip']:
                    same+=1;loop+=ring
                else:
                    diff+=1
                    if len(bad)<12:bad.append((c['x'],c['y'],'%X'%c['code'],'%X'%code,c['flip'],attr&3))
            res.append(dict(set=name,model_layer=layer,map='%06X'%l['map'],equal=same,of_them_ring_columns=loop,different=diff,outside_map=out,first_different=bad))
            print('map %06X (model layer %d, %s): %d blocks equal (%d in ring columns past the map), %d different, %d outside the map'%(l['map'],layer,name,same,loop,diff,out))
        layers={c['layer'] for c in m['cells']}
        for layer in sorted(layers):
            pack=1 if layer in (1,2) else layer
            if pack in first:check([c for c in m['cells'] if c['layer']==layer],layer,by_map[(pack,first[pack])],'cells')
        maps={(c['layer'],c['map']) for c in m['swapped_cells']}
        for layer,mp in sorted(maps):
            pack=1 if layer in (1,2) else layer
            l=by_map.get((pack,int(mp,16)))
            if l is None:
                res.append(dict(set='swapped',model_layer=layer,map=mp[2:],note='not a map of the camera list (loaded by an actor)',blocks=sum(1 for c in m['swapped_cells'] if (c['layer'],c['map'])==(layer,mp))))
                continue
            check([c for c in m['swapped_cells'] if (c['layer'],c['map'])==(layer,mp)],layer,l,'swapped')
        rep['model']=res
    if a.rom_model:
        assert a.model,'--rom-model needs the observed --model for the colours'
        m=json.loads(a.model.read_text());mid_layer=1 if any(l['object']==0x10e8c6 and l['map'] for l in loads) else 2
        model_layer={0x10c72e:3,0x10d7b6:mid_layer,0x10e83e:0,0x10e8c6:2}
        firsts={}
        for l in loads:
            if l['tiles']:firsts.setdefault(l['object'],l['map'])
        obs={}
        for c in m['cells']:
            for o,ml in model_layer.items():
                if ml==c['layer'] and o in firsts:obs.setdefault(firsts[o],[]).append(c)
        for c in m['swapped_cells']:obs.setdefault(int(c['map'],16),[]).append(c)
        def table_rows(table,column):
            # Column table (+3E of the layer object, op 0x18's last long): (first block column, word) pairs to FFFF; the word's
            # high byte is the first block row drawn and the low byte the number of rows (docs/MISION4.md). No table: every row.
            if not table:return (0,256)
            at=table;cur=(0,0)
            while rom.u16(at)!=0xffff:
                if rom.u16(at)<=column:cur=(rom.u16(at+2)>>8,rom.u16(at+2)&255)
                at+=4
            return cur
        mask=lambda attr:7 if attr&8 else 3 if attr&4 else 0
        cells=[];palettes=list(m['palettes']);stats=[];numbers={};pal_frames={k.upper():int(v) for k,v in (x.split(':') for x in a.palette_frames.split(',') if x)}
        for l in loads:
            if not l['tiles']:continue
            layer=model_layer[l['object']];y0=240-16*(l['top']+1)-(8 if layer<2 else 0)
            votes={};seen=0;hidden=0;outside=[]
            for c in obs.get(l['map'],[]):
                i,j=c['x']//16-l['column'],(c['y']-y0)//16
                while l['ring'] and i>=l['width']:i-=32
                while not l['ring'] and i>=l['width'] and l['width']>=32 and l['object']==0x10c72e:i-=32
                b=l['tiles'].get((i,j))
                if b is None:outside.append(c);continue
                if b[0]&~mask(b[1])!=c['code'] or (b[1]&3)!=c['flip']:continue
                seen+=1;votes.setdefault(b[1]>>8,Counter())[c['palette']]+=1
                first,count=table_rows(l['table'],i)
                if not first<=j<first+count:hidden+=1
            assert not hidden,('observed block outside the rows of its column table',hex(l['map']),hidden)
            changes={n:len(v) for n,v in votes.items() if len(v)>1}
            ram_pal={}
            if a.palette_capture and '%06X'%l['map'] in pal_frames:
                raw=(a.palette_capture/('palette%d.bin'%pal_frames['%06X'%l['map']])).read_bytes()
                ram=lambda n:[int.from_bytes(raw[32*n+2*k:32*n+2*k+2],'big') for k in range(16)]
                agree=sum(1 for n,v in votes.items() if palettes[v.most_common(1)[0][0]][1:]==ram(n)[1:])
                assert agree==len(votes),('palette RAM snapshot differs from the observed colours',hex(l['map']),agree,len(votes))
                for n in {attr>>8 for code,attr in l['tiles'].values()}-set(votes):
                    cols=ram(n);cols[0]=0
                    if cols not in palettes:palettes.append(cols)
                    ram_pal[n]=palettes.index(cols)
            base=(l['progress']&~511) if l['ring'] else 0;emitted=0;uncoloured=Counter()
            for (i,j),(code,attr) in sorted(l['tiles'].items()):
                if not code&0xffff and not attr:continue
                first,count=table_rows(l['table'],i)
                if not first<=j<first+count:continue
                if attr>>8 not in votes and attr>>8 not in ram_pal:uncoloured[attr>>8]+=1;continue
                pal=votes[attr>>8].most_common(1)[0][0] if attr>>8 in votes else ram_pal[attr>>8]
                xs=[base+16*i,base+16*i+512] if l['ring'] else [16*(l['column']+i)]
                for x in xs:
                    if l['ring'] and x>=base+512+352:continue
                    cells.append(dict(layer=layer,x=x,y=y0+16*j,code=code&~mask(attr),flip=attr&3,animation_mask=mask(attr),palette=pal,observations=0));emitted+=1
            # Blocks MAME showed outside the map: the ring keeps what its slots held (the columns before the map's first one,
            # the rows under a 16-row picture, the columns past the map's end that an actor patches); kept as observed.
            kept=0;have={(c['layer'],c['x'],c['y']) for c in cells}
            for c in outside:
                xs=[c['x']]
                if l['ring']:x0=base+(c['x']-base)%512;xs=[x for x in (x0,x0+512) if x<base+512+352]
                elif l['object']==0x10c72e and c['x']>=a.outside_limit:continue
                for x in xs:
                    if (layer,x,c['y']) in have:continue
                    have.add((layer,x,c['y']));kept+=1
                    cells.append(dict(layer=layer,x=x,y=c['y'],code=c['code'],flip=c['flip'],animation_mask=c['animation_mask'],palette=c['palette'],observations=c.get('observations',1)))
            if l['object']==0x10c72e and l['width']>=32:
                # Past the terrain map's end: the observed blocks (first seen) up to --outside-limit.
                for c in obs.get(l['map'],[]):
                    if c['x']>=16*(l['column']+l['width']) and c['x']<a.outside_limit and (layer,c['x'],c['y']) not in have:
                        have.add((layer,c['x'],c['y']));kept+=1
                        cells.append(dict(layer=layer,x=c['x'],y=c['y'],code=c['code'],flip=c['flip'],animation_mask=c['animation_mask'],palette=c['palette'],observations=c.get('observations',1)))
            # Palette number -> colour set of this map (tools/extract_map_patches.py: the blocks of its patches).
            numbers['%06X'%l['map']]={str(n):(votes[n].most_common(1)[0][0] if n in votes else ram_pal[n]) for n in sorted(set(votes)|set(ram_pal))}
            stats.append(dict(map='%06X'%l['map'],model_layer=layer,ring=l['ring'],ring_base=base,observed_equal=seen,rom_blocks=emitted,observed_outside_kept=kept,
                              palette_numbers=len(votes),palette_numbers_from_palette_ram=sorted(ram_pal),palette_numbers_with_several_colour_sets=changes,blocks_without_observed_colours=dict(uncoloured)))
            print('ROM model: map %06X layer %d: %d ROM blocks, %d observed blocks outside the map kept, %d palette numbers (%d with more than one colour set), %d blocks without colours'%(
                l['map'],layer,emitted,kept,len(votes),len(changes),sum(uncoloured.values())))
        out=dict(m);out['source']='tools/export_stage_rom.py: ROM blocks of the maps of the camera list %06X; colours of the observed blocks (%s)'%(rom.u32(a.descriptor),m['source'])
        out['cells']=cells;out['swapped_cells']=[];out['conflicts']=[];out['palettes']=palettes;out['palette_numbers']=numbers
        out['width']=max(c['x'] for c in cells if c['layer']==3)+16;out['camera_max']=a.camera_max if a.camera_max is not None else m['camera_max']
        out['selection']='ROM maps (pre-patch state); observed blocks only outside the maps'
        a.rom_model.write_text(json.dumps(out)+'\n');rep['rom_model']=stats
    a.output.write_text(json.dumps(doc)+'\n')
    if a.report:a.report.write_text(json.dumps(rep,indent=1)+'\n')
    print('grid %d x %d, row origin %d, %d cells not empty, tail %d -> %s'%(columns,rows,row0,rep['grid']['nonempty'],a.tail,a.output))
if __name__=='__main__':main()
