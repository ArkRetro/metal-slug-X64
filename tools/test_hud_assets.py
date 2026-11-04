#!/usr/bin/env python3
"""Synthetic FIX nibble/atlas boundaries, independent of proprietary assets."""
import unittest

from build_hud import make_atlas, rgba16_palette


class HudAtlasTests(unittest.TestCase):
    def test_column_decode_and_atlas_nibbles(self):
        source = bytearray(131072)
        for character in range(32, 96):
            for y in range(8):
                for x in range(8):
                    pen = (character + x + y) & 15
                    address = (0x300 + character) * 32 + (16, 24, 0, 8)[x // 2] + y
                    source[address] |= pen << ((x & 1) * 4)
        atlas = make_atlas(source)
        self.assertEqual(len(atlas), 2048)
        for i in range(4096):
            x, y = i % 64, i // 64
            character = 32 + x // 8 + y // 8 * 8
            pen = (atlas[i // 2] >> (0 if x & 1 else 4)) & 15
            self.assertEqual(pen, (character + x % 8 + y % 8) & 15)

    def test_transparent_pen_zero_and_opaque_black(self):
        palette = rgba16_palette([0] * 16)
        self.assertEqual(palette[0], 0)
        self.assertEqual(palette[1:], [1] * 15)

    def test_bad_source_size_rejected(self):
        with self.assertRaises(ValueError):
            make_atlas(bytes(1))


if __name__ == '__main__':
    unittest.main()
