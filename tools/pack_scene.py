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
    props=json.loads((ROOT/'filesystem/mission1/props.json').read_text())
    palette_base=len(mission['palettes']);mission['palettes'].extend(props['palettes'])
    mission['cells'].extend(dict(c,palette=c['palette']+palette_base) for c in props['cells'])
    backdrop=palette_rgb(mission['backdrop'])
    map_images,map_cells=pack_map(mission,(ROOT/'filesystem/sprites.bin').read_bytes(),image)
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
           f'#define SCENE_MAP_BACKDROP 0x{bytes((*backdrop,255)).hex()}u']
    for ctype,name,items in [('scene_chunk_t','scene_chunks',chunks),('scene_bank_t','scene_banks',banks),('scene_image_t','scene_images',images),('scene_image_t','scene_background',background)]:
        lines.append(f'static const {ctype} {name}[]={{')
        lines.extend('  {'+','.join(str(v) for v in item)+'},' for item in items);lines.append('};')
    lines.append('static const scene_native_descriptor_t scene_native_descriptors[]={')
    lines.extend('  {0x%x,%d,%d},'%d for d in sorted(descriptors));lines.append('};')
    # Streaming working set of the native bank: chunks of each program's poses;
    # the three largest model three concurrent animations (budget check).
    parts={d:[images[native_start+first+i][0] for i in range(count)] for d,first,count in descriptors}
    program_chunks=sorted((len({pg//8 for c in prog['commands'] if c['kind']=='POSE' for pg in parts[c['asset']]}) for prog in programs),reverse=True)
    lines.append(f'#define SCENE_NATIVE_PROGRAM_CHUNKS_MAX3 {sum(program_chunks[:3])}')
    # Boss: chunks of every boss pose (the whole fight's set) and of the
    # largest single program; tools/check_boss_budget.py measures the set
    # drawn per logic pass of the recorded fights.
    boss_tags={int(f['descriptor'],16) for f in boss['frames']}
    boss_chunks={pg//8 for d in boss_tags for pg in parts[d]}
    lines.append(f'#define SCENE_BOSS_CHUNKS {len(boss_chunks)}')
    lines.append(f'#define SCENE_BOSS_PAGES {len({pg for d in boss_tags for pg in parts[d]})}')
    lines.append('static const scene_native_palette_t scene_native_palettes[]={')
    lines.extend('  {0x%x,%d},'%(k,v) for k,v in sorted(native_palettes.items()));lines.append('};')
    lines.append('static const scene_image_t scene_map_images[]={')
    lines.extend('  {'+','.join(map(str,item))+'},' for item in map_images);lines.append('};')
    lines.append('static const scene_map_cell_t scene_map_cells_data[]={')
    lines.extend('  {'+f'{layer},{x},{y},'+'{'+','.join(map(str,phases))+'}},' for layer,x,y,phases in map_cells);lines.append('};')
    lines.append('static const uint16_t scene_palettes[][16] __attribute__((aligned(16)))={')
    lines.extend('  {'+','.join(hex(v) for v in pal)+'},' for pal in palettes);lines.extend(['};','#endif'])
    (ROOT/'build/scene_meta.h').write_text('\n'.join(lines)+'\n')
    report=dict(frames=len(images),native_descriptors=len(descriptors),boss_descriptors=len(boss_tags),boss_pages=len({pg for d in boss_tags for pg in parts[d]}),boss_chunks=len(boss_chunks),native_palettes=len(native_palettes),pages=real_pages,palettes=len(palettes),chunks=len(chunks),decoded_bytes=len(pages)*PAGE,packed_bytes=len(blob),source_rgba_bytes=source_bytes,sha256=hashlib.sha256(blob).hexdigest(),background_ci4=sum(i[-1]==0 for i in background),background_rgba16=sum(i[-1]==1 for i in background))
    (ROOT/'build/scene_pack_report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))

if __name__=='__main__':main()
