#!/usr/bin/env python3
"""Compile captured helper inputs/outputs and offline-converted animation data."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_animation_fixture.json').read_text())
def arr(values):return '{'+','.join(str(v) for v in values)+'}'
lines=['/* Captured data; expected outputs do not come from the native engine. */',
    '#define ANIM_EXTERNAL_SPEED_WRITES '+str(len(f['external_speed_writes']))]
for i,p in enumerate(f['programs']):
    lines.append(f'static const neo_anim_command_t anim_commands_{i}[]={{')
    for c in p['commands']:
        lines.append('{NEO_ANIM_'+c['kind']+','+','.join(str(c[k]) for k in ('asset','target','operand','flags','dx','dy'))+'},')
    lines.append('};')
lines.append('static const neo_anim_program_t anim_programs[]={')
for i,p in enumerate(f['programs']):lines.append('{anim_commands_'+str(i)+','+str(len(p['commands']))+'},')
lines.append('};')
draws=[d for c in f['cases'] for d in c.get('draws',[])]
lines.append('static const neo_anim_draw_t anim_expected_draws[]={')
for d in draws:lines.append(arr([d[k] for k in ('descriptor','x','y','attributes','z')])+',')
lines+=['};','typedef struct { uint16_t cursor,timer,speed; uint8_t flash,field18; uint32_t field50; } anim_fixture_state_t;',
    'static const struct { unsigned program; bool init; uint16_t requested; anim_fixture_state_t before,after; neo_anim_render_t render; uint16_t after_attr; bool complete; unsigned draw_start,draw_count; } anim_cases[]={']
draw_start=0
for c in f['cases']:
    b,a=c['before'],c['after'];init=c['kind']=='init'
    def state(row,cursor):return arr([cursor,row['timer'],row['speed'],row['f17'],row['f18'],row['field50']])
    if init:render='{0}';afterattr=0;complete=0
    else:
        palettes=[(b['palettes']>>shift)&255 for shift in (24,16,8,0)]
        render='{'+','.join(str(v) for v in [b['x'],b['y'],b['base']&65535,b['z'],b['correction'],b['f16']>>8])+','+arr(palettes)+','+str(int(bool(b['f5f']&8 and b['phase']&1)))+',0}'
        afterattr=c['calls'][-1]['after'] if c['calls'] else b['base']&65535
        complete=a['sr']&1
    n=len(c.get('draws',[]))
    lines.append('{'+','.join([str(c['program']),str(int(init)),str(b.get('requested_speed',0)),state(b,c['before_cursor']),state(a,c['after_cursor']),render,str(afterattr),str(complete),str(draw_start),str(n)])+'},')
    draw_start+=n
lines.append('};')
(ROOT/'build').mkdir(exist_ok=True);(ROOT/'build/animation_fixture.h').write_text('\n'.join(lines)+'\n')
