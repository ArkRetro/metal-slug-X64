"""Original-reference synthetic inputs for native player position recovery."""
import ctypes as ct
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
class Player(ct.Structure):
    _fields_=[('object_header',ct.c_uint32),('x',ct.c_uint16),('y',ct.c_uint16),('use_x',ct.c_uint8),('use_y',ct.c_uint8)]
class Frame(ct.Structure):
    _fields_=[('players',Player*2),('fallback_x',ct.c_uint16),('fallback_y',ct.c_uint16),('enabled',ct.c_uint8)]
class Registers(ct.Structure):
    _fields_=[('d',ct.c_uint32*8),('sr',ct.c_uint16)]
def make_cases():
    cases=[]
    for enabled in (0,1,128):
        for mask in range(4):
            for axes in range(4):
                for variant in range(2):
                    i=len(cases);regs=[((n+1)*0x11335577+(i+1)*0x10203)&0xffffffff for n in range(16)]
                    regs[13]=0x108000;regs[15]=0x10f000
                    players=[]
                    for p in range(2):players.append(dict(object_header=(0 if p==0 else 0xffffffff) if mask&(1<<p) else 0x3f1e,x=[0,0xffff,0x8000,0x7fff][(i+p)%4],y=[0x8000,0,0x7fff,0xffff][(i+p+variant)%4],use_x=(axes&1)*255,use_y=((axes>>1)&1)*128))
                    cases.append(dict(index=i,registers=regs,sr=0x2700|((i*7)&31),frame=dict(players=players,fallback_x=[0,0xffff][variant],fallback_y=[0x8000,0][variant],enabled=enabled)))
    return cases

def ram_input(case):
    ram=bytearray((i*37+(case['index']+1)*19+(i>>5)*7)&255 for i in range(65536))
    f=case['frame'];ram[0xc322]=f['enabled'];ram[0xe9b8:0xe9ba]=f['fallback_x'].to_bytes(2,'big');ram[0xe9ba:0xe9bc]=f['fallback_y'].to_bytes(2,'big')
    for p,actor in enumerate(f['players']):
        base=0x14c4+p*0xb0
        ram[base:base+4]=actor['object_header'].to_bytes(4,'big')
        ram[base+0x3c:base+0x3e]=actor['x'].to_bytes(2,'big');ram[base+0x40:base+0x42]=actor['y'].to_bytes(2,'big')
        ram[base+0x7a]=actor['use_x'];ram[base+0x7b]=actor['use_y']
    ram[0xff00:0xff02]=bytes.fromhex('60fe');return ram

def run_c(lib,case):
    f=case['frame'];frame=Frame((Player*2)(*(Player(**p) for p in f['players'])),f['fallback_x'],f['fallback_y'],f['enabled'])
    r=Registers((ct.c_uint32*8)(*case['registers'][:8]),case['sr']);lib.neo_player_positions(ct.byref(frame),ct.byref(r))
    regs=list(case['registers']);regs[:8]=list(r.d)
    if f['enabled']:regs[8]=0x101574
    return dict(status=0,registers=regs,sr=r.sr)
