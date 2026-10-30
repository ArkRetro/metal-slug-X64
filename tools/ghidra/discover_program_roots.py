#!/usr/bin/env python3
"""Find candidate 68000 entry points across ALL P1 bytes, recording their evidence.

Byte signatures are candidates, not proof that the surrounding bytes are code.
"""
import argparse,hashlib,json
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--inputs',type=Path,required=True);p.add_argument('--prior',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
b=(a.inputs/'m68k-p1.bin').read_bytes();roots={int(s,16) for s in a.prior.read_text().splitlines()};evidence=[]
for pc in range(0,len(b)-8,2):
 op=int.from_bytes(b[pc:pc+2],'big');target=int.from_bytes(b[pc+2:pc+6],'big')
 if not (0x100<=target<0x100000 and target%2==0):continue
 kind={0x4eb9:'JSR_absolute_candidate',0x4ef9:'JMP_absolute_candidate',0x2cbc:'actor_state_write_candidate'}.get(op)
 if op in (0x41f9,0x207c):
  following=b[pc+6:pc+36]
  if any(following.find(bytes.fromhex(call))>=0 for call in ('4eb9000040bc','4eb90000410e')):kind='allocator_A0_candidate'
 if kind:roots.add(target);evidence.append(dict(site=hex(pc),target=hex(target),evidence=kind,verified_execution=False))
(a.output/'seeds.txt').write_text('\n'.join(f'{n:x}' for n in sorted(roots))+'\n')
(a.output/'candidate-evidence.json').write_text(json.dumps(evidence,indent=2)+'\n')
(a.output/'validation.json').write_text(json.dumps(dict(status='WHOLE_P1_SIGNATURE_CANDIDATES',roots=len(roots),sites=len(evidence),rom_sha256=hashlib.sha256(b).hexdigest(),limits='Complete byte scan for these signatures only; data may resemble instructions. Does not establish complete game code coverage.'),indent=2)+'\n')
print('Candidate roots',len(roots),'from',len(evidence),'sites')
