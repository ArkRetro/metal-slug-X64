#!/usr/bin/env python3
"""The sprite-list writers of the 68K (11D04 / 11E60 and their 32 column writers a106..fffe), translated
routine to routine, checked word for word against MAME 0.264 VRAM dumps of consecutive frames.

Per game frame: 11D04 (end of the frame, after the 113EA sort) writes SCB1/2/4 of the list that has just been
built, into the sprites the screen does not show (forward from the window start up to the other pass's +6, or
backward from the end down to +4), and flips 1946; at the next vblank 11E60 writes SCB3 of every column and the
SCB1/2/4 the first pass could not (the overlap), then hides the rest of the window (103A6 / 10BC8: SCB3 = 8000).
The writer is chosen per entry by (zoom word != FFFF) << 2 | attribute bits 0..1 (a086 + 4 * that, +0x20 for
11E60, +0x40 backward). Entry (a03e): +0 x, +2 y, +4 zoom x / zoom y bytes (actor +12 / +13), +6 palette << 8 |
attribute & 7, +8 descriptor (P2: bank = bits 20..23 - 1). Descriptor: word count << 8 | xor of the attribute;
per group x, y, columns << 8 | rows, then the rows' tile words of every column.

Usage: check_zoom_scb.py CAPTURE [--rom mslugx.zip] [--frames A-B]. CAPTURE: dumps of every frame (tas_updates.lua,
MSX_TAS_DUMP_PERIOD=1): for each dump f just after an 11D04 (1946 != 1947) and the next dump g after the other list's,
VRAM(f) + 11E60(f's list) + 11D04(g's list) must equal VRAM(g) in SCB1..SCB4 of the windows' sprites (3954 / 395C),
and the windows' +4 / +6 RAM(g). Pairs where a002 changed the windows (another screen) are not compared."""
import argparse,struct,sys,zipfile
from pathlib import Path

A5=0x108000
def ram_w(r,a):return struct.unpack('>H',r[a-0x100000:a-0x100000+2])[0]
def ram_l(r,a):return struct.unpack('>I',r[a-0x100000:a-0x100000+4])[0]
def s16(v):v&=0xffff;return v-0x10000 if v&0x8000 else v

def load_p2(rom):
    raw=zipfile.ZipFile(rom).read('250-p2.ep1');p=bytearray(len(raw));p[0::2]=raw[1::2];p[1::2]=raw[0::2];return bytes(p)

class Writer:
    """One column writer: pass 'D' (11D04, table a086 / a0c6) or 'E' (11E60, a0a6 / a0e6), forward or backward."""
    def __init__(self,vram,p2):self.v=vram;self.p2=p2;self.writes=0
    def w(self,addr,data):self.v[addr&0xffff]=data&0xffff;self.writes+=1
    def entry(self,pas,fwd,e,win):
        x,y,zoom,d1,ptr=e
        zx,zy=zoom>>8,zoom&0xff
        zoomed=zoom!=0xffff;hflip=bool(d1&1);vflip=bool(d1&2)
        a3=(((ptr>>20)&0xf)-1)*0x100000+(ptr&0xfffff)
        def rd():
            nonlocal a3
            v=struct.unpack('>H',self.p2[a3:a3+2])[0];a3+=2;return v
        idx=win[2] if fwd else win[3]
        w0=rd();d1=(d1&0xff00)|((d1^w0)&0xff)
        for _ in range(w0>>8):
            xo,yo,cr=rd(),rd(),rd()
            cols,rows=cr>>8,cr&0xff
            if hflip:xo=(~xo)&0xffff
            if vflip:yo=((~yo)+((rows<<4)&0xff0))&0xffff
            if not zoomed:
                d5=s16(xo+x)<<7
                v=((yo+y)<<7)&0xffff;scb3=(v&0xff00)|((v|rows)&0xff)
                d4=0
            else:
                m=zy+1
                v=((((s16(yo)*m)&~0xff)>>1)+((y<<7)&0xffff))&0xffff
                scb3=(v&0xff00)|((v|((rows*m+0xff)>>8))&0xff)
                m=zx+1
                p=s16(xo)*m;d4=p&0xff;d5=((p&~0xff)>>1)+(s16(x)<<7)
                d3=s16((m<<4)-0x100)
            n=(rows-1)&0x1f
            for c in range(cols):
                if c and not hflip:   # the x step before every later column (c49e / b970)
                    d5+=(((d4+0x100)&0xff00)>>1) if zoomed else 0x800
                lim={('D',True):win[3],('E',True):win[1],('D',False):win[2],('E',False):win[0]}[(pas,fwd)]
                if fwd and s16(idx-lim)>=0:break
                if not fwd and s16(idx-lim)<=0:break
                if zoomed:
                    d4=(d4&0xff)+d3
                    if d4<0:a3+=2*(n+1);continue   # no width: skipped, no x step
                    if hflip:d5-=((d4+0x100)&0xff00)>>1
                elif hflip:d5-=0x800
                if (((d5>>1)+0x200)&0xffff)>=0x5000:a3+=2*(n+1);continue
                if not fwd:idx=(idx-1)&0xffff
                show=True
                if pas=='E':
                    self.w(0x8201+idx,scb3)
                    show=(s16(idx-win[3])>=0) if fwd else (s16(idx-win[2])<0)
                if show:
                    self.w(0x8401+idx,d5&0xffff)
                    self.w(0x8001+idx,((d4&0xff00)|zy) if zoomed else 0xffff)
                    a=((idx+1)<<6)&0xffff
                    tiles=[rd() for _ in range(n+1)]
                    for k,t in enumerate(tiles):
                        o=2*(n-k) if vflip else 2*k
                        self.w(a+o,t);self.w(a+o+1,d1)
                    if zoomed:
                        for k in range(n+1,32):self.w(a+2*k,0)
                else:a3+=2*(n+1)
                if fwd:idx=(idx+1)&0xffff
            else:continue
            break
        return self.done(fwd,win,idx)
    def done(self,fwd,win,idx):
        if fwd:win[2]=idx
        else:win[3]=idx

