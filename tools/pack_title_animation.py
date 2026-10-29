#!/usr/bin/env python3
"""Recover original title motion and sound; require every frame to match MAME."""
import argparse,hashlib,json,struct,subprocess,wave,zipfile,zlib
from pathlib import Path
import numpy as np
from PIL import Image
from render_intro_reference import Renderer,raster_states
from pack_cinematic import delta,nanos
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser();p.add_argument('capture',type=Path);p.add_argument('--bios',type=Path,required=True);p.add_argument('--first',type=int,default=300);p.add_argument('--last',type=int,default=660);a=p.parse_args()
frames={r['frame']:r for l in (a.capture/'frames.jsonl').read_text().splitlines() if (r:=json.loads(l))}
with zipfile.ZipFile(a.bios) as z:zoom=z.read('000-lo.lo')
r=Renderer((ROOT/'filesystem/sprites.bin').read_bytes(),(ROOT/'filesystem/fixed.bin').read_bytes(),zoom)
out=ROOT/'filesystem/cinematics';previous=np.zeros(304*224,dtype=np.uint16);packets=[];crcs=[]
for f in range(a.first,a.last+1):
 meta=frames[f];assert meta['fixed_source']==1
 v=struct.unpack('>34304H',(a.capture/f'vram{f}.bin').read_bytes());pal=struct.unpack('>4096H',(a.capture/f'palette{f}.bin').read_bytes())
 states,_,colors,_=raster_states(a.capture,f,True)
 rgb,_=r.render(v,pal,meta['animation_counter'],states,meta['animation_disabled'],colors)
 ref=np.asarray(Image.open(a.capture/f'game{f}.png').convert('RGB'))
 mismatch=int((np.max(abs(rgb.astype(np.int16)-ref.astype(np.int16)),2)[:,8:312]>1).sum())
 assert not mismatch,('Unexplained title mismatch',f,mismatch)
 rgb=rgb[:,8:312].astype(np.uint16);pixels=(((rgb[:,:,0]>>3)<<11)|((rgb[:,:,1]>>3)<<6)|((rgb[:,:,2]>>3)<<1)|1).reshape(-1)
 encoded=delta(pixels,previous);packets.append(struct.pack('>I',zlib.crc32(encoded))+encoded);previous=pixels
 crcs.append(zlib.crc32(pixels.astype('>u2').tobytes()))
 if (f-a.first)%30==0:print('Matched title frame',f,flush=True)
count=len(packets);offset=32+4*(count+1);offsets=[offset]
for packet in packets:offset+=len(packet);offsets.append(offset)
movie=struct.pack('>4s7I',b'MSXC',1,304,224,count,max(map(len,packets)),59185606,1000000)+struct.pack('>'+str(count+1)+'I',*offsets)+b''.join(packets)
(out/'title-motion.msxc').write_bytes(movie)
with wave.open(str(a.capture/'reference.wav')) as w:
 assert w.getframerate()==48000 and w.getnchannels()==2
 first=nanos(frames[a.first]['time'])*48000//10**9;last=nanos(frames[a.last+1]['time'])*48000//10**9
 w.setpos(first);pcm=w.readframes(last-first)
 with wave.open(str(out/'title-motion.wav'),'wb') as target:target.setnchannels(2);target.setsampwidth(2);target.setframerate(48000);target.writeframes(pcm)
subprocess.run(['/workspace/toolchains/package/opt/libdragon/bin/audioconv64','--wav-resample','32000','--wav-compress','1','--wav-loop','false','-o',str(out),str(out/'title-motion.wav')],check=True)
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
report=dict(status='ALL_FRAMES_MATCH',first_frame=a.first,last_frame=a.last,frames=count,frame_crc32=crcs,movie_bytes=len(movie),movie_sha256=sha(out/'title-motion.msxc'),audio_sha256=sha(out/'title-motion.wav64'),provenance={n:sha(a.capture/n) for n in ('capture.json','frames.jsonl','raster.jsonl','reference.wav')},limits='Recovered original Metal Slug X title reference, not yet an X64 adaptation or complete Mission 1. No generated announcement is used.')
(out/'title-motion.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS: original title motion and sound',count,'frames')
