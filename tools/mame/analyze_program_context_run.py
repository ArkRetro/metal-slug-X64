#!/usr/bin/env python3
"""Publish bounded natural-execution evidence, preserving unobserved domains."""
import argparse
import collections
import csv
import hashlib
import json
import re
import shutil
from pathlib import Path

if __package__:
    from .compare_context_decoders import parse_assembly
else:
    from compare_context_decoders import parse_assembly

ROOT = Path(__file__).resolve().parents[2]


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def events_from_log(path):
    events = []
    for line in path.read_text().splitlines():
        if not line.startswith('CTX '):
            continue
        fields = dict(re.findall(r'([a-z0-9_]+)=([^\s]+)', line))
        events.append({key: value if key == 'kind' else int(value, 10 if key == 'frame' else 16)
                       for key, value in fields.items()})
    return events


def indirect_observations(events):
    return [
        dict(site=f'0x{site:06X}', observed_targets=[f'0x{target:06X}' for target in sorted({e['target'] for e in events if e['kind'] == 'irq_target_change' and e['site'] == site})],
             exhaustive=False, event_scope='Logged on target change, not every call')
        for site in (0x22da, 0x2354)
    ]


def trace_observation(path, name, context, canonical):
    executed = []
    for line in path.read_text().splitlines():
        match = re.match(r'^FRAME=(\d+) A4=[0-9A-F]+ A0=[0-9A-F]+ SR=[0-9A-F]+ ([0-9A-F]{6}): (.*)$', line)
        if not match:
            continue
        pc = int(match[2], 16)
        if context['cpu_base'] <= pc < context['cpu_base'] + context['size']:
            assert pc in canonical, ('Executed RAM instruction not in static model', name, hex(pc))
            assert ' '.join(match[3].split()).lower() == ' '.join(canonical[pc]['text'].split()).lower()
            executed.append(pc)
    return dict(context=name, file=path.name, ram_instruction_records=len(executed),
                distinct_ram_addresses=[f'0x{pc:06X}' for pc in sorted(set(executed))],
                trace_scope='First-copy breakpoint until next frame_done only; does not cover every activation')


def analyze(capture, out, base):
    out.mkdir(parents=True, exist_ok=False)
    source = json.loads((capture / 'capture.json').read_text())
    assert source['status'] == 'ORIGINAL_INPUT_ONLY_CONTEXT_CAPTURE'
    assert source['debugger_log_sha256'] == sha(capture / 'debug.log')
    assert source['inputs_sha256'] == sha(capture / 'inputs.jsonl')
    assert source['script_sha256'] == sha(capture / 'capture.lua')
    layout = json.loads((base / 'layout.json').read_text())
    contexts = {c['id']: c for c in layout['contexts'] if c['id'].startswith('RAM_COPY_')}
    events = events_from_log(capture / 'debug.log')
    counts = dict(collections.Counter(e['kind'] for e in events))
    assert events[-1]['kind'] == 'capture_end' and events[-1]['frame'] == source['frames']
    for name in ('debug.log', 'capture.lua', 'capture.json', 'inputs.jsonl'):
        shutil.copy2(capture / name, out / name)
    (out / 'events.jsonl').write_text(''.join(json.dumps(e, sort_keys=True) + '\n' for e in events))
    matches = {name: [] for name in contexts}
    with (out / 'frames.tsv').open('w') as stream:
        fields = ['frame', 'ram_sha256'] + [name + '_sha256' for name in contexts]
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter='\t', lineterminator='\n')
        writer.writeheader()
        for frame in range(1, source['frames'] + 1):
            data = (capture / f'ram/{frame:04d}.bin').read_bytes()
            assert len(data) == 0x10000
            row = dict(frame=frame, ram_sha256=hashlib.sha256(data).hexdigest())
            for name, context in contexts.items():
                offset = context['cpu_base'] - 0x100000
                digest = hashlib.sha256(data[offset:offset + context['size']]).hexdigest()
                row[name + '_sha256'] = digest
                if digest == context['sha256']:
                    matches[name].append(frame)
            writer.writerow(row)
    observations = []
    for name, label, suffix in (('RAM_COPY_100000', 'ram_100000', '100000'), ('RAM_COPY_10FC00', 'ram_10fc00', '10fc00')):
        entries = [e for e in events if e['kind'] == label]
        snapshot = capture / ('last-entry-' + suffix + '.bin')
        digest = None
        if entries:
            assert all(e['site'] == contexts[name]['cpu_base'] for e in entries)
            assert snapshot.stat().st_size == contexts[name]['size']
            digest = sha(snapshot)
            assert digest == contexts[name]['sha256'], ('Wrong code version at last entry', name)
        else:
            assert not snapshot.exists()
        observations.append(dict(context=name, entry_count=len(entries),
                                 entry_frames=[e['frame'] for e in entries],
                                 last_entry_image_sha256=digest,
                                 frame_end_image_matches=len(matches[name]),
                                 first_frame_end_match=matches[name][0] if matches[name] else None,
                                 last_frame_end_match=matches[name][-1] if matches[name] else None,
                                 execution='observed_at_breakpoint_site' if entries else 'not_observed_in_this_run',
                                 native_domain_closed=False))
    canonical = {}
    static = json.loads((base / 'decoder/validation.json').read_text())
    for group in static['groups']:
        if not group['context'].startswith('RAM_COPY_'):
            continue
        path = base / 'decoder' / group['file']
        assert sha(path) == static['assembly_sha256'][group['file']]
        canonical[group['context']] = canonical.get(group['context'], {}) | parse_assembly(path, 'm68000')
    traces = []
    for name, filename in (('RAM_COPY_100000', 'copy_100000.log'), ('RAM_COPY_10FC00', 'copy_10fc00.log')):
        path = capture / filename
        if not path.exists():
            continue
        shutil.copy2(path, out / filename)
        traces.append(trace_observation(path, name, contexts[name], canonical[name]))
    files = {p.name: sha(p) for p in out.iterdir() if p.is_file()}
    report = dict(status='BOUNDED_ORIGINAL_RAM_CONTEXT_EXECUTION_EVIDENCE',
                  bios=source['bios'], completed_frames=source['frames'],
                  frame_convention='Event frame is the last completed frame_done counter; RAM frame N is read at the end of N. reported_pc is the CPU debugger register, not the breakpoint instruction site.',
                  layout_sha256=sha(base / 'layout.json'), files=files,
                  event_counts=counts, contexts=observations, traces=traces,
                  indirect_call_observations=indirect_observations(events),
                  native_domain_closed=False, whole_program_decompiled=False,
                  limits='Input-only observation, not injected code execution or native C equivalence. No observation is not a proof of unreachability. Frame-end presence alone is not execution; last-entry files certify only the last activation image. Instruction traces cover bounded first-copy intervals. Original instruction semantics and all runtime mutation/alias domains remain open.')
    (out / 'observations.json').write_text(json.dumps(report, indent=2) + '\n')
    return report


