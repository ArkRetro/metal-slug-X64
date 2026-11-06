#!/usr/bin/env python3
"""Import the numerical data read by the native soldier routines.

Source: program-fixed.bin / program-bank.bin of a MAME 0.264 capture made with
tools/mame/capture_soldier_campaign.lua. Only bounded data tables are kept,
each with its ROM address; no code bytes are exported."""
import argparse,hashlib,json,struct
from pathlib import Path
def words(data,offset,count,signed=False):return list(struct.unpack_from('>%d%s'%(count,'h' if signed else 'H'),data,offset))
def attack(fixed,address):
    x,y,w,h,kind=struct.unpack_from('>5h',fixed,address);klass,damage,end=struct.unpack_from('>HHh',fixed,address+10)
    assert end<0,'Only single-entry attack lists are imported'
    return dict(rom=hex(address),rect=[x,y,w,h],kind=kind,klass=klass,damage=damage)
def hurtbox(data,address,base):
    t,f=data[address-base],data[address-base+1];r=list(struct.unpack_from('>4h',data,address-base+2))
    assert struct.unpack_from('>H',data,address-base+10)[0]==0x8000
    return dict(rom=hex(address),type=t,flags=f,rect=r)
def spawn_offsets(script):
    """Spawn-table offsets of every op 7 record (walk by record length)."""
    out=set();pc=0
    while pc+1<len(script):
        op,ln=script[pc],script[pc+1]
        if op==7:out.add(struct.unpack_from('>H',script,pc+4)[0])
        if not ln:break
        pc+=ln
    return sorted(out)
def handoff_rng(capture,frame):
    ram=(capture/f'ram{frame}.bin').read_bytes()
    return dict(frame=frame,words=list(struct.unpack_from('>55H',ram,0x16)),cursor=(struct.unpack_from('>I',ram,0x84)[0]-0x100016)//2)
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    p.add_argument('--rng-frame',type=int,default=4060,help='RAM dump whose RNG ring seeds the native world (mission handoff)')
    a=p.parse_args();fixed=(a.capture/'program-fixed.bin').read_bytes();bank=(a.capture/'program-bank.bin').read_bytes()
    out=dict(source='MAME 0.264 mslugx euro capture; fixed CPU block and P2 bank 1 window',
        program_fixed_sha256=hashlib.sha256(fixed).hexdigest(),program_bank_sha256=hashlib.sha256(bank).hexdigest(),
        rank_lookup=dict(rom='0x025FA6',values=list(fixed[0x25fa6:0x25fa6+256])),
        group_map=dict(rom='0x0B5332',values=list(fixed[0xb5332:0xb533a])),
        timer70=dict(rom='0x2D3C16',header=list(bank[0xd3c16:0xd3c18]),values=words(bank,0xd3c18,96)),
        timer72=dict(rom='0x2D3B54',header=list(bank[0xd3b54:0xd3b56]),values=words(bank,0xd3b56,96)),
        attack_sense=attack(fixed,0xae970),attack_knife=attack(fixed,0xae980),
        attack_leap=attack(fixed,0xae990),attack_leap_hit=attack(fixed,0xae9a0),attack_wait=attack(fixed,0xae9b0),
        pistol=[attack(fixed,0x1b4c2+16*i) for i in range(16)],
        cull_rect=dict(rom='0x0B04F4',values=words(fixed,0xb04f4,4,True)),
        retreat_rect=dict(rom='0x0B04FC',values=words(fixed,0xb04fc,4,True)),
        # 0B0396: idle animations (random pick) and the alert rectangle (01AFFC).
        idle_programs=dict(rom='0x0B037E',values=[struct.unpack_from('>I',fixed,0xb037e+4*i)[0] for i in range(4)]),
        alert_rect=dict(rom='0x0B038E',values=words(fixed,0xb038e,4,True)),
        # 04115A soldier variant: removal rectangle of its tail (01945C).
        variant_cull_rect=dict(rom='0x040FF2',values=words(fixed,0x40ff2,4,True)),
        hurtboxes=[hurtbox(fixed,x,0) for x in (0x3bb40,0x3bb4c,0x3bb58,0x3bb64,0x3bb70,0x3bb7c)]+[hurtbox(bank,0x24057c,0x200000)],
        player_hurtbox=hurtbox(bank,0x2d0aa8,0x200000),
        hit_matrix=dict(rom='0x004582',rows=[list(fixed[0x4582+20*t:0x4582+20*t+20]) for t in range(8)]),
        # Mission 1 level script (bank 1, ROM 2D6314..2D7132 and its op 3
        # end record); region_end 2D6A28 is the record after the camera-1920
        # lock (2D6A1A), the end of the village region.
        # Record bytes only; the interpreter is native code (src/neo_script.c).
        mission_script=dict(rom='0x2D6314',end='0x2D7134',region_end='0x2D6A28',bytes=list(bank[0xd6314:0xd7134])),
        spawn_table=dict(rom='0x0197DC',entries={hex(o):'0x%08X'%struct.unpack_from('>I',fixed,0x197dc+o)[0]
            for o in spawn_offsets(bank[0xd6314:0xd7134])+[o for o in spawn_offsets(bank[0xd719a:0xd7214]) if o not in spawn_offsets(bank[0xd6314:0xd7134])]}),
        # Sub-script records started by generic actors (ROM 00087A table,
        # +74 index): 2D719A of the swordsman 0A8C62 above the wagon.
        sub_scripts=[dict(rom='0x2D719A',end='0x2D7214')],
        # RNG ring (RAM 100016..100083, cursor 100084) dumped at the mission
        # handoff of the capture; the native world starts from it because the
        # attract/intro consumers that produce it are not part of the port.
        handoff_rng=handoff_rng(a.capture,a.rng_frame),
        # Flight attack row of the thrown object (03BD70 -> 0049E8).
        throw_rectangles=dict(rom=['0x03BC18','0x01DEC0'],flight=words(fixed,0x3bc18,4,True),burst=words(fixed,0x1dec0,4,True)),
        throw_attack=dict(rom='0x0E0894',values=list(struct.unpack_from('>5h2H',fixed,0xe0894))),
        angle_tables=dict(rom='0x002C7C',log=list(fixed[0x2c7c:0x2c7c+256]),atan=list(fixed[0x2d7c:0x2d7c+256])))
    assert out['timer70']['header']==[1,255] and out['timer72']['header']==[1,255]
    a.output.write_text(json.dumps(out,indent=1)+'\n');print('tables written')
if __name__=='__main__':main()
