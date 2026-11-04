"""Real-service condition input fixtures and C ABI adapter. No mocked helpers."""
import ctypes as ct,hashlib
from condition_harness import ROOT,Registers,Bus,R8,R16,W8,W16,HELPER,make_cases as base_cases,ram_input as base_ram

def make_cases():
    cases=base_cases()
    for opcode in (20,21):
        for player in (0,1):
            c=dict(cases[-1]);c['index']=len(cases);c['opcode']=opcode;c['force_trap_player']=player;cases.append(c)
    return cases

def ram_input(case):
    ram=base_ram(case);i=case.get('world_index',case['index']);mask=(i//3)%4;ram[0xc322]=[0,1,128][i%3]
    ram[0xe9b8:0xe9ba]=case['x'].to_bytes(2,'big');ram[0xe9ba:0xe9bc]=case['y'].to_bytes(2,'big')
    for p in range(2):
        obj=0x14c4+p*0xb0;table=0xc608+p*0x24
        header=0 if mask&(1<<p) else 0x3f1e
        ram[obj:obj+4]=header.to_bytes(4,'big')
        ram[obj+0x7a]=(i>>p)&1;ram[obj+0x7b]=(i>>(p+1))&1
        ram[obj+0x3c:obj+0x3e]=(case['helper_d'][p*2]&65535).to_bytes(2,'big')
        ram[obj+0x40:obj+0x42]=(case['helper_d'][p*2+1]&65535).to_bytes(2,'big')
        state=case['query'][p]&255
        ram[table:table+2]=int(state!=0).to_bytes(2,'big');ram[table+0x21]=state
        ram[table+0x1f]=(i*19+p)&255;ram[table+0x20]=(i*37+p)&255
        ram[table+0x18:table+0x1a]=case['x'].to_bytes(2,'big');ram[table+0x14:table+0x16]=case['y'].to_bytes(2,'big')
    if 'force_trap_player' in case:
        player=case['force_trap_player']
        for p in range(2):
            obj=0x14c4+p*0xb0;table=0xc608+p*0x24
            ram[obj:obj+4]=(0).to_bytes(4,'big');ram[table:table+2]=int(p==player).to_bytes(2,'big');ram[table+0x21]=0
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
    callbacks=[R8(rb),R16(rw),W8(wb),W16(ww),HELPER()]
    bus=Bus(None,*callbacks);s=Registers((ct.c_uint32*8)(*case['registers'][:8]),(ct.c_uint32*8)(*case['registers'][8:]),case['sr'])
    status=lib.msx_condition_evaluate_native(ct.byref(s),ct.byref(bus));assert not errors,errors
    return dict(status=status,registers=list(s.d)+list(s.a),sr=s.sr,ram_sha256=hashlib.sha256(ram).hexdigest(),calls=[]),ram
