#!/usr/bin/env python3
"""Extract independently observed actor-field animation operations for Marco."""
import argparse,csv,hashlib,json,re,struct
from pathlib import Path
KEYS=('f72','f76','f77','f78')
def fields(line):return {k:int(v,10 if k=='frame' else 16) for k,v in re.findall(r'(\w+)=([0-9A-F]+)',line)}
def extract(capture,ghidra,output):
 log=(capture/'debug.log').read_text();assert 'Error in command' not in log
 bank=(capture/'program-bank.bin').read_bytes()
 assembly=list(csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t'));addresses={int(r['address'],16) for r in assembly}
 assert {0x445e,0x446a,0x4476,0x44ae,0x44b4,0x44b8}<=addresses
 rows=[fields(line) for line in log.splitlines() if line.startswith('MARCO_COMMAND')];cases=[]
 for before,after in zip(rows,rows[1:]):
  at=before['cursor']-0x200000;op=bank[at]
  if op not in (0x18,0x1c,0x20,0xc):continue
  assert before['frame']==after['frame'] and before['state']==after['state']
  c=dict(frame=before['frame'],command_rom=f"0x{before['cursor']:06X}",state_ram=f"0x{before['state']:06X}",before=[before[k] for k in KEYS],after=[after[k] for k in KEYS])
  if op==0x18:
   kind='WRITE_BYTE';field=struct.unpack_from('>H',bank,at+2)[0];value=bank[at+1];size=4
  elif op==0x1c:kind='WRITE_WORD';field=bank[at+1];value=struct.unpack_from('>H',bank,at+2)[0];size=4
  elif op==0x20:kind='WRITE_LONG';field=bank[at+1];value=struct.unpack_from('>I',bank,at+2)[0];size=6
  else:
   kind='BRANCH_FIELD';field=struct.unpack_from('>H',bank,at+2)[0];value=bank[at+1];size=8
  assert field in (0x72,0x74,0x76,0x77,0x78),(hex(op),hex(field))
  if op==0xc:
   target=struct.unpack_from('>I',bank,at+4)[0]
   taken=bool(before['f78'])==bool(value)
   assert after['cursor']==(target if taken else before['cursor']+size)
   assert c['before']==c['after'];c['taken']=taken;c['original_target']=f'0x{target:06X}'
  else:
   assert after['cursor']==before['cursor']+size
   expected=c['before'].copy()
   if field==0x72:expected[0]=value if op==0x20 else (expected[0]&65535)|(value<<16)
   elif field==0x74:expected[0]=(expected[0]&0xffff0000)|value
   else:expected[{0x76:1,0x77:2,0x78:3}[field]]=value
   assert c['after']==expected
  c.update(kind=kind,field=field,value=value);cases.append(c)
 assert len(cases)>=500 and {c['kind'] for c in cases}=={'WRITE_BYTE','WRITE_WORD','WRITE_LONG','BRANCH_FIELD'}
 sha=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
 report=dict(source='Executed MAME 0.276 command entry probes plus addressed Ghidra cross-check',fields=KEYS,cases=cases,provenance={n:sha(capture/n) for n in ('capture.json','debug.log','program-fixed.bin','program-bank.bin')},ghidra_sha256=sha(ghidra/'instructions.tsv'),limits=['Player body/weapon animation field operations only; complete controller and callbacks remain separate work.','Field names are neutral; the complete gameplay meanings of field72/76/77/78 are not asserted.','DEC.B command has addressed static evidence but no executed case in this capture.'])
 output.parent.mkdir(parents=True,exist_ok=True);output.write_text(json.dumps(report,indent=2)+'\n');print('Verified',len(cases),'original field/branch commands')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();extract(a.capture,a.ghidra,a.output)
