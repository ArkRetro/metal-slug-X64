#!/usr/bin/env python3
"""Compare original condition dispatch with all player helpers executed, not stubbed."""
import argparse,ctypes as ct,hashlib,json,os,re,subprocess,sys,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools'))
from condition_consumers_harness import Bus,Registers,Result,make_cases,ram_input,run_c
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for name in ('cfg','nvram'):(out/name).mkdir()
with zipfile.ZipFile(a.rom_dir/'mslugx.zip') as z:raw=z.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest()==json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom=bytearray(raw);rom[::2]=raw[1::2];rom[1::2]=raw[::2]
# Every published canonical instruction byte must still match the provided ROM.
assembly_files=[ROOT/'decomp/recovered/condition-dispatch/rom_01a5ca.asm']+list((ROOT/'decomp/recovered/condition-consumers').glob('*.asm'))
for line in '\n'.join(path.read_text() for path in assembly_files).splitlines():
    match=re.match(r'^([0-9A-F]{6}): ((?:[0-9A-F]{4} )+)',line)
    if match:
        pc=int(match[1],16);data=bytes.fromhex(match[2]);assert rom[pc:pc+len(data)]==data,hex(pc)
cases=make_cases()
for case in cases:(out/f"input-{case['index']}.ram").write_bytes(ram_input(case))
names=[f'd{i}' for i in range(8)]+[f'a{i}' for i in range(7)]+['sp']
commands=[]
for target in [0x1a02a,0x1a056,0x1a0a6,0x1a372,0x4188,0x1a510,0x1a526,0xaaacc]:
    status=2 if target==0xaaacc else 0
    commands.append(f'bpset {target:x},1,{{printf "CONS_END case=%d target={target:08X} debug_pc=%08X status={status} sr=%04X'+''.join(f' {n}=%08X' for n in names)+'",temp0,pc,sr,'+','.join(names)+';pc=10ff00;g}')
for entry in [0x1a01e,0x1a2d8,0x1a4f2]:
    commands.append(f'bpset {entry:x},1,{{printf "CONS_BEGIN case=%d entry={entry:08X} debug_pc=%08X script=%08X timer=%04X",temp0,pc,a0,w@(a6+72);g}}')