def pending(r):
    """The list 11D04 has written and 11E60 has still to show: (forward?, window copies, entries)."""
    f=r[0x9946]
    fwd=bool(f&0x80)
    base=A5+(0x1948 if fwd else 0x294e)
    end=ram_l(r,base+0x400)
    ents=[]
    for p in range(base,end,4):
        o=ram_w(r,p);a=base+o
        ents.append((ram_w(r,a),ram_w(r,a+2),ram_w(r,a+4),ram_w(r,a+6),ram_l(r,a+8)))
    return fwd,ents,f!=r[0x9947]

def windows(r):return [[ram_w(r,A5+b+2*i) for i in range(4)] for b in (0x3954,0x395c)]

class CWriter:
    """The same entries through src/neo_scb.c (--c build/host/neo_scb.so)."""
    def __init__(self,lib,vram,p2):
        import ctypes as C
        self.C=C;self.v=vram;self.p2=p2;self.writes=0;self.lib=C.CDLL(str(lib))
        RD=C.CFUNCTYPE(C.c_uint16,C.c_void_p,C.c_uint32,C.c_uint32);WR=C.CFUNCTYPE(None,C.c_void_p,C.c_uint16,C.c_uint16)
        def rd(_,ptr,i):
            a=(((ptr>>20)&0xf)-1)*0x100000+(ptr&0xfffff)+2*i;return (p2[a]<<8)|p2[a+1]
        def wr(_,a,d):self.v[a]=d;self.writes+=1
        self.rd=RD(rd);self.wr=WR(wr)
        class E(C.Structure):_fields_=[('x',C.c_uint16),('y',C.c_uint16),('zoom',C.c_uint16),('attr',C.c_uint16),('descriptor',C.c_uint32)]
        self.E=E
    def entry(self,pas,fwd,e,win):
        C=self.C;w=(C.c_uint16*4)(*win)
        self.lib.neo_scb_entry(C.c_bool(pas=='E'),C.c_bool(fwd),C.byref(self.E(*e)),w,self.rd,self.wr,None)
        win[:]=list(w)
    def w(self,a,d):self.v[a&0xffff]=d&0xffff;self.writes+=1
