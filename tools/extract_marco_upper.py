#!/usr/bin/env python3
"""Whole upper-controller calls, with independently observed service operations.

Only bounded numerical before/after values are exported. No ROM code, graphics,
full RAM or script tables are copied into the public fixture.
"""
import argparse,hashlib,json,re,struct
from pathlib import Path
from extract_marco_fields import fields
ROOT=Path(__file__).resolve().parents[1]
def extract(capture,output):
 text=(capture/'debug.log').read_text();assert 'Error in command' not in text
 rom=(capture/'program-fixed.bin').read_bytes();cases=[];active=None;choice=None;query=None;animation=None
 header=(ROOT/'src/marco_upper.h').read_text();pcs=[0xc0000+int(v,16) for v in re.findall(r'MUT_([0-9A-F]{4})',header)]
 tables=[]
 for pc in pcs:
  assert struct.unpack_from('>H',rom,pc)[0]==0x41fa
  tables.append(pc+2+struct.unpack_from('>h',rom,pc+2)[0])
 assert struct.unpack_from('>H',rom,0xc37e8)[0]==0x43fa
 compat=0xc37ea+struct.unpack_from('>h',rom,0xc37ea)[0]
 assert compat==0xbedc0
 metadata=dict(table_loads=pcs,table_tags=tables,fire_tables=[list(struct.unpack_from('>16I',rom,pc)) for pc in (0xc3c10,0xc3c50,0xc3c90)],compatibility=list(struct.unpack_from('>8H',rom,compat)),combo_limits=list(rom[0xc38fc:0xc393c]))
 def selected_pointer(table,form,character,direct):
  offset=((form*4+character)*4 if not direct else character*4)&65535
  assert offset<32768
  ptr=struct.unpack_from('>I',rom,table+offset)[0]
  if not ptr and not direct:ptr=struct.unpack_from('>I',rom,table+(offset&12))[0]
  return ptr
 for line in text.splitlines():
  if not line.startswith('MARCO_'):continue
  tag=line.split()[0];r=fields(line)
  if tag in ('MARCO_UPPER_IN','MARCO_UPPER_RESET_IN'):
   assert active is None
   assert r['player']==0,'Add independent evidence for the other input buffer'
   active=dict(kind='reset' if 'RESET' in tag else 'update',before=r,operations=[])
  elif tag in ('MARCO_UPPER_OUT','MARCO_UPPER_RESET_OUT'):
   assert active is not None and choice is None and query is None and animation is None
   assert r['frame']>=active['before']['frame'];active['after']=r;cases.append(active);active=None
  elif active is not None:
   if tag in ('MARCO_PROGRAM_IN','MARCO_DIRECT_PROGRAM_IN'):
    assert choice is None and animation is None
    if tag=='MARCO_PROGRAM_IN':assert r['part']==1
    choice=dict(kind='select',table=r['table'],direct=tag=='MARCO_DIRECT_PROGRAM_IN')
   elif tag in ('MARCO_PROGRAM_CHOICE','MARCO_DIRECT_PROGRAM_CHOICE') and choice is not None:
    choice['program']=r['program']
   elif tag in ('MARCO_PROGRAM_OUT','MARCO_DIRECT_PROGRAM_OUT') and choice is not None:
    b=active['before'];assert choice['program']==selected_pointer(choice['table'],b['form'],b['character'],choice['direct'])
    assert r['cursor']==choice['program'] and r['timer']==0 and r['speed']==256
    active['operations'].append(choice);choice=None
   elif tag=='MARCO_AVAILABLE_IN':
    assert query is None and animation is None;query=r
   elif tag=='MARCO_AVAILABLE_OUT':
    assert query is not None
    offset=((query['form']*4+query['character'])*4)&65535;assert offset<32768
    pointer=struct.unpack_from('>I',rom,query['table']+offset)[0]
    assert r['carry']==bool(pointer)
    active['operations'].append(dict(kind='available',table=query['table'],value=bool(r['carry'])));query=None
   elif tag=='MARCO_UPPER_RANDOM':active['operations'].append(dict(kind='random',value=r['value']))
   elif tag=='MARCO_UPPER_LATCH':active['operations'].append(dict(kind='latch',before=r['word'],mask=2))
   elif tag=='MARCO_WEAPON_IN':
    assert animation is None;animation=dict(kind='animate',before=r,allocations=[],grenades=0,draws=[],events=[])
   elif tag=='MARCO_DRAW' and animation is not None:
    animation['draws'].append(dict(descriptor=r['descriptor'],x=r['xy']>>16,y=r['xy']&65535,attributes=r['attr'],z=r['z']))
   elif tag in ('MARCO_EFFECT_IN','MARCO_SERVICE_IN') and animation is not None:
    animation['events'].append(dict(target=r.get('target',0x120f8),site=r['payload']-6 if 'payload' in r else None,allocations=[]))
    if r.get('target')==0xc4786:animation['grenades']+=1
   elif tag=='MARCO_UPPER_SPAWN_RESULT':
    assert animation is not None;animation['allocations'].append(bool(r['success']))
    assert animation['events'];animation['events'][-1]['allocations'].append(bool(r['success']))
   elif tag=='MARCO_WEAPON_OUT':
    assert animation is not None;animation['after']=r;animation['complete']=bool(r['d7']&65535)
    active['operations'].append(animation);animation=None
 assert active is None and animation is None and query is None and choice is None
 assert cases and any(c['kind']=='update' for c in cases)
 # Compact records retain every field name once and every case in exact order.
 columns=list(cases[0]['before']);assert all(list(c['before'])==columns and list(c['after'])==columns for c in cases)
 packed=[dict(kind=c['kind'],before=[c['before'][k] for k in columns],after=[c['after'][k] for k in columns],operations=c['operations']) for c in cases]
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 provenance={n:sha(capture/n) for n in ('capture.json','debug.log','program-fixed.bin','program-bank.bin')}
 limits=['Only the measured upper-controller calls; outer body/world updates remain caller inputs.','CI may replay measured animation/allocator services; private integration must also execute real native animation programs.','State +95 changes only after actual successful projectile allocation; grenade counters follow the observed global option.','Source-mode addresses are provenance tags mapped offline to native enums, never callable native pointers.']
 lines=['{','"source":'+json.dumps('MAME 0.276 full upper-controller entry/return, service calls and outcomes')+',','"columns":'+json.dumps(columns)+',','"metadata":'+json.dumps(metadata)+',','"cases":[']
 lines += [json.dumps(c,separators=(',',':'))+(',' if i+1<len(packed) else '') for i,c in enumerate(packed)]
 lines+= ['],','"provenance":'+json.dumps(provenance)+',','"limits":'+json.dumps(limits),'}']
 output.write_text('\n'.join(lines)+'\n')
 print('Extracted',len(cases),'whole calls;',sum(c['kind']=='reset' for c in cases),'resets;',sum(len(c['operations']) for c in cases),'observed services')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();extract(a.capture,a.output)
