#!/usr/bin/env python3
"""Exercise chunk boundaries, opacity, animation and independent pixel audit."""
import unittest
from build_prerender_background import compose_chunks, layer_origins, pack_prerender_map, verify_composition
from mission_tiles import rgba16


def encode_tiles(tiles):
    out = bytearray(len(tiles) * 128)
    for code, pixels in enumerate(tiles):
        for y in range(16):
            for x in range(16):
                offset = code * 128 + (64 if x < 8 else 0) + y * 4
                for pen_bit, byte in enumerate((0, 2, 1, 3)):
                    if pixels[y * 16 + x] & (1 << pen_bit):
                        out[offset + byte] |= 1 << (x & 7)
    return bytes(out)


def cell(x=0, y=0, code=0, layer=0, flip=0, animation_mask=0):
    return dict(x=x, y=y, code=code, layer=layer, flip=flip,
                animation_mask=animation_mask, palette=0)


def model(*cells):
    return dict(cells=list(cells), palettes=[[0] + [0x1000 + i * 0x111 for i in range(1, 16)]])


class PrerenderTests(unittest.TestCase):
    def test_negative_position_and_crossing_four_chunks(self):
        # Mixed alignment falls back to world origin zero. The transparent
        # second source tile affects alignment without contributing pixels.
        m = model(cell(-1, -1), cell(32, 32, code=1))
        crom = encode_tiles([[1] * 256, [0] * 256])
        blocks = compose_chunks(m, crom)
        self.assertEqual(set(blocks), {(0, -32, -32), (0, 0, -32), (0, -32, 0), (0, 0, 0)})
        self.assertEqual(sum(bool(v) for b in blocks.values() for v in b[0]), 256)
        self.assertGreater(verify_composition(m, crom, blocks), 0)

    def test_transparent_later_prop_preserves_underlying_pixels(self):
        top = [0] * 256
        top[0] = 2
        m = model(cell(675, 95, layer=4), cell(675, 95, code=1, layer=4))
        blocks = compose_chunks(m, encode_tiles([[1] * 256, top]))
        palette = rgba16(m['palettes'][0])
        b = blocks[(4, 675, 79)][0]
        self.assertEqual(b[16 * 32], palette[2])
        self.assertEqual(b[16 * 32 + 1], palette[1])
        # Reversing SCB order must visibly change the overlapping pixel.
        m['cells'].reverse()
        reversed_blocks = compose_chunks(m, encode_tiles([[1] * 256, top]))
        self.assertEqual(reversed_blocks[(4, 675, 79)][0][16 * 32], palette[1])

    def test_all_eight_autoanimation_phases_and_dedup(self):
        m = model(cell(animation_mask=7), cell(32, 0, animation_mask=7))
        crom = encode_tiles([[phase + 1] * 256 for phase in range(8)])
        calls = []
        def image(pixels, w, h, *, force_rgba):
            self.assertTrue(force_rgba)
            calls.append(pixels)
            return len(calls) - 1, 65535, 0, 0, w, h, 1
        images, cells, report = pack_prerender_map(m, crom, image)
        self.assertEqual(len(images), 8)
        self.assertEqual(cells[0][3], cells[1][3])
        self.assertEqual(len(set(cells[0][3])), 8)
        self.assertEqual(report['tmem_bytes_per_page'], 2048)
        self.assertEqual(report['animated_cells'], 2)
        self.assertEqual(calls[7][0], rgba16(m['palettes'][0])[8])

    def test_flips_are_baked_without_losing_palette(self):
        tile = [0] * 256
        tile[0], tile[15], tile[240], tile[255] = 1, 2, 3, 4
        m = model(*(cell(i * 32, 0, flip=i) for i in range(4)))
        blocks = compose_chunks(m, encode_tiles([tile]))
        palette = rgba16(m['palettes'][0])
        self.assertEqual([blocks[(0, i * 32, 0)][0][0] for i in range(4)],
                         [palette[i] for i in (1, 2, 3, 4)])
        verify_composition(m, encode_tiles([tile]), blocks)

    def test_audit_detects_changed_pixel_and_missing_block(self):
        m, crom = model(cell()), encode_tiles([[1] * 256])
        blocks = compose_chunks(m, crom)
        blocks[(0, 0, 0)][0][0] = 0
        with self.assertRaisesRegex(AssertionError, 'Prerender mismatch'):
            verify_composition(m, crom, blocks)
        with self.assertRaisesRegex(AssertionError, 'Prerender mismatch'):
            verify_composition(m, crom, {})

    def test_uniform_layer_alignment_preserves_source_16px_edges(self):
        m = model(cell(0, 8), cell(16, 24), cell(0, -8, layer=1))
        crom = encode_tiles([[1] * 256])
        self.assertEqual(layer_origins(m), {0: (0, 8), 1: (0, 8)})
        blocks = compose_chunks(m, crom)
        self.assertEqual(set(blocks), {(0, 0, 8), (1, 0, -24)})
        # The second original tile starts at an exact 16x16 page subrect.
        palette = rgba16(m['palettes'][0])
        self.assertEqual(blocks[(0, 0, 8)][0][16 * 32 + 16], palette[1])
        self.assertEqual(blocks[(0, 0, 8)][0][16], 0)
        verify_composition(m, crom, blocks)

    def test_unknown_phase_mask_and_crom_tile_rejected(self):
        with self.assertRaisesRegex(AssertionError, 'autoanimation mask'):
            compose_chunks(model(cell(animation_mask=2)), encode_tiles([[1] * 256]))
        with self.assertRaises(AssertionError):
            compose_chunks(model(cell(code=1)), encode_tiles([[1] * 256]))


if __name__ == '__main__':
    unittest.main()
