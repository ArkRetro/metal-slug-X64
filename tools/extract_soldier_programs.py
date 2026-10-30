#!/usr/bin/env python3
"""Convert the soldier, death and grenade animation programs to native commands.

Reads the CPU bank window saved by tools/mame/capture_soldier_campaign.lua
(program-bank.bin, the P2 bank mapped during Mission 1) and the fixed block.
Every reachable command is converted to a typed native command; an opcode or
callback outside the recovered set becomes UNRECOVERED (it fails explicitly in
C when reached) instead of being guessed."""
import argparse,hashlib,json,struct
from pathlib import Path
ROOTS=[0x23d510,0x23d594,0x23d5e2,0x23d692,0x23d6ec,0x23d712,0x23d7ee,0x23d8c0,0x23d9ea,0x23da62,0x23db34,0x23db7e,
    0x23ddb4,0x23ddd6,0x23de6a,0x23df8e,0x23dfd2,0x23dfe4,0x23e01e,0x23e0da,0x23e1da,0x23e2ea,0x23e394,0x23e43c,0x23e47c,
    0x23e4ee,0x23e5b8,0x23e6ae,0x23e71a,0x23e7ac,0x23e82e,0x23edf4,0x23ee3c,0x23eeb6,0x23eeea,0x23ef98,0x23f0fc,0x23f15a,
    0x23f1bc,0x23f200,0x23f270,0x23dbe2,0x23dc26,0x23dc6a,0x23dcfe,
    0x2402fa,0x24039c,0x2403ee,0x240454,0x2404fe,0x240588,0x2406ec,0x240a3e,0x240ae6,0x240c6e,0x240c76,0x240d2e,0x240d3c,
    0x240dc0,0x240dee,0x240e8e,0x240f8e,0x24103c,0x241056,0x2410e2,
    0x23f3f0,0x22d2a6]
# Region entities (camera 0..1920) translated as generic actors. Their 0x14
# callbacks become CALL commands handled by the family code (an unknown one
# fails explicitly there).
ROOTS+=[0x236818,0x236846,0x2367e4,0x2367fe,0x236874,0x2368a2,0x2368d0,
    0x22cdd4,0x22ce6c,0x22cf2c,0x22d264,0x22d27a,0x22d290,0x22e48a,0x22eac2,0x22e7f2,0x22e8ae,
    0x2af47a,0x2afb56,0x2af960,0x2afc60,0x2afcca,0x2919fe,
    0x2411f2,0x24116e,0x24123e,0x2412ee,0x24124c,0x241354]
# Rest of the mission: the robed swordsman 0A8C46 (boss arena, toggles RAM
# 10C606 that starts the boss).
ROOTS+=[0x2a7372,0x2a73b4,0x2a7434,0x2a74e0,0x2a759c]
# Domed building 054904 family (camera 1920): domes 054C2C, banners 054B1C,
# hatches 054F60, launchers 055056, 05518A and missiles 055638.
ROOTS+=[0x24a334,0x24a33e,0x24a348,0x24a352,0x24a35c,0x24a366,0x24a370,0x24a450,0x24a530,
    0x24ac42,0x24abb0,0x24a610,0x24a6f2,0x24a742,0x24a7ce,0x24a910,0x24aa18,0x2a89b8,0x2a89c4,0x2a89d0,0x2a89dc,0x2a89e8,0x2a89f4,0x2a8a00]
