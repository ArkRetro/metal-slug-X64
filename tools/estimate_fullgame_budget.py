#!/usr/bin/env python3
"""Scene budget of every mission area map (tools/extract_mission_map.py
--ram-layers output) with the paging rules of tools/pack_scene.py, plus a
whole-cartridge estimate.

Map pages: tools/mission_tiles.pack_map groups the tiles (every animation
phase) of each palette in 64x64 CI4 atlases of 16 tiles (2048 bytes); pages
are stored 8 per 16 KiB chunk and RLE-packed when smaller (pack_scene.rle).
Working set: for every observation of the map (one per snapshot, exact layer
offsets), the chunks of every cell within the renderer's 48 px prefetch
margin (horizontal and vertical) for this and the next observation, plus the
12 pinned actor chunks of the current pack (Marco, transitions, soldier;
docs/ESQUEMA_PORT_N64.md). Caches: 32 chunk slots (4 MiB console) and 192
(3 MiB cache of the 8 MiB console). Actor graphics come from the per-mission
inventories (unique 16x16 tiles of the drawn descriptors, CI4 128 B/tile).
Nothing in filesystem/, build/ or romfs/ is written."""
import argparse,json
from pathlib import Path
from mission_tiles import pack_map,decode
from pack_scene import rle,PAGE,CHUNK
ROOT=Path(__file__).resolve().parents[1]
PINNED=12;SLOTS={'4MiB':32,'8MiB':192};MARGIN=48

def with_variants(model):
    """Recorded coordinate variants must be resident too: each alternative
    becomes a virtual cell of its layer (as tools/estimate_mission_budget.py)."""
    model=dict(model);palettes=[list(p) for p in model['palettes']];index={tuple(p):i for i,p in enumerate(palettes)}
    cells=list(model['cells'])
    for v in model['conflicts']:
        for ch in v['choices']:
            key=tuple(ch['palette'])
            if key not in index:index[key]=len(palettes);palettes.append(list(key))
            cells.append(dict(layer=v['layer'],x=v['x'],y=v['y'],code=ch['code'],flip=ch['flip'],animation_mask=ch['animation'],palette=index[key]))
    model.update(cells=cells,palettes=palettes);return model

def area_budget(model,crom):
    model=with_variants(model);pages=[]
    def make_image(pixels,w,h,dx=0,dy=0,force_actor=False,indices=None,source_palette=None):
        data=bytearray(PAGE)
        for y in range(h):
            for x in range(w):data[y*32+x//2]|=indices[y*w+x]<<(0 if x&1 else 4)
        pages.append(bytes(data));return (len(pages)-1,0,dx,dy,w,h,0)
    images,cells=pack_map(model,crom,make_image)
    while len(pages)%8:pages.append(bytes(PAGE))
    packed=0
    for s in range(0,len(pages),8):
        d=b''.join(pages[s:s+8]);packed+=min(len(rle(d)),len(d))
    chunks=len(pages)//8
    by_layer={}
    for layer,x,y,phases in cells:by_layer.setdefault(layer,[]).append((x,y,{images[q][0]//8 for q in phases}))
    per=[]
    for o in model['observations']:
        used=set()
        for layer,items in by_layer.items():
            if not o['active'][layer]:continue
            ox=o['offsets'][layer];oy=-o['vertical_offsets'][layer]
            for x,y,ch in items:
                if -MARGIN-16<x-ox<320+MARGIN and -MARGIN-16<y-oy<224+MARGIN:used|=ch
        per.append((o['frame'],used))
    worst=(0,None)
    for i,(f,u) in enumerate(per):
        w=len(u|(per[i+1][1] if i+1<len(per) else set()))+PINNED
        worst=max(worst,(w,f))
    return dict(cells=len(model['cells']),palettes=len(model['palettes']),pages=len([p for p in pages if any(p)]),
        chunks=chunks,decoded_bytes=chunks*CHUNK,packed_bytes=packed,observations=len(per),
        worst_working_set=worst[0],worst_frame=worst[1],pinned_actor_chunks=PINNED,
        fits={k:worst[0]<=v for k,v in SLOTS.items()})

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--maps',type=json.loads,required=True,help='{"2":["path/map.json",...],...}')
    p.add_argument('--inventories',type=json.loads,default={},help='{"2":"data/mission2_inventory.json",...}')
    p.add_argument('--crom',type=Path,default=ROOT/'filesystem/sprites.bin')
    p.add_argument('--output',type=Path)
    a=p.parse_args();crom=a.crom.read_bytes();out=dict(source=__doc__.splitlines()[0],missions={})
    total=dict(map_packed=0,map_tiles=set(),actor_tiles=set())
    for mission,paths in a.maps.items():
        m=dict(areas={})
        for path in paths:
            model=json.loads(Path(path).read_text())
            b=area_budget(model,crom);m['areas'][Path(path).parent.name]=b
            total['map_packed']+=b['packed_bytes']
            for c in model['cells']:
                for ph in range(c['animation_mask']+1):total['map_tiles'].add(c['code']|ph)
        m['map_packed_bytes']=sum(b['packed_bytes'] for b in m['areas'].values())
        m['worst_working_set']=max(b['worst_working_set'] for b in m['areas'].values())
        inv=a.inventories.get(mission)
        if inv:
            inv=json.loads(Path(inv).read_text());tiles=set()
            for k in inv['kinds']:tiles.update(k.get('tile_codes',[]))
            m['actor_unique_tiles']=sum(k['unique_tiles'] for k in inv['kinds']);total['actor_tiles'].update(tiles)
            m['actor_ci4_bytes']=m['actor_unique_tiles']*128
        out['missions'][mission]=m
    out['totals']=dict(map_packed_bytes=total['map_packed'],map_unique_tiles=len(total['map_tiles']))
    if a.output:a.output.write_text(json.dumps(out,indent=1)+'\n')
    print(json.dumps(out,indent=1))

if __name__=='__main__':main()
