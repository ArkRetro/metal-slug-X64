#!/usr/bin/env python3
"""Can the RDP draw the LSPC shrink exactly (TEXTURE_RECTANGLE, point sampling: s/t 10.5, ds/dt 5.10)? Per shrink
nibble (COLUMNS, also mirrored) and per L0 zoom y tile row: the fewest exact affine pieces; then rectangles per frame
of the zoomed sprites of the dumps of ZC, ZT and T1. docs/MISION6.md "Zoom con el RDP"."""
import sys,zipfile,functools;sys.path.insert(0,'tools')
from check_lspc_shrink import COLUMNS
def fits(seq):
  # some s0 (1/32) and d (1/1024) with floor((32*s0+d*i)/1024)==seq[i]
  if len(seq)==1:return True
  k=len(seq)-1;a=seq[-1]-seq[0]
  for d in range(max(-16*1024,((a-1)*1024)//k-2),min(16*1024,((a+1)*1024)//k+3)):
    lo=-10**9;hi=10**9   # S range in 1/1024, S multiple of 32
    for i,c in enumerate(seq):
      lo=max(lo,c*1024-d*i);hi=min(hi,c*1024+1023-d*i)
      if lo>hi:break
    else:
      S=-(-lo//32)*32
      if S<=hi:return True
  return False
def pieces(seq):
  @functools.lru_cache(None)
  def best(k):
    if k==len(seq):return 0
    return min(1+best(j) for j in range(k+1,len(seq)+1) if fits(seq[k:j]))
  return best(0)
tot=[]
for n,c in enumerate(COLUMNS):
  p=pieces(tuple(c));q=pieces(tuple(15-x for x in c));tot.append((p,q));print('H',n,p,q)
l0=zipfile.ZipFile((sys.argv[1] if len(sys.argv)>1 else 'neogeo.zip')).read('000-lo.lo')
V={}
worst=0;wz=None
for zy in range(256):
  rows={}
  for zl in range(256):
    b=l0[(zy<<8)|zl];rows.setdefault(b>>4,[]).append(b&15)
  # lines of one tile row are consecutive output lines? check contiguity
  per={t:pieces(tuple(v)) for t,v in rows.items()}
  V[zy]=per
  m=max(per.values())
  if m>worst:worst,wz=m,zy
print('V worst pieces per tile row',worst,'at zy',hex(wz),'; zy=ff:',max(V[255].values()))
import collections
print('V histogram of max per tile row over zy:',collections.Counter(max(p.values()) for p in V.values()))
# per-frame rect estimate from VRAM dumps
import glob,struct
from check_lspc_shrink import sprites,line_tile
for d in ['build/captures/ZC','build/captures/ZT','/home/user/captures-m6/fam/T1']:
  best=0;bf=None
  for f in sorted(glob.glob(d+'/vram*.bin')):
    v=struct.unpack('>34304H',open(f,'rb').read());n=0
    for slot,x,y,rw,zx,zy in sprites(v):
      if not rw or (zx==15 and zy==255):continue
      seen=set()
      for r in range(16,240):
        t=line_tile(l0,zy,rw,(r-y)&511)
        if t and t[0] not in seen:seen.add(t[0]);n+=tot[zx][0]*V[zy].get(t[0] if not (r-y)&256 else t[0]^0x1f,1)
    if n>best:best,bf=n,f
  print(d,'max rects/frame',best,bf[-14:])
