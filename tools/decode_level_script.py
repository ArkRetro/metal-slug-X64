#!/usr/bin/env python3
"""Statically decode an original level script from the program ROM.

Grammar recovered from the walkers ROM 019F7E (blocking ops 0..3), 01A056/
01A0A6 (non-blocking ops 6..11), 01A372 (skip) and the condition evaluator
01A5CA. Spawn records (op 7) resolve their constructor through the spawn table
ROM 0197DC (long entries; bit 31 = not counted in the record's group). The
program ROM is read from the user's own MAME set (P1 fixed block, P2 bank
window 0x200000..0x2FFFFF at bank 1, the bank active while Mission 1 runs).
Nothing is executed; this only parses bytes."""
import argparse,json,struct,zipfile
from pathlib import Path

SPAWN_TABLE=0x0197DC
MISSION1_SCRIPT=0x2D6314
OPS={0:'camera_wait',1:'timer_wait',2:'condition_wait',3:'end',4:'end_choice',5:'end_block',
     6:'nop',7:'spawn',8:'runner',9:'block',0xa:'random_choice',0xb:'if',0xc:'camera_unlock',
     0xd:'autoscroll_start',0xe:'autoscroll_stop',0xf:'set_player_reference_69b8',
     0x10:'set_region_4330',0x11:'grouped_runner'}
# ROM 01A5CA dispatch. 0..8: live count of that group (RAM 10C326+type);
# 9: sum of groups 0..7; A: always true; B..12 compare the player position
# returned by 01903A with RAM 10C330/10C332 (op 10 operands); 13..15 player
# presence/0AAA88 queries. Types >8 other than 9/A keep their numeric name.
COMPARE={0:'==',1:'>',2:'<',3:'!='}

def swap(data):
    out=bytearray(len(data));out[0::2]=data[1::2];out[1::2]=data[0::2];return bytes(out)

class Program:
    def __init__(self,zip_path,bank=1):
        with zipfile.ZipFile(zip_path) as z:
            self.fixed=swap(z.read('250-p1.p1'));self.p2=swap(z.read('250-p2.ep1'))
        self.bank=bank
    def read(self,address,n):
        if address<0x100000:return self.fixed[address:address+n]
        assert 0x200000<=address<0x300000,hex(address)
        o=self.bank*0x100000+address-0x200000;return self.p2[o:o+n]
    def u8(self,a):return self.read(a,1)[0]
    def u16(self,a):return struct.unpack('>H',self.read(a,2))[0]
    def s16(self,a):return struct.unpack('>h',self.read(a,2))[0]
    def u32(self,a):return struct.unpack('>I',self.read(a,4))[0]

def condition(prog,a):
    kind,cmp,value=prog.u8(a),prog.u8(a+1),prog.u16(a+2)
    c=dict(type=kind,compare=COMPARE.get(cmp,cmp),value=value)
    if kind<9:c['subject']=f'group{kind}'
    elif kind==9:c['subject']='groups0..7'
    elif kind==0xa:c['subject']='always'
    else:c['subject']=f'player_query_{kind:02X}'
    return c

def statement(prog,a,depth=0):
    op,n=prog.u8(a),prog.u8(a+1)
    assert op<0x12,('Unknown script op',hex(a),op)
    assert n>=2 or op in (3,),('Zero-length statement',hex(a))
    s=dict(address=f'0x{a:06X}',op=op,name=OPS[op],length=n)
    end=a+n
    if op==0:
        s['camera']=prog.u16(a+2)
        stop=prog.u16(a+4)
        if stop!=0xffff:s['lock']=dict(stop=stop,params=[prog.u16(a+6+2*i) for i in range(4)])
    elif op==1:s['frames']=prog.u16(a+2)
    elif op==2:s['condition']=condition(prog,a+2)
    elif op==3:end=a  # 019F7E: the actor stays here (+5F bit 7, parent +60 bit 7).
    elif op==7:
        group,flags,offset=prog.u8(a+2),prog.u8(a+3),prog.u16(a+4)
        entry=prog.u32(SPAWN_TABLE+offset)
        s.update(group=group,flags=flags,variant=flags&7,table_offset=f'0x{offset:X}',
            constructor=f'0x{entry&0xffffff:06X}',group_counted=not entry>>31 and group<9,
            x=prog.u16(a+6),y=prog.u16(a+8),x_screen_relative=bool(flags&0x80),y_screen_relative=bool(flags&0x40),
            words=[prog.u16(a+10+2*i) for i in range((n-10)//2)])
    elif op==8:
        s.update(first_delay=prog.u16(a+2),period=prog.u16(a+4),camera_limit=prog.u16(a+6),count=prog.u16(a+8),
                 condition=condition(prog,a+10))
        child=statement(prog,end,depth+1);s['body']=child;end=child['_end']
    elif op==0x11:
        s['group']=prog.u16(a+2)
        child=statement(prog,end,depth+1);s['body']=child;end=child['_end']
    elif op==0xb:
        s['condition']=condition(prog,a+2)
        child=statement(prog,end,depth+1);s['then']=child;end=child['_end']
    elif op in (9,0xa):
        stop=5 if op==9 else 4;children=[]
        while prog.u8(end)!=stop:
            child=statement(prog,end,depth+1);children.append(child);end=child['_end']
        s['statements' if op==9 else 'alternatives']=children;end+=prog.u8(end+1)
    elif op==0xf:s.update(x=prog.u16(a+2),y=prog.u16(a+4))
    elif op==0x10:s.update(x=prog.u16(a+2),y=prog.u16(a+4))
    s['_end']=end
    return s

def decode(prog,base=MISSION1_SCRIPT):
    out=[];a=base
    while True:
        s=statement(prog,a);out.append(s)
        if s['op']==3:break
        assert s['op'] in (0,1,2) or 6<=s['op']<0x12,('Unexpected top-level op',s['address'],s['op'])
        a=s['_end']
    def clean(x):
        if isinstance(x,dict):return {k:clean(v) for k,v in x.items() if k!='_end'}
        if isinstance(x,list):return [clean(v) for v in x]
        return x
    return [clean(s) for s in out]

def walk(statements):
    for s in statements:
        yield s
        for key in ('body','then'):
            if key in s:yield from walk([s[key]])
        for key in ('statements','alternatives'):
            if key in s:yield from walk(s[key])

def blocking_addresses(statements):
    """Addresses where the script actor can rest (+6E): the top-level blocking
    statements and the op 3 end."""
    return [int(s['address'],16) for s in statements if s['op']<=3]

if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('zip',type=Path);p.add_argument('--base',type=lambda v:int(v,0),default=MISSION1_SCRIPT)
    p.add_argument('--output',type=Path)
    a=p.parse_args();prog=Program(a.zip);script=decode(prog,a.base)
    spawns=[s for s in walk(script) if s['op']==7]
    report=dict(source='Program ROM P1 + P2 bank 1 of the user MAME set; grammar of ROM 019F7E/01A0A6/01A372/01A5CA',
        base=f'0x{a.base:06X}',end=script[-1]['address'],statements=script,
        constructors=sorted({s['constructor'] for s in spawns}))
    text=json.dumps(report,indent=1)+'\n'
    if a.output:a.output.write_text(text)
    print(len(script),'top-level statements;',len(spawns),'spawn records; end at',report['end'])
