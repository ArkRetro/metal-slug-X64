#!/usr/bin/env python3
"""Verify owned program ROMs and record the captured CPU bank's actual source."""
import argparse,hashlib,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
def swapped(data):
    out=bytearray(data);out[::2]=data[1::2];out[1::2]=data[::2];return bytes(out)
p=argparse.ArgumentParser();p.add_argument('rom',type=Path);p.add_argument('captures',type=Path);a=p.parse_args()
manifest=json.loads((ROOT/'tools/rom_manifest.json').read_text())
with zipfile.ZipFile(a.rom) as archive:
    raw={name:archive.read(name) for name in ('250-p1.p1','250-p2.ep1')}
for name,data in raw.items():assert hashlib.sha1(data).hexdigest()==manifest[name]['sha1'],name
p1=swapped(raw['250-p1.p1']);p2=swapped(raw['250-p2.ep1'])
fixed=(a.captures/'program-fixed.bin').read_bytes();bank=(a.captures/'program-bank.bin').read_bytes()
matches=[i+1 for i in range(4) if p2[i*0x100000:(i+1)*0x100000]==bank]
assert len(matches)==1,('No unique matching physical program bank',matches)
(a.captures/'rom-physical-bank1.bin').write_bytes(p2[:0x100000])
files=('program-fixed.bin','program-bank.bin','bios.bin','ram2360.bin','palette2360.bin','rom-physical-bank1.bin','instructions.log','debug.log','commands.log')
report=dict(program_rom_sha1={name:hashlib.sha1(data).hexdigest() for name,data in raw.items()},
 fixed_cpu_view_matches_word_swapped_p1=(fixed==p1),captured_cpu_bank_start='0x200000',
 captured_bank_frame=2360,matched_physical_rom_bank=matches[0],
 physical_bank_address=hex(matches[0]*0x100000),
 overlay_note='Physical bank 1 is an overlay, not an unbanked CPU mapping at 0x100000; CPU work RAM remains separate.',
 files={name:dict(bytes=(a.captures/name).stat().st_size,sha256=hashlib.sha256((a.captures/name).read_bytes()).hexdigest()) for name in files})
(a.captures/'inputs-provenance.json').write_text(json.dumps(report,indent=2)+'\n')
print('Captured CPU bank maps to verified physical ROM bank',matches[0],'; P1 CPU view equality:',fixed==p1)
