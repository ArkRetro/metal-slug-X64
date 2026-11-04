#!/usr/bin/env python3
"""Seed only observed RTS leaves with matching owned P1 bytes and call linkage."""
import argparse
import csv
import hashlib
import json
from pathlib import Path

if __package__:
    from .analyze_program_context_run import events_from_log, inspect_published
else:
    from analyze_program_context_run import events_from_log, inspect_published

ROOT = Path(__file__).resolve().parents[2]
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--inputs', type=Path, required=True)
parser.add_argument('--output', type=Path, required=True)
args = parser.parse_args()
base = ROOT / 'decomp/canonical-model'
folder = base / 'runtime/euro-start-mission1'
inspect_published(folder, base)
program = json.loads((ROOT / 'decomp/program-set.json').read_text())['programs'][0]
assert program['source'] == '250-p1.p1'
p1 = (args.inputs / 'm68k-p1.bin').read_bytes()
assert hashlib.sha256(p1).hexdigest() == program['cpu_order_sha256']
events = events_from_log(folder / 'debug.log')
seeds = {}
for index, event in enumerate(events):
    if event['kind'] != 'irq_idle_entry':
        continue
    pc = event['site']
    assert pc % 2 == 0 and 0x100 <= pc < len(p1) - 1
    assert event['opcode'] == 0x4e75 and p1[pc:pc+2] == bytes.fromhex('4e75')
    links = [e for e in events[:index] if e['kind'] == 'irq_target_change'
             and e['target'] == pc and e['site'] + 2 == event['return_pc']]
    assert links, 'No observed indirect call linking to this leaf'
    evidence = folder / 'debug.log'
    seeds[pc] = dict(context='M68K_FIXED_P1', pc=f'{pc:08x}', opcode='4e75',
                     evidence_file=str(evidence.relative_to(ROOT)),
                     evidence_sha256=hashlib.sha256(evidence.read_bytes()).hexdigest(),
                     status='observed_RTS_leaf_model_seed_not_native_C')
with args.output.open('w') as stream:
    writer = csv.DictWriter(stream, fieldnames=['context', 'pc', 'opcode', 'evidence_file', 'evidence_sha256', 'status'], delimiter='\t', lineterminator='\n')
    writer.writeheader()
    writer.writerows(seeds[pc] for pc in sorted(seeds))
print('Prepared', len(seeds), 'observed leaf seeds; no unobserved RAM values promoted')
