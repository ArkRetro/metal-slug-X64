#!/usr/bin/env python3
"""Compile original M68000 platform-event observations for host/MIPS tests."""
import json
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
data=json.loads((ROOT/'data/m3_platform_reward_fixture.json').read_text())
effects=json.loads((ROOT/'data/m3_platform_reward_effects_fixture.json').read_text())
assert effects['score_effects'] is True and len(effects['cases'])==len(data['cases'])
assert effects['rom_cpu_order_sha256']==data['rom_cpu_order_sha256']
arr=lambda values:'{'+','.join(str(v) for v in values)+'}'
record=lambda value:arr(bytes.fromhex(value))
lines=['/* Generated independent M68000 observations; not game assets. */',
    '#ifndef M3_REWARD_FIXTURE_H','#define M3_REWARD_FIXTURE_H',
    '#include <stdint.h>','#include <stdbool.h>',
    'typedef struct {uint16_t player,d1;uint32_t tag,value,x,y,px,py;} m3_reward_request_t;',
    'static const struct {const char *name;uint8_t before[192],owner[192],after[192];',
    'int32_t rx,ry,old_rx,old_ry;uint8_t correction,epoch,mask;bool preserve,deleted;',
    'struct {uint16_t x,y;uint8_t flags;} player[2];unsigned count;m3_reward_request_t requests[2];',
    'struct {uint8_t after[192],score_before[2][4],score[2][4];unsigned sound_count;uint16_t sounds[2];',
    'unsigned label_count;uint8_t labels[2][192];unsigned allocation_count;',
    'struct {uint16_t reserve;uint32_t routine,creator;} allocations[2];} effects;} m3_reward_cases[]={']
for case,effect in zip(data['cases'],effects['cases']):
    src=case['input'];dst=case['expected'];root=src['root']
    esrc=effect['input'];edst=effect['expected']
    assert effect['name']==case['name'] and esrc['actor_hex']==src['actor_hex']
    assert edst['deleted']==dst['deleted'] and edst['requests']==dst['requests']
    assert len(edst['sounds'])==len(edst['labels'])==len(edst['allocations'])==len(dst['requests'])
    for allocation in edst['allocations']:
        assert allocation==dict(reserve=0xe,routine=0x8d330,creator=0x101414)
    assert len(dst['requests'])<=2
    players='{'+','.join(arr([p['x'],p['y'],p['flags15']]) for p in src['players'])+'}'
    requests=[]
    for r in dst['requests']:
        requests.append(arr([r['player'],r['d1'],r['kind_tag'],
            (r['kind_words'][0]<<16)|r['kind_words'][1],
            r['actor_x'],r['actor_y'],r['projected_x'],r['projected_y']]))
    requests+=['{0}']*(2-len(requests))
    labels=[record(label['actor_hex']) for label in edst['labels']]
    labels+=['{0}']*(2-len(labels))
    allocations=[arr([r['reserve'],r['routine'],r['creator']]) for r in edst['allocations']]
    allocations+=['{0}']*(2-len(allocations))
    effect_fields=[record(edst['actor_hex']),'{'+','.join(record(s) for s in esrc['score_hex'])+'}',
        '{'+','.join(record(s) for s in edst['score_hex'])+'}',str(len(edst['sounds'])),
        arr(edst['sounds']+[0]*(2-len(edst['sounds']))),str(len(edst['labels'])),
        '{'+','.join(labels)+'}',str(len(edst['allocations'])),'{'+','.join(allocations)+'}']
    fields=[json.dumps(case['name']),record(src['actor_hex']),record(src['creator_hex']),
        record(dst['actor_hex']),str(root['x']),str(root['y']),str(root['prev_x']),str(root['prev_y']),
        str(root['correction']),str(src['epoch']),str(src['active_mask']),
        str(int(src['preserve_previous'])),str(int(dst['deleted'])),players,
        str(len(dst['requests'])),'{'+','.join(requests)+'}','{'+','.join(effect_fields)+'}']
    lines.append('{'+','.join(fields)+'},')
lines+=['};','#endif']
out=ROOT/'build/m3_reward_fixture.h';out.parent.mkdir(exist_ok=True)
out.write_text('\n'.join(lines)+'\n')
print('Compiled',len(data['cases']),'original platform reward calls and full score/label effects')
