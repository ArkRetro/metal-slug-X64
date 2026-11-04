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
 @lru_cache(128)
 def colors(self,palette):return np.array([palette_rgb(w) for w in palette],dtype=np.uint8)
 def render(self,v,p,counter,line_states=None,animation_disabled=False,palette_states=None):
  pixels=np.full((224,320),4095,dtype=np.uint16);columns=[];x=y=rows=zx=zy=0;drawn=zoomed=0
  # Raster states share immutable VRAM tuples across unchanged scanlines.
  # Cache by identity while retaining the tuple, so Python cannot reuse an id.
  # Parsing 381 descriptors again on every line changes no hardware behavior.
  column_states={}
  def columns_for(words):
   saved=column_states.get(id(words))
   if saved is not None:return saved[1]
   columns=[];x=y=rows=zx=zy=0
   for slot in range(381):
    control,z=words[0x8200+slot],words[0x8000+slot]
    if control&64:x=(x+zx+1)&511;zx=z>>8&15
    else:x=words[0x8400+slot]>>7;y=512-(control>>7);rows=control&63;zx=z>>8&15;zy=z&255
    columns.append((slot,x,y,rows,zx,zy))
   column_states[id(words)]=(words,columns)
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
    mask=0 if animation_disabled else 7 if attr&8 else 3 if attr&4 else 0;code=(code&~mask)|(counter&mask)
    if attr&2:ty^=15
    xs=self.xsamples[zx];xs=[15-i for i in xs] if attr&1 else xs
    pens=self.tile(code)[ty,xs];indices=(np.arange(len(xs))+x)&511;valid=(indices<320)&(pens!=0)
    pixels[sy-16,indices[valid]]=pens[valid]+(attr>>8)*16;drawn+=1;zoomed+=zx!=15 or zy!=255
  for sy in range(16,240):
   words=line_states[sy][1] if line_states is not None else v
   for x in range(40):
    word=words[0x7000+x*32+sy//8];pens=self.character(word&4095)[sy&7];dst=pixels[sy-16,x*8:x*8+8];opaque=pens!=0;dst[opaque]=pens[opaque]+(word>>12)*16
  if palette_states is not None:
   rgb=np.empty((224,320,3),dtype=np.uint8)
   for sy in range(16,240):rgb[sy-16]=self.colors(palette_states[sy])[pixels[sy-16]]
  else:rgb=self.colors(tuple(p))[pixels]
  return rgb,dict(sprite_lines=drawn,zoomed_lines=zoomed)
@lru_cache(2)
def raster_records(folder,kind='vram'):
 groups={}
 for line in (folder/'raster.jsonl').read_text().splitlines():
  r=json.loads(line)
  if (kind=='vram' and r.get('address')==0x3c0002) or (kind=='palette' and 'palette_index' in r):groups.setdefault(r['frame'],[]).append(r)
 return groups
@lru_cache(2)
def bank_palette_records(folder):
 groups={}
 for line in (folder/'raster.jsonl').read_text().splitlines():
  r=json.loads(line)
  if 'palette_index' in r or r.get('palette_bank_write'):groups.setdefault(r['frame'],[]).append(r)
 return groups

@lru_cache(2)
def frame_metadata(folder):
 return {r['frame']:r for line in (folder/'frames.jsonl').read_text().splitlines() if (r:=json.loads(line))}

def bank_palette_states(folder,frame):
 """Replay both physical banks, checking every pre-write and final snapshot."""
 meta=frame_metadata(folder)
 assert not meta[frame-1]['screen_shadow'] and not meta[frame]['screen_shadow'],'Screen-shadow transitions need separate reconstruction'
 read=lambda f:struct.unpack('>8192H',(folder/f'palette-banks{f}.bin').read_bytes())
 memory=list(read(frame-1));active=meta[frame-1]['palette_bank'];groups={};applied=0;visible=False
 def apply(r):
  nonlocal active,applied
  if r.get('palette_bank_write'):
   assert r['bank_before']==active,('Bank pre-state differs',frame,r)
   active=0x1000 if r['address']&0x10 else 0
  else:
   assert r['palette_bank']==active,('Write bank differs',frame,r)
   address=active+r['palette_index']
   assert memory[address]==r['word'],('Banked pre-write differs',frame,r)
   memory[address]=(memory[address]&(~r['mask']&65535))|(r['data']&r['mask'])
  applied+=1
 for r in bank_palette_records(folder).get(frame-1,[]):
  if r['line']>=240:assert not visible;apply(r)
  else:visible=True;groups.setdefault(r['line'],[]).append(r)
 current=tuple(memory[active:active+4096]);states={}
 for line in range(240):
  changes=groups.get(line,[])
  for r in changes:apply(r)
  if changes:current=tuple(memory[active:active+4096])
  if line>=16:states[line]=current
 assert tuple(memory)==read(frame),'Missing physical-bank palette writes'
 assert active==meta[frame]['palette_bank'],'Missing bank selection'
 return states,applied

def raster_states(folder,frame,palette_raster=False):
 """Replay measured writes with the original per-line sprite-list latch."""
 def reconstruct(initial,final,writes,index_key,pairs):
  memory=list(initial);groups={};visible=False;applied=0
  def apply(r):
   nonlocal applied
   address=r[index_key];assert memory[address]==r['word'],('Unmatched pre-write memory',r)
   memory[address]=(memory[address]&(~r['mask']&65535))|(r['data']&r['mask']);applied+=1
  for r in writes:
   if r['line']>=240:assert not visible;apply(r)
   else:visible=True;groups.setdefault(r['line'],[]).append(r)
  states={};current=tuple(memory)
  for line in range(240):
   before=current;changes=groups.get(line,[])
   for r in changes:apply(r)
   if changes:current=tuple(memory)
   if line>=16:states[line]=(before,current) if pairs else current
  assert tuple(memory)==final,'Missing writes between frame boundaries'
  return states,applied
 vram=lambda f:struct.unpack('>34304H',(folder/f'vram{f}.bin').read_bytes())
 pal=lambda f:struct.unpack('>4096H',(folder/f'palette{f}.bin').read_bytes())
 states,n=reconstruct(vram(frame-1),vram(frame),raster_records(folder).get(frame-1,[]),'vram_offset',True)
 if palette_raster:
  if (folder/f'palette-banks{frame}.bin').exists():colors,pn=bank_palette_states(folder,frame)
  else:colors,pn=reconstruct(pal(frame-1),pal(frame),raster_records(folder,'palette').get(frame-1,[]),'palette_index',False)
  return states,n,colors,pn
 assert pal(frame-1)==pal(frame),'Palette raster changes need separate recovery'
 return states,n
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
  rgb,stats=renderer.render(v,p,meta[frame]['animation_counter'] if 'animation_disabled' in meta[frame] else meta[frame+1]['animation_counter'],states,meta[frame].get('animation_disabled',False));stats['raster_writes']=raster_writes;ref=np.asarray(Image.open(a.capture/f'game{frame}.png').convert('RGB'))
  assert ref.shape==rgb.shape,(ref.shape,rgb.shape)
  delta=np.max(np.abs(rgb.astype(np.int16)-ref.astype(np.int16)),axis=2);bad=int((delta[:,8:312]>1).sum())
  Image.fromarray(rgb).save(a.output/f'reconstructed{frame}.png');reports.append(dict(frame=frame,mismatched_pixels=bad,**stats));print(reports[-1],flush=True)
 sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
 (a.output/'validation.json').write_text(json.dumps(dict(status='FAIL' if any(r['mismatched_pixels'] for r in reports) else 'PASS',frames=reports,capture_frames_sha256=sha(a.capture/'frames.jsonl'),raster_reference_sha256=sha(a.raster_reference/'raster.jsonl') if a.raster_reference else None,screenshot_sha256={str(f):sha(a.capture/f'game{f}.png') for f in a.frames},phase_policy='Use recorded counter/disable state when available; the older capture format retains its explicitly limited next-boundary phase assumption.',limits='Offline reference reconstruction; no native intro integration. Older captures without the disable field still assume enabled auto-animation; new captures record it explicitly. Nonzero differences require explanation, not masking.'),indent=2)+'\n')
 assert not any(r['mismatched_pixels'] for r in reports),'Intro reference reconstruction still differs; inspect validation.json'
