#!/usr/bin/env python3
"""Extract the spawned actor's initial ballistics and persistent free flight."""
import argparse,csv,hashlib,json,struct
from pathlib import Path
from extract_soldier_rng import values
from extract_soldier_contact import actor,signed

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text();assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0x3bc28,0x3bc8e,0x3bd14,0x3bd3a,0x3bd50,0x143e4,0x14468,0x139b8,0x12e64}<=addresses
    rows=[];active=None;initial=None;call=None;spawn=None
    for line in log.splitlines():
        tag=line.split(' ',1)[0]
        if tag=='ATTACK_CALL_IN':call=values(line)
        elif tag=='ATTACK_CALL_OUT':
            if call and call['target']==0x3bb8e:spawn=dict(before=call,after=values(line))
            call=None
        elif tag=='THROW_INIT_TARGET':assert initial is None;initial=values(line)
        elif tag=='THROW_FLIGHT_IN':assert active is None;active=dict(before=values(line))
        elif tag.startswith('THROW_') and active is not None:
            assert tag not in active;active[tag]=values(line)
            if tag=='THROW_FLIGHT_OUT':rows.append(active);active=None
    assert initial and spawn and active is None and len(rows)==33
    cases=[]
    for c in rows:
        before,kin,kout,after=(c[k] for k in ('before','THROW_KERNEL_IN','THROW_KERNEL_OUT','THROW_COMMIT_OUT'))
        assert actor(c['THROW_FLIGHT_OUT'])==actor(after)
        assert kout['d6']==0x80000000 and kout['d7']==0x28
        assert len({r['frame'] for r in c.values()})==1
        cases.append(dict(frame=before['frame'],before=actor(before),after_acceleration=actor(c['THROW_AIR_IN']),
            request=dict(x=signed(kin['d0']),y=signed(kin['d1']),dx=signed(kin['d2']),dy=signed(kin['d3']),parent_tag=kin['a0'],
                mode=kin['mode'],mode8=kin['mode8'],mode9=kin['mode9'],modea=kin['modea'],moded=kin['moded']),
            kernel=dict(x=signed(kout['d0']),y=signed(kout['d1']),vx=signed(kout['d2']),vy=signed(kout['d3']),
                parent_tag=kout['a0'],kind_angle_word=kout['d7'],flags=kout['d6']&65535),after=actor(after),
            parent=dict(tag=kout['a0'],x=signed(kout['p14']),y=signed(kout['p18']),previous_x=signed(kout['p1c']),previous_y=signed(kout['p20']),correction=kout['p25']),epoch=before['epoch']))
    for a,b in zip(cases,cases[1:]):assert a['after']==b['before'],'External physics state must be modeled, not injected'
    assert spawn['after']['child']==0x1016d4
    source=spawn['before'];scene={k:signed(source[v]) for k,v in [('x','scene_x'),('y','scene_y'),('previous_x','scene_oldx'),('previous_y','scene_oldy')]}
    scene.update(tag=spawn['after']['cparent'],correction=0)
    scene['correction']=cases[0]['parent']['correction'];assert scene==cases[0]['parent']
    fixed=(capture/'program-fixed.bin').read_bytes()
    air_kind=struct.unpack_from('>H',fixed,0x12e6a+0x4000)[0];assert air_kind==0x28
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276 identified child allocated by 03BB8E, exact helper boundaries',
        provenance=dict(capture_sha256={n:digest(capture/n) for n in ('debug.log','program-fixed.bin','run-command.json','capture-options.json','child-2559.log')},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),
        tables=dict(target_rectangle=list(struct.unpack_from('>4h',fixed,0x3bc20)),air_kind=air_kind),
        spawn=dict(frame=source['frame'],source=dict(projected_x=signed(source['parent_x']),projected_y=signed(source['parent_y']),
            cached_x=signed(source['oldx']),cached_y=signed(source['oldy']),flags15=source['attr']&255),
            scene=scene,child=dict(x=signed(spawn['after']['cx']),y=signed(spawn['after']['cy']),vx=signed(spawn['after']['cvx']),vy=signed(spawn['after']['cvy']),parent_tag=spawn['after']['cparent'],flags15=spawn['after']['cflags'])),
        initialization=dict(before=actor(initial),after=cases[0]['before'],field6e=initial['f6e'],
            scene={k:initial[k] for k in ('p1x','p1y','p2x','p2y')}|dict(active_mask=initial['active'])),
        cases=cases,limits=['One executed throw, field6e=0, P1-only target; other initialization branches are static-only.',
            '33 airborne no-terrain-contact steps; dynamic collision/impact and later phases remain external.',
            'Stationary scene parent and caller-established absence of relevant dynamic kernel candidates.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n');print('Extracted one native spawn/throw and',len(f['cases']),'persistent flight boundaries')
