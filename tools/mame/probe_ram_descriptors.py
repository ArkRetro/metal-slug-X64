#!/usr/bin/env python3
"""Original RAM descriptor helpers vs structured C; controlled states, real bindings.

Initial RAM/registers are injected. Pipeline cases inherit the preceding
result without reinjection. Unknown callbacks/returns stop at named boundaries.
"""
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import zipfile
from pathlib import Path

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT/'tools'))
from descriptor_harness import base_ram,fnv32,load_library,make_cases,ram_input,run_c

parser=argparse.ArgumentParser(description=__doc__)
parser.add_argument('--rom-dir',type=Path,required=True)
parser.add_argument('--output',type=Path,required=True)
parser.add_argument('--reuse-capture',action='store_true')
args=parser.parse_args()
out=args.output.resolve()
with zipfile.ZipFile(args.rom_dir/'mslugx.zip') as archive:raw=archive.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest()==json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom=bytearray(raw);rom[::2],rom[1::2]=raw[1::2],raw[::2]
code=bytes(rom[0x1158:0x22a2])
layout=json.loads((ROOT/'decomp/canonical-model/layout.json').read_text())
assert hashlib.sha256(code).hexdigest()==next(c['sha256'] for c in layout['contexts'] if c['id']=='RAM_COPY_100000')
cases=make_cases()
if args.reuse_capture:
    assert json.loads((out/'cases.json').read_text())==cases
    assert json.loads((out/'finished.json').read_text())['cases']==len(cases)
