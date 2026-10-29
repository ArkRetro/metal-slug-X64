#!/usr/bin/env python3
"""Extract read-only executed C0D64 ammunition service boundaries."""
import argparse,hashlib,json,re
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
a=p.parse_args();cases=[];pending=None
for line in (a.capture/'debug.log').read_text().splitlines():
    match=re.search(r'MARCO_AMMO_(IN|OUT) frame=(\d+) (.*)',line)
    if not match:continue
    row={k:int(v,16) for k,v in re.findall(r'(\w+)=([0-9A-Fa-f]+)',match[3])}
    assert row['base']==0x108000
    if match[1]=='IN':
        assert pending is None
        pending=row
    else:
        assert pending is not None and pending['slot']==row['slot']
        assert all(pending[k]==row[k] for k in ('cap','kind','cheat','special'))
        cases.append(dict(before=pending,after=row));pending=None
assert cases and pending is None
sha=lambda name:hashlib.sha256((a.capture/name).read_bytes()).hexdigest()
result=dict(source='Executed MAME C0D64..C0DE2; read-only probes, scheduled controller inputs',
    cases=cases,provenance={n:sha(n) for n in ('capture.lua','debug.log','program-fixed.bin','plan.json')},
    limits=['Only observed weapon kinds/branches are dynamically verified.','This gate alone does not implement projectile allocation, release, weapon swapping, damage or audio.'])
a.output.write_text(json.dumps(result,indent=2)+'\n')
print('Extracted ammunition calls:',len(cases))
