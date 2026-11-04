#!/usr/bin/env python3
"""Lossless RGBA5551 -> CI4 pages, deduplicated and RLE-packed in 16 KiB chunks."""
from pathlib import Path
import argparse,hashlib,json,shutil,struct,zlib
from mission_tiles import pack_map,rgba16
from build_prerender_background import pack_prerender_map
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
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--background',choices=('tiles','prerender'),default='prerender')
    args=parser.parse_args()
    pages=[]; page_ids={}; palettes=[(0,)+(0xffff,)*15]; pal_ids={palettes[0]:0}
    images=[];banks=[];background=[];source_bytes=0;ci8_palettes=[]
    def palette_id(values):
        key=tuple(values)+(0,)*(16-len(values))
        if key not in pal_ids:pal_ids[key]=len(palettes);palettes.append(key)
        return pal_ids[key]
    def image(pixels,w,h,dx=0,dy=0,force_actor=False,indices=None,source_palette=None,force_rgba=False,force_ci8=False):
        colors=sorted(set(pixels)-{0}); indexed=indices is not None or len(colors)<=15 or (0 not in pixels and len(colors)<=16)
        if force_rgba:indexed=False
        if force_actor:assert len(colors)<=15
        if force_ci8:
            assert w==32 and h==32 and len(ci8_palettes)==1
            values=ci8_palettes[0];mapping={v:i for i,v in enumerate(values)}
            data=bytearray(PAGE)
            for y in range(h):
                for x in range(w):
                    data[y*32+x]=mapping[pixels[y*w+x]]
                    assert values[data[y*32+x]]==pixels[y*w+x]
            pal=0;fmt=2
        elif indexed:
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
    crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    if args.background=='prerender':
        # Scaled distant layers retain their original CI4 sampling intervals.
        # Baking their atlas boundaries changes RDP edge samples at fractional
        # scales. Terrain/buildings/props (unit scale) can be composed exactly.
        distant=dict(mission,cells=[c for c in mission['cells'] if c['layer']<2])
        scenery=dict(mission,cells=[c for c in mission['cells'] if c['layer']>=2])
        map_images,map_cells=pack_map(distant,crom,image)
        colors=sorted({v for c in scenery['cells'] for v in rgba16(mission['palettes'][c['palette']])})
        if len(colors)<=256:ci8_palettes.append(tuple(colors))
        baked_images,baked_cells,prerender_report=pack_prerender_map(scenery,crom,image,'ci8' if ci8_palettes else 'rgba16')
        prerender_report['shared_palette_colors']=len(colors) if ci8_palettes else 0
        base=len(map_images);map_images.extend(baked_images)
        map_cells.extend((layer,x,y,[base+p for p in phases]) for layer,x,y,phases in baked_cells)
        prerender_report['distant_layer_policy']='Original CI4 tiles on scaled layers 0/1; prerendered unit-scale layers 2/3/4'
        cell_size=32
        (ROOT/'build/prerender_background_report.json').write_text(json.dumps(prerender_report,indent=2)+'\n')
    else:
        map_images,map_cells=pack_map(mission,crom,image)
        cell_size=16
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
           f'#define SCENE_MAP_CELL_SIZE {cell_size}',
           f'#define SCENE_MAP_BACKDROP 0x{bytes((*backdrop,255)).hex()}u']
    for ctype,name,items in [('scene_chunk_t','scene_chunks',chunks),('scene_bank_t','scene_banks',banks),('scene_image_t','scene_images',images),('scene_image_t','scene_background',background)]:
        lines.append(f'static const {ctype} {name}[]={{')
        lines.extend('  {'+','.join(str(v) for v in item)+'},' for item in items);lines.append('};')
    lines.append('static const scene_image_t scene_map_images[]={')
    lines.extend('  {'+','.join(map(str,item))+'},' for item in map_images);lines.append('};')
    lines.append('static const scene_map_cell_t scene_map_cells_data[]={')
    lines.extend('  {'+f'{layer},{x},{y},{16 if args.background=="tiles" or layer<2 else 32},'+'{'+','.join(map(str,phases))+'}},' for layer,x,y,phases in map_cells);lines.append('};')
    lines.append('static const uint16_t scene_palettes[][16] __attribute__((aligned(16)))={')
    lines.extend('  {'+','.join(hex(v) for v in pal)+'},' for pal in palettes);lines.append('};')
    lines.append('static const uint16_t scene_ci8_palettes[][256] __attribute__((aligned(16)))={')
    lines.extend('  {'+','.join(hex(v) for v in pal)+'},' for pal in (ci8_palettes or [(0,)]));lines.extend(['};','#endif'])
    (ROOT/'build/scene_meta.h').write_text('\n'.join(lines)+'\n')
    report=dict(background_mode=args.background,map_cells=len(map_cells),map_cell_size=cell_size,frames=len(images),pages=real_pages,palettes=len(palettes),ci8_palettes=len(ci8_palettes),ci8_colors=len(ci8_palettes[0]) if ci8_palettes else 0,chunks=len(chunks),decoded_bytes=len(pages)*PAGE,packed_bytes=len(blob),source_rgba_bytes=source_bytes,sha256=hashlib.sha256(blob).hexdigest(),background_ci4=sum(i[-1]==0 for i in background),background_rgba16=sum(i[-1]==1 for i in background))
    (ROOT/'build/scene_pack_report.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps(report))

if __name__=='__main__':main()
