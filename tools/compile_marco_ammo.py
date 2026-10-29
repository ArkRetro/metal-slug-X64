#!/usr/bin/env python3
"""Compile numerical ammunition observations without ROM contents."""
import argparse,json
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--fixture',type=Path,default=Path('data/marco_ammo_fixture.json'))
p.add_argument('--output',type=Path,default=Path('build/marco_ammo_fixture.h'))
a=p.parse_args();data=json.loads(a.fixture.read_text())
rows=['static const struct {marco_ammo_t before,after;unsigned request,unlimited,special,allowed;} ammo_cases[]={']
for c in data['cases']:
    b=c['before'];v=c['after']
    row=lambda s:'{'+','.join(str(s[k]) for k in ('ammo','live','cap','kind'))+'}'
    rows.append('{'+row(b)+','+row(v)+','+','.join(map(str,(b['request'],int(bool(b['cheat']&8)),int(bool(b['special'])),v['sr']&1)))+'},')
rows.append('};');a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text('\n'.join(rows)+'\n')
