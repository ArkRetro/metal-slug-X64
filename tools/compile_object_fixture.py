#!/usr/bin/env python3
"""Compile measured throw lifecycle and offline converted animation programs."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_object_fixture.json').read_text())
def rec(v):return '{'+','.join('.'+k+'='+str(int(x)) for k,x in v.items())+'}'
def arr(v):return '{'+','.join(str(int(x)) for x in v)+'}'
lines=['/* Original body/draw/control outputs; not native-generated oracles. */']
for i,p in enumerate(f['programs']):
 lines.append('static const neo_anim_command_t object_commands_'+str(i)+'[]={')
 for c in p['commands']:lines.append('{NEO_ANIM_'+c['kind']+','+','.join(str(c[k]) for k in ('asset','target','operand','flags','dx','dy'))+'},')
 lines.append('};')
lines.append('static const neo_anim_program_t object_programs[]={'+','.join('{object_commands_'+str(i)+','+str(len(p['commands']))+'}' for i,p in enumerate(f['programs']))+'};')
lines+=['static const neo_cull_bounds_t object_bounds='+rec(f['bounds'])+';',
 'static const uint16_t object_flight_rectangle[4]='+arr(f['flight_rectangle'])+';',
 'static const uint16_t object_burst_rectangle[4]='+arr(f['burst_rectangle'])+';',
 'static const uint8_t object_initial_flash='+str(f['initial']['f17'])+';',
 'static const struct { unsigned frame; neo_contact_actor_t body; uint16_t cursor,timer,speed; uint8_t flash,field18; uint32_t field50; neo_anim_render_t render; neo_anim_draw_t draw; uint16_t attributes,video_phase,region_code; uint8_t phase,epoch; bool other_contact,deleted; } object_cases[]={']
for c in f['cases']:
 b,a=c['animation_before'],c['animation_after'];p=[b['palettes']>>n&255 for n in (24,16,8,0)]
 render='{'+','.join(str(v) for v in (b['x'],b['y'],b['base']&65535,b['z'],b['correction'],b['f16']>>8))+','+arr(p)+','+str(int(bool(b['f5f']&8 and b['phase']&1)))+'}'
 lines.append('{'+','.join([str(c['frame']),rec(c['expected']),str(c['after_cursor'])]+[str(a[k]) for k in ('timer','speed','f17','f18','field50')]+[render,rec(c['draw'])]+[str(int(c[k])) for k in ('final_attributes','video_phase','region_code','phase','epoch','other_contact','deleted')])+'},')
lines.append('};');(ROOT/'build/object_fixture.h').write_text('\n'.join(lines)+'\n')
