#!/usr/bin/env python3
"""Extract motion boundaries and numerical tables from actual debugger captures."""
import argparse,csv,hashlib,json,struct
from pathlib import Path
from extract_soldier_rng import values

def signed(v):return v if v<0x80000000 else v-0x100000000
def actor(r):
    return dict(x=signed(r['x']),y=signed(r['y']),vx=signed(r['vx']),vy=signed(r['vy']),
        scalar62=signed(r['f62']),projected_x=signed(r['px']),projected_y=signed(r['py']),
        cached_x=signed(r['oldx']),cached_y=signed(r['oldy']),parent_tag=r['parent'],
        kind_angle=r['f6c'],field66=r['f66'],word1e=r['f1e'],flags15=r['f15'],flags5f=r['f5f'],
        timer5a=r['f5a'],mark=r['mark'],correction=r['correction'])
def extract(capture,ghidra):
    text=(capture/'debug.log').read_text()
    assert 'MAME debugger version 0.276' in text and 'Error in command' not in text
    assert 'CONSTRUCTOR_WRITE frame=2351' in text
    rows=list(csv.DictReader((ghidra/'instructions.tsv').open(),delimiter='\t'))
    addresses={int(r['address'],16) for r in rows}
    assert {0x2e98,0x2ea0,0x2ea6,0x2eae,0x12e64,0x14492,0x144f4,0x1456a,0x14632,0x1abc8,0x1ad8c}<=addresses
    active=None;cases=[];ignored=0
    for line in text.splitlines():
        tag=line.split(' ',1)[0]
        if tag not in ('GROUND_IN','AIR_IN','VECTOR_IN','VECTOR_OUT','KERNEL_IN','KERNEL_OUT','COMMIT_OUT','GROUND_OUT'):continue
        row=values(line)
        if tag=='GROUND_IN':assert active is None;active=dict(before=row)
        elif tag=='AIR_IN':raise AssertionError('Air wrapper needs its own verified path')
        elif tag=='KERNEL_IN' and active is None:ignored+=1
        elif active is not None:
            if tag=='GROUND_OUT':active['after']=row;cases.append(active);active=None
            else:
                assert tag not in active,('Nested or duplicate stage',tag)
                active[tag]=row
    assert active is None and len(cases)==54
    result=[]
    for c in cases:
        before,kin,kout,commit,after=(c[k] for k in ('before','KERNEL_IN','KERNEL_OUT','COMMIT_OUT','after'))
        assert len({r['frame'] for r in (before,kin,kout,commit,after)})==1
        assert actor(commit)==actor(after)
        assert before['epoch']==commit['epoch']
        result.append(dict(frame=before['frame'],before=actor(before),prepared_flags15=kin['f15'],
            request=dict(x=signed(kin['d0']),y=signed(kin['d1']),dx=signed(kin['d2']),dy=signed(kin['d3']),
                parent_tag=kin['a0'],mode=kin['mode'],mode8=kin['mode8'],mode9=kin['mode9'],modea=kin['modea'],moded=kin['moded']),
            kernel=dict(x=signed(kout['d0']),y=signed(kout['d1']),vx=signed(kout['d2']),vy=signed(kout['d3']),
                parent_tag=kout['a0'],kind_angle_word=kout['d7'],flags=kout['d6']&65535),
            parent=dict(tag=kout['a0'],x=signed(kout['p14']),y=signed(kout['p18']),
                previous_x=signed(kout['p1c']),previous_y=signed(kout['p20']),correction=kout['p25']),
            epoch=before['epoch'],after=actor(after),returned_word=after['d0'],
            vector=dict(scalar=signed(c['VECTOR_IN']['d0']),angle=(c['VECTOR_IN']['d1']>>8)&255,
                vx=signed(c['VECTOR_OUT']['d1']),vy=-signed(c['VECTOR_OUT']['d0']))))
    fixed=(capture/'program-fixed.bin').read_bytes();ram=(capture/'ram2360.bin').read_bytes()
    mapping=struct.unpack_from('>I',ram,0xc00e)[0]
    assert 0<=mapping<len(fixed)-8,'Group map is not in this captured fixed CPU block'
    tables=dict(coefficients=list(struct.unpack_from('>320h',fixed,0x29fc)),
        angles=[struct.unpack_from('>H',fixed,0x12e6a+4*i)[0] for i in range(14)],groups=list(fixed[mapping:mapping+8]),
        provenance=dict(coefficients_rom='0x0029FC',angles_rom='0x012E6A',group_map_rom=hex(mapping),group_pointer_ram='0x10C00E'))
    return dict(source='MAME 0.276 euro BIOS, constructor-gated motion probes; no gameplay RAM patches',
        provenance=dict(capture_sha256={name:hashlib.sha256((capture/name).read_bytes()).hexdigest()
            for name in ('debug.log','instructions.log','program-fixed.bin','ram2360.bin','run-command.json')},
            ghidra_assembly_sha256=hashlib.sha256((ghidra/'instructions.tsv').read_bytes()).hexdigest(),
            ghidra_language='68000:BE:32:MC68020'),ignored_non_motion_kernel_calls=ignored,tables=tables,cases=result,
        limits=['Ground wrapper only; no captured air-wrapper calls.',
            'Single-plane static terrain subset is not the complete 013968 kernel.',
            'Walls, hazards, stacked floors, moving objects and unobserved branches require further recovery.'])
if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('ghidra',type=Path)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args();f=extract(a.capture,a.ghidra)
    a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(f,indent=2)+'\n')
    print('Extracted',len(f['cases']),'ground motion boundaries; excluded',f['ignored_non_motion_kernel_calls'],'other kernel calls')
