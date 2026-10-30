#!/usr/bin/env python3
"""Import the ROM data tables read by the region's generic actors.

Each entry is (address, length) with the reading routine as provenance. Data
only: no code bytes are exported (cell maps, hurtbox/attack descriptors, rank
tables, rectangles). Source: program-fixed.bin / program-bank.bin of a MAME
0.264 capture (tools/mame/capture_soldier_campaign.lua)."""
import argparse,hashlib,json
from pathlib import Path
# address, length, reader (provenance)
BLOBS=[
    (0x546F0,0x17A,'054904 domed building: programs, offsets, delays, hurtboxes, attack row, debris lists, cull rectangle'),
    (0x553B2,0x20,'0553D2 dome missile launch offsets'),(0x5545C,0x1C,'055478 dome missile frame/zoom by height'),
    (0x5597C,0x20,'dome missile program hurtbox (+50)'),(0x2D1CF4,0x34C,'054904 domed building rank tables (0260C8)'),
    (0x2CE974,0x40,'029160 truck health (0260C8)'),(0x2CE9A8,0x40,'0296CA truck speed (0260C8)'),
    (0x29B62,0x18,'029240 truck hurtboxes (+50)'),(0x29B6E,0x18,'029802 truck body hurtboxes (+50)'),
    (0x29B92,0x10,'0292BC truck push attack (0049E8)'),(0x29BA2,0x10,'029ACE truck attack (0049E8)'),
    (0x29BB2,0x10,'029B56 truck explosion attack (0049E8)'),(0x29BC2,0x08,'02922E truck solid box (004E76)'),
    (0x29BCA,0x08,'029550 truck visibility rectangle (0C8BEC)'),(0x29BDA,0x08,'029A82 truck player rectangle (01AFFC)'),
    (0x29BE2,0x12,'029AEE truck smoke list (01DA40)'),
    (0x29C58,0x42,'029A1E truck cell map'),(0x29C9A,0x42,'029A26 truck cell map'),(0x29CDC,0x42,'029A04 truck cell map'),
    (0x29D1E,0x42,'029A0C truck cell map'),(0x29D60,0x34,'029A2E truck cell map'),
    (0x29FC,0x280,'002E7C/01D9F0 sine table (cosine at +0x80)'),
    (0x1DEB8,0x48,'01C9FA/01DA2E effect cull rectangles (019504)'),
    (0xDC85A,0x108,'0DC97E bomber: smoke lists/tables (01DA40), palettes, cull rectangles, hurtboxes, attack rows'),
    (0x2D06CE,0x330,'0DC9EE.. bomber rank tables (0260C8)'),
    (0x0B57BE,0xB2,'0C83E6 Mission 1 camera/map command list (pointer RAM 10E964)'),
    (0x2D2FC0,0x80,'0A8CA0 robed swordsman rank tables 2D2FC0/2D2FDA/2D2FF4 (0260C8)'),
    (0x0E64D8,0x08,'0A9A56 robed swordsman cull rectangle (01945C)'),
    (0x00087A,0x10,'0A8F38 sub-script records of the robed swordsman (+74 index)'),
    (0x2D7214,0x02,'019F1A sub-script record 2D7214 (op 3)'),
]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args();fixed=(a.capture/'program-fixed.bin').read_bytes();bank=(a.capture/'program-bank.bin').read_bytes()
    out=dict(source='MAME 0.264 mslugx euro capture; fixed CPU block and P2 bank 1 window',
        program_fixed_sha256=hashlib.sha256(fixed).hexdigest(),program_bank_sha256=hashlib.sha256(bank).hexdigest(),blobs=[])
    for addr,n,why in BLOBS:
        data=fixed[addr:addr+n] if addr<0x100000 else bank[addr-0x200000:addr-0x200000+n]
        out['blobs'].append(dict(rom='0x%06X'%addr,reader=why,hex=data.hex()))
    a.output.write_text(json.dumps(out,indent=1)+'\n');print(len(BLOBS),'blobs')
if __name__=='__main__':main()
