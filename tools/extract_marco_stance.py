#!/usr/bin/env python3
"""Extract executed entry initializers, stopping before the C1FA6 idle update."""
import argparse,hashlib,json,re,struct
from pathlib import Path
from extract_marco_fields import fields
p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path)
p.add_argument('--output',type=Path,required=True);a=p.parse_args()
rom=(a.capture/'program-fixed.bin').read_bytes();tables=[]
for pc in (0xc1e3c,0xc1ea2,0xc1f20,0xc1f8a):
    assert struct.unpack_from('>H',rom,pc)[0]==0x41fa
    tables.append(pc+2+struct.unpack_from('>h',rom,pc+2)[0])
cases=[];active=pending=None
text=(a.capture/'debug.log').read_text();assert 'Error in command' not in text
for line in text.splitlines():
    if not line.startswith('MARCO_'):continue
    tag=line.split()[0];r=fields(line)
    if tag=='MARCO_STANCE_IN':
        assert active is None;active=dict(before=r,operations=[])
        # The form-2 landing effect is outside this numerical initializer fixture.
        assert r['entry']!=0xc1ebe or r['f93']!=2
    elif tag=='MARCO_STANCE_OUT':
        assert active is not None and pending is None and r['exit']==0xc1fa6
        active['after']=r;cases.append(active);active=None
    elif active is not None:
        if tag=='MARCO_PROGRAM_IN':
            assert pending is None and r['part']==0 and r['table'] in tables
            pending=dict(kind=0,table=r['table'])
            offset=(r['form']*4+r['character'])*4
            ptr=struct.unpack_from('>I',rom,r['table']+offset)[0]
            if not ptr:ptr=struct.unpack_from('>I',rom,r['table']+(offset&12))[0]
            pending['program']=ptr
        elif tag=='MARCO_PROGRAM_CHOICE':
            assert pending is not None and pending['program']==r['program']
        elif tag=='MARCO_PROGRAM_OUT':
            assert pending is not None and r['cursor']==pending['program'] and r['timer']==0 and r['speed']==256
            active['operations'].append(pending);pending=None
assert cases and active is None and pending is None
columns=[k for k in cases[0]['before'] if k not in ('frame','entry','mode')]
rows=[]
for c in cases:
    assert set(c['before'])==set(columns)|{'frame','entry','mode'}
    assert set(c['after'])==set(columns)|{'frame','exit','mode'} and c['after']['mode']==0xc1fa6
    rows.append(dict(entry=c['before']['entry'],before=[c['before'][k] for k in columns],after=[c['after'][k] for k in columns],operations=c['operations']))
sha=lambda n:hashlib.sha256((a.capture/n).read_bytes()).hexdigest()
result=dict(source='Read-only MAME stance entry boundaries before C1FA6; original program selection checked against ROM',columns=columns,table_tags=tables,cases=rows,
    provenance={n:sha(n) for n in ('capture.json','capture.lua','debug.log','program-fixed.bin')},
    limits=['No idle update, world query or animation advance is replayed here.','Form-2 landing effect is not observed; native service must be supplied explicitly.'])
a.output.write_text(json.dumps(result,separators=(',',':'))+'\n');print('Extracted stance entries:',len(rows))
