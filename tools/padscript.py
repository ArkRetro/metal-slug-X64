#!/usr/bin/env python3
"""Pad scripts of the test cartridge (mslugx-testpad.z64, src/msx_testpad.c; B78, docs/FALLOS.md "Mando por guion").

The test cartridge numbers every pad read from the power-on and logs each change of the buttons as
`MSX PAD n=N btn=XXXX`. A recording run (the cartridge without a script, keys from xdotool) leaves its script in its
log; a replay cartridge carries that script and gives the game the same buttons and the same steps per frame at the same reads, so two replays in
ares are the same run.

  padscript.py check script.txt                 # encode + decode equal (make check)
  padscript.py from-log ares.log script.txt     # the `MSX PAD n= btn=` lines of a recording -> script (text)
  padscript.py apply ROM.z64 script.txt OUT.z64 [--then-record]  # ROM + the packed script at the next 64 KiB boundary
      (magic MSXPAD02, 4 bytes per change: encode() below, checked by decoding; --then-record: past the script's last entry the cartridge records again, a continued recording)

Script text: one `N K XXXX` per line: from pad read N, stream K (0 the buttons, 1 a frame's simulation steps, 2 a paused
frame's steps) has the hexadecimal value XXXX; N not decreasing.
"""
import re, struct, sys
from pathlib import Path

MAGIC = b'MSXPAD02'
ALIGN = 0x10000      # the script starts at the next 64 KiB boundary past the cartridge (src/msx_testpad.c scans them)
DELTA_MAX = 0x3FFF   # an entry's read delta (14 bits); a longer gap is split with kind-3 entries (advance only)
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


def encode(rows):
    """Each entry 4 bytes, big-endian: u16 (kind << 14 | reads since the previous entry), u16 value.
    Kind 3: no value, the reads only (a gap over DELTA_MAX). Lossless: decode(encode(rows)) == rows."""
    out, last = bytearray(), 0
    for n, k, v in rows:
        if n < last or not 0 <= k <= 2 or not 0 <= v <= 0xFFFF: raise SystemExit(f'bad entry {n} {k} {v:04X}')
        d = n - last
        while d > DELTA_MAX:
            out += struct.pack('>HH', 3 << 14 | DELTA_MAX, 0); d -= DELTA_MAX
        out += struct.pack('>HH', k << 14 | d, v); last = n
    return bytes(out)


def decode(blob):
    rows, n = [], 0
    for i in range(0, len(blob), 4):
        h, v = struct.unpack_from('>HH', blob, i)
        n += h & DELTA_MAX
        if h >> 14 != 3: rows.append((n, h >> 14, v))
    return rows


def apply(rom, rows, out, then_record=False):
    data = bytearray(Path(rom).read_bytes())
    at = (len(data) + ALIGN - 1) & ~(ALIGN - 1)
    data += bytes(at - len(data))
    body = encode(rows)
    if decode(body) != rows: raise SystemExit('pad script: the packed entries do not decode to the script')
    data += MAGIC + struct.pack('>II', len(body) // 4, 1 if then_record else 0)   # flag 1: past the script the cartridge records
    data += body
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
    elif len(sys.argv) == 3 and sys.argv[1] == 'check':   # make check: the packing is lossless on a script
        rows = read_script(sys.argv[2]); body = encode(rows)
        if decode(body) != rows: raise SystemExit(f'FAIL: {sys.argv[2]}: packed script differs')
        print(f'PASS: {sys.argv[2]}: {len(rows)} changes, {len(rows) * 8} -> {len(body)} bytes, decoded equal')
    else:
        raise SystemExit(__doc__)


if __name__ == '__main__':
    main()
