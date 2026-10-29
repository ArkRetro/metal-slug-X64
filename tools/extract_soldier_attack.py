#!/usr/bin/env python3
"""Extract the original attack program, exact draws and its two native effects."""
import argparse,csv,hashlib,json
from pathlib import Path
from extract_soldier_rng import values
from extract_soldier_animation import program

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text();assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0x40648,0x4064e,0x3bb8e,0x3bba4,0x1ac94,0x1acf2,0x44ec,0x44ee}<=addresses
    converted,mapping=program((capture/'program-bank.bin').read_bytes(),0x23d7ee)
    cases=[];pending=None;raw=None;call=None
    for line in log.splitlines():
        if not line.startswith('ATTACK_'):continue
        tag=line.split(' ',1)[0];r=values(line)
        if tag=='ATTACK_INIT_IN':assert pending is None;pending=dict(kind='init',before=r)
        elif tag=='ATTACK_INIT_OUT':assert pending and pending['kind']=='init';pending['after']=r;cases.append(pending);pending=None
        elif tag=='ATTACK_IN':assert pending is None;pending=dict(kind='step',before=r,draws=[],calls=[])
        elif tag=='ATTACK_RAW':assert pending and raw is None;raw=r
        elif tag=='ATTACK_DRAW':
            assert pending and raw and raw['descriptor']==r['descriptor'];raw=None
            pending['draws'].append(dict(descriptor=r['descriptor'],x=r['xy']>>16,y=r['xy']&65535,attributes=r['attr'],z=r['zreg']))
        elif tag=='ATTACK_CALL_IN':assert pending and call is None;call=r
        elif tag=='ATTACK_CALL_OUT':
            assert call and call['target'] in (0x40648,0x3bb8e)
            pending['calls'].append(dict(before=call,after=r));call=None
        elif tag=='ATTACK_OUT':assert pending and pending['kind']=='step' and raw is None and call is None;pending['after']=r;cases.append(pending);pending=None
        else:raise AssertionError(tag)
    assert pending is None and len(cases)==21 and cases[0]['kind']=='init'
    previous=None
    for c in cases:
        b,a=c['before'],c['after'];assert b['frame']==a['frame']
        c['before_cursor']=0 if c['kind']=='init' else mapping[b['ptr']];c['after_cursor']=mapping[a['ptr']]
        if previous:
            for key in ('ptr','timer','speed','f17','f18','field50'):assert b[key]==previous[key],(key,b['frame'])
        previous=a
    calls=[c for step in cases for c in step.get('calls',[])];assert [c['before']['target'] for c in calls]==[0x40648,0x3bb8e]
    assert sum(len(c.get('draws',[])) for c in cases)==20 and sum(bool(c['after'].get('sr',0)&1) for c in cases)==1
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276 attack helper/draw/callback probes; program 23D7EE converted offline',
        provenance=dict(capture_sha256={n:digest(capture/n) for n in ('debug.log','program-bank.bin','program-fixed.bin','run-command.json','capture-options.json')},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),program=converted,cases=cases,
        palette_asset=0x35c,palette_handle=calls[0]['after']['palettes']>>24,
        limits=['One observed attack, 20 updates and two callbacks; not the full soldier state machine.',
            'Palette replacement is a native resource-service interface; the original palette-bank allocator is not emulated.',
            'Spawn queue/global scheduling and spawned actor impact/damage phases remain outside this animation helper.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n');print('Extracted 20 attack steps/draws, palette replacement and native throw spawn')
