#!/usr/bin/env python3
"""Independent SCB/palette framebuffer references for native animation output.

Requires an already audited video capture matching every actor boundary in the
logic capture. No PNG is used as a source texture. Expected images come from
Neo Geo SCBs/C-ROM, not from the native drawing or animation implementations.
"""
import argparse,collections,hashlib,json,re,struct,zlib
from pathlib import Path
import numpy as np
from export_marco import reconstruct,palette_rgb
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('logic',type=Path);p.add_argument('video',type=Path);p.add_argument('--audit',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--publication',action='store_true');a=p.parse_args()
sha=lambda path:hashlib.sha256(path.read_bytes()).hexdigest()
audit=json.loads(a.audit.read_text());assert audit['status']=='PASS'
assert all(sha(a.video/name)==digest for name,digest in audit['provenance'].items())
for name in ('program-fixed.bin','program-bank.bin'):assert (a.logic/name).read_bytes()==(a.video/name).read_bytes()
plan=json.loads((a.logic/'plan.json').read_text());last=plan['last'];first=plan['first']
for frame in range(first,last+1):assert (a.logic/f'actor{frame}.bin').read_bytes()==(a.video/f'actor{frame}.bin').read_bytes(),('Different world/controller sequence',frame)
assets=json.loads((ROOT/'build/marco_pose_assets.json').read_text());profiles=assets['palette_rgba5551'];crom=(ROOT/'filesystem/sprites.bin').read_bytes()
draws=collections.defaultdict(list)
for frame,descriptor,xy,attributes,z in re.findall(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+) xy=([0-9A-F]+) attr=([0-9A-F]+) z=([0-9A-F]+)',(a.logic/'debug.log').read_text()):
 draws[int(frame)].append((int(descriptor,16),int(xy,16),int(attributes,16)))
display_at={}
if a.publication:
 from neo_queue_publication import published_batches
 batches,visible,_=published_batches((a.logic/'debug.log').read_text())
 for video,batch in sorted(visible.items()):display_at.setdefault(batch,video)
 draws={batch:[(d['descriptor'],d['xy'],d['attr']) for d in items] for batch,items in batches.items() if batch in display_at}
known={p['tag'] for p in assets['poses']};poses={p['tag']:p for p in assets['poses']};frames=[];limit_source=None
for frame,items in sorted(draws.items()):
 if not a.publication and frame+3>last:continue
 video_frame=display_at[frame] if a.publication else frame+3
 assert all(d[0] in known for d in items)
 palette=struct.unpack('>4096H',(a.video/f'palette{video_frame}.bin').read_bytes())
 banks=sorted({d[2]>>8 for d in items});assert len(banks)<=4
 native=[]
 for bank in banks:
  words=palette[bank*16:bank*16+16]
  converted=[0]+[((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1 for r,g,b in (palette_rgb(w) for w in words[1:])]
  assert converted in profiles,(frame,bank);native.append(profiles.index(converted))
 vram=struct.unpack('>34304H',(a.video/f'vram{video_frame}.bin').read_bytes())
 image,_=reconstruct(vram,palette,crom,selected_palette=set(banks))
 indices=np.frombuffer(image.info['neo_palette_indices'],dtype=np.uint16).reshape(256,320)
 lut=np.array([((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1 for r,g,b in (palette_rgb(w) for w in palette)],dtype=np.uint16)
 fb=np.ones((240,320),dtype='>u2');crop=indices[16:240,8:312];fb[8:232,8:312]=np.where(crop!=0,lut[crop],1)
 frames.append(dict(frame=frame,crc=zlib.crc32(fb.tobytes()),banks=banks,profiles=native))
 if limit_source is None and len(items)==1 and np.count_nonzero(indices)==poses[items[0][0]]['opaque_pixels']:
  descriptor,xy,attributes=items[0];ox=xy>>16;oy=(512-(xy&65535))&511
  points=[]
  for y,x in zip(*np.nonzero(indices)):
   dx=int(x)-ox;dy=int(y)-oy
   if attributes&1:dx=-dx-2
   if attributes&2:dy=-dy-2
   points.append((dx,dy,int(lut[indices[y,x]])))
  limit_source=(descriptor,points,native[0],frame)

lines=['/* Independent audited Neo Geo SCB reference; crop 8,16..312,240 -> 8,8. */','static const struct {unsigned frame;uint32_t crc;uint8_t banks[4],profiles[4],count;} pixel_frames[]={']
arr=lambda x:'{'+','.join(str(v) for v in x)+'}'
for f in frames:lines.append('{'+str(f['frame'])+','+str(f['crc'])+'u,'+arr(f['banks'])+','+arr(f['profiles'])+','+str(len(f['banks']))+'},')
assert limit_source is not None
limits=[];descriptor,points,profile,source_frame=limit_source
for ax,ay in ((0,0),(511,511),(8,16),(312,240)):
 for transform in range(4):
  fb=np.ones((240,320),dtype='>u2')
  for dx,dy,color in points:
   xx=(ax+(-dx-2 if transform&1 else dx))&511
   yy=((ay+(-dy-2 if transform&2 else dy))&511)-8
   if 8<=xx<312 and 8<=yy<232:fb[yy,xx]=color
  limits.append(dict(draw=[descriptor,ax,(-ay)&65535,transform,0],profile=profile,crc=zlib.crc32(fb.tobytes())))
lines+=['};','/* Synthetic transforms/edges of an independently observed SCB pose. */','static const struct {neo_anim_draw_t draw;unsigned profile;uint32_t crc;} native_limit_cases[]={']
for c in limits:lines.append('{'+arr(c['draw'])+','+str(c['profile'])+','+str(c['crc'])+'u},')
lines+=['};'];a.output.write_text('\n'.join(lines)+'\n')
report=dict(kind='publication_batches' if a.publication else 'physical_frame_submissions',frames=len(frames),synthetic_transform_limits=len(limits),synthetic_source_frame=source_frame,logic_provenance=sha(a.logic/'capture.json'),video_provenance=sha(a.video/'capture.json'),audit_sha256=sha(a.audit),metadata_sha256=sha(ROOT/'build/marco_pose_assets.json'),header_sha256=sha(a.output),limits='Native render of helper submissions only. World, table choice and TLUT selection still supplied at recorded controller boundaries. Only submissions with independently observed SCB output are included. No gameplay release.')
a.output.with_suffix('.json').write_text(json.dumps(report,indent=2)+'\n');print('Compiled',len(frames),'independent framebuffer references')
