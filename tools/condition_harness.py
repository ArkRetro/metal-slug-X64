"""Synthetic inputs and ctypes runner for the addressed condition recovery.

Expected outputs live separately in the original-MAME fixture. This module
never calculates the expected game condition.
"""
import ctypes as ct
import hashlib
from pathlib import Path

ROOT=Path(__file__).resolve().parents[1]
class Registers(ct.Structure):
    _fields_=[('d',ct.c_uint32*8),('a',ct.c_uint32*8),('sr',ct.c_uint16)]
R8=ct.CFUNCTYPE(ct.c_uint8,ct.c_void_p,ct.c_uint32)
R16=ct.CFUNCTYPE(ct.c_uint16,ct.c_void_p,ct.c_uint32)
W8=ct.CFUNCTYPE(None,ct.c_void_p,ct.c_uint32,ct.c_uint8)
W16=ct.CFUNCTYPE(None,ct.c_void_p,ct.c_uint32,ct.c_uint16)
HELPER=ct.CFUNCTYPE(ct.c_bool,ct.c_void_p,ct.c_int,ct.POINTER(Registers))
class Bus(ct.Structure):
    _fields_=[('context',ct.c_void_p),('read8',R8),('read16',R16),
              ('write8',W8),('write16',W16),('helper',HELPER)]

def make_cases():
    cases=[]
    def append(op,relation=0,threshold=0,counters=None,variant=0,query=(0,0)):
        i=len(cases);seed=i+1
        regs=[((n+1)*0x11335577+seed*0x10203)&0xffffffff for n in range(16)]
        regs[9]=0x101000;regs[13]=0x108000;regs[15]=0x10f000
        edges=[0,1,0x7fff,0x8000,0xfffe,0xffff]
        h=[((n+1)*0x27182818+seed*0x10203)&0xffffffff for n in range(8)]
        for n in range(4):h[n]=(h[n]&0xffff0000)|edges[(variant//(n+1)+n)%6]
        h[7]=(h[7]&0xffff0000)|[0,1,0xffff,0x8000,0x7fff,2][variant%6]
        cases.append(dict(index=i,opcode=op,relation=relation,threshold=threshold,
            counters=counters or [((n+seed)*13)&255 for n in range(9)],
            registers=regs,sr=0x2700|((seed*7)&31),helper_d=h,
            helper_sr=0x2700|((seed*11)&31),helper_a0=0x10b000+i,
            helper_a2=0x10b800+i*2,query=list(query),
            x=edges[(variant//3)%6],y=edges[(variant//5+1)%6]))
    for op in range(9):
        for relation in range(4):
            for count,threshold in [(0,0),(0,1),(255,255),(255,254),(128,32768)]:
                append(op,relation,threshold,[count]*9)
    for counters in ([0]*9,[1]*9,[31]*9,[32]*9,[128]*9,[255]*9,list(range(9))):
        for relation in (0,1,2,3,255):append(9,relation,sum(counters[:8])&0xffff,counters)
    for variant in range(4):append(10,variant=variant)
    for op in range(11,20):
        for variant in range(24):append(op,variant=variant)
    for op in (20,21):
        for query in [(0,0),(0,1),(1,0),(0x80,0),(0x100,0xffffff80),(0xffffff00,0x100)]:
            append(op,query=query)
    return cases

def ram_input(case):
    seed=case['index']+1
    ram=bytearray((i*37+seed*19+(i>>5)*7)&255 for i in range(65536))
    ram[0x1000:0x1004]=bytes([case['opcode'],case['relation']])+case['threshold'].to_bytes(2,'big')
    ram[0xc326:0xc32f]=bytes(case['counters'])
    ram[0xc330:0xc332]=case['x'].to_bytes(2,'big')
    ram[0xc332:0xc334]=case['y'].to_bytes(2,'big')
    for i,value in enumerate(case['helper_d']):ram[0xe000+4*i:0xe004+4*i]=value.to_bytes(4,'big')
    ram[0xff00:0xff02]=bytes.fromhex('60fe')
    return ram

def run_c(lib,case,helper_available=True):
    ram=ram_input(case);errors=[];calls=[]
    def rb(_,address):
        if 0x100000<=address<0x110000:return ram[address-0x100000]
        errors.append(('read',hex(address)));return 0
    def rw(_,address):return (rb(_,address)<<8)|rb(_,address+1)
    def wb(_,address,value):
        if 0x100000<=address<0x110000:ram[address-0x100000]=value
        else:errors.append(('write',hex(address)))
    def ww(_,address,value):wb(_,address,value>>8);wb(_,address+1,value&255)
    def helper(_,kind,pointer):
        s=pointer.contents
        ret=(rw(None,s.a[7])<<16)|rw(None,s.a[7]+2)
        calls.append(dict(kind=kind,d0=s.d[0],a0=s.a[0],sp=s.a[7],ret=ret,sr=s.sr))
        old=s.d[0]
        for n,value in enumerate(case['helper_d']):s.d[n]=value
        if kind==1:s.d[0]=case['query'][int(old!=0)]
        s.a[0]=case['helper_a0'];s.a[2]=case['helper_a2'];s.sr=case['helper_sr']
        return helper_available
    refs=[R8(rb),R16(rw),W8(wb),W16(ww),HELPER(helper)]
    bus=Bus(None,*refs)
    s=Registers((ct.c_uint32*8)(*case['registers'][:8]),(ct.c_uint32*8)(*case['registers'][8:]),case['sr'])
    status=lib.msx_rom_01a5ca(ct.byref(s),ct.byref(bus))
    assert not errors,(case['index'],errors)
    return dict(status=status,registers=list(s.d)+list(s.a),sr=s.sr,
                ram_sha256=hashlib.sha256(ram).hexdigest(),calls=calls),ram
