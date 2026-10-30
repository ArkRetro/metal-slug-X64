#!/usr/bin/env python3
"""Account for ALL supplied program-ROM bytes, keeping code/data status explicit."""
import argparse,hashlib,json,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
out=a.output;out.mkdir(parents=True,exist_ok=False);known=json.loads((ROOT/'tools/rom_manifest.json').read_text())
with zipfile.ZipFile(a.rom) as z:programs={name:z.read(name) for name in ['250-p1.p1','250-p2.ep1','250-m1.m1']}
files=[]
for name,raw in programs.items():
 assert hashlib.sha1(raw).hexdigest()==known[name]['sha1']
 data=raw
 if name!='250-m1.m1':
  data=bytearray(raw);data[::2]=raw[1::2];data[1::2]=raw[::2]
 stem={'250-p1.p1':'m68k-p1','250-p2.ep1':'m68k-p2-physical','250-m1.m1':'z80-m1-physical'}[name]
 (out/(stem+'.bin')).write_bytes(data)
 files.append(dict(source=name,size=len(raw),source_sha1=hashlib.sha1(raw).hexdigest(),cpu_order_sha256=hashlib.sha256(data).hexdigest(),role='Program ROM; full code/data partition not yet verified'))
 if name=='250-p2.ep1':
  for bank in range(4):(out/f'm68k-p2-bank{bank+1}.bin').write_bytes(data[bank*0x100000:(bank+1)*0x100000])
 if name=='250-m1.m1':
  # Driver reset entries 1E/0E/06/02 map the initial windows linearly through F7FF.
  (out/'z80-initial-map.bin').write_bytes(data[:0xf800])
manifest=dict(status='ALL_PROGRAM_ROM_INPUTS_ACCOUNTED',programs=files,total_program_bytes=sum(len(b) for b in programs.values()),m68k_banks=[dict(physical_bank=n+1,cpu_window='0x200000..0x2FFFFF',source_offset_in_p2=n*0x100000,size=0x100000,execution_status='Not assumed executable; requires code/data and bank-state recovery') for n in range(4)],z80=dict(initial_map='M1 physical 0..0xF7FF, followed by RAM 0xF800..0xFFFF',remaining_physical_bytes=len(programs['250-m1.m1'])-0xf800,bank_windows=[dict(cpu_start=hex(base),size=size,physical_pages=len(programs['250-m1.m1'])//size) for base,size in [(0x8000,0x4000),(0xc000,0x2000),(0xe000,0x1000),(0xf000,0x800)]]),limits='Accounting for every program byte is not decompiling every instruction. Graphics/sample ROMs are data. This manifest does not mark unknown bytes as code or claim a complete native port.')
(out/'program-set.json').write_text(json.dumps(manifest,indent=2)+'\n');print(json.dumps(manifest,indent=2))
