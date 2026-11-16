#!/usr/bin/env python3
"""Sound effect table of the Z80 sound driver (250-m1.m1 of the user's ROM).

Translated from the driver (docs/AUDIO_JUEGO.md 19):
- 2001: a latch byte < 0x20 is the bank (F819), a byte >= 0x20 the command
  (F818); with bank >= 0x10 the effect handler 20C6 reads the far pointer
  0x0088 + (bank - 0x10) * 4 (3 bytes, linear m1 offset, 57BE), then entry
  (command - 0x20) * 4 of that table: the effect's header, run by 506F.
- Header (506F): four request words (little endian) whose playing tracks
  the effect stops first (5204: same F818/F819 word, started by another
  command), the group byte D, then 5-byte track entries (3-byte pointer,
  one byte, priority E) up to a zero pointer.
- Track slot (50CF, eight slots of 0x23 bytes at FDF6): with D != 0 a slot of
  the same group started by another command is stopped and reused when its
  priority byte >= E, else the new track is dropped (lower byte = higher
  priority, equal bytes cut each other); other groups take a free slot.

Numbers only: the ROM is read from the zip at run time.
  sound_driver_table.py --rom ROMS/mslugx.zip 1087 108A 116E
  sound_driver_table.py --rom ROMS/mslugx.zip --check data/sound_policy.json
"""
import argparse,json,zipfile
from pathlib import Path
def load(rom):
    with zipfile.ZipFile(rom) as z:return z.read('250-m1.m1')
def p3(m,o):return m[o]|m[o+1]<<8|m[o+2]<<16
def effect(m,word):
    """Driver header of an effect request word (bank 0x10..0x1F)."""
    bank,cmd=word>>8,word&0xFF
    assert 0x10<=bank<0x20 and cmd>=0x20,'not an effect word: %04X'%word
    ptr=p3(m,p3(m,0x88+(bank-0x10)*4)+(cmd-0x20)*4)
    stops=[m[ptr+2*i]|m[ptr+2*i+1]<<8 for i in range(4)]
    group=m[ptr+8];q=ptr+9;tracks=[]
    while p3(m,q):tracks.append(dict(track=p3(m,q),byte=m[q+3],priority=m[q+4]));q+=5
    return dict(word='%04X'%word,header=ptr,stops=['%04X'%s for s in stops if s],group=group,tracks=tracks)
def same_rule(a,b):
    """Two effects the slot allocator treats alike: same group, same stop
    list, same track priorities."""
    return a['group']==b['group'] and a['stops']==b['stops'] and [t['priority'] for t in a['tracks']]==[t['priority'] for t in b['tracks']]
def predict(a,b):
    """Outcome of 'A then B' 12 frames later from the slot rule alone (no
    stop lists, no hardware channel stealing)."""
    if a['group']==0 or a['group']!=b['group']:return 'overlap'
    return 'cut' if a['tracks'][0]['priority']>=b['tracks'][0]['priority'] else 'drop'
def main():
    p=argparse.ArgumentParser(description=__doc__,formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument('--rom',type=Path,required=True,help='mslugx.zip')
    p.add_argument('--check',type=Path,help='sound_policy.json: compare its measured pair outcomes with the slot rule')
    p.add_argument('words',nargs='*')
    a=p.parse_args();m=load(a.rom)
    for w in a.words:print(json.dumps(effect(m,int(w,16))))
    if a.check:
        pol=json.loads(a.check.read_text())['sounds'];hdr={l:effect(m,int(s['word'],16)) for l,s in pol.items()}
        agree=disagree=unclear=0;bad=[]
        for l,s in pol.items():
            for k,mode in s['pairs'].items():
                x,y=k.split('>')
                if x!=l or y not in hdr:continue
                if mode=='unclear':unclear+=1;continue
                pr=predict(hdr[x],hdr[y])
                if pr==mode:agree+=1
                else:disagree+=1;bad.append('%s measured %s, rule %s (groups %02X/%02X, priorities %02X/%02X)'%(k,mode,pr,
                    hdr[x]['group'],hdr[y]['group'],hdr[x]['tracks'][0]['priority'],hdr[y]['tracks'][0]['priority']))
        print('pairs: %d agree, %d disagree, %d unclear'%(agree,disagree,unclear))
        for b in bad:print(' ',b)
if __name__=='__main__':main()