def run_pass(wr,pas,fwd,ents,wins):
    for w in wins:
        if fwd:w[2]=w[0]
        else:w[3]=w[1]
    for e in (ents if fwd else reversed(ents)):
        d1=e[3];win=wins[1] if d1&4 else wins[0]
        wr.entry(pas,fwd,(e[0],e[1],e[2],d1&~4&0xffff,e[4]),win)
    if pas=='E':
        for w in wins:
            if fwd:
                d0=w[2] if s16(w[2])>s16(w[3]) else w[3]
                for k in range((w[1]-d0)&0x1ff):wr.w(0x8201+d0+k,0x8000)
            else:
                d0=w[2] if s16(w[2])<s16(w[3]) else w[3]
                for k in range((d0-w[0])&0x1ff):wr.w(0x8201+d0-1-k,0x8000)

def main():
    ap=argparse.ArgumentParser();ap.add_argument('capture',type=Path);ap.add_argument('--frames')
    ap.add_argument('--c',type=Path,help='src/neo_scb.c built as a shared library: the C translation instead')
    ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    a=ap.parse_args()
    if not a.capture.is_dir() or not a.rom.is_file():print('check_zoom_scb: skipped (no capture or ROM)');return 0
    p2=load_p2(a.rom)
    frames=sorted(int(f.stem[3:]) for f in a.capture.glob('ram*.bin'))
    if a.frames:lo,hi=map(int,a.frames.split('-'));frames=[f for f in frames if lo<=f<=hi]
    tot=dict(frames=0,words=0,diff=0,zoom_entries=0,writes=0);bad=0;variants=[0]*8
    fs=set(frames)
    def flags(f):r=(a.capture/f'ram{f}.bin').read_bytes();return r[0x9946],r[0x9947]
    fl={f:flags(f) for f in frames}
    for f in frames:
        # f: 11D04 has written a list and 1946 is flipped; g: the next dump where the other list is so (11E60 of f's
        # list at a vblank, then 11D04 of g's; dumps in between can fall inside 11D04 and are not compared).
        if fl[f][0]==fl[f][1]:continue
        if fl.get(f-1,(None,None))[0]==fl[f][0]:continue   # the same state already compared from its first dump
        g=f+1
        while g in fs and not (fl[g][0]!=fl[g][1] and fl[g][0]!=fl[f][0]):g+=1
        if g not in fs:continue
        r0=(a.capture/f'ram{f}.bin').read_bytes();r1=(a.capture/f'ram{g}.bin').read_bytes()
        v0=list(struct.unpack('>34304H',(a.capture/f'vram{f}.bin').read_bytes()))
        v1=struct.unpack('>34304H',(a.capture/f'vram{g}.bin').read_bytes())
        fwd0,e0,p0=pending(r0);fwd1,e1,p1=pending(r1)
        wins=windows(r0);wr=CWriter(a.c,v0,p2) if a.c else Writer(v0,p2)
        run_pass(wr,'E',fwd0,e0,wins);run_pass(wr,'D',fwd1,e1,wins)
        if [w[:2] for w in windows(r0)]!=[w[:2] for w in windows(r1)]:continue   # a002 set other windows (another screen)
        # the windows' sprites (index i: sprite i + 1): SCB1 and SCB2..4; sprites outside them are other writers'
        ids=sorted({i for w in windows(r0) if w[0]!=0xffff for i in range(w[0],w[1])})
        rng=[0x40*(i+1)+k for i in ids for k in range(64)]+[b+1+i for b in (0x8000,0x8200,0x8400) for i in ids]
        d=[i for i in rng if v0[i]!=v1[i]]
        wn=windows(r1);wok=[w[2:] for w in wins]==[w[2:] for w in wn]
        z=sum(1 for e in e0+e1 if e[2]!=0xffff)
        for e in e0+e1:variants[(e[2]!=0xffff)<<2|(e[3]&3)]+=1
        tot['frames']+=1;tot['words']+=len(rng);tot['diff']+=len(d);tot['zoom_entries']+=z;tot['writes']+=wr.writes
        if d or not wok:
            bad+=1;print(f,g,'differ',len(d),'words',[(hex(i),hex(v0[i]),hex(v1[i])) for i in d[:6]],'windows',wins,wn)
        else:print(f,g,'equal: %d entries (%d zoomed), %d writes'%(len(e0)+len(e1),z,wr.writes))
    print(tot,'entries per writer (zoom<<2|flips):',variants)
    return 1 if bad or not tot['frames'] else 0
if __name__=='__main__':sys.exit(main())
