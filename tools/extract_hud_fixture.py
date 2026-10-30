#!/usr/bin/env python3
"""HUD fixture (docs/ARMAS_VEHICULOS.md §11) from the weapons captures.

For each pair of consecutive dumps f0 < f1 of a capture (tools/capture_weapons.lua:
vram/ram<frame>.bin, weapons.csv) it keeps the HUD rows of the fix map (VRAM
7000..74FF, rows 3, 4, 5, 28 and 29) at both dumps, the panel fields of the
two panel actors 101204/1012B4, the blink counters 10E9DA, the timer 1013DE and
the frame counter 10000E at f0 and f1 (the game updates every second video
frame: the steps that ran are the frames of the capture's parity), the score 10C46A, prisoners 10C5FC and the
RAM flags at f1, and per frame f0..f1 the weapons.csv inputs of 0C071C/
0C0764 (weapon block 10C718, P1 vehicle slot 10C608, input word 10E9EC,
P1 +15) and the frame counter 10000E (a game update ran when it changed),
timer 1013DE, score 10C46A, prisoners 10C5FC, P1 routine and +5F. Intervals with a player (re)start (debug.log player_init) or with
a panel outside its play loop (0C962C/0C9644 for P1, 0C9340 for P2) are left
out. Every other fix row must hold its baseline (checked here).

The fixture holds RAM/VRAM words of the capture, no ROM data.
Usage: extract_hud_fixture.py OUT.json.gz CAPTURE[:frozen] ...
"""
import csv,gzip,json,re,struct,sys
from pathlib import Path

ROWS=(3,4,5,28,29)
PLAY=(0x0C962C,0x0C9644)

def fix(path):
    w=struct.unpack('>1280H',Path(path).read_bytes()[0xE000:0xEA00])
    return w

def main():
    out=sys.argv[1];caps=[]
    total=bad_rows=0
    for arg in sys.argv[2:]:
        d,_,mode=arg.partition(':');d=Path(d)
        frames=sorted(int(m.group(1)) for f in d.glob('vram*.bin') if (m:=re.match(r'vram(\d+)\.bin',f.name)))
        rows={}
        with open(d/'weapons.csv') as fh:
            for r in csv.DictReader(fh):rows[int(r['frame'])]=r
        starts=[int(m.group(1)) for m in re.finditer(r'frame=(\d+) player_init',(d/'debug.log').read_text())]
        base=None;intervals=[]
        for f0,f1 in zip(frames,frames[1:]):
            r0=Path(d/f'ram{f0}.bin').read_bytes();r1=Path(d/f'ram{f1}.bin').read_bytes()
            v0=fix(d/f'vram{f0}.bin');v1=fix(d/f'vram{f1}.bin')
            if base is None:base=v0
            for c in range(40):
                for row in range(32):
                    if row not in ROWS and v1[c*32+row]!=base[c*32+row]:bad_rows+=1
            ok=not any(f0<fr<=f1 for fr in starts)
            for r in (r0,r1):
                if struct.unpack('>I',r[0x1204:0x1208])[0] not in PLAY or struct.unpack('>I',r[0x12b4:0x12b8])[0]!=0x0C9340:ok=False
            if not ok or any(f not in rows for f in range(f0,f1+1)):continue
            def panel(r,a):
                g=lambda o:r[a+o];w=lambda o:struct.unpack('>H',r[a+o:a+o+2])[0]
                return [w(0x6e),w(0x74),w(0x76),w(0x78),w(0x7a),w(0x7c),w(0x7e),g(0x84),g(0x85),g(0x86),g(0x89),g(0x8a),g(0x8b)]
            steps=[]
            for f in range(f0,f1+1):
                r=rows[f]
                steps.append(r['weapon_block'][:40]+r['slot_p1']+r['input_word']+r['flags15'].zfill(2)+r['video']+r['timer']+r['score']+r['rescued']+r['routine'].zfill(8)+r['f5f'].zfill(2))
            intervals.append(dict(f0=f0,f1=f1,
                fix0=[v0[c*32+row] for row in ROWS for c in range(40)],
                fix1=[v1[c*32+row] for row in ROWS for c in range(40)],
                panel=[panel(r0,0x1204),panel(r0,0x12b4)],
                blink=[r0[0xE9DA],r0[0xE9DB]],timer=struct.unpack('>H',r0[0x13DE:0x13E0])[0],
                video=struct.unpack('>H',r0[0xE:0x10])[0],video1=struct.unpack('>H',r1[0xE:0x10])[0],
                score=r1[0xC46A:0xC46E].hex(),rescued=struct.unpack('>H',r1[0xC5FC:0xC5FE])[0],
                flags0=dict(c303=r0[0xC303],e9e3=r0[0xE9E3],c300=r0[0xC300],f003=r0[0xF003]),
                flags=dict(hud=r1[0xE9E1],c2e7=r1[0xC2E7],f001=r1[0xF001],f003=r1[0xF003],mvs=r1[0xFD82],
                           fd91=r1[0xFD91],c2e6=r1[0xC2E6],c303=r1[0xC303],e9e3=r1[0xE9E3],c300=r1[0xC300]),
                timer1=struct.unpack('>H',r1[0x13DE:0x13E0])[0],steps=steps))
        total+=len(intervals)
        caps.append(dict(name=d.name,frozen=mode=='frozen',intervals=intervals))
        print(d.name,len(frames),'dumps',len(intervals),'intervals',file=sys.stderr)
    print('fix words outside the HUD rows differing from the first dump:',bad_rows,file=sys.stderr)
    with gzip.open(out,'wt') as fh:json.dump(dict(source='tools/capture_weapons.lua runs '+' '.join(sys.argv[2:]),rows=ROWS,captures=caps),fh,separators=(',',':'))
    print(total,'intervals ->',out,file=sys.stderr)

if __name__=='__main__':main()
