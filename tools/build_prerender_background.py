#!/usr/bin/env python3
"""Bake observed arcade scenery into lossless 32x32 RGBA5551/CI8 chunks.

Only supplied map/prop cells are composited. Actors, collision and events are
not inferred here. All eight arcade autoanimation phases remain addressable.
The callback API matches mission_tiles.pack_map with force_rgba=True added.
"""
from collections import defaultdict
from pathlib import Path
import argparse
import hashlib
import json
import struct
import zlib

from mission_tiles import decode, rgba16

ROOT = Path(__file__).resolve().parents[1]
SIDE = 32
PAGE_BYTES = SIDE * SIDE * 2


def _validate(model, crom):
    assert len(crom) % 128 == 0, 'Partial C-ROM tile'
    for cell in model['cells']:
        assert 0 <= cell['layer'] < 5, 'Unsupported scenery layer'
        assert 0 <= cell['palette'] < len(model['palettes']), 'Unknown palette'
        assert cell['animation_mask'] in (0, 1, 3, 7), 'Unsupported autoanimation mask'
        assert 0 <= cell['flip'] <= 3, 'Unsupported tile flip'
        assert len(model['palettes'][cell['palette']]) == 16
        for phase in range(8):
            assert 0 <= (cell['code'] | (phase & cell['animation_mask'])) < len(crom) // 128
        assert -32768 <= cell['x'] <= 32736 and -32768 <= cell['y'] <= 32736


def layer_origins(model):
    """Keep every uniformly aligned source tile on a 16-pixel subrectangle.

    Scaling a single 32-pixel RDP rectangle can round differently from the
    original 16-pixel rectangles. A layer-specific bake origin lets the
    renderer split the uploaded page at the original raster boundaries.
    Mixed prop coordinates have no common grid and use origin zero per axis.
    """
    grouped = defaultdict(list)
    for cell in model['cells']:
        grouped[cell['layer']].append(cell)
    result = {}
    for layer, cells in grouped.items():
        remainders = [{c[axis] % 16 for c in cells} for axis in ('x', 'y')]
        result[layer] = tuple(next(iter(r)) if len(r) == 1 else 0 for r in remainders)
    return result


def _chunk_start(coordinate, origin):
    return (coordinate - origin) // SIDE * SIDE + origin


def compose_chunks(model, crom):
    """Return (layer,x,y)->eight pixel lists, preserving source overlap order."""
    _validate(model, crom)
    palettes = [rgba16(p) for p in model['palettes']]
    origins = layer_origins(model)
    decoded = {}
    phases = []
    for phase in range(8):
        blocks = {}
        for cell in model['cells']:
            key = (cell['code'] | (phase & cell['animation_mask']), cell['flip'])
            if key not in decoded:
                decoded[key] = decode(crom, *key)
            pens = decoded[key]
            palette = palettes[cell['palette']]
            for row in range(16):
                y = cell['y'] + row
                for col in range(16):
                    pen = pens[row * 16 + col]
                    if not pen:
                        continue  # Transparent later tiles never erase earlier props.
                    x = cell['x'] + col
                    ox, oy = origins[cell['layer']]
                    bx, by = _chunk_start(x, ox), _chunk_start(y, oy)
                    address = (cell['layer'], bx, by)
                    if address not in blocks:
                        blocks[address] = [0] * (SIDE * SIDE)
                    blocks[address][(y - by) * SIDE + x - bx] = palette[pen]
        phases.append(blocks)
    locations = sorted(set().union(*(p.keys() for p in phases)))
    blank = [0] * (SIDE * SIDE)
    return {location: [p.get(location, blank) for p in phases] for location in locations}


def verify_composition(model, crom, blocks):
    """Compare to a full-layer raster using a separate C-ROM pixel decoder.

    This intentionally composes pixels in world coordinates, without using
    chunk boundaries or the production decode() function. It detects dropped
    overlap, flip, autoanimation and negative-coordinate pixels.
    """
    by_layer = defaultdict(list)
    for cell in model['cells']:
        by_layer[cell['layer']].append(cell)
    checked = 0
    origins = layer_origins(model)
    for layer, cells in by_layer.items():
        ox, oy = origins[layer]
        x0 = _chunk_start(min(c['x'] for c in cells), ox)
        y0 = _chunk_start(min(c['y'] for c in cells), oy)
        x1 = _chunk_start(max(c['x'] + 15 for c in cells), ox) + SIDE
        y1 = _chunk_start(max(c['y'] + 15 for c in cells), oy) + SIDE
        width, height = x1 - x0, y1 - y0
        for phase in range(8):
            canvas = [0] * (width * height)
            for c in cells:
                palette = rgba16(model['palettes'][c['palette']])
                code = c['code'] | (phase & c['animation_mask'])
                for y in range(16):
                    sy = 15 - y if c['flip'] & 2 else y
                    for x in range(16):
                        sx = 15 - x if c['flip'] & 1 else x
                        base = code * 128 + (64 if sx < 8 else 0) + sy * 4
                        bit = 1 << (sx % 8)
                        pen = ((1 if crom[base] & bit else 0)
                               | (2 if crom[base + 2] & bit else 0)
                               | (4 if crom[base + 1] & bit else 0)
                               | (8 if crom[base + 3] & bit else 0))
                        if pen:
                            canvas[(c['y'] + y - y0) * width + c['x'] + x - x0] = palette[pen]
            for by in range(y0, y1, SIDE):
                for bx in range(x0, x1, SIDE):
                    baked = blocks.get((layer, bx, by))
                    for row in range(SIDE):
                        original = canvas[(by + row - y0) * width + bx - x0:
                                          (by + row - y0) * width + bx - x0 + SIDE]
                        actual = baked[phase][row * SIDE:(row + 1) * SIDE] if baked else [0] * SIDE
                        assert actual == original, ('Prerender mismatch', layer, phase, bx, by, row)
                        checked += SIDE
    return checked


