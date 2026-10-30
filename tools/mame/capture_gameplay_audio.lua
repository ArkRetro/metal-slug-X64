-- Gameplay sound-command capture for Mission 1 (MAME 0.264, mslugx, euro BIOS).
-- Inputs only: no RAM or register is written. Records, with -wavwrite PCM:
--   debug.log      LATCH lines: every 68000 write to the sound latch 0x320000
--                  (value, PC, frame) -- what the Z80 driver receives;
--                  REQ lines: every byte queued in the 68000 sound ring
--                  0x10BF68..0x10BFE7 by ROM 01209C, with D0 and the stack
--                  words that hold the request word and the caller return
--                  address (decoded by tools/extract_sound_map.py);
--   frames.jsonl   emulated time of every frame boundary;
--   inputs.jsonl   every input change.
-- Env: MSX_CAPTURE_DIR (required), MSX_CAPTURE_END_FRAME (default 9000),
-- MSX_CONFIRM_FRAME (default 3620), MSX_PLAN (default 'fight').
-- MSX_PLAN='knife' and 'survive' are described at their functions.
-- MSX_PLAN='vehicle' reproduces the armoured-vehicle encounter of
-- tools/capture_vehicle.lua (same inputs and its capture-only survival writes:
-- timer, invulnerability, ammunition) and logs the vehicle actors
-- (routines 0x29168..0x29B62) to vehicle.csv; sound commands are unaffected.
local output=assert(os.getenv('MSX_CAPTURE_DIR'),'Set MSX_CAPTURE_DIR')
local finish=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '9000')
local confirm=tonumber(os.getenv('MSX_CONFIRM_FRAME') or '3620')
local plan=os.getenv('MSX_PLAN') or 'fight'
local cpu=manager.machine.devices[':maincpu']
local space=cpu.spaces['program']
local frame=0
local screen=manager.machine.screens[':screen']
local timing=assert(io.open(output..'/frames.jsonl','w'))
local inputs=assert(io.open(output..'/inputs.jsonl','w'))
local function reg(name) return cpu.state[name].value end
local function button(name,value)
 local found=false
 for tag,port in pairs(manager.machine.ioport.ports) do
  for key,field in pairs(port.fields) do
   if key==name or (tag:find('JOY1') and key=='%p '..name:sub(4)) then field:set_value(value);found=true end
  end
 end
 assert(found,'Missing input '..name)
 inputs:write(string.format('{"frame":%d,"name":"%s","value":%d}\n',frame,name,value))
end
local actions={[700]={{'Coin 1',1}},[710]={{'Coin 1',0}},[760]={{'1 Player Start',1}},[770]={{'1 Player Start',0}},
 [confirm]={{'P1 A',1}},[confirm+10]={{'P1 A',0}}}
local function at(f,name,value) actions[f]=actions[f] or {};table.insert(actions[f],{name,value}) end
-- 'fight': walk right firing, grenades and jumps, pauses that let rebels knife
-- or throw; a coin keeps the game running after a lost life.
local held={}
local function hold(name,value) if held[name]~=value then held[name]=value;button(name,value) end end
local function fight(f)
 if f<4100 then return end
 local pause=(f>=5000 and f<5400) or (f>=7600 and f<7900)
 hold('P1 Right',(not pause) and 1 or 0)
 local fire=(not pause) and (f%12<6)
 hold('P1 A',fire and 1 or 0)
 hold('P1 C',(f%300>=150 and f%300<156) and 1 or 0)
 hold('P1 B',(f%420>=300 and f%420<310) and 1 or 0)
end
-- 'knife': walk in to the first rebels, then stand still without firing so
-- that they reach Marco with the knife (no RAM writes).
local function knife(f)
 if f<4100 then return end
 hold('P1 Right',f<4250 and 1 or 0)
end
-- 'survive': the 'fight' inputs with the capture-only survival writes of the
-- vehicle plan, to observe more rebel deaths in one run.
local function survive(f)
 if f>=4000 then space:write_u8(0x1013de,0x5a);space:write_u8(0x101523,space:read_u8(0x101523)|0x40) end
 fight(f)
