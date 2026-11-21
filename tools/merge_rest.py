#!/usr/bin/env python3
"""Add the descriptors of some programs to a mission's rest-native export.

tools/export_rest_sprites.py writes every descriptor its ROOTS pose (the
bosses of other missions too). A mission's private export
(filesystem/missionN/rest-native.json) holds the previous export plus only
the descriptors the newly translated programs pose (docs/MISION6.md), so the
mission pack does not grow with frames it never draws:

  merge_rest.py FULL.json PREVIOUS.json OUTPUT.json --roots 23c0de,23c212,... [--palettes fd,ff,...]

FULL is export_rest_sprites.py's output for the new captures (--keep
PREVIOUS). Frames: PREVIOUS's, then FULL's frames of the descriptors posed
(POSE commands, data/soldier_programs.json) by --roots that PREVIOUS lacks.
Palettes: PREVIOUS's, plus FULL's for the ids PREVIOUS lacks (only --palettes
when given). Nothing of PREVIOUS changes."""
import argparse, json
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def main():
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('full', type=Path); p.add_argument('previous', type=Path); p.add_argument('output', type=Path)
    p.add_argument('--roots', required=True); p.add_argument('--palettes')
    a = p.parse_args()
    roots = {int(x, 16) for x in a.roots.split(',')}
    progs = json.loads((ROOT / 'data/soldier_programs.json').read_text())['programs']
    known = {q['root'] for q in progs}
    assert roots <= known, 'not in data/soldier_programs.json: ' + ' '.join('%06X' % r for r in sorted(roots - known))
    posed = {hex(c['asset']) for q in progs if q['root'] in roots for c in q['commands'] if c['kind'] == 'POSE'}
    full = json.loads(a.full.read_text()); prev = json.loads(a.previous.read_text())
    assert full['p2_sha256'] == prev['p2_sha256']
    have = {f['descriptor'] for f in prev['frames']}
    new = [f for f in full['frames'] if f['descriptor'] in posed and f['descriptor'] not in have]
    empty = sorted(set(prev.get('empty', [])) | (set(full.get('empty', [])) & posed), key=lambda x: int(x, 16))
    lost = posed - have - {f['descriptor'] for f in new} - set(empty)
    assert not lost, 'posed descriptors FULL does not hold: ' + ' '.join(sorted(lost))
    want = {hex(int(x, 16)) for x in a.palettes.split(',')} if a.palettes else None
    pal = dict(prev['palettes']); added = []
    for k, v in full['palettes'].items():
        if k not in pal and (want is None or k in want):
            pal[k] = v; added.append(k)
    if want:
        assert want <= set(pal), 'palettes not in FULL: ' + ' '.join(sorted(want - set(pal)))
    pal = dict(sorted(pal.items(), key=lambda kv: int(kv[0], 16)))
    out = dict(prev, frames=prev['frames'] + new, palettes=pal, empty=empty,
               missing_palettes=[m for m in prev.get('missing_palettes', []) if m not in pal])
    a.output.write_text(json.dumps(out, separators=(',', ':')) + '\n')
    print(json.dumps(dict(frames=len(out['frames']), added_frames=len(new), posed=len(posed), palettes=len(pal), added_palettes=added)))


if __name__ == '__main__':
    main()
