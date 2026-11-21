#!/usr/bin/env python3
"""Convert a BizHawk Arcade/MAME 'Input Log.txt' (1 player Metal Slug X TAS,
TASVideos #6663) into one input mask per frame for tools/tas_play.lua.

The movie is untrusted data: run as `python3 -I tools/tas_convert.py LOG OUT`
from outside its directory. Only the expected characters are accepted.

LogKey: #Coin 1|Coin 2|Service 1|Service Mode|1 Player Start|2 Players Start|
        #P1 A|P1 B|P1 C|P1 D|P1 Down|P1 Left|P1 Right|P1 Up|#P2 ...
Line k (0-based) of the [Input] block after the LogKey is BizHawk frame k.
Output: one hex mask per line (frame order); bits 0..5 the system columns,
bits 8..15 the P1 columns, bits 16..23 the P2 columns, in LogKey order.
"""
import sys

KEY = ('LogKey:#Coin 1|Coin 2|Service 1|Service Mode|1 Player Start|2 Players Start|'
       '#P1 A|P1 B|P1 C|P1 D|P1 Down|P1 Left|P1 Right|P1 Up|'
       '#P2 A|P2 B|P2 C|P2 D|P2 Down|P2 Left|P2 Right|P2 Up|')


def main():
    src, dst = sys.argv[1], sys.argv[2]
    with open(src, 'r', encoding='ascii') as f:
        lines = f.read().splitlines()
    if lines[0] != '[Input]' or lines[1] != KEY or lines[-1] != '[/Input]':
        raise SystemExit('unexpected log header/footer')
    masks = []
    for n, line in enumerate(lines[2:-1]):
        parts = line.split('|')
        if len(parts) != 5 or parts[0] or parts[4] or [len(p) for p in parts[1:4]] != [6, 8, 8]:
            raise SystemExit('bad line %d' % (n + 2))
        m = 0
        for base, group in ((0, parts[1]), (8, parts[2]), (16, parts[3])):
            for i, c in enumerate(group):
                if c != '.':
                    if not ('!' <= c <= '~'):
                        raise SystemExit('bad char line %d' % (n + 2))
                    m |= 1 << (base + i)
        masks.append(m)
    with open(dst, 'w') as f:
        for m in masks:
            f.write('%x\n' % m)
    first = {}
    for i, m in enumerate(masks):
        for b in range(24):
            if m >> b & 1 and b not in first:
                first[b] = i
    print('frames', len(masks), 'first press per bit', first)


if __name__ == '__main__':
    main()
