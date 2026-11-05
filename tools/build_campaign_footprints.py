#!/usr/bin/env python3
"""Extract the original B5870 foreground edits and their native redraw resources.

B53BE describes a 72x5 column-major tile map. One live call replaces a 16x16
foreground cell; transparent replacement pixels expose the original lower
layers. These CI4 atlases also contain those recovered map layers and props so
the renderer can reconstruct only the touched rectangle over a prerender.
No screenshot, frame timeline or captured actor state becomes a resource.
"""
import argparse
import collections
import hashlib
import json
import struct
import subprocess
import zipfile
from pathlib import Path

from PIL import Image
from build_marco_campaign_assets import preserve_ci4_indices, rgba_palette, swapped
from import_rom import validate
from mission_tiles import decode, rgba16

ROOT = Path(__file__).resolve().parents[1]
STAMP_PALETTES = {0x11: 0x403, 0x12: 0x404, 0x15: 0x407, 0x16: 0x408,
                  0x1d: 0x40f, 0x1e: 0x410, 0x22: 0x414, 0x53: 0x468}


def original_palette(p2, bank, words):
    values = []
    for word in words[1:]:
        red = ((word >> 14) & 1) | ((word >> 7) & 30)
        green = ((word >> 13) & 1) | ((word >> 3) & 30)
        blue = ((word >> 12) & 1) | ((word << 1) & 30)
        values.append((word & 0x8000) | (red << 10) | (green << 5) | blue)
    at = p2.find(struct.pack('>15H', *values)) - 2
    assert 0x100000 <= at < 0x200000 and at % 32 == 0, 'Missing exact source-ROM palette'
    descriptor = (at - 0x100000) // 32
    palette = rgba_palette(bank, descriptor)
    assert palette == rgba16(words), 'Palette loader conversion differs'
    return descriptor, palette


