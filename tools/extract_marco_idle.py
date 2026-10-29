#!/usr/bin/env python3
"""Extract read-only idle decision boundaries before world/animation services."""
import argparse,json,hashlib
from pathlib import Path
from extract_marco_fields import fields
p=argparse.ArgumentParser();p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
active=None;cases=[]
for line in (a.capture/'debug.log').read_text().splitlines():
 if line.startswith('MARCO_IDLE_IN '):
  assert active is None;active=fields(line)
 elif line.startswith('MARCO_IDLE_OUT '):
  assert active is not None;cases.append(dict(before=active,after=fields(line)));active=None
assert cases and active is None
sha=lambda n:hashlib.sha256((a.capture/n).read_bytes()).hexdigest()
a.output.write_text(json.dumps(dict(source='Read-only MAME C1FA6..C2022 decision boundaries',cases=cases,provenance={n:sha(n) for n in ('capture.lua','debug.log','program-fixed.bin')},limits=['No idle world decisions, animation advance or campaign completion covered.']),separators=(',',':'))+'\n')
print('Extracted idle decisions:',len(cases))
