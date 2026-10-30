#!/usr/bin/env python3
"""Offline regression against hashes/registers captured from original M68000.

Inputs are synthetic and deterministic. Expected outputs come from MAME,
not from the recovered C. No game ROM, emulator or N64 SDK is required here.
"""
import argparse
import ctypes as ct
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--sanitize', action='store_true')
a = p.parse_args()
fixture = json.loads((ROOT/'decomp/recovered/rom_008512.validation.json').read_text())
assert fixture['status'] == 'PASS_CONTROLLED_ORIGINAL_M68000_COMPARISON'

class Registers(ct.Structure):
    _fields_ = [('d', ct.c_uint32*8), ('a', ct.c_uint32*8), ('sr', ct.c_uint16)]

R8 = ct.CFUNCTYPE(ct.c_uint8, ct.c_void_p, ct.c_uint32)
R16 = ct.CFUNCTYPE(ct.c_uint16, ct.c_void_p, ct.c_uint32)
W8 = ct.CFUNCTYPE(None, ct.c_void_p, ct.c_uint32, ct.c_uint8)
W16 = ct.CFUNCTYPE(None, ct.c_void_p, ct.c_uint32, ct.c_uint16)

class Bus(ct.Structure):
    _fields_ = [('context', ct.c_void_p), ('read8', R8), ('read16', R16),
               ('write8', W8), ('write16', W16)]

def check(lib, case):
    seed = case['index']+1
    ram = bytearray((i*37+seed*19+(i>>5)*7)&255 for i in range(65536))
    mode = case['mode']
    for slot in range(256):
        addr = 0x78fa+slot*32
        dirty = (mode == 'all' or (mode == 'first' and slot == 0)
                 or (mode == 'last' and slot == 255)
                 or (mode == 'sparse' and slot%7 == 3))
        ram[addr] = (ram[addr]&127) | (128 if dirty else 0)
    ram[0x9942:0x9944] = bytes.fromhex('2000' if mode == 'fill' else '0000')
    ram[0x9944] = case['final_bank']
    ram[0x9945] = (seed&127) | (0 if mode.startswith('flag_off') else 128)
    ram[0xff00:0xff02] = bytes.fromhex('60fe')
    banks = [bytearray().join(((i*257+seed*31+b*13)&65535).to_bytes(2, 'big')
             for i in range(4096)) for b in (0, 1)]
    bank = 1
    events, errors = [], []

    def read8(_, addr):
        if 0x100000 <= addr < 0x110000:
            return ram[addr-0x100000]
        if 0x400000 <= addr < 0x402000:
            return banks[bank][addr-0x400000]
        errors.append(('read', hex(addr)))
        return 0

    def read16(_, addr):
        return (read8(_, addr)<<8) | read8(_, addr+1)

    def write8(_, addr, value):
        nonlocal bank
        if 0x100000 <= addr < 0x110000:
            ram[addr-0x100000] = value
        elif addr in (0x300001, 0x3a000f, 0x3a001f):
            events.append(f'{addr&~1:06x} {value:04x} 00ff')
            if addr != 0x300001:
                bank = int(addr == 0x3a001f)
        else:
            errors.append(('write8', hex(addr)))

    def write16(_, addr, value):
        if 0x100000 <= addr < 0x110000:
            offset = addr-0x100000
            ram[offset:offset+2] = value.to_bytes(2, 'big')
        elif 0x400000 <= addr < 0x402000:
            offset = addr-0x400000
            banks[bank][offset:offset+2] = value.to_bytes(2, 'big')
        else:
            errors.append(('write16', hex(addr)))

    callbacks = [R8(read8), R16(read16), W8(write8), W16(write16)]
    bus = Bus(None, *callbacks)
    regs = Registers((ct.c_uint32*8)(*case['registers'][:8]),
                     (ct.c_uint32*8)(*case['registers'][8:]), case['sr'])
    lib.msx_rom_008512(ct.byref(regs), ct.byref(bus))
    assert not errors, errors
    assert list(regs.d)+list(regs.a) == case['output_registers'], (seed, 'registers')
    assert regs.sr == case['output_sr'], (seed, 'SR')
    assert hashlib.sha256(ram).hexdigest() == case['ram_sha256'], (seed, 'RAM')
    assert [hashlib.sha256(b).hexdigest() for b in banks] == case['palette_sha256'], (seed, 'palettes')
    assert bank == case['final_bank'], (seed, 'bank')
    assert events == case['io'], (seed, 'I/O writes')

with tempfile.TemporaryDirectory(prefix='msx-palette-') as directory:
    library = Path(directory)/'kernel.so'
    cmd = ['cc', '-std=c11', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC']
    if a.sanitize:
        cmd += ['-fsanitize=undefined', '-fno-sanitize-recover=all']
    cmd += [str(ROOT/'decomp/recovered/rom_008512.c'), '-o', str(library)]
    subprocess.run(cmd, check=True)
    lib = ct.CDLL(str(library))
    lib.msx_rom_008512.argtypes = [ct.POINTER(Registers), ct.POINTER(Bus)]
    for case in fixture['results']:
        check(lib, case)
print(f"PASS: {len(fixture['results'])} original-M68000 palette fixtures")
