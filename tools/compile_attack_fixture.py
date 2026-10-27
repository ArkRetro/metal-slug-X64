#!/usr/bin/env python3
"""Compile measured attack animation/effect boundaries for native replay."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/soldier_attack_fixture.json').read_text())
t=json.loads((ROOT/'data/soldier_throw_fixture.json').read_text())
def arr(v):return '{'+','.join(str(int(x)) for x in v)+'}'
def state(r,cursor):return arr([cursor,r['timer'],r['speed'],r['f17'],r['f18'],r['field50']])
lines=['/* Independent original animation outputs. */','static const unsigned attack_first_frame='+str(f['cases'][1]['before']['frame'])+';' ,'static const neo_anim_command_t attack_commands[]={']
for c in f['program']['commands']:lines.append('{NEO_ANIM_'+c['kind']+','+','.join(str(c[k]) for k in ('asset','target','operand','flags','dx','dy'))+'},')
lines+=['};','static const neo_anim_program_t attack_program={attack_commands,'+str(len(f['program']['commands']))+'};',
    'static const uint8_t attack_palette_handle='+str(f['palette_handle'])+';',
    'static const struct { uint16_t cursor,timer,speed; uint8_t flash,field18; uint32_t field50; } attack_initial='+state(f['cases'][0]['before'],0)+';',
    'static const neo_anim_draw_t attack_draws[]={']
for c in f['cases'][1:]:
 for d in c['draws']:lines.append(arr([d[k] for k in ('descriptor','x','y','attributes','z')])+',')
lines+=['};','static const struct { uint16_t cursor,timer,speed; uint8_t flash,field18; uint32_t field50; neo_anim_render_t render; bool complete; unsigned events; } attack_cases[]={']
for c in f['cases'][1:]:
 b,a=c['before'],c['after'];palettes=[(b['palettes']>>n)&255 for n in (24,16,8,0)]
 render='{'+','.join(str(v) for v in (b['x'],b['y'],b['base']&65535,b['z'],b['correction'],b['f16']>>8))+','+arr(palettes)+','+str(int(bool(b['f5f']&8 and b['phase']&1)))+'}'
 lines.append(state(a,c['after_cursor'])[:-1]+','+render+','+str(int(bool(a['sr']&1)))+','+str(len(c['calls']))+'},')
 for call in c['calls']:
  if call['before']['target']==0x3bb8e:
   original=call['before'];assert original['frame']==t['spawn']['frame']
   for k,source in [('parent_x','projected_x'),('parent_y','projected_y'),('oldx','cached_x'),('oldy','cached_y')]:assert original[k]==t['spawn']['source'][source]%(1<<32)
lines.append('};');(ROOT/'build/attack_fixture.h').write_text('\n'.join(lines)+'\n')
