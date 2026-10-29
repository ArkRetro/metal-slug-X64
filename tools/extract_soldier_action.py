#!/usr/bin/env python3
"""Extract original weighted-action inputs/outputs, including full RNG states."""
import argparse,csv,hashlib,json
from pathlib import Path
from extract_soldier_rng import values,cursor

def row(line):
    prefix,pool=line.split(' pool=');r=values(prefix)
    assert len(pool)==220;r['words']=[int(pool[i:i+4],16) for i in range(0,220,4)]
    r['cursor']=cursor(r['cursor']);return r

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text();assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0xaee0c,0xaeeac,0xaeeb4,0xaef10,0xaef12,0x1af3c,0x1affc}<=addresses
    active=None;cases=[]
    for line in log.splitlines():
        if line.startswith('ACTION_IN '):assert active is None;active=row(line)
        elif line.startswith('ACTION_OUT '):
            assert active is not None;after=row(line);assert active['frame']==after['frame']
            cases.append(dict(before=active,after=after));active=None
    assert active is None and cases and any(c['after']['result']==1 for c in cases)
    for c in cases:
        assert c['after']['result'] in (0,1),'Do not silently generalize action coverage'
        if c['after']['result']==0:assert c['after']['words']==c['before']['words'] and c['after']['cursor']==c['before']['cursor']
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276: hold right from 2280 to 2360, then remain near first soldier; no RAM patches',
        provenance=dict(capture_sha256={name:digest(capture/name) for name in ('debug.log','capture-options.json','run-command.json','program-fixed.bin')},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),
        cases=cases,limits=['Only one active player and action mask 02 are dynamically covered.',
            'The observed nonzero choice selects ROM AF918; its animation callbacks and projectile are not ported by this module.',
            'Filtered-mask and two-player branches remain static translations without dynamic coverage.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n')
    print('Extracted',len(f['cases']),'action boundaries;',sum(c['after']['result']!=0 for c in f['cases']),'nonzero choices')
