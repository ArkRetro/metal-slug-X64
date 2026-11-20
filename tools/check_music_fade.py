#!/usr/bin/env python3
"""The sound driver's master level (M1, Z80): the fade 0A xx and its tick, translated, against MAME 0.264.

0A xx (4FAF): F806 = 16 * [0x0700 + l] of the M1 ROM (0: F806 = FFFF), F808 = 1 (2136). Every timer A interrupt
(TA = 691: (1024 - 691) * 144 / 8 MHz = 5.994 ms) runs 4D67: hl = F804, bc = F806;
  F808 != 0 (out): hl == 0 stays; else hl -= bc, 0 on a borrow;
4FFF..5003 (a music's bank byte 00) stores F805 = 0 and F808 = 0: the next ticks bring it back.
  F808 == 0 (in): h == FF stays; else hl += bc, FFFF on a carry or while h < FF... (4D7A..4D89, as listed).
The capture (tas_updates.lua MSX_TAS_Z80_TAP=1, from end27900 to 73410: z80tap.txt, every write to F804..F808 with
the machine time) gives each tick's stored word (F804 / F805); the model, run tick by tick from the first stored
level with the F806 / F808 the driver held at that tick, must give every one.

  check_music_fade.py TAP [--rom mslugx.zip]"""
import argparse,sys,zipfile
from pathlib import Path
def step(hl,bc,out):
    """4D67..4D97, one timer A tick."""
    if out:
        if hl==0:return 0
        hl-=bc
        return 0 if hl<0 else hl
    if hl>>8==0xff:return hl
    hl+=bc
    if hl>0xffff:return 0xffff
    if hl>>8<0xff:return 0xffff   # 4D7D..4D80: cp FF / jr c 4D85 (as in the ROM)
    return hl
def main():
    ap=argparse.ArgumentParser();ap.add_argument('tap',type=Path);ap.add_argument('--rom',type=Path,default=Path('/home/user/roms/mslugx.zip'))
    a=ap.parse_args()
    if not a.tap.is_file():print('check_music_fade: skipped (no capture)');return 0
    mem={0xf804:0,0xf805:0,0xf806:0,0xf807:0,0xf808:0};ticks=0;bad=0;last=None;direct=0;fades=[];cur=None;level=None
    for l in a.tap.read_text().split('\n'):
        if not l:continue
        _,tm,ad,v,fr=l.split();ad=int(ad,16);v=int(v,16)
        if ad==0xf804:low=v;last=ad;continue
        if ad==0xf805 and last!=0xf804:   # 4FFF (a music's bank byte 00): xor a / ld (F805),a, not a tick
            level=(v<<8)|(level&0xff);mem[ad]=v;last=ad;direct+=1;continue
        last=ad
        if ad==0xf805:
            hl=v<<8|low
            if level is not None:
                want=step(level,mem[0xf806]|mem[0xf807]<<8,mem[0xf808]!=0)
                if want!=hl:bad+=1;print('tick',ticks,'frame',fr,'model %04X capture %04X'%(want,hl)) if bad<10 else None
                if cur and hl!=level:cur[2]+=1
            level=hl;ticks+=1;continue
        mem[ad]=v
        if ad==0xf807:cur=[int(fr),mem[0xf806]|v<<8,0];fades.append(cur)
    print('ticks',ticks,'direct F805 stores',direct,'differ',bad,'F806 settings (frame, F806, ticks that changed the level):',fades)
    return 1 if bad or ticks==0 else 0
if __name__=='__main__':sys.exit(main())
