#!/usr/bin/env python3
"""Extract measured RNG/state fixtures; never simulate the original algorithms.

Full debugger logs stay private. Fixtures retain a numerical RNG state and its
observed mutations, not ROM bytes, full RAM images or tentative decompiled C.
"""
import argparse,csv,hashlib,json,re,struct
from pathlib import Path

def values(line):
    result={}
    for key,value in re.findall(r'(\w+)=([0-9A-F]+)',line):
        if key=='state':result[key]=[int(value[i:i+4],16) for i in range(0,len(value),4)]
        else:result[key]=int(value,10 if key in ('seq','frame') else 16)
    return result

def cursor(address):
    assert 0x100016<=address<=0x100082 and not address&1,hex(address)
    return (address-0x100016)//2

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text()
    assert 'MAME debugger version 0.276' in log
    assert 'Error in command' not in log and 'unknown command' not in log
    assembly=list(csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t'))
    addresses={int(row['address'],16) for row in assembly}
    assert {0x2976,0x2994,0x29ba,0x29c6,0x29d2,0x29ee,0xaf6d0,0xaf6fe,0xaf724}<=addresses
    pairs={};seeds=[];calls=[];decisions=[];stage=[]
    for line in log.splitlines():
        tag=line.split(' ',1)[0]
        if tag in ('SEED_IN','RNG_IN'):
            assert tag not in pairs,'Unexpected nested call; do not guess pairing'
            pairs[tag]=values(line)
        elif tag in ('SEED_OUT','RNG_OUT'):
            before=pairs.pop(tag.replace('_OUT','_IN'));after=values(line)
            assert (before['seq'],before['ret'],before['a6'])==(after['seq'],after['ret'],after['a6'])
            assert len(before['state'])==len(after['state'])==55
            assert before['d0']&0xffff0000==after['d0']&0xffff0000
            if tag=='SEED_OUT':
                seeds.append(dict(frame=before['frame'],request=before['d0']&65535,
                    counter_before=before['counter'],counter_after=after['counter'],
                    expected_d0=after['d0']&65535,cursor=cursor(after['cursor']),words=after['state']))
            else:
                if calls:
                    assert before['state']==calls[-1]['after']['state'],'Uncaptured RNG state write'
                    assert before['cursor']==calls[-1]['after']['cursor']
                calls.append(dict(before=before,after=after))
        elif tag=='STATE':
            row=values(line)
            if row['site']==0xaf6d0:assert not stage;stage=[row]
            elif row['site']==0xaf6f0:assert len(stage)==1;stage.append(row)
            elif row['site']==0xaf72a:
                assert len(stage)==2;motion,animation=stage;stage=[]
                assert motion['frame']==animation['frame']==row['frame']
                # No writes to these input fields occur between the probes.
                for key in ('f84','f3c','f62','f7c','f6c'):assert row[key]==animation[key],key
                decisions.append(dict(frame=row['frame'],routine_before=motion['routine'],
                    d6=motion['d6']&65535,field78=motion['f78'],field84=animation['f84'],
                    field3c=animation['f3c'],field62=animation['f62'],field7c=animation['f7c'],
                    field6c=animation['f6c'],expected_routine=row['routine']))
            else:raise AssertionError(('Unexpected probe',row))
    assert not pairs and not stage and calls and seeds and decisions
    assert [p['before']['seq'] for p in calls]==list(range(1,len(calls)+1))
    steps=[];initializers=[]
    for index,pair in enumerate(calls):
        before,after=pair['before'],pair['after']
        changes=[dict(index=i,value=b) for i,(a,b) in enumerate(zip(before['state'],after['state'])) if a!=b]
        assert len(changes)<=1,'Expected one observed XOR destination, not a bulk state mutation'
        steps.append(dict(frame=before['frame'],return_pc=before['ret'],a6=before['a6'],
            expected_value=after['d0']&65535,cursor=cursor(after['cursor']),changes=changes))
        if before['ret']==0xaf670 and before['a6']==0x101f14:
            second=calls[index+1]
            assert second['before']['ret']==0xaf67c and second['before']['a6']==0x101f14
            frame=before['frame']+1
            ram=(capture/f'ram{frame}.bin').read_bytes();offset=0x1f14
            initializers.append(dict(step=index,frame_after=frame,flags15=before['f15'],field92=before['f92'],
                expected62=struct.unpack_from('>i',ram,offset+0x62)[0],expected26=struct.unpack_from('>H',ram,offset+0x26)[0]))
    # Ensure a claimed transition is present in the executed M68000 instruction trace.
    trace=(capture/'state-instructions.log').read_text()
    assert 'A6=00101F14' in trace and '0AF702: move.l  #$af890, (A6)' in trace
    assert '0AF8AA: asr.l   #3, D0' in trace and '0AF8AC: sub.l   D0, ($62,A6)' in trace
    assert any(row['expected_routine']==0xaf890 for row in decisions)
    damping=[]
    for frame in range(2100,2459,2):
        before=(capture/f'ram{frame}.bin').read_bytes();offset=0x1f14
        if struct.unpack_from('>I',before,offset)[0] not in (0xaf890,0xaf8a6):continue
        after=(capture/f'ram{frame+2}.bin').read_bytes()
        damping.append(dict(frame_before=frame,frame_after=frame+2,
            value_before=struct.unpack_from('>i',before,offset+0x62)[0],
            value_after=struct.unpack_from('>i',after,offset+0x62)[0]))
    assert damping
    files={name:hashlib.sha256((capture/name).read_bytes()).hexdigest()
        for name in ('debug.log','instructions.log','state-instructions.log','run-command.json')}
    for item in initializers:
        name=f"ram{item['frame_after']}.bin";files[name]=hashlib.sha256((capture/name).read_bytes()).hexdigest()
    for item in damping:
        for frame in (item['frame_before'],item['frame_after']):
            name=f'ram{frame}.bin';files[name]=hashlib.sha256((capture/name).read_bytes()).hexdigest()
    return dict(source='MAME 0.276 euro BIOS, entry/return breakpoints; no RAM overrides',
        provenance=dict(capture_sha256=files,ghidra_language='68000:BE:32:MC68020',
            ghidra_assembly_sha256=hashlib.sha256((ghidra/'instructions.tsv').read_bytes()).hexdigest()),
        seeds=seeds,initial_cursor=cursor(calls[0]['before']['cursor']),initial_words=calls[0]['before']['state'],
        steps=steps,initializers=initializers,decisions=decisions,damping=damping,
        limits=['RNG call order across all native actors is not implemented.',
            'Only AF6D0..AF724 decision prefix; later decisions can overwrite this result.',
            'Ground helper, animation helper, full soldier AI and other state semantics remain incomplete.'])

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();fixture=extract(a.capture,a.ghidra)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(fixture,indent=2)+'\n')
    print('Extracted',len(fixture['seeds']),'seed calls,',len(fixture['steps']),'consecutive RNG calls,',len(fixture['initializers']),'first-soldier initializations,',len(fixture['decisions']),'decision prefixes and',len(fixture['damping']),'damping updates')
