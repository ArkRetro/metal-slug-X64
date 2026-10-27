#!/usr/bin/env python3
"""Build lossless CI4 .sprite halves with libdragon mksprite, no dithering."""
import argparse,json,subprocess
from pathlib import Path
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser();p.add_argument('--mksprite',required=True);args=p.parse_args()
m=json.loads((ROOT/'filesystem/vehicle.json').read_text())
source=ROOT/'build/vehicle-png';source.mkdir(parents=True,exist_ok=True)
out=ROOT/'romfs/vehicles';out.mkdir(parents=True,exist_ok=True)
files=[]
for pose,frame in enumerate(m['frames']):
    packed=bytes.fromhex(frame['indices_hex'])
    pens=[(packed[i//2]>>(0 if i&1 else 4))&15 for i in range(128*64)]
    for flash,name in enumerate(('palette_normal','palette_flash')):
        palette=m[name]
        colors=[((v>>11&31)*255//31,(v>>6&31)*255//31,(v>>1&31)*255//31,255 if v&1 else 0) for v in palette]
        for part in range(2):
            im=Image.new('RGBA',(64,64))
            im.putdata([colors[pens[y*128+part*64+x]] for y in range(64) for x in range(64)])
            path=source/f'v{pose}-{flash}-{part}.png';im.save(path);files.append(str(path))
subprocess.run([args.mksprite,'-f','CI4','-D','NONE','-c','0','-o',str(out),*files],check=True)
(ROOT/'build/vehicle-sprites.stamp').write_text('36 CI4 sprites, 64x64, 2048 bytes per upload; no dithering\n')
