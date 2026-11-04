#!/usr/bin/env python3
"""Build the native prototype HUD from original S-ROM FIX glyphs and palette.

The labels/layout are authored; current prototype health/kills are not presented
as original arcade score, lives or timer. Only source art is recovered here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct

from export_marco import palette_rgb

ROOT = Path(__file__).resolve().parents[1]


def glyph_pen(fixed, tile, x, y):
    return (fixed[tile * 32 + (16, 24, 0, 8)[x // 2] + y] >> ((x & 1) * 4)) & 15


def make_atlas(fixed):
    if len(fixed) != 4096 * 32:
        raise ValueError('Expected 4096 S-ROM FIX tiles')
    atlas = bytearray(2048)
    for character in range(32, 96):
        slot = character - 32
        for y in range(8):
            for x in range(8):
                dx, dy = (slot & 7) * 8 + x, (slot >> 3) * 8 + y
                pen = glyph_pen(fixed, 0x300 + character, x, y)
                atlas[dy * 32 + dx // 2] |= pen << (0 if dx & 1 else 4)
    return bytes(atlas)


def rgba16_palette(words):
    result = [0]
    for word in words[1:]:
        r, g, b = palette_rgb(word)
        result.append((r >> 3) << 11 | (g >> 3) << 6 | (b >> 3) << 1 | 1)
    return result


def validate_source(capture, frame, fixed, palette_words, palette_id=0):
    """Compare only opaque original FIX pixels; never sample screenshot art."""
    from PIL import Image
    words = struct.unpack('>34304H', (capture / f'vram{frame}.bin').read_bytes())
    screen = Image.open(capture / f'game{frame}.png').convert('RGB')
    if screen.size != (320, 224):
        raise ValueError('Expected uncropped 320x224 MAME reference')
    glyphs, pixels = 0, 0
    observed = set()
    for column in range(1, 39):
        for row in range(2, 30):
            word = words[0x7000 + column * 32 + row]
            tile = word & 4095
            if word >> 12 != palette_id or not 0x320 <= tile < 0x360:
                continue
            opaque = 0
            for y in range(8):
                for x in range(8):
                    pen = glyph_pen(fixed, tile, x, y)
                    if not pen:
                        continue
                    wanted = palette_rgb(palette_words[pen])
                    got = screen.getpixel((column * 8 + x, row * 8 + y - 16))
                    if max(abs(a - b) for a, b in zip(wanted, got)) > 1:
                        raise ValueError(f'FIX pixel mismatch at {column * 8+x},{row * 8+y-16}')
                    opaque += 1
            if opaque:
                glyphs += 1
                pixels += opaque
                observed.add(chr(tile - 0x300))
    if not pixels:
        raise ValueError('No opaque source glyphs validated')
    return dict(glyph_instances=glyphs, opaque_pixels=pixels, observed_characters=''.join(sorted(observed)))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture', type=Path, required=True)
    parser.add_argument('--frame', type=int, default=2100)
    parser.add_argument('--palette', type=int, default=0)
    parser.add_argument('--fixed', type=Path, default=ROOT / 'filesystem/fixed.bin')
    parser.add_argument('--output', type=Path, default=ROOT / 'build/hud_assets.h')
    parser.add_argument('--report', type=Path, default=ROOT / 'filesystem/hud.json')
    args = parser.parse_args()
    if not 0 <= args.palette < 256:
        parser.error('Palette must be 0..255')
    fixed = args.fixed.read_bytes()
    palette_path = args.capture / f'palette{args.frame}.bin'
    palette_data = palette_path.read_bytes()
    all_words = struct.unpack('>4096H', palette_data)
    words = all_words[args.palette * 16:args.palette * 16 + 16]
    palette = rgba16_palette(words)
    atlas = make_atlas(fixed)
    evidence = validate_source(args.capture, args.frame, fixed, words, args.palette)
    text = ['/* Original FIX source art; authored prototype HUD labels. */',
            '#ifndef MSX_HUD_ASSETS_H', '#define MSX_HUD_ASSETS_H', '#include <stdint.h>',
            'static const uint8_t msx_hud_glyphs[2048] __attribute__((aligned(16)))={']
    for start in range(0, len(atlas), 32):
        text.append('    ' + ','.join(f'0x{b:02x}' for b in atlas[start:start+32]) + ',')
    text.extend(['};', 'static const uint16_t msx_hud_palette[16] __attribute__((aligned(16)))={',
                 '    ' + ','.join(hex(w) for w in palette), '};', '#endif'])
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text('\n'.join(text) + '\n')
    sha = lambda data: hashlib.sha256(data).hexdigest()
    report = dict(status='PASS', source_frame=args.frame, palette_id=args.palette,
                  fixed_sha256=sha(fixed), palette_sha256=sha(palette_data),
                  vram_sha256=sha((args.capture / f'vram{args.frame}.bin').read_bytes()),
                  screenshot_sha256=sha((args.capture / f'game{args.frame}.png').read_bytes()),
                  atlas_sha256=sha(atlas), atlas_bytes=len(atlas), **evidence,
                  limits='Original S-ROM font and captured palette, not original HUD logic. HP/KILLS/PISTOL labels/layout are authored and must use current native counters; no invented score, lives, ammo or timer.')
    args.report.parent.mkdir(parents=True, exist_ok=True)
    args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
