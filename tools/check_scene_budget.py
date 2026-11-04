#!/usr/bin/env python3
"""Check the streamed scene working set against the 4 MiB and 8 MiB caches.

Mirrors scene_assets.c/scene_renderer.c: pinned player banks, the soldier and
projectile banks, and every map cell inside the -48..352 prefetch margin for
all eight animation phases. Layer windows and scroll come from
src/mission_camera.h; frames in flight are covered by taking the union over a
16-pixel camera window (the camera advances at most a few pixels per frame).
"""
import argparse,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
CHUNK=16384;WINDOW=16

def table(text,name):
    body=text[text.index(name+'[]={'):];body=body[:body.index('};')]
    return [list(map(int,re.findall(r'-?\d+',line))) for line in body.splitlines()[1:]]

def constant(text,name):
    return int(re.search(name+r'=(\d+)',text).group(1))

def scroll(layer,value):
    return (value+1)//2 if layer==0 else (value*3+2)//4 if layer==1 else value

def check(meta_path,camera_path):
    meta=meta_path.read_text();camera=camera_path.read_text()
    camera_max=constant(camera,'MSX_CAMERA_MAX');switch=constant(camera,'MSX_MID_FULL_SPEED_CAMERA')
    assert int(re.search(r'SCENE_MAP_CAMERA_MAX (\d+)',meta).group(1))==camera_max,'Pack and runtime camera limits differ'
    chunks=int(re.search(r'SCENE_CHUNK_COUNT (\d+)',meta).group(1))
    banks=table(meta,'scene_banks');images=table(meta,'scene_images')
    map_images=table(meta,'scene_map_images');cells=table(meta,'scene_map_cells_data')
    fixed=set()
    for start,first,second,*_ in banks:
        fixed.update(images[i][0]//8 for i in range(start,start+first+second))
    per_camera=[]
    for c in range(camera_max+1):
        used=set()
        for layer,x,y,size,*phases in cells:
            source=min(layer,3)
            if (source==1 and c>=switch) or (source==2 and c<switch):continue
            if -48<=x-scroll(source,c)<352:used.update(map_images[p][0]//8 for p in phases)
        per_camera.append(used)
    worst=(0,0)
    for c in range(camera_max+1):
        union=set(fixed).union(*per_camera[c:c+WINDOW])
        worst=max(worst,(len(union),c))
    caches={'4MiB':512*1024//CHUNK,'8MiB':3*1024*1024//CHUNK}
    report=dict(chunks=chunks,fixed_actor_chunks=len(fixed),worst_working_set=worst[0],worst_camera=worst[1],
                cache_slots={k:min(v,chunks) for k,v in caches.items()},
                fits_without_eviction={k:chunks<=v for k,v in caches.items()})
    cell_size=int(re.search(r'SCENE_MAP_CELL_SIZE (\d+)',meta).group(1))
    report['cell_size']=cell_size
    report['requires_expansion_pak']=cell_size==32
    report['working_set_fits']={k:worst[0]<=v for k,v in report['cache_slots'].items()}
    for name,slots in report['cache_slots'].items():
        # The prerender campaign explicitly requires 8 MiB. The CI4 reference
        # path must continue to fit both supported cache sizes.
        if cell_size==32 and name=='4MiB':continue
        assert worst[0]<=slots,('Scene working set exceeds cache',name,worst,slots)
    return report

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--meta',type=Path,default=ROOT/'build/scene_meta.h')
    p.add_argument('--camera',type=Path,default=ROOT/'src/mission_camera.h')
    a=p.parse_args();print(json.dumps(check(a.meta,a.camera)))
