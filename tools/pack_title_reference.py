#!/usr/bin/env python3
"""Reconstruct the original title plate; adapt only FIX coin/credit prompts."""
import argparse,hashlib,json,struct,zipfile,zlib
from pathlib import Path
import numpy as np
from PIL import Image
from render_intro_reference import Renderer
from pack_cinematic import delta
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser();p.add_argument('capture',type=Path);p.add_argument('--bios',type=Path,required=True);a=p.parse_args()
frame=660;meta={r['frame']:r for l in (a.capture/'frames.jsonl').read_text().splitlines() if (r:=json.loads(l))}
v=list(struct.unpack('>34304H',(a.capture/f'vram{frame}.bin').read_bytes()));pal=struct.unpack('>4096H',(a.capture/f'palette{frame}.bin').read_bytes())
with zipfile.ZipFile(a.bios) as z:zoom=z.read('000-lo.lo')
r=Renderer((ROOT/'filesystem/sprites.bin').read_bytes(),(ROOT/'filesystem/fixed.bin').read_bytes(),zoom)
counter=meta[frame]['animation_counter'] if 'animation_disabled' in meta[frame] else meta[frame+1]['animation_counter']
original,_=r.render(v,pal,counter,animation_disabled=meta[frame].get('animation_disabled',False))
reference=np.asarray(Image.open(a.capture/f'game{frame}.png').convert('RGB'))
assert not (np.max(np.abs(original.astype(np.int16)-reference.astype(np.int16)),2)[:,8:312]>1).any()
assert ''.join(chr(v[0x7000+x*32+26]&255) for x in range(15,26))=='INSERT COIN'
for x,ch in zip(range(15,26),'PRESS START'):v[0x7000+x*32+26]=0x300+ord(ch)
for x in range(27,37):v[0x7000+x*32+29]=0x320
rgb,_=r.render(v,pal,counter,animation_disabled=meta[frame].get('animation_disabled',False))
pixels=rgb[:,8:312].astype(np.uint16);pixels=(((pixels[:,:,0]>>3)<<11)|((pixels[:,:,1]>>3)<<6)|((pixels[:,:,2]>>3)<<1)|1).reshape(-1)
encoded=delta(pixels,np.zeros_like(pixels));packet=struct.pack('>I',zlib.crc32(encoded))+encoded
movie=struct.pack('>4s7I',b'MSXC',1,304,224,1,len(packet),59185606,1000000)+struct.pack('>2I',40,40+len(packet))+packet
out=ROOT/'filesystem/cinematics';(out/'title.msxc').write_bytes(movie)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report=dict(source_frame=660,baseline_reference_matches=True,changes=['FIX row 26 columns 15..25: INSERT COIN -> PRESS START','FIX row 29 columns 27..36: remove credit counter'],movie_sha256=sha(out/'title.msxc'),pixels_crc32=f'{zlib.crc32(pixels.astype(">u2").tobytes()):08x}',limits='Single original title plate adapted for N64 input, not the full animated arcade introduction.',source_sha256={n:sha(a.capture/n) for n in ('vram660.bin','palette660.bin','game660.png','frames.jsonl')})
(out/'title.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS: reconstructed title plate and bounded prompt adaptation')
