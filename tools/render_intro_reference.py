#!/usr/bin/env python3
"""Reconstruct captured intro art from C/S-ROM data, including hardware zoom.

Offline verification only, never a CPU emulator or screenshot-texture exporter.
Hardware behavior reference: MAME 0.276 neogeo_spr.cpp (BSD-3-Clause),
Bryan McPhail, Ernesto Corvi, Andrew Prime, Zsolt Vasvari; thanks to Fuzz.
"""
import argparse,hashlib,json,struct,zipfile
from functools import lru_cache
from pathlib import Path
import numpy as np
from PIL import Image
from export_marco import palette_rgb
from mission_tiles import decode
ROOT=Path(__file__).resolve().parents[1]
ZOOM_X=(0x0080,0x0880,0x0888,0x2888,0x288a,0x2a8a,0x2aaa,0xaaaa,0xaaea,0xbaea,0xbaeb,0xbbeb,0xbbef,0xfbef,0xfbff,0xffff)
class Renderer:
 def __init__(self,crom,fixed,zoom):
  self.crom=crom;self.fixed=fixed;self.zoom=zoom
  self.xsamples=[[i for i in range(16) if bits&(0x8000>>i)] for bits in ZOOM_X]
 @lru_cache(16384)
 def tile(self,code):return np.array(decode(self.crom,code,0),dtype=np.uint16).reshape(16,16)
 @lru_cache(4096)
 def character(self,code):
  return np.array([[self.fixed[code*32+(16,24,0,8)[x//2]+y]>>((x&1)*4)&15 for x in range(8)] for y in range(8)],dtype=np.uint16)
 def render(self,v,p,counter,line_states=None):
  pixels=np.full((224,320),4095,dtype=np.uint16);columns=[];x=y=rows=zx=zy=0;drawn=zoomed=0
  def columns_for(words):
   columns=[];x=y=rows=zx=zy=0
   for slot in range(381):
    control,z=words[0x8200+slot],words[0x8000+slot]
    if control&64:x=(x+zx+1)&511;zx=z>>8&15
    else:x=words[0x8400+slot]>>7;y=512-(control>>7);rows=control&63;zx=z>>8&15;zy=z&255
    columns.append((slot,x,y,rows,zx,zy))
   return columns
  columns=columns_for(v)
  for sy in range(16,240):
   if line_states is not None:v=line_states[sy][1];columns=columns_for(v)
   active=[c for c in columns if c[3] and (c[3]>=32 or ((sy-c[2])&511)<c[3]*16)][:96]
   if line_states is not None:
    before=columns_for(line_states[sy][0]);ids={c[0] for c in [c for c in before if c[3] and (c[3]>=32 or ((sy-c[2])&511)<c[3]*16)][:96]}
    active=[c for c in columns if c[0] in ids and (not c[3] or c[3]>=32 or ((sy-c[2])&511)<c[3]*16)]
   for slot,x,y,rows,zx,zy in active:
    if 320<=x<=496:continue
    line=(sy-y)&511;zline=line&255;invert=bool(line&256)
    if invert:zline^=255
    if rows>32:
     zline%=((zy+1)*2)
     if zline>zy:zline=(zy+1)*2-1-zline;invert=not invert
    lookup=self.zoom[zy*256+zline];ty=lookup&15;row=lookup>>4
    if invert:ty^=15;row^=31
    low,attr=v[slot*64+row*2:slot*64+row*2+2];code=low|((attr<<12)&0xf0000)
    mask=7 if attr&8 else 3 if attr&4 else 0;code=(code&~mask)|(counter&mask)
    if attr&2:ty^=15
    xs=self.xsamples[zx];xs=[15-i for i in xs] if attr&1 else xs
    pens=self.tile(code)[ty,xs];indices=(np.arange(len(xs))+x)&511;valid=(indices<320)&(pens!=0)
    pixels[sy-16,indices[valid]]=pens[valid]+(attr>>8)*16;drawn+=1;zoomed+=zx!=15 or zy!=255
  for sy in range(16,240):
   words=line_states[sy][1] if line_states is not None else v
   for x in range(40):
    word=words[0x7000+x*32+sy//8];pens=self.character(word&4095)[sy&7];dst=pixels[sy-16,x*8:x*8+8];opaque=pens!=0;dst[opaque]=pens[opaque]+(word>>12)*16
  colors=np.array([palette_rgb(w) for w in p],dtype=np.uint8)
  return colors[pixels],dict(sprite_lines=drawn,zoomed_lines=zoomed)
def raster_states(folder,frame):
 """Rebuild only measured VDP writes; never alter screenshots or mask pixels."""
 memory=list(struct.unpack('>34304H',(folder/f'vram{frame-1}.bin').read_bytes()))
 records=[json.loads(l) for l in (folder/'raster.jsonl').read_text().splitlines()]
 writes=[r for r in records if r.get('frame')==frame-1 and r.get('address')==0x3c0002]
 assert writes
 groups={};visible=False;applied=0
 def apply(r):
  nonlocal applied
  address=r['vram_offset'];assert memory[address]==r['word'],('Unmatched pre-write VRAM',r)
  memory[address]=(memory[address]&(~r['mask']&65535))|(r['data']&r['mask']);applied+=1
 for r in writes:
  if r['line']>=240:assert not visible;apply(r)
  else:visible=True;groups.setdefault(r['line'],[]).append(r)
 states={}
 for line in range(240):
  before=tuple(memory)
  for r in groups.get(line,[]):apply(r)
  if line>=16:states[line]=(before,tuple(memory))
 assert tuple(memory)==struct.unpack('>34304H',(folder/f'vram{frame}.bin').read_bytes())
 assert (folder/f'palette{frame-1}.bin').read_bytes()==(folder/f'palette{frame}.bin').read_bytes(),'Palette raster changes need separate recovery'
 return states,applied
if __name__=='__main__':
 parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('capture',type=Path);parser.add_argument('--bios',type=Path,required=True);parser.add_argument('--output',type=Path,required=True);parser.add_argument('--raster-reference',type=Path);parser.add_argument('--frames',type=int,nargs='+',default=[360,540,660,900,1800,3000,3600,3780,3960,4080,4200]);a=parser.parse_args()
 a.output.mkdir(parents=True,exist_ok=True);meta={r['frame']:r for l in (a.capture/'frames.jsonl').read_text().splitlines() if (r:=json.loads(l))}
 with zipfile.ZipFile(a.bios) as z:zoom=z.read('000-lo.lo')
 renderer=Renderer((ROOT/'filesystem/sprites.bin').read_bytes(),(ROOT/'filesystem/fixed.bin').read_bytes(),zoom)
 reports=[]
 for frame in a.frames:
  v=struct.unpack('>34304H',(a.capture/f'vram{frame}.bin').read_bytes());p=struct.unpack('>4096H',(a.capture/f'palette{frame}.bin').read_bytes())
  # The screenshot includes the render interval following this frame-done
  # observation; use its recorded next-boundary counter, never frame & 7.
  states=None;raster_writes=0
  if a.raster_reference and (a.raster_reference/f'vram{frame-1}.bin').is_file():
   assert (a.capture/f'game{frame}.png').read_bytes()==(a.raster_reference/f'game{frame}.png').read_bytes(),'Raster reference is a different scene'
   states,raster_writes=raster_states(a.raster_reference,frame)
  rgb,stats=renderer.render(v,p,meta[frame+1]['animation_counter'],states);stats['raster_writes']=raster_writes;ref=np.asarray(Image.open(a.capture/f'game{frame}.png').convert('RGB'))
  assert ref.shape==rgb.shape,(ref.shape,rgb.shape)
  delta=np.max(np.abs(rgb.astype(np.int16)-ref.astype(np.int16)),axis=2);bad=int((delta[:,8:312]>1).sum())
  Image.fromarray(rgb).save(a.output/f'reconstructed{frame}.png');reports.append(dict(frame=frame,mismatched_pixels=bad,**stats));print(reports[-1],flush=True)
 sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
 (a.output/'validation.json').write_text(json.dumps(dict(status='FAIL' if any(r['mismatched_pixels'] for r in reports) else 'PASS',frames=reports,capture_frames_sha256=sha(a.capture/'frames.jsonl'),raster_reference_sha256=sha(a.raster_reference/'raster.jsonl') if a.raster_reference else None,screenshot_sha256={str(f):sha(a.capture/f'game{f}.png') for f in a.frames},phase_policy='Use the recorded counter at the next frame-done boundary; verified against these screenshots, not all raster intervals.',limits='Offline reference reconstruction; no native intro integration. Auto-animation enabled is assumed for this bounded profile. Nonzero differences require explanation, not masking.'),indent=2)+'\n')
 assert not any(r['mismatched_pixels'] for r in reports),'Intro reference reconstruction still differs; inspect validation.json'
