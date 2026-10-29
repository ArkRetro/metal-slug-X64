#!/usr/bin/env python3
"""Extract observed inline pose selection with its actual pre-read field value."""
import argparse,hashlib,json,re,struct
from pathlib import Path
from extract_marco_fields import fields
p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
text=(a.capture/'debug.log').read_text();bank=(a.capture/'program-bank.bin').read_bytes();cases=[];pending=None;command=None
for line in text.splitlines():
 if line.startswith('MARCO_COMMAND'):
  if pending is not None:cases.append(pending);pending=None
  command=fields(line)
 elif line.startswith('MARCO_SELECT'):
  s=fields(line);assert command and command['cursor']==s['cursor'] and command['frame']==s['frame'];at=s['cursor']-0x200000
  assert bank[at]==0x10 and bank[at+1]==s['field'] and struct.unpack_from('>H',bank,at+2)[0]==s['count']*8
  poses=[]
  for i in range(s['count']):
   pos=at+4+i*8;op,flags,dx,dy,asset=struct.unpack_from('>BBbbI',bank,pos);assert op==4
   poses.append(dict(flags=flags,dx=dx,dy=dy,asset=asset))
  render={k:s[k] for k in ('x','y','attributes','z','correction','flash_palette')}
  render['palettes']=[s['palettes']>>shift&255 for shift in (24,16,8,0)]
  pending=dict(frame=s['frame'],command_rom=f"0x{s['cursor']:06X}",state_ram=f"0x{s['state']:06X}",field=s['field'],value=s['value'],timer=command['timer'],flash=command['f17'],poses=poses,render=render,draws=[])
 elif line.startswith('MARCO_DRAW') and pending is not None:
  row=fields(line);assert row['frame']==pending['frame'];selected=pending['poses'][min(pending['value'],len(pending['poses'])-1)]
  assert row['descriptor']==selected['asset']
  r=pending['render'];dx=selected['dx']*2+(selected['flags']>>1&1);dy=selected['dy']*2+(selected['flags']&1)
  x=(r['x']+(-dx if r['attributes']&1 else dx)-(r['correction']&1))&65535
  y=(r['y']+(-dy if r['attributes']&2 else dy)-(r['correction']>>1&1))&65535
  assert (x,y,r['z'])==(row['xy']>>16,row['xy']&65535,row['z']),('Pre-read render inputs do not match the executed helper',pending['frame'],(x,y,r['z']),(row['xy']>>16,row['xy']&65535,row['z']))
  pending['draws'].append(dict(descriptor=row['descriptor'],x=row['xy']>>16,y=row['xy']&65535,attributes=row['attr'],z=row['z']))
if pending is not None:cases.append(pending)
assert len(cases)>=272 and all(len(c['draws'])<=1 for c in cases)
sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
report=dict(source='Executed MAME pre-read 0x004412 and draw 0x004406 probes',cases=cases,provenance={n:sha(a.capture/n) for n in ('capture.json','debug.log','program-fixed.bin','program-bank.bin')},limits=['Observed body/weapon inline selectors only.','Render inputs probed before the original selector reads the field; checked against executed draw coordinates.','Complete player controller is not established by isolated selector tests.'])
a.output.write_text(json.dumps(report,indent=2)+'\n');print('Verified',len(cases),'original selectors',sorted({c['field'] for c in cases}))
