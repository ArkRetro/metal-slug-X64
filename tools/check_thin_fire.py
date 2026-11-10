#!/usr/bin/env python3
"""The thin fire program timing of src/marco_thinfire.c rebuilt from the ROM.

For each id of the tables 0C3C10 (level), 0C3C50 (up), 0C3C90 (down) and the
crouched 0C24DE, the form-1 row (+93*4+6E = 0x10; 0C0F62/0C0F32 fall back to
the row & 0xC when it is 0) names a program of bank 1 (0x200000..); 004260
starts it and 00427E steps it once per player update: a WAIT n (00 nn) holds
n updates (+4 counts down 0x100 a step from n<<8), an END (28) after a WAIT
ends in the update that WAIT expires, a JUMP (08) continues at its target.
The update of the first WRITE_BYTE +76 (18 vv 00 76) and of the END must be
the table's; the aim programs (0BF9D8.. of 0C4100..0C415A) their END."""
import re,struct,sys,zipfile
from pathlib import Path
SIZE={0:2,4:8,0x0c:8,0x18:4,0x1c:4,0x20:6,0x24:2,0x30:4}
def load(rom):
    z=zipfile.ZipFile(rom)
    def sw(raw):
        b=bytearray(len(raw));b[0::2]=raw[1::2];b[1::2]=raw[0::2];return b
    return sw(z.read('250-p1.p1')),sw(z.read('250-p2.ep1'))[0x100000:0x200000]
def timing(bank,pc):
    c=0;u=0;t76=None
    while True:
        at=pc-0x200000;op,arg=bank[at],bank[at+1]
        if op==0:
            d6=c
            if c<=0:c+=arg<<8
            if d6>=0:c-=0x100
            u+=1
            if c>0:continue
            if bank[at+2]==0x28:return t76,u-1
            pc+=2;continue
        if op==8:pc=struct.unpack_from('>I',bank,at+2)[0];continue
        if op==0x28:return t76,u
        if op==0x10:pc+=4+struct.unpack_from('>H',bank,at+2)[0];continue
        if op==0x14:pc+=2+arg;continue
        if op==0x18 and bank[at+3]==0x76 and t76 is None:t76=u
        pc+=SIZE[op]
def row(p1,table,form=1,char=0):
    d7=(form*4+char)*4;p=struct.unpack_from('>I',p1,table+d7)[0]
    return p or struct.unpack_from('>I',p1,table+(d7&0xc))[0]
def main():
    rom=sys.argv[1] if len(sys.argv)>1 else '/home/user/roms/mslugx.zip'
    p1,bank=load(rom);src=Path(__file__).resolve().parent.parent/'src/marco_thinfire.c';text=src.read_text()
    bad=0;n=0
    def table(name):
        body=re.search(r'static const timing_t '+name+r'\[16\]=\{(.*?)\};',text,re.S).group(1)
        return {int(i):(int(a),int(b)) for i,a,b in re.findall(r'\[(\d+)\]=\{(\d+),(\d+)\}',body)}
    for name,base in (('level',0xc3c10),('up',0xc3c50),('down',0xc3c90),('crouch',0xc24de)):
        t=table(name)
        for id,(t76,end) in t.items():
            prog=row(p1,struct.unpack_from('>I',p1,base+id*4)[0]);rom_t=timing(bank,prog);n+=1
            if rom_t!=(t76,end):bad+=1;print('%s id %X (%06X): table %s, ROM %s'%(name,id,prog,(t76,end),rom_t))
    aims=dict(re.findall(r'(AIM_\w+)=(\d+)',text))
    for name,t in (('AIM_UP',0xbf9d8),('AIM_UP_LEVEL',0xbfa98),('AIM_UP_DOWN',0xbfb18),('AIM_DOWN',0xbfb58),('AIM_DOWN_LEVEL',0xbfbd8),('AIM_DOWN_UP',0xbfc18)):
        end=timing(bank,row(p1,t))[1];n+=1
        if end!=int(aims[name]):bad+=1;print('%s: table %s, ROM %d'%(name,aims[name],end))
    print('thin fire programs: %d timings against the ROM, %d differences'%(n,bad))
    return bad!=0
if __name__=='__main__':sys.exit(main())
