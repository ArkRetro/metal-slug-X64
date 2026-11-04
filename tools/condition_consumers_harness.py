"""Synthetic script consumers with real native services, original outputs separate."""
import ctypes as ct,hashlib,json
from native_conditions_harness import ROOT,Bus,Registers,R8,R16,W8,W16,HELPER,ram_input as world_ram
ENTRIES=[0x1a01e,0x1a2d8,0x1a4f2]
class Result(ct.Structure):_fields_=[('status',ct.c_int),('target',ct.c_uint32)]
def make_cases():
    reference=json.loads((ROOT/'data/native_conditions_fixture.json').read_text());picked={}
    for row in reference['results']:
        c=row['input'];e=row['expected'];picked.setdefault((c['opcode'],2 if e['status'] else e['sr']&1),c)
    cases=[]
    for entry in range(3):
        for source in picked.values():
            for timer in ((0,1,0x8000,0xffff) if entry==2 else (0,)):
                c=dict(source);c['world_index']=source['index'];c['index']=len(cases);c['consumer']=entry;c['entry_pc']=ENTRIES[entry]
                c['registers']=list(source['registers']);c['registers'][8]=0x10a000;c['registers'][14]=0x106000
                c['timer']=timer;c['reload']=[0,1,0x8000,0xffff][c['index']%4];c['length']=[0,14,128,255][c['index']%4]
                cases.append(c)
    return cases

def ram_input(case):
    ram=world_ram(case);base=0xa000;offset=10 if case['consumer']==2 else 2
    ram[base+1]=case['length'];ram[base+4:base+6]=case['reload'].to_bytes(2,'big')
    ram[base+offset:base+offset+4]=bytes([case['opcode'],case['relation']])+case['threshold'].to_bytes(2,'big')
    ram[0x6072:0x6074]=case['timer'].to_bytes(2,'big');ram[0x6074]=0xa5
    return ram

def run_c(lib,case):
    ram=ram_input(case);errors=[]
    def rb(_,a):
        if 0x100000<=a<0x110000:return ram[a-0x100000]
        errors.append(('read',hex(a)));return 0
    def rw(_,a):return rb(_,a)*256+rb(_,a+1)
    def wb(_,a,v):
        if 0x100000<=a<0x110000:ram[a-0x100000]=v
        else:errors.append(('write',hex(a)))
    def ww(_,a,v):wb(_,a,v>>8);wb(_,a+1,v&255)
    refs=[R8(rb),R16(rw),W8(wb),W16(ww),HELPER()];bus=Bus(None,*refs)
    r=Registers((ct.c_uint32*8)(*case['registers'][:8]),(ct.c_uint32*8)(*case['registers'][8:]),case['sr'])
    outcome=lib.msx_condition_consume(case['consumer'],ct.byref(r),ct.byref(bus));assert not errors,errors
    return dict(status=outcome.status,target=outcome.target,registers=list(r.d)+list(r.a),sr=r.sr,ram_sha256=hashlib.sha256(ram).hexdigest()),ram
