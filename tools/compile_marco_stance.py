#!/usr/bin/env python3
"""Generate portable numerical stance-entry fixtures, without ROM assets."""
import argparse,json
from pathlib import Path
from compile_marco_upper import MODES as UPPER_MODES
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--fixture',type=Path,default=Path('data/marco_stance_fixture.json'))
p.add_argument('--output',type=Path,default=Path('build/marco_stance_fixture.h'));a=p.parse_args()
d=json.loads(a.fixture.read_text());columns=d['columns'];ops=[];cases=[]
modes={0xc1e34:'MST_STAND',0xc1e94:'MST_BRAKE',0xc1ebe:'MST_LAND',0xc1efa:'MST_IDLE_START',0xc1f4a:'MST_IDLE_FAST'}
def row(values):return '{'+','.join(UPPER_MODES[v] if k=='upper_mode' else str(v)+'u' for k,v in zip(columns,values))+'}'
for c in d['cases']:
    cases.append('{'+modes[c['entry']]+','+row(c['before'])+','+row(c['after'])+','+str(len(ops))+','+str(len(c['operations']))+'},')
    for op in c['operations']:ops.append('{0,'+str(op['table'])+'u,'+str(op['program'])+'u},')
lines=['enum {'+','.join('C_'+k.upper() for k in columns)+',C_COUNT};',
    'static const uint32_t stance_table_tags[]={'+','.join(str(t)+'u' for t in d['table_tags'])+'};',
    'typedef struct {unsigned kind;uint32_t table,program;} stance_op_t;',
    'static const stance_op_t stance_operations[]={']+ops+['};',
    'static const struct {marco_stance_mode_t entry;uint32_t before[C_COUNT],after[C_COUNT];unsigned first,count;} stance_cases[]={']+cases+['};']
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text('\n'.join(lines)+'\n')
