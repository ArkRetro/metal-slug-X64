"""Synthetic platform imports for the manually recovered boot selector.

These model inputs to the CPU routine, not a multi-slot Neo Geo implementation.
"""
import ctypes as ct
import random
import struct
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


class Registers(ct.Structure):
    _fields_ = [('d', ct.c_uint32 * 8), ('a', ct.c_uint32 * 8), ('sr', ct.c_uint16)]


R8 = ct.CFUNCTYPE(ct.c_uint8, ct.c_void_p, ct.c_uint32)
R16 = ct.CFUNCTYPE(ct.c_uint16, ct.c_void_p, ct.c_uint32)
W8 = ct.CFUNCTYPE(None, ct.c_void_p, ct.c_uint32, ct.c_uint8)


class Imports(ct.Structure):
    _fields_ = [('context', ct.c_void_p), ('read8', R8), ('read16', R16), ('write8', W8)]


def header(magic=0, product=0x250, flag=1, repeat=None):
    return dict(magic=magic, product=product, product_later=product if repeat is None else repeat, flag=flag)


def make_cases():
    rng = random.Random(0x10fc00)
    cases = []

    def add(name, count, default=None, overrides=(), high_count=0, fallback=None):
        regs = [rng.getrandbits(32) for _ in range(16)]
        regs[2] = (regs[2] & 0xffffff00) | (rng.randrange(256) if fallback is None else fallback)
        regs[6] = (regs[6] & 0xffff0000) | (high_count << 8) | rng.randrange(256)
        regs[9] = 0x3a0000 | ([0, 0xa5000000, 0xff000000][len(cases) % 3])
        regs[15] = 0x10ffe0
        cases.append(dict(index=len(cases), name=name, count=count,
                          previous_slot=rng.randrange(256), registers=regs,
                          sr=0x2700 | (len(cases) & 31),
                          default=header() if default is None else default,
                          overrides=[dict(slot=slot, **h) for slot, h in overrides]))

    valid = 0x4e454f2d
    for count in (1, 2, 6, 0):
        for fallback in (0, 1, 0x7f, 0x80, 0xff):
            add(f'invalid_count{count}_fallback{fallback}', count, fallback=fallback)
    for slot in (0, 1, 5, 0x7f, 0xff):
        count = (slot + 1) & 255
        add(f'special_at_{slot}', count, overrides=[(slot, header(valid, 0x240))])
        add(f'ordinary_at_{slot}', count, overrides=[(slot, header(valid))])
    add('zero_count_special_first_initial_borrow', 0, overrides=[(0, header(valid, 0x240))])
    add('special_overrides_prior_ordinary', 6, overrides=[(0, header(valid)), (4, header(valid, 0x240))])
    add('ordinary_keeps_first', 6, default=header(valid))
    add('product_one_excluded', 6, default=header(valid, 1), fallback=0x80)
    for flag in (0, 2, 0x80, 0xff):
        add(f'flag_{flag}_excluded', 6, default=header(valid, flag=flag), fallback=0xff)
    for magic in (0x4e454f2c, 0xce454f2d, 0x4e444f2d, 0xffffffff):
        add(f'magic_{magic:08x}_excluded', 2, default=header(magic), fallback=0x7f)
    add('second_product_read_becomes_one', 2, default=header(valid, 0x250, repeat=1), fallback=0x80)
    add('second_product_read_becomes_ordinary', 2, default=header(valid, 1, repeat=0x250))
    add('special_does_not_read_flag', 1, default=header(valid, 0x240, flag=0))
    add('word_counter_preserves_high_byte', 1, high_count=1, fallback=0xff)
    add('word_counter_maximum_65536', 0, high_count=0xff, fallback=0x80)
    add('word_counter_wrap_then_special', 0, high_count=1, overrides=[(255, header(valid, 0x240))])
    return cases


def nv_input(case):
    return bytearray((i * 13 + case['index'] * 29 + (i >> 7)) & 255 for i in range(65536))


def run_c(lib, case):
    slots = [case['default']] * 256
    for h in case['overrides']:
        slots[h['slot']] = h
    selected = 0
    reads = [0] * 256
    events = bytearray()
    nv = nv_input(case)
    errors = []

    def emit(op, address, value):
        events.extend(struct.pack('>BIH', op, address, value))

    def rb(_, address):
        if address == 0xd00108:
            value = case['previous_slot']
        elif address == 0xd00047:
            value = case['count']
        elif address == 0x114:
            value = slots[selected]['flag']
        else:
            errors.append(('read8', address))
            value = 0
        emit(0, address, value)
        return value

    def rw(_, address):
        h = slots[selected]
        if address == 0x100:
            value = h['magic'] >> 16
        elif address == 0x102:
            value = h['magic'] & 65535
        elif address == 0x108:
            value = h['product_later'] if reads[selected] else h['product']
            reads[selected] += 1
        else:
            errors.append(('read16', address))
            value = 0
        emit(1, address, value)
        return value

    def wb(_, address, value):
        nonlocal selected
        emit(2, address, value)
        if address == 0x380021:
            selected = value
        elif address == 0xd00058:
            nv[0x58] = value
        elif address not in (0x3a0003, 0x3a001d, 0x3a000d, 0x3a0013):
            errors.append(('write8', address))

    callbacks = [R8(rb), R16(rw), W8(wb)]
    imports = Imports(None, *callbacks)
    regs = Registers((ct.c_uint32 * 8)(*case['registers'][:8]),
                     (ct.c_uint32 * 8)(*case['registers'][8:]), case['sr'])
    lib.msx_boot_select(ct.byref(regs), ct.byref(imports))
    assert not errors, errors
    return dict(registers=list(regs.d) + list(regs.a), sr=regs.sr), bytes(events), bytes(nv)


def fnv32(data):
    result = 2166136261
    for value in data:
        result = ((result ^ value) * 16777619) & 0xffffffff
    return result
