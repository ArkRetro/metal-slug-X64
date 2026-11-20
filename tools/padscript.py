#!/usr/bin/env python3
"""Pad scripts of the test cartridge (mslugx-testpad.z64, src/msx_testpad.c; B78, docs/FALLOS.md "Mando por guion").

The test cartridge numbers every pad read from the power-on and logs each change of the buttons as
`MSX PAD n=N btn=XXXX`. A recording run (the cartridge without a script, keys from xdotool) leaves its script in its
log; a replay cartridge carries that script and gives the game the same buttons at the same reads, so two replays in
ares are the same run.

  padscript.py from-log ares.log script.txt     # the `MSX PAD n= btn=` lines of a recording -> script (text)
  padscript.py apply ROM.z64 script.txt OUT.z64  # ROM + the script at the next 1 MiB boundary (magic MSXPAD01)

Script text: one `N XXXX` per line (the read where the buttons change, hexadecimal buttons), N increasing.
"""
import re, struct, sys
from pathlib import Path

MAGIC = b'MSXPAD01'
LINE = re.compile(r'MSX PAD n=(\d+) btn=([0-9A-F]{4})')


def from_log(text):
    out = []
    for n, b in LINE.findall(text):
        n = int(n)
        if out and n <= out[-1][0]:
            raise SystemExit(f'pad read {n} after {out[-1][0]}: two power-ons in one log?')
        out.append((n, int(b, 16)))
    return out


def read_script(path):
    rows = []
    for ln in Path(path).read_text().split('\n'):
        ln = ln.split('#')[0].strip()
        if ln:
            n, b = ln.split()
            rows.append((int(n), int(b, 16)))
    return rows


def apply(rom, rows, out):
    data = bytearray(Path(rom).read_bytes())
    at = (len(data) + 0xFFFFF) & ~0xFFFFF
    data += bytes(at - len(data))
    data += MAGIC + struct.pack('>II', len(rows), 0)
    for n, b in rows:
        data += struct.pack('>IHH', n, b, 0)
    Path(out).write_bytes(bytes(data))
    return at


def main():
    if len(sys.argv) == 4 and sys.argv[1] == 'from-log':
        rows = from_log(Path(sys.argv[2]).read_text(errors='replace'))
        Path(sys.argv[3]).write_text(''.join(f'{n} {b:04X}\n' for n, b in rows))
        print(f'{len(rows)} changes, last read {rows[-1][0] if rows else 0}')
    elif len(sys.argv) == 5 and sys.argv[1] == 'apply':
        rows = read_script(sys.argv[3])
        at = apply(sys.argv[2], rows, sys.argv[4])
        print(f'{len(rows)} changes at 0x{at:07X}')
    else:
        raise SystemExit(__doc__)


if __name__ == '__main__':
    main()