end
local vehicle_log=nil
local function vehicle(f)
 if f==1450 then button('P1 A',1) elseif f==1460 then button('P1 A',0) end
 if f==1550 then button('P1 A',1) elseif f==1710 then button('P1 A',0) end
 if f>=2000 then
  space:write_u8(0x1013de,0x5a);space:write_u8(0x101523,space:read_u8(0x101523)|0x40)
  space:write_u8(0x101549,0x99);space:write_u8(0x101288,2)
 end
 if f==2100 then space:write_u8(0x10c71e,3) end
 if f>=2190 then
  space:write_u8(0x10c725,0xff);space:write_u8(0x10c720,0x63);space:write_u8(0x10c621,0x30);space:write_u8(0x10c627,0x63)
  if f>=3600 then space:write_u8(0x10c71e,0) end
  local back=f>=3600
  hold('P1 Right',back and 0 or 1);hold('P1 Left',back and f<3602 and 1 or 0)
  local paused=f>=2880 and f<3600
  hold('P1 A',(not paused and f%12<6) and 1 or 0)
  hold('P1 B',(f>3000 and f%120<8) and 1 or 0)
  hold('P1 C',(not paused and f>2900 and f%60<8) and 1 or 0)
 end
 if f>=2800 then
  vehicle_log=vehicle_log or assert(io.open(output..'/vehicle.csv','w'))
  for address=0x102000,0x107ffc,4 do
   local routine=space:read_u32(address)
   if routine>=0x29168 and routine<0x29b62 and space:read_u32(address+0x38)==0x10c72e then
    vehicle_log:write(string.format('%d,%x,%x,%d\n',f,address,routine,space:read_u16(address+0x4e)))
   end
  end
 end
end
if plan=='vehicle' then actions={[700]={{'Coin 1',1}},[710]={{'Coin 1',0}},[760]={{'1 Player Start',1}},[770]={{'1 Player Start',0}}} end
if plan=='fight' then at(4700,'Coin 1',1);at(4710,'Coin 1',0);at(9000,'Coin 1',1);at(9010,'Coin 1',0) end
local dbg=assert(manager.machine.debugger,'Launch MAME with -debug -debugger none')
local function now() return tostring(manager.machine.time) end
emu.register_frame_done(function()
 -- Count emulated video frames: frame_done callbacks can be skipped when
 -- the host runs slowly under -nothrottle.
 local current=screen:frame_number()
 if current<=frame then return end
 local previous=frame
 frame=current;dbg:command(string.format('temp9=%x',frame))
 if previous==0 then
  -- Lua write taps on these ranges are dropped by the Neo Geo map in 0.264;
  -- debugger watchpoints (as in capture_intro_audio.lua) are reliable.
  dbg:command('wpset 320000:maincpu,1,w,1,{printf "LATCH frame=%d pc=%X value=%02X\\n",temp9,curpc,wpdata;g}')
  dbg:command('wpset 10bf68:maincpu,80,w,curpc==120d2,{printf "REQ frame=%d byte=%02X d0=%04X s0=%08X s4=%04X s6=%08X\\n",temp9,wpdata,d0&ffff,maincpu.pd@sp,maincpu.pw@(sp+4),maincpu.pd@(sp+6);g}')
 end
 for f=previous+1,frame do
  if actions[f] then for _,a in ipairs(actions[f]) do button(a[1],a[2]) end end
  if plan=='fight' then fight(f) elseif plan=='knife' then knife(f) elseif plan=='survive' then survive(f) elseif plan=='vehicle' then vehicle(f) end
 end
 timing:write(string.format('{"frame":%d,"time":"%s","callbacks_skipped":%d}\n',frame,now(),frame-previous-1))
 if frame>=finish then
  for _,f in ipairs({timing,inputs,vehicle_log}) do f:close() end
  manager.machine:exit()
 end
end,'frame')
