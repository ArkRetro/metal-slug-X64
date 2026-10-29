#!/usr/bin/env python3
"""Compile independent MAME weighted-action fixtures for host/MIPS tests."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_action_fixture.json').read_text())
def rec(v):return '{'+','.join('.'+k+'='+str(x) for k,x in v.items())+'}'
def arr(v):return '{'+','.join(str(x) for x in v)+'}'
def signed(v):return v if v<0x80000000 else v-0x100000000
lines=['/* Measured inputs and outputs; no replacement decision logic here. */',
    'static const struct { neo_control_t before,after; neo_contact_actor_t actor; neo_control_frame_t scene; uint8_t mask,result,cursor_before,cursor_after; uint16_t words_before[55],words_after[55]; } action_cases[]={']
for c in f['cases']:
    a,b=c['before'],c['after']
    state=lambda r:rec(dict(flags7c=r['f7c'],selected80=r['f80'],timer70=r['f70'],delay92=r['f92'],target8e=r['f8e'],target90=r['f90']))
    actor=rec(dict(projected_x=signed(a['px']),projected_y=signed(a['py']),flags15=a['f15']))
    scene=rec({k:a[k] for k in ('p1x','p1y','p2x','p2y') }|dict(active_mask=a['active'],origin_x=a['originx'],origin_y=a['originy']))
    lines.append('{'+','.join([state(a),state(b),actor,scene,str(a['f81']),str(b['result']),str(a['cursor']),str(b['cursor']),arr(a['words']),arr(b['words'])])+'},')
lines.append('};');(ROOT/'build/action_fixture.h').write_text('\n'.join(lines)+'\n')
