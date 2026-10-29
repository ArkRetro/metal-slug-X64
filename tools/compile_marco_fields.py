#!/usr/bin/env python3
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
f=json.loads((ROOT/'data/marco_fields_fixture.json').read_text())
lines=['/* Independent MAME before/after values, no original program bytes. */',
 'static const struct {neo_anim_kind_t kind;uint16_t field;uint32_t value;uint32_t before[4],after[4];bool taken;} marco_field_cases[]={']
for c in f['cases']:
 arr=lambda v:'{'+','.join(str(n)+'u' for n in v)+'}'
 lines.append('{NEO_ANIM_'+c['kind']+','+str(c['field'])+','+str(c['value'])+'u,'+arr(c['before'])+','+arr(c['after'])+','+str(int(c.get('taken',False)))+'},')
lines.append('};');(ROOT/'build').mkdir(exist_ok=True);(ROOT/'build/marco_fields.h').write_text('\n'.join(lines)+'\n')
