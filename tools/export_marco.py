#!/usr/bin/env python3
"""Reconstruct Marco from original VRAM + palette; require screenshot agreement.

MAME: mslugx, euro BIOS, mission 1 initial area. Palette 0x63 is Marco in
these captures. This is a specific verified extraction, not a universal parser.
Source layout: MAME neogeo_spr.cpp; resistor conversion: neogeo_v.cpp.
"""
import argparse
from array import array
import hashlib
import json
from pathlib import Path
import struct
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
SAMPLES = (list(range(2100, 2220, 4)), list(range(2300, 2420, 4)))
WIDTH, HEIGHT = 48, 64
INTERVAL = 4
ACTIONS = False
MOTION = False
CONDUCTANCE = tuple(1 / r for r in (3900, 2200, 1000, 470, 220))


def palette_rgb(word):
    channels = (((word >> 14) & 1) | ((word >> 7) & 30),
                ((word >> 13) & 1) | ((word >> 3) & 30),
                ((word >> 12) & 1) | ((word << 1) & 30))
    denominator = sum(CONDUCTANCE) + (1 / 8200 if word & 0x8000 else 0)
    return tuple(round(255 * sum(g for bit, g in enumerate(CONDUCTANCE)
                                if value >> bit & 1) / denominator)
                 for value in channels)


def reconstruct(vram, palette, crom, selected_palette=0x63, x_range=None):
    image = Image.new('RGBA', (320, 256))
    indices = bytearray(320*256)
    palette_indices = array('H',[0])*(320*256)
    owners = array('i',[-1])*(320*256)   # the record (16x16 tile) on top at each pixel
    selected_palettes = {selected_palette} if isinstance(selected_palette,int) else set(selected_palette)
    records = []
    x = y = rows = zx = zy = 0
    for slot in range(1, 381):
        control, zoom = vram[0x8200 + slot], vram[0x8000 + slot]
        if control & 64:
            x = (x + zx + 1) & 511
            zx = (zoom >> 8) & 15
        else:
            x = vram[0x8400 + slot] >> 7
            y = (512 - (control >> 7)) & 511
            rows, zy, zx = control & 63, zoom & 255, (zoom >> 8) & 15
        if not 0 < rows <= 32:
            continue
        for row in range(rows):
            low, attr = vram[slot * 64 + row * 2:slot * 64 + row * 2 + 2]
            if attr >> 8 not in selected_palettes:
                continue
            if x_range is not None and not x_range[0] <= x < x_range[1]:
                continue
            assert zx == 15 and zy == 255, 'Unimplemented sprite zoom'
            assert not attr & 12, 'Unimplemented auto-animation'
            tile = low | ((attr << 12) & 0xf0000)
            assert tile < len(crom) // 128
            records.append(dict(slot=slot, tile=tile, attr=attr, x=x, y=y+row*16))
            for py in range(16):
                for px in range(16):
                    xx, yy = (15-px if attr & 1 else px), (15-py if attr & 2 else py)
                    offset = tile * 128 + (64 if xx < 8 else 0) + yy * 4
                    pen = sum(((crom[offset+b] >> (xx % 8)) & 1) << bit
                              for bit, b in enumerate((0, 2, 1, 3)))
                    dx, dy = (x+px)&511, (y+row*16+py)&511
                    if pen and 0 <= dx < 320 and 0 <= dy < 256:
                        image.putpixel((dx, dy), palette_rgb(palette[(attr>>8)*16+pen])+(255,))
                        indices[dy*320+dx] = pen
                        palette_indices[dy*320+dx] = (attr>>8)*16+pen
                        owners[dy*320+dx] = len(records)-1
    image.info["neo_owners"] = owners
    image.info["neo_indices"] = bytes(indices)
    image.info["neo_palette_indices"] = palette_indices.tobytes()
    return image, records


