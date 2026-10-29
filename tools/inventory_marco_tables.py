#!/usr/bin/env python3
"""Inspect explicitly requested variant rows of observed Marco tables.

Static inspection is not execution coverage. Rows are supplied explicitly;
no table length, transformation name or whole-game completeness is inferred.
"""
import argparse,hashlib,json,struct
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--fixture',type=Path,default=Path('data/marco_controller_fixture.json'));p.add_argument('--forms',type=int,nargs='+',required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
fixture=json.loads(a.fixture.read_text());fixed=(a.capture/'program-fixed.bin').read_bytes();bank=(a.capture/'program-bank.bin').read_bytes()
assert hashlib.sha256(fixed).hexdigest()==fixture['provenance']['program-fixed.bin']
assert hashlib.sha256(bank).hexdigest()==fixture['provenance']['program-bank.bin']
assert all(c['before']['character']==0 for c in fixture['choices'])
tables=sorted({c['before']['table'] for c in fixture['choices']});roots=set();rows=[]
for table in tables:
 for form in a.forms:
  assert 0<=form<=255;offset=form*16;assert offset<32768
  primary=struct.unpack_from('>I',fixed,table+offset)[0]
  selected=primary or struct.unpack_from('>I',fixed,table)[0]
  assert not selected or 0x200000<=selected<0x300000,(hex(table),form,hex(selected))
  if selected:roots.add(selected)
  rows.append(dict(table=table,character=0,form=form,primary=primary,selected=selected,coverage='STATIC_READ_ONLY'))
a.output.mkdir(parents=True,exist_ok=False)
report=dict(rows=rows,roots=sorted(roots),provenance=fixture['provenance'],limits=['Only tables reached in the recorded input experiments.','Only supplied variant rows, character selector zero; not an inferred table extent.','Nonzero pointers are inspected ROM data, not verified player transitions or complete animation cycles.','No weapon/form semantic labels assigned to the unexecuted rows.'])
(a.output/'inventory.json').write_text(json.dumps(report,indent=2)+'\n');(a.output/'roots.txt').write_text(''.join(f'{root:x}\n' for root in sorted(roots)))
print('STATIC ONLY:',len(rows),'explicit table cells;',len(roots),'distinct program pointers')
