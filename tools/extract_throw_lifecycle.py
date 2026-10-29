#!/usr/bin/env python3
"""Extract exact graphics/control lifecycle after the soldier's deferred spawn."""
import argparse,csv,hashlib,json,struct
from pathlib import Path
from extract_soldier_rng import values
from extract_soldier_animation import program
from extract_soldier_contact import signed

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text();assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0x3bc28,0x3bd3a,0x3bd98,0x3be6c,0x1c8fc,0x1c9e6,0x19504}<=addresses
    bank=(capture/'program-bank.bin').read_bytes();fixed=(capture/'program-fixed.bin').read_bytes()
    roots=[0x23f3f0,0x22d2a6];programs=[];maps=[]
    for root in roots:p,m=program(bank,root);programs.append(p);maps.append(m)
    cases=[];active=None;pending_init=None;initial=None;raw=None;cull=None;initializations=[]
    for line in log.splitlines():
        tag=line.split(' ',1)[0]
        if tag=='CHILD_IN':assert active is None;active=dict(before=values(line))
        elif tag=='OBJECT_INIT_IN':
            assert active and pending_init is None;pending_init=values(line)
            if initial is None:initial=pending_init
        elif tag=='OBJECT_INIT_OUT':
            assert pending_init;initializations.append(dict(before=pending_init,after=values(line)));pending_init=None
        elif tag=='OBJECT_IN':assert active and 'animation_before' not in active;active['animation_before']=values(line)
        elif tag=='OBJECT_RAW':assert active and raw is None;raw=values(line)
        elif tag=='OBJECT_DRAW':
            assert active and raw;d=values(line);assert d['descriptor']==raw['descriptor']
            active['draw']=dict(descriptor=d['descriptor'],x=d['xy']>>16,y=d['xy']&65535,attributes=d['attr'],z=d['zreg']);raw=None
        elif tag=='OBJECT_OUT':assert active and raw is None;active['animation_after']=values(line)
        elif tag in ('OBJECT_HIT_OUT','OBJECT_OTHER_OUT','OBJECT_CYCLE_IN','OBJECT_CYCLE_OUT'):
            assert active and tag not in active;active[tag]=values(line)
        elif tag=='OBJECT_CULL_IN':assert active and cull is None;cull=values(line)
        elif tag=='OBJECT_CULL_OUT':
            assert active and cull;active['cull']=dict(before=cull,after=values(line));cull=None
        elif tag in ('CHILD_OUT','CHILD_DELETE'):
            assert active and pending_init is None and raw is None and cull is None
            active['after']=values(line);active['deleted']=tag=='CHILD_DELETE';cases.append(active);active=None
    assert active is None and len(cases)==41 and len(initializations)==2 and initial
    assert sum(c['deleted'] for c in cases)==1 and cases[-1]['deleted']
    assert [x['before']['program'] for x in initializations]==roots
    common_bounds=None;result=[]
    for c in cases:
        b,a,r,z=c['before'],c['after'],c['animation_before'],c['animation_after']
        assert b['frame']==a['frame']==r['frame']==z['frame']
        flying=r['routine']==0x3bd3a;program_index=0 if flying else 1
        assert flying or r['routine']==0x1c9e6
        assert (r['base']&255)==(a['flags']&255)
        cycle=c.get('OBJECT_CYCLE_IN');hit=c.get('OBJECT_HIT_OUT');other=c.get('OBJECT_OTHER_OUT')
        if flying:assert cycle and hit and other and c['OBJECT_CYCLE_OUT']['attr']==a['flags']
        else:assert not cycle and not hit and not other
        if 'cull' in c:
            cb=c['cull']['before'];assert not(c['cull']['after']['sr']&1)
            bounds=dict(has_parent=bool(cb['parent']),x=signed(cb['parentx']) if cb['parent'] else 0,y=signed(cb['parenty']) if cb['parent'] else 0,
                previous_x=signed(cb['parentoldx']) if cb['parent'] else 0,previous_y=signed(cb['parentoldy']) if cb['parent'] else 0,
                **{k:cb[k] for k in ('left','top','right','bottom')})
            if common_bounds is None:common_bounds=bounds
            assert bounds==common_bounds
            rectangle=list(struct.unpack_from('>4H',fixed,0x3bc18 if flying else 0x1dec0))
            assert rectangle==[cb[f'rect{i}'] for i in range(4)]
        expected=dict(x=signed(a['x']),y=signed(a['y']),vx=signed(a['vx']),vy=signed(a['vy']),parent_tag=a['parent'],
            projected_x=signed(a['px']),projected_y=signed(a['py']),cached_x=signed(r['oldx']),cached_y=signed(r['oldy']),
            kind_angle=a['f6c']>>16,field66=a['f64']&65535,word1e=r['z'],flags15=a['flags']&255,flags5f=a['f5f'],mark=r['mark'],correction=r['correction'])
        assert expected['field66']==r['f66']
        result.append(dict(frame=b['frame'],program=program_index,animation_before=r,animation_after=z,
            before_cursor=maps[program_index][r['ptr']],after_cursor=maps[program_index][z['ptr']],expected=expected,draw=c['draw'],deleted=c['deleted'],
            phase=2 if a['routine']==0x3be6c else (1 if a['routine']==0x3bd3a else 3),
            epoch=r['epoch'],video_phase=cycle['phase16'] if cycle else 0,region_code=hit['result'] if hit else 0,other_contact=bool(other['sr']&1) if other else False,
            final_attributes=a['flags']))
    assert sum(c['program']==0 for c in result)==33 and sum(c['program']==1 for c in result)==8
    assert sum(c['region_code']!=0 for c in result)==1 and not any(c['other_contact'] for c in result)
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276 complete identified throw actor, animation/body/palette/query boundaries',
        provenance=dict(capture_sha256={n:digest(capture/n) for n in ('debug.log','program-fixed.bin','program-bank.bin','capture-options.json','run-command.json')},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),
        programs=programs,initial=initial,bounds=common_bounds,flight_rectangle=list(struct.unpack_from('>4H',fixed,0x3bc18)),burst_rectangle=list(struct.unpack_from('>4H',fixed,0x1dec0)),
        cases=result,limits=['One observed throw and subsequent visual effect; scene collision query answers remain external.',
            'The flight resolver remains the stationary-parent no-terrain-contact subset.',
            'The audio request is emitted as an event; no YM2610 emulation or completed audio asset conversion.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n');print('Extracted 33 flight and 8 subsequent effect updates, 41 draws and deletion')
