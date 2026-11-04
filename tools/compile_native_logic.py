#!/usr/bin/env python3
"""Generate C fixtures from original MAME outputs, with verified sparse RAM deltas."""
import argparse,hashlib,json,sys
from pathlib import Path
from condition_harness import ROOT,ram_input
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--capture',type=Path,help='Regenerate sparse deltas from original condition-dispatch capture');a=p.parse_args()
fixture=json.loads((ROOT/'decomp/recovered/rom_01a5ca.validation.json').read_text())
patch_path=ROOT/'data/condition_ram_patches.json'
if a.capture:
    rows=[]
    for row in fixture['results']:
        case=row['input'];n=case['index'];before=ram_input(case);after=(a.capture/f'result-{n}.ram').read_bytes()
        assert len(after)==65536 and hashlib.sha256(after).hexdigest()==row['expected']['ram_sha256']
        rows.append(dict(index=n,patches=[[i,y] for i,(x,y) in enumerate(zip(before,after)) if x!=y]))
    patch_path.write_text(json.dumps(dict(source='Original MAME condition-dispatch-138 outputs; synthetic RAM only',cases=rows),separators=(',',':'))+'\n')
patches=json.loads(patch_path.read_text())['cases'];assert len(patches)==len(fixture['results'])
lines=['/* Generated from original M68000 output fixtures. No ROM program bytes. */',
 'typedef struct { uint16_t offset; uint8_t value; } logic_patch_t;',
 'typedef struct { uint32_t kind,d0,a0,sp,ret; uint16_t sr; } logic_call_t;',
 'typedef struct { msx_condition_registers input,expected; uint8_t opcode,relation,counters[9]; uint16_t threshold,x,y; uint32_t helper_d[8],query[2],ha0,ha2; uint16_t helper_sr,patch_first,patch_count,call_first,call_count; } logic_case_t;',
 'typedef struct { neo_player_query_slot_t slots[2]; neo_player_query_registers_t input,expected; unsigned status; } query_case_t;']
def nums(values):return '{'+','.join(f'UINT32_C(0x{x:x})' for x in values)+'}'
def regs(values,sr):return '{'+nums(values[:8])+','+nums(values[8:])+f',0x{sr:x}'+'}'
allpatch=[];allcalls=[];cases=[]
for row,patch in zip(fixture['results'],patches):
    c=row['input'];e=row['expected'];assert patch['index']==c['index'];ram=ram_input(c)
    for offset,value in patch['patches']:assert 0<=offset<65536 and 0<=value<256;ram[offset]=value
    assert hashlib.sha256(ram).hexdigest()==e['ram_sha256'],'Sparse patch fixture differs from original output hash'
    case='{'+regs(c['registers'],c['sr'])+','+regs(e['registers'],e['sr'])+f",{c['opcode']},{c['relation']},"+nums(c['counters'])+f",{c['threshold']},{c['x']},{c['y']},"+nums(c['helper_d'])+','+nums(c['query'])+f",{c['helper_a0']},{c['helper_a2']},{c['helper_sr']},{len(allpatch)},{len(patch['patches'])},{len(allcalls)},{len(e['calls'])}"+'}'
    cases.append(case);allpatch+=patch['patches'];allcalls+=e['calls']
assert len(allpatch)<65536 and len(allcalls)<65536
lines+=['static const logic_patch_t logic_patches[]={'+','.join('{'+str(o)+','+str(v)+'}' for o,v in allpatch)+'};']
lines+=['static const logic_call_t logic_calls[]={'+','.join('{'+','.join(str(c[k]) for k in ('kind','d0','a0','sp','ret','sr'))+'}' for c in allcalls)+'};']
lines+=['static const logic_case_t logic_cases[]={\n'+',\n'.join(cases)+'\n};']
queries=json.loads((ROOT/'data/player_query_fixture.json').read_text());qcases=[]
for row in queries['results']:
    c=row['input'];e=row['expected']
    slots='{'+','.join('{'+','.join(str(s[k]) for k in ('object_header','active','field18','field14','state21','field1f','field20'))+'}' for s in c['slots'])+'}'
    qcases.append('{'+slots+',{'+nums(c['registers'][:5])+f",{c['sr']},255"+'},{'+nums(e['registers'][:5])+f",{e['sr']},{e['selected_slot']}"+'},'+str(e['status'])+'}')
lines+=['static const query_case_t query_cases[]={\n'+',\n'.join(qcases)+'\n};']
positions=json.loads((ROOT/'data/player_positions_fixture.json').read_text());pcases=[]
lines+=['typedef struct { neo_player_positions_frame_t frame; neo_player_positions_registers_t input,expected; } position_case_t;']
for row in positions['results']:
    c=row['input'];e=row['expected'];f=c['frame']
    players='{'+','.join('{'+','.join(str(p[k]) for k in ('object_header','x','y','use_x','use_y'))+'}' for p in f['players'])+'}'
    frame='{'+players+f",{f['fallback_x']},{f['fallback_y']},{f['enabled']}"+'}'
    pcases.append('{'+frame+',{'+nums(c['registers'][:8])+f",{c['sr']}"+'},{'+nums(e['registers'][:8])+f",{e['sr']}"+'}}')
lines+=['static const position_case_t position_cases[]={\n'+',\n'.join(pcases)+'\n};']
out=ROOT/'build/native_logic_fixture.h';out.parent.mkdir(exist_ok=True);out.write_text('\n'.join(lines)+'\n')
print('Generated',len(cases),'condition cases,',len(qcases),'query cases;',len(allpatch),'original sparse RAM bytes;',len(pcases),'position cases')
