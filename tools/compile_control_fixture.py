#!/usr/bin/env python3
"""Compile captured common-control data without computing replacement outputs."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_control_fixture.json').read_text())
contact=json.loads((ROOT/'data/soldier_contact_fixture.json').read_text())
animation=json.loads((ROOT/'data/soldier_animation_fixture.json').read_text())
assert [c['frame'] for c in f['cases'][1:]]==[c['frame'] for c in contact['cases']]
assert [c['frame'] for c in contact['cases']]==[c['before']['frame'] for c in animation['cases'] if c['kind']=='step']
def arr(v):return '{'+','.join(str(int(x)) for x in v)+'}'
def record(v):return '{'+','.join('.'+k+'='+str(int(x)) for k,x in v.items())+'}'
tables=['/* Numeric ROM tables; provenance in soldier_control_fixture.json. */']
for name,typ in [('rank_lookup','uint8_t'),('probability','uint16_t')]:
    tables.append('static const '+typ+' control_'+name+'[]='+arr(f['tables'][name])+';')
tables.append('static const uint8_t control_probability_mask='+str(f['tables']['probability_mask'])+';')
lines=['/* Independent measured inputs/outputs. */',
    'static const uint16_t control_initial_words[55]='+arr(f['initial_words'])+';',
    'static const uint8_t control_initial_cursor='+str(f['initial_cursor'])+';',
    'static const struct { uint16_t value,mixed,sr; uint8_t cursor; int8_t changed_index; uint16_t changed_value; } control_rng[]={']
for s in f['rng']:
    c=s['changes'][0] if s['changes'] else dict(index=-1,value=0)
    lines.append(arr([s['value'],s['mixed'],s['sr'],s['cursor'],c['index'],c['value']])+',')
lines+=['};','static const uint16_t control_grids[][128]={']
for c in f['cases']:lines.append(arr(c['grid'])+',')
lines+=['};','static const struct { neo_control_t before,target,prefix,after; neo_control_frame_t frame; unsigned start,end; uint16_t threshold; uint32_t probe1,probe2; } control_cases[]={']
for i,c in enumerate(f['cases']):
    context=record(c['context'])[:-1]+',.delay_grid=control_grids['+str(i)+']}'
    lines.append('{'+','.join([record(c[k]) for k in ('before','after_target','after_prefix','after')])+','+context+','+
        ','.join(str(c[k])+('u' if k in ('probe1','probe2') else '') for k in ('rng_before','rng_after','threshold','probe1','probe2'))+'},')
lines+=['};','#define CONTROL_SIGN_DIFFERENCES '+str(f['returned_sign_differs_from_final_xor'])]
(ROOT/'build').mkdir(exist_ok=True)
(ROOT/'build/control_tables.h').write_text('\n'.join(tables)+'\n')
(ROOT/'build/control_fixture.h').write_text('\n'.join(lines)+'\n')
