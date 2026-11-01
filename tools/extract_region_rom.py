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
    (0xB6938,0x1A8,'0B6566 destructible building: cull rectangle, hurtbox, debris lists, stamps, roof map'),
    (0x2D3A5C,0x40,'0B6566 building health (0260C8)'),
    (0x546F0,0x17A,'054904 domed building: programs, offsets, delays, hurtboxes, attack row, debris lists, cull rectangle'),
    (0x553B2,0x20,'0553D2 dome missile launch offsets'),(0x5545C,0x1C,'055478 dome missile frame/zoom by height'),
    (0x5597C,0x20,'dome missile program hurtbox (+50)'),(0x2D1CF4,0x34C,'054904 domed building rank tables (0260C8)'),
    (0x2CE974,0x40,'029160 truck health (0260C8)'),(0x2CE9A8,0x40,'0296CA truck speed (0260C8)'),
    (0x29B62,0x18,'029240 truck hurtboxes (+50)'),(0x29B6E,0x18,'029802 truck body hurtboxes (+50)'),
    (0x29B92,0x10,'0292BC truck push attack (0049E8)'),(0x29BA2,0x10,'029ACE truck attack (0049E8)'),
    (0x29BB2,0x10,'029B56 truck explosion attack (0049E8)'),(0x29BC2,0x0C,'02922E truck solid box (004E76; 004EF2 reads six words, the last two are the bytes of 029BCA)'),
    (0x29BCA,0x08,'029550 truck visibility rectangle (0C8BEC)'),(0x29BDA,0x08,'029A82 truck player rectangle (01AFFC)'),
    (0x29BE2,0x12,'029AEE truck smoke list (01DA40)'),
    (0x29C58,0x42,'029A1E truck cell map'),(0x29C9A,0x42,'029A26 truck cell map'),(0x29CDC,0x42,'029A04 truck cell map'),
    (0x29D1E,0x42,'029A0C truck cell map'),(0x29D60,0x34,'029A2E truck cell map'),
    (0x29FC,0x280,'002E7C/01D9F0 sine table (cosine at +0x80)'),
    (0x1DEB8,0x48,'01C9FA/01DA2E effect cull rectangles (019504)'),
    (0x1DF00,0x128,'01D0F8 debris cull rectangles 01DF00..01DF1F, program table 01DF20 and rectangle table 01DF94 (by +91); 01D4DA fragment programs 01E008'),
    (0xDC85A,0x108,'0DC97E bomber: smoke lists/tables (01DA40), palettes, cull rectangles, hurtboxes, attack rows'),
    (0x2D06CE,0x330,'0DC9EE.. bomber rank tables (0260C8)'),
    (0x0B57BE,0xB2,'0C83E6 Mission 1 camera/map command list (pointer RAM 10E964)'),
    (0x9F610,0x6C,'09D846 swordsmen: hurtbox, attack rows 09F62C/09F63C, cull 09F64C, player rectangle 09F654, programs 09F65C, deaths 09F66C'),
    (0x2D2244,0x31C,'09D846 swordsmen rank tables 2D2244..2D24B0 and knife 09ECCC tables 2D2512/2D252C/2D253C (0260C8)'),
    (0x2D2FC0,0x80,'0A8CA0 robed swordsman rank tables 2D2FC0/2D2FDA/2D2FF4 (0260C8)'),
    (0x0E64D8,0x08,'0A9A56 robed swordsman cull rectangle (01945C)'),
    (0x00087A,0x10,'0A8F38 sub-script records of the robed swordsman (+74 index)'),
    (0x2D7214,0x02,'019F1A sub-script record 2D7214 (op 3)'),
    (0x33924,0x374,'033C98 covered wagon: body/roof/wreck cell maps (0140C0), hurtbox, cull rectangle, debris list'),
    (0x2D3E5C,0x60,'033C98 covered wagon health (0260C8)'),
    (0x39FB0,0xA4,'039590 Arabian swordsman: walk/attack program tables, hurtbox, attack row 03A004, cull rectangle 03A014, player rectangles 03A01C/03A04C'),
    (0x2D257C,0x188,'039DA8/039E9A Arabian swordsman rank tables 2D257C/2D25DE/2D2640/2D26A2 (0260C8)'),
    (0x34320,0x08,'03458E Camel Slug mount rectangle (0AA610)'),(0x92A2A,0x08,'092A32 SV-001 mount rectangle (0AA610)'),
    (0x3B424,0xBC,'03B4E0 vulcan turret: aim tables 03B424/03B464, start angles 03B4A4, aim table pointers 03B4B0, palettes 03B4C8/03B4D4'),(0x97E08,0x80,'03B65C SV-001 vulcan aim tables 097E08/097E48'),
    (0x342B8,0x150,'03458E Camel Slug: hurtboxes 0342B8 (+50 by +86), cull 034308, mount rectangle 034320, palettes 034328/034330/034338, pose table 034340, attack rows 034348/034358, program tables 034368..034407'),
    (0x354BA,0x20,'0354DA Camel Slug push attack rows'),(0x16DBE,0x10,'0128E8 player/vehicle screen records 016DBE/016DC6 (RAM 10C01C)'),
    (0x23A58E,0x20,'034F1A/034FD2 Camel rider and effect program tables by player'),(0xC4756,0x30,'0C0808 bomb constructors by type and stance'),
    (0xE61D4,0x310,'0A9CB6 death pieces: speeds E61D4, angles E6206, lists E6220.., cull rectangle E64D8 (0A9A56)'),
    (0x35644,0x24,'035668 creature: cull rectangle, hurtbox, program speeds 035658'),
    (0x99D62,0x64,'099862 prisoner: hurtbox 099D62, cull rectangles 099D6E/099D76, player rectangle 099D7E, wriggle programs 099D86'),
    (0x9AD4C,0x9C,'099DCC/099E06 prisoners: hurtbox 09AD4C, view rectangle 09ADD0, player rectangle 09ADE0, item table 09ADE8'),
    (0x2D0B9E,0x160,'099862 prisoner rank tables 2D0B9E/2D0BB8/2D0BD2 (0260C8) and item lists 2D0BEC (099D12)'),
    (0xE553E,0x20,'08C5EA item kinds deleted when their program ends'),
    (0xE559E,0x178,'08D330 score label: palettes by player 0E559E (0E55AA/0E55AE), blink sequence 0E55CA, digit speeds 0E55EC, digit programs 0E565A/0E56EE (08D7B4)'),
    (0xE561C,0x10,'08C0CC item pickup rectangle E561C, cull rectangle E5624 (0C8BEC)'),
    (0xE581A,0x870,'08D2DA item records (0x5A kinds x 0x18: flags, program, palettes, pickup and motion routines)'),
    (0xE4E84,0x2EC,'087E36 armoured vehicle: hurtbox E4E84, solid E4E90, attack rows E4E9E/E4EBE, cull E4ECE, player rectangle E4ED6, wreck maps E4EDE/E4F36/E4F8E/E4FC0, platform map E4FF2, debris lists E5060/E50B2, platform offsets E5104 (089412)'),
    (0x89554,0x100,'0886CE armoured vehicle pieces: offset and speed table 089554 (16 entries)'),
    (0x2D0E40,0x220,'087E36 armoured vehicle rank tables 2D0E40/2D0EA2/2D0F04/2D0F66 and speeds 2D0FC8/2D0FFA (0260C8)'),
    (0xE5170,0xEE,'088F20 armoured vehicle pieces table 0E516E (10 x 0x18: program, speeds, accelerations, offsets; first two bytes in the blob above)'),
    (0x334C0,0x8,'08982A exhaust smoke palette table 0334C0'),
    (0x9ADA0,0x78,'09A67E hidden item box: hurtboxes 09ADA0.., view rectangle 09ADD8 (0C8BEC), hurtbox table 09ADF8 and scatter ranges 09AE08 by +6E'),
    (0x1B4C2,0x14C,'01B882 player bullet attack rows 01B4C2..01B5B2 (by direction) and 01B5C2 (01BBE2); grenade 01B5D2 (01BFD0), its explosion rows 01B5E2/01B5F2 and their list 01B602 (01C19A); 0049E8'),
    (0x1B842,0x40,'01B882 player bullet attack row table (direction +6F)'),
    (0xE5526,0x78,'08B896 item points: special values E5526 (+7B), combo values E5556, repeated value E559A'),
    (0x2D719A,0x80,'0A8F38 sub-script record 2D719A of the swordsman 0A8C62 (+74 = 1; 019F1A interpreter)'),
    (0x8FC46,0x36,'08F058 generic projectile attack rows 08FC46/08FC56 and cull rectangle 08FC74 (019504)'),
    (0x8FD5C,0x140,'08F058 generic projectile rows: table 08FD5C (+88 & 7) and rows 08FD7C..08FE9C (16 x vx, gravity, vy by +8A)'),
    (0x2C7C,0x200,'002EB4 angle of a vector: tables 002C7C and 002D7C'),
    (0x5EB46,0x23E,'05ED84 rocket launcher: hurtboxes 5EB46/5EB62, solid boxes 5EB6E/5EB7C (004E76), push rows 5EB8A/5EB9A (0049E8), rocket hurtbox 5EBAA, cull rectangle 5EBC6, trail offsets by angle 5EBD6, program tables 5ECD6..5ED36 (fire, reload, flinch, walk), rocket offsets 5ED46/5ED56'),
    (0x5FFA0,0x200,'05FD8A rocket: attack rows 5FFA0..6009E and their table by angle 600A0 (0049E8)'),
    (0x60348,0x24,'060490 aim flash program tables 60348/6035C by the shot and the stance'),
    (0xE0962,0x2D0,'04857C helicopter formation: path point lists E0962 (049CB2), bank table by heading E09F4, shell offsets E0A0E, bomb offsets E0A1E, smoke lists E0A3E/E0A50 and their puff table E0A62, cull rectangles E0AA2/E0AAA, hurtbox E0AB2, attack rows E0ABE/E0AE2, path tables E0AF2/E0AFE and the path programs E0B0A..E0C32 (021288)'),
    (0x48E1A,0x8,'048D5E helicopter wreck rectangle (019504)'),
    (0x49006,0x4,'048F8C debris puff palette bytes by +8C'),
    (0x49762,0x8,'04940C lone helicopter player rectangle (01AF3C)'),
    (0x2D14CE,0x5DE,'04857C helicopter rank tables 2D14CE..2D1A92 (health 2D14CE/2D1530, timers 2D1592..2D171A, bomb counts 2D177C/2D17DE/2D1840, intervals 2D18BC/2D18D6, speeds 2D18F0..2D1A92) (0260C8)'),
    (0x2D1310,0x15C,'05ED84 rocket launcher rank tables 2D1310 (health), 2D1372 (speed), 2D13A4 (wait), 2D13D6 (shots), 2D1408 (aim), 2D143A (rocket health) (0260C8)'),
    (0xA0AEA,0x110,'09F682 camel riders: rider hurtboxes A0AEA/A0AF6, head hurtbox A0B02, shot attack row A0B0E, cull rectangles A0B1E/A0B26 (01945C/019504), piece and case speeds A0B2E/A0B5E, trot offsets A0B6A (0A0774), rider program tables A0B82/A0BBA by the camel state (0A098C)'),
    (0x2D303E,0x3DA,'09F682 camel riders rank tables 2D303E (head health), 2D30A0 (speed), 2D30BA (shot speed), 2D311C/2D31E0/2D317E (shot timers), 2D3242 (shots), 2D3306/2D3320 (distances), 2D333A/2D33B6 (0A0296 entry) (0260C8)'),
    (0xE08B8,0xAA,'03FC5A/0400AA market stall: cull rectangle E08B8 (03FADA), box offsets E0900, debris list E090C, collision stamps E091E/E0932 (0C88B0), hurtbox E0942, player rectangle E094E'),
    (0x32646,0x8,'03264E village porter cull rectangle (01945C)'),
    (0x605AA,0x256,'06090A MISSION COMPLETE letters: letter records 605AA/60614 (060DC4 forms), 606A6 (060E02, number 060B06 first) and 60728 (060E3C, mission 5): +60 program, +61, +72 delay, +6E/+70 target'),
    (0x60F44,0x58,'06090A letters: program tables 60F44 (letters by +60) and 60F7C (mission number by 10C2F4), cull rectangle 60F94 (01945C)'),
    (0x31B86,0x16,'032040 civilians: flight program speeds by +62 (004256)'),
    (0x31D34,0x4C,'031D80/031D92 civilians: program tables 031D34/031D40, cull rectangle 031D4C (01945C), hurtbox 031D54, offsets 031D60, panic hit kinds 031D68, command sequences 031D74/031D7A'),
    (0x63600,0x20,'063F32 C weapon shell: contact row 063600 and search row 063610 (0049E8)'),
    (0x63690,0x10,'0636B0 C explosion blast attack row 063690 (0049E8 by 063740)'),
    (0x63716,0x8,'063756 C blast cull rectangle (019504)'),
    (0x63814,0x8,'063FDC C shell cull rectangle (019504)'),
    (0x63620,0x60,'0638B4 R rocket: contact rows 063620 (level) / 063630 (vertical) and search rows 063660 / 063670 (0049E8)'),
    (0x63680,0x30,'063A92 R explosion blast rows 063680 / 063690 / 0636A0 (0636B0 payloads, 0049E8 by 063740)'),
    (0x63804,0x8,'0638B4 R rocket cull rectangle 063804 (019504)'),
    (0x2D0AE4,0x20,'0C3744 knife reach query row 2D0AE4 and 0C419E cut row 2D0AF4 (0049E8)'),
    (0x3BB70,0xC,'soldier hurtbox 03BB70: a C shell target (01B162 reads its first rectangle)'),
    (0x3BB40,0x30,'soldier hurtboxes 03BB40 / 03BB58: R rocket targets (01B162 reads their first rectangle)'),
    (0x14522,0x48,'014C0C..014C6A SV-001 tread: mode table 014522 of the kernel sweep (10C004) by the probe 013C1A word'),
    (0x18522,0x4,'014CF8 SV-001 tread: with no surface under the vehicle 013C1A leaves d5 = 0x4000 and the mode is read at 014522+0x4000 (code bytes)'),
    (0x924DE,0x554,'092A32 SV-001: hurtbox records 0924DE.., hurtbox tables 092742 / 092786 (ground by +77) / 0927CA (air) by +74 (092832), rectangles 092A10, cull 092A1A (0C8BEC, 093A46), player rectangle 092A2A (0AA610)'),
    (0x93882,0x58,'093A08 SV-001 exit: palette list 093882 (word FFFF + offset loops; walked by 093692 / 093A46)'),
    (0x939C8,0x40,'093A46 SV-001 exit attack rows 0939C8 / 0939D8, the push row 0939E8 of 093692 and the blast row 0939F8 of 093AFA (0049E8)'),
    (0x94E7A,0x30,'094F1C / 094FF8 SV-001 shell: contact row 094E7A and the burst row 094E8A (0049E8)'),
    (0x95460,0x7C,'0954DC SV-001 rider entry: offsets 095460 by the tank +74, program tables 0954A4 / 0954B4 / 0954C4 by +82, palettes 0954D4'),
    (0x95646,0x88,'0956CE SV-001 dust: offsets 095646 by +74'),
    (0x290F3A,0x12,'29090E SV-001 muzzle flash: zoom bytes 290F3A by +74 (program callback 290F4C)'),
    (0x291CEA,0xAC9E,'SV-001 programs: pose tables 291CEA..29C988 read by the program callbacks 09280E / 092820 (+70 = table[+74 / +75])'),
    (0x2B0386,0x1D2E,'SV-001 pose records 2B0386..2B20B4 (+70: turret anchor 0939BA, bomb offset 0929E6, cannon offset 092934)'),
    (0x30800,0xC0,'0308C0 prisoner tally strings 030800..0308BF (RECAPTURED PRISONER, the special prisoners and ranks 030825..03085A, blanks 03085F, GREAT!/DRAW!/NO PRISONER, 1P/2P, blank 03089C, x10000, +30000 and its blanks)'),
    (0x2C982,0x154,'0306D0 prisoner ranks: list 02C982 (count word, strings; 03010A), English index 02C9F8 and the Japanese list 02CA38 / index 02CA96 (0029F2)'),
    (0x2CAD6,0x1D96,'0306D0 English prisoner names 02CAD6 (count word 989, zero-terminated strings; 030152)'),
    (0x3D512,0x884,'03DD96 font tables: page X two-byte codes 03D512, pages H 03D596, K 03D796, k 03D996 and h 03DB96'),
    (0x3E194,0x114,'03E0A8 accent pairs of the font page 0x32 (03E194..03E2A7)'),
    (0x16DCE,0x8AE,'01824A/0182B6 mission stage records (table 017D44, missions 1..6) and their stage descriptors 016DCE.. (+A)'),
    (0x17D44,0x18,'01824A mission record table (six long pointers)'),
    (0x2D4A60,0x2F0,'0613CA Mission 2 rebels: rank tables 2D4A60..2D4CB2 (0260C8), jump table 2D4CCC (06243E), offsets 2D4D2C and delays 2D4D3C'),
    (0xB7000,0x08,'Mission 2 cell group map (RAM 10C00E = 0B7000, 01296C)'),(0xE1B6A,0x90,'Mission 2 stage 2 dynamic collision map E1B6A (0140C0, the platform under the rebels)'),
    (0xE121E,0x60,'0613CA Mission 2 rebels: cull rectangles E121E/E1226 (01945C/019504), hurtbox E1246 (+50), attack row E1252 (0049E8), solid box E1262 (004E76)'),
    (0xE0818,0x7C,'03AF08 stage transition fix-map columns: entry 3AED8 (E0818) and exit 3AEF0 (E0856)'),
]
def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('capture',type=Path);p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    if a.capture.suffix=='.zip':
        # The MAME ROM set itself: fixed P1 and P2 bank 1, the window of every
        # capture (tools/decode_level_script.Program).
        from decode_level_script import Program
        prog=Program(a.capture);fixed=prog.fixed[:0x100000];bank=prog.p2[0x100000:0x200000]
    else:fixed=(a.capture/'program-fixed.bin').read_bytes();bank=(a.capture/'program-bank.bin').read_bytes()
    out=dict(source='MAME 0.264 mslugx euro capture; fixed CPU block and P2 bank 1 window',
        program_fixed_sha256=hashlib.sha256(fixed).hexdigest(),program_bank_sha256=hashlib.sha256(bank).hexdigest(),blobs=[])
    for addr,n,why in BLOBS:
        data=fixed[addr:addr+n] if addr<0x100000 else bank[addr-0x200000:addr-0x200000+n]
        out['blobs'].append(dict(rom='0x%06X'%addr,reader=why,hex=data.hex()))
    a.output.write_text(json.dumps(out,indent=1)+'\n');print(len(BLOBS),'blobs')
if __name__=='__main__':main()
