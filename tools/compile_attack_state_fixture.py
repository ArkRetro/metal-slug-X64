#!/usr/bin/env python3
"""Compile independent main attack-state observations."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_attack_state_fixture.json').read_text())
def rec(v):return '{'+','.join('.'+k+'='+str(int(x)) for k,x in v.items())+'}'
lines=['/* Captured MAME inputs/expected outputs; not production actor data. */','static const struct { neo_soldier_state_t before,begun,after; neo_contact_actor_t actor_before; neo_control_t control_before,control_begun,control_after; struct { neo_contact_actor_t before,after,commit; neo_contact_result_t kernel; neo_contact_parent_t parent; uint8_t epoch; } passes[2]; } attack_state_cases[]={']
for c in f['cases']:
 vals=[rec(c[k]) for k in ('before','begun','after','actor_before','control_before','control_begun','control_after')]
 vals.append('{'+','.join('{'+','.join([rec(p[k]) for k in ('before','after','commit','kernel','parent')]+[str(p['epoch'])])+'}' for p in c['passes'])+'}')
 lines.append('{'+','.join(vals)+'},')
lines.append('};')
hit=json.loads((ROOT/'data/soldier_hit_geometry_fixture.json').read_text())
lines.append('static const struct { neo_hit_rectangle_t query,rectangle; uint16_t target_x,target_y; uint8_t flags; bool hit; uint16_t impact_x,impact_y; } hit_cases[]={')
for c in hit['cases']:
 lines.append('{'+','.join([rec(c['query']),rec(c['rectangle'])]+[str(int(c.get(k,0))) for k in ('target_x','target_y','flags','hit','impact_x','impact_y')])+'},')
lines.append('};');(ROOT/'build/attack_state_fixture.h').write_text('\n'.join(lines)+'\n')
