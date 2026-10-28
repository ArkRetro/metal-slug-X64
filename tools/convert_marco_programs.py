#!/usr/bin/env python3
"""Convert observed Marco script graphs to typed native data, never 68000 code.

Unrecovered callbacks remain explicit stopping points. The private graph is
not a claim of complete player control or complete game-wide pose coverage.
"""
import argparse,collections,hashlib,json,re,struct
from pathlib import Path
from extract_marco_fields import fields

def convert(capture,output,event_evidence=None):
 bank=(capture/'program-bank.bin').read_bytes();text=(capture/'debug.log').read_text();assert 'Error in command' not in text
 initial_roots={int(v,16) for v in re.findall(r'MARCO_INIT frame=\d+ program=([0-9A-F]+)',text)}
 verified_events={};events=[]
 if event_evidence is not None:
  proof=json.loads(event_evidence.read_text());assert proof['status']=='UNCHANGED_MONITORED_ANIMATION_FIELDS'
  for name,digest in proof['provenance'].items():assert hashlib.sha256((capture/name).read_bytes()).hexdigest()==digest
  verified_events={e['site']:e for e in proof['sites']}
 roots=set(initial_roots)
 observed=[fields(l) for l in text.splitlines() if l.startswith('MARCO_COMMAND')]
 roots.update(r['cursor'] for r in observed);nodes={};unrecovered=[]
 def walk(pc):
  while pc not in nodes:
   assert 0x200000<=pc<0x300000,hex(pc);at=pc-0x200000;op,arg=bank[at:at+2]
   c=dict(address=pc,kind='',asset=0,target=0,operand=0,flags=0,dx=0,dy=0);nodes[pc]=c
   if op==0:c.update(kind='WAIT',operand=arg);size=2
   elif op==4:
    c.update(kind='POSE',asset=struct.unpack_from('>I',bank,at+4)[0],flags=arg,dx=struct.unpack_from('b',bank,at+2)[0],dy=struct.unpack_from('b',bank,at+3)[0]);size=8
   elif op==8:
    assert arg==0;dest=struct.unpack_from('>I',bank,at+2)[0];c.update(kind='JUMP',target_address=dest);walk(dest);return
   elif op==0xc:
    field=struct.unpack_from('>H',bank,at+2)[0];dest=struct.unpack_from('>I',bank,at+4)[0]
    c.update(kind='BRANCH_FIELD',asset=field,operand=arg,target_address=dest);walk(dest);size=8
   elif op==0x10:
    length=struct.unpack_from('>H',bank,at+2)[0];assert length and length%8==0 and length//8<=255
    c.update(kind='SELECT_FIELD_POSE',asset=arg,operand=length//8,target_address=pc+4)
    for offset in range(0,length,8):
     where=pc+4+offset;assert bank[at+4+offset]==4
     x=at+4+offset;nodes[where]=dict(address=where,kind='POSE',asset=struct.unpack_from('>I',bank,x+4)[0],flags=bank[x+1],dx=struct.unpack_from('b',bank,x+2)[0],dy=struct.unpack_from('b',bank,x+3)[0],target=0,operand=0)
    size=4+length
   elif op==0x14:
    assert arg>=4;callback=struct.unpack_from('>I',bank,at+2)[0]
    payload=bank[at+6:at+2+arg].hex()
    if pc in verified_events:
     e=verified_events[pc];assert e['kind']=='CALLBACK' and e['target']==callback and e['payload']==payload
     c.update(kind='EVENT',asset=len(events));events.append(dict(e,native_id=c['asset']))
    else:
     c.update(kind='UNRECOVERED',asset=callback);unrecovered.append(dict(command=pc,callback=callback,payload=payload))
    size=2+arg
   elif op==0x18:
    c.update(kind='WRITE_BYTE',asset=arg,target=struct.unpack_from('>H',bank,at+2)[0]);size=4
   elif op==0x1c:
    c.update(kind='WRITE_WORD',target=arg,asset=struct.unpack_from('>H',bank,at+2)[0]);size=4
   elif op==0x20:
    c.update(kind='FIELD50' if arg==0x50 else 'WRITE_LONG',target=arg,asset=struct.unpack_from('>I',bank,at+2)[0]);size=6
   elif op==0x24:c.update(kind='DECREMENT_BYTE',target=arg);size=2
   elif op==0x28:c.update(kind='FINISH');return
   elif op==0x30:
    token=struct.unpack_from('>H',bank,at+2)[0]
    if pc in verified_events:
     e=verified_events[pc];assert e['kind']=='SERVICE' and e['target']==0x120f8 and e['token']==token
     c.update(kind='EVENT',asset=len(events));events.append(dict(e,native_id=c['asset']))
    else:
     c.update(kind='UNRECOVERED',asset=token);unrecovered.append(dict(command=pc,service='original_0x0120F8',token=token))
    size=4
   else:raise AssertionError(('Unrecovered script command',hex(pc),hex(op)))
   pc+=size
 for root in sorted(roots):walk(root)
 ordered=sorted(nodes);indices={pc:i for i,pc in enumerate(ordered)}
 for c in nodes.values():
  if 'target_address' in c:c['target']=indices[c.pop('target_address')]
 for c in nodes.values():
  if c['kind']=='SELECT_FIELD_POSE':assert all(nodes[ordered[c['target']+i]]['kind']=='POSE' for i in range(c['operand']))
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 graph=dict(initial_program_roots=sorted(initial_roots),entry_seeds=[dict(source=pc,native_index=indices[pc]) for pc in sorted(roots)],commands=[nodes[pc] for pc in ordered],unrecovered=unrecovered,events=events,provenance={n:sha(capture/n) for n in ('capture.json','debug.log','program-bank.bin')},limits='Observed script graphs only. UNRECOVERED callbacks/services must stop native execution, not become silent no-ops. Complete player controller still required.')
 output.mkdir(parents=True,exist_ok=False);(output/'graph.json').write_text(json.dumps(graph,indent=2)+'\n')
 lines=['/* Converted native script data, not executable original addresses. */','#include "neo_animation.h"','static const neo_anim_command_t marco_commands[]={']
 for pc in ordered:
  c=nodes[pc];lines.append('{NEO_ANIM_'+c['kind']+','+','.join(str(c[k]) for k in ('asset','target','operand','flags','dx','dy'))+'},')
 lines+=['};','static const neo_anim_program_t marco_script={marco_commands,sizeof(marco_commands)/sizeof(*marco_commands)};']
 assert len(nodes)<65536;(output/'marco_script.h').write_text('\n'.join(lines)+'\n')
 print('Converted',len(nodes),'commands,',len(roots),'observed command seeds;',len(unrecovered),'explicit unrecovered callbacks/services')
 print('Measured native request sites:',len(events))
 print(collections.Counter(c['kind'] for c in nodes.values()))
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--event-evidence',type=Path);a=p.parse_args();convert(a.capture,a.output,a.event_evidence)
