#!/usr/bin/env python3
"""Compile original descriptor outputs and checked RAM deltas, excluding ROM bytes."""
import hashlib
import json
from pathlib import Path
from descriptor_harness import base_ram

ROOT=Path(__file__).resolve().parents[1]
fixture=json.loads((ROOT/'data/ram_descriptors_fixture.json').read_text())
assert fixture['status']=='PASS_ORIGINAL_RAM_DESCRIPTOR_HELPERS_WITH_NATIVE_CALLBACK_SUBSET'
for name,expected in fixture['source_sha256'].items():
    assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==expected
layout=json.loads((ROOT/'decomp/canonical-model/layout.json').read_text())
assert fixture['code_sha256']==next(c['sha256'] for c in layout['contexts'] if c['id']=='RAM_COPY_100000')

def registers(values,sr):
    return '{{'+','.join(f'0x{v:08x}u' for v in values[:8])+'},{'+','.join(f'0x{v:08x}u' for v in values[8:])+'},'+f'0x{sr:04x}u'+'}'

def apply(ram,patches):
    for offset,value in patches:
        assert 0x114a<=offset<65536 and 0<=value<=255
        ram[offset]=value

patches=[];rows=[];previous=None
for row in fixture['results']:
    c=row['input'];i=c['index'];assert i==len(rows)
    if c['chain_from']<0:
        before=base_ram(i)
    else:
        assert c['chain_from']==i-1 and not row['input_patches']
        before=bytearray(previous)
    apply(before,row['input_patches'])
    assert hashlib.sha256(before).hexdigest()==row['normalized_input_ram_sha256']
    after=bytearray(before);apply(after,row['output_patches'])
    assert hashlib.sha256(after).hexdigest()==row['normalized_output_ram_sha256']
    input_first=len(patches);patches.extend(row['input_patches'])
    output_first=len(patches);patches.extend(row['output_patches'])
    e=row['expected']
    rows.append('{'+registers(c['registers'],c['sr'])+','+registers(e['registers'],e['sr'])+','+
                '{'+f'{e["status"]},0x{e["boundary"]:08x}u,0x{e["target"]:08x}u'+'},'+
                f'{input_first}u,{len(row["input_patches"])}u,{output_first}u,{len(row["output_patches"])}u,{row["write_events"]}u,0x{row["write_fnv32"]:08x}u,{c["chain_from"]},{c["mode"]}u'+'},')
    previous=after
lines=['/* Original MAME RAM deltas; no source program-ROM bytes embedded. */',
       '#include "../decomp/recovered/ram_descriptors.h"',
       'typedef struct { uint16_t offset; uint8_t value; } descriptor_patch_t;',
       'typedef struct { msx_descriptor_registers input,expected; msx_descriptor_result result; uint32_t input_first,input_count,output_first,output_count,writes,write_hash; int32_t chain_from; uint8_t mode; } descriptor_case_t;',
       'static const descriptor_patch_t descriptor_patches[] = {']
lines.extend('{'+f'{offset}u,{value}u'+'},' for offset,value in patches)
lines+=['};','static const descriptor_case_t descriptor_cases[] = {']+rows+['};',f'#define DESCRIPTOR_CASES {len(rows)}u']
assert len(rows)==fixture['cases']
out=ROOT/'build/ram_descriptors_fixture.h';out.parent.mkdir(exist_ok=True);out.write_text('\n'.join(lines)+'\n')
print('Compiled',len(rows),'descriptor transitions with checked RAM deltas')
