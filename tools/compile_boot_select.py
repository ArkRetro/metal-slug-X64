#!/usr/bin/env python3
"""Compile original-MAME boot-selector outputs into portable C/MIPS fixtures."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
fixture = json.loads((ROOT / 'data/boot_select_fixture.json').read_text())
assert fixture['status'] == 'PASS_ORIGINAL_BOOT_SELECTOR_WITH_SYNTHETIC_PLATFORM_IMPORTS'
for name, expected in fixture['source_sha256'].items():
    assert hashlib.sha256((ROOT / name).read_bytes()).hexdigest() == expected
layout = json.loads((ROOT / 'decomp/canonical-model/layout.json').read_text())
assert fixture['code_sha256'] == next(c['sha256'] for c in layout['contexts'] if c['id'] == 'RAM_COPY_10FC00')


def registers(regs, sr):
    return '{{' + ','.join(f'0x{x:08x}u' for x in regs[:8]) + '},{' + ','.join(f'0x{x:08x}u' for x in regs[8:]) + '},' + f'0x{sr:04x}u' + '}'


def header(h):
    return '{' + f'0x{h["magic"]:08x}u,{h["product"]}u,{h["product_later"]}u,{h["flag"]}u' + '}'


lines = [
    '/* Generated from original MAME CPU outputs with synthetic platform imports. */',
    '#include "../decomp/recovered/ram_10fc00.h"',
    'typedef struct { uint32_t magic; uint16_t product,product_later; uint8_t flag; } boot_header_t;',
    'typedef struct { uint8_t slot; boot_header_t header; } boot_override_t;',
    'typedef struct { msx_boot_select_registers input,expected; boot_header_t header; uint32_t io_count,io_hash; uint16_t first,count; uint8_t scan_count,previous_slot,nv_result; } boot_case_t;',
    'static const boot_override_t boot_overrides[] = {',
]
overrides = []
cases = []
for row in fixture['results']:
    c = row['input']
    first = len(overrides)
    overrides.extend(c['overrides'])
    cases.append('{' + registers(c['registers'], c['sr']) + ',' +
                 registers(row['expected']['registers'], row['expected']['sr']) + ',' + header(c['default']) + ',' +
                 f'{row["io_events"]}u,0x{row["io_fnv32"]:08x}u,{first}u,{len(c["overrides"])}u,{c["count"]}u,{c["previous_slot"]}u,{row["nv_result"]}u' + '},')
lines.extend('{' + str(h['slot']) + 'u,' + header(h) + '},' for h in overrides)
lines += ['};', 'static const boot_case_t boot_cases[] = {'] + cases + ['};', f'#define BOOT_SELECT_CASES {len(cases)}u']
assert len(cases) == fixture['cases']
out = ROOT / 'build/boot_select_fixture.h'
out.parent.mkdir(exist_ok=True)
out.write_text('\n'.join(lines) + '\n')
print('Compiled', len(cases), 'original boot-selector fixtures')
