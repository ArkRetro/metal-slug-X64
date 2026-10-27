#!/usr/bin/env python3
"""Compile exact removal-predicate fixture inputs/outputs for host and MIPS."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_cull_fixture.json').read_text())
control=json.loads((ROOT/'data/soldier_control_fixture.json').read_text())
assert [c['frame'] for c in f['cases'][:54]]==[c['frame'] for c in control['cases'][1:]]
def rec(v):return '{'+','.join('.'+k+'='+str(int(x)) for k,x in v.items())+'}'
lines=['/* Original measured scene/actor inputs and returned carry. */',
    'static const struct { neo_contact_actor_t actor; neo_cull_bounds_t bounds; uint16_t rectangle[4]; bool outside; } cull_cases[]={']
for c in f['cases']:lines.append('{'+rec(c['actor'])+','+rec(c['bounds'])+',{'+','.join(str(x) for x in c['rectangle'])+'},'+str(int(c['outside']))+'},')
lines.append('};');(ROOT/'build/cull_fixture.h').write_text('\n'.join(lines)+'\n')
