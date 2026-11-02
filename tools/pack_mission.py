#!/usr/bin/env python3
"""Per-mission scene pack (build/missionN.pack; romfs/ once the runtime
loads it), docs/MISION2.md.

One self-describing file per mission so the cartridge holds every mission
while RDRAM holds only the current one: the header, stage table, cell,
image and palette tables are read into RAM at the mission start (a few tens
of KiB); the CI4 map pages stay in the ROM in 16 KiB chunks (8 pages,
RLE-packed when smaller, tools/pack_scene.py codec) and stream through the
scene cache like Mission 1's actor chunks. Actor graphics of the mission
(per-mission inventory) are a later section of the same file.

Input per stage: filesystem/missionN/sK/map.json (tools/extract_mission_map.py
--ram-layers) and collision.json (tools/extract_region_collision.py
--descriptor-layers). Coordinate variants (dynamic map writes, still needing
their native events) are counted, not packed.

Layout (big endian, every table 4-byte aligned; version 3, read by
src/scene_assets.c scene_mission_*):
  'MSXM' u16 version=3 u16 mission u16 stages u16 palettes u32 images u32 chunks
  u32 cells_total u32 grid_bytes u32 actors u32 actor_images u32 resources
  stage[stages] (128 bytes):
                  u32 descriptor u16 width u16 camera_max u32 backdrop_rgba
                  u32 first_cell u32 cell_count u16 grid_columns u16 grid_rows
                  i16 grid_origin_x i16 grid_origin_y u32 grid_offset
                  u32 unknown_offset (bit mask of never observed grid cells,
                    same order as the grid; 0 = none) u32 image_first u32 image_count
                  blob[3] = camera command list (descriptor +0), script section
                    (descriptor +8 through the table at 0x000800), player
                    records (descriptor +4, 019604): u32 rom u32 offset u32 length
                  layer[4]: u8 x_num u8 x_den u8 y_num u8 y_den i16 visible_min
                    i16 visible_max (layer camera x range where it is drawn)
                  u32 patch_offset u16 patch_count: map patches 0C70FC
                    (u32 sub-map, u32 layer object each; their cells are
                    the stage's last, layer byte 0x80 | patch << 3 | layer)
                  pad to 128
  cell[]:    u8 layer u8 0 i16 x i16 y u16 image[8]        (22 bytes)
  image[]:   u16 page u16 palette u16 y u16 h              (8 bytes)
  palette[]: u16 rgba5551[16]
  chunk[]:   u32 offset u32 length u32 crc32 u32 codec
  actor[]:   u32 descriptor u16 first u16 count               (8 bytes)
  actor_image[]: u16 page u16 palette i16 x i16 y u16 w u16 h (12 bytes)
  resource[]: u16 logical palette resource u16 palette        (4 bytes)
  grids (u8 cells, column major) with their unknown masks, the ROM blobs,
  then the chunks (64-byte aligned). Grids with never observed cells are
  packed with their mask: the runtime treats an unknown cell as empty and
  counts it (explicit stop, docs/MISION2.md "Fase 2"). The runtime loads the tables of
  one stage at a time (stage table offsets)."""
import argparse,hashlib,json,struct,zlib
from pathlib import Path
from mission_tiles import pack_map
from pack_scene import rle,unrle,PAGE
from export_marco import palette_rgb
ROOT=Path(__file__).resolve().parents[1]
DESCRIPTORS={2:(0x016DDE,0x016DEE,0x016DFE),3:(0x016E0E,)}

def actor_frames(mission):
    """Native actor descriptors of the mission (filesystem/missionN/rest-native.json,
    tools/export_rest_sprites.py --output) that the scene pack (pack_scene.py:
    soldier, Mission 1 boss and rest-of-mission exports) does not have, with
    their palette resources missing there. Empty descriptors get no parts."""
    path=ROOT/f'filesystem/mission{mission}/rest-native.json'
    if not path.exists():return [],{},[]
    known={};have={}
    for name in ('soldier-native.json','boss-native.json','rest-native.json'):
        f=ROOT/'filesystem'/name
        if not f.exists():continue
        d=json.loads(f.read_text());have.update(d['palettes'])
        known.update((x['descriptor'],x) for x in d['frames']);known.update((e,None) for e in d.get('empty',[]))
    m=json.loads(path.read_text())
    for f in m['frames']:
        assert f['descriptor'] not in known or known[f['descriptor']] is None or known[f['descriptor']]['pens_hex']==f['pens_hex'],('descriptor differs from the scene pack',f['descriptor'])
    for k,v in m['palettes'].items():assert have.get(k,v)==v,('palette differs from the scene pack',k)
    frames=[f for f in m['frames'] if f['descriptor'] not in known]
    empty=[e for e in m.get('empty',[]) if e not in known and e not in {f['descriptor'] for f in frames}]
    return frames,{k:v for k,v in m['palettes'].items() if k not in have},empty

