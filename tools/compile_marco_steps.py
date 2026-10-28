#!/usr/bin/env python3
"""Compile private MAME helper journals; never check script/ROM data into Git.

Depth is a supplied draw input: this capture did not probe actor+1E. These tests
verify propagation of that input, not its original derivation by the controller.
"""
import argparse,json,hashlib
from pathlib import Path
KEYS=('cursor','timer','speed','f17','f18','field50','f72','f76','f77','f78','f79','x','y','cached_x','cached_y','attr')
def compile_steps(fixture,graph_path,output):
 data=json.loads(fixture.read_text());graph=json.loads(graph_path.read_text())
 assert data['provenance']['debug.log']==graph['provenance']['debug.log']
 indices={c['address']:i for i,c in enumerate(graph['commands'])}
 events={e['site']:e['native_id'] for e in graph['events']}
 arr=lambda values:'{'+','.join(str(v) for v in values)+'}'
 lines=['/* Private measured helper boundaries. See compile_marco_steps.py. */',
 'enum {'+','.join('P_'+k.upper() for k in KEYS)+',P_COUNT};',
 'typedef struct {uint32_t before[P_COUNT],after[P_COUNT]; neo_anim_render_t render; unsigned part,complete,draw_first,draw_count,event_first,event_count;} pair_case_t;',
 'typedef struct {uint32_t id;uint16_t x,y,attributes;} pair_event_t;']
 draws=[];requests=[];rows=[]
 for c in data['cases']:
  def snap(s):
   return arr([indices[s[k]] if k=='cursor' and s[k] else s[k] for k in KEYS])
  b=c['before'];z=c['draws'][0]['z'] if c['draws'] else 0
  assert all(d['z']==z for d in c['draws'])
  render='{'+','.join(str(b[k]) for k in ('x','y','attr'))+','+str(z)+','+str(b['correction'])+','+str(b['flash_palette'])+','+arr([b['palettes']>>s&255 for s in (24,16,8,0)])+','+str(int(bool(b['flags5f']&8 and b['phase']&1)))+',0}'
  rows.append('{'+snap(b)+','+snap(c['after'])+','+render+','+','.join(str(v) for v in (int(c['weapon']),int(c['complete']),len(draws),len(c['draws']),len(requests),len(c['events'])))+'},')
  draws+=c['draws'];requests += [dict(e,id=events[e['site']]) for e in c['events']]
 lines+=['static const neo_anim_draw_t pair_draws[]={']+[arr([d[k] for k in ('descriptor','x','y','attributes','z')])+',' for d in draws]+['};','static const pair_event_t pair_events[]={']+[arr([e[k] for k in ('id','x','y','attributes')])+',' for e in requests]+['};','static const pair_case_t pair_cases[]={']+rows+['};']
 output.parent.mkdir(parents=True,exist_ok=True);output.write_text('\n'.join(lines)+'\n')
 print('Compiled',len(rows),'helper calls,',len(draws),'draws,',len(requests),'requests')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('fixture',type=Path);p.add_argument('graph',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();compile_steps(a.fixture,a.graph,a.output)