commands.append(f'trace {out}/instructions.log,maincpu,noloop')
lua=r'''local frame,index=0,0
local cpu=manager.machine.devices[':maincpu'].spaces.program
local function load(path)
 local f=assert(io.open(path,'rb'));local data=f:read('*a');f:close()
 for i=1,#data,2 do cpu:write_u16(0x100000+i-1,string.unpack('>I2',data,i)) end
end
local function save(path)
 local f=assert(io.open(path,'wb'))
 for i=0,32767 do f:write(string.pack('>I2',cpu:read_u16(0x100000+i*2))) end
 f:close()
end
'''
lua+='local output='+json.dumps(str(out))+'\n'
lua+='local cases={'+','.join('{entry='+str(c['entry_pc'])+',sr='+str(c['sr'])+',hsr='+str(c['helper_sr'])+',a0='+str(c['helper_a0'])+',a2='+str(c['helper_a2'])+',q0='+str(c['query'][0])+',q1='+str(c['query'][1])+',regs={'+','.join(map(str,c['registers']))+'}}' for c in cases)+'}\n'
lua+=r'''emu.register_frame_done(function()
 frame=frame+1
 if frame<2 then return end
 if index>0 then save(output..'/result-'..(index-1)..'.ram') end
 if index==#cases then manager.machine:exit();return end
 local c=cases[index+1]
 cpu:write_u8(0x300001,0) -- harness keepalive, outside the tested routine
 load(output..'/input-'..index..'.ram')
'''
lua+='if index==0 then\n'+'\n'.join('manager.machine.debugger:command('+json.dumps(s)+')' for s in commands)+'\nend\n'
lua+=r'''local temps={index,c.q0,c.q1,c.hsr,0,c.a0,c.a2}
 for i=1,#temps do manager.machine.debugger:command('temp'..(i-1)..'='..string.format('0x%x',temps[i])) end
 for i=1,16 do local name=(i<=8 and 'd'..(i-1) or (i==16 and 'sp' or 'a'..(i-9)));manager.machine.debugger:command(name..'='..string.format('0x%x',c.regs[i])) end
 manager.machine.debugger:command('sr='..string.format('0x%x',c.sr))
 manager.machine.debugger:command('pc='..string.format('0x%x',c.entry))
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
        with (out/'mame.log').open('w') as log:subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=180)
    finally:server.terminate();server.wait(timeout=8)
text=(out/'debug.log').read_text();mlog=(out/'mame.log').read_text()
assert 'error in ' not in text.lower() and 'LUA ERROR' not in mlog,(text[-2000:],mlog[-2000:])
ends=re.findall(r'CONS_END case=(\d+) target=([0-9A-F]+) debug_pc=[0-9A-F]+ status=(\d+) sr=([0-9A-F]+)((?: (?:[da][0-7]|sp)=[0-9A-F]+){16})',text)
begins=re.findall(r'CONS_BEGIN case=(\d+) entry=([0-9A-F]+) debug_pc=[0-9A-F]+ script=([0-9A-F]+) timer=([0-9A-F]+)',text)
assert len(ends)==len(cases)==len(begins),(len(ends),len(cases),len(begins))
libpath=out/'conditions.so'
subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-shared','-fPIC',str(ROOT/'decomp/recovered/rom_01a5ca.c'),str(ROOT/'decomp/recovered/condition_services.c'),str(ROOT/'decomp/recovered/condition_consumers.c'),str(ROOT/'src/neo_player_query.c'),str(ROOT/'src/neo_player_positions.c'),'-o',str(libpath)],check=True)
lib=ct.CDLL(str(libpath));lib.msx_condition_consume.argtypes=[ct.c_int,ct.POINTER(Registers),ct.POINTER(Bus)];lib.msx_condition_consume.restype=Result
results=[]
for case,begin,end in zip(cases,begins,ends):
    i=case['index'];assert int(begin[0])==i==int(end[0])
    assert [int(v,16) for v in begin[1:]]==[case['entry_pc'],case['registers'][8],case['timer']]
    expected=dict(status=int(end[2]),target=int(end[1],16),registers=[int(v,16) for v in re.findall(r'=([0-9A-F]+)',end[4])],sr=int(end[3],16),ram_sha256=hashlib.sha256((out/f'result-{i}.ram').read_bytes()).hexdigest())
    actual,ram=run_c(lib,case)
    for key in expected:assert actual[key]==expected[key],(i,case['opcode'],key,actual[key],expected[key])
    assert ram==(out/f'result-{i}.ram').read_bytes(),(i,'full RAM')
    results.append(dict(input=case,expected=expected))
report=dict(status='PASS_ORIGINAL_CONDITION_CONSUMERS_WITH_REAL_SERVICES',entries=['0x01A01E','0x01A2D8','0x01A4F2'],stop_boundaries=['0x01A02A','0x01A056','0x01A0A6','0x01A372','0x004188','0x01A510','0x01A526','0x0AAACC'],cases=len(cases),opcodes=list(range(22)),results=results,source_sha256={name:hashlib.sha256((ROOT/name).read_bytes()).hexdigest() for name in ['decomp/recovered/condition_consumers.c','decomp/recovered/rom_01a5ca.c','decomp/recovered/condition_services.c','src/neo_player_query.c','src/neo_player_positions.c']},rom_cpu_order_sha256=hashlib.sha256(rom).hexdigest(),trace_sha256=hashlib.sha256((out/'instructions.log').read_bytes()).hexdigest(),limits='Injected RAM/registers and masked IRQ. Original dispatch, handlers and 01903A/0AAA88/01964A/0040AC execute without synthetic helper outputs. No runtime valid-index-domain, bus cycle timing, remaining event interpreter or campaign data producer validation. Stops before the returned continuation/RTS/trap boundary.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(cases),'original consumer transitions with real services: full RAM, registers, SR and destination')
