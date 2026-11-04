#!/usr/bin/env python3
"""Decode addressed ROM ranges with MAME's actual M68000 decoder; no RAM patches."""
import argparse,hashlib,json,os,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--range',action='append',required=True,help='hex start:length');a=p.parse_args()
ranges=[tuple(int(x,16) for x in item.split(':')) for item in a.range]
assert all(0<=s<0x100000 and 0<n<=0x100000-s for s,n in ranges)
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for n in ('cfg','nvram'):(out/n).mkdir()
lines=['local f=0','emu.register_frame_done(function()','f=f+1','if f==2 then']
for start,length in ranges:
 name=out/f'dasm-{start:06x}.txt';assert ',' not in str(name) and '"' not in str(name)
 lines.append('manager.machine.debugger:command('+json.dumps(f'dasm "{name}",0x{start:x},0x{length:x},1')+')')
lines+=['local p=assert(io.open('+json.dumps(str(out/'program-fixed.bin'))+",'wb'))", "p:write(manager.machine.devices[':maincpu'].spaces.program:read_range(0,0xfffff,8));p:close()",'manager.machine:exit()','end',"end,'frame')"]
(out/'capture.lua').write_text('\n'.join(lines)+'\n')
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
with (out/'xvfb.log').open('w') as xlog:
 server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
 try:
  number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
  cmd=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(out/'capture.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
  with (out/'mame.log').open('w') as log:subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=60)
  assert 'Error in command' not in (out/'debug.log').read_text() and 'LUA ERROR' not in (out/'mame.log').read_text()
  assert all((out/f'dasm-{s:06x}.txt').is_file() for s,n in ranges)
  with zipfile.ZipFile(a.rom_dir/'mslugx.zip') as archive:raw=archive.read('250-p1.p1')
  assert hashlib.sha1(raw).hexdigest()==json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
  expected=bytearray(raw);expected[::2]=raw[1::2];expected[1::2]=raw[::2]
  observed=(out/'program-fixed.bin').read_bytes()
  assert all(observed[s:s+n]==expected[s:s+n] for s,n in ranges),'Selected CPU bytes differ from provided ROM'
  (out/'validation.json').write_text(json.dumps(dict(status='STATIC_M68000_DISASSEMBLY',ranges=[dict(start=hex(s),length=n) for s,n in ranges],selected_ranges_match_rom=True,cpu_view_sha256=hashlib.sha256(observed).hexdigest(),p1_sha256=hashlib.sha256(expected).hexdigest(),whole_view_matches_p1=observed==expected,limits='Canonical decoder output; instructions were not executed at these addresses.'),indent=2)+'\n')
 finally:server.terminate();server.wait(timeout=8)
print(out)
