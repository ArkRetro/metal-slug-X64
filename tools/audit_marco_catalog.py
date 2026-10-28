#!/usr/bin/env python3
"""Verify captured Marco draw descriptors against SCBs and independent MAME pixels.

Writes numerical coverage to the requested report. Original image/tile data
and full observations stay private. An input case is NOT an animation identity.
"""
import argparse,collections,hashlib,json,re,struct
from functools import lru_cache
from pathlib import Path
import numpy as np
from PIL import Image
from export_marco import reconstruct,top_sprite_pixel,palette_rgb
from export_vehicle import program
from mission_tiles import decode
ROOT=Path(__file__).resolve().parents[1]

def audit(capture,rom_path,output):
 plan=json.loads((capture/'plan.json').read_text());states={r['frame']:r for line in (capture/'states.jsonl').read_text().splitlines() if (r:=json.loads(line))}
 text=(capture/'debug.log').read_text();assert 'Error in command' not in text
 rom=program(rom_path);crom=(ROOT/'filesystem/sprites.bin').read_bytes()
 @lru_cache(None)
 def pose(address):
  at=address-0x100000;assert 0<=at<len(rom)-2
  count,hi=rom[at:at+2];at+=2;assert 0<count<=32
  tiles=[]
  for _ in range(count):
   dx,dy,w,h=struct.unpack_from('>hhBB',rom,at);at+=6;assert w and h
   for col in range(w):
    for row in range(h):
     code=struct.unpack_from('>H',rom,at)[0]|(hi<<12);at+=2
     tiles.append((dx+col*16,-dy+row*16,np.array(decode(crom,code,0),dtype=np.uint8).reshape(16,16)))
  return tiles
 draws=collections.defaultdict(list)
 for f,d,xy,attr,z in re.findall(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+) xy=([0-9A-F]+) attr=([0-9A-F]+) z=([0-9A-F]+)',text):draws[int(f)].append(dict(descriptor=int(d,16),xy=int(xy,16),attr=int(attr,16)))
 @lru_cache(128)
 def composed(frame):
  out=np.zeros((256,320),dtype=np.uint16)
  for d in draws[frame]:
   ox=d['xy']>>16;oy=(512-(d['xy']&65535))&511
   mirror=d['attr']&1;flip=d['attr']&2
   for x,y,tile in pose(d['descriptor']):
    for py in range(16):
     for px in range(16):
      pen=tile[py,px]
      # Original flipped descriptor origin has a further -1 displacement,
      # measured against SCB columns, not a visual centering heuristic.
      xx=(ox+(-x-px-2 if mirror else x+px))&511
      yy=(oy+(-y-py-2 if flip else y+py))&511
      if pen and xx<320 and yy<256:out[yy,xx]=(d['attr']>>8)*16+int(pen)
  return out
 updates={int(v) for v in re.findall(r'MARCO_COMMAND frame=(\d+)',text)}|set(draws)
 last=None;known_palettes={0x63};rows=[];pixels=occluded=0;failures=[];normal_tags=set();visible_frames=0;other_palette_frames=0;profiles={};profile_uses=collections.Counter()
 fixed=(ROOT/'filesystem/fixed.bin').read_bytes()
 for frame in range(plan['first'],plan['last']+1):
  # A capture at the frame boundary sees commands from the previous buffer.
  # This two-video-tick buffer delay is checked against SCBs on every frame.
  if frame-3 in updates:last=frame-3
  if last is None:continue
  v=struct.unpack('>34304H',(capture/f'vram{frame}.bin').read_bytes());pal=struct.unpack('>4096H',(capture/f'palette{frame}.bin').read_bytes())
  known_palettes.update(d['attr']>>8 for d in draws[last])
  active_palettes=known_palettes
  image,_=reconstruct(v,pal,crom,selected_palette=active_palettes);indices=np.frombuffer(image.info['neo_palette_indices'],dtype=np.uint16).reshape(256,320)
  # Each original submission lasts two video ticks. An absent submission
  # must clear the previous pose (not freeze it through respawn blinking).
  # The SCB comparison below independently rejects any incorrect expiry.
  source_frame=last if frame-last<=4 else frame-3
  actual=composed(source_frame);difference=np.count_nonzero(actual[16:240,8:312]!=indices[16:240,8:312])
  if difference:
   failures.append(dict(frame=frame,probe_frame=source_frame,pixels=int(difference)));continue
  reference=np.asarray(Image.open(capture/f'game{frame}.png').convert('RGB'))
  colors={bank*16+pen:np.array(palette_rgb(pal[bank*16+pen]),dtype=np.int16) for bank in active_palettes for pen in range(16)}
  for bank in active_palettes:
   values=pal[bank*16:bank*16+16];key=hashlib.sha256(struct.pack('>16H',*values)).hexdigest()
   profiles[key]=list(values);profile_uses[key]+=1
  ys,xs=np.nonzero(indices[16:240,8:312]);xs+=8;ys+=16
  for y,x in zip(ys,xs):
   rgb=colors[indices[y,x]];ref=reference[y-16,x]
   if np.max(np.abs(rgb-ref.astype(np.int16)))<=1:pixels+=1;continue
   # A later sprite or FIX glyph must independently explain every occlusion.
   top=top_sprite_pixel(v,pal,crom,int(x),int(y))
   word=v[0x7000+(int(x)//8)*32+int(y)//8];code=word&4095
   pen=fixed[code*32+(16,24,0,8)[int(x)%8//2]+int(y)%8]>>((int(x)&1)*4)&15
   cover=palette_rgb(pal[(word>>12)*16+pen]) if pen else top[1] if top and top[0] not in active_palettes else None
   if cover is None or max(abs(int(a)-int(b)) for a,b in zip(cover,ref))>1:
    raise AssertionError(('Unexplained original screenshot mismatch',frame,int(x),int(y),rgb,ref))
   occluded+=1
  if len(xs):
   visible_frames+=1;normal_tags.update(d['descriptor'] for d in draws[source_frame])
  if any(d["attr"]>>8!=0x63 for d in draws[last]):other_palette_frames+=1
  rows.append(frame)
  if frame%100==0:print('Verified Marco frame',frame,flush=True)
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 cases=[]
 for case in plan['cases']:
  frames=[f for f in rows if case['first']<=f<=case['last']]
  cases.append(dict(name=case['name'],input_conditions=case['inputs'],first=case['first'],last=case['last'],verified_frames=len(frames),routines=sorted({f"0x{states[f]['routine']:06X}" for f in frames})))
 report=dict(status='PASS' if not failures else 'FAILED',source='MAME 0.276 euro, controller input only',verified_frames=len(rows),visible_actor_frames=visible_frames,remapped_palette_frames=other_palette_frames,palette_profiles=[dict(sha256=key,words=values,frame_uses=profile_uses[key]) for key,values in profiles.items()],descriptor_addresses=sorted(f'0x{tag:06X}' for tag in normal_tags),other_palette_only_descriptors=sorted({f"0x{d['descriptor']:06X}" for ds in draws.values() for d in ds if d['descriptor'] not in normal_tags}),matched_pixels=pixels,independently_explained_occlusions=occluded,frame_failures=failures,cases=cases,provenance={n:sha(capture/n) for n in ('capture.json','plan.json','debug.log','states.jsonl','program-fixed.bin','program-bank.bin')},limits=[str(len(plan['cases']))+' input conditions, not a complete game-wide Marco animation inventory.','Descriptor data are physical cartridge offsets, never executable native pointers.','Active 304x224 pixels; clipped margins do not establish complete offscreen art.','Input cases do not prove all weapon variants, damage/death paths or transformations. Palette identity is checked per observed draw.'])
 output.parent.mkdir(parents=True,exist_ok=True);output.write_text(json.dumps(report,indent=2)+'\n');assert not failures,failures[:8]
 print('PASS',len(rows),'frames,',len(report['descriptor_addresses']),'descriptors,',pixels,'pixels')

if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--rom',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();audit(a.capture,a.rom,a.output)
