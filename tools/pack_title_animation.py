#!/usr/bin/env python3
"""Recover original title motion and sound; require every frame to match MAME."""
import argparse,hashlib,json,struct,zipfile,zlib
from pathlib import Path
import numpy as np
from PIL import Image
from render_intro_reference import Renderer,raster_states
from pack_cinematic import delta
from title_x64 import load_digits,compose,DIGIT_KEYS,DIGIT_X
from title_announcement import prepare as prepare_audio
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser();p.add_argument('capture',type=Path);p.add_argument('--bios',type=Path,required=True);p.add_argument('--first',type=int,default=300);p.add_argument('--last',type=int,default=660);p.add_argument('--adapt-x64',type=Path,help='Authored transparent paired digit sprite');a=p.parse_args()
frames={r['frame']:r for l in (a.capture/'frames.jsonl').read_text().splitlines() if (r:=json.loads(l))}
with zipfile.ZipFile(a.bios) as z:zoom=z.read('000-lo.lo')
r=Renderer((ROOT/'filesystem/sprites.bin').read_bytes(),(ROOT/'filesystem/fixed.bin').read_bytes(),zoom)
digits=load_digits(a.adapt_x64) if a.adapt_x64 else None
preview=ROOT/'build/title-x64-frames';preview.mkdir(parents=True,exist_ok=True)
out=ROOT/'filesystem/cinematics';previous=np.zeros(304*224,dtype=np.uint16);packets=[];crcs=[]
for f in range(a.first,a.last+1):
 meta=frames[f];assert meta['fixed_source']==1
 v=struct.unpack('>34304H',(a.capture/f'vram{f}.bin').read_bytes());pal=struct.unpack('>4096H',(a.capture/f'palette{f}.bin').read_bytes())
 states,_,colors,_=raster_states(a.capture,f,True)
 rgb,_=r.render(v,pal,meta['animation_counter'],states,meta['animation_disabled'],colors)
 ref=np.asarray(Image.open(a.capture/f'game{f}.png').convert('RGB'))
 mismatch=int((np.max(abs(rgb.astype(np.int16)-ref.astype(np.int16)),2)[:,8:312]>1).sum())
 assert not mismatch,('Unexplained title mismatch',f,mismatch)
 if digits is not None:
  baseline=rgb
  rgb=compose(rgb,f,digits,r,colors)
  allowed=np.zeros(rgb.shape[:2],dtype=bool);allowed[192:200,112:208]=True;allowed[216:224,208:296]=True
  if f>=610:allowed[:160,211:296]=True
  assert not np.any((rgb!=baseline)[~allowed]),('Edit escaped authored regions',f)
 if digits is not None:Image.fromarray(rgb[:,8:312]).save(preview/f'{f:04}.png')
 rgb=rgb[:,8:312].astype(np.uint16);pixels=(((rgb[:,:,0]>>3)<<11)|((rgb[:,:,1]>>3)<<6)|((rgb[:,:,2]>>3)<<1)|1).reshape(-1)
 encoded=delta(pixels,previous);packets.append(struct.pack('>I',zlib.crc32(encoded))+encoded);previous=pixels
 crcs.append(zlib.crc32(pixels.astype('>u2').tobytes()))
 if (f-a.first)%30==0:print('Matched title frame',f,flush=True)
count=len(packets);offset=32+4*(count+1);offsets=[offset]
for packet in packets:offset+=len(packet);offsets.append(offset)
movie=struct.pack('>4s7I',b'MSXC',1,304,224,count,max(map(len,packets)),59185606,1000000)+struct.pack('>'+str(count+1)+'I',*offsets)+b''.join(packets)
(out/'title-motion.msxc').write_bytes(movie)
announcement=prepare_audio(a.capture,out,frames,a.first,a.last,digits is not None)
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
report=dict(status='SOURCE_FRAMES_MATCH_THEN_AUTHORED_X64' if digits else 'ALL_FRAMES_MATCH',first_frame=a.first,last_frame=a.last,frames=count,source_pixels_checked=count*304*224,unmodified_source_matches=True,frame_crc32=crcs,movie_bytes=len(movie),movie_sha256=sha(out/'title-motion.msxc'),audio_sha256=sha(out/'title-motion.wav64'),audio_format='Stereo PCM16 at 32000 Hz, uncompressed WAV64',provenance={n:sha(a.capture/n) for n in ('capture.json','frames.jsonl','raster.jsonl','reference.wav')},limits='Recovered original Metal Slug X title reference, not yet an X64 adaptation or complete Mission 1. No generated announcement is used.')
if digits is not None:report.update(title='METAL SLUG X64',announcement=announcement,title_hold_frames=announcement.get('title_hold_frames',0),authored_digits_sha256=sha(a.adapt_x64),authored_digit_keys=DIGIT_KEYS,authored_digit_x=DIGIT_X,limits='Animated X64 title adaptation; original letters, particles, flash and synchronized audio, authored extra digits and console prompts. Not complete Mission 1; original announcement followed by synthetic sixty-four; not an original actor voice clone.')
(out/'title-motion.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS:', 'X64 title adaptation' if digits else 'original title motion and sound',count,'frames')