# Covered wagon 033C98 family: drive, stop, open (callback 239ECE), wreck,
# canvas 034022 and pieces 034094.
ROOTS+=[0x239dc8,0x239df6,0x239e24,0x239e30,0x239ee4,0x239f26,0x239f32]
# Arabian swordsmen 039590/0395B6 (stand, walk/attack tables 039FB0/039FC8,
# turn, slash 039B60, flight, sword 039C62) and the shared death pieces.
ROOTS+=[0x23c8b0,0x23c8f8,0x23c97c,0x23c782,0x23c790,0x23c79e,0x23c7ac,0x23c7ba,0x23c7c8,0x23c7dc,0x23c7ea,
    0x23c7f8,0x23c806,0x23c814,0x23c822,0x23c836,0x23c9ba,0x23ca32,0x2a769c,0x269340,0x2a765a,0x2a78f0,0x2a796e,
    0x2a77f4,0x2a7872,0x2a79ec,0x2a7a6a,0x2a7ae8,0x2a7b2a,0x2a76f8,0x2a7776,
    0x23b3c6,0x23b5b8,0x23b4e4,0x23b41c]   # creature 035668
# Prisoner 099862: tied (wriggle table 099D86 by +80), salute, item gift
# (callback 099CCC), release jump and run.
ROOTS+=[0x29f6d0,0x29f9ac,0x29fa22,0x29fa40,0x29fa78,0x29f85e,0x29f874,0x29f88a,0x29f8a0,0x29f8b6,0x29f8cc,0x29f8e2,
    0x29f8f8,0x29f90e,0x29f924,0x29f93a,0x29f950,0x29f966,0x29f97c,0x29f992,0x29f99c]
