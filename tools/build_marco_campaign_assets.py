#!/usr/bin/env python3
"""Build live Marco table/program/CI4 resources directly from the local cartridge.

The campaign owns state, input and world services. This conversion contains no
captured actor state, animation timeline, screenshots or fixture outcomes. Only
normal form / Marco (selector zero) is bound. Other selectors stop explicitly.
Original callback requests retain their target, site and payload as data; the
native owner must implement them or stop the actor.
"""
import argparse
import hashlib
import json
import re
import struct
import subprocess
import zipfile
from pathlib import Path

from PIL import Image

from export_marco import palette_rgb
from import_rom import validate
from mission_tiles import decode

ROOT = Path(__file__).resolve().parents[1]


def swapped(data):
    result = bytearray(data)
    result[::2], result[1::2] = data[1::2], data[::2]
    return bytes(result)


def preserve_ci4_indices(path, pens, width, height):
    """Keep cartridge pens even when the PNG loader merges duplicate colors.

    mksprite owns the resource header/extension/palette. With compression off,
    the public sprite_t layout places row-packed CI4 pixels after eight bytes.
    LodePNG can remap two equal RGBA palette entries during 4-to-8-bit decode;
    restore the original indices so alternate native palettes stay correct.
    """
    data = bytearray(path.read_bytes())
    assert struct.unpack_from('>HH', data) == (width, height)
    assert data[5] & 0x1f == 8 and data[5] & 0x80, 'Expected extended uncompressed CI4 sprite'
    assert len(pens) == width * height and all(0 <= pen < 16 for pen in pens)
    stride = (width + 1) // 2
    payload = bytearray(stride * height)
    for y in range(height):
        for x in range(width):
            payload[y * stride + x // 2] |= pens[y * width + x] << (0 if x & 1 else 4)
    extension = (8 + len(payload) + 7) & ~7
    assert struct.unpack_from('>H', data, extension)[0] >= 124
    palette_at = struct.unpack_from('>I', data, extension + 4)[0]
    assert extension + 124 <= palette_at <= len(data) - 32
    data[8:8 + len(payload)] = payload
    path.write_bytes(data)


def lea(fixed, address):
    assert struct.unpack_from('>H', fixed, address)[0] == 0x41fa, hex(address)
    return address + 2 + struct.unpack_from('>h', fixed, address + 2)[0]


def source_keys(fixed, header, prefix):
    pcs = [0xc0000 + int(value, 16) for value in
           re.findall(r'\b' + prefix + r'_([0-9A-F]{4})\b', header.read_text())]
    return [lea(fixed, pc) for pc in pcs]


def graph(bank, roots):
    nodes, events = {}, []

    def walk(pc):
        while pc not in nodes:
            assert 0x200000 <= pc < 0x300000 and pc % 2 == 0, hex(pc)
            assert len(nodes) < 16384, 'Malformed animation graph'
            at = pc - 0x200000
            op, arg = bank[at:at + 2]
            node = dict(address=pc, kind='', asset=0, target=0,
                        operand=0, flags=0, dx=0, dy=0)
            nodes[pc] = node
            if op == 0:
                node.update(kind='WAIT', operand=arg)
                size = 2
            elif op == 4:
                node.update(kind='POSE', asset=struct.unpack_from('>I', bank, at + 4)[0],
                            flags=arg, dx=struct.unpack_from('b', bank, at + 2)[0],
                            dy=struct.unpack_from('b', bank, at + 3)[0])
                size = 8
            elif op == 8:
                assert arg == 0
                target = struct.unpack_from('>I', bank, at + 2)[0]
                node.update(kind='JUMP', target_address=target)
                walk(target)
                return
            elif op == 0xc:
                target = struct.unpack_from('>I', bank, at + 4)[0]
                node.update(kind='BRANCH_FIELD', operand=arg,
                            asset=struct.unpack_from('>H', bank, at + 2)[0],
                            target_address=target)
                walk(target)
                size = 8
            elif op == 0x10:
                length = struct.unpack_from('>H', bank, at + 2)[0]
                assert length and length % 8 == 0 and length // 8 <= 255
                node.update(kind='SELECT_FIELD_POSE', asset=arg, operand=length // 8,
                            target_address=pc + 4)
                for offset in range(0, length, 8):
                    where = at + 4 + offset
                    assert bank[where] == 4
                    nodes[pc + 4 + offset] = dict(
                        address=pc + 4 + offset, kind='POSE', target=0, operand=0,
                        flags=bank[where + 1], dx=struct.unpack_from('b', bank, where + 2)[0],
                        dy=struct.unpack_from('b', bank, where + 3)[0],
                        asset=struct.unpack_from('>I', bank, where + 4)[0])
                size = 4 + length
            elif op in (0x14, 0x30):
                if op == 0x14:
                    assert arg >= 4
                    target = struct.unpack_from('>I', bank, at + 2)[0]
                    payload = bank[at + 6:at + 2 + arg]
                    size = 2 + arg
                else:
                    target = 0x120f8
                    payload = bank[at + 2:at + 4]
                    size = 4
                if op == 0x14 and target == 0x1adaa and not payload:
                    node.update(kind='FLIP_FACING')
                else:
                    node.update(kind='EVENT', asset=len(events))
                    events.append(dict(site=pc, target=target, payload=payload.hex(),
                                       service=op == 0x30))
            elif op in (0x18, 0x1c):
                value = struct.unpack_from('>H', bank, at + 2)[0]
                node.update(kind='WRITE_BYTE' if op == 0x18 else 'WRITE_WORD',
                            asset=arg if op == 0x18 else value,
                            target=value if op == 0x18 else arg)
                size = 4
            elif op == 0x20:
                node.update(kind='FIELD50' if arg == 0x50 else 'WRITE_LONG', target=arg,
                            asset=struct.unpack_from('>I', bank, at + 2)[0])
                size = 6
            elif op == 0x24:
                node.update(kind='DECREMENT_BYTE', target=arg)
                size = 2
            elif op == 0x28:
                node.update(kind='FINISH')
                return
            else:
                raise ValueError(f'Unrecovered animation opcode {op:02x} at {pc:06x}')
            pc += size

    for root in sorted(roots):
        walk(root)
    ordered = sorted(nodes)
    indices = {pc: index for index, pc in enumerate(ordered)}
    for node in nodes.values():
        if 'target_address' in node:
            node['target'] = indices[node.pop('target_address')]
        if node['kind'] == 'SELECT_FIELD_POSE':
            assert all(nodes[ordered[node['target'] + i]]['kind'] == 'POSE'
                       for i in range(node['operand']))
    return [nodes[pc] for pc in ordered], indices, events


def rgba_palette(bank, descriptor):
    # The original palette loader transposes cartridge RGB555 to Neo Geo bits.
    # Apply the same resistor-DAC conversion used by the independent SCB audit.
    words = struct.unpack_from('>16H', bank, descriptor * 32)
    result = [0]
    for word in words[1:]:
        red, green, blue = word >> 10 & 31, word >> 5 & 31, word & 31
        neo = ((red >> 1) << 8) | ((green >> 1) << 4) | (blue >> 1)
        neo |= ((red & 1) << 14) | ((green & 1) << 13) | ((blue & 1) << 12)
        neo |= word & 0x8000
        red, green, blue = palette_rgb(neo)
        result.append(((red >> 3) << 11) | ((green >> 3) << 6) | ((blue >> 3) << 1) | 1)
    return result


def textures(p2, crom, descriptors, palettes, output, mksprite):
    png_dir = output / 'marco-campaign-png'
    png_dir.mkdir(parents=True, exist_ok=True)
    sprite_dir = ROOT / 'romfs/marco-campaign'
    sprite_dir.mkdir(parents=True, exist_ok=True)
    colors = [((v >> 11 & 31) * 255 // 31, (v >> 6 & 31) * 255 // 31,
               (v >> 1 & 31) * 255 // 31) for v in palettes[0]]
    parts, poses, paths, original_pixels = [], [], [], []
    for address in sorted(descriptors):
        assert 0x100000 <= address < 0x200000, hex(address)
        at = address - 0x100000
        groups, high = p2[at:at + 2]
        at += 2
        assert 0 < groups <= 32
        opaque = {}
        for _ in range(groups):
            dx, dy, width, height = struct.unpack_from('>hhBB', p2, at)
            at += 6
            assert width and height and width * height <= 256
            for col in range(width):
                for row in range(height):
                    code = struct.unpack_from('>H', p2, at)[0] | (high << 12)
                    at += 2
                    tile = decode(crom, code, 0)
                    for y in range(16):
                        for x in range(16):
                            pen = tile[y * 16 + x]
                            if pen:
                                opaque[(dx + col * 16 + x, -dy + row * 16 + y)] = pen
        assert opaque, hex(address)
        left = min(x for x, y in opaque)
        top = min(y for x, y in opaque)
        right = max(x for x, y in opaque) + 1
        bottom = max(y for x, y in opaque) + 1
        first = len(parts)
        for top_part in range(top, bottom, 64):
            for left_part in range(left, right, 64):
                width, height = min(64, right - left_part), min(64, bottom - top_part)
                pens = [opaque.get((left_part + x, top_part + y), 0)
                        for y in range(height) for x in range(width)]
                image = Image.new('P', (width, height))
                image.putpalette([v for color in colors for v in color])
                image.putdata(pens)
                image.info['transparency'] = bytes([0] + [255] * 15)
                path = png_dir / f'p{len(parts)}.png'
                image.save(path, bits=4, optimize=False)
                assert list(Image.open(path).tobytes()) == pens
                fingerprint = 2166136261
                for pen in pens:
                    fingerprint = ((fingerprint ^ pen) * 16777619) & 0xffffffff
                paths.append(str(path))
                original_pixels.append((pens, width, height))
                parts.append([left_part, top_part, width, height, fingerprint])
        poses.append([address, first, len(parts) - first])
    subprocess.run([str(mksprite), '-f', 'CI4', '-D', 'NONE', '-c', '0', '-o',
                    str(sprite_dir), *paths], check=True, stdout=subprocess.DEVNULL)
    for index, (pens, width, height) in enumerate(original_pixels):
        preserve_ci4_indices(sprite_dir / f'p{index}.sprite', pens, width, height)
    lines = ['/* Original cartridge descriptors/C-ROM; generated offline. */',
             '#ifndef MSX_MARCO_CAMPAIGN_POSE_META_H', '#define MSX_MARCO_CAMPAIGN_POSE_META_H',
             f'#define MARCO_CAMPAIGN_POSES {len(poses)}u',
             f'#define MARCO_CAMPAIGN_PARTS {len(parts)}u',
             f'#define MARCO_CAMPAIGN_PALETTES {len(palettes)}u',
             'static const uint16_t marco_campaign_palettes[][16] __attribute__((aligned(16)))={' +
             ','.join('{' + ','.join(map(str, palette)) + '}' for palette in palettes) + '};',
             'static const struct {int16_t x,y;uint16_t width,height;uint32_t indices_fnv;} marco_campaign_parts[]={']
    lines += ['{' + ','.join(map(str, part)) + 'u},' for part in parts]
    lines += ['};', 'static const struct {uint32_t source_tag;uint16_t first,count;} marco_campaign_poses[]={']
    lines += ['{' + ','.join(map(str, pose)) + '},' for pose in poses]
    lines += ['};', '#endif']
    (output / 'marco_campaign_pose_meta.h').write_text('\n'.join(lines) + '\n')
    return poses, parts, sprite_dir


def build(args):
    validate(args.rom)
    with zipfile.ZipFile(args.rom) as archive:
        fixed = swapped(archive.read('250-p1.p1'))
        p2 = swapped(archive.read('250-p2.ep1'))
        # ROM_LOAD16_BYTE layout, reconstructed from the validated set. Do not
        # trust an unrelated or stale filesystem/sprites.bin import.
        crom = bytearray()
        for odd in (1, 3, 5):
            left = archive.read(f'250-c{odd}.c{odd}')
            right = archive.read(f'250-c{odd + 1}.c{odd + 1}')
            pair = bytearray(len(left) + len(right))
            pair[0::2], pair[1::2] = left, right
            crom.extend(pair)
    bank = p2[0x100000:0x200000]
    tags = {
        'upper': source_keys(fixed, ROOT / 'src/marco_upper.h', 'MUT'),
        'motion': source_keys(fixed, ROOT / 'src/marco_motion.h', 'MMT'),
        'crouch': source_keys(fixed, ROOT / 'src/marco_crouch.h', 'MCT'),
        'stance': source_keys(fixed, ROOT / 'src/marco_stance.h', 'MST'),
    }
    upper_fire = [list(struct.unpack_from('>16I', fixed, pc))
                  for pc in (0xc3c10, 0xc3c50, 0xc3c90)]
    # C248C's fire table is indexed by the weapon kind before C0F32.
    crouch_fire = list(struct.unpack_from('>16I', fixed, 0xc24de))
    # Assert against the original C246A absolute LEA source, not fixture state.
    assert crouch_fire[0] == 0xbf058, 'Crouch fire table moved or wrong cartridge'
    table_tags = set(sum(tags.values(), [])) | set(sum(upper_fire, [])) | set(crouch_fire)
    table_values = {tag: list(struct.unpack_from('>4I', fixed, tag)) for tag in sorted(table_tags)}
    roots = {values[0] for values in table_values.values() if values[0]}
    commands, indices, events = graph(bank, roots)
    args.output.mkdir(parents=True, exist_ok=True)
    arr = lambda values: '{' + ','.join(str(v) + 'u' for v in values) + '}'
    lines = ['/* Real cartridge table/program data; no captured controller timeline. */',
             '#ifndef MSX_MARCO_CAMPAIGN_ASSETS_H', '#define MSX_MARCO_CAMPAIGN_ASSETS_H',
             '#include <stddef.h>', '#include "marco_controller.h"',
             'static const neo_anim_command_t marco_campaign_commands[]={']
    for command in commands:
        lines.append('{NEO_ANIM_' + command['kind'] + ',' + ','.join(str(command[k])
                     for k in ('asset', 'target', 'operand', 'flags', 'dx', 'dy')) + '},')
    lines += ['};', 'static const neo_anim_program_t marco_campaign_script={marco_campaign_commands,' +
              str(len(commands)) + '};', 'static const uint32_t marco_campaign_source_commands[]=' +
              arr([command['address'] for command in commands]) + ';']
    for part, values in tags.items():
        lines.append('static const uint32_t marco_campaign_' + part + '_tags[]=' + arr(values) + ';')
    lines += ['static const uint32_t marco_campaign_upper_fire_tags[3][16]={' +
              ','.join(arr(row) for row in upper_fire) + '};',
              'static const uint32_t marco_campaign_crouch_fire_tags[16]=' + arr(crouch_fire) + ';',
              'static const uint16_t marco_campaign_compatibility[]=' +
              arr(struct.unpack_from('>8H', fixed, 0xbedc0)) + ';',
              'static const uint8_t marco_campaign_combo_limits[]=' + arr(fixed[0xc38fc:0xc393c]) + ';']
    for index, (tag, values) in enumerate(table_values.items()):
        lines.append(f'static const marco_program_slot_t marco_campaign_slots_{index}[]={{')
        for character, pointer in enumerate(values):
            if not pointer:
                lines.append('{NULL,0,MARCO_PROGRAM_ABSENT},')
            elif character == 0:
                lines.append('{&marco_campaign_script,' + str(indices[pointer]) + ',MARCO_PROGRAM_READY},')
            else:
                lines.append('{NULL,0,MARCO_PROGRAM_UNKNOWN},')
        lines.append('};')
    lines.append('static const struct {uint32_t tag;marco_program_table_t table;} marco_campaign_asset_tables[]={')
    lines += ['{' + str(tag) + 'u,{marco_campaign_slots_' + str(index) + ',4}},'
              for index, tag in enumerate(table_values)]
    lines += ['};', 'static const struct {uint32_t target,site;uint8_t payload[16],length;bool service;} marco_campaign_asset_events[]={']
    for event in events:
        payload = bytes.fromhex(event['payload'])
        assert len(payload) <= 16
        lines.append('{' + str(event['target']) + 'u,' + str(event['site']) + 'u,' +
                     arr(payload) + ',' + str(len(payload)) + ',' + str(int(event['service'])) + '},')
    lines += ['};', '''static inline bool asset_table(uint32_t tag,const marco_program_table_t **out) {
    if (!out) return false;
    for (unsigned i=0;i<sizeof(marco_campaign_asset_tables)/sizeof(*marco_campaign_asset_tables);i++)
        if (marco_campaign_asset_tables[i].tag==tag) {*out=&marco_campaign_asset_tables[i].table;return true;}
    return false;
}
static inline bool asset_event(unsigned index,uint32_t *target,uint32_t *site) {
    if (!target || !site || index>=sizeof(marco_campaign_asset_events)/sizeof(*marco_campaign_asset_events)) return false;
    *target=marco_campaign_asset_events[index].target;*site=marco_campaign_asset_events[index].site;return true;
}
static inline bool asset_program(uint32_t tag,const neo_anim_program_t **program,uint16_t *entry) {
    if (!program || !entry) return false;
    for (unsigned i=0;i<sizeof(marco_campaign_source_commands)/sizeof(*marco_campaign_source_commands);i++)
        if (marco_campaign_source_commands[i]==tag) {*program=&marco_campaign_script;*entry=(uint16_t)i;return true;}
    return false;
}''', '#endif']
    (args.output / 'marco_campaign_assets.h').write_text('\n'.join(lines) + '\n')
    palettes = [rgba_palette(bank, descriptor) for descriptor in (0x376, 0x390)]
    descriptors = {command['asset'] for command in commands if command['kind'] == 'POSE'}
    poses, parts, sprite_dir = textures(p2, crom, descriptors, palettes, args.output, args.mksprite)
    sha = lambda data: hashlib.sha256(data).hexdigest()
    report = dict(source='Original cartridge P1 tables, P2 animation/descriptor/palette data and C-ROM',
                  selectors=dict(form=0, character=0), tables={hex(k): v for k, v in table_values.items()},
                  commands=commands, events=events, poses=poses, parts=parts,
                  palette_source_descriptors=[0x376, 0x390], palette_rgba5551=palettes,
                  provenance=dict(rom_sha256=sha(args.rom.read_bytes()), fixed_sha256=sha(fixed),
                                  bank_sha256=sha(bank), crom_sha256=sha(crom)),
                  limits='Static ROM reachability is not execution coverage. Native callback handlers must implement requests or fail. Unsupported forms/characters are UNKNOWN; no alternate poses or assets are substituted.')
    (args.output / 'marco_campaign_assets.json').write_text(json.dumps(report, indent=2) + '\n')
    (args.output / 'marco-campaign-assets.stamp').write_text(
        f'{len(table_values)} tables; {len(commands)} commands; {len(poses)} descriptors; {len(parts)} CI4 parts\n')
    print(f'Built live Marco: {len(table_values)} ROM tables, {len(commands)} commands, '
          f'{len(events)} explicit events, {len(poses)} descriptors, {len(parts)} CI4 parts; '
          f'{sum(p.stat().st_size for p in sprite_dir.glob("*.sprite"))} texture ROM bytes')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--rom', type=Path, required=True)
    parser.add_argument('--mksprite', type=Path, required=True)
    parser.add_argument('--output', type=Path, default=ROOT / 'build')
    build(parser.parse_args())
