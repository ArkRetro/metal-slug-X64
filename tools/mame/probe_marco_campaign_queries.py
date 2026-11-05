#!/usr/bin/env python3
"""Compare typed contact registration/proximity with original 004C24/0049E8.

Injected actor/scene RAM and masked interrupts isolate the services. Original
ROM descriptors, clipping, registry masks and narrow phase execute unmodified.
"""
import argparse,ctypes as ct,hashlib,json,os,re,subprocess,zipfile
from pathlib import Path
ROOT=Path(__file__).resolve().parents[2]
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--rom-dir',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
a=p.parse_args();out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
for name in ('cfg','nvram'):(out/name).mkdir()
cases=[]
for x,y in [(129,320),(139,320),(140,320),(60,320),(87,320),(-30,320),(352,320),(129,227),(129,228),(129,499),(129,503)]:
 for flags in (0,1,2,3):cases.append(dict(x=x&65535,y=y,flags=flags,tag=0x3bb40,kill=False,timer=3))
cases += [dict(x=129,y=320,flags=0,tag=tag,kill=kill,timer=timer)
          for tag in (0,0x3bb40,0x2d0aa8,0x2d0ab4,0x2d0ac0,0x2d0acc,0x2d0ad8)
          for kill in (False,True) for timer in (0,16)]
lua='local cases='+json.dumps(cases).replace(':','=').replace('"','').replace('[','{').replace(']','}')+'\n'
lua+='local output='+json.dumps(str(out))+'\n'
lua+=r'''local cpu=manager.machine.devices[':maincpu'].spaces.program
local debug=manager.machine.debugger
local frame,index=0,0
emu.register_frame_done(function()
 frame=frame+1;if frame<2 then return end
 if index==#cases then manager.machine:exit();return end
 local c=cases[index+1]
 for addr=0x100000,0x10fffe,2 do cpu:write_u16(addr,0) end
 cpu:write_u8(0x300001,0);cpu:write_u8(0x2ffff0,1)
 cpu:write_u16(0x10ff00,0x60fe)
 cpu:write_u32(0x101050,c.tag);cpu:write_u16(0x10103c,c.x);cpu:write_u16(0x101040,c.y)
 cpu:write_u8(0x101015,c.flags);cpu:write_u8(0x10105a,c.timer);cpu:write_u16(0x10104e,32)
 cpu:write_u8(0x1063e2,0xff);cpu:write_u8(0x1067e2,c.kill and 1 or 0)
 cpu:write_u16(0x10123c,100);cpu:write_u16(0x101240,320)
 if index==0 then
  debug:command('bpset 4dc8,1,{printf "REGISTER case=%d count=%d slot=%d timer=%d flags=%d hp=%d carry=%d\n",temp0,w@106696,b@101059,b@10105a,b@10105f,w@10104e,sr&1;b@1063e2=0;a6=101200;a0=2d0ae4;pc=49e8;g}')
  debug:command('bpset 4ba2,1,{printf "PROXIMITY case=%d value=%d carry=%d\n",temp0,d0&ffff,sr&1;pc=10ff00;g}')
  debug:command('bpset 4baa,1,{printf "PROXIMITY case=%d value=%d carry=%d\n",temp0,d0&ffff,sr&1;pc=10ff00;g}')
 end
 debug:command('temp0='..string.format('%x',index))
 debug:command('sr=2700');debug:command('a5=108000');debug:command('a6=101000')
 debug:command('sp=10fe00');debug:command('pc=4c24');debug:command('g')
 index=index+1
end,'frame')
'''
(out/'capture.lua').write_text(lua)
env=dict(os.environ,LD_LIBRARY_PATH='/workspace/emulator/root/usr/lib/x86_64-linux-gnu',
         SDL_VIDEODRIVER='dummy',SDL_AUDIODRIVER='dummy')
