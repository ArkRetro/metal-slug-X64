#!/usr/bin/env python3
"""Compile measured reposition-state boundaries and offline pose tables."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_locomotion_fixture.json').read_text())
def rec(v):return '{'+','.join('.'+k+'='+str(int(x)) for k,x in v.items())+'}'
def arr(v):return '{'+','.join(str(int(x)) for x in v)+'}'
lines=['/* Independent MAME inputs and expected outputs. */']
for i,p in enumerate(f['programs']):
 lines.append('static const neo_anim_command_t loco_commands_'+str(i)+'[]={')
 for c in p['commands']:lines.append('{NEO_ANIM_'+c['kind']+','+','.join(str(c[k]) for k in ('asset','target','operand','flags','dx','dy'))+'},')
 lines.append('};')
lines+=['static const neo_anim_program_t loco_programs[]={'+','.join('{loco_commands_'+str(i)+','+str(len(p['commands']))+'}' for i,p in enumerate(f['programs']))+'};',
 'static const uint32_t loco_tags[]='+arr([p['root'] for p in f['programs']])+';',
 'static const uint16_t loco_grids[][128]={']
for c in f['cases']:lines.append(arr(c['grid'])+',')
lines+=['};','static const struct { bool init; unsigned program,rng_calls; neo_soldier_state_t before,begun,after; neo_control_t control_before,control_after; neo_contact_actor_t actor_before,actor_body,actor_after; uint16_t speed_before,speed_body,speed_after; uint8_t ready61,epoch; neo_contact_parent_t parent; neo_contact_result_t kernel; neo_soldier_senses_t senses; neo_control_frame_t scene; uint16_t rng_before[55],rng_after[55]; uint8_t cursor_before,cursor_after; neo_anim_render_t render; uint16_t anim_before_cursor,anim_before_timer,anim_after_cursor,anim_after_timer; uint8_t flash,field18; uint32_t field50,after_field50; bool complete; neo_anim_draw_t draw; } loco_cases[]={']
for i,c in enumerate(f['cases']):
 b,a=c['anim_before'],c['anim_after'];palettes=[b['palettes']>>n&255 for n in (24,16,8,0)]
 render='{'+','.join(str(v) for v in (b['x'],b['y'],b['base']&65535,b['z'],b['correction'],b['f16']>>8))+','+arr(palettes)+','+str(int(bool(b['f5f']&8 and b['phase']&1)))+','+str(b['f61'])+'}'
 scene=rec(c['context'])[:-1]+',.delay_grid=loco_grids['+str(i)+']}'
 senses=rec({k:c[k] for k in ('region_code','object_code','object_flags')})[:-1]+',.delay_grid=loco_grids['+str(i)+']}'
 values=[str(int(c['init'])),str(c['program']),str(c['rng_calls'])]+[rec(c[k]) for k in ('before','after_begin','after','control_before','control_after','actor_before','actor_body','actor_after')]+[str(c[k]) for k in ('speed_before','speed_body','speed_after','ready61','epoch')]+[rec(c[k]) for k in ('parent','kernel')]+[senses,scene,arr(c['rng_before']['words']),arr(c['rng_after']['words']),str(c['rng_before']['cursor']),str(c['rng_after']['cursor']),render,str(c['before_cursor']),str(b['timer']),str(c['after_cursor']),str(a['timer']),str(b['f17']),str(b['f18']),str(b['field50']),str(a['field50']),str(int(bool(a['sr']&1))),rec(c['draw'])]
 lines.append('{'+','.join(values)+'},')
lines.append('};');(ROOT/'build/locomotion_fixture.h').write_text('\n'.join(lines)+'\n')
