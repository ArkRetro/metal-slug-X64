#!/usr/bin/env python3
"""Replay structural checks on published static MAME decoder evidence."""
import argparse
import collections
import csv
import hashlib
import json
from pathlib import Path

if __package__:
    from .compare_context_decoders import parse_assembly
else:
    from compare_context_decoders import parse_assembly

ROOT = Path(__file__).resolve().parents[2]


def inspect(base):
    evidence = base / 'decoder'
    report = json.loads((evidence / 'validation.json').read_text())

    def digest(path):
        return hashlib.sha256(path.read_bytes()).hexdigest()

    assert report['layout_sha256'] == digest(base / 'layout.json')
    layout = json.loads((base / 'layout.json').read_text())
    contexts = {c['id']: c for c in layout['contexts']}
    candidate = {}
    for name, expected in report['candidate_inputs_sha256'].items():
        # Published reports retain repository-relative input names.
        path = base / Path(name).relative_to('decomp/canonical-model')
        assert path.resolve().is_relative_to(base.resolve())
        assert digest(path) == expected
        for row in csv.DictReader(path.open(), delimiter='\t'):
            assert row['location'] not in candidate
            candidate[row['location']] = row
    decoded = {}
    for group in report['groups']:
        path = evidence / group['file']
        assert path.resolve().is_relative_to(evidence.resolve())
        assert digest(path) == report['assembly_sha256'][group['file']]
        cpu = contexts[group['context']]['cpu']
        for pc, row in parse_assembly(path, cpu).items():
            assert group['start'] <= pc < group['end']
            location = group['context'] + ':' + f'{pc:08x}'
            assert location not in decoded
            decoded[location] = row
    for context in report['contexts']:
        c = contexts[context['context']]
        assert context['size'] == c['size'] and context['readback_sha256'] == c['sha256']
        assert context['ram_image_installed'] == c['id'].startswith('RAM_COPY_')
    comparison = evidence / 'comparison.tsv'
    assert digest(comparison) == report['comparison_sha256']
    rows = list(csv.DictReader(comparison.open(), delimiter='\t'))
    assert len(rows) == len(candidate) == report['candidate_instructions']
    assert {r['location'] for r in rows} == candidate.keys()
    mismatches = []
    aliases = {'movea': 'move', 'dbf': 'dbra', 'bhs': 'bcc', 'blo': 'bcs'}

    def mnemonic(text):
        word = text.split()[0].lower().split('.')[0]
        return aliases.get(word, word)

    for row in rows:
        original = decoded.get(row['location'])
        for key, value in candidate[row['location']].items():
            assert row[key] == value
        if row['status'] == 'boundary_length_match':
            assert original is not None
            assert len(original['bytes']) == int(row['length']) == int(row['mame_length'])
            assert original['text'] == row['mame_instruction']
            if mnemonic(row['instruction']) != mnemonic(row['mame_instruction']):
                mismatches.append(row['location'])
    counts = dict(collections.Counter(r['status'] for r in rows))
    assert counts == report['statuses']
    stores = base / 'M68K_FIXED_P1/descriptor-seeds.tsv'
    store_count = 0
    if stores.is_file():
        for row in csv.DictReader(stores.open(), delimiter='\t'):
            pointer = int(row['target'].split(':')[1], 16)
            assert decoded[row['store_site']]['bytes'] == bytes.fromhex('28bc') + pointer.to_bytes(4, 'big')
            assert row['target'] in decoded
            store_count += 1
    return dict(status='STATIC_DECODER_EVIDENCE_INTEGRITY_PASS', candidate_instructions=len(rows),
                statuses=counts, normalized_mnemonic_differences=mismatches,
                descriptor_store_encodings_matched=store_count,
                limits='Matching boundary/length/mnemonic does not prove operand semantics, code reachability, native C equivalence or game completion.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--base', type=Path, default=ROOT / 'decomp/canonical-model')
    args = parser.parse_args()
    print(json.dumps(inspect(args.base), indent=2))