def reference_crops(model, props, p2, crom, output):
    """Independent full-scene pixel reference for the native scissor test."""
    palettes = [rgba16(words) for words in model['palettes'] + props['palettes']]
    cells = [dict(cell) for cell in model['cells']]
    for cell in props['cells']:
        cell = dict(cell)
        cell['palette'] += len(model['palettes'])
        cells.append(cell)

    def pen_at(code, flip, x, y):
        sx, sy = 15 - x if flip & 1 else x, 15 - y if flip & 2 else y
        at = code * 128 + (64 if sx < 8 else 0) + sy * 4
        bit = 1 << (sx % 8)
        return ((1 if crom[at] & bit else 0) | (2 if crom[at + 2] & bit else 0) |
                (4 if crom[at + 1] & bit else 0) | (8 if crom[at + 3] & bit else 0))

    references = []
    for camera, column, row in ((0, 7, 2), (0, 8, 2), (0, 4, 0),
                                (320, 30, 1), (640, 48, 3), (1024, 70, 1)):
        rise = 0 if camera <= 944 else min(16, (camera - 944 + 3) // 4)
        left, top = column * 16 - camera, 120 + row * 16 + rise
        pixels = [rgba16([0, model['backdrop']])[1]] * 256
        for layer in range(5):
            if layer == 3:
                low, attributes = struct.unpack_from('>HH', p2, 0x206780 + (column * 5 + row) * 4)
                code, flip = low | ((attributes & 0xf0) << 12), attributes & 3
                source_bank = attributes >> 8
                palette = (rgba_palette(p2[0x100000:0x200000], STAMP_PALETTES[source_bank])
                           if source_bank in STAMP_PALETTES else None)
                for y in range(16):
                    for x in range(16):
                        pen = pen_at(code, flip, x, y)
                        if pen:
                            assert palette is not None
                            pixels[y * 16 + x] = palette[pen]
                continue
            if (layer == 1 and camera >= 1023) or (layer == 2 and camera < 1023):
                continue
            offset = (camera + 1) // 2 if layer == 0 else (camera * 3 + 2) // 4 if layer == 1 else camera
            height = (rise + 1) // 2 if layer == 0 else (rise * 3 + 2) // 4 if layer == 1 else rise
            for cell in cells:
                if cell['layer'] != layer:
                    continue
                cx, cy = cell['x'] - offset, 8 + cell['y'] + height
                if cx >= left + 16 or cy >= top + 16 or cx + 16 <= left or cy + 16 <= top:
                    continue
                for y in range(max(top, cy), min(top + 16, cy + 16)):
                    for x in range(max(left, cx), min(left + 16, cx + 16)):
                        pen = pen_at(cell['code'], cell['flip'], x - cx, y - cy)
                        if pen:
                            pixels[(y - top) * 16 + x - left] = palettes[cell['palette']][pen]
        references.append((camera, column, row, left, top, pixels))
    lines = ['/* Independent cartridge-bitplane reference, no atlas/renderer calls. */',
             'static const struct {unsigned camera;int column,row,x,y;uint16_t pixels[256];} footprint_expected[]={']
    for record in references:
        lines.append('{' + ','.join(map(str, record[:5])) + ',{' + ','.join(map(str, record[5])) + '}},')
    lines.append('};')
    (output / 'footprint_expected.h').write_text('\n'.join(lines) + '\n')


def build(args):
    validate(args.rom)
    with zipfile.ZipFile(args.rom) as archive:
        fixed = swapped(archive.read('250-p1.p1'))
        p2 = swapped(archive.read('250-p2.ep1'))
        crom = bytearray()
        for odd in (1, 3, 5):
            left = archive.read(f'250-c{odd}.c{odd}')
            right = archive.read(f'250-c{odd + 1}.c{odd + 1}')
            pair = bytearray(len(left) + len(right))
            pair[0::2], pair[1::2] = left, right
            crom.extend(pair)
    assert struct.unpack_from('>IIHH', fixed, 0xb53be) == (0x306780, 0, 72, 5)
    assert struct.unpack_from('>IIHH', fixed, 0xb533a) == (0x300500, 0x41cf3e, 240, 18)
    bank = p2[0x100000:0x200000]
    model = json.loads((ROOT / 'filesystem/mission1/map.json').read_text())
    props = json.loads((ROOT / 'filesystem/mission1/props.json').read_text())
    palette_descriptors, palettes = [], []
    for words in model['palettes'] + props['palettes']:
        descriptor, palette = original_palette(p2, bank, words)
        palette_descriptors.append(descriptor)
        palettes.append(palette)
    stamp_palette_ids = {}
    for source, descriptor in STAMP_PALETTES.items():
        if descriptor not in palette_descriptors:
            palette_descriptors.append(descriptor)
            palettes.append(rgba_palette(bank, descriptor))
        stamp_palette_ids[source] = palette_descriptors.index(descriptor)
    cells = [dict(cell) for cell in model['cells'] if cell['layer'] != 3]
    for cell in props['cells']:
        cell = dict(cell)
        cell['palette'] += len(model['palettes'])
        cells.append(cell)
    cells.sort(key=lambda cell: cell['layer']) # Stable: preserve overlapping prop order.
    tiles = collections.defaultdict(set)
    for cell in cells:
        for phase in range(8):
            tiles[cell['palette']].add((cell['code'] | (phase & cell['animation_mask']), cell['flip']))
    stamps, removals = [], 0
    for x in range(72):
        for row in range(5):
            code, attributes = struct.unpack_from('>HH', p2, 0x206780 + (x * 5 + row) * 4)
            old = struct.unpack_from('>HH', p2, 0x200500 + (x * 18 + row + 10) * 4)
            code |= (attributes & 0xf0) << 12
            pens = decode(crom, code, attributes & 3)
            old_pens = decode(crom, old[0] | ((old[1] & 0xf0) << 12), old[1] & 3)
            removals += sum(bool(a) and not b for a, b in zip(old_pens, pens))
            if any(pens):
                assert attributes >> 8 in stamp_palette_ids, hex(attributes)
                key = (stamp_palette_ids[attributes >> 8], code, attributes & 3)
                tiles[key[0]].add(key[1:])
            else:
                assert code == 0x40000, hex(code)
                key = None
            original = struct.unpack_from('>HH', p2, 0x206780 + (x * 5 + row) * 4)
            stamps.append(dict(x=x, row=row, code=code, attributes=attributes, key=key,
                               changed=old != original))
    png_dir = args.output / 'campaign-footprint-png'
    sprite_dir = ROOT / 'romfs/campaign-footprints'
    png_dir.mkdir(parents=True, exist_ok=True)
    sprite_dir.mkdir(parents=True, exist_ok=True)
    atlases, images, paths, lookup, original_pixels = [], [], [], {}, []
    for palette, keys in sorted(tiles.items()):
        keys = sorted(keys)
        colors = [((v >> 11 & 31) * 255 // 31, (v >> 6 & 31) * 255 // 31,
                   (v >> 1 & 31) * 255 // 31) for v in palettes[palette]]
        for first in range(0, len(keys), 16):
            pixels = [0] * 4096
            atlas = len(atlases)
            for slot, (code, flip) in enumerate(keys[first:first + 16]):
                pens = decode(crom, code, flip)
                sx, sy = slot % 4 * 16, slot // 4 * 16
                for y in range(16):
                    pixels[(sy + y) * 64 + sx:(sy + y) * 64 + sx + 16] = pens[y * 16:y * 16 + 16]
                lookup[(palette, code, flip)] = len(images)
                images.append([atlas, sx, sy])
            fingerprint = 2166136261
            for pen in pixels:
                fingerprint = ((fingerprint ^ pen) * 16777619) & 0xffffffff
            image = Image.new('P', (64, 64))
            image.putpalette([channel for color in colors for channel in color])
            image.putdata(pixels)
            image.info['transparency'] = bytes([0] + [255] * 15)
            path = png_dir / f'a{atlas}.png'
            image.save(path, bits=4, optimize=False)
            assert list(Image.open(path).tobytes()) == pixels
            paths.append(str(path))
            original_pixels.append(pixels)
            atlases.append([palette, fingerprint])
    subprocess.run([str(args.mksprite), '-f', 'CI4', '-D', 'NONE', '-c', '0',
                    '-o', str(sprite_dir), *paths], check=True, stdout=subprocess.DEVNULL)
    for index, pixels in enumerate(original_pixels):
        preserve_ci4_indices(sprite_dir / f'a{index}.sprite', pixels, 64, 64)
    arr = lambda values: '{' + ','.join(str(v) for v in values) + '}'
    lines = ['/* Cartridge B53BE edits; exact ROM palettes/C-ROM and recovered map coordinates. */',
             '#ifndef MSX_CAMPAIGN_FOOTPRINT_META_H', '#define MSX_CAMPAIGN_FOOTPRINT_META_H',
             f'#define CAMPAIGN_FOOTPRINT_ATLASES {len(atlases)}u',
             'static const uint16_t campaign_footprint_palettes[][16] __attribute__((aligned(16)))={' +
             ','.join(arr(palette) for palette in palettes) + '};',
             'static const struct {uint16_t palette;uint32_t fingerprint;} campaign_footprint_atlases[]={']
    lines += ['{' + str(pal) + ',' + str(fnv) + 'u},' for pal, fnv in atlases]
    lines += ['};', 'static const struct {uint16_t atlas;uint8_t x,y;} campaign_footprint_images[]={']
    lines += [arr(image) + ',' for image in images]
    lines += ['};', 'static const struct {uint16_t image;bool changed;} campaign_footprint_stamps[72][5]={']
    for x in range(72):
        lines.append('{' + ','.join('{' + str(lookup[tuple(s['key'])] if s['key'] else 65535) + ',' +
                                  str(int(s['changed'])) + '}' for s in stamps[x * 5:x * 5 + 5]) + '},')
    lines += ['};', 'static const struct {int16_t x,y;uint16_t images[8];uint8_t layer;} campaign_footprint_cells[]={']
    for cell in cells:
        phases = [lookup[(cell['palette'], cell['code'] | (phase & cell['animation_mask']), cell['flip'])]
                  for phase in range(8)]
        lines.append('{' + str(cell['x']) + ',' + str(cell['y']) + ',' + arr(phases) + ',' + str(cell['layer']) + '},')
    counts = [sum(cell['layer'] == layer for cell in cells) for layer in range(5)]
    firsts = [sum(counts[:layer]) for layer in range(5)]
    lines += ['};', 'static const uint16_t campaign_footprint_layer_first[]=' + arr(firsts) + ';',
              'static const uint16_t campaign_footprint_layer_count[]=' + arr(counts) + ';', '#endif']
    args.output.mkdir(parents=True, exist_ok=True)
    (args.output / 'campaign_footprint_meta.h').write_text('\n'.join(lines) + '\n')
    reference_crops(model, props, p2, crom, args.output)
    for stamp in stamps:
        stamp.pop('key')
    report = dict(source_descriptor=0xb53be, source_graphics=0x306780, physical_p2_offset=0x206780,
                  columns=72, rows=5, tile_pixels=16, collision_source=None,
                  request='column=signed_actor_x_word ASR4; row=7-(signed_actor_y_word ASR4)',
                  scene_projection='map x=column*16,y=112+row*16; viewport y+8',
                  replacement_transparent_pixels_removing_old_opaque=removals,
                  palette_source_descriptors=palette_descriptors, stamps=stamps,
                  atlases=len(atlases), images=len(images), redraw_cells=len(cells),
                  source_sha256={str(path.relative_to(ROOT)): hashlib.sha256(path.read_bytes()).hexdigest()
                                 for path in (ROOT / 'filesystem/mission1/map.json', ROOT / 'filesystem/mission1/props.json')},
                  rom_sha256=hashlib.sha256(args.rom.read_bytes()).hexdigest(),
                  limits='Resources only. Native scene identity, original guard, live call timing and active32x32 tile window must be owned by the caller. Immutable map coordinates cover the existing recovered Mission1 region.')
    (args.output / 'campaign_footprints.json').write_text(json.dumps(report, indent=2) + '\n')
    (args.output / 'campaign-footprints.stamp').write_text(
        f'360 source edits; {len(atlases)} CI4 atlases; {len(images)} tiles; 2048 bytes maximum upload\n')
    print(f'Built original footprints:360 source edits,{len(atlases)} CI4 atlases,'
          f'{len(images)} tiles,{removals} required erasures; '
          f'{sum(path.stat().st_size for path in sprite_dir.glob("*.sprite"))} ROM bytes')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--rom', type=Path, required=True)
    parser.add_argument('--mksprite', type=Path, required=True)
    parser.add_argument('--output', type=Path, default=ROOT / 'build')
    build(parser.parse_args())
