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
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();fixed=(a.capture/'program-fixed.bin').read_bytes();bank=(a.capture/'program-bank.bin').read_bytes()
    out=dict(source='MAME 0.264 mslugx euro capture; fixed CPU block and P2 bank 1 window',
        program_fixed_sha256=hashlib.sha256(fixed).hexdigest(),program_bank_sha256=hashlib.sha256(bank).hexdigest(),
        rank_lookup=dict(rom='0x025FA6',values=list(fixed[0x25fa6:0x25fa6+256])),
        group_map=dict(rom='0x0B5332',values=list(fixed[0xb5332:0xb533a])),
        timer70=dict(rom='0x2D3C16',header=list(bank[0xd3c16:0xd3c18]),values=words(bank,0xd3c18,96)),
        timer72=dict(rom='0x2D3B54',header=list(bank[0xd3b54:0xd3b56]),values=words(bank,0xd3b56,96)),
        attack_sense=attack(fixed,0xae970),attack_knife=attack(fixed,0xae980),
        attack_leap=attack(fixed,0xae990),attack_leap_hit=attack(fixed,0xae9a0),
        pistol=[attack(fixed,0x1b4c2+16*i) for i in range(16)],
        cull_rect=dict(rom='0x0B04F4',values=words(fixed,0xb04f4,4,True)),
        retreat_rect=dict(rom='0x0B04FC',values=words(fixed,0xb04fc,4,True)),
        hurtboxes=[hurtbox(fixed,x,0) for x in (0x3bb40,0x3bb4c,0x3bb58,0x3bb64,0x3bb70,0x3bb7c)]+[hurtbox(bank,0x24057c,0x200000)],
        player_hurtbox=hurtbox(bank,0x2d0aa8,0x200000),
        hit_matrix=dict(rom='0x004582',rows=[list(fixed[0x4582+20*t:0x4582+20*t+20]) for t in range(8)]),
        # Mission 1 level script (bank 1, ROM 2D6314) up to the statement that
        # follows the region's two "all groups cleared" waits (2D651A, whose
        # player-region condition belongs to the next segment). Record bytes
        # only; the interpreter is native code (src/neo_script.c).
        mission_script=dict(rom='0x2D6314',end='0x2D651A',bytes=list(bank[0xd6314:0xd651a])),
        spawn_table=dict(rom='0x0197DC',entries={hex(o):'0x%08X'%struct.unpack_from('>I',fixed,0x197dc+o)[0]
            for o in (0x1f8,0x204,0x21c,0x2ac,0x2c8,0x2d0,0x31c,0x354,0x358)}),
        angle_tables=dict(rom='0x002C7C',log=list(fixed[0x2c7c:0x2c7c+256]),atan=list(fixed[0x2d7c:0x2d7c+256])))
    assert out['timer70']['header']==[1,255] and out['timer72']['header']==[1,255]
    a.output.write_text(json.dumps(out,indent=1)+'\n');print('tables written')
if __name__=='__main__':main()
