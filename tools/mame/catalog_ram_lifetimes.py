#!/usr/bin/env python3
"""Document concrete static writes that invalidate a different RAM code image.

This transcribes a bounded initializer prefix, not a CPU interpreter or a
claim that the prefix executed during gameplay.
"""
import argparse
import hashlib
import json
from pathlib import Path

if __package__:
    from .compare_context_decoders import parse_assembly
else:
    from compare_context_decoders import parse_assembly

ROOT = Path(__file__).resolve().parents[2]


def catalog(base):
    evidence = base / 'decoder'
    report = json.loads((evidence / 'validation.json').read_text())
    layout = json.loads((base / 'layout.json').read_text())
    contexts = {c['id']: c for c in layout['contexts']}
    decoded = {}
    for group in report['groups']:
        if not group['context'].startswith('RAM_COPY_'):
            continue
        path = evidence / group['file']
        assert path.resolve().is_relative_to(evidence.resolve())
        assert hashlib.sha256(path.read_bytes()).hexdigest() == report['assembly_sha256'][group['file']]
        decoded.setdefault(group['context'], {}).update(parse_assembly(path, 'm68000'))
    source = decoded['RAM_COPY_100000']
    target = contexts['RAM_COPY_10FC00']
    # LEA absolute,A5; LEA d16(A5),A4; MOVE.L #imm,(A4);
    # LEA d16(A4),A4; MOVE.L #imm,(A4). Preserve actual encoded immediates.
    required = {0x100000: ('4bf9', 6), 0x10003a: ('49ed', 4),
                0x10003e: ('28bc', 6), 0x100056: ('49ec', 4),
                0x10005a: ('28bc', 6)}
    for pc, (prefix, size) in required.items():
        data = source[pc]['bytes']
        assert data.hex().startswith(prefix) and len(data) == size
    a5 = int.from_bytes(source[0x100000]['bytes'][2:], 'big')
    a4 = a5 + int.from_bytes(source[0x10003a]['bytes'][2:], 'big', signed=True)
    initial_slot = a4
    writes = []
    for pc in (0x10003e, 0x10005a):
        if pc == 0x10005a:
            a4 += int.from_bytes(source[0x100056]['bytes'][2:], 'big', signed=True)
        value = source[pc]['bytes'][2:]
        overlaps = []
        for instruction_pc, instruction in decoded['RAM_COPY_10FC00'].items():
            data = instruction['bytes']
            start, end = max(a4, instruction_pc), min(a4 + len(value), instruction_pc + len(data))
            if start >= end:
                continue
            overlaps.append(dict(location=f'RAM_COPY_10FC00:{instruction_pc:08x}',
                                 instruction=instruction['text'],
                                 write_interval=[start, end],
                                 original_bytes=data[start-instruction_pc:end-instruction_pc].hex(),
                                 replacement_bytes=value[start-a4:end-a4].hex()))
        assert target['cpu_base'] <= a4 < a4 + len(value) <= target['cpu_base'] + target['size']
        assert overlaps and any(r['original_bytes'] != r['replacement_bytes'] for r in overlaps)
        writes.append(dict(store_site=f'RAM_COPY_100000:{pc:08x}', runtime_address=a4,
                           value_be_hex=value.hex(), overlapping_initial_code=overlaps,
                           consequence='Initial RAM_COPY_10FC00 image cannot be reused after this write without restoration/version change'))
    return dict(status='STATIC_RAM_CODE_LIFETIME_CONFLICTS_NOT_EXECUTION_PROOF',
                layout_sha256=hashlib.sha256((base / 'layout.json').read_bytes()).hexdigest(),
                decoder_validation_sha256=hashlib.sha256((evidence / 'validation.json').read_bytes()).hexdigest(),
                initializer='RAM_COPY_100000:00100000',
                prefix_end='RAM_COPY_100000:0010005a',
                derived_a5=a5, initial_descriptor_address=initial_slot,
                writes=writes,
                limits='Conditional on straight-line execution of this initializer prefix with the captured initial bytes and no intervening register clobber. Interrupt preservation is not proven here. No natural run or complete write/alias analysis is claimed. Two static copy images are distinct lifetimes, not simultaneously valid immutable code.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--base', type=Path, default=ROOT / 'decomp/canonical-model')
    parser.add_argument('--output', type=Path, default=ROOT / 'decomp/canonical-model/ram-lifetimes.json')
    args = parser.parse_args()
    result = catalog(args.base)
    args.output.write_text(json.dumps(result, indent=2) + '\n')
    print('Documented', len(result['writes']), 'conditional RAM code-image invalidations')
