#!/usr/bin/env python3
"""Compile numerical observed body-controller boundaries, without ROM assets."""
import argparse,json
from pathlib import Path
from compile_marco_upper import MODES as UPPER_MODES,ANIM,scalar
ROOT=Path(__file__).resolve().parents[1]
MODES=dict(zip((0xc2214,0xc2236,0xc2294,0xc229c,0xc22a4,0xc22b6,0xc2338,0xc2368,0xc23ca,0xc23f2,0xc2458,0xc246a,0xc2588,0xc1e34,0xc2654,0xc2706,0xc271c),('MCR_ENTER','MCR_ENTER_WAIT','MCR_AFTER_GRENADE','MCR_AFTER_FIRE','MCR_IDLE_START','MCR_IDLE','MCR_TURN_START','MCR_TURN','MCR_MOVE_START','MCR_MOVE','MCR_GRENADE','MCR_FIRE','MCR_ATTACK','MCR_STAND_REQUEST','MCR_JUMP_REQUEST','MCR_FALL_ACTION_REQUEST','MCR_FALL_REQUEST')))
def compile_fixture(fixture,output,profile="crouch",modes=None):
 modes=MODES if modes is None else modes
 data=json.loads(fixture.read_text());columns=[k for k in data['columns'] if k!='mode'];ops=[];cases=[];draws=[];events=[]
 arr=lambda v:'{'+','.join(str(x)+'u' for x in v)+'}'
 def row(v):
  r=dict(zip(data['columns'],v))
  return '{'+','.join(UPPER_MODES[r[k]] if k=='upper_mode' else str(r[k])+'u' for k in columns)+'}'
 def full(r):return row([r[k] for k in data['columns']])
 for c in data['cases']:
  after=dict(zip(data['columns'],c['after']));target=c['exit'] if c['exit']!=0xc32f4 else after['mode']
  cases.append('{'+modes[c['entry']]+','+modes[target]+','+row(c['before'])+','+row(c['after'])+','+str(len(ops))+','+str(len(c['operations']))+','+str(int(c['exit']!=0xc32f4))+'},')
  for op in c['operations']:
   kind={'select':0,'move':1,'animate':2}[op['kind']];s='{.kind='+str(kind)
   if kind==0:s+=',.table='+str(op['table'])+'u,.program='+str(op['program'])+'u'
   elif kind==1:s+=',.service='+str(op.get('service',0))+',.before='+full(op['before'])+',.after='+full(op['after'])
   else:
    b=op['before'];pal=b['palettes']
    s+=',.view={.render={.x='+str(b['x'])+'u,.y='+str(b['y'])+'u,.attributes='+str(b['attr'])+'u,.z='+str(op.get('view_z') or 0)+'u,.correction='+str(b['correction'])+'u,.flash_palette='+str(b['flash_palette'])+'u,.palettes='+arr([(pal>>shift)&255 for shift in (24,16,8,0)])+',.suppress='+str(int(bool((b['flags5f']&8) and (b['phase']&1))))+'},.cached_x='+str(b['cached_x'])+'u,.cached_y='+str(b['cached_y'])+'u}'
    s+=',.complete='+str(int(op['complete']))+',.anim_before='+arr([scalar(op['before'],k) for k in ANIM])+',.anim_after='+arr([scalar(op['after'],k) for k in ANIM])
    s+=',.draw_first='+str(len(draws))+',.draw_count='+str(len(op['draws']))+',.event_first='+str(len(events))+',.event_count='+str(len(op['events']))
    draws+=op['draws'];events+=op['events']
   ops.append(s+'},')
 lines=['/* Measured crouch blocks; world/animation service boundaries remain explicit. */','enum {'+','.join('C_'+k.upper() for k in columns)+',C_COUNT};','enum {'+','.join('A_'+k.upper() for k in ANIM)+',A_COUNT};',
 'static const uint32_t crouch_table_tags[]='+arr(data['metadata']['table_tags'])+';',
 'static const uint32_t crouch_fire_tables[]='+arr(data['metadata']['fire_tables'])+';',
 'static const neo_anim_draw_t crouch_draws[]={']+[arr([d[k] for k in ('descriptor','x','y','attributes','z')])+',' for d in draws]+([] if draws else ['{0},'])+['};',
 'static const struct {uint32_t target,site,spawn_mask;unsigned spawn_count;} crouch_events[]={']+[arr([e['target'],e['site'] or 0,sum(int(v)<<i for i,v in enumerate(e['allocations'])),len(e['allocations'])])+',' for e in events]+([] if events else ['{0},'])+['};',
 'typedef struct {unsigned kind,service;marco_animation_view_t view;uint32_t table,program,before[C_COUNT],after[C_COUNT],anim_before[A_COUNT],anim_after[A_COUNT];bool complete;unsigned draw_first,draw_count,event_first,event_count;} crouch_op_t;',
 'static const crouch_op_t crouch_operations[]={']+ops+['};',
 'static const struct {marco_crouch_mode_t entry,target;uint32_t before[C_COUNT],after[C_COUNT];unsigned first,count;bool tail_call;} crouch_cases[]={']+cases+['};']
 output.parent.mkdir(parents=True,exist_ok=True);text='\n'.join(lines)+'\n'
 if profile=='motion':text=text.replace('crouch','motion')
 output.write_text(text);print('Compiled',len(cases),profile+' blocks;',len(ops),'service calls')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--fixture',type=Path,default=ROOT/'data/marco_crouch_fixture.json');p.add_argument('--output',type=Path,default=ROOT/'build/marco_crouch_fixture.h');a=p.parse_args();compile_fixture(a.fixture,a.output)
