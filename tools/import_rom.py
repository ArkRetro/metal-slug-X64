#!/usr/bin/env python3
"""Validate the local MAME set before importing the fixed-layer graphics."""
import argparse
import hashlib
import json
from pathlib import Path
import zipfile
import zlib

ROOT = Path(__file__).resolve().parents[1]


def validate(path):
    manifest = json.loads((ROOT / 'tools/rom_manifest.json').read_text())
    with zipfile.ZipFile(path) as archive:
        names = archive.namelist()
        if len(names) != len(set(names)) or set(names) != set(manifest):
            raise ValueError('El ZIP no contiene exactamente el set mslugx esperado')
        fixed = None
        for name, expected in manifest.items():
            if archive.getinfo(name).file_size > 8 * 1024 * 1024:
                raise ValueError(f'Tamaño inesperado: {name}')
            data = archive.read(name)
            if (f'{zlib.crc32(data):08x}' != expected['crc32'] or
                    hashlib.sha1(data).hexdigest() != expected['sha1']):
                raise ValueError(f'ROM distinta o dañada: {name}')
            if name == '250-s1.s1':
                fixed = data
        return fixed


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('zip', type=Path)
    args = parser.parse_args()
    fixed = validate(args.zip)
    target = ROOT / 'filesystem/fixed.bin'
    target.parent.mkdir(exist_ok=True)
    target.write_bytes(fixed)
    # Fixed filenames, never extract paths from the ZIP. ROM_LOAD16_BYTE layout.
    temporary = target.parent / 'sprites.bin.tmp'
    try:
        with zipfile.ZipFile(args.zip) as archive, temporary.open('wb') as output:
            for odd in (1, 3, 5):
                left = archive.read(f'250-c{odd}.c{odd}')
                right = archive.read(f'250-c{odd+1}.c{odd+1}')
                combined = bytearray(len(left) + len(right))
                combined[0::2], combined[1::2] = left, right
                output.write(combined)
        temporary.replace(target.parent / 'sprites.bin')
    finally:
        temporary.unlink(missing_ok=True)
    # The soft-DIP table of the options screen (tools/extract_softdip.py).
    with zipfile.ZipFile(args.zip) as archive:
        p1 = archive.read('250-p1.p1')
    import extract_softdip
    softdip = target.parent / 'softdip.bin'
    if not softdip.is_symlink():
        softdip.write_bytes(extract_softdip.table(p1)[1])
    print('13 ROM verificadas (CRC32 y SHA1); FIX, 48 MiB de sprites y tabla de soft-DIP importados')


if __name__ == '__main__':
    main()
