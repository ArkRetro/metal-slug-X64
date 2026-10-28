#!/usr/bin/env python3
"""Compare the runtime camera model (src/mission_camera.h) with measured frames.

Each observation in map.json stores the per-layer scroll measured from SCBs.
This reports, for the layers active in that frame, where the native formulas
for X scroll, camera height and the middle-bank switch differ.
"""
import argparse,collections,json,re
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]

def model(header):
    value=lambda name:int(re.search(name+r'=(\d+)',header).group(1))
    start,top,switch=value('MSX_CAMERA_RISE_START'),value('MSX_CAMERA_RISE_MAX'),value('MSX_MID_FULL_SPEED_CAMERA')
    height=lambda c:0 if c<=start else min(top,(c-start+3)//4)
    scroll=lambda layer,v:(v+1)//2 if layer==0 else (v*3+2)//4 if layer==1 else v
    visible=lambda layer,c:c<switch if layer==1 else c>=switch if layer==2 else True
    return height,scroll,visible

def compare(map_path,header_path):
    height,scroll,visible=model(header_path.read_text())
    observations=json.loads(map_path.read_text())['observations'];kinds=collections.Counter();frames=[]
    for o in observations:
        c=o['camera'];found=set()
        for layer in range(4):
            if not o['active'][layer]:continue
            if not visible(layer,c):found.add('layer window')
            if o['offsets'][layer]!=scroll(layer,c):found.add(f'x layer {layer}')
            if o['vertical_offsets'][layer]!=scroll(layer,height(c)):found.add('height')
        kinds.update(found)
        if found:frames.append(dict(frame=o['frame'],camera=c,captures=Path(o['captures']).name,differences=sorted(found)))
    return dict(frames=len(observations),exact=len(observations)-len(frames),differences=dict(kinds),frames_differing=frames)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--model',type=Path,default=ROOT/'filesystem/mission1/map.json')
    p.add_argument('--camera',type=Path,default=ROOT/'src/mission_camera.h')
    a=p.parse_args();r=compare(a.model,a.camera)
    print(json.dumps({k:v for k,v in r.items() if k!='frames_differing'}))
