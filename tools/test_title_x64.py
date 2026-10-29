#!/usr/bin/env python3
"""Regress the title flash/transparent FIX bug and authored motion boundaries."""
import numpy as np
from title_x64 import compose, digit_y

class FixedFont:
    def character(self, code):
        pens = np.zeros((8, 8), dtype=np.uint16)
        pens[0, 0] = 1
        return pens
    def colors(self, palette):
        return np.zeros((16, 3), dtype=np.uint8)

font = FixedFont()
digits = [np.full((47, 43, 4), 255, dtype=np.uint8) for _ in range(2)]
palettes = {line: None for line in range(16, 240)}
white = np.full((224, 320, 3), 255, dtype=np.uint8)
flash = compose(white, 640, digits, font, palettes)
# Each lit glyph writes one opaque pixel; its transparent pixels MUST retain flash.
black = np.all(flash == 0, axis=2)
assert black.sum() == len('PRESS START'), 'Transparent FIX pixels made a black bar'
assert np.all(flash[216:224, 208:296] == 255), 'Coin-counter removal damaged flash'
assert np.all(white == 255), 'Compositor modified source reference in place'
assert digit_y(0, 609) is None and digit_y(1, 619) is None
assert digit_y(0, 610) == -92 and digit_y(1, 620) == -92
assert digit_y(0, 635) == digit_y(1, 635) == 113
assert digit_y(0, 900) == digit_y(1, 900) == 113
print('PASS: transparent FIX text/counter preserve flash, source immutable, authored falls land together')
