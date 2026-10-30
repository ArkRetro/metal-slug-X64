#!/usr/bin/env python3
"""Execute original 008512 to its RTS and compare addressed C recovery.

Controlled injected RAM and registers, not a gameplay trace. Checks full work
RAM, both palette banks, all registers and CCR, plus actual I/O write sequence.
"""
import argparse, ctypes as ct, hashlib, json, os, re, subprocess, zipfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('--rom-dir', type=Path, required=True)
p.add_argument('--output', type=Path, required=True)
a = p.parse_args(); out = a.output.resolve(); out.mkdir(parents=True, exist_ok=False)
for name in ('cfg', 'nvram'): (out / name).mkdir()
with zipfile.ZipFile(a.rom_dir / 'mslugx.zip') as archive: raw = archive.read('250-p1.p1')
assert hashlib.sha1(raw).hexdigest() == json.loads((ROOT/'tools/rom_manifest.json').read_text())['250-p1.p1']['sha1']
rom = bytearray(raw); rom[::2] = raw[1::2]; rom[1::2] = raw[::2]
# Prove the two collapsed loops cover every original opcode, including offsets.
assert rom[0x8540:0x854c] == bytes.fromhex('0f9067084cd00c3f48d10c3f')
for slot in range(1, 256):
    d = (slot*32).to_bytes(2, 'big')
    expect = bytes.fromhex('0fa8')+d+bytes.fromhex('670c4ce80c3f')+d+bytes.fromhex('48e90c3f')+d
    start = 0x854c+(slot-1)*18
    assert rom[start:start+18] == expect, hex(start)
assert rom[0x9772:0x9b72] == bytes.fromhex('48e0ff00')*256
cases = []
# Both final banks; global dirty flag on/off, zero/sparse/all dirty, full fill.
for bank in (0, 1):
    for mode in ('flag_off', 'none', 'first', 'last', 'sparse', 'all', 'fill', 'flag_off_zero', 'flag_off_negative'):
        seed = len(cases)+1
        ram = bytearray((i*37+seed*19+(i>>5)*7)&255 for i in range(65536))
        for slot in range(256):
            addr = 0x78fa+slot*32
            dirty = mode == 'all' or (mode == 'first' and slot == 0) or (mode == 'last' and slot == 255) or (mode == 'sparse' and slot%7 == 3)
            ram[addr] = (ram[addr]&127) | (128 if dirty else 0)
        ram[0x9942:0x9944] = bytes.fromhex('2000' if mode == 'fill' else '0000')
        ram[0x9944] = bank
        ram[0x9945] = (seed&127) | (0 if mode.startswith('flag_off') else 128)
        ram[0xff00:0xff02] = bytes.fromhex('60fe')
        banks = [bytearray().join(((i*257+seed*31+b*13)&65535).to_bytes(2,'big') for i in range(4096)) for b in (0,1)]
        regs = [((i+1)*0x11335577+seed*0x10203)&0xffffffff for i in range(16)]
        regs[13] = 0x108000; regs[15] = 0x10ffe0
        if mode == 'flag_off_zero': regs[0] = 0
        if mode == 'flag_off_negative': regs[0] = 0xffffff80
        sr = 0x2700 | ((seed*7)&31)
        case = dict(index=seed-1, mode=mode, final_bank=bank, sr=sr, registers=regs)
        cases.append((case, ram, banks))
        (out/f'input-{seed-1}.ram').write_bytes(ram)
        for b in (0,1): (out/f'input-{seed-1}.pal{b}').write_bytes(banks[b])
(out/'cases.json').write_text(json.dumps([c[0] for c in cases], indent=2)+'\n')
names = [f'd{i}' for i in range(8)]+[f'a{i}' for i in range(7)]+['sp']
formats = ''.join(f' {name}=%08X' for name in names)
commands = ['bpset 9758,1,{printf "PAL_END case=%d sr=%04X'+formats+'",temp0,sr,'+','.join(names)+';temp7++;pc=10ff00;g}', 'temp7=0', f'trace {out}/instructions.log,maincpu,noloop']

