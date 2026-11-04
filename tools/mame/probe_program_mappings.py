#!/usr/bin/env python3
"""Verify CPU-visible bank/vector mappings in an isolated MAME instance.

Explicitly changes mapping selectors in a paused frame callback. Not gameplay
coverage or proof of executable content. ROM/BIOS binaries remain private.
"""
import argparse,hashlib,json,os,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for n in ('cfg','nvram'):(out/n).mkdir()
with zipfile.ZipFile(a.rom_dir/'mslugx.zip') as z:raw={n:z.read(n) for n in ('250-p1.p1','250-p2.ep1','250-m1.m1')}
manifest=json.loads((ROOT/'tools/rom_manifest.json').read_text())
for name,b in raw.items():assert hashlib.sha1(b).hexdigest()==manifest[name]['sha1'],name
def swapped(b):
 data=bytearray(b);data[::2]=b[1::2];data[1::2]=b[::2];return bytes(data)
p1=swapped(raw['250-p1.p1']);p2=swapped(raw['250-p2.ep1']);m1=raw['250-m1.m1']
with zipfile.ZipFile(a.rom_dir/'neogeo.zip') as z:sm1=z.read('sm1.sm1')
lua=r'''local frame=0
local main=manager.machine.devices[':maincpu'].spaces.program
local audio=manager.machine.devices[':audiocpu'].spaces.program
local io=manager.machine.devices[':audiocpu'].spaces.io
local function save(path,data) local f=assert(io_file.open(path,'wb'));f:write(data);f:close() end
'''
# Avoid shadowing Lua's standard io library with the emulated IO space.
lua=lua.replace('local frame=0','local io_file=io\nlocal frame=0')
lua+='local output='+json.dumps(str(out))+'\n'
lua+=r'''emu.register_frame_done(function()
 frame=frame+1
 if frame~=2 then return end
 save(output..'/bios-vectors.bin',main:read_range(0xc00000,0xc0007f,8))
 for mode=0,1 do
  main:write_u8(mode==0 and 0x3a0003 or 0x3a0013,0)
  save(output..'/vectors-'..mode..'.bin',main:read_range(0,0xff,8))
 end
 for selector=0,7 do
  main:write_u16(0x2ffff0,selector)
  save(output..'/p2-selector-'..selector..'.bin',main:read_range(0x200000,0x2fffff,8))
 end
 for mode=0,1 do
  main:write_u8(mode==0 and 0x3a000b or 0x3a001b,0)
  save(output..'/audio-fixed-'..mode..'.bin',audio:read_range(0,0x7fff,8))
 end
 local windows={{port=8,base=0xf000,size=0x800},{port=9,base=0xe000,size=0x1000},{port=10,base=0xc000,size=0x2000},{port=11,base=0x8000,size=0x4000}}
 for _,w in ipairs(windows) do
  local f=assert(io_file.open(output..'/z80-port-'..w.port..'.bin','wb'))
  for selector=0,255 do
   io:read_u8((selector<<8)|w.port)
   f:write(audio:read_range(w.base,w.base+w.size-1,8))
  end
  f:close()
 end
 manager.machine:exit()
end,'frame')
'''
(out/'capture.lua').write_text(lua)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
with (out/'xvfb.log').open('w') as xlog:
 server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
 try:
  display=server.stdout.readline().strip();assert display.isdigit();env['DISPLAY']=':'+display
  cmd=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(out/'capture.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
  with (out/'mame.log').open('w') as log:subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=120)
 finally:server.terminate();server.wait(timeout=8)
assert 'LUA ERROR' not in (out/'mame.log').read_text()
for mode in (0,1):
 observed=(out/f'vectors-{mode}.bin').read_bytes();expected=((out/'bios-vectors.bin').read_bytes() if mode==0 else p1[:128])+p1[128:256]
 assert observed==expected,('vector mapping',mode)
bank_results=[]
for selector in range(8):
 bank=selector if selector<4 else 0;observed=(out/f'p2-selector-{selector}.bin').read_bytes();expected=p2[bank*0x100000:(bank+1)*0x100000]
 # Metal Slug X cartridge protection overrides READS at 2FFFE0..2FFFEF.
 assert observed[:0xfffe0]==expected[:0xfffe0] and observed[0xffff0:]==expected[0xffff0:],('P2 map',selector)
 bank_results.append(dict(selector=selector,physical_p2_bank=bank,source_offset=bank*0x100000,compared_bytes=0x100000-16,protection_overlay_excluded='0x2FFFE0..0x2FFFEF',observed_sha256=hashlib.sha256(observed).hexdigest()))
for mode,source in [(0,sm1),(1,m1)]:assert (out/f'audio-fixed-{mode}.bin').read_bytes()==source[:0x8000],('audio fixed source',mode)
audio_results=[]
for region,(port,base,size) in enumerate([(8,0xf000,0x800),(9,0xe000,0x1000),(10,0xc000,0x2000),(11,0x8000,0x4000)]):
 observed=(out/f'z80-port-{port}.bin').read_bytes();assert len(observed)==256*size
 for selector in range(256):
  source=(selector<<(11+region))&(len(m1)-1)
  assert observed[selector*size:(selector+1)*size]==m1[source:source+size],('Z80 bank',port,selector,hex(source))
 audio_results.append(dict(io_port=port,cpu_base=base,window_size=size,selector_count=256,unique_source_pages=len(m1)//size,source_formula=f'(selector << {11+region}) & 0x{len(m1)-1:X}',observed_sha256=hashlib.sha256(observed).hexdigest()))
report=dict(status='PASS_CONTROLLED_CPU_MAPPING_COMPARISON',m68k_p2_selectors=bank_results,z80_windows=audio_results,z80_selector_states_compared=1024,vector_overlay_bytes=128,vector_modes_compared=2,audio_fixed_bytes=0x8000,audio_fixed_modes_compared=2,audio_bios_sha1=hashlib.sha1(sm1).hexdigest(),inputs={n:hashlib.sha1(b).hexdigest() for n,b in raw.items()},limits='Controlled mapping writes/IO reads in one frame callback; original mapping device executes. Not natural gameplay, execution coverage, code/data classification, sound synthesis or full protection-device semantics. P2 protection reads 2FFFE0..2FFFEF excluded from ROM byte comparison.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS: 8 P2 selectors, 1024 Z80 selector/window states both vector modes and both audio-fixed modes')
