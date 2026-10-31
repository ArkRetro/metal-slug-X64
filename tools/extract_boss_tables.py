#!/usr/bin/env python3
"""Export the ROM data read by the native boss (src/neo_boss.c) from the
user's MAME set: regions of the fixed P1 block (boss data 0E9000..0E9A00:
hurtbox/attack row tables, program tables, aim table 0E9288, missile paths
0E95E2..0E9860, cull rectangles; shell rows 02A794..02A900; the generic
fire 08FA4E: muzzle offsets 08FD1C, mirror table 08FEE0), the bit-count
table 025FA6 used by 0260C8, the sine/cosine table 0029FC (320 words), and
P2 bank 1 (CPU 0x200000 window during Mission 1) 2CA800..2CB440: the
difficulty tables read through 0260C8 and the shell velocity rows; the
turret gun's muzzle programs 28B77C."""
import argparse,hashlib,json
from pathlib import Path
from extract_boss_programs import rom,ROOT
P1_REGIONS=((0xe9000,0xa00),(0x2a794,0x16c),(0x8fd1c,0x1d4))
BANK_REGIONS=((0x2ca800,0xc40),(0x28b77c,0x40))
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('zip',type=Path)
    p.add_argument('--output',type=Path,default=ROOT/'data/boss_tables.json');a=p.parse_args()
    p1,p2=rom(a.zip);bank=p2[0x100000:0x200000]
    regions=[dict(label=s,bytes=p1[s:s+n].hex()) for s,n in P1_REGIONS]
    regions+=[dict(label=s,bytes=bank[s-0x200000:s-0x200000+n].hex()) for s,n in BANK_REGIONS]
    sine=[int.from_bytes(p1[0x29fc+2*i:0x29fe+2*i],'big') for i in range(320)]
    out=dict(source='Program ROM of the user MAME set (mslugx): P1 and P2 bank 1',
        p1_sha256=hashlib.sha256(p1).hexdigest(),bit_counts=list(p1[0x25fa6:0x25fa6+256]),sine=sine,regions=regions)
    a.output.write_text(json.dumps(out,separators=(',',':'))+'\n')
    print(len(regions),'regions')
if __name__=='__main__':main()