cmd=['/workspace/emulator/root/usr/games/mame','mslugx','-rompath',str(a.rom_dir.resolve()),'-bios','euro',
     '-video','none','-sound','none','-skip_gameinfo','-nothrottle','-debug','-debugger','none','-debuglog',
     '-debugscript',str(ROOT/'tools/mame/start.cmd'),'-autoboot_delay','0','-autoboot_script',str(out/'capture.lua'),
     '-cfg_directory',str(out/'cfg'),'-nvram_directory',str(out/'nvram')]
with (out/'mame.log').open('w') as log:
 subprocess.run(cmd,cwd=out,env=env,stdout=log,stderr=subprocess.STDOUT,check=True,timeout=60)
log=(out/'debug.log').read_text();assert 'LUA ERROR' not in (out/'mame.log').read_text()
registered=re.findall(r'REGISTER case=(\d+) count=(\d+) slot=(\d+) timer=(\d+) flags=(\d+) hp=(\d+) carry=(\d+)',log)
proximity=re.findall(r'PROXIMITY case=(\d+) value=(\d+) carry=(\d+)',log)
assert len(registered)==len(proximity)==len(cases),(len(registered),len(proximity),len(cases),log[-2000:])
class Actor(ct.Structure):
 _fields_=[('identity',ct.c_uint32),('field50_tag',ct.c_uint32),('projected_x',ct.c_uint16),
  ('projected_y',ct.c_uint16),('hitpoints',ct.c_uint16),('flags15',ct.c_uint8),('flags5f',ct.c_uint8),
  ('timer5a',ct.c_uint8),('impact_x',ct.c_int8),('impact_y',ct.c_int8),('registry_index',ct.c_uint8)]
class Entry(ct.Structure):
 _fields_=[('actor',Actor),('x_cells',ct.c_uint16),('y_cells',ct.c_uint8),('groups',ct.c_uint8)]
class Registry(ct.Structure):_fields_=[('entries',Entry*64),('count',ct.c_uint8)]
library=out/'queries.so'
subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-shared','-fPIC','-Isrc',
 'src/marco_campaign_queries.c','src/neo_hit.c','-o',str(library)],cwd=ROOT,check=True)
lib=ct.CDLL(str(library));lib.marco_campaign_hit_register.restype=ct.c_bool
lib.marco_campaign_hit_proximity.restype=ct.c_bool
results=[]
for i,(case,original_reg,original_prox) in enumerate(zip(cases,registered,proximity)):
 assert i==int(original_reg[0])==int(original_prox[0])
 registry=Registry();actor=Actor(identity=2,field50_tag=case['tag'],projected_x=case['x'],
  projected_y=case['y'],hitpoints=32,flags15=case['flags'],timer5a=case['timer'])
 value=ct.c_uint16();carry=ct.c_bool()
 assert lib.marco_campaign_hit_register(ct.byref(registry),ct.byref(actor),case['kill'],ct.byref(value),ct.byref(carry))
 actual=[registry.count,actor.registry_index,actor.timer5a,actor.flags5f,actor.hitpoints,int(carry.value)]
 assert actual==list(map(int,original_reg[1:])),(i,case,actual,original_reg)
 player=Actor(identity=1,projected_x=100,projected_y=320)
 assert lib.marco_campaign_hit_proximity(ct.byref(registry),ct.byref(player),ct.byref(value),ct.byref(carry))
 assert [value.value,int(carry.value)]==list(map(int,original_prox[1:])),(i,case,value.value,original_prox)
 results.append(dict(input=case,registration=actual,proximity=value.value))
report=dict(status='PASS_ORIGINAL_MARCO_REGISTRATION_AND_COMMON_PROXIMITY',cases=len(cases),results=results,
 source_sha256=hashlib.sha256((ROOT/'src/marco_campaign_queries.c').read_bytes()).hexdigest(),
 limits='Injected scene RAM; actual unmodified original ROM services execute. No campaign actor production, damaging attack query, debug rectangle drawing, timing or stack parity claimed.')
(out/'validation.json').write_text(json.dumps(report,indent=2)+'\n')
print('PASS',len(cases),'original 004C24/0049E8 calls')
