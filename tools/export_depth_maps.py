#!/usr/bin/env python3
"""Export technical depth/normal maps from original silhouettes + authored depth.

No RGB-to-depth heuristic and no modern framebuffer postprocess are used.
The N64 evaluates the same analytic fields at mesh vertices, not per pixel.
"""
import argparse,json
from pathlib import Path
from PIL import Image
from mission_tiles import decode
from retro_geometry import DepthField
ROOT=Path(__file__).resolve().parents[1]

def export(camera,output):
    author=json.loads((ROOT/'data/retro_surfaces.json').read_text())
    model=json.loads((ROOT/'filesystem/mission1/map.json').read_text())
    props=json.loads((ROOT/'filesystem/mission1/props.json').read_text())
    grid=json.loads((ROOT/'filesystem/mission1/collision.json').read_text())
    geometry=DepthField(author,grid)
    crom=(ROOT/'filesystem/sprites.bin').read_bytes()
    depth=Image.new('I',(320,240),65535);normal=Image.new('RGB',(320,240),(128,128,255))
    entries=model['cells']+props['cells'];entries.sort(key=lambda c:c['layer']) # Stable for overlapping prop SCBs.
    for c in entries:
        layer=c['layer'];z=author['layer_depths'][layer];ratio=320/(320+z)
        left=round(c['x']-camera*ratio);top=8+c['y']
        if left+16<=8 or left>=312:continue
        pens=decode(crom,c['code'],c['flip'])
        for y in range(16):
            for x in range(16):
                if not pens[y*16+x]:continue
                px,py=left+x,top+y
                if not 8<=px<312 or not 8<=py<232:continue
                value,n=geometry.sample(c['x']+x,py,layer)
                depth.putpixel((px,py),round(max(0,min(1,(value+8)/520))*65535))
                normal.putpixel((px,py),tuple(round((v*.5+.5)*255) for v in n))
    output.mkdir(parents=True,exist_ok=True)
    depth.convert('I;16').save(output/f'depth-c{camera}.png');normal.save(output/f'normal-c{camera}.png')
    metadata=dict(camera=camera,size=[320,240],depth='unsigned 16-bit PNG: z=-8+sample/65535*520; positive away from camera',normal='RGB8 tangent/view normal: (rgb/255)*2-1; +Z toward camera',runtime='analytic depth derivatives evaluated per mesh vertex; PNGs are authoring/debug assets, not runtime normal-texture fetches',scope='observed mission-1 region; authored 2.5D art direction, not recovered original depth')
    (output/f'maps-c{camera}.json').write_text(json.dumps(metadata,indent=2)+'\n')
    print('Depth/normal maps exported for camera',camera)

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--camera',type=int,default=460)
    p.add_argument('--output',type=Path,default=ROOT/'build/retro25d/maps');a=p.parse_args();export(a.camera,a.output)
