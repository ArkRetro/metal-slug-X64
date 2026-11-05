#!/usr/bin/env python3
"""Build the four normal-pistol directions from the actual cartridge poses.

No rotation/resampling is applied. The original SELECT_FIELD_POSE table chooses
one of two C-ROM tiles and the original flip flags for each cardinal angle.
"""
import argparse
import hashlib
import json
import struct
import zipfile
from pathlib import Path

from build_marco_campaign_assets import rgba_palette, swapped
from import_rom import validate
from mission_tiles import decode

ROOT = Path(__file__).resolve().parents[1]


def build(rom, output, preview=None):
    validate(rom)
    with zipfile.ZipFile(rom) as archive:
        fixed = swapped(archive.read('250-p1.p1'))
        p2 = swapped(archive.read('250-p2.ep1'))
        crom = bytearray()
        for odd in (1,3,5):
            left = archive.read(f'250-c{odd}.c{odd}')
            right = archive.read(f'250-c{odd+1}.c{odd+1}')
            pair = bytearray(len(left)+len(right))
            pair[::2],pair[1::2] = left,right
            crom.extend(pair)
    assert fixed[0x1b782:0x1b78c] == bytes.fromhex('303c10f74eb9000120f8')
    program = struct.unpack_from('>I',fixed,0x1b79e)[0]
    assert program == 0x22c12e
    bank = p2[0x100000:0x200000]
    at = program-0x200000
    assert bank[at:at+6] == bytes.fromhex('0001106f0080')
    assert struct.unpack_from('>H',fixed,0x1b7ae)[0] == 0x202
    palette = rgba_palette(bank,0x202)
    directions = []
    for angle in (0,64,128,192):
        index = angle//16  # 01B7EE..01B804, exact at cardinal angles.
        site = at+6+index*8
        op,flags,dx,dy,descriptor = struct.unpack_from('>BBbbI',bank,site)
        assert op == 4 and dx == dy == 0
        groups,high,x,y,width,height,low = struct.unpack_from('>BBhhBBH',p2,descriptor-0x100000)
        assert (groups,x,y,width,height) == (1,-8,8,1,1)
        code = low | (high<<12)
        flips = (flags>>2)&3
        pens = bytes(decode(crom,code,flips))
        packed = bytes((pens[i]<<4)|pens[i+1] for i in range(0,256,2))
        opaque = [(i%16,i//16) for i,pen in enumerate(pens) if pen]
        bounds = [min(x for x,y in opaque),min(y for x,y in opaque),
                  max(x for x,y in opaque)+1,max(y for x,y in opaque)+1]
        directions.append(dict(angle=angle,source_program=program,source_pose=site+0x200000,
            source_descriptor=descriptor,source_flags=flags,code=code,
            width=16,height=16,pivot=[8+(flips&1),8+((flips>>1)&1)],bounds=bounds,
            palette_rgba5551=palette,indices_hex=packed.hex(),
            indices_sha256=hashlib.sha256(packed).hexdigest(),opaque_pixels=len(opaque)))
    assert [d['code'] for d in directions] == [0x2cbd3,0x2cbd7,0x2cbd3,0x2cbd7]
    assert [d['source_flags'] for d in directions] == [0x40,0x44,0x4c,0x48]
    # Independent horizontal artwork was previously checked against 792 MAME
    # framebuffer pixels. This source decoder must reproduce it exactly.
    legacy = ROOT/'filesystem/projectiles.json'
    if legacy.is_file():
        reference = json.loads(legacy.read_text())
        assert directions[0]['indices_hex'] == reference['indices_hex']
        assert palette == reference['palette_rgba5551']
    result = dict(source='Supplied original cartridge P1 initializer, P2 pose/descriptor/palette data, C-ROM',
        sound_token=0x10f7,palette_source_descriptor=0x202,palette_source_address=0x204040,
        directions=directions,
        provenance=dict(rom_sha256=hashlib.sha256(rom.read_bytes()).hexdigest(),
            fixed_sha256=hashlib.sha256(fixed).hexdigest(),bank_sha256=hashlib.sha256(bank).hexdigest(),
            crom_sha256=hashlib.sha256(crom).hexdigest()),
        limits='Normal pistol cardinal artwork only. Original flags are baked into pens; preserve per-direction pivot. No rotated horizontal substitute, playback timeline, impact art or weapon variant is included.')
    output.parent.mkdir(parents=True,exist_ok=True)
    output.write_text(json.dumps(result,indent=2)+'\n')
    if preview:
        from PIL import Image
        preview.mkdir(parents=True,exist_ok=True)
        colors = [((v>>11&31)*255//31,(v>>6&31)*255//31,(v>>1&31)*255//31,255 if v&1 else 0) for v in palette]
        for d in directions:
            packed = bytes.fromhex(d['indices_hex'])
            pens = [(packed[i//2]>>(0 if i&1 else 4))&15 for i in range(256)]
            image = Image.new('RGBA',(16,16));image.putdata([colors[pen] for pen in pens])
            image.save(preview/f"pistol-{d['angle']}.png")
    print('Built source pistol directions: '+', '.join(f"{d['angle']}:{d['bounds'][2]-d['bounds'][0]}x{d['bounds'][3]-d['bounds'][1]}" for d in directions))
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--rom',type=Path,required=True)
    parser.add_argument('--output',type=Path,default=ROOT/'filesystem/campaign-projectiles.json')
    parser.add_argument('--preview',type=Path)
    args = parser.parse_args()
    build(args.rom,args.output,args.preview)