lua = r'''local frame,index=0,0
local cpu=manager.machine.devices[':maincpu'].spaces.program
local bank_item=emu.item(assert(manager.machine.devices[':'].items['0/m_palette_bank']))
local active=false
local events={}
local io_tap=cpu:install_write_tap(0x300000,0x3a001f,'palette_kernel_io',function(address,data,mask)
 if active and (address==0x300000 or address==0x3a000e or address==0x3a001e) then
  events[#events+1]=string.format('%06x %04x %04x',address,data,mask)
 end
end)
local function load(path,base)
 local f=assert(io.open(path,'rb'));local data=f:read('*a');f:close()
 for i=1,#data,2 do cpu:write_u16(base+i-1,string.unpack('>I2',data,i)) end
end
local function save(path,base,words)
 local f=assert(io.open(path,'wb'))
 for i=0,words-1 do f:write(string.pack('>I2',cpu:read_u16(base+i*2))) end
 f:close()
end
'''
lua += 'local output='+json.dumps(str(out))+'\n'
lua += 'local cases='+ '{'+','.join('{sr='+str(c['sr'])+',regs={'+','.join(map(str,c['registers']))+'}}' for c,_,_ in cases)+'}\n'
lua += 'emu.register_frame_done(function()\nframe=frame+1\nif frame<2 then return end\nactive=false\n'
lua += r'''if index>0 then
 local n=index-1
 local f=assert(io.open(output..'/result-'..n..'.io','w'));f:write(table.concat(events,'\n')..'\n');f:close()
 f=assert(io.open(output..'/result-'..n..'.bank','w'));f:write(tostring(bank_item:read(0)));f:close()
 save(output..'/result-'..n..'.ram',0x100000,32768)
 for b=0,1 do cpu:write_u8(b==0 and 0x3a000f or 0x3a001f,0);save(output..'/result-'..n..'.pal'..b,0x400000,4096) end
end
if index==#cases then io_tap:remove();manager.machine:exit();return end
local c=cases[index+1]
load(output..'/input-'..index..'.ram',0x100000)
for b=0,1 do cpu:write_u8(b==0 and 0x3a000f or 0x3a001f,0);load(output..'/input-'..index..'.pal'..b,0x400000) end
-- Deliberately leave bank 1 selected before each call.
events={}
'''
lua += 'if index==0 then\n'+ '\n'.join('manager.machine.debugger:command('+json.dumps(s)+')' for s in commands)+'\nend\n'
lua += r'''manager.machine.debugger:command('temp0='..string.format('%x',index))
for i=1,16 do local name=(i<=8 and 'd'..(i-1) or (i==16 and 'sp' or 'a'..(i-9)));manager.machine.debugger:command(name..'='..string.format('%x',c.regs[i])) end
manager.machine.debugger:command('sr='..string.format('%x',c.sr))
active=true
manager.machine.debugger:command('pc=8512')
manager.machine.debugger:command('g')
index=index+1
end,'frame')
'''
(out/'capture.lua').write_text(lua)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu')
with (out/'xvfb.log').open('w') as xlog:
    server=subprocess.Popen(['/workspace/emulator/root/usr/bin/Xvfb','-displayfd','1','-screen','0','1024x768x24','-ac','-nolisten','tcp'],stdout=subprocess.PIPE,stderr=xlog,text=True,env=env)
    try:
        display=server.stdout.readline().strip(); assert display.isdigit(); env['DISPLAY']=':'+display
        cmd=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro','-video','soft','-sound','none','-window','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog','-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(out/'capture.lua'),'-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
        with (out/'mame.log').open('w') as log: subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=90)
    finally: server.terminate();server.wait(timeout=8)
