"""Controlled RAM descriptor states, not a proven campaign actor-table domain."""
import ctypes as ct
import random
import struct
from boot_select_harness import ROOT, Registers, R8, R16, W8, fnv32

W16 = ct.CFUNCTYPE(None, ct.c_void_p, ct.c_uint32, ct.c_uint16)


class Memory(ct.Structure):
    _fields_ = [('context', ct.c_void_p), ('read8', R8), ('read16', R16),
                ('write8', W8), ('write16', W16)]


class Result(ct.Structure):
    _fields_ = [('status', ct.c_int), ('boundary', ct.c_uint32), ('target', ct.c_uint32)]


def make_cases():
    rng = random.Random(0x10082e)
    cases = []

    def add(name, mode, **options):
        regs = [rng.getrandbits(32) for _ in range(16)]
        alias = 0xa5000000 if len(cases) % 3 == 1 else 0
        regs[8] = 0x10008c
        regs[12] = alias | (0x10fc00 + options.get('current', 0x40))
        regs[13] = alias | 0x108000
        regs[15] = (alias | options.get('sp', 0x10ffe0))
        regs[0] = (regs[0] & 0xffffff00) | options.get('priority', 64)
        if 'callback' in options:
            regs[8] = options['callback']
        c = dict(index=len(cases), name=name, mode=mode, registers=regs,
                 sr=0x2700 | (len(cases) & 31), chain_from=-1, current=options.get('current', 0x40),
                 priority=options.get('priority', 64), available=options.get('available', 6),
                 alloc_cursor=options.get('alloc_cursor', 2), return_cursor=options.get('return_cursor', 2),
                 new_slot=options.get('new_slot', 0x80), slots=[0,0x40,0x60,0x20],
                 callbacks=[], sp=options.get('sp',0x10ffe0))
        c.update(options)
        cases.append(c)
        return c

    for current in (0, 0x40, 0x60, 0x20):
        for priority in (0, 1, 31, 32, 64, 96, 254):
            add(f'allocate_from_{current}_priority_{priority}', 0, current=current, priority=priority)
    for cursor in (0, 1, 5, 6, 8, 255):
        for priority in (1, 64, 254):
            add(f'allocate_cursor_{cursor}_priority_{priority}', 0, current=0x60, alloc_cursor=cursor, priority=priority)
    for available in (1, 2, 0x7f, 0x80, 0xff):
        add(f'allocate_available_{available}', 0, available=available)
    for priority in (0, 64, 255):
        add(f'allocate_empty_priority_{priority}', 0, available=0, priority=priority)
    for node in (0, 0x20, 0x40, 0x60, 0xe0):
        add(f'allocate_alias_slot_{node}', 0, new_slot=node)
    for callback in (0, 0x00800000, 0xdeadbeef):
        add(f'allocate_counter_alias_callback_{callback:08x}', 0, new_slot=0xe8, callback=callback)
    add('allocate_stack_alias_new_slot', 0, sp=0x10fc90)
    for node in (0x40, 0x60):
        for available in (0, 1, 255):
            for cursor in (0, 6, 7, 0x7f, 255):
                add(f'remove_{node}_available_{available}_cursor_{cursor}', 1,
                    current=node, available=available, return_cursor=cursor)
    for node in (0, 0x20):
        add(f'remove_sentinel_{node}', 1, current=node)
    add('remove_stack_alias_node', 1, current=0x40, sp=0x10fc4c)
    for head in (0x100158, 0x10008c, 0x1000a4, 0, 0xa510008c):
        for tail in (0x100158, 0x10008c, 0x1000a4):
            add(f'dispatch_{head:08x}_{tail:08x}', 2, slots=[0,0x20], callbacks=[head,tail])
    add('dispatch_long_native_list', 2, slots=[0,0x40,0x60,0x80,0x20],
        callbacks=[0x100158,0x10008c,0x100158,0x10008c,0x100158])
    for sp in (0x10fc20, 0x10fc04, 0x10fc1e):
        add(f'dispatch_changed_return_{sp:06x}', 2, slots=[0,0x20],
            callbacks=[0x10008c,0x100158], sp=sp)
    first = add('pipeline_allocate', 0, current=0x40, priority=64, callback=0x10008c)
    second = add('pipeline_dispatch_without_reinjecting', 2, chain_from=first['index'])
    add('pipeline_next_dispatch_pending', 2, chain_from=second['index'])
    return cases


