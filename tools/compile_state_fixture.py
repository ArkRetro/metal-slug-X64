#!/usr/bin/env python3
"""Compile measured state boundaries, keeping expected outputs independent."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_state_fixture.json').read_text())
control=json.loads((ROOT/'data/soldier_control_fixture.json').read_text())
animation=json.loads((ROOT/'data/soldier_animation_fixture.json').read_text())
assert [c['frame'] for c in f['cases']]==[c['frame'] for c in control['cases'][1:]]
def rec(v):return '{'+','.join('.'+k+'='+str(x) for k,x in v.items())+'}'
lines=['/* Expected outputs are actual MAME probe values. */',
    'static const struct { neo_soldier_state_t before,after; int32_t scalar_before,scalar_motion,scalar_after; uint16_t speed_before,speed_motion,speed_after; unsigned init_count,rng_calls; neo_soldier_senses_t senses; } state_cases[]={']
for i,c in enumerate(f['cases']):
    before,after=c['before'],c['after'];states=[{k:r[k] for k in ('routine','field78','field7e','field81')} for r in (before,after)]
    senses=rec(c['senses'])[:-1]+',.delay_grid=control_grids['+str(i)+']}' # pre-common grid, previous cycle.
    lines.append('{'+','.join([rec(s) for s in states]+[str(r[k]) for k in ('scalar62',) for r in (before,c['before_motion'],after)]+[str(r['speed']) for r in (before,c['before_motion'],after)]+[str(c['init_count']),str(c['rng_calls']),senses])+'},')
lines+=['};','static const uint32_t state_program_tags[]={'+','.join(str(p['root']) for p in animation['programs'])+'};']
(ROOT/'build/state_fixture.h').write_text('\n'.join(lines)+'\n')

(ROOT/'build/state_tables.h').write_text('/* Numeric ROM data, addresses in SOLDIER_STATE.md. */\n#ifdef NEO_STATE_RECT_TABLE\nstatic const int16_t state_action_rect[]={'+','.join(str(x) for x in f['tables']['action_rect'])+'};\n#endif\n#ifdef NEO_STATE_ROUTINE_TABLE\nstatic const uint32_t state_action_routines[]={'+','.join(str(x) for x in f['tables']['action_routines'])+'};\n#endif\n')
