#!/usr/bin/env python3
"""Extract native state-machine boundaries from per-dispatch MAME file traces."""
import argparse,csv,hashlib,json,re,struct
from pathlib import Path
from extract_soldier_rng import values

def signed(v):return v if v<0x80000000 else v-0x100000000

def fields(r):
    return dict(routine=r['routine'],scalar62=signed(r['f62']),speed=r['f26'],field78=r['f78'],field7e=r['f7e'],field81=r['f81'])

def extract(capture,ghidra):
    text=(capture/'debug.log').read_text()
    assert 'MAME debugger version 0.276' in text and 'Error in command' not in text
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0xaf380,0xaf65a,0xaf6c0,0xaf890,0xaf8a6,0xaf8d4,0xaf8ea,0xb0336,0xb0354,0xb051e}<=addresses
    grouped={}
    for line in text.splitlines():
        tag=line.split(' ',1)[0]
        if tag not in ('DISPATCH_IN','DISPATCH_OUT','STATE_BOUNDARY','STATE_REGION_QUERY_OUT','STATE_OBJECT_QUERY_OUT','STATE_ANIM_START','STATE_ANIM_IN'):continue
        r=values(line)
        if r['frame']<2460:grouped.setdefault(r['frame'],[]).append((tag,r))
    assert sorted(grouped)==list(range(2351,2460,2))
    cases=[];previous=None;trace_files=[]
    for frame,rows in sorted(grouped.items()):
        starts=[r for t,r in rows if t=='DISPATCH_IN'];ends=[r for t,r in rows if t=='DISPATCH_OUT']
        assert len(starts)==len(ends)==1
        before,after=starts[0],ends[0]
        trace=capture/f'actor-{frame}.log';lines=trace.read_text().splitlines();trace_files.append(trace)
        assert lines and all(f'frame={frame} ' in line for line in lines)
        if frame==2351:
            assert before['routine']==0xaf17e and after['routine']==0xaf65a
            previous=fields(after);continue
        assert fields(before)==previous,('External main-state write',frame)
        main=[r for t,r in rows if t=='STATE_BOUNDARY' and r['site']==0xb051e]
        assert len(main)==1 and fields(main[0])==fields(after)
        anim=[r for t,r in rows if t=='STATE_ANIM_IN'];assert len(anim)==1
        region=[r for t,r in rows if t=='STATE_REGION_QUERY_OUT'];objects=[r for t,r in rows if t=='STATE_OBJECT_QUERY_OUT']
        assert len(region)<=1 and len(objects)<=1
        instructions=[]
        for line in lines:
            m=re.search(r' ([0-9A-F]{6}): (.*)',line);assert m
            instructions.append(int(m[1],16))
        cutoff=instructions.index(0xb051e)
        rng_calls=instructions[:cutoff].count(0x29c6)
        assert rng_calls==(2 if before['routine']==0xaf65a else 0)
        cases.append(dict(frame=frame,before=fields(before),after=fields(main[0]),
            before_motion=dict(scalar62=signed(anim[0]['f62']),speed=anim[0]['f26']),
            init_count=sum(t=='STATE_ANIM_START' for t,r in rows),rng_calls=rng_calls,
            senses=dict(region_code=region[0]['d0']&65535 if region else 65535,
                object_code=objects[0]['d0']&65535 if objects else 65535,
                object_flags=objects[0]['d1']&65535 if objects else 0)))
        previous=fields(after)
    assert len(cases)==54 and sum(c['rng_calls'] for c in cases)==4 and sum(c['init_count'] for c in cases)==6
    fixed=(capture/'program-fixed.bin').read_bytes();assert struct.unpack_from('>I',fixed,0xaf494)[0]==0
    def digest(p):return hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276, first-soldier constructor-gated per-dispatch traces',
        provenance=dict(capture_sha256={p.name:digest(p) for p in [capture/'debug.log',capture/'program-fixed.bin',capture/'program-bank.bin',capture/'run-command.json']+trace_files},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),
        tables=dict(action_rect=list(struct.unpack_from('>4h',fixed,0xaee04)),action_routines=list(struct.unpack_from('>8I',fixed,0xaf494))),
        cases=cases,limits=['Constructor remains an external initial condition.',
            'Scene region/object query engines and common forward probes are not ported.',
            'Integrated state replay covers grounded observed states with nonzero timer70; no complete soldier AI or mission coverage.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n')
    print('Extracted',len(f['cases']),'native state boundaries, six animation starts and four RNG calls')
