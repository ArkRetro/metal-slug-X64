#!/usr/bin/env python3
"""+72/+74 fixture (src/marco_legs.c) from the Marco catalog capture
(tools/mame/run_marco_catalog.py --paired-trace, e.g. marco-cat1): for the
named input cases, every body program step (MARCO_STEP_OUT of 00427A on
the body state 1014E4) with the frame, the program cursor (ROM address,
provenance), +78 and +72/+74 after the step, the ground flag (+15 bit 6) of
the frame, the body's descriptor drawn in that step (MARCO_DRAW) and
whether the case moves. Capture RAM only.
Usage: extract_legs_fixture.py CAPTURE OUT.json CASE..."""
import json,re,sys
from pathlib import Path
d=Path(sys.argv[1]);plan={c['name']:c for c in json.loads((d/'plan.json').read_text())['cases']}
states={r['frame']:r for r in map(json.loads,(d/'states.jsonl').read_text().splitlines())}
steps=[(int(f),int(c,16),int(v78,16),int(v72,16)) for f,c,v72,v78 in re.findall(r'MARCO_STEP_OUT frame=(\d+) state=001014E4 cursor=([0-9A-F]+) .*? f72=([0-9A-F]+) f76=\w+ f77=\w+ f78=([0-9A-F]+)',(d/'debug.log').read_text())]
drawn={}
for f,desc in re.findall(r'MARCO_DRAW frame=(\d+) descriptor=([0-9A-F]+)',(d/'debug.log').read_text()):drawn.setdefault(int(f),int(desc,16))
cases=[]
for name in sys.argv[3:]:
    c=plan[name];moving=any(b in c['inputs'] for b in ('Left','Right'))
    rows=[[f,cur,v78,v72,(states[f]['flags']>>6)&1,drawn.get(f,0)] for f,cur,v78,v72 in steps if c['first']<=f<=c['last']]
    cases.append(dict(name=name,moving=moving,jump='jump' in c['inputs'],steps=rows))
json.dump(dict(source=d.name,cases=cases),open(sys.argv[2],'w'),separators=(',',':'))
print(sum(len(c['steps']) for c in cases),'body steps ->',sys.argv[2])
