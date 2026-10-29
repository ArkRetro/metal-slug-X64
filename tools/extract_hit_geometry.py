#!/usr/bin/env python3
"""Extract observed narrow-phase candidates without assigning health semantics."""
import argparse,csv,hashlib,json,re,struct
from pathlib import Path
from extract_soldier_rng import values

def extract(capture,ghidra):
 text=(capture/'debug.log').read_text();assert 'Error in command' not in text
 addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
 assert {0x4798,0x47c0,0x47e6,0x4816,0x484a}<=addresses
 pending=None;cases=[]
 for line in text.splitlines():
  if line.startswith('HIT_CANDIDATE_IN '):
   assert pending is None;prefix,data=line.split(' data=');pending=values(prefix);pending['words']=list(struct.unpack('>24H',bytes.fromhex(data)))
  elif line.startswith('HIT_CANDIDATE_OUT '):
   out=values(line);assert pending and pending['frame']==out['frame'];r=pending;pending=None
   assert r['descriptor'] and r['callback']==0 and r['words'][1]!=0x8000 and r['words'][5]==0x8000
   trace=(capture/f"child-{r['frame']}.log").read_text();hit='00484A:' in trace
   assert hit==(out['result']==5)
   expected={}
   if hit:
    # Tracelog records registers before each instruction, including both stores.
    for pc,reg,name in (('004816','D2','impact_y'),('00484A','D1','impact_x')):
     rows=[l for l in trace.splitlines() if pc+':' in l];assert len(rows)==1
     expected[name]=int(re.search(reg+r'=([0-9A-F]{8})',rows[0])[1],16)&65535
   cases.append(dict(frame=r['frame'],target_x=r['x'],target_y=r['y'],flags=r['flags'],
    query=dict(x=r['d4']&65535,y=r['d4']>>16,width=r['d5']&65535,height=r['d5']>>16),
    rectangle=dict(zip(('x','y','width','height'),r['words'][1:5])),hit=hit,**expected))
 assert pending is None and len(cases)==8 and sum(c['hit'] for c in cases)==1
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 return dict(cases=cases,source='MAME 0.276 allocated-child query 0x049E8, candidate 0x04762 and executed narrow-phase stores',
  provenance=dict(capture_sha256=sha(capture/'debug.log'),assembly_sha256=sha(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),
  limits=['Eight P1 candidates, one overlap; mirror branches have static analysis only.','No registry/filter engine, damage, player death or health interpretation is implemented by the geometry helper.'])
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.write_text(json.dumps(extract(a.capture,a.ghidra),indent=2)+'\n')
