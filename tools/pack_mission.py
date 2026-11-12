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

Layout (big endian, every table 4-byte aligned; read by
src/scene_assets.c scene_mission_*). Version 5 (docs/COMPRESION.md) is
version 4 with the chunks stored as LZ4 blocks, the actor pages another
mission's pack already holds taken from that pack (chunk word bits 8..15)
and a 64-byte trailer naming those packs; --format 4 writes version 4:
  'MSXM' u16 version=5 u16 mission u16 stages u16 palettes u32 images u32 chunks
  u32 cells_total u32 grid_bytes u32 actors u32 actor_images u32 resources
  u32 data_offset u32 data_length: the mission's own data 'MSXD' (mission_data;
    0 = none): the ROM blobs and animation programs of the mission's actors
    that the console image leaves out (groups m2/m3 of data/region_rom.json
    and data/soldier_programs.json; docs/MEMORY_OPTIMIZATION.md rule 5)
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
                  u32 recon_offset u32 recon_length: the stage's map
                    reconnaissance blob 'MSXR' (recon_blob; 0 = none; not
                    read by the runtime yet)
                  pad to 128
  cell[]:    u8 layer u8 0 i16 x i16 y u16 image[8]        (22 bytes)
  image[]:   u16 page u16 palette u16 y u16 h              (8 bytes)
  palette[]: u16 rgba5551[16]
  chunk[]:   u32 offset u32 length u32 crc32 u32 word: codec in bits 0..7
               (0 raw, 1 run pairs, 2 one LZ4 block; tools/pack_store.py),
               bits 8..15 the mission whose pack holds the chunk at `offset`
               (0: this pack). The CRC is of the decoded 16 KiB.
  actor[]:   u32 descriptor u16 first u16 count               (8 bytes)
  actor_image[]: u16 page u16 palette i16 x i16 y u16 w u16 h (12 bytes)
  resource[]: u16 logical palette resource u16 palette        (4 bytes)
  grids (u8 cells, column major) with their unknown masks, the ROM blobs,
  then the chunks (64-byte aligned). Grids with never observed cells are
  packed with their mask: the runtime treats an unknown cell as empty and
  counts it (explicit stop, docs/MISION2.md "Fase 2"). The runtime loads the tables of
  one stage at a time (stage table offsets).
  Version 5, the file's last 64 bytes: 'MSXX' u16 packs u16 0, then per pack
  whose chunks are used (at most 5): u16 mission u16 0 u32 its size in bytes
  u32 the CRC-32 of the whole pack (the runtime refuses the mission when the
  pack on the cartridge has another size; pack_store.py checks the CRC)."""
import argparse,hashlib,json,struct,zlib
from pathlib import Path
from mission_tiles import pack_map
from pack_scene import PAGE
import pack_store
from export_marco import palette_rgb
ROOT=Path(__file__).resolve().parents[1]
DESCRIPTORS={2:(0x016DDE,0x016DEE,0x016DFE),3:(0x016E0E,),4:(0x016E1E,),5:(0x016E2E,0x016E3E,0x016E4E),6:(0x016E5E,0x016E6E,0x016E7E)}

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

# Mission 4 shares enemies with Mission 3 (grenadiers, shield soldiers, the bomber's family...): its pack carries both groups.
GROUPS={2:('m2',),3:('m3',),4:('m3','m4'),5:('m3','m5'),6:('m3','m4','m5','m6')}   # Mission 6 stage 1: the rafts, the bridge spans and the bridge's collision map ('m6'); Mission 5 uses the Mission 3 soldiers 05C53C and their weapons too
def c_layer(patch):
    layers={c['layer'] for c in patch['cells']};assert len(layers)==1;return layers.pop()
def anim_kinds():
    """Numbers of neo_anim_kind_t (src/neo_animation.h, in declaration order)."""
    import re
    text=(ROOT/'src/neo_animation.h').read_text()
    body=re.search(r'typedef enum \{(.*?)\} neo_anim_kind_t;',text,re.S).group(1)
    body=re.sub(r'/\*.*?\*/','',body,flags=re.S)
    return {n.strip():i for i,n in enumerate(x for x in body.split(',') if x.strip())}
KIND_NAMES={'WAIT':'NEO_ANIM_WAIT','POSE':'NEO_ANIM_POSE','JUMP':'NEO_ANIM_JUMP','FIELD50':'NEO_ANIM_FIELD50','FINISH':'NEO_ANIM_FINISH',
    'FLIP_FACING':'NEO_ANIM_FLIP_FACING','UNRECOVERED':'NEO_ANIM_UNRECOVERED','REPLACE_PALETTE':'NEO_ANIM_REPLACE_PALETTE',
    'SPAWN_THROW':'NEO_ANIM_SPAWN_THROW','SELECT_POSE':'NEO_ANIM_SELECT_POSE','SET_BYTE':'NEO_ANIM_SET_BYTE','SET_WORD':'NEO_ANIM_SET_WORD',
    'SET_LONG':'NEO_ANIM_SET_LONG','DEC_BYTE':'NEO_ANIM_DEC_BYTE','COND_JUMP':'NEO_ANIM_COND_JUMP','SWITCH':'NEO_ANIM_SWITCH',
    'SOUND':'NEO_ANIM_SOUND','CALL':'NEO_ANIM_CALL','SELECT_FIELD_POSE':'NEO_ANIM_SELECT_FIELD_POSE'}
def lender_data(lenders,groups):
    """The pack among `lenders` whose 'MSXD' holds only groups this mission
    carries too (docs/COMPRESION.md, Duplicados: Missions 4 and 5 carry the
    Mission 3 group): (pack, its groups, its blob records, its program
    records, its header bytes), or None."""
    for other in lenders:
        g=GROUPS.get(other.mission,())
        if not g or not set(g)<=set(groups) or not other.data_length:continue
        d=other.raw[other.data_offset:other.data_offset+other.data_length]
        assert d[:4]==b'MSXD'
        version,nb,np_=struct.unpack_from('>HHH',d,4)
        if version!=1:continue   # a lender that borrows itself: not taken
        blobs=[struct.unpack_from('>III',d,12+12*i) for i in range(nb)]
        progs=[struct.unpack_from('>IIHH',d,12+12*nb+12*i) for i in range(np_)]
        return other,g,blobs,progs,12+12*nb+12*np_
    return None
def mission_data(mission,lenders=()):
    """'MSXD' u16 version=1 u16 blobs u16 programs u16 0, blob[] (u32 rom
    address, u32 offset, u32 length), program[] (u32 tag, u32 offset, u16
    commands, u16 0; tags ascending), then the data: blob bytes and
    10-byte commands (u8 kind, i24 asset, u16 target, u8 operand, u8 flags,
    i8 dx, i8 dy: neo_anim_command_t, as compile_soldier_ai_tables.py
    writes them). Offsets from the block start, 2-byte aligned.

    Version 2 (a lender in `lenders`, lender_data): the groups another
    mission's pack holds are not stored again. 'MSXD' u16 2 u16 blobs u16
    programs u16 lender mission, u32 offset in the lender's pack of its
    data after its records, u32 length of that data; then the records as
    in version 1. The loader reads the lender's data right after this
    block: a record offset >= this block's length (even) points into it.
    Returns (block, report, lender mission or 0)."""
    groups=GROUPS.get(mission,())
    lend=lender_data(lenders,groups)
    if lend:groups=tuple(g for g in groups if g not in lend[1])
    rom=json.loads((ROOT/'data/region_rom.json').read_text());progs=json.loads((ROOT/'data/soldier_programs.json').read_text())
    blobs=[b for b in rom['blobs'] if b.get('group') in groups]
    programs=sorted((p for p in progs['programs'] if p.get('group') in groups),key=lambda p:p['root'])
    if not blobs and not programs and not lend:return b'',{},0
    kinds=anim_kinds()
    nb=len(blobs)+(len(lend[2]) if lend else 0);np_=len(programs)+(len(lend[3]) if lend else 0)
    base=(20 if lend else 12)+12*nb+12*np_;body=bytearray();brecs=[];precs=[]
    for b in blobs:
        data=bytes.fromhex(b['hex']);brecs.append((int(b['rom'],16),base+len(body),len(data)));body+=data
        if len(body)&1:body.append(0)
    seen=set();cmds=[]
    for p in programs:
        assert p['root'] not in seen;seen.add(p['root'])
        precs.append((p['root'],base+len(body),len(p['commands']),0))
        for c in p['commands']:
            asset=0x1adaa if c['kind']=='FLIP_FACING' else c['asset']
            asset&=0xffffffff
            assert asset<0x800000 or asset>=0xff800000,('asset',hex(asset))
            body+=struct.pack('>B',kinds[KIND_NAMES[c['kind']]])+(asset&0xffffff).to_bytes(3,'big')+struct.pack('>HBBbb',c['target'],c['operand'],c['flags'],c['dx'],c['dy'])
    report=dict(blobs=len(blobs),blob_bytes=sum(len(b['hex'])//2 for b in blobs),programs=len(programs),commands=sum(len(p['commands']) for p in programs))
    if lend:
        other,_,lblobs,lprogs,lhead=lend
        own=base+len(body);assert own%2==0
        shift=own-lhead   # the lender's data lands right after this block
        brecs+=[(r,o+shift,n) for r,o,n in lblobs]
        for t,o,n,z in lprogs:
            assert t not in seen,('program in two groups',hex(t));seen.add(t);precs.append((t,o+shift,n,z))
        precs.sort()
        head=b'MSXD'+struct.pack('>HHHHII',2,nb,np_,other.mission,other.data_offset+lhead,other.data_length-lhead)
        report.update(lender=other.mission,borrowed_blobs=len(lblobs),borrowed_programs=len(lprogs),borrowed_bytes=other.data_length-lhead)
    else:
        precs.sort();head=b'MSXD'+struct.pack('>HHHH',1,nb,np_,0)
    # scene_stage_rom serves an address from the first record that holds it:
    # records by descending end, so an address in two blobs is served from
    # the one with more bytes after it (Mission 5's rank table 2D388E, 306 B,
    # starts in the last 2 bytes of the Mission 3 blob 2D36A4: served from
    # there, a read of the table ran past that blob). Overlapping blobs must
    # hold the same bytes.
    lent=lend[0].raw[lend[0].data_offset+lend[4]:lend[0].data_offset+lend[0].data_length] if lend else b''
    def blob_bytes(o,n):
        own=base+len(body)
        return bytes(body[o-base:o-base+n]) if o<own else lent[o-own:o-own+n]
    for i,(r,o,n) in enumerate(brecs):
        for r2,o2,n2 in brecs[i+1:]:
            lo,hi=max(r,r2),min(r+n,r2+n2)
            if lo<hi:assert blob_bytes(o+lo-r,hi-lo)==blob_bytes(o2+lo-r2,hi-lo),('blobs differ where they overlap',hex(r),hex(r2))
    brecs.sort(key=lambda x:(-(x[0]+x[2]),x[0]))
    blob=head+b''.join(struct.pack('>III',*r) for r in brecs)+b''.join(struct.pack('>IIHH',*r) for r in precs)+bytes(body)
    assert len(blob)==base+len(body)
    report['bytes']=len(blob)
    return blob,report,(lend[0].mission if lend else 0)
def build(mission,stage_dirs,crom,rom=None,fmt=5,shared=()):
    MISSION[0]=mission;assert fmt in (4,5) and (fmt==5 or not shared)
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
        # the patch (its sub-map with the source offsets D0 in bits 24..27
        # and D1 in 28..31, as neo_world.c g_map_patch keys it; layer object
        # with the target column D4 in bits 21..31, B63: the Mission 3
        # stampers copy the same frame of 0B85EA to several wagons).
        patch_list=[]
        if (d/'patches.json').exists():
            for i,pt in enumerate(json.loads((d/'patches.json').read_text())['patches']):
                # Patches 16..63 (Mission 4, docs/MISION4.md): the layer bits 5..7 are the banks 1..3 of the
                # terrain layer 3 (patch = 16 * bank + bits 3..6); bank 0 keeps its layer in bits 0..2.
                assert i<64 and (i<16 or c_layer(pt)==3)
                d0,d1=pt['d'][0],pt['d'][1]
                patch_list.append((int(pt['submap'],16)|(d0&15)<<24|(d1&15)<<28,int(pt['layer_object'],16)|(pt['d'][4]&0x7ff)<<21))
                assert len(set(patch_list))==len(patch_list),'two patches with the same key'
                for c in pt['cells']:
                    if c['colors'] not in model['palettes']:model['palettes'].append(c['colors'])
                    model['cells'].append(dict(layer=0x80|(i&15)<<3|(c['layer'] if i<16 else 4+(i>>4)),x=c['x'],y=c['y'],code=c['code'],flip=c['flip'],
                                               animation_mask=c['animation_mask'],palette=model['palettes'].index(c['colors']),observations=0))
        if LOOSE[0]:
            # The column origin (+32) wraps at 4096 columns (16 * 4096 = 65536
            # px): cell x of a moving layer is taken modulo 65536 (first kept).
            seen=set();keep=[]
            for c in model['cells']:
                c['x']=(c['x']+32768)%65536-32768
                if (c['layer'],c['x'],c['y']) not in seen:seen.add((c['layer'],c['x'],c['y']));keep.append(c)
            model['cells']=keep
        if (d/'layers_rom.json').exists():report_fill=fill_loop_layer(model,json.loads((d/'layers_rom.json').read_text()),crom)
        else:report_fill=None
        st_images,st_cells=pack_map(model,crom,image)
        if (d/'layers_rom.json').exists():
            # Mission 3 sky pictures (docs/MISSION3_DEBUG.md G2): drawn only from
            # the pre-rendered columns (tools/prerender_stage.py sky_cells); the
            # pack carries just their palettes (palette RAM of run4).
            from mission_tiles import rgba16
            lay=json.loads((d/'layers_rom.json').read_text())
            sec=lay['section_palettes'][0]['palettes']
            for m in lay['maps']:
                if m['layer']==0 and m.get('placement','').startswith('0C714E'):
                    for n in sorted({c[5] for c in m['cells']}):
                        key=tuple(rgba16(sec['%02X'%n]));key=key+(0,)*(16-len(key))
                        if key not in pal_ids:pal_ids[key]=len(palettes);palettes.append(key)
        images.extend((pg,pal,y,h) for pg,pal,_,y,_,h,_ in st_images)
        first=len(cells);cells.extend((layer,x,y,[base+i for i in phases]) for layer,x,y,phases in st_cells)
        col=json.loads((d/'collision.json').read_text())
        grid=bytes.fromhex(col['cells_hex']);assert len(grid)==col['columns']*col['rows']
        # The runtime reads grid row y >> 3 of the kernel's y and never the
        # origin (tools/rebase_collision.py, docs/MISION3.md).
        # A ROM grid in kernel rows (tools/export_stage_rom.py, Mission 4) may start below the kernel's row 0:
        # its row origin is packed (grid_origin_y) and the runtime applies it (neo_collision_set_row_origin).
        assert col['origin_cells']==[0,0] or (ROM_LAYERS[0] and col['origin_cells'][0]==0 and col['origin_cells'][1]<0),('grid origin: run tools/rebase_collision.py',d,col['origin_cells'])
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
        # The stage's first value held by three observations in a row: the
        # first ones can be frames of the stage's fade-in (Mission 3: 0 and
        # 22627 before 23173; B58, a black backdrop behind the train layer's
        # empty blocks). The later values (palette events not ported) stay out.
        stable=next((backdrops[i] for i in range(len(backdrops)-2) if backdrops[i]==backdrops[i+1]==backdrops[i+2]),None)
        if stable is not None:backdrops=[stable]+[b for b in backdrops if b!=stable]
        rules=rom_native_rules(rom,DESCRIPTORS[mission][k],model) if ROM_LAYERS[0] else rom_layer_rules(rom,DESCRIPTORS[mission][k]) if LOOSE[0] and rom is not None else layer_rules(model)
        stages.append(dict(descriptor=DESCRIPTORS.get(mission,(0,)*len(stage_dirs))[k],width=model['width'],camera_max=model['camera_max'],
                           backdrop=int.from_bytes(bytes((*palette_rgb(backdrops[0]),255)),'big') if backdrops else 0xff,
                           first=first,count=len(st_cells),columns=col['columns'],rows=col['rows'],origin=col['origin_cells'],
                           grid=offset,unknown=unknown,image_first=base,image_count=len(st_images),rules=rules,patches=patch_list))
        report.append(dict(stage=d.name,cells=len(st_cells),images=len(st_images),coordinate_variants=len(model['conflicts']),
                           backdrops=sorted(set(backdrops)),grid=[col['columns'],col['rows']],grid_unknown=col.get('unknown_cells'),
                           grid_mutable=len(col.get('mutable_cells',[])),layer_rules=rules,tower=tower,merged_maps=merged,loop_layer_fill=report_fill))
    blobs=bytearray()
    if rom is not None:
        for k,st in enumerate(stages):
            st['blobs']=[]
            for rom_at,data in stage_blobs(rom,st['descriptor']):
                while len(blobs)%4:blobs.append(0)
                st['blobs'].append((rom_at,len(blobs),len(data)));blobs.extend(data)
            report[k]['blobs']=[(f'0x{r:06X}',n) for r,_,n in st['blobs']]
    for k,st in enumerate(stages):
        # Map reconnaissance (Mission 3, docs/MISION3.md): the ROM maps of
        # every layer, the dynamic collision and the sections; kept in the
        # pack, not read by the runtime yet.
        st['recon']=None
        if rom is not None and (stage_dirs[k]/'layers_rom.json').exists():
            while len(blobs)%4:blobs.append(0)
            data=recon_blob(stage_dirs[k],rom);st['recon']=(len(blobs),len(data));blobs.extend(data)
            report[k]['recon_bytes']=len(data)
    for k,st in enumerate(stages):
        while len(blobs)%4:blobs.append(0)
        st['patch_at']=len(blobs)
        for sub,obj in st['patches']:blobs.extend(struct.pack('>II',sub,obj))
        if st['patches']:report[k]['map_patches']=[f'0x{sub:08X}' if sub>>24 else f'0x{sub:06X}' for sub,_ in st['patches']]
    # Mission actors (docs/MISION2.md "Fase 2e"): <=64x64 parts on their own
    # pages after the maps; the draw overrides the palette with the logical
    # resource (scene_native_palette, mission resources after the scene's).
    frames,resources,empty=actor_frames(mission)
    while len(pages)%8:pages.append(bytes(PAGE))   # the actors start a chunk
    actor_base=len(pages)
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
    # Actor pages another mission's pack already holds (docs/COMPRESION.md):
    # a chunk of that pack with at least SHARED_MIN pages this mission's
    # actors draw is not stored again; its record points into that pack and
    # its 8 pages become this pack's last ones. The other actor pages keep
    # their order; the map pages and their ids do not change.
    borrowed=[];trailer=[]
    if shared:
        need={pages[i]:i for i in range(actor_base,len(pages))};found={}
        for other in shared:
            assert other.version>=5 and other.mission!=mission
            used=False
            for c,(off,ln,crc,word) in enumerate(other.chunks):
                if word>>8&0xff:continue   # held by a third pack: taken from that one, if given
                ps=[other.page(8*c+k) for k in range(8)]
                if sum(1 for x in set(ps) if x in need and x not in found)<SHARED_MIN:continue
                for k,x in enumerate(ps):
                    if x in need:found.setdefault(x,8*len(borrowed)+k)
                borrowed.append((ps,[off,ln,crc,(word&0xff)|other.mission<<8]));used=True
            if used:trailer.append((other.mission,len(other.raw),zlib.crc32(other.raw)))
        own=[x for x in pages[actor_base:] if x not in found]
        while len(own)%8:own.append(bytes(PAGE))
        first_borrowed=actor_base+len(own);new={x:actor_base+i for i,x in reversed(list(enumerate(own)))}
        new.update((x,first_borrowed+i) for x,i in found.items())
        actor_images=[(new[pages[im[0]]] if im[0]>=actor_base else im[0],)+im[1:] for im in actor_images]
        pages=pages[:actor_base]+own
    real_pages=len(pages)+8*len(borrowed)
    while len(pages)%8:pages.append(bytes(PAGE))
    chunks=[];body=bytearray()
    decoded_chunks=[b''.join(pages[start:start+8]) for start in range(0,len(pages),8)]
    for decoded,(packed,codec) in zip(decoded_chunks,pack_store.encode(decoded_chunks,legacy=fmt<5)):
        chunks.append([len(body),len(packed),zlib.crc32(decoded),codec])
        body.extend(packed);body.extend(bytes((-len(body))%64))
    own_chunks=len(chunks);chunks+=[rec for _,rec in borrowed]
    pages+=[x for ps,_ in borrowed for x in ps]
    data,data_report,lender=mission_data(mission,shared if fmt>=5 else ())
    if lender and all(m!=lender for m,_,_ in trailer):
        other=next(o for o in shared if o.mission==lender);trailer.append((other.mission,len(other.raw),zlib.crc32(other.raw)))
    head=bytearray(b'MSXM')+struct.pack('>HHHHIIIIIIIII',fmt,mission,len(stages),len(palettes),len(images),len(chunks),len(cells),len(grids),len(actors),len(actor_images),len(res),0,len(data))
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
    while len(head)%4:head.append(0)
    if data:struct.pack_into('>I',head,40,len(head));head+=data
    head+=bytes((-len(head))%64)
    for i,c in enumerate(chunks):
        if i<own_chunks:c[0]+=len(head)
        struct.pack_into('>4I',head,table_at+16*i,*c)
    for k,st in enumerate(stages):
        rec=struct.pack('>IHHIIIHHhhIIII',st['descriptor'],st['width'],st['camera_max'],st['backdrop'],st['first'],st['count'],
                        st['columns'],st['rows'],st['origin'][0],st['origin'][1],grids_at+st['grid'],grids_at+st['unknown'] if st['unknown'] else 0,
                        st['image_first'],st['image_count'])
        for r,o,n in st.get('blobs',[(0,0,0)]*3):rec+=struct.pack('>III',r,blobs_at+o if n else 0,n)
        for xn,xd,yn,yd,lo,hi in st['rules']:rec+=struct.pack('>BBBBhh',xn,xd,yn,yd,lo,hi)
        rec+=struct.pack('>IH',blobs_at+st['patch_at'] if st['patches'] else 0,len(st['patches']))
        rec+=struct.pack('>II',blobs_at+st['recon'][0] if st['recon'] else 0,st['recon'][1] if st['recon'] else 0)
        assert len(rec)<=STAGE_BYTES;head[stage_at+STAGE_BYTES*k:stage_at+STAGE_BYTES*k+len(rec)]=rec
    blob=bytes(head)+bytes(body)
    if fmt>=5:
        assert len(blob)%64==0 and len(trailer)<=5
        tail=b'MSXX'+struct.pack('>HH',len(trailer),0)+b''.join(struct.pack('>HHII',m,0,n,crc) for m,n,crc in trailer)
        blob+=tail+bytes(64-len(tail))
    return blob,dict(mission=mission,stages=report,pages=real_pages,chunks=len(chunks),palettes=len(palettes),images=len(images),cells=len(cells),
                     actors=len(actors),actor_images=len(actor_images),actor_pages=len({im[0] for im in actor_images}),resources=len(res),
                     header_bytes=len(head),mission_data=data_report,decoded_bytes=len(pages)*PAGE,packed_bytes=len(blob),version=fmt,
                     chunk_bytes=len(body),borrowed_chunks={m:sum(1 for _,r in borrowed if r[3]>>8==m) for m,_,_ in trailer},sha256=hashlib.sha256(blob).hexdigest())

STAGE_BYTES=128
SHARED_MIN=5   # pages of a chunk (of 8) the mission must use to take it from another pack
def recon_blob(stage,rom):
    """'MSXR' map reconnaissance of a stage (docs/MISION3.md), big endian:
      'MSXR' u16 version=1 u16 maps u16 patches u16 dynamic_maps u16 sections
      u8 train_slot_row u8 palette_numbers
      map[maps] (28 bytes): u32 layer_object u32 descriptor u16 block_columns
        u16 block_rows i16 x0 i16 y0 (model px) u16 camera_progress (FFFF:
        placed by an actor or a patch) u16 kind (1 tiles, 2 collision)
        u32 tiles_offset u32 collision_offset (from the blob start: the ROM
        words, tile u16 + attribute u16 per block, and the 4 collision bytes
        per block, column major as 0C72EC reads them)
      patch[patches] (20 bytes): u32 frame u32 sub-map u16 d0..d5 (0C70FC)
      dynamic[dynamic_maps] (12 bytes): u32 ROM map u32 offset u32 length
        (+0 dx dy w h, +4 flag, +5 group, 4 bytes, w*h cells; 0140C0)
      section[sections] (20 bytes): u32 first_frame u32 last_frame i16 camera0
        i16 camera1 u32 foreground map u32 palette_offset (palette_numbers x
        (u8 number, u8 0, u16 colours[16]) of the section's palette RAM)"""
    lay=json.loads((stage/'layers_rom.json').read_text());dyn=json.loads((stage/'dynamic.json').read_text())
    sections=json.loads((stage/'stage.json').read_text())['sections']
    subs=sorted({int(p['submap'],16) for p in dyn['patches']})
    maps=list(lay['maps'])+[dict(object='10C72E',descriptor='%06X'%sub,tiles='%06X'%rom_u32(rom,sub),collision='%06X'%rom_u32(rom,sub+4),
                                block_columns=rom_u16(rom,sub+8),block_rows=rom_u16(rom,sub+10),x0=0,y0=0) for sub in subs]
    pal=lay['section_palettes'];numbers=sorted(pal[0]['palettes'])
    dmaps=sorted(dyn['dynamic_maps'].items())
    base=16+28*len(maps)+20*len(dyn['patches'])+12*len(dmaps)+20*len(sections)
    body=bytearray()
    def put(data):
        while (base+len(body))%4:body.append(0)
        at=base+len(body);body.extend(data);return at
    recs=[]
    for m in maps:
        w,h=m['block_columns'],m['block_rows'];t,c=int(m['tiles'],16),int(m['collision'],16)
        to=put(rom_p2(rom,t,4*w*h)) if t not in (0,0xffffffff) else 0
        co=put(rom_p2(rom,c,4*w*h)) if c not in (0,0xffffffff) else 0
        cam=m.get('camera_progress');cam=0xffff if cam is None else cam
        recs.append(struct.pack('>IIHHhhHHII',int(m['object'],16),int(m['descriptor'],16),w,h,m['x0'],m['y0'],cam,(1 if to else 0)|(2 if co else 0),to,co))
    pats=[struct.pack('>II6H',p['frame'],int(p['submap'],16),*p['d']) for p in dyn['patches']]
    dm=[]
    for name,hexd in dmaps:
        data=bytes.fromhex(hexd);dm.append(struct.pack('>III',int(name,16),put(data),len(data)))
    secs=[]
    for sec in sections:
        pp=next(p for p in pal if p['section']==sec['id'])
        at=put(b''.join(struct.pack('>BB16H',int(n,16),0,*pp['palettes'][n]) for n in numbers))
        secs.append(struct.pack('>IIhhII',sec['frames'][0],sec['frames'][1],sec['camera'][0],sec['camera'][1],int(sec['map'],16),at))
    head=b'MSXR'+struct.pack('>HHHHHBB',1,len(recs),len(pats),len(dm),len(secs),dyn['train']['ring_slot_row'],len(numbers))
    head+=b''.join(recs)+b''.join(pats)+b''.join(dm)+b''.join(secs)
    assert len(head)==base
    return head+bytes(body)
def rom_u32(rom,a):return struct.unpack('>I',rom.read(a,4))[0]
def rom_u16(rom,a):return struct.unpack('>H',rom.read(a,2))[0]
def rom_p2(rom,a,n):
    # Map data pointers are linear P2 addresses (0C71CC): offset a - 0x100000.
    return bytes(rom.p2[a-0x100000:a-0x100000+n])
def fill_loop_layer(model,lay,crom):
    """Mission 3, the train 10D7B6 (layer 1, ROM map 0B8566): its column
    origin loops (neo_train.c), so MAME showed each layer x only at some
    camera/train phases and the first-seen cells leave holes (B58: black
    background where the port's phase differs). Layer x = 16 * block column
    of the ROM map (export_m3_layers.py: 3814 observed cells equal at their
    x, the others seen across an origin step): every position MAME never
    showed takes the ROM block (code, flip, auto-animation) with the colours
    the observed cells of the same palette number had at an equal block.
    Observed cells are kept; empty blocks (all pens 0) are left out."""
    from mission_tiles import decode
    m=next((m for m in lay['maps'] if m['object']=='10D7B6' and m.get('cells')),None)
    if m is None:return None
    layer=m['layer'];seen={(c['x'],c['y']):c for c in model['cells'] if c['layer']==layer}
    votes={}
    for x,y,code,flip,anim,n in m['cells']:
        c=seen.get((x,y))
        if c and c['code']==code and c['flip']==flip:votes.setdefault(n,{}).setdefault(c['palette'],0);votes[n][c['palette']]+=1
    pal={n:max(v,key=v.get) for n,v in votes.items()}
    added=empty=unknown=replaced=0
    # Observed cells that are not the ROM block at their x were seen across an
    # origin step (a frame whose +32 moved within the snapshot lag): the
    # wrong column there (G19: a forest at camera 1520 where MAME shows the
    # canyon). They take the ROM block with its voted colours.
    rom={(x,y):(code,flip,anim,n) for x,y,code,flip,anim,n in m['cells']}
    for c in [c for c in model['cells'] if c['layer']==layer]:
        r=rom.get((c['x'],c['y']))
        if r is None or (r[0],r[1])==(c['code'],c['flip']) or r[3] not in pal:continue
        c.update(code=r[0],flip=r[1],animation_mask=r[2],palette=pal[r[3]]);replaced+=1
    for x,y,code,flip,anim,n in m['cells']:
        if (x,y) in seen:continue
        if not any(decode(crom,code|ph,flip)!=[0]*256 for ph in range(anim+1)):empty+=1;continue
        if n not in pal:unknown+=1;continue
        model['cells'].append(dict(layer=layer,x=x,y=y,code=code,flip=flip,animation_mask=anim,palette=pal[n],observations=0));added+=1
    return dict(added=added,empty=empty,palette_unknown=unknown,palettes=len(pal),replaced=replaced)
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
LAYER_OBJECTS=(0x10E83E,0x10D7B6,0x10E8C6,0x10C72E)   # pack layers 0..3 (docs/MISION3.md)
def rom_layer_rules(rom,descriptor):
    """Mission 3 (docs/APROXIMACIONES.md A5): the layer rules from the stage's
    camera list instead of a fit. Op 0x1C (0C86C8) sets a layer object's
    factors +26/+28; 0C6F9E moves the layer by root step * factor / 256 (the
    remainder kept in +2A), so 0x100 is the root itself: x = camera. Op 0x18
    (0C8654) loads a map (+C) or, with 0, leaves the layer without one: it is
    drawn from its first map to the target where the map goes (progress =
    camera x on the first path, op 0x14 at (0, 0)). Layers whose motion is not
    a camera rule draw themselves (actor.c: the train ring, the sky pictures
    from 0x140). A layer that is never given a map is never drawn."""
    from fractions import Fraction as F
    a=rom.u32(descriptor);target=0;factors={};mapped={};start={};end={};rules=[]
    for _ in range(4000):
        op,n=rom.u16(a),rom.u16(a+2)
        if op==8:break
        assert n and n<0x40,('camera list',hex(a),op,n)
        if op==0:target=rom.u16(a+4)
        elif op==0x14:assert start or (rom.u16(a+4),rom.u16(a+6))==(0,0),('first path not at (0, 0)',hex(a))
        elif op==0x1c:factors.setdefault(rom.u32(a+4),[]).append((target,rom.s16(a+8),rom.s16(a+10)))
        elif op==0x18:
            o=rom.u32(a+4);m=rom.u32(a+12)!=0
            if m and not mapped.get(o):start.setdefault(o,target)
            if not m and mapped.get(o):end.setdefault(o,target)
            mapped[o]=m
        a+=n
    for o in LAYER_OBJECTS:
        if o not in start:rules.append((1,1,1,1,1,0));continue
        stop=end.get(o,1<<16)
        # The factors in force while the rule draws it: one pair.
        inside={(fx,fy) for t,fx,fy in factors.get(o,[]) if t<stop}
        assert len(inside)==1,('layer factor changes while drawn by its rule',hex(o),inside)
        fx,fy=(F(v,256) for v in inside.pop())
        # The renderer compares the camera word (the ceiling of the camera x)
        # with the bounds; the list compares the progress (its floor here):
        # progress < stop is camera <= stop but at x = stop, where the layer
        # already draws itself (actor.c). Only a start at 0 is expected.
        assert start[o]==0,('layer mapped after the start',hex(o))
        lo=-32768;hi=stop if o in end else 32767
        rules.append((fx.numerator,fx.denominator,fy.numerator,fy.denominator,lo,hi))
    return rules
def rom_native_rules(rom,descriptor,model):
    """Mission 4 on (docs/MISION4.md): the rules from the camera list, for a
    runtime that moves the layer objects itself (src/neo_train.c neo_layers_*:
    0C81C0 / 0C6F9E, ops 0x14 / 0x1C / 0x44). A layer whose factors stay
    0x100 / 0x100 and that is never put back at the root (op 0x44) follows the
    camera: rule 1/1. Any other mapped layer (the sky 10E83E: factors 0x40/0,
    0x80/0x80, 0x80/0 and two op 0x44) gets the rule that never draws
    (visible 1..0): the runtime draws it at its object's own +14/+18. The
    object 10D7B6 is the model's layer 2 while 10E8C6 has no map
    (tools/neo_scene.py), layer 1 otherwise. Nothing is fitted."""
    a=rom.u32(descriptor);factors={};rebased=set();mapped=set()
    for _ in range(4000):
        op,n=rom.u16(a),rom.u16(a+2)
        if op==8:break
        assert n and n<0x40,('camera list',hex(a),op,n)
        if op==0x1c:factors.setdefault(rom.u32(a+4),set()).add((rom.s16(a+8),rom.s16(a+10)))
        elif op==0x44:rebased.add(rom.u32(a+4))
        elif op==0x18 and rom.u32(a+12):mapped.add(rom.u32(a+4))
        a+=n
    never=(1,1,1,1,1,0);rules=[never]*4
    slots={0x10E83E:0,0x10D7B6:1 if 0x10E8C6 in mapped else 2,0x10E8C6:2,0x10C72E:3}
    for o in mapped:
        if factors.get(o)=={(0x100,0x100)} and o not in rebased:rules[slots[o]]=(1,1,1,1,-32768,32767)
    assert rules[3]!=never,'the terrain layer does not follow the camera'
    return rules
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
MISSION=[0];LOOSE=[False];LOOSE_ERRORS=[];ROM_LAYERS=[False]

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('mission',type=int);p.add_argument('stages',type=Path,nargs='+',help='stage directories in play order')
    p.add_argument('--crom',type=Path,default=ROOT/'filesystem/sprites.bin');p.add_argument('--output',type=Path)
    p.add_argument('--report',type=Path)
    p.add_argument('--loose-layers',action='store_true',help='Mission 3: layers that move on their own get the best median fit (worst error in the report)')
    p.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'),help='MAME set (P ROMs) for the per-stage ROM tables')
    p.add_argument('--rom-layers',action='store_true',help='Mission 4 on: layer rules from the camera list for the native layer objects, ROM grid with its row origin')
    p.add_argument('--format',type=int,choices=(4,5),default=5,help='pack version (4: run-pair chunks, nothing taken from other packs; for the comparison test)')
    p.add_argument('--shared',type=Path,nargs='*',help='version 5 packs whose chunks this one may use (default: the lower-numbered missionN.pack beside the output)')
    a=p.parse_args();LOOSE[0]=a.loose_layers;ROM_LAYERS[0]=a.rom_layers
    from decode_level_script import Program
    out=a.output or ROOT/f'romfs/mission{a.mission}.pack'
    shared=[]
    if a.format>=5:
        paths=a.shared if a.shared is not None else [q for m in range(2,a.mission) for q in [out.with_name(f'mission{m}.pack')] if q.exists()]
        shared=[pack_store.MissionPack(q) for q in paths]
        # A pack another one uses must sit beside the output under its own name (the cartridge's layout).
        assert all(q.path.resolve()==out.with_name(f'mission{q.mission}.pack').resolve() for q in shared),'--shared packs must be missionN.pack beside the output'
    blob,report=build(a.mission,a.stages,a.crom.read_bytes(),Program(a.rom),a.format,shared)
    # romfs/: the game loads it at the mission switch (docs/MISION2.md).
    out.parent.mkdir(exist_ok=True);out.write_bytes(blob)
    if a.format>=5:   # read back as every tool does: each chunk decodes to its CRC, here or in the pack that holds it
        back=pack_store.MissionPack(out)
        for i in range(len(back.chunks)):back.chunk(i)
    if LOOSE_ERRORS:report['loose_layer_errors']=[dict(layer=L,worst=w,median=m) for L,w,m in LOOSE_ERRORS]
    if a.report:a.report.write_text(json.dumps(report,indent=1)+'\n')
    print(json.dumps({k:v for k,v in report.items() if k!='stages'}))
    for s in report['stages']:print(' ',s)
if __name__=='__main__':main()