def base_ram(index):
    return bytearray((i*37+(index+1)*19+(i>>5)*7)&255 for i in range(65536))


def ram_input(case, code=None):
    ram = base_ram(case['index'])
    if code is not None:
        ram[:len(code)] = code
    def word(offset, value):
        ram[offset:offset+2] = value.to_bytes(2, 'big')
    def long(offset, value):
        ram[offset:offset+4] = value.to_bytes(4, 'big')
    slots = case['slots']
    priorities = {0:0,0x20:255,0x40:32,0x60:96,0x80:128}
    for i,slot in enumerate(slots):
        at = 0xfc00 + slot
        target = case['callbacks'][i] if case['callbacks'] else 0x100158
        long(at,target)
        ram[at+4] = slots[i-1] if i else slots[-1]
        ram[at+5] = slots[i+1] if i+1 < len(slots) else 0
        ram[at+6] = priorities[slot]
        word(at+0x1c,0x1234);word(at+0x1e,0xabcd)
    ram[0xfce0:0xfce7] = bytes((0x40,0x60,0x80,0xa0,0xc0,0,0x20))
    ram[0xfce7] = case['alloc_cursor'];ram[0xfce8] = case['return_cursor']
    ram[0xfce9] = case['available']
    # Cursor 8 intentionally aliases the return-index byte in that fixture.
    ram[0xfce0+case['alloc_cursor']] = case['new_slot']
    ram[0xff00:0xff02] = bytes.fromhex('60fe')
    return ram


def load_library(path):
    lib = ct.CDLL(str(path))
    for name in ('msx_descriptor_allocate', 'msx_descriptor_remove', 'msx_descriptor_dispatch'):
        getattr(lib,name).argtypes = [ct.POINTER(Registers),ct.POINTER(Memory)]
    lib.msx_descriptor_dispatch.restype = Result
    return lib


def run_c(lib, case, ram, registers=None, sr=None):
    ram = bytearray(ram)
    errors = []
    writes = bytearray()
    def offset(address, size):
        if not (0x10114a <= address and address+size <= 0x110000):
            errors.append(('non_data_RAM_access', address, size))
            return 0
        return address-0x100000
    def rb(_, address):
        return ram[offset(address,1)]
    def rw(_, address):
        if address&1:errors.append(('unaligned',address))
        at = offset(address,2)
        return int.from_bytes(ram[at:at+2],'big')
    def wb(_, address, value):
        writes.extend(struct.pack('>BIH',0,address,value))
        ram[offset(address,1)] = value
    def ww(_, address, value):
        if address&1:errors.append(('unaligned',address))
        writes.extend(struct.pack('>BIH',1,address,value))
        at = offset(address,2);ram[at:at+2] = value.to_bytes(2,'big')
    callbacks = [R8(rb),R16(rw),W8(wb),W16(ww)]
    memory = Memory(None,*callbacks)
    regs = case['registers'] if registers is None else registers
    r = Registers((ct.c_uint32*8)(*regs[:8]),(ct.c_uint32*8)(*regs[8:]),
                  case['sr'] if sr is None else sr)
    mode = case['mode']
    status,target,boundary = 0,0,0
    if mode==0:
        lib.msx_descriptor_allocate(ct.byref(r),ct.byref(memory))
    elif mode==1:
        lib.msx_descriptor_remove(ct.byref(r),ct.byref(memory))
    else:
        result=lib.msx_descriptor_dispatch(ct.byref(r),ct.byref(memory))
        status,target,boundary=result.status,result.target,result.boundary
    assert not errors,errors
    return dict(registers=list(r.d)+list(r.a),sr=r.sr,status=status,target=target,boundary=boundary),ram,bytes(writes)