def top_sprite_pixel(vram, palette, crom, px, py):
    """Resolve a reference mismatch only when a later non-actor sprite covers it.

    Projectiles can cover the muzzle. Retain the actor artwork, but verify the
    occluder's actual color against MAME instead of accepting an arbitrary mask.
    """
    result = None
    x = y = rows = zx = zy = 0
    for slot in range(1, 381):
        c, z = vram[0x8200+slot], vram[0x8000+slot]
        if c & 64:
            x = (x+zx+1) & 511; zx = (z >> 8) & 15
        else:
            x = vram[0x8400+slot] >> 7; y = (512-(c >> 7)) & 511
            rows, zx, zy = c & 63, (z >> 8) & 15, z & 255
        dy = (py-y) & 511
        if not rows or ((px-x)&511) >= 16 or dy >= min(rows,32)*16:
            continue
        if zx != 15 or zy != 255:
            result = None; continue
        low, attr = vram[slot*64+(dy//16)*2:slot*64+(dy//16)*2+2]
        if attr & 12:
            result = None; continue
        code = low | ((attr << 12) & 0xf0000)
        sx, sy = (px-x)&511, dy%16
        if attr & 1: sx = 15-sx
        if attr & 2: sy = 15-sy
        offset = code*128+(64 if sx<8 else 0)+sy*4
        pen = sum(((crom[offset+b] >> (sx%8)) & 1) << bit
                  for bit,b in enumerate((0,2,1,3)))
        if pen:
            result = (attr >> 8, palette_rgb(palette[(attr >> 8)*16+pen]))
    return result


def export(captures, output, secondary=None):
    crom = (ROOT / 'filesystem/sprites.bin').read_bytes()
    data = bytearray(struct.pack('>4s6H', b'MSXA', WIDTH, HEIGHT, 30, 30, INTERVAL, 0))
    indexed = bytearray(struct.pack('>4s6H', b'MSXI', WIDTH, HEIGHT, 30, 30, INTERVAL, 0))
    report = dict(source='MAME 0.276 / mslugx / euro / palette 0x63', frames=[], matched_pixels=0, verified_occluded_pixels=0)
    preview = Image.new('RGBA', (WIDTH*15, HEIGHT*4))
    for sequence_index, sequence in enumerate(SAMPLES):
        folder=secondary if sequence_index==1 and secondary is not None else captures
        for frame in sequence:
            vram = struct.unpack('>34304H', (folder/f'vram{frame}.bin').read_bytes())
            palette = struct.unpack('>4096H', (folder/f'palette{frame}.bin').read_bytes())
            image, records = reconstruct(vram, palette, crom)
            reference = Image.open(folder/f'game{frame}.png').convert('RGB')
            bbox = image.getbbox()
            assert bbox, f'No actor in {frame}'
            left, top, right, bottom = bbox
            assert right-left <= WIDTH and bottom-top <= 56, f'Actor too large in {frame}'
            for y in range(top, bottom):
                for x in range(left, right):
                    color = image.getpixel((x, y))
                    if color[3]:
                        assert 16 <= y < 240, f'Actor outside reference {frame}'
                        ref = reference.getpixel((x, y-16))
                        if max(abs(a-b) for a,b in zip(color[:3], ref)) <= 1:
                            report['matched_pixels'] += 1
                        else:
                            occluder = top_sprite_pixel(vram,palette,crom,x,y)
                            assert occluder and occluder[0] != 0x63 and max(abs(a-b) for a,b in zip(occluder[1],ref)) <= 1, (frame,x,y,color,ref)
                            report['verified_occluded_pixels'] += 1
            normalized = Image.new('RGBA', (WIDTH, HEIGHT))
            # Bottom-center alignment is provisional; not a decoded game pivot.
            offset_x = WIDTH//2 + left-120 if ACTIONS else (WIDTH-(right-left))//2
            if MOTION:
                feet = [x for y in range(max(top,bottom-8),bottom) for x in range(left,right) if image.getpixel((x,y))[3]]
                pivot = (min(feet)+max(feet)+1)//2
                offset_x = WIDTH//2 + left-pivot
            assert offset_x>=0 and offset_x+(right-left)<=WIDTH, (frame, 'Actor canvas overflow')
            offset_y = 56-(bottom-top)
            normalized.paste(image.crop(bbox), (offset_x, offset_y))
            raw_indices = bytearray(WIDTH*HEIGHT)
            original_indices = image.info['neo_indices']
            for yy in range(top,bottom):
                for xx in range(left,right):
                    raw_indices[(offset_y+yy-top)*WIDTH+offset_x+xx-left] = original_indices[yy*320+xx]
            for pen in range(16):
                r,g,b = palette_rgb(palette[0x630+pen])
                color = ((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|1 if pen else 0
                indexed.extend(struct.pack('>H',color))
            indexed.extend((raw_indices[i]<<4)|raw_indices[i+1] for i in range(0,len(raw_indices),2))
            index = len(report['frames'])
            preview.paste(normalized, ((index%15)*WIDTH, (index//15)*HEIGHT))
            raw = bytearray()
            for y in range(HEIGHT):
                for x in range(WIDTH):
                    r,g,b,a = normalized.getpixel((x,y))
                    pixel = ((r>>3)<<11)|((g>>3)<<6)|((b>>3)<<1)|(1 if a else 0)
                    raw.extend(struct.pack('>H', pixel))
            data.extend(raw)
            report['frames'].append(dict(source_frame=frame, bounds=bbox, sprites=records,
                                         sha256=hashlib.sha256(raw).hexdigest()))
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_bytes(data)
    output.with_suffix('.idx4').write_bytes(indexed)
    output.with_suffix('.json').write_text(json.dumps(report, indent=2)+'\n')
    preview.save(output.with_suffix('.png'))
    print(f"PASS: {len(report['frames'])} frames, {report['matched_pixels']} original pixels matched; {len(data)} bytes")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('captures', type=Path)
    parser.add_argument('--actions', action='store_true')
    parser.add_argument('--motion', action='store_true')
    parser.add_argument('--secondary-captures', type=Path)
    parser.add_argument('--output', type=Path, default=ROOT/'filesystem/marco.bin')
    args = parser.parse_args()
    if args.actions:
        ACTIONS = True; WIDTH = 80; INTERVAL = 2
        SAMPLES = (list(range(2192,2252,2)),list(range(2300,2360,2)))
        if args.output == ROOT/'filesystem/marco.bin': args.output = ROOT/'filesystem/marco-actions.bin'
    if args.motion:
        MOTION = True; WIDTH = 96; INTERVAL = 2
        SAMPLES = (list(range(2300,2360,2)),list(range(2192,2252,2)))
        if args.output == ROOT/'filesystem/marco.bin': args.output = ROOT/'filesystem/marco-motion.bin'
    export(args.captures, args.output, args.secondary_captures)
