#!/usr/bin/env python3
"""Fixture of the order of the Mission 6 bridge's update (docs/MISION6.md,
"Puente") from a capture with MSX_FULL_ORDER_TAP and MSX_FULL_CAMERA
(captures-m6/fam/O, runO.sh): one record per update of the driver 0BCE2C
(its write of 10E94E at 0BCE8E), with the positions in the CPU's access
order of

  cam   the camera's last store 10E9D8 (0C7EB6; it starts the update),
  step  0C81C0's read of 10E94E (0C826C; -1: the layer took the root's step),
  wx    01AC38's write of 10D7B6 +0 (01AC40),
  w14   01ABC8's write of +14 (01AC08),
  w36   0C748E's write of +36 (0C74D8; -1: not written),
  swing 0BD1B8's read of +0 (0BD1C4),
  drive 0BCE2C's write of 10E94E (0BCE8E),
  r36   0BCEB0's read of +36 (0BCEE0),

and the values: +0 before and after the step, the step read, the driver's
write, the +36 the driver read, and from camera.csv (the store 10E9D8, before
the step) 10E970/10E974 (the root's step), 10E956..10E959 (layer flags) and
10D7B6 +26/+28 (factors) and +2A/+2B (remainders).

  python3 tools/extract_m6order_fixture.py CAPTURE [--out data/m6order_fixture.json.gz]
"""
import argparse, gzip, json
from pathlib import Path


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('capture', type=Path); ap.add_argument('--out', default='data/m6order_fixture.json.gz')
    a = ap.parse_args()
    cam = {}
    with open(a.capture / 'camera.csv') as f:
        next(f)
        for line in f:
            t = line.rstrip('\n').split(',')
            block, layers = bytes.fromhex(t[5]), t[7].split(':')
            mid = bytes.fromhex(layers[1])
            w = lambda b, o, n: int.from_bytes(b[o:o + n], 'big')
            cam[int(t[0])] = dict(root_vx=w(block, 0x22, 4), root_vy=w(block, 0x26, 4), flags=list(block[8:12]),
                                  fx=w(mid, 0x26, 2), fy=w(mid, 0x28, 2), frac_x=mid[0x2a], frac_y=mid[0x2b])
    # One group per update: from a camera store to the next one (an update can
    # run over two video frames: the store in one, the actors in the next).
    groups, cur = [], None
    with open(a.capture / 'order.txt') as f:
        for line in f:
            fr, kind, ad, data, mask, pc = line.split()
            if pc == 'c7eb6' and kind == 'R':
                continue   # camera.csv's own reads (the lua tap of 10E9D8)
            if kind == 'C':
                cur = (int(fr), []); groups.append(cur)
            if cur is not None:
                cur[1].append((kind, int(ad, 16), int(data, 16), int(mask, 16), int(pc, 16)))
    recs = []
    for fr, ev in groups:

        def find(kind, ad, pc):
            for i, e in enumerate(ev):
                if e[0] == kind and e[1] == ad and e[4] == pc:
                    return i
            return -1

        def long_at(kind, ad, pc):
            hi, lo = find(kind, ad, pc), find(kind, ad + 2, pc)
            return -1 if hi < 0 or lo < 0 else ev[hi][2] << 16 | ev[lo][2]
        drive = find('W', 0x10e94e, 0xbce8e)
        if drive < 0:
            continue
        r = dict(frame=fr, cam=find('C', 0x10e9d8, 0xc7eb6), step=find('R', 0x10e94e, 0xc826c), wx=find('W', 0x10d7b6, 0x1ac40),
                 w14=find('W', 0x10d7ca, 0x1ac08), w36=find('W', 0x10d7ec, 0xc74d8), swing=find('R', 0x10d7b6, 0xbd1c4),
                 drive=drive, r36=find('R', 0x10d7ec, 0xbcee0),
                 x_before=long_at('R', 0x10d7b6, 0x1ac40), x_after=long_at('W', 0x10d7b6, 0x1ac40),
                 step_value=long_at('R', 0x10e94e, 0xc826c), drive_value=long_at('W', 0x10e94e, 0xbce8e))
        i = r['r36']
        r['o36'] = ev[i][2] if i >= 0 else -1
        r.update(cam.get(fr, {}))
        recs.append(r)
    with gzip.open(a.out, 'wt') as f:
        json.dump({'source': str(a.capture), 'updates': recs}, f, separators=(',', ':'))
    print(f'{a.out}: {len(recs)} updates of 0BCE2C, {sum(r["step"] >= 0 for r in recs)} with the own step')


if __name__ == '__main__':
    main()
