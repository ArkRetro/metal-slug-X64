#!/usr/bin/env python3
"""Check that a mission's private sprite export carries every sprite of the
Mission 1 export (tools/export_rest_sprites.py --keep chain).

The player brings his weapons, bombs and effects into every mission: their
descriptors and logical palettes are exported with Mission 1 (the laser
beam 01E22C: roots 22EBB6..22F476 and palettes 0x98/0x99/0x9A; the rocket
of weapon 8 2684D2/2684AA; the bombs, the H, the flames...). A mission
export made before those roots existed draws nothing for them (src/actor.c
counts the miss): the cause of BUG_MATRIX B29 (the laser's beam invisible in
Mission 2 of test ROM 22). Usage:

  check_mission_sprites.py filesystem/rest-native.json filesystem/mission2/rest-native.json

Exit 1 with the missing descriptors and palettes."""
import json,sys
def load(path):
    d=json.load(open(path))
    empty=d.get('empty',[]);empty=[empty] if isinstance(empty,str) else empty
    return {int(f['descriptor'],16) for f in d['frames']}|{int(x,16) for x in empty},{int(p,16) for p in d['palettes']}
def main():
    if len(sys.argv)!=3:sys.exit(__doc__)
    base_desc,base_pal=load(sys.argv[1]);desc,pal=load(sys.argv[2])
    md=sorted(base_desc-desc);mp=sorted(base_pal-pal)
    if md or mp:
        print('%s lacks %d descriptors and %d palettes of %s: descriptors %s palettes %s'%(sys.argv[2],len(md),len(mp),sys.argv[1],
            ' '.join('%06x'%x for x in md[:24])+(' ...' if len(md)>24 else ''),' '.join('%x'%x for x in mp)))
        sys.exit(1)
    print('PASS: %s carries the %d descriptors and %d palettes of %s'%(sys.argv[2],len(base_desc),len(base_pal),sys.argv[1]))
if __name__=='__main__':main()
