#!/usr/bin/env python3
"""Convert the animation programs of the Mission 1 boss family (constructor
ROM 0D5976, routines 0D5976..0D74D4) and of its shell (generic projectile
0x2A260 path) to native commands, from the user's MAME set.

Program roots are every immediate program operand of the boss code
(`MOVEA.L #prog,A1` before 004256) plus the program tables it indexes
(0E919C, 0E91BC, 0E91DC, 0E91FC, 0E9218, 0E9234, 0E9250, 0E926C, 28B77C).
The programs live in P2 bank 1 (CPU window 0x200000..0x2FFFFF while
Mission 1 runs). Op 0x10 selects a pose by an actor byte (+61 in the
soldier programs, +83/+A6 here); op 0x14 callbacks become CALL commands
with the ROM label as provenance only. Anything else is UNRECOVERED and
fails explicitly in C when reached."""
import argparse,hashlib,json,struct,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parent.parent
# (table, entries) indexed by the boss routines; counts read from the code.
TABLES=((0xe919c,8),(0xe91bc,8),(0xe91dc,8),(0xe91fc,7),(0xe9218,7),(0xe9234,7),(0xe9250,7),(0xe926c,7),(0x28b77c,16))
# op 0x14 callbacks reached from boss programs: ROM label -> native meaning
CALLS={0x2a680:'projectile trail sprite (spawns 0x2A3B4 effect)',0x2a6a2:'projectile palette cycle (+85 -> +14)',
    0xd72e4:'spawn part 0D69DC',0xd7308:'spawn part 0D65EE',0xd7330:'spawn effect 01C706',
    0xd7354:'sound 0x1021, spawn effect 01C71E and debris 0E9360',0xd738e:'debris 0E934E',
    0xd739c:'sound 0x10EA, spawn muzzle 0D6FFC and shell 0D6F90',0xd74c6:'+66/+1E = 0xE0EF',0x1adaa:'flip facing'}
def swap(raw):
    out=bytearray(raw);out[::2]=raw[1::2];out[1::2]=raw[::2];return bytes(out)
def rom(zip_path):
    with zipfile.ZipFile(zip_path) as z:return swap(z.read('250-p1.p1')),swap(z.read('250-p2.ep1'))
def roots(p1,bank):
    out=set()
    def rl(a):
        if a<0x100000:return struct.unpack_from('>I',p1,a)[0]
        return struct.unpack_from('>I',bank,a-0x200000)[0]
    # MOVEA.L #imm,A1 (227C) followed by MOVE.W #$100,D0 / JSR 004256 in 0D5976..0D74D4
    for a in range(0xd5976,0xd74d4,2):
        if p1[a:a+2]==b'\x22\x7c' and p1[a+6:a+10]==b'\x30\x3c\x01\x00':out.add(struct.unpack_from('>I',p1,a+2)[0])
    for t,n in TABLES:
        for i in range(n):out.add(rl(t+4*i))
    out|={0x236c74,0x236c9a}   # shell (02A296, from 0D6FF6 -> 02A260) and its trail (02A3B8)
    return sorted(out)
def convert(bank,root):
    commands=[];where={}
    def rd(pc,n):
        assert 0x200000<=pc<0x300000,hex(pc);return bank[pc-0x200000:pc-0x200000+n]
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
                c=add(pc,kind='SELECT_POSE' if arg==0x61 else 'SELECT_FIELD_POSE',asset=arg,operand=length//8);c['target']=len(commands)
                for i in range(length//8):
                    at=pc+4+i*8;assert rd(at,1)[0]==4
                    dx,dy=struct.unpack('bb',rd(at+2,2));add(at,kind='POSE',flags=rd(at+1,1)[0],dx=dx,dy=dy,asset=struct.unpack('>I',rd(at+4,4))[0])
                pc+=4+length
            elif op==0x14:
                routine=struct.unpack('>I',rd(pc+2,4))[0];param=struct.unpack('>H',rd(pc+6,2))[0] if arg>=6 else 0
                if routine==0x1adaa:add(pc,kind='FLIP_FACING')
                elif routine in CALLS:add(pc,kind='CALL',asset=routine,target=param)
                else:add(pc,kind='UNRECOVERED',asset=routine)
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
            if fresh and commands[-1]['kind'] not in ('JUMP','FINISH','UNRECOVERED'):
                fixups.append((add(-pc,kind='JUMP'),pc))
    for c,t in fixups:c['target']=where[t]
    return commands
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path)
    p.add_argument('--output',type=Path,default=ROOT/'data/boss_programs.json');a=p.parse_args()
    p1,p2=rom(a.zip);bank=p2[0x100000:0x200000]
    programs=[dict(root=r,commands=convert(bank,r)) for r in roots(p1,bank)]
    kinds={};calls={}
    for prog in programs:
        for c in prog['commands']:
            kinds[c['kind']]=kinds.get(c['kind'],0)+1
            if c['kind'] in ('CALL','UNRECOVERED'):calls['0x%06X'%c['asset']]=CALLS.get(c['asset'],'unrecovered')
    out=dict(source='Program ROM of the user MAME set (mslugx): P1 code 0D5976..0D74D4 for the roots, P2 bank 1 for the programs',
        p2_bank1_sha256=hashlib.sha256(bank).hexdigest(),command_kinds=kinds,callbacks=calls,programs=programs,
        limits=['Callbacks outside CALLS are UNRECOVERED and fail explicitly when reached.',
                'Fall-through into an already converted command is an explicit JUMP (negative address marker).'])
    a.output.write_text(json.dumps(out,separators=(',',':'))+'\n')
    print(len(programs),'programs',kinds,calls)
if __name__=='__main__':main()
