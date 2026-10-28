#!/usr/bin/env python3
"""Export bounded, observed crouch-body calls and their external service boundaries."""
import argparse,hashlib,json,re,struct
from pathlib import Path
from extract_marco_fields import fields
ROOT=Path(__file__).resolve().parents[1]
def extract(capture,output):
 text=(capture/'debug.log').read_text();assert 'Error in command' not in text
 rom=(capture/'program-fixed.bin').read_bytes()
 pcs=[0xc0000+int(v,16) for v in re.findall(r'MCT_([0-9A-F]{4})',(ROOT/'src/marco_crouch.h').read_text())]
 tables=[]
 for pc in pcs:
  assert struct.unpack_from('>H',rom,pc)[0]==0x41fa
  tables.append(pc+2+struct.unpack_from('>h',rom,pc+2)[0])
 metadata=dict(table_loads=pcs,table_tags=tables,fire_tables=list(struct.unpack_from('>16I',rom,0xc24de)))
 active=None;pending=None;cases=[]
 for l in text.splitlines():
  if not l.startswith('MARCO_'):continue
  tag=l.split()[0];r=fields(l)
  if tag=='MARCO_BODY_IN':
   assert active is None;active=dict(before=r,operations=[])
  elif tag=='MARCO_BODY_OUT':
   assert active is not None and pending is None
   active['after']=r;cases.append(active);active=None
  elif active is not None:
   if tag=='MARCO_PROGRAM_IN':
    assert pending is None and r['part']==0
    pending=dict(kind='select',table=r['table'])
   elif tag=='MARCO_PROGRAM_CHOICE':
    assert pending is not None;pending['program']=r['program']
    b=active['before'];offset=(b['f93']*4+b['character'])*4
    ptr=struct.unpack_from('>I',rom,pending['table']+offset)[0]
    if not ptr:ptr=struct.unpack_from('>I',rom,pending['table']+(offset&12))[0]
    assert ptr==r['program']
   elif tag=='MARCO_PROGRAM_OUT':
    assert pending is not None and r['cursor']==pending['program'] and r['timer']==0 and r['speed']==256
    active['operations'].append(pending);pending=None
   elif tag=='MARCO_BODY_MOVE_IN':
    assert pending is None;pending=dict(kind='move',before=r)
   elif tag=='MARCO_BODY_MOVE_OUT':
    assert pending is not None and pending['kind']=='move';pending['after']=r
    # Every changed world field must be an explicit service output.
    changed=[k for k in r if k!='frame' and r[k]!=pending['before'][k]]
    assert set(changed)<={'x','y','vx','vy','attr','field50'},changed
    active['operations'].append(pending);pending=None
   elif tag=='MARCO_STEP_IN':
    assert pending is None and r['state']==0x1014e4
    pending=dict(kind='animate',before=r,view_z=r.get('z'),draws=[],events=[])
   elif tag in ('MARCO_STEP_OUT','MARCO_NULL_OUT'):
    assert pending is not None and pending['kind']=='animate'
    pending['after']=r;pending['complete']=bool(r['sr']&1)
    active['operations'].append(pending);pending=None
   elif tag=='MARCO_DRAW' and pending is not None and pending['kind']=='animate':
    pending['draws'].append(dict(descriptor=r['descriptor'],x=r['xy']>>16,y=r['xy']&65535,attributes=r['attr'],z=r['z']))
   elif tag in ('MARCO_EFFECT_IN','MARCO_SERVICE_IN') and pending is not None and pending['kind']=='animate':
    pending['events'].append(dict(target=r.get('target',0x120f8),site=r['payload']-6 if 'payload' in r else None,allocations=[]))
   elif tag=='MARCO_UPPER_SPAWN_RESULT' and pending is not None and pending['kind']=='animate':
    assert pending['events'];pending['events'][-1]['allocations'].append(bool(r['success']))
 assert active is None and pending is None and cases
 columns=[k for k in cases[0]['before'] if k not in ('frame','entry')]
 assert all(set(c['before'])==set(columns)|{'frame','entry'} and set(c['after'])==set(columns)|{'frame','exit'} for c in cases)
 rows=[dict(frame=c['before']['frame'],entry=c['before']['entry'],exit=c['after']['exit'],before=[c['before'][k] for k in columns],after=[c['after'][k] for k in columns],operations=c['operations']) for c in cases]
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 report=dict(source='MAME 0.276 observed crouch-controller blocks, read-only RAM probes and scheduled inputs',columns=columns,metadata=metadata,provenance={n:sha(capture/n) for n in ('capture.json','debug.log','program-fixed.bin','program-bank.bin')},limits=['Only C2214..C263E; standing, jumping, falling and common C32F4 processing are explicit external tail calls.','Host fixtures replay measured ground and animation services; this alone is not full native animation/campaign integration.','Body script allocator outcomes are world inputs; native projectile counters are computed.'])
 lines=['{']+[json.dumps(k)+':'+json.dumps(v)+',' for k,v in report.items()]+['"cases":[']+[json.dumps(c,separators=(',',':'))+(',' if i+1<len(rows) else '') for i,c in enumerate(rows)]+[']}']
 output.write_text('\n'.join(lines)+'\n');print('Extracted',len(cases),'crouch blocks and',sum(len(c['operations']) for c in cases),'services')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();extract(a.capture,a.output)
