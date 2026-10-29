#!/usr/bin/env python3
"""Shared readers for the MAME 0.264 soldier captures (per-frame RAM dumps,
print-only RNG/kernel logs). No emulation: only decodes recorded bytes."""
import re,struct
from pathlib import Path
RAM=0x100000
ROOT_NODE=0x1010A4
PLAYER=0x1014C4
def s32(v):return v-(1<<32) if v&0x80000000 else v
def s16(v):return v-(1<<16) if v&0x8000 else v
class Frame:
    def __init__(self,data):self.d=data
    def b(self,a):return self.d[a-RAM]
    def w(self,a):return struct.unpack_from('>H',self.d,a-RAM)[0]
    def l(self,a):return struct.unpack_from('>I',self.d,a-RAM)[0]
    def raw(self,a,n):return self.d[a-RAM:a-RAM+n]
    def actors(self):
        out=[];n=ROOT_NODE;seen=set()
        while n and n not in seen and RAM<=n<RAM+0x10000:
            seen.add(n);out.append((n,self.l(n)));n=self.l(n+8)
        return out
    def rng(self):
        words=list(struct.unpack_from('>55H',self.d,0x16));cur=(self.l(0x100084)-0x100016)//2
        return words,cur
class Capture:
    def __init__(self,path):
        self.path=Path(path);self.cache={}
    def frame(self,f):
        if f not in self.cache:
            if len(self.cache)>64:self.cache.clear()
            self.cache[f]=Frame((self.path/f'ram{f}.bin').read_bytes())
        return self.cache[f]
    def rng_calls(self):
        out={}
        for line in (self.path/'debug.log').read_text().splitlines():
            if line.startswith('RNG'):
                m=re.match(r'RNG f=(\d+) a6=(\w+) ret=(\w+)',line)
                out.setdefault(int(m.group(1))+1,[]).append((int(m.group(2),16),int(m.group(3),16)))
        return out  # keyed by the video frame whose dump follows the calls
def rng_step(words,cur):
    v=words[cur];nxt=(cur+1)%55;t=(nxt+23)%55;words[t]^=v;return v,nxt
SOLDIER_FIELDS=(('routine','l',0x00),('creator','l',0x0C),('flags15','b',0x15),('f16','b',0x16),('flash','b',0x17),('field18','b',0x18),
    ('palette','b',0x19),('word1e','w',0x1E),('anim','l',0x20),('timer','w',0x24),('speed','w',0x26),
    ('x','l',0x28),('y','l',0x2C),('vx','l',0x30),('vy','l',0x34),('parent','l',0x38),('px','l',0x3C),('py','l',0x40),
    ('cx','l',0x44),('cy','l',0x48),('mark','b',0x4C),('correction','b',0x4D),('health','w',0x4E),('field50','l',0x50),
    ('timer5a','b',0x5A),('f5b','b',0x5B),('f5c','b',0x5C),('f5d','b',0x5D),('f5e','b',0x5E),('flags5f','b',0x5F),('f60','b',0x60),('field61','b',0x61),('scalar62','l',0x62),
    ('field66','w',0x66),('variant68','b',0x68),('group69','b',0x69),('kind_angle','w',0x6C),('selector6e','w',0x6E),
    ('timer70','w',0x70),('field72','w',0x72),('counter74','w',0x74),('field76','w',0x76),('field78','w',0x78),
    ('flags7c','b',0x7C),('field7e','b',0x7E),('selected80','b',0x80),('field81','b',0x81),('target84','l',0x84),
    ('field86','w',0x86),('velocity88','l',0x88),('phase8c','w',0x8C),('target8e','w',0x8E),('target90','w',0x90),('delay92','w',0x92))
def soldier(frame,node):
    out={}
    for name,kind,off in SOLDIER_FIELDS:
        out[name]=getattr(frame,kind)(node+off)
    return out
def environment(frame):
    return dict(scroll=frame.w(0x10E99E),terrain=[frame.l(0x10C742),frame.l(0x10C746),frame.l(0x10C74A),frame.l(0x10C74E)],
        epoch=frame.b(0x10C33A),active=frame.b(0x10C33C),mode88=frame.b(0x100088),rank=frame.b(0x10FD8B),
        p1=[frame.l(PLAYER),frame.w(PLAYER+0x3C),frame.w(PLAYER+0x40),frame.l(PLAYER+0x28),frame.l(PLAYER+0x2C)],
        p1x=frame.w(0x101500),p1y=frame.w(0x101504),p2x=frame.w(0x1015B0),p2y=frame.w(0x1015B4),
        p1live=frame.l(PLAYER)!=0x3F1E,p2live=frame.l(0x101574)!=0x3F1E,
        grid=list(struct.unpack_from('>128H',frame.d,0x67E6)),groups=list(frame.raw(0x10C326,9)),bounds=bounds(frame))
FIXED=None
def bounds(frame):
    # Scene bounds read by ROM 01945C/019504 through the pointer at RAM 10C2F0
    # (a descriptor in the fixed ROM block or in RAM).
    ptr=frame.l(0x10C2F0)
    if ptr>=RAM:rd=lambda a,n:frame.raw(a,n)
    else:
        assert FIXED is not None,'set soldier_capture.FIXED to program-fixed.bin bytes'
        rd=lambda a,n:FIXED[a:a+n]
    parent,left,top,right,bottom=struct.unpack('>IHHHH',rd(ptr,12))
    out=dict(pointer=ptr,parent=parent,left=left,top=top,right=right,bottom=bottom)
    if parent:out.update(x=frame.l(parent+0x14),y=frame.l(parent+0x18),px=frame.l(parent+0x1C),py=frame.l(parent+0x20))
    return out
