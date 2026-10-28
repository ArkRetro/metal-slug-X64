-- Campaign soldier evidence: per-frame RAM (capture_soldier_campaign.lua),
-- every original RNG call (ROM 0029C6) with its caller actor and return PC,
-- and palette RAM every MSX_PALETTE_STEP frames. Requires -debug -debugger none.
-- Only breakpoint actions that print; no gameplay RAM or register writes.
local here=assert(debug.getinfo(1,'S').source:match('^@(.*/)'))
dofile(here..'capture_soldier_campaign.lua')
local output=os.getenv('MSX_CAPTURE_DIR')
local first=tonumber(os.getenv('MSX_RAM_FIRST') or '2100')
local last=tonumber(os.getenv('MSX_CAPTURE_END_FRAME') or '3600')
local step=tonumber(os.getenv('MSX_PALETTE_STEP') or '20')
local cpu=manager.machine.devices[':maincpu'].spaces['program']
local dbg=assert(manager.machine.debugger,'Launch MAME with -debug -debugger none')
local frame=0
emu.register_frame_done(function()
 frame=frame+1
 dbg:command(string.format('temp9=%x',frame))
 if frame==first-1 then
  dbg:command('bpset 29c6,1,{printf "RNG f=%d a6=%06X ret=%06X cur=%04X",temp9,a6,d@(sp),w@(100084);g}')
 end
 if frame>=first and frame<=last and (frame-first)%step==0 then
  local p=assert(io.open(output..'/palette'..frame..'.bin','wb'))
  for i=0,4095 do p:write(string.pack('>I2',cpu:read_u16(0x400000+i*2))) end
  p:close()
 end
end,'frame')
