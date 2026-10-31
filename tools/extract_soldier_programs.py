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
ROOTS+=[0x28652c,0x28745c,0x287516,0x287814,0x2878ec,0x2879c4,0x287b40,0x2864b0,0x28635a,0x28639c,0x2861bc,0x28617e]
# Player bullets 01B6DC..01BD00: flight (H 22C1B6/22C3BE, pistol 22C12E),
# impact 22E504, wall impacts 22C884/22C81E.
ROOTS+=[0x22c1b6,0x22c3be,0x22c12e,0x22e504,0x22c884,0x22c81e]
# Grenade 01BF10/01BF46 (22C950) and its explosion 01C152 (22CB60).
ROOTS+=[0x22c950,0x22cb60]
# Armoured vehicle 087E36: wait, stop, drive, destroyed, aim, fire, flinch,
# reverse (callbacks 089412 platform offset, 08938A pieces).
ROOTS+=[0x2841b4,0x2841de,0x28425e,0x2842de,0x284416,0x2847de,0x284828,0x28454e,0x2845f2,0x2846ba,0x2848aa]
# Its children: shell 088B52, case 0896A0, smokes 088ED8/0897E2/08982A,
# pieces 08943C and the ten pieces 088F20 (table 0E516E).
ROOTS+=[0x284aba,0x284b0c,0x284b40,0x284a14,0x236272,0x24855c,0x239bca,0x247da8]
ROOTS+=[0x28499c+0xc*i for i in range(10)]
# Every item kind with the motion 08B9B2 (records 0E581A; kind 36 is dropped
# by the hidden boxes 09A67E, 54 seen in the release runs).
ROOTS+=[0x28631e,0x286336,0x28632a,0x286342,0x28634e,0x2864e0,0x28652c,0x28657c,0x286216,0x286258,0x28629a,0x2862dc,0x2875ae,0x286fc2,0x286dee,0x2866e6,0x287078,0x286eaa,0x2871c2,0x28741a,0x2876a4,0x28702c,0x28745c,0x287374,0x2867a8,0x287516,0x28756c,0x286c6c,0x286b54,0x287144,0x28724c,0x286840,0x2867ea,0x287814,0x2878ec,0x2879c4,0x287b40,0x287bb4,0x287a9c,0x28775a,0x287bc4,0x287c7e,0x28829a,0x288396,0x2887d2,0x28a720,0x24a200,0x2864a4,0x2864b0,0x2864bc,0x2864c8,0x2864d4,0x28635a,0x2863de,0x28639c,0x286420,0x286462,0x28aa54]
# Swordsmen 09D846/09D862/09D876 (patrol, run, walk, turn, jump, slash,
# deaths 09E842/09E8C2/09E920/09EAB8, hop and jump in 09E382..09E722).
SWORD_ROOTS=[0x2a1200,0x2a1166,0x2a1242,0x2a124c,0x2a12ca,0x2a12ee,0x2a1382,0x2a11d0,0x2a148a,0x2a13ca,0x2a13d4,
    0x2a1430,0x2a1446,0x2a154c,0x2a159e,0x2a15be,0x2a1778,0x2a178e,0x2a17c2,0x2a1892,0x2a1962,0x2a1a32,0x2a1b02,
    0x2a1b7c,0x2a1b54,0x2a1bce,0x2a1b6a,0x2a1be4,0x2a1bf6,0x2a16a6,0x2a1724,0x2a174e,
    0x2a1c84,0x2a1dbc,0x22e97e]   # knife 09ECCC (flight, landing); death splash 01C97A
ROOTS+=SWORD_ROOTS
# Prisoners 099DCC/099E06 (tied, released, run, salute, fall, ladder climb
# 0402E6, rescue) and the rope 09A97C they leave.
PRISONER_ROOTS=[0x29fad8,0x29fb34,0x29fcbe,0x29fde6,0x29fe52,0x29fbec,0x29fed0,0x29feec,0x2a02b6,0x2a0260,0x29ffe4,
    0x29fffa,0x2a0074,0x2a011c,0x2a0128,0x2a01d0,0x2a024a,0x29fe0a]
ROOTS+=PRISONER_ROOTS
# Item kinds with the motions 08BA10 (1A), 08BA38 (24) and 08BAB2 (35, 38,
# 3A..3C, 3E, 43, 45..47, 58: the foods), records 0E581A +4.
ROOTS+=[0x2865cc,0x2876e6,0x287d64,0x28851a,0x288af2,0x288c1e,0x288e1c,0x28988e,0x28a94c,0x28a582,0x288d18,0x28a672,0x28aa8c]
# Rocket launcher 05ED84 (idle, stop, turn, deaths, fire by shot, reload by
# shot, flinch, walk), its rocket 05FD8A (2648FA), the aim flash 060490 and
# the muzzle 0602E4 (programs in the P1 ROM).
ROCKET_ROOTS=[0x263bc6,0x263bf4,0x263d7a,0x263dcc,0x2640e2,0x264072,0x264b00,0x264bca,0x264c94,0x264e46,
    0x264152,0x264176,0x26419a,0x2641be,0x2641e2,0x264206,0x26422a,0x26424e,
    0x264272,0x264374,0x26443e,0x264508,0x2645d2,0x26469c,0x264766,0x264830,
    0x263fc6,0x263e1e,0x26401c,0x263ef2,0x263c78,0x263d24,0x263c22,0x263cce,0x2648fa,
    0x60370,0x60394,0x603b8,0x603dc,0x60400,0x60424,0x60448,0x6046c,0x601a0,0x60242]
