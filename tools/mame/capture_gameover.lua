-- Game-over capture for tools/extract_continue_fixture.py (docs/MISION1_COMPLETA.md).
-- MAME 0.264, mslugx, euro BIOS (MVS). Coin+start, then the player walks right
-- until it dies (Right released while dead). Lives 101288 forced to 0 once at frame 2000 (capture
-- only) so the next death is the game over 0C9848. MSX_GO_COIN=<frame>: insert
-- a coin at that frame and press start 60 frames later (continue variant).
-- MSX_GO_TAPB=1: B pressed 4 frames every 40 in frames 3600..4399.
-- MSX_GO_DUMP=<period>: from frame 2000 also full snapshots every <period>
-- frames (vram<f>.bin: VRAM words 0000..85FF with the sprite tables,
-- palette<f>.bin, ram<f>.bin, game<f>.png) and actors.csv: the routine of
-- every live node of the actor pool (1010A4 + 0xB0 n) whenever the set
-- changes (sprite actors of the game over and the continue).
-- MSX_GO_TRACE=1: trace.txt from frame 3400: 'M frame bytes' the mission
-- actor 101364 (+00..+AF) after every frame, 'S frame w0..w5' every
-- sprite-list entry it writes (PC 00A078, A6 = 101364: its 00427A at
-- 01883C, the CONTINUE? program 018120) and 'I frame node bytes' each
-- item node 08C374.. the first frame it is seen (the item of 019798).
-- Run: mame mslugx -bios euro -video none -sound none -nothrottle
--   -autoboot_script tools/mame/capture_gameover.lua (MSX_CAPTURE_DIR set).
local out=assert(os.getenv('MSX_CAPTURE_DIR'))
local last=tonumber(os.getenv('MSX_GO_LAST') or '9000')
local coin2=tonumber(os.getenv('MSX_GO_COIN') or '0')
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local video=emu.item(assert(manager.machine.devices[':spritegen'].items['0/m_videoram']))
local fields={}
for _,port in pairs(manager.machine.ioport.ports) do for name,f in pairs(port.fields) do fields[name]=f end end
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0},[1450]={'P1 A',1},[1460]={'P1 A',0},[1550]={'P1 A',1},[1700]={'P1 A',0}}
if coin2>0 then actions[coin2]={'Coin 1',1};actions[coin2+10]={'Coin 1',0};actions[coin2+60]={'1 Player Start',1};actions[coin2+70]={'1 Player Start',0} end
local log=assert(io.open(out..'/state.csv','w'))
log:write('frame,panel_routine,f84,f85,f86,f88,f89,f8c,mod1,mod2,sysmode,hud_on,join,two,dip8a,mvs,credits_d00034,mission_routine,p1_routine,video,f60,f74,ctimer,score,f80,f8d,in435d\n')
local fixlog=assert(io.open(out..'/fix.bin','wb'))
local walk=os.getenv('MSX_GO_WALK')~='0'
local tapb=os.getenv('MSX_GO_TAPB')=='1'
local lastfix=nil;local frame=0
local dump_period=tonumber(os.getenv('MSX_GO_DUMP') or '0')
local actors=dump_period>0 and assert(io.open(out..'/actors.csv','w')) or nil
local last_actors=nil
local trace=os.getenv('MSX_GO_TRACE')=='1' and assert(io.open(out..'/trace.txt','w')) or nil
local items_seen={}
local function hexbytes(a,n) return (cpu:read_range(a,a+n-1,8):gsub('.',function(c) return string.format('%02x',string.byte(c)) end)) end
if trace then
 local st=manager.machine.devices[':maincpu'].state
 local entry={}
 tap_sprites=cpu:install_write_tap(0x100000,0x10ffff,'go_sprites',function(address,data,mask)
  if frame<3400 or st['CURPC'].value~=0xa078 or st['A6'].value~=0x101364 then return end
  entry[#entry+1]=string.format('%x',data&0xffff)
  if #entry==6 then trace:write('S '..(frame+1)..' '..table.concat(entry,' ')..'\n');entry={} end
 end)
end
local function snapshot(f)
 local v=assert(io.open(out..'/vram'..f..'.bin','wb'))
 local t={};for i=0,0x85ff do t[#t+1]=string.pack('>I2',video:read(i)) end
 v:write(table.concat(t));v:close()
 local r=assert(io.open(out..'/ram'..f..'.bin','wb'));r:write(cpu:read_range(0x100000,0x10ffff,8));r:close()
 local p=assert(io.open(out..'/palette'..f..'.bin','wb'))
 for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
 p:close()
 manager.machine.screens[':screen']:snapshot(out..'/game'..f..'.png')
end
emu.register_frame_done(function()
 frame=frame+1
 if actions[frame] then local a=actions[frame];if fields[a[1]] then fields[a[1]]:set_value(a[2]) end end
 if frame==2000 then cpu:write_u8(0x101288,0) end
 if tapb and frame>=3600 and frame<4400 and fields['P1 B'] then fields['P1 B']:set_value((frame%40)<4 and 1 or 0) end
 local dead=cpu:read_u32(0x101204)~=0x0c9644
 if fields['P1 Right'] then fields['P1 Right']:set_value((frame>=2000 and not dead and walk) and 1 or 0) end
 if frame==2002 then for _,r in ipairs({{'program-fixed.bin',0,0xfffff},{'program-bank.bin',0x200000,0x2fffff}}) do local f=assert(io.open(out..'/'..r[1],'wb'));f:write(cpu:read_range(r[2],r[3],8));f:close() end end
 if frame>=1900 then
  local P=0x101204
  log:write(string.format('%d,%06x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%06x,%06x,%04x,%02x,%04x,%04x,%s,%04x,%02x,%02x\n',frame,cpu:read_u32(P),cpu:read_u8(P+0x84),cpu:read_u8(P+0x85),cpu:read_u8(P+0x86),cpu:read_u8(P+0x88),cpu:read_u8(P+0x89),cpu:read_u8(P+0x8c),
   cpu:read_u8(0x10fdb6),cpu:read_u8(0x10fdb7),cpu:read_u8(0x10fd80),cpu:read_u8(0x10e9e1),cpu:read_u8(0x10f003),cpu:read_u8(0x10c2e7),cpu:read_u8(0x10fd8a),cpu:read_u8(0x10fd82),cpu:read_u8(0xd00034),cpu:read_u32(0x101364),cpu:read_u32(0x1014c4),cpu:read_u16(0x10000e),cpu:read_u8(P+0x60),cpu:read_u16(P+0x74),cpu:read_u16(0x1013e2),string.format('%08x',cpu:read_u32(0x10c46a)),cpu:read_u16(P+0x80),cpu:read_u8(P+0x8d),cpu:read_u8(0x10435d)))
  local t={};for i=0x7000,0x74ff do t[#t+1]=string.pack('>I2',video:read(i)) end
  local s=table.concat(t)
  if s~=lastfix then fixlog:write(string.pack('>I4',frame));fixlog:write(s);lastfix=s
   if frame%2==0 then manager.machine.screens[':screen']:snapshot(out..'/s'..frame..'.png') end
  end
 end
 if trace and frame>=3400 then
  trace:write(string.format('M %d %s\n',frame,hexbytes(0x101364,0xb0)))
  for n=0,99 do
   local a=0x1010a4+n*0xb0;local r=cpu:read_u32(a)
   if r>=0x8c374 and r<0x8c800 then
    if not items_seen[a] then items_seen[a]=true;trace:write(string.format('I %d %x %s\n',frame,a,hexbytes(a,0xb0))) end
   else items_seen[a]=nil end
  end
 end
 if actors and frame>=2000 then
  local t={}
  for n=0,99 do local a=0x1010a4+n*0xb0;local r=cpu:read_u32(a);if r~=0x3f1e and r~=0 then t[#t+1]=string.format('%x:%x',a,r) end end
  local s=table.concat(t,' ')
  if s~=last_actors then actors:write(frame..','..s..'\n');last_actors=s end
  if frame%dump_period==0 then snapshot(frame) end
 end
 if frame>=last then log:close();fixlog:close();if actors then actors:close() end;if trace then trace:close() end;manager.machine:exit() end
end)
