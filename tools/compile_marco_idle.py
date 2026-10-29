#!/usr/bin/env python3
import argparse,json
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('--fixture',type=Path,default=Path('data/marco_idle_fixture.json'));p.add_argument('--output',type=Path,default=Path('build/marco_idle_fixture.h'));a=p.parse_args()
f=json.loads(a.fixture.read_text());keys=('attr','f5f','f6c','f9a','f9b','f9c','f9d','packed','gate_mode','gate_current','gate_limit')
branches=dict(zip((0xc271c,0xc2214,0xc285c,0xc2654,0xc215e,0xc1f4a,0xc2144,0xc2022),('MSI_FALL','MSI_CROUCH','MSI_RUN','MSI_JUMP','MSI_MELEE','MSI_FAST','MSI_DRAW','MSI_WORLD')))
row=lambda d:'{'+','.join(str(d[k])+'u' for k in keys)+'}'
lines=['enum {'+','.join('I_'+k.upper() for k in keys)+',I_COUNT};','static const struct {uint32_t before[I_COUNT],after[I_COUNT];marco_idle_branch_t branch;} idle_cases[]={']
for c in f['cases']:lines.append('{'+row(c['before'])+','+row(c['after'])+','+branches[c['after']['exit']]+'},')
lines.append('};');a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text('\n'.join(lines)+'\n')
