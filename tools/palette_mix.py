#!/usr/bin/env python3
"""Palette slot colours as the original's vblank builds them (008402).

A slot of 1068EA holds +A (its resource), +C (a target) and +E (a weight);
009CCE sets +C / +E and the dirty bit. With +A = +C, or the weight 0, the
slot shows +A (005978); with a weight of 31 or more it shows +C (005978 on
+C); otherwise 005EA0 mixes them per channel: base = +A's channel,
difference = +C's - +A's (16-bit word arithmetic of the 68000, the high
channel keeps the colour's bit 15), base + the signed byte at
0051D8 + 64 * weight + difference, and the 15-bit word goes through the
table 224000 as 005978 does (index = word * 2 as a signed word). Pen 0 is
not written (005EA0 / 005978 start at the second word).

Palette n is the 16 words at 200000 + 32 n (P2 bank 1, as 009B84).

python3 tools/palette_mix.py mslugx.zip --verify CAPTURE: every RAM / palette
dump pair of the capture where a slot has +C other than +A is compared with
this.

--into EXPORT --plain 3bf,... --mix 38e:3c2: adds to a mission export
(rest-native.json) the plain palettes and, for each pair (in the order of
src/neo_palmix.h), the weights 1..30 as the resources 4000 | pair << 5 | n.
A palette the export already has must be the same.
"""
import argparse,gzip,hashlib,json,struct,sys
from pathlib import Path
sys.path.insert(0,str(Path(__file__).resolve().parent))
from decode_level_script import Program
from mission_tiles import rgba16

class Mixer:
    def __init__(self,zip_path):self.p=Program(zip_path,bank=1)
    def word(self,n,k):return self.p.u16(0x200000+n*32+2*k)
    def convert(self,w):
        i=(2*w)&0xffff
        i=i-0x10000 if i&0x8000 else i
        return self.p.u16(0x224000+i)
    def plain(self,n):return [0]+[self.convert(self.word(n,k)) for k in range(1,16)]
    def entry(self,n,diff):
        """The byte of 0051D8 + 64 n + diff (diff a signed word)."""
        return self.p.u8(0x51d8+64*n+diff)
    def mix(self,a,c,n):
        if n==0:return self.plain(a)                                  # 005EA2
        if n>=0x1f:return self.plain(c)                               # 005EAC (005978 on +C)
        out=[0]
        for k in range(1,16):
            d0,d1=self.word(a,k),self.word(c,k)                       # d0: +A (a1), d1: +C (a0)
            def chan(x,y):
                d=((y&0x1f)-(x&0x1f))&0xffff
                ds=d-0x10000 if d&0x8000 else d
                t=self.entry(n,ds)
                return ((d&0xff00)|t)+(x&0x1f)&0xffff                 # move.b then add.w
            lo=chan(d0,d1)
            d0>>=5;d1>>=5
            mid=chan(d0,d1)
            d0>>=5;d1>>=5
            d=(d1-d0)&0xffff;ds=d-0x10000 if d&0x8000 else d
            hi=(((d&0xff00)|self.entry(n,ds))+d0)&0xffff
            r=(hi<<5)&0xffff;r=(r&0xff00)|((r|mid)&0xff)
            r=(r<<5)&0xffff;r=(r&0xff00)|((r|lo)&0xff)
            r=(r*2)&0xffff;i=r-0x10000 if r&0x8000 else r
            out.append(self.p.u16(0x224000+i))
        return out
    def slot(self,a,c,n):
        return self.plain(a) if a==c else self.mix(a,c,n)

def verify(m,capture,cases=None):
    """The palette dump of frame f shows the slots as the update of f - 1
    left them (the vblank builds them before the update of f): with a RAM
    dump of f - 1 that state is used (paired), else the state of f (only a
    slot that did not change in that update can match)."""
    ok=bad=paired=unpaired=0;frames=set(int(f.stem[3:]) for f in capture.glob('ram*.bin'))
    for f in sorted(frames):
        pal=capture/f'palette{f}.bin'
        if not pal.exists():continue
        src=f-1 if f-1 in frames else f
        ram=(capture/f'ram{src}.bin').read_bytes();pw=struct.unpack('>4096H',pal.read_bytes())
        for s in range(256):
            e=ram[0x68EA+16*s:0x68EA+16*s+16]
            a,c=struct.unpack('>HH',e[10:14]);n=e[14]
            if a in (0,0xffff) or c in (0,0xffff) or a==c:continue
            want=m.slot(a,c,n);got=list(pw[s*16:s*16+16])
            if want[1:]==got[1:]:
                ok+=1;paired+=src!=f
                if cases is not None:cases[(a,c,n)]=hashlib.sha256(struct.pack('>15H',*got[1:])).hexdigest()
            elif src==f:unpaired+=1   # the update of f may have changed the slot: not comparable
            else:
                bad+=1;print(f'{f} slot {s}: +A {a:X} +C {c:X} +E {n:X}: differs',file=sys.stderr)
    return ok,bad,paired,unpaired

if __name__=='__main__':
    ap=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('rom',type=Path);ap.add_argument('--verify',type=Path,action='append',default=[])
    ap.add_argument('--fixture',type=Path);ap.add_argument('--check',type=Path);ap.add_argument('--into',type=Path);ap.add_argument('--plain',default='');ap.add_argument('--mix',action='append',default=[])
    a=ap.parse_args()
    if not a.rom.exists():print(f'SKIP: palette mix (no {a.rom})');sys.exit(0)
    m=Mixer(a.rom)
    if a.into:
        d=json.loads(a.into.read_text());pal=d['palettes'];added=[0]
        def put(rid,words):
            k=hex(rid);v=rgba16(words)
            assert pal.get(k,v)==v,('palette differs from the export',k)
            if k not in pal:pal[k]=v;added[0]+=1
        for r in [int(x,16) for x in a.plain.split(',') if x]:put(r,m.plain(r))
        for p,pair in enumerate(a.mix):
            A,C=(int(x,16) for x in pair.split(':'));put(A,m.plain(A));put(C,m.plain(C))
            for n in range(1,31):put(0x4000|p<<5|n,m.mix(A,C,n))
        d['palettes']=dict(sorted(pal.items(),key=lambda kv:int(kv[0],16)));a.into.write_text(json.dumps(d))
        print(f'{a.into}: {added[0]} palettes added')
    cases={}
    for cap in a.verify:
        ok,bad,paired,unpaired=verify(m,cap,cases)
        print(f'{cap}: {ok} slots with a target equal ({paired} with the previous update), {bad} different, {unpaired} unpaired not comparable')
        if bad:sys.exit(1)
    if a.fixture:
        a.fixture.write_bytes(gzip.compress(json.dumps(dict(source='MAME 0.264 palette RAM of the slots with a target (tools/palette_mix.py --verify): sha256 of pens 1..15',
            cases=[[f'{k[0]:x}',f'{k[1]:x}',k[2],v] for k,v in sorted(cases.items())])).encode(),mtime=0))
        print(f'{a.fixture}: {len(cases)} (+A, +C, +E) cases')
    if a.check:
        d=json.loads(gzip.decompress(a.check.read_bytes()));bad=0
        for A,C,n,h in d['cases']:
            if hashlib.sha256(struct.pack('>15H',*m.slot(int(A,16),int(C,16),n)[1:])).hexdigest()!=h:bad+=1;print('differs',A,C,n)
        print(('FAIL' if bad else 'PASS')+f': palette mix 005EA0 / 005978: {len(d["cases"])-bad} of {len(d["cases"])} slot states equal to MAME')
        sys.exit(1 if bad else 0)