def build(mission,stage_dirs,crom,rom=None):
    MISSION[0]=mission
    pages=[];page_ids={};palettes=[];pal_ids={}
    def image(pixels,w,h,dx=0,dy=0,force_actor=False,indices=None,source_palette=None):
        assert indices is not None and w==64 and h==64
        key=tuple(source_palette)+(0,)*(16-len(source_palette))
        if key not in pal_ids:pal_ids[key]=len(palettes);palettes.append(key)
        data=bytearray(PAGE)
        for y in range(64):
            for x in range(64):data[y*32+x//2]|=indices[y*64+x]<<(0 if x&1 else 4)
        data=bytes(data)
        if data not in page_ids:page_ids[data]=len(pages);pages.append(data)
        return (page_ids[data],pal_ids[key],dx,dy,w,h,0)
    stages=[];cells=[];images=[];grids=bytearray();report=[]
    for k,d in enumerate(stage_dirs):
        model=json.loads((d/'map.json').read_text())
        base=len(images)
        tower=tower_templates(model,rom) if rom is not None else None
        merged=merge_swapped(model)
        # Map patches 0C70FC (tools/extract_map_patches.py, B37): cells of
        # patch i on layer L packed with layer byte 0x80 | i << 3 | L, after
        # the stage's cells (the layer sort keeps them last); the runtime
        # draws them instead of the cells they cover once the game applies
        # the patch (its sub-map, layer object).
        patch_list=[]
        if (d/'patches.json').exists():
            for i,pt in enumerate(json.loads((d/'patches.json').read_text())['patches']):
                assert i<16
                patch_list.append((int(pt['submap'],16),int(pt['layer_object'],16)))
                for c in pt['cells']:
                    if c['colors'] not in model['palettes']:model['palettes'].append(c['colors'])
                    model['cells'].append(dict(layer=0x80|i<<3|c['layer'],x=c['x'],y=c['y'],code=c['code'],flip=c['flip'],
                                               animation_mask=c['animation_mask'],palette=model['palettes'].index(c['colors']),observations=0))
        if LOOSE[0]:
            # The column origin (+32) wraps at 4096 columns (16 * 4096 = 65536
            # px): cell x of a moving layer is taken modulo 65536 (first kept).
            seen=set();keep=[]
            for c in model['cells']:
                c['x']=(c['x']+32768)%65536-32768
                if (c['layer'],c['x'],c['y']) not in seen:seen.add((c['layer'],c['x'],c['y']));keep.append(c)
            model['cells']=keep
        st_images,st_cells=pack_map(model,crom,image)
        images.extend((pg,pal,y,h) for pg,pal,_,y,_,h,_ in st_images)
        first=len(cells);cells.extend((layer,x,y,[base+i for i in phases]) for layer,x,y,phases in st_cells)
        col=json.loads((d/'collision.json').read_text())
        grid=bytes.fromhex(col['cells_hex']);assert len(grid)==col['columns']*col['rows']
        while len(grids)%4:grids.append(0)
        offset=len(grids);grids.extend(grid)
        unknown=0
        if col.get('unknown_cells'):
            # Never observed cells (docs/MISION2.md): a bit mask in grid order.
            flags=bytes.fromhex(col['unknown_mask_hex']);assert len(flags)==len(grid)
            mask=bytearray((len(grid)+7)//8)
            for i,f in enumerate(flags):
                if f:mask[i>>3]|=1<<(i&7)
            while len(grids)%4:grids.append(0)
            unknown=len(grids);grids.extend(mask)
        backdrops=[o.get('backdrop') for o in model['observations'] if o.get('backdrop') is not None]
        rules=layer_rules(model)
        stages.append(dict(descriptor=DESCRIPTORS.get(mission,(0,)*len(stage_dirs))[k],width=model['width'],camera_max=model['camera_max'],
                           backdrop=int.from_bytes(bytes((*palette_rgb(backdrops[0]),255)),'big') if backdrops else 0xff,
                           first=first,count=len(st_cells),columns=col['columns'],rows=col['rows'],origin=col['origin_cells'],
                           grid=offset,unknown=unknown,image_first=base,image_count=len(st_images),rules=rules,patches=patch_list))
        report.append(dict(stage=d.name,cells=len(st_cells),images=len(st_images),coordinate_variants=len(model['conflicts']),
                           backdrops=sorted(set(backdrops)),grid=[col['columns'],col['rows']],grid_unknown=col.get('unknown_cells'),
                           grid_mutable=len(col.get('mutable_cells',[])),layer_rules=rules,tower=tower,merged_maps=merged))
    blobs=bytearray()
    if rom is not None:
        for k,st in enumerate(stages):
            st['blobs']=[]
            for rom_at,data in stage_blobs(rom,st['descriptor']):
                while len(blobs)%4:blobs.append(0)
                st['blobs'].append((rom_at,len(blobs),len(data)));blobs.extend(data)
            report[k]['blobs']=[(f'0x{r:06X}',n) for r,_,n in st['blobs']]
    for k,st in enumerate(stages):
        while len(blobs)%4:blobs.append(0)
        st['patch_at']=len(blobs)
        for sub,obj in st['patches']:blobs.extend(struct.pack('>II',sub,obj))
        if st['patches']:report[k]['map_patches']=[f'0x{sub:06X}' for sub,_ in st['patches']]
    # Mission actors (docs/MISION2.md "Fase 2e"): <=64x64 parts on their own
    # pages after the maps; the draw overrides the palette with the logical
    # resource (scene_native_palette, mission resources after the scene's).
    frames,resources,empty=actor_frames(mission)
    while len(pages)%8:pages.append(bytes(PAGE))   # the actors start a chunk
    actors=[];actor_images=[];res=[]
    for k,v in sorted(resources.items(),key=lambda t:int(t[0],16)):
        key=tuple(v)+(0,)*(16-len(v))
        if key not in pal_ids:pal_ids[key]=len(palettes);palettes.append(key)
        res.append((int(k,16),pal_ids[key]))
    for f in frames:
        pens=bytes.fromhex(f['pens_hex']);w,h=f['width'],f['height'];first=len(actor_images)
        for py0 in range(0,h,64):
            for px0 in range(0,w,64):
                pw,ph=min(64,w-px0),min(64,h-py0);data=bytearray(PAGE)
                for y in range(ph):
                    for x in range(pw):data[y*32+x//2]|=(pens[(py0+y)*w+px0+x]&15)<<(0 if x&1 else 4)
                data=bytes(data)
                if data not in page_ids:page_ids[data]=len(pages);pages.append(data)
                actor_images.append((page_ids[data],0,f['x']+px0,f['y']+py0,pw,ph))
        actors.append((int(f['descriptor'],16),first,len(actor_images)-first))
    actors+=[(int(e,16),0,0) for e in empty];actors.sort()
    real_pages=len(pages)
    while len(pages)%8:pages.append(bytes(PAGE))
    chunks=[];body=bytearray()
    for start in range(0,len(pages),8):
        decoded=b''.join(pages[start:start+8]);packed=rle(decoded)
        if len(packed)<len(decoded):codec=1;assert unrle(packed)==decoded
        else:codec=0;packed=decoded
        chunks.append([len(body),len(packed),zlib.crc32(decoded),codec])
        body.extend(packed);body.extend(bytes((-len(body))%64))
    head=bytearray(b'MSXM')+struct.pack('>HHHHIIIIIII',3,mission,len(stages),len(palettes),len(images),len(chunks),len(cells),len(grids),len(actors),len(actor_images),len(res))
    stage_at=len(head);head+=bytes(STAGE_BYTES*len(stages))
    for layer,x,y,ph in cells:head+=struct.pack('>BBhh8H',layer,0,x,y,*ph)
    for im in images:head+=struct.pack('>4H',*im)
    for p in palettes:head+=struct.pack('>16H',*p)
    table_at=len(head);head+=bytes(16*len(chunks))
    for a in actors:head+=struct.pack('>IHH',*a)
    for im in actor_images:head+=struct.pack('>HHhhHH',*im)
    for r in res:head+=struct.pack('>HH',*r)
    grids_at=len(head);head+=grids
    while len(head)%4:head.append(0)
    blobs_at=len(head);head+=blobs
    head+=bytes((-len(head))%64)
    for i,c in enumerate(chunks):c[0]+=len(head);struct.pack_into('>4I',head,table_at+16*i,*c)
    for k,st in enumerate(stages):
        rec=struct.pack('>IHHIIIHHhhIIII',st['descriptor'],st['width'],st['camera_max'],st['backdrop'],st['first'],st['count'],
                        st['columns'],st['rows'],st['origin'][0],st['origin'][1],grids_at+st['grid'],grids_at+st['unknown'] if st['unknown'] else 0,
                        st['image_first'],st['image_count'])
        for r,o,n in st.get('blobs',[(0,0,0)]*3):rec+=struct.pack('>III',r,blobs_at+o if n else 0,n)
        for xn,xd,yn,yd,lo,hi in st['rules']:rec+=struct.pack('>BBBBhh',xn,xd,yn,yd,lo,hi)
        rec+=struct.pack('>IH',blobs_at+st['patch_at'] if st['patches'] else 0,len(st['patches']))
        assert len(rec)<=STAGE_BYTES;head[stage_at+STAGE_BYTES*k:stage_at+STAGE_BYTES*k+len(rec)]=rec
    blob=bytes(head)+bytes(body)
    return blob,dict(mission=mission,stages=report,pages=real_pages,chunks=len(chunks),palettes=len(palettes),images=len(images),cells=len(cells),
                     actors=len(actors),actor_images=len(actor_images),actor_pages=len({im[0] for im in actor_images}),resources=len(res),
                     header_bytes=len(head),decoded_bytes=len(pages)*PAGE,packed_bytes=len(blob),sha256=hashlib.sha256(blob).hexdigest())

STAGE_BYTES=128
def merge_swapped(model):
    """Foreground maps swapped in place (Mission 3: 0B853A -> 0B8600 ->
    0B8550, docs/MISION3.md) whose cells agree with the main map wherever
    both were seen: they join the main map (world coordinates, layer 3 at
    x 1). Other swapped maps (overlays) stay out of the pack."""
    main={(c['x'],c['y']):c for c in model['cells'] if c['layer']==3};out={}
    for m in sorted({c['map'] for c in model.get('swapped_cells',[]) if c['layer']==3}):
        cs=[c for c in model['swapped_cells'] if c['layer']==3 and c['map']==m]
        if any((c['x'],c['y']) in main and main[(c['x'],c['y'])]['code']!=c['code'] for c in cs):continue
        new=[dict(c) for c in cs if (c['x'],c['y']) not in main]
        for c in new:c.pop('map',None);main[(c['x'],c['y'])]=c
        model['cells'].extend(new);out[m]=len(new)
    return out
def layer_rules(model):
    """Per-layer scroll rules fitted to the observations (docs/MISION2.md
    "Fase 2"): x = camera*x_num/x_den, rise = camera_y*y_num/y_den (camera_y
    is the foreground's vertical offset from the stage start), the layer is
    drawn while visible_min <= camera <= visible_max. The fit must be exact
    up to the snapshot lag (8 px), else the tool stops."""
    from fractions import Fraction as F
    obs=model['observations'];base=obs[0]['vertical_offsets'];rules=[]
    if LOOSE[0]:
        # Mission 3 (docs/MISION3.md): layers 0/1 move on their own (sky map
        # dropped to pointer 0, 10D7B6 driven by an unrecovered event): the
        # offsets are unwrapped (16-bit RAM hints), a layer counts as active
        # only while its first map is shown, and the best fit by median is
        # kept without the 8 px check (its worst error is reported).
        obs=[dict(o,offsets=list(o['offsets'])) for o in obs];prev=[None]*4;acc=[0]*4
        for o in obs:
            for L in range(4):
                v=o['offsets'][L]
                if prev[L] is not None:acc[L]+=(v-prev[L]+32768)%65536-32768
                else:acc[L]=v
                prev[L]=v;o['offsets'][L]=acc[L]
            if o.get('map_origins'):
                o['active']=[a and o['map_origins'][L][0]==obs[0]['map_origins'][L][0] or (L==3 and a) for L,a in enumerate(o['active'])]
    for L in range(4):
        on=[o for o in obs if o['active'][L]];off=[o for o in obs if not o['active'][L]]
        if not on:rules.append((1,1,1,1,1,0));continue
        fits=[]
        for f in (F(1),F(1,2),F(3,4),F(1,4),F(0)):
            errs=sorted(abs(o['offsets'][L]-f*o['camera']) for o in on)
            fits.append((errs[len(errs)//2] if LOOSE[0] else errs[-1],f))
        ex,fx=min(fits,key=lambda t:(t[0],-t[1]))
        fits=[]
        for f in (F(1),F(1,2),F(3,4),F(1,4),F(0)):
            ey=max(abs((o['vertical_offsets'][L]-base[L])-f*(o['vertical_offsets'][3]-base[3])) for o in on)
            fits.append((ey,f))
        ey,fy=min(fits,key=lambda t:(t[0],-t[1]))
        if len({o['camera'] for o in on})==1:fx=F(1,2) if L in (1,2) else fx
        if LOOSE[0]:LOOSE_ERRORS.append((L,max(abs(o['offsets'][L]-fx*o['camera']) for o in on).__float__(),float(ex)))
        else:assert ex<=8 and ey<=8,('layer rule does not fit',L,float(ex),float(ey))
        lo=min(o['camera'] for o in on);hi=max(o['camera'] for o in on)
        # Thresholds halfway between the last observation of one state and
        # the first of the other (never inside an observed range).
        below=[o['camera'] for o in off if o['camera']<lo];above=[o['camera'] for o in off if o['camera']>hi]
        lo=(lo+max(below)+1)//2 if below else -32768
        hi=(hi+min(above))//2 if above else 32767
        rules.append((fx.numerator,fx.denominator,fy.numerator,fy.denominator,lo,hi))
    return rules

# Stage 3 tower (docs/MISION2.md "Fase 2d"): the writer 0B7C38 copies 24 x 3
# blocks of the layer map 0B740A at (0x10, 3 * type) to the window blocks
# (0x10, 0x66 + 10C6B2); 0C714E copies the sprite map's blocks of the same
# rectangle (+0 of the descriptor: u16 tile, u16 attribute per 16-px block,
# column major, 0x88 blocks per column). The recorded cells of the climb
# (layer 3, y < TOWER_TOP, x 256..639) are run4's sequence: they are dropped
# and the 13 templates are packed as layer-3 cells at x >= TOWER_TEMPLATE_X
# (never on screen): template t, block (i, j) at (TOWER_TEMPLATE_X + 16 i,
# 16 (3 t + j)). The renderer draws piece k (10C6B2 = 3k) of type t at
# (256 + 16 i, TOWER_BASE_Y - 48 k + 16 j) (scene_render_set_tower).
TOWER_MAP=0x0B740A;TOWER_TEMPLATE_X=0x4000;TOWER_BASE_Y=-1408;TOWER_TOP=-1360;TOWER_COLUMN=0x10;TOWER_TYPES=13
def tower_templates(model,rom):
    if not any(o.get('map_origins') and o['map_origins'][3][0]=='0x%06X'%TOWER_MAP for o in model['observations']):return None
    gfx,_,_,blocks=struct.unpack_from('>IIHH',rom.fixed,TOWER_MAP);base=gfx-0x100000
    def entry(col,b):
        t,a=struct.unpack_from('>HH',rom.p2,base+(col*blocks+b)*4)
        return t|((a>>4)&0xf)<<16,a>>8,a&3,7 if a&8 else 3 if a&4 else 0
    # Static rows (map block b at y = top + 16 b, top = -1936): ROM palette -> map palette,
    # and the check that the recorded cells are this sprite map.
    top=TOWER_BASE_Y-16*(0x87-0x66);pal={};same=0   # window block W = 0x87 - b
    cells=[c for c in model['cells'] if c['layer']==3]
    for c in cells:
        if c['y']<TOWER_TOP:continue
        code,p,flip,anim=entry(c['x']//16,(c['y']-top)//16)
        if code==c['code']:
            assert pal.setdefault(p,c['palette'])==c['palette'],('palette',p);same+=1
    assert same>=0.9*sum(1 for c in cells if c['y']>=TOWER_TOP),('sprite map',same)
    climb=[c for c in cells if c['y']<TOWER_TOP and 256<=c['x']<640]
    assert all(256<=c['x']<640 for c in cells if c['y']<TOWER_TOP),'tower cells outside the piece columns'
    # Every recorded piece is one of the templates (run4's sequence).
    rec={(c['x'],c['y']):c['code'] for c in climb};pieces=0
    for k in range(200):
        y0=TOWER_BASE_Y-48*k
        if not any((x,y0+16*j) in rec for x in range(256,640,16) for j in range(3)):continue
        assert any(all(rec.get((256+16*i,y0+16*j),code)==code for i in range(24) for j in range(3)
                       for code in [entry(TOWER_COLUMN+i,3*t+j)[0]]) for t in range(TOWER_TYPES)),('piece',k);pieces+=1
    ids={id(c) for c in climb};model['cells']=[c for c in model['cells'] if id(c) not in ids]
    for t in range(TOWER_TYPES):
        for j in range(3):
            for i in range(24):
                code,p,flip,anim=entry(TOWER_COLUMN+i,3*t+j)
                assert p in pal,('template palette not recorded',t,p)
                model['cells'].append(dict(layer=3,x=TOWER_TEMPLATE_X+16*i,y=16*(3*t+j),code=code,flip=flip,animation_mask=anim,palette=pal[p],observations=0))
    return dict(recorded_pieces=pieces,dropped_cells=len(climb),template_cells=TOWER_TYPES*72)
def stage_blobs(rom,descriptor):
    """ROM tables a stage reads (descriptor 016DDE..): the camera command
    list (+0, walked to its hold, op 8, as 0C83E6 does), the script section
    (+8, word index into the table at 0x000800) and the player records (+4,
    12-byte records to the 0xFFFF threshold, 019658)."""
    cam=rom.u32(descriptor);a=cam
    while True:
        op,n=rom.u16(a),rom.u16(a+2)
        if op==8:a+=6;break
        assert n and n<0x40,('camera list',hex(a),op,n);a+=n
    out=[(cam,rom.read(cam,a-cam))]
    entry=rom.u32(descriptor+8);script=rom.u32(entry)
    sec=json.loads((ROOT/f'data/mission{MISSION[0]}_script.json').read_text())['sections']
    end=next(int(s['end'],16) for s in sec if int(s['base'],16)==script)
    out.append((script,rom.read(script,end+2-script)))
    players=rom.u32(descriptor+4);a=players
    while rom.u16(a+12)!=0xffff:a+=12
    out.append((players,rom.read(players,a+14-players)))
    return out
MISSION=[0];LOOSE=[False];LOOSE_ERRORS=[]

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('mission',type=int);p.add_argument('stages',type=Path,nargs='+',help='stage directories in play order')
    p.add_argument('--crom',type=Path,default=ROOT/'filesystem/sprites.bin');p.add_argument('--output',type=Path)
    p.add_argument('--report',type=Path)
    p.add_argument('--loose-layers',action='store_true',help='Mission 3: layers that move on their own get the best median fit (worst error in the report)')
    p.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'),help='MAME set (P ROMs) for the per-stage ROM tables')
    a=p.parse_args();LOOSE[0]=a.loose_layers
    from decode_level_script import Program
    blob,report=build(a.mission,a.stages,a.crom.read_bytes(),Program(a.rom))
    # romfs/: the game loads it at the mission switch (docs/MISION2.md).
    out=a.output or ROOT/f'romfs/mission{a.mission}.pack';out.parent.mkdir(exist_ok=True);out.write_bytes(blob)
    if LOOSE_ERRORS:report['loose_layer_errors']=[dict(layer=L,worst=w,median=m) for L,w,m in LOOSE_ERRORS]
    if a.report:a.report.write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='stages'}))
    for s in report['stages']:print(' ',s)
if __name__=='__main__':main()
