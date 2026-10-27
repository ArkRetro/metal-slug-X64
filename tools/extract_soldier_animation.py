#!/usr/bin/env python3
"""Convert observed soldier animation data and extract independent probe fixtures."""
import argparse,csv,hashlib,json,struct
from pathlib import Path
from extract_soldier_rng import values

def program(bank,root):
    commands=[];locations={}
    def walk(pc):
        while pc not in locations:
            assert 0x200000<=pc<0x300000
            offset=pc-0x200000;op=bank[offset];arg=bank[offset+1]
            c=dict(address=pc,kind='',asset=0,target=0,operand=0,flags=0,dx=0,dy=0)
            locations[pc]=len(commands);commands.append(c)
            if op==0:
                c.update(kind='WAIT',operand=arg);size=2
            elif op==4:
                c.update(kind='POSE',flags=arg,dx=struct.unpack_from('b',bank,offset+2)[0],
                    dy=struct.unpack_from('b',bank,offset+3)[0],asset=struct.unpack_from('>I',bank,offset+4)[0]);size=8
            elif op==0x20:
                assert arg==0x50,'Unrecovered actor-field write; do not silently map it'
                c.update(kind='FIELD50',asset=struct.unpack_from('>I',bank,offset+2)[0]);size=6
            elif op==8:
                assert arg==0
                target=struct.unpack_from('>I',bank,offset+2)[0];c.update(kind='JUMP',target_address=target)
                walk(target);break
            elif op==0x28:
                c['kind']='FINISH';break
            elif op==0x14:
                assert arg==4,'Unrecovered callback payload'
                callback=struct.unpack_from('>I',bank,offset+2)[0]
                c.update(kind='FLIP_FACING' if callback==0x1adaa else 'UNRECOVERED',asset=callback);size=6
            else:raise AssertionError(('Unrecovered command',hex(pc),hex(op)))
            pc+=size
    walk(root)
    for c in commands:
        if c['kind']=='JUMP':c['target']=locations[c.pop('target_address')]
    return dict(root=root,commands=commands),locations

def extract(capture,ghidra):
    text=(capture/'debug.log').read_text()
    assert 'MAME debugger version 0.276' in text and 'Error in command' not in text
    assert 'CONSTRUCTOR_WRITE frame=2351' in text
    rows=list(csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t'))
    addresses={int(r['address'],16) for r in rows}
    assert {0x4256,0x427a,0x431e,0x432c,0x433c,0x4352,0x4398,0x43fc,0x4476,0x449e,0x44ec,0x1adaa}<=addresses
    bank=(capture/'program-bank.bin').read_bytes();programs=[];maps=[];roots={}
    cases=[];active=None;pending=None;raw=None;call=None
    for line in text.splitlines():
        if not line.startswith('ANIM_'):continue
        tag=line.split(' ',1)[0];row=values(line)
        # Early probe logs used a speculative label. Preserve the observed
        # +0x50 value without claiming its complete gameplay semantics.
        if 'collision' in row:row['field50']=row.pop('collision')
        if tag=='ANIM_INIT_IN':
            assert pending is None
            root=row['program']
            if root not in roots:
                roots[root]=len(programs);p,m=program(bank,root);programs.append(p);maps.append(m)
            active=roots[root];pending=dict(kind='init',program=active,before=row)
        elif tag=='ANIM_INIT_OUT':
            assert pending and pending['kind']=='init'
            pending['after']=row;cases.append(pending);pending=None
        elif tag=='ANIM_IN':
            assert pending is None and active is not None
            pending=dict(kind='step',program=active,before=row,draws=[],calls=[])
        elif tag=='ANIM_RAW':
            assert pending and raw is None;raw=row
        elif tag=='ANIM_DRAW':
            assert pending and raw and raw['descriptor']==row['descriptor']
            pending['draws'].append(dict(descriptor=row['descriptor'],x=row['xy']>>16,y=row['xy']&65535,
                attributes=row['attr'],z=row['zreg'],raw_flags=raw['flags'],raw_dx=raw['dx'],raw_dy=raw['dy']))
            raw=None
        elif tag=='ANIM_CALL_IN':assert call is None;call=row
        elif tag=='ANIM_CALL_OUT':
            assert pending and call and call['target']==0x1adaa
            assert call['attr']^row['attr']==1
            pending['calls'].append(dict(target=call['target'],before=call['attr'],after=row['attr']));call=None
        elif tag in ('ANIM_OUT','ANIM_NULL_OUT'):
            assert pending and pending['kind']=='step' and raw is None and call is None
            pending['after']=row;cases.append(pending);pending=None
        else:raise AssertionError(('Unexpected probe',tag))
    assert pending is None and raw is None and call is None
    external_speed=[];previous=None
    for case in cases:
        m=maps[case['program']];before,after=case['before'],case['after']
        assert before['frame']==after['frame']
        case['before_cursor']=m[before['ptr']] if case['kind']=='step' else 0
        case['after_cursor']=m[after['ptr']]
        if previous:
            for key in ('ptr','timer','f17','f18','field50'):
                assert before[key]==previous[key],('Unmodeled external state write',key,before['frame'])
            if before['speed']!=previous['speed']:
                external_speed.append(dict(frame=before['frame'],before=previous['speed'],after=before['speed']))
        previous=after
    assert len(cases)==60,'Expected exactly six starts and 54 mission soldier steps'
    return dict(source='MAME debugger helper probes after verified actor constructor; Ghidra addressed assembly',
        provenance=dict(capture_sha256={name:hashlib.sha256((capture/name).read_bytes()).hexdigest()
            for name in ('debug.log','instructions.log','program-bank.bin','program-fixed.bin','run-command.json')},
            ghidra_assembly_sha256=hashlib.sha256((ghidra/'instructions.tsv').read_bytes()).hexdigest(),
            language='68000:BE:32:MC68020'),programs=programs,cases=cases,external_speed_writes=external_speed,
        limits=['Only observed animation commands and callback 01ADAA; unknown callbacks fail explicitly.',
            'Descriptor output is independent of Neo Geo SCB writes; the complete native soldier is not integrated.'])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();f=extract(a.capture,a.ghidra)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(f,indent=2)+'\n')
    print('Extracted',len(f['programs']),'programs,',len(f['cases']),'helper cases and',sum(len(c.get('draws',[])) for c in f['cases']),'original draw calls')
