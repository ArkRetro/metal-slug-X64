#!/usr/bin/env python3
"""Check that the Marco part sprites (romfs/marco/p*.sprite, from
build_marco_poses.py) are the ones of the pose catalog the game is compiled
with (build/marco_pose_meta.h): count, CI4 format, size and pixel
fingerprint, the same tests marco_renderer_load runs on the console.

A catalog and sprites from different conversions make the console reject
every part and fall back to the six-state animation (MiSTer report on test
ROM 18: "Marco lost his animations"), so the build stops here instead.
Usage: check_marco_romfs.py [META] [SPRITE_DIR]"""
import re,struct,sys
from pathlib import Path
meta=Path(sys.argv[1] if len(sys.argv)>1 else 'build/marco_pose_meta.h')
sprites=Path(sys.argv[2] if len(sys.argv)>2 else 'romfs/marco')
text=meta.read_text()
count=int(re.search(r'#define MARCO_PARTS (\d+)u',text).group(1))
body=text.split('marco_parts[]={',1)[1].split('};',1)[0]
parts=[tuple(int(v) for v in m.groups()) for m in re.finditer(r'\{(-?\d+),(-?\d+),(\d+),(\d+),(\d+)\}',body)]
errors=[]
if len(parts)!=count:errors.append(f'{meta}: {len(parts)} parts listed, MARCO_PARTS {count}')
files=sorted(sprites.glob('p*.sprite'))
if len(files)!=count:errors.append(f'{sprites}: {len(files)} sprites, the catalog has {count} parts')
for i,(_,_,w,h,fnv) in enumerate(parts):
    f=sprites/f'p{i}.sprite'
    if not f.exists():errors.append(f'{f}: missing');continue
    d=f.read_bytes();sw,sh,_,flags=struct.unpack('>HHBB',d[:6])
    if flags&0x1f!=8 or (sw,sh)!=(w,h):errors.append(f'{f}: format {flags&0x1f:#x} {sw}x{sh}, catalog CI4 {w}x{h}');continue
    stride=(w+1)//2;fp=2166136261
    for y in range(h):
        row=d[8+y*stride:8+(y+1)*stride]
        for x in range(w):fp=((fp^((row[x//2]>>(0 if x&1 else 4))&15))*16777619)&0xffffffff
    if fp!=fnv:errors.append(f'{f}: pixels differ from the catalog')
for e in errors[:20]:print('ERROR',e)
if errors:sys.exit(f'FAIL: {len(errors)} Marco parts do not match {meta}: rebuild both with tools/build_marco_poses.py (docs/MARCO_CAMPAIGN.md)')
print(f'PASS: {count} Marco parts in {sprites} match {meta}')
