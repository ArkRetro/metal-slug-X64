#!/usr/bin/env python3
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1];f=json.loads((ROOT/'data/marco_selector_fixture.json').read_text())
lines=['/* Original pre-selector inputs and executed draws. */']
for i,c in enumerate(f['cases']):
 lines.append(f'static const neo_anim_command_t marco_select_commands_{i}[]={{')
 lines.append('{NEO_ANIM_SELECT_FIELD_POSE,'+str(c['field'])+',1,'+str(len(c['poses']))+',0,0,0},')
 for pose in c['poses']:lines.append('{NEO_ANIM_POSE,'+str(pose['asset'])+',0,0,'+','.join(str(pose[k]) for k in ('flags','dx','dy'))+'},')
 lines.extend(['{NEO_ANIM_FINISH,0,0,0,0,0,0}','};'])
lines.append('static const struct {neo_anim_program_t program;uint8_t value,flash;uint16_t field,timer;neo_anim_render_t render;unsigned count;neo_anim_draw_t draw;} marco_select_cases[]={')
arr=lambda v:'{'+','.join(str(x) for x in v)+'}'
for i,c in enumerate(f['cases']):
 r=c['render'];render='{'+','.join(str(r[k]) for k in ('x','y','attributes','z','correction','flash_palette'))+','+arr(r['palettes'])+',false,0}'
 d=c['draws'][0] if c['draws'] else dict(descriptor=0,x=0,y=0,attributes=0,z=0)
 lines.append('{{marco_select_commands_'+str(i)+','+str(len(c['poses'])+2)+'},'+','.join(str(c[k]) for k in ('value','flash','field','timer'))+','+render+','+str(len(c['draws']))+','+arr([d[k] for k in ('descriptor','x','y','attributes','z')])+'},')
lines.append('};');(ROOT/'build').mkdir(exist_ok=True);(ROOT/'build/marco_selectors.h').write_text('\n'.join(lines)+'\n')
