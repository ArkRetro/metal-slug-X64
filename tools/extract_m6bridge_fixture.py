#!/usr/bin/env python3
"""Fixture of the Mission 6 bridge driver (src/neo_m6bridge.c) from a family
capture with the owners of 10D7B6 (docs/MISION6.md, captures-m6/fam/R:
boss-trace.txt.gz). One record per update of 0BCE2C: what it read
(10E970, 10C706, 10C70A, 10C704, 10C702, 10D7E8 +32, 10D7EA +34, 10D7EC +36,
10C70C) and what it wrote (10C706, 10E94E, +32, +34, 10C704, 10C70C; None:
not written).

  python3 tools/extract_m6bridge_fixture.py CAPTURE/boss-trace.txt.gz [--out data/m6bridge_fixture.json.gz]
"""
import argparse, gzip, json
IN = {'e970': (0x10e970, 4), 'c706': (0x10c706, 4), 'c70a': (0x10c70a, 1), 'c704': (0x10c704, 2), 'c702': (0x10c702, 2),
      'o32': (0x10d7e8, 2), 'o34': (0x10d7ea, 2), 'o36': (0x10d7ec, 2), 'c70c': (0x10c70c, 2)}
OUT = {'c706': (0x10c706, 4), 'e94e': (0x10e94e, 4), 'o32': (0x10d7e8, 2), 'o34': (0x10d7ea, 2), 'c704': (0x10c704, 2), 'c70c': (0x10c70c, 2)}


def value(m, a, n):
    if n == 1:
        return m[a] >> 8 if a in m else None
    if n == 2:
        return m.get(a)
    return (m[a] << 16 | m[a + 2]) if a in m and a + 2 in m else None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('trace'); ap.add_argument('--out', default='data/m6bridge_fixture.json.gz')
    a = ap.parse_args()
    ups, cur = [], None
    for line in gzip.open(a.trace, 'rt'):
        t = line.split()
        if not t:
            continue
        if t[0] == 'U':
            cur = {'r': {}, 'w': {}} if t[2] == 'bce2c' else None
            if cur is not None:
                ups.append(cur)
        elif cur is not None and t[0] in 'RW':
            addr, v = int(t[1], 16), int(t[3], 16)
            if t[0] == 'R':
                cur['r'].setdefault(addr, v)   # the first read: the value before the update
            else:
                cur['w'][addr] = v
    recs = [{'in': {k: value(u['r'], *IN[k]) for k in IN}, 'out': {k: value(u['w'], *OUT[k]) for k in OUT}} for u in ups]
    with gzip.open(a.out, 'wt') as f:
        json.dump({'source': a.trace, 'updates': recs}, f, separators=(',', ':'))
    print(f'{a.out}: {len(recs)} updates of 0BCE2C')


if __name__ == '__main__':
    main()
