#!/usr/bin/env python3
"""Extract common-control boundaries, RNG flags and native numerical tables."""
import argparse,csv,hashlib,json,re,struct
from pathlib import Path
from extract_soldier_rng import values,cursor

def signed32(v):return v if v<0x80000000 else v-0x100000000
def state(r):
    return dict(target84=signed32(r['f84']),velocity88=signed32(r['f88']),phase8c=r['f8c'],timer70=r['f70'],
        counter74=r['f74'],delay92=r['f92'],target8e=r['f8e'],target90=r['f90'],selector6e=r['f6e'],
        flags15=r['f15'],flags5f=r['f5f'],flags7c=r['f7c'],selected80=r['f80'],variant68=r['f68'])
def row(line):
    if ' grid=' in line:
        line,grid=line.split(' grid=');r=values(line)
        assert len(grid)==512;r['grid']=list(struct.unpack('>128H',bytes.fromhex(grid)))
        return r
    return values(line)
def extract(capture,ghidra):
    log=(capture/'debug.log').read_text()
    assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    assembly=list(csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t'))
    assert {0x29ee,0xae9c0,0xaeb3e,0xaeb5e,0xaecae,0xaece2,0xaeda6,0x260c8,0x510c}<= {int(r['address'],16) for r in assembly}
    calls=[];cases=[];active=None;rng_in=None
    for line in log.splitlines():
        tag=line.split(' ',1)[0]
        if tag=='RNG_IN':assert rng_in is None;rng_in=row(line)
        elif tag=='RNG_OUT':
            a,b=rng_in,row(line);assert a and a['seq']==b['seq'];rng_in=None
            assert b['sr']&3==0 and bool(b['sr']&8)==bool(b['mixed']&32768) and bool(b['sr']&4)==(b['mixed']==0)
            if calls:assert a['state']==calls[-1]['after']['state'] and a['cursor']==calls[-1]['after']['cursor']
            calls.append(dict(before=a,after=b))
        elif tag=='TARGET_IN':assert active is None;active=dict(target_before=row(line))
        elif tag in ('TARGET_OUT','PLAYER_TARGETS','CONTROL_IN','CONTROL_PREFIX_OUT','CONTROL_OUT','PROBABILITY_OUT','PROBE1_OUT','PROBE2_OUT'):
            assert active is not None
            assert tag not in active;active[tag]=row(line)
            if tag=='CONTROL_OUT':cases.append(active);active=None
        elif tag=='DELAY_UPDATE_IN':
            assert active and tag not in active;active[tag]=row(line)
    assert active is None and rng_in is None and len(calls)==276 and len(cases)==55
    assert [p['before']['seq'] for p in calls]==list(range(1,277))
    samples=[];previous=None
    for c in cases:
        before,mid,after=(c[k] for k in ('CONTROL_IN','CONTROL_PREFIX_OUT','CONTROL_OUT'))
        frame=before['frame'];assert all(c[k]['frame']==frame for k in c)
        assert state(c['TARGET_OUT'])==state(before)
        target_before=state(c['target_before'])
        if previous:
            for k in target_before:
                if k!='flags15':assert target_before[k]==previous[k],('Unrecovered external write',frame,k)
        previous=state(after)
        ctx=dict(x=before['px']>>16,y=before['py']>>16,origin_x=before['originx'],origin_y=before['originy'],
            p1x=before['p1x'],p1y=before['p1y'],p2x=before['p2x'],p2y=before['p2y'],
            active_mask=before['active'],mode88=before['mode88'],rank=before['rank'],targets_valid=bool(before['active']))
        assert before['active']==(before['p1active']|before['p2active']*2)
        if 'PLAYER_TARGETS' in c:
            p=c['PLAYER_TARGETS'];assert p['d7']&65535
            assert p['d0']&65535==ctx['p1x'] and p['d1']&65535==ctx['p1y']
            assert before['active']==1 and p['d2']&65535==ctx['p1x'] and p['d3']&65535==ctx['p1y']
        samples.append(dict(frame=frame,before=target_before,after_target=state(before),after_prefix=state(mid),after=state(after),
            context=ctx,grid=before['grid'],rng_before=before['seq'],rng_after=mid['seq'],
            threshold=c['PROBABILITY_OUT']['threshold'],probe1=c['PROBE1_OUT']['d6'],probe2=c['PROBE2_OUT']['d6']))
    steps=[]
    for p in calls:
        a,b=p['before'],p['after'];changes=[dict(index=i,value=y) for i,(x,y) in enumerate(zip(a['state'],b['state'])) if x!=y]
        assert len(changes)<=1
        steps.append(dict(value=b['d0']&65535,mixed=b['mixed'],sr=b['sr'],cursor=cursor(b['cursor']),changes=changes))
    fixed=(capture/'program-fixed.bin').read_bytes();bank=(capture/'program-bank.bin').read_bytes()
    assert bank[0xd3cd8:0xd3cda]==b'\x01\xff'
    tables=dict(rank_lookup=list(fixed[0x25fa6:0x260a6]),probability_mask=255,
        probability=list(struct.unpack_from('>102H',bank,0xd3cda)))
    return dict(source='MAME 0.276, constructor-gated control probes and SR after actual EOR.W',
        provenance=dict(capture_sha256={n:hashlib.sha256((capture/n).read_bytes()).hexdigest()
            for n in ('debug.log','instructions.log','program-fixed.bin','program-bank.bin','run-command.json')},
            ghidra_assembly_sha256=hashlib.sha256((ghidra/'instructions.tsv').read_bytes()).hexdigest(),
            language='68000:BE:32:MC68020'),tables=tables,
        initial_words=calls[0]['before']['state'],initial_cursor=cursor(calls[0]['before']['cursor']),rng=steps,cases=samples,
        returned_sign_differs_from_final_xor=sum(bool(s['value']&32768)!=bool(s['mixed']&32768) for s in steps),
        limits=['Global inter-actor RNG call schedule is still externally replayed.',
            'Two forward terrain-query results, player/scene configuration and dynamic delay grid remain external inputs.',
            'Only the recorded one-player profile is dynamically verified; complete soldier state machine remains incomplete.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();f=extract(a.capture,a.ghidra)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(f,indent=2)+'\n')
    print('Extracted',len(f['cases']),'control cycles and',len(f['rng']),'RNG flag samples;',f['returned_sign_differs_from_final_xor'],'return/XOR sign mismatches')
