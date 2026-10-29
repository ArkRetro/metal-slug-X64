#!/usr/bin/env python3
"""Convert authored title artwork to the native 320x240 RGBA5551 title plate."""
import argparse,hashlib,json,struct,zlib
from pathlib import Path
import numpy as np
from PIL import Image
from pack_cinematic import delta
p=argparse.ArgumentParser();p.add_argument('art',type=Path);a=p.parse_args()
root=Path(__file__).resolve().parents[1];out=root/'filesystem/cinematics';out.mkdir(parents=True,exist_ok=True)
rgb=np.asarray(Image.open(a.art).convert('RGB').resize((320,240),Image.Resampling.NEAREST)).astype(np.uint16)
pixels=(((rgb[:,:,0]>>3)<<11)|((rgb[:,:,1]>>3)<<6)|((rgb[:,:,2]>>3)<<1)|1).reshape(-1)
encoded=delta(pixels,np.zeros_like(pixels));packet=struct.pack('>I',zlib.crc32(encoded))+encoded
movie=struct.pack('>4s7I',b'MSXC',1,320,240,1,len(packet),59185606,1000000)+struct.pack('>2I',40,40+len(packet))+packet
(out/'title.msxc').write_bytes(movie)
report=dict(title='METAL SLUG X64',origin='Authored with image generation from supplied arcade title reference; N64-era styled lettering, not an official Nintendo font',art_sha256=hashlib.sha256(a.art.read_bytes()).hexdigest(),movie_sha256=hashlib.sha256(movie).hexdigest(),pixels_crc32=f'{zlib.crc32(pixels.astype(">u2").tobytes()):08x}',conversion='320x240 nearest-neighbor then RGBA5551, no runtime filtering',limits='Adapted single title plate, not the full arcade title animation or complete Mission 1.')
(out/'title.json').write_text(json.dumps(report,indent=2)+'\n');print('Packed METAL SLUG X64 title, CRC',report['pixels_crc32'])
