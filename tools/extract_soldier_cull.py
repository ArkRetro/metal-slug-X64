#!/usr/bin/env python3
"""Extract the directional removal helper, including its observed positive case."""
import argparse,csv,hashlib,json
from pathlib import Path
from extract_soldier_rng import values

def signed(v):return v if v<0x80000000 else v-0x100000000

def extract(capture,ghidra):
    log=(capture/'debug.log').read_text();assert 'MAME debugger version 0.276' in log and 'Error in command' not in log
    addresses={int(r['address'],16) for r in csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t')}
    assert {0x1945c,0x19480,0x194a2,0x194f8,0x194fe,0xb0546}<=addresses
    active=None;cases=[]
    for line in log.splitlines():
        if line.startswith('CULL_IN '):assert active is None;active=values(line)
        elif line.startswith('CULL_OUT '):
            assert active is not None;b=values(line);a=active;active=None
            cases.append(dict(frame=a['frame'],return_frame=b['frame'],
                actor=dict(projected_x=signed(a['px']),projected_y=signed(a['py']),cached_x=signed(a['oldx']),cached_y=signed(a['oldy']),flags15=a['f15']),
                bounds=dict(has_parent=bool(a['parent']),x=signed(a['parentx']) if a['parent'] else 0,y=signed(a['parenty']) if a['parent'] else 0,
                    previous_x=signed(a['parentoldx']) if a['parent'] else 0,previous_y=signed(a['parentoldy']) if a['parent'] else 0,
                    **{k:a[k] for k in ('left','top','right','bottom')}),rectangle=[a[f'rect{i}'] for i in range(4)],outside=bool(b['sr']&1)))
    assert active is None and len(cases)==105 and sum(c['outside'] for c in cases)==1
    assert cases[-1]['outside'] and cases[-1]['frame']==2561
    deletion=[values(l) for l in log.splitlines() if l.startswith('LIFECYCLE_DELETE ')]
    assert len(deletion)==1 and deletion[0]['frame']==2561
    digest=lambda p:hashlib.sha256(p.read_bytes()).hexdigest()
    return dict(source='MAME 0.276 exact 01945C inputs and carry at 0B0546',
        provenance=dict(capture_sha256={n:digest(capture/n) for n in ('debug.log','capture-options.json','run-command.json','actor-2561.log')},
            ghidra_assembly_sha256=digest(ghidra/'instructions.tsv'),language='68000:BE:32:MC68020'),cases=cases,
        lifecycle=dict(actor_ram='0x101F14',deletion_entry_rom='0x004126',frame=2561),
        limits=['Only this actor and observed scene bounds/history are dynamically covered.',
            'The native predicate does not implement the original linked allocator or destruction cleanup.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();f=extract(a.capture,a.ghidra);a.output.write_text(json.dumps(f,indent=2)+'\n')
    print('Extracted',len(f['cases']),'cull decisions including deletion at frame 2561')
