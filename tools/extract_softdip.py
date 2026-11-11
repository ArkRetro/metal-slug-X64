#!/usr/bin/env python3
"""The soft-DIP table of the user's ROM for the N64 options screen
(docs/OPCIONES.md, src/msx_options.c): the euro header table of 250-p1.p1
(pointer at 0x11E: 0x316E), in the BIOS format (16-byte name, time and count
settings, ten list bytes, then the 12-character strings), copied as is to
filesystem/softdip.bin. The P1 ROM is checked against tools/rom_manifest.json
first. ROM data: never in git.

  python3 tools/extract_softdip.py /path/mslugx.zip [--out filesystem/softdip.bin]
"""
import argparse, hashlib, json, zipfile, zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
NAME = '250-p1.p1'
POINTER = 0x11E   # euro table (0x116 Japan, 0x11A USA)


def table(p1):
    """The table at the euro pointer; its length follows from its header."""
    p = bytearray(p1); p[0::2], p[1::2] = p1[1::2], p1[0::2]   # ROM_LOAD16_WORD_SWAP
    a = int.from_bytes(p[POINTER:POINTER + 4], 'big')
    h = p[a:a + 32]
    names = sum(1 for t in (0, 2) if h[16 + t:18 + t] != b'\xff\xff')
    names += sum(1 for c in (20, 21) if h[c] != 0xff)
    for b in h[22:32]:
        if not b:
            break
        names += 1 + (b & 15)
    return a, bytes(p[a:a + 32 + 12 * names])


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('zip', type=Path)
    ap.add_argument('--out', type=Path, default=ROOT / 'filesystem/softdip.bin')
    a = ap.parse_args()
    expected = json.loads((ROOT / 'tools/rom_manifest.json').read_text())[NAME]
    with zipfile.ZipFile(a.zip) as z:
        p1 = z.read(NAME)
    if f'{zlib.crc32(p1):08x}' != expected['crc32'] or hashlib.sha1(p1).hexdigest() != expected['sha1']:
        raise SystemExit(f'{NAME}: ROM distinta o dañada')
    address, data = table(p1)
    if a.out.is_symlink():
        raise SystemExit(f'{a.out} es un enlace: no se escribe a través de él (docs/AGENTES.md)')
    a.out.parent.mkdir(parents=True, exist_ok=True)
    a.out.write_bytes(data)
    print(f'{a.out}: tabla de soft-DIP EU 0x{address:X}, {len(data)} bytes ({data[:16].decode("latin-1").strip()})')


if __name__ == '__main__':
    main()
