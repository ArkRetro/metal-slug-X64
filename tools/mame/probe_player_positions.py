#!/usr/bin/env python3
"""Execute original 01903A with real 01964A/0040AC helpers and compare native C."""
import argparse,ctypes as ct,hashlib,json,os,re,subprocess,sys,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools'))
from player_positions_harness import Frame,Registers,make_cases,ram_input,run_c
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for n in ('cfg','nvram'):(out/n).mkdir()
with zipfile.ZipFile(a.rom_dir/'mslugx.zip') as z:raw=z.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest()==json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom=bytearray(raw);rom[::2]=raw[1::2];rom[1::2]=raw[::2]
cases=make_cases()
for c in cases:(out/f"input-{c['index']}.ram").write_bytes(ram_input(c))
names=[f'd{i}' for i in range(8)]+[f'a{i}' for i in range(7)]+['sp']
commands=[]
for address,status in [('19052',0),('190ba',0)]:
    commands.append('bpset '+address+',1,{printf "POSITIONS_END case=%d status='+str(status)+' sr=%04X'+''.join(f' {n}=%08X' for n in names)+'",temp0,sr,'+','.join(names)+';pc=10ff00;g}')
commands+=['bpset 1903a,1,{printf "POSITIONS_BEGIN case=%d selector=%08X",temp0,d0;g}',f'trace {out}/instructions.log,maincpu,noloop']
lua=r'''local frame,index=0,0
local cpu=manager.machine.devices[':maincpu'].spaces.program
local function load(path)
 local f=assert(io.open(path,'rb'));local data=f:read('*a');f:close()
 for i=1,#data,2 do cpu:write_u16(0x100000+i-1,string.unpack('>I2',data,i)) end
end
'''
lua+='local output='+json.dumps(str(out))+'\n'
lua+='local cases={'+','.join('{sr='+str(c['sr'])+',regs={'+','.join(map(str,c['registers']))+'}}' for c in cases)+'}\n'
lua+=r'''emu.register_frame_done(function()
 frame=frame+1
 if frame<2 then return end
 if index==#cases then manager.machine:exit();return end
 local c=cases[index+1]
 cpu:write_u8(0x300001,0) -- harness watchdog keepalive, outside tested call
 load(output..'/input-'..index..'.ram')
'''
lua+='if index==0 then\n'+'\n'.join('manager.machine.debugger:command('+json.dumps(s)+')' for s in commands)+'\nend\n'
lua+=r'''manager.machine.debugger:command('temp0='..string.format('0x%x',index))
 for i=1,16 do local name=(i<=8 and 'd'..(i-1) or (i==16 and 'sp' or 'a'..(i-9)));manager.machine.debugger:command(name..'='..string.format('0x%x',c.regs[i])) end
 manager.machine.debugger:command('sr='..string.format('0x%x',c.sr))
 manager.machine.debugger:command('pc=1903a')
 manager.machine.debugger:command('g')
 index=index+1
end,'frame')
'''
(out/'capture.lua').write_text(lua)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
with (out/'xvfb.log').open('w') as xlog:
    server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
    try:
        display=server.stdout.readline().strip();assert display.isdigit();env['DISPLAY']=':'+display
        cmd=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(out/'capture.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
        with (out/'mame.log').open('w') as log:subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=90)
    finally:server.terminate();server.wait(timeout=8)
text=(out/'debug.log').read_text();mlog=(out/'mame.log').read_text()
assert 'error in ' not in text.lower() and 'LUA ERROR' not in mlog,(text[-2000:],mlog[-2000:])
ends=re.findall(r'POSITIONS_END case=(\d+) status=(\d+) sr=([0-9A-F]+)((?: (?:[da][0-7]|sp)=[0-9A-F]+){16})',text)
begins=re.findall(r'POSITIONS_BEGIN case=(\d+) selector=([0-9A-F]+)',text)
assert len(ends)==len(cases)==len(begins),(len(ends),len(cases),len(begins))
libpath=out/'query.so';subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-shared','-fPIC',str(ROOT/'src/neo_player_positions.c'),'-o',str(libpath)],check=True)
lib=ct.CDLL(str(libpath));lib.neo_player_positions.argtypes=[ct.POINTER(Frame),ct.POINTER(Registers)]
results=[]
for c,begin,end in zip(cases,begins,ends):
    i=c['index'];assert int(begin[0])==i==int(end[0]);assert int(begin[1],16)==c['registers'][0]
    expected=dict(status=int(end[1]),registers=[int(v,16) for v in re.findall(r'=([0-9A-F]+)',end[3])],sr=int(end[2],16))
    actual=run_c(lib,c)
    assert actual==expected,(i,actual,expected)
    results.append(dict(input=c,expected=expected))
report=dict(status='PASS_ORIGINAL_PLAYER_POSITIONS_WITH_REAL_HELPERS',entry='0x01903A',helpers=['0x01964A','0x0040AC'],cases=len(cases),results=results,source_sha256=hashlib.sha256((ROOT/'src/neo_player_positions.c').read_bytes()).hexdigest(),rom_cpu_order_sha256=hashlib.sha256(rom).hexdigest(),trace_sha256=hashlib.sha256((out/'instructions.log').read_bytes()).hexdigest(),limits='Injected RAM/registers and masked IRQ; original query and object guard execute without helper stubs. Stops before outer RTS; no stack residue/timing or real campaign-data producer/integration validation.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(cases),'original position queries including real enable and object guards')
