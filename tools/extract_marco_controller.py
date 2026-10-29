#!/usr/bin/env python3
"""Extract executed controller table choices/input filtering without guesses.

Only bounded numerical table words and before/after fields are exported. ROM,
full journals and converted executable script data remain private.
"""
import argparse,hashlib,json,struct
from pathlib import Path
from extract_marco_fields import fields

def extract(capture,output):
 text=(capture/'debug.log').read_text();assert 'Error in command' not in text
 fixed=(capture/'program-fixed.bin').read_bytes();bank=(capture/'program-bank.bin').read_bytes()
 def word(address):
  if 0<=address<=0xffffc:return struct.unpack_from('>I',fixed,address)[0]
  assert 0x200000<=address<=0x2ffffc,hex(address)
  return struct.unpack_from('>I',bank,address-0x200000)[0]
 inputs=[];choices=[];active_input=None;active_choice=None
 for line in text.splitlines():
  if not line.startswith(('MARCO_INPUT_','MARCO_PROGRAM_')):continue
  row=fields(line);tag=line.split()[0]
  if tag=='MARCO_INPUT_IN':
   assert active_input is None;active_input=row
  elif tag=='MARCO_INPUT_OUT':
   assert active_input is not None and active_input['frame']<=row['frame']
   inputs.append(dict(before=active_input,after=row));active_input=None
  elif tag=='MARCO_PROGRAM_IN':
   assert active_choice is None;active_choice=dict(before=row)
  elif tag=='MARCO_PROGRAM_CHOICE':
   assert active_choice and 'choice' not in active_choice
   b=active_choice['before'];assert row['frame']>=b['frame'] and row['part']==b['part'] and row['table']==b['table']
   offset=((b['form']*4+b['character'])*4)&65535
   signed=offset if offset<32768 else offset-65536
   primary=word(b['table']+signed)
   expected_offset=offset if primary else offset&12
   expected=primary or word(b['table']+expected_offset)
   assert row['offset']==expected_offset and row['program']==expected
   active_choice.update(choice=row,primary=primary,requested_offset=offset)
  elif tag=='MARCO_PROGRAM_OUT':
   assert active_choice and 'choice' in active_choice
   b=active_choice['before'];choice=active_choice['choice']
   assert row['frame']>=b['frame'] and row['part']==b['part']
   assert row['cursor']==choice['program'] and row['timer']==0 and row['speed']==256
   assert row['flash']==b['flash'] and row['field50']==b['field50'] and row['f18']==(0 if b['part']==0 else b['f18'])
   active_choice['after']=row;choices.append(active_choice);active_choice=None
 assert active_input is None and active_choice is None
 assert inputs and choices
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 report=dict(source='MAME 0.276 controller input filtering and program selection entry/choice/return probes',inputs=inputs,choices=choices,provenance={n:sha(capture/n) for n in ('capture.json','debug.log','program-fixed.bin','program-bank.bin')},limits=['Input-buffer selection CA2CE remains an external service.','Caller selection of the animation table and overall state transitions remain external.','Only observed table words are recovered; no table extent or unused entry is inferred.','Unobserved forms, characters and fallback paths require separate coverage.'])
 output.write_text(json.dumps(report,indent=2)+'\n')
 print('Verified',len(inputs),'complete input-filter calls,',len(choices),'table selections;',sum(not c['primary'] for c in choices),'observed fallbacks')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();extract(a.capture,a.output)
