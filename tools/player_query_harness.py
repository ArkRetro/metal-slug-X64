"""Synthetic inputs and ctypes adapter; expected outputs come from original MAME."""
import ctypes as ct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
class Slot(ct.Structure):
    _fields_=[('object_header',ct.c_uint32),('active',ct.c_uint16),('field18',ct.c_uint16),('field14',ct.c_uint16),('state21',ct.c_uint8),('field1f',ct.c_uint8),('field20',ct.c_uint8)]
class Registers(ct.Structure):
    _fields_=[('d',ct.c_uint32*5),('sr',ct.c_uint16),('selected_slot',ct.c_uint8)]
def make_cases():
    cases=[]
    for selector in (0,1,0x100,0xffffff80):
        for header in (0x3f1e,0,0xffffffff):
            for active in (0,1):
                for state in (0,1,0x80,0xff):
                    i=len(cases);regs=[((n+1)*0x11335577+(i+1)*0x10203)&0xffffffff for n in range(16)]
                    regs[0]=selector;regs[13]=0x108000;regs[15]=0x10f000
                    selected=int(selector&255!=0);slots=[]
                    for slot in range(2):
                        slots.append(dict(object_header=header if slot==selected else 0x3f1e,active=active,
                            state21=state,field1f=(i*19+slot*7)&255,field20=(i*37+slot*13)&255,
                            field18=[0,1,0x7fff,0x8000,0xffff][i%5],field14=[0,0xffff,0x8000,0x7fff,1][i%5]))
                    cases.append(dict(index=i,registers=regs,sr=0x2700|((i*7)&31),slots=slots))
    return cases

def ram_input(case):
    ram=bytearray((i*37+(case['index']+1)*19+(i>>5)*7)&255 for i in range(65536))
    for slot,p in enumerate(case['slots']):
        ram[(0x14c4 if slot==0 else 0x1574):(0x14c4 if slot==0 else 0x1574)+4]=p['object_header'].to_bytes(4,'big')
        base=0xc608+slot*0x24
        ram[base:base+2]=p['active'].to_bytes(2,'big')
        for name,offset in [('state21',0x21),('field1f',0x1f),('field20',0x20)]:ram[base+offset]=p[name]
        for name,offset in [('field18',0x18),('field14',0x14)]:ram[base+offset:base+offset+2]=p[name].to_bytes(2,'big')
    ram[0xff00:0xff02]=bytes.fromhex('60fe');return ram

def run_c(lib,case):
    slots=(Slot*2)(*(Slot(**p) for p in case['slots']))
    r=Registers((ct.c_uint32*5)(*case['registers'][:5]),case['sr'],255)
    status=lib.neo_player_query(slots,ct.byref(r))
    regs=list(case['registers']);regs[:5]=list(r.d)
    regs[8]=0x1014c4+r.selected_slot*0xb0
    regs[12]=0x10c608+r.selected_slot*0x24
    return dict(status=status,registers=regs,sr=r.sr,selected_slot=r.selected_slot)