log=(out/'debug.log').read_text(); mlog=(out/'mame.log').read_text()
assert 'error in ' not in log.lower() and 'LUA ERROR' not in mlog,(log[-2000:],mlog[-2000:])
ends=re.findall(r'PAL_END case=(\d+) sr=([0-9A-F]+)((?: (?:[da][0-7]|sp)=[0-9A-F]+){16})',log)
assert len(ends)==len(cases),(len(ends),len(cases),log[-2000:])
# Compare the recovered function, not another hand-written algorithm.
libpath=out/'kernel.so'
subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-shared','-fPIC',str(ROOT/'decomp/recovered/rom_008512.c'),'-o',str(libpath)],check=True)
class Registers(ct.Structure): _fields_=[('d',ct.c_uint32*8),('a',ct.c_uint32*8),('sr',ct.c_uint16)]
R8=ct.CFUNCTYPE(ct.c_uint8,ct.c_void_p,ct.c_uint32);R16=ct.CFUNCTYPE(ct.c_uint16,ct.c_void_p,ct.c_uint32)
W8=ct.CFUNCTYPE(None,ct.c_void_p,ct.c_uint32,ct.c_uint8);W16=ct.CFUNCTYPE(None,ct.c_void_p,ct.c_uint32,ct.c_uint16)
class Bus(ct.Structure): _fields_=[('context',ct.c_void_p),('read8',R8),('read16',R16),('write8',W8),('write16',W16)]
lib=ct.CDLL(str(libpath)); lib.msx_rom_008512.argtypes=[ct.POINTER(Registers),ct.POINTER(Bus)]
results=[]
for (case,initial_ram,initial_banks),(number,sr,regtext) in zip(cases,ends):
    assert int(number)==case['index']
    ram=bytearray(initial_ram);banks=[bytearray(b) for b in initial_banks];bank=1;events=[];errors=[]
    def read8(_,addr):
        if 0x100000<=addr<0x110000:return ram[addr-0x100000]
        if 0x400000<=addr<0x402000:return banks[bank][addr-0x400000]
        errors.append(('read',hex(addr)));return 0
    def read16(_,addr):return (read8(_,addr)<<8)|read8(_,addr+1)
    def write8(_,addr,value):
        global bank
        if 0x100000<=addr<0x110000:ram[addr-0x100000]=value
        elif addr in (0x300001,0x3a000f,0x3a001f):
            events.append(f'{addr&~1:06x} {value:04x} 00ff')
            if addr!=0x300001:bank=int(addr==0x3a001f)
        else:errors.append(('write8',hex(addr)))
    def write16(_,addr,value):
        if 0x100000<=addr<0x110000:ram[addr-0x100000:addr-0x100000+2]=value.to_bytes(2,'big')
        elif 0x400000<=addr<0x402000:banks[bank][addr-0x400000:addr-0x400000+2]=value.to_bytes(2,'big')
        else:errors.append(('write16',hex(addr)))
    refs=[R8(read8),R16(read16),W8(write8),W16(write16)]
    bus=Bus(None,*refs);regs=Registers((ct.c_uint32*8)(*case['registers'][:8]),(ct.c_uint32*8)(*case['registers'][8:]),case['sr'])
    lib.msx_rom_008512(ct.byref(regs),ct.byref(bus));assert not errors,errors
    n=case['index']; actual_regs=[int(value,16) for value in re.findall(r'=([0-9A-F]+)',regtext)]
    assert list(regs.d)+list(regs.a)==actual_regs,(n,'registers',list(regs.d)+list(regs.a),actual_regs)
    assert regs.sr==int(sr,16),(n,'sr',hex(regs.sr),sr)
    assert ram==(out/f'result-{n}.ram').read_bytes(),(n,'RAM')
    for b in (0,1):assert banks[b]==(out/f'result-{n}.pal{b}').read_bytes(),(n,'palette',b)
    bank_offset=int((out/f'result-{n}.bank').read_text())
    assert bank_offset in (0,4096),(n,'MAME bank word offset',bank_offset)
    assert bank==bank_offset//4096,(n,'bank')
    actual_io=[]
    for line in (out/f'result-{n}.io').read_text().splitlines():
        addr,value,mask=(int(part,16) for part in line.split())
        actual_io.append(f'{addr:06x} {value&mask:04x} {mask:04x}')
    assert events==actual_io,(n,'I/O',events,actual_io)
    results.append(dict(**case,matched=True,ram_sha256=hashlib.sha256(ram).hexdigest(),palette_sha256=[hashlib.sha256(b).hexdigest() for b in banks],io=events,output_registers=actual_regs,output_sr=int(sr,16)))
report=dict(status='PASS_CONTROLLED_ORIGINAL_M68000_COMPARISON',entry='0x008512',stop_before_rts='0x009758',cases=len(cases),collapsed_dirty_blocks=256,collapsed_fill_blocks=256,results=results,source_sha256=hashlib.sha256((ROOT/'decomp/recovered/rom_008512.c').read_bytes()).hexdigest(),rom_cpu_order_sha256=hashlib.sha256(rom).hexdigest(),trace_sha256=hashlib.sha256((out/'instructions.log').read_bytes()).hexdigest(),limits='Injected RAM/registers, masked interrupts, valid counter 0 or 0x2000. Not gameplay, all counter values, cycle timing, palette word bus ordering or native N64 backend integration. Stops before RTS.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n'); print('PASS',len(cases),'original M68000 calls: full RAM, both palette banks, registers, SR, bank and I/O writes')