else:
    out.mkdir(parents=True,exist_ok=False)
    for name in ('cfg','nvram'):(out/name).mkdir()
    (out/'cases.json').write_text(json.dumps(cases,indent=2)+'\n')
    for case in cases:
        if case['chain_from']<0:(out/f'input-{case["index"]}.ram').write_bytes(ram_input(case,code))
    def lua_value(value):
        if isinstance(value,dict):return '{'+','.join(k+'='+lua_value(v) for k,v in value.items())+'}'
        if isinstance(value,list):return '{'+','.join(map(lua_value,value))+'}'
        if isinstance(value,str):return json.dumps(value)
        return str(value)
    names=[f'd{i}' for i in range(8)]+[f'a{i}' for i in range(7)]+['sp']
    commands=[]
    for site,condition,status,target in (
        (0x100834,'temp1==0',0,'0'),(0x1008d6,'temp1==0',0,'0'),
        (0x100914,'temp1==1',0,'0'),(0x10082c,'temp1==2',0,'0'),
        (0x10081a,'temp1==2 && (a0&0xffffff)!=0x10008c && (a0&0xffffff)!=0x100158',1,'a0'),
        (0x1000a2,'temp1==2 && d@(sp)!=0x10081c',2,'d@(sp)'),
        (0x100158,'temp1==2 && d@(sp)!=0x10081c',2,'d@(sp)'),
    ):
        action=f'printf "DESC_END case=%d status={status} stop={site:08X} target=%08X sr=%04X'+''.join(f' {n}=%08X' for n in names)+'",temp0,'+target+',sr,'+','.join(names)+';trace off,maincpu;pc=0x10ff00;g'
        commands.append(f'bpset 0x{site:x}:maincpu,{condition},{{{action}}}')
    lua='local output='+json.dumps(str(out))+'\nlocal cases='+lua_value(cases)+'\n'
    lua+=r'''
local cpu=manager.machine.devices[':maincpu'].spaces.program
local device=manager.machine.devices[':maincpu']
local dbg=assert(manager.machine.debugger)
local frame,index,running,active,started=0,0,false,false,0
local writes
local ram_tap=cpu:install_write_tap(0x100000,0x10ffff,'descriptor_writes',function(address,data,mask)
 if not active then return end
 if mask==0xffff then writes:write(string.pack('>BI4I2',1,address,data))
 elseif mask==0xff00 then writes:write(string.pack('>BI4I2',0,address,data>>8))
 elseif mask==0x00ff then writes:write(string.pack('>BI4I2',0,address+1,data&255))
 else error('Unexpected write mask') end
end)
local function load(path)
 local f=assert(io.open(path,'rb'));local data=f:read('*a');f:close()
 for i=1,#data,2 do cpu:write_u16(0x100000+i-1,string.unpack('>I2',data,i)) end
end
local function save(path)
 local f=assert(io.open(path,'wb'));f:write(cpu:read_range(0x100000,0x10ffff,8));f:close()
end
'''
    lua+='\n'.join('dbg:command('+json.dumps(command)+')' for command in commands)+'\n'
    lua+=r'''
emu.register_frame_done(function()
 assert(ram_tap~=nil) -- Retain the userdata while emulation runs.
 frame=frame+1
 if frame<2 then return end
 cpu:write_u8(0x300001,0) -- Harness watchdog keepalive, outside work RAM.
 if running then
  local pc=device.state['PC'].value&0xffffff
  assert(frame-started<30,'Descriptor walk did not reach its boundary')
  if pc~=0x10ff00 and pc~=0x10ff02 then return end
  active=false;writes:close();save(output..'/result-'..(index-1)..'.ram');running=false
 end
 if index==#cases then
  local f=assert(io.open(output..'/finished.json','w'));f:write('{"cases":'..index..'}');f:close()
  manager.machine:exit();return
 end
 local c=cases[index+1]
 if c.chain_from<0 then
  load(output..'/input-'..index..'.ram')
  for i=1,16 do
   local name=i<=8 and 'd'..(i-1) or (i==16 and 'sp' or 'a'..(i-9))
   dbg:command(name..'='..string.format('0x%x',c.registers[i]))
  end
  dbg:command('sr='..string.format('0x%x',c.sr))
 else
  assert(c.chain_from==index-1)
  save(output..'/input-'..index..'.ram') -- Observe inherited RAM; do not load it.
 end
 dbg:command(string.format('temp0=0x%x;temp1=0x%x',index,c.mode))
 writes=assert(io.open(output..'/result-'..index..'.writes','wb'))
 if index==0 or c.chain_from>=0 then dbg:command('trace "'..output..'/case-'..index..'.trace",maincpu,noloop') end
 active=true;running=true;started=frame
 local entry=({0x10082e,0x1008d8,0x10080e})[c.mode+1]
 dbg:command('pc='..string.format('0x%x',entry));dbg:command('g');index=index+1
end,'frame')
'''
    (out/'capture.lua').write_text(lua)
    env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
    with (out/'xvfb.log').open('w') as xlog:
        server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
        try:
            number=server.stdout.readline().strip();assert number.isdigit();env['DISPLAY']=':'+number
            command=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(args.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(out/'capture.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
            with (out/'mame.log').open('w') as log:subprocess.run(command,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=240)
        finally:server.terminate();server.wait(timeout=8)
log=(out/'debug.log').read_text()
assert 'error in ' not in log.lower() and 'LUA ERROR' not in (out/'mame.log').read_text()
ends=re.findall(r'DESC_END case=(\d+) status=(\d+) stop=([0-9A-F]+) target=([0-9A-F]+) sr=([0-9A-F]+)((?: (?:[da][0-7]|sp)=[0-9A-F]+){16})',log)
assert len(ends)==len(cases),(len(ends),len(cases))
library=out/'descriptors.so'
subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-shared','-fPIC',str(ROOT/'decomp/recovered/ram_descriptors.c'),'-o',str(library)],check=True)
lib=load_library(library)
results=[];previous_state=None;previous_ram=None;normalized_prefix=None
for original,end in zip(cases,ends):
    case=dict(original);index=case['index'];assert int(end[0])==index
    initial=(out/f'input-{index}.ram').read_bytes()
    if case['chain_from']>=0:
        assert initial==previous_ram
        case['registers']=previous_state['registers'];case['sr']=previous_state['sr']
    else:
        normalized_prefix=base_ram(index)[:len(code)]
    expected=dict(registers=[int(v,16) for v in re.findall(r'=([0-9A-F]+)',end[5])],
                  sr=int(end[4],16),status=int(end[1]),target=int(end[3],16),
                  boundary=int(end[2],16) if case['mode']==2 else 0)
    actual,ram,writes=run_c(lib,case,initial)
    assert actual==expected,(case['name'],'registers/result',actual,expected)
    reference=(out/f'result-{index}.ram').read_bytes()
    assert ram==reference,(case['name'],'full RAM')
    reference_writes=(out/f'result-{index}.writes').read_bytes()
    assert writes==reference_writes,(case['name'],'ordered writes',writes.hex(),reference_writes.hex())
    assert reference[:len(code)]==code,'Code image changed during experiment'
    output_patches=[[i,b] for i,(a,b) in enumerate(zip(initial,reference)) if a!=b]
    assert all(offset>=len(code) for offset,value in output_patches)
    normalized_input=bytearray(initial);normalized_input[:len(code)]=normalized_prefix
    normalized_output=bytearray(reference);normalized_output[:len(code)]=normalized_prefix
    input_patches=[] if case['chain_from']>=0 else [[i,b] for i,(a,b) in enumerate(zip(base_ram(index),ram_input(case))) if a!=b]
    results.append(dict(input=case,expected=expected,stop_site='0x'+end[2],
                        input_patches=input_patches,output_patches=output_patches,
                        original_input_ram_sha256=hashlib.sha256(initial).hexdigest(),
                        original_output_ram_sha256=hashlib.sha256(reference).hexdigest(),
                        normalized_input_ram_sha256=hashlib.sha256(normalized_input).hexdigest(),
                        normalized_output_ram_sha256=hashlib.sha256(normalized_output).hexdigest(),
                        write_events=len(writes)//7,write_fnv32=fnv32(writes),
                        write_sha256=hashlib.sha256(writes).hexdigest()))
    previous_state=actual;previous_ram=bytes(ram)
report=dict(status='PASS_ORIGINAL_RAM_DESCRIPTOR_HELPERS_WITH_NATIVE_CALLBACK_SUBSET',
            cases=len(cases),source_sha256={n:hashlib.sha256((ROOT/n).read_bytes()).hexdigest() for n in ('decomp/recovered/ram_descriptors.c','decomp/recovered/ram_descriptors.h')},
            code_sha256=hashlib.sha256(code).hexdigest(),observer_sha256=hashlib.sha256((out/'capture.lua').read_bytes()).hexdigest(),
            debugger_log_sha256=hashlib.sha256((out/'debug.log').read_bytes()).hexdigest(),
            results=results,limits='Controlled RAM/register inputs, masked IRQ, pure work-RAM imports. Original allocator/remover/dispatcher and callbacks 10008C/100158 execute unchanged. Unknown callbacks stop before JSR; changed return links stop before callback RTS. Final pipeline inherits RAM and registers without reinjection. Full RAM/registers/SR and ordered writes compared. No bus-read/cycle timing, malformed nonterminating lists, outer RTS pop, complete callback domain, natural gameplay or campaign binding claim.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS',len(cases),'original descriptor transitions: registers/SR, full RAM, ordered writes and inherited pipeline')