ROOTS+=ROCKET_ROOTS
# Helicopter formation 04857C: rotor 24483E, lone helicopter 24473E,
# helicopter 245178 (wait) / 245188 (flight by the bank +96) / 244712, the
# droppers 244DD6/244E6E/244F06 (callbacks 04A1AE/04A208/04A262), bomb
# 244DBC, impact 245106, shell 244F9E, and the lone helicopter's turn tables
# 244FF4/245092 (245004..24502E, 2450A2..2450CC).
HELI_ROOTS=[0x24473e,0x24483e,0x244dd6,0x244e6e,0x244f06,0x244dbc,0x244f9e,0x244712,0x245106,0x245178,0x245188,
    0x245004,0x245012,0x245020,0x24502e,0x2450a2,0x2450b0,0x2450be,0x2450cc]
ROOTS+=HELI_ROOTS
# Camel riders 09F682: camel walk/stand/swap/leave 2A1DD8/2A1E56/2A1EA6/
# 2A1EF6/2A1F34/2A1F72, rider tables A0B82/A0BBA (0A098C, 16 entries each:
# +8B = FF reads entry 15), head 2A24EC/2A251E/2A2550/2A2578/2A25A0/2A25CE,
# shot 2A263E, piece 2A2658, case 2A26FE, dropped sword 2A1CEE (09EE34).
CAMEL_ROOTS=[0x2a1dd8,0x2a1e56,0x2a1ea6,0x2a1ef6,0x2a1f34,0x2a1f72,0x2a21c4,0x2a222c,0x2a22a0,0x2a2334,0x2a2380,
    0x2a23d8,0x2a2178,0x2a219e,0x2a212c,0x2a2152,0x2a1ffe,0x2a2084,0x2a2430,0x2a2266,0x2a21f8,0x2a22ea,0x2a235a,
    0x2a23b6,0x2a240e,0x2a248e,0x2a24ec,0x2a251e,0x2a2550,0x2a2578,0x2a25a0,0x2a25ce,0x2a263e,0x2a2658,0x2a26fe,0x2a1cee]
ROOTS+=CAMEL_ROOTS
# Village decoration 0B6298 (camera 336) and the market stall 0400AA.
ROOTS+=[0x2a89ac,0x2402ca]
# Village porter 03264E: figure 2389CE/2386F0, walker 237F36.
ROOTS+=[0x2389ce,0x2386f0,0x237f36]
# Mission end: MISSION COMPLETE letters 06090A (tables 060F44, 060F7C) and
# the prisoner tally 0308C0 (parade 237AEC/237B84/237BD0/237C48, tables 031276).
ROOTS+=[0x264ff8+0xa*i for i in range(14)]+[0x265084+0xa*i for i in range(6)]
ROOTS+=[0x237aec,0x237b84,0x237bd0,0x237c48,0x237c52,0x237c68,0x237c7e,0x237c94,0x237caa]
# Callbacks with a native meaning (ROM routine -> native kind, asset).
PALETTE={0x4061c:0x385,0x40632:0x214,0x40648:0x35c,0x4065e:0x20e,0x40674:0x221,0x4068a:0x21a,0x406a0:0x20e}
GENERIC={0x40948,0x40956,0x4095e,0x23e418,0x23e420,0x23e42e,0xaef14}
# Player's vehicles: Camel Slug 03458E (state programs 034368.., the rider
# graphic 034F1A of players 1/2 (tables 034388/0343A8), its no-bomb effect
# 034FD2 (23A59E), the dismount 23A0C6, the camel's smoke/spray children
# 03523C.. and 03533A/03555A), the vulcan turret 03B4E0 (23CE0C), its spark
# 03BA76 and end piece 03B936, the bullets 01B6A0/01BE24 and the dust 059E30.
ROOTS+=[0x23a1da,0x23a306,0x23a350,0x23a376,0x23a3c0,0x23a3fc,0x23a508,0x23a276,
    0x23a5ae,0x23a6aa,0x23a6ec,0x23a712,0x23a754,0x23a778,0x23a7d0,0x23a62c,
    0x23a88c,0x23a988,0x23a9ca,0x23a9f0,0x23aa32,0x23aa56,0x23aaae,0x23a90a,
    0x23a84e,0x23ab2c,0x23a0c6,0x23b212,0x23b19c,0x23b126,0x23b288,0x23b324,0x23b3ba,
    0x23ce0c,0x23d4e0,0x23d46c,0x22c1b8,0x22c4c6,0x22c040,0x24c49c]
# The generic fire 08FA4E of the boss turret's gun 0D6168: its projectile
# 08F058 (the flash 08EFFA uses 22D264, already above).
ROOTS+=[0x28c32c]
def convert(bank,root,fixed=None):
    commands=[];where={}
    def rd(pc,n):
        # Programs of the P1 ROM (the rocket launcher's flash 060490 and
        # muzzle 0602E4) read the fixed block.
        if pc<0x100000:assert fixed is not None;return fixed[pc:pc+n]
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
    bank=(a.capture/'program-bank.bin').read_bytes();fixed=(a.capture/'program-fixed.bin').read_bytes()
    programs=[dict(root=r,commands=convert(bank,r,fixed)) for r in ROOTS]
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
