#!/usr/bin/env python3
"""Build a ROM-derived, noninteractive mission-entry stream and native audio.

Every encoded frame must agree with its independently captured MAME image.
PNG files are read only for comparison, never for asset generation.
"""
import argparse,hashlib,json,struct,subprocess,wave,zlib,zipfile
from pathlib import Path
import numpy as np
from PIL import Image
from render_intro_reference import Renderer,raster_states
ROOT=Path(__file__).resolve().parents[1]
def delta(current,previous):
 changed=np.flatnonzero(current!=previous);out=bytearray();cursor=0
 if len(changed):
  cuts=np.r_[0,np.flatnonzero(np.diff(changed)>3)+1,len(changed)]
  def block(start,end,repeat):
   nonlocal cursor
   skip=start-cursor
   while skip>65535:out.extend(struct.pack('>HH',65535,0));cursor+=65535;skip-=65535
   while start<end:
    count=min(32767,end-start);out.extend(struct.pack('>HH',skip,count|(32768 if repeat else 0)))
    out.extend(struct.pack('>H',int(current[start])) if repeat else current[start:start+count].astype('>u2').tobytes())
    start+=count;cursor=start;skip=0
  for a,b in zip(cuts[:-1],cuts[1:]):
   start=int(changed[a]);end=int(changed[b-1])+1;segment=current[start:end]
   edges=np.r_[0,np.flatnonzero(segment[1:]!=segment[:-1])+1,len(segment)];raw=start
   for low,high in zip(edges[:-1],edges[1:]):
    if high-low>=5:
     if raw<start+low:block(raw,start+int(low),False)
     block(start+int(low),start+int(high),True);raw=start+int(high)
   if raw<end:block(raw,end,False)
 out.extend(b'\0\0\0\0');return bytes(out)
def nanos(s):return int(s.replace('.','').replace(',',''))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--bios',type=Path,required=True);p.add_argument('--first-frame',type=int,default=3830);p.add_argument('--last-frame',type=int,default=4059);p.add_argument('--audio-last-frame',type=int,default=4199);a=p.parse_args()
 assert a.first_frame<a.last_frame<a.audio_last_frame
 out=ROOT/'filesystem/cinematics';out.mkdir(parents=True,exist_ok=True)
 frames={r['frame']:r for l in (a.capture/'frames.jsonl').read_text().splitlines() if (r:=json.loads(l))};players={r['frame']:r for l in (a.capture/'player.jsonl').read_text().splitlines() if (r:=json.loads(l))}
 handoff=players[a.last_frame+1];assert handoff['flags15']&64 and not handoff['vx'] and not handoff['vy']
 with zipfile.ZipFile(a.bios) as z:zoom=z.read('000-lo.lo')
 renderer=Renderer((ROOT/'filesystem/sprites.bin').read_bytes(),(ROOT/'filesystem/fixed.bin').read_bytes(),zoom)
 previous=np.zeros(304*224,dtype=np.uint16);packets=[];checks=[];maximum=0;visual=[]
 for f in range(a.first_frame,a.last_frame+1):
  meta=frames[f];assert meta['fixed_source']==1
  v=struct.unpack('>34304H',(a.capture/f'vram{f}.bin').read_bytes());pal=struct.unpack('>4096H',(a.capture/f'palette{f}.bin').read_bytes())
  states,n,colors,pn=raster_states(a.capture,f,True)
  rgb,_=renderer.render(v,pal,meta['animation_counter'],states,meta['animation_disabled'],colors)
  reference=np.asarray(Image.open(a.capture/f'game{f}.png').convert('RGB'));diff=np.max(abs(rgb.astype(np.int16)-reference.astype(np.int16)),2)[:,8:312]
  assert not (diff>1).any(),('Original reference differs',f,int((diff>1).sum()))
  rgb=rgb[:,8:312].astype(np.uint16);pixels=(((rgb[:,:,0]>>3)<<11)|((rgb[:,:,1]>>3)<<6)|((rgb[:,:,2]>>3)<<1)|1).reshape(-1)
  encoded=delta(pixels,previous);packet=struct.pack('>I',zlib.crc32(encoded))+encoded;packets.append(packet);maximum=max(maximum,len(packet));previous=pixels
  checks.append(zlib.crc32(pixels.astype('>u2').tobytes()));visual.append(dict(frame=f,vram_writes=n,palette_writes=pn))
  if (f-a.first_frame)%30==0:print('Validated/encoded source frame',f,'bytes',len(packet),flush=True)
 count=len(packets);offset=32+4*(count+1);offsets=[offset]
 for packet in packets:offset+=len(packet);offsets.append(offset)
 header=struct.pack('>4s7I',b'MSXC',1,304,224,count,maximum,59185606,1000000)
 movie=header+struct.pack('>'+str(len(offsets))+'I',*offsets)+b''.join(packets);(out/'entry.msxc').write_bytes(movie)
 with wave.open(str(a.capture/'reference.wav')) as source:
  assert source.getframerate()==48000 and source.getnchannels()==2 and source.getsampwidth()==2
  first=nanos(frames[a.first_frame]['time'])*48000//10**9;last=nanos(frames[a.audio_last_frame]['time'])*48000//10**9;assert last<=source.getnframes();source.setpos(first);pcm=source.readframes(last-first)
  with wave.open(str(out/'entry.wav'),'wb') as target:target.setnchannels(2);target.setsampwidth(2);target.setframerate(48000);target.writeframes(pcm)
 cmd=[str(ROOT.parent/'toolchains/package/opt/libdragon/bin/audioconv64'),'--wav-resample','32000','--wav-compress','1','--wav-loop','false','-o',str(out),str(out/'entry.wav')];subprocess.run(cmd,check=True)
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 report=dict(status='PACKED_ALL_FRAMES_MATCH',first_frame=a.first_frame,last_frame=a.last_frame,frames=count,maximum_packet=maximum,movie_bytes=len(movie),movie_sha256=sha(out/'entry.msxc'),audio_sha256=sha(out/'entry.wav64'),audio_first_sample=first,audio_last_sample=last,handoff=handoff,frame_crc32=checks,visual=visual,provenance={n:sha(a.capture/n) for n in ('capture.json','frames.jsonl','player.jsonl','raster.jsonl','reference.wav')},limits='Only the noninteractive mission-entry segment. Title/tutorial/selection and ongoing gameplay music/effects remain separate work.')
 (out/'entry.json').write_text(json.dumps(report,indent=2)+'\n')
 print('Packed',count,'frames,',len(movie),'bytes; maximum packet',maximum)
