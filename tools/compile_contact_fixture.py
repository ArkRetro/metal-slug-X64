#!/usr/bin/env python3
"""Compile measured contact fixtures and integer coefficient tables."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_contact_fixture.json').read_text())
animation=json.loads((ROOT/'data/soldier_animation_fixture.json').read_text())
steps=[c for c in animation['cases'] if c['kind']=='step']
assert len(steps)==len(f['cases'])
for motion,step in zip(f['cases'],steps):
    a,b=motion['after'],step['before'];assert motion['frame']==b['frame']
    assert (a['projected_x']>>16)&65535==b['x'] and (a['projected_y']>>16)&65535==b['y']
    assert a['correction']==b['correction'] and a['word1e']==b['z'] and a['flags15']==b['base']&255
def array(values):return '{'+','.join(str(v) for v in values)+'}'
def record(value):return array(value.values())
tables=['/* Original numeric tables; provenance in data/soldier_contact_fixture.json. */']
for name,ctype in [('coefficients','int16_t'),('angles','uint16_t'),('groups','uint8_t')]:
    table_name='group_map' if name=='groups' else name
    tables.append('static const '+ctype+' contact_'+table_name+'[]='+array(f['tables'][name])+';')
lines=['/* Captured boundaries, never computed with the replacement solver. */',
    'static const struct { neo_contact_actor_t before,after; uint8_t prepared_flags15; neo_contact_request_t request; neo_contact_result_t result; neo_contact_parent_t parent; uint8_t epoch; uint32_t returned; int32_t scalar,vx,vy; uint8_t angle; } contact_cases[]={']
for c in f['cases']:
    lines.append('{'+','.join([record(c['before']),record(c['after']),str(c['prepared_flags15']),record(c['request']),
        record(c['kernel']),record(c['parent']),str(c['epoch']),str(c['returned_word'])+'u',
        str(c['vector']['scalar']),str(c['vector']['vx']),str(c['vector']['vy']),str(c['vector']['angle'])])+'},')
lines.append('};')
(ROOT/'build').mkdir(exist_ok=True)
(ROOT/'build/contact_tables.h').write_text('\n'.join(tables)+'\n')
(ROOT/'build/contact_fixture.h').write_text('\n'.join(lines)+'\n')
