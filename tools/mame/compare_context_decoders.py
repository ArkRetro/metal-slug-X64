#!/usr/bin/env python3
"""Compare candidate instruction boundaries with MAME's original CPU decoders.

RAM copy images are installed explicitly in an isolated frame callback, then
decoded without executing them. This is not dynamic code coverage or C proof.
"""
import argparse
import collections
import csv
import hashlib
import json
import os
import re
import subprocess
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def parse_assembly(path, cpu):
    result = {}
    width = 4 if cpu == 'm68000' else 2
    for line in path.read_text().splitlines():
        address, separator, body = line.partition(':')
        if not separator or not re.fullmatch('[0-9A-Fa-f]+', address):
            raise ValueError('Unexpected decoder output: ' + line)
        tokens = body.split()
        opcodes = []
        while tokens and re.fullmatch('[0-9A-F]{' + str(width) + '}', tokens[0]):
            opcodes.append(tokens.pop(0))
        assert opcodes and tokens, line
        pc = int(address, 16)
        assert pc not in result
        result[pc] = dict(bytes=bytes.fromhex(''.join(opcodes)), text=' '.join(tokens))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--rom-dir', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=False)
    for name in ('cfg', 'nvram', 'assembly'):
        (out / name).mkdir()
    base = ROOT / 'decomp/canonical-model'
    layout = json.loads((base / 'layout.json').read_text())
    contexts = {c['id']: c for c in layout['contexts']}
    manifest = json.loads((ROOT / 'tools/rom_manifest.json').read_text())
    with zipfile.ZipFile(args.rom_dir / 'mslugx.zip') as archive:
        images = {n: archive.read(n) for n in ('250-p1.p1', '250-m1.m1')}
    for name, data in images.items():
        assert hashlib.sha1(data).hexdigest() == manifest[name]['sha1']
    raw = images['250-p1.p1']
    swapped = bytearray(raw)
    swapped[::2], swapped[1::2] = raw[1::2], raw[::2]
    images['250-p1.p1'] = bytes(swapped)
    inputs, rows = {}, []
    for program in ('M68K_FIXED_P1', 'Z80_INITIAL_CART'):
        source = base / program / 'instructions.tsv'
        inputs[str(source.relative_to(ROOT))] = sha(source.read_bytes())
        with source.open() as stream:
            rows.extend(csv.DictReader(stream, delimiter='\t'))
    expected = {}
    groups = []
    for name in sorted({r['location'].split(':')[0] for r in rows}):
        context = contexts[name]
        image = b''.join(images[p['image']][p['offset']:p['offset'] + p['size']]
                         for p in context['source_pieces'])
        assert len(image) == context['size'] and sha(image) == context['sha256']
        expected[name] = image
        if name.startswith('RAM_COPY_'):
            (out / (name + '.bin')).write_bytes(image)
        sequence = sorted((int(r['location'].split(':')[1], 16), int(r['length']))
                          for r in rows if r['location'].split(':')[0] == name)
        for pc, size in sequence:
            if groups and groups[-1]['context'] == name and groups[-1]['end'] == pc:
                groups[-1]['end'] = pc + size
            else:
                groups.append(dict(context=name, start=pc, end=pc + size))
    lua = ["local frame=0", "emu.register_frame_done(function()", "frame=frame+1", "if frame~=2 then return end",
           "local main=manager.machine.devices[':maincpu'].spaces.program",
           "local audio=manager.machine.devices[':audiocpu'].spaces.program",
           "local ports=manager.machine.devices[':audiocpu'].spaces.io",
           "main:write_u8(0x3a0013,0)",  # cartridge vectors
           "main:write_u8(0x3a001b,0)"]  # cartridge audio fixed source
    for port, selector in ((8, 0x1e), (9, 0x0e), (10, 6), (11, 2)):
        lua.append(f'ports:read_u8(0x{(selector << 8) | port:x})')
    for name in expected:
        c = contexts[name]
        if name.startswith('RAM_COPY_'):
            lua += ['local f=assert(io.open(' + json.dumps(str(out / (name + '.bin'))) + ",'rb'))",
                    'local data=f:read("*a");f:close()',
                    f'for i=1,#data do main:write_u8({c["cpu_base"]}+i-1,data:byte(i)) end']
        space = 'main' if c['cpu'] == 'm68000' else 'audio'
        lua += ['local f=assert(io.open(' + json.dumps(str(out / (name + '-readback.bin'))) + ",'wb'))",
                f'f:write({space}:read_range({c["cpu_base"]},{c["cpu_base"] + c["size"] - 1},8));f:close()']
    for group in groups:
        c = contexts[group['context']]
        group['file'] = f'assembly/{group["context"]}-{group["start"]:08x}.asm'
        cpu = 'maincpu' if c['cpu'] == 'm68000' else 'audiocpu'
        # Bare hex such as a6/d0 can resolve to CPU registers in debugger expressions.
        command = f'dasm "{out / group["file"]}",0x{group["start"]:x},0x{group["end"] - group["start"]:x},1,{cpu}'
        lua.append('manager.machine.debugger:command(' + json.dumps(command) + ')')
    lua += ['manager.machine:exit()', "end,'frame')"]
    (out / 'capture.lua').write_text('\n'.join(lua) + '\n')
    env = dict(os.environ, LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    with (out / 'xvfb.log').open('w') as xlog:
        server = subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb', '-displayfd', '1', '-screen', '0', '1024x768x24', '-ac', '-nolisten', 'tcp'], stdout=subprocess.PIPE, stderr=xlog, text=True, env=env)
        try:
            number = server.stdout.readline().strip()
            assert number.isdigit()
            env['DISPLAY'] = ':' + number
            command = ['/workspace/emulator/root/usr/games/mame', 'mslugx', '-rompath', str(args.rom_dir.resolve()), '-bios', 'euro', '-video', 'soft', '-sound', 'none', '-window', '-skip_gameinfo', '-nothrottle', '-debug', '-debugger', 'none', '-debuglog', '-debugscript', str(ROOT / 'tools/mame/start.cmd'), '-autoboot_delay', '0', '-autoboot_script', str(out / 'capture.lua'), '-cfg_directory', str(out / 'cfg'), '-nvram_directory', str(out / 'nvram')]
            with (out / 'mame.log').open('w') as log:
                subprocess.run(command, cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=120)
        finally:
            server.terminate()
            server.wait(timeout=8)
    assert 'LUA ERROR' not in (out / 'mame.log').read_text()
    assert 'Error in command' not in (out / 'debug.log').read_text()
    decoded = {}
    for name, image in expected.items():
        assert (out / (name + '-readback.bin')).read_bytes() == image, ('CPU image mismatch', name)
        decoded[name] = {}
    for group in groups:
        name = group['context']
        instructions = parse_assembly(out / group['file'], contexts[name]['cpu'])
        assert all(group['start'] <= pc < group['end'] for pc in instructions), 'Decoder escaped requested span'
        assert not decoded[name].keys() & instructions.keys()
        decoded[name].update(instructions)
    comparison = []
    for row in rows:
        name, pc = row['location'].split(':')
        address = int(pc, 16)
        original = decoded[name].get(address)
        status = 'candidate_boundary_not_in_mame_stream'
        if original:
            offset = address - contexts[name]['cpu_base']
            assert original['bytes'] == expected[name][offset:offset + len(original['bytes'])]
            status = 'boundary_length_match' if len(original['bytes']) == int(row['length']) else 'length_mismatch'
            if re.search(r'\bdc\.[bwl]\b|illegal instruction', original['text'], re.I):
                status = 'mame_rejected_encoding'
        comparison.append(dict(**row, mame_length=len(original['bytes']) if original else 0,
                               mame_instruction=original['text'] if original else '-', status=status))
    with (out / 'comparison.tsv').open('w') as stream:
        writer = csv.DictWriter(stream, fieldnames=list(comparison[0]), delimiter='\t', lineterminator='\n')
        writer.writeheader()
        writer.writerows(comparison)
    counts = dict(collections.Counter(r['status'] for r in comparison))
    report = dict(status='STATIC_DECODER_COMPARISON_NOT_EXECUTION',
                  mame_version='0.276', layout_sha256=sha((base / 'layout.json').read_bytes()),
                  candidate_inputs_sha256=inputs, candidate_instructions=len(comparison),
                  statuses=counts, contexts=[dict(context=n, cpu=contexts[n]['cpu'],
                  readback_sha256=sha(b), size=len(b), ram_image_installed=n.startswith('RAM_COPY_'))
                  for n, b in expected.items()], groups=groups,
                  comparison_sha256=sha((out / 'comparison.tsv').read_bytes()),
                  assembly_sha256={g['file']: sha((out / g['file']).read_bytes()) for g in groups},
                  limits='Static boundaries, lengths and encoding acceptance only. Text differences are retained without equating semantics. RAM images are installed by the harness, not observed execution. No candidate is promoted to verified executable code or recovered C. A stream mismatch can hide later candidate boundaries.')
    (out / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(dict(candidate_instructions=len(comparison), statuses=counts)))


if __name__ == '__main__':
    main()
