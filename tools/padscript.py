#!/usr/bin/env python3
"""Pad scripts of the test cartridge (mslugx-testpad.z64, src/msx_testpad.c; B78, docs/FALLOS.md "Mando por guion").

The test cartridge numbers every pad read from the power-on and logs each change of the buttons as
`MSX PAD n=N btn=XXXX`. A recording run (the cartridge without a script, keys from xdotool) leaves its script in its
log; a replay cartridge carries that script and gives the game the same buttons and the same steps per frame at the same reads, so two replays in
ares are the same run.

  padscript.py from-log ares.log script.txt     # the `MSX PAD n= btn=` lines of a recording -> script (text)
  padscript.py apply ROM.z64 script.txt OUT.z64 [--then-record]  # ROM + the script at the next 1 MiB boundary
      (magic MSXPAD01; --then-record: past the script's last entry the cartridge records again, a continued recording)

Script text: one `N K XXXX` per line: from pad read N, stream K (0 the buttons, 1 a frame's simulation steps, 2 a paused
frame's steps) has the hexadecimal value XXXX; N not decreasing.
"""
import re, struct, sys
from pathlib import Path

MAGIC = b'MSXPAD01'
LINE = re.compile(r'MSX PAD n=(\d+) (?:btn=([0-9A-F]{4})|s([12])=(\d+))\n')


def from_log(text):
    out = []
    for n, b, k, v in LINE.findall(text):
        n = int(n)
        if out and n < out[-1][0]:
            raise SystemExit(f'pad read {n} after {out[-1][0]}: two power-ons in one log?')
        out.append((n, 0, int(b, 16)) if b else (n, int(k), int(v)))
    return out


def read_script(path):
    rows = []
    for ln in Path(path).read_text().split('\n'):
        ln = ln.split('#')[0].strip()
        if ln:
            n, k, v = ln.split()
            rows.append((int(n), int(k), int(v, 16)))
    return rows


def apply(rom, rows, out, then_record=False):
    data = bytearray(Path(rom).read_bytes())
    at = (len(data) + 0xFFFFF) & ~0xFFFFF
    data += bytes(at - len(data))
    data += MAGIC + struct.pack('>II', len(rows), 1 if then_record else 0)   # flag 1: past the script the cartridge records
    for n, k, v in rows:
        data += struct.pack('>IHH', n, v, k)
    Path(out).write_bytes(bytes(data))
    return at


def main():
    if len(sys.argv) == 4 and sys.argv[1] == 'from-log':
        rows = from_log(Path(sys.argv[2]).read_text(errors='replace'))
        Path(sys.argv[3]).write_text(''.join(f'{n} {k} {v:04X}\n' for n, k, v in rows))
        print(f'{len(rows)} changes, last read {rows[-1][0] if rows else 0}')
    elif len(sys.argv) in (5, 6) and sys.argv[1] == 'apply':
        rows = read_script(sys.argv[3])
        at = apply(sys.argv[2], rows, sys.argv[4], then_record=sys.argv[5:] == ['--then-record'])
        print(f'{len(rows)} changes at 0x{at:07X}')
    else:
        raise SystemExit(__doc__)


if __name__ == '__main__':
    main()