def pack_prerender_map(model, crom, make_image, pixel_format='rgba16'):
    """Return (images, cells, report) compatible with scene_meta.h generation.

    The callback uses force_rgba=True (default) or force_ci8=True and must
    return the existing seven-field scene_image_t tuple. The caller provides
    a lossless shared palette for CI8. Pages may be deduplicated by the caller.
    """
    assert pixel_format in ('rgba16','ci8'), 'Unsupported baked pixel format'
    blocks = compose_chunks(model, crom)
    checked = verify_composition(model, crom, blocks)
    images, cells = [], []
    unique = {}
    for (layer, x, y), phases in blocks.items():
        ids = []
        for pixels in phases:
            raw = struct.pack('>1024H', *pixels)
            if raw not in unique:
                unique[raw] = len(images)
                image = make_image(pixels, SIDE, SIDE, **({'force_rgba':True} if pixel_format=='rgba16' else {'force_ci8':True}))
                assert len(image) == 7 and image[4:] == (SIDE, SIDE, 1 if pixel_format=='rgba16' else 2)
                images.append(image)
            ids.append(unique[raw])
        cells.append((layer, x, y, ids))
    report = dict(format='RGBA5551' if pixel_format=='rgba16' else 'CI8', chunk_width=SIDE, chunk_height=SIDE,
                  tmem_bytes_per_page=PAGE_BYTES if pixel_format=='rgba16' else SIDE*SIDE, phases=8,
                  layer_origins=layer_origins(model),
                  source_cells=len(model['cells']), baked_cells=len(cells),
                  unique_pages=len(images), decoded_bytes=len(images) * PAGE_BYTES,
                  animated_cells=sum(len(set(c[3])) > 1 for c in cells),
                  independent_comparison_pixels=checked,
                  scope='Only supplied observed scenery; no inferred terrain, actors or events')
    return images, cells, report


def load_model(map_path, props_path):
    model = json.loads(Path(map_path).read_text())
    props = json.loads(Path(props_path).read_text())
    base = len(model['palettes'])
    model['palettes'].extend(props['palettes'])
    model['cells'].extend(dict(c, palette=c['palette'] + base) for c in props['cells'])
    return model


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--map', type=Path, default=ROOT / 'filesystem/mission1/map.json')
    p.add_argument('--props', type=Path, default=ROOT / 'filesystem/mission1/props.json')
    p.add_argument('--crom', type=Path, default=ROOT / 'filesystem/sprites.bin')
    p.add_argument('--output', type=Path, default=ROOT / 'build/prerender-background')
    a = p.parse_args()
    model, crom = load_model(a.map, a.props), a.crom.read_bytes()
    pages = []
    def make_image(pixels, width, height, *, force_rgba):
        assert force_rgba and (width, height) == (SIDE, SIDE)
        index = len(pages)
        pages.append(struct.pack('>1024H', *pixels))
        return index, 65535, 0, 0, width, height, 1
    images, cells, report = pack_prerender_map(model, crom, make_image)
    # Standalone artifact: independently compressed page records. Runtime scene
    # integration can instead aggregate these same pages in its existing cache.
    blob, records = bytearray(), []
    for page in pages:
        packed = zlib.compress(page)
        codec = 'zlib' if len(packed) < len(page) else 'raw'
        data = packed if codec == 'zlib' else page
        records.append(dict(offset=len(blob), length=len(data), crc32=f'{zlib.crc32(page):08x}', codec=codec))
        blob.extend(data)
    a.output.mkdir(parents=True, exist_ok=True)
    (a.output / 'background.pack').write_bytes(blob)
    report.update(packed_bytes=len(blob), sha256=hashlib.sha256(blob).hexdigest(),
                  source_hashes={str(path): hashlib.sha256(path.read_bytes()).hexdigest()
                                 for path in (a.map, a.props, a.crom)})
    (a.output / 'background.json').write_text(json.dumps(dict(report=report, images=images, cells=cells, pages=records), indent=2) + '\n')
    print(json.dumps(report, indent=2))


if __name__ == '__main__':
    main()