# Items 08C374/08C37C of the kinds with the motion 08B9B2 (records 0E581A:
# 06, 18, 1C, 29, 2A, 2B, 2F, 4B, 4F (H), 51) and the appearance 2861BC.
ROOTS+=[0x28652c,0x28745c,0x287516,0x287814,0x2878ec,0x2879c4,0x287b40,0x2864b0,0x28635a,0x28639c,0x2861bc]
# Armoured vehicle 087E36: wait, stop, drive, destroyed, aim, fire, flinch,
# reverse (callbacks 089412 platform offset, 08938A pieces).
ROOTS+=[0x2841b4,0x2841de,0x28425e,0x2842de,0x284416,0x2847de,0x284828,0x28454e,0x2845f2,0x2846ba,0x2848aa]
# Callbacks with a native meaning (ROM routine -> native kind, asset).
PALETTE={0x4061c:0x385,0x40632:0x214,0x40648:0x35c,0x4065e:0x20e,0x40674:0x221,0x4068a:0x21a,0x406a0:0x20e}
GENERIC={0x40948,0x40956,0x4095e,0x23e418,0x23e420,0x23e42e,0xaef14}
def convert(bank,root):
    commands=[];where={}
    def rd(pc,n):
        assert 0x200000<=pc<0x300000;return bank[pc-0x200000:pc-0x200000+n]
    def add(pc,**k):
        c=dict(address=pc,kind='',asset=0,target=0,operand=0,flags=0,dx=0,dy=0);c.update(k)
        where.setdefault(pc,len(commands));commands.append(c);return c
    pending=[root];fixups=[]
    while pending:
        pc=pending.pop(0);fresh=pc not in where
        while pc not in where:
            op,arg=rd(pc,2)
            if op==0:add(pc,kind='WAIT',operand=arg);pc+=2
            elif op==4:
                dx,dy=struct.unpack('bb',rd(pc+2,2));add(pc,kind='POSE',flags=arg,dx=dx,dy=dy,asset=struct.unpack('>I',rd(pc+4,4))[0]);pc+=8
            elif op==8:
                t=struct.unpack('>I',rd(pc+2,4))[0];fixups.append((add(pc,kind='JUMP'),t));pending.append(t);break
            elif op==0x0c:
                f=struct.unpack('>H',rd(pc+2,2))[0];t=struct.unpack('>I',rd(pc+4,4))[0]
                fixups.append((add(pc,kind='COND_JUMP',asset=f,operand=1 if arg else 0),t));pending.append(t);pc+=8
            elif op==0x10:
                length=struct.unpack('>H',rd(pc+2,2))[0];assert length%8==0
                # 004412: pose selected by the actor byte `arg` (+61 for the soldiers).
                c=add(pc,kind='SELECT_POSE' if arg==0x61 else 'SELECT_FIELD_POSE',asset=arg,operand=length//8);c['target']=len(commands)
                for i in range(length//8):
                    at=pc+4+i*8;assert rd(at,1)[0]==4
                    dx,dy=struct.unpack('bb',rd(at+2,2));add(at,kind='POSE',flags=rd(at+1,1)[0],dx=dx,dy=dy,asset=struct.unpack('>I',rd(at+4,4))[0])
                pc+=4+length
            elif op==0x14:
                routine=struct.unpack('>I',rd(pc+2,4))[0];param=struct.unpack('>H',rd(pc+6,2))[0] if arg>=6 else 0
                if routine==0x1adaa:add(pc,kind='FLIP_FACING')
                elif routine in PALETTE:add(pc,kind='REPLACE_PALETTE',asset=PALETTE[routine],target=routine&0xffff)
                elif routine==0x3bb8e:add(pc,kind='SPAWN_THROW',asset=0x3bc28)
                # 03BBBC/03BBEA: same object with +6E=1 / 2 and offsets (0,24) / (0,32).
                elif routine in (0x3bbbc,0x3bbea):add(pc,kind='SPAWN_THROW',asset=routine)
                elif routine in GENERIC:add(pc,kind='CALL',asset=routine,target=param)
                else:add(pc,kind='CALL',asset=routine,target=param)
                pc+=2+arg
            elif op==0x18:add(pc,kind='SET_BYTE',target=struct.unpack('>H',rd(pc+2,2))[0],asset=arg);pc+=4
            elif op==0x1c:add(pc,kind='SET_WORD',target=arg,asset=struct.unpack('>H',rd(pc+2,2))[0]);pc+=4
            elif op==0x20:
                v=struct.unpack('>I',rd(pc+2,4))[0]
                add(pc,kind='FIELD50',asset=v) if arg==0x50 else add(pc,kind='SET_LONG',target=arg,asset=v);pc+=6
            elif op==0x24:add(pc,kind='DEC_BYTE',target=arg);pc+=2
            elif op==0x28:add(pc,kind='FINISH');break
            elif op==0x2c:
                length=struct.unpack('>H',rd(pc+2,2))[0];assert length%2==0
                add(pc,kind='SWITCH',asset=arg,operand=length//2);table=pc+4
                for i in range(length//2):
                    t=table+struct.unpack('>h',rd(table+2*i,2))[0]+length
                    fixups.append((add(table+2*i,kind='JUMP'),t));pending.append(t)
                pc=table+length
            elif op==0x30:add(pc,kind='SOUND',target=struct.unpack('>H',rd(pc+2,2))[0]);pc+=4
            else:add(pc,kind='UNRECOVERED',asset=op);break
        else:
            # Falling into an already converted address: make the flow explicit.
            if fresh and commands[-1]['kind'] not in ('JUMP','FINISH','UNRECOVERED'):
                fixups.append((add(-pc,kind='JUMP'),pc))
    for c,t in fixups:c['target']=where[t]
    return commands
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    bank=(a.capture/'program-bank.bin').read_bytes()
    programs=[dict(root=r,commands=convert(bank,r)) for r in ROOTS]
    kinds={}
    for prog in programs:
        for c in prog['commands']:kinds[c['kind']]=kinds.get(c['kind'],0)+1
    out=dict(source='MAME 0.264 mslugx euro, CPU window 0x200000..0x2FFFFF saved during Mission 1 (P2 bank 1)',
        program_bank_sha256=hashlib.sha256(bank).hexdigest(),command_kinds=kinds,programs=programs,
        limits=['Callbacks without a dedicated kind are CALL commands; the owning actor handles or rejects them explicitly.',
                'Fall-through into an already converted command is expressed as an explicit JUMP (negative address marker).'])
    a.output.write_text(json.dumps(out,separators=(',',':'))+'\n')
    print(kinds)
if __name__=='__main__':main()
