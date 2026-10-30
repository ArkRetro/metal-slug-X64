#!/usr/bin/env python3
"""Compare original condition handlers with addressed C, with helpers intercepted.

Executes 01A5CA..01A7F2 in MAME. Injected RAM/registers; 01903A/0AAA88 are
controlled service boundaries, not verified helpers or a gameplay capture.
"""
import argparse,ctypes as ct,hashlib,json,os,re,subprocess,sys,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2];sys.path.insert(0,str(ROOT/'tools'))
from condition_harness import Bus,Registers,make_cases,ram_input,run_c
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for name in ('cfg','nvram'):(out/name).mkdir()
with zipfile.ZipFile(a.rom_dir/'mslugx.zip') as z:raw=z.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest()==json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom=bytearray(raw);rom[::2]=raw[1::2];rom[1::2]=raw[::2]
# Every published canonical instruction byte must still match the provided ROM.
for line in (ROOT/'decomp/recovered/condition-dispatch/rom_01a5ca.asm').read_text().splitlines():
    match=re.match(r'^([0-9A-F]{6}): ((?:[0-9A-F]{4} )+)',line)
    if match:
        pc=int(match[1],16);data=bytes.fromhex(match[2]);assert rom[pc:pc+len(data)]==data,hex(pc)
cases=make_cases()
for case in cases:(out/f"input-{case['index']}.ram").write_bytes(ram_input(case))
names=[f'd{i}' for i in range(8)]+[f'a{i}' for i in range(7)]+['sp']
end='bpset 1a5dc,1,{printf "COND_END case=%d sr=%04X'+''.join(f' {n}=%08X' for n in names)+'",temp0,sr,'+','.join(names)+';pc=10ff00;g}'
commands=[end,'bpset 1a5ca,1,{printf "COND_BEGIN case=%d opcode=%02X relation=%02X threshold=%04X",temp0,b@(a1),b@(a1+1),w@(a1+2);g}',f'trace {out}/instructions.log,maincpu,noloop']
for address,kind in [('1903a',0),('aaa88',1)]:
    text=f'bpset {address},1,{{printf "COND_HELPER case=%d kind={kind} d0=%08X a0=%08X sp=%08X ret=%08X sr=%04X",temp0,d0,a0,sp,d@(sp),sr;temp7=d0;'
    text+=''.join(f'd{n}=d@({0x10e000+n*4:x});' for n in range(8))
    if kind:text+='d0=(temp7==0)*temp1+(temp7!=0)*temp2;'
    text+='a0=temp5;a2=temp6;sr=temp3;pc=d@(sp);sp=sp+4;g}'
    commands.append(text)
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
lua+='local cases={'+','.join('{sr='+str(c['sr'])+',hsr='+str(c['helper_sr'])+',a0='+str(c['helper_a0'])+',a2='+str(c['helper_a2'])+',q0='+str(c['query'][0])+',q1='+str(c['query'][1])+',regs={'+','.join(map(str,c['registers']))+'}}' for c in cases)+'}\n'
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
 manager.machine.debugger:command('pc=1a5ca')
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
ends=re.findall(r'COND_END case=(\d+) sr=([0-9A-F]+)((?: (?:[da][0-7]|sp)=[0-9A-F]+){16})',text)
begins=re.findall(r'COND_BEGIN case=(\d+) opcode=([0-9A-F]+) relation=([0-9A-F]+) threshold=([0-9A-F]+)',text)
assert len(ends)==len(cases)==len(begins),(len(ends),len(cases),len(begins))
calls={i:[] for i in range(len(cases))}
for match in re.finditer(r'COND_HELPER case=(\d+) kind=(\d+) d0=([0-9A-F]+) a0=([0-9A-F]+) sp=([0-9A-F]+) ret=([0-9A-F]+) sr=([0-9A-F]+)',text):
    case=int(match[1]);values=[int(v,16) for v in match.groups()[2:]]
    calls[case].append(dict(zip(('kind','d0','a0','sp','ret','sr'),[int(match[2])]+values)))
libpath=out/'conditions.so'
subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-shared','-fPIC',str(ROOT/'decomp/recovered/rom_01a5ca.c'),'-o',str(libpath)],check=True)
lib=ct.CDLL(str(libpath));lib.msx_rom_01a5ca.argtypes=[ct.POINTER(Registers),ct.POINTER(Bus)]
results=[]
for case,begin,end in zip(cases,begins,ends):
    i=case['index'];assert int(begin[0])==i==int(end[0])
    assert [int(v,16) for v in begin[1:]]==[case['opcode'],case['relation'],case['threshold']]
    expected=dict(status=0,registers=[int(v,16) for v in re.findall(r'=([0-9A-F]+)',end[2])],sr=int(end[1],16),ram_sha256=hashlib.sha256((out/f'result-{i}.ram').read_bytes()).hexdigest(),calls=calls[i])
    actual,ram=run_c(lib,case)
    for key in expected:assert actual[key]==expected[key],(i,case['opcode'],key,actual[key],expected[key])
    assert ram==(out/f'result-{i}.ram').read_bytes(),(i,'full RAM')
    results.append(dict(input=case,expected=expected))
report=dict(status='PASS_CONTROLLED_M68000_HANDLERS_WITH_INTERCEPTED_HELPERS',entry='0x01A5CA',stop_before_rts='0x01A5DC',cases=len(cases),opcodes=list(range(22)),results=results,source_sha256=hashlib.sha256((ROOT/'decomp/recovered/rom_01a5ca.c').read_bytes()).hexdigest(),rom_cpu_order_sha256=hashlib.sha256(rom).hexdigest(),trace_sha256=hashlib.sha256((out/'instructions.log').read_bytes()).hexdigest(),limits='Injected RAM/registers and masked IRQ. Original dispatch/handlers execute; 01903A/0AAA88 intercepted with synthetic outputs. No helper semantic, runtime valid-index-domain, cycle timing, complete script interpreter or campaign/native N64 backend validation. Stops before outer RTS.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n');print('PASS',len(cases),'original condition calls: RAM, registers, SR and helper boundaries')
