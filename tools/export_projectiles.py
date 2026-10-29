#!/usr/bin/env python3
"""Recover the pistol projectile from SCB/C-ROM/palette and verify against MAME.

The current provisional horizontal enemy shots share this pistol sprite until
the corresponding original enemy weapon systems are recovered.
"""
import argparse
import hashlib
import json
import struct
from pathlib import Path
from PIL import Image
from export_marco import ROOT, palette_rgb, top_sprite_pixel
from mission_tiles import decode

CODE = 0x2cbd3
PALETTE = 0x71


def export(folder):
    crom = (ROOT/'filesystem/sprites.bin').read_bytes()
    pens = bytes(decode(crom, CODE, 0))
    samples = []
    original_palette = None
    matched = 0
    for file in sorted(folder.glob('vram*.bin')):
        frame = int(file.stem[4:])
        v = struct.unpack('>34304H', file.read_bytes())
        palette = struct.unpack('>4096H', (folder/f'palette{frame}.bin').read_bytes())
        reference = Image.open(folder/f'game{frame}.png').convert('RGB')
        x = y = rows = zx = zy = 0
        for slot in range(1, 381):
            control, zoom = v[0x8200+slot], v[0x8000+slot]
            if control & 64:
                x = (x+zx+1) & 511
                zx = (zoom >> 8) & 15
            else:
                x = v[0x8400+slot] >> 7
                y = (512-(control >> 7)) & 511
                rows, zy, zx = control & 63, zoom & 255, (zoom >> 8) & 15
            if not 0 < rows <= 32:
                continue
            for row in range(rows):
                low, attr = v[slot*64+row*2:slot*64+row*2+2]
                code = low | ((attr << 12) & 0xf0000)
                yy = (y+row*16) & 511
                if code != CODE or attr >> 8 != PALETTE:
                    continue
                assert zx == 15 and zy == 255 and not attr & 12, 'Unsupported projectile transform'
                if not (8 <= x and x+16 <= 312 and 32 <= yy and yy+16 <= 224):
                    continue  # Exclude clipped or HUD-covered samples.
                source = bytes(decode(crom, code, attr & 3))
                visible = 0
                for py in range(16):
                    for px in range(16):
                        pen = source[py*16+px]
                        if not pen:
                            continue
                        ref = reference.getpixel((x+px, yy+py-16))
                        rgb = palette_rgb(palette[PALETTE*16+pen])
                        if max(abs(a-b) for a, b in zip(ref, rgb)) <= 1:
                            visible += 1
                        else:
                            top = top_sprite_pixel(v, palette, crom, x+px, yy+py)
                            assert top and top[0] != PALETTE and max(abs(a-b) for a, b in zip(ref, top[1])) <= 1, (frame, slot, px, py)
                if not visible:
                    continue
                words = palette[PALETTE*16:(PALETTE+1)*16]
                if original_palette is None:
                    original_palette = words
                assert original_palette == words, 'Projectile palette changed; add a distinct variant'
                matched += visible
                samples.append(dict(frame=frame, slot=slot, x=x, y=yy, flip=attr & 3, matched_pixels=visible))
    assert len(samples) >= 3 and matched >= 100, 'Insufficient independent projectile observations'
    rgba = []
    for pen, word in enumerate(original_palette):
        r, g, b = palette_rgb(word)
        rgba.append(((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | 1 if pen else 0)
    packed = bytes((pens[i] << 4) | pens[i+1] for i in range(0, 256, 2))
    opaque = [(i % 16, i // 16) for i, p in enumerate(pens) if p]
    bounds = [min(x for x, y in opaque), min(y for x, y in opaque), max(x for x, y in opaque)+1, max(y for x, y in opaque)+1]
    result = dict(source='MAME 0.276 / mslugx / euro', code=CODE, palette=PALETTE,
                  width=16, height=16, pivot=[8, 8], bounds=bounds,
                  palette_rgba5551=rgba, indices_hex=packed.hex(),
                  indices_sha256=hashlib.sha256(packed).hexdigest(),
                  matched_pixels=matched, samples=samples,
                  scope='Original pistol art; provisional enemy horizontal shots currently share it.')
    (ROOT/'filesystem/projectiles.json').write_text(json.dumps(result, indent=2)+'\n')
    print(f'Pistol projectile: {bounds[2]-bounds[0]}x{bounds[3]-bounds[1]}, {len(samples)} observations, {matched} MAME pixels matched')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('captures', type=Path)
    export(parser.parse_args().captures)
