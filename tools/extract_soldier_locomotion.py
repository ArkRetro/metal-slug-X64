#!/usr/bin/env python3
"""Extract reposition-state boundaries, numerical data and conditional poses."""
import argparse,csv,hashlib,json,re,struct
from pathlib import Path
from extract_soldier_rng import values,cursor
from extract_soldier_contact import actor,signed
from extract_soldier_control import state as control
from extract_soldier_animation import program

def state(r):return dict(routine=r['routine'],field78=r['f78'],field7e=r['f7e'],field81=r['f81'],field61=r['f61'])
def row(line):
    if ' pool=' not in line:return values(line)
    prefix,pool=line.split(' pool=');r=values(prefix);assert len(pool)==220
    r['words']=[int(pool[i:i+4],16) for i in range(0,220,4)];r['cursor']=cursor(r['cursor']);return r

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text();assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0xaf4f6,0xaf532,0xaf540,0xaf572,0xaf582,0xaf5b8,0xaf604,0xaf656,0x4412,0x4428,0x4446,0x12dee,0x1319e}<=addresses
    cases=[];active=None;last_state=None;ground=False;raw=None;programs=[];maps=[];roots={}
    bank=(capture/'program-bank.bin').read_bytes()
    for line in log.splitlines():
        tag=line.split(' ',1)[0]
        if tag=='LOCO_GRID':
            prefix,grid=line.split(' grid=');r=values(prefix);assert last_state and len(grid)==512
            assert r['site']==last_state['site'] and r['frame']==last_state['frame'];last_state['grid']=list(struct.unpack('>128H',bytes.fromhex(grid)));continue
        if tag not in ('LOCO_SEED_IN','LOCO_BODY_IN','LOCO_MAIN_OUT','LOCO_POSE_READY','LOCO_GROUND_IN','LOCO_KERNEL_IN','LOCO_KERNEL_OUT','LOCO_COMMIT_OUT','LOCO_INIT_IN','LOCO_INIT_OUT','LOCO_ANIM_IN','LOCO_ANIM_OUT','LOCO_RAW','LOCO_DRAW','STATE_REGION_QUERY_OUT','STATE_OBJECT_QUERY_OUT'):continue
        r=row(line)
        if 'words' in r:last_state=r
        if tag=='LOCO_SEED_IN':assert active is None;active=dict(before=r,init=True)
        elif tag=='LOCO_BODY_IN':
            if active is None:active=dict(before=r,init=False)
            assert 'body' not in active;active['body']=r
        elif tag=='LOCO_MAIN_OUT':
            if active is not None:
                assert not ground and 'anim_after' in active;active['after']=r;cases.append(active);active=None
        elif active is not None:
            if tag=='LOCO_GROUND_IN':assert not ground;ground=True;active['motion_before']=r
            elif tag=='LOCO_KERNEL_IN':
                if ground:assert 'kernel_in' not in active;active['kernel_in']=r
            elif tag=='LOCO_KERNEL_OUT':assert ground;active['kernel_out']=r
            elif tag=='LOCO_COMMIT_OUT':assert ground;active['motion_after']=r;ground=False
            elif tag=='LOCO_INIT_IN':
                assert active['init'];root=r['program']
                if root not in roots:roots[root]=len(programs);p,m=program(bank,root);programs.append(p);maps.append(m)
                active['init_before']=r;active['program']=roots[root]
            elif tag=='LOCO_INIT_OUT':active['init_after']=r
            elif tag=='LOCO_POSE_READY':active['ready']=r
            elif tag=='LOCO_ANIM_IN':
                root_index=active.get('program',cases[-1]['program'] if cases else None);assert root_index is not None
                active['program']=root_index;active['anim_before']=r
            elif tag=='LOCO_ANIM_OUT':active['anim_after']=r
            elif tag=='LOCO_RAW':assert raw is None;raw=r
            elif tag=='LOCO_DRAW':
                assert raw and raw['descriptor']==r['descriptor'];raw=None
                active['draw']=dict(descriptor=r['descriptor'],x=r['xy']>>16,y=r['xy']&65535,attributes=r['attr'],z=r['zreg'])
            elif tag=='STATE_REGION_QUERY_OUT':assert 'region' not in active;active['region']=r['d0']&65535
            elif tag=='STATE_OBJECT_QUERY_OUT':assert 'object' not in active;active['object']=dict(code=r['d0']&65535,flags=r['d1']&65535)
    assert active is None and raw is None and len(cases)==34 and sum(c['init'] for c in cases)==5
    result=[]
    for c in cases:
        b,body,a=c['before'],c['body'],c['after'];g,k,o,z=(c[k] for k in ('motion_before','kernel_in','kernel_out','motion_after'))
        assert all(r['frame']==b['frame'] for r in (body,a,g,k,o,z,c['anim_before'],c['anim_after'],c['ready']))
        trace=capture/f"actor-{b['frame']}.log";pcs=[int(m[1],16) for l in trace.read_text().splitlines() if (m:=re.search(r' ([0-9A-F]{6}):',l))]
        calls=pcs[:pcs.index(0xb051e)].count(0x29c6)
        assert (a['cursor']-b['cursor'])%55==calls
        before_actor=actor(g);before_actor['scalar62']=signed(b['f62'])
        ctx={k:b[k] for k in ('p1x','p1y','p2x','p2y')};ctx.update(active_mask=b['active'],origin_x=b['originx'],origin_y=b['originy'])
        result.append(dict(frame=b['frame'],init=c['init'],program=c['program'],before=state(b),after_begin=state(body),after=state(a),
            scalar_before=signed(b['f62']),speed_before=b['speed'],speed_body=body['speed'],speed_after=a['speed'],
            control_before=control(b),control_after=control(a),context=ctx,grid=b['grid'],
            actor_before=before_actor,actor_body=actor(g),actor_after=actor(z),ready61=c['ready']['f61'],
            kernel=dict(x=signed(o['d0']),y=signed(o['d1']),vx=signed(o['d2']),vy=signed(o['d3']),parent_tag=o['a0'],kind_angle_word=o['d7'],flags=o['d6']&65535),
            parent=dict(tag=o['a0'],x=signed(o['p14']),y=signed(o['p18']),previous_x=signed(o['p1c']),previous_y=signed(o['p20']),correction=o['p25']),epoch=g['epoch'],
            anim_before=c['anim_before'],anim_after=c['anim_after'],before_cursor=maps[c['program']][c['anim_before']['ptr']],after_cursor=maps[c['program']][c['anim_after']['ptr']],
            draw=c['draw'],region_code=c.get('region',65535),object_code=c.get('object',{}).get('code',65535),object_flags=c.get('object',{}).get('flags',0),
            rng_before=dict(words=b['words'],cursor=b['cursor']),rng_after=dict(words=a['words'],cursor=a['cursor']),rng_calls=calls))
    assert sum(c['rng_calls'] for c in result)==6
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276 original AF4F6/AF572 main-state, physics, pose and RNG boundaries',
        provenance=dict(capture_sha256={n:digest(capture/n) for n in ('debug.log','program-fixed.bin','program-bank.bin','capture-options.json','run-command.json')},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),programs=programs,cases=result,
        limits=['Common-control/scene inputs and the RNG state at each main-state boundary remain external.',
            'Motion and animation persist within five reposition segments, not across unrecovered intervening states.',
            'Only observed inline selector values are dynamically covered; wall AF072 path remains unsupported.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n');print('Extracted 34 reposition updates, five starts, six native RNG calls and original draws')
