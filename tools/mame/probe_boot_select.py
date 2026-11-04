#!/usr/bin/env python3
"""Compare original RAM:10FC00 with C under explicit synthetic platform imports.

The original bytes execute in MAME. Header/NV reads are supplied by taps and
slot writes are routed to physical slot zero. This is CPU-logic evidence,
not real multi-slot hardware validation or natural gameplay coverage.
"""
import argparse
import ctypes as ct
import hashlib
import json
import os
import re
import subprocess
import sys
import zipfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from boot_select_harness import Imports, Registers, fnv32, make_cases, nv_input, run_c

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('--rom-dir', type=Path, required=True)
parser.add_argument('--output', type=Path, required=True)
args = parser.parse_args()
out = args.output.resolve()
out.mkdir(parents=True, exist_ok=False)
for name in ('cfg', 'nvram'):
    (out / name).mkdir()
with zipfile.ZipFile(args.rom_dir / 'mslugx.zip') as archive:
    raw = archive.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest() == json.loads((ROOT / 'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom = bytearray(raw)
rom[::2], rom[1::2] = raw[1::2], raw[::2]
code = bytes(rom[0x10ee:0x1158])
layout = json.loads((ROOT / 'decomp/canonical-model/layout.json').read_text())
context = next(c for c in layout['contexts'] if c['id'] == 'RAM_COPY_10FC00')
assert len(code) == 106 and hashlib.sha256(code).hexdigest() == context['sha256']
cases = make_cases()
for c in cases:
    ram = bytearray((i * 37 + (c['index'] + 1) * 19 + (i >> 5) * 7) & 255 for i in range(65536))
    ram[0xfc00:0xfc6a] = code
    ram[0xff00:0xff02] = bytes.fromhex('60fe')
    (out / f'input-{c["index"]}.ram').write_bytes(ram)
    (out / f'input-{c["index"]}.nv').write_bytes(nv_input(c))


def lua_value(value):
    if isinstance(value, dict):
        return '{' + ','.join(str(k) + '=' + lua_value(v) for k, v in value.items()) + '}'
    if isinstance(value, list):
        return '{' + ','.join(map(lua_value, value)) + '}'
    if isinstance(value, str):
        return json.dumps(value)
    return str(value)


names = [f'd{i}' for i in range(8)] + [f'a{i}' for i in range(7)] + ['sp']
end = 'bpset 0x10fc68:maincpu,1,{printf "BOOT_END case=%d sr=%04X' + ''.join(f' {n}=%08X' for n in names) + '",temp0,sr,' + ','.join(names) + ';trace off,maincpu;pc=0x10ff00;g}'
lua = 'local output=' + json.dumps(str(out)) + '\nlocal cases=' + lua_value(cases) + '\n'
lua += r'''
local frame,index,running,started=0,0,false,0
local cpu=manager.machine.devices[':maincpu'].spaces.program
local device=manager.machine.devices[':maincpu']
local dbg=assert(manager.machine.debugger)
local active,selected,current,events=false,0,nil,nil
local products={}
local function emit(op,address,value) events:write(string.pack('>BI4I2',op,address,value)) end
local function read_event(address,value,mask)
 if mask==0xffff then emit(1,address,value)
 elseif mask==0xff00 then emit(0,address,value>>8)
 elseif mask==0x00ff then emit(0,address+1,value&255)
 else error('Unexpected read mask') end
end
local slots={}
local header_tap=cpu:install_read_tap(0x100,0x115,'boot_headers',function(address,data,mask)
 if not active then return end
 local h=slots[selected];local value
 if address==0x100 then value=h.magic>>16
 elseif address==0x102 then value=h.magic&0xffff
 elseif address==0x108 then
  value=(products[selected] or 0)==0 and h.product or h.product_later
  products[selected]=(products[selected] or 0)+1
 elseif address==0x114 then value=h.flag<<8
 else error('Unexpected header read') end
 read_event(address,value,mask);return (data&(~mask&0xffff))|(value&mask)
end)
local nv_tap=cpu:install_read_tap(0xd00046,0xd00109,'boot_nv_reads',function(address,data,mask)
 if not active then return end
 local value
 if address==0xd00046 and mask==0x00ff then value=current.count
 elseif address==0xd00108 and mask==0xff00 then value=current.previous_slot<<8
 else error('Unexpected NV read') end
 read_event(address,value,mask);return (data&(~mask&0xffff))|(value&mask)
end)
local function write_event(address,data,mask)
 if mask==0x00ff then emit(2,address+1,data&255)
 elseif mask==0xff00 then emit(2,address,(data>>8)&255)
 else error('Unexpected write mask') end
end
local control_tap=cpu:install_write_tap(0x3a0000,0x3a001f,'boot_controls',function(address,data,mask)
 if active then write_event(address,data,mask) end
end)
local slot_tap=cpu:install_write_tap(0x380020,0x380021,'boot_slot_import',function(address,data,mask)
 if not active then return end
 assert(mask==0x00ff);write_event(address,data,mask);selected=data&255
 return 0 -- Explicit fixture: keep MAME's physical slot safe; headers follow logical selection.
end)
local nv_write_tap=cpu:install_write_tap(0xd00058,0xd00059,'boot_nv_write',function(address,data,mask)
 if active then write_event(address,data,mask) end
end)
local retained_taps={header_tap,nv_tap,control_tap,slot_tap,nv_write_tap}
local function load(path,base)
 local f=assert(io.open(path,'rb'));local data=f:read('*a');f:close()
 for i=1,#data,2 do cpu:write_u16(base+i-1,string.unpack('>I2',data,i)) end
end
local function save(path,base)
 local f=assert(io.open(path,'wb'));f:write(cpu:read_range(base,base+65535,8));f:close()
end
'''
lua += 'dbg:command(' + json.dumps(end) + ')\n'
lua += r'''
emu.register_frame_done(function()
 assert(#retained_taps==5) -- Closure ownership prevents Lua GC removing imports.
 frame=frame+1
 if frame<2 then return end
 cpu:write_u8(0x300001,0) -- Harness watchdog maintenance, outside recorded imports.
 if running then
  local pc=device.state['PC'].value&0xffffff
  assert(frame-started<120,'Routine did not reach its return boundary')
  if pc~=0x10ff00 and pc~=0x10ff02 then return end
  active=false;events:close()
  save(output..'/result-'..(index-1)..'.ram',0x100000)
  save(output..'/result-'..(index-1)..'.nv',0xd00000)
  running=false
 end
 if index==#cases then manager.machine:exit();return end
 current=cases[index+1];selected=0;products={};slots={}
 for i=0,255 do slots[i]=current.default end
 for _,h in ipairs(current.overrides) do slots[h.slot]=h end
 load(output..'/input-'..index..'.ram',0x100000)
 cpu:write_u8(0x3a001d,0) -- Harness unlock before loading synthetic NV memory.
 load(output..'/input-'..index..'.nv',0xd00000)
 events=assert(io.open(output..'/result-'..index..'.io','wb'))
 dbg:command(string.format('temp0=0x%x',index))
 for i=1,16 do
  local name=i<=8 and 'd'..(i-1) or (i==16 and 'sp' or 'a'..(i-9))
  dbg:command(name..'='..string.format('0x%x',current.registers[i]))
 end
 dbg:command('sr='..string.format('0x%x',current.sr))
 if index==0 or index==20 or index==30 then
  dbg:command('trace "'..output..'/case-'..index..'.trace",maincpu,noloop')
 end
 active=true;running=true;started=frame
 dbg:command('pc=0x10fc00');dbg:command('g')
 index=index+1
end,'frame')
'''
(out / 'capture.lua').write_text(lua)
env = dict(os.environ, LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
with (out / 'xvfb.log').open('w') as xlog:
    server = subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb', '-displayfd', '1', '-screen', '0', '1024x768x24', '-ac', '-nolisten', 'tcp'], stdout=subprocess.PIPE, stderr=xlog, text=True, env=env)
    try:
        number = server.stdout.readline().strip()
        assert number.isdigit()
        env['DISPLAY'] = ':' + number
        command = ['/workspace/emulator/root/usr/games/mame', 'mslugx', '-rompath', str(args.rom_dir.resolve()), '-bios', 'euro', '-video', 'soft', '-sound', 'none', '-window', '-skip_gameinfo', '-nothrottle', '-debug', '-debugger', 'none', '-debuglog', '-debugscript', str(ROOT / 'tools/mame/start.cmd'), '-autoboot_delay', '0', '-autoboot_script', str(out / 'capture.lua'), '-cfg_directory', str(out / 'cfg'), '-nvram_directory', str(out / 'nvram')]
        with (out / 'mame.log').open('w') as log:
            subprocess.run(command, cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT, check=True, timeout=240)
    finally:
        server.terminate()
        server.wait(timeout=8)
log = (out / 'debug.log').read_text()
assert 'error in ' not in log.lower() and 'LUA ERROR' not in (out / 'mame.log').read_text()
ends = re.findall(r'BOOT_END case=(\d+) sr=([0-9A-F]+)((?: (?:[da][0-7]|sp)=[0-9A-F]+){16})', log)
assert len(ends) == len(cases), (len(ends), len(cases))
library = out / 'boot-select.so'
subprocess.run(['cc', '-std=c11', '-Wall', '-Wextra', '-Werror', '-shared', '-fPIC', str(ROOT / 'decomp/recovered/ram_10fc00.c'), '-o', str(library)], check=True)
lib = ct.CDLL(str(library))
lib.msx_boot_select.argtypes = [ct.POINTER(Registers), ct.POINTER(Imports)]
results = []
for case, end in zip(cases, ends):
    index = case['index']
    assert int(end[0]) == index
    expected = dict(registers=[int(v, 16) for v in re.findall(r'=([0-9A-F]+)', end[2])], sr=int(end[1], 16))
    actual, io, nv = run_c(lib, case)
    assert actual == expected, (case['name'], actual, expected)
    assert io == (out / f'result-{index}.io').read_bytes(), (case['name'], 'import sequence')
    assert nv == (out / f'result-{index}.nv').read_bytes(), (case['name'], 'full NV memory')
    assert (out / f'result-{index}.ram').read_bytes() == (out / f'input-{index}.ram').read_bytes(), (case['name'], 'work RAM/code mutated')
    results.append(dict(input=case, expected=expected, io_events=len(io)//7,
                        io_fnv32=fnv32(io), io_sha256=hashlib.sha256(io).hexdigest(),
                        nv_sha256=hashlib.sha256(nv).hexdigest(),
                        nv_result=(out / f'result-{index}.nv').read_bytes()[0x58]))
report = dict(status='PASS_ORIGINAL_BOOT_SELECTOR_WITH_SYNTHETIC_PLATFORM_IMPORTS',
              entry='RAM_COPY_10FC00:0010fc00', stop_before_rts='0x10FC68', cases=len(cases),
              code_sha256=hashlib.sha256(code).hexdigest(),
              source_sha256={n:hashlib.sha256((ROOT/n).read_bytes()).hexdigest() for n in ('decomp/recovered/ram_10fc00.c','decomp/recovered/ram_10fc00.h')},
              observer_sha256=hashlib.sha256((out/'capture.lua').read_bytes()).hexdigest(),
              debugger_log_sha256=hashlib.sha256((out/'debug.log').read_bytes()).hexdigest(),
              results=results,
              limits='Injected work RAM/NV/registers and masked IRQ. Header/NV read taps supply fixture imports; slot writes are recorded then forced to physical slot zero. Original 106-byte CPU routine executes unchanged. Full import byte streams, 64-KiB work RAM/code, 64-KiB NV memory, registers and final SR compared. Not natural replay, real multi-slot validation, interrupt/cycle timing, N64 platform binding or campaign integration. Stops before outer RTS.')
(out / 'validation.json').write_text(json.dumps(report, indent=2) + '\n')
print('PASS', len(cases), 'original boot selector calls against C; complete import streams, memory, registers and SR')
