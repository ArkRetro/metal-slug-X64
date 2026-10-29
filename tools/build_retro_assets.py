#!/usr/bin/env python3
"""Generate authored depth/normal assets and tiny N64 fog; never infer normals from RGB."""
import argparse,json,math,random
from pathlib import Path
from PIL import Image
from retro_geometry import DepthField
ROOT=Path(__file__).resolve().parents[1]
# Mirrors src/retro25d.h. A frame draws at most 16 ground + 5*8 plane + 4*8 shadow triangles.
GROUND_TRIANGLES,PLANE_TRIANGLES,SHADOW_TRIANGLES=16,8,8
MAX_VISIBLE_PLANES,MAX_SHADOWS,MAX_TRIANGLES=5,4,96
CULL_MARGIN,VIEW_WIDTH,PLANE_MAX_DEPTH=32,320,24

def validate_planes(planes):
    """Sort planes by left edge and check the per-camera budget anywhere in world X.

    Planes may sit at any world X (the level can grow past the observed region):
    the runtime culls them against the camera, so the limit is how many overlap
    one cull window, not how many exist in total."""
    names=set();out=[]
    for p in planes:
        name=p.get('name');assert name and name not in names,f'Plane needs a unique name: {name!r}';names.add(name)
        l,t,r,b=p['rect'];assert 0<=l<r and 0<=t<b<=240,f'{name}: rect must be [left,top,right,bottom] with left<right, top<bottom<=240'
        corners=[p['depth']+dx*p['dx']+dy*p['dy'] for dx in (0,r-l) for dy in (0,b-t)]
        assert all(-6<=z<=PLANE_MAX_DEPTH for z in corners),f'{name}: depth {min(corners):.2f}..{max(corners):.2f} outside -6..{PLANE_MAX_DEPTH}'
        assert abs(p['dx'])<=.25 and abs(p['dy'])<=.25,f'{name}: keep |dx|,|dy|<=0.25 (subtle relief)'
        out.append(p)
    out.sort(key=lambda p:(p['rect'][0],p['rect'][2]))
    # Camera range in which each plane passes retro_plane_in_view().
    spans=[(p['rect'][0]-VIEW_WIDTH-CULL_MARGIN,p['rect'][2]+CULL_MARGIN) for p in out]
    worst=0;where=None
    for start,_ in spans:
        n=sum(1 for a,b in spans if a<=start<=b)
        if n>worst:worst,where=n,start
    assert worst<=MAX_VISIBLE_PLANES,f'{worst} planes overlap one camera window near camera_x={where}; limit {MAX_VISIBLE_PLANES}'
    assert GROUND_TRIANGLES+MAX_VISIBLE_PLANES*PLANE_TRIANGLES+MAX_SHADOWS*SHADOW_TRIANGLES<=MAX_TRIANGLES
    return out,worst

def build(output):
    model=json.loads((ROOT/'data/retro_surfaces.json').read_text());output.mkdir(parents=True,exist_ok=True)
    geometry=DepthField(model,json.loads((ROOT/'filesystem/mission1/collision.json').read_text()))
    planes,worst=validate_planes(model['planes'])
    assert GROUND_TRIANGLES+worst*PLANE_TRIANGLES+MAX_SHADOWS*SHADOW_TRIANGLES<=MAX_TRIANGLES,'Geometry exceeds N64 triangle budget'
    rng=random.Random(250);small=Image.new('L',(8,8));small.putdata([rng.randrange(30,230) for _ in range(64)])
    noise=small.resize((32,32),Image.Resampling.BILINEAR);fog=[]
    for y in range(32):
        for x in range(32):
            # Smooth vertical edges. IA8 has 4 intensity + 4 alpha bits.
            envelope=math.sin(math.pi*y/31)**2*math.sin(math.pi*x/31)**2
            alpha=round(noise.getpixel((x,y))/255*envelope*15)
            fog.append(0xf0|alpha)
    lines=['/* Generated authored geometry and 1024-byte IA8 fog. */',
           'static const float retro_layer_depths[5]={'+','.join(str(v)+'f' if '.' in str(v) else str(v)+'.0f' for v in model['layer_depths'])+'};',
           'static const retro_plane_t retro_planes[]={']
    for p in planes:lines.append('  {'+','.join(str(float(v))+'f' for v in (*p['rect'],p['depth'],p['dx'],p['dy']))+'}, /* '+p['name']+' */')
    if not planes:lines.append('  {0},')
    lines.extend(['};',f'#define RETRO_PLANE_COUNT {len(planes)}u',f'#define RETRO_MAX_PLANES_IN_VIEW {worst}u',f'#define RETRO_GROUND_WIDTH {geometry.width}',
        'static const uint16_t retro_ground[]={'+','.join(str(round(geometry.ground(min(x,geometry.width-1))*256)) for x in range(0,geometry.width+1,8))+'};',
        'static uint8_t retro_fog[1024] __attribute__((aligned(16)))={'])
    for n in range(0,1024,32):lines.append('  '+','.join(hex(v) for v in fog[n:n+32])+',')
    lines.append('};');(output/'retro_assets.h').write_text('\n'.join(lines)+'\n')
    # Technical assets in canonical foreground/world image coordinates.
    width=geometry.width;height=240;depth=Image.new('L',(width,height));normals=Image.new('RGB',(width,height),(128,128,255))
    for y in range(height):
        for x in range(width):
            z,n=geometry.sample(x,y,3)
            depth.putpixel((x,y),max(0,min(255,round((z+8)/48*255))))
            normals.putpixel((x,y),tuple(round((v*.5+.5)*255) for v in n))
    depth.save(output/'depth.png');normals.save(output/'normals.png');noise.save(output/'fog-noise.png')
    (output/'assets.json').write_text(json.dumps(dict(depth_encoding='depth=-8 + L/255*48; world units',normal_encoding='view tangent RGB = (N*0.5+0.5)*255; +Z toward camera',fog_bytes=len(fog),planes=len(planes),max_planes_in_view=worst,worst_case_triangles=GROUND_TRIANGLES+worst*PLANE_TRIANGLES+MAX_SHADOWS*SHADOW_TRIANGLES),indent=2)+'\n')
    print('Retro assets:',len(planes),'planes,',worst,'max in view,',len(fog),'bytes IA8 fog')

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,default=ROOT/'build/retro25d');build(p.parse_args().output)