def inspect_published(folder, base):
    report = json.loads((folder / 'observations.json').read_text())
    assert report['layout_sha256'] == sha(base / 'layout.json')
    assert not report['native_domain_closed'] and not report['whole_program_decompiled']
    for filename, digest in report['files'].items():
        path = folder / filename
        assert path.resolve().is_relative_to(folder.resolve())
        assert sha(path) == digest
    capture = json.loads((folder / 'capture.json').read_text())
    assert capture['frames'] == report['completed_frames'] and capture['bios'] == report['bios']
    for filename, field in (('debug.log', 'debugger_log_sha256'), ('inputs.jsonl', 'inputs_sha256'), ('capture.lua', 'script_sha256')):
        assert sha(folder / filename) == capture[field]
    events = events_from_log(folder / 'debug.log')
    assert events == [json.loads(line) for line in (folder / 'events.jsonl').read_text().splitlines()]
    assert dict(collections.Counter(e['kind'] for e in events)) == report['event_counts']
    assert report.get('indirect_call_observations', indirect_observations(events)) == indirect_observations(events)
    assert events[-1]['kind'] == 'capture_end' and events[-1]['frame'] == report['completed_frames']
    assert all(0 <= e['frame'] <= report['completed_frames'] for e in events)
    layout = json.loads((base / 'layout.json').read_text())
    contexts = {c['id']: c for c in layout['contexts']}
    with (folder / 'frames.tsv').open() as stream:
        frames = list(csv.DictReader(stream, delimiter='\t'))
    assert [int(r['frame']) for r in frames] == list(range(1, report['completed_frames'] + 1))
    for row in frames:
        assert all(re.fullmatch('[0-9a-f]{64}', v) for k, v in row.items() if k != 'frame')
    for context in report['contexts']:
        name = context['context']
        kind = 'ram_100000' if name == 'RAM_COPY_100000' else 'ram_10fc00'
        entries = [e for e in events if e['kind'] == kind]
        assert len(entries) == context['entry_count']
        assert [e['frame'] for e in entries] == context['entry_frames']
        assert all(e['site'] == contexts[name]['cpu_base'] for e in entries)
        expected = contexts[name]['sha256']
        matches = [int(r['frame']) for r in frames if r[name + '_sha256'] == expected]
        assert len(matches) == context['frame_end_image_matches']
        assert (matches[0] if matches else None) == context['first_frame_end_match']
        assert (matches[-1] if matches else None) == context['last_frame_end_match']
        assert context['last_entry_image_sha256'] == (expected if entries else None)
        assert context['execution'] == ('observed_at_breakpoint_site' if entries else 'not_observed_in_this_run')
        assert not context['native_domain_closed']
    static = json.loads((base / 'decoder/validation.json').read_text())
    for trace in report['traces']:
        canonical = {}
        for group in static['groups']:
            if group['context'] == trace['context']:
                path = base / 'decoder' / group['file']
                assert sha(path) == static['assembly_sha256'][group['file']]
                canonical.update(parse_assembly(path, 'm68000'))
        assert trace == trace_observation(folder / trace['file'], trace['context'], contexts[trace['context']], canonical)
    return report


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture', type=Path)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--base', type=Path, default=ROOT / 'decomp/canonical-model')
    parser.add_argument('--verify-published', action='store_true')
    args = parser.parse_args()
    if not args.verify_published and args.capture is None:
        parser.error('--capture is required unless --verify-published is selected')
    result = inspect_published(args.output, args.base) if args.verify_published else analyze(args.capture, args.output, args.base)
    print(json.dumps(result['contexts'], indent=2))
