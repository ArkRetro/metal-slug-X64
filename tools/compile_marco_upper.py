#!/usr/bin/env python3
"""Compile bounded controller service fixtures; no original scripts are required."""
import argparse,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
MODES={0:'MUP_DISABLED',0xc381c:'MUP_RESET',0xc38b2:'MUP_SETTLE',0xc397e:'MUP_POSE0',0xc3a5e:'MUP_ACTIVE',0xc3b82:'MUP_MELEE_WAIT',0xc3bcc:'MUP_GRENADE',0xc3cd0:'MUP_FIRE',0xc3d9a:'MUP_FIRE_WAIT',0xc3e66:'MUP_REPEAT_A_WAIT',0xc3f1a:'MUP_REPEAT_B_WAIT',0xc3ffa:'MUP_TURN_WAIT',0xc3fa6:'MUP_TURN_DOWN',0xc3fb4:'MUP_TURN_UP',0xc3fc2:'MUP_TURN_FROM_UP',0xc3fd0:'MUP_TURN_FROM_DOWN',0xc4176:'MUP_AIM_WAIT'}
ANIM=('cursor','timer','speed','flash','f18','field50','f72','f76','f77','f78','f79')
# Lower helper probes use f17; upper entry probes use flash.
def scalar(r,k):return r['f17'] if k=='flash' else r[k]
def compile_fixture(fixture,output):
 data=json.loads(fixture.read_text());columns=data['columns'];metadata=data['metadata'];operations=[];cases=[];draws=[];events=[]
 arr=lambda values:'{'+','.join(str(v)+'u' for v in values)+'}'
 for c in data['cases']:
  def values(row):return '{'+','.join(MODES[v] if k=='mode' else str(v)+'u' for k,v in zip(columns,row))+'}'
  cases.append('{'+str(int(c['kind']=='reset'))+','+values(c['before'])+','+values(c['after'])+','+str(len(operations))+','+str(len(c['operations']))+'},')
  for op in c['operations']:
   kind={'select':0,'available':1,'random':2,'animate':3,'latch':4}[op['kind']];s='{.kind='+str(kind)
   if kind==0:s+=',.table='+str(op['table'])+'u,.direct='+str(int(op['direct']))+',.program='+str(op['program'])+'u'
   elif kind==1:s+=',.table='+str(op['table'])+'u,.value='+str(int(op['value']))
   elif kind==2:s+=',.value='+str(op['value'])+'u'
   elif kind==3:
    s+=',.complete='+str(int(op['complete']))+',.before='+arr([scalar(op['before'],k) for k in ANIM])+',.after='+arr([scalar(op['after'],k) for k in ANIM])
    s+=',.spawn_mask='+str(sum(int(v)<<i for i,v in enumerate(op['allocations'])))+'u,.spawn_count='+str(len(op['allocations']))+',.grenades='+str(op['grenades'])
    s+=',.draw_first='+str(len(draws))+',.draw_count='+str(len(op['draws']))+',.event_first='+str(len(events))+',.event_count='+str(len(op['events']))
    draws+=op['draws'];events+=op['events']
   else:s+=',.word='+str(op['before'])+'u,.mask='+str(op['mask'])+'u'
   operations.append(s+'},')
 lines=['/* Measured upper-controller calls. Animation/allocator services are replayed. */','enum {'+','.join('U_'+k.upper() for k in columns)+',U_COUNT};','enum {'+','.join('A_'+k.upper() for k in ANIM)+',A_COUNT};',
 'typedef struct {unsigned kind;uint32_t table,program,value,word,mask;bool direct,complete;uint32_t before[A_COUNT],after[A_COUNT],spawn_mask;unsigned spawn_count,grenades,draw_first,draw_count,event_first,event_count;} upper_op_t;',
 'static const uint16_t upper_compatibility[]='+arr(metadata['compatibility'])+';',
 'static const uint8_t upper_combo_limits[]='+arr(metadata['combo_limits'])+';',
 'static const uint32_t upper_table_tags[]='+arr(metadata['table_tags'])+';',
 'static const uint32_t upper_fire_tables[3][16]={'+','.join(arr(row) for row in metadata['fire_tables'])+'};',
 'static const neo_anim_draw_t upper_draws[]={']+[arr([d[k] for k in ('descriptor','x','y','attributes','z')])+',' for d in draws]+['};',
 'static const struct {uint32_t target,site,spawn_mask;unsigned spawn_count;} upper_events[]={']+[arr([e['target'],e['site'] or 0,sum(int(v)<<i for i,v in enumerate(e['allocations'])),len(e['allocations'])])+',' for e in events]+['};',
 'static const upper_op_t upper_operations[]={']+operations+['};','static const struct {bool reset;uint32_t before[U_COUNT],after[U_COUNT];unsigned first,count;} upper_cases[]={']+cases+['};']
 output.parent.mkdir(parents=True,exist_ok=True);output.write_text('\n'.join(lines)+'\n');print('Compiled',len(cases),'upper-controller calls and',len(operations),'observed service operations')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--fixture',type=Path,default=ROOT/'data/marco_upper_fixture.json');p.add_argument('--output',type=Path,default=ROOT/'build/marco_upper_fixture.h');a=p.parse_args();compile_fixture(a.fixture,a.output)
