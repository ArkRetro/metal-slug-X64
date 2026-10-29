#!/usr/bin/env python3
"""Compile measured numerical fixtures without evaluating a replacement RNG."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_rng_fixture.json').read_text())
def array(values):return '{'+','.join(str(v) for v in values)+'}'
lines=['/* Generated from measured MAME probes, not C model outputs. */',
       'static const uint16_t rng_initial_words[55]='+array(f['initial_words'])+';',
       'static const uint8_t rng_initial_cursor='+str(f['initial_cursor'])+';',
       'static const struct { uint16_t request,counter_before,counter_after,d0; uint8_t cursor; uint16_t words[55]; } rng_seeds[]={']
for s in f['seeds']:
    lines.append('{'+','.join(str(s[k]) for k in ('request','counter_before','counter_after','expected_d0','cursor'))+','+array(s['words'])+'},')
lines+=['};','static const struct { uint16_t value; uint8_t cursor; int8_t index; uint16_t changed; } rng_steps[]={']
for s in f['steps']:
    assert len(s['changes'])<=1
    change=s['changes'][0] if s['changes'] else dict(index=-1,value=0)
    lines.append(array([s['expected_value'],s['cursor'],change['index'],change['value']])+',')
lines+=['};','static const struct { unsigned step; uint8_t flags15; uint16_t field92; int32_t expected62; uint16_t expected26; } rng_initializers[]={']
for s in f['initializers']:
    lines.append(array([s[k] for k in ('step','flags15','field92','expected62','expected26')])+',')
lines+=['};','static const struct { uint32_t before,after; neo_soldier_ground_inputs_t input; } soldier_decisions[]={']
for s in f['decisions']:
    lines.append('{'+str(s['routine_before'])+','+str(s['expected_routine'])+','+array([s[k] for k in ('d6','field78','field84','field3c','field62','field7c','field6c')])+'},')
lines+=['};','static const struct { int32_t before,after; } soldier_damping[]={']
for s in f['damping']:lines.append(array([s['value_before'],s['value_after']])+',')
lines+=['};']
(ROOT/'build').mkdir(exist_ok=True)
(ROOT/'build/rng_fixture.h').write_text('\n'.join(lines)+'\n')
