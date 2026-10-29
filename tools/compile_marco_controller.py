#!/usr/bin/env python3
"""Compile bounded controller fixtures, optionally using private converted scripts."""
import argparse,json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
def compile_controller(fixture,output,graph_path=None):
 data=json.loads(fixture.read_text());roots=sorted({c['choice']['program'] for c in data['choices'] if c['choice']['program']})
 native=None
 if graph_path:
  graph=json.loads(graph_path.read_text());native={c['address']:i for i,c in enumerate(graph['commands'])}
  assert all(root in native for root in roots)
 lines=['/* Measured controller boundary inputs and outputs, not ROM scripts. */']
 if native is None:
  lines+=['static const neo_anim_command_t controller_stub_commands[]={{.kind=NEO_ANIM_FINISH}};',
   'static const neo_anim_program_t controller_stub_programs[]={']+['{controller_stub_commands,1},' for _ in roots]+['};']
 else:lines+=['#include "marco_script.h"']
 lines+=['static const marco_program_slot_t controller_roots[]={']
 for i,root in enumerate(roots):
  ref='&marco_script' if native is not None else '&controller_stub_programs['+str(i)+']'
  lines.append('{'+ref+','+str(native[root] if native is not None else 0)+',MARCO_PROGRAM_READY},')
 lines+=['};','static const struct {marco_input_state_t state;marco_input_gate_t gate;uint32_t packed,result;uint8_t flags5f,flags9a,accumulator9b;} controller_inputs[]={']
 arr=lambda xs:'{'+','.join(str(x) for x in xs)+'}'
 for c in data['inputs']:
  b=c['before'];a=c['after']
  lines.append('{'+arr([b[k] for k in ('attr','f5f','f9a','f9b','f9c')])+','+arr([b[k] for k in ('mode','current','limit')])+','+','.join(str(v) for v in (b['packed'],a['result'],a['f5f'],a['f9a'],a['f9b']))+'},')
 lines+=['};','static const struct {uint16_t character,slot;uint8_t part,form,flash,field18;uint32_t field50;uint8_t after18;} controller_choices[]={']
 for c in data['choices']:
  b=c['before'];assert c['primary'] and c['requested_offset']==0,'Expand compiler for observed nonzero indices/fallback cases'
  lines.append(arr([b['character'],roots.index(c['choice']['program']),b['part'],b['form'],b['flash'],b['f18'],b['field50'],c['after']['f18']])+',')
 lines+=['};'];output.parent.mkdir(parents=True,exist_ok=True);output.write_text('\n'.join(lines)+'\n')
 print('Compiled',len(data['inputs']),'input calls and',len(data['choices']),'selections;',len(roots),'native root references' if native is not None else 'distinct stub identities (not animation content)')
if __name__=='__main__':
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--fixture',type=Path,default=ROOT/'data/marco_controller_fixture.json');p.add_argument('--output',type=Path,default=ROOT/'build/marco_controller_fixture.h');p.add_argument('--graph',type=Path);a=p.parse_args();compile_controller(a.fixture,a.output,a.graph)
