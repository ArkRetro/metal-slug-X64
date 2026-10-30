-- Discovery capture for the Mission 1 soldiers of the playable N64 region.
-- MAME 0.264, mslugx euro BIOS. No gameplay RAM is written: inputs only.
-- Env: MSX_CAPTURE_DIR (required), MSX_CAPTURE_END_FRAME (default 3600),
-- MSX_RIGHT_FRAME (default 2280), MSX_STOP_RIGHT_FRAME (0 = never),
-- MSX_FIRE_FRAME (0 = never press fire), MSX_RAM_FIRST (default 2100).
-- MSX_CONFIRM_FRAME>0 selects the mission-entry timeline of
-- capture_intro_audio.lua (coin, start, single A confirmation only); zero keeps
-- the first-soldier timeline (A at 1450 and held 1550..1710).
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local function env(name,default) return tonumber(os.getenv(name) or default) end
local end_frame=env('MSX_CAPTURE_END_FRAME',3600)
local right_frame=env('MSX_RIGHT_FRAME',2280)
local stop_right=env('MSX_STOP_RIGHT_FRAME',0)
local fire_frame=env('MSX_FIRE_FRAME',0)
local ram_first=env('MSX_RAM_FIRST',2100)
local confirm=env('MSX_CONFIRM_FRAME',0)
-- Long runs (camera 0..1920): from MSX_UP_FRAME hold Up for the first half of
-- every MSX_UP_PERIOD frames (aim up while firing); from MSX_COIN_FRAME insert
-- a coin and press 1P Start every MSX_COIN_PERIOD frames (continue after a
-- game over; harmless while playing). Zero keeps the historical timelines.
local up_frame=env('MSX_UP_FRAME',0)
local up_period=env('MSX_UP_PERIOD',96)
local coin_frame=env('MSX_COIN_FRAME',0)
-- From MSX_JUMP_FRAME press jump (B) for 8 frames every MSX_JUMP_PERIOD frames.
local jump_frame=env('MSX_JUMP_FRAME',0)
local jump_period=env('MSX_JUMP_PERIOD',90)
local coin_period=env('MSX_COIN_PERIOD',600)
local frame=0
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local actions={[700]={'Coin 1',1},[710]={'Coin 1',0},[760]={'1 Player Start',1},[770]={'1 Player Start',0}}
if confirm>0 then actions[confirm]={'P1 A',1};actions[confirm+10]={'P1 A',0}
else actions[1450]={'P1 A',1};actions[1460]={'P1 A',0} end
local fields={}
for _,port in pairs(manager.machine.ioport.ports) do for name,field in pairs(port.fields) do fields[name]=field end end
local function button(name,value)
 for tag,p in pairs(manager.machine.ioport.ports) do
  for key,f in pairs(p.fields) do
   if key==name or (string.find(tag,'JOY1') and key=='%p '..string.sub(name,4)) then f:set_value(value) end
  end
 end
end
local inputs=assert(io.open(output..'/inputs.log','w'))
emu.register_frame_done(function()
 frame=frame+1
 if actions[frame] then local a=actions[frame]; button(a[1],a[2]); inputs:write(frame..' '..a[1]..' '..a[2]..'\n') end
 if confirm==0 and frame==1550 then button('P1 A',1) end
 if confirm==0 and frame==1710 then button('P1 A',0) end
 if frame==right_frame then button('P1 Right',1); inputs:write(frame..' right 1\n') end
 if stop_right>0 and frame==stop_right then button('P1 Right',0); inputs:write(frame..' right 0\n') end
 if fire_frame>0 and frame>=fire_frame then
  local v=frame%12<6 and 1 or 0; button('P1 A',v)
  if frame==fire_frame then inputs:write(frame..' fire toggling 12\n') end
 end
 if up_frame>0 and frame>=up_frame then
  local v=(frame-up_frame)%up_period<up_period//2 and 1 or 0; button('P1 Up',v)
  if frame==up_frame then inputs:write(frame..' up toggling '..up_period..'\n') end
 end
 if jump_frame>0 and frame>=jump_frame then
  local k=(frame-jump_frame)%jump_period
  if k==0 then button('P1 B',1) elseif k==8 then button('P1 B',0) end
  if frame==jump_frame then inputs:write(frame..' jump every '..jump_period..'\n') end
 end
 if coin_frame>0 and frame>=coin_frame then
  local k=(frame-coin_frame)%coin_period
  if k==0 then button('Coin 1',1) elseif k==10 then button('Coin 1',0)
  elseif k==30 then button('1 Player Start',1) elseif k==40 then button('1 Player Start',0) end
  if frame==coin_frame then inputs:write(frame..' coin+start every '..coin_period..'\n') end
 end
 if frame>=ram_first and frame<=end_frame then
  local ram=assert(io.open(output..'/ram'..frame..'.bin','wb'))
  ram:write(cpu:read_range(0x100000,0x10ffff,8));ram:close()
 end
 if frame==ram_first then
  for _,region in ipairs({{'program-fixed.bin',0x000000,0x0fffff},{'program-bank.bin',0x200000,0x2fffff}}) do
   local file=assert(io.open(output..'/'..region[1],'wb'));file:write(cpu:read_range(region[2],region[3],8));file:close()
  end
 end
 if frame==end_frame then inputs:close();manager.machine:exit() end
end,'frame')
