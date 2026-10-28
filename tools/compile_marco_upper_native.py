#!/usr/bin/env python3
"""Bind the upper controller's table keys to real converted native programs."""
import argparse,json,struct,shutil
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--part',choices=('upper','crouch'),default='upper');p.add_argument('capture',type=Path);p.add_argument('graph',type=Path);p.add_argument('--fixture',type=Path,default=ROOT/'data/marco_upper_fixture.json');p.add_argument('--output',type=Path,required=True);a=p.parse_args()
d=json.loads(a.fixture.read_text());g=json.loads(a.graph.read_text());assert d['provenance']['debug.log']==g['provenance']['debug.log']
if a.part=='crouch':assert all(op.get('view_z') is not None for c in d['cases'] for op in c['operations'] if op['kind']=='animate'),'Native drawing requires independently captured Z'
rom=(a.capture/'program-fixed.bin').read_bytes();indices={c['address']:i for i,c in enumerate(g['commands'])};tags={}
for c in d['cases']:
 for op in c['operations']:
  if op['kind']=='select':tags[op['table']]=not op.get('direct',False) or tags.get(op['table'],False)
  if op['kind']=='available':tags[op['table']]=True
lines=['/* Private native table bindings, never original CPU code pointers. */','#include "marco_script.h"',
 'static const uint32_t upper_source_commands[]={'+','.join(str(c['address'])+'u' for c in g['commands'])+'};',
 'static const uint32_t upper_event_targets[]={'+','.join(str(e['target'])+'u' for e in g['events'])+'};',
 'static const uint32_t upper_event_sites[]={'+','.join(str(e['site'])+'u' for e in g['events'])+'};']
entries=[]
for i,(tag,normal) in enumerate(sorted(tags.items())):
 count=16 if normal else 4;values=struct.unpack_from('>'+str(count)+'I',rom,tag);present=sum(int(bool(v))<<j for j,v in enumerate(values));entries.append((tag,count,present,i))
 lines.append('static const marco_program_slot_t upper_native_slots_'+str(i)+'[]={')
 for ptr in values:
  if not ptr:lines.append('{NULL,0,MARCO_PROGRAM_ABSENT},')
  elif ptr in indices:lines.append('{&marco_script,'+str(indices[ptr])+',MARCO_PROGRAM_READY},')
  else:lines.append('{NULL,0,MARCO_PROGRAM_UNKNOWN},')
 lines.append('};')
lines+=['static const struct {uint32_t tag;marco_program_table_t table;uint16_t present;} upper_native_tables[]={']
for tag,count,present,i in entries:lines.append('{'+str(tag)+'u,{upper_native_slots_'+str(i)+','+str(count)+'},'+str(present)+'u},')
lines.append('};');a.output.mkdir(parents=True,exist_ok=True);(a.output/('marco_'+a.part+'_native.h')).write_text(('\n'.join(lines)+'\n').replace('upper_',a.part+'_'));shutil.copyfile(a.graph.parent/'marco_script.h',a.output/'marco_script.h');print('Bound',len(entries),'source tables to native animation programs')
