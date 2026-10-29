#!/usr/bin/env python3
"""Extract observed grounded attack main-state and its two contact passes."""
import argparse,csv,hashlib,json,re
from pathlib import Path
from extract_soldier_rng import values
from extract_soldier_contact import actor,signed
from extract_soldier_control import state as control

def state(r):
 return dict(routine=r['routine'],field78=r['f78'],field7e=r['f7e'],field81=r['f81'],field61=r['f61'],field72=r['f72'])
def extract(capture,ghidra):
 log=(capture/'debug.log').read_text();assert 'Error in command' not in log
 pcs={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
 assert {0xaf918,0xaf964,0xaf994,0x1436a,0x143a2,0x1456a}<=pcs
 cases=[];active=None;phase=None;pending=None
 for line in log.splitlines():
  tag=line.split(' ',1)[0]
  if not tag.startswith('ATTACK_'):continue
  r=values(line)
  if tag=='ATTACK_STATE_INIT':pending=r
  elif tag=='ATTACK_STATE_BODY':
   assert active is None;active=dict(before=pending or r,begun=r,passes=[]);pending=None
  elif tag in ('ATTACK_STATIONARY_IN','ATTACK_GROUND_IN'):
   assert active and phase is None;phase=dict(before=r)
  elif tag=='ATTACK_CONTACT_IN' and phase is not None:
   assert 'request' not in phase;phase['request']=r
  elif tag=='ATTACK_CONTACT_OUT':
   assert phase;phase['kernel']=r
  elif tag=='ATTACK_CONTACT_COMMIT':
   assert phase;phase['commit']=r
   if len(active['passes'])==1:phase['after']=r;active['passes'].append(phase);phase=None
  elif tag=='ATTACK_STATIONARY_OUT':
   assert phase;phase['after']=r;active['passes'].append(phase);phase=None
  elif tag=='ATTACK_STATE_END':
   assert active and phase is None and len(active['passes'])==2;active['after']=r;cases.append(active);active=None
 assert active is None and len(cases)==20
 # The integrated regression reuses the independently captured animation fixture.
 # Require identical original boundaries in this new capture before combining them.
 reference=json.loads((Path(__file__).resolve().parents[1]/'data/soldier_attack_fixture.json').read_text())
 for tag,key in (('ATTACK_IN','before'),('ATTACK_OUT','after')):
  observed=[values(l) for l in log.splitlines() if l.startswith(tag+' ')]
  expected=[c[key] for c in reference['cases'] if c['kind']=='step']
  assert observed==expected,('Animation capture differs',tag)
 result=[]
 for c in cases:
  b=c['before'];passes=[]
  for p in c['passes']:
   o=p['kernel'];kin=p['request'];assert o['frame']==b['frame']
   passes.append(dict(before=actor(p['before']),after=actor(p['after']),commit=actor(p['commit']),
    kernel=dict(x=signed(o['d0']),y=signed(o['d1']),vx=signed(o['d2']),vy=signed(o['d3']),parent_tag=o['a0'],kind_angle_word=o['d7'],flags=o['d6']&65535),
    parent=dict(tag=o['a0'],x=signed(o['p14']),y=signed(o['p18']),previous_x=signed(o['p1c']),previous_y=signed(o['p20']),correction=o['p25']),epoch=kin['epoch'],
    mode=kin['mode'],mode8=kin['mode8'],mode9=kin['mode9']))
  trace=(capture/f"actor-{b['frame']}.log").read_text();sequence=[int(m[1],16) for l in trace.splitlines() if (m:=re.search(r' ([0-9A-F]{6}):',l))]
  assert sequence[:sequence.index(0xb051e)].count(0x29c6)==0
  seed=dict(passes[0]['before']);seed['scalar62']=signed(b['f62'])
  result.append(dict(frame=b['frame'],before=state(b),begun=state(c['begun']),after=state(c['after']),actor_before=seed,control_before=control(b),control_begun=control(c['begun']),control_after=control(c['after']),passes=passes))
 digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 return dict(source='MAME 0.276 original AF918/AF964 and 1436A/1456A wrapper boundaries',cases=result,provenance=dict(capture_sha256={n:digest(capture/n) for n in ('debug.log','program-fixed.bin','program-bank.bin','capture-options.json','run-command.json')},ghidra_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),limits=['Grounded flat static support only; no dynamic contact candidates in this observed profile.','Common-control inputs at each main-state entry remain supplied by the capture.'])
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n');print('Extracted 20 attack states and 40 contact passes')
