#!/usr/bin/env python3
"""Build native-service C fixtures; sparse deltas are from original MAME RAM."""
import argparse,hashlib,json
from condition_harness import ROOT,ram_input as base_ram
from native_conditions_harness import ram_input as world_ram
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--capture',help='Regenerate output deltas from original MAME capture');a=p.parse_args()
fixture=json.loads((ROOT/'data/native_conditions_fixture.json').read_text());path=ROOT/'data/native_condition_ram_patches.json'
if a.capture:
    from pathlib import Path
    patches=[]
    for row in fixture['results']:
        c=row['input'];before=world_ram(c);after=(Path(a.capture)/f"result-{c['index']}.ram").read_bytes()
        assert len(after)==65536 and hashlib.sha256(after).hexdigest()==row['expected']['ram_sha256']
        patches.append([[i,y] for i,(x,y) in enumerate(zip(before,after)) if x!=y])
    path.write_text(json.dumps(patches,separators=(',',':'))+'\n')
patches=json.loads(path.read_text());assert len(patches)==len(fixture['results'])
def nums(values):return '{'+','.join(f'UINT32_C(0x{x:x})' for x in values)+'}'
def regs(values,sr):return '{'+nums(values[:8])+','+nums(values[8:])+f',0x{sr:x}'+'}'
inputs=[];outputs=[];cases=[]
for row,patch in zip(fixture['results'],patches):
    c=row['input'];e=row['expected'];before=base_ram(c);world=world_ram(c)
    input_patch=[[i,y] for i,(x,y) in enumerate(zip(before,world)) if x!=y]
    after=bytearray(world)
    for offset,value in patch:assert 0<=offset<65536 and 0<=value<256;after[offset]=value
    assert hashlib.sha256(after).hexdigest()==e['ram_sha256']
    base='{'+regs(c['registers'],c['sr'])+','+regs(e['registers'],e['sr'])+f",{c['opcode']},{c['relation']},"+nums(c['counters'])+f",{c['threshold']},{c['x']},{c['y']},"+nums(c['helper_d'])+','+nums(c['query'])+f",{c['helper_a0']},{c['helper_a2']},{c['helper_sr']},0,0,0,0"+'}'
    cases.append('{'+base+f",{len(inputs)},{len(input_patch)},{len(outputs)},{len(patch)},{e['status']}"+'}')
    inputs+=input_patch;outputs+=patch
assert len(inputs)<65536 and len(outputs)<65536
lines=['/* Original MAME output fixtures; real native services, no mocked helpers. */',
       'typedef struct { logic_case_t base; uint16_t input_first,input_count,output_first,output_count; unsigned status; } native_service_case_t;']
for name,values in [('native_input_patches',inputs),('native_output_patches',outputs)]:lines.append('static const logic_patch_t '+name+'[]={'+','.join('{'+str(o)+','+str(v)+'}' for o,v in values)+'};')
lines.append('static const native_service_case_t native_service_cases[]={\n'+',\n'.join(cases)+'\n};')
out=ROOT/'build/native_conditions_fixture.h';out.parent.mkdir(exist_ok=True);out.write_text('\n'.join(lines)+'\n');print('Generated',len(cases),'real-service cases;',len(inputs),'input /',len(outputs),'original output patch bytes')
