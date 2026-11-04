#!/usr/bin/env python3
"""Check published context evidence and enumerate candidate flow frontiers.

This checks structural integrity, not instruction validity or native equivalence.
"""
import argparse
import csv
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def inspect(base):
    def digest(path):
        return hashlib.sha256(path.read_bytes()).hexdigest()

    layout = json.loads((base / 'layout.json').read_text())
    report = json.loads((base / 'model-validation.json').read_text())
    roots = json.loads((base / 'context-roots.json').read_text())
    assert report['layout_sha256'] == roots['layout_sha256'] == digest(base / 'layout.json')
    assert report['mapping_validation_sha256'] == digest(base / 'mapping-validation.json')
    contexts = {c['id']: c for c in layout['contexts']}
    assert len(contexts) == len(layout['contexts']) == report['contexts']
    all_roots, frontier = [], []
    occupied = {name: set() for name in contexts}
    models = {}
    for program in report['programs']:
        folder = base / program['program']
        assert folder.resolve().is_relative_to(base.resolve())
        tables = {}
        for filename, expected in program['files'].items():
            path = folder / filename
            assert path.resolve().is_relative_to(folder.resolve())
            assert digest(path) == expected, str(path)
            with path.open() as stream:
                tables[filename] = list(csv.DictReader(stream, delimiter='\t'))
        functions = {r['entry'] for r in tables['functions.tsv']}
        assert len(functions) == len(tables['functions.tsv']) == program['candidate_functions']
        for row in tables['instructions.tsv']:
            context, pc = row['location'].split(':')
            c = contexts[context]
            address, size = int(pc, 16), int(row['length'])
            assert c['cpu_base'] <= address < address + size <= c['cpu_base'] + c['size']
            assert row['owner'] in functions
            occupied[context].update(range(address, address + size))
            assert row['location'] not in models, 'Duplicate instruction location'
            models[row['location']] = row['owner']
        for row in tables['roots.tsv']:
            assert row['instruction_present'] == 'true'
            assert models.get(row['location']) == row['owner']
            assert row['owner'] in functions
            assert row['location'] == row['context'] + ':' + row['pc']
        all_roots.extend(tables['roots.tsv'])
        for row in tables['edges.tsv']:
            assert row['owner'] in functions and row['site'] in models
            if row['kind'].startswith('computed') or row['target_owner'] not in functions:
                frontier.append(dict(program=program['program'], **row,
                                     disposition='unresolved_in_this_candidate_model'))
    assert all_roots == roots['roots']
    assert len(all_roots) == report['seeded_context_roots']
    required = {(c['id'], pc) for c in contexts.values() for pc in c['root_pcs']}
    actual = {(r['context'], int(r['pc'], 16)) for r in all_roots}
    assert required == actual
    return dict(
        status='STRUCTURAL_INTEGRITY_PASS_NATIVE_RECOVERY_OPEN',
        roots=all_roots,
        candidate_contexts=[dict(context=name, container_bytes=c['size'],
                                 candidate_instruction_bytes=len(occupied[name]),
                                 bytes_without_candidate_instruction=c['size'] - len(occupied[name]),
                                 verified_code_data_partition=False)
                            for name, c in contexts.items()],
        candidate_flow_frontier=frontier,
        limits='Candidate instructions are not verified code; remaining bytes are not proven data. '
               'A flow missing in this graph may exist in another recovery artifact. '
               'Roots and byte totals do not measure whole-game completion.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--base', type=Path, default=ROOT / 'decomp/canonical-model')
    parser.add_argument('--output', type=Path)
    args = parser.parse_args()
    result = inspect(args.base)
    if args.output:
        args.output.write_text(json.dumps(result, indent=2) + '\n')
    print(result['status'], '; roots=', len(result['roots']),
          '; candidate frontier=', len(result['candidate_flow_frontier']))
