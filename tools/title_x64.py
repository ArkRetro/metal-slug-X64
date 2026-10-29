"""Authored X64 title compositor. Timings here are authored, not ROM discoveries.

Original letters, particles and flash remain in the reconstructed source frames.
Only the extra digits and console prompts are authored. No runtime image filters.
"""
from pathlib import Path
import numpy as np
from PIL import Image

# Source-frame coordinates in the measured 59.185606 Hz title timeline.
DIGIT_KEYS = (
    ((610, -92), (635, 113), (637, 108), (646, 113)),
    ((620, -92), (635, 113), (637, 108), (646, 113)),
)
DIGIT_X = (211, 253)
DIGIT_SIZE = ((43, 47), (43, 47))


def load_digits(path):
    image = Image.open(path).convert('RGBA')
    alpha = image.getchannel('A').point(lambda p: 255 if p >= 200 else 0)
    image.putalpha(alpha)
    image = image.crop(alpha.getbbox())
    # Separate the generated paired numerals at the transparent inter-digit gap.
    a = np.asarray(image)[:, :, 3]
    column_count = (a > 0).sum(axis=0)
    center = len(column_count) // 2
    split = center - 130 + int(np.argmin(column_count[center - 130:center + 130]))
    digits = []
    for index, bounds in enumerate(((0, 0, split, image.height), (split, 0, image.width, image.height))):
        digit = image.crop(bounds)
        digit = digit.crop(digit.getchannel('A').getbbox())
        digit = digit.resize(DIGIT_SIZE[index], Image.Resampling.NEAREST)
        digits.append(np.array(digit))
    return digits


def digit_y(index, frame):
    keys = DIGIT_KEYS[index]
    if frame < keys[0][0]:
        return None
    for (start, first), (end, last) in zip(keys, keys[1:]):
        if frame < end:
            # Accelerating fall; linear rebound after the impact.
            u = (frame - start) / (end - start)
            if start == keys[0][0]:
                u *= u
            return round(first + (last - first) * u)
    return keys[-1][1]


def compose(rgb, frame, digits, renderer, palette_states):
    result = rgb.copy()
    # Remove the coin counter; it has no meaning on the N64 title screen.
    result[216:224, 208:296] = rgb[216:224, 8:9]
    # Preserve the source's blink cadence; replace INSERT COIN on its lit frames.
    lit = bool(np.max(rgb[192:200, 112:208]) > 100)
    result[192:200, 112:208] = rgb[192:200, 8:9]
    if lit:
        for column, character in enumerate('PRESS START'):
            pens = renderer.character(0x300 + ord(character))
            for row in range(8):
                colors = renderer.colors(palette_states[208 + row])
                target = result[192 + row, 112 + column * 8:120 + column * 8]
                opaque = pens[row] != 0
                target[opaque] = colors[pens[row][opaque]]
    for index, digit in enumerate(digits):
        y = digit_y(index, frame)
        if y is None:
            continue
        x = DIGIT_X[index]
        pixels = digit[:, :, :3].astype(np.float32)
        # Match the source's silver -> hot gold reveal; all integer pixels after packing.
        gold = min(1., max(0., (frame - 648) / 18))
        light = pixels.mean(axis=2) / 255
        warm = np.stack((np.minimum(255, light * 330), light * 170, light * 22), axis=2)
        pixels = pixels * (1 - gold) + warm * gold
        # The measured source flash covers the digits as well as the original letters.
        flash = float(np.median(rgb[0:16, 8:24])) / 255 if 638 <= frame <= 670 else 0
        pixels = pixels * (1 - flash) + 255 * flash
        top, bottom = max(0, y), min(224, y + digit.shape[0])
        if bottom <= top:
            continue
        source = slice(top - y, bottom - y)
        mask = digit[source, :, 3] >= 200
        area = result[top:bottom, x:x + digit.shape[1]]
        area[mask] = np.clip(pixels[source][mask], 0, 255).astype(np.uint8)
    return result
