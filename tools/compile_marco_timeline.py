#!/usr/bin/env python3
"""Join independently extracted controller and animation calls in debugger order."""
import argparse,hashlib,json
from pathlib import Path
from extract_marco_fields import fields
p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('steps',type=Path);p.add_argument('--controller',type=Path,default=Path('data/marco_controller_fixture.json'));p.add_argument('--output',type=Path,required=True);p.add_argument('--publication',action='store_true');a=p.parse_args()
controller=json.loads(a.controller.read_text());steps=json.loads(a.steps.read_text());log=(a.capture/'debug.log').read_text();digest=hashlib.sha256(log.encode()).hexdigest()
assert controller['provenance']['debug.log']==steps['provenance']['debug.log']==digest
counts=[0,0,0];ops=[];pub_flag=None;publications=0
for line in log.splitlines():
 if a.publication and line.startswith(('MARCO_QUEUE_SUBMIT ','MARCO_QUEUE_SWAP ','MARCO_VIDEO_STATE ')):
  r=fields(line);tag=line.split()[0]
  if tag=='MARCO_QUEUE_SWAP':
   publications+=1;pub_flag=r['data']&255;ops.append((3,publications,r['frame']))
  else:
   if pub_flag is not None and r['flag']!=pub_flag:
    assert publications==0,'Missing publication after watchpoint setup'
    ops.append((4,0,r['frame']))
   pub_flag=r['flag']
  continue
 if not line.startswith(('MARCO_INPUT_OUT ','MARCO_PROGRAM_OUT ','MARCO_STEP_OUT ','MARCO_WEAPON_OUT ')):continue
 row=fields(line);tag=line.split()[0]
 if tag=='MARCO_INPUT_OUT':
  kind=0;assert row==controller['inputs'][counts[0]]['after']
 elif tag=='MARCO_PROGRAM_OUT':
  kind=1;assert row==controller['choices'][counts[1]]['after']
 elif tag=='MARCO_STEP_OUT' and row['state']==0x10154c:continue
 else:
  kind=2;assert row==steps['cases'][counts[2]]['after']
 ops.append((kind,counts[kind],row['frame']));counts[kind]+=1
assert counts==[len(controller['inputs']),len(controller['choices']),len(steps['cases'])]
a.output.write_text('/* Executed input/select/step ordering. Caller state transitions remain external. */\nstatic const struct {unsigned kind,index,frame;} controller_timeline[]={\n'+''.join('{%d,%d,%d},\n'%op for op in ops)+'};\n')
print('Joined',len(ops),'controller/animation/publication operations in original order:',counts,'publications:',publications)
