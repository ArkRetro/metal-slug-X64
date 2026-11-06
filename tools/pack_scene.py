#!/usr/bin/env python3
"""Lossless RGBA5551 -> CI4 pages, deduplicated and RLE-packed in 16 KiB chunks."""
from pathlib import Path
import hashlib,json,shutil,struct,zlib
from mission_tiles import pack_map
from export_marco import palette_rgb
ROOT=Path(__file__).resolve().parents[1]
PAGE=2048; CHUNK=16384

def rle(data):
    out=bytearray();i=0
    while i<len(data):
        n=1
        while n<255 and i+n<len(data) and data[i+n]==data[i]: n+=1
        out.extend((n,data[i]));i+=n
    return bytes(out)

def unrle(data):
    assert len(data)%2==0
    out=b''.join(bytes([data[i+1]])*data[i] for i in range(0,len(data),2))
    assert len(out)==CHUNK
    return out

def main():
    pages=[]; page_ids={}; palettes=[(0,)+(0xffff,)*15]; pal_ids={palettes[0]:0}
    images=[];banks=[];background=[];source_bytes=0
    def palette_id(values):
        key=tuple(values)+(0,)*(16-len(values))
        if key not in pal_ids:pal_ids[key]=len(palettes);palettes.append(key)
        return pal_ids[key]
    def image(pixels,w,h,dx=0,dy=0,force_actor=False,indices=None,source_palette=None):
        colors=sorted(set(pixels)-{0}); indexed=indices is not None or len(colors)<=15 or (0 not in pixels and len(colors)<=16)
        if force_actor:assert len(colors)<=15
        if indexed:
            values=list(source_palette) if indices is not None else ([0]+colors if len(colors)<=15 else colors)
            mapping={v:i for i,v in enumerate(values)};data=bytearray(PAGE)
            for y in range(h):
                for x in range(w):data[y*32+x//2] |= (indices[y*w+x] if indices is not None else mapping[pixels[y*w+x]]) << (0 if x&1 else 4)
            # Independent round-trip verifies palette identity and nibble order.
            for y in range(h):
                for x in range(w):
                    pen=(data[y*32+x//2]>>(0 if x&1 else 4))&15
                    assert values[pen]==pixels[y*w+x]
                    if indices is not None:assert pen==indices[y*w+x]
            pal=palette_id(values);fmt=0
        else:
            assert w<=32 and h<=32
            data=bytearray(PAGE)
            for y in range(h):
                for x in range(w):struct.pack_into('>H',data,(y*32+x)*2,pixels[y*w+x])
            pal=65535;fmt=1
        key=(fmt,bytes(data))
        if key not in page_ids:page_ids[key]=len(pages);pages.append(bytes(data))
        return (page_ids[key],pal,dx,dy,w,h,fmt)
    for name in ('marco','marco-actions','soldier','marco-motion'):
        raw=(ROOT/f'filesystem/{name}.bin').read_bytes();source_bytes+=len(raw)
        magic,w,h,a,b,interval,_=struct.unpack('>4s6H',raw[:16]);assert magic==b'MSXA'
        assert len(raw)==16+(a+b)*w*h*2
        original=(ROOT/f'filesystem/{name}.idx4').read_bytes()
        assert struct.unpack('>4s6H',original[:16])==(b'MSXI',w,h,a,b,interval,0)
        indexed_stride=32+w*h//2
        assert len(original)==16+(a+b)*indexed_stride
        banks.append((len(images),a,b,interval,w,h))
        for f in range(a+b):
            pixels=list(struct.unpack('>'+str(w*h)+'H',raw[16+f*w*h*2:16+(f+1)*w*h*2]))
            pixels=[v if v&1 else 0 for v in pixels]
            opaque=[(i%w,i//w) for i,v in enumerate(pixels) if v]
            x0=min(x for x,y in opaque);x1=max(x for x,y in opaque)+1
            y0=min(y for x,y in opaque);y1=max(y for x,y in opaque)+1
            assert x1-x0<=64 and y1-y0<=64
            crop=[pixels[y*w+x] for y in range(y0,y1) for x in range(x0,x1)]
            start=16+f*indexed_stride
            palette=struct.unpack('>16H',original[start:start+32]);packed=original[start+32:start+indexed_stride]
            raw_indices=[(packed[(y*w+x)//2]>>(0 if x&1 else 4))&15 for y in range(y0,y1) for x in range(x0,x1)]
            images.append(image(crop,x1-x0,y1-y0,x0,y0,True,raw_indices,palette))
    projectile=json.loads((ROOT/'filesystem/projectiles.json').read_text())
    w,h=projectile['width'],projectile['height']
    assert (w,h)==(16,16) and projectile['pivot']==[8,8]
    packed=bytes.fromhex(projectile['indices_hex'])
    assert len(packed)==w*h//2 and hashlib.sha256(packed).hexdigest()==projectile['indices_sha256']
    indices=[(packed[i//2]>>(0 if i&1 else 4))&15 for i in range(w*h)]
    palette=projectile['palette_rgba5551'];assert len(palette)==16 and palette[0]==0
    x0,y0,x1,y1=projectile['bounds'];assert 0<=x0<x1<=w and 0<=y0<y1<=h
    assert all(x0<=i%w<x1 and y0<=i//w<y1 for i,p in enumerate(indices) if p)
    crop=[indices[y*w+x] for y in range(y0,y1) for x in range(x0,x1)]
    banks.append((len(images),1,0,1,w,h))
    images.append(image([palette[p] for p in crop],x1-x0,y1-y0,x0,y0,True,crop,palette))
    source_bytes+=w*h*2
    # Native soldier/death/throw descriptors (tools/export_soldier_sprites.py):
    # pens with origin-relative offsets, split into <=64x64 parts. The image
    # palette is the soldier's default resource 0x20E; the renderer overrides
    # it with the logical resource of each draw through scene_native_palette.
    native=json.loads((ROOT/'filesystem/soldier-native.json').read_text())
    # Mission 1 boss descriptors (tools/export_boss_sprites.py) share the
    # streamed SCENE_NATIVE bank; their palettes are other logical resources.
    boss=json.loads((ROOT/'filesystem/boss-native.json').read_text())
    native['frames']=native['frames']+boss['frames'];native['palettes']=dict(native['palettes'],**boss['palettes'])
    # Rest-of-mission generic actors (tools/export_rest_sprites.py), when exported.
    rest_native=ROOT/'filesystem/rest-native.json'
    if rest_native.exists():
        rest_sprites=json.loads(rest_native.read_text())
        known={f['descriptor'] for f in native['frames']}
        native['frames']+=[f for f in rest_sprites['frames'] if f['descriptor'] not in known]
        native['palettes']=dict(native['palettes'],**rest_sprites['palettes'])
    # BLOOD ON palettes (tools/export_blood_palettes.py): splash 0x37A, pieces 0x18.
    blood_native=ROOT/'filesystem/blood-native.json'
    if blood_native.exists():
        for k,v in json.loads(blood_native.read_text())['palettes'].items():native['palettes'].setdefault(k,v)
    native_palettes={int(k,16):palette_id(v) for k,v in native['palettes'].items()}
    base_palette=native['palettes']['0x20e'];descriptors=[];native_start=len(images)
    # Pack the descriptors in program order so that one animation streams as
    # few consecutive chunks as possible (SCENE_NATIVE is not pinned).
    programs=json.loads((ROOT/'data/soldier_programs.json').read_text())['programs'];rank={}
    boss_programs=json.loads((ROOT/'data/boss_programs.json').read_text())['programs']
    for prog in programs+boss_programs:
        for c in prog['commands']:
            if c['kind']=='POSE':rank.setdefault(c['asset'],len(rank))
    native_frames=sorted(native['frames'],key=lambda f:rank.get(int(f['descriptor'],16),len(rank)))
    for f in native_frames:
        pens=bytes.fromhex(f['pens_hex']);w,h=f['width'],f['height'];first=len(images)
        for py0 in range(0,h,64):
            for px0 in range(0,w,64):
                pw,ph=min(64,w-px0),min(64,h-py0)
                crop=[pens[(py0+y)*w+px0+x] for y in range(ph) for x in range(pw)]
                images.append(image([base_palette[q] for q in crop],pw,ph,f['x']+px0,f['y']+py0,True,crop,base_palette))
        descriptors.append((int(f["descriptor"],16),first-native_start,len(images)-first))
    # Descriptors without any tile ("empty" of the exporters; 0x1C3F06 is one
    # group of zero columns, the blank pose of the programs 247DA8 (armoured
    # vehicle children) and 24AC42): a draw of one shows nothing on the
    # original, so they are packed with no parts instead of being missing.
    empty=set(boss.get('empty',[]))|(set(rest_sprites.get('empty',[])) if rest_native.exists() else set())
    packed={d[0] for d in descriptors}
    descriptors+=[(t,0,0) for t in sorted(int(e,16) for e in empty) if t not in packed]
    banks.append((native_start,len(images)-native_start,0,1,64,64))
    source_bytes+=sum(f['width']*f['height']*2 for f in native['frames'])
    raw=(ROOT/'filesystem/desert.bin').read_bytes();source_bytes+=len(raw)
    pixels=struct.unpack('>71680H',raw)
    for row in range(7):
        for col in range(10):
            w=min(32,304-col*32)
            crop=[pixels[(row*32+y)*320+8+col*32+x] for y in range(32) for x in range(w)]
            background.append(image(crop,w,32))
    mission=json.loads((ROOT/'filesystem/mission1/map.json').read_text())
    assert len(mission.get('backdrop_values',[mission['backdrop']]))==1,'Multiple observed backdrops need native event playback before scene packing'
    # Rest of the mission (camera 1920..3520, docs/MISSION1_REST.md): merged
    # with the region map; overlapping cells must be identical. The second
    # backdrop is the palette loaded by the camera command list at progress
    # 2304 (ROM 0B56CA); the runtime switches on that command's event.
    backdrop_after=None
    rest_path=ROOT/'filesystem/mission1/rest/map.json'
    if rest_path.exists():
        from estimate_mission_budget import merge
        rest=json.loads(rest_path.read_text())
        for c in mission['cells']:c['segment']=0
        mission,_=merge(dict(mission,conflicts=[]),dict(rest,conflicts=[]))
        for c in mission['cells']:c.setdefault('segment',1)
        # Destroyed-shop patch (0B6882, sub-map 0B6AD2 on layer 10D7B6): the
        # 62 coordinate variants of the rest map; the packed map keeps the
        # first observed (intact) cell and the other choice goes to a
        # separate list the renderer swaps in once the building is down
        # (tools/decode_map_commands.py --map checks them against the ROM).
        base={(c['layer'],c['x'],c['y']):c for c in mission['cells']}
        for cf in rest['conflicts']:
            b=base[(cf['layer'],cf['x'],cf['y'])];bpal=mission['palettes'][b['palette']]
            post=[ch for ch in cf['choices'] if (ch['code'],ch['flip'],list(ch['palette']))!=(b['code'],b['flip'],list(bpal))]
            assert len(post)==1,('Patch variant',cf)
            ch=post[0];pal=list(ch['palette'])
            if pal not in mission['palettes']:mission['palettes'].append(pal)
            mission['cells'].append(dict(layer=5,x=cf['x'],y=cf['y'],code=ch['code'],flip=ch['flip'],animation_mask=ch['animation'],
                palette=mission['palettes'].index(pal),segment=1))
        # Rubble map 0B53A8 that the collapse 0B58CC loads into 10C72E
        # (swapped_cells of tools/extract_mission_map.py, display coordinates):
        # virtual layer 6, drawn as layer 3 once the building is down.
        for c in rest.get('swapped_cells',[]):
            assert (c['layer'],c['map'])==(3,'0x0B53A8'),('Unexpected swapped map',c)
            pal=list(rest['palettes'][c['palette']])
            if pal not in mission['palettes']:mission['palettes'].append(pal)
            mission['cells'].append(dict(layer=6,x=c['x'],y=c['y'],code=c['code'],flip=c['flip'],animation_mask=c['animation_mask'],
                palette=mission['palettes'].index(pal),segment=1))
        values=[v for v in rest['backdrop_values'] if v!=mission['backdrop']]
        assert len(values)==1;backdrop_after=values[0]
    props=json.loads((ROOT/'filesystem/mission1/props.json').read_text())
    palette_base=len(mission['palettes']);mission['palettes'].extend(props['palettes'])
    mission['cells'].extend(dict(c,palette=c['palette']+palette_base) for c in props['cells'])
    backdrop=palette_rgb(mission['backdrop'])
    # The map's pages start on a chunk (8 pages): the streamed map working
    # set (check_scene_budget.py, 32 chunks with the 4 MiB cache) no longer
    # depends on how many actor pages come before it (B51: the burning
    # figures 08732C shifted the map by part of a chunk: 33 at camera 303).
    while len(pages)%8:pages.append(bytes(PAGE))
    map_images,map_cells=pack_map(mission,(ROOT/'filesystem/sprites.bin').read_bytes(),image)
    patch_cells=[c for c in map_cells if c[0]==5];collapse_cells=[c for c in map_cells if c[0]==6];map_cells=[c for c in map_cells if c[0]<5]
    real_pages=len(pages)
    while len(pages)%8:pages.append(bytes(PAGE))
    blob=bytearray();chunks=[]
    for start in range(0,len(pages),8):
        decoded=b''.join(pages[start:start+8]);packed=rle(decoded)
        if len(packed)<len(decoded):codec=1;assert unrle(packed)==decoded
        else:codec=0;packed=decoded
        assert len(blob)%64==0
        chunks.append((len(blob),len(packed),zlib.crc32(decoded),codec))
        blob.extend(packed);blob.extend(bytes((-len(blob))%64))
    (ROOT/'romfs').mkdir(exist_ok=True);(ROOT/'build').mkdir(exist_ok=True)
    (ROOT/'romfs/scene.pack').write_bytes(blob)
    for name in ('fixed.bin','sprites.bin'):shutil.copyfile(ROOT/'filesystem'/name,ROOT/'romfs'/name)
    lines=['/* Generated by tools/pack_scene.py; do not edit. */','#ifndef SCENE_META_H','#define SCENE_META_H',
           f'#define SCENE_PAGE_COUNT {real_pages}',f'#define SCENE_CHUNK_COUNT {len(chunks)}',
           f'#define SCENE_PALETTE_COUNT {len(palettes)}',f'#define SCENE_PACK_BYTES {len(blob)}',
           f'#define SCENE_MAP_CELL_COUNT {len(map_cells)}',f'#define SCENE_MAP_CAMERA_MAX {mission["camera_max"]}',
           f'#define SCENE_MAP_BACKDROP 0x{bytes((*backdrop,255)).hex()}u',
           f'#define SCENE_MAP_BACKDROP_AFTER 0x{bytes((*palette_rgb(backdrop_after if backdrop_after is not None else mission["backdrop"]),255)).hex()}u']
    for ctype,name,items in [('scene_chunk_t','scene_chunks',chunks),('scene_bank_t','scene_banks',banks),('scene_image_t','scene_images',images),('scene_image_t','scene_background',background)]:
        lines.append(f'static const {ctype} {name}[]={{')
        lines.extend('  {'+','.join(str(v) for v in item)+'},' for item in items);lines.append('};')
    lines.append('static const scene_native_descriptor_t scene_native_descriptors[]={')
    lines.extend('  {0x%x,%d,%d},'%d for d in sorted(descriptors));lines.append('};')
    # Streaming working set of the native bank: chunks of each program's poses;
    # the three largest model three concurrent animations (budget check).
    parts={d:[images[native_start+first+i][0] for i in range(count)] for d,first,count in descriptors}
    program_chunks=sorted((len({pg//8 for c in prog['commands'] if c['kind']=='POSE' for pg in parts.get(c['asset'],())}) for prog in programs),reverse=True)
    lines.append(f'#define SCENE_NATIVE_PROGRAM_CHUNKS_MAX3 {sum(program_chunks[:3])}')
    # Boss: chunks of every boss pose (the whole fight's set) and of the
    # largest single program; tools/check_boss_budget.py measures the set
    # drawn per logic pass of the recorded fights.
    boss_tags={int(f['descriptor'],16) for f in boss['frames']}
    boss_chunks={pg//8 for d in boss_tags for pg in parts[d]}
    lines.append(f'#define SCENE_BOSS_CHUNKS {len(boss_chunks)}')
    # Prefetched while the boss fight runs (scene_prefetch_boss).
    lines.append('#define SCENE_BOSS_CHUNK_LIST {'+','.join(str(c) for c in sorted(boss_chunks))+'}')
    lines.append(f'#define SCENE_BOSS_PAGES {len({pg for d in boss_tags for pg in parts[d]})}')
    # Script lookahead (scene_prefetch_ahead): every spawn of the
    # rest-of-mission inventory (data/mission1_rest_inventory.json: the
    # script's spawn records, the camera of each spawn and the descriptors
    # each actor kind drew in the MAME playthrough) gives the chunks of those
    # descriptors keyed by the spawn camera; they are requested while the
    # camera approaches it. Sorted by camera.
    ahead=set()
    inventory=ROOT/'data/mission1_rest_inventory.json'
    if inventory.exists():
        for kind in json.loads(inventory.read_text())['kinds']:
            kind_chunks={pg//8 for d in kind['descriptors'] for pg in parts.get(int(d['address'],16),())}
            for spawn in kind['spawns']:ahead.update((spawn['camera'],c) for c in kind_chunks)
    ahead=sorted(ahead)
    lines.append(f'#define SCENE_AHEAD_COUNT {len(ahead)}')
    lines.append('static const uint16_t scene_ahead[][2]={'+(','.join('{%d,%d}'%a for a in ahead) or '{0xffff,0}')+'};')
    soldier_chunks={pg//8 for prog in programs for c in prog['commands'] if c['kind']=='POSE' for pg in parts.get(c['asset'],())}
    # 16-pixel sprite columns of each boss descriptor (origin-relative x,
    # unmirrored): the original builder drops a column whose left edge is
    # outside -8..311 (00A17A..00A18E); boss_renderer.c clips likewise.
    cl=['/* Generated by tools/pack_scene.py; do not edit. */','#ifndef BOSS_COLUMNS_H','#define BOSS_COLUMNS_H','#include <stdint.h>']
    starts=[];index=[]
    for f in sorted(boss['frames'],key=lambda f:int(f['descriptor'],16)):
        index.append((int(f['descriptor'],16),len(starts),len(f['columns'])));starts+=f['columns']
    cl.append('typedef struct { uint32_t tag; uint16_t first,count; } boss_column_run_t;')
    cl.append('static const int16_t boss_column_starts[]={%s};'%','.join(map(str,starts)))
    cl.append('static const boss_column_run_t boss_column_runs[]={%s};'%','.join('{0x%x,%d,%d}'%x for x in index))
    cl.append('#define BOSS_EMPTY_DESCRIPTORS {%s}'%','.join(boss.get('empty',[])))
    cl.append('#endif')
    (ROOT/'build/boss_columns.h').write_text('\n'.join(cl)+'\n')
    lines.append('static const scene_native_palette_t scene_native_palettes[]={')
    lines.extend('  {0x%x,%d},'%(k,v) for k,v in sorted(native_palettes.items()));lines.append('};')
    lines.append('static const scene_image_t scene_map_images[]={')
    lines.extend('  {'+','.join(map(str,item))+'},' for item in map_images);lines.append('};')
    # Map cells as 8-byte records (src/scene_assets.h): the first phase's
    # image and, for an animated cell, the index of its phase pattern (image
    # deltas per video phase, shared: Mission 1 has one) instead of 8 ids
    # per cell (22 bytes; ~61 KB of the program image, docs/RENDIMIENTO.md).
    patterns=[]
    def cell(layer,x,y,phases):
        d=tuple(p-phases[0] for p in phases)
        if not any(d): return '  {%d,255,%d,%d,%d},'%(layer,x,y,phases[0])
        assert all(-128<=v<=127 for v in d),'cell phase delta out of int8'
        if d not in patterns: patterns.append(d)
        return '  {%d,%d,%d,%d,%d},'%(layer,patterns.index(d),x,y,phases[0])
    lines.append('static const scene_map_cell_t scene_map_cells_data[]={')
    lines.extend(cell(layer,x,y,phases) for layer,x,y,phases in map_cells);lines.append('};')
    lines.append(f'#define SCENE_PATCH_CELL_COUNT {len(patch_cells)}')
    lines.append('static const scene_map_cell_t scene_patch_cells_data[]={')
    lines.extend(cell(2,x,y,phases) for layer,x,y,phases in patch_cells)
    if not patch_cells:lines.append('  {0,255,0,0,0},')
    lines.append('};')
    lines.append(f'#define SCENE_COLLAPSE_CELL_COUNT {len(collapse_cells)}')
    lines.append('static const scene_map_cell_t scene_collapse_cells_data[]={')
    lines.extend(cell(3,x,y,phases) for layer,x,y,phases in collapse_cells)
    if not collapse_cells:lines.append('  {0,255,0,0,0},')
    lines.append('};')
    assert len(patterns)<=8,'more cell phase patterns than SCENE_CELL_PATTERNS leaves for the stages'
    lines.append(f'#define SCENE_CELL_PATTERN_COUNT {len(patterns)}')
    lines.append('#define SCENE_CELL_PATTERNS_INIT {'+','.join('{'+','.join(map(str,p))+'}' for p in patterns)+'}')
    lines.append('static const uint16_t scene_palettes[][16] __attribute__((aligned(16)))={')
    lines.extend('  {'+','.join(hex(v) for v in pal)+'},' for pal in palettes);lines.extend(['};','#endif'])
    (ROOT/'build/scene_meta.h').write_text('\n'.join(lines)+'\n')
    report=dict(frames=len(images),native_descriptors=len(descriptors),boss_descriptors=len(boss_tags),boss_pages=len({pg for d in boss_tags for pg in parts[d]}),boss_chunks=len(boss_chunks),native_palettes=len(native_palettes),pages=real_pages,palettes=len(palettes),chunks=len(chunks),decoded_bytes=len(pages)*PAGE,packed_bytes=len(blob),source_rgba_bytes=source_bytes,sha256=hashlib.sha256(blob).hexdigest(),background_ci4=sum(i[-1]==0 for i in background),background_rgba16=sum(i[-1]==1 for i in background),ahead_pairs=len(ahead),ahead_chunks=len({c for _,c in ahead}),soldier_program_chunks=len(soldier_chunks))
    (ROOT/'build/scene_pack_report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))

if __name__=='__main__':main()
