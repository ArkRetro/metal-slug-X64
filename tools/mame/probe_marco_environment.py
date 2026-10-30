#!/usr/bin/env python3
"""Controlled M68000 execution probe of original C31BE..C31DC arithmetic.

This changes scratch RAM/registers in an isolated MAME process. It is not a
natural gameplay capture and does not execute the external world helpers.
"""
import argparse,hashlib,json,os,random,re,subprocess
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for n in ('cfg','nvram'):(out/n).mkdir()
values=[0,1,0xffffffff,0x80000000,0x7fffffff,0x100000,0xc0000,0xffe80000]
cases=[(x,y) for x in values for y in values];rng=random.Random(110)
cases += [(rng.getrandbits(32),rng.getrandbits(32)) for _ in range(192)]
count=len(cases)
commands=[
 'temp0=0','sr=2700','a6=1014c4','a0=10a008','temp1=d@(a6+80)','temp2=d@(a6+34)',
 f'bpset c31dc,temp0<{count-1:x},{{printf "ENV_SAMPLE index=%d input80=%08X input_vy=%08X long80=%08X vy=%08X\\n",temp0,temp1,temp2,d@(a6+80),d@(a6+34);temp0++;d@(a6+80)=d@(a0);d@(a6+34)=d@(a0+4);temp1=d@(a6+80);temp2=d@(a6+34);a0=a0+8;pc=c31be;g}}',
 f'bpset c31dc,temp0=={count-1:x},{{printf "ENV_SAMPLE index=%d input80=%08X input_vy=%08X long80=%08X vy=%08X\\n",temp0,temp1,temp2,d@(a6+80),d@(a6+34);trace off,maincpu;pc=10ff00;g}}',
 f'trace {out}/instructions.log,maincpu,noloop', 'pc=c31be']
lines=['local frame=0',"local cpu=manager.machine.devices[':maincpu'].spaces.program",'emu.register_frame_done(function()','frame=frame+1','if frame==2 then']
for i,(x,y) in enumerate(cases):
 lines += [f'cpu:write_u32({0x10a000+i*8},{x})',f'cpu:write_u32({0x10a004+i*8},{y})']
lines += ['cpu:write_u16(0x10ff00,0x60fe)',f'cpu:write_u32(0x101544,{cases[0][0]})',f'cpu:write_u32(0x1014f8,{cases[0][1]})']
for cmd in commands:lines.append('manager.machine.debugger:command('+json.dumps(cmd)+')')
lines += ['end','if frame==4 then manager.machine:exit() end',"end,'frame')"]
(out/'capture.lua').write_text('\n'.join(lines)+'\n')
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
with (out/'xvfb.log').open('w') as xlog:
 server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
 try:
  display=server.stdout.readline().strip();assert display.isdigit();env['DISPLAY']=':'+display
  cmd=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(out/'capture.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
  with (out/'mame.log').open('w') as log:subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=60)
 finally:server.terminate();server.wait(timeout=8)
text=(out/'debug.log').read_text();assert 'Error in command' not in text,text[-1500:]
rows=re.findall(r'ENV_SAMPLE index=(\d+) input80=([0-9A-F]+) input_vy=([0-9A-F]+) long80=([0-9A-F]+) vy=([0-9A-F]+)',text)
assert len(rows)==count,(len(rows),count,text[-1000:])
fixture=[]
for index,(i,ix,iy,x,y) in enumerate(rows):
 assert int(i)==index and (int(ix,16),int(iy,16))==cases[index]
 fixture.append(dict(input80=cases[index][0],input_vy=cases[index][1],output80=int(x,16),output_vy=int(y,16)))
report=dict(status='CONTROLLED_M68000_PROBE',range='0x0C31BE..0x0C31DC',cases=fixture,trace_sha256=hashlib.sha256((out/'instructions.log').read_bytes()).hexdigest(),limits='Original arithmetic executed with injected scratch inputs, masked interrupts and intercepted helper boundary; not gameplay/world-service coverage.')
(out/'fixture.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS: controlled M68000 arithmetic probe',len(fixture))
